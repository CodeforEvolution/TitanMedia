#ifndef __EM_THREAD_LISTENER
#define __EM_THREAD_LISTENER

class EMThread;

class __declspec(dllexport) EMThreadListener
{
public:
	virtual ~EMThreadListener();
	virtual void ThreadRun(EMThread* p_opThread) = 0;
	virtual void OnThreadCreated(EMThread* p_opThread);
	virtual void OnThreadKilled(EMThread* p_opThread);

protected:
	EMThreadListener();
private:
};

#endif