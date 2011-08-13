#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#include "EMBeVideoConsumerNode.h"
#include "EMBeBufferRepository.h"
#include "EMBeMediaUtility.h"

#include <MediaKit.h>
#include <OS.h>
#include <StorageKit.h>

EMBeVideoConsumerNode* EMBeVideoConsumerNode::m_opInstance = NULL;

EMBeVideoConsumerNode* EMBeVideoConsumerNode::Instance()
{
	if(m_opInstance == NULL)
		m_opInstance = new EMBeVideoConsumerNode();
	return m_opInstance;
}

void EMBeVideoConsumerNode::Delete()
{
	delete m_opInstance;
	m_opInstance = NULL;
}

EMBeVideoConsumerNode::EMBeVideoConsumerNode() 
	: 	BMediaNode("NativeVideoReader"), 
		BBufferConsumer(B_MEDIA_RAW_VIDEO),
		m_spBitmap(NULL),
		m_opHistoryBuffer(NULL),
		m_opBufferForDestructorIfQuit(NULL),
		m_spBuffers(NULL),
		m_sControlPortID(0),
		m_vTimeout(0),
		m_opReceiver(NULL),
		m_vLastBufferToReceive(false),
		m_vLastBufferToReceivePrepare(false),
		m_vBuffersOutInTheOpen(0)
{
	m_vpName = "EMBeVideoConsumerNode";
/*	m_sInput.source = media_source::null;
	m_sInput.destination = media_destination::null;
*/
	
	string oVideoName(EM_THREAD_NAME_VIDEO_CONSUMER);
	strcpy(m_sInput.name, oVideoName.c_str());

	m_sThread = spawn_thread(ThreadEntry, EM_THREAD_NAME_VIDEO_CONSUMER, 110, this);
	resume_thread(m_sThread);
	status_t err = BMediaRoster::Roster() -> RegisterNode(this);
	EMBeBufferRepository:: Instance() -> AddListener(this);
	if(err != B_OK)
		EMDebugger("ERROR! Could not register video consumer node!");
	EMBeMediaUtility::push(this, "EMBeVideoConsumerNode");
}

EMBeVideoConsumerNode::~EMBeVideoConsumerNode()
{
//	EMDebugger("Destructor of video consumer node!");
	EMBeBufferRepository:: Instance() -> RemoveListener(this);
	BMediaRoster::Roster() -> UnregisterNode(this);
	status_t vErrorCode = B_OK;
	write_port(m_sControlPortID, EM_PORT_MESSAGE_SHUTDOWN, 0, 0);
	while(wait_for_thread(m_sThread, &vErrorCode) == B_INTERRUPTED)
		snooze(1000);
	delete_port(m_sControlPortID);
	EMBeMediaUtility::pop("EMBeVideoConsumerNode");
	delete m_opReceiver;
}

status_t EMBeVideoConsumerNode::AcceptFormat(const media_destination& p_destination, media_format* p_format)
{
	if(p_destination != m_sInput.destination)
		return B_MEDIA_BAD_DESTINATION;
	
	if(p_format->type <= 0) 
	{
		p_format->type = B_MEDIA_RAW_VIDEO;
		p_format->u.raw_video = media_raw_video_format::wildcard;
	}
	else if(p_format->type != B_MEDIA_RAW_VIDEO) 
	{
		p_format->type = B_MEDIA_RAW_VIDEO;
		p_format->u.raw_video = media_raw_video_format::wildcard;
		return B_MEDIA_BAD_FORMAT;
	}
	return B_OK;
}

BMediaAddOn* EMBeVideoConsumerNode::AddOn(int32* p_outInternalID) const
{
	if (p_outInternalID) *p_outInternalID = 0;
	return NULL;
}

void EMBeVideoConsumerNode::ChangeToNewVideoBufferSize()
{
/*	if(m_opHistoryBuffer != NULL)
	{
		if(m_opReceiver -> LockPreviewWindow())
		{
			//m_opHistoryBuffer -> Recycle();
			//m_opHistoryBuffer = NULL;
			m_vLastBufferToReceive = true;
			m_opReceiver -> UnlockPreviewWindow();
		}
		else
		{
			EMDebugger("Fault in LOCKING PreviewWindow 1");
		}

	}
*/
//	m_spBitmap = EMBeBufferRepository::Instance() -> GetVideoBitmaps();
//	if(m_opHistoryBuffer != NULL)
//	{
//		m_opHistoryBuffer -> Recycle();
//		m_opHistoryBuffer = NULL;
//	} else
//		m_spBitmap = EMBeBufferRepository::Instance() -> GetVideoBitmaps();

	m_vLastBufferToReceivePrepare = true;
}

void EMBeVideoConsumerNode::BufferReceived(BBuffer* p_opBuffer)
{
	m_vBuffersOutInTheOpen++;
	if(m_vLastBufferToReceivePrepare)
	{
		m_opEmergencyBuffer = p_opBuffer;
		m_vLastBufferToReceive = true;
	}
		
	if(m_opReceiver == NULL)
		m_opReceiver = new EMBeVideoDisplayer();

	if(m_opReceiver -> LockPreviewWindow())
	{
		if(! m_vLastBufferToReceive || m_opHistoryBuffer == NULL)
		{
			if(! m_vLastBufferToReceive)
			{
				m_opReceiver -> HandleNativeBuffer(m_spBitmap[p_opBuffer -> Flags()]);
			}
			else
			{
				m_opReceiver -> UnlockPreviewWindow();
				p_opBuffer -> Recycle();
				m_vBuffersOutInTheOpen--;
				m_spBitmap = EMBeBufferRepository::Instance() -> GetVideoBitmaps();
			}
		}

		if(m_opHistoryBuffer != NULL)
		{
			if(! m_vLastBufferToReceive)
			{
				m_opHistoryBuffer -> Recycle();
				m_opHistoryBuffer = NULL;
				m_vBuffersOutInTheOpen--;
				m_opHistoryBuffer = p_opBuffer;
				p_opBuffer = NULL;
			}
			else
			{
				m_opHistoryBuffer -> Recycle();
				m_vBuffersOutInTheOpen--;
				p_opBuffer -> Recycle();
				m_vBuffersOutInTheOpen--;
				m_opHistoryBuffer = NULL;
				//m_spBitmap = EMBeBufferRepository::Instance() -> GetVideoBitmaps();
				//EMDebugger("Pandoras Case");
				m_opReceiver -> UnlockPreviewWindow();
			}
		}
		else
		{
			//m_vLastBufferToReceive = false;
		}
	}
	else
	{
		emerr << "WARNING! VideoConsumer couldn't get preview window's lock! Recycling BBuffer!" << endl;
		p_opBuffer = NULL;
	}

	if(! m_vLastBufferToReceive)
	{
		if(m_opHistoryBuffer == NULL)
		{
			m_opHistoryBuffer = p_opBuffer;
			p_opBuffer = NULL;
		}
	}
	else
	{
		m_vLastBufferToReceive = false;
		m_vLastBufferToReceivePrepare = false;
	}
}

void EMBeVideoConsumerNode::LockWindow()
{
	if(m_opReceiver != NULL)
		m_opReceiver -> LockPreviewWindow();
	else
		EMDebugger("ERROR! Preview window is NULL, so can't call LockPreviewWindow!");
}

void EMBeVideoConsumerNode::UnlockWindow()
{
	if(m_opReceiver != NULL)
		m_opReceiver -> UnlockPreviewWindow();
	else
		EMDebugger("ERROR! Preview window is NULL, so can't call UnlockPreviewWindow!");
}

status_t EMBeVideoConsumerNode::Connected(const media_source& p_source, const media_destination& p_destination, const media_format& p_format, media_input* p_outInput)
{
	//Delete Buffers just in case
//	DeleteBuffers();	
	//	Only accept connection requests when we're not already connected.
	if (m_sInput.source != media_source::null) 
	{
		return B_MEDIA_BAD_DESTINATION;
	}
	//	Only accept connection requests on the one-and-only available input.
	if (p_destination != m_sInput.destination) 
	{
		return B_MEDIA_BAD_DESTINATION;
	}
	//	Other than that, we accept pretty much anything. The format has been
	//	pre-cleared through AcceptFormat(), and we accept any format anyway.
	m_sInput.source = p_source;
	m_sInput.format = p_format;
	//	Tell whomever is interested that there's now a connection.
	//	This is the most important line -- return our connection information
	//	to the world so it can use it!
	*p_outInput = m_sInput;
//	uint32 user_data = 0;
//	int32 change_tag = 1;	

	if(! EMBeBufferRepository::Instance() -> InitializeVideoE(0, 2, &m_sInput.format))
		EMDebugger("ERROR! Could not initialize video buffer repository!");
	m_spBitmap = EMBeBufferRepository::Instance() -> GetVideoBitmaps();
	
	EMBeBufferRepository:: Instance() -> AddListener(this);
	
//	if(m_opHistoryBuffer != NULL)
//	{
//		m_opHistoryBuffer -> Recycle();
//		m_opHistoryBuffer = NULL;
//	}
	
	return B_OK;


/*	if(m_sInput.source != media_source::null)
	{
0		EMDebug("EMBeVideoConsumerNode::Connected() - ERROR! We can only accept one connection! It's allready in use!!");
		return B_MEDIA_BAD_SOURCE;
	}
	if(p_destination != m_sInput.destination)
	{
		EMDebug("EMBeVideoConsumerNode::Connected() - ERROR! Destination is not an input into this node!");
		return B_MEDIA_BAD_DESTINATION;
	}
	m_sInput.source = p_source;
	m_sInput.format = p_format;
	*p_outInput = m_sInput;
	return B_OK;
*/
}

port_id EMBeVideoConsumerNode::ControlPort(void) const
{
	return m_sControlPortID;
}

status_t
EMBeVideoConsumerNode::CreateBuffers(const media_format & with_format)
{
	return B_OK;
}

void
EMBeVideoConsumerNode::DeleteBuffers()
{
	if (m_spBuffers)
	{
		delete m_spBuffers;
		m_spBuffers = NULL;
		
		for (int32 j = 0; j < EMBeBufferRepository::Instance() -> GetNumVideoBitmaps(); j++)
			if (m_spBitmap[j] -> IsValid())
			{
				delete m_spBitmap[j];
				m_spBitmap[j] = NULL;
			}
	}
}

void EMBeVideoConsumerNode::Disconnected(const media_source& p_sSource, const media_destination& p_sDestination) 
{
	if(p_sSource != m_sInput.source) 
		return;
	if(p_sDestination != m_sInput.destination) 
		return;
	m_sInput.source = media_source::null;
}

void EMBeVideoConsumerNode::DisposeInputCookie(int32 p_cookie)
{
}

status_t EMBeVideoConsumerNode::FormatChanged(const media_source& p_source, const media_destination& p_destination, int32 p_something, const media_format& p_format)
{
	return B_OK;
}

status_t EMBeVideoConsumerNode::GetLatencyFor(const media_destination& p_forWhom, bigtime_t* p_outLatency, media_node_id* p_outTimeSource)
{
	if(p_forWhom != m_sInput.destination)
		return B_MEDIA_BAD_DESTINATION;
	
	*p_outLatency = 40000LL; //FIX THIS!
	*p_outTimeSource = TimeSource()->ID();

	return B_OK;
}


status_t EMBeVideoConsumerNode::GetNextInput(int32* p_cookie, media_input* p_outInput)
{


	if(*p_cookie == 0) 
	{
		if (m_sInput.source == media_source::null) 
		{		
			m_sInput.format.type = B_MEDIA_RAW_VIDEO;
			m_sInput.format.u.raw_video = media_raw_video_format::wildcard;
			//m_sInput.format.u.raw_video.format = 4;
			m_sInput.node = Node();
			m_sInput.destination.port = ControlPort();
			m_sInput.destination.id = Node().node;
			
		}			
		*p_outInput = m_sInput;
		*p_cookie = 1;
		return B_OK;
	}

	*p_outInput = m_sInput;
	*p_cookie = 1;
	
	return B_BAD_INDEX;
}

status_t EMBeVideoConsumerNode::GetNodeAttributes(media_node_attribute* p_outAttributes, size_t inMaxCount)
{
	return B_OK;
}


status_t EMBeVideoConsumerNode::HandleMessage(int32 message, const void * data, size_t size)
{
	if((BBufferConsumer::HandleMessage(message, data, size) < 0) && (BMediaNode::HandleMessage(message, data, size) < 0))
	{
		HandleBadMessage(message, data, size);
		return B_ERROR;
	}
	return B_OK;
}

void EMBeVideoConsumerNode::NodeRegistered(void)
{
}


void EMBeVideoConsumerNode::Preroll(void)
{
}

void EMBeVideoConsumerNode::Seek(bigtime_t media_time, bigtime_t performance_time)
{
}


//Maybe not neccessary
void EMBeVideoConsumerNode::ServiceThread()
{
	//Begin by creating a new port that we can listen to, and set the node's service port to that ID...
	m_sControlPortID = create_port(10, m_vpName);
	if(m_sControlPortID == B_BAD_VALUE || m_sControlPortID == B_NO_MORE_PORTS)
	{
		emerr << "ERROR! EMBeVideoConsumer::ServiceThread() - Could not create port... Bad port value, or no more ports!" << endl;
	}
	else
	{
		//Set my single input's port to the control port ID we just reserved...
		m_sInput.destination.port = m_sControlPortID;
		m_sInput.destination.id = 1;
		
		char * msg = new char[B_MEDIA_MESSAGE_SIZE];

		while(true)
		{
			bigtime_t timeout = Timeout();
			int32 code = 0;
			ssize_t size = read_port_etc(ControlPort(), &code, msg, B_MEDIA_MESSAGE_SIZE, B_TIMEOUT, timeout);
			if(size >= 0)
			{
				//Did we get a shut-down-message (from the destuctor) ?
				if(code == EM_PORT_MESSAGE_SHUTDOWN) 
				{	//Ok, we got a "must-quit-now" message (sent to the port from my 
					//own destructor, but using a different thread)....
					break;
				}
				else
				{	//Ok, handle the message.... 
					//timeoutCount = 0;
					HandleMessage(code, msg, size);
				}
			}
			else if(size == B_TIMED_OUT)
			{
				//Timed out! ... We're probably a little too fast for the producer!
			}
			else if(size == B_BAD_PORT_ID)
			{
				emerr << "ERROR! EMBeVideoConsumer::ServiceThread() - Bad port id! Aborting!" << endl;
				break;
			}
			else
			{ 	//Bad read!
				emerr << "ERROR! EMBeVideoConsumer::ServiceThread() - Bad port read! Quiting immediately!!" << endl;
				break;
			}
		} //End infinite loop!
	}
}


void EMBeVideoConsumerNode::SetReceiver(EMBeVideoDisplayer* p_opReceiver)
{
	m_opReceiver = p_opReceiver;
}

void EMBeVideoConsumerNode::SetRunMode(run_mode p_mode)
{
}

void EMBeVideoConsumerNode::SetTimeSource(BTimeSource* time_source)
{
}

void EMBeVideoConsumerNode::Start(bigtime_t performance_time)
{
}

void EMBeVideoConsumerNode::Stop(bigtime_t performance_time, bool immediate)
{
}


//Maybe not neccessary
status_t EMBeVideoConsumerNode::ThreadEntry(void* obj)
{
	((EMBeVideoConsumerNode*) obj)->ServiceThread();
	return 0;
}

bigtime_t EMBeVideoConsumerNode::Timeout( void )
{
	m_vTimeout = (m_vTimeout < 1000000) ? 1000000 : m_vTimeout * 2;
	return m_vTimeout;
}

void EMBeVideoConsumerNode::TimeWarp(bigtime_t at_real_time, bigtime_t to_performance_time)
{
}

void EMBeVideoConsumerNode::ProducerDataStatus(const media_destination& p_destination, int32 p_status, bigtime_t p_atPerformanceTime) 
{
}


//Unimplemented but derrived
//void EMBeVideoConsumerNode::DisposeInputCookie(int32 p_cookie)  {}
//status_t EMBeVideoConsumerNode::FormatChanged(const media_source& p_source, const media_destination& p_destination, int32 p_something, const media_format& p_format) { return B_OK; }
//status_t EMBeVideoConsumerNode::GetNodeAttributes(media_node_attribute* p_outAttributes, size_t inMaxCount) { return B_OK; }
//void EMBeVideoConsumerNode::NodeRegistered(void)  {}
//void EMBeVideoConsumerNode::Preroll() {}
//void EMBeVideoConsumerNode::ProducerDataStatus(const media_destination& p_destination, int32 p_status, bigtime_t p_atPerformanceTime) {}
//void EMBeVideoConsumerNode::SetRunMode(run_mode p_mode) {}
//void EMBeVideoConsumerNode::Seek(bigtime_t media_time, bigtime_t performance_time) {}
//void EMBeVideoConsumerNode::SetTimeSource(BTimeSource * time_source) {}
//void EMBeVideoConsumerNode::Start(bigtime_t performance_time) {}
//void EMBeVideoConsumerNode::Stop(bigtime_t performance_time, bool immediate) {}
//void EMBeVideoConsumerNode::TimeWarp(bigtime_t at_real_time, bigtime_t to_performance_time) {}
void EMBeVideoConsumerNode::RecycleHistoryBuffer()
{
	if(! m_vLastBufferToReceive)
	{
		if(m_opHistoryBuffer != NULL)
		{
//			if(m_opReceiver -> LockPreviewWindow())
//			{
				m_opHistoryBuffer -> Recycle();				
				m_opHistoryBuffer = NULL;
				m_vBuffersOutInTheOpen--;
//				m_opReceiver -> UnlockPreviewWindow();
//			}
//			else
//			{
//				EMDebugger("Fault in Locking PreviewWindow 2");
//			}
		}
	}
	/*if(m_vLastBufferToReceivePrepare)
	{
		m_opEmergencyBuffer -> Recycle();
	}
	*/
}
bool EMBeVideoConsumerNode::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
		case EM_MESSAGE_BUFFER_SIZE_CHANGED:
//			EMDebugger("Chhange buffer size message received in video consumer!");
			ChangeToNewVideoBufferSize();
			break;
		case EM_MESSAGE_BUFFER_DELETION:
//			EMDebugger("Reccle history buffer message received in video consumer!");
			RecycleHistoryBuffer();
			break;
		default:
			break;
	};
	
	return true;
}

bool EMBeVideoConsumerNode::ClearData()
{
//	EMDebugger("Cleardata in video consumer");

	if(m_opHistoryBuffer != NULL)
		m_opHistoryBuffer -> Recycle();
	m_opHistoryBuffer = NULL;
	return true;
}

#endif

