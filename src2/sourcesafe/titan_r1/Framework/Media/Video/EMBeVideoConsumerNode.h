/*******************************************************
* Creator: Jesper Svensson
* Portability: native
*-------------------------------------------------------
* 
* This class should be used to "lift" the BeOS-native 
* media buffers up to our own "EM-specific" level.
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_VIDEO_CONSUMER_NODE
#define __EM_BE_VIDEO_CONSUMER_NODE

//#include "EMThread.h"
//#include "EMThreadListener.h"

#include <BufferConsumer.h>
#include <iostream>
#include <InterfaceKit.h>
#include "EMBeVideoDisplayer.h"
#include "EMBeBufferRepository.h"
#include "EMListener.h"

class EMBeVideoConsumerNode : public BBufferConsumer, public EMListener
{
public:
	virtual ~EMBeVideoConsumerNode();
	static EMBeVideoConsumerNode* Instance();
	static void Delete();
	status_t AcceptFormat(const media_destination& p_destination, media_format* p_format);
	BMediaAddOn* AddOn(int32* p_outInternalID) const;
	void BufferReceived(BBuffer* p_opBuffer);
	status_t Connected(const media_source& p_source, const media_destination& p_destination, const media_format& p_format, media_input* p_outInput);
	virtual port_id ControlPort(void) const;
	void Disconnected(const media_source& p_producer, const media_destination& p_whichInput);
	void DisposeInputCookie(int32 p_cookie);
	status_t FormatChanged(const media_source& p_source, const media_destination& p_destination, int32 p_something, const media_format& p_format);
	status_t GetLatencyFor(const media_destination& p_forWhom, bigtime_t* p_outLatency, media_node_id* p_outTimeSource);
	status_t GetNextInput(int32* p_cookie, media_input* p_outInput);
	status_t GetNodeAttributes(media_node_attribute* p_outAttributes, size_t inMaxCount);
	virtual	status_t HandleMessage(int32 message, const void * data, size_t size);
	void NodeRegistered(void);
	virtual	void Preroll(void);
	void ProducerDataStatus(const media_destination& p_destination, int32 p_status, bigtime_t p_atPerformanceTime);
	void RecycleHistoryBuffer();
	virtual	void Seek(bigtime_t media_time, bigtime_t performance_time);
	void SetReceiver(EMBeVideoDisplayer* p_opReceiver);
	void SetRunMode(run_mode p_mode);
	virtual	void SetTimeSource(BTimeSource * time_source);
	virtual void Start(bigtime_t performance_time); 
	virtual void Stop(bigtime_t performance_time, bool immediate); 
	bigtime_t Timeout(void);
	virtual	void TimeWarp(bigtime_t at_real_time, bigtime_t to_performance_time);
	void ChangeToNewVideoBufferSize();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);

	bool ClearData();
	void LockWindow();
	void UnlockWindow();
	
private:
	status_t CreateBuffers(const media_format & with_format);
	void DeleteBuffers();
	void ServiceThread();
	static status_t ThreadEntry(void* obj);

private:
	EMBeVideoConsumerNode();
	static EMBeVideoConsumerNode* m_opInstance;

	port_id	m_sControlPortID;
	media_destination m_sDestination;
	media_input	m_sInput;
	BBitmap** m_spBitmap;
	BBufferGroup *m_spBuffers;
	thread_id m_sThread;
	char* m_vpName;
	bigtime_t m_vTimeout;
	EMBeVideoDisplayer* m_opReceiver;
	BBuffer* m_opHistoryBuffer;
	BBuffer* m_opEmergencyBuffer;
	BBuffer* m_opBufferForDestructorIfQuit;
	bool m_vLastBufferToReceive;
	bool m_vLastBufferToReceivePrepare;
	int m_vBuffersOutInTheOpen;
};

#endif

#endif
