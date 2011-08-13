#include "EMBeWindowContainer.h"

#ifdef PLATFORM_BEOS

#include "EMBeWindow.h"
#include "EMView.h"

EMBeWindowContainer::EMBeWindowContainer(const EMRect p_oRect, const char* p_vpName, const EMWindowType p_oType)
{
	BRect oRect(p_oRect.m_vLeft, p_oRect.m_vTop, p_oRect.m_vRight, p_oRect.m_vBottom);
	switch(p_oType)
	{
		case EM_WINDOW_NORMAL:
			m_opWindow = new EMBeWindow(this, oRect, p_vpName, B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS);
			break;
		case EM_WINDOW_NUDGE:
			m_opWindow = new EMBeWindow(this, oRect, p_vpName, B_DOCUMENT_WINDOW, B_ASYNCHRONOUS_CONTROLS);
			break;
		case EM_WINDOW_UNRESIZABLE:
			m_opWindow = new EMBeWindow(this, oRect, p_vpName, B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_NOT_RESIZABLE);
			break;
		case EM_WINDOW_TOOLBOX:
			m_opWindow = new EMBeWindow(this, oRect, p_vpName, B_FLOATING_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_NOT_RESIZABLE);
			break;
		case EM_WINDOW_POPUP:
			m_opWindow = new EMBeWindow(this, oRect, p_vpName, B_NO_BORDER_WINDOW_LOOK, B_MODAL_ALL_WINDOW_FEEL, B_ASYNCHRONOUS_CONTROLS | B_NOT_RESIZABLE | B_NO_WORKSPACE_ACTIVATION);
			break;
		default:
			EMDebugger("EMBeWindowContainer: Window type not allowed: EM_WINDOW_UNKNOWN");
	}
}

EMBeWindowContainer::~EMBeWindowContainer()
{
	// BeOS deletes all windows itsel so nothing is done here
/*
	;//cout_commented_out_4_release << "~EMBeWindowContainer()" << endl;
	while(m_opWindow -> ChildAt(0) != NULL)
	{
		;//cout_commented_out_4_release << "~EMBeWindowContainer(): removing child" << endl;
		m_opWindow -> RemoveChild(m_opWindow -> ChildAt(0));
	}
*/
}

void EMBeWindowContainer::AddChild(EMGUIComponent* p_opView)
{
	m_opWindow -> AddChild((BView*) p_opView -> GetNativeView());
	p_opView -> InitComponent();
}

EMRect EMBeWindowContainer::Bounds() const
{
	BRect oRect = m_opWindow -> Bounds();
	return EMRect(oRect.left, oRect.top, oRect.right, oRect.bottom);
}

EMRect EMBeWindowContainer::Frame() const
{
	BRect oRect = m_opWindow -> Frame();
	return EMRect(oRect.left, oRect.top, oRect.right, oRect.bottom);
}

void* EMBeWindowContainer::GetNativeView() const
{
	return (void*) m_opWindow;
}

void EMBeWindowContainer::Hide()
{
	m_opWindow -> Hide();
}

bool EMBeWindowContainer::IsHidden()
{
	return m_opWindow -> IsHidden();
}

bool EMBeWindowContainer::Lock()
{
/*
thread_id vThreadID = m_opWindow -> LockingThread();
thread_id vThisThreadID = find_thread(NULL);
thread_info oThreadInfo;
thread_info oThisThreadInfo;
get_thread_info(vThisThreadID, &oThisThreadInfo);
if(vThreadID != -1)
{
get_thread_info(vThreadID, &oThreadInfo);
if(vThreadID == vThisThreadID)
;//cout_commented_out_4_release << "Before locking window. Already locked by the same thread: " << oThisThreadInfo.name << endl;
else
;//cout_commented_out_4_release << "Before locking window. Already locked by a thread: " << oThreadInfo.name << ", this thread: " << oThisThreadInfo.name << endl;
}
else
;//cout_commented_out_4_release << "Before locking window. This thread: " << oThisThreadInfo.name << endl;
bool vResult =  m_opWindow -> Lock();
;//cout_commented_out_4_release << " ------------------------  Window Locked (" << vResult << ") " << oThisThreadInfo.name << " -------------------" << endl;
//EMDebugger("Window Locked");
	return vResult;
*/
return m_opWindow -> Lock();
}

void EMBeWindowContainer::MoveTo(float p_vX, float p_vY)
{
	m_opWindow -> MoveTo(p_vX, p_vY);
}

void EMBeWindowContainer::SetBounds(EMRect p_oBounds)
{
	m_opWindow -> MoveTo(p_oBounds.m_vLeft, p_oBounds.m_vTop);
	m_opWindow -> ResizeTo(p_oBounds.GetWidth(), p_oBounds.GetHeight());
}

void EMBeWindowContainer::SetFrame(EMRect p_oFrame)
{
	if(Frame() == p_oFrame)
		return;
		
	if(Frame().m_vTop != p_oFrame.m_vTop || Frame().m_vLeft != p_oFrame.m_vLeft)
		m_opWindow -> MoveTo(p_oFrame.m_vLeft, p_oFrame.m_vTop);
	if(Frame().GetWidth() != p_oFrame.GetWidth() || Frame().GetHeight() != p_oFrame.GetHeight())
		m_opWindow -> ResizeTo(p_oFrame.GetWidth(), p_oFrame.GetHeight());
}

void EMBeWindowContainer::SetSizeLimits(float p_vMinimumWidth, float p_vMaximumWidth, float p_vMinimumHeight, float p_vMaximumHeght)
{
	m_opWindow -> SetSizeLimits(p_vMinimumWidth, p_vMaximumWidth, p_vMinimumHeight, p_vMaximumHeght);
}

void EMBeWindowContainer::SetTitle(const char* p_vpTitle)
{
	m_opWindow -> SetTitle(p_vpTitle);
}

void EMBeWindowContainer::Show()
{
	m_opWindow -> Show();
}

void EMBeWindowContainer::Unlock()
{
//;//cout_commented_out_4_release << " ------------------------  Window Unlocked -------------------" << endl;
//;//cout_commented_out_4_release << " ------------------------  Window Unlocked -------------------" << endl;
	m_opWindow -> Unlock();
}

#endif
