#include "EMWinResizeTab.h"

#ifdef PLATFORM_WINDOWS

#include "EMFactory.h"
#include "EMView.h"
#include <windows.h>
#include <shlobj.h>

EMWinResizeTab::EMWinResizeTab(void* v_opParentNativeView, EMRect p_oParentRect) :
m_opView(EMFactory::Instance() -> CreateView(EMRect(p_oParentRect.GetWidth() - EM_METRICS_NATIVE_SCROLLBAR_HEIGHT + 1,
													p_oParentRect.GetHeight() - EM_METRICS_NATIVE_SCROLLBAR_HEIGHT + 1,
													p_oParentRect.GetWidth(),
													p_oParentRect.GetHeight()), 0))
{
	CreateStatusWindow(WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, "", static_cast<HWND>(m_opView -> GetNativeView()), 0);
	

	SetWindowLong(reinterpret_cast<HWND>(m_opView -> GetNativeView()), GWL_WNDPROC, GetWindowLong(reinterpret_cast<HWND>(v_opParentNativeView), GWL_WNDPROC));
}

EMWinResizeTab::~EMWinResizeTab()
{
}

EMView* EMWinResizeTab::GetView()
{
	return m_opView;
}

#endif