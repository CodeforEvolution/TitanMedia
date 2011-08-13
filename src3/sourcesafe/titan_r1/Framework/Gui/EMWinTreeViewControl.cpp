#include "EMWinTreeViewControl.h"

#ifdef PLATFORM_WINDOWS

#include "EMApplication.h"
#include "EMCommandRepository.h"
#include "EMWinGroupBox.h"
#include "EMWinView.h"
#include "EMWinWindow.h"

#include "EMWinListViewControl.h"

#include <windowsx.h>

EMWinTreeViewControl::EMWinTreeViewControl(const EMRect p_oFrame, const char* p_vpName, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand) :
m_oFrame(p_oFrame),
m_oBounds(0, 0, p_oFrame.m_vRight - p_oFrame.m_vLeft, p_oFrame.m_vBottom - p_oFrame.m_vTop),
m_vCommand(p_vCommand),
m_vMessage(p_vMessage),
m_vResizingMode(p_vResizingMode),
m_opListViewControl(NULL),
m_vStyle(TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES | TVS_SHOWSELALWAYS | WS_VISIBLE | WS_CHILD/* | WS_BORDER*/)
{
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
}

EMWinTreeViewControl::~EMWinTreeViewControl()
{
//	DeleteAllItems();
}

void EMWinTreeViewControl::AddChild(EMGUIComponent* p_opView)
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

bool EMWinTreeViewControl::AddToComponent(EMGUIComponent* p_opComponent)
{
	HWND opParent;
//	HIMAGELIST    hImageList;
//	SHFILEINFO    sfi;
//	ITEMIDLIST oItemList;
//	LPITEMIDLIST opItemList = &oItemList;

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
		m_opWindowHandle = CreateWindowEx(/*WS_EX_CLIENTEDGE*/ WS_EX_STATICEDGE, WC_TREEVIEW , "Test", m_vStyle, m_oFrame.m_vLeft,  
								m_oFrame.m_vTop, m_oFrame.m_vRight - m_oFrame.m_vLeft + 1, m_oFrame.m_vBottom - m_oFrame.m_vTop + 1,
								opParent, NULL, EMApplication::GetApplicationInstance(), NULL);
		SendMessage(m_opWindowHandle, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), true);
		SetWindowLong(m_opWindowHandle, GWL_USERDATA, reinterpret_cast<int32>(this));
//		ListView_SetExtendedListViewStyle(m_opWindowHandle, LVS_EX_FULLROWSELECT);
		m_opDefaultWindowProcedure = reinterpret_cast<WNDPROC>(GetWindowLong(m_opWindowHandle, GWL_WNDPROC));
		SetWindowLong(m_opWindowHandle, GWL_WNDPROC, reinterpret_cast<LONG>(SubclassedWindowProcedure));

	}
	else
		eo << "Could not load CommonControl dll" << ef;

	InitiateTreeViewImageList();
/*
    hImageList = (HIMAGELIST)SHGetFileInfo((LPCSTR)"C:\\", 
                                           0,
                                           &sfi, 
                                           sizeof(SHFILEINFO), 
                                           SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
	//SHGetSpecialFolderLocation(m_opWindowHandle, CSIDL_DESKTOP, &opItemList);

	// Attach ImageList to TreeView

    if (hImageList && m_opWindowHandle)
        TreeView_SetImageList(m_opWindowHandle, hImageList, 0);

	LPSHELLFOLDER lpsf=0;
    LPITEMIDLIST  lpi=0;
    HRESULT hr;
    
    hr=SHGetDesktopFolder(&lpsf);

    if (SUCCEEDED(hr))
    {
       TreeView_DeleteAllItems(m_opWindowHandle);

       /*FillTreeView(m_opWindowHandle,
                    lpsf,
                    opItemList,
                    TVI_ROOT, true);/
	   FillTreeView(m_opWindowHandle,
                    lpsf,
                    NULL,
                    TVI_ROOT);
    }
        
    if (lpsf) lpsf->Release();
*/

//	SetParent(m_opWindowHandle, opParent);
//	SetWindowPos(m_opWindowHandle, NULL, m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.GetWidth() + 1, m_oFrame.GetHeight() + 1, SWP_NOZORDER);	

	return true;
}

/*void EMWinListViewControl::AddItem(const char* p_opText)
{
	static int vItemID = 0;

	LVITEM vItem;
	vItem.mask = LVIF_TEXT | LVIF_PARAM ;
	vItem.pszText = (char*)p_opText;
	vItem.iItem = vItemID++;
	vItem.iSubItem = 0;
	
	LPLVITEM test = new LVITEM;

	memcpy(test , &vItem, sizeof(vItem));
	test -> lParam = reinterpret_cast<LPARAM>(m_opWindowHandle);
	vItem.lParam = reinterpret_cast<LPARAM>(test);
	m_oItemPointers.push_back(test);

	if(ListView_InsertItem(m_opWindowHandle, &vItem) == -1)
		eo << "Faild adding text item" << ef;
}

void EMWinListViewControl::AddSubItem(const char* p_opText, int p_vItemId, int p_vColumn)
{
	LVITEM vItem;
	vItem.mask = LVIF_TEXT;
	vItem.pszText = (char*)p_opText;
	vItem.iItem = p_vItemId;
	vItem.iSubItem = p_vColumn;
	if(ListView_SetItem(m_opWindowHandle, &vItem) != 0)
		eo << "SetItem SubItem: " << (char*) p_opText << " to ItemId: " << p_vItemId << " and Column: " << p_vColumn << " OK!" << ef;
}
*/

EMRect EMWinTreeViewControl::Bounds()
{
	return m_oBounds;
}

LPITEMIDLIST EMWinTreeViewControl::CopyItemIdList(LPMALLOC p_opMalloc, LPITEMIDLIST p_spItemIdList)
{
	LPITEMIDLIST spItemIdListTemp((LPITEMIDLIST)p_opMalloc -> Alloc(p_spItemIdList -> mkid.cb + sizeof(p_spItemIdList -> mkid.cb)));

	CopyMemory((void*)spItemIdListTemp, (const void*)p_spItemIdList, p_spItemIdList -> mkid.cb + sizeof(p_spItemIdList -> mkid.cb));

	return spItemIdListTemp;
}

LPITEMIDLIST EMWinTreeViewControl::CreateItemIdList(uint32 p_vSize)
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

void EMWinTreeViewControl::ExecuteCommand()
{
	if(m_vCommand != 0)
		EMCommandRepository::Instance() -> ExecuteCommand(m_vCommand);
}

void EMWinTreeViewControl::ExpandTree(LPSHELLFOLDER p_spShellFolder, ITEMIDLIST* p_spItemIdList, HTREEITEM p_vTreeItemHandle, bool p_vFirst)
{
	TV_ITEM			sTreeViewItem;							// TreeView Item.
	TV_INSERTSTRUCT sTreeViewInsert;						// TreeView Insert Struct.
	HTREEITEM		vPreviousTreeItemHandle(NULL);			// Previous Item Added.
//	LPSHELLFOLDER	spShellFolder = NULL;
	LPENUMIDLIST	opEnumIdList(NULL);
	LPITEMIDLIST	spItemIdList(NULL);
//	LPITEMIDLIST	spTempItemIdList = NULL;
	LPITEMIDLIST	spThisItemIdList(NULL);
	LPTVITEMDATA	spTreeViewItemData(NULL);		//Include LPTVITEMDATA as struct in this class.
	LPMALLOC		opMalloc(NULL);
	uint32			vFetched;
//	uint32			vCount = 0;
	HRESULT			hr;
	char			vBuffer[256];
	HWND			opParentWindowHandle = GetParent(m_opWindowHandle);

	hr = SHGetMalloc(&opMalloc);
	if (FAILED(hr))
	{
	//	EMDebugger("Faild to GetMalloc for Shell");
		return;
	}

	// Hourglass on!

	SetCapture(opParentWindowHandle);
	SetCursor(LoadCursor(NULL, IDC_WAIT));

	if (SUCCEEDED(hr))
	{
		hr = p_spShellFolder -> EnumObjects(opParentWindowHandle, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &opEnumIdList);

		if (SUCCEEDED(hr))
		{
			opEnumIdList -> Reset();
			while ((/*p_vFirst ? p_vFirst : */S_OK == opEnumIdList -> Next(1, &spItemIdList, &vFetched)))
			{
				//Create a fully qualified path to the current item
				//The SH* shell api's take a fully qualified path pidl,
				//(see GetIcon above where I call SHGetFileInfo) whereas the
				//interface methods take a relative path pidl.

				ULONG ulAttrs = SFGAO_HASSUBFOLDER | SFGAO_FOLDER;

				p_spShellFolder -> GetAttributesOf(1, (const _ITEMIDLIST**)&spItemIdList, &ulAttrs);

				if (ulAttrs & (SFGAO_HASSUBFOLDER | SFGAO_FOLDER))
				{
					//We need this next if statement so that we don't add things like
					//the MSN to our tree.  MSN is not a folder, but according to the
					//shell is has subfolders....

					if (ulAttrs & SFGAO_FOLDER)
					{
						sTreeViewItem.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

						if (ulAttrs & SFGAO_HASSUBFOLDER)
						{
							//This item has sub-folders, so let's put the + in the TreeView.
							//The first time the user clicks on the item, we'll populate the
							//sub-folders then.

							sTreeViewItem.cChildren = 1;
							sTreeViewItem.mask |= TVIF_CHILDREN;
						}

						//OK, let's get some memory for our ITEMDATA struct

						spTreeViewItemData = (LPTVITEMDATA)opMalloc -> Alloc(sizeof(TVITEMDATA)); //FIXME: Where is this set free?
						if (!spTreeViewItemData)
							break;//goto Done;
   
						//Now get the friendly name that we'll put in the treeview...
						if(!p_vFirst)
						{
							if (!GetName(p_spShellFolder, spItemIdList, SHGDN_NORMAL, vBuffer))
								break;//goto Done;
						}
						else
						{
							if (!GetName(p_spShellFolder, p_spItemIdList, SHGDN_NORMAL, vBuffer))
								break;//goto Done;
						}

eo << "vBuffer " << vBuffer << ef;

						sTreeViewItem.pszText	 = vBuffer;
						sTreeViewItem.cchTextMax = MAX_PATH;

						if(!p_vFirst)
							spThisItemIdList = MergeItemIdLists(p_spItemIdList, spItemIdList, p_vFirst);
						else
							spThisItemIdList = p_spItemIdList;

						//Now, make a copy of the ITEMIDLIST

						if(!p_vFirst)
							spTreeViewItemData -> m_spItemIdList = CopyItemIdList(opMalloc, spItemIdList);
						else
							spTreeViewItemData -> m_spItemIdList = /*CopyItemIdList(opMalloc, */p_spItemIdList/*)*/;

						GetNormalAndSelectedIcons(spThisItemIdList, &sTreeViewItem);

						spTreeViewItemData -> m_spParentShellFolder = p_spShellFolder;    //Store the parent folders SF
						p_spShellFolder -> AddRef();

						//So, you ask, what's the story here?  Why do we have 2
						//functions that apparently do the same thing?  Well, 
						//ParseDisplayName() (used in GetFullyQualPidl) does not 
						//work well for non-file system objects such as the My 
						//Computer and Net Hood.  So, since we know that these 
						//guys will only show up when we are enumerating the root 
						//of the namespace (as identified by an p_vTreeItemHandle of TVI_ROOT), 
						//we special case this here.  You *could* use ConcatPidls() 
						//in ALL cases, but the GetFullyQualPidl() is shown for 
						//demonstration purposes, since it's the more intuative way.

//Commented out for now, since we need ConcatPidls for *any* non-file system object.
//This includes things like Control Panel, et al.

//						if (p_vTreeItemHandle == TVI_ROOT)
						if(!p_vFirst)
							spTreeViewItemData -> m_spItemIdListSecond = MergeItemIdLists(p_spItemIdList, spItemIdList, p_vFirst);
						else
							spTreeViewItemData -> m_spItemIdListSecond = p_spItemIdList;
//						else
//							spTreeViewItemData -> lpifq=GetFullyQualPidl(p_spShellFolder, spItemIdList);
   
						sTreeViewItem.lParam = (LPARAM)spTreeViewItemData;
   
						// Populate the TreeVeiw Insert Struct
						// The item is the one filled above.
						// Insert it after the last item inserted at this level.
						// And indicate this is a root entry.

						sTreeViewInsert.item		 = sTreeViewItem;
						sTreeViewInsert.hInsertAfter = vPreviousTreeItemHandle;
						sTreeViewInsert.hParent		 = p_vTreeItemHandle;

						// Add the item to the tree

						vPreviousTreeItemHandle = TreeView_InsertItem(m_opWindowHandle, &sTreeViewInsert);
					}

					opMalloc -> Free( spThisItemIdList);
					spThisItemIdList = NULL;
				}

				opMalloc -> Free( spItemIdList);	//Finally, free the pidl that the shell gave us...
				spItemIdList = NULL;
				if(p_vFirst)
				{
					p_vFirst = false;
					break;
				}
			}
		}

	}
	else
		return;

//Done:

	// Hourglass off!

	ReleaseCapture();
	SetCursor(LoadCursor(NULL, IDC_ARROW));

	if (opEnumIdList)
		opEnumIdList->Release();

	//The following 2 if statements will only be TRUE if we got here on an
	//error condition from the "goto" statement.  Otherwise, we free this memory
	//at the end of the while loop above.
	if (spItemIdList && opMalloc)
		opMalloc->Free( spItemIdList);

	if (spThisItemIdList && opMalloc)
		opMalloc->Free( spThisItemIdList);

	if (opMalloc)
		opMalloc->Release();
}

void EMWinTreeViewControl::DeleteAllItems()
{
	LPTVITEMDATA spTreeViewItemData;
	TVITEM sTreeViewItem;
	char temp[255];
	sTreeViewItem.mask = TVIF_PARAM | TVIF_TEXT;
	sTreeViewItem.pszText = temp;
	sTreeViewItem.cchTextMax = 255;
//	sTreeViewItem.hItem = TreeView_GetRoot(m_opWindowHandle);
	HTREEITEM vItemParent = TreeView_GetRoot(m_opWindowHandle);
//	HTREEITEM vItemParent = TVI_ROOT;//TreeView_GetNextItem(m_opWindowHandle, TVI_ROOT, TVGN_ROOT);
	HTREEITEM vItemSibiling(NULL);
	HTREEITEM vItemChild(NULL);
	HTREEITEM vItemParentParent(NULL);

	bool vContinue(true);
	bool vGoBack(false);

	

	if(vItemParent == NULL)
		return;

	LPMALLOC		opMalloc(NULL);
	HRESULT hr = SHGetMalloc(&opMalloc);
	if (FAILED(hr))
	{
	//	EMDebugger("Faild to GetMalloc for Shell");
		return;
	}
	
	while(1)
	{
		vItemChild = TreeView_GetChild(m_opWindowHandle, vItemParent);
		if(vItemChild == NULL)
		{
			vItemSibiling = TreeView_GetNextSibling(m_opWindowHandle, vItemParent);
			if(vItemSibiling == NULL)
			{
				vItemParentParent = TreeView_GetParent(m_opWindowHandle, vItemParent);
				if(vItemParentParent == NULL)
					vContinue = false;
				else
				{
			//		vItemParent = vItemParentParent;
			//		vItemSibiling = TreeView_GetNextSibling(m_opWindowHandle, vItemParent);
					vGoBack = true;
				}
			}
			sTreeViewItem.hItem = vItemParent;
			TreeView_GetItem(m_opWindowHandle, &sTreeViewItem);
			spTreeViewItemData = (LPTVITEMDATA)sTreeViewItem.lParam;
			opMalloc -> Free(spTreeViewItemData -> m_spItemIdList);
			opMalloc -> Free(spTreeViewItemData -> m_spItemIdListSecond);
			//spTreeViewItemData -> m_spParentShellFolder -> Release();
			vItemParent = vItemSibiling;
			if(vContinue == false)
				break;
			if(vGoBack)
			{
				vItemParent = vItemParentParent;
				vItemSibiling = TreeView_GetNextSibling(m_opWindowHandle, vItemParent);
				sTreeViewItem.hItem = vItemParent;
				TreeView_GetItem(m_opWindowHandle, &sTreeViewItem);
				spTreeViewItemData = (LPTVITEMDATA)sTreeViewItem.lParam;
				opMalloc -> Free(spTreeViewItemData -> m_spItemIdList);
				opMalloc -> Free(spTreeViewItemData -> m_spItemIdListSecond);
			//	spTreeViewItemData -> m_spParentShellFolder -> Release();
				vItemParent = vItemSibiling;
				vGoBack = false;
			}

		}
		else
			vItemParent = vItemChild;
	}

//	spTreeViewItemData = (LPTVITEMDATA)sTreeViewItem.lParam;
//	spTreeViewItemData -> m_spParentShellFolder -> Release();

	TreeView_DeleteAllItems(m_opWindowHandle);
	opMalloc -> Release();
}

void EMWinTreeViewControl::Enable(bool p_vEnable)
{
	EnableWindow(m_opWindowHandle, p_vEnable);
}

EMRect EMWinTreeViewControl::Frame() const
{
	return m_oFrame;
}

int EMWinTreeViewControl::GetIcon(LPITEMIDLIST p_spItemIdList, uint32 p_vFlags)
{
	SHFILEINFO sShellFileInfo;

	SHGetFileInfo((LPCSTR)p_spItemIdList, 0, &sShellFileInfo, sizeof(SHFILEINFO), p_vFlags);

	return sShellFileInfo.iIcon;
}

EMListViewControl* EMWinTreeViewControl::GetListView()
{
	return m_opListViewControl;
}

bool EMWinTreeViewControl::GetName(LPSHELLFOLDER p_spShellFolder, LPITEMIDLIST p_spItemIdList, DWORD p_vFlags, char* p_vFriendlyName)
{
	bool   bSuccess = true;
	STRRET sStringReturn;
	int test;
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

			test = sizeof(p_vFriendlyName);
			test = WideCharToMultiByte(CP_ACP,					// CodePage
									   0/*WC_COMPOSITECHECK | WC_DISCARDNS | WC_SEPCHARS | WC_DEFAULTCHAR*/, // dwFlags
									   sStringReturn.pOleStr,				// lpWideCharStr
									   -1,						// cchWideChar
									   p_vFriendlyName,			// lpMultiByteStr
									   255/*sizeof(lpFriendlyName)*/, // cchMultiByte,
									   NULL,					// lpDefaultChar,
									   NULL);					// lpUsedDefaultChar
			opMalloc -> Free(sStringReturn.pOleStr);
			break;
		case STRRET_OFFSET:
			lstrcpy(p_vFriendlyName, (LPSTR)p_spItemIdList + sStringReturn.uOffset);
			break;
		case STRRET_CSTR:
			lstrcpy(p_vFriendlyName, (LPSTR)sStringReturn.cStr);
			break;
		default:
			return false;
		}
	}
	else
		return false;
	return true;
}

void* EMWinTreeViewControl::GetNativeView() const
{
	return m_opWindowHandle;
}

LPITEMIDLIST EMWinTreeViewControl::GetNext(const LPITEMIDLIST p_spItemIdList)
{
	char* vpTempMemStorage = (char*)p_spItemIdList;

	vpTempMemStorage += p_spItemIdList -> mkid.cb;

	return (LPITEMIDLIST)vpTempMemStorage;
}

void EMWinTreeViewControl::GetNormalAndSelectedIcons(LPITEMIDLIST p_spItemIdList, LPTV_ITEM p_spTreeViewItem)
{
	//Note that we don't check the return value here because if GetIcon()
	//fails, then we're in big trouble...
eo << "Getting Icon" << ef;
	p_spTreeViewItem -> iImage = GetIcon(p_spItemIdList, SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

	p_spTreeViewItem -> iSelectedImage = GetIcon(p_spItemIdList, SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_OPENICON);
}

uint32 EMWinTreeViewControl::GetResizingMode()
{
	return m_vResizingMode;
}

uint32 EMWinTreeViewControl::GetSize(const LPITEMIDLIST p_spItemIdList)
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

// Generic window procedure passed to WIN32 API
LRESULT CALLBACK EMWinTreeViewControl::GlobalWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo)
{
//eo << "EMWinListViewControl::GlobalWindowProcedure" << ef;
	EMWinTreeViewControl* opTreeViewControl;
	opTreeViewControl = reinterpret_cast<EMWinTreeViewControl*>(GetWindowLong(p_vWindowHandle, GWL_USERDATA));

/*eo << "LVN_BEGINDRAG " << LVN_BEGINDRAG << ef;
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
*/
/*
eo << "NM_FIRST " << NM_FIRST << ef;
eo << "NM_FIRST - 1 " << NM_FIRST - 1 << ef;
eo << "NM_LASR " << NM_LAST << ef;
eo << "TVN_FIRST " << TVN_FIRST << ef;
eo << "TVN_LASR " << TVN_LAST << ef;
eo << "NM_SETCURSOR " << NM_SETCURSOR << ef;
eo << "Code " << reinterpret_cast<LPNMHDR>(p_vParameterTwo) -> code << ef;
*/

	HRESULT hr;
	LPTVITEMDATA spTreeViewItemData;  //Long pointer to TreeView item data
	NMTREEVIEW *spNMTreeView = (NMTREEVIEW *)p_vParameterTwo;
	LPSHELLFOLDER spShellFolder = NULL;
	TVSORTCB sTreeViewSortChild;

	if(opTreeViewControl)
	{
		uint32 vMessage(0);
		switch(HIWORD(p_vParameterOne))
		{
		case BN_CLICKED:
//eo << "BN_CLICKED " << BN_CLICKED << ef;
			break;
		default:
			eo << "Default" << ef;
			vMessage = 0;
		};

		switch(reinterpret_cast<LPNMHDR>(p_vParameterTwo) -> code)
		{
		case NM_CLICK:
			eo << "NM_CLICK" << ef;
			vMessage = opTreeViewControl -> m_vMessage;
			break;
		case NM_DBLCLK:
			eo << "NM_DBLCLK" << ef;
			break;
		case NM_RCLICK:
			eo << "NM_RCLICK" << ef;
			break;

		case TVN_SELCHANGED:
eo << "TVN_SELCHANGE" << ef;
			if(opTreeViewControl -> __m_vNoUpdate)
				break;
		       //Do this only if we are not exiting the application...

			spTreeViewItemData = (LPTVITEMDATA)spNMTreeView -> itemNew.lParam;

			hr = spTreeViewItemData -> m_spParentShellFolder -> BindToObject(spTreeViewItemData -> m_spItemIdList, NULL, IID_IShellFolder, (LPVOID *)&spShellFolder);
			if(SUCCEEDED(hr))
			{
				dynamic_cast<EMWinListViewControl*>(opTreeViewControl -> GetListView()) -> PopulateListView(spTreeViewItemData, spShellFolder);
				spShellFolder -> Release();
			}
			break;
		case TVN_ITEMEXPANDING:
eo << "TVN_ITEMEXPANDING" << ef;
			if(opTreeViewControl -> __m_vNoUpdate)
				break;
			if((spNMTreeView -> itemNew.state & TVIS_EXPANDEDONCE))
				break;

			spTreeViewItemData = (LPTVITEMDATA)spNMTreeView -> itemNew.lParam;
	
			hr=spTreeViewItemData -> m_spParentShellFolder -> BindToObject(spTreeViewItemData -> m_spItemIdList, NULL, IID_IShellFolder, (LPVOID *)&spShellFolder);
	
			if(SUCCEEDED(hr))
			{
eo << "SUCCEEDED" << ef;
				opTreeViewControl -> ExpandTree(spShellFolder, spTreeViewItemData -> m_spItemIdListSecond, spNMTreeView -> itemNew.hItem);
				spShellFolder -> Release();
			}
	
			sTreeViewSortChild.hParent     = spNMTreeView -> itemNew.hItem;
			sTreeViewSortChild.lpfnCompare = TreeViewCompareProcedure;
			sTreeViewSortChild.lParam      = 0;

			TreeView_SortChildrenCB(reinterpret_cast<HWND>(opTreeViewControl -> GetNativeView()), &sTreeViewSortChild, 0);
	
			break;

		default:
			vMessage = 0;
		};
		if(vMessage != 0)
			if(opTreeViewControl -> Notify(vMessage))
				return 0;
	}
//eo << "EMWinListViewControl is Calling DefWindowProc()" << ef;
	return DefWindowProc(p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
}

void EMWinTreeViewControl::Hide()
{
	ShowWindow(m_opWindowHandle, SW_HIDE);
	DeleteAllItems();
}

void EMWinTreeViewControl::InitiateTreeViewImageList()
{
	HIMAGELIST hImageList;
	SHFILEINFO sShellFileInfo;
//	ITEMIDLIST oItemList;
//	LPITEMIDLIST opItemList = &oItemList;
	LPSHELLFOLDER spShellFolder = NULL;
	HRESULT hr;

	hImageList = (HIMAGELIST)SHGetFileInfo((LPCSTR)"C:\\", 0, &sShellFileInfo, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
	//SHGetSpecialFolderLocation(m_opWindowHandle, CSIDL_DESKTOP, &opItemList);

	// Attach ImageList to TreeView

	if (hImageList && m_opWindowHandle)
		TreeView_SetImageList(m_opWindowHandle, hImageList, 0);

	hr = SHGetDesktopFolder(&spShellFolder);

	if (SUCCEEDED(hr))
	{
		TreeView_DeleteAllItems(m_opWindowHandle);
		//DeleteAllItems();

		//FillTreeView(m_opWindowHandle, spShellFolder, opItemList, TVI_ROOT, true);
		ExpandTree(spShellFolder, NULL, TVI_ROOT, false);
	}

//ULONG test;
	if (spShellFolder)
		spShellFolder -> Release();
//		while(test = spShellFolder ->Release() == 0)
//			;

}

void EMWinTreeViewControl::Invalidate(const EMRect p_oRect)
{
	RECT sRect;
	if(SetRect(&sRect, p_oRect.m_vLeft, p_oRect.m_vTop, p_oRect.m_vRight + 1, p_oRect.m_vBottom + 1) == 0)
eo << "Invalidate: SetRect failed" << ef;
	if(InvalidateRect(m_opWindowHandle, &sRect, true/*false*/) == 0)
eo << "Invalidate: InvalidateRect failed" << ef;
}

bool EMWinTreeViewControl::IsEnabled()
{
	return IsWindowEnabled(m_opWindowHandle) != 0;
}

LPITEMIDLIST EMWinTreeViewControl::MergeItemIdLists(const LPITEMIDLIST p_spItemIdListOne, const LPITEMIDLIST p_spItemIdListTwo, bool p_vFirst)
{
	LPITEMIDLIST spItemIdListNew;
	uint32 vItemIdListOneSize;
	uint32 vItemIdListTwoSize;

	if (p_spItemIdListOne)  //May be NULL
		vItemIdListOneSize = GetSize(p_spItemIdListOne) - sizeof(p_spItemIdListOne -> mkid.cb);
	else
		vItemIdListOneSize = 0;
 
	vItemIdListTwoSize = GetSize(p_spItemIdListTwo);

	spItemIdListNew = CreateItemIdList(vItemIdListOneSize + vItemIdListTwoSize);
	if (spItemIdListNew)
	{
		if (p_spItemIdListOne || p_vFirst)
			hmemcpy(spItemIdListNew, p_spItemIdListOne, vItemIdListOneSize);
		if(p_vFirst)
			return spItemIdListNew;
		hmemcpy(((LPSTR)spItemIdListNew) + vItemIdListOneSize, p_spItemIdListTwo, vItemIdListTwoSize);
//		Assert(GetSize(pidlNew) == vItemIdListOneSize + vItemIdListTwoSize);
	}
	return spItemIdListNew;
}

void EMWinTreeViewControl::SetCommand(uint32 p_vCommand)
{
	m_vCommand = p_vCommand;
}

void EMWinTreeViewControl::SetFrame(EMRect p_oFrame)
{
	m_oFrame = p_oFrame;
	m_oBounds.m_vRight = p_oFrame.m_vRight - p_oFrame.m_vLeft;
	m_oBounds.m_vBottom = p_oFrame.m_vBottom - p_oFrame.m_vTop;
	MoveWindow(m_opWindowHandle, p_oFrame.m_vLeft, p_oFrame.m_vTop, p_oFrame.GetWidth() + 1, p_oFrame.GetHeight() + 1, true);
//	SetWindowPos(m_opWindowHandle, HWND_BOTTOM, p_oFrame.m_vLeft, p_oFrame.m_vTop, p_oFrame.GetWidth() + 1, p_oFrame.GetHeight() + 1, SWP_NOZORDER | SWP_NOACTIVATE/* | SWP_NOREDRAW*/);

}

void EMWinTreeViewControl::SetLabel(const char* p_vpLabel)
{
	SetWindowText(m_opWindowHandle, p_vpLabel);
}

void EMWinTreeViewControl::SetListView(EMListViewControl* p_opListViewControl)
{
	m_opListViewControl = dynamic_cast<EMWinListViewControl*>(p_opListViewControl);
	//m_opListViewControl = p_opListViewControl;
}

void EMWinTreeViewControl::SetWindowWindow(EMWinWindow* p_opWindow)
{
	m_opWindowWindow = p_opWindow;
}

void EMWinTreeViewControl::Show()
{
	ShowWindow(m_opWindowHandle, SW_SHOW);
}

LRESULT CALLBACK EMWinTreeViewControl::SubclassedWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo)
{
//eo << "ListViewControl subclassing" << ef;
	EMWinTreeViewControl* opTreeView;
	opTreeView = reinterpret_cast<EMWinTreeViewControl*>(GetWindowLong(p_vWindowHandle, GWL_USERDATA));

	if(opTreeView)
	{
		if(p_vMessage == WM_MOUSEMOVE)
		{
		//	eo << "TreeViewControl  WM_MOUSEMOVE" << ef;
			SendMessage(GetParent(p_vWindowHandle), p_vMessage, p_vParameterOne, p_vParameterTwo);
		}
	}
	return CallWindowProc(opTreeView -> m_opDefaultWindowProcedure, p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
}

int CALLBACK EMWinTreeViewControl::TreeViewCompareProcedure(LPARAM lparam1, LPARAM lparam2, LPARAM lparamSort)
{
    LPTVITEMDATA lptvid1=(LPTVITEMDATA)lparam1;
    LPTVITEMDATA lptvid2=(LPTVITEMDATA)lparam2;
    HRESULT   hr;

    hr = lptvid1 -> m_spParentShellFolder -> CompareIDs(0, lptvid1->m_spItemIdList, lptvid2->m_spItemIdList);

    if (FAILED(hr))
       return 0;

    return (short)SCODE_CODE(GetScode(hr));
}

#endif
