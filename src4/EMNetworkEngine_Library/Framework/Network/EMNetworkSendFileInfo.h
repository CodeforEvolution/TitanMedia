/*******************************************************
* Creator: Martin Johansson
* Portability: Native, very native
*-------------------------------------------------------
* 
* Martin Johansson, 01-09-11
*******************************************************/


#include "EMGlobals.h"
#include "EMNetworkGlobals.h"

#ifndef __EM_NETWORK_SEND_FILE_INFO
#define __EM_NETWORK_SEND_FILE_INFO

#include <windows.h>

#include "EMUserInfo.h"
#include "EMNetworkFileInfo.h"

class EMNetworkSendFileInfo : public EMNetworkFileInfo
{
public:
	EMNetworkSendFileInfo(const char* p_opFileName);
	~EMNetworkSendFileInfo();

	void PhaseChange();
	void TransferData();

	int Upload();

	void TaskEvent(TSonorkApiTaskEvent* p_opTask);

private:
	EMNetworkSendFileInfo() {};

private:
};

#endif

