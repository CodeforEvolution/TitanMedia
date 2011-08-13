#include "EMUniqueID.h"

#include <sstream>
#include <string>

uint32 EMUniqueID::m_vUniqueID = 0;
char EMUniqueID::m_vUniqueName[40];
uint32 EMUniqueID::GetUniqueID()
{
	m_vUniqueID++;
	return m_vUniqueID;
}

const char* EMUniqueID::GetUniqueName()
{
	ostringstream oStream;
	oStream << "UniqueName" << GetUniqueID();
	strcpy(m_vUniqueName, oStream.str().c_str());
	return m_vUniqueName;
}

