/*******************************************************
* Creator: Martin Johansson
* Portability: Native, very native
*-------------------------------------------------------
* 
* Base class for all items that can receive a TaskMsg.
*
* Martin Johansson, 01-08-01
*******************************************************/

#ifndef __EM_NETWORK_TASK
#define __EM_NETWORK_TASK

struct TSonorkApiTaskEvent;

class EMNetworkTask
{
public:
	virtual ~EMNetworkTask() {};


	virtual void TaskEvent(TSonorkApiTaskEvent* p_opTask) = 0;
protected:
	EMNetworkTask() {};
};

#endif