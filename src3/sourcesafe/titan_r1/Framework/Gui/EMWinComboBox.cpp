#include "EMWinComboBox.h"

#include "EMApplication.h"
#include "EMCommandRepository.h"
#include "EMKeyboard.h"
#include "EMKeyboardGlobals.h"
#include "EMMessages.h"
#include "EMWinGroupBox.h"
#include "EMWinView.h"
#include "EMWinWindow.h"

const int MAX_ITEM_LENGTH = 255;

EMWinComboBox::EMWinComboBox(const EMRect p_oFrame, uint32 p_vMessage, uint32 p_vResizingMode, bool p_vAutoSort) :
m_oFrame(p_oFrame),
m_vMessage(p_vMessage),
m_vResizingMode(p_vResizingMode),
m_vStyle(WS_CHILD | WS_VISIBLE | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST | WS_VSCROLL)
{
	if(p_vAutoSort)
		m_vStyle |= CBS_SORT;
// Hmm, why can't the window be created here (won't send any messages)?
/*
	m_opWindowHandle = CreateWindowEx(WS_EX_CLIENTEDGE, "combobox", "combobox", vStyle, m_oFrame.m_vLeft,  
							m_oFrame.m_vTop, m_oFrame.m_vRight - m_oFrame.m_vLeft + 1, m_oFrame.m_vBottom - m_oFrame.m_vTop + 1,
							EMWinWindow::GetDummyWindowHandle(), NULL, EMApplication::GetApplicationInstance(), NULL);

	SendMessage(m_opWindowHandle, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), true);
	SetWindowLong(m_opWindowHandle, GWL_USERDATA, reinterpret_cast<int32>(this));
*/
}

EMWinComboBox::~EMWinComboBox()
{
	SetWindowLong(m_opWindowHandle, GWL_WNDPROC, reinterpret_cast<LONG>(m_opDefaultWindowProcedure));
	SetWindowLong(m_opWindowHandle, GWL_USERDATA, 0);
	DestroyWindow(m_opWindowHandle);
}

bool EMWinComboBox::AddString(string p_oItem)
{
	SendMessage(m_opWindowHandle, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(p_oItem.c_str()));
//SelectItem(GetNrItems() - 1);
	return true;
}

bool EMWinComboBox::AddToComponent(EMGUIComponent* p_opComponent)
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

	m_opWindowHandle = CreateWindowEx(0, "combobox", "combobox", m_vStyle, m_oFrame.m_vLeft,  
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

void EMWinComboBox::Clear()
{
	SendMessage(m_opWindowHandle, CB_RESETCONTENT, 0, 0);
}

void EMWinComboBox::Enable(bool p_vEnable)
{
	EnableWindow(m_opWindowHandle, p_vEnable);
}

int EMWinComboBox::FindString(const char* p_vpString)
{
	int vResult(SendMessage(m_opWindowHandle, CB_FINDSTRINGEXACT, 0, reinterpret_cast<LPARAM>(const_cast<char*>(p_vpString))));
	if(vResult == CB_ERR)
		return -1;
	return vResult;
}

EMRect EMWinComboBox::Frame() const
{
	return m_oFrame;
}

void* EMWinComboBox::GetNativeView() const
{
	return m_opWindowHandle;
}

int EMWinComboBox::GetNrItems()
{
	return SendMessage(m_opWindowHandle, CB_GETCOUNT, 0, 0);
}

string EMWinComboBox::GetSelection()
{
	char vString[MAX_ITEM_LENGTH];
	SendMessage(m_opWindowHandle, CB_GETLBTEXT, SendMessage(m_opWindowHandle, CB_GETCURSEL, 0, 0), reinterpret_cast<LPARAM>(vString));
	return string(vString);
}

int EMWinComboBox::GetSelectionIndex()
{
	return SendMessage(m_opWindowHandle, CB_GETCURSEL, 0, 0);
}

LRESULT CALLBACK EMWinComboBox::GlobalWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo)
{
//eo << "EMWinComboBox::GlobalWindowProcedure" << ef;
	EMWinComboBox* opComboBox;
	opComboBox = reinterpret_cast<EMWinComboBox*>(GetWindowLong(p_vWindowHandle, GWL_USERDATA));

	if(opComboBox)
	{
		uint32 vMessage;
		switch(HIWORD(p_vParameterOne))
		{
		case CBN_EDITCHANGE:
			vMessage = 0;//opComboBox -> m_vMessage;
			break;
		case CBN_SELCHANGE:
			eo << "CBN_SELCHANGE" << ef;
			vMessage = opComboBox -> m_vMessage;
			break;
		default:
			vMessage = 0;
		}
		if(vMessage != 0)
			if(opComboBox -> Notify(vMessage))
				return 0;
	}
	return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
}

uint32 EMWinComboBox::GetResizingMode()
{
	return m_vResizingMode;
}

void EMWinComboBox::Hide()
{
	ShowWindow(m_opWindowHandle, SW_HIDE);
}

bool EMWinComboBox::InsertString(string p_oItem, int p_vIndex)
{
	SendMessage(m_opWindowHandle, CB_INSERTSTRING, p_vIndex, reinterpret_cast<LPARAM>(p_oItem.c_str()));
	return true;
}

bool EMWinComboBox::IsEnabled()
{
	return IsWindowEnabled(m_opWindowHandle) != 0;
}

bool EMWinComboBox::IsVisible()
{
	return IsWindowVisible(m_opWindowHandle) != 0;
}

void EMWinComboBox::RemoveString(int p_vIndex)
{
	SendMessage(m_opWindowHandle, CB_DELETESTRING, p_vIndex, 0);
}

void EMWinComboBox::SelectItem(int p_vIndex)
{
	SendMessage(m_opWindowHandle, CB_SETCURSEL, p_vIndex, 0);
}

void EMWinComboBox::SetFocus()
{
	::SetFocus(m_opWindowHandle);
}

void EMWinComboBox::SetFrame(EMRect p_oFrame)
{
	m_oFrame = p_oFrame;
	MoveWindow(m_opWindowHandle, p_oFrame.m_vLeft, p_oFrame.m_vTop, p_oFrame.GetWidth() + 1, p_oFrame.GetHeight() + 1, true); //Changed to true to test if working ok
}

void EMWinComboBox::Show()
{
	ShowWindow(m_opWindowHandle, SW_SHOW);
}

LRESULT CALLBACK EMWinComboBox::SubclassedWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo)
{
	EMWinComboBox* opComboBox;
	opComboBox = reinterpret_cast<EMWinComboBox*>(GetWindowLong(p_vWindowHandle, GWL_USERDATA));

	if(p_vMessage == WM_CHAR)
	{					
eo << "Window: WM_KEYDOWN: " << ef;
		switch(static_cast<int>(p_vParameterOne))
		{
		case EM_RETURN:
			opComboBox -> Notify(EM_GUI_COMPONENT_ENTER_PRESSED);
			return 0;
		case EM_ESCAPE:
			opComboBox -> Notify(EM_GUI_COMPONENT_ESC_PRESSED);
			return 0;
		case EM_TAB:
			if(EM_SHIFT_KEY & EMKeyboard::Instance() -> GetModifiers())
				opComboBox -> Notify(EM_GUI_COMPONENT_SHIFT_TAB_PRESSED);	
			else
				opComboBox -> Notify(EM_GUI_COMPONENT_TAB_PRESSED);
			return 0;
		}
	}
	if(p_vMessage == WM_SETFOCUS)
		opComboBox -> Notify(EM_GUI_COMPONENT_GOT_FOCUS);	

	return CallWindowProc(opComboBox -> m_opDefaultWindowProcedure, p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
}
