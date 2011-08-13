#include "EMWinView.h"

#ifdef PLATFORM_WINDOWS

#include "EMApplication.h"
#include "EMBitmap.h"
#include "EMFactory.h"
#include "EMGUIUtilities.h"
#include "EMKeyboard.h"
#include "EMMessages.h"
#include "EMMouseCursor.h"
#include "EMUniqueID.h"
#include "EMWinButton.h"
#include "EMWinCheckBox.h"
#include "EMWinComboBox.h"
#include "EMWinGroupBox.h"
#include "EMWinListBox.h"
#include "EMWinListViewControl.h"
#include "EMWinMessages.h"
#include "EMWinMouseCursor.h"
#include "EMWinRadioButton.h"
#include "EMWinScrollbar.h"
#include "EMWinTabControl.h"
#include "EMWinTextControl.h"
#include "EMWinToolTip.h"
#include "EMWinTreeViewControl.h"
#include "EMWinWindow.h"

#include <string>
#include <utility>

HFONT EMWinView::m_opPlainFont = NULL;
HFONT EMWinView::m_opBoldFont = NULL;
HFONT EMWinView::m_opFixedFont = NULL;
EMMouseCursorType EMWinView::m_eDefaultMouseCursor = EM_MOUSE_CURSOR_ARROW;

const int MAX_NR_MOUSE_CURSORS = 50;

//bool EMWinView::m_vMouseButtonOne = false;
//bool EMWinView::m_vMouseButtonTwo = false;
//bool EMWinView::m_vMouseButtonThree = false;
//bool EMWinView::m_vMouseHookIsSet = false;
//HHOOK EMWinView::m_opMouseProcHook = NULL;

vector<EMWinMouseCursor*> EMWinView::m_oMouseCursors(MAX_NR_MOUSE_CURSORS, NULL);

EMWinView::EMWinView(const EMRect p_oFrame, uint32 p_vResizingMode, bool p_vDoubleBuffered) :
m_eCurrentMouseCursor(EM_MOUSE_CURSOR_UNKNOWN),
//m_eDefaultMouseCursor(EM_MOUSE_CURSOR_ARROW),
m_eFont(EM_PLAIN),
m_oBackgroundColor(0, 0, 0, 0),
m_oBounds(EMRect(0, 0, p_oFrame.m_vRight - p_oFrame.m_vLeft, p_oFrame.m_vBottom - p_oFrame.m_vTop)),
m_oFrame(p_oFrame),
//m_oMouseCursors(MAX_NR_MOUSE_CURSORS, NULL),
m_opClipRegion(NULL),
//m_opCurrentBrush(CreateSolidBrush(RGB(0, 0, 0))),
//m_opCurrentPen(CreatePen(PS_SOLID, 1, RGB(0, 0, 0))),
m_opPlainPaintHandle(NULL),
m_opWindowWindow(NULL),
m_vBeginPaintCount(0),
//m_vBufferScrollOffsetX(0),
//m_vBufferScrollOffsetY(0),
m_vCurrentAlpha(255),
m_vIsDoubleBuffered(p_vDoubleBuffered),
m_vResizingMode(p_vResizingMode),
m_vScrollOffsetX(0),
m_vScrollOffsetY(0)
{
	string oUniqueName = EMUniqueID::GetUniqueName();

	WNDCLASSEX sWindowClass;
	sWindowClass.cbSize = sizeof(WNDCLASSEX);
	sWindowClass.style = CS_DBLCLKS;// | CS_HREDRAW | CS_VREDRAW;
	sWindowClass.lpfnWndProc = GlobalWindowProcedure;
	sWindowClass.cbClsExtra = 0;
	sWindowClass.cbWndExtra = 0;       
	sWindowClass.hInstance = EMApplication::GetApplicationInstance();
	sWindowClass.hIcon = NULL;
	sWindowClass.hCursor = NULL;//LoadCursor(NULL, IDC_ARROW);
	sWindowClass.hbrBackground = NULL; //(HBRUSH) (COLOR_WINDOW + 1);
	sWindowClass.lpszMenuName = NULL;
	sWindowClass.lpszClassName = oUniqueName.c_str();
	sWindowClass.hIconSm = NULL;

	if(RegisterClassEx(&sWindowClass))
	{
		m_opWindowHandle = CreateWindow(oUniqueName.c_str(), 
		oUniqueName.c_str(), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, m_oFrame.m_vLeft,  m_oFrame.m_vTop, m_oFrame.m_vRight - m_oFrame.m_vLeft + 1, 
		m_oFrame.m_vBottom - m_oFrame.m_vTop + 1, EMWinWindow::GetDummyWindowHandle(), NULL, EMApplication::GetApplicationInstance(), NULL);

		SetWindowLong(m_opWindowHandle, GWL_USERDATA, reinterpret_cast<int32>(this));

//eo << "View created: width: " << m_oFrame.GetWidth() << ", height: " << m_oFrame.GetHeight() << ef;
	}
	else
		EMDebugger("Could not register windows class");

	if(m_vIsDoubleBuffered)
	{
		m_opPlainPaintHandle = GetDC(m_opWindowHandle);
		m_opDoubleBufferedPaintHandle = CreateCompatibleDC(m_opPlainPaintHandle);
		m_opDoubleBufferingBitmap = CreateCompatibleBitmap(m_opPlainPaintHandle, 1600, 1400);
		SelectObject(m_opDoubleBufferedPaintHandle, m_opDoubleBufferingBitmap);
		ReleaseDC(m_opWindowHandle, m_opPlainPaintHandle);
		m_opPlainPaintHandle = NULL;
	}

	if(m_opPlainFont == NULL)
	{
		m_opPlainPaintHandle = GetDC(m_opWindowHandle);
//		int vFontHeight = -1 * MulDiv(10, GetDeviceCaps(m_opPlainPaintHandle, LOGPIXELSY), 72);
		// FIXME: Free the fonts at an appropriate time when quitting
		m_opPlainFont = CreateFont(11, 5, 0, 0, 0, false, false, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_EMBEDDED, PROOF_QUALITY/*ANTIALIASED_QUALITY*/, FF_DONTCARE, "Tahoma"/*NULL*/);
		ReleaseDC(m_opWindowHandle, m_opPlainPaintHandle);
		m_opPlainPaintHandle = NULL;
	}
	if(m_opFixedFont == NULL)
	{
		m_opPlainPaintHandle = GetDC(m_opWindowHandle);
//		int vFontHeight = -1 * MulDiv(10, GetDeviceCaps(m_opPlainPaintHandle, LOGPIXELSY), 72);
		// FIXME: Free the fonts at an appropriate time when quitting
		m_opFixedFont = CreateFont(13, 5, 0, 0, 0, false, false, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_EMBEDDED, PROOF_QUALITY/*ANTIALIASED_QUALITY*/, FF_MODERN, NULL);
		ReleaseDC(m_opWindowHandle, m_opPlainPaintHandle);
		m_opPlainPaintHandle = NULL;
	}
}

EMWinView::~EMWinView()
{
	if(m_opPlainPaintHandle == NULL)
		m_opPlainPaintHandle = GetDC(m_opWindowHandle);
	SelectObject(m_opPlainPaintHandle, GetStockObject(SYSTEM_FONT));
//	if(DeleteObject(m_opPlainFont) == 0)
//eo << "~EMWinView: DeleteObject failed" << ef;
	ReleaseDC(m_opWindowHandle, m_opPlainPaintHandle);
m_opPlainPaintHandle = NULL; // Should not be necessary, or?
	if(m_vIsDoubleBuffered)
	{
		DeleteObject(m_opDoubleBufferingBitmap);
		DeleteDC(m_opDoubleBufferedPaintHandle);
	}
	SetWindowLong(m_opWindowHandle, GWL_USERDATA, 0);
	DestroyWindow(m_opWindowHandle);
	m_opWindowHandle = NULL;
}

void EMWinView::AddChild(EMGUIComponent* p_opView)
{
//	AssertWindowLocked();
	if(p_opView == NULL)
		EMDebugger("Child is NULL");
	if(m_opWindowWindow == NULL)
		EMDebugger("Base window is NULL");
	if(dynamic_cast<EMWinView*>(p_opView))
		(dynamic_cast<EMWinView*>(p_opView)) -> SetWindowWindow(m_opWindowWindow);
#ifndef EM_BASIC_COMPONENTS
	else if(dynamic_cast<EMWinGroupBox*>(p_opView))
		(dynamic_cast<EMWinGroupBox*>(p_opView)) -> SetWindowWindow(m_opWindowWindow);
	else if(dynamic_cast<EMWinTabControl*>(p_opView))
		(dynamic_cast<EMWinTabControl*>(p_opView)) -> SetWindowWindow(m_opWindowWindow);
	else if(dynamic_cast<EMWinScrollbar*>(p_opView))
		(dynamic_cast<EMWinScrollbar*>(p_opView)) -> SetWindowWindow(m_opWindowWindow);
#endif // EM_BASIC_COMPONENTS
	p_opView -> AddToComponent(this);
	m_oChildComponents.push_back(p_opView);
}

bool EMWinView::AddToComponent(EMGUIComponent* p_opComponent)
{
	HWND opParent;
	if(dynamic_cast<EMWinWindow*>(p_opComponent))
	{
		EMWinWindow* opWindow = dynamic_cast<EMWinWindow*>(p_opComponent);
		opParent = opWindow -> GetWindowHandle();
	}
	else if(dynamic_cast<EMWinView*>(p_opComponent))
	{
		EMWinView* opView = dynamic_cast<EMWinView*>(p_opComponent);
		opParent = opView -> GetWindowHandle();
	}
	else if(dynamic_cast<EMWinTabControl*>(p_opComponent))
	{
		EMWinTabControl* opView = dynamic_cast<EMWinTabControl*>(p_opComponent);
		opParent = static_cast<HWND>(opView -> GetNativeView());
	}
	else
	{
		EMDebugger("The parent is neither a window nor a view");
		return false;
	}
/*
	string oUniqueName = EMGUIUtilities::Instance() -> GetUniqueName();

	WNDCLASSEX sWindowClass;
	sWindowClass.cbSize = sizeof(WNDCLASSEX);
	sWindowClass.style = CS_DBLCLKS;// | CS_HREDRAW | CS_VREDRAW;
	sWindowClass.lpfnWndProc = GlobalWindowProcedure;
	sWindowClass.cbClsExtra = 0;
	sWindowClass.cbWndExtra = 0;       
	sWindowClass.hInstance = EMApplication::GetApplicationInstance();
	sWindowClass.hIcon = NULL;
	sWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	sWindowClass.hbrBackground = NULL;//(HBRUSH) (COLOR_WINDOW + 1);
	sWindowClass.lpszMenuName = NULL;
	sWindowClass.lpszClassName = oUniqueName.c_str();
	sWindowClass.hIconSm = NULL;

	if(RegisterClassEx(&sWindowClass))
	{
		m_opWindowHandle = CreateWindow(oUniqueName.c_str(), 
		oUniqueName.c_str(), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, m_oFrame.m_vLeft,  m_oFrame.m_vTop, m_oFrame.m_vRight - m_oFrame.m_vLeft + 1, 
		m_oFrame.m_vBottom - m_oFrame.m_vTop + 1, opParent, NULL, EMApplication::GetApplicationInstance(), NULL);

		SetWindowLong(m_opWindowHandle, GWL_USERDATA, reinterpret_cast<int32>(this));

eo << "View created: width: " << m_oFrame.GetWidth() << ", height: " << m_oFrame.GetHeight() << ef;
	}
	else
		EMDebugger("Could not register windows class");

//	ShowWindow(m_vWindowHandle, SW_SHOW);

	if(m_vIsDoubleBuffered)
	{
		m_opPlainPaintHandle = GetDC(m_opWindowHandle);
		m_opDoubleBufferedPaintHandle = CreateCompatibleDC(m_opPlainPaintHandle);
		m_opDoubleBufferingBitmap = CreateCompatibleBitmap(m_opPlainPaintHandle, 1600, 1200);
		SelectObject(m_opDoubleBufferedPaintHandle, m_opDoubleBufferingBitmap);
		ReleaseDC(m_opWindowHandle, m_opPlainPaintHandle);
	}
*/

	SetParent(m_opWindowHandle, opParent);
	SetWindowPos(m_opWindowHandle, NULL, m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.GetWidth() + 1, m_oFrame.GetHeight() + 1, SWP_NOZORDER);	

	return true;
}

void EMWinView::AssertWindowLocked() const
{
	if(m_opWindowWindow != NULL) // FIXME: Design away this check?
		if(!(m_opWindowWindow -> IsLocked()))
			EMDebugger("The window must be locked before calling this method");
}

void EMWinView::BeginPaint()
{
	AssertWindowLocked();
	BeginPaint(false);
}

void EMWinView::BeginPaint(bool p_vTriggeredFromInvalidate)
{
if(m_vBeginPaintCount > 0)
{
eo << "m_vBeginPaintCount > 0: " << m_vBeginPaintCount << ef;
//EMDebugger("Arrhh, m_vBeginPaintCount > 0");
}
	m_vBeginPaintCount++;
	if(m_vBeginPaintCount > 1)
		return; // BeginPaint is already called at least once
//m_opPlainPaintHandle = ::BeginPaint(m_opWindowHandle, &m_sPaintStruct);
//eo << "BeginPaint (EMWinView)" << ef;
	if(p_vTriggeredFromInvalidate)
	{
		m_opPlainPaintHandle = ::BeginPaint(m_opWindowHandle, &m_sPaintStruct);

		if(m_sPaintStruct.rcPaint.left == 0 && m_sPaintStruct.rcPaint.top == 0 && m_sPaintStruct.rcPaint.right == 0 && m_sPaintStruct.rcPaint.bottom == 0)
			m_oClippingRect = EMRect();
		else
		{
			m_oClippingRect.m_vLeft = m_sPaintStruct.rcPaint.left + m_vScrollOffsetX;
			m_oClippingRect.m_vTop = m_sPaintStruct.rcPaint.top + m_vScrollOffsetY;
			m_oClippingRect.m_vRight = m_sPaintStruct.rcPaint.right - 1 + m_vScrollOffsetX;
			m_oClippingRect.m_vBottom = m_sPaintStruct.rcPaint.bottom - 1 + m_vScrollOffsetY;
		}
	}
	else
	{
		if(m_opPlainPaintHandle != NULL)
			EMDebugger("EMWinView::BeginPaint is already called");
		m_opPlainPaintHandle = GetDC(m_opWindowHandle);
		m_oClippingRect = EMRect();//m_oBounds;
	}

	m_eDrawingMode = EM_OP_COPY;

//	SetViewportOrgEx(m_opPlainPaintHandle, -1 * m_vScrollOffsetX, -1 * m_vScrollOffsetY, NULL);
	if(m_vIsDoubleBuffered)
	{
		SetTextAlign(m_opDoubleBufferedPaintHandle, TA_NOUPDATECP | TA_BASELINE);
//		SetViewportOrgEx(m_opDoubleBufferedPaintHandle, -1 * m_vScrollOffsetX, -1 * m_vScrollOffsetY, NULL);
		SelectObject(m_opDoubleBufferedPaintHandle, CreatePen(PS_SOLID, 1, RGB(0, 0, 0)));
		SelectObject(m_opDoubleBufferedPaintHandle, CreateSolidBrush(RGB(0, 0, 0)));
	}
	else
	{
		SetTextAlign(m_opPlainPaintHandle, TA_NOUPDATECP | TA_BASELINE);
		SelectObject(m_opPlainPaintHandle, CreatePen(PS_SOLID, 1, RGB(0, 0, 0)));
		SelectObject(m_opPlainPaintHandle, CreateSolidBrush(RGB(0, 0, 0)));
	}

}

EMRect EMWinView::Bounds() const
{
//	AssertWindowLocked(); // FIXME: Shouldn't this require a lock?
	return m_oBounds;
}

void EMWinView::CalculateStringHeight(const char* p_vpString, int32 &p_vAscent, int32 &p_vDescent)
{
/*
	AssertWindowLocked();
	TEXTMETRIC sMetrics;
	bool vIsDrawing(false);
	if(m_opPlainPaintHandle != NULL)
		vIsDrawing = true;
	else
		m_opPlainPaintHandle = GetDC(m_opWindowHandle);
	GetTextMetrics(m_opPlainPaintHandle, &sMetrics);
	if(!vIsDrawing)
		ReleaseDC(m_opWindowHandle, m_opPlainPaintHandle);
	p_vAscent = sMetrics.tmAscent;
	p_vDescent = sMetrics.tmDescent;
*/
	AssertWindowLocked();
	TEXTMETRIC sMetrics;
	bool vIsDrawing(false);
	HDC opPaintHandle = GetDC(m_opWindowHandle);
	SetFont(m_eFont);
	GetTextMetrics(opPaintHandle, &sMetrics);
	if(ReleaseDC(m_opWindowHandle, opPaintHandle) == 0)
eo << "CalculateStringHeight: ReleaseDC failed" << ef;
	p_vAscent = sMetrics.tmAscent;
	p_vDescent = sMetrics.tmDescent;
}

int32 EMWinView::CalculateStringWidth(const char* p_vpString)
{
	AssertWindowLocked();
	SIZE sSize;
	bool vIsDrawing(false);
	if(m_opPlainPaintHandle != NULL)
		vIsDrawing = true;
	else
	{
		if(m_vIsDoubleBuffered)
			;//m_opDoubleBufferedPaintHandle = GetDC(m_opWindowHandle);
		else
			m_opPlainPaintHandle = GetDC(m_opWindowHandle);
		//SetFont(m_eFont);
	}
	SetFont(m_eFont);
	if(m_vIsDoubleBuffered)
		GetTextExtentPoint32(m_opDoubleBufferedPaintHandle, p_vpString, strlen(p_vpString), &sSize);
	else
		GetTextExtentPoint32(m_opPlainPaintHandle, p_vpString, strlen(p_vpString), &sSize);
	if(!vIsDrawing)
	{
		if(m_vIsDoubleBuffered)
		{
			//ReleaseDC(m_opWindowHandle, m_opDoubleBufferedPaintHandle);
			//m_opDoubleBufferedPaintHandle = NULL;
		}
		else
		{
			ReleaseDC(m_opWindowHandle, m_opPlainPaintHandle);
			m_opPlainPaintHandle = NULL;
		}
	}
	return sSize.cx;

/*
	SIZE sSize;
	HDC opPaintHandle = GetDC(m_opWindowHandle);

	SelectObject(opPaintHandle, m_opPlainFont);
	GetTextExtentPoint32(opPaintHandle, p_vpString, strlen(p_vpString), &sSize);
	if(ReleaseDC(m_opWindowHandle, opPaintHandle) == 0)
eo << "CalculateStringWidth: ReleaseDC failed" << ef;
	return sSize.cx;
*/
}

void EMWinView::CleanUp()
{
	DeleteObject(m_opPlainFont);
	DeleteObject(m_opFixedFont);

	for(int vIndex = 0; vIndex < MAX_NR_MOUSE_CURSORS; vIndex++)
		if(m_oMouseCursors[vIndex] != NULL)
			delete m_oMouseCursors[vIndex];

//	UnhookWindowsHookEx(m_opMouseProcHook);
}

void EMWinView::ConstrainClippingRect(EMRect p_oRect)
{
//if(p_oRect.m_vRight - m_vScrollOffsetX > 5000 || p_oRect.m_vLeft - m_vScrollOffsetX > 5000)
//EMDebugger("Aha!");
	AssertWindowLocked();

	//p_oRect.OffsetBy(-1 * m_vScrollOffsetX, -1 * m_vScrollOffsetY);
	if(!p_oRect.Intersects(m_oBounds))
		p_oRect = EMRect();

	if(!p_oRect.Contains(m_oBounds))
	{
		if(p_oRect.m_vRight > m_oBounds.m_vRight)
			p_oRect.m_vRight = m_oBounds.m_vRight + 1;
		if(p_oRect.m_vBottom > m_oBounds.m_vBottom)
			p_oRect.m_vBottom = m_oBounds.m_vBottom + 1;
		if(p_oRect.m_vTop < m_oBounds.m_vTop)
			p_oRect.m_vTop = m_oBounds.m_vLeft - 1;
		if(p_oRect.m_vLeft < m_oBounds.m_vLeft)
			p_oRect.m_vLeft = m_oBounds.m_vLeft - 1;
	}

	if(m_opClipRegion != NULL)
	{
		if(DeleteObject(m_opClipRegion) == 0)
eo << "ConstrainClippingRect: DeleteObject failed" << ef;
		m_opClipRegion = NULL;
	}
	if(!p_oRect.IsValid())
	{
		SelectClipRgn(m_opPlainPaintHandle, NULL);
		if(m_vIsDoubleBuffered)
			SelectClipRgn(m_opDoubleBufferedPaintHandle, NULL);
	}
	else
	{
		m_opClipRegion = CreateRectRgn(p_oRect.m_vLeft - m_vScrollOffsetX, p_oRect.m_vTop - m_vScrollOffsetY, p_oRect.m_vRight - m_vScrollOffsetX + 1, p_oRect.m_vBottom - m_vScrollOffsetY + 1);
		SelectClipRgn(m_opPlainPaintHandle, m_opClipRegion);
		if(m_vIsDoubleBuffered)
			SelectClipRgn(m_opDoubleBufferedPaintHandle, m_opClipRegion);
	}
}

void EMWinView::ConvertToScreenCoordinates(int32 &p_vX, int32 &p_vY)
{
	POINT sPoint;
	sPoint.x = p_vX - m_vScrollOffsetX;
	sPoint.y = p_vY - m_vScrollOffsetY;
	ClientToScreen(m_opWindowHandle, &sPoint);
	p_vX = sPoint.x;
	p_vY = sPoint.y;
}

void EMWinView::DrawAndUnlockBuffer(EMRect p_oUpdateRect)
{

	AssertWindowLocked();
	if(!m_vIsDoubleBuffered)
		EMDebugger("The view is not double buffered");
	BitBlt(m_opPlainPaintHandle, p_oUpdateRect.m_vLeft - m_vScrollOffsetX, p_oUpdateRect.m_vTop - m_vScrollOffsetY, p_oUpdateRect.GetWidth() + 1, p_oUpdateRect.GetHeight() + 1, 
			m_opDoubleBufferedPaintHandle, p_oUpdateRect.m_vLeft - m_vScrollOffsetX, p_oUpdateRect.m_vTop - m_vScrollOffsetY, SRCCOPY);

}

void EMWinView::DrawBitmap(const EMBitmap* p_opBitmap)
{
	DrawBitmap(p_opBitmap, 0, 0);
}

void EMWinView::DrawBitmap(const EMBitmap* p_opBitmap, int32 p_vX, int32 p_vY)
{

	AssertWindowLocked();
	EMRect oRect(p_opBitmap -> Bounds());
	HDC opMemDC = CreateCompatibleDC(m_opPlainPaintHandle);
	HGDIOBJ upOldObject = SelectObject(opMemDC, p_opBitmap -> GetNativeBitmap());
	if(m_eDrawingMode == EM_OP_ALPHA)
	{
		BLENDFUNCTION sBlendFunction = { AC_SRC_OVER, 0, m_vCurrentAlpha, 0 };
		if(m_vIsDoubleBuffered)
			AlphaBlend(m_opDoubleBufferedPaintHandle, p_vX - m_vScrollOffsetX, p_vY - m_vScrollOffsetY, oRect.GetWidth() + 1, oRect.GetHeight() + 1, opMemDC, 0, 0, oRect.GetWidth() + 1, oRect.GetHeight() + 1, sBlendFunction);
		else
			AlphaBlend(m_opPlainPaintHandle, p_vX - m_vScrollOffsetX, p_vY - m_vScrollOffsetY, oRect.GetWidth() + 1, oRect.GetHeight() + 1, opMemDC, 0, 0, 1, 1, sBlendFunction);
	}
	else
	{
		if(m_vIsDoubleBuffered)
			BitBlt(m_opDoubleBufferedPaintHandle, p_vX - m_vScrollOffsetX, p_vY - m_vScrollOffsetY, oRect.GetWidth() + 1, oRect.GetHeight() + 1, opMemDC, 0, 0, SRCCOPY);
		else
			BitBlt(m_opPlainPaintHandle, p_vX - m_vScrollOffsetX, p_vY - m_vScrollOffsetY, oRect.GetWidth() + 1, oRect.GetHeight() + 1, opMemDC, 0, 0, SRCCOPY);
	}
	SelectObject(opMemDC, upOldObject);
	if(DeleteDC(opMemDC) == 0)
eo << "DrawBitmap: DeleteDC failed" << ef;
	return;

}

void EMWinView::DrawBitmap(const EMBitmap* p_opBitmap, EMRect p_oSource, EMRect p_oDestination)
{

	AssertWindowLocked();
	EMRect oRect(p_opBitmap -> Bounds());
	HDC opMemDC = CreateCompatibleDC(m_opPlainPaintHandle);
	HGDIOBJ upOldObject = SelectObject(opMemDC, p_opBitmap -> GetNativeBitmap());
	if(m_eDrawingMode == EM_OP_ALPHA)
	{
		BLENDFUNCTION sBlendFunction = { AC_SRC_OVER, 0, m_vCurrentAlpha, 0 };
		if(m_vIsDoubleBuffered)
			AlphaBlend(m_opDoubleBufferedPaintHandle, p_oDestination.m_vLeft - m_vScrollOffsetX, p_oDestination.m_vTop - m_vScrollOffsetY, p_oDestination.GetWidth(), p_oDestination.GetHeight(), opMemDC, 0, 0, 
						p_oSource.GetWidth(), p_oSource.GetHeight(), sBlendFunction);
		else
			AlphaBlend(m_opPlainPaintHandle, p_oDestination.m_vLeft - m_vScrollOffsetX, p_oDestination.m_vTop - m_vScrollOffsetY, p_oDestination.GetWidth(), p_oDestination.GetHeight(), opMemDC, 0, 0, 
						p_oSource.GetWidth(), p_oSource.GetHeight(), sBlendFunction);
	}
	else
	{
		if(m_vIsDoubleBuffered)
			StretchBlt(m_opDoubleBufferedPaintHandle, p_oDestination.m_vLeft - m_vScrollOffsetX, p_oDestination.m_vTop - m_vScrollOffsetY, p_oDestination.GetWidth(), p_oDestination.GetHeight(), opMemDC, 0, 0, 
						p_oSource.GetWidth(), p_oSource.GetHeight(), SRCCOPY);
		else
			StretchBlt(m_opPlainPaintHandle, p_oDestination.m_vLeft - m_vScrollOffsetX, p_oDestination.m_vTop - m_vScrollOffsetY, p_oDestination.GetWidth(), p_oDestination.GetHeight(), opMemDC, 0, 0, 
						p_oSource.GetWidth(), p_oSource.GetHeight(), SRCCOPY);
	}
	SelectObject(opMemDC, upOldObject);
	if(DeleteDC(opMemDC) == 0)
eo << "DrawBitmap (stretch): DeleteDC failed" << ef;

}

void EMWinView::DrawBuffer(EMRect p_oUpdateRect)
{
	// FIXME: Implement this
}

// FIXME: Consider wrapping issues here?
void EMWinView::DrawString(int32 p_vX, int32 p_vY, const char* p_vpText)
{
	AssertWindowLocked();

	// To prevent wrapping (if windows uses 16-bit values for the coordinates, wouldn't surprise me)
	if(p_vX - m_vScrollOffsetX > 10000) // Don't think anyone has a screen resolution of 10000x...
		return;
	if(p_vY - m_vScrollOffsetY > 10000)
		return;

	SetFont(m_eFont);
	if(m_vIsDoubleBuffered)
	{
		SetBkMode(m_opDoubleBufferedPaintHandle, TRANSPARENT);
		TextOut(m_opDoubleBufferedPaintHandle, p_vX - m_vScrollOffsetX, p_vY - m_vScrollOffsetY, p_vpText, strlen(p_vpText));
		SetBkMode(m_opDoubleBufferedPaintHandle, OPAQUE);
	}
	else
	{
		SetBkMode(m_opPlainPaintHandle, TRANSPARENT);
		TextOut(m_opPlainPaintHandle, p_vX - m_vScrollOffsetX, p_vY - m_vScrollOffsetY, p_vpText, strlen(p_vpText));
		SetBkMode(m_opPlainPaintHandle, OPAQUE);
	}

}

void EMWinView::EnableDoubleBuffering(bool p_vEnabled)
{
	if(m_vIsDoubleBuffered == p_vEnabled)
		return;

	if(m_vIsDoubleBuffered)
	{
		if(DeleteObject(SelectObject(m_opDoubleBufferedPaintHandle, GetStockObject(BLACK_PEN))) == 0)
eo << "EndPaint: DeleteObject failed" << ef;
		if(DeleteObject(SelectObject(m_opDoubleBufferedPaintHandle, GetStockObject(BLACK_BRUSH))) == 0)
eo << "EndPaint: DeleteObject failed" << ef;
	}
	else
	{
		if(DeleteObject(SelectObject(m_opPlainPaintHandle, GetStockObject(BLACK_PEN))) == 0)
eo << "EndPaint: DeleteObject failed" << ef;
		if(DeleteObject(SelectObject(m_opPlainPaintHandle, GetStockObject(BLACK_BRUSH))) == 0)
eo << "EndPaint: DeleteObject failed" << ef;
	}

	m_vIsDoubleBuffered = p_vEnabled;
}

void EMWinView::EndPaint()
{
	EndPaint(false);
}

void EMWinView::EndPaint(bool p_vTriggeredFromInvalidate)
{
//::EndPaint(m_opWindowHandle, &m_sPaintStruct);
//eo << "EndPaint (EMWinView)" << ef;
	m_vBeginPaintCount--;
	if(m_vBeginPaintCount > 0)
		return; // BeginPaint is still called more times than EndPaint

	if(m_vIsDoubleBuffered)
	{
		if(DeleteObject(SelectObject(m_opDoubleBufferedPaintHandle, GetStockObject(BLACK_PEN))) == 0)
eo << "EndPaint: DeleteObject failed" << ef;
		if(DeleteObject(SelectObject(m_opDoubleBufferedPaintHandle, GetStockObject(BLACK_BRUSH))) == 0)
eo << "EndPaint: DeleteObject failed" << ef;
	}
	else
	{
		if(DeleteObject(SelectObject(m_opPlainPaintHandle, GetStockObject(BLACK_PEN))) == 0)
eo << "EndPaint: DeleteObject failed" << ef;
		if(DeleteObject(SelectObject(m_opPlainPaintHandle, GetStockObject(BLACK_BRUSH))) == 0)
eo << "EndPaint: DeleteObject failed" << ef;
	}

	if(p_vTriggeredFromInvalidate)
		::EndPaint(m_opWindowHandle, &m_sPaintStruct);
	else
		ReleaseDC(m_opWindowHandle, m_opPlainPaintHandle);

	m_oClippingRect = EMRect();
//	ReleaseDC(m_opWindowHandle, m_opPlainPaintHandle); // Needed?

	m_opPlainPaintHandle = NULL;
}

void EMWinView::FillRect(EMRect p_oRect)
{
	AssertWindowLocked();

//if(p_oRect.m_vRight - m_vScrollOffsetX > 5000 || p_oRect.m_vLeft - m_vScrollOffsetX > 5000)
//EMDebugger("Aha!");
	/*
	if(p_oRect.m_vLeft > m_oBounds.m_vRight)
		return;
	if(p_oRect.m_vTop > m_oBounds.m_vBottom)
		return;
	if(p_oRect.m_vRight < m_oBounds.m_vLeft)
		return;
	if(p_oRect.m_vBottom < m_oBounds.m_vTop)
		return;
	*/
	if(p_oRect.m_vRight > m_oBounds.m_vRight)
		p_oRect.m_vRight = m_oBounds.m_vRight;
	if(p_oRect.m_vBottom > m_oBounds.m_vBottom)
		p_oRect.m_vBottom = m_oBounds.m_vBottom;
	if(p_oRect.m_vTop < m_oBounds.m_vTop)
		p_oRect.m_vTop = m_oBounds.m_vLeft;
	if(p_oRect.m_vLeft < m_oBounds.m_vLeft)
		p_oRect.m_vLeft = m_oBounds.m_vLeft;

	if(!p_oRect.IsValid())
		return;
		

//eo << "Filling rect: left: " << p_oRect.m_vLeft << ", top: " << p_oRect.m_vTop << ", right: " << p_oRect.m_vRight + 1 << ", bottom: " << p_oRect.m_vBottom + 1 << ef;
	RECT sRect;
/*
	if(m_eDrawingMode == EM_OP_ALPHA && p_oRect.m_vLeft < 50)
	{
if(m_vIsDoubleBuffered)
eo << "Alpha 1 1: " << GetTickCount() << ef;
		HDC opAlphaPaintHandle = CreateCompatibleDC(m_opPlainPaintHandle);
		HBITMAP opAlphaBitmap = CreateCompatibleBitmap(opAlphaPaintHandle, p_oRect.m_vRight - p_oRect.m_vLeft + 1, p_oRect.m_vBottom - p_oRect.m_vTop + 1);
		SelectObject(opAlphaPaintHandle, opAlphaBitmap);
if(m_vIsDoubleBuffered)
eo << "Alpha 1 2: " << GetTickCount() << ef;
		if(SetRect(&sRect, 0, 0, p_oRect.m_vRight - p_oRect.m_vLeft + 1, p_oRect.m_vBottom - p_oRect.m_vTop + 1) == 0)
eo << "FillRect: SetRect failed" << ef;
		::FillRect(opAlphaPaintHandle, &sRect, m_opCurrentBrush);
if(m_vIsDoubleBuffered)
eo << "Alpha 1 3: " << GetTickCount() << ef;
		BLENDFUNCTION sBlendFunction = { AC_SRC_OVER, 0, m_vCurrentAlpha, 0 };
		if(m_vIsDoubleBuffered)
			AlphaBlend(m_opDoubleBufferedPaintHandle, p_oRect.m_vLeft, p_oRect.m_vTop, p_oRect.m_vRight - p_oRect.m_vLeft + 1, p_oRect.m_vBottom - p_oRect.m_vTop + 1, opAlphaPaintHandle, 0, 0, p_oRect.m_vRight - p_oRect.m_vLeft + 1, p_oRect.m_vBottom - p_oRect.m_vTop + 1, sBlendFunction);
		else
			AlphaBlend(m_opPlainPaintHandle, p_oRect.m_vLeft, p_oRect.m_vTop, p_oRect.m_vRight - p_oRect.m_vLeft + 1, p_oRect.m_vBottom - p_oRect.m_vTop + 1, opAlphaPaintHandle, 0, 0, p_oRect.m_vRight - p_oRect.m_vLeft + 1, p_oRect.m_vBottom - p_oRect.m_vTop + 1, sBlendFunction);
if(m_vIsDoubleBuffered)
eo << "Alpha 1 4: " << GetTickCount() << ef;
		if(DeleteDC(opAlphaPaintHandle) == 0)
eo << "FillRect: DeleteDC failed" << ef;
		if(DeleteObject(opAlphaBitmap) == 0)
eo << "FillRect: DeleteObject failed" << ef;
if(m_vIsDoubleBuffered)
eo << "Alpha 1 5: " << GetTickCount() << ef;
	}
*/

	if(m_eDrawingMode == EM_OP_ALPHA)
	{
		HDC opAlphaPaintHandle = CreateCompatibleDC(m_opPlainPaintHandle);
		HBITMAP opAlphaBitmap = CreateCompatibleBitmap(opAlphaPaintHandle, 1, 1);
		SelectObject(opAlphaPaintHandle, opAlphaBitmap);
		if(SetRect(&sRect, 0, 0, 1, 1) == 0)
eo << "FillRect: SetRect failed" << ef;
		::FillRect(opAlphaPaintHandle, &sRect, m_opCurrentBrush);
		BLENDFUNCTION sBlendFunction = { AC_SRC_OVER, 0, m_vCurrentAlpha, 0 };
		if(m_vIsDoubleBuffered)
			AlphaBlend(m_opDoubleBufferedPaintHandle, p_oRect.m_vLeft - m_vScrollOffsetX, p_oRect.m_vTop - m_vScrollOffsetY, p_oRect.m_vRight - p_oRect.m_vLeft + 1, p_oRect.m_vBottom - p_oRect.m_vTop + 1, opAlphaPaintHandle, 0, 0, 1, 1, sBlendFunction);
		else
			AlphaBlend(m_opPlainPaintHandle, p_oRect.m_vLeft - m_vScrollOffsetX, p_oRect.m_vTop - m_vScrollOffsetY, p_oRect.m_vRight - p_oRect.m_vLeft + 1, p_oRect.m_vBottom - p_oRect.m_vTop + 1, opAlphaPaintHandle, 0, 0, 1, 1, sBlendFunction);
		if(DeleteDC(opAlphaPaintHandle) == 0)
eo << "FillRect: DeleteDC failed" << ef;
		if(DeleteObject(opAlphaBitmap) == 0)
eo << "FillRect: DeleteObject failed" << ef;
	}
	else if(m_eDrawingMode == EM_OP_INVERT)
	{
		if(SetRect(&sRect, p_oRect.m_vLeft - m_vScrollOffsetX, p_oRect.m_vTop - m_vScrollOffsetY, p_oRect.m_vRight - m_vScrollOffsetX + 1, p_oRect.m_vBottom - m_vScrollOffsetY + 1) == 0)
eo << "FillRect: SetRect failed" << ef;
		if(m_vIsDoubleBuffered)
			::InvertRect(m_opDoubleBufferedPaintHandle, &sRect);
		else
			::InvertRect(m_opPlainPaintHandle, &sRect);
	}
	else
	{
		if(SetRect(&sRect, p_oRect.m_vLeft - m_vScrollOffsetX, p_oRect.m_vTop - m_vScrollOffsetY, p_oRect.m_vRight - m_vScrollOffsetX + 1, p_oRect.m_vBottom - m_vScrollOffsetY + 1) == 0)
eo << "FillRect: SetRect failed" << ef;
		if(m_vIsDoubleBuffered)
			::FillRect(m_opDoubleBufferedPaintHandle, &sRect, m_opCurrentBrush);
		else
			::FillRect(m_opPlainPaintHandle, &sRect, m_opCurrentBrush);
/*
		else
		{
			SetViewportOrgEx(m_opPlainPaintHandle, 0, 0, NULL);
			SetRect(&sRect, p_oRect.m_vLeft - m_vScrollOffsetX, p_oRect.m_vTop - m_vScrollOffsetY, p_oRect.m_vRight - m_vScrollOffsetX + 1, p_oRect.m_vBottom - m_vScrollOffsetY + 1);
			::FillRect(m_opPlainPaintHandle, &sRect, m_opCurrentBrush);
			SetViewportOrgEx(m_opPlainPaintHandle, -1 * m_vScrollOffsetX, -1 * m_vScrollOffsetY, NULL);
		}
*/
	}

}

EMRect EMWinView::Frame() const
{
//	AssertWindowLocked(); // FIXME: Shouldn't this require a lock?
	return m_oFrame;
}

EMRect EMWinView::GetClippingRect() const
{
	AssertWindowLocked();
	return m_oClippingRect;
/*
	if(m_opPlainPaintHandle == NULL)
	{
		RECT sRect;
		GetUpdateRect(m_opWindowHandle, &sRect, false);
		return EMRect(sRect.left, sRect.top, sRect.right, sRect.bottom);
	}
	return EMRect(m_sPaintStruct.rcPaint.left, m_sPaintStruct.rcPaint.top, m_sPaintStruct.rcPaint.right, m_sPaintStruct.rcPaint.bottom);
*/
}

EMView* EMWinView::GetLastMouseDownView()
{
	// FIXME: Implement this
	return NULL;
}

void EMWinView::GetMouse(int32 &p_vX, int32 &p_vY, bool &p_vButtonOne, bool &p_vButtonTwo, bool &p_vButtonThree) const
{
	AssertWindowLocked();
	POINT sPoint;
	GetCursorPos(&sPoint);
	ScreenToClient(m_opWindowHandle, &sPoint);
	p_vX = sPoint.x + m_vScrollOffsetX;//m_vMouseX;
	p_vY = sPoint.y + m_vScrollOffsetY;//m_vMouseY;
//	p_vButtonOne = m_vMouseButtonOne;
//	p_vButtonTwo = m_vMouseButtonTwo;
//	p_vButtonThree = m_vMouseButtonThree;
	p_vButtonOne = GetKeyState(VK_LBUTTON) < 0;
	p_vButtonTwo = GetKeyState(VK_RBUTTON) < 0;
	p_vButtonThree = GetKeyState(VK_MBUTTON) < 0;
}

void* EMWinView::GetNativeView() const
{
	return m_opWindowHandle;
}

uint32 EMWinView::GetResizingMode()
{
	return m_vResizingMode;
}


void EMWinView::GetScreenInfo(int32* p_vpWidth, int32* p_vpHeight, int32* p_vpBitDepth)
{
	HDC opDeviceContext = GetDC(NULL);
	*p_vpWidth = GetDeviceCaps(opDeviceContext, HORZRES);
	*p_vpHeight = GetDeviceCaps(opDeviceContext, VERTRES);
	*p_vpBitDepth = GetDeviceCaps(opDeviceContext, BITSPIXEL);
	ReleaseDC(NULL, opDeviceContext);
}

HWND EMWinView::GetWindowHandle()
{
	return m_opWindowHandle;
}

EMWinWindow* EMWinView::GetWindowWindow()
{
	return m_opWindowWindow;
}

// Generic window procedure passed to WIN32 API
LRESULT CALLBACK EMWinView::GlobalWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo)
{
	if(p_vMessage == WM_PARENTNOTIFY) // Why is this needed? (crasches once in a while if not checked)
		return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);

	// Now let's determine if the message origin is from somewhere else than a view
	if(p_vMessage == WM_VSCROLL || p_vMessage == WM_HSCROLL || p_vMessage == WM_COMMAND || p_vMessage == WM_NOTIFY || p_vMessage == WM_PARENTNOTIFY || p_vMessage == NATIVE_MESSAGE_SET_SCROLL_INFO)
	{
/*
uint32 x1 = WM_VSCROLL;
uint32 x2 = WM_HSCROLL;
uint32 x3 = WM_COMMAND;
uint32 x4 = WM_NOTIFY;
uint32 x5 = WM_PARENTNOTIFY;
uint32 x6 = NATIVE_MESSAGE_SET_SCROLL_INFO;
*/
int32 vWindowLong = GetWindowLong(reinterpret_cast<HWND>(p_vParameterTwo), GWL_USERDATA);
EMGUIComponent* opComponent = reinterpret_cast<EMGUIComponent*>(vWindowLong);
		//EMGUIComponent* opComponent = reinterpret_cast<EMGUIComponent*>(GetWindowLong(reinterpret_cast<HWND>(p_vParameterTwo), GWL_USERDATA));
//		if(!dynamic_cast<EMWinView*>(opComponent))
		HWND opRealWindowHandle;
		opRealWindowHandle = reinterpret_cast<HWND>(p_vParameterTwo);
//		if(p_vMessage == WM_VSCROLL || p_vMessage == WM_HSCROLL)
#ifndef EM_BASIC_COMPONENTS
		if(dynamic_cast<EMWinScrollbar*>(opComponent))
			return EMWinScrollbar::GlobalWindowProcedure(opRealWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
		if(p_vMessage == WM_COMMAND)
		{
//eo << "WM_COMMAND" << ef;
			EMGUIComponent* opComponent = reinterpret_cast<EMGUIComponent*>(GetWindowLong(reinterpret_cast<HWND>(p_vParameterTwo), GWL_USERDATA));
			if(dynamic_cast<EMWinButton*>(opComponent))
				return EMWinButton::GlobalWindowProcedure(opRealWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
			else if(dynamic_cast<EMWinCheckBox*>(opComponent))
				return EMWinCheckBox::GlobalWindowProcedure(opRealWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
			else if(dynamic_cast<EMWinListBox*>(opComponent))
				return EMWinListBox::GlobalWindowProcedure(opRealWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
			else if(dynamic_cast<EMWinComboBox*>(opComponent))
				return EMWinComboBox::GlobalWindowProcedure(opRealWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
			else if(dynamic_cast<EMWinRadioButton*>(opComponent))
				return EMWinRadioButton::GlobalWindowProcedure(opRealWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
			else
			{
//				EMDebugger("Unknown component");
//				return 0;
				eo << "UNKNOWN COMPONENT!!!" << ef;
				return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
			}

		}
#endif // EM_BASIC_COMPONENTS
		if(p_vMessage == WM_NOTIFY)
		{			
//eo << "WM_NOTIFY" << ef;
			EMGUIComponent* opComponent = reinterpret_cast<EMGUIComponent*>(GetWindowLong(reinterpret_cast<LPNMHDR>(p_vParameterTwo) -> hwndFrom, GWL_USERDATA));

// Let's see if the pointer is valid, FIXME: This is a semi-ugly hack
try
{
opComponent -> GetResizingMode();
}
catch(...)
{
return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
}
#ifndef EM_BASIC_COMPONENTS
			if(dynamic_cast<EMWinTabControl*>(opComponent))
				return EMWinTabControl::GlobalWindowProcedure(reinterpret_cast<LPNMHDR>(p_vParameterTwo) -> hwndFrom, p_vMessage, p_vParameterOne, p_vParameterTwo);
			if(dynamic_cast<EMWinListViewControl*>(opComponent))
				return EMWinListViewControl::GlobalWindowProcedure(reinterpret_cast<LPNMHDR>(p_vParameterTwo) -> hwndFrom, p_vMessage, p_vParameterOne, p_vParameterTwo);
			if(dynamic_cast<EMWinTreeViewControl*>(opComponent))
				return EMWinTreeViewControl::GlobalWindowProcedure(reinterpret_cast<LPNMHDR>(p_vParameterTwo) -> hwndFrom, p_vMessage, p_vParameterOne, p_vParameterTwo);
			if(reinterpret_cast<LPNMHDR>(p_vParameterTwo) -> code == UDN_DELTAPOS)
				return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
			if(reinterpret_cast<LPNMHDR>(p_vParameterTwo) -> code == HDN_ITEMCLICKA)
				return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
			if(reinterpret_cast<LPNMHDR>(p_vParameterTwo) -> code == HDN_ENDTRACKA)
				return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
			if(reinterpret_cast<LPNMHDR>(p_vParameterTwo) -> code == HDN_ITEMCHANGINGA)
				return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
			if(reinterpret_cast<LPNMHDR>(p_vParameterTwo) -> code == HDN_ITEMCHANGEDA)
				return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
			if(reinterpret_cast<LPNMHDR>(p_vParameterTwo) -> code == HDN_DIVIDERDBLCLICKA)
				return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
			if(dynamic_cast<EMWinToolTip*>(opComponent))
				return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
#endif // EM_BASIC_COMPONENTS
			EMDebugger("Unknown component");
			return 0;
		}
/*		if(p_vMessage == WM_PARENTNOTIFY)
		{			
eo << "WM_PARENTNOTIFY" << ef;
			EMGUIComponent* opComponent = reinterpret_cast<EMGUIComponent*>(GetWindowLong(reinterpret_cast<HWND>(p_vParameterTwo), GWL_USERDATA));
			if(dynamic_cast<EMWinListViewControl*>(opComponent))
				return EMWinListViewControl::GlobalWindowProcedure(reinterpret_cast<HWND>(p_vParameterTwo), p_vMessage, p_vParameterOne, p_vParameterTwo);
			else
			{
		//		EMDebugger("Unknown component");
		//		return 0;
			}
		}
*/
		// add other components here
	}
	// Now we assume that it is a EMWinView that sent the message

	EMWinView * opView = reinterpret_cast<EMWinView*>(GetWindowLong(p_vWindowHandle, GWL_USERDATA));

	// If the view isn't added to a parent, DefWindowProc takes care of business
	bool vWillDraw(false);

//string oMessageType; // debug stuff
//char vMessageType[80]; // debug stuff

	if(opView)
	{
		if(opView -> m_opWindowWindow == NULL)
			return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);

		uint32 vMessage(0);
		switch(p_vMessage)
		{
		case WM_LBUTTONDOWN:
//oMessageType = "WM_LBUTTONDOWN";
			vMessage = EM_MOUSE_DOWN;
			if(opView -> m_opWindowWindow -> GetType() != EM_WINDOW_DIALOG)
				::SetFocus(opView -> m_opWindowWindow -> GetWindowHandle());
			//m_vMouseButtonOne = true;
			//m_vMouseButtonTwo = false;
			//m_vMouseButtonThree = false;
			SetCapture(opView -> m_opWindowHandle);
			break;
		case WM_MBUTTONDOWN:
//oMessageType = "WM_MBUTTONDOWN";
			vMessage = EM_MOUSE_DOWN;
			if(opView -> m_opWindowWindow -> GetType() != EM_WINDOW_DIALOG)
				::SetFocus(opView -> m_opWindowWindow -> GetWindowHandle());
			//m_vMouseButtonThree = true;
			//m_vMouseButtonOne = false;
			//m_vMouseButtonTwo = false;
			SetCapture(opView -> m_opWindowHandle);
			break;
		case WM_RBUTTONDOWN:
//oMessageType = "WM_RBUTTONDOWN";
			vMessage = EM_MOUSE_DOWN;
			if(opView -> m_opWindowWindow -> GetType() != EM_WINDOW_DIALOG)
				::SetFocus(opView -> m_opWindowWindow -> GetWindowHandle());
			//m_vMouseButtonTwo = true;
			//m_vMouseButtonOne = false;
			//m_vMouseButtonThree = false;
			SetCapture(opView -> m_opWindowHandle);
			break;
		case WM_LBUTTONUP:
//oMessageType = "WM_LBUTTONUP";
			vMessage = EM_MOUSE_UP;
			//m_vMouseButtonOne = false;
			if(GetCapture() == opView -> m_opWindowHandle)
				ReleaseCapture();
//				SetCapture(NULL);
			break;
		case WM_MBUTTONUP:
//oMessageType = "WM_MBUTTONUP";
			vMessage = EM_MOUSE_UP;
			//m_vMouseButtonThree = false;
			if(GetCapture() == opView -> m_opWindowHandle)
				ReleaseCapture();
			break;
		case WM_RBUTTONUP:
//oMessageType = "WM_RBUTTONUP";
			vMessage = EM_MOUSE_UP;
			//m_vMouseButtonTwo = false;
			if(GetCapture() == opView -> m_opWindowHandle)
				ReleaseCapture();
			break;
		case WM_MOUSEMOVE:
//oMessageType = "WM_MOUSEMOVE";
			vMessage = EM_MOUSE_MOVED;
			opView -> m_vMouseCursorIsSet = false;
//			opView -> m_vMouseX = LOWORD(p_vParameterTwo);
//			opView -> m_vMouseY = HIWORD(p_vParameterTwo);
			break;
		case WM_LBUTTONDBLCLK:
//oMessageType = "WM_LBUTTONDBLCLK";
			// First simulate a single left mouse click
			opView -> LockWindow();
			//opView -> m_vMouseButtonOne = true;
			//opView -> m_vMouseButtonTwo = false;
			//opView -> m_vMouseButtonThree = false;
			opView -> Notify(EM_MOUSE_DOWN);
			opView -> UnlockWindow();
			// Here comes the "real" message
			vMessage = EM_DOUBLE_CLICK;
			break;
		case WM_RBUTTONDBLCLK:
//oMessageType = "WM_RBUTTONDBLCLK";
			// Simulate a single right mouse click
			opView -> LockWindow();
			//opView -> m_vMouseButtonTwo = true;
			//opView -> m_vMouseButtonOne = false;
			//opView -> m_vMouseButtonThree = false;
			opView -> Notify(EM_MOUSE_DOWN);
			opView -> UnlockWindow();
			break;
		case WM_PAINT:
//oMessageType = "WM_PAINT";
			vMessage = EM_DRAW;
			vWillDraw = true;
			break;
		case WM_ERASEBKGND:
//oMessageType = "WM_ERASEBKGND";
			{
				if(opView -> m_oBackgroundColor.m_vAlpha == 0) // If the view is transparent
					return 0;
				HDC opPaintHandle = reinterpret_cast<HDC>(p_vParameterOne);
				RECT oRect;
				if(SetRect(&oRect, (opView -> m_oBounds).m_vLeft - (opView -> m_vScrollOffsetX), (opView -> m_oBounds).m_vTop - (opView -> m_vScrollOffsetY), (opView -> m_oBounds).m_vRight - (opView -> m_vScrollOffsetX) + 1, (opView -> m_oBounds).m_vBottom - (opView -> m_vScrollOffsetY) + 1) == 0)
eo << "WM_ERASEBKGND: SetRect failed" << ef;
				HBRUSH opBrush = CreateSolidBrush(RGB(opView -> m_oBackgroundColor.m_vRed, opView -> m_oBackgroundColor.m_vGreen, opView -> m_oBackgroundColor.m_vBlue));
				::FillRect(opPaintHandle, &oRect, opBrush);
				if(DeleteObject(opBrush) == 0)
eo << "WM_ERASEBKGND: DeleteObject failed" << ef;
//if(opView -> m_opPlainPaintHandle != NULL)
//EMDebugger("BeginPaint without EndPaint");
				return 1;
			}
		case WM_SIZE:
//oMessageType = "WM_SIZE";
/*
			if(!m_vMouseHookIsSet)
			{
				if((m_opMouseProcHook = SetWindowsHookEx(WH_MOUSE, EMWinView::MouseProc, EMApplication::GetApplicationInstance(), GetCurrentThreadId())) == NULL)
eo << "Could not set up mouse hook" << ef;
				m_vMouseHookIsSet = true;
			}
*/
			if(p_vParameterOne == SIZE_MINIMIZED) // Do not process minimizing windows
				break;
			// Since most WM_SIZE comes from SetFrame, the window is probably already locked
			vMessage = EM_VIEW_BOUNDS_CHANGED;
			if(opView -> WindowIsLocked())
			{
				if(!opView -> Notify(vMessage))
				{
//if(opView -> m_opPlainPaintHandle != NULL)
//EMDebugger("BeginPaint without EndPaint");
					return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
				}
//if(opView -> m_opPlainPaintHandle != NULL)
//EMDebugger("BeginPaint without EndPaint");
				return 0;
			}
			break;
/*
		case WM_CREATE:
			if(!m_vMouseHookIsSet)
			{
				if((m_opMouseProcHook = SetWindowsHookEx(WH_MOUSE, MouseProc, EMApplication::GetApplicationInstance(), 0)) == NULL)
eo << "Could not set up mouse hook" << ef;
				else
eo << "Mouse hook set up" << ef;
				m_vMouseHookIsSet = true;
				break;
			}
*/
		default:
//oMessageType = "unspecified";
			break;
		}
		if(vMessage != 0)
		{
//strcpy(vMessageType, oMessageType.c_str());
			opView -> LockWindow();
			if(vWillDraw)
			{
//eo << "BeginPaint (WM_PAINT)" << ef;
				opView -> BeginPaint(true);
			}
//eo << "Message notify started" << ef;
			bool vResult = opView -> Notify(vMessage);
//eo << "Message notify completed: " << vResult << ef;
			if(vWillDraw)
			{
//eo << "End paint (WM_PAINT)" << ef;
					opView -> EndPaint(true);
			}
			if(vMessage == EM_MOUSE_MOVED && !(opView -> m_vMouseCursorIsSet))
				opView -> SetMouseCursor(opView -> m_eDefaultMouseCursor, true); // If no cursor is set, the default cursor is used
//else
//{
if(opView -> m_opPlainPaintHandle != NULL)
{
EMDebugger("BeginPaint without EndPaint");
opView -> m_opPlainPaintHandle = NULL; // To prevent one zillion dialogs
}
//}
if(opView -> m_vBeginPaintCount > 0)
{
EMDebugger("m_vBeginPaintCount is not 0");
opView -> m_vBeginPaintCount = 0; // To prevent one zillion dialogs
}
			if(vResult)
			{
				opView -> UnlockWindow();
				return 0;
			}
			opView -> UnlockWindow();
		}
	}
//eo << "DefWindowProc, p_vMessage = " << p_vMessage << ef;
	return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
}

void EMWinView::Hide()
{
	ShowWindow(m_opWindowHandle, SW_HIDE);
}

void EMWinView::Invalidate(EMRect p_oRect)
{
	AssertWindowLocked();

	if(p_oRect.m_vRight > m_oBounds.m_vRight)
		p_oRect.m_vRight = m_oBounds.m_vRight + 1;
	if(p_oRect.m_vBottom > m_oBounds.m_vBottom)
		p_oRect.m_vBottom = m_oBounds.m_vBottom + 1;
	if(p_oRect.m_vTop < m_oBounds.m_vTop)
		p_oRect.m_vTop = m_oBounds.m_vLeft - 1;
	if(p_oRect.m_vLeft < m_oBounds.m_vLeft)
		p_oRect.m_vLeft = m_oBounds.m_vLeft - 1;

	if(!p_oRect.IsValid())
		return;

	RECT sRect;
	if(SetRect(&sRect, p_oRect.m_vLeft - m_vScrollOffsetX, p_oRect.m_vTop - m_vScrollOffsetY, p_oRect.m_vRight + 1 - m_vScrollOffsetX, p_oRect.m_vBottom + 1 - m_vScrollOffsetY) == 0)
eo << "Invalidate: SetRect failed" << ef;
	if(InvalidateRect(m_opWindowHandle, &sRect, true/*false*/) == 0)
eo << "Invalidate: InvalidateRect failed" << ef;
}

bool EMWinView::IsDoubleBuffered()
{
	return m_vIsDoubleBuffered;
}

bool EMWinView::IsHidden()
{
	return IsWindowVisible(m_opWindowHandle) == 0;
}

bool EMWinView::IsPainting()
{
	return m_opPlainPaintHandle != NULL;
}

bool EMWinView::LockBuffer()
{
	// FIXME: Implement this?
	return true;
}

bool EMWinView::LockWindow()
{
	if(m_opWindowWindow != NULL) // FIXME: Design away this check?
	{
//if(m_opWindowWindow -> IsLockedByThread())
//eo << "Window is already locked by this thread" << ef;
//eo << "Locking window" << ef;
		return m_opWindowWindow -> Lock();
	}
	/*
	if(IsPainting())
	{
MessageBox(NULL, "Locked the window while someone's painting in it", "Lock issue", MB_OK);
		EMDebugger("Trying to lock window while someone's painting in it");
	}
	*/
	return true;
}
/*
LRESULT CALLBACK EMWinView::MouseProc(int p_vCode, WPARAM p_vParameterOne, LPARAM p_vParameterTwo)
{
eo << "EMWinView::MouseProc" << ef;
	if(p_vCode >= 0)
	{
eo << "MouseProc: " << WM_LBUTTONDOWN << ", " << p_vParameterOne << ef;
	}
	return CallNextHookEx(m_opMouseProcHook, p_vCode, p_vParameterOne, p_vParameterTwo);
}
*/
void EMWinView::ReceiveAllMouseEvents(bool vReceiveAll)
{
	AssertWindowLocked();
	if(vReceiveAll)
		SetCapture(m_opWindowHandle);
	else
		ReleaseCapture();
}

bool EMWinView::RemoveChild(EMGUIComponent* p_opView)
{
	// FIXME: Implement this
	return true;
}
/*
void EMWinView::ResetMouse()
{
	m_vMouseButtonOne = false;
	m_vMouseButtonThree = false;
	m_vMouseButtonTwo = false;
}
*/
void EMWinView::ScrollBy(int32 p_vX, int32 p_vY)
{
/*
	AssertWindowLocked();
//	ScrollWindowEx(m_opWindowHandle, p_vX, p_vY, NULL, NULL, NULL, NULL, SW_INVALIDATE);
	ScrollWindow(m_opWindowHandle, p_vX, p_vY, NULL, NULL);
	m_vScrollOffsetX += p_vX;
	m_vScrollOffsetY += p_vY;
	if(m_opPlainPaintHandle != NULL)
	{
		if(m_vIsDoubleBuffered)
			SetViewportOrgEx(m_opDoubleBufferedPaintHandle, m_vScrollOffsetX, m_vScrollOffsetY, NULL);
		SetViewportOrgEx(m_opPlainPaintHandle, m_vScrollOffsetX, m_vScrollOffsetY, NULL);
	}
//	m_vBufferScrollOffsetY += p_vY;
//	m_vBufferScrollOffsetY += p_vY;
//	SetWindowOrgEx
*/
}

void EMWinView::ScrollTo(int32 p_vX, int32 p_vY)
{
	AssertWindowLocked();
//	ScrollWindowEx(m_opWindowHandle, -1 * (p_vX - m_vScrollOffsetX), -1 * (p_vY - m_vScrollOffsetY), NULL, NULL/*&sClippingRect*/, NULL, NULL, SW_SCROLLCHILDREN);
	ScrollWindow(m_opWindowHandle, -1 * (p_vX - m_vScrollOffsetX), -1 * (p_vY - m_vScrollOffsetY), NULL, NULL);
	m_vScrollOffsetX = p_vX;
	m_vScrollOffsetY = p_vY;
	m_oBounds.OffsetTo(m_vScrollOffsetX, m_vScrollOffsetY);
/*
	if(m_opPlainPaintHandle != NULL)
	{
		if(m_vIsDoubleBuffered)
			SetViewportOrgEx(m_opDoubleBufferedPaintHandle, -1 * m_vScrollOffsetX, -1 * m_vScrollOffsetY, NULL);
		SetViewportOrgEx(m_opPlainPaintHandle, -1 * m_vScrollOffsetX, -1 * m_vScrollOffsetY, NULL);
	}
*/
}

void EMWinView::SetBitmap(EMBitmap* p_opBitmap)
{
	// FIXME: Implement this (needed?)
}

void EMWinView::SetDefaultColor()
{
	SetViewColor(EMColor(GetRValue(GetSysColor(COLOR_3DFACE)), GetGValue(GetSysColor(COLOR_3DFACE)), GetBValue(GetSysColor(COLOR_3DFACE)), 255));
}

void EMWinView::SetDefaultCursor(EMMouseCursorType p_eDefaultCursor)
{
	m_eDefaultMouseCursor = p_eDefaultCursor;
}

void EMWinView::SetDrawingMode(EMDrawingMode p_vMode)
{
	AssertWindowLocked();
	m_eDrawingMode = p_vMode; // Needed?
	if(m_vIsDoubleBuffered)
	{
		if(p_vMode == EM_OP_COPY)
			SetROP2(m_opDoubleBufferedPaintHandle, R2_COPYPEN);
		else if(p_vMode == EM_OP_INVERT)
			SetROP2(m_opDoubleBufferedPaintHandle, R2_NOT);
		else if(p_vMode == EM_OP_ALPHA)
			SetROP2(m_opDoubleBufferedPaintHandle, R2_COPYPEN); // FIXME: Support alpha painting?
		else
			SetROP2(m_opDoubleBufferedPaintHandle, R2_COPYPEN);
	}
	else
	{
		if(p_vMode == EM_OP_COPY)
			SetROP2(m_opPlainPaintHandle, R2_COPYPEN);
		else if(p_vMode == EM_OP_INVERT)
			SetROP2(m_opPlainPaintHandle, R2_NOT);
		else if(p_vMode == EM_OP_ALPHA)
			SetROP2(m_opPlainPaintHandle, R2_COPYPEN); // FIXME: Support alpha painting?
		else
			SetROP2(m_opPlainPaintHandle, R2_COPYPEN);
	}
}

void EMWinView::SetFont(EMFont p_vFont)
{
	AssertWindowLocked();
	m_eFont = p_vFont;
	
	if(!IsPainting())
		return;

	if(m_vIsDoubleBuffered)
	{
		if(m_eFont == EM_PLAIN)
//			SelectObject(m_opDoubleBufferedPaintHandle, GetStockObject(DEFAULT_GUI_FONT));
			SelectObject(m_opDoubleBufferedPaintHandle, m_opPlainFont);
		else if(m_eFont == EM_BOLD)
			SelectObject(m_opDoubleBufferedPaintHandle, GetStockObject(DEFAULT_GUI_FONT));
		else
			SelectObject(m_opDoubleBufferedPaintHandle, GetStockObject(ANSI_FIXED_FONT));
	}
	else
	{
		if(m_eFont == EM_PLAIN)
//			SelectObject(m_opPlainPaintHandle, GetStockObject(DEFAULT_GUI_FONT));
			SelectObject(m_opPlainPaintHandle, m_opPlainFont);
		else if(m_eFont == EM_BOLD)
			SelectObject(m_opPlainPaintHandle, GetStockObject(DEFAULT_GUI_FONT));
		else
			SelectObject(m_opPlainPaintHandle, GetStockObject(ANSI_FIXED_FONT));
	}
}

void EMWinView::SetFontSize(EMFont p_vFont, int p_vPointSize)
{
	bool vDeleteHandle(false);
	if(m_opPlainPaintHandle == NULL)
	{
		m_opPlainPaintHandle = GetDC(m_opWindowHandle);
		vDeleteHandle = true;
	}

	int vFontHeight = -1 * MulDiv(p_vPointSize, GetDeviceCaps(m_opPlainPaintHandle, LOGPIXELSY), 72);
	if(p_vFont == EM_PLAIN)
	{
		DeleteObject(m_opPlainFont);
		m_opPlainFont = CreateFont(vFontHeight, 0, 0, 0, 0, false, false, false, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_EMBEDDED, /*PROOF_QUALITY*/ANTIALIASED_QUALITY, FF_DONTCARE, "Tahoma"/*NULL*/);
	}
	else
		EMDebugger("Only support new font size on EM_PLAIN font");

	if(vDeleteHandle)
	{
		ReleaseDC(m_opWindowHandle, m_opPlainPaintHandle);
		m_opPlainPaintHandle = NULL;
	}
}

void EMWinView::SetFrame(EMRect p_oFrame)
{
	AssertWindowLocked();
	if(p_oFrame == m_oFrame)
		return; // Nothing needs to change
	EMRect oOldFrame = m_oFrame;
	m_oFrame = p_oFrame;
	m_oBounds = EMRect(0, 0, m_oFrame.GetWidth(), m_oFrame.GetHeight());
	m_oBounds.OffsetTo(m_vScrollOffsetX, m_vScrollOffsetY);
	MoveWindow(m_opWindowHandle, p_oFrame.m_vLeft, p_oFrame.m_vTop, p_oFrame.GetWidth() + 1, p_oFrame.GetHeight() + 1, true);
// FIXME: A temporary fix: Repaint the whole window when resizing
//Invalidate(Bounds());
//	EMGUIUtilities::Instance() -> ResizeToParent(&m_oChildComponents, oOldFrame, p_oFrame);
}

void EMWinView::SetHighColor(const EMColor p_vColor)
{
//SetMouseCursor(EM_MOUSE_CURSOR_RESIZE_CLIP_LEFT);
if(m_vBeginPaintCount > 1)
eo << "SetHighColor: m_vBeginPaintCount: " << m_vBeginPaintCount << ef;
	AssertWindowLocked();
	m_vCurrentHighColor = RGB(p_vColor.m_vRed, p_vColor.m_vGreen, p_vColor.m_vBlue);
	m_vCurrentAlpha = p_vColor.m_vAlpha;
	if(m_vIsDoubleBuffered)
	{
		m_opCurrentPen = CreatePen(PS_SOLID, 1, m_vCurrentHighColor);
		if(DeleteObject(SelectObject(m_opDoubleBufferedPaintHandle, m_opCurrentPen/*CreatePen(PS_SOLID, 1, m_vCurrentHighColor)*/)) == 0)
eo << "SetHighColor: DeleteObject failed 1" << ef;
		m_opCurrentBrush = CreateSolidBrush(m_vCurrentHighColor);
		if(DeleteObject(SelectObject(m_opDoubleBufferedPaintHandle, m_opCurrentBrush/*CreateSolidBrush(m_vCurrentHighColor)*/)) == 0)
eo << "SetHighColor: DeleteObject failed 2" << ef;
		SetTextColor(m_opDoubleBufferedPaintHandle, m_vCurrentHighColor);
	}
	else
	{
		m_opCurrentPen = CreatePen(PS_SOLID, 1, m_vCurrentHighColor);
		if(DeleteObject(SelectObject(m_opPlainPaintHandle, m_opCurrentPen/*CreatePen(PS_SOLID, 1, m_vCurrentHighColor)*/)) == 0)
eo << "SetHighColor: DeleteObject failed 3" << ef;
		m_opCurrentBrush = CreateSolidBrush(m_vCurrentHighColor);
		if(DeleteObject(SelectObject(m_opPlainPaintHandle, m_opCurrentBrush/*CreateSolidBrush(m_vCurrentHighColor)*/)) == 0)
eo << "SetHighColor: DeleteObject failed 4" << ef;
		SetTextColor(m_opPlainPaintHandle, m_vCurrentHighColor);
	}
//	SetDCPenColor(m_opPaintHandle, RGB(p_vColor.m_vRed, p_vColor.m_vGreen, p_vColor.m_vBlue));

}

void EMWinView::SetLowColor(const EMColor p_vColor)
{

	AssertWindowLocked();
	if(m_vIsDoubleBuffered)
		SetBkColor(m_opDoubleBufferedPaintHandle, RGB(p_vColor.m_vRed, p_vColor.m_vGreen, p_vColor.m_vBlue));
	else
		SetBkColor(m_opPlainPaintHandle, RGB(p_vColor.m_vRed, p_vColor.m_vGreen, p_vColor.m_vBlue));
}

// The mouse cursor can only be set if the default mouse cursor is set to the standard arrow
void EMWinView::SetMouseCursor(EMMouseCursorType p_eType, bool p_vAlwaysSucceed)
{
	if(m_eDefaultMouseCursor != EM_MOUSE_CURSOR_ARROW && !p_vAlwaysSucceed)
		return;

	m_vMouseCursorIsSet = true;

	if(p_eType == m_eCurrentMouseCursor)
	{
		SetCursor(m_oMouseCursors[static_cast<int>(p_eType)] -> GetNativeCursor());
		return;
	}

	EMWinMouseCursor* opCursor;
	if(m_oMouseCursors[static_cast<int>(p_eType)] == NULL)
	{
		opCursor = EM_new EMWinMouseCursor(p_eType);
		m_oMouseCursors[static_cast<int>(p_eType)] = opCursor;
	}
	else
		opCursor = m_oMouseCursors[static_cast<int>(p_eType)];

	SetCursor(opCursor -> GetNativeCursor());
	m_eCurrentMouseCursor = p_eType;
}

void EMWinView::SetViewColor(const EMColor p_vColor)
{
	AssertWindowLocked();
	m_oBackgroundColor = p_vColor;
}

void EMWinView::SetWindowWindow(EMWinWindow* p_opWindow)
{
	m_opWindowWindow = p_opWindow;
}

void EMWinView::Show()
{
	ShowWindow(m_opWindowHandle, SW_SHOW);
}

// This method cannot be used to draw single pixel dots
void EMWinView::StrokeLine(int32 p_vX1, int32 p_vY1, int32 p_vX2, int32 p_vY2)
{
	AssertWindowLocked();

//if(p_vX2 - p_vX1 > 5000 || p_vY2 - p_vY1 > 5000 || p_vX1 - m_vScrollOffsetX > 5000 || p_vX2 - m_vScrollOffsetX > 5000)
//EMDebugger("Aha!");
	HDC opPaintHandle;
	if(m_vIsDoubleBuffered)
		opPaintHandle = m_opDoubleBufferedPaintHandle;
	else
		opPaintHandle = m_opPlainPaintHandle;

	if(p_vY1 == p_vY2 || p_vX1 == p_vX2)
	{
		if(p_vY1 == p_vY2) // A horizontal line
		{
			if(p_vY1 > m_oBounds.m_vBottom)
				return;
			if(p_vX1 < m_oBounds.m_vLeft)
				p_vX1 = m_oBounds.m_vLeft;
			if(p_vX2 < m_oBounds.m_vLeft)
				p_vX2 = m_oBounds.m_vLeft;
			if(p_vX1 > m_oBounds.m_vRight)
				p_vX1 = m_oBounds.m_vRight + 1;
			if(p_vX2 > m_oBounds.m_vRight)
				p_vX2 = m_oBounds.m_vRight + 1;
			p_vX2++;
		}
		else // A vertical line
		{
			if(p_vX1 > m_oBounds.m_vRight)
				return;
			if(p_vY1 < m_oBounds.m_vTop)
				p_vY1 = m_oBounds.m_vTop;
			if(p_vY2 < m_oBounds.m_vTop)
				p_vY2 = m_oBounds.m_vTop;
			if(p_vY1 > m_oBounds.m_vBottom)
				p_vY1 = m_oBounds.m_vBottom + 1;
			if(p_vY2 > m_oBounds.m_vBottom)
				p_vY2 = m_oBounds.m_vBottom + 1;
			p_vY2++;
		}

		MoveToEx(opPaintHandle, p_vX1 - m_vScrollOffsetX, p_vY1 - m_vScrollOffsetY, NULL);
		LineTo(opPaintHandle, p_vX2 - m_vScrollOffsetX, p_vY2 - m_vScrollOffsetY);
	}
	else // A diagonal line (a bit more tricky)
	{
		if((p_vX1 > m_oBounds.m_vRight && p_vX2 > m_oBounds.m_vRight) || (p_vY1 > m_oBounds.m_vBottom && p_vY2 > m_oBounds.m_vBottom))
			return;

		p_vX1 -= m_vScrollOffsetX;
		p_vX2 -= m_vScrollOffsetX;
		p_vY1 -= m_vScrollOffsetY;
		p_vY2 -= m_vScrollOffsetY;

		// Not an optimal solution, but quicker to implement
		if(p_vX1 > 0x7FFF || p_vX2 > 0x7FFF || p_vY1 > 0x7FFF || p_vY2 > 0x7FFF) // FIXME: 0x7FFF should be INT16_MAX
		{
			if(p_vX2 > 0x7FFF || p_vY2 > 0x7FFF)
			{
				p_vX2 /= 2;
				p_vY2 /= 2;
			}
			else
			{
				p_vX1 /= 2;
				p_vY1 /= 2;
			}
		}
		// FIXME: Fix coordinates that are < 0
		/*
		if(p_vX1 < 0)
		{
			int32 vOffset = p_vX1 + (p_vX2 - p_vX1) * (static_cast<float>(-1 * p_vX1) / (p_vX2 - p_vX1));
			p_vX1 += vOffset;
			p_vY1 += vOffset;
		}
		*/
		MoveToEx(opPaintHandle, p_vX1, p_vY1, NULL);
		LineTo(opPaintHandle, p_vX2, p_vY2);
		SetPixel(opPaintHandle, p_vX2, p_vY2, m_vCurrentHighColor);
	}

/*
	if(m_vIsDoubleBuffered)
	{
		MoveToEx(m_opDoubleBufferedPaintHandle, p_vX1 - m_vScrollOffsetX, p_vY1 - m_vScrollOffsetY, NULL);
		LineTo(m_opDoubleBufferedPaintHandle, p_vX2 - m_vScrollOffsetX, p_vY2 - m_vScrollOffsetY);
		SetPixel(m_opDoubleBufferedPaintHandle, p_vX2 - m_vScrollOffsetX, p_vY2 - m_vScrollOffsetY, m_vCurrentHighColor);
	}
	else
	{
		MoveToEx(m_opPlainPaintHandle, p_vX1 - m_vScrollOffsetX, p_vY1 - m_vScrollOffsetY, NULL);
		LineTo(m_opPlainPaintHandle, p_vX2 - m_vScrollOffsetX, p_vY2 - m_vScrollOffsetY);
		SetPixel(m_opPlainPaintHandle, p_vX2 - m_vScrollOffsetX, p_vY2 - m_vScrollOffsetY, m_vCurrentHighColor);
	}
*/
}

void EMWinView::StrokeRect(EMRect p_oRect)
{
	AssertWindowLocked();

//if(p_oRect.GetWidth() > 5000 || p_oRect.m_vRight - m_vScrollOffsetX > 5000 || p_oRect.m_vLeft - m_vScrollOffsetX > 5000)
//EMDebugger("Aha!");
	HDC opPaintHandle;
	if(m_vIsDoubleBuffered)
		opPaintHandle = m_opDoubleBufferedPaintHandle;
	else
		opPaintHandle = m_opPlainPaintHandle;
	/*
	if(m_oBounds.Contains(p_oRect)
	{
		MoveToEx(opPaintHandle, p_oRect.m_vLeft, p_oRect.m_vTop, NULL);
		LineTo(opPaintHandle, p_oRect.m_vRight, p_oRect.m_vTop);
		LineTo(opPaintHandle, p_oRect.m_vRight, p_oRect.m_vBottom);
		LineTo(opPaintHandle, p_oRect.m_vLeft, p_oRect.m_vBottom);
		LineTo(opPaintHandle, p_oRect.m_vLeft, p_oRect.m_vTop);
		return;
	}
	
	if(p_oRect.m_vLeft > m_oBounds.m_vRight)
		return;
	if(p_oRect.m_vTop > m_oBounds.m_vBottom)
		return;
	if(p_oRect.m_vRight < m_oBounds.m_vLeft)
		return;
	if(p_oRect.m_vBottom < m_oBounds.m_vTop)
		return;
	*/
	if(p_oRect.m_vRight > m_oBounds.m_vRight)
		p_oRect.m_vRight = m_oBounds.m_vRight + 1;
	if(p_oRect.m_vBottom > m_oBounds.m_vBottom)
		p_oRect.m_vBottom = m_oBounds.m_vBottom + 1;
	if(p_oRect.m_vTop < m_oBounds.m_vTop)
		p_oRect.m_vTop = m_oBounds.m_vLeft - 1;
	if(p_oRect.m_vLeft < m_oBounds.m_vLeft)
		p_oRect.m_vLeft = m_oBounds.m_vLeft - 1;

	if(!p_oRect.IsValid())
		return;

	p_oRect.OffsetBy(-1 * m_vScrollOffsetX, -1 * m_vScrollOffsetY);

	MoveToEx(opPaintHandle, p_oRect.m_vLeft, p_oRect.m_vTop, NULL);
	LineTo(opPaintHandle, p_oRect.m_vRight, p_oRect.m_vTop);
	LineTo(opPaintHandle, p_oRect.m_vRight, p_oRect.m_vBottom);
	LineTo(opPaintHandle, p_oRect.m_vLeft, p_oRect.m_vBottom);
	LineTo(opPaintHandle, p_oRect.m_vLeft, p_oRect.m_vTop);

/*
	p_oRect.OffsetBy(-1 * m_vScrollOffsetX, -1 * m_vScrollOffsetY);
	if(m_vIsDoubleBuffered)
	{
		MoveToEx(opPaintHandle, p_oRect.m_vLeft, p_oRect.m_vTop, NULL);
		LineTo(opPaintHandle, p_oRect.m_vRight, p_oRect.m_vTop);
		LineTo(opPaintHandle, p_oRect.m_vRight, p_oRect.m_vBottom);
		LineTo(opPaintHandle, p_oRect.m_vLeft, p_oRect.m_vBottom);
		LineTo(opPaintHandle, p_oRect.m_vLeft, p_oRect.m_vTop);
	}
	else
	{
		MoveToEx(opPaintHandle, p_oRect.m_vLeft, p_oRect.m_vTop, NULL);
		LineTo(opPaintHandle, p_oRect.m_vRight, p_oRect.m_vTop);
		LineTo(opPaintHandle, p_oRect.m_vRight, p_oRect.m_vBottom);
		LineTo(opPaintHandle, p_oRect.m_vLeft, p_oRect.m_vBottom);
		LineTo(opPaintHandle, p_oRect.m_vLeft, p_oRect.m_vTop);
	}
*/
}

void EMWinView::Sync()
{
	// FIXME: Implement this (needed?)
}

void EMWinView::UpdateBufferFrame()
{
	// FIXME: Implement this (needed?)
}

bool EMWinView::WindowIsLocked()
{
	if(m_opWindowWindow == NULL)
		return false;
	return m_opWindowWindow -> IsLocked();
}

void EMWinView::UnlockBuffer()
{
	// FIXME:v Implement this
}

void EMWinView::UnlockWindow()
{
	/*
	if(IsPainting())
	{
MessageBox(NULL, "Trying to unlock window while still painting in it", "Lock issue", MB_OK);
		EMDebugger("Trying to unlock window while still painting in it");
	}
	*/
	if(m_opWindowWindow != NULL) // FIXME: Design away this check?
		m_opWindowWindow -> Unlock();
//eo << "Window unlocked" << ef;
}

#endif

