#include "EMBeMessageTranslator.h"

#ifdef PLATFORM_BEOS

#include "EMMessages.h"

#include <AppDefs.h>

uint32 EMBeMessageTranslator::TranslateMessage(uint32 p_vMessage)
{
	switch(p_vMessage)
	{
		case B_MOUSE_DOWN:
			return EM_MOUSE_DOWN;
		case B_MOUSE_UP:
			return EM_MOUSE_UP;
		case B_MOUSE_MOVED:
			return EM_MOUSE_MOVED;
		default:
			return p_vMessage;
	}
}
		
#endif
