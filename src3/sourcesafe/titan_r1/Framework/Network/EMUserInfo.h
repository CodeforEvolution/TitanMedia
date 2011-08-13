/*******************************************************
* Creator: Martin Johansson
* Portability: Native, very native
*-------------------------------------------------------
* 
* Martin Johansson, 01-08-01
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_USER_INFO
#define __EM_USER_INFO

class TSonorkUserData;
struct TSonorkId;
struct SONORK_DWORD2;
struct TSonorkApiTaskEvent;

class EMUserInfo
{
public:
	EMUserInfo();
	EMUserInfo(TSonorkUserData* p_opUser, bool p_vIsOnline);
	~EMUserInfo();

//	void TaskEvent(TSonorkApiTaskEvent* p_opTask);

	static uint64 ConvertId(const TSonorkId& p_oID);
	static uint64 ConvertId(const SONORK_DWORD2& p_oID);
	static SONORK_DWORD2 ConvertId(const uint64 p_vID);


	TSonorkUserData* GetHandle();
	void ResetHandle();

	uint32 GetLowId();
	uint32 GetHighId();
	uint64 GetId();

	string m_opName;
	uint64 m_opUID;
	int m_vOnline;

	int64 m_vTaskId;

protected:
	TSonorkUserData* m_opUser;	
	bool m_vHandleInit;

private:
};

#endif
