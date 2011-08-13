/*******************************************************
* Creator: Martin Johansson
* Portability: Native, very native
*-------------------------------------------------------
* 
* This class is used to keep track of and seperate
* search, add and delete queries.
*
* Martin Johansson, 01-10-16
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_NETWORK_SEARCH_ITEM
#define __EM_NETWORK_SEARCH_ITEM

#include "EMNetworkGlobals.h"
#include "EMNetworkTask.h"

class EMNetworkSearchItem : public EMNetworkTask

{
public:
	EMNetworkSearchItem(int64 p_vTaskId, EMTaskType p_vType);

	int64 GetId();

	void TaskEvent(TSonorkApiTaskEvent* p_opTask);

	int64 m_vTaskId;
	EMTaskType m_vType;
	EMPhase m_vPhase;
protected:
private:
};

#endif
