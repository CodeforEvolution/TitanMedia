#include "EMWindow.h"

EMWindow::EMWindow() :
__m_vCurrentComponent(0),
__m_vNextWasCalled(false),
__m_vPreviosWasCalled(false)
{
}

EMWindow::~EMWindow()
{
}

void EMWindow::AddTabComponent(EMGUIComponent* p_opComponent)
{
	__m_oTabOrder.push_back(p_opComponent);
}

void EMWindow::NextTab()
{
	__m_vNextWasCalled = true;
	if(++__m_vCurrentComponent > __m_oTabOrder.size() - 1)
		__m_vCurrentComponent = 0;
	while(__m_oTabOrder[__m_vCurrentComponent] -> IsEnabled() == false || __m_oTabOrder[__m_vCurrentComponent] -> IsVisible() == false)
		if(++__m_vCurrentComponent > __m_oTabOrder.size() - 1)
			__m_vCurrentComponent = 0;
	__m_oTabOrder[__m_vCurrentComponent] -> SetFocus();
}

void EMWindow::PreviousTab()
{
	__m_vPreviosWasCalled = true;
	if(--__m_vCurrentComponent < 0)
		__m_vCurrentComponent = __m_oTabOrder.size() - 1;
	while(__m_oTabOrder[__m_vCurrentComponent] -> IsEnabled() == false || __m_oTabOrder[__m_vCurrentComponent] -> IsVisible() == false)
		if(--__m_vCurrentComponent < 0)
			__m_vCurrentComponent = __m_oTabOrder.size() - 1;
	__m_oTabOrder[__m_vCurrentComponent] -> SetFocus();
}

