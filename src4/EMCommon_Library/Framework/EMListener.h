/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_LISTENER
#define __EM_LISTENER

class EMListenerRepository;

class __declspec(dllexport) EMListener
{
public:
	virtual ~EMListener();
	virtual bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage) = 0; // Returns true if message is handled

protected:
	EMListener();
};

#endif
	