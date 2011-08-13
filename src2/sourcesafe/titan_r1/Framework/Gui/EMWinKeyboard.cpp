#include "EMWinKeyboard.h"

#ifdef PLATFORM_WINDOWS

#include "CommandIDs.h" // FIXME: Application class in the framework
#include "EMKeyboardGlobals.h"

#include <windows.h>
/*
bool EMWinKeyboard::BindKey(char p_vKey, uint32 p_vModifiers, uint32 p_vCommandID)
{
	return EMKeyboard::BindKey(p_vKey, p_vModifiers, p_vCommandID);
}
*/

// The 32-bit value returned here reflects the status of the modifier keys such as
// shift and num lock. The EM standard is identical to the BeOS standard. Other
// platforms will probably have to translate their values to conform to this standard
uint32 EMWinKeyboard::GetModifiers()
{
/*
eo << GetKeyState(VK_SHIFT) << ", " << (GetKeyState(VK_SHIFT) & 0x80) << ef;
eo << GetKeyState(VK_CONTROL) << ", " << (GetKeyState(VK_CONTROL) & 0x80) << ef;
eo << GetKeyState(VK_CAPITAL) << ", " << (GetKeyState(VK_MENU) & 0x80) << ef;
*/
	uint32 vModifiers(0);
	if((GetKeyState(VK_SHIFT) & 0x80) > 0)
		vModifiers |= EM_SHIFT_KEY;
	if((GetKeyState(VK_CONTROL) & 0x80) > 0)
		vModifiers |= EM_CONTROL_KEY;
	if((GetKeyState(VK_MENU) & 0x80) > 0)
		vModifiers |= EM_COMMAND_KEY;
	if(GetKeyState(VK_CAPITAL))
		vModifiers |= EM_CAPS_LOCK;
	if(GetKeyState(VK_NUMLOCK))
		vModifiers |= EM_NUM_LOCK;
	if(GetKeyState(VK_SCROLL))
		vModifiers |= EM_SCROLL_LOCK;
	return vModifiers;
}

#endif

