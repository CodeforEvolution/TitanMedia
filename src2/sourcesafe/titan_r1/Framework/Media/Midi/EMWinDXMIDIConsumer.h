#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_DX_MIDI_CONSUMER
#define __EM_WIN_DX_MIDI_CONSUMER

#include "EMListener.h"
#include "EMListenerRepository.h"
#include "EMThreadListener.h"

#include <dmusicc.h>
#include <dmusici.h>
#include <map>
#include <objbase.h>
#include <streams.h>

class EMListenerRepository;
class EMMediaMIDIEvent;
class EMMediaMIDITrack;
class EMRealtimeMIDIInputDescriptor;
class EMRealtimeMIDIOutputDescriptor;
class EMRecordingMIDITrackRepository;
class EMSemaphore;
class EMThread;
class EMWinDXMIDIProducer;

class EMMediaMIDIHandleToPortMapping
{
public:
	EMMediaMIDIHandleToPortMapping() { m_uHandle = NULL; m_opPort = NULL; m_opInput = NULL; memset(m_oppTracks, 0, sizeof(EMMediaMIDITrack*) * 512); memset(m_oppOutputs, 0, sizeof(EMRealtimeMIDIOutputDescriptor*) * 512); };
	HANDLE m_uHandle;
	IDirectMusicPort8* m_opPort;
	EMRealtimeMIDIInputDescriptor* m_opInput;
	EMMediaMIDITrack* m_oppTracks[512];
	EMRealtimeMIDIOutputDescriptor* m_oppOutputs[512];
};

class EMWinDXMIDIConsumer : public EMThreadListener, public EMListener
{
public:
	~EMWinDXMIDIConsumer();
	static void Delete();
	void ClearData();
	bool AddPortE(EMRealtimeMIDIInputDescriptor* p_opInputDescriptor, void* p_upNativeData);
	bool InitCheckE();
	static EMWinDXMIDIConsumer* Instance();
	bool IsActiveMIDIRoutingInput() const;
	void OnThreadCreated(EMThread* p_opThread);
	void OnThreadKilled(EMThread* p_opThread);
	void SetActiveMIDIRoutingInput(bool p_vFlag, int32 p_vInputID);
	void StartE();
	bool StartRecordingE();
	bool StopRecordingE();
	void ThreadRun(EMThread* p_opThread);
	bool SetTrackAsListener(EMMediaMIDITrack* p_opTrack, EMRealtimeMIDIInputDescriptor* p_opToInput, EMRealtimeMIDIOutputDescriptor* p_opRouteToOutput);
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	//TODO: Implement a list of EMMIDIEventFilters...
	//void AddFilter(EMMediaMIDIFilter* p_opFilter);
	//void RemoveFilter(EMMediaMIDIFilter* p_opFilter);

	bool FilterEvent(uint8* p_vpData, uint8 p_vLength);

#ifdef DEBUG
	bool __IsAlive() const { return m_vConsumerThreadIsAlive; };
#endif

private:
	static EMWinDXMIDIConsumer* m_opInstance;
	EMWinDXMIDIConsumer();
	EMThread* m_opThread;
	IDirectMusicPerformance8* m_opDirectMusicPerformance;
	IDirectMusicBuffer8* m_opIDirectMusicBuffer8;
	EMSemaphore* m_opRecordingSemaphore;
	bool m_vIsCurrentlyRecording;
	bool m_vShouldRun;
	REFERENCE_TIME m_uStartReferenceTime;
	REFERENCE_TIME m_vPrfTmStart;
	int64 m_vSngTmStart;
	bool m_vIsActiveMIDIInputDevice;
	EMWinDXMIDIProducer* m_opMIDIOutputProducer;
	bool m_vIsInitialized;
	EMRealtimeMIDIInputDescriptor* m_opLastReceiver;
	uint64 m_vLastSongTmEventTime;
	uint64 m_vLastLengthBytes;
	void* m_vpLastData;
	EMMediaMIDIHandleToPortMapping m_oPortsMapping[512];
	HANDLE m_uEvents[512];
	int16 m_vNumEvents;
	HANDLE m_uShutdownSemaphore;
	list<IDirectMusicPort8*> m_oFailedPortsToRemove;

	bool m_vConsumerThreadIsAlive;
};

#endif

#endif