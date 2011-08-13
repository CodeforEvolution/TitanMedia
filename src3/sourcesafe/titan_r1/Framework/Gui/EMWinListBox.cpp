#include "EMWinListBox.h"

#include "EMApplication.h"
#include "EMCommandRepository.h"
#include "EMKeyboard.h"
#include "EMKeyboardGlobals.h"
#include "EMMessages.h"
#include "EMWinGroupBox.h"
#include "EMWinView.h"
#include "EMWinWindow.h"

const int MAX_ITEM_LENGTH = 255;

EMWinListBox::EMWinListBox(const EMRect p_oFrame, uint32 p_vMessage, uint32 p_vResizingMode, bool p_vAutoSort, bool p_vMultipleSelection) :
m_oFrame(p_oFrame),
m_vMessage(p_vMessage),
m_vResizingMode(p_vResizingMode)
{
	m_vStyle = WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | LBS_NOTIFY;
	if(p_vAutoSort)
		m_vStyle |= LBS_SORT;
	if(p_vMultipleSelection)
		m_vStyle |= LBS_MULTIPLESEL;
// Hmm, why can't the window be created here (won't send any messages)?
/*
	m_opWindowHandle = CreateWindowEx(WS_EX_CLIENTEDGE, "listbox", "listbox", WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY, m_oFrame.m_vLeft,  
							m_oFrame.m_vTop, m_oFrame.m_vRight - m_oFrame.m_vLeft + 1, m_oFrame.m_vBottom - m_oFrame.m_vTop + 1,
							EMWinWindow::GetDummyWindowHandle(), NULL, EMApplication::GetApplicationInstance(), NULL);

	SendMessage(m_opWindowHandle, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), true);
	SetWindowLong(m_opWindowHandle, GWL_USERDATA, reinterpret_cast<int32>(this));
*/
}

EMWinListBox::~EMWinListBox()
{
	SetWindowLong(m_opWindowHandle, GWL_WNDPROC, reinterpret_cast<LONG>(m_opDefaultWindowProcedure));
	SetWindowLong(m_opWindowHandle, GWL_USERDATA, 0);
	DestroyWindow(m_opWindowHandle);
}

bool EMWinListBox::AddString(string p_oItem)
{
	SendMessage(m_opWindowHandle, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(p_oItem.c_str()));
	return true;
}

bool EMWinListBox::AddToComponent(EMGUIComponent* p_opComponent)
{
	HWND opParent;

	if(dynamic_cast<EMWinView*>(p_opComponent))
	{
		EMWinView* opView = dynamic_cast<EMWinView*>(p_opComponent);
		opParent = opView -> GetWindowHandle();
	}
	else if(dynamic_cast<EMWinGroupBox*>(p_opComponent))
	{
		EMWinGroupBox* opView = dynamic_cast<EMWinGroupBox*>(p_opComponent);
		opParent = static_cast<HWND>(opView -> GetNativeView());
	}
	else
	{
		EMDebugger("The parent must be a view or a group box");
		return false;
	}

	m_opWindowHandle = CreateWindowEx(WS_EX_CLIENTEDGE, "listbox", "listbox", m_vStyle, m_oFrame.m_vLeft,  
							m_oFrame.m_vTop, m_oFrame.m_vRight - m_oFrame.m_vLeft + 1, m_oFrame.m_vBottom - m_oFrame.m_vTop + 1,
							opParent, NULL, EMApplication::GetApplicationInstance(), NULL);

	SendMessage(m_opWindowHandle, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), true);
	SetWindowLong(m_opWindowHandle, GWL_USERDATA, reinterpret_cast<int32>(this));

//	SetParent(m_opWindowHandle, opParent);
//	SetWindowPos(m_opWindowHandle, NULL, m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.GetWidth() + 1, m_oFrame.GetHeight() + 1, SWP_NOZORDER);	

	m_opDefaultWindowProcedure = reinterpret_cast<WNDPROC>(GetWindowLong(m_opWindowHandle, GWL_WNDPROC));
	SetWindowLong(m_opWindowHandle, GWL_WNDPROC, reinterpret_cast<LONG>(SubclassedWindowProcedure));

	return true;
}

void EMWinListBox::Clear()
{
	SendMessage(m_opWindowHandle, LB_RESETCONTENT, 0, 0);
}

void EMWinListBox::Enable(bool p_vEnable)
{
	EnableWindow(m_opWindowHandle, p_vEnable);
}

EMRect EMWinListBox::Frame() const
{
	return m_oFrame;
}

void* EMWinListBox::GetNativeView() const
{
	return m_opWindowHandle;
}

int EMWinListBox::GetNrItems()
{
	return SendMessage(m_opWindowHandle, LB_GETCOUNT, 0, 0);
}

string EMWinListBox::GetSelection()
{
	char vString[MAX_ITEM_LENGTH];
	SendMessage(m_opWindowHandle, LB_GETTEXT, SendMessage(m_opWindowHandle, LB_GETCURSEL, 0, 0), reinterpret_cast<LPARAM>(vString));
	return string(vString);
}

int EMWinListBox::GetSelectionIndex()
{
	return SendMessage(m_opWindowHandle, LB_GETCURSEL, 0, 0);
}

LRESULT CALLBACK EMWinListBox::GlobalWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo)
{
eo << "EMWinListBox::GlobalWindowProcedure" << ef;
	EMWinListBox* opListBox;
	opListBox = reinterpret_cast<EMWinListBox*>(GetWindowLong(p_vWindowHandle, GWL_USERDATA));

	if(opListBox)
	{
//		eo << "In If" << ef;
		uint32 vMessage;
		switch(HIWORD(p_vParameterOne))
		{
		case BN_CLICKED:
			eo << "BN_CLICKED " << BN_CLICKED << ef;
			break;
		default:
			eo << "Default" << ef;
			vMessage = 0;
		}

		switch(HIWORD(p_vParameterOne))
		{
		case LBN_DBLCLK:
			vMessage = EM_LIST_BOX_DOUBLE_CLICKED;
			break;
		case LBN_SELCHANGE:
			vMessage = opListBox -> m_vMessage;
			break;
		default:
			vMessage = 0;
		}
		if(vMessage != 0)
			if(opListBox -> Notify(vMessage))
				return 0;
	}
	return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
}

uint32 EMWinListBox::GetResizingMode()
{
	return m_vResizingMode;
}

void EMWinListBox::Hide()
{
	ShowWindow(m_opWindowHandle, SW_HIDE);
}

bool EMWinListBox::IsEnabled()
{
	return IsWindowEnabled(m_opWindowHandle) != 0;
}

bool EMWinListBox::IsVisible()
{
	return IsWindowVisible(m_opWindowHandle) != 0;
}

bool EMWinListBox::InsertString(string p_oItem, int p_vIndex)
{
	SendMessage(m_opWindowHandle, LB_INSERTSTRING, p_vIndex, reinterpret_cast<LPARAM>(p_oItem.c_str()));
	return true;
}

void EMWinListBox::RemoveString(int p_vIndex)
{
	SendMessage(m_opWindowHandle, LB_DELETESTRING, p_vIndex, 0);
}

void EMWinListBox::SelectItem(int p_vIndex)
{
	SendMessage(m_opWindowHandle, LB_SETCURSEL, p_vIndex, 0);
}

void EMWinListBox::SetFocus()
{
	::SetFocus(m_opWindowHandle);
}

void EMWinListBox::SetFrame(EMRect p_oFrame)
{
	m_oFrame = p_oFrame;
	MoveWindow(m_opWindowHandle, p_oFrame.m_vLeft, p_oFrame.m_vTop, p_oFrame.GetWidth() + 1, p_oFrame.GetHeight() + 1, true); //Changed to true to test if working ok
}

void EMWinListBox::Show()
{
	ShowWindow(m_opWindowHandle, SW_SHOW);
}

LRESULT CALLBACK EMWinListBox::SubclassedWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo)
{
	EMWinListBox* opListBox;
	opListBox = reinterpret_cast<EMWinListBox*>(GetWindowLong(p_vWindowHandle, GWL_USERDATA));

	if(p_vMessage == WM_CHAR)
	{					
eo << "Window: WM_KEYDOWN: " << ef;
		switch(static_cast<int>(p_vParameterOne))
		{
		case EM_RETURN:
			opListBox -> Notify(EM_GUI_COMPONENT_ENTER_PRESSED);
			return 0;
		case EM_ESCAPE:
			opListBox -> Notify(EM_GUI_COMPONENT_ESC_PRESSED);
			return 0;
		case EM_TAB:
			if(EM_SHIFT_KEY & EMKeyboard::Instance() -> GetModifiers())
				opListBox -> Notify(EM_GUI_COMPONENT_SHIFT_TAB_PRESSED);	
			else
				opListBox -> Notify(EM_GUI_COMPONENT_TAB_PRESSED);
			return 0;
		}
	}
	if(p_vMessage == WM_SETFOCUS)
		opListBox -> Notify(EM_GUI_COMPONENT_GOT_FOCUS);	

	return CallWindowProc(opListBox -> m_opDefaultWindowProcedure, p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
}

