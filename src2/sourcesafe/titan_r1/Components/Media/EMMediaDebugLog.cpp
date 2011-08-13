#include "EMMediaDebugLog.h"

#include <fstream>
#include <string>
#include <time.h>

#define EM_MEDIA_LOG_FILENAME "\\zZ-TitanMediaSystem-Zz.log"

EMMediaDebugLog* EMMediaDebugLog::m_opInstance = NULL;

EMMediaDebugLog* EMMediaDebugLog::Instance()
{
	if(m_opInstance == NULL)
		m_opInstance = EM_new EMMediaDebugLog();
	return m_opInstance;
}

void EMMediaDebugLog::Delete()
{ 
	delete m_opInstance;
	m_opInstance = NULL;
}

EMMediaDebugLog::EMMediaDebugLog()
{
#ifndef _DEBUG
	return;
#endif
	m_uSemaphore = CreateSemaphore(NULL, 1, 1, "EMDebugLogSemaphore");
#ifdef _DEBUG
	m_opLogFile = EM_new ofstream(EM_MEDIA_LOG_FILENAME, ios::out | ios::trunc);
	(*m_opLogFile) << "Date / time stamp\t\tThread ID\tDebug text" << endl;
	(*m_opLogFile) << "----------------------------------------------------------------------------------------------" << endl;
	m_opLogFile -> close();
#endif
}

EMMediaDebugLog::~EMMediaDebugLog()
{
#ifndef _DEBUG
	return;
#endif
	HRESULT vResult = WaitForSingleObject(m_uSemaphore, INFINITE);
	if(vResult == WAIT_OBJECT_0)
	{
#ifdef _DEBUG
		m_opLogFile -> close();
		delete m_opLogFile;
		m_opLogFile = NULL;
#endif
		CloseHandle(m_uSemaphore);
	}
	else
		MessageBox(NULL, "EMMediaDebugLog failed to acquire protection semaphore!", "Error!", MB_OK);
}

void EMMediaDebugLog::operator<<(const char* p_vpString)
{
#ifndef _DEBUG
	return;
#endif
	Log(p_vpString);
}

void EMMediaDebugLog::Log(const char* p_vpString)
{
#ifndef _DEBUG
	return;
#endif
	HRESULT vResult = WaitForSingleObject(m_uSemaphore, INFINITE);
	if(vResult == WAIT_OBJECT_0)
	{
#ifdef _DEBUG
		time_t now;
		tm when;
		time( &now );
		when = *localtime(&now);
		string oNow(asctime(&when));

		m_opLogFile -> open(EM_MEDIA_LOG_FILENAME, ios::out | ios::app);
		char* vpStr = new char[oNow.length()];
		memcpy(vpStr, oNow.c_str(), oNow.length() -1);
		vpStr[oNow.length() -1] = '\0';
		oNow = vpStr;

		uint32 vCurrentThreadID = (uint32) GetCurrentThreadId();

		(*m_opLogFile) << oNow.c_str() << "\t" << vCurrentThreadID << "\t" << flush;
		(*m_opLogFile) << p_vpString << endl;
		m_opLogFile -> close();
#endif
		ReleaseSemaphore(m_uSemaphore, 1, NULL);
	}
	else
		MessageBox(NULL, "EMMediaDebugLog failed to acquire protection semaphore!", "Error!", MB_OK);
}
