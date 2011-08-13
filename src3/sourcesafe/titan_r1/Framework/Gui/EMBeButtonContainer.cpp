#include "EMBeButtonContainer.h"

#ifdef PLATFORM_BEOS

#include "EMCommand.h"
#include "EMCommandRepository.h"
#include "EMBeButton.h"

EMBeButtonContainer::EMBeButtonContainer(const EMRect p_oRect, const char* p_vpName, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand)
{
	uint32 vResizingMode = 0;
	
	if((p_vResizingMode & EM_FOLLOW_LEFT) == EM_FOLLOW_LEFT)
		vResizingMode = vResizingMode | B_FOLLOW_LEFT;
	if((p_vResizingMode & EM_FOLLOW_RIGHT) == EM_FOLLOW_RIGHT)
		vResizingMode = vResizingMode | B_FOLLOW_RIGHT;
	if((p_vResizingMode & EM_FOLLOW_H_CENTER) == EM_FOLLOW_H_CENTER)
		vResizingMode = vResizingMode | B_FOLLOW_H_CENTER;
	if((p_vResizingMode & EM_FOLLOW_TOP) == EM_FOLLOW_TOP)
		vResizingMode = vResizingMode | B_FOLLOW_TOP;
	if((p_vResizingMode & EM_FOLLOW_BOTTOM) == EM_FOLLOW_BOTTOM)
		vResizingMode = vResizingMode | B_FOLLOW_BOTTOM;
	if((p_vResizingMode & EM_FOLLOW_V_CENTER) == EM_FOLLOW_V_CENTER)
		vResizingMode = vResizingMode | B_FOLLOW_V_CENTER;

	BRect oRect(p_oRect.m_vLeft, p_oRect.m_vTop, p_oRect.m_vRight, p_oRect.m_vBottom);
	BMessage* opMess = new BMessage(p_vMessage);
	m_vCommand = p_vCommand;
	
	m_opButton = new EMBeButton(this, oRect, p_vpName, opMess, vResizingMode);	
}

EMBeButtonContainer::~EMBeButtonContainer()
{
}

void EMBeButtonContainer::ExecuteCommand()
{
	if(m_vCommand != 0)
		EMCommandRepository::Instance() -> ExecuteCommand(m_vCommand);
}

EMRect EMBeButtonContainer::Frame() const
{
	BRect oRect = m_opButton -> Frame();
	return EMRect(oRect.left, oRect.top, oRect.right, oRect.bottom);
}

void* EMBeButtonContainer::GetNativeView() const
{
	return (void*) m_opButton;
}

void EMBeButtonContainer::Hide()
{
	m_opButton -> Hide();
}

void EMBeButtonContainer::InitComponent()
{
	m_opButton -> SetTarget(m_opButton);
}

void EMBeButtonContainer::SetCommand(uint32 p_vCommand)
{
	m_vCommand = p_vCommand;
}

void EMBeButtonContainer::SetLabel(const char* p_vpLabel)
{
	m_opButton -> SetLabel(p_vpLabel);
}

void EMBeButtonContainer::Show()
{
	m_opButton -> Show();
}

#endif
