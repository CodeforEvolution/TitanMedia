/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_LIST_BOX
#define __EM_WIN_LIST_BOX

#include "EMGUIGlobals.h"
#include "EMRect.h"
#include "EMListBox.h"

#include <windows.h>

class EMWinListBox : public EMListBox
{
public:
	EMWinListBox(const EMRect p_oFrame, uint32 p_vMessage, uint32 p_vResizingMode, bool p_vAutoSort, bool p_vMultipleSelection);
	~EMWinListBox();
	bool AddString(string p_oItem);
	bool AddToComponent(EMGUIComponent* p_opComponent);
	void Clear();
	void Enable(bool p_vEnable);
	void ExecuteCommand();
	EMRect Frame() const;
	void* GetNativeView() const;
	int GetNrItems();
	string GetSelection();
	int GetSelectionIndex();
	static LRESULT CALLBACK GlobalWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo);
	uint32 GetResizingMode();
	void Hide();
	bool InsertString(string p_oItem, int p_vIndex);
	bool IsEnabled();
	bool IsVisible();
	void RemoveString(int p_vIndex);
	void SelectItem(int p_vIndex);
	void SetFocus();
	void SetFrame(EMRect p_oFrame);
	void Show();

private:
	static LRESULT CALLBACK SubclassedWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo);

private:
	EMRect m_oFrame;
	WNDPROC m_opDefaultWindowProcedure;
	HWND  m_opWindowHandle;
	uint32 m_vMessage;
	uint32 m_vResizingMode;
	int32 m_vStyle;
};

#endif

#endif
