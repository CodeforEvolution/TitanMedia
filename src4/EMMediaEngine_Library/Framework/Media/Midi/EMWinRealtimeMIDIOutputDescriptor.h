#ifndef __EM_WIN_REALTIME_MIDI_OUTPUT_DESCRIPTOR
#define __EM_WIN_REALTIME_MIDI_OUTPUT_DESCRIPTOR

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMRealtimeMIDIOutputDescriptor.h"
#include "EMMediaDataBuffer.h"

class EMWinDXMIDIProducer;
class EMListenerRepository;
class EMMediaMIDIEvent;

class EMWinRealtimeMIDIOutputDescriptor : public EMRealtimeMIDIOutputDescriptor
{
public:
	EMWinRealtimeMIDIOutputDescriptor(const GUID* p_upMIDIPortGUID); //uint32 p_vNativeMIDIDeviceNumber);
	~EMWinRealtimeMIDIOutputDescriptor();

	bool PrepareToPlayE();
	void StartE();
	void StopE();
	bool ClearData();
	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool LoadData(EMProjectDataLoader* p_opLoader);
	bool InitCheckE();
	EMMediaDataBuffer* ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList);
	bool InitializeEventRepositoryE();
	bool UninitializeEventRepositoryE();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);

	void SetActive(bool p_vFlag);
	bool IsActive() const;
//	bool RouteEvent(EMMediaMIDIEvent* p_opEvent);
	EMWinDXMIDIProducer* GetProducer() const;
	bool RegisterAsClockMaster();
	bool UnregisterAsClockMaster();

private:
	GUID m_uMIDIPortGUID;
	EMWinDXMIDIProducer* m_opMIDIProducer;
};

#endif

#endif