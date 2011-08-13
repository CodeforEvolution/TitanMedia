#include "srk_defs.h"
#pragma hdrstop
#include "srk_thread.h"
#include <process.h>

void TSonorkThread::SetPriority(int priority)
{
	if(ThreadHandle()!=INVALID_HANDLE_VALUE)
		SetThreadPriority(ThreadHandle(),priority);
}
void TSonorkThread::Suspend()
{
	if( ThreadHandle() != INVALID_HANDLE_VALUE )
		SuspendThread( ThreadHandle() );
}
void TSonorkThread::Resume()
{
	if( ThreadHandle() != INVALID_HANDLE_VALUE )
		ResumeThread( ThreadHandle() );
}
void
 TSonorkThread::Terminate(DWORD timeout)
{
	if( thread_info.flags & TSonorkThread::F_STARTED )
	if( !(thread_info.flags & TSonorkThread::F_TERMINATED) )
	if( ThreadHandle() != INVALID_HANDLE_VALUE )
	{
		SetTerminateFlag();
		DWORD rv=WaitForSingleObject(ThreadHandle(),timeout);
		if(rv!=WAIT_OBJECT_0)
		{
			if(rv==WAIT_FAILED)
			{
				/*rv=GetLastError();
				if(rv!=6)
				{
					SONORK_Trace("WARNING: %s exit error %d\n",ThreadName(),GetLastError());
				}
				*/
			}
			else
			{
				const char *str=(rv == WAIT_ABANDONED?"ABANDON":"TIMEOUT");
				sonork_printf("WARNING: %s Terminated by %s\n"
					,ThreadName()
					,str);
			}

		}
	}
}
void WINAPI TSonorkThread::_Run(TSonorkThread*T)
{

	sonork_printf("%04x %s Started\n",T->ThreadId(),T->ThreadName());

	T->thread_info.flags&=~(TSonorkThread::F_TERMINATE|TSonorkThread::F_TERMINATED);
	T->thread_info.flags|=TSonorkThread::F_EXECUTING;
	T->thread_info.exit_code=T->Run();
	T->thread_info.flags&=~TSonorkThread::F_EXECUTING;
	T->thread_info.flags|=TSonorkThread::F_TERMINATED;

	sonork_printf("%04x %s Stopped\n",T->ThreadId(),T->ThreadName());

	CloseHandle(T->thread_info.handle);
	T->thread_info.handle=INVALID_HANDLE_VALUE;
#if defined(__BORLANDC__)
	_endthread();
#else
	ExitThread(T->thread.exit_code);
#endif

}
bool  TSonorkThread::Start()
{
	SECURITY_ATTRIBUTES sa=
	{sizeof(SECURITY_ATTRIBUTES),0,TRUE};

#if defined(__BORLANDC__) && 0==1
	thread_info.handle=(HANDLE)
	_beginthreadNT(
		(void(*)(void *))this->_Run
		,4096
		 ,(void*)this
		 ,&sa
		 ,CREATE_SUSPENDED
		 ,&thread_info.id);
#else
	thread_info.handle=CreateThread(
		&sa,4096
		,(LPTHREAD_START_ROUTINE)(this->_Run)
		,this
		,CREATE_SUSPENDED
		,&thread_info.id);
#endif
	if(thread_info.handle!=INVALID_HANDLE_VALUE && thread_info.handle!=NULL)
	{
		thread_info.flags|=TSonorkThread::F_STARTED;
		::ResumeThread( ThreadHandle() );
		return true;
	}
	else
	{
		thread_info.handle=INVALID_HANDLE_VALUE;
		return false;
	}
}
TSonorkThread::TSonorkThread(const char *name)
{
	thread_info.exit_code=thread_info.id=thread_info.flags=0;
	thread_info.handle=INVALID_HANDLE_VALUE;
	thread_info.name=name;
}
TSonorkThread::~TSonorkThread()
{
	if(thread_info.handle!=INVALID_HANDLE_VALUE )
	{
		CloseHandle(ThreadHandle());
	}
}
