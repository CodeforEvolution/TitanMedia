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

#ifndef __EM_WIN_LIST_VIEW_CONTROL
#define __EM_WIN_LIST_VIEW_CONTROL

#include "EMGUIGlobals.h"
#include "EMRect.h"
#include "EMListViewControl.h"

#include <windows.h>
#include <commctrl.h>
#include <shlobj.h>
#include <vector>

#include "EMWinExplorerStructs.h"
#include "EMListener.h"

const uint16 EM_SORT_ASCENDING = 0x0100;
const uint16 EM_SORT_DESCENDING = 0x0200;

class EMMenuItem;
class EMPopUpMenu;
class EMWinWindow;

class EMWinDragDrop;
class EMDragDropData;


/*class LParamClass
{
public:
	HWND m_vWindowHandle;
	const char* m_vpFullPathAndFileName;
};*/

class EMWinListViewControl : public EMListViewControl
{
public:
	EMWinListViewControl(const EMRect p_oFrame, const char* p_vpName, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand, EMListViewStyle p_eListViewStyle, bool p_vEnableDragDrop, bool p_vEnableHoverSelect);
	~EMWinListViewControl();
	void AddChild(EMGUIComponent* p_opView);
	void AddColumn(char* p_opName, uint32 p_vWidth, EMAlignment p_eAlignment);
	bool AddToComponent(EMGUIComponent* p_opComponent);
	void AddItem(const char* p_opText);
	void AddSubItem(const char* p_opText, int p_vColumn);
	void AddSubItem(const char* p_opText, int p_vItemId, int p_vColumn);
	EMRect Bounds();
	void ChangeIdInItemPointerVector();
	void ChangeTextInItemPointerVector(const char* p_vLookAfterText, char* p_vChangeTextTo);
	static int CALLBACK CompareFunc(LPARAM p_vParameterOne, LPARAM p_vParameterTwo, LPARAM p_vParameterSort);
	static int CompareString(const char* p_vpStringOne, const char* p_vpStringTwo);
	void DeleteAllItems();
	bool DeleteItem(int p_vItemID);
	void DeleteSelectedItem();
	bool DoPopupMenu(LPSHELLFOLDER p_spShellFolder, LPITEMIDLIST p_spItemIdList, POINT &p_sPoint);
	void Enable(bool p_vEnable);
	void EnableHoverSelect(bool p_vEnable, int32 p_vHoverTime); // -1 = Default hovertime is set. -1 is set in parent class as default value
	void ExecuteCommand();
	EMRect Frame() const;
	int32 GetItem(const char* p_vpUniqueStringIndex);
	int GetItemCount();
	void* GetNativeView() const;
	string GetSelection(bool FullPath);
	bool GetSelection(LVITEM* p_spListViewItem);
	uint32 GetSelectionID();
	char* GetSelectionSubItem(int16 p_vColumn); //If p_vColumn == -1 UniqueIndexColumn is used
	char* GetSubItem(uint32 p_vItem, int16 p_vColomn); //If p_vColumn == -1 UniqueIndexColumn is used
	EMWinWindow* GetWindowWindow();
	EMTreeViewControl* GetTreeView();
	static LRESULT CALLBACK GlobalWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo);
	uint32 GetResizingMode();
	void Hide();				   
	int HitTest(LVHITTESTINFO* p_spListViewHitTestInfo, POINT &p_sPoint);
	void Invalidate(const EMRect p_oRect);
	bool IsDragEnabled();
	bool IsEnabled();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	BOOL PopulateListView(LPTVITEMDATA p_spItemIdList, LPSHELLFOLDER p_spShellFolder);
	void Scroll(int32 p_vItemID); // If p_vItemID == -1 last item is selected and will only scroll if PrevItem bottom is out of bounds
	void SetCommand(uint32 p_vCommand);
	void SetFocus();
	void SetFrame(EMRect p_oFrame);
	void SetLabel(const char* p_vpLabel);
	void SetTreeView(EMTreeViewControl* p_vTreeView);
	void SetUniqueIndexColumn(int16 p_vUniqueIndexColumn); // If set to -1 sort will not work and no unique index column will be set
	void SetWindowWindow(EMWinWindow* p_opWindow);
	void Show();
	void SwitchView(EMListViewStyle p_eListViewStyle);

protected:
	void ChangeSubItem(uint32 p_vItemID, uint16 p_vColumn, const char* p_vpChangeItTo);
	LPITEMIDLIST CopyItemIdList(LPMALLOC p_opMalloc, LPITEMIDLIST p_spItemIdList);
	LPITEMIDLIST CreateItemIdList(uint32 cbSize);
	int GetIcon(LPITEMIDLIST p_spItemIdList, uint32 p_vFlags);
	bool GetName(LPSHELLFOLDER p_spShellFolder, LPITEMIDLIST p_spItemIdList, uint32 p_vFlags, char* p_vpFriendlyName);
	LPITEMIDLIST GetNext(const LPITEMIDLIST p_spItemIdList);
	uint32 GetSize(const LPITEMIDLIST p_spItemIdList);
	bool WINAPI InitiateListViewImageLists();
	BOOL WINAPI InitiateListViewItems(LPTVITEMDATA lptvid, LPSHELLFOLDER lpsf);
	LPITEMIDLIST MergeItemIdLists(const LPITEMIDLIST p_spItemIdListOne, const LPITEMIDLIST p_spItemIdListTwo);

	void SetDragEnter(bool p_vDragEnter){m_vDragEnter = p_vDragEnter;};
	void SetDragMove(bool p_vDragMove){m_vDragMove = p_vDragMove;};
	void SetDragLeave(bool p_vDragLeave){m_vDragLeave = p_vDragLeave;};
	void SetDragDrop(bool p_vDragDrop){m_vDragDrop = p_vDragDrop;};
	bool IsDragEnter(){return m_vDragEnter;};
	bool IsDragMove(){return m_vDragMove;};
	bool IsDragLeave(){return m_vDragLeave;};
	bool IsDragDrop(){return m_vDragDrop;};

private:
	static LRESULT CALLBACK SubclassedWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo);

private:
	EMRect m_oBounds;
	list<EMGUIComponent*> m_oChildComponents;
	EMRect m_oFrame;
	vector<LPLVITEM> m_oItemPointers;  //FIXME: Make me a list instead of a vector. When you got time.
	EMTreeViewControl* m_opTreeViewControl;
	HWND  m_opWindowHandle;
	EMWinWindow* m_opWindowWindow; // The actual window that lies on the desktop
	//LVCOLUMN m_vColumnInfo;
	uint32 m_vCommand;
	bool m_vEnableDrag;
	//static uint32 m_vCompareItemCounter;
	uint32 m_vMessage;
	uint32 m_vResizingMode;
	DWORD m_vStyle;
	DWORD m_vListView_EX_Style;

	LPDROPTARGET m_opIDropTarget;
	string m_oSelection;
	char m_vpSelection[EM_MAX_STRING_BUFFER_SIZE];
	int32 m_vItemID;
	EMMenuItem* m_opMenuItem;
	EMPopUpMenu* m_opPopupMenu;
	int16 m_vUniqueIndexColumn;
	WNDPROC m_opDefaultWindowProcedure;

	bool m_vDragEnter;
	bool m_vDragMove;
	bool m_vDragLeave;
	bool m_vDragDrop;

	EMWinDragDrop* m_opDragDrop;
	EMDragDropData* m_opDragDropData;
};

#endif

#endif
