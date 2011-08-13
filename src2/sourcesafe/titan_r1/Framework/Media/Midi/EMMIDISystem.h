#ifndef __EM_MIDI_SYSTEM
#define __EM_MIDI_SYSTEM

#include "EMListener.h"

class EMListenerRepository;

class EMMIDISystem : public EMListener
{
public:
	virtual ~EMMIDISystem();
	virtual bool InitCheckE() = 0;

	virtual bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage) = 0;

protected:
	EMMIDISystem();
	bool m_vMIDISystemIsInitialized;
};

#endif