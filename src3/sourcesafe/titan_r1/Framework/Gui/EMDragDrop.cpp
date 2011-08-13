#include "EMDragDrop.h"

#ifdef PLATFORM_WINDOWS
#include "EMWinDragDrop.h"
#endif

EMDragDrop* EMDragDrop::m_opInstance = NULL;

EMDragDrop::EMDragDrop()
{
}

EMDragDrop::~EMDragDrop()
{
}

EMDragDrop* EMDragDrop::Instance()
{
	if(m_opInstance == NULL)
	{
#ifdef PLATFORM_WINDOWS
		m_opInstance = EM_new EMWinDragDrop;
#endif
	}
	return m_opInstance;
}