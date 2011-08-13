#include "EMBeView.h"

#ifdef PLATFORM_BEOS

#include "EMBeMessageTranslator.h"
#include "EMBeViewContainer.h"
#include "EMFactory.h"
#include "EMKeyboard.h"
#include "EMListener.h"
#include "EMMessages.h"

#include <Window.h>
#include <Message.h>

EMBeViewContainer* EMBeView::m_opLastMouseDownView = NULL;
float EMBeView::m_opLastMouseDownCoordinateX = 0xFFFF;
float EMBeView::m_opLastMouseDownCoordinateY = 0xFFFF;
const float DOUBLE_CLICK_RANGE = 10;

EMBeView::EMBeView(EMBeViewContainer* p_opContainer, const BRect p_oFrame, uint32 p_vResizingMode, uint32 p_vFlags) :
BView(p_oFrame, NULL, p_vResizingMode, p_vFlags),
m_opContainer(p_opContainer)
{
}

EMBeView::~EMBeView()
{
}

void EMBeView::AttachedToWindow()
{
	// This prevents heavy processing to get far behind mouse movements
	SetEventMask(0, B_NO_POINTER_HISTORY);
}

void EMBeView::Draw(const BRect p_oRect)
{
	if(!(m_opContainer -> Notify(EM_DRAW)))
		BView::Draw(p_oRect);
}

EMBeViewContainer* EMBeView::GetLastMouseDownView()
{
	return m_opLastMouseDownView;
}

void EMBeView::KeyDown(const char* vBytes, int32 vNrBytes)
{
	BMessage* opMessage = Window() -> CurrentMessage();
	int32 vKeyCode;
	opMessage -> FindInt32("raw_char", &vKeyCode);
	int32 vModifiers;
	opMessage -> FindInt32("modifiers", &vModifiers);
	EMKeyboard::Instance() -> NotifyKey(m_opContainer,  vKeyCode, true, vModifiers);
}

void EMBeView::KeyUp(const char* vBytes, int32 vNrBytes)
{
	BMessage* opMessage = Window() -> CurrentMessage();
	int32 vKeyCode;
	opMessage -> FindInt32("raw_char", &vKeyCode);
	int32 vModifiers;
	opMessage -> FindInt32("modifiers", &vModifiers);
	EMKeyboard::Instance() -> NotifyKey(m_opContainer,  vKeyCode, false, vModifiers);
}

// Forwards messages
void EMBeView::MessageReceived(BMessage* p_opMessage)
{
//;//cout_commented_out_4_release << "EMBeView::MessageReceived: " << p_opMessage -> what << endl;
	uint32 vMessage = EMBeMessageTranslator::TranslateMessage(p_opMessage -> what);
	if(!(m_opContainer -> Notify(vMessage)))
		BView::MessageReceived(p_opMessage);
}

void EMBeView::MouseDown(const BPoint p_oPoint)
{
//;//cout_commented_out_4_release << "MouseDown" << endl;
	// Makes sure the window is focused during entire time mouse is down
	SetMouseEventMask(B_POINTER_EVENTS, B_LOCK_WINDOW_FOCUS);
	MakeFocus(); // Receive focus to catch keyboard events
	int32 vClicks;
	Window() -> CurrentMessage() -> FindInt32("clicks", &vClicks);
	float vMoveDistanceX;
	float vMoveDistanceY;
	
	if(p_oPoint.x - m_opLastMouseDownCoordinateX >= 0)
		vMoveDistanceX = p_oPoint.x - m_opLastMouseDownCoordinateX;
	else
		vMoveDistanceX = m_opLastMouseDownCoordinateX - p_oPoint.x;
	if(p_oPoint.y - m_opLastMouseDownCoordinateY >= 0)
		vMoveDistanceY = p_oPoint.y - m_opLastMouseDownCoordinateY;
	else
		vMoveDistanceY = m_opLastMouseDownCoordinateY - p_oPoint.y;
	if(vClicks > 1 && m_opLastMouseDownView == m_opContainer && vMoveDistanceX <= DOUBLE_CLICK_RANGE && vMoveDistanceY <= DOUBLE_CLICK_RANGE)
	{
		m_opContainer -> Notify(EM_MOUSE_DOWN); // Perhaps this should be removed in the future?
		m_opContainer -> Notify(EM_DOUBLE_CLICK);
	}
	else
	{
		m_opContainer -> Notify(EM_MOUSE_DOWN);
	}
	
	m_opLastMouseDownCoordinateX = p_oPoint.x;
	m_opLastMouseDownCoordinateY = p_oPoint.y;
	m_opLastMouseDownView = m_opContainer;
}

void EMBeView::MouseMoved(const BPoint p_oPoint, uint32 p_vTransit, const BMessage* p_opMessage)
{
//;//cout_commented_out_4_release << "MouseMoved" << endl;
// Why can I not call the base class version of the method and not loose some MouseDown calls?
//	if(!(m_opContainer -> Notify(EM_MOUSE_MOVED)))
//		BView::MouseMoved(p_oPoint, p_vTransit, p_opMessage);
//;//cout_commented_out_4_release << "mouse moved " << this << endl;

	// Sporadic calls to this method (because of bug in BeOS?), therefore this check
	if(p_oPoint != m_oPoint)
	{
		m_opContainer -> Notify(EM_MOUSE_MOVED);
		m_oPoint = p_oPoint;
	}
}

void EMBeView::MouseUp(const BPoint p_oPoint)
{
//;//cout_commented_out_4_release << "MouseUp" << endl;
//	if(!(m_opContainer -> Notify(EM_MOUSE_UP)))
//		BView::MouseUp(p_oPoint);
m_opContainer -> Notify(EM_MOUSE_UP);
}

void EMBeView::FrameResized(float p_vWidth, float p_vHeight)
{
	if(!(m_opContainer -> Notify(EM_VIEW_BOUNDS_CHANGED)))
		BView::FrameResized(p_vWidth, p_vHeight);
}

#endif
