#include "EMThreadRepository.h"

#include "EMThread.h"

//#include <algorithm>
//#include <memory>

EMThreadRepository* EMThreadRepository::m_opInstance = NULL;

EMThreadRepository::EMThreadRepository()
{
}

EMThreadRepository::~EMThreadRepository()
{
	map<string, EMThread*>::iterator oIterator = m_oThreads.begin();
	while(oIterator != m_oThreads.end())
	{
		delete oIterator -> second;
		oIterator++;
	}
eo << "EMThreadRepository::~EMThreadRepository completed" << ef;
}

EMThread* EMThreadRepository::CreateThread(const char* p_vpName, int32 p_vPriority, int32 p_vSleepTime)
{
	if(m_oThreads.find(p_vpName) != m_oThreads.end())
	{
		;//cout_commented_out_4_release << "Thread already exists, size: " << m_oThreads.size() << endl;
		return NULL;
	}
	EMThread* opThread;
	opThread = EMThread::CreateEMThread(p_vpName, p_vPriority, p_vSleepTime);
	m_oThreads[string(p_vpName)] = opThread;
;//cout_commented_out_4_release << "Thread created" << endl;
	return opThread;
}

void EMThreadRepository::DeleteThread(const char* p_vpName)
{
	if(m_oThreads.find(string(p_vpName)) == m_oThreads.end())
	{
		EMDebugger("EMThreadRepository::DeleteThread: Could not find thread");
		return;
	}
	EMThread* opThread = m_oThreads[string(p_vpName)];	
	m_oThreads.erase(string(p_vpName));
	delete opThread;
}

EMThread* EMThreadRepository::GetThread(const char* p_vpName)
{
	map<string, EMThread*>::iterator oIterator;
	if((oIterator = m_oThreads.find(string(p_vpName))) == m_oThreads.end())
	{
		;//cout_commented_out_4_release << "Thread \"" << p_vpName << "\" was not found" << endl;
		return NULL;
	}
	return oIterator -> second;
/*
	map<string, EMThread*>::iterator oIterator;
	oIterator = m_oThreads.find(string(p_vpName));
	if(oIterator == m_oThreads.end())
		return NULL;
	return oIterator -> second;
*/
}

EMThreadRepository* EMThreadRepository::Instance()
{
	if(m_opInstance != NULL)
		return m_opInstance;

	m_opInstance = EM_new EMThreadRepository();
	return m_opInstance;
}

