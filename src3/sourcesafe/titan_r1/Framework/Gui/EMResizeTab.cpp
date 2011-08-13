#include "EMResizeTab.h"

#include "EMWinResizeTab.h"

EMResizeTab::EMResizeTab()
{
}

EMResizeTab::~EMResizeTab()
{
}

EMResizeTab* EMResizeTab::Create(void* p_opParentNativeView, EMRect p_oParentRect)
{
#ifdef PLATFORM_WINDOWS
	return EM_new EMWinResizeTab(p_opParentNativeView, p_oParentRect);
#endif
	return NULL;
}