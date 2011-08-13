#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#include "EMBeMediaRenderingNode.h"

#include "EMBeMediaUtility.h"
#include "EMMediaFormat.h"
#include "EMMediaTimer.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMVideoBitmapScaler.h"
#include "DirectoryIDs.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"

//TEMPORARY
#include <InterfaceKit.h>
#include <media/MediaDecoder.h>
//END TEMPORARY

#include <MediaKit.h>
#include <OS.h>
#include <StorageKit.h>

media_file_format GetMediaFileFormat(string p_oExtension, media_format* p_spFormat)
{
	int32 cookie = 0; //, cookie2;
	media_format format, outfmt;
	media_file_format mfi;
//	media_codec_info mci;
	format = *p_spFormat;
	
	while(get_next_file_format(&cookie, &mfi) == B_OK) 
	{
		if(strcmp(mfi.short_name, p_oExtension.c_str()) == 0)
			return mfi;
	}
		EMDebugger("No such family!");

}

media_codec_info GetCodecInfo(media_file_format p_sFileFormat, media_format* p_spFormat)
{
	int32 cookie = 0; //, cookie2;
	media_format format, outfmt;
//	media_file_format mfi;
	media_codec_info mci;
	
	while(get_next_encoder(&cookie, &p_sFileFormat, p_spFormat, &outfmt, &mci) == B_OK)
	{
	}
	
	return mci;
}

EMBeMediaRenderingNode* EMBeMediaRenderingNode::m_opInstance = NULL;

EMBeMediaRenderingNode* EMBeMediaRenderingNode::Instance()
{
	if(m_opInstance == NULL)
		m_opInstance = new EMBeMediaRenderingNode();
	return m_opInstance;
}

EMBeMediaRenderingNode::EMBeMediaRenderingNode() 
	:	BMediaNode("Titan consumer node"), 
		BBufferConsumer(B_MEDIA_MULTISTREAM),
		m_vShouldRender(false),
		m_opMediaFile(NULL),
		m_opAudioTrack(NULL),
		m_opVideoTrack(NULL),
		vVideoFrameCount(0),
		vVideoKeyFrameRate(10),
		m_opVideoScaler(NULL),
		m_opReceiver(NULL),
		m_opPreviewingImage(NULL),
		m_vReadyForWriting(false),
		m_vRenderVideo(false),
		m_vRenderAudio(false),
		m_vRenderFileSequenceNumber(0),
		m_oDesiredFormat(NULL)
{
	m_vLocalHeight = 0;
	m_vLocalWidth = 0;
	m_sControlPortID = 0;
	m_sInputs[EM_AUDIO_INPUT].source = media_source::null;
	m_sInputs[EM_VIDEO_INPUT].source = media_source::null;

	m_sControlPortID = create_port(10, "Titan consumer node port");
	if(m_sControlPortID == B_BAD_VALUE || m_sControlPortID == B_NO_MORE_PORTS)
		EMDebugger("ERROR! EMBeMediaRenderingNode::ServiceThread() - Could not create port... Bad port value, or no more ports!");

	m_vSemaphore = create_sem(1, "BMediaTrack protection sem");
	m_opVideoScaler = new EMVideoBitmapScaler();
	EMMediaTimer::Instance() -> AddListener(this);

	m_sAudioFormat.type = B_MEDIA_RAW_AUDIO;
	m_sAudioFormat.u.raw_audio = media_raw_audio_format::wildcard;
	m_sVideoFormat.type = B_MEDIA_RAW_VIDEO;
	m_sVideoFormat.u.raw_video = media_raw_video_format::wildcard;
	
	EMBeMediaUtility::GetRosterE() -> RegisterNode(this);
	EMBeMediaUtility::push(this, "EMBeMediaRenderingNode");
}

EMBeMediaRenderingNode::~EMBeMediaRenderingNode()
{
	EMMediaTimer::Instance() -> RemoveListener(this);
//	EMDebugger("Rendering node destructor");
	BMediaRoster::Roster() -> UnregisterNode(this);
	status_t vErrorCode = B_OK;
	write_port(m_sControlPortID, EM_PORT_MESSAGE_SHUTDOWN, 0, 0);
	while(wait_for_thread(m_vThread, &vErrorCode) == B_INTERRUPTED)
		snooze(1000);
	delete_port(m_sControlPortID);
	delete m_opPreviewingImage;
	EMBeMediaUtility::pop("EMBeMediaRenderingNode");
}

status_t EMBeMediaRenderingNode::AcceptFormat(const media_destination& p_destination, media_format* p_format)
{
	if(p_destination != m_sInputs[EM_AUDIO_INPUT].destination && p_destination != m_sInputs[EM_VIDEO_INPUT].destination)
	{
		EMDebugger("EMBeMediaRenderingNode::AcceptFormat() - ERROR! Destination doesn't belong in this node!");
		return B_MEDIA_BAD_DESTINATION;
	}

	media_format sInputFormat;	
	if(p_destination == m_sInputs[EM_AUDIO_INPUT].destination)
		sInputFormat = m_sPreferedInputFormat[EM_AUDIO_INPUT];
	else if(p_destination == m_sInputs[EM_VIDEO_INPUT].destination)
		sInputFormat = m_sPreferedInputFormat[EM_VIDEO_INPUT];

	if(p_format -> type <= 0 && p_format -> type != sInputFormat.type) 
	{
		p_format -> type = sInputFormat.type;
		p_format -> u = sInputFormat.u;
	}

	if(p_format -> type == B_MEDIA_RAW_AUDIO)
		if(p_format -> u.raw_audio.buffer_size != sInputFormat.u.raw_audio.buffer_size)
			p_format -> u.raw_audio.buffer_size = sInputFormat.u.raw_audio.buffer_size;

	return B_OK;
}

BMediaAddOn* EMBeMediaRenderingNode::AddOn(int32* p_outInternalID) const
{
	if (p_outInternalID) 
		*p_outInternalID = 0;
	return NULL;
}

void EMBeMediaRenderingNode::BufferReceived(BBuffer* p_opBuffer)
{
	int64 vFrames = 0;

	if(p_opBuffer -> Header() -> type == B_MEDIA_RAW_AUDIO)
		;//cout_commented_out_4_release << "AudioBuffer " << flush;
	if(p_opBuffer -> Header() -> type == B_MEDIA_RAW_VIDEO)
		;//cout_commented_out_4_release << "VideoBuffer " << flush;
		
	
	acquire_sem(m_vSemaphore);
	if(m_vShouldRender && m_vReadyForWriting)
	{
		if((p_opBuffer -> Header() -> type == B_MEDIA_RAW_AUDIO) && m_vRenderAudio)
		{
			EMMediaFormat oAudioFormat(EM_TYPE_RAW_AUDIO);
			oAudioFormat.CreateFormat(&(m_sInputs[EM_AUDIO_INPUT].format));
			vFrames = EMBeMediaUtility::BytesToFrames(p_opBuffer -> SizeUsed(), &oAudioFormat);
			status_t vResult = m_opAudioTrack -> WriteFrames((const void*) (p_opBuffer -> Data()), vFrames, B_MEDIA_KEY_FRAME);
			//status_t vResult = m_opVideoTrack -> WriteFrames((const void*) (p_opBuffer -> Data()), vFrames, B_MEDIA_KEY_FRAME);
			if(vResult != B_OK)
				emerr << "ERROR! Couldn't write data to audio track: \"" << strerror(vResult) << "\"" << endl;
		}
		else if((p_opBuffer -> Header() -> type == B_MEDIA_RAW_VIDEO) && m_vRenderVideo)
		{
			//Fetch the EMMediaFormat of the video data buffer, that we stored in a byte-sequence in the user_data field of the BBuffer, in the EMBeRealtimeVideoOutputDescriptor
			char* vpUserData = (char*) p_opBuffer -> Header() -> user_data;
			EMMediaFormat oCurrentBufferFormat(EM_TYPE_RAW_VIDEO);
			memcpy(&oCurrentBufferFormat, vpUserData, sizeof(EMMediaFormat));

//			if(m_opReceiver == NULL)
//				m_opReceiver = new EMBeVideoDisplayer();

			//Then make sure we scale the video data to this format!
			;//cout_commented_out_4_release<<"Size of Data() available:"<<p_opBuffer -> SizeAvailable()<<endl;
			;//cout_commented_out_4_release<<"Size of Data() used:"<<p_opBuffer -> SizeUsed()<<endl;
			void* opData = m_opVideoScaler -> Scale(p_opBuffer -> Data(), &oCurrentBufferFormat, m_oDesiredFormat);
				
/*			if(m_opReceiver -> LockPreviewWindow())
			{
				if(m_vLocalHeight != m_oDesiredFormat -> m_vHeight || m_vLocalWidth != m_oDesiredFormat -> m_vWidth || m_opPreviewingImage == NULL)
				{	//If we received a image format that differs from the format of the existing image, we have to allocate a new one!
					;//cout_commented_out_4_release<<"Has to change the format for it to function :)"<<endl;
					if(m_opPreviewingImage != NULL)
						delete m_opPreviewingImage;
					m_opPreviewingImage = new BBitmap(BRect(0.0, 0.0, m_oDesiredFormat -> m_vWidth-1, m_oDesiredFormat -> m_vHeight-1), B_RGB32, false, false);
					if(! m_opPreviewingImage -> IsValid())
						EMDebugger("ERROR! BBitmap not valid!");
					m_vLocalWidth = m_oDesiredFormat -> m_vWidth;
					m_vLocalHeight = m_oDesiredFormat -> m_vHeight;
				}
				//Now copy the buffer-data onto the previewing window
				memcpy(m_opPreviewingImage -> Bits(), opData, m_opPreviewingImage -> BitsLength());
				m_opReceiver -> HandleNativeBuffer(m_opPreviewingImage);
			}
			else
				EMDebugger("ERROR! Vide rendering node failed to lock the window for previewing video!"); 
*/
				
			vFrames = 1;
			if(opData != NULL)
			{
//				if(m_opVideoTrack -> WriteFrames(opData, vFrames, (((vVideoFrameCount++) % vVideoKeyFrameRate) == 0 ? B_MEDIA_KEY_FRAME : 0))) 
				if(m_opVideoTrack -> WriteFrames(opData, vFrames, B_MEDIA_KEY_FRAME)) 
					emerr << "ERROR! Couldn't write data to video track!" << endl;
			} 
			else
			{
//				if(m_opVideoTrack -> WriteFrames(p_opBuffer -> Data(), vFrames, (((vVideoFrameCount++) % vVideoKeyFrameRate) == 0 ? B_MEDIA_KEY_FRAME : 0))) 
				if(m_opVideoTrack -> WriteFrames(p_opBuffer -> Data(), vFrames, B_MEDIA_KEY_FRAME))
					emerr << "ERROR! Couldn't write data to video track!" << endl;			
			}

		}
		else
			;//cout_commented_out_4_release << "Encountered a buffer that shouldnt be written to file!" << endl;

	}
	release_sem(m_vSemaphore);
	p_opBuffer -> Recycle();
	snooze(5000);
}

status_t EMBeMediaRenderingNode::Connected(const media_source& p_source, 
										  const media_destination& p_destination, 
										  const media_format& p_format,
										  media_input* p_outInput)
{
	if(m_sInputs[EM_AUDIO_INPUT].source != media_source::null && m_sInputs[EM_VIDEO_INPUT].source != media_source::null)
		return B_MEDIA_BAD_SOURCE;
	if(p_destination != m_sInputs[EM_AUDIO_INPUT].destination && p_destination != m_sInputs[EM_VIDEO_INPUT].destination)
		return B_MEDIA_BAD_DESTINATION;

	if(p_destination == m_sInputs[EM_AUDIO_INPUT].destination)
	{
		memcpy(&(m_sInputs[EM_AUDIO_INPUT].source), &p_source, sizeof(media_source));
		memcpy(&(m_sInputs[EM_AUDIO_INPUT].format), &p_format, sizeof(media_format));
		memcpy(p_outInput, &m_sInputs[EM_AUDIO_INPUT], sizeof(media_input));
		memcpy(&m_sAudioFormat, &p_format, sizeof(media_format));
	}
	else if(p_destination == m_sInputs[EM_VIDEO_INPUT].destination)
	{
		memcpy(&(m_sInputs[EM_VIDEO_INPUT].source), &p_source, sizeof(media_source));
		memcpy(&(m_sInputs[EM_VIDEO_INPUT].format), &p_format, sizeof(media_format));
		memcpy(p_outInput, &m_sInputs[EM_VIDEO_INPUT], sizeof(media_input));
		memcpy(&m_sVideoFormat, &p_format, sizeof(media_format));
	}
	else 
		return B_MEDIA_BAD_DESTINATION; 
	
	return B_OK;
}

port_id EMBeMediaRenderingNode::ControlPort(void) const
{
	return m_sControlPortID;
}

void EMBeMediaRenderingNode::Disconnected(const media_source& p_sSource, const media_destination& p_sDestination) 
{
	if(p_sSource != m_sInputs[EM_AUDIO_INPUT].source && p_sSource != m_sInputs[EM_VIDEO_INPUT].source) 
		return;
	if(p_sDestination != m_sInputs[EM_AUDIO_INPUT].destination && p_sDestination != m_sInputs[EM_VIDEO_INPUT].destination) 
		return;

	if(p_sSource == m_sInputs[EM_AUDIO_INPUT].source)
	{
		m_sInputs[EM_AUDIO_INPUT].source = media_source::null;
	}
	else if(p_sSource == m_sInputs[EM_VIDEO_INPUT].source)
	{
		m_sInputs[EM_VIDEO_INPUT].source = media_source::null;
	}
}

void EMBeMediaRenderingNode::DisposeInputCookie(int32 p_cookie)
{
}

status_t EMBeMediaRenderingNode::FormatChanged(const media_source& p_source, 
											  const media_destination& p_destination, 
											  int32 p_something, 
											  const media_format& p_format)
{
	return B_OK;
}

status_t EMBeMediaRenderingNode::GetLatencyFor(const media_destination& p_forWhom, 
											  bigtime_t* p_outLatency, 
											  media_node_id* p_outTimeSource)
{
	return B_OK;
}

status_t EMBeMediaRenderingNode::GetNextInput(int32* p_cookie, media_input* p_outInput)
{
	if(*p_cookie >= 0 && *p_cookie <= 1) 
	{
		if(m_sInputs[*p_cookie].source == media_source::null) 
		{
			m_sInputs[*p_cookie].format = m_sPreferedInputFormat[*p_cookie]; //.type = B_MEDIA_RAW_AUDIO;
			m_sInputs[*p_cookie].format.type = m_sPreferedInputFormat[*p_cookie].type;
			m_sInputs[*p_cookie].node = Node();
			m_sInputs[*p_cookie].destination.port = ControlPort();
			m_sInputs[*p_cookie].destination.id = *p_cookie;
		}
		*p_outInput = m_sInputs[*p_cookie];
		(*p_cookie)++;
		return B_OK;
	}
	return B_BAD_INDEX;
}

status_t EMBeMediaRenderingNode::HandleMessage(int32 message, const void * data, size_t size)
{
	if((BBufferConsumer::HandleMessage(message, data, size) < 0) && 
	   (BMediaNode::HandleMessage(message, data, size) < 0))
	{
		HandleBadMessage(message, data, size);
		return B_ERROR;
	}
	return B_OK;
}

void EMBeMediaRenderingNode::NodeRegistered(void)
{
	m_vThread = spawn_thread(ThreadEntry, EM_THREAD_NAME_RENDERING_NODE, 2, this); //EM_PRODUCTION_THREADPRIO_ACTIVE, this);
	resume_thread(m_vThread);
	m_sInputs[EM_AUDIO_INPUT].destination.port = ControlPort();
	m_sInputs[EM_AUDIO_INPUT].destination.id = EM_AUDIO_INPUT;
	m_sInputs[EM_VIDEO_INPUT].destination.port = ControlPort();
	m_sInputs[EM_VIDEO_INPUT].destination.id = EM_VIDEO_INPUT;
}

void EMBeMediaRenderingNode::Preroll(void)
{
	EMDebugger("Consumer nore prerolling!");
}

void EMBeMediaRenderingNode::Seek(bigtime_t media_time, bigtime_t performance_time)
{
}

status_t EMBeMediaRenderingNode::ThreadEntry(void* p_upObject)
{
	if(p_upObject != NULL)
		static_cast<EMBeMediaRenderingNode*>(p_upObject) -> Run();
	return B_OK;
}

void EMBeMediaRenderingNode::Run()
{
	char * msg = new char[B_MEDIA_MESSAGE_SIZE];
	while(true)
	{
		bigtime_t timeout = 1000;
		int32 code = 0;
		ssize_t size = read_port_etc(ControlPort(), &code, msg, B_MEDIA_MESSAGE_SIZE, B_TIMEOUT, timeout);
		if(size >= 0)
		{
			if(code == EM_PORT_MESSAGE_SHUTDOWN)
				break;
			HandleMessage(code, msg, size);
		}
		else if(size == B_BAD_PORT_ID)
		{
			EMDebugger("ERROR! Consumer did bad read port!");
		}
		snooze(1000);
	}
	delete msg;
}

void EMBeMediaRenderingNode::SetTimeSource(BTimeSource* time_source)
{
}

void EMBeMediaRenderingNode::Start(bigtime_t performance_time)
{
	if(!m_vReadyForWriting)
		PrepareFile();
}

void EMBeMediaRenderingNode::Stop(bigtime_t performance_time, bool immediate)
{
}

void EMBeMediaRenderingNode::ProducerDataStatus(const media_destination& p_destination, 
											   int32 p_status, 
											   bigtime_t p_atPerformanceTime) 
{
	if(p_status == B_DATA_AVAILABLE)
		;
	else if(p_status == B_DATA_NOT_AVAILABLE)
		;
	else if(p_status == B_PRODUCER_STOPPED)
		;
}

bool EMBeMediaRenderingNode::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
		case EM_MESSAGE_START:
		{
			acquire_sem(m_vSemaphore);
			if(EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingAudio() || EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingVideo())
			{
				if(PrepareFile())
					m_vShouldRender = true;
				else
					m_vShouldRender = false;
			}
			release_sem(m_vSemaphore);
			break;
		}
		case EM_MESSAGE_STOP:
		{
			acquire_sem(m_vSemaphore);
			m_vShouldRender = false;
			CloseFile();
			release_sem(m_vSemaphore);
			break;
		}
		default:
			break;
	};
	return true;
}

bool EMBeMediaRenderingNode::Initialize()
{
	m_sPreferedInputFormat[EM_AUDIO_INPUT].u.raw_audio.buffer_size = media_raw_audio_format::wildcard.buffer_size;
	m_sPreferedInputFormat[EM_AUDIO_INPUT].type = B_MEDIA_RAW_AUDIO;
	m_sPreferedInputFormat[EM_AUDIO_INPUT].u.raw_audio.format = media_raw_audio_format::B_AUDIO_UCHAR; //media_raw_audio_format::B_AUDIO_SHORT;
	m_sPreferedInputFormat[EM_AUDIO_INPUT].u.raw_audio.channel_count = 1; //EM_AUDIO_NUM_CHANNELS;

	string oAudioName("RenderAudio2Disk");
	string oVideoName("RenderVideo2Disk");
	strcpy(m_sInputs[EM_AUDIO_INPUT].name, oAudioName.c_str());
	strcpy(m_sInputs[EM_VIDEO_INPUT].name, oVideoName.c_str());
	m_sPreferedInputFormat[EM_VIDEO_INPUT].type = B_MEDIA_RAW_VIDEO;
	m_sPreferedInputFormat[EM_VIDEO_INPUT].u.encoded_video = media_encoded_video_format::wildcard;
	return true;
}

bool EMBeMediaRenderingNode::HaveAudioConnection() const
{
	return (m_sAudioFormat.u.raw_audio != media_raw_audio_format::wildcard);
}

bool EMBeMediaRenderingNode::HaveVideoConnection() const
{
	return (m_sVideoFormat.u.raw_video != media_raw_video_format::wildcard);
}

bool EMBeMediaRenderingNode::PrepareFile()
{
	//Prepare rendering format
	m_oDesiredFormat = new EMMediaFormat(EM_TYPE_RAW_VIDEO);
	m_oDesiredFormat -> m_vWidth = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_RENDER_FORMAT_WIDTH)));
	m_oDesiredFormat -> m_vHeight = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_RENDER_FORMAT_HEIGHT)));
	m_oDesiredFormat -> m_vBytesPerRow = m_oDesiredFormat -> m_vWidth * 4; // * 4 must be changed later on to be desired color space (changed in rendering options)

	m_vRenderVideo = false;
	m_vRenderAudio = false;
	m_opFamilyName = static_cast<string*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_RENDER_CODEC_FAMILY));
	m_opVideoCodecName = static_cast<string*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_RENDER_CODEC));
	m_opAudioCodecName = static_cast<string*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_RENDER_CODEC));
	int vQuality = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_RENDER_CODEC_QUALITY)));
	m_vVideoFrameRate = *(static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_RENDER_FRAMERATE)));

	string oRenderFileName = EMBeMediaUtility::MakeRenderFileName(m_vRenderFileSequenceNumber++);

	m_vpQuality = float(vQuality/100.0);

	if( *m_opFamilyName == "unknown")
		return true;
		
	int32 vCookie = 0;
	media_format_family sFamily;
	if(m_opMediaFile == NULL)	
	{
		entry_ref sRef;
		if(get_ref_for_path(oRenderFileName.c_str(), &sRef) != B_OK)
			EMDebugger("ERROR! Could not get ref to file");

		//GET THE REQUESTED FAMILY
		while(get_next_file_format(&vCookie, &m_sFileFormat) == B_OK)
		{
			if (strcmp(m_sFileFormat.pretty_name, m_opFamilyName -> c_str()) == 0)
			{
				sFamily = m_sFileFormat.family;
								
				break;
			}
		}
		
		//END GETTINg FAMILY
		m_opMediaFile = new BMediaFile(&sRef, &m_sFileFormat);
		if(m_opMediaFile -> InitCheck() != B_OK)
			EMDebugger("ERROR! Could not initialize BMediaFile");
	}

	if(HaveAudioConnection() && HaveVideoConnection() && m_opVideoTrack == NULL && m_opMediaFile != NULL)
	{	
		media_format sFormat;
		memcpy(&sFormat, &m_sVideoFormat, sizeof(media_format));
		
		media_codec_info mci;
		vCookie = 0;
		media_format outfmt;
		
		sFormat.u.raw_video.display.line_width =  m_oDesiredFormat -> m_vWidth;
		sFormat.u.raw_video.display.line_count =  m_oDesiredFormat -> m_vHeight;

		outfmt.u.raw_video.display.line_width =  m_oDesiredFormat -> m_vWidth;
		outfmt.u.raw_video.display.line_count =  m_oDesiredFormat -> m_vHeight;

		while(get_next_encoder(&vCookie, &m_sFileFormat, &sFormat, &outfmt, &mci) == B_OK)
		{
			if(strcmp(mci.pretty_name, m_opVideoCodecName -> c_str()) == 0)
			{
				;//cout_commented_out_4_release<<"SHORT NAME:" << mci.short_name << endl;
				m_vRenderVideo = true;
				break;
			}
		}

		if(m_vRenderVideo)
		{	

			sFormat.u.raw_video.field_rate = m_vVideoFrameRate;
			sFormat.u.raw_video.display.line_width =  m_oDesiredFormat -> m_vWidth;
			sFormat.u.raw_video.display.line_count =  m_oDesiredFormat -> m_vHeight;
			m_opVideoTrack = m_opMediaFile -> CreateTrack(&sFormat, &mci);
			if(m_opVideoTrack == NULL)
				EMDebugger("ERROR! Could not create video track in BMediaFile");	

			if(m_opVideoTrack -> InitCheck() != B_OK)
				EMDebugger("ERROR! Could not initialize video track in BMediaFile!");

			//Set quality
			if(m_opVideoTrack -> SetQuality(m_vpQuality) == B_OK)
				; //;//cout_commented_out_4_release<< "QUALITY WAS SET TO:" << m_vpQuality << endl;

			encode_parameters oEncodeParams;
			m_opVideoTrack -> GetEncodeParameters(&oEncodeParams);
			oEncodeParams.avg_field_size = 3000000;
			oEncodeParams.max_field_size = 6000000;
			;//cout_commented_out_4_release<< "Quality:" << oEncodeParams.quality << endl;
			;//cout_commented_out_4_release<< "avg_field_size:" << oEncodeParams.avg_field_size << endl;
			;//cout_commented_out_4_release<< "max_field_size:" << oEncodeParams.max_field_size << endl;
		}

	}


	if(HaveAudioConnection() && HaveVideoConnection() && m_opAudioTrack == NULL && m_opMediaFile != NULL)
	{
		media_format sFormat;
		memcpy(&sFormat, &m_sAudioFormat, sizeof(media_format));
		
		media_codec_info mci;
		int32 vCookie = 0;
		media_format outfmt;

		while(get_next_encoder(&vCookie, &m_sFileFormat, &sFormat, &outfmt, &mci) == B_OK)
		{
			;//cout_commented_out_4_release<< "AUDIO CODEC:" << mci.pretty_name << endl;
			if(strcmp(mci.pretty_name, m_opAudioCodecName -> c_str()) == 0)
			{
				;//cout_commented_out_4_release<< "GONNA USE AUDIO CODEC:" << mci.pretty_name << endl;
				m_vRenderAudio = true;
				break;
			}
		}


		if(m_vRenderAudio)
		{
			m_opAudioTrack = m_opMediaFile -> CreateTrack(&sFormat, &mci);
			if(m_opAudioTrack == NULL)
			{
				EMDebugger("ERROR! Could not create audio track in BMediaFile");	
			}
			if(m_opAudioTrack -> InitCheck() != B_OK)
				EMDebugger("ERROR! Could not initialize audio track in BMediaFile!");
		}

	}

			m_vRenderAudio = true;
	if(HaveAudioConnection() && HaveVideoConnection() && (m_opAudioTrack != NULL || m_opVideoTrack != NULL) && ! m_vReadyForWriting)
	{	
		if(m_opMediaFile -> CommitHeader() != B_OK)
		{
			CloseFile();
			(new BAlert("", "Couldn't commit Header(Maybe render-format doesn't support this frame size?)", "So Be It"))->Go();
			return false;
		} else
			m_vReadyForWriting = true;
	}
	;//cout_commented_out_4_release<<"At the moment " << m_opMediaFile -> CountTracks() << " Tracks have been created" << endl;
	
	return true;
}

bool EMBeMediaRenderingNode::CloseFile()
{
	if(m_oDesiredFormat != NULL)
	{
		delete m_oDesiredFormat;
		m_oDesiredFormat = NULL;
	}
	if(m_opMediaFile != NULL)
	{
		if(m_opMediaFile -> CloseFile() != B_OK)
			emerr << "ERROR! The media file could not be closed!" << endl;
		
		if(m_opVideoTrack != NULL)
		{
			m_opMediaFile -> ReleaseTrack(m_opVideoTrack);
			m_opVideoTrack = NULL;
		}

		if(m_opAudioTrack != NULL)
		{
			m_opMediaFile -> ReleaseTrack(m_opAudioTrack);
			m_opAudioTrack = NULL;
		}

		delete m_opMediaFile;
		m_opMediaFile = NULL;
	
		m_vReadyForWriting = false;
	}
	else
		;//emout_commented_out_4_release << "WARNING! No render-result media file to close!" << endl;
	
	return true;
}

bool EMBeMediaRenderingNode::ClearData()
{
	m_vRenderFileSequenceNumber = 0;
	return true;
}

bool EMBeMediaRenderingNode::SaveData(EMProjectDataSaver* p_opSaver)
{
	p_opSaver -> SaveUInt32(m_vRenderFileSequenceNumber);
	return true;
}

bool EMBeMediaRenderingNode::LoadData(EMProjectDataLoader* p_opLoader)
{
	m_vRenderFileSequenceNumber = p_opLoader -> LoadUInt32();
	return true;
}

#endif

