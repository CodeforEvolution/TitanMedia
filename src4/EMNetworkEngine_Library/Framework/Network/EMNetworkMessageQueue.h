/*******************************************************
* Creator: Martin Johansson
* Portability: Native, very native
*-------------------------------------------------------
* 
* Martin Johansson, 01-08-01
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_NETWORK_MESSAGE_QUEUE
#define __EM_NETWORK_MESSAGE_QUEUE

#include <windows.h>
#include <list>

template<class T> class EMNetworkMessageQueue
{
public:
	EMNetworkMessageQueue();
	~EMNetworkMessageQueue();

	void AddMessage(T p_opMsg);
	T GetMessage();
protected:
	list<T> m_oQueue;

	HANDLE m_hMutex;
private:
};

#include "EMNetworkMessageQueue.cpp"

#endif
