/*******************************************************
* Creator: Martin Rudefelt
* Component: none
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_KEYBOARD
#define __EM_KEYBOARD

#include "EMGUIGlobals.h"
#include "EMListenerRepository.h"
#include "EMThreadListener.h"

#include <list>
#include <map>

class EMGUIComponent;
class EMSemaphore;
class EMThread;

class EMKeyboard : public EMListenerRepository//, public EMThreadListener
{
public:
	virtual ~EMKeyboard();
	bool BindKey(char p_vKey, uint32 p_vModifiers, uint32 p_vCommandID);
	bool EnableKeyBinding(uint32 p_vCommand, bool p_vEnabled);
	virtual uint32 GetModifiers() = 0;
	static EMKeyboard* Instance();
	EMKey GetKey();
	bool NotifyKey(EMGUIComponent* p_opOrigin, char p_vKey, bool p_vKeyStatus, uint32 p_vModifiers);
	//void SetEnabled(bool p_vEnabled);
	//void ThreadRun(EMThread* p_opThread);
	
protected:
	EMKeyboard();

private:
	uint64 GenerateCodeKey(char p_vKey, uint32 p_vModifiers);
	void TriggerCommand(EMKey p_oKey);

private:
	map<uint64, uint32> m_oKeyMappings;
	static EMKeyboard* m_opInstance;
	EMKey m_oKey;
	//EMKey m_oKeyThreadCopy;
	//EMSemaphore* m_opSemaphore;
	//EMThread* m_opThread;
	bool m_vProcessKey;
	list<uint32> m_oDisabledKeyBindings;
};

#endif

	
	