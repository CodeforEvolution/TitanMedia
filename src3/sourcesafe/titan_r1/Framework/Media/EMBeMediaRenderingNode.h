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

#ifndef __EM_BE_MEDIA_RENDERING_NODE
#define __EM_BE_MEDIA_RENDERING_NODE

#include "EMListener.h"
#include "EMBeVideoDisplayer.h"
#include <MediaKit.h>
#include <list>

class EMMediaDataBuffer;
class EMWaveFileWriter;
class EMVideoBitmapScaler;
class EMProjectDataLoader;
class EMProjectDataSaver;
class EMMediaFormat;
const int32 EM_AUDIO_INPUT = 0;
const int32 EM_VIDEO_INPUT = 1;

class EMBeMediaRenderingNode : public BBufferConsumer, public EMListener
{
public:
	static EMBeMediaRenderingNode* Instance();
	~EMBeMediaRenderingNode();
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
	
	bool PrepareFile();
	bool CloseFile();
	
	bool HaveAudioConnection() const;
	bool HaveVideoConnection() const;
	
	bool ClearData();
	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool LoadData(EMProjectDataLoader* p_opLoader);
	
private:
	static EMBeMediaRenderingNode* m_opInstance;
	EMBeMediaRenderingNode();
	void Run();
	port_id	m_sControlPortID;
	thread_id m_vThread;
	media_input m_sInputs[2];
	media_format m_sPreferedInputFormat[2];
	
	bool m_vShouldRender;
	BMediaFile* m_opMediaFile;
	BMediaTrack* m_opAudioTrack;
	BMediaTrack* m_opVideoTrack;
	
	int64 vVideoFrameCount;
	int64 vVideoKeyFrameRate;
	
	sem_id m_vSemaphore;
	EMVideoBitmapScaler* m_opVideoScaler;
	EMBeVideoDisplayer* m_opReceiver;

	media_file_format m_sFileFormat;
	
	int32 m_vLocalHeight;
	int32 m_vLocalWidth;
	BBitmap* m_opPreviewingImage;
	
	media_format m_sAudioFormat;
	media_format m_sVideoFormat;
	
	bool m_vReadyForWriting;
	
	string* m_opFamilyName;
	string* m_opVideoCodecName;
	string* m_opAudioCodecName;
	float m_vpQuality;
	bool m_vRenderVideo;
	bool m_vRenderAudio;
	float m_vVideoFrameRate;
	uint32 m_vRenderFileSequenceNumber;
	EMMediaFormat* m_oDesiredFormat;
	
};

#endif

#endif
