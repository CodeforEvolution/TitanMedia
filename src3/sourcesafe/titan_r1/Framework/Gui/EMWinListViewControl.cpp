#include "EMWinListViewControl.h"

#ifdef PLATFORM_WINDOWS

#include "CommandIDs.h"
#include "DirectoryIDs.h"
#include "EMApplication.h"
#include "EMCommandRepository.h"
#include "EMKeyboard.h"
#include "EMKeyboardGlobals.h"
#include "EMMessageBox.h"
#include "EMMessages.h"
#include "EMWinGroupBox.h"
#include "EMWinView.h"
#include "EMWinWindow.h"

#include "EMWinDropSource.h"
#include "EMWinDropTarget.h"
#include "EMWinTreeViewControl.h"

#include "EMMenuItem.h"
#include "EMPopupMenu.h"
#include "EMFactory.h"

#include "EMWinDragDrop.h"


#include <Windowsx.h>


/*BOOL DoTheMenuThing(HWND hwnd, 
                    LPSHELLFOLDER lpsfParent,
                    LPITEMIDLIST  lpi,
                    LPPOINT lppt)
{
    LPCONTEXTMENU lpcm;
    HRESULT       hr;
    char          szTemp[64];
    CMINVOKECOMMANDINFO cmi;
    DWORD               dwAttribs=0;
    int                 idCmd;
    HMENU               hMenu;
    BOOL                bSuccess=TRUE;

    hr=lpsfParent->GetUIObjectOf(hwnd,
                                         1,  //Number of objects to get attributes of
                                         (const _ITEMIDLIST**)&lpi,
                                         IID_IContextMenu,
                                         0,
                                         (LPVOID *)&lpcm);
    if (SUCCEEDED(hr))  
    {
       hMenu=CreatePopupMenu();

       if (hMenu)
       {
          hr=lpcm->QueryContextMenu(hMenu,
                                            0,
                                            1,
                                            0x7fff,
                                            CMF_EXPLORE);
          if (SUCCEEDED(hr))
          {
             idCmd=TrackPopupMenu(hMenu, 
                                  TPM_LEFTALIGN | TPM_RETURNCMD | TPM_RIGHTBUTTON, 
                                  lppt->x, 
                                  lppt->y,
                                  0,
                                  hwnd,
                                  NULL);

             if (idCmd)
             {
                cmi.cbSize = sizeof(CMINVOKECOMMANDINFO);
                cmi.fMask  = 0;
                cmi.hwnd   = hwnd;
                cmi.lpVerb = MAKEINTRESOURCE(idCmd-1);
                cmi.lpParameters = NULL;
      	        cmi.lpDirectory  = NULL;
                cmi.nShow        = SW_SHOWNORMAL;
                cmi.dwHotKey     = 0;
                cmi.hIcon        = NULL;
                hr=lpcm->InvokeCommand(&cmi);
                if (SUCCEEDED(hr))  
                    ;  //Do nothing
                else
                {
                   MessageBox(hwnd, 
                              "InvokeCommand failed!!", 
                              "Shell Extension Sample", 
                              MB_OK);
                   wsprintf(szTemp, "hr=%lx", hr);
                   MessageBox(hwnd, 
                              szTemp, 
                              "Shell Extension Sample", 
                              MB_OK);
                }
             }

          }
          else
             bSuccess = FALSE;

          DestroyMenu(hMenu);
       }
       else
          bSuccess = FALSE;

       lpcm->Release();
    } 
    else
    {
       MessageBox(hwnd, 
                  "GetUIObjectOf failed!!", 
                  "Shell Extension Sample", 
                  MB_OK);
       wsprintf(szTemp, "hr=%lx", hr);
       MessageBox(hwnd, 
                  szTemp, 
                  "Shell Extension Sample", 
                  MB_OK);

       bSuccess = FALSE;
    }

    return bSuccess;
}*/

EMWinListViewControl::EMWinListViewControl(const EMRect p_oFrame, const char* p_vpName, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand, EMListViewStyle p_eListViewStyle, bool p_vEnableDrag, bool p_vEnableHoverSelect) :
m_oFrame(p_oFrame),
m_oBounds(0, 0, p_oFrame.m_vRight - p_oFrame.m_vLeft, p_oFrame.m_vBottom - p_oFrame.m_vTop),
m_vCommand(p_vCommand),
m_vMessage(p_vMessage),
m_vResizingMode(p_vResizingMode),
m_vStyle(WS_CHILD | WS_VISIBLE /*| WS_BORDER*/ | LVS_SHAREIMAGELISTS | /*LVS_EDITLABELS |*/ LVS_AUTOARRANGE | LVS_SINGLESEL),
m_vListView_EX_Style(LVS_EX_FULLROWSELECT),
m_vEnableDrag(p_vEnableDrag),
m_opTreeViewControl(NULL),
m_opIDropTarget(NULL),
m_vItemID(0),
m_opPopupMenu(EMFactory::Instance() -> CreatePopUpMenu()),
m_vUniqueIndexColumn(0),
m_vDragEnter(false),
m_vDragMove(false),
m_vDragLeave(false),
m_vDragDrop(false),
m_opDragDrop(NULL),
m_opDragDropData(NULL)
{
	switch(p_eListViewStyle)
	{
	case EM_LIST_VIEW_STYLE_ICON:
		m_vStyle |= LVS_ICON;
		break;
	case EM_LIST_VIEW_STYLE_SMALL_ICON:
		m_vStyle |= LVS_SMALLICON;
		break;
	case EM_LIST_VIEW_STYLE_LIST:
		m_vStyle |= LVS_LIST;
		break;
	case EM_LIST_VIEW_STYLE_REPORT:
		m_vStyle |= LVS_REPORT;
	//	m_vStyle |= LVS_EDITLABELS;   // Rename disabled
		break;
	}
	if(p_vEnableHoverSelect)
		m_vListView_EX_Style |= LVS_EX_TRACKSELECT;
	// To get resize, move to work, i.e. so it doesn't add a pixel on this control
	m_oFrame.m_vRight--;
	m_oFrame.m_vBottom--;

/*	INITCOMMONCONTROLSEX oInitCommonControlEX;
	oInitCommonControlEX.dwSize = sizeof(oInitCommonControlEX);
	oInitCommonControlEX.dwICC = ICC_TAB_CLASSES;

	if(InitCommonControlsEx(&oInitCommonControlEX))
	{
		m_opWindowHandle = CreateWindowEx(WS_EX_CLIENTEDGE , WC_LISTVIEW , p_vpName, m_vStyle, m_oFrame.m_vLeft,  
								m_oFrame.m_vTop, m_oFrame.m_vRight - m_oFrame.m_vLeft + 1, m_oFrame.m_vBottom - m_oFrame.m_vTop + 1,
								EMWinWindow::GetDummyWindowHandle(), NULL, EMApplication::GetApplicationInstance(), NULL);
		SendMessage(m_opWindowHandle, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), true);
		SetWindowLong(m_opWindowHandle, GWL_USERDATA, reinterpret_cast<int32>(this));
	}
	else
		eo << "Could not load CommonControl dll" << ef;
*/
//	ShowWindow(m_opWindowHandle, SW_SHOW);
	m_opPopupMenu -> AddListener(this);

	m_opMenuItem = EMFactory::Instance() -> CreateMenuItem(&string("Large Icon"), CHANGE_LIST_VIEW_CONTROL_STYLE_TO_ICON);
	m_opPopupMenu -> AddItem(m_opMenuItem);
	m_opMenuItem = EMFactory::Instance() -> CreateMenuItem(&string("Small Icon"), CHANGE_LIST_VIEW_CONTROL_STYLE_TO_SMALL_ICON);
	m_opPopupMenu -> AddItem(m_opMenuItem);
	m_opMenuItem = EMFactory::Instance() -> CreateMenuItem(&string("List"), CHANGE_LIST_VIEW_CONTROL_STYLE_TO_LIST);
	m_opPopupMenu -> AddItem(m_opMenuItem);
}

EMWinListViewControl::~EMWinListViewControl()
{
/*	if(m_vEnableDragDrop)
	{
		if(FAILED(RevokeDragDrop(m_opWindowHandle)))
			EMDebugger("Faild to RevokeDragDrop!");
		delete m_opIDropTarget;
	}
*/

	//FIXME: Make sure that every allocated c-string in vector<LPLVITEM> m_oItemPointers is freed (LPLVITEM -> pszText)

/*	while(m_oItemPointers.size() != 0)
	{
		delete [] m_oItemPointers[m_oItemPointers.size() - 1] -> pszText;
		m_oItemPointers.pop_back();
	}
*/
	DeleteAllItems();
}

void EMWinListViewControl::AddChild(EMGUIComponent* p_opView)
{
//	AssertWindowLocked();
	if(dynamic_cast<EMWinView*>(p_opView))
		(dynamic_cast<EMWinView*>(p_opView)) -> SetWindowWindow(m_opWindowWindow);
	else if(dynamic_cast<EMWinGroupBox*>(p_opView))
		(dynamic_cast<EMWinGroupBox*>(p_opView)) -> SetWindowWindow(m_opWindowWindow);
//	else if(dynamic_cast<EMWinTabControl*>(p_opView))
//		(dynamic_cast<EMWinTabControl*>(p_opView)) -> SetWindowWindow(m_opWindowWindow);
	p_opView -> AddToComponent(this);
	m_oChildComponents.push_back(p_opView);

}

void EMWinListViewControl::AddColumn(char* p_opName, uint32 p_vWidth, EMAlignment p_eAlignment)
{
	static uint32 vOrder = 0;

	LVCOLUMN vColumnInfo;

	vColumnInfo.mask = LVCF_FMT | LVCF_ORDER | LVCF_TEXT | LVCF_WIDTH;
	
	if(p_eAlignment == EM_ALIGNMENT_LEFT)
		vColumnInfo.fmt = LVCFMT_LEFT;
	if(p_eAlignment == EM_ALIGNMENT_CENTER)
		vColumnInfo.fmt = LVCFMT_CENTER;
	if(p_eAlignment == EM_ALIGNMENT_RIGHT)
		vColumnInfo.fmt = LVCFMT_RIGHT;

	vColumnInfo.cx = p_vWidth;
	vColumnInfo.pszText = p_opName;
	vColumnInfo.cchTextMax = strlen(p_opName) + 1;
	vColumnInfo.iOrder = vOrder++;

	ListView_InsertColumn(m_opWindowHandle, vOrder, &vColumnInfo);
}

void EMWinListViewControl::AddItem(const char* p_opText)
{
	LVITEM vItem;
	LPLVITEM sListViewItem = EM_new LVITEM;

	vItem.mask				  = LVIF_TEXT | LVIF_PARAM ;
	vItem.pszText			  = const_cast<char*>(p_opText);
	vItem.iItem				  = m_vItemID;
	vItem.iSubItem			  = 0;

	sListViewItem -> mask	  = LVIF_TEXT | LVIF_PARAM ;
	sListViewItem -> pszText  = strdup(p_opText);
	sListViewItem -> iItem	  = m_vItemID++;
	sListViewItem -> iSubItem = 0;

	//memcpy(sListViewItem , &vItem, sizeof(vItem));
	pair<HWND, const char*>* opStoreValuesForListViewItemLParam;
	if(m_vUniqueIndexColumn == 0)
		opStoreValuesForListViewItemLParam = EM_new pair<HWND, const char*>(m_opWindowHandle, strdup(p_opText));
	else
		opStoreValuesForListViewItemLParam = EM_new pair<HWND, const char*>(m_opWindowHandle, NULL);
	sListViewItem -> lParam	  = reinterpret_cast<LPARAM>(opStoreValuesForListViewItemLParam);
	vItem.lParam              = reinterpret_cast<LPARAM>(sListViewItem);
	m_oItemPointers.push_back(sListViewItem);

	if(ListView_InsertItem(m_opWindowHandle, &vItem) == -1)
		eo << "Faild adding text item" << ef;
	else
		eo << "Succeeded adding text item" << ef;
//	ChangeIdInItemPointerVector();
}

void EMWinListViewControl::AddSubItem(const char* p_opText, int p_vColumn)
{
	LVITEM vItem;
	vItem.mask = LVIF_TEXT;
	vItem.pszText = const_cast<char*>(p_opText);
	vItem.iItem = m_vItemID - 1;
	vItem.iSubItem = p_vColumn;
	if(ListView_SetItem(m_opWindowHandle, &vItem) != 0)
	{
		eo << "SetItem SubItem: " << (char*) p_opText << " to ItemId: " << m_vItemID - 1 << " and Column: " << p_vColumn << " OK!" << ef;
		if(p_vColumn == m_vUniqueIndexColumn && m_vUniqueIndexColumn >= 0)
		{
			for(int vVectorCounter = 0; vVectorCounter < m_oItemPointers.size(); vVectorCounter++)
				if(m_oItemPointers[vVectorCounter] -> iItem == m_vItemID - 1)
					reinterpret_cast<pair<HWND, const char*>*>(m_oItemPointers[vVectorCounter] -> lParam) -> second = const_cast<const char*>(strdup(p_opText));
		}
	}
	else
		eo << "SetItem SubItem: " << (char*) p_opText << " to ItemId: " << m_vItemID - 1 << " and Column: " << p_vColumn << " !!!!FAILED!!!!" << ef;
}

void EMWinListViewControl::AddSubItem(const char* p_opText, int p_vItemID, int p_vColumn)
{
	LVITEM vItem;
	vItem.mask = LVIF_TEXT;
	vItem.pszText = const_cast<char*>(p_opText);
	vItem.iItem = p_vItemID;
	vItem.iSubItem = p_vColumn;
	if(ListView_SetItem(m_opWindowHandle, &vItem) != 0)
	{
		eo << "SetItem SubItem: " << (char*) p_opText << " to ItemId: " << m_vItemID - 1 << " and Column: " << p_vColumn << " OK!" << ef;
		if(p_vColumn == m_vUniqueIndexColumn && m_vUniqueIndexColumn >= 0)
		{
			for(int vVectorCounter = 0; vVectorCounter < m_oItemPointers.size(); vVectorCounter++)
				if(m_oItemPointers[vVectorCounter] -> iItem == m_vItemID - 1)
					reinterpret_cast<pair<HWND, const char*>*>(m_oItemPointers[vVectorCounter] -> lParam) -> second = const_cast<const char*>(strdup(p_opText));
		}
	}
	else
		eo << "SetItem SubItem: " << (char*) p_opText << " to ItemId: " << m_vItemID - 1 << " and Column: " << p_vColumn << " !!!!FAILED!!!!" << ef;
}

bool EMWinListViewControl::AddToComponent(EMGUIComponent* p_opComponent)
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

	INITCOMMONCONTROLSEX oInitCommonControlEX;
	oInitCommonControlEX.dwSize = sizeof(oInitCommonControlEX);
	oInitCommonControlEX.dwICC = ICC_TAB_CLASSES;

	if(InitCommonControlsEx(&oInitCommonControlEX))
	{
		m_opWindowHandle = CreateWindowEx(/*WS_EX_CLIENTEDGE*/ WS_EX_STATICEDGE, WC_LISTVIEW , "Test", m_vStyle, m_oFrame.m_vLeft,  
								m_oFrame.m_vTop, m_oFrame.m_vRight - m_oFrame.m_vLeft + 1, m_oFrame.m_vBottom - m_oFrame.m_vTop + 1,
								opParent, NULL, EMApplication::GetApplicationInstance(), NULL);
		SendMessage(m_opWindowHandle, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), true);
		SetWindowLong(m_opWindowHandle, GWL_USERDATA, reinterpret_cast<int32>(this));
		ListView_SetExtendedListViewStyle(m_opWindowHandle, m_vListView_EX_Style);
		if(m_vListView_EX_Style & LVS_EX_TRACKSELECT)
		{
			eo << "  * * * LVS_EX_TRACKSELECT style set * * * -> Hover time = 0ms" << ef;
			ListView_SetHoverTime(m_opWindowHandle, 1);
		}

		m_opDefaultWindowProcedure = reinterpret_cast<WNDPROC>(GetWindowLong(m_opWindowHandle, GWL_WNDPROC));
		SetWindowLong(m_opWindowHandle, GWL_WNDPROC, reinterpret_cast<LONG>(SubclassedWindowProcedure));
	}
	else
		eo << "Could not load CommonControl dll" << ef;

	if(m_opTreeViewControl != NULL)
		InitiateListViewImageLists();

/*	if(m_vEnableDragDrop)
	{
//		LPSHELLFOLDER opShellFolder;
//		LPITEMIDLIST opItemList;

//		SHGetDesktopFolder(&opShellFolder);
//		SHGetSpecialFolderLocation(m_opWindowHandle, CSIDL_DESKTOP, &opItemList);


		m_opIDropTarget = EM_new EMWinDropTarget(this, LIST_VIEW_CONTROL);
//		opShellFolder ->GetUIObjectOf(m_opWindowHandle, 1, (LPCITEMIDLIST*)&opItemList, IID_IDropTarget, 0, (LPVOID *)&m_opIDropTarget);
		RegisterDragDrop(m_opWindowHandle, m_opIDropTarget);
	}
*/
//	SetParent(m_opWindowHandle, opParent);
//	SetWindowPos(m_opWindowHandle, NULL, m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.GetWidth() + 1, m_oFrame.GetHeight() + 1, SWP_NOZORDER);	

	return true;
}

EMRect EMWinListViewControl::Bounds()
{
	return m_oBounds;
}


void EMWinListViewControl::ChangeIdInItemPointerVector()
{
	char vText[EM_MAX_STRING_BUFFER_SIZE];

	for(int vVectorCounter = 0; vVectorCounter < m_oItemPointers.size(); vVectorCounter++)
	{
		for(int vListViewCounter = 0; vListViewCounter < ListView_GetItemCount(m_opWindowHandle); vListViewCounter++)
		{
			ListView_GetItemText(m_opWindowHandle, vListViewCounter, m_vUniqueIndexColumn, vText, EM_MAX_STRING_BUFFER_SIZE);
			pair<HWND, const char*>* test = reinterpret_cast<pair<HWND, const char*>*>(m_oItemPointers[vVectorCounter] -> lParam);
			if(strcmp(reinterpret_cast<pair<HWND, const char*>*>(m_oItemPointers[vVectorCounter] -> lParam) -> second, vText) == 0)
				m_oItemPointers[vVectorCounter] -> iItem = vListViewCounter;
		}
	}
}

void EMWinListViewControl::ChangeSubItem(uint32 p_vItemID, uint16 p_vColumn, const char* p_vpChangeItTo)
{
	LVITEM vItem;
	vItem.mask = LVIF_TEXT;
	vItem.pszText = const_cast<char*>(p_vpChangeItTo);
	vItem.iItem = p_vItemID;
	vItem.iSubItem = p_vColumn;
	if(ListView_SetItem(m_opWindowHandle, &vItem) != 0)
		eo << "Change SubItem: " << (char*) p_vpChangeItTo << " to ItemId: " << p_vItemID << " and Column: " << p_vColumn << " OK!" << ef;
	else
		eo << "Change SubItem: " << (char*) p_vpChangeItTo << " to ItemId: " << m_vItemID - 1 << " and Column: " << p_vColumn << " !!!!FAILED!!!!" << ef;
//	ChangeIdInItemPointerVector();
}

//FIXME: Change this function to go on LVITEM::iItem instead of a string
void EMWinListViewControl::ChangeTextInItemPointerVector(const char* p_vLookAfterText, char* p_vChangeTextTo)
{
	for(int vCounter = 0; vCounter < m_oItemPointers.size(); vCounter++)
		if(strcmp(m_oItemPointers[vCounter] -> pszText, p_vLookAfterText) == 0)
		{
			eo << "m_oItemPointers[vCounter] -> pszText = " << m_oItemPointers[vCounter] -> pszText << ef;
			delete [] m_oItemPointers[vCounter] -> pszText;
			m_oItemPointers[vCounter] -> pszText = strdup(p_vChangeTextTo);
			//ChangeIdInItemPointerVector();
			return;
		}
	EMDebugger("Cound not find string to change!");
}

int CALLBACK EMWinListViewControl::CompareFunc(LPARAM p_vParameterOne, LPARAM p_vParameterTwo, LPARAM p_vParameterSort)
{
//eo << "In CompareFunc" << ef;

	if(p_vParameterSort != 0)
	{
		LPLVITEM spListItemOne = reinterpret_cast<LPLVITEM>(p_vParameterOne);
		LPLVITEM spListItemTwo = reinterpret_cast<LPLVITEM>(p_vParameterTwo);
		HWND opWindowHandle(reinterpret_cast<pair<HWND, const char*>*>(spListItemOne -> lParam) -> first);
		char vTextOne[EM_MAX_STRING_BUFFER_SIZE];
		char vTextTwo[EM_MAX_STRING_BUFFER_SIZE];

		uint16 vColumn;
		bool vSortAscending;
		if(p_vParameterSort - EM_SORT_DESCENDING < 0)
		{
			vColumn = p_vParameterSort - EM_SORT_ASCENDING;
			vSortAscending = true;
		}
		else
		{
			vColumn = p_vParameterSort - EM_SORT_DESCENDING;
			vSortAscending = false;
		}

		ListView_GetItemText(opWindowHandle, spListItemOne -> iItem, vColumn, vTextOne, EM_MAX_STRING_BUFFER_SIZE);
		ListView_GetItemText(opWindowHandle, spListItemTwo -> iItem, vColumn, vTextTwo, EM_MAX_STRING_BUFFER_SIZE);

	eo << "p_vParameterOne = " << spListItemOne -> pszText << " p_vParameterTwo = " << spListItemTwo -> pszText << ef;
	eo << "vTextOne = " << vTextOne << " vTextTwo = " << vTextTwo << ef;
	eo << "opListItemOne ID = " << spListItemOne -> iItem << " opListItemTwo ID = " << spListItemTwo -> iItem << ef;
	eo << "ColomnID = " << p_vParameterSort << ef;
		if(vSortAscending)
		{
			eo << "SortAscending: Compare Result -> " << stricmp(vTextOne, vTextTwo) << ef << ef;
			return CompareString(vTextOne, vTextTwo);
		}
		eo << "SortDecending: Compare Result -> " << stricmp(vTextOne, vTextTwo) << ef << ef;
		return CompareString(vTextTwo, vTextOne);
	}
	else
	{
		LPLVITEMDATA spListViewItemDataOne = (LPLVITEMDATA)p_vParameterOne;
		LPLVITEMDATA spListViewItemDataTwo = (LPLVITEMDATA)p_vParameterTwo;
		HRESULT   hr;

		hr = spListViewItemDataOne -> m_spParentShellFolder -> CompareIDs(0, spListViewItemDataOne -> m_spItemIdList, spListViewItemDataTwo -> m_spItemIdList);

		if (FAILED(hr))
			return 0;

		return (short)SCODE_CODE(GetScode(hr));
	}
}

int EMWinListViewControl::CompareString(const char* p_vpStringOne, const char* p_vpStringTwo)
{
	uint32 vSumOne(0);
	uint32 vSumTwo(0);
//	uint32 vNumberSumOne(0);
//	uint32 vNumberSumTwo(0);
	bool vFirst(true);
	
	for(uint32 vCounter = 0; p_vpStringOne[vCounter] || p_vpStringTwo[vCounter]; vCounter++)
	{
		if(p_vpStringOne[vCounter] >= 48 && p_vpStringOne[vCounter] <= 57 && p_vpStringTwo[vCounter] >= 48 && p_vpStringTwo[vCounter] <= 57)
		{
			if(vFirst != true)
			{
				vFirst = true;
				p_vpStringOne += vCounter;
				p_vpStringTwo += vCounter;
				vCounter = 0;
			}
			vSumOne = atoi(p_vpStringOne);
			vSumTwo = atoi(p_vpStringTwo);
			if(vSumOne == vSumTwo)
				while(p_vpStringOne[vCounter])
				{
					if(p_vpStringOne[vCounter] >= 48 && p_vpStringOne[vCounter] <= 57)
						vCounter++;
					else
						break;
				}
				if(!p_vpStringOne[vCounter])
					break;
/*			}
			else
			{
				vSumOne += p_vpStringOne[vCounter] - 48;
				vSumTwo += p_vpStringTwo[vCounter] - 48;
			}
*/
		}
		else if(vFirst && p_vpStringOne[vCounter] >= 48 && p_vpStringOne[vCounter] <= 57)
		{
			return -1;
		}
		else if(vFirst && p_vpStringTwo[vCounter] >= 48 && p_vpStringTwo[vCounter] <= 57)
		{
			return 1;
		}
		else
		{
			if(p_vpStringOne[vCounter] >= 97)
				vSumOne += p_vpStringOne[vCounter] - 32;
			else
				vSumOne += p_vpStringOne[vCounter];

			if(p_vpStringTwo[vCounter] >= 97)
				vSumTwo += p_vpStringTwo[vCounter] - 32;
			else
				vSumTwo += p_vpStringTwo[vCounter];
		}
eo << "vSumOne = " << vSumOne << ef;
eo << "vSumTwo = " << vSumTwo << ef;
		if(vSumOne > vSumTwo)
			return 1;
		else if(vSumOne < vSumTwo)
			return -1;
		vFirst = false;
	}
	return 0;
}

LPITEMIDLIST EMWinListViewControl::CopyItemIdList(LPMALLOC p_opMalloc, LPITEMIDLIST p_spItemIdList)
{
	LPITEMIDLIST spItemIdListTemp((LPITEMIDLIST)p_opMalloc -> Alloc(p_spItemIdList -> mkid.cb + sizeof(p_spItemIdList -> mkid.cb)));

	CopyMemory((PVOID)spItemIdListTemp, (CONST VOID *)p_spItemIdList, p_spItemIdList -> mkid.cb + sizeof(p_spItemIdList -> mkid.cb));

	return spItemIdListTemp;
}

LPITEMIDLIST EMWinListViewControl::CreateItemIdList(uint32 p_vSize)
{
	LPMALLOC opMalloc;
	HRESULT hr;
	LPITEMIDLIST spItemIdList = NULL;

	hr = SHGetMalloc(&opMalloc);

	if(FAILED(hr))
		return 0;

	spItemIdList = (LPITEMIDLIST)opMalloc -> Alloc(p_vSize);

	if(spItemIdList)
		_fmemset(spItemIdList, 0, p_vSize);		// zero-init for external task   alloc

	if(opMalloc)
		opMalloc -> Release();

	return spItemIdList;
}

void EMWinListViewControl::DeleteAllItems()
{
	if(!ListView_GetItemCount(m_opWindowHandle))
		return;
	if(m_opTreeViewControl == NULL)
		while(m_oItemPointers.size() != 0)
		{
			delete [] m_oItemPointers[m_oItemPointers.size() - 1] -> pszText;
			m_oItemPointers.pop_back();
			m_vItemID = 0;
		}
	else
	{
		int32 vListItemID(-1);
		LVITEM sListViewItem;
		LPLVITEMDATA sListViewItemData(NULL);
		sListViewItem.mask = LVIF_PARAM;
		sListViewItem.iSubItem = 0;
		LPMALLOC opMalloc;
		
		HRESULT hr = SHGetMalloc(&opMalloc);

		if(FAILED(hr))
			return;

		vListItemID = ListView_GetNextItem(m_opWindowHandle, vListItemID, LVNI_ALL);
		while(vListItemID != -1)
		{
			sListViewItem.iItem = vListItemID;
			ListView_GetItem(m_opWindowHandle, &sListViewItem);
			sListViewItemData = reinterpret_cast<LPLVITEMDATA>(sListViewItem.lParam);
			if(sListViewItemData == NULL)
				break;
			opMalloc -> Free(sListViewItemData -> m_spItemIdList);
			//opMalloc -> Free(sListViewItemData -> m_spParentShellFolder);
			opMalloc -> Free(sListViewItemData);
			sListViewItem.lParam = NULL;
			ListView_SetItem(m_opWindowHandle, &sListViewItem);
			vListItemID = ListView_GetNextItem(m_opWindowHandle, vListItemID, LVNI_ALL);
		}
		opMalloc -> Release();
	}
	ListView_DeleteAllItems(m_opWindowHandle);
}

bool EMWinListViewControl::DeleteItem(int p_vItemID)
{
	bool vResult(ListView_DeleteItem(m_opWindowHandle, p_vItemID) != 0);
	for(int vVectorCounter = 0; vVectorCounter < m_oItemPointers.size(); vVectorCounter++)
	{
		if(m_oItemPointers[vVectorCounter] -> iItem == p_vItemID)
		{
			delete [] m_oItemPointers[vVectorCounter] -> pszText;
			m_oItemPointers.erase(&m_oItemPointers[vVectorCounter]);
			m_vItemID--;
			break;
		}
	}
	ChangeIdInItemPointerVector();
	return vResult;
}

void EMWinListViewControl::DeleteSelectedItem()
{
	int vListViewItem(ListView_GetSelectionMark(m_opWindowHandle));
	if(vListViewItem != -1)
	{
		if(DeleteItem(vListViewItem))
			eo << "Item Deleted" << ef;
		else
			eo << "Cound not Delete item" << ef;
	}
	else
		eo << "Cound not Get Item to delete" << ef;
}

bool EMWinListViewControl::DoPopupMenu(LPSHELLFOLDER p_spShellFolder, LPITEMIDLIST p_spItemIdList, POINT &p_sPoint)
{
	LPCONTEXTMENU		opContextMenu;
	HRESULT				hr;
	char				vTemp[64];
	CMINVOKECOMMANDINFO	sInvokeCommandInfo;
	int					vCommandID;
	HMENU				vMenuHandle;
	bool				vSuccess = true;

	hr = p_spShellFolder -> GetUIObjectOf(m_opWindowHandle, 1, (const _ITEMIDLIST**)&p_spItemIdList, IID_IContextMenu, 0, (LPVOID *)&opContextMenu); //Second param: Number of objects to get attributes of

	if(SUCCEEDED(hr))
	{
		vMenuHandle = CreatePopupMenu();

		if(vMenuHandle)
		{
			hr = opContextMenu -> QueryContextMenu(vMenuHandle, 0, 1, 0x7fff, CMF_DEFAULTONLY /*CMF_EXPLORE*/ | CMF_CANRENAME);
			if(SUCCEEDED(hr))
			{
				vCommandID = TrackPopupMenu(vMenuHandle, TPM_LEFTALIGN | TPM_RETURNCMD | TPM_RIGHTBUTTON, p_sPoint.x, p_sPoint.y, 0, m_opWindowHandle, NULL);

				if(vCommandID)
				{
					sInvokeCommandInfo.cbSize			= sizeof(CMINVOKECOMMANDINFO);
					sInvokeCommandInfo.fMask			= 0;
					sInvokeCommandInfo.hwnd				= m_opWindowHandle;
					sInvokeCommandInfo.lpVerb			= MAKEINTRESOURCE(vCommandID-1);
					sInvokeCommandInfo.lpParameters		= NULL;
					sInvokeCommandInfo.lpDirectory		= NULL;
					sInvokeCommandInfo.nShow			= SW_SHOWNORMAL;
					sInvokeCommandInfo.dwHotKey			= 0;
					sInvokeCommandInfo.hIcon			= NULL;
					
					hr = opContextMenu -> InvokeCommand(&sInvokeCommandInfo);
					if(SUCCEEDED(hr))  
						;  //Do nothing
					else
					{
						MessageBox(m_opWindowHandle, "InvokeCommand failed!!", "Shell Extension Sample", MB_OK);
						wsprintf(vTemp, "hr=%lx", hr);
						MessageBox(m_opWindowHandle, vTemp, "Shell Extension Sample", MB_OK);
					}
				}
			}
			else
				vSuccess = false;
			DestroyMenu(vMenuHandle);
		}
		else
			vSuccess = false;
		opContextMenu -> Release();
	}
	else
	{
		MessageBox(m_opWindowHandle, "GetUIObjectOf failed!!", "Shell Extension Sample", MB_OK);
		wsprintf(vTemp, "hr=%lx", hr);
		MessageBox(m_opWindowHandle, vTemp, "Shell Extension Sample", MB_OK);

		vSuccess = false;
	}

	return vSuccess;
}

void EMWinListViewControl::Enable(bool p_vEnable)
{
	EnableWindow(m_opWindowHandle, p_vEnable);
}

void EMWinListViewControl::EnableHoverSelect(bool p_vEnable, int32 p_vHoverTime)
{
	if(p_vEnable)
	{
		ListView_SetExtendedListViewStyleEx(m_opWindowHandle, LVS_EX_TRACKSELECT, LVS_EX_TRACKSELECT);
		ListView_SetHoverTime(m_opWindowHandle, p_vHoverTime);
	}
	else
		ListView_SetExtendedListViewStyleEx(m_opWindowHandle, LVS_EX_TRACKSELECT, 0);
}

void EMWinListViewControl::ExecuteCommand()
{
	if(m_vCommand != 0)
		EMCommandRepository::Instance() -> ExecuteCommand(m_vCommand);
}

EMRect EMWinListViewControl::Frame() const
{
	return m_oFrame;
}

int EMWinListViewControl::GetIcon(LPITEMIDLIST p_spItemIdList, uint32 p_vFlags)
{
	SHFILEINFO sShellFileInfo;

	SHGetFileInfo((LPCSTR)p_spItemIdList, 0, &sShellFileInfo, sizeof(SHFILEINFO), p_vFlags);

	return sShellFileInfo.iIcon;
}

int32 EMWinListViewControl::GetItem(const char* p_vpUniqueStringIndex)
{
	int32 vItem(-1);
	bool vStringFound(false);
	char vTempUniqueStringIndex[EM_MAX_STRING_BUFFER_SIZE];

	if(m_vUniqueIndexColumn < 0)
		return m_vUniqueIndexColumn;

	while(!vStringFound)
	{
		vItem = ListView_GetNextItem(m_opWindowHandle, vItem, 0);
		if(vItem == -1)
			break;

		ListView_GetItemText(m_opWindowHandle, vItem, m_vUniqueIndexColumn, vTempUniqueStringIndex, EM_MAX_STRING_BUFFER_SIZE);
		if(strcmp(p_vpUniqueStringIndex, vTempUniqueStringIndex) == 0)
			vStringFound = true;
	};
	return vItem;	
}

int EMWinListViewControl::GetItemCount()
{
	return ListView_GetItemCount(m_opWindowHandle);
}

bool EMWinListViewControl::GetName(LPSHELLFOLDER p_spShellFolder, LPITEMIDLIST p_spItemIdList, uint32 p_vFlags, char* p_vpFriendlyName)
{
//	BOOL   bSuccess=TRUE;
	STRRET sStringReturn;
	LPMALLOC opMalloc(NULL);

	HRESULT hr = SHGetMalloc(&opMalloc);
	if (FAILED(hr))
	{
	//	EMDebugger("Faild to GetMalloc for Shell");
		return false;
	}

	if(NOERROR == p_spShellFolder -> GetDisplayNameOf(p_spItemIdList, p_vFlags, &sStringReturn))
	{
		switch(sStringReturn.uType)
		{
		case STRRET_WSTR:
			WideCharToMultiByte(CP_ACP,					// CodePage
								0,						// dwFlags
								sStringReturn.pOleStr,			// lpWideCharStr
								-1,						// cchWideChar
								p_vpFriendlyName,			// lpMultiByteStr
								255,//sizeof(lpFriendlyName), // cchMultiByte,
								NULL,					// lpDefaultChar,
								NULL);					// lpUsedDefaultChar
			opMalloc -> Free(sStringReturn.pOleStr);
			break;

		case STRRET_OFFSET:
			lstrcpy(p_vpFriendlyName, (LPSTR)p_spItemIdList + sStringReturn.uOffset);
			break;

		case STRRET_CSTR:
			lstrcpy(p_vpFriendlyName, (LPSTR)sStringReturn.cStr);
			break;

		default:
			return false;
		}
	}
	else
		return false;

	return true;
}

void* EMWinListViewControl::GetNativeView() const
{
	return m_opWindowHandle;
}

LPITEMIDLIST EMWinListViewControl::GetNext(const LPITEMIDLIST p_spItemIdList)
{
	char* vpMem = (LPSTR)p_spItemIdList;

	vpMem += p_spItemIdList -> mkid.cb;

	return (LPITEMIDLIST)vpMem;
}

uint32 EMWinListViewControl::GetResizingMode()
{
	return m_vResizingMode;
}

string EMWinListViewControl::GetSelection(bool p_vFullPath)
{
	char vItemText[EM_MAX_STRING_BUFFER_SIZE * 100];
	string oItemText;
	char vItemText2[EM_MAX_STRING_BUFFER_SIZE * 100];

	LPLVITEMDATA spListViewItemData;
	LVITEM sListViewItem;

	if(p_vFullPath)
	{
		sListViewItem.mask = LVIF_PARAM;
		sListViewItem.iItem = ListView_GetSelectionMark(m_opWindowHandle);
		sListViewItem.iSubItem = 0;

		if(!GetSelection(&sListViewItem))
			return "";
		spListViewItemData = (LPLVITEMDATA)sListViewItem.lParam;
		oItemText = spListViewItemData -> m_vPath;
		SHGetPathFromIDList(spListViewItemData -> m_spItemIdList, vItemText2);
		string oFileName(vItemText2);
		if(oItemText[1] == ':' && oItemText.length() < 4)
			oFileName.erase(0, oFileName.find_last_of('\\') + 1);
		else
			oFileName.erase(0, oFileName.find_last_of('\\'));
		oItemText.append(oFileName.c_str());
	}
	else
	{
		ListView_GetItemText(m_opWindowHandle, ListView_GetSelectionMark(m_opWindowHandle), 0, vItemText, EM_MAX_STRING_BUFFER_SIZE);
		oItemText = vItemText;
	}
	return m_oSelection = oItemText.c_str();
}

bool EMWinListViewControl::GetSelection(LVITEM* p_spListViewItem)
{
	return ListView_GetItem(m_opWindowHandle, p_spListViewItem) != 0;
}

uint32 EMWinListViewControl::GetSelectionID()
{
	return ListView_GetSelectionMark(m_opWindowHandle);
}

//If p_vColumn == -1 UniqueIndexColumn is used
char* EMWinListViewControl::GetSelectionSubItem(int16 p_vColumn)
{
/*	char vTemp[255];
	LVITEM sListViewItem;
	sListViewItem.mask = LVIF_TEXT;
	sListViewItem.iItem = ListView_GetSelectionMark(m_opWindowHandle);
	sListViewItem.iSubItem = p_vColumn;
	sListViewItem.pszText = vTemp;
	sListViewItem.cchTextMax = 255;

	if(!GetSelection(&sListViewItem))
		return "";
	return sListViewItem.pszText;
*/
	if(m_vUniqueIndexColumn < 0)
		return NULL;
	if(p_vColumn == -1)  // ListView_GetItemtText is a Macro so {} is needed
	{
		ListView_GetItemText(m_opWindowHandle, ListView_GetSelectionMark(m_opWindowHandle), m_vUniqueIndexColumn, m_vpSelection, EM_MAX_STRING_BUFFER_SIZE);
	}
	else
	{
		ListView_GetItemText(m_opWindowHandle, ListView_GetSelectionMark(m_opWindowHandle), p_vColumn, m_vpSelection, EM_MAX_STRING_BUFFER_SIZE);
	}
	return m_vpSelection/*sListViewItem.pszText*/;
}

uint32 EMWinListViewControl::GetSize(const LPITEMIDLIST p_spItemIdList)
{
	uint32 vTotalSize = 0;
	LPITEMIDLIST spItemIdListTemp;

	if(p_spItemIdList)
	{
		vTotalSize += sizeof(p_spItemIdList -> mkid.cb);		// Null terminator
		if(p_spItemIdList -> mkid.cb)
		{
			vTotalSize += p_spItemIdList -> mkid.cb;
			spItemIdListTemp = GetNext(p_spItemIdList);
			while(spItemIdListTemp -> mkid.cb)
			{
				vTotalSize += spItemIdListTemp -> mkid.cb;
				spItemIdListTemp = GetNext(spItemIdListTemp);
			}
		}
	}
	return vTotalSize;
}

char* EMWinListViewControl::GetSubItem(uint32 p_vItem, int16 p_vColumn)
{
/*	char vTemp[255];
	LVITEM sListViewItem;
	sListViewItem.mask = LVIF_TEXT;
	sListViewItem.iItem = p_vItem;
	sListViewItem.iSubItem = p_vColumn;
	sListViewItem.pszText = vTemp;
	sListViewItem.cchTextMax = 255;

	if(!GetSelection(&sListViewItem))
		return "";
	return sListViewItem.pszText;
*/
	if(m_vUniqueIndexColumn < 0)
		return NULL;
	if(p_vColumn == -1)  // ListView_GetItemtText is a Macro so {} is needed
	{
		ListView_GetItemText(m_opWindowHandle, p_vItem, m_vUniqueIndexColumn, m_vpSelection, EM_MAX_STRING_BUFFER_SIZE);
	}
	else
	{
		ListView_GetItemText(m_opWindowHandle, p_vItem, p_vColumn, m_vpSelection, EM_MAX_STRING_BUFFER_SIZE);
	}
	return m_vpSelection/*sListViewItem.pszText*/;

}

EMTreeViewControl* EMWinListViewControl::GetTreeView()
{
	return m_opTreeViewControl;
}

EMWinWindow* EMWinListViewControl::GetWindowWindow()
{
	return m_opWindowWindow;
}

// Generic window procedure passed to WIN32 API
LRESULT CALLBACK EMWinListViewControl::GlobalWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo)
{
//eo << "EMWinListViewControl::GlobalWindowProcedure" << ef;
	EMWinListViewControl* opListViewControl;
	opListViewControl = reinterpret_cast<EMWinListViewControl*>(GetWindowLong(p_vWindowHandle, GWL_USERDATA));
	static string oTextBeforeChange;
	static uint16 vLastPressedColumn = -1; // Set -1 to get highest value possible

/*
eo << "LVN_BEGINDRAG " << LVN_BEGINDRAG << ef;
eo << "LVN_BEGINLABELEDIT " << LVN_BEGINLABELEDIT << ef;
eo << "LVN_BEGINRDRAG " << LVN_BEGINRDRAG << ef;
eo << "LVN_COLUMNCLICK " << LVN_COLUMNCLICK << ef;
eo << "LVN_DELETEALLITEMS " << LVN_DELETEALLITEMS << ef;
eo << "LVN_DELETEITEM " << LVN_DELETEITEM << ef; 
eo << "LVN_ENDLABELEDIT " << LVN_ENDLABELEDIT << ef; 
eo << "LVN_GETDISPINFO " << LVN_GETDISPINFO << ef; 
eo << "LVN_GETINFOTIP " << LVN_GETINFOTIP << ef; 
eo << "LVN_HOTTRACK " << LVN_HOTTRACK << ef; 
eo << "LVN_INSERTITEM " << LVN_INSERTITEM << ef; 
eo << "LVN_ITEMACTIVATE " << LVN_ITEMACTIVATE << ef; 
eo << "LVN_ITEMCHANGED " << LVN_ITEMCHANGED << ef; 
eo << "LVN_ITEMCHANGING " << LVN_ITEMCHANGING << ef;
eo << "LVN_KEYDOWN " << LVN_KEYDOWN << ef;
eo << "LVN_MARQUEEBEGIN " << LVN_MARQUEEBEGIN << ef; 
eo << "LVN_ODCACHEHINT " << LVN_ODCACHEHINT << ef; 
eo << "LVN_ODFINDITEM " << LVN_ODFINDITEM << ef;
eo << "LVN_ODSTATECHANGED " << LVN_ODSTATECHANGED << ef; 
eo << "LVN_SETDISPINFO " << LVN_SETDISPINFO << ef; 
eo << "NM_CLICK " << NM_CLICK << ef;  
eo << "NM_CUSTOMDRAW " << NM_CUSTOMDRAW << ef;
eo << "NM_DBLCLK " << NM_DBLCLK << ef;
eo << "NM_HOVER " << NM_HOVER << ef;
eo << "NM_KILLFOCUS " << NM_KILLFOCUS << ef;
eo << "NM_RCLICK " << NM_RCLICK << ef;
eo << "NM_RDBLCLK " << NM_RDBLCLK << ef;
eo << "NM_RELEASEDCAPTURE " << NM_RELEASEDCAPTURE << ef;
eo << "NM_RETURN " << NM_RETURN << ef;
eo << "NM_SETFOCUS " << NM_SETFOCUS << ef;
eo << "HDN_FIRST " << HDN_FIRST << ef;
eo << "HDN_LASR " << HDN_LAST << ef;
eo << "HDN_DIVIDERDBLCLICKA " << HDN_DIVIDERDBLCLICKA << ef;
*/
	LVHITTESTINFO sListViewHitTestInfo;
   LV_DISPINFO    *plvdi;
   LPTVITEMDATA   lptvid;  //Long pointer to TreeView item data
   LPLVITEMDATA   lplvid;  //Long pointer to ListView item data
	NMLISTVIEW *spNMListView = (NMLISTVIEW *)p_vParameterTwo;
	HRESULT hr;
	LPSHELLFOLDER spShellFolder = NULL;
	POINT sPoint;
	LVITEM sListViewItem;
//	LPDATAOBJECT   opDataObject;
//	DWORD          dwEffect;

//	TV_ITEM sTreeViewItem;
//	char vItemText[255];
//	NMTREEVIEW sNMTreeView;

	static vColumnCounter = 0;
	if(opListViewControl)
	{
		uint32 vMessage(0);
//eo << "HIWORD(p_vParameterOne) = " << HIWORD(p_vParameterOne) << ef;
		switch(HIWORD(p_vParameterOne))
		{
		case BN_CLICKED:
//eo << "BN_CLICKED " << BN_CLICKED << ef;
			break;
		default:
			eo << "Default" << ef;
			vMessage = 0;
		};

		if(p_vMessage == WM_MOUSEMOVE)
			SendMessage(GetParent(p_vWindowHandle), WM_MOUSEMOVE, p_vParameterOne, p_vParameterTwo);

//eo << "reinterpret_cast<LPNMHDR>(p_vParameterTwo) -> code = " << reinterpret_cast<LPNMHDR>(p_vParameterTwo) -> code << ef;
		switch(reinterpret_cast<LPNMHDR>(p_vParameterTwo) -> code)
		{
		case NM_CLICK:
			{
			eo << "NM_CLICK" << ef;
			vMessage = opListViewControl -> m_vMessage;
			UINT test;
			ListView_GetNumberOfWorkAreas(opListViewControl -> m_opWindowHandle, &test);
			eo << "ListView_GetNumberOfWorkAreas = " << test << ef;
			break;
			}
		case NM_DBLCLK:
			{
				if(opListViewControl -> __m_vNoUpdate)
					break;
				eo << "NM_DBLCLK, ListView" << ef;
				if(opListViewControl -> GetTreeView() == NULL)
					break;

				SHFILEINFO sFileInfo;
				const char* vpPath = opListViewControl ->GetSelection(true).c_str();

				SHGetFileInfo(vpPath, 0, &sFileInfo, sizeof(sFileInfo), SHGFI_ATTRIBUTES);
				
				if(sFileInfo.dwAttributes & SFGAO_FILESYSTEM)
				{
					if(!(sFileInfo.dwAttributes & SFGAO_FOLDER))
					{
						eo << "Testing DoubleClick on a file" << ef;
						SHELLEXECUTEINFO sExecuteInfo;
						sExecuteInfo.cbSize = sizeof(sExecuteInfo);
						sExecuteInfo.fMask = 0;
						sExecuteInfo.hwnd = NULL;//static_cast<HWND>(opListViewControl -> GetNativeView());
						sExecuteInfo.lpVerb = "open";
						sExecuteInfo.lpFile = vpPath;
						sExecuteInfo.lpParameters = "";
						sExecuteInfo.lpDirectory = "";
						sExecuteInfo.nShow = SW_SHOWNORMAL;
						
						ShellExecuteEx(&sExecuteInfo);
				//		break;
					}
					else if(sFileInfo.dwAttributes & SFGAO_FOLDER)
					{

						HTREEITEM vTreeItemHandle = TreeView_GetSelection(static_cast<HWND>(opListViewControl -> GetTreeView() -> GetNativeView()));
						TVITEM sTreeViewItem;

						TreeView_Expand(static_cast<HWND>(opListViewControl -> GetTreeView() ->GetNativeView()), vTreeItemHandle, TVE_EXPAND);

						vTreeItemHandle = TreeView_GetChild(static_cast<HWND>(opListViewControl ->GetTreeView() -> GetNativeView()), vTreeItemHandle);

						while(vTreeItemHandle)
						{
							char oTemp[255];
							sTreeViewItem.mask = TVIF_HANDLE | TVIF_TEXT;
							sTreeViewItem.hItem = vTreeItemHandle;
							sTreeViewItem.pszText = oTemp;
							sTreeViewItem.cchTextMax = 255;

							TreeView_GetItem(static_cast<HWND>(opListViewControl -> GetTreeView() -> GetNativeView()), &sTreeViewItem);

							if(strcmp(sTreeViewItem.pszText, opListViewControl -> GetSelection(false).c_str()) == 0)
								break;
							vTreeItemHandle = TreeView_GetNextSibling(static_cast<HWND>(opListViewControl -> GetTreeView() -> GetNativeView()), vTreeItemHandle);
						}
						if(vTreeItemHandle)
						{
							TreeView_Expand(static_cast<HWND>(opListViewControl -> GetTreeView() -> GetNativeView()), vTreeItemHandle, TVE_EXPAND);
							TreeView_Select(static_cast<HWND>(opListViewControl -> GetTreeView() -> GetNativeView()), vTreeItemHandle, TVGN_CARET);
						}
						else
						{
//							EMMessageBox::Instance() ->DisplayDialog(opListViewControl -> m_opWindowWindow  -> GetNativeView(), "Can't handle this type of file with doubleclick.\nTry use right mouse-buttin instead.", "Can't handle file type", EM_DIALOG_ICON_ERROR | EM_DIALOG_BUTTON_OK);
						}
					}
				break;
				}
			}
		case NM_RCLICK:

			//TODO: Add a popup meny to change displaymode. I.e. ICON, SMALLICON, LIST, REPORT
			if(opListViewControl -> __m_vNoUpdate)
				break;

			eo << "NM_RCLICK" << ef;
			if(opListViewControl -> GetTreeView() == NULL)
				break;
			opListViewControl -> HitTest(&sListViewHitTestInfo, sPoint);
			if (sListViewHitTestInfo.flags & LVHT_ONITEM)
			{
				sListViewItem.mask = LVIF_PARAM;
				sListViewItem.iItem = sListViewHitTestInfo.iItem;
				sListViewItem.iSubItem = 0;

				if(!opListViewControl -> GetSelection(&sListViewItem))
					break;
				lplvid=(LPLVITEMDATA)sListViewItem.lParam;
				ClientToScreen(static_cast<HWND>(opListViewControl -> GetNativeView()), &sPoint);
				opListViewControl -> DoPopupMenu(lplvid->m_spParentShellFolder, lplvid->m_spItemIdList, sPoint);
			}
			else
			{
				POINT sPoint;
				GetCursorPos(&sPoint);
				opListViewControl -> m_opPopupMenu -> PopUp(sPoint.x, sPoint.y, NULL);
			}
			break;
		case LVN_BEGINLABELEDIT:
			if(opListViewControl -> __m_vNoUpdate)
				break;
			eo << "LVN_BEGINLABELEDIT" << ef;
			vMessage = EM_LIST_VIEW_ITEM_LABEL_MAY_CHANGE;
			oTextBeforeChange = opListViewControl -> GetSelection(false);
			break;
		case LVN_ENDLABELEDIT:
			{
				if(opListViewControl -> __m_vNoUpdate)
					break;
				eo << "LVN_ENDLABELEDIT" << ef;
				LPLVITEM opTempItem(&reinterpret_cast<NMLVDISPINFO*>(p_vParameterTwo) -> item);
				HWND opWindowHandle(static_cast<HWND>(opListViewControl -> GetNativeView()));
				if(opTempItem -> pszText != NULL)
				{
					vMessage = EM_LIST_VIEW_ITEM_LABEL_CHANGED; // Only send message if there is something to send. If we are here there is something to send.
					ListView_SetItemText(opWindowHandle, ListView_GetSelectionMark(opWindowHandle), 0, opTempItem -> pszText);
					opListViewControl -> ChangeTextInItemPointerVector(oTextBeforeChange.c_str(), opTempItem -> pszText);
				}
				break;
			}
		case LVN_COLUMNCLICK:
			{
				if(opListViewControl -> __m_vNoUpdate)
					break;
				if(opListViewControl -> m_vUniqueIndexColumn < 0) // if true -> Sort disabled
					break;
				uint16 opColumnID(reinterpret_cast<LPNMLISTVIEW>(p_vParameterTwo) -> iSubItem);
				eo << "LVN_COLUMNCLICK" << ef;
				eo << "ID = " << opColumnID << ef;
				if(vLastPressedColumn != opColumnID)
				{
					if(ListView_SortItems(static_cast<HWND>(opListViewControl -> GetNativeView()), EMWinListViewControl::CompareFunc, EM_SORT_ASCENDING | opColumnID) == 0)
						eo << "Faild to sort ListView!" << ef;
					vLastPressedColumn = opColumnID;
				}
				else
				{
					if(ListView_SortItems(static_cast<HWND>(opListViewControl -> GetNativeView()), EMWinListViewControl::CompareFunc, EM_SORT_DESCENDING | opColumnID) == 0)
						eo << "Faild to sort ListView!" << ef;
					vLastPressedColumn = -1;
				}
				if(opListViewControl -> GetTreeView() == NULL)
					opListViewControl -> ChangeIdInItemPointerVector();
				//m_vCompareItemCounter = 0;
				break;
			}
		case LVN_GETDISPINFO:
		//	eo << "LVN_GETDISPINFO" << ef;
			if(opListViewControl -> __m_vNoUpdate)
				break;
			if(opListViewControl -> GetTreeView() == NULL)
				break;
			plvdi  = (LV_DISPINFO *)p_vParameterTwo;
			lplvid = (LPLVITEMDATA)plvdi->item.lParam;

			if(plvdi->item.mask & LVIF_IMAGE)
			{
				TVITEM tvi;
				LPITEMIDLIST spTempItemIDList;
				HTREEITEM hti;
				LPMALLOC		opMalloc(NULL);

				hr = SHGetMalloc(&opMalloc);
				if(FAILED(hr))
					break;

				hti = TreeView_GetSelection(reinterpret_cast<HWND>(opListViewControl -> GetTreeView() -> GetNativeView()));
				tvi.mask = TVIF_PARAM;
				tvi.hItem = hti;

				TreeView_GetItem(reinterpret_cast<HWND>(opListViewControl -> GetTreeView() ->GetNativeView()), &tvi);

				lptvid=(LPTVITEMDATA)tvi.lParam;

				spTempItemIDList = opListViewControl -> MergeItemIdLists(lptvid -> m_spItemIdListSecond, lplvid -> m_spItemIdList);

				plvdi -> item.iImage = opListViewControl -> GetIcon(spTempItemIDList, SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

				opMalloc -> Free(spTempItemIDList);
				opMalloc -> Release();

				if(lplvid -> m_vAttributes & SFGAO_LINK)
				{
					plvdi -> item.mask |= LVIF_STATE;
					plvdi -> item.stateMask = LVIS_OVERLAYMASK;
					plvdi -> item.state = INDEXTOOVERLAYMASK(2);
				}
			}

			if(plvdi -> item.mask & LVIF_TEXT)
			{
				opListViewControl -> GetName(lplvid -> m_spParentShellFolder, lplvid -> m_spItemIdList, SHGDN_NORMAL, plvdi -> item.pszText);

		//		eo << "plvdi->item.pszText = " << plvdi->item.pszText << ef;
			}
			break;

		case LVN_BEGINDRAG:
			{
				if(opListViewControl -> __m_vNoUpdate)
					break;
				vMessage = EM_BEGIN_DRAG;
				eo << "LVN_BEGINDRAG" << ef;
				opListViewControl -> SetFocus();
				if(opListViewControl -> GetTreeView() != NULL)
				{
				/*	LPMALLOC		opMalloc(NULL);
					sListViewItem.mask = LVIF_PARAM;
					sListViewItem.iItem = spNMListView->iItem;
					sListViewItem.iSubItem = 0;

					hr = SHGetMalloc(&opMalloc);
					if(FAILED(hr))
						return 0;

					if(!opListViewControl -> GetSelection(&sListViewItem))
						return 0;
					lplvid=(LPLVITEMDATA)sListViewItem.lParam;

					opDataObject = (LPDATAOBJECT)opMalloc -> Alloc(sizeof(LPDATAOBJECT));

					hr = lplvid -> m_spParentShellFolder -> GetUIObjectOf(static_cast<HWND>(opListViewControl -> GetNativeView()), 1, (const struct _ITEMIDLIST **)&lplvid -> m_spItemIdList, IID_IDataObject, 0, (LPVOID *)&opDataObject);  //Second Parameter: Number of objects to get attributes of

					if(SUCCEEDED(hr))
					{
						eo << "SUCCEEDED" << ef;
						LPDROPSOURCE opDropSource(new EMWinDropSource());

						FORMATETC sFormat;
						sFormat.cfFormat = CF_HDROP + 5000;
						sFormat.tymed = TYMED_FILE;
						sFormat.lindex = -1;
						sFormat.dwAspect = DVASPECT_CONTENT;
						sFormat.ptd = NULL;
						
						STGMEDIUM sMedium;
						string oTest;
						oTest.append(opListViewControl -> GetSelection(true));
		eo << "EMWinListViewControl::GlobalWindowProcedure; oTest = " << oTest << ef;
						
						sMedium.tymed = TYMED_FILE;
						sMedium.lpszFileName = (unsigned short*)oTest.c_str();
						sMedium.pUnkForRelease = NULL;

						opDataObject -> SetData(&sFormat, &sMedium, TRUE);
		eo << "EMWinListViewControl::GlobalWindowProcedure; sMedium.lpszFileName = " << (char*)sMedium.lpszFileName << ef;
			
						if(opDropSource)
						{
							uint32 test = DoDragDrop(opDataObject, opDropSource, DROPEFFECT_COPY | DROPEFFECT_MOVE, &dwEffect);
							if(test == DRAGDROP_S_DROP)
								eo << "DRAGDROP_S_DROP" << ef;
							else if(test == DRAGDROP_S_CANCEL)
								eo << "DRAGDROP_S_CANCEL" << ef;
							else if(test == E_OUTOFMEMORY)
								eo << "E_OUTOFMEMORY" << ef;
							else 
								eo << "E_UNSPEC" << ef;
						}
						opDataObject -> Release();
						opDropSource -> Release();
					}
					*/
/*					if(opListViewControl -> m_opDragDrop != NULL)
						delete opListViewControl -> m_opDragDrop;
					opListViewControl -> m_opDragDrop = EM_new EMWinDragDrop;
					if(opListViewControl -> m_opDragDropData != NULL)
						delete opListViewControl -> m_opDragDropData;
					opListViewControl -> m_opDragDropData = EM_new EMDragDropData;
					opListViewControl -> m_opDragDropData -> m_opString = "Test";
					opListViewControl -> m_opDragDrop -> DoDragDrop(opListViewControl, opListViewControl, opListViewControl -> m_opDragDropData, EM_LIST_VIEW_CONTROL, EM_LIST_VIEW_CONTROL);
*/				}
				break;
			}
		case TVN_ITEMEXPANDING:
			eo << " *** At the wrong place!!! Can't do TVN_ITEMEXPANDING here!!!! ***" << ef;
			break;
		case LVN_KEYDOWN:
			{
				if(opListViewControl -> __m_vNoUpdate)
					break;
				eo << "LVN_KEYDOWN" << ef;
				LPNMLVKEYDOWN sKey(reinterpret_cast<LPNMLVKEYDOWN>(p_vParameterTwo));
				eo << "KeyPressed = " << sKey -> wVKey << ef;
				eo << "KeyPressed flag = " << sKey -> flags << ef;
				if(sKey -> wVKey == 46 && sKey -> flags == 339)
				{
					eo << "Delete from ListView Control" << ef;
					if(opListViewControl -> Notify(EM_LIST_VIEW_ITEM_DELETED))
						opListViewControl -> DeleteSelectedItem();
				}
	/*			if(sKey -> wVKey == 27 && sKey -> flags == 1)
				{
					eo << "Delete All from Media Pool" << ef;
					opListViewControl -> DeleteAllItems();
				}
*/
				break;
			}
		default:
			vMessage = 0;
		};
		if(vMessage != 0)
			if(opListViewControl -> Notify(vMessage))
				return 0;
	}
//eo << "EMWinListViewControl is Calling DefWindowProc()" << ef;
	return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
}

void EMWinListViewControl::Hide()
{
	ShowWindow(m_opWindowHandle, SW_HIDE);
}

int EMWinListViewControl::HitTest(LVHITTESTINFO* p_spListViewHitTestInfo, POINT &p_sPoint)
{
	//POINT sPoint;

	GetCursorPos((LPPOINT)&p_sPoint);
	ScreenToClient(m_opWindowHandle, &p_sPoint);
	p_spListViewHitTestInfo -> pt = p_sPoint;
	return ListView_HitTest(m_opWindowHandle, p_spListViewHitTestInfo);
}


bool WINAPI EMWinListViewControl::InitiateListViewImageLists()
{
	HIMAGELIST vImageListSmallHandle;
	HIMAGELIST hImageListLargeHandle;
	SHFILEINFO sShellFileInfo;

	vImageListSmallHandle = (HIMAGELIST)SHGetFileInfo((LPCSTR)"C:\\", 0, &sShellFileInfo, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

	hImageListLargeHandle = (HIMAGELIST)SHGetFileInfo((LPCSTR)"C:\\", 0, &sShellFileInfo, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_LARGEICON);

	if (vImageListSmallHandle && hImageListLargeHandle)
	{
		ListView_SetImageList(m_opWindowHandle, vImageListSmallHandle, LVSIL_SMALL);
		ListView_SetImageList(m_opWindowHandle, hImageListLargeHandle, LVSIL_NORMAL);
	}
	else
		return false;

	return true;
}

BOOL WINAPI EMWinListViewControl::InitiateListViewItems(LPTVITEMDATA p_spTreeViewItemData, LPSHELLFOLDER p_spShellFolder)
{
	LVITEM			sListViewItem;
	int				vListViewItemCounter;
	HRESULT			hr;
	LPMALLOC		opMalloc(NULL);
//	LPITEMIDLIST	spThisItemIdList(NULL);
	LPITEMIDLIST	spItemIdList(NULL);
	LPENUMIDLIST	opEnumIdList(NULL);
	LPLVITEMDATA	spListViewItemData(NULL);
	uint32			vFetched;
	uint32			vAttributes;
	HWND			opParentWindowHandle = GetParent(m_opWindowHandle);
	uint32			vFlags;

char vItemText[EM_MAX_STRING_BUFFER_SIZE];
//	char szBuff[255];

	sListViewItem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;

	hr = SHGetMalloc(&opMalloc);
	if(FAILED(hr))
		return FALSE;

	// Hourglass on!
eo << "Hourglass on" << ef;

	SetCapture(opParentWindowHandle);
	SetCursor(LoadCursor(NULL, IDC_WAIT));

	if(SUCCEEDED(hr))
	{
		hr = p_spShellFolder -> EnumObjects(opParentWindowHandle, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &opEnumIdList);

		if(SUCCEEDED(hr))
		{
			vListViewItemCounter = 0;

			while (S_OK == opEnumIdList->Next(1, &spItemIdList, &vFetched))
			{
				//OK, let's get some memory for our ITEMDATA struct

				spListViewItemData = (LPLVITEMDATA)opMalloc -> Alloc(sizeof(LVITEMDATA));
				if(!spListViewItemData)
					break;//goto Done;

//Don't do this here, since we are now using the LPSTR_TEXTCALLBACK method...
//
//				//Now get the friendly name that we'll put in the treeview...
//
//				GetName(lpsf, lpi, SHGDN_NORMAL, szBuff);

				//Note that since we are interested in the display attributes as well as
				//the other attributes, we need to set ulAttrs to SFGAO_DISPLAYATTRMASK
				//before calling GetAttributesOf();

				vAttributes = SFGAO_DISPLAYATTRMASK;

				p_spShellFolder -> GetAttributesOf(1, (const _ITEMIDLIST**)&spItemIdList, &vAttributes);
				spListViewItemData -> m_vAttributes = vAttributes;

//				SHGetPathFromIDList(p_spTreeViewItemData -> m_spItemIdListSecond, vItemText);
//				eo << vItemText << ef;

				//spThisItemIdList = MergeItemIdLists(p_spTreeViewItemData -> m_spItemIdListSecond, spItemIdList);

				sListViewItem.iItem			= vListViewItemCounter++;
				sListViewItem.iSubItem		= 0;
//				sListViewItem.pszText		= szBuff;
				sListViewItem.pszText		= LPSTR_TEXTCALLBACK;
				sListViewItem.cchTextMax	= MAX_PATH;
				vFlags						= SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON;

//Don't do this here, since we are now using the I_IMAGECALLBACK method...
//
//				sListViewItem.iImage		= GetIcon(lpifqThisItem, uFlags);
				sListViewItem.iImage		= I_IMAGECALLBACK;

				spListViewItemData -> m_spParentShellFolder = p_spShellFolder;
				p_spShellFolder -> AddRef();

				//Now, make a copy of the ITEMIDLIST
				spListViewItemData -> m_spItemIdList = CopyItemIdList(opMalloc, spItemIdList);
				SHGetPathFromIDList(p_spTreeViewItemData -> m_spItemIdListSecond, vItemText);
				strcpy(spListViewItemData -> m_vPath, vItemText);

//				SHGetPathFromIDList(spItemIdList, vItemText);
//				eo << vItemText << ef;


				sListViewItem.lParam = (LPARAM)spListViewItemData;

				// Add the item to the listview

				if(ListView_InsertItem(m_opWindowHandle, &sListViewItem) == -1)
					return FALSE;

				//opMalloc -> Free(spThisItemIdList);
				//spThisItemIdList = NULL;
				opMalloc -> Free(spItemIdList);		//Finally, free the pidl that the shell gave us...
				spItemIdList = NULL;
			}
		}
	}

//Done:

	// Hourglass off!
eo << "Hourglass on" << ef;

	ReleaseCapture();
	SetCursor(LoadCursor(NULL, IDC_ARROW));

	if(opEnumIdList)
		opEnumIdList -> Release();

	//The following 2 if statements will only be TRUE if we got here on an
	//error condition from the "goto" statement.  Otherwise, we free this memory
	//at the end of the while loop above.
	if(spItemIdList && opMalloc)
		opMalloc -> Free(spItemIdList);
//	if(spThisItemIdList && opMalloc)
//		opMalloc -> Free(spThisItemIdList);
	if(opMalloc)
		opMalloc -> Release();

	return TRUE;
}

void EMWinListViewControl::Invalidate(const EMRect p_oRect)
{
	RECT sRect;
	if(SetRect(&sRect, p_oRect.m_vLeft, p_oRect.m_vTop, p_oRect.m_vRight + 1, p_oRect.m_vBottom + 1) == 0)
eo << "Invalidate: SetRect failed" << ef;
	if(InvalidateRect(m_opWindowHandle, &sRect, true/*false*/) == 0)
eo << "Invalidate: InvalidateRect failed" << ef;
}

bool EMWinListViewControl::IsDragEnabled()
{
	return m_vEnableDrag;
}

bool EMWinListViewControl::IsEnabled()
{
	return IsWindowEnabled(m_opWindowHandle) != 0;
}

LPITEMIDLIST EMWinListViewControl::MergeItemIdLists(const LPITEMIDLIST p_spItemIdListOne, const LPITEMIDLIST p_spItemIdListTwo)
{
	LPITEMIDLIST spItemIdListNew;
	UINT vItemIdListOneSize;
	UINT vItemIdListTwoSize;

	if(p_spItemIdListOne)	//May be NULL
		vItemIdListOneSize = GetSize(p_spItemIdListOne) - sizeof(p_spItemIdListOne -> mkid.cb);
	else
		vItemIdListOneSize = 0;

	vItemIdListTwoSize = GetSize(p_spItemIdListTwo);

	spItemIdListNew = CreateItemIdList(vItemIdListOneSize + vItemIdListTwoSize);
	if(spItemIdListNew)
	{
		if(p_spItemIdListOne)
			hmemcpy(spItemIdListNew, p_spItemIdListOne, vItemIdListOneSize);
		hmemcpy(((LPSTR)spItemIdListNew) + vItemIdListOneSize, p_spItemIdListTwo, vItemIdListTwoSize);
	}
	return spItemIdListNew;
}

bool EMWinListViewControl::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
	case CHANGE_LIST_VIEW_CONTROL_STYLE_TO_ICON:
		SwitchView(EM_LIST_VIEW_STYLE_ICON);
		return true;
	case CHANGE_LIST_VIEW_CONTROL_STYLE_TO_SMALL_ICON:
		SwitchView(EM_LIST_VIEW_STYLE_SMALL_ICON);
		return true;
	case CHANGE_LIST_VIEW_CONTROL_STYLE_TO_LIST:
		SwitchView(EM_LIST_VIEW_STYLE_LIST);
		return true;
	case CHANGE_LIST_VIEW_CONTROL_STYLE_TO_REPORT:
		SwitchView(EM_LIST_VIEW_STYLE_REPORT);
		return true;
	}
	return false;
}

BOOL EMWinListViewControl::PopulateListView(LPTVITEMDATA p_spItemIdList, LPSHELLFOLDER p_spShellFolder)
{
	//ListView_DeleteAllItems(m_opWindowHandle);
	DeleteAllItems();

	if (!InitiateListViewImageLists() || !InitiateListViewItems(p_spItemIdList, p_spShellFolder))
		return FALSE;

	ListView_SortItems(m_opWindowHandle, CompareFunc, 0);
	return TRUE;
}

void EMWinListViewControl::SetCommand(uint32 p_vCommand)
{
	m_vCommand = p_vCommand;
}

void EMWinListViewControl::SetFocus()
{
	::SetFocus(m_opWindowHandle);
}

void EMWinListViewControl::SetFrame(EMRect p_oFrame)
{
	m_oFrame = p_oFrame;
	m_oBounds.m_vRight = p_oFrame.m_vRight - p_oFrame.m_vLeft;
	m_oBounds.m_vBottom = p_oFrame.m_vBottom - p_oFrame.m_vTop;
	MoveWindow(m_opWindowHandle, p_oFrame.m_vLeft, p_oFrame.m_vTop, p_oFrame.GetWidth() + 1, p_oFrame.GetHeight() + 1, true); //Changed to true to test if working ok
//	SetWindowPos(m_opWindowHandle, HWND_BOTTOM, p_oFrame.m_vLeft, p_oFrame.m_vTop, p_oFrame.GetWidth() + 1, p_oFrame.GetHeight() + 1, SWP_NOZORDER | SWP_NOACTIVATE/* | SWP_NOREDRAW*/);
}

void EMWinListViewControl::SetLabel(const char* p_vpLabel)
{
	SetWindowText(m_opWindowHandle, p_vpLabel);
}

void EMWinListViewControl::Scroll(int32 p_vItemID)
{
	RECT sItemRect;
	RECT sPrevItemRect;
	if(p_vItemID > -1)
	{
		ListView_GetItemRect(m_opWindowHandle, p_vItemID, &sItemRect, LVIR_LABEL);
		EMDebugger("Scroll to a specific item does Not work yet");
	}
	else
	{
		ListView_GetItemRect(m_opWindowHandle, m_vItemID - 2, &sPrevItemRect, LVIR_LABEL);
		ListView_GetItemRect(m_opWindowHandle, m_vItemID - 1, &sItemRect, LVIR_LABEL);
	}
	if(sItemRect.bottom + sItemRect.bottom - sItemRect.top > m_oFrame.GetHeight() && sPrevItemRect.bottom <= m_oFrame.GetHeight())
	{
		ListView_Scroll(m_opWindowHandle, 0, (sItemRect.bottom - sItemRect.top) * 2);
		eo << "Scrolling" << ef;
	}
}

void EMWinListViewControl::SetTreeView(EMTreeViewControl* p_vTreeView)
{
	//m_opTreeViewControl = dynamic_cast<EMWinTreeViewControl*>(p_vTreeView);
	m_opTreeViewControl = p_vTreeView;
}

void EMWinListViewControl::SetWindowWindow(EMWinWindow* p_opWindow)
{
	m_opWindowWindow = p_opWindow;
}

void EMWinListViewControl::SetUniqueIndexColumn(int16 p_vUniqueIndexColumn)
{
	m_vUniqueIndexColumn = p_vUniqueIndexColumn;
}

void EMWinListViewControl::Show()
{
	ShowWindow(m_opWindowHandle, SW_SHOW);
}

LRESULT CALLBACK EMWinListViewControl::SubclassedWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo)
{
//eo << "ListViewControl subclassing" << ef;
	EMWinListViewControl* opListView;
	opListView = reinterpret_cast<EMWinListViewControl*>(GetWindowLong(p_vWindowHandle, GWL_USERDATA));

	if(opListView)
	{
		if(p_vMessage == WM_MOUSEMOVE)
		{
		//	eo << "ListViewControl  WM_MOUSEMOVE" << ef;
			SendMessage(GetParent(p_vWindowHandle), p_vMessage, p_vParameterOne, p_vParameterTwo);
		}

	if(p_vMessage == WM_CHAR)
		{					
	eo << "Window: WM_KEYDOWN: " << ef;
			switch(static_cast<int>(p_vParameterOne))
			{
			case EM_RETURN:
				opListView -> Notify(EM_GUI_COMPONENT_ENTER_PRESSED);
				return 0;
			case EM_ESCAPE:
				opListView -> Notify(EM_GUI_COMPONENT_ESC_PRESSED);
				return 0;
			case EM_TAB:
				if(EM_SHIFT_KEY & EMKeyboard::Instance() -> GetModifiers())
					opListView -> Notify(EM_GUI_COMPONENT_SHIFT_TAB_PRESSED);	
				else
					opListView -> Notify(EM_GUI_COMPONENT_TAB_PRESSED);
				return 0;
			}
		}
		if(p_vMessage == WM_SETFOCUS)
			opListView -> Notify(EM_GUI_COMPONENT_GOT_FOCUS);	
	}
	return CallWindowProc(opListView -> m_opDefaultWindowProcedure, p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);

}
/*
	if(opListView)
	{
		switch(p_vMessage)
		{
		case WM_NCLBUTTONDOWN:
	//		eo << "ListView: WM_NCLBUTTONDOWN" << ef;
			return 0;
		case WM_NCMOUSEMOVE:
	//		eo << "ListView: WM_NCMOUSEMOVE" << ef;
			if(opListView ->IsDragMove())
			{
				eo << "ListView: DragLeave" << ef;
				opListView -> SetDragEnter(false);
				opListView ->SetDragMove(false);

			}
			else
			{
				opListView -> SetDragEnter(true);
				eo << "ListView: DragEnter" << ef;
			}
			return 0;
*		case WM_MOUSEMOVE:
	//		eo << "ListView: EM_MOUSEMOVE" << ef;
			if(opListView ->IsDragEnter())
			{
				opListView ->SetDragMove(true);
				eo << "ListView: DragMove" << ef;
			}
			return 0;
		case WM_LBUTTONUP:
	//		eo << "ListView: WM_LBUTTONUP" << ef;
			if(opListView ->IsDragMove())
			{
				eo << "ListView: DragDrop" << ef;
			}
			opListView -> SetDragEnter(false);
			opListView ->SetDragMove(false);

			return 0;
		}
/*		uint32 vMessage();
		switch(HIWORD(p_vParameterOne))
		{
		}
		if(vMessage != 0)
			if(opButton -> Notify(vMessage))
				return 0;
*
	}

	switch(p_vMessage)
	{
	case WM_MOUSEMOVE:
//		eo << "ListView: EM_MOUSEMOVE" << ef;
		if(opListView ->IsDragEnter())
		{
			opListView ->SetDragMove(true);
			eo << "ListView: DragMove" << ef;
		}
		return 0;
	case WM_LBUTTONUP:
//		eo << "ListView: WM_LBUTTONUP" << ef;
		if(opListView ->IsDragMove())
		{
			eo << "ListView: DragDrop" << ef;
		}
		opListView -> SetDragEnter(false);
		opListView ->SetDragMove(false);

		return 0;
	}

	int vResult = CallWindowProc(opListView -> m_opDefaultWindowProcedure, p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
//	if(p_vMessage == WM_LBUTTONUP)
//		SetFocus(reinterpret_cast<HWND>(GetForegroundWindow()));
	return vResult;

//	return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);

}*/

void EMWinListViewControl::SwitchView(EMListViewStyle p_eListViewStyle)
{
	uint32 vViewStyle;
	uint32 vStyle = GetWindowLong(m_opWindowHandle, GWL_STYLE);

	switch(p_eListViewStyle)
	{
	case EM_LIST_VIEW_STYLE_ICON:
		vViewStyle = LVS_ICON;
		break;
	case EM_LIST_VIEW_STYLE_SMALL_ICON:
		vViewStyle = LVS_SMALLICON;
		break;
	case EM_LIST_VIEW_STYLE_LIST:
		vViewStyle = LVS_LIST;
		break;
	case EM_LIST_VIEW_STYLE_REPORT:
		vViewStyle = LVS_REPORT;
		break;
	}

	if((vStyle & LVS_TYPEMASK) != vViewStyle)
		SetWindowLong(m_opWindowHandle, GWL_STYLE, (vStyle & ~LVS_TYPEMASK) | vViewStyle);
}

#endif
