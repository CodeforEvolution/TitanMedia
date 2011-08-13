#include "EMGlobals.h"

#include "EMThread.h"
#include "EMThreadListener.h"

#ifdef PLATFORM_BEOS
#include "EMBeThread.h"
#endif

#ifdef PLATFORM_WINDOWS
#include "EMWinThread.h"
#endif

EMThread::EMThread(const char* p_vpName) :
m_oThreadName(p_vpName),
m_vIsAlive(false),
m_vShouldRun(false),
m_vDoNotModify(false)
{
}

EMThread::~EMThread()
{
}

void EMThread::AddListener(EMThreadListener* p_opListener)
{
	if(m_vDoNotModify)
		m_oScheduledAdditions.push_back(p_opListener);
	else
	{
		m_oListeners.push_back(p_opListener);
		m_oListeners.unique();
	}
}

EMThread* EMThread::CreateEMThread(const char* p_vpName, int32 p_vPriority, int32 p_vSleepTime)
{
#ifdef PLATFORM_BEOS
	return EM_new EMBeThread(p_vpName, p_vPriority, p_vSleepTime);
#endif
#ifdef PLATFORM_WINDOWS
	return EM_new EMWinThread(p_vpName, p_vPriority, p_vSleepTime);
#endif
}

bool EMThread::IsListening(EMThreadListener* p_opListener)
{
	list<EMThreadListener*>::iterator oIterator = m_oListeners.begin();
	while(oIterator != m_oListeners.end())
	{
		if((*oIterator) == p_opListener)
			return true;
		oIterator++;
	}
	return false;
}

bool EMThread::IsRunning() const
{
	return m_vShouldRun;
}

void EMThread::Notify()
{
	// Special case to allow deletion of this object from the receiver
	if(m_oListeners.size() == 1)
	{
		EMThreadListener* opListener = *(m_oListeners.begin());
		opListener -> ThreadRun(this);
	}
	else
	{
		m_vDoNotModify = true;
		list<EMThreadListener*>::iterator oIterator = m_oListeners.begin();
		while(oIterator != m_oListeners.end())
		{
			(*oIterator) -> ThreadRun(this);
			oIterator++;
		}
		m_vDoNotModify = false;
		if(m_oScheduledRemovals.size() > 0)
		{
			list<EMThreadListener*>::iterator oIterator = m_oScheduledRemovals.begin();
			while(oIterator != m_oScheduledRemovals.end())
			{
				m_oListeners.remove(*oIterator);
				oIterator++;
			}
			m_oScheduledRemovals.clear();
		}	
		if(m_oScheduledAdditions.size() > 0)
		{
			list<EMThreadListener*>::iterator oIterator = m_oScheduledAdditions.begin();
			while(oIterator != m_oScheduledAdditions.end())
			{
				m_oListeners.push_back(*oIterator);
				oIterator++;
			}
			m_oScheduledAdditions.clear();
		}	
	}
}

void EMThread::NotifyCreated()
{
	list<EMThreadListener*>::iterator oIterator = m_oListeners.begin();
	while(oIterator != m_oListeners.end())
	{
		(*oIterator) -> OnThreadCreated(this);
		oIterator++;
	}
}

void EMThread::NotifyKilled()
{
	list<EMThreadListener*>::iterator oIterator = m_oListeners.begin();
	while(oIterator != m_oListeners.end())
	{
		(*oIterator) -> OnThreadKilled(this);
		oIterator++;
	}
}

void EMThread::RemoveListener(EMThreadListener* p_opListener)
{
	if(m_vDoNotModify)
		m_oScheduledRemovals.push_back(p_opListener);
	else
		m_oListeners.remove(p_opListener);
}


