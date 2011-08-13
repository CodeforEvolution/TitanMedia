/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_MESSAGE_PUMP
#define __EM_WIN_MESSAGE_PUMP

#include "EMThreadListener.h"

#include <list>
#include <windows.h>

using namespace std;

class EMSemaphore;
class EMThread;

class EMWinMessage
{
public:
	EMWinMessage() {};
	EMWinMessage(HWND p_opWindowHandle, uint16 p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo) :
	m_opWindowHandle(p_opWindowHandle),
	m_vMessage(p_vMessage),
	m_vParameterOne(p_vParameterOne),
	m_vParameterTwo(p_vParameterTwo)
	{
	}

public:
	HWND m_opWindowHandle;
	uint16 m_vMessage;
	WPARAM m_vParameterOne;
	LPARAM m_vParameterTwo;
};

class __declspec(dllexport) EMWinMessagePump : public EMThreadListener
{
public:
	EMWinMessagePump();
	~EMWinMessagePump();
	void AddMessage(HWND p_opWindowHandle, uint16 p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo);
	static EMWinMessagePump* Instance();
	void ThreadRun(EMThread* p_opThread);

private:
	list<EMWinMessage> m_oMessageQueue;
	EMWinMessage m_oMessageThreadCopy;
	static EMWinMessagePump* m_opInstance;
	EMSemaphore* m_opLockThreadSemaphore;
	EMSemaphore* m_opProtectDataSemaphore;
	EMThread* m_opThread;
};

#endif

#endif