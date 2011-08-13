#include "EMUserInfo.h"
#include "sonorkvcapi.h"
#include "srk_data_types.h"

#include "EMNetworkEngine.h"

EMUserInfo::EMUserInfo()
:  m_opUser(NULL),
   m_vHandleInit(false),
   m_vTaskId(-1)
{
}

EMUserInfo::EMUserInfo(TSonorkUserData* p_opUser, bool p_vIsOnline)
:  m_vHandleInit(false),
   m_vOnline(p_vIsOnline)
{
	m_opName = p_opUser -> alias.CStr();
	m_opUID = ConvertId(p_opUser -> userId);

	m_opUser = new TSonorkUserData();
	m_opUser -> Set(*p_opUser);
}

EMUserInfo::~EMUserInfo()
{
	if(m_opUser != NULL)
		delete m_opUser;
}

uint64 EMUserInfo::GetId()
{
	return m_opUID;
}

uint64 EMUserInfo::ConvertId(const TSonorkId &p_oID)
{
	union
	{
		TSonorkId sonork;
		uint64 em;
	} id_convert;

	id_convert.sonork = p_oID;
	return id_convert.em;
}


uint64 EMUserInfo::ConvertId(const SONORK_DWORD2 &p_oID)
{
	union
	{
		SONORK_DWORD2 sonork;
		uint64 em;
	} id_convert;

	id_convert.sonork = p_oID;
	return id_convert.em;
}

SONORK_DWORD2 EMUserInfo::ConvertId(const uint64 p_vID)
{
	union
	{
		SONORK_DWORD2 sonork;
		uint64 em;
	} id_convert;

	id_convert.em = p_vID;
	return id_convert.sonork;
}



uint32 EMUserInfo::GetLowId()
{
	return m_opUID & (uint32(-1));
}

uint32 EMUserInfo::GetHighId()
{
	return (m_opUID >> 32) & (int32(-1));
}

TSonorkUserData* EMUserInfo::GetHandle()
{
	if(!m_vHandleInit)
	{
//		EMNetworkEngine::Instance() -> GetAPI() -> GetUserHandle(m_opUserHandle);
		m_vHandleInit = true;
	}

	return m_opUser;
}

void EMUserInfo::ResetHandle()
{
	m_vHandleInit = false;
}
