#include "EMWinWindow.h"

#ifdef PLATFORM_WINDOWS

#include "EMApplication.h"
#include "EMBitmap.h"
#include "EMCommandRepository.h"
#include "EMGUIUtilities.h"
#include "EMKeyboard.h"
#include "EMKeyboardGlobals.h"
#include "EMMessages.h"
#include "EMResourceRepository.h"
#include "EMSemaphore.h"
#include "EMUniqueID.h"
#include "EMWinMenuBar.h"
#include "EMWinMessages.h"
#include "EMWinTextControl.h"
#include "EMWinPopUpMenu.h"
#include "EMWinView.h"
#include "ResourceIDs.h" // FIXME: App class in framework

const int DEAFULT_MAXIMUM_WINDOW_SIZE = 2000;

EMWinWindow* EMWinWindow::m_opMainWindow = NULL;
HWND EMWinWindow::m_opDummyWindowHandle = NULL;
//bool EMWinWindow::m_vIconMaskIsSet = false;
HICON EMWinWindow::m_opWindowIcon = NULL;
bool EMWinWindow::m_vWindowIsDeleted = 0;
unsigned char EMWinWindow::m_opIconMaskBits[EM_SMALL_ICON_SIZE * EM_SMALL_ICON_SIZE / 8] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// The first window created will become the main window (i.e. the window containing the MS Windows message loop)
EMWinWindow::EMWinWindow(const EMRect p_oRect, const char* p_vpName, const EMWindowType p_eType) :
m_eWindowType(p_eType),
m_oBounds(EMRect(0, 0, p_oRect.m_vRight - p_oRect.m_vLeft, p_oRect.m_vBottom - p_oRect.m_vTop)),
m_oFrame(p_oRect),
m_oName(p_vpName),
m_opSemaphore(EMSemaphore::CreateEMSemaphore(p_vpName, 1, 1)),
m_vLockCount(0),
m_vLockingThreadID(-1),
m_vMaximumHeight(DEAFULT_MAXIMUM_WINDOW_SIZE),
m_vMaximumWidth(DEAFULT_MAXIMUM_WINDOW_SIZE),
m_vMinimumHeight(0),
m_vMinimumWidth(0),
m_vReSize(true),
m_vHasFocus(false),
m_vIsEnabled(true)
{ 
	EMUniqueID::GetUniqueName();
	string oUniqueName = EMUniqueID::GetUniqueName();

	if(m_opWindowIcon == NULL)
	{
		EMBitmap* opBitmap = EMResourceRepository::GetBitmap(RES_APP_ICON_SMALL);

		memset(m_opIconMaskBits, 0, 16 * 16 / 8);

		unsigned char vpBits[16 * 16 * 3];
		GetBitmapBits(static_cast<HBITMAP>(opBitmap -> GetNativeBitmap()), opBitmap -> GetSize(), vpBits);

		// Set the bits in the mask. All pixels in the bitmap with the rgb value of FF00FF will result in a 1, else 0
		int vCounterIncrease = 3;
		for(int vIndex = 0; vIndex < 16 * 16 * vCounterIncrease; vIndex += vCounterIncrease)
		{
			if(vpBits[vIndex] == 0xFF && vpBits[vIndex + 1] == 0 && vpBits[vIndex + 2] == 0xFF)
			{
				vpBits[vIndex] = 0;
				vpBits[vIndex + 1] = 0;
				vpBits[vIndex + 2] = 0;
				m_opIconMaskBits[static_cast<int>(vIndex / (8 * vCounterIncrease))] |= (0x80 >> ((vIndex / vCounterIncrease) % 8));
			}
		}

		// Flip the bits (yep it's a hack)
		memcpy(opBitmap -> GetBits(), vpBits, 16 * 16 * 3);
		GetBitmapBits(static_cast<HBITMAP>(opBitmap -> GetNativeBitmap()), opBitmap -> GetSize(), vpBits);
		memcpy(opBitmap -> GetBits(), vpBits, 16 * 16 * 3);
		
		HBITMAP opMask = CreateBitmap(16, 16, 1, 1, m_opIconMaskBits);

		ICONINFO sIconInfo;
		sIconInfo.fIcon = true;
		sIconInfo.hbmColor = static_cast<HBITMAP>(opBitmap -> GetNativeBitmap());
		sIconInfo.hbmMask = opMask;
		sIconInfo.xHotspot = 0;
		sIconInfo.yHotspot = 0;

		m_opWindowIcon = CreateIconIndirect(&sIconInfo);

		DeleteObject(opMask);
	}

	WNDCLASSEX sWindowClass;
	sWindowClass.cbSize = sizeof(WNDCLASSEX);
	if(p_eType == EM_WINDOW_DIALOG)
		sWindowClass.style = 0x00000808;
	else
		sWindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	sWindowClass.lpfnWndProc = GlobalWindowProcedure;
	sWindowClass.cbClsExtra = 0;
	sWindowClass.cbWndExtra = 0;       
	sWindowClass.hInstance = EMApplication::GetApplicationInstance();
	sWindowClass.hIcon = m_opWindowIcon;//LoadIcon(NULL, IDI_APPLICATION);
	sWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	sWindowClass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	sWindowClass.lpszMenuName = NULL;
	sWindowClass.lpszClassName = oUniqueName.c_str();
	sWindowClass.hIconSm = m_opWindowIcon;

	if(RegisterClassEx(&sWindowClass))
	{
		HWND vParent = NULL;
		if(p_eType == EM_WINDOW_DIALOG)
		{
			eo << "Create Dialog Window" << ef;
			// This is how a dialog should be created. And more code too!!
/*			DLGTEMPLATE sDialogTemplate;
			sDialogTemplate.style = DS_MODALFRAME | WS_POPUP | WS_CAPTION;
			sDialogTemplate.dwExtendedStyle = WS_EX_TOPMOST;
			sDialogTemplate.cdit = 1; // Is this correct?
			sDialogTemplate.x = p_oRect.m_vLeft;
			sDialogTemplate.y = p_oRect.m_vTop;
			sDialogTemplate.cx = ((EMRect)p_oRect).GetWidth();
			sDialogTemplate.cy = ((EMRect)p_oRect).GetHeight();

			DialogBoxIndirect(EMApplication::GetApplicationInstance(), spDialogTemplate, GetMainWindowHandle(), 0);
			//m_opWindowHandle = CreateDialogIndirectParam(EMApplication::GetApplicationInstance(), &sDialogTemplate, GetMainWindowHandle(), NULL, 0);
			return;
*/
			m_vStyleEX = /*WS_EX_TOPMOST |*/ WS_EX_DLGMODALFRAME;// | WS_EX_TOOLWINDOW;
			m_vStyle = DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU;/*WS_OVERLAPPED*//* | DS_3DLOOK | DS_SETFONT | DS_CONTEXTHELP*/
			vParent = GetMainWindowHandle();
		}
		else if(p_eType == EM_WINDOW_NORMAL)
		{
			m_vStyleEX = WS_EX_OVERLAPPEDWINDOW;
			m_vStyle = WS_CLIPCHILDREN/* | WS_OVERLAPPED*/ | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX;//WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
		}
		else if(p_eType == EM_WINDOW_NUDGE)
		{
			EMDebugger("This window type is no longer supported");
			return;
//			vStyleEX = WS_EX_OVERLAPPEDWINDOW;
//			m_vStyle = WS_CLIPCHILDREN | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;//WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;// | SBS_SIZEBOX | WS_VSCROLL | WS_HSCROLL ;//  | SBS_SIZEBOXBOTTOMRIGHTALIGN; // FIXME: Fix a resize "nudge" in some way
		}
		else if(p_eType == EM_WINDOW_POPUP)
		{
			m_vStyleEX = 0;
			m_vStyle = WS_POPUP;
			vParent = GetMainWindowHandle();
		}
		else if(p_eType == EM_WINDOW_SPLASH)
		{
			m_vStyleEX = 0;//WS_EX_TOPMOST;
			m_vStyle = WS_POPUP | WS_BORDER;
			vParent = GetDummyWindowHandle();
		}
		else if(p_eType == EM_WINDOW_POPUP_BORDER)
		{
			m_vStyleEX = WS_EX_DLGMODALFRAME;
			m_vStyle = WS_POPUP;// | DS_3DLOOK;
			vParent = GetMainWindowHandle();
		}
		else if(p_eType == EM_WINDOW_UNRESIZABLE)
		{
			m_vStyleEX = WS_EX_OVERLAPPEDWINDOW;
			m_vStyle = WS_CLIPCHILDREN | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		}
		else
			EMDebugger("This window type is not implemented yet");

		RECT rc;
		rc.left = p_oRect.m_vLeft;
		rc.top = p_oRect.m_vTop;
		rc.right = p_oRect.m_vRight;
		rc.bottom = p_oRect.m_vBottom;

//		AdjustWindowRect(&rc, m_vStyle, false);
		AdjustWindowRectEx(&rc, m_vStyle, false, m_vStyleEX);

		m_oBounds.m_vRight = -rc.left + rc.right;
		m_oBounds.m_vBottom = -rc.top + rc.bottom;

		m_opWindowHandle = CreateWindowEx(m_vStyleEX, oUniqueName.c_str(), 
		p_vpName, m_vStyle, p_oRect.m_vLeft,  p_oRect.m_vTop, -rc.left + rc.right/*p_oRect.m_vRight - p_oRect.m_vLeft*/, 
		-rc.top + rc.bottom/*p_oRect.m_vBottom - p_oRect.m_vTop*/, vParent/*hParent*/, NULL/*hMenu*/, EMApplication::GetApplicationInstance()/*m_vApplicationInstance*/, NULL);
/*
		m_opWindowHandle = CreateWindow(p_vpName, p_vpName, WS_OVERLAPPEDWINDOW, p_oRect.m_vLeft, 
										p_oRect.m_vTop,  p_oRect.m_vRight - p_oRect.m_vLeft, 
										p_oRect.m_vBottom - p_oRect.m_vTop, NULL, NULL, 
										m_vApplicationInstance, NULL);
*/
//		SetWindowLong(m_opWindowHandle, 0, static_cast<int32>(EM_COMPONENT_WINDOW));
		SetWindowLong(m_opWindowHandle, GWL_USERDATA, reinterpret_cast<int32>(this));

eo << "Window created: name: " << p_vpName << ", width: " << m_oFrame.GetWidth() << ", height: " << m_oFrame.GetHeight() << ef;
	}

	if(m_opMainWindow == NULL && (p_eType == EM_WINDOW_NORMAL || p_eType == EM_WINDOW_UNRESIZABLE))
		m_opMainWindow = this;
//	ShowWindow(m_opWindowHandle, SW_SHOW);
}

EMWinWindow::~EMWinWindow()
{
	delete m_opSemaphore;
//	SetWindowLong(m_opWindowHandle, GWL_USERDATA, 0);
	DestroyWindow(m_opWindowHandle);
	m_opWindowHandle = NULL;
	m_vWindowIsDeleted = true;
}

// In the MS Windows case, components add themselves
void EMWinWindow::AddChild(EMGUIComponent* p_opView)
{
//	AssertLocked();
	if(dynamic_cast<EMWinView*>(p_opView))
		(dynamic_cast<EMWinView*>(p_opView)) -> SetWindowWindow(this);
	p_opView -> AddToComponent(this);
	m_oChildComponents.push_back(p_opView);
}

void EMWinWindow::AssertLocked() const
{
	if(m_vLockingThreadID == -1)
		EMDebugger("The window must be locked before calling this method");
}
/*
void EMWinWindow::BeginPaint()
{
	m_vPaintHandle = ::BeginPaint(m_opWindowHandle, &m_sPaintStruct);
}

void EMWinWindow::EndPaint()
{
	::EndPaint(m_opWindowHandle, &m_sPaintStruct);
}
*/
EMRect EMWinWindow::Bounds() const	// FIXME: When time permits, change this to non-const function. NOTE: This is a big fix for almost every class in GUI-framework
{
//	AssertLocked();
	switch(m_eWindowType)
	{
	case EM_WINDOW_DIALOG:
	case EM_WINDOW_POPUP:
		break;
	default:
		if(m_vReSize)
		{
			RECT sRect;
			RECT sRect2;
			GetWindowRect(m_opWindowHandle, &sRect);
			sRect2 = sRect;
			//opWindow -> SetFrame(EMRect(sRect.left, sRect.top, sRect.right, sRect.bottom));
			if(GetMenu(m_opWindowHandle) == NULL)
			{
				AdjustWindowRect(&sRect, m_vStyle, false);
				(int32)m_oFrame.m_vLeft		= sRect2.left - (sRect.left - sRect2.left);
				(int32)m_oFrame.m_vTop		= sRect2.top - (sRect.top - sRect2.top);
				(int32)m_oFrame.m_vRight	= sRect2.right - (sRect.right - sRect2.right);
				(int32)m_oFrame.m_vBottom	= sRect2.bottom - (sRect.bottom - sRect2.bottom);
				(int32)m_oBounds.m_vRight	= ((EMRect)m_oFrame).GetWidth();
				(int32)m_oBounds.m_vBottom	= ((EMRect)m_oFrame).GetHeight();

			}
			else
			{
				AdjustWindowRect(&sRect, m_vStyle, true);
				(int32)m_oFrame.m_vLeft		= sRect2.left - (sRect.left - sRect2.left) + 2;
				(int32)m_oFrame.m_vTop		= sRect2.top - (sRect.top - sRect2.top) + 2;
				(int32)m_oFrame.m_vRight	= sRect2.right - (sRect.right - sRect2.right) * 2 + 1;
				(int32)m_oFrame.m_vBottom	= sRect2.bottom - (sRect.bottom - sRect2.bottom) * 2 + 1;
				(int32)m_oBounds.m_vRight	= ((EMRect)m_oFrame).GetWidth();
				(int32)m_oBounds.m_vBottom	= ((EMRect)m_oFrame).GetHeight();
			}
			(bool)m_vReSize = false;
		}
		break;
	}
//eo << "Bounds() New window dimensions (bounds/frame):" << ef;
//((EMRect)m_oBounds).Print();
//((EMRect)m_oFrame).Print();

	return m_oBounds;
}

void EMWinWindow::CleanUp()
{
	if(m_opWindowIcon != NULL)
		DestroyIcon(m_opWindowIcon);
	DestroyWindow(m_opDummyWindowHandle);
}

void EMWinWindow::ConvertFromWorkAreaToWindow(EMRect &p_oRect)
{
	RECT rc;
	rc.left = p_oRect.m_vLeft;
	rc.top = p_oRect.m_vTop;
	rc.right = p_oRect.m_vRight;
	rc.bottom = p_oRect.m_vBottom;
	
	AdjustWindowRectEx(&rc, m_vStyle, true, m_vStyleEX);

	p_oRect.m_vLeft = rc.left;
	p_oRect.m_vTop = rc.top;
	p_oRect.m_vRight = rc.right + 1;
	p_oRect.m_vBottom = rc.bottom + 1;
}

EMRect EMWinWindow::Frame() const	// FIXME: When time permits, change this to non-const function. NOTE: This is a big fix for almost every class in GUI-framework
{
//	AssertLocked();
	switch(m_eWindowType)
	{
	case EM_WINDOW_DIALOG:
	case EM_WINDOW_POPUP:
		break;
	default:
		if(m_vReSize)
		{
			RECT sRect;
			RECT sRect2;
			GetWindowRect(m_opWindowHandle, &sRect);
			sRect2 = sRect;
			//opWindow -> SetFrame(EMRect(sRect.left, sRect.top, sRect.right, sRect.bottom));
			if(GetMenu(m_opWindowHandle) == NULL)
			{
				AdjustWindowRect(&sRect, m_vStyle, false);
				(int32)m_oFrame.m_vLeft		= sRect2.left - (sRect.left - sRect2.left);
				(int32)m_oFrame.m_vTop		= sRect2.top - (sRect.top - sRect2.top);
				(int32)m_oFrame.m_vRight	= sRect2.right - (sRect.right - sRect2.right);
				(int32)m_oFrame.m_vBottom	= sRect2.bottom - (sRect.bottom - sRect2.bottom);
				(int32)m_oBounds.m_vRight	= ((EMRect)m_oFrame).GetWidth();
				(int32)m_oBounds.m_vBottom	= ((EMRect)m_oFrame).GetHeight();

			}
			else
			{
				AdjustWindowRect(&sRect, m_vStyle, true);
				(int32)m_oFrame.m_vLeft		= sRect2.left - (sRect.left - sRect2.left) + 2;
				(int32)m_oFrame.m_vTop		= sRect2.top - (sRect.top - sRect2.top) + 2;
				(int32)m_oFrame.m_vRight	= sRect2.right - (sRect.right - sRect2.right) * 2 + 1;
				(int32)m_oFrame.m_vBottom	= sRect2.bottom - (sRect.bottom - sRect2.bottom) * 2 + 1;
				(int32)m_oBounds.m_vRight	= ((EMRect)m_oFrame).GetWidth();
				(int32)m_oBounds.m_vBottom	= ((EMRect)m_oFrame).GetHeight();
			}
			(bool)m_vReSize = false;
		}
		break;
	}
//eo << "Frame() New window dimensions (bounds/frame):" << ef;
//((EMRect)m_oBounds).Print();
//((EMRect)m_oFrame).Print();

	return m_oFrame;
}

HWND EMWinWindow::GetDummyWindowHandle()
{
	if(m_opDummyWindowHandle == NULL)
	{
		WNDCLASSEX sWindowClass;
		sWindowClass.cbSize = sizeof(WNDCLASSEX);
		sWindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		sWindowClass.lpfnWndProc = GlobalWindowProcedure;
		sWindowClass.cbClsExtra = 0;
		sWindowClass.cbWndExtra = 0;       
		sWindowClass.hInstance = EMApplication::GetApplicationInstance();
		sWindowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		sWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		sWindowClass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
		sWindowClass.lpszMenuName = NULL;
		sWindowClass.lpszClassName = "DummyWindow";
		sWindowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		RegisterClassEx(&sWindowClass);

		m_opDummyWindowHandle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "DummyWindow", 
		"DummyWindow", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, 0, 0, 10, 10, NULL, NULL, EMApplication::GetApplicationInstance(), NULL);
		if(m_opDummyWindowHandle == NULL)
eo << "m_opDummyWindowHandle == NULL" << ef;
	}
	return m_opDummyWindowHandle;
}

HWND EMWinWindow::GetMainWindowHandle()
{
	if(m_opMainWindow == NULL)
		EMDebugger("The main window is not set yet");
	return m_opMainWindow -> GetWindowHandle();
}

void* EMWinWindow::GetNativeView() const
{
	return m_opWindowHandle;
}

EMWindowType EMWinWindow::GetType()
{
	return m_eWindowType;
}

HWND EMWinWindow::GetWindowHandle()
{
	return m_opWindowHandle;
}

// Generic window procedure passed to WIN32 API
// p_vParameterOne in Windows terms is "wParam"
// p_vParameterTwo in Windows terms is "lParam"
LRESULT CALLBACK EMWinWindow::GlobalWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo)
{
//return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
	EMWinWindow * opWindow;
        
	opWindow = (EMWinWindow *)GetWindowLong(p_vWindowHandle, GWL_USERDATA);
//if(p_vMessage == WM_RBUTTONUP)
//	if(opWindow != NULL)
//		eo << "GlobalWindowProcedure opWindow = " << (char*)opWindow -> m_oName.c_str() << "Message = " << p_vMessage << ef;
//	else
//		eo << "GlobalWindowProcedure opWindow = " << (uint32)opWindow << ef;
//	bool vWillDraw(false);
	if(opWindow)
	{
		m_vWindowIsDeleted = false;

		uint32 vMessage(0);
		switch(p_vMessage)
		{
#ifndef EM_BASIC_COMPONENTS
		case WM_COMMAND:
			{
				// Assume here that these messages comes from a menu since buttons etc. 
				// always should be added to EMViews, but let's check just to be safe
				if(p_vParameterTwo != 0) // No menu message if this value is a window handle
				{
					EMDebugger("The message is not coming from a menu");
					return 0;
				}
				uint32 vCommand = LOWORD(p_vParameterOne);

				//dynamic_cast<EMWinMenuBar*>(EMMenuBar::Instance()) -> SetLastTriggeredMenuItemID(vCommand);
				EMWinMenuBar::SetLastTriggeredMenuItemID(vCommand);

				if(vCommand < COMMAND_NOT_TRIGGERED_MENU_ITEM_OFFSET)
					EMCommandRepository::Instance() -> ExecuteTriggeredCommand(vCommand);

				EMMenuBar::Instance() -> Notify(EM_MENU_ITEM_TRIGGERED);
				return 0;
			}
#endif // EM_BASIC_COMPONENTS
		case WM_MOVE:
			if(p_vParameterOne == SIZE_MINIMIZED) // Do not process minimizing windows
				break;
			opWindow -> m_vReSize = true;
			break;

		case WM_SIZE:
			{
				if(p_vParameterOne == SIZE_MINIMIZED) // Do not process minimizing windows
					break;
				vMessage = EM_VIEW_BOUNDS_CHANGED;
				opWindow -> m_vReSize = true;
				break;
/*
				EMRect oOldBounds = opWindow -> m_oBounds;
				EMRect oTempRect;
				(opWindow -> m_oFrame).m_vLeft = LOWORD(p_vParameterTwo) - 1;
				(opWindow -> m_oFrame).m_vTop = HIWORD(p_vParameterTwo) - 1;
				(opWindow -> m_oFrame).m_vRight = LOWORD(p_vParameterTwo) - 1 + (opWindow -> m_oFrame).m_vLeft;
				(opWindow -> m_oFrame).m_vBottom = HIWORD(p_vParameterTwo) - 1 + (opWindow -> m_oFrame).m_vTop;
				opWindow -> m_oBounds.m_vRight = (opWindow -> m_oFrame).GetWidth();
				opWindow -> m_oBounds.m_vBottom = (opWindow -> m_oFrame).GetHeight();

eo << "New window dimensions (bounds/frame):" << ef;
(opWindow -> Bounds()).Print();
(opWindow -> Frame()).Print();

				// Since most WM_SIZE comes from SetFrame, the window is already locked
				if(opWindow -> IsLocked())
				{
					EMGUIUtilities::Instance() -> ResizeToParent(&(opWindow -> m_oChildComponents), oOldBounds, opWindow -> m_oBounds);
				}
				else if(opWindow -> Lock())
				{
					EMGUIUtilities::Instance() -> ResizeToParent(&(opWindow -> m_oChildComponents), oOldBounds, opWindow -> m_oBounds);
					opWindow -> Unlock();
				}
				break;
*/	
			}

		case WM_GETMINMAXINFO:
			{
				LPPOINT opPoint = reinterpret_cast<LPPOINT>(p_vParameterTwo);
				opPoint[3].x = opWindow -> m_vMinimumWidth;
				opPoint[3].y = opWindow -> m_vMinimumHeight;
				opPoint[4].x = opWindow -> m_vMaximumWidth;
				opPoint[4].y = opWindow -> m_vMaximumHeight;
				break;
			}

		case WM_KEYDOWN:
			{
				//eo << "Window: WM_KEYDOWN: " << static_cast<int>(p_vParameterOne) << ef;
				if(opWindow -> m_eWindowType == EM_WINDOW_DIALOG)
					break;
				// If a mouse button is pressed, keys are ignored
				if(GetKeyState(VK_LBUTTON) < 0 || GetKeyState(VK_RBUTTON) < 0 || GetKeyState(VK_MBUTTON) < 0 || p_vParameterTwo == VK_F1)
					break;
				uint32 vKey =  TranslateKey(static_cast<int>(p_vParameterOne));
				if(vKey != 0)
					EMKeyboard::Instance() -> NotifyKey(opWindow,  vKey, true, EMKeyboard::Instance() -> GetModifiers());
				break;
			}

		case WM_KEYUP:
			{
				//eo << "Window: WM_KEYUP: " << static_cast<int>(p_vParameterOne) << ef;
				if(opWindow -> m_eWindowType == EM_WINDOW_DIALOG)
					break;
				// If a mouse button is pressed, keys are ignored
//				if(GetKeyState(VK_LBUTTON) < 0 || GetKeyState(VK_RBUTTON) < 0 || GetKeyState(VK_MBUTTON) < 0)
//					break;
				uint32 vKey =  TranslateKey(static_cast<int>(p_vParameterOne));
				if(vKey != 0)
					EMKeyboard::Instance() -> NotifyKey(opWindow,  vKey, false, EMKeyboard::Instance() -> GetModifiers());
				break;
			}
/*
		case WM_CHAR:
			eo << "WM_CHAR: " << (int) p_vParameterOne << ef;
			if(opWindow -> Lock())
			{
				EMKeyboard::Instance() -> NotifyKey(opWindow,  static_cast<int>(p_vParameterOne), true, EMKeyboard::Instance() -> GetModifiers());//vModifiers);
				opWindow -> Unlock();
			}
			break;
*/
		case WM_SETFOCUS:
			//opWindow -> m_vHasFocus = true;
			vMessage = EM_GUI_COMPONENT_GOT_FOCUS;
			break;
/*
		case WM_KILLFOCUS:
			eo << "Window: WM_KILLFOCUS" << ef;
//			SetFocus(GetParent(reinterpret_cast<HWND>(p_vParameterOne)));
			opWindow -> m_vHasFocus = false;
		return 0;

		case WM_ENABLE:
			opWindow -> m_vIsEnabled = p_vParameterOne != 0;
			return 0;
*/
		case WM_CLOSE:
			vMessage = EM_CLOSE_WINDOW;
			break;

/*		case WM_DESTROY:
			//vMessage = EM_QUIT_REQUESTED;
			//break;
			return 0;
		case WM_PARENTNOTIFY:
			return 0;
*/
		case NATIVE_MESSAGE_QUIT_APPLICATION:
			{
				EMApplication::Instance() -> QuitApplication();
				return 0;
			}

		default:
			break;
		}

		if(vMessage != 0)
			if(opWindow -> Lock())
			{
				if(opWindow -> Notify(vMessage))
				{
					if(!m_vWindowIsDeleted)
						opWindow -> Unlock();
					return 0;
				}
				if(!m_vWindowIsDeleted)
					opWindow -> Unlock();
			}
	}
#ifndef EM_BASIC_COMPONENTS
	if(p_vMessage == WM_COMMAND)
	{
		switch(HIWORD(p_vParameterOne))
		{
		case EN_UPDATE:
			HWND opRealWindowHandle;
			opRealWindowHandle = reinterpret_cast<HWND>(p_vParameterTwo);
			return EMWinTextControl::GlobalWindowProcedure(opRealWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
		};
	}
#endif // EM_BASIC_COMPONENTS
	return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
}
 
void EMWinWindow::Hide()
{
	ShowWindow(m_opWindowHandle, SW_HIDE);
}

bool EMWinWindow::IsEnabled()
{
	return m_vIsEnabled;
}

bool EMWinWindow::IsHidden()
{
	AssertLocked();
	return IsWindowVisible(m_opWindowHandle) == 0;
}

bool EMWinWindow::IsLocked()
{
	return m_vLockingThreadID != -1;
}

bool EMWinWindow::IsLockedByThread()
{
	return GetCurrentThreadId() == m_vLockingThreadID;
}

bool EMWinWindow::IsMaximized()
{
//	AssertLocked();
	return IsZoomed(m_opWindowHandle) != 0;
}

bool EMWinWindow::Lock()
{
//eo << "Locking window " << (long) m_opWindowHandle << " with thread ID: " << GetCurrentThreadId() << " and current thread ID: " << m_vLockingThreadID << ef;
	if(m_vLockingThreadID == GetCurrentThreadId())
	{
//eo << "Locking twice: " << m_vLockingThreadID << ", " << GetCurrentProcessId() << ef;
//int32 xxx = m_vLockingThreadID;
//int32 yyy = GetCurrentThreadId();
//		EMDebugger("Trying to lock semaphore by same thread twice");//return true;
		m_vLockCount++;
//eo << "Locking window by same thread: " << m_vLockingThreadID << ", lock count is now: " << m_vLockCount << ef;
		return true;
	}
int32 vCurrentThreadID = GetCurrentThreadId(); // Debug stuff
//eo << m_oName.c_str() <<": Will Acquire" << ef;
	m_opSemaphore -> Acquire();
	m_vLockCount++;
//eo << m_oName.c_str() <<":    Done" << ef;
	m_vLockingThreadID = GetCurrentThreadId();
	return true;
}

WPARAM EMWinWindow::MessageLoop(void)
{
	MSG sMessage;

	while(GetMessage(&sMessage, NULL, 0, 0))
	{
//		if(!TranslateAccelerator((HWND) m_opMainWindow -> GetNativeView(), m_opAcceleratorHandle, &sMessage))
		{
			TranslateMessage(&sMessage);
			DispatchMessage(&sMessage);
		}
	}

	return sMessage.wParam;
}

void EMWinWindow::MoveTo(int32 p_vX, int32 p_vY)
{
	MoveWindow(m_opWindowHandle, p_vX, p_vY, m_oBounds.GetWidth(), m_oBounds.GetHeight(), true);
}

void EMWinWindow::SetBounds(EMRect p_oBounds)
{
	SetFrame(p_oBounds);
}

void EMWinWindow::SetCurrentFocus()
{
	if(__m_vNextWasCalled || __m_vPreviosWasCalled)
	{
		__m_vNextWasCalled = false;
		__m_vPreviosWasCalled = false;
		return;
	}
	HWND vCurrentFocus = GetFocus();
	for(int vCount = 0; vCount < __m_oTabOrder.size(); vCount++)
	{
		if(vCurrentFocus == static_cast<HWND>(__m_oTabOrder[vCount] -> GetNativeView()))
		{
			__m_oTabOrder[vCount] -> SetFocus();
			__m_vCurrentComponent = vCount;
			break;
		}
	}
}

void EMWinWindow::SetFrame(EMRect p_oFrame)
{
	SetFrame(p_oFrame, true);
}

void EMWinWindow::SetFrame(EMRect p_oFrame, bool p_vAdjustRect)
{
	AssertLocked();
	EMRect oOldBounds = m_oBounds;
	
		RECT rc;
		rc.left = p_oFrame.m_vLeft;
		rc.top = p_oFrame.m_vTop;
		rc.right = p_oFrame.m_vRight;
		rc.bottom = p_oFrame.m_vBottom;
	if(p_vAdjustRect)
	{
		//AdjustWindowRect(&rc, m_vStyle, false);
		AdjustWindowRectEx(&rc, m_vStyle, false, m_vStyleEX);
	eo << "EMWinWindow::SetFrame: ";
	p_oFrame.Print();
	}
		p_oFrame.m_vRight = rc.right + p_oFrame.m_vLeft - rc.left;
		p_oFrame.m_vBottom = rc.bottom + p_oFrame.m_vTop - rc.top;
	m_oBounds.m_vRight = p_oFrame.GetWidth();
	m_oBounds.m_vBottom = p_oFrame.GetHeight();
eo << " set frame: " << p_oFrame.m_vRight << ", " << p_oFrame.m_vBottom << ", " << m_oBounds.m_vRight << ", " << m_oBounds.m_vBottom << ef;
	
	MoveWindow(m_opWindowHandle, p_oFrame.m_vLeft, p_oFrame.m_vTop, p_oFrame.GetWidth(), p_oFrame.GetHeight(), true);
//	EMGUIUtilities::Instance() -> ResizeToParent(&m_oChildComponents, oOldBounds, p_oFrame);
}

void EMWinWindow::SetMaximized(bool p_vMaximized)
{
	AssertLocked();
	if(p_vMaximized)
		ShowWindow(m_opWindowHandle, SW_SHOWMAXIMIZED);
	else
		ShowWindow(m_opWindowHandle, SW_SHOWNORMAL);
}

/*
void EMWinWindow::SetPopUpMenu(EMWinPopUpMenu* p_opMenu)
{
	m_opPopUpMenu = p_opMenu;
}
*/
void EMWinWindow::SetSizeLimits(int32 p_vMinimumWidth, int32 p_vMaximumWidth, int32 p_vMinimumHeight, int32 p_vMaximumHeight)
{
	m_vMaximumHeight = static_cast<int>(p_vMaximumHeight);
	m_vMaximumWidth = static_cast<int>(p_vMaximumWidth);
	m_vMinimumHeight = static_cast<int>(p_vMinimumHeight);
	m_vMinimumWidth = static_cast<int>(p_vMinimumWidth);
}

void EMWinWindow::SetTitle(const char* p_vpTitle)
{
	// FIXME: Check if window is lock?
	SetWindowText(m_opWindowHandle, p_vpTitle);
}

void EMWinWindow::Show()
{
	ShowWindow(m_opWindowHandle, SW_SHOW);
}

WPARAM EMWinWindow::StartMessageLoop()
{
	if(m_opMainWindow == NULL) // If there are no windows at all
		return 0;
	return m_opMainWindow -> MessageLoop();
}

uint32 EMWinWindow::TranslateKey(uint32 p_vKeyCode)
{
	if(p_vKeyCode >= VK_NUMPAD0 && p_vKeyCode <= VK_NUMPAD9)
		return p_vKeyCode - 0x30;
	if(p_vKeyCode >= 97 && p_vKeyCode <=122 && p_vKeyCode != VK_ADD && p_vKeyCode != VK_SUBTRACT && p_vKeyCode != VK_MULTIPLY) //Ignore function keys but not some numpad keys
		return 0;
	if(p_vKeyCode >= 65 && p_vKeyCode <= 90)
		return p_vKeyCode + 32;
	if(p_vKeyCode == 221)
		return 229;
	if(p_vKeyCode == 222)
		return 228;
	if(p_vKeyCode == 192)
		return 246;
/*	if(p_vKeyCode == 221)
		return 197;
	if(p_vKeyCode == 222)
		return 196;
	if(p_vKeyCode == 192)
		return 214;
*/	if(p_vKeyCode == 187 || p_vKeyCode == 107)
		return '+';
	if(p_vKeyCode == 189 || p_vKeyCode == 109)
		return '-';
	if(p_vKeyCode == VK_MULTIPLY)
		return 42;//191;
	if(p_vKeyCode == 111)
		return 55;
	if(p_vKeyCode == 110)
		return 188;
	if(p_vKeyCode == 16 || p_vKeyCode == 17) // Shift and Ctrl are not sent
		return 0;
	return p_vKeyCode;
/*
	if(p_vKeyCode >= 48 && p_vKeyCode <=57)
		return '0' + p_vKeyCode - 48;
	if(p_vKeyCode >= 65 && p_vKeyCode <=90)
		return 'a' + p_vKeyCode - 65;
*/
}

void EMWinWindow::Unlock()
{
	m_vLockCount--;
	if(m_vLockCount < 0)
	{
		EMDebugger("Lock count < 0");
	//m_opSemaphore -> Acquire();
	//m_opSemaphore -> Acquire();
	//m_opSemaphore -> Acquire();
	//m_opSemaphore -> Acquire();
	//m_opSemaphore -> Acquire();
	}
	if(m_vLockCount == 0)
	{
		m_vLockingThreadID = -1;
//eo << m_oName.c_str() <<": Will Release" << ef;
		m_opSemaphore -> Release();
//eo << m_oName.c_str() <<":    Done" << ef;
//eo << "Window " << (long) m_opWindowHandle << " unlocked by thread " << GetCurrentThreadId() << ef;
	}
//	else
//		eo << "Window lock count is now " << m_vLockCount << ef;
}

#endif
