#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_MIDI_SYSTEM
#define __EM_WIN_MIDI_SYSTEM

#include "EMMIDISystem.h"

#include "EMMIDIGlobals.h"
#include "EMRealtimeInputDescriptor.h"
#include "EMRealtimeOutputDescriptor.h"

class EMWinDXMIDIProducer;
class EMWinDXMIDIConsumer;

#include <list>
#include <Mtype.h>
//#include <uuids.h>
#include <dmusici.h>
#include <dmusicc.h>

class EMWinMIDISystem : public EMMIDISystem
{
public:
	static EMWinMIDISystem* Instance();
	static void Delete();
	~EMWinMIDISystem();
	
	bool InitCheckE();
	list<EMRealtimeInputDescriptor*>* GetMIDIInputs();
	list<EMRealtimeOutputDescriptor*>* GetMIDIOutputs();
	void StartMIDISystemE();
	
	//IDirectMusicPort8* SearchForMIDIDevice(uint32 p_vDeviceNum);
	bool CreatePerformanceE(IDirectMusicPerformance8** p_oppPerformance);
	bool CreatePortE(IDirectMusicPort8** p_oppPort, GUID p_uPortGUID);
	//bool CreatePerformanceE(IDirectMusicPerformance8** p_oppPerformance, IDirectMusicPort8** p_oppPort, uint32 p_vDeviceNumber);
	IDirectMusic8* GetDirectMusicObject() const;
	IReferenceClock* GetReferenceClock() const;

	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);

private:
	EMWinMIDISystem();
	static EMWinMIDISystem* m_opNativeInstance;
	IDirectMusic* m_opDirectMusic;
	IDirectMusic8* m_opDirectMusic8;
	IReferenceClock* m_opMasterMIDIClock;
	EMWinDXMIDIConsumer* m_opConsumer;
	EMWinDXMIDIProducer* m_opProducer;
};

#endif

#endif