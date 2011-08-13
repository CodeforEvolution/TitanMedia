/*******************************************************
* Creator: Richard Kronfält
* Portability: native
*-------------------------------------------------------
* Your average BBufferConsumer derivate. Can receive 
* buffers from 1 input, and sends them to a receiving
* EMBeAudioDiskSource object for conversion.
* 
* This class should be used to "lift" the BeOS-native 
* media buffers up to our own "EM-specific" level.
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_AUDIO_CONSUMER_NODE
#define __EM_BE_AUDIO_CONSUMER_NODE

#include "EMListener.h"
#include "EMMediaBufferSource.h"

#include <MediaKit.h>
#include <list>
#include <fstream>

class EMMediaDataBuffer;
class EMWaveFileWriter;

class EMBeAudioConsumerNode : public BBufferConsumer, public EMListener, public EMMediaBufferSource
{
public:
	EMBeAudioConsumerNode(media_output* p_spPhysicalInOutput);
	~EMBeAudioConsumerNode();
	status_t AcceptFormat(const media_destination& p_destination, media_format* p_format);
	BMediaAddOn* AddOn(int32* p_outInternalID) const;
	void BufferReceived(BBuffer* p_opBuffer);
	status_t Connected(const media_source& p_source, const media_destination& p_destination, const media_format& p_format, media_input* p_outInput);
	port_id ControlPort(void) const;
	void Disconnected(const media_source& p_producer, const media_destination& p_whichInput);
	void DisposeInputCookie(int32 p_cookie);
	status_t FormatChanged(const media_source& p_source, const media_destination& p_destination, int32 p_something, const media_format& p_format);
	status_t GetLatencyFor(const media_destination& p_forWhom, bigtime_t* p_outLatency, media_node_id* p_outTimeSource);
	status_t GetNextInput(int32* p_cookie, media_input* p_outInput);
	status_t HandleMessage(int32 message, const void * data, size_t size);
	bool Initialize();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void NodeRegistered(void);
	void Preroll(void);
	void ProducerDataStatus(const media_destination& p_destination, int32 p_status, bigtime_t p_atPerformanceTime);
	void Seek(bigtime_t media_time, bigtime_t performance_time);
	void SetTimeSource(BTimeSource * time_source);
	void Start(bigtime_t performance_time); 
	void Stop(bigtime_t performance_time, bool immediate); 
	static status_t ThreadEntry(void* p_upObject);
//	void ReadBuffer(EMMediaDataBuffer* p_opBuffer, int64 p_vForFrame);
	
	void CloseTargetFile();
	void SetTargetFile(string p_oFileName);
	
//	bool ClearData();
//	bool ClearData();
//	bool SaveData(EMProjectDataSaver* p_opSaver);
//	bool LoadData(EMProjectDataLoader* p_opLoader);
	bool Shutdown();
	
private:
	void Run();
	port_id	m_sControlPortID;
	media_input	m_sInput;
	thread_id m_vThread;
	media_output m_sPhysicalInOutput;
	media_format m_sPreferedInputFormat;
	sem_id m_vSemaphore;
	list<BBuffer*> m_oReceivedBuffers;
	EMWaveFileWriter* m_opWaveFile;
	
	bool m_vIsRecording;
	bool m_vTemp;
};

#endif

#endif
