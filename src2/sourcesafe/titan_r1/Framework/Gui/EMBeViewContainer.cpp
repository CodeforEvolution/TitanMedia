#include "EMBeViewContainer.h"

#ifdef PLATFORM_BEOS

#include "EMBeView.h"
#include "EMBitmap.h"
#include "EMGUIComponent.h"
#include "EMGUIGlobals.h"
#include "EMListener.h"

#include <Bitmap.h>
#include <Font.h>
#include <Region.h>
#include <View.h>
#include <Window.h>
#include <Point.h>

BBitmap* EMBeViewContainer::m_opBuffer = NULL;
EMBeView* EMBeViewContainer::m_opBufferView = NULL;
int EMBeViewContainer::m_vExistingDoubleBufferedViews = 0;

EMBeViewContainer::EMBeViewContainer(const EMRect p_oFrame, uint32 p_vResizingMode, bool p_vDoubleBuffered) :
m_opPlainView(new EMBeView(this, BRect(p_oFrame.m_vLeft, p_oFrame.m_vTop, p_oFrame.m_vRight, p_oFrame.m_vBottom), 0, B_WILL_DRAW | B_FRAME_EVENTS | B_NAVIGABLE)),
m_vBufferScrollOffsetX(0),
m_vBufferScrollOffsetY(0),
m_vIsDoubleBuffered(p_vDoubleBuffered)
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
	if((p_vResizingMode & EM_FOLLOW_LEFT_RIGHT) == EM_FOLLOW_LEFT_RIGHT)
		vResizingMode = vResizingMode | B_FOLLOW_LEFT_RIGHT;
	if((p_vResizingMode & EM_FOLLOW_TOP_BOTTOM) == EM_FOLLOW_TOP_BOTTOM)
		vResizingMode = vResizingMode | B_FOLLOW_TOP_BOTTOM;

	m_opPlainView -> SetResizingMode(vResizingMode);

	if(p_vDoubleBuffered && m_opBuffer == NULL) // The global double buffering buffer is now needed but not set yet
	{
		BRect oRect;
		oRect.left = 0;//p_oFrame.m_vLeft;
		oRect.top = 0;//p_oFrame.m_vTop;
		oRect.right = 1600;//p_oFrame.m_vRight;
		oRect.bottom = 1200;//p_oFrame.m_vBottom;
		
		m_opBuffer = new BBitmap(BRect(0, 0, /*oRect.Width()*/1600, /*oRect.Height()*/1200), B_RGB16, true);
		m_opBufferView = new EMBeView(this, oRect, vResizingMode, B_WILL_DRAW | B_FRAME_EVENTS | B_NAVIGABLE);
		m_opBufferView -> SetViewColor(B_TRANSPARENT_COLOR); // To avoid the background to be redrawn on every update = flicker
		if(m_opBuffer -> Lock())
		{
			m_opBuffer -> AddChild(m_opBufferView);
			m_opBufferView -> ResizeTo(1600, 1200);
			m_opBuffer -> Unlock();
		}
	}
		
//	auto_ptr<EMBeView> opPlainView(new EMBeView(this, oRect, vResizingMode, B_WILL_DRAW | B_FRAME_EVENTS | B_NAVIGABLE));
//	EMBeView* opPlainView(new EMBeView(this, oRect, vResizingMode, B_WILL_DRAW | B_FRAME_EVENTS));
	m_opPlainView -> SetViewColor(B_TRANSPARENT_COLOR); // To avoid the background to be redrawn on every update = flicker
	if(p_vDoubleBuffered)
	{
/*
		auto_ptr<BBitmap> opBuffer(new BBitmap(BRect(0, 0, 1600, 1200), B_RGB16, true));
		auto_ptr<EMBeView> opBufferView(new EMBeView(this, oRect, vResizingMode, B_WILL_DRAW | B_FRAME_EVENTS | B_NAVIGABLE));
		opBufferView -> SetViewColor(B_TRANSPARENT_COLOR); // To avoid the background to be redrawn on every update = flicker
		if(opBuffer -> Lock())
		{
			opBuffer -> AddChild(opBufferView.get());
			opBuffer -> Unlock();
		}
*/
//		m_opPlainView = opPlainView;
//		m_opBufferView = opBufferView;
//		m_opBuffer = opBuffer;
		m_opDrawView = m_opBufferView;
		m_vExistingDoubleBufferedViews++;
	}
	else
	{
//		m_opPlainView = opPlainView;
		m_opDrawView = m_opPlainView;
//		m_opDrawView = m_opPlainView;
	}
}

EMBeViewContainer::~EMBeViewContainer()
{
	;//cout_commented_out_4_release << "~EMBeViewContainer()" << endl;
	// SInce BeOS deletes allt its windows and attached views itself, only
	// the bitmap and its view are deleted here (done by the auto_ptr)
//	m_opBufferView -> RemoveSelf();
/*
	// The view must be removed from an eventual window since it will be
	// deleted twice otherwise (by the window and by the auto_ptr destructor)
	if(m_opPlainView -> Window() != NULL)
	{
		;//cout_commented_out_4_release << "~EMBeViewContainer(): removing view from window...";
		m_opPlainView -> RemoveSelf();
		;//cout_commented_out_4_release << "done" << endl;
	}
	else 
		;//cout_commented_out_4_release <<"~EMBeViewContainer(): not attached" << endl;
*/
	if(m_vIsDoubleBuffered)
	{
		m_vExistingDoubleBufferedViews--;
		if(m_vExistingDoubleBufferedViews == 0) // If double buffered stuff should be deleted
		{
			delete m_opBufferView;
			m_opBufferView = NULL;
			delete m_opBuffer;
			m_opBuffer = NULL;
		}
	}
}

void EMBeViewContainer::AddChild(EMGUIComponent* p_opView)
{
	// Sub-views in a double-buffered view is currently not allowed
	if(m_vIsDoubleBuffered)
		EMDebugger("Adding views to a double-buffered view is not allowed");
	m_opPlainView -> AddChild((BView*) p_opView -> GetNativeView());
	p_opView -> InitComponent();
}

// Enters debugger if window is unlocked
void EMBeViewContainer::AssertWindowLocked()
{
	if(!(m_opPlainView -> Window() -> IsLocked()))
		EMDebugger("Window must be locked before entering this method\n");
}

EMRect EMBeViewContainer::Bounds() const
{
//	BRect oRect = m_opDrawView -> Bounds();
	// Using the plain view so that the bitmap buffer does not have to be locked
	// so often if the view is double-buffered
	BRect oRect = m_opPlainView -> Bounds();
	return EMRect(oRect.left, oRect.top, oRect.right, oRect.bottom);
}
	
void EMBeViewContainer::CalculateStringHeight(const char* p_vpString, float &p_vAscent, float &p_vDescent)
{
	font_height oHeight;
	m_opDrawView -> GetFontHeight(&oHeight);
	p_vAscent = oHeight.ascent;
	p_vDescent = oHeight.descent;
	if(p_vAscent - static_cast<int>(p_vAscent) > 0)
		p_vAscent = static_cast<int>(p_vAscent) + 1;
	if(p_vDescent - static_cast<int>(p_vDescent) > 0)
		p_vDescent = static_cast<int>(p_vDescent) + 1;
}

float EMBeViewContainer::CalculateStringWidth(const char* p_vpString)
{
	float vWidth = m_opDrawView -> StringWidth(p_vpString);
	if(vWidth - static_cast<int>(vWidth) > 0)
		vWidth = static_cast<int>(vWidth) + 1;
	return vWidth;
}

void EMBeViewContainer::ConstrainClippingRect(const EMRect p_oRect)
{
	if(!p_oRect.IsValid())
	{
		m_opPlainView -> ConstrainClippingRegion(NULL);
		if(m_vIsDoubleBuffered)
			m_opBufferView -> ConstrainClippingRegion(NULL);
		return;
	}
	
//	BRegion* oRegion = new BRegion();
	BRegion oRegion;
	BRect oRect;
	
	oRect.left = p_oRect.m_vLeft;
	oRect.top = p_oRect.m_vTop;
	oRect.right = p_oRect.m_vRight;
	oRect.bottom = p_oRect.m_vBottom;
//	oRegion -> Set(oRect);
//	m_opPlainView -> ConstrainClippingRegion(oRegion);
	oRegion.Set(oRect);
	m_opPlainView -> ConstrainClippingRegion(&oRegion);
	if(m_vIsDoubleBuffered)
		m_opBufferView -> ConstrainClippingRegion(&oRegion);
}

void EMBeViewContainer::ConvertToScreenCoordinates(float &p_vX, float &p_vY)
{
	BPoint oNewPoint = m_opPlainView -> ConvertToScreen(BPoint(p_vX, p_vY));
	p_vX = oNewPoint.x;
	p_vY = oNewPoint.y;
}

void EMBeViewContainer::DrawAndUnlockBuffer(EMRect p_oUpdateRect)
{
//;//cout_commented_out_4_release << "EMBeViewContainer::DrawAndUnlockBuffer: ";
//p_oUpdateRect.Print();
	if(!m_vIsDoubleBuffered)
		EMDebugger("DrawAndUnlockBuffer() does not make sense in a non-double-buffered view");

	if(!p_oUpdateRect.IsValid())
	{
		m_opBuffer -> Unlock();
		return;
	}

	BRect oSourceRect(p_oUpdateRect.m_vLeft, p_oUpdateRect.m_vTop, p_oUpdateRect.m_vRight, p_oUpdateRect.m_vBottom);
	oSourceRect.OffsetBy(-1 * m_vBufferScrollOffsetX, -1 * m_vBufferScrollOffsetY);
	BRect oDestinationRect(p_oUpdateRect.m_vLeft, p_oUpdateRect.m_vTop, p_oUpdateRect.m_vRight, p_oUpdateRect.m_vBottom);
//	BRect oDrawRect(p_oUpdateRect.m_vLeft, p_oUpdateRect.m_vTop, p_oUpdateRect.m_vRight, p_oUpdateRect.m_vBottom);
	m_opBufferView -> Sync();
//	m_opPlainView -> DrawBitmap(m_opBuffer.get(), oDrawRect, oDrawRect);
	m_opPlainView -> DrawBitmap(m_opBuffer, oSourceRect, oDestinationRect);
	m_opBuffer -> Unlock();
}

void EMBeViewContainer::DrawBitmap(const EMBitmap* p_opBitmap)
{
	m_opDrawView -> DrawBitmap((BBitmap*) p_opBitmap -> GetNativeBitmap());
}

void EMBeViewContainer::DrawBitmap(const EMBitmap* p_opBitmap, float p_vX, float p_vY)
{
	m_opDrawView -> DrawBitmap((BBitmap*) p_opBitmap -> GetNativeBitmap(), BPoint(p_vX, p_vY));
}

void EMBeViewContainer::DrawBitmap(const EMBitmap* p_opBitmap, EMRect p_oSource, EMRect p_oDestination)
{
	if(!p_oSource.IsValid())
		return;

	if(!p_oDestination.IsValid())
		return;

	BRect oSourceRect(p_oSource.m_vLeft, p_oSource.m_vTop, p_oSource.m_vRight, p_oSource.m_vBottom);
	BRect oDestinationRect(p_oDestination.m_vLeft, p_oDestination.m_vTop, p_oDestination.m_vRight, p_oDestination.m_vBottom);
	m_opDrawView -> DrawBitmap((BBitmap*) p_opBitmap -> GetNativeBitmap(), oSourceRect, oDestinationRect);
}
/*
void EMBeViewContainer::DrawBitmapOverlay(const EMBitmap* p_opBitmap, EMRect p_oSource, EMRect p_oDestination)
{
	if(!p_oSource.IsValid())
		return;

	if(!p_oDestination.IsValid())
		return;

	BRect oSourceRect(p_oSource.m_vLeft, p_oSource.m_vTop, p_oSource.m_vRight, p_oSource.m_vBottom);
	BRect oDestinationRect(p_oDestination.m_vLeft, p_oDestination.m_vTop, p_oDestination.m_vRight, p_oDestination.m_vBottom);
	rgb_color oColor;
	m_opDrawView -> SetViewOverlay((BBitmap*) p_opBitmap -> GetNativeBitmap(), oSourceRect, oDestinationRect, &oColor, B_OVERLAY_FILTER_HORIZONTAL | B_OVERLAY_FILTER_VERTICAL);
}
*/
void EMBeViewContainer::DrawBuffer(EMRect p_oUpdateRect)
{
	if(!m_vIsDoubleBuffered)
		EMDebugger("DrawBuffer() does not make sense in a non-double-buffered view");

	if(!p_oUpdateRect.IsValid())
		return;

	BRect oSourceRect(p_oUpdateRect.m_vLeft, p_oUpdateRect.m_vTop, p_oUpdateRect.m_vRight, p_oUpdateRect.m_vBottom);
	oSourceRect.OffsetBy(-1 * m_vBufferScrollOffsetX, -1 * m_vBufferScrollOffsetY);
	BRect oDestinationRect(p_oUpdateRect.m_vLeft, p_oUpdateRect.m_vTop, p_oUpdateRect.m_vRight, p_oUpdateRect.m_vBottom);
	m_opBufferView -> Sync();
	m_opPlainView -> DrawBitmap(m_opBuffer, oSourceRect, oDestinationRect);
}

/*
void EMBeViewContainer::DrawBuffer(EMRect p_oUpdateRect)
{
	if(!p_oUpdateRect.IsValid())
		return;

	if(!m_vIsDoubleBuffered)
		EMDebugger("DrawBuffer() does not make sense in a non-double-buffered view");
	BRect oDrawRect(p_oUpdateRect.m_vLeft, p_oUpdateRect.m_vTop, p_oUpdateRect.m_vRight, p_oUpdateRect.m_vBottom);
	m_opBufferView -> Sync();
	m_opPlainView -> DrawBitmap(m_opBuffer.get(), oDrawRect, oDrawRect);
}
*/

void EMBeViewContainer::DrawString(float p_vX, float p_vY, const char* p_vpText)
{
	m_opDrawView -> DrawString(p_vpText, BPoint(p_vX, p_vY));
}

void EMBeViewContainer::EnableDoubleBuffering(bool p_vEnabled)
{
	if(!m_vIsDoubleBuffered)
		EMDebugger("EnableDoubleBuffering can only be called on a view that has been created double buffered");
	
	if(!p_vEnabled)
		m_opDrawView = m_opPlainView;
	else
		m_opDrawView = m_opBufferView;
}

void EMBeViewContainer::FillRect(const EMRect p_oRect)
{
	if(!p_oRect.IsValid())
		return;

	BRect oRect;
	
	oRect.left = p_oRect.m_vLeft;
	oRect.top = p_oRect.m_vTop;
	oRect.right = p_oRect.m_vRight;
	oRect.bottom = p_oRect.m_vBottom;

	m_opDrawView -> FillRect(oRect);
}

EMRect EMBeViewContainer::Frame() const
{
//	BRect oRect = m_opDrawView -> Frame();
	// Using the plain view so that the bitmap buffer does not have to be locked
	// so often if the view is double-buffered
	BRect oRect = m_opPlainView -> Frame();
	return EMRect(oRect.left, oRect.top, oRect.right, oRect.bottom);
}
	
EMRect EMBeViewContainer::GetClippingRect() const
{
	BRegion oRegion;
	BRect oRect;
	EMRect oRect2;
	
//	m_opDrawView -> GetClippingRegion(&oRegion);
	m_opPlainView -> GetClippingRegion(&oRegion);
/*
;//cout_commented_out_4_release << "Number of clipping rects: " << oRegion.CountRects() << endl;
for(int vIndex = 0; vIndex < oRegion.CountRects(); vIndex++)
{
	oRect = oRegion.RectAt(vIndex);
	oRect2.m_vLeft = oRect.left;
	oRect2.m_vTop = oRect.top;
	oRect2.m_vRight = oRect.right;
	oRect2.m_vBottom = oRect.bottom;
	;//cout_commented_out_4_release << '\t';
	oRect2.Print();
}
*/
	oRect = oRegion.Frame();
	oRect2.m_vLeft = oRect.left;
	oRect2.m_vTop = oRect.top;
	oRect2.m_vRight = oRect.right;
	oRect2.m_vBottom = oRect.bottom;
	return oRect2;
}

EMView* EMBeViewContainer::GetLastMouseDownView()
{
	return m_opPlainView -> GetLastMouseDownView();
}

void EMBeViewContainer::GetMouse(float &p_vX, float &p_vY, bool &p_vButtonOne, bool &p_vButtonTwo, bool &p_vButtonThree) const
{
	BPoint oPoint;
	uint32 vButtons;
	
	m_opPlainView -> GetMouse(&oPoint, &vButtons, false);

	p_vX = oPoint.x;
	p_vY = oPoint.y;
	
	if((vButtons & B_PRIMARY_MOUSE_BUTTON) == B_PRIMARY_MOUSE_BUTTON)
		p_vButtonOne = true;
	else
		p_vButtonOne = false;
	if((vButtons & B_SECONDARY_MOUSE_BUTTON) == B_SECONDARY_MOUSE_BUTTON)
		p_vButtonTwo = true;
	else
		p_vButtonTwo = false;
	if((vButtons & B_TERTIARY_MOUSE_BUTTON) == B_TERTIARY_MOUSE_BUTTON)
		p_vButtonThree = true;
	else
		p_vButtonThree = false;
}

void* EMBeViewContainer::GetNativeView() const
{
	return (void*) m_opPlainView;
//	return m_opPlainView;
}

void EMBeViewContainer::Hide()
{
	m_opPlainView -> Hide();
}

void EMBeViewContainer::Invalidate(const EMRect p_oRect)
{
	if(!p_oRect.IsValid())
		return;

	BRect oRect;
	
	oRect.left = p_oRect.m_vLeft;
	oRect.top = p_oRect.m_vTop;
	oRect.right = p_oRect.m_vRight;
	oRect.bottom = p_oRect.m_vBottom;

	m_opPlainView -> Invalidate(oRect);
}

bool EMBeViewContainer::IsDoubleBuffered()
{
	return m_vIsDoubleBuffered;
}

bool EMBeViewContainer::IsHidden()
{
	return m_opPlainView -> IsHidden();
}

bool EMBeViewContainer::LockBuffer()
{
	if(!m_vIsDoubleBuffered)
		EMDebugger("LockBuffer() does not make sense in a non-double-buffered view");
	return m_opBuffer -> Lock();
}

bool EMBeViewContainer::LockWindow()
{
/*
thread_id vThreadID = m_opPlainView -> Window() -> LockingThread();
thread_id vThisThreadID = find_thread(NULL);
thread_info oThreadInfo;
thread_info oThisThreadInfo;
get_thread_info(vThisThreadID, &oThisThreadInfo);
if(vThreadID != -1)
{
get_thread_info(vThreadID, &oThreadInfo);
if(vThreadID == vThisThreadID)
;//cout_commented_out_4_release << "Before locking window (from view). Already locked by the same thread: " << oThisThreadInfo.name << endl;
else
;//cout_commented_out_4_release << "Before locking window (from view). Already locked by a thread: " << oThreadInfo.name << ", this thread: " << oThisThreadInfo.name << endl;
}
else
;//cout_commented_out_4_release << "Before locking window (from view). This thread: " << oThisThreadInfo.name << endl;
bool vResult =  m_opPlainView -> Window() -> Lock();
;//cout_commented_out_4_release << " ------------------------  Window Locked (from view) (" << vResult << ") " << oThisThreadInfo.name << " -------------------" << endl;
//EMDebugger("Window locked by view");
	return vResult;
*/
	return m_opPlainView -> Window() -> Lock();
}

void EMBeViewContainer::ReceiveAllMouseEvents(bool vReceiveAll)
{
	if(vReceiveAll)
		m_opPlainView -> SetEventMask(B_POINTER_EVENTS, B_NO_POINTER_HISTORY);
	else
		m_opPlainView -> SetEventMask(0, B_NO_POINTER_HISTORY);
}

bool EMBeViewContainer::RemoveChild(EMGUIComponent* p_opView)
{
	// Sub-views in a double-buffered view is currently not allowed
	if(m_vIsDoubleBuffered)
		EMDebugger("Removing views from a double-buffered view is not allowed");
	return m_opPlainView -> RemoveChild((BView*) p_opView -> GetNativeView());
}

void EMBeViewContainer::ScrollBy(float p_vX, float p_vY)
{
	m_opPlainView -> ScrollBy(p_vX, p_vY);
	if(m_vIsDoubleBuffered)
		m_opBufferView -> ScrollBy(p_vX, p_vY);
}

void EMBeViewContainer::ScrollTo(float p_vX, float p_vY)
{
	m_opPlainView -> ScrollTo(p_vX, p_vY);
	if(m_vIsDoubleBuffered)
	{
		m_opBufferView -> SetOrigin(-1 * p_vX, -1 * p_vY);
		m_vBufferScrollOffsetX = p_vX;
		m_vBufferScrollOffsetY = p_vY;
//		m_opBufferView -> ScrollTo(p_vX, p_vY);
	}
}

void EMBeViewContainer::SetBitmap(EMBitmap* p_opBitmap)
{
	// Setting a bitmap in a double-buffered view is currently not allowed
	if(m_vIsDoubleBuffered)
		EMDebugger("Setting a bitmap in a double-buffered view is not allowed");
	m_opPlainView -> SetViewBitmap(static_cast<BBitmap*>(p_opBitmap -> GetNativeBitmap()));
}

void EMBeViewContainer::SetDrawingMode(EMDrawingMode p_vMode)
{
	switch(p_vMode)
	{
	case EM_OP_COPY:
		m_opDrawView -> SetDrawingMode(B_OP_COPY);
		break;
	case EM_OP_ALPHA:
		m_opDrawView -> SetDrawingMode(B_OP_ALPHA);
		m_opDrawView -> SetBlendingMode(B_CONSTANT_ALPHA, B_ALPHA_OVERLAY);
		break;
	case EM_OP_INVERT:
		m_opDrawView -> SetDrawingMode(B_OP_INVERT);
		break;
	default:
		m_opDrawView -> SetDrawingMode(B_OP_COPY);
	}
}

void EMBeViewContainer::SetFont(EMFont p_vFont)
{
	switch(p_vFont)
	{
	case EM_PLAIN:
		m_opDrawView -> SetFont(be_plain_font);
		break;
	case EM_BOLD:
		m_opDrawView -> SetFont(be_bold_font);
		break;
	case EM_FIXED:
		m_opDrawView -> SetFont(be_fixed_font);
		break;
	}
}

void EMBeViewContainer::SetFrame(EMRect p_oFrame)
{
	if(Frame() == p_oFrame)
		return;
		
	if(Frame().m_vTop != p_oFrame.m_vTop || Frame().m_vLeft != p_oFrame.m_vLeft)
		m_opPlainView -> MoveTo(p_oFrame.m_vLeft, p_oFrame.m_vTop);
	if(Frame().GetWidth() != p_oFrame.GetWidth() || Frame().GetHeight() != p_oFrame.GetHeight())
	{
		m_opPlainView -> ResizeTo(p_oFrame.GetWidth(), p_oFrame.GetHeight());
/*
		if(m_vIsDoubleBuffered)
		{
if(m_opBuffer == NULL)
;//cout_commented_out_4_release << "Hmm, the buffer is NULL" << endl;
			if(m_opBuffer -> Lock())
			{
				m_opBufferView -> ResizeTo(p_oFrame.GetWidth(), p_oFrame.GetHeight());
				m_opBuffer -> Unlock();
			}
		}
*/
	}
/*
	if(m_vIsDoubleBuffered)
	{
		m_opBuffer -> RemoveChild(m_opBufferView.get());
		delete m_opBuffer.get();
		auto_ptr<BBitmap> opBuffer(new BBitmap(BRect(0, 0, p_oFrame.GetWidth(), p_oFrame.GetHeight()), B_RGB16, true));
		if(opBuffer -> Lock())
		{
			opBuffer -> AddChild(m_opBufferView.get());

			m_opBufferView -> ResizeTo(p_oFrame.GetWidth(), p_oFrame.GetHeight());
//			m_opBufferView -> MoveTo(p_oFrame.m_vLeft, p_oFrame.m_vTop);

			opBuffer -> Unlock();
		}
		m_opBuffer = opBuffer;
	}
*/
}

void EMBeViewContainer::SetHighColor(const EMColor p_vColor)
{
	rgb_color sColor;
	sColor.red = p_vColor.m_vRed;
	sColor.blue = p_vColor.m_vBlue;
	sColor.green = p_vColor.m_vGreen;
	sColor.alpha = p_vColor.m_vAlpha;

	m_opDrawView -> SetHighColor(sColor);
}

void EMBeViewContainer::SetLowColor(const EMColor p_vColor)
{
	rgb_color sColor;
	sColor.red = p_vColor.m_vRed;
	sColor.blue = p_vColor.m_vBlue;
	sColor.green = p_vColor.m_vGreen;
	sColor.alpha = p_vColor.m_vAlpha;

	m_opDrawView -> SetLowColor(sColor);
}
/*
// This method is from the game kit
void EMBeViewContainer::SetMousePosition(float p_vX, float p_vY)
{
	set_mouse_position(static_cast<int32>(p_vX), static_cast<int32>(p_vY));
}
*/
void EMBeViewContainer::SetViewColor(const EMColor p_vColor)
{
	rgb_color sColor;
	sColor.red = p_vColor.m_vRed;
	sColor.blue = p_vColor.m_vBlue;
	sColor.green = p_vColor.m_vGreen;
	sColor.alpha = p_vColor.m_vAlpha;

	m_opPlainView -> SetViewColor(sColor);
	if(m_vIsDoubleBuffered)
		m_opBufferView -> SetViewColor(sColor);
}

void EMBeViewContainer::Show()
{
	m_opPlainView -> Show();
}

void EMBeViewContainer::StrokeLine(float p_vX1, float p_vY1, float p_vX2, float p_vY2)
{
	m_opDrawView -> StrokeLine(BPoint(p_vX1, p_vY1), BPoint(p_vX2, p_vY2));
}

void EMBeViewContainer::StrokeRect(EMRect p_oRect)
{
	if(!p_oRect.IsValid())
		return;

	m_opDrawView -> StrokeRect(BRect(p_oRect.m_vLeft, p_oRect.m_vTop, p_oRect.m_vRight, p_oRect.m_vBottom));
}

void EMBeViewContainer::Sync()
{
	m_opDrawView -> Sync();
}

void EMBeViewContainer::UpdateBufferFrame()
{
/*
	if(m_opBuffer -> Lock())
	{
		m_opBufferView -> ResizeTo((m_opPlainView -> Bounds()).Width(), (m_opPlainView -> Bounds()).Height());
;//cout_commented_out_4_release << "EMBeViewContainer::UpdateBufferFrame: m_opBufferView: ";
;//cout_commented_out_4_release << (m_opPlainView -> Bounds()).Width() << ", " << (m_opPlainView -> Bounds()).Height() << endl;
		m_opBuffer -> Unlock();
	}
*/
}

bool EMBeViewContainer::WindowIsLocked()
{
	return m_opPlainView -> Window() -> IsLocked();
}

void EMBeViewContainer::UnlockBuffer()
{
	if(!m_vIsDoubleBuffered)
		EMDebugger("UnlockBuffer() does not make sense in a non-double-buffered view");
	m_opBuffer -> Unlock();
}

void EMBeViewContainer::UnlockWindow()
{
//;//cout_commented_out_4_release << " ------------------------  Window Unlocked (from view) -------------------" << endl;
//;//cout_commented_out_4_release << " ------------------------  Window Unlocked (from view) -------------------" << endl;
	m_opPlainView -> Window() -> Unlock();
}

#endif
