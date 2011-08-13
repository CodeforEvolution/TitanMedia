/*******************************************************
* This code is the copyrighted property of Elegant Media
*-------------------------------------------------------
* Creator: Johan Dunberg
* Component: Framework, GUI
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_TREE_VIEW_CONTROL
#define __EM_WIN_TREE_VIEW_CONTROL

#include "EMGUIGlobals.h"
#include "EMRect.h"
#include "EMTreeViewControl.h"

#include <windows.h>
#include <commctrl.h>
#include <shlobj.h>
#include <vector>

#include "EMWinExplorerStructs.h"

class EMWinWindow;
//class EMWinListViewControl;

class EMWinTreeViewControl : public EMTreeViewControl
{
public:
	EMWinTreeViewControl(const EMRect p_oFrame, const char* p_vpName, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand);
	~EMWinTreeViewControl();
	void AddChild(EMGUIComponent* p_opView);
	bool AddToComponent(EMGUIComponent* p_opComponent);
//	void AddItem(const char* p_opText);
//	void AddSubItem(const char* p_opText, int p_vItemId, int p_vColumn);
	EMRect Bounds();
	void DeleteAllItems();
	void Enable(bool p_vEnable);
	void ExecuteCommand();
	void ExpandTree(LPSHELLFOLDER p_spShellFolder, ITEMIDLIST* p_spItemIdList, HTREEITEM p_vTreeItemHandle, bool p_vFirst = false);
	EMRect Frame() const;
	EMListViewControl* GetListView();
	void* GetNativeView() const;
	static LRESULT CALLBACK GlobalWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo);
	uint32 GetResizingMode();
	void Hide();
	void Invalidate(EMRect p_oRect);
	bool IsEnabled();
	void SetCommand(uint32 p_vCommand);
	void SetFrame(EMRect p_oFrame);
	void SetLabel(const char* p_vpLabel);
	void SetListView(EMListViewControl* p_opListViewControl);
	void SetWindowWindow(EMWinWindow* p_opWindow);
	void Show();
	static int CALLBACK TreeViewCompareProcedure(LPARAM lparam1, LPARAM lparam2, LPARAM lparamSort);

protected:
	LPITEMIDLIST CopyItemIdList(LPMALLOC p_opMalloc, LPITEMIDLIST p_spItemIdList);
	LPITEMIDLIST CreateItemIdList(uint32 p_vSize);
	int GetIcon(LPITEMIDLIST p_spItemIdList, uint32 p_vFlags);
	bool GetName(LPSHELLFOLDER p_spShellFolder, LPITEMIDLIST p_spItemIdList, DWORD p_vFlags, char* p_vFriendlyName); 
	LPITEMIDLIST GetNext(const LPITEMIDLIST p_spItemIdList);
	void GetNormalAndSelectedIcons(LPITEMIDLIST p_spItemIdList, LPTV_ITEM p_spTreeViewItem);
	uint32 GetSize(const LPITEMIDLIST p_spItemIdList);
	void InitiateTreeViewImageList();
	LPITEMIDLIST MergeItemIdLists(const LPITEMIDLIST p_spItemIdListOne, const LPITEMIDLIST p_spItemIdListTwo, bool p_vFirst);

private:
	static LRESULT CALLBACK SubclassedWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo);

private:
	EMRect m_oBounds;
	list<EMGUIComponent*> m_oChildComponents;
	EMRect m_oFrame;
	EMListViewControl* m_opListViewControl;
	HWND  m_opWindowHandle;
	EMWinWindow* m_opWindowWindow; // The actual window that lies on the desktop
	uint32 m_vCommand;
	uint32 m_vMessage;
	uint32 m_vResizingMode;
	DWORD m_vStyle;
	WNDPROC m_opDefaultWindowProcedure;

};

#endif

#endif
