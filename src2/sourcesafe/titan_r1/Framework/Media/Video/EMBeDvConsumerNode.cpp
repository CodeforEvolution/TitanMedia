#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#include "EMBeDvConsumerNode.h"
#include "EMBeMediaUtility.h"

#include <app/Application.h>
#include <interface/Alert.h>
#include <interface/View.h>
#include <kernel/scheduler.h>
#include <media/Buffer.h>
#include <media/TimeSource.h>
#include <media/MediaRoster.h>
#include <media/BufferGroup.h>
#include <media/MediaDefs.h>
#include <support/StopWatch.h>
#include "EMMediaEngine.h"
#include "CommandIDs.h"

#include <stdio.h>

#define M1 ((double)1000000.0)
#define JITTER		20000

#define	FUNCTION	;//_commented_out_4_release_printf
#define ERROR		
#define PROGRESS	
#define LOOP		

#if DEBUG
	#define FPRINTF fprintf
#else
	static inline void FPRINTF(FILE*, const char*, ...) { }
#endif

EMBeDvConsumerNode::EMBeDvConsumerNode(media_output* p_spPhysicalInOutput)
	:	BMediaNode("DV Consumer Node"),
		BMediaEventLooper(),
		BBufferConsumer(B_MEDIA_RAW_VIDEO),
		EMMediaBufferSource(EM_TYPE_ENCODED_VIDEO),
		mConnectionActive(false),
		mMyLatency(20000),
		mRecording(false),
		m_spPhysicalInOutput(p_spPhysicalInOutput),
		m_vFrameCount(0)
{
	EMBeMediaUtility::push(this, "EMBeDvConsumerNode");

	m_opReceiver = NULL;
	mCurrrentBuffer = 0;
//	mAudioDecoder = NULL;
//	mVideoDecoder = NULL;

	AddNodeKind(B_BUFFER_CONSUMER);

	mIn.destination.port = ControlPort();
	mIn.destination.id = 0;
	mIn.source = media_source::null;

	mIn.format.type = B_MEDIA_ENCODED_VIDEO;
	mIn.format.u.encoded_video = media_encoded_video_format::wildcard;

	BMediaEventLooper::SetPriority(B_REAL_TIME_DISPLAY_PRIORITY);
	EMBeMediaUtility::GetRosterE() -> RegisterNode(this);
//	mVolume = new BVolume();
	mTimeCode = new BTimeCode();
}

EMBeDvConsumerNode::~EMBeDvConsumerNode()
{
;//cout_commented_out_4_release<<"1"<<endl;
	if (mRecording) {
		this->StopRecording();		
		this->FinishRecording();
	}
;//cout_commented_out_4_release<<"2"<<endl;
	BMediaEventLooper::Quit();

/*
	if (mVideoDecoder->Lock()) {
		mVideoDecoder->Quit();
	}
	mVideoDecoder = NULL;

	if (mAudioDecoder) {
		delete mAudioDecoder;
		mAudioDecoder = NULL;
	}
*/
	if (mTimeCode) {
		delete mTimeCode;
		mTimeCode = NULL;
	}
;//cout_commented_out_4_release<<"3"<<endl;
	if(m_opReceiver != NULL)
		delete m_opReceiver;
;//cout_commented_out_4_release<<"4"<<endl;
	
	BMediaRoster::Roster() -> UnregisterNode(this);
	EMBeMediaUtility::pop("EMBeDvConsumerNode");
;//cout_commented_out_4_release<<"5"<<endl;
}

BMediaAddOn *
EMBeDvConsumerNode::AddOn(long *cookie) const
{
/*
	FUNCTION("EMBeDvConsumerNode::AddOn\n");
	// do the right thing if we're ever used with an add-on
	*cookie = mInternalID;
	return mAddOn;
*/
	return NULL;
}
void EMBeDvConsumerNode::Lock()
{
	m_opReceiver -> LockPreviewWindow();
}

void EMBeDvConsumerNode::Unlock()
{
	m_opReceiver -> UnlockPreviewWindow();
}

bool
EMBeDvConsumerNode::IsRecording()
{
	return mRecording;
}


status_t
EMBeDvConsumerNode::buffer_flusher(void * data)
{
	EMBeDvConsumerNode * aFileWriter = (EMBeDvConsumerNode *)data;
	const void * ptr;
	int32 size = 0;
	off_t total = 0;

	while (aFileWriter->mRecording && ((size = aFileWriter->mFIFO->BeginGet(&ptr, aFileWriter->mBufferSize, 1000000LL)) > 0)) {
		if (0 > aFileWriter->mVideoTrack->WriteChunk(ptr, size, B_MEDIA_KEY_FRAME)) 
		{
			aFileWriter->StopRecording();
			be_app->PostMessage(B_QUIT_REQUESTED);
		}
		else 
		{
		}
		aFileWriter->mFIFO->EndGet();
		total += size;
		aFileWriter->mFrames++;
	}
	if (size == 0) 
	{
		//fprintf(stderr, "This probably means a 3 second timeout expired without any data to record.\n");
	}
	return B_OK;
}

float
EMBeDvConsumerNode::FreeSpace()
{
	float aFreeSpace = ((float)mVolume.FreeBytes() / (float)mVolume.Capacity());
	return aFreeSpace * 100;
}

void
EMBeDvConsumerNode::TimeDisplayText(char *text)
{
	mTimeCode->SetLinearFrames(mFrames);
}

status_t
EMBeDvConsumerNode::StartAudioPreview()
{
//	status_t	err;
/*
	if (mAudioDecoder) {
		err = mAudioDecoder->Start();
		if(err < B_OK) {
			;//_commented_out_4_release_printf("mAudioDecoder->Start() failed %s\n", strerror(err));
			return B_ERROR;
		}
	}
*/
	return B_OK;
}

status_t
EMBeDvConsumerNode::StopAudioPreview()
{
//	status_t	err;
/*
	if (mAudioDecoder) {
		err = mAudioDecoder->Stop();
		if(err < B_OK) {
			;//_commented_out_4_release_printf("mAudioDecoder->Start() failed %s\n", strerror(err));
			return B_ERROR;
		}
	}
*/
	return B_OK;
}

void EMBeDvConsumerNode::StartVideoDecoder()
{
/*	status_t err = EMBeMediaUtility::GetRosterE()->SetRunModeNode(Node(), B_RECORDING);
	if (err < B_OK) {
		EMDebugger("ERROR: MediaRoster->SetRunModeNode() !");
	}

	err = EMBeMediaUtility::GetRosterE()->StartNode(m_spPhysicalInOutput -> node, 0);
	if (err < B_OK) {
		EMDebugger("Couldn't start video input!");
	}

	err = EMBeMediaUtility::GetRosterE()->StartNode(Node(), 0);
	if (err < B_OK) {
		EMDebugger("Couldn't start video displayer!");
	}
*/

}
void
EMBeDvConsumerNode::Start(bigtime_t performance_time)
{

//START NODE
	EMBeMediaUtility::GetRosterE() -> StartNode(TimeSource() -> Node(), 0);

	//m_vIsRecording = true;

	status_t err = EMBeMediaUtility::GetRosterE()->SetRunModeNode(Node(), B_RECORDING);
	if (err < B_OK) {
		EMDebugger("MediaRoster->SetRunModeNode() : EMBeDvConsumerNode");
	}

	err = EMBeMediaUtility::GetRosterE()->StartNode(m_spPhysicalInOutput -> node, 0);
	if (err < B_OK) {
		EMDebugger("Couldn't start video input!");
	}

/*	err = EMBeMediaUtility::GetRosterE()->StartNode(Node(), 0);
	if (err < B_OK) {
		EMDebugger("Couldn't start video displayer!");
	}
*/


//	status_t	err;
//	BRect		aRect;

/*
	if (mVideoDecoder) {
		delete mVideoDecoder;
	}

	aRect = BRect(20.0, 20.0, 20.0 + 639.0, 20.0 + 479.0);
	mVideoDecoder = new PreviewWindow(aRect, "dvg: preview window");
	err = mVideoDecoder->SetTo(mIn.format);
	if(err < B_OK) {
		;//_commented_out_4_release_printf("mVideoDecoder->SetTo failed %s\n", strerror(err));
		return B_ERROR;
	}
*/

}

status_t
EMBeDvConsumerNode::StopVideoDecoder()
{
/*
	if (mVideoDecoder->Lock()) {
		mVideoDecoder->Quit();
		delete mVideoDecoder;
		mVideoDecoder = NULL;
	}
*/
	return B_OK;
}

status_t EMBeDvConsumerNode::SetFileName(string p_oFileName, string* format_family)
{
	m_oFilename = p_oFileName;
	mFormatFamily = format_family;
	return B_OK;
}

void EMBeDvConsumerNode::Stop(bigtime_t performance_time, bool immediate)
{

	BMediaEventLooper::Stop(performance_time, immediate);
	EMBeMediaUtility::GetRosterE() -> StopNode(m_spPhysicalInOutput -> node, 0);
	//m_vIsRecording = false;
//	EMBeMediaUtility::GetRosterE() -> StopNode(TimeSource() -> Node(), 0);
}

bool
EMBeDvConsumerNode::GetRef(entry_ref *outRef)
{
	;//_commented_out_4_release_printf("INFO: EMBeDvConsumerNode::GetRef(%s)\n", mMovieFileRef->name);

	if (outRef != NULL) {
		outRef->device = mMovieFileRef->device;
		outRef->directory = mMovieFileRef->directory;
		outRef->set_name(mMovieFileRef->name);
		return true;
	} else {
		return false;
	}
}

void
EMBeDvConsumerNode::NodeRegistered()
{

	BMediaEventLooper::Run();
}

status_t
EMBeDvConsumerNode::RequestCompleted(const media_request_info & info)
{
	return B_OK;
}

status_t
EMBeDvConsumerNode::HandleMessage(int32 message, const void * data, size_t size)
{
	status_t status = B_OK;

	if ((status = BMediaNode::HandleMessage(message, data, size)) != B_OK)
	{
		if ((status = BBufferConsumer::HandleMessage(message, data, size)) != B_OK)
		{
			BMediaNode::HandleBadMessage(message, data, size);
			status = B_ERROR;
		}
	}

	return status;
}

void
EMBeDvConsumerNode::BufferReceived(BBuffer * p_opBuffer)
{
	status_t err;

	if ((mVideoTrack) && (mRecording)) {
		m_vFrameCount++;
		char vpPercentage[40];
		sprintf(vpPercentage, "%d | Time:%d:%d:%d:%d", (int32) m_vFrameCount, (int32) ((m_vFrameCount / (int32)mIn.format.u.raw_video.field_rate)/3600)%60,(int32) ((m_vFrameCount / (int32)mIn.format.u.raw_video.field_rate)/60)%60,
														(int32)((m_vFrameCount / (int32)mIn.format.u.raw_video.field_rate))%60, (int32) (m_vFrameCount % (int32)mIn.format.u.raw_video.field_rate));
		string oString = string("Recording Frame: ") + vpPercentage;
//		EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_WRITE_STATUS_BAR_MESSAGE, &oString);	
		err = this->mFIFO->CopyNextBufferIn(p_opBuffer->Data(), p_opBuffer->Header()->size_used, 100000, false);
		if (err < (int32)p_opBuffer->Header()->size_used) {
			(new BAlert("", "Couldn't continue recording(Out of HD-space?)", "So Be It"))->Go();
			StopRecording();
			FinishRecording();			
		}
	}

	if(m_opReceiver == NULL)
	{
		m_opReceiver = new EMBeVideoDisplayer();
		SetTo(mIn.format);
	}
	
	if(m_opReceiver == NULL)
		EMDebugger("m_opReceiver is NULL!");

/*	if (BMediaEventLooper::RunState() == BMediaEventLooper::B_STOPPED)
	{
		p_opBuffer->Recycle();
		return;
	}
*/	
	if(p_opBuffer)
	{
		memcpy(mBuffer, p_opBuffer->Data(), p_opBuffer->Header()->size_used);
		mBufferHeader = p_opBuffer->Header();

		if ((mBuffer != NULL) && (mBitmap[mCurrrentBuffer] != NULL))
		{
			mBitmap[mCurrrentBuffer]->Lock();
			err = mBufferDecoder.DecodeBuffer(mBuffer, mBufferSize, mBitmap[mCurrrentBuffer]->Bits(),
											  &frame_count, mBufferHeader);
			if(err != B_NO_ERROR)
			{
				;//_commented_out_4_release_printf("DecodeBuffer (video) failed %s\n", strerror(err));
			}
		if(m_opReceiver -> LockPreviewWindow())
		{	
			m_opReceiver -> HandleNativeBuffer(mBitmap[mCurrrentBuffer]);
		}
		mCurrrentBuffer++;
		mCurrrentBuffer = mCurrrentBuffer % 2;
		}
		p_opBuffer -> Recycle();
	}

/*	if ((mVideoTrack) && (mRecording)) {
		err = this->mFIFO->CopyNextBufferIn(buffer->Data(), buffer->Header()->size_used, 100000, false);
		if (err < (int32)buffer->Header()->size_used) {
			fprintf(stderr, "CopyNextBufferIn(%p, %ld, B_INFINITE_TIMEOUT, false) failed with %ld.\n",
				buffer->Data(), buffer->Header()->size_used, err);
			fprintf(stderr, "error while recording: %s; bailing\n", strerror(err));
			(new BAlert("", "error while recording", "Quit"))->Go();
			be_app->PostMessage(B_QUIT_REQUESTED);
		}
	}

	if (mAudioDecoder) {
		if (mAudioDecoder->IsPlaying()) {
			mAudioDecoder->DecodeBuffer(buffer);
		}
	}

	if (mVideoDecoder) {
		if (mVideoDecoder->IsRunning()) {
			mVideoDecoder->DecodeBuffer(buffer);
		}
	}

	buffer->Recycle();
*/
}

void
EMBeDvConsumerNode::ProducerDataStatus(
	const media_destination &for_whom,
	int32 status,
	bigtime_t at_media_time)
{

	if (for_whom != mIn.destination)	
		return;
}

status_t
EMBeDvConsumerNode::Connected(
	const media_source & producer,
	const media_destination & where,
	const media_format & with_format,
	media_input * out_input)
{
	status_t	err = B_OK;
	
	mIn.source = producer;
	mIn.format = with_format;
	mIn.node = Node();
	sprintf(mIn.name, "EMBeDvConsumerNode");
	*out_input = mIn;

	if (mIn.format.u.encoded_video.output.field_rate == 25.0) {
		;//_commented_out_4_release_printf("B_TIMECODE_25\n");
		mTimeCode->SetType(B_TIMECODE_25);
	} else {
		;//_commented_out_4_release_printf("B_TIMECODE_30_DROP_2\n");
		mTimeCode->SetType(B_TIMECODE_30_DROP_2);
	}

	mBufferSize = mIn.format.u.encoded_video.frame_size;
	mFIFO = new BBlockFIFO(mBufferSize, 1, 32);

	err = mFIFO->InitCheck();
	if (err < B_OK) {
		;//_commented_out_4_release_printf("ERROR: failed to create mFIFO\n");
		return B_ERROR;
	}
/*
	BRect	aRect(20.0, 20.0, 20.0 + 639.0, 20.0 + 479.0);
	mVideoDecoder = new PreviewWindow(aRect, "dvg: preview window");
	err = mVideoDecoder->SetTo(mIn.format);
	if(err < B_OK) {
		;//_commented_out_4_release_printf("mVideoDecoder->SetTo failed %s\n", strerror(err));
		return B_ERROR;
	}
*/
/*
	mAudioDecoder = new PreviewAudio();
	err = mAudioDecoder->SetTo(with_format);
	if(err < B_OK) {
		;//_commented_out_4_release_printf("mAudioDecoder->SetTo failed %s\n", strerror(err));
		return B_ERROR;
	}
*/
	return B_OK;
}

status_t
EMBeDvConsumerNode::PrepareForRecording()
{
	int32					cookie;
	media_file_format		mfi;
	media_format			format = mIn.format;
	status_t				err = B_OK;

	// find the right file -format handler
	cookie = 0;
	while(get_next_file_format(&cookie, &mfi) == B_OK)
	{
		if (strcmp(mfi.pretty_name, mFormatFamily -> c_str()) == 0)
		{
			goto writer_found;
		}
	}
	;//_commented_out_4_release_printf("failed to find a file format handler !\n");
	return B_ERROR;

	// Create and init the mediaFile
writer_found:
	if((mfi.capabilities & 0x80) == 0)
	{
			(new BAlert("", "Format Family not valid for ouput chose another one", "So Be It"))->Go();
			//It's OK .. this error has been handled return B_OK(for now)
			return B_OK;
	}	

///
	m_oFilename = m_oFilename + string(".") + string(mfi.file_extension);
	entry_ref sEntry;
	get_ref_for_path(m_oFilename.c_str(), &sEntry);

	if(mMovieFileRef) 
	{
		delete mMovieFileRef;
	}
	mMovieFileRef = new entry_ref(sEntry);
	mVolume.SetTo(sEntry.device);

///


	mMediaFile = new BMediaFile(mMovieFileRef, &mfi);
	err = mMediaFile->InitCheck();
	if (err != B_OK) {
		;//_commented_out_4_release_printf("failed to properly init the output file... (%s)\n", strerror(err));
		delete mMediaFile;
		return B_ERROR;
	}

	mVideoTrack = mMediaFile->CreateTrack(&format);

	if(!mVideoTrack){
		;//_commented_out_4_release_printf("Failed to create video track\n");
		delete mMediaFile;
		return B_ERROR;
	}

	// Add the copyright and commit the header
//	mMediaFile->AddCopyright("created with dvg by kotobuki");
	mMediaFile->CommitHeader();

	return B_OK;
}

status_t
EMBeDvConsumerNode::StartRecording()
{
	if (mRecording) {
		return B_OK;
	}

	mFIFO->Reset();
	// start the thread
	mThread = spawn_thread(buffer_flusher, EM_THREAD_NAME_VIDEO_RECORDER, 100, this);
	if (mThread < B_OK) {
		return B_ERROR;
	}
	// set mRecording to true, and start recorder
	mRecording = true;
	status_t err = resume_thread(mThread);
	if (err < B_OK) {
		kill_thread(mThread);
		mThread = -1;
		(new BAlert("Some kind of error occured[123ABC]", strerror(err), "So Be It"))->Go();
		return B_ERROR;
	}

	return B_OK;
}

status_t
EMBeDvConsumerNode::StopRecording()
{
	mRecording = false;

	status_t status;
	mFIFO->CopyNextBufferIn(&status, 0, B_INFINITE_TIMEOUT, true);
	wait_for_thread(mThread, &status);

	mFrames = 0;
	m_vFrameCount = 0;
	return B_OK;
}

status_t
EMBeDvConsumerNode::FinishRecording()
{
	status_t	err;

	if (mMediaFile) {
		mVideoTrack -> Flush();
		err = mMediaFile -> CloseFile();

		if (err < B_OK) {
			EMDebugger("Couldn't close file for some reason");
		}

		delete mMediaFile;
		mMediaFile = NULL;
		mVideoTrack = NULL;

		return B_OK;
	} else {
		return B_ERROR;
	}
}

void
EMBeDvConsumerNode::Disconnected(
	const media_source & producer,
	const media_destination & where)
{

	if (where == mIn.destination && producer == mIn.source)
	{
		// disconnect the connection
		mIn.source = media_source::null;
		mConnectionActive = false;
	}
}

status_t
EMBeDvConsumerNode::AcceptFormat(
	const media_destination & dest,
	media_format * format)
{
	if (dest != mIn.destination)
	{
		emerr << "BAD DESTINATION : EMBeDvConsumerNode" << endl;
		return B_MEDIA_BAD_DESTINATION;	
	}
	
	if (format->type == B_MEDIA_NO_TYPE)
		format->type = B_MEDIA_ENCODED_VIDEO;
	
	if (format->type != B_MEDIA_ENCODED_VIDEO)
	{
		emerr << "BAD FORMAT : EMBeDvConsumerNode" << endl;
		return B_MEDIA_BAD_FORMAT;
	}

	char format_string[256];		
	string_for_format(*format, format_string, 256);

	return B_OK;
}

status_t
EMBeDvConsumerNode::GetNextInput(
	int32 * cookie,
	media_input * out_input)
{
	// custom build a destination for this connection
	// put connection number in id
	if (*cookie < 1)
	{
		mIn.node = Node();
		mIn.destination.id = *cookie;
		sprintf(mIn.name, "Video Consumer Node");
		*out_input = mIn;
		(*cookie)++;
		return B_OK;
	}
	else
	{
		//emerr << "BAD INDEX (naughty cookie): EMBeDvConsumerNode" << endl;
		return B_MEDIA_BAD_DESTINATION;
	}
}

void
EMBeDvConsumerNode::DisposeInputCookie(int32 /* cookie */)
{
	// do something here
}

status_t
EMBeDvConsumerNode::GetLatencyFor(
	const media_destination &for_whom,
	bigtime_t * out_latency,
	media_node_id * out_timesource)
{
/*	;//cout_commented_out_4_release<< "EMBeDvConsumerNode::GetLatencyFor" << endl;
	
	if (for_whom != mIn.destination)
		return B_MEDIA_BAD_DESTINATION;
	
	*out_latency = mMyLatency;
	*out_timesource = TimeSource()->ID();
*/
	return B_OK;
}

status_t
EMBeDvConsumerNode::FormatChanged(
	const media_source & producer,
	const media_destination & consumer, 
	int32 from_change_count,
	const media_format &format)
{
	if (consumer != mIn.destination)
		return B_MEDIA_BAD_DESTINATION;

	if (producer != mIn.source)
		return B_MEDIA_BAD_SOURCE;

	mIn.format = format;
	
	return B_OK;
}

void
EMBeDvConsumerNode::HandleEvent(
	const media_timed_event *event, 
	bigtime_t lateness,
	bool realTimeEvent)
{
	switch (event->type)
	{
		case BTimedEventQueue::B_START:
			break;

		case BTimedEventQueue::B_STOP:
			BMediaEventLooper::EventQueue()->FlushEvents(lateness, BTimedEventQueue::B_ALWAYS, true, BTimedEventQueue::B_HANDLE_BUFFER);
			break;

		case BTimedEventQueue::B_USER_EVENT:
			break;

		default:
			break;
	}			
}

status_t
EMBeDvConsumerNode::SetTo(const media_format &format)
{
	status_t err;

	// media decoder handling
	media_format encformat = format;
	media_format rawformat;

	err = mBufferDecoder.SetTo(&encformat);
	if(err < B_OK) {
		;//_commented_out_4_release_printf("FindDecoder failed %s\n", strerror(err));
		return B_ERROR;
	}

	// get a little bit of info about this guy
	media_codec_info decoderinfo;
	err = mBufferDecoder.GetDecoderInfo(&decoderinfo);
	if(err < B_OK) {
		;//_commented_out_4_release_printf("GetDecoderInfo failed %s\n", strerror(err));
		return B_ERROR;
	}
	;//_commented_out_4_release_printf("Found video decoder, %s\n", decoderinfo.pretty_name);

	rawformat.require_flags = 0;
	rawformat.deny_flags = B_MEDIA_MULTIPLE_BUFFERS
	                     | B_MEDIA_CONTIGUOUS_BUFFER
	                     | B_MEDIA_MAUI_UNDEFINED_FLAGS;

	rawformat.type = B_MEDIA_RAW_VIDEO;
	rawformat.u.raw_video = media_raw_video_format::wildcard;
	rawformat.u.raw_video.display.line_width = encformat.u.encoded_video.output.display.line_width;
	rawformat.u.raw_video.display.line_count = encformat.u.encoded_video.output.display.line_count;
	rawformat.u.raw_video.display.format = B_YCbCr422;

	uint32 line_width = rawformat.u.raw_video.display.line_width;
	uint32 line_count = rawformat.u.raw_video.display.line_count;

	mBitmapOverlay = true;

	for (int i=0; i <2; i++) {
		mBitmap[i] = new BBitmap(BRect(0, 0, line_width-1, line_count-1), B_BITMAP_CLEAR_TO_WHITE | B_BITMAP_IS_CONTIGUOUS | B_BITMAP_WILL_OVERLAY, B_YCbCr422);
		if ((mBitmap[i]->InitCheck() != B_OK) || !mBitmap[i]->IsValid()) {
			;//_commented_out_4_release_printf("Error: can't use overlay !\n");
			delete mBitmap[i];
			mBitmap[i] = NULL;

			mBitmapOverlay = false;
		}
	}

	if (mBitmapOverlay != true) {
		for (int i=0; i <2; i++) {
			if (mBitmap[i]) {
				delete mBitmap[i];
				mBitmap[i] = NULL;
			}

			mBitmap[i] = new BBitmap(BRect(0, 0, line_width-1, line_count-1), B_RGB16, false, true);
			rawformat.u.raw_video.display.format = B_RGB16;
		}
		//EMDebugger("Supposed to be overlay!?");
	}

	// negotiate the output format
	err = mBufferDecoder.SetOutputFormat(&rawformat);
	if(err < B_OK) {
		;//_commented_out_4_release_printf("OutputFormat failed %s\n", strerror(err));
		return B_ERROR;
	}
	if(rawformat.require_flags & (B_MEDIA_MAUI_UNDEFINED_FLAGS |
	                              B_MEDIA_MULTIPLE_BUFFERS |
                                  B_MEDIA_CONTIGUOUS_BUFFER)) {
		;//_commented_out_4_release_printf("OutputFormat not acceptable\n");
		return B_ERROR;
	}

//	rgb_color	aKeyColor;

/*	if (mBitmapOverlay) {
		err = mView->SetViewOverlay(mBitmap[0], &aKeyColor, B_FOLLOW_ALL, B_OVERLAY_FILTER_HORIZONTAL | B_OVERLAY_FILTER_VERTICAL);
		mView->SetViewColor(aKeyColor);
		mView->Invalidate();
	} else {
		mScreen = new BScreen(this);
	}
*/
	;//_commented_out_4_release_printf("size = %ld\n", format.u.encoded_video.frame_size);

	mBufferSize = format.u.encoded_video.frame_size;
	mBuffer = new char[mBufferSize];

//	this->Show();
//	mRunning = true;

	return B_OK;
}


bool EMBeDvConsumerNode::Initialize()
{
	status_t err = EMBeMediaUtility::GetRosterE() -> GetTimeSource(&timesourceNode);
	if (err < B_OK) {
		EMDebugger("Can't get TimeSource!");
		return false;
	}

	// find free input to video window
	int32 cnt = 0;
	err = EMBeMediaUtility::GetRosterE()->GetFreeInputsFor(Node(), &m_to, 1, &cnt, B_MEDIA_ENCODED_VIDEO);
	if (err < B_OK || cnt < 1) 
	{
		//;//cout_commented_out_4_release<< "The video-recording output is busy!?" << endl;
		return false;
	}

	// connect them
	media_format format;
	format.type = B_MEDIA_ENCODED_VIDEO;
	format.u.encoded_video = media_encoded_video_format::wildcard;

	err = EMBeMediaUtility::GetRosterE()->Connect(m_spPhysicalInOutput -> source, m_to.destination, &format, m_spPhysicalInOutput, &m_to);
	if (err < B_OK) 
	{
		emerr << "Connection failed : DvConsumerNode: " << strerror(err) << endl;
		return false;
	}

/*	mConnection.format = format;
	mConnection.source = m_from.source;
	mConnection.destination = m_to.destination;
*/
//	mConnected = true;

	// Set time source for output node
	err = EMBeMediaUtility::GetRosterE()->SetTimeSourceFor(Node().node, timesourceNode.node);
	if (err < B_OK) 
	{
		//;//cout_commented_out_4_release<< "Couldn't set TimeSource for video display : EMBeDvConsumerNode" << endl;
		return false;
	}

	// Set time source for video input
	err = EMBeMediaUtility::GetRosterE()->SetTimeSourceFor(m_spPhysicalInOutput -> node.node, timesourceNode.node);
	if (err < B_OK) 
	{
		//;//cout_commented_out_4_release<< "Couldn't set TimeSource for video input : EMBeDvConsumerNode" << endl;
		return false;
	}

}

bool EMBeDvConsumerNode::DisconnectNode()
{
		status_t err;
		// disconnect
//DVBufferProducer::Disconnect
		err = EMBeMediaUtility::GetRosterE() -> Disconnect(m_spPhysicalInOutput -> node.node, m_spPhysicalInOutput -> source, Node().node, m_to.destination);
		if (err < B_OK) 
		{
			//emerr << "Couldn't Disconnect Nodes : EMBeDvConsumerNode" << endl;
		}
	
//		;//cout_commented_out_4_release<< "Releasing producer node" << endl;
/*		EMBeMediaUtility::GetRosterE()->ReleaseNode(m_spPhysicalInOutput -> node);
		if (err < B_OK) {
			;//cout_commented_out_4_release<< "mMediaRoster->ReleaseNode(mConnection.producer)" << err << endl;
		}
*/

		EMBeMediaUtility::GetRosterE()->ReleaseNode(Node());
		if(err < B_OK) 
		{
			//emerr << "Couldn't release Node : EMBeDvConsumerNode" << endl;
		}

	return true;
}

#endif

