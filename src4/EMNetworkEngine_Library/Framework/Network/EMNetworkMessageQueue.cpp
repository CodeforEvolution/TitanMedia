#include "EMNetworkMessageQueue.h"

#ifndef __EM_NETWORK_MESSAGE_QUEUE_CPP
#define __EM_NETWORK_MESSAGE_QUEUE_CPP

template<class T> EMNetworkMessageQueue<T>::EMNetworkMessageQueue()
{
	m_hMutex = CreateMutex(NULL, false, NULL);

	if(m_hMutex == NULL)
		exit(-1);
}

template<class T> EMNetworkMessageQueue<T>::~EMNetworkMessageQueue()
{
	CloseHandle(m_hMutex);
}

template<class T> void EMNetworkMessageQueue<T>::AddMessage(T p_opMsg)
{
	WaitForSingleObject(m_hMutex, INFINITE);
	m_oQueue.push_back(p_opMsg);
	ReleaseMutex(m_hMutex);
}

template<class T> T EMNetworkMessageQueue<T>::GetMessage()
{
	T opMsg;

	WaitForSingleObject(m_hMutex, INFINITE);
	opMsg = m_oQueue.front();
	m_oQueue.pop_front();
	ReleaseMutex(m_hMutex);

	return opMsg;
}

#endif


