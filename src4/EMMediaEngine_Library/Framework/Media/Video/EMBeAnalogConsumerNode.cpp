//	Copyright (c) 1998-99, Be Incorporated, All Rights Reserved.
//	SMS
//	EMBeAnalogConsumerNode.cpp
#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#include <View.h>
#include <stdio.h>
#include <fcntl.h>
#include <Buffer.h>
#include <unistd.h>
#include <string.h>
#include <NodeInfo.h>
#include <scheduler.h>
#include <TimeSource.h>
#include <StringView.h>
#include <MediaRoster.h>
#include <Application.h>
#include <BufferGroup.h>

#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMBeAnalogConsumerNode.h"
#include "EMBeMediaUtility.h"
#include "EMMediaEngine.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"
#include "CommandIDs.h"

#define M1 ((double)1000000.0)
#define JITTER		20000

#define	FUNCTION	;//_commented_out_4_release_printf
#define ERROR		;//_commented_out_4_release_printf
#define PROGRESS	;//_commented_out_4_release_printf
#define LOOP		;//_commented_out_4_release_printf


//---------------------------------------------------------------

EMBeAnalogConsumerNode::EMBeAnalogConsumerNode(const char * name, BMediaAddOn *addon, const uint32 internal_id, media_output* p_spPhysicalInOutput) :
	BBufferConsumer(B_MEDIA_RAW_VIDEO),
	BMediaEventLooper(),
	BMediaNode(name),
	mInternalID(internal_id),
	mAddOn(addon),
	mRate(1000000),
	mImageFormat(0),
	mTranslator(0),
	mMyLatency(20000),
	mBuffers(NULL),
	mOurBuffers(false),
	jakob(0),
	m_spPhysicalInOutput(p_spPhysicalInOutput),
	m_vRenderFileSequenceNumber(0),
	m_vFrameCount(0)
	
{
	EMBeMediaUtility::push(this, "EMBeAnalogConsumerNode");
	m_vReadyForWriting = false;
//	AddNodeKind(B_PHYSICAL_OUTPUT);
//	SetEventLatency(0);

	m_opReceiver = NULL;
	for (uint32 j = 0; j < 3; j++)
	{
		mBitmap[j] = NULL;
		mBufferMap[j] = 0;
	}

	BMediaEventLooper::SetBufferDuration(200000);
	SetEventLatency(2000000);

	status_t status = EMBeMediaUtility::GetRosterE()->RegisterNode(this);
	if (status != B_OK) {
		EMDebugger("Can't register the analog video");
	}
	//BMediaEventLooper::SetPriority(B_DISPLAY_PRIORITY);
	
	//BMediaEventLooper::SetBufferDuration(200000);
//	SetEventLatency(2000000);
	//BMediaEventLooper::SetRunMode(B_DROP_DATA);
}

//---------------------------------------------------------------

EMBeAnalogConsumerNode::~EMBeAnalogConsumerNode()
{
	;//_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::~EMBeAnalogConsumerNode\n");
	//status_t status;
	
	SetRunState(B_QUITTING);
	DeleteBuffers();
	BMediaEventLooper::Quit();
	SetRunState(B_TERMINATED);

	BMediaRoster::Roster() -> UnregisterNode(this);
	EMBeMediaUtility::pop("EMBeAnalogConsumerNode");	
	BMediaEventLooper::Quit();	
	
}

/********************************
	From BMediaNode
********************************/

//---------------------------------------------------------------

BMediaAddOn *
EMBeAnalogConsumerNode::AddOn(long *cookie) const
{
	;//_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::AddOn\n");
	// do the right thing if we're ever used with an add-on
	*cookie = mInternalID;
	return mAddOn;
}


//---------------------------------------------------------------

// This implementation is required to get around a bug in
// the ppc compiler.

void 
EMBeAnalogConsumerNode::Start(bigtime_t performance_time)
{
	BMediaEventLooper::Start(performance_time);
/*	status_t error = EMBeMediaUtility::GetRosterE() -> StartNode(TimeSource() -> Node(), 0);
	if(error != B_OK)
		EMDebugger("Couldn't start timesource() for some erason");

	//m_vIsRecording = true;

	error = EMBeMediaUtility::GetRosterE()->SetRunModeNode(Node(), B_RECORDING);
	if (error < B_OK) {
		EMDebugger(";//ERROR_commented_out_4_release_FUNCTION: MediaRoster->SetRunModeNode() !");
	}

	error = EMBeMediaUtility::GetRosterE()->StartNode(m_spPhysicalInOutput -> node, 0);
	if (error < B_OK)
		EMDebugger("Couldn't start video input!");
*/
}

void 
EMBeAnalogConsumerNode::Stop(bigtime_t performance_time, bool immediate)
{
	BMediaEventLooper::Stop(performance_time, immediate);
/*	EMBeMediaUtility::GetRosterE() -> StopNode(TimeSource() -> Node(), 0);
	int32 vChangeTag;
	SetOutputEnabled(mIn.source, mDestination, false, NULL, &vChangeTag);
*/
	m_vFrameCount = 0;
}

/*
void 
EMBeAnalogConsumerNode::Seek(bigtime_t media_time, bigtime_t performance_time)
{
	BMediaEventLooper::Seek(media_time, performance_time);
}

void 
EMBeAnalogConsumerNode::TimeWarp(bigtime_t at_real_time, bigtime_t to_performance_time)
{
	BMediaEventLooper::TimeWarp(at_real_time, to_performance_time);
}

status_t 
EMBeAnalogConsumerNode::DeleteHook(BMediaNode *node)
{
	return BMediaEventLooper::DeleteHook(node);
}
*/
//---------------------------------------------------------------

void
EMBeAnalogConsumerNode::NodeRegistered()
{
	;//_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::NodeRegistered\n");

	SetPriority(B_REAL_TIME_PRIORITY);
	Run();

	mIn.destination.port = ControlPort();
	mIn.destination.id = 0;
	mIn.source = media_source::null;
	mIn.format.type = B_MEDIA_RAW_VIDEO;
	//mIn.format.u.raw_video = vid_format;
}

//---------------------------------------------------------------

status_t
EMBeAnalogConsumerNode::RequestCompleted(const media_request_info & info)
{
//	int32* changeTag;
//	void* userData;

	;//_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::RequestCompleted\n");
/*	char format_string[256];		
	string_for_format(info.format, format_string, 256);
	;//_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::AcceptFormat: %s\n", format_string);
	if((info.format.u.raw_video.display.line_width != m_sFormat.u.raw_video.display.line_width) || (info.format.u.raw_video.display.line_width != m_sFormat.u.raw_video.display.line_width))
	{
		mIn.format = info.format;
		m_sFormat = info.format;
		;//cout_commented_out_4_release<<"FRAME RATE:"<< m_sFormat.u.raw_video.field_rate << endl;
		;//cout_commented_out_4_release<<"Delete buffers"<<endl;
		DeleteBuffers();
		;//cout_commented_out_4_release<<"Create buffers"<<endl;
		CreateBuffers(m_sFormat);
		;//cout_commented_out_4_release<<"Set output for"<<endl;	
		BBufferConsumer::SetOutputBuffersFor(info.source, info.destination, mBuffers, userData, changeTag, true);
		;//cout_commented_out_4_release<<"End change format"<<endl;
	} else
		;//cout_commented_out_4_release<< "Format seems to be the same as before ; I will not change anything!" << endl;
*/
	switch(info.what)
	{
		case media_request_info::B_SET_OUTPUT_BUFFERS_FOR:
			{
				if (info.status != B_OK)
					;//ERROR_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::RequestCompleted: Not using our buffers!\n");
			}
			break;
		case media_request_info::B_FORMAT_CHANGED:
			{
			}
			break;
		case media_request_info::B_SET_OUTPUT_ENABLED:
			{
			}
			break;
		case media_request_info::B_REQUEST_FORMAT_CHANGE:
			{
			}
			break;
		case media_request_info::B_SET_VIDEO_CLIPPING_FOR:
			{
			}
			break;
	}
	return B_OK;
}

//---------------------------------------------------------------

status_t
EMBeAnalogConsumerNode::HandleMessage(int32 message, const void * data, size_t size)
{
	if((BBufferConsumer::HandleMessage(message, data, size) < 0) && (BMediaNode::HandleMessage(message, data, size) < 0))
	{
		HandleBadMessage(message, data, size);
		return B_ERROR;
	}
	return B_OK;
}

//---------------------------------------------------------------

void
EMBeAnalogConsumerNode::BufferReceived(BBuffer * buffer)
{
	if (RunState() == B_STOPPED)
	{
		buffer->Recycle();
		return;
	}
	if(jakob<0)
	{
		mBuffers->ReclaimAllBuffers();
		jakob++;
	}
	media_timed_event event(buffer->Header()->start_time, BTimedEventQueue::B_HANDLE_BUFFER,
						buffer, BTimedEventQueue::B_RECYCLE_BUFFER);
	EventQueue()->AddEvent(event);
}

void EMBeAnalogConsumerNode::Lock()
{
	m_opReceiver -> LockPreviewWindow();
}

void EMBeAnalogConsumerNode::Unlock()
{
	m_opReceiver -> UnlockPreviewWindow();
}

//---------------------------------------------------------------

void
EMBeAnalogConsumerNode::ProducerDataStatus(
	const media_destination &for_whom,
	int32 status,
	bigtime_t at_media_time)
{
	;//_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::ProducerDataStatus\n");

	if (for_whom != mIn.destination)	
		return;
}

//---------------------------------------------------------------

status_t
EMBeAnalogConsumerNode::CreateBuffers(
	const media_format & with_format)
{
	;//_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::CreateBuffers\n");
	
	// delete any old buffers
	DeleteBuffers();	

	status_t status = B_OK;

	// create a buffer group
	uint32 mXSize = with_format.u.raw_video.display.line_width;
	uint32 mYSize = with_format.u.raw_video.display.line_count;	
	//uint32 mRowBytes = with_format.u.raw_video.display.bytes_per_row;
	color_space mColorspace = with_format.u.raw_video.display.format;
	;//PROGRESS_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::CreateBuffers - Colorspace = %d\n", mColorspace);

	mBuffers = new BBufferGroup();
	status = mBuffers->InitCheck();
	if (B_OK != status)
	{
		;//ERROR_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::CreateBuffers - ;//ERROR_commented_out_4_release_FUNCTION CREATING BUFFER GROUP\n");
		return status;
	}
	// and attach the  bitmaps to the buffer group
	for (uint32 j=0; j < 3; j++)
	{
		mBitmap[j] = new BBitmap(BRect(0, 0, (mXSize-1), (mYSize - 1)), mColorspace, true, true);
		if (mBitmap[j]->IsValid())
		{						
			buffer_clone_info info;
			if ((info.area = area_for(mBitmap[j]->Bits())) == B_ERROR)
				;//ERROR_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::CreateBuffers - ;//ERROR_commented_out_4_release_FUNCTION IN AREA_FOR\n");;
			info.offset = 0;
			info.size = (size_t)mBitmap[j]->BitsLength();
			info.flags = j;
			info.buffer = 0;

			if ((status = mBuffers->AddBuffer(info)) != B_OK)
			{
				;//ERROR_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::CreateBuffers - ;//ERROR_commented_out_4_release_FUNCTION ADDING BUFFER TO GROUP\n");
				return status;
			} else ;//PROGRESS_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::CreateBuffers - SUCCESSFUL ADD BUFFER TO GROUP\n");
		}
		else 
		{
			;//ERROR_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::CreateBuffers - ;//ERROR_commented_out_4_release_FUNCTION CREATING VIDEO RING BUFFER: %08x\n", status);
			return B_ERROR;
		}	
	}
	
	BBuffer ** buffList = new BBuffer * [3];
	for (int j = 0; j < 3; j++) buffList[j] = 0;
	
	if ((status = mBuffers->GetBufferList(3, buffList)) == B_OK)					
		for (int j = 0; j < 3; j++)
			if (buffList[j] != NULL)
			{
				mBufferMap[j] = (uint32) buffList[j];
				;//PROGRESS_commented_out_4_release_FUNCTION(" j = %d buffer = %08x\n", j, mBufferMap[j]);
			}
			else
			{
				;//ERROR_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::CreateBuffers ;//ERROR_commented_out_4_release_FUNCTION MAPPING RING BUFFER\n");
				return B_ERROR;
			}
	else
		;//ERROR_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::CreateBuffers ;//ERROR_commented_out_4_release_FUNCTION IN GET BUFFER LIST\n");
		
	;//_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::CreateBuffers - EXIT\n");
	return status;
}

//---------------------------------------------------------------

void
EMBeAnalogConsumerNode::DeleteBuffers()
{
	;//_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::DeleteBuffers\n");
	//status_t status;
	if (mBuffers)
	{
//		mBuffers->ReclaimAllBuffers();
//		mBuffers->WaitForBuffers();
	
		delete mBuffers;
		mBuffers = NULL;
		
		for (uint32 j = 0; j < 3; j++)
			if (mBitmap[j]->IsValid())
			{
				
				delete mBitmap[j];
				mBitmap[j] = NULL;
			}
	}
	;//_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::DeleteBuffers - EXIT\n");
}

//---------------------------------------------------------------

status_t
EMBeAnalogConsumerNode::ChangeColorDepth(const media_format & with_format)
{
	mIn.format = with_format;
	CreateBuffers(with_format);
	return B_OK;
}

status_t
EMBeAnalogConsumerNode::Connected(
	const media_source & producer,
	const media_destination & where,
	const media_format & with_format,
	media_input * out_input)
{
	;//_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::Connected\n");
	
	mIn.source = producer;
	mIn.format = with_format;
	mIn.node = Node();
	sprintf(mIn.name, "Video Analog Consumer Node");
	*out_input = mIn;

	uint32 user_data = 0;
	int32 change_tag = 1;	
	if (CreateBuffers(with_format) == B_OK)
	{
		BBufferConsumer::SetOutputBuffersFor(producer, mDestination, 
											mBuffers, (void *)&user_data, &change_tag, true);
	}
	else
	{
		;//ERROR_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::Connected - COULDN'T CREATE BUFFERS\n");
		return B_ERROR;
	}

	
	;//_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::Connected - EXIT\n");
	return B_OK;
}

//---------------------------------------------------------------

void
EMBeAnalogConsumerNode::Disconnected(
	const media_source & producer,
	const media_destination & where)
{
	;//_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::Disconnected\n");

	if (where == mIn.destination && producer == mIn.source)
	{
		// disconnect the connection
		mIn.source = media_source::null;
	}

}

//---------------------------------------------------------------

status_t
EMBeAnalogConsumerNode::AcceptFormat(
	const media_destination & dest,
	media_format * format)
{
	if (dest != mIn.destination)
	{
		;//ERROR_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::AcceptFormat - BAD DESTINATION\n");
		return B_MEDIA_BAD_DESTINATION;	
	}
	
	if (format->type == B_MEDIA_NO_TYPE)
		format->type = B_MEDIA_RAW_VIDEO;
	
	if (format->type != B_MEDIA_RAW_VIDEO)
	{
		;//ERROR_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::AcceptFormat - BAD FORMAT\n");
		return B_MEDIA_BAD_FORMAT;
	}

	if (format->u.raw_video.display.format != B_RGB32 &&
		format->u.raw_video.display.format != B_RGB16 &&
		format->u.raw_video.display.format != B_RGB15 &&			
		format->u.raw_video.display.format != B_GRAY8 &&			
		format->u.raw_video.display.format != media_raw_video_format::wildcard.display.format)
	{
		//ERROR("AcceptFormat - not a format we know about!\n");
		return B_MEDIA_BAD_FORMAT;
	}
		
	if (format->u.raw_video.display.format == media_raw_video_format::wildcard.display.format)
	{
		format->u.raw_video.display.format = B_RGB16;
	}

	char format_string[256];		
	string_for_format(*format, format_string, 256);
	m_sFormat = *format;
	;//_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::AcceptFormat: %s\n", format_string);

	return B_OK;
}

//---------------------------------------------------------------

void EMBeAnalogConsumerNode::ChangeFormat(media_format p_sFormat)
{
/*	int32* changeTag;
	void* userData;
	;//cout_commented_out_4_release<<"request format change"<<endl;
	p_sFormat.u.raw_video.field_rate = m_sFormat.u.raw_video.field_rate;
	status_t error = RequestFormatChange(m_spPhysicalInOutput -> source, m_to.destination, p_sFormat, userData, changeTag);
	if(error != B_OK)
		EMDebugger("Couldn't request format change");
*/
}

status_t
EMBeAnalogConsumerNode::GetNextInput(
	int32 * cookie,
	media_input * out_input)
{
	;//_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::GetNextInput\n");

	// custom build a destination for this connection
	// put connection number in id

/*	if (*cookie < 1)
	{
		mIn.node = Node();
		mIn.destination.id = *cookie;
		sprintf(mIn.name, "Video Consumer");
		*out_input = mIn;
		(*cookie)++;
		return B_OK;
	}
	else
	{
		ERROR("EMBeAnalogConsumerNode::GetNextInput - - BAD INDEX\n");
		return B_MEDIA_BAD_DESTINATION;
	}
*/	
	if (!*cookie) 
	{
		if (mIn.source == media_source::null) 
		{
			mIn.format.type = B_MEDIA_RAW_VIDEO;
			mIn.format.u.raw_video = media_raw_video_format::wildcard;
			//m_sInput.format.u.raw_video.format = 4;
			mIn.node = Node();
			mIn.destination.port = ControlPort();
			mIn.destination.id = 1;
		}
		*out_input = mIn;
		*cookie = 1;
		return B_OK;
	}
	return B_BAD_INDEX;
	
}

//---------------------------------------------------------------

void
EMBeAnalogConsumerNode::DisposeInputCookie(int32 /*cookie*/)
{
}

//---------------------------------------------------------------

status_t
EMBeAnalogConsumerNode::GetLatencyFor(
	const media_destination &for_whom,
	bigtime_t * out_latency,
	media_node_id * out_timesource)
{
	;//_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::GetLatencyFor\n");
	
	if (for_whom != mIn.destination)
		return B_MEDIA_BAD_DESTINATION;
	
	*out_latency = mMyLatency;
	*out_timesource = TimeSource()->ID();

	return B_OK;
}


//---------------------------------------------------------------

status_t
EMBeAnalogConsumerNode::FormatChanged(
				const media_source & producer,
				const media_destination & consumer, 
				int32 from_change_count,
				const media_format &format)
{
//	;//cout_commented_out_4_release<<"EMBeAnalogConsumerNode::FormatChanged"<<endl;
	
	if (consumer != mIn.destination)
		return B_MEDIA_BAD_DESTINATION;

	if (producer != mIn.source)
		return B_MEDIA_BAD_SOURCE;

	mIn.format = format;
	
	return CreateBuffers(format);
}

//---------------------------------------------------------------

void
EMBeAnalogConsumerNode::HandleEvent(
	const media_timed_event *event,
	bigtime_t lateness,
	bool realTimeEvent)

{
	
	BBuffer *buffer;
	
	uint32 index = 0;

	uint32 user_data = 0;
	int32 change_tag = 1;	
	int32* mPriority;
		BBuffer ** buffList = new BBuffer * [3];

	switch (event->type)
	{
		case BTimedEventQueue::B_START:
			;//PROGRESS_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::HandleEvent - START\n");
			break;
		case BTimedEventQueue::B_STOP:
			;//PROGRESS_commented_out_4_release_FUNCTION("EMBeAnalogConsumerNode::HandleEvent - STOP\n");
			
			//BBufferConsumer::SetOutputBuffersFor(mIn.source, mDestination, 
			//								NULL, (void *)&user_data, &change_tag, false);
			//BMediaRoster::SetOutputBuffersFor(mIn.source, NULL);								
//			BBufferConsumer::SetOutputEnabled(mIn.source, mDestination, 
//											false, (void *)&user_data, &change_tag);
			;//PROGRESS_commented_out_4_release_FUNCTION("VideoConsumer::HandleEvent - STOP\n");
			//BBufferConsumer::SetOutputBuffersFor(mIn.source, mDestination, mBuffers, (void *)&user_data, &change_tag, true);
			BBufferConsumer::SetOutputBuffersFor(mIn.source, mDestination, mBuffers, (void *)&user_data, &change_tag, true);
			EventQueue()->FlushEvents(event->event_time, BTimedEventQueue::B_ALWAYS, true, BTimedEventQueue::B_HANDLE_BUFFER);
			//BBufferConsumer::SetOutputBuffersFor(mIn.source, mDestination, mBuffers, (void *)&user_data, &change_tag, true);
			//BMediaEventLooper::EventQueue()->FlushEvents(lateness, BTimedEventQueue::B_ALWAYS, true, BTimedEventQueue::B_HANDLE_BUFFER);

//			DeleteBuffers();


/*		for (int j = 0; j < 3; j++) buffList[j] = 0;
	
		if (mBuffers->GetBufferList(3, buffList) == B_OK)
		for (int j = 0; j < 3; j++)
			if (buffList[j] != NULL)
			{
				buffList[j]->Recycle();
				;//cout_commented_out_4_release<<"Kalle"<<endl;
			}
*/

//			EventQueue()->FlushEvents(event->event_time, BTimedEventQueue::B_ALWAYS, true, BTimedEventQueue::B_HANDLE_BUFFER);
			break;

		case BTimedEventQueue::B_HANDLE_BUFFER:

			buffer = (BBuffer *) event->pointer;

			// see if this is one of our buffers
			mOurBuffers = true;
			while(index < 3)
				if ((uint32)buffer == mBufferMap[index])
					break;
				else
					index++;
					
			if (index == 3)
			{
				// no, buffers belong to consumer
				mOurBuffers = false;
				index = 0;
			}
										
				if (!mOurBuffers)
				{
					// not our buffers, so we need to copy
					memcpy(mBitmap[index]->Bits(), buffer->Data(),mBitmap[index]->BitsLength());
				}	
				uint32 flags;
				if ((mBitmap[index]->ColorSpace() == B_GRAY8) && !bitmaps_support_space(mBitmap[index]->ColorSpace(), &flags))
				{
					// handle mapping of GRAY8 until app server knows how
					uint32 *start = (uint32 *)mBitmap[index]->Bits();
					int32 size = mBitmap[index]->BitsLength();
					uint32 *end = start + size/4;
					for (uint32 *p = start; p < end; p++)
						*p = (*p >> 3) & 0x1f1f1f1f;
				}
				//mBuffers->ReclaimAllBuffers();
				
				//"Start" Preview if not active
				if(m_opReceiver == NULL)
				{
					m_opReceiver = new EMBeVideoDisplayer();
					//SetTo(mIn.format);
				}
	
				if(m_opReceiver == NULL)
					EMDebugger("m_opReceiver is NULL!");

				if(m_opReceiver -> LockPreviewWindow())
				{	
					m_opReceiver -> HandleNativeBuffer(mBitmap[index]);
				}

				if(m_vReadyForWriting)
				{
//					;//cout_commented_out_4_release<<"Writing"<<endl;
					m_vFrameCount++;
					char vpPercentage[40];
					sprintf(vpPercentage, "%d | Time:%d:%d:%d:%d", (int32) m_vFrameCount, (int32) ((m_vFrameCount / (int32)mIn.format.u.raw_video.field_rate)/3600)%60,(int32) ((m_vFrameCount / (int32)mIn.format.u.raw_video.field_rate)/60)%60,
														(int32)((m_vFrameCount / (int32)mIn.format.u.raw_video.field_rate))%60, (int32) (m_vFrameCount % (int32)mIn.format.u.raw_video.field_rate));
					string oString = string("Recording Frame: ") + vpPercentage;
//					EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_WRITE_STATUS_BAR_MESSAGE, &oString);	
					status_t err = m_opVideoTrack->WriteFrames(mBitmap[index] -> Bits(), 1, B_MEDIA_KEY_FRAME);//mh.u.encoded_video.field_flags);//field_flag =0 if not a keyframe
					if(err != B_OK)
						EMDebugger("ERROR WRITING analog FRAME TO File!");
				}
				buffer->Recycle();

			break;
		
		default:
//			;//cout_commented_out_4_release<<"EMBeAnalogConsumerNode::HandleEvent - BAD EVENT"<<endl;
			break;
	}			
}

bool EMBeAnalogConsumerNode::Initialize(media_format& p_oFormat)
{
//	;//cout_commented_out_4_release<<">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Initializing Analog VIDEO inputs" << endl;
	/* find the time source */
	status_t status = EMBeMediaUtility::GetRosterE()->GetTimeSource(&timesourceNode);
	if (status != B_OK) {
//		;//cout_commented_out_4_release<<"Can't get a time source:" << status << endl;
		return false;
	}


	
	/* register the node */
//	status = EMBeMediaUtility::GetRosterE()->RegisterNode(fVideoConsumer);


	//fPort = fVideoConsumer->ControlPort();

	

	/* find free consumer input */
	int32 cnt = 0;
	status = EMBeMediaUtility::GetRosterE()->GetFreeInputsFor(Node(), &m_to, 1, &cnt, B_MEDIA_RAW_VIDEO);
	if (status != B_OK || cnt < 1) {
		status = B_RESOURCE_UNAVAILABLE;
//		;//cout_commented_out_4_release<< "Can't find an available connection to the video window:" << strerror(status) << endl;
		return false;
	}
	/* Connect The Nodes!!! */
	
	/* connect producer to consumer */
	status = EMBeMediaUtility::GetRosterE()->Connect(m_spPhysicalInOutput -> source, m_to.destination, &p_oFormat, m_spPhysicalInOutput, &m_to);
	if (status != B_OK) {
		emerr << "ERROR! Can't connect the video source to the video window: Gonna try interlaced" << status << endl;
		p_oFormat.u.raw_video.interlace = 2;
		status = EMBeMediaUtility::GetRosterE()->Connect(m_spPhysicalInOutput -> source, m_to.destination, &p_oFormat, m_spPhysicalInOutput, &m_to);
		if (status != B_OK)
		{
			emerr << "ERROR! Can't still connect the video source to the video window:" << strerror(status) << endl;
			m_vRenderVideo = false;
			return false;
		}
	}

/*	DeleteBuffers();
	if (CreateBuffers(with_format) == B_OK)
	{
		BBufferConsumer::SetOutputBuffersFor(producer, mDestination, 
											mBuffers, (void *)&user_data, &change_tag, true);
	}
	else
	{
		ERROR("EMBeAnalogConsumerNode::Connected - COULDN'T CREATE BUFFERS\n");
		return B_ERROR;
	}
*/

	// Set time source for output node
/*	status = EMBeMediaUtility::GetRosterE()->SetTimeSourceFor(Node().node, timesourceNode.node);
	if (status < B_OK) {
		EMDebugger("Couldn't set TimeSource for video display!");
		return false;
	}

	// Set time source for video input
	status = EMBeMediaUtility::GetRosterE()->SetTimeSourceFor(m_spPhysicalInOutput -> node.node, timesourceNode.node);
	if (status < B_OK) {
		EMDebugger("Couldn't set TimeSource for video input!");
		return false;
	}
*/
	return true;
}

bool EMBeAnalogConsumerNode::DisconnectNode()
{
		status_t err;
		// disconnect
//		EMBeMediaUtility::GetRosterE() -> StopNode(TimeSource() -> Node(), 0);
//		EMBeMediaUtility::GetRosterE() -> StopNode(Node(), 0, true);
		//ANALOGBufferProducer::Disconnect
		err = EMBeMediaUtility::GetRosterE()->Disconnect(m_spPhysicalInOutput -> node.node, m_spPhysicalInOutput -> source, Node().node, m_to.destination);
		if (err < B_OK) {
			emerr << "EMBeAnalogConsumerNode: occured when mMediaRoster -> Disconnect" << err << endl;
		}
		EMBeMediaUtility::GetRosterE()->ReleaseNode(Node());
		if (err < B_OK) {
			emerr << "EMBeAnalogConsumerNode: occured when mMediaRoster->ReleaseNode(mConnection.consumer): " << err << endl;
		}

	return true;
}

bool EMBeAnalogConsumerNode::PrepareForRecording()
{
	//SKA DENNA RADEN VARA HÄR!?!?!?!?
	m_opMediaFile = NULL;
	m_vRenderVideo = false;

	m_opFamilyName = static_cast<string*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_RENDER_CODEC_FAMILY));
	m_opVideoCodecName = static_cast<string*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_RENDER_CODEC));
	m_opAudioCodecName = static_cast<string*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_RENDER_CODEC));
	int vQuality = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_RENDER_CODEC_QUALITY)));

	string oRenderFileName = EMBeMediaUtility::MakeVideoRecordingFileName(m_vRenderFileSequenceNumber++, ID());
	m_vpQuality = float(vQuality/100.0);

	if( *m_opFamilyName == "unknown")
		return true;
		
	int32 vCookie = 0;
	if(m_opMediaFile == NULL)	
	{
		entry_ref sRef;
		if(get_ref_for_path(oRenderFileName.c_str(), &sRef) != B_OK)
			EMDebugger("EMBeAnalogConsumerNode: Could not get ref to file");


		//GET THE REQUESTED FAMILY
		while(get_next_file_format(&vCookie, &m_sFileFormat) == B_OK)
		{
			if (strcmp(m_sFileFormat.pretty_name, m_opFamilyName -> c_str()) == 0)
			{
				break;
			}
		}
		
		//END GETTINg FAMILY
		m_opMediaFile = new BMediaFile(&sRef, &m_sFileFormat);
		if(m_opMediaFile -> InitCheck() != B_OK)
			EMDebugger("EMBeAnalogConsumerNode: Could not initialize BMediaFile");
	}

//	if(HaveAudioConnection() && HaveVideoConnection() && m_opVideoTrack == NULL && m_opMediaFile != NULL)
//	{	
		media_format sFormat;
		memcpy(&sFormat, &m_sFormat, sizeof(media_format));
		
		media_codec_info mci;
		vCookie = 0;
		media_format outfmt;
		
		while(get_next_encoder(&vCookie, &m_sFileFormat, &sFormat, &outfmt, &mci) == B_OK)
		{
			if(strcmp(mci.pretty_name, m_opVideoCodecName -> c_str()) == 0)
			{
				m_vRenderVideo = true;
				break;
			}
		}

		if(m_vRenderVideo)
		{	
			sFormat.u.raw_video.field_rate = m_sFormat.u.raw_video.field_rate;
			m_opVideoTrack = m_opMediaFile -> CreateTrack(&sFormat, &mci);
			if(m_opVideoTrack == NULL)
				EMDebugger("EMBeAnalogConsumerNode: Could not create video track in BMediaFile");	

			if(m_opVideoTrack -> InitCheck() != B_OK)
				EMDebugger("EMBeAnalogConsumerNode: Could not initialize video track in BMediaFile!");

			//Set quality
			if(m_opVideoTrack -> SetQuality(m_vpQuality) == B_OK)
				;//;//cout_commented_out_4_release<< "QUALITY WAS SET TO 0.5" << endl;
		}
	//}

	if((m_opVideoTrack != NULL) && ! (m_vReadyForWriting))
	{	
		if(m_opMediaFile -> CommitHeader() != B_OK)
			EMDebugger("EMBeAnalogConsumerNode: Could not commit header of BMediaFile");
		
		m_vReadyForWriting = true;
	}
	return true;
}

bool EMBeAnalogConsumerNode::CloseFile()
{
	m_vReadyForWriting = false;
	if(m_opMediaFile != NULL)
	{
		if(m_opMediaFile -> CloseFile() != B_OK)
			emerr << "EMBeAnalogConsumerNode: The media file could not be closed!" << endl;
		
		if(m_opVideoTrack != NULL)
		{
			m_opMediaFile -> ReleaseTrack(m_opVideoTrack);
			m_opVideoTrack = NULL;
		}

/*		if(m_opAudioTrack != NULL)
		{
			m_opMediaFile -> ReleaseTrack(m_opAudioTrack);
			m_opAudioTrack = NULL;
		}
*/
		delete m_opMediaFile;
		m_opMediaFile = NULL;
	}
	
	return true;
}

bool EMBeAnalogConsumerNode::ClearData()
{
	m_vRenderFileSequenceNumber = 0;
	return true;
}

bool EMBeAnalogConsumerNode::SaveData(EMProjectDataSaver* p_opSaver)
{
	p_opSaver -> SaveUInt32(m_vRenderFileSequenceNumber);
	return true;
}

bool EMBeAnalogConsumerNode::LoadData(EMProjectDataLoader* p_opLoader)
{
	m_vRenderFileSequenceNumber = p_opLoader -> LoadUInt32();
	return true;
}

//---------------------------------------------------------------

#endif
