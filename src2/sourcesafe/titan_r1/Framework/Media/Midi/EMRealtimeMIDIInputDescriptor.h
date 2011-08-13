#ifndef __EM_REALTIME_MIDI_INPUT_DESCRIPTOR
#define __EM_REALTIME_MIDI_INPUT_DESCRIPTOR

#include "EMGlobals.h"

#include "EMRealtimeInputDescriptor.h"
#include "EMMediaDataBuffer.h"

class EMRealtimeMIDIInputDescriptor : public EMRealtimeInputDescriptor
{
public:
	virtual ~EMRealtimeMIDIInputDescriptor();
	virtual bool PrepareToPlayE() = 0;
	virtual void StartE() = 0;
	virtual void StopE() = 0;
	virtual bool ClearData() = 0;
	virtual bool SaveData(EMProjectDataSaver* p_opSaver) = 0;
	virtual bool LoadData(EMProjectDataLoader* p_opLoader) = 0;
	virtual bool InitCheckE() = 0;
	virtual bool StartPreviewE() = 0;
	virtual bool StopPreviewE() = 0;

	virtual void SetActive(bool p_vFlag, int32 p_vOutputID) = 0;
	virtual bool IsActive() const = 0;

protected:
	EMRealtimeMIDIInputDescriptor();
private:
};

#endif