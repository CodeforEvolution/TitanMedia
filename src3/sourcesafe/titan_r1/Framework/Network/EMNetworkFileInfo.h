/*******************************************************
* Creator: Martin Johansson
* Portability: Native, very native
*-------------------------------------------------------
* 
* Martin Johansson, 01-08-01
*******************************************************/

#include "EMGlobals.h"
#include "EMNetworkGlobals.h"

#ifndef __EM_NETWORK_FILE_INFO
#define __EM_NETWORK_FILE_INFO

#include "EMNetworkTask.h"


#include <windows.h>

struct TSonorkApiFileSession;
struct TSonorkApiTaskEvent;

class EMUserInfo;

class EMNetworkFileInfo : public EMNetworkTask
{
public:
	EMNetworkFileInfo();
	virtual ~EMNetworkFileInfo();

	virtual void PhaseChange() = 0;
	virtual void TransferData() = 0;

//	virtual void TaskEvent(TSonorkApiTaskEvent* p_opTask) = 0;

	static void FileEvent(TSonorkApiFileSession* p_opSession);

	uint64 GetId();

	TSonorkApiFileSession* m_opSession;

	int32 m_vTaskID;

	EMUserInfo* m_opRemoteUser;

	int32 m_vTransfered;
	int32 m_vSize;

	EMFilePhase m_vPhase;

	bool m_vSending;

	bool m_vIsMediaMaterial;

	string m_oFilePath;
	string m_oFileName;

	HANDLE m_hFile;

private:
	static uint64 m_vNextID;
	const uint64 m_vID;
};

#endif
