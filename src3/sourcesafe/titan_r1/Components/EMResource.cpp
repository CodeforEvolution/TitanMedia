#include "EMResource.h"

#include "EMBitmap.h"

#include <list>
#include <string>

EMResource::EMResource() :
m_eType(EM_UNKNOWN_RESOURCE),
m_vID(0),
m_opResource(NULL)
{
}

EMResource::EMResource(EMResType p_eType, int p_vID, void* p_opResource) :
m_eType(p_eType),
m_vID(p_vID),
m_opResource(p_opResource)
{
}

EMResource::~EMResource()
{
	switch(m_eType)
	{
	case EM_STRING:
		delete static_cast<string*>(m_opResource);
		break;
	case EM_STRING_LIST:
		{
			list<string*>* opStrings = static_cast<list<string*>*>(m_opResource);
			string* opString;
			while(opStrings -> size() > 0)
			{
				opString = opStrings -> front();
				opStrings -> pop_front();
				delete opString;
			}
//			opStrings = static_cast<list<string*>*>(m_opResource);
//			delete static_cast<list<string>*>(m_opResource);
			break;
		}
	case EM_BITMAP:
		delete static_cast<EMBitmap*>(m_opResource);
		break;
	default:
		break;
	}
}

