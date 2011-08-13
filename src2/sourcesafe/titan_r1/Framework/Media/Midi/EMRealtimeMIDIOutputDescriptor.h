#ifndef __EM_REALTIME_MIDI_OUTPUT_DESCRIPTOR
#define __EM_REALTIME_MIDI_OUTPUT_DESCRIPTOR

#include "EMGlobals.h"

#include "EMRealtimeOutputDescriptor.h"
#include "EMMediaDataBuffer.h"
#include "EMListener.h"
#include "EMListenerRepository.h"

class EMMediaMIDIEvent;

class EMRealtimeMIDIOutputDescriptor : public EMRealtimeOutputDescriptor, public EMListener
{
public:
	virtual ~EMRealtimeMIDIOutputDescriptor();
	virtual bool PrepareToPlayE() = 0;
	virtual void StartE() = 0;
	virtual void StopE() = 0;
	virtual bool ClearData() = 0;
	virtual bool SaveData(EMProjectDataSaver* p_opSaver) = 0;
	virtual bool LoadData(EMProjectDataLoader* p_opLoader) = 0;
	virtual bool InitCheckE() = 0;
	virtual EMMediaDataBuffer* ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList) = 0;
	virtual bool InitializeEventRepositoryE() = 0;
	virtual bool UninitializeEventRepositoryE() = 0;
	virtual bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage) = 0;
	virtual void SetActive(bool p_vFlag) = 0;
	virtual bool IsActive() const = 0;
//	virtual bool RouteEvent(EMMediaMIDIEvent* p_opEvent) = 0;

protected:
	EMRealtimeMIDIOutputDescriptor();
private:
};

#endif