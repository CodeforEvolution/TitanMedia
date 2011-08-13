/*******************************************************
* Creator: Martin Rudefelt
* Component: none
* Portability: non-native
*-------------------------------------------------------
* Contains a native code section
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_THREAD
#define __EM_THREAD

#include "EMSemaphore.h"

#include <list>
#include <string>

class __declspec(dllexport) EMThreadListener;

// Thread priorities are between 0 and 120 where 120 is the highest priority
const int32 EM_THREAD_IDLE_PRIORITY = 1;
const int32 EM_THREAD_LOW_PRIORITY = 5;
const int32 EM_THREAD_MEDIUM_PRIORITY = 15;
const int32 EM_THREAD_HIGH_PRIORITY = 90;
const int32 EM_THREAD_REALTIME_PRIORITY = 110;
const int32 EM_THREAD_HIGH_REALTIME_PRIORITY = 120;

class __declspec(dllexport) EMThread
{
public:
	virtual ~EMThread();
	void AddListener(EMThreadListener* p_opListener);
	static EMThread* CreateEMThread(const char* p_vpName, int32 p_vPriority = EM_THREAD_MEDIUM_PRIORITY, int32 p_vSleepTime = 1000);
	bool IsListening(EMThreadListener* p_opListener);
	bool IsRunning() const;
	virtual void Kill(bool p_vCalledFromThreadRun = false) = 0;
	void Notify();
	void NotifyCreated();
	void NotifyKilled();
	void RemoveListener(EMThreadListener* p_opListener);
	virtual void SetPriority(int32 p_vPriority) = 0;
	virtual void Sleep(int64 p_vMicroSeconds) = 0;
	virtual void Start() = 0;
	virtual void StartSuspended() = 0;
	virtual void Suspend(bool p_vCalledFromThreadRun = false) = 0;
	virtual void Resume() = 0;
	virtual void WaitForThread() = 0;

protected:
	EMThread(const char* p_vpName);

protected:
	string m_oThreadName;
	bool m_vIsAlive;
	bool m_vShouldRun;
	int32 m_vSleepTime;

private:
	list<EMThreadListener*> m_oListeners; // Does not own its listeners
	list<EMThreadListener*> m_oScheduledAdditions;
	list<EMThreadListener*> m_oScheduledRemovals;
	bool m_vDoNotModify;
};

#endif