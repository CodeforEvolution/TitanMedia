#ifndef __EM_WIN_REALTIME_MIDI_INPUT_DESCRIPTOR
#define __EM_WIN_REALTIME_MIDI_INPUT_DESCRIPTOR

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMRealtimeMIDIInputDescriptor.h"

class EMWinDXMIDIConsumer;

class EMWinRealtimeMIDIInputDescriptor : public EMRealtimeMIDIInputDescriptor
{
public:
	EMWinRealtimeMIDIInputDescriptor(const GUID* p_upMIDIPortGUID);
	~EMWinRealtimeMIDIInputDescriptor();

	bool PrepareToPlayE();
	void StartE();
	void StopE();
	bool ClearData();
	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool LoadData(EMProjectDataLoader* p_opLoader);
	bool InitCheckE();
	bool StartPreviewE();
	bool StopPreviewE();

	void SetActive(bool p_vFlag, int32 p_vOutputID);  
	bool IsActive() const;

private:
//	uint32 m_vNativeMIDIDeviceNumber;
	GUID m_uMIDIPortGUID;
	EMWinDXMIDIConsumer* m_opMIDIConsumer;
	;
};

#endif

#endif