/*******************************************************
* Creator: Richard Kronfalt
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_REALTIME_AUDIO_INPUT_DESCRIPTOR
#define __EM_BE_REALTIME_AUDIO_INPUT_DESCRIPTOR

#include "EMRealtimeInputDescriptor.h"

class EMMediaDataBuffer;
class EMMediaSignalMeter;
class EMProjectDataSaver;
class EMProjectDataLoader;

class EMBeRealtimeAudioInputDescriptor : public EMRealtimeInputDescriptor
{
public:
	EMBeRealtimeAudioInputDescriptor(const media_node* p_spMediaOutput);
	~EMBeRealtimeAudioInputDescriptor();
	void ReadBuffer(EMMediaDataBuffer* p_opBuffer, int64 p_vForFrame);
	bool InitCheckE();
	bool PrepareToPlayE();
	bool StartPreviewE();	
	void StartE();
	void StopE();
	bool StopPreviewE();
	bool ClearData();
	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool LoadData(EMProjectDataLoader* p_opLoader);
	
private:
	media_node m_sSystemInputNode;
	EMMediaSignalMeter* m_opSignalMeter;
	uint32 m_vTakeSequenceNumber;
	BBufferConsumer* m_opNode;
};

#endif

#endif
