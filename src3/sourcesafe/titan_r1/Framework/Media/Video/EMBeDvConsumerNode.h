
#ifndef EM_BE_DV_CONSUMER_NODE
#define EM_BE_DV_CONSUMER_NODE

#include "EMBeVideoDisplayer.h"
#include "EMMediaBufferSource.h"
#include "BlockFIFO.h"

#include <Bitmap.h>
#include <Screen.h>
#include <View.h>
#include <Window.h>
#include <kernel/OS.h>
#include <MediaNode.h>
#include <BufferConsumer.h>
#include <TimedEventQueue.h>
#include <MediaEventLooper.h>
#include <MediaDecoder.h>
#include <MediaFile.h>
#include <MediaTrack.h>
#include <TimeCode.h>
#include <Entry.h>
#include <Volume.h>

//#include "PreviewWindow.h"
//#include "PreviewAudio.h"

class EMBeDvConsumerNode : public BMediaEventLooper, public BBufferConsumer, public EMMediaBufferSource
{
public:
	EMBeDvConsumerNode(media_output* p_spPhysicalInOutput);
	~EMBeDvConsumerNode();
	
//	BMediaNode methods
public:

	virtual	BMediaAddOn	*AddOn(long *cookie) const;

protected:

	virtual void		NodeRegistered();
	virtual	status_t 	RequestCompleted(
							const media_request_info & info);
							
	virtual	status_t 	HandleMessage(
							int32 message,
							const void * data,
							size_t size);

//  BMediaEventLooper methods
protected:
	virtual void		HandleEvent(
							const media_timed_event *event,
							bigtime_t lateness,
							bool realTimeEvent = false);
	
//	BBufferConsumer
public:
	
	virtual	status_t	AcceptFormat(
							const media_destination &dest,
							media_format * format);
	virtual	status_t	GetNextInput(
							int32 * cookie,
							media_input * out_input);
							
	virtual	void		DisposeInputCookie(
							int32 cookie);

	void 				Start(bigtime_t performance_time); 
	void 				Stop(bigtime_t performance_time, bool immediate); 
	bool 				DisconnectNode();
	void Lock();
	void Unlock();
	
protected:

	virtual	void		BufferReceived(
							BBuffer * p_opBuffer);
	
private:
	virtual	void		ProducerDataStatus(
							const media_destination &for_whom,
							int32 status,
							bigtime_t at_media_time);									
	virtual	status_t	GetLatencyFor(
							const media_destination &for_whom,
							bigtime_t * out_latency,
							media_node_id * out_id);	
	virtual	status_t	Connected(
							const media_source &producer,
							const media_destination &where,
							const media_format & with_format,
							media_input * out_input);							
	virtual	void		Disconnected(
							const media_source &producer,
							const media_destination &where);							
	virtual	status_t	FormatChanged(
							const media_source & producer,
							const media_destination & consumer, 
							int32 from_change_count,
							const media_format & format);

	status_t		SetTo(const media_format &format);


//	implementation
public:
	status_t 			SetFileName(string p_oFileName, string* format_family);
	status_t 			PrepareForRecording();
	status_t 			StartRecording();
	status_t 			StopRecording();
	status_t 			FinishRecording();

	status_t 			StartAudioPreview();
	status_t 			StopAudioPreview();
	void				StartVideoDecoder();
	bool				GetRef(entry_ref *outRef);
	void				TimeDisplayText(char *text);
	bool				IsRecording();
	float				FreeSpace();
	status_t			StopVideoDecoder();
	bool 				Initialize();



private:
	uint32				mInternalID;
	BMediaAddOn			*mAddOn;

	bool				mConnectionActive;
	media_input			mIn;
	media_destination	mDestination;
	bigtime_t			mMyLatency;

	BMediaFile      	*mMediaFile;
	BMediaTrack    		*mVideoTrack;

//	entry_ref       	mMovieFileRef;
	entry_ref       	*mMovieFileRef;
	string*				mFormatFamily;

	bool				mRecording;

//	int64				mCurrentFrame;
//	int64				mPreviousFrame;

//	PreviewWindow		*mVideoDecoder;
//	PreviewAudio		*mAudioDecoder;

	BBlockFIFO			*mFIFO;
	thread_id			mThread;
	static status_t		buffer_flusher(void * data);
	size_t				mBufferSize;
	BVolume				mVolume;
	BTimeCode			*mTimeCode;
	int32				mFrames;
	char				*mBuffer;
	media_header		*mBufferHeader;
	EMBeVideoDisplayer* m_opReceiver;
	BMediaBufferDecoder	mBufferDecoder;
	BBitmap				*mBitmap[2];
	bool				mBitmapOverlay;
	uint64				mCurrrentBuffer;
	int64				frame_count;		

//FOR DV CONSUMER NODE
	media_output*		m_spPhysicalInOutput;
	EMBeDvConsumerNode*	m_DvConsumerNode;
	media_node			timesourceNode;
	media_input		m_to;
	int32 m_vFrameCount;
	string m_oFilename;
//	media_node 			m_spConsumerNode;
/*	media_source source;
	media_destination destination;
	media_format format;
*/
	
};

#endif // DV_FILE_WRITER
