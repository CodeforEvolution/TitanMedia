/*******************************************************
* Creator: Martin Johansson
* Portability: Native, very native
*-------------------------------------------------------
* 
* Martin Johansson, 01-09-11
*******************************************************/


#include "EMGlobals.h"
#include "EMNetworkGlobals.h"

#ifndef __EM_NETWORK_RECV_FILE_INFO
#define __EM_NETWORK_RECV_FILE_INFO

#include "EMNetworkFileInfo.h"

struct TSonorkMsg;
struct TSonorkFileInfo;
struct TSonorApiTaskEvent;
struct TSonorkServiceData;

class EMNetworkRecvFileInfo : public EMNetworkFileInfo
{
public:
	EMNetworkRecvFileInfo(TSonorkMsg* p_opMsg);
	~EMNetworkRecvFileInfo();

	void PhaseChange();
	void TransferData();

	int Download();

	static void TaskEvent(TSonorkApiFileSession* p_opSession);
	void TaskEvent(TSonorkApiTaskEvent* p_opTask){};

	TSonorkFileInfo* m_opInfo;

private:
	EMNetworkRecvFileInfo() {};

	TSonorkServiceData* m_opService;
};

#endif
