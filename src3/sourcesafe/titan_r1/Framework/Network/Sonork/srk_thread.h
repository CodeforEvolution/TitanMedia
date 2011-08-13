#if !defined(SONORK_THREAD_H)
#define SONORK_THREAD_H
#include "srk_defs.h"



class TSonorkThread
{
	enum TSonorkThreadFlags
	{
		 F_TERMINATE	=0x80000000
		,F_TERMINATED   =0x40000000
		,F_STARTED	=0x00000001
		,F_EXECUTING	=0x00000002
	};
	struct {
		DWORD			id;
		HANDLE			handle;
		UINT			flags;
		const char *		name;
		int			exit_code;
		TSonorkLock		mutex;
	}thread_info;
protected:
	TSonorkThread(const char *name);
	virtual ~TSonorkThread();

	static void WINAPI
		_Run(TSonorkThread*);
	virtual int
		Run()=0;

	HANDLE
		ThreadHandle()	const
		{ return thread_info.handle;}

	const char*
		ThreadName() const
		{ return thread_info.name;}

	DWORD	ThreadId() const
		{ return thread_info.id;}

	void
		Suspend();
public:

	void	LockThread(UINT line)
			{ thread_info.mutex.Lock(__FILE__,line);}

	void	UnlockThread()
			{ thread_info.mutex.Unlock();}

	UINT	ThreadFlags()		const
		{ return thread_info.flags;	}

	void	SetTerminateFlag()
		{ thread_info.flags|=F_TERMINATE;	}

	BOOL	ShouldTerminate()	const
		{ return thread_info.flags&F_TERMINATE;}

	BOOL	IsTerminated() 		const
		{ return thread_info.flags&F_TERMINATED;}

	int	ExitCode()			const
		{ return thread_info.exit_code;}

	void 	SetPriority(int);

	virtual void
		Terminate(DWORD timeout=5000);

	void
		Resume();

	bool
		Start();

	BOOL
		IsRunning() const
		{ return thread_info.flags&TSonorkThread::F_EXECUTING; }


};

#endif
