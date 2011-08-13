#include "EMVideoPreview.h"

#include "EMBitmap.h"

EMVideoPreview* EMVideoPreview::m_opInstance = NULL;

EMVideoPreview::EMVideoPreview()
{
}

EMVideoPreview::~EMVideoPreview()
{
}

EMVideoPreview* EMVideoPreview::Instance()
{
	return m_opInstance;
}

void EMVideoPreview::SetInstance(EMVideoPreview* p_opInstance)
{
	if(p_opInstance == NULL)
		EMDebugger("p_opInstance is set to NULL");
	m_opInstance = p_opInstance;
}


