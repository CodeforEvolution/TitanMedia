/*******************************************************
* Creator: Richard Kronfalt
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_REALTIME_AUDIO_INPUT_DESCRIPTOR
#define __EM_WIN_REALTIME_AUDIO_INPUT_DESCRIPTOR

#include "EMRealtimeInputDescriptor.h"
#include "EMWinNodeConnection.h"

class EMMediaDataBuffer;
class EMMediaSignalMeter;
class EMProjectDataSaver;
class EMProjectDataLoader;

struct IBaseFilter;

class EMWinRealtimeAudioInputDescriptor : public EMRealtimeInputDescriptor
{
public:
	EMWinRealtimeAudioInputDescriptor(IBaseFilter* p_opSourceFilter);
	~EMWinRealtimeAudioInputDescriptor();
	bool InitCheckE();
	bool PrepareToPlayE();
	bool StartPreviewE();	
	void StartE();
	void StopE();
	bool StopPreviewE();
	bool ClearData();
	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool LoadData(EMProjectDataLoader* p_opLoader);

	void SetArmed(bool p_vShouldArm);

	EMMediaSignalMeter* GetSignalMeter();
	
private:
	EMMediaSignalMeter* m_opSignalMeter;
	uint32 m_vTakeSequenceNumber;
	EMWinNodeConnection m_oConnection;
	bool m_vIsHookedUp;
	bool m_vIsArmed;
};

#endif

#endif
