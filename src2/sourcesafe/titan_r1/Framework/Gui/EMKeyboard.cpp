#include "EMKeyboard.h"

#ifdef PLATFORM_BEOS
#include "EMBeKeyboard.h"
#endif
#ifdef PLATFORM_WINDOWS
#include "EMWinKeyboard.h"
#endif

#include "EMCommandRepository.h"
#include "EMKeyboardGlobals.h"
#include "EMMessages.h"
//#include "EMSemaphore.h"
//#include "EMThreadRepository.h"
#include "EMView.h"

#include <algorithm>

const char* KEYBOARD_THREAD_NAME = "Keyboard Thread";
const char* KEYBOARD_SEMAPHORE_NAME = "Keyboard Semaphore";

EMKeyboard* EMKeyboard::m_opInstance = NULL;

EMKeyboard::EMKeyboard() :
m_vProcessKey(false)
{
//	m_opSemaphore = EMSemaphore::CreateEMSemaphore(KEYBOARD_SEMAPHORE_NAME);
//	m_opSemaphore -> Acquire();
//	m_opThread = EMThreadRepository::Instance() -> CreateThread(KEYBOARD_THREAD_NAME);
//	m_opThread -> Start();
//	m_opThread -> AddListener(this);
}

EMKeyboard::~EMKeyboard()
{
//	delete m_opSemaphore;
//	EMThreadRepository::Instance() -> DeleteThread(KEYBOARD_THREAD_NAME);
}

// p_vKey must be lower-case
bool EMKeyboard::BindKey(char p_vKey, uint32 p_vModifiers, uint32 p_vCommandID)
{
	uint64 vKey = GenerateCodeKey(p_vKey, p_vModifiers);
//eo << "Key: " << p_vKey << ", modifiers: " <<  p_vModifiers << ", code: " << (int) vKey << ef;

	if(m_oKeyMappings.find(vKey) != m_oKeyMappings.end())
		return false; // This key is already bound
	if(!(EMCommandRepository::Instance() -> CommandExists(p_vCommandID)))
		return false; // There is no such command
	m_oKeyMappings[vKey] = p_vCommandID;
	return true;
}

bool EMKeyboard::EnableKeyBinding(uint32 p_vCommand, bool p_vEnabled)
{
	if(!p_vEnabled)
	{
		m_oDisabledKeyBindings.push_back(p_vCommand);
		m_oDisabledKeyBindings.unique();
	}
	else
		m_oDisabledKeyBindings.remove(p_vCommand);

	return true;
}

uint64 EMKeyboard::GenerateCodeKey(char p_vKey, uint32 p_vModifiers)
{
	uint64 vKey = p_vKey;
	uint32 vNewModifiers = 0;
	vKey = vKey << 32;
	if(p_vModifiers & EM_CONTROL_KEY)
		vNewModifiers = EM_CONTROL_KEY;
	if(p_vModifiers & EM_SHIFT_KEY)
		vNewModifiers |= EM_SHIFT_KEY;
	if(p_vModifiers & EM_COMMAND_KEY)
		vNewModifiers |= EM_COMMAND_KEY;
	vKey += vNewModifiers;
	return vKey;
}

EMKeyboard* EMKeyboard::Instance()
{
	if(m_opInstance != NULL)
		return m_opInstance;	
#ifdef PLATFORM_BEOS	
	m_opInstance = EM_new EMBeKeyboard();
#endif
#ifdef PLATFORM_WINDOWS
	m_opInstance = EM_new EMWinKeyboard();
#endif
	return m_opInstance;
}

// Should only be called in response to a EM_KEY_CHANGED message 
EMKey EMKeyboard::GetKey()
{
	return m_oKey;
}

// This method receives keys not handled directly by the original receiver and
// distributes them to interested listeners
// p_vKeyStatus is true for a pressed key and false for a released key
// returns true if anyone handles the key
// update: The return value no longer means anything
bool EMKeyboard::NotifyKey(EMGUIComponent* p_opOrigin, char p_vKey, bool p_vKeyStatus, uint32 p_vModifiers)
{
	//m_opSemaphore -> Acquire();
	m_oKey.m_opOrigin = p_opOrigin;
	m_oKey.m_vKey = p_vKey;
	m_oKey.m_vKeyStatus = p_vKeyStatus;
	m_oKey.m_vModifiers = p_vModifiers;
	m_vProcessKey = true;
//	m_opThread -> Start();

	if(m_oKey.m_vKeyStatus)
		Notify(EM_KEY_DOWN);
	else
		Notify(EM_KEY_UP);
	if(m_oKey.m_vKeyStatus) // Key is pressed
		TriggerCommand(m_oKey);
	else // Key is released
		EMCommandRepository::Instance() -> SetKeyReleased();	
	
	//m_opSemaphore -> Release();
	return true;
}

/*
void EMKeyboard::SetEnabled(bool p_vEnabled)
{
	if(p_vEnabled)
		EMThreadRepository::Instance() -> GetThread(KEYBOARD_THREAD_NAME) -> Resume();
	else
		EMThreadRepository::Instance() -> GetThread(KEYBOARD_THREAD_NAME) -> Suspend();
}

void EMKeyboard::ThreadRun(EMThread* p_opThread)
{
eo << "EMKeyboard::ThreadRun" << ef;
	m_opSemaphore -> Acquire();
	if(m_vProcessKey)
	{
		m_oKeyThreadCopy = m_oKey;
		m_vProcessKey = false;
		m_opSemaphore -> Release();
	}
	else
	{
		m_opSemaphore -> Release();
		return;
	}
	if(m_oKeyThreadCopy.m_vKeyStatus)
		Notify(EM_KEY_DOWN);
	else
		Notify(EM_KEY_UP);
	if(m_oKeyThreadCopy.m_vKeyStatus) // Key is pressed
		TriggerCommand(m_oKeyThreadCopy);
	else // Key is released
		EMCommandRepository::Instance() -> SetKeyReleased();

	m_opThread -> Sleep(10000);
}
*/

void EMKeyboard::TriggerCommand(EMKey p_oKey)
{
	uint64 vKey = GenerateCodeKey(p_oKey.m_vKey, p_oKey.m_vModifiers);

	if(m_oKeyMappings.find(vKey) != m_oKeyMappings.end())
		if(find(m_oDisabledKeyBindings.begin(), m_oDisabledKeyBindings.end(), m_oKeyMappings[vKey]) == m_oDisabledKeyBindings.end()) // If not disabled
			EMCommandRepository::Instance() -> ExecuteTriggeredCommand(m_oKeyMappings[vKey]);
}


