#include "EMMediaIDManager.h"

int32 EMMediaIDManager::m_vID = 0;
string EMMediaIDManager::m_oDummyString = "";

EMMediaIDManager::EMMediaIDManager()
{
}

EMMediaIDManager::~EMMediaIDManager()
{
}

int32 EMMediaIDManager::MakeID()
{
	int32 vID = -1;
	vID = (++m_vID);
	return (vID);
}

const char* EMMediaIDManager::MakeUniqueString()
{
	int32 vID = (++m_vID);
	char container[256];
	itoa(vID, container, 10);
	m_oDummyString = container;
	return m_oDummyString.c_str();
}