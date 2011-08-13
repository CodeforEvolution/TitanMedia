#ifndef __EM_RESOURCE
#define __EM_RESOURCE

enum EMResType
{
	EM_STRING,
	EM_STRING_LIST,
	EM_BITMAP,
	EM_UNKNOWN_RESOURCE
};

class EMResource
{
public:
	EMResource();
	EMResource(EMResType p_eType, int p_vID, void* p_opResource);
	virtual ~EMResource();
	
public:
	EMResType m_eType;
	void* m_opResource;
	int m_vID;
};

#endif