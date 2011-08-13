#include "EMBeKeyboard.h"

#ifdef PLATFORM_BEOS

#include <InterfaceDefs.h>

// The 32-bit value returned here reflects the status of the modifier keys such as
// shift and num lock. The EM standard is identical to the BeOS standard. Other
// platforms will probably have to translate their values to conform to this standard
uint32 EMBeKeyboard::GetModifiers()
{
	return modifiers();
}

#endif


