/*******************************************************
* Creator: Richard Kronfalt
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_REALTIME_AUDIO_OUTPUT_DESCRIPTOR
#define __EM_WIN_REALTIME_AUDIO_OUTPUT_DESCRIPTOR

#include "EMRealtimeOutputDescriptor.h"
#include "EMMediaDataBuffer.h"
#include "EMListenerRepository.h"
#include "EMListener.h"
#include "EMWinNodeConnection.h"

#include "EMWinRealtimeRenderToDiskOutputDescriptor.h"
#include "EMOutputRepository.h"

#include <list>

class EMWinDirectSoundPlayback;
class EMProjectDataSaver;
class EMProjectDataLoader;
class EMWinMediaOutputFilter;

#ifdef _DEBUG
class EMWinDirectSoundPlayback;
#endif

class EMWinRealtimeAudioOutputDescriptor : public EMRealtimeOutputDescriptor, public EMListener
{
public:
	EMWinRealtimeAudioOutputDescriptor(const GUID* p_upGUID);
	virtual ~EMWinRealtimeAudioOutputDescriptor();
	bool InitCheckE();
	bool PrepareToPlayE();
	void StartE();
	void StopE();
	EMMediaDataBuffer* ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList);
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	bool ClearData();
	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool LoadData(EMProjectDataLoader* p_opLoader);
	bool RegisterAsClockMaster();
	bool UnregisterAsClockMaster();

#ifdef _DEBUG
	EMWinDirectSoundPlayback* GetDSPlayback();
#endif

private:
	EMRealtimeOutputDescriptor* GetAudioRenderToDiskDestination();
private:
	EMWinDirectSoundPlayback* m_opAudioOutput;
	GUID m_uDeviceGUID;
};

#endif

#endif
