/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_THREAD_REPOSITORY
#define __EM_THREAD_REPOSITORY

#include "EMThread.h"

#include <map>
#include <string>

const int32 MIN_THREAD_SLEEP_TIME = 5000;

class EMThreadRepository
{
public:
	virtual ~EMThreadRepository();
	EMThread* CreateThread(const char* p_vpName, int32 p_vPriority = EM_THREAD_MEDIUM_PRIORITY, int32 p_vSleepTime = MIN_THREAD_SLEEP_TIME);
	void DeleteThread(const char* p_vpName);
	EMThread* GetThread(const char* p_vpName);
	static EMThreadRepository* Instance();
	
protected:
	EMThreadRepository();

private:
	static EMThreadRepository* m_opInstance;
	map<string, EMThread*> m_oThreads;
};

#endif


