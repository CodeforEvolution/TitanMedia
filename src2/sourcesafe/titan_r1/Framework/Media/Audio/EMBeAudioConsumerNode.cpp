#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#include "EMBeAudioConsumerNode.h"

#include "EMBeMediaUtility.h"
#include "EMMediaTimer.h"
#include "EMMediaDataBuffer.h"
#include "EMWaveFileWriter.h"
#include "EMBufferDestination.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMMediaEngine.h"

#include <MediaKit.h>
#include <OS.h>
#include <StorageKit.h>
#include <fstream>

EMBeAudioConsumerNode::EMBeAudioConsumerNode(media_output* p_spPhysicalInOutput) 
	:	BMediaNode(EM_THREAD_NAME_AUDIO_RECORDER), 
		BBufferConsumer(B_MEDIA_RAW_AUDIO),
		EMMediaBufferSource(EM_TYPE_RAW_AUDIO),
		m_opWaveFile(NULL),
		m_vIsRecording(false),
		m_vTemp(false)
{
	memcpy(&m_sPhysicalInOutput, p_spPhysicalInOutput, sizeof(media_output));

	m_sControlPortID = 0;
	m_sInput.source = media_source::null;
	m_sInput.destination = media_destination::null;

	m_sControlPortID = create_port(10, EM_THREAD_NAME_AUDIO_RECORDER);
	if(m_sControlPortID == B_BAD_VALUE || m_sControlPortID == B_NO_MORE_PORTS)
		EMDebugger("ERROR! EMBeAudioConsumerNode::ServiceThread() - Could not create port... Bad port value, or no more ports!");

	m_vSemaphore = create_sem(1, "Buffer protection sem");
	EMBeMediaUtility::GetRosterE() -> RegisterNode(this);
	EMBeMediaUtility::push(this, "EMBeAudioConsumerNode");
	
	
}

EMBeAudioConsumerNode::~EMBeAudioConsumerNode()
{
	BMediaRoster::Roster() -> UnregisterNode(this);
	status_t vErrorCode = B_OK;
	write_port(m_sControlPortID, EM_PORT_MESSAGE_SHUTDOWN, 0, 0);
	while(wait_for_thread(m_vThread, &vErrorCode) == B_INTERRUPTED)
		snooze(1000);
	delete_port(m_sControlPortID);
	delete m_opWaveFile;
	EMBeMediaUtility::pop("EMBeAudioConsumerNode");
}

status_t EMBeAudioConsumerNode::AcceptFormat(const media_destination& p_destination, media_format* p_format)
{
	if(p_destination != m_sInput.destination)
	{
		EMDebugger("EMBeAudioConsumerNode::AcceptFormat() - ERROR! Destination doesn't belong in this node!");
		return B_MEDIA_BAD_DESTINATION;
	}
	
	if(p_format -> type <= 0) 
	{
		p_format->type = B_MEDIA_RAW_AUDIO;
		p_format->u.raw_audio = media_raw_audio_format::wildcard;
	}
	else if(p_format -> type != B_MEDIA_RAW_AUDIO) 
	{
		p_format -> type = B_MEDIA_RAW_AUDIO;
		p_format -> u.raw_audio = media_raw_audio_format::wildcard;
		EMDebugger("EMBeAudioConsumerNode::AcceptFormat() - ERROR! Bad format!");
		return B_MEDIA_BAD_FORMAT;
	}
	
	if(p_format -> u.raw_audio.buffer_size != static_cast<uint64>(EM_AUDIO_BUFFER_SIZE))
	{
		p_format -> u.raw_audio.buffer_size = EM_AUDIO_BUFFER_SIZE;
	}

	return B_OK;
}

BMediaAddOn* EMBeAudioConsumerNode::AddOn(int32* p_outInternalID) const
{
	if (p_outInternalID) 
		*p_outInternalID = 0;
	return NULL;
}

void EMBeAudioConsumerNode::BufferReceived(BBuffer* p_opBuffer)
{
	if(p_opBuffer != NULL)
	{	
		if(m_opWaveFile != NULL && m_vIsRecording)
		{
			m_opWaveFile -> WriteData(p_opBuffer -> Data(), p_opBuffer -> SizeUsed());
			EMMediaTimer::Instance() -> IncreaseNowFrame(EMBeMediaUtility::BytesToFrames(p_opBuffer -> SizeUsed(), EMBeMediaUtility::GetSystemAudioFormat()));
		}
		p_opBuffer -> Recycle();
	}
}

status_t EMBeAudioConsumerNode::Connected(const media_source& p_source, 
										  const media_destination& p_destination, 
										  const media_format& p_format,
										  media_input* p_outInput)
{
	if(m_sInput.source != media_source::null)
	{
		EMDebugger("EMBeAudioConsumerNode::Connected() - ERROR! We can only accept one connection! It's allready in use!!");
		return B_MEDIA_BAD_SOURCE;
	}
	if(p_destination != m_sInput.destination)
	{
		EMDebugger("EMBeAudioConsumerNode::Connected() - ERROR! Destination is not an input into this node!");
		return B_MEDIA_BAD_DESTINATION;
	}
	m_sInput.source = p_source;
	m_sInput.format = p_format;
	*p_outInput = m_sInput;
	
	return B_OK;
}

port_id EMBeAudioConsumerNode::ControlPort(void) const
{
	return m_sControlPortID;
}

void EMBeAudioConsumerNode::Disconnected(const media_source& p_sSource, const media_destination& p_sDestination) 
{
	if(p_sSource != m_sInput.source) 
		return;
	if(p_sDestination != m_sInput.destination) 
		return;
	m_sInput.source = media_source::null;
}

void EMBeAudioConsumerNode::DisposeInputCookie(int32 p_cookie)
{
}

status_t EMBeAudioConsumerNode::FormatChanged(const media_source& p_source, 
											  const media_destination& p_destination, 
											  int32 p_something, 
											  const media_format& p_format)
{
	return B_OK;
}

status_t EMBeAudioConsumerNode::GetLatencyFor(const media_destination& p_forWhom, 
											  bigtime_t* p_outLatency, 
											  media_node_id* p_outTimeSource)
{
	return B_OK;
}

status_t EMBeAudioConsumerNode::GetNextInput(int32* p_cookie, media_input* p_outInput)
{
	if (!*p_cookie) 
	{
		if (m_sInput.source == media_source::null) 
		{
			m_sInput.format.type = B_MEDIA_RAW_AUDIO;
			m_sInput.format.u.raw_audio = media_raw_audio_format::wildcard;
			m_sInput.format.u.raw_audio.format = 4;
			m_sInput.format.u.raw_audio.buffer_size = EM_AUDIO_BUFFER_SIZE;
			m_sInput.node = Node();
			m_sInput.destination.port = ControlPort();
			m_sInput.destination.id = 1;
		}
		*p_outInput = m_sInput;
		*p_cookie = 1;
		return B_OK;
	}
	return B_BAD_INDEX;
}

status_t EMBeAudioConsumerNode::HandleMessage(int32 message, const void * data, size_t size)
{
	if((BBufferConsumer::HandleMessage(message, data, size) < 0) && 
	   (BMediaNode::HandleMessage(message, data, size) < 0))
	{
		HandleBadMessage(message, data, size);
		return B_ERROR;
	}
	return B_OK;
}

void EMBeAudioConsumerNode::NodeRegistered(void)
{
	m_vThread = spawn_thread(ThreadEntry, EM_THREAD_NAME_AUDIO_RECORDER, 2, this); //EM_PRODUCTION_THREADPRIO_ACTIVE, this);
	resume_thread(m_vThread);
}

void EMBeAudioConsumerNode::Preroll(void)
{
	EMDebugger("Consumer nore prerolling!");
}

void EMBeAudioConsumerNode::Seek(bigtime_t media_time, bigtime_t performance_time)
{
}

status_t EMBeAudioConsumerNode::ThreadEntry(void* p_upObject)
{
	if(p_upObject != NULL)
		static_cast<EMBeAudioConsumerNode*>(p_upObject) -> Run();
	return B_OK;
}

void EMBeAudioConsumerNode::Run()
{
	m_sInput.destination.port = m_sControlPortID;
	m_sInput.destination.id = 1;
	char * msg = EM_new char[B_MEDIA_MESSAGE_SIZE];
	while(true)
	{
		bigtime_t timeout = 1000;//Timeout();
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

void EMBeAudioConsumerNode::SetTimeSource(BTimeSource* time_source)
{
}

void EMBeAudioConsumerNode::SetTargetFile(string p_oFileName)
{
	if(m_opWaveFile != NULL)
		delete m_opWaveFile;

	m_opWaveFile = EM_new EMWaveFileWriter(p_oFileName.c_str());
	if(! m_opWaveFile -> InitCheckE())
		emerr << "ERROR! Could not initialize output WAVE-file!" << endl;
}

void EMBeAudioConsumerNode::CloseTargetFile()
{
	;//cout_commented_out_4_release << "Now writing WAVE-Header!" << endl;
	if(m_opWaveFile != NULL)
	{
		;//cout_commented_out_4_release << "Writing..." << endl;
		m_opWaveFile -> WriteHeaderE(EMBeMediaUtility::GetSystemAudioFormat());
	}
	//EMDebugger("Target file closed!");
}

void EMBeAudioConsumerNode::Start(bigtime_t performance_time)
{
	int32 vChangeTag;
	SetOutputEnabled(m_sInput.source, m_sInput.destination, true, NULL, &vChangeTag);
	int64 vStartingTime = TimeSource() -> Now() + 5000;
	EMBeMediaUtility::GetRosterE() -> StartNode(TimeSource() -> Node(), vStartingTime);
	EMBeMediaUtility::GetRosterE() -> StartNode(m_sPhysicalInOutput. node, vStartingTime);
	status_t vResult = EMBeMediaUtility::GetRosterE() -> StartNode(m_sPhysicalInOutput.node, 0);
	if(vResult != B_OK)
		EMDebugger("ERROR! Could not start physical input node!");
	m_vIsRecording = true;
}

void EMBeAudioConsumerNode::Stop(bigtime_t performance_time, bool immediate)
{
	m_vIsRecording = false;
	EMBeMediaUtility::GetRosterE() -> StopNode(TimeSource() -> Node(), 0);
	int32 vChangeTag;
	SetOutputEnabled(m_sInput.source, m_sInput.destination, false, NULL, &vChangeTag);
}

void EMBeAudioConsumerNode::ProducerDataStatus(const media_destination& p_destination, 
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

bool EMBeAudioConsumerNode::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
		case EM_MESSAGE_START:
			break;
		case EM_MESSAGE_STOP:
			break;
		default:
			break;
	};
	return true;
}

bool EMBeAudioConsumerNode::Initialize()
{
	float vAudioFrameRate = (float) *(static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_FRAMERATE)));
	BTimeSource* opTimeSource = EMBeMediaUtility::GetRosterE() -> MakeTimeSourceFor(m_sPhysicalInOutput.node);
	status_t err = EMBeMediaUtility::GetRosterE() -> SetTimeSourceFor(Node().node, opTimeSource -> Node().node);
	if(err != B_OK)
		EMDebugger("ERROR! Could not set time source for self!");
	if(EMBeMediaUtility::GetRosterE() -> SetTimeSourceFor(m_sPhysicalInOutput.node.node, opTimeSource -> Node().node) != B_OK)
		EMDebugger("ERROR! Could not set time source for destination node!");
	EMBeMediaUtility::GetRosterE() -> StartNode(opTimeSource -> Node(), 0);
	
	m_sPreferedInputFormat.u.raw_audio = media_raw_audio_format::wildcard;
	m_sPreferedInputFormat.type = B_MEDIA_RAW_AUDIO;
	m_sPreferedInputFormat.u.raw_audio.format = media_raw_audio_format::B_AUDIO_SHORT;
	m_sPreferedInputFormat.u.raw_audio.frame_rate = vAudioFrameRate;
	m_sPreferedInputFormat.u.raw_audio.byte_order = B_MEDIA_LITTLE_ENDIAN;
	m_sPreferedInputFormat.u.raw_audio.buffer_size = EM_AUDIO_BUFFER_SIZE;
	m_sPreferedInputFormat.u.raw_audio.channel_count = EM_AUDIO_NUM_CHANNELS;

	char vpName[B_MEDIA_NAME_LENGTH]; 
	m_sInput.node = this -> Node();
	m_sInput.format = m_sPreferedInputFormat;
	m_sInput.destination = media_destination::null;
	m_sInput.destination.id = 2000;
	m_sInput.destination.port = this -> Node().port;
	m_sInput.source = media_source::null;
	sprintf(vpName, "%s%d", "Titan physical input IN #", 1);
	strcpy(m_sInput.name, vpName);
	memset(vpName, 0, B_MEDIA_NAME_LENGTH);
	
	int32 vNum;
	media_input sConsumerInput;
	EMBeMediaUtility::GetRosterE() -> GetFreeInputsFor(Node(), &sConsumerInput, 1, &vNum);

	media_format sFormat = m_sPhysicalInOutput.format;
	status_t vErrorCode = EMBeMediaUtility::GetRosterE() -> Connect(m_sPhysicalInOutput.source, sConsumerInput.destination, &sFormat, &m_sPhysicalInOutput, &sConsumerInput);
	if(vErrorCode)
		return false;

	return true;
}

bool EMBeAudioConsumerNode::Shutdown()
{
	status_t vErrorCode = EMBeMediaUtility::GetRosterE() -> Disconnect(m_sPhysicalInOutput.node.node, m_sPhysicalInOutput.source, Node().node, m_sInput.destination);
	if(vErrorCode)
	{
		emerr << "ERROR! Consumer node could not disconnect source from itself: " << strerror(vErrorCode) << endl;
		EMDebugger("ERROR! Consumer node could not disconnect source from itself");
		return false;
	}
	return true;
}

#endif


