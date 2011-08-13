/*******************************************************
* Creator: Martin Johansson
* Component: none
* Portability: non-native
*-------------------------------------------------------
*
*******************************************************/


#ifndef __EM_NETWORK_GLOBALS
#define __EM_NETWORK_GLOBALS

enum EMPhase 
{
	EM_PHASE_ERROR = -1,
	EM_PHASE_NONE = 0,
	EM_PHASE_CONNECTING,
	EM_PHASE_DOWNLOADING_USERS,
	EM_PHASE_UPDATING_SID,
	EM_PHASE_WORKING,
	EM_PHASE_DONE
};

enum EMFilePhase
{
	EM_FILE_PHASE_ERROR = -1,
	EM_FILE_PHASE_NONE = 0,
	EM_FILE_PHASE_RESOLVING,
	EM_FILE_PHASE_CONNECTING,
	EM_FILE_PHASE_SENDING,
	EM_FILE_PHASE_DONE
};

enum EMTaskType
{
	EM_TASK_SEARCH_USER,
	EM_TASK_ADD_USER,
	EM_TASK_DELETE_USER
};

#endif
