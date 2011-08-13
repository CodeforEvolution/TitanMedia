/*******************************************************
* Creator: Jesper Svensson
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_WIN_MEDIA_SINGLETON_MUXER
#define __EM_WIN_MEDIA_SINGLETON_MUXER
#include "EMRealtimeEncoderDescriptor.h"

class EMWinRealtimeRenderToDiskOutputDescriptor;
#include "EMWinVideoShuffleOutputInterface.h"

#include <dshow.h>

class EMWinMediaSingletonMuxer
{
public:
	~EMWinMediaSingletonMuxer();
	EMWinMediaSingletonMuxer();
	static EMWinMediaSingletonMuxer* Instance();
	void CreateFileRendererFilter();
	void CreateFilterGraph();
	IPin* GetAMuxInPin();
	IGraphBuilder* GetFilterGraph();
	IPin* GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir);
	void SetVideoIsReady(bool p_vReadyState);
	void SetAudioIsReady(bool p_vReadyState);
	void SetRenderName(char* p_vpName);
	bool AllIsReady();
	bool AudioIsReady();
	bool VideoIsReady();
	int32 m_vAudioEncoderID;
	int32 m_vVideoEncoderID;
	EMMediaFamily m_vCurrentlyUsedFamily;
	EMRealtimeEncoderDescriptor* GetEncoderDescriptor(int32 p_vEncoderID);
	bool GetEncoderPropertyPages(int32 p_vEncoderID, bool p_vShow);
	bool GetFilterPropertyPages(int32 p_vEncoderID, bool p_vShow);
	bool SetCompressionQuality(double p_vQuality, int32 p_vVideoEncoderID = 0);
	void StoreVideoRealTimeRenderToDiskObject(EMWinRealtimeRenderToDiskOutputDescriptor* m_opObject);
	void StoreAudioRealTimeRenderToDiskObject(EMWinRealtimeRenderToDiskOutputDescriptor* m_opObject);
	EMWinRealtimeRenderToDiskOutputDescriptor* GetVideoRealTimeRenderToDiskObject();
	EMWinRealtimeRenderToDiskOutputDescriptor* GetAudioRealTimeRenderToDiskObject();
	char* GetCompressionCapabilities(int32 p_vEncoderID);
	bool TempLinkSourceWithEncoder(int32 p_vEncoderID);
	void TempDisLinkSourceWithEncoder();
	void CreateDummyFilterGraph();
	void SetCompressionCapabilityInfo(int32 p_vEncoderID, int32  p_vPropertyID);
	void SetCompressionCapability();
	bool ErrorHasOccured();
	void SetErrorHasOccured(bool p_vErrorStatus);
	string* GetRenderName();
private:
	IFileSinkFilter* m_opFileZinkInterface;
	static EMWinMediaSingletonMuxer* m_opInstance;
	IGraphBuilder* m_opGraphBuilder;
	IGraphBuilder* m_opDummyGraphBuilder;
	IBaseFilter* m_opAviMuxFilter;
	IBaseFilter* m_opFileWriterFilter;

	IPin* m_opMuxInPin;
	bool m_vVideoIsReady;
	bool m_vAudioIsReady;
	string m_oOutputFileName;
	LPVOID m_vpEncoderState;
	EMWinRealtimeRenderToDiskOutputDescriptor* m_opVideoRenderObject;
	EMWinRealtimeRenderToDiskOutputDescriptor* m_opAudioRenderObject;
	AUDIO_STREAM_CONFIG_CAPS m_opAudioConfigCap;
	VIDEO_STREAM_CONFIG_CAPS m_opVideoConfigCap;
	string* m_opAudioCompressionCapabilityArray;
	string* m_opVideoCompressionCapabilityArray;
	string m_vErrorString;
	int m_vpiCount;
	//Special attributes for the encoders
	IBaseFilter* m_vShuffleDummyFilter;
	EMWinVideoShuffleOutputInterface* m_opShuffleDummyFilterInterface;
	IPin* m_opEncoderDummyInPin;
	IPin* m_opEncoderDummyOutPin;
	IPin* m_opShuffleDummyFilterOutPin;
	IBaseFilter* m_opEncoderDummyFilter;
	EMMediaType m_vActiveDummyType;
	int32 m_vCompressionCapabilityEncoder;
	int32 m_vCompressionCapabilityID;
	bool m_vErrorStatus;
	//End of special attributes
};

#endif