#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_DX_MIDI_PRODUCER
#define __EM_WIN_DX_MIDI_PRODUCER

#include "EMListener.h"
#include "EMMediaMIDIEventListener.h"
#include "EMThreadListener.h"

#include <map>
#include <objbase.h>
#include <streams.h>
#include <dmusici.h>
#include <dmusicc.h>

class EMListenerRepository;
class EMMediaMIDIEvent;
class EMMediaMIDIEventRepository;
class EMMediaTimer;
class EMRealtimeMIDIInputDescriptor;
class EMRealtimeMIDIOutputDescriptor;
class EMSemaphore;
class EMThread;

class EMWinDXMIDIProducer : public EMThreadListener, public EMListener//, public EMMediaMIDIEventListener
{
public:
	static EMWinDXMIDIProducer* Instance();
	static void Delete();

	~EMWinDXMIDIProducer();

	bool AddPortE(EMRealtimeMIDIOutputDescriptor* p_opOutputDescriptor, void* p_upNativeData);
	void ClearData();
	void Flush();
	bool InitCheckE();

	//	DMUS_PMSG* MakeMIDIMessage(uint64 p_vStartTime, bool p_vIsNote);
//	void MakeNoteOnMessage(DMUS_PMSG* p_upMsg, EMMediaMIDIEvent* p_opEvent, IDirectMusicPort8* p_opPort);
//	void MakeStatusMessage(DMUS_PMSG* p_upMsg, EMMediaMIDIEvent* p_opEvent, IDirectMusicPort8* p_opPort);

	void OnThreadCreated(EMThread* p_opThread);
	void OnThreadKilled(EMThread* p_opThread);
	bool RemovePortE(EMRealtimeMIDIOutputDescriptor* p_opTrack); //??
	HRESULT SendNote(DMUS_PMSG* p_upMessage);
	void SetEventRepository(EMMediaMIDIEventRepository* p_opRepository);
	void StartE();
	void StartPlaybackE();
	void StopPlaybackE();
	void ThreadRun(EMThread* p_opThread);
	bool DeliverMIDIEvent(EMMediaMIDIEvent* p_opEvent);
	void SetActiveMIDIRoutingOutputChannel(uint32 p_vMIDIChannel);
	uint32 GetActiveMIDIRoutingOutputChannel() const;
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void BroadcastMIDIMessage(uint64 p_vLengthBytes, void* p_vpData);
	void NotesOff(); 
	bool SendEvent(EMRealtimeMIDIOutputDescriptor* p_opReceiver, uint64 p_vSngTmEventTime, uint64 p_vLengthBytes, void* p_vpData, uint32 p_vMIDIChannel);
	bool SendEvent(EMRealtimeMIDIOutputDescriptor* p_opReceiver, uint64 p_vSngTmEventTime, uint64 p_vLengthBytes, void* p_vpData);
	bool SendEvent(IDirectMusicPort8* p_opReceiver, uint64 p_vSngTmEventTime, uint64 p_vLengthBytes, void* p_vpData, uint32 p_vMIDIChannel);

	REFERENCE_TIME SongTimeToReferenceTime(int64 p_vSongTime);
	DMUS_PMSG* ConvertToDirectMusicMessage(EMMediaMIDIEvent* p_opEvent, IDirectMusicPort8* p_opDestinationPort);

private:
	EMWinDXMIDIProducer();
	static EMWinDXMIDIProducer* m_opInstance;
	
	EMThread* m_opThread;
	IDirectMusicPerformance8* m_opDirectMusicPerformance;
	IDirectMusicPort8* m_opPort;

	EMSemaphore* m_opRunningSemaphore;
	EMSemaphore* m_opRoutingProtection;

	REFERENCE_TIME m_vRefTmStart;
	int64 m_vSngTmStart;

	//For MIDI event routing...
	uint32 m_vActiveRoutingOutputChannel;

	EMMediaMIDIEvent* m_opEvent;
	int64 m_vSystemTimeWhenStarted;

	map<EMRealtimeMIDIOutputDescriptor*, IDirectMusicPort8*> m_oPorts;
	map<EMRealtimeMIDIInputDescriptor*, EMRealtimeMIDIOutputDescriptor*> m_oMappings;
	map<IDirectMusicPort8*, int32> m_oPChannels;
	list<IDirectMusicPort8*> m_oPortList;

	bool m_vIsInitialized;
	int32 m_vPChannelCount;

	EMMediaMIDIEventRepository* m_opMIDIEventRepository;
	EMMediaTimer* m_opMediaTimer;

	EMRealtimeMIDIInputDescriptor* m_opActiveInput;
	EMRealtimeMIDIOutputDescriptor* m_opActiveOutput;

	HANDLE m_uAbortSignal;
	list<IDirectMusicPort8*> m_oFailedPortsToRemove;

	EMMediaMIDIEvent* m_opNextEventToDeliver;
	int64 m_vNextEventTime;

	map<EMRealtimeMIDIOutputDescriptor*, bool*> m_oNoteStatus;
    IDirectMusicGraph* m_opDirectMusicGraph;

	map<IDirectMusicPort8*, bool*> m_oOns;
};

#endif

#endif