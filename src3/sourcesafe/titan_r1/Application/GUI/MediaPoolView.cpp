#include "MediaPoolView.h"

#include "CommandIDs.h"
#include "DirectoryIDs.h"
#include "EMCommandRepository.h"
#include "EMDragDrop.h"
#include "EMGClip.h"
#include "EMFactory.h"
#include "EMGClipRepository.h"
#include "EMGUIUtilities.h"
#include "EMView.h"
#include "EMListViewControl.h"
#include "EMWinDropTarget.h"
#include "EMMessageBox.h"
#include "EMMessages.h"
#ifdef PLATFORM_WINDOWS
#include "EMWinDragDrop.h"
#endif
#include "EMMediaGlobals.h"
//#include "EMDragDrop.h"

MediaPoolView* MediaPoolView::m_opInstance = NULL;

MediaPoolView::MediaPoolView() : 
m_opDragDropData(NULL),
m_opView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 800, 800), EM_FOLLOW_LEFT | EM_FOLLOW_TOP)),
m_opListViewControl(EMFactory::Instance() -> CreateListViewControl(EMRect(0, 0, 800, 800), "ListView", 0, EM_FOLLOW_LEFT | EM_FOLLOW_TOP, 0, EM_LIST_VIEW_STYLE_REPORT, true))
{
	m_oNoValueFound.first = 0;
	m_oNoValueFound.second = -1;
}

MediaPoolView::~MediaPoolView()
{
	delete m_opListViewControl;
	delete m_opView;
}

void MediaPoolView::AddEntry(const char* p_vpFileName, const uint32 p_vEntry, const uint32 p_vType, const char* p_vpRealFileName)
{
	Pair oTemp; // = {p_vEntry, p_vType};
	oTemp.first = p_vEntry;
	oTemp.second = p_vType;
	m_oEntries[string(p_vpFileName)] = oTemp;
	m_oPathsMappedToEntries[p_vEntry] = p_vpFileName;
	if(p_vpRealFileName)
		m_oRealAudioPathToPath[p_vpRealFileName] = p_vpFileName;
}

void MediaPoolView::AddEntry(const char* p_vpFileName, Pair* p_vpEntry)
{
	m_oEntries[string(p_vpFileName)] = *p_vpEntry;
	m_oPathsMappedToEntries[p_vpEntry -> first] = p_vpFileName;

}

void MediaPoolView::AddEntryPair(int32* p_vEntryIDs)
{
	m_oVideoWithAudioEntries.push_back(EntryPair(p_vEntryIDs[0], p_vEntryIDs[1]));
}

void MediaPoolView::ChangeInstances(uint32 p_vRow)
{
	char vTemp[100];

	pair<uint32, uint32>* oTemp = GetEntry(m_opListViewControl -> GetSubItem(p_vRow, 10));
	int64 vInstaces(*reinterpret_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_USAGE_COUNT, &oTemp -> first, NULL, NULL)));

	m_opListViewControl -> ChangeSubItem(p_vRow, 1, itoa(vInstaces, vTemp, 10));
}

bool MediaPoolView::CheckIfExist(const char* p_vFullpathFilename)
{
	for(int vCounter = 0; vCounter < m_opListViewControl -> GetItemCount(); vCounter++)
	{
		if(strcmpi(p_vFullpathFilename, m_opListViewControl -> GetSubItem(vCounter, -1)) == 0)
			return true;
	}
	return false;
}

bool MediaPoolView::ClearData()
{
	m_opListViewControl -> DeleteAllItems();
	m_oEntries.erase(m_oEntries.begin(), m_oEntries.end());
	m_oPathsMappedToEntries.erase(m_oPathsMappedToEntries.begin(), m_oPathsMappedToEntries.end());
	m_oRealAudioPathToPath.erase(m_oRealAudioPathToPath.begin(), m_oRealAudioPathToPath.end());
	m_oVideoWithAudioEntries.erase(m_oVideoWithAudioEntries.begin(), m_oVideoWithAudioEntries.end());

	return true;
}

void MediaPoolView::DeleteAllClipBelongingToFile(const char* p_vpFileName) //p_vpFileName must be full path to file
{
	list<EMGClip*> oClipToDelete;
	pair<uint32, uint32>* oTemp = GetEntry(p_vpFileName);
	EMMediaPoolEntryInfo* opEntryInfo = static_cast<EMMediaPoolEntryInfo*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_INFO, &oTemp -> first, NULL, NULL));
	list<int32>::iterator oMediaIDIterator = opEntryInfo -> m_oClipIDs.begin();

	while(oMediaIDIterator != opEntryInfo -> m_oClipIDs.end())
	{
		oClipToDelete.push_back(EMGClipRepository::Instance() -> GetClip(*oMediaIDIterator));
		oMediaIDIterator++;
	}
	
	list<EMGClip*>::iterator oClipIterator = oClipToDelete.begin();
	while(oClipIterator != oClipToDelete.end())
	{
		if((*oClipIterator) -> GetFirstInGroup() != NULL)
			EMGUIUtilities::Instance() -> RemoveFromClipGroup((*oClipIterator) -> GetFirstInGroup(), (*oClipIterator));
		oClipIterator++;
	}
	EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DELETE_CLIPS_FROM_LIST, &oClipToDelete, NULL, NULL);
}

void MediaPoolView::EraseEntry(const char* p_vpFileName, bool p_vForceErase)
{
	int32 vDeleteThisEntryTo(-1);

	Pair vSelectionTempEntry = m_oEntries[p_vpFileName];
	m_oEntries.erase(p_vpFileName);
	
	list<EntryPair>::iterator oIterator = m_oVideoWithAudioEntries.begin();

	while(oIterator != m_oVideoWithAudioEntries.end())
	{
		if(oIterator -> first == vSelectionTempEntry.first)
		{
			if(p_vForceErase == false)
			{
	//			if(EMMessageBox::Instance() -> DisplayDialog(m_opListViewControl -> GetNativeView(), "Do you want to remove the Video-file, and all its clips, belonging to this Audio-file, and all its clips?", "Remove?", EM_DIALOG_ICON_QUESTION | EM_DIALOG_BUTTON_YES_NO) == EM_DIALOG_YES_BUTTON)
	//			{
					eo << "Delete here Video too" << ef;
					//vDeleteThisEntryTo = oIterator -> second;
					m_oVideoWithAudioEntries.erase(oIterator);
	//			}
			}
			else
				m_oVideoWithAudioEntries.erase(oIterator);
			break;
		}
		if(oIterator -> second == vSelectionTempEntry.first)
		{
			if(p_vForceErase == false)
			{
	//			if(EMMessageBox::Instance() -> DisplayDialog(m_opListViewControl -> GetNativeView(), "Do you want to remove the Audio-file, and all its clips, belonging to this Video-file?", "Remove?", EM_DIALOG_ICON_QUESTION | EM_DIALOG_BUTTON_YES_NO) == EM_DIALOG_YES_BUTTON)
	//			{
					eo << "Delete here Audio too" << ef;
					//vDeleteThisEntryTo = oIterator -> first;
					m_oVideoWithAudioEntries.erase(oIterator);
	//			}
			}
			else
				m_oVideoWithAudioEntries.erase(oIterator);
			break;
		}
		oIterator++;
	}
	
	if(vDeleteThisEntryTo != -1)
	{
		DeleteAllClipBelongingToFile(m_oPathsMappedToEntries[vDeleteThisEntryTo].c_str());
		m_opListViewControl -> DeleteItem(m_opListViewControl -> GetItem(m_oPathsMappedToEntries[vDeleteThisEntryTo].c_str()));
		EraseEntry(m_oPathsMappedToEntries[vDeleteThisEntryTo].c_str());
	}

	m_oPathsMappedToEntries.erase(vSelectionTempEntry.first);
//	return &m_vSelectionTempEntry;
}

Pair* MediaPoolView::GetEntry(const char* p_vpFileName)
{
 	if(m_oEntries.count(p_vpFileName))
		return &m_oEntries[p_vpFileName];
	else
	{
		if(m_oRealAudioPathToPath.count(p_vpFileName))
			return GetEntry(m_oRealAudioPathToPath[p_vpFileName].c_str());
		return &m_oNoValueFound;
	}
}

uint32 MediaPoolView::GetIndexID(const char* p_vpFileName)
{
	return m_opListViewControl -> GetItem(p_vpFileName);
}

EMListViewControl* MediaPoolView::GetListViewControl()
{
	return m_opListViewControl;
}

int32 MediaPoolView::GetPairEntry(int32 p_vEntryID)
{
	list<EntryPair>::iterator oIterator = m_oVideoWithAudioEntries.begin();

	while(oIterator != m_oVideoWithAudioEntries.end())
	{
		if(oIterator -> first == p_vEntryID)
			return oIterator -> second;
		if(oIterator -> second == p_vEntryID)
			return oIterator -> first;
		oIterator++;
	}
	return -1;
}

EMView* MediaPoolView::GetView()
{
	return m_opView;
}

void MediaPoolView::Hide()
{
}

void MediaPoolView::Init()
{

	m_opView ->AddChild(m_opListViewControl);
/*#ifdef PLATFORM_WINDOWS
	m_opIDropTarget = EM_new EMWinDropTarget(this, MEDIA_POOL_LIST_VIEW_CONTROL);
//	RegisterDragDrop(static_cast<HWND>(m_opListViewControl -> GetNativeView()), m_opIDropTarget);
#endif*/
	m_opView -> AddListener(this);
//	m_opView -> SetViewColor(EMColor(192, 192, 192, 255));
	m_opListViewControl -> AddListener(this);

	m_opListViewControl -> AddColumn("Name", 100);
	m_opListViewControl -> AddColumn("Instances", 60, EM_ALIGNMENT_CENTER);
	m_opListViewControl -> AddColumn("Length", 100, EM_ALIGNMENT_RIGHT);
	m_opListViewControl -> AddColumn("Size", 75, EM_ALIGNMENT_RIGHT);
	m_opListViewControl -> AddColumn("Sample Rate", 75, EM_ALIGNMENT_RIGHT);
	m_opListViewControl -> AddColumn("Bit Depth", 60, EM_ALIGNMENT_RIGHT);
	m_opListViewControl -> AddColumn("Channels", 60, EM_ALIGNMENT_RIGHT);
	m_opListViewControl -> AddColumn("Format", 75);
	m_opListViewControl -> AddColumn("Frame Rate", 70, EM_ALIGNMENT_RIGHT);
	m_opListViewControl -> AddColumn("Frame Size", 65, EM_ALIGNMENT_RIGHT);
	m_opListViewControl -> AddColumn("FullPath and FileName", 700);

	m_opListViewControl -> SetUniqueIndexColumn(10);
#ifdef PLATFORM_WINDOWS
	EMDragDrop::Instance() -> RegisterTarget(m_opListViewControl, EM_LIST_VIEW_CONTROL);
	dynamic_cast<EMWinDragDrop*>(EMDragDrop::Instance()) -> SetMediaPool(this);
#endif
}

MediaPoolView* MediaPoolView::Instance()
{
	if(m_opInstance == NULL)
		m_opInstance = EM_new MediaPoolView();
	return m_opInstance;
}

void MediaPoolView::LoadComplete()
{
	eo << "*********** Add to media pool here! ************" << ef;
	int32 vEntryIDs[2];
	list<int32>* opPoolEntries = static_cast<list<int32>*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ALL_ENTRY_IDS, NULL, NULL, NULL));
	if(opPoolEntries -> size() == 0)
		return;
	list<int32>::iterator oIterator = opPoolEntries -> begin();
	string oFullPathAndFileName;
	while(oIterator != opPoolEntries -> end())
	{
		EMMediaPoolEntryInfo* opEntryInfo = static_cast<EMMediaPoolEntryInfo*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_INFO, &(*oIterator)));
		if(opEntryInfo -> IsAudio())
		{
			vEntryIDs[0] = *oIterator;
			vEntryIDs[1] = -1;
			oFullPathAndFileName = static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME, &vEntryIDs[0]));
			//oFullPathAndFileName.erase(oFullPathAndFileName.find_last_of('\\'), 1);
			EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_TO_MEDIA_POOL_VIEW, const_cast<char*>(oFullPathAndFileName.c_str()), vEntryIDs);
			MediaPoolView::Instance() -> ChangeInstances(MediaPoolView::Instance() -> GetIndexID(static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME, &vEntryIDs[0]))));
		}
		else
		{
			vEntryIDs[0] = -1;
			vEntryIDs[1] = *oIterator;
			oFullPathAndFileName = static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME, &vEntryIDs[1]));
			//oFullPathAndFileName.erase(oFullPathAndFileName.find_last_of('\\'), 1);
			EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_TO_MEDIA_POOL_VIEW, const_cast<char*>(oFullPathAndFileName.c_str()), vEntryIDs);
			MediaPoolView::Instance() -> ChangeInstances(MediaPoolView::Instance() -> GetIndexID(static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME, &vEntryIDs[1]))));
		}
		oIterator++;
	}
}

bool MediaPoolView::Lock()
{
	return m_opView -> LockWindow();
}

bool MediaPoolView::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
	case EM_DRAW:
		if(p_opSender == m_opView)
		{
//			eo << "EM_DRAW: m_oBackgroundView" << ef;
			m_opView -> SetHighColor(EMColor(192, 192, 192, 255));
			m_opView -> FillRect(m_opView -> Bounds());
		}
		return true;
	case EM_VIEW_BOUNDS_CHANGED: // Most often due to the user resizing the window
		m_opListViewControl -> SetFrame(m_opView -> Bounds());
		m_opView -> Invalidate(m_opView -> Bounds());
		return true;
	case EM_LIST_VIEW_ITEM_LABEL_MAY_CHANGE:
		if(p_opSender == m_opListViewControl)
		{
			eo << "EM_LIST_VIEW_ITEM_LABEL_MAY_CHANGE" << ef;
			//Temporary stored entry in m_oSelectionTempString
//			m_oSelectionTempString = m_opListViewControl -> GetSelectionSubItem(-1);
			return true;
		}
		return false;

	case EM_LIST_VIEW_ITEM_LABEL_CHANGED:
		if(p_opSender == m_opListViewControl)
		{
			eo << "EM_LIST_VIEW_ITEM_LABEL_CHANGED" << ef;
			// First take away the old entry - thats been temporary stored in m_oSelectionTempString - from the map and add the new entry.
//			AddEntry(m_opListViewControl -> GetSelectionSubItem(-1), EraseEntry(m_oSelectionTempString.c_str()));
			return true;
		}
		return false;
	case EM_BEGIN_DRAG:
		if(p_opSender == m_opListViewControl)
		{
			eo << "MediaPool -> EM_BEGIN_DRAG" << ef;
			if(m_opListViewControl -> IsDragEnabled())
			{
		/*		//int vID(1);
				//uint32 vDirectoryID = DIRECTORY_ID_AUDIO_DATA_USED;
				//string oFullPath = *static_cast<string*>(EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_GET_DIRECTORY, &vID, &vDirectoryID));
				//oFullPath = *static_cast<string*>(EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_GET_DIRECTORY, &vID, &vDirectoryID));

				//oFullPath.append("\\");
				//oFullPath.append(m_opListViewControl -> GetSelection(false).c_str());
				//oFullPath.append(".wav");

				pair<uint32, uint32>* oTemp = GetEntry(m_opListViewControl -> GetSelectionSubItem(-1));
		eo << " GetEntry = " << oTemp -> first << " " << oTemp -> second << ef;
				int32 vTemp(oTemp -> first);

				string oFullPath = static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME, &vTemp/*&vEntryIDs[0]*));
		eo << " oFullPath = " << (char*)oFullPath.c_str() << ef;

				if(strcmp("", oFullPath.c_str()) == 0)
					return 0;

#ifdef PLATFORM_WINDOWS		// The part below will be in another class later. A native class. And a non-native class will have an interface to the native class.
				DWORD          dwEffect;
				LPDATAOBJECT	opDataObject;
				HRESULT			hr;
				LPMALLOC		opMalloc(NULL);
				LPSHELLFOLDER	opShellFolder;
				LPITEMIDLIST	opSelectedFileIdList;
				uint32			vStringLength(oFullPath.length());

				hr = SHGetMalloc(&opMalloc);
				if(FAILED(hr))
					return 0;

				opSelectedFileIdList = (LPITEMIDLIST)opMalloc -> Alloc(sizeof(LPITEMIDLIST));
				opDataObject = (LPDATAOBJECT)opMalloc -> Alloc(sizeof(LPDATAOBJECT));
		eo << " ItemIdList and DataObject alloc" << ef;

				hr = SHGetDesktopFolder(&opShellFolder);

				char* test = const_cast<char*>(oFullPath.c_str());

				OLECHAR szOleChar[MAX_PATH];
				MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, test, -1, szOleChar, sizeof(szOleChar));

				opShellFolder -> ParseDisplayName(NULL, NULL, szOleChar, NULL, &opSelectedFileIdList, NULL);
				hr = opShellFolder -> GetUIObjectOf(static_cast<HWND>(m_opListViewControl -> GetNativeView()), 1, (const struct _ITEMIDLIST **)&opSelectedFileIdList, IID_IDataObject, 0, (LPVOID *)&opDataObject);  //Second Parameter: Number of objects to get attributes of
		eo << " DataObject Loaded" << ef;
				if(opSelectedFileIdList)
					opMalloc ->Free(opSelectedFileIdList);
				opShellFolder -> Release();
				opMalloc -> Release();

				if(SUCCEEDED(hr))
				{
					eo << " SUCCEEDED" << ef;
					LPDROPSOURCE opDropSource(new EMWinDropSource());

					FORMATETC sFormat;
					sFormat.cfFormat = CF_HDROP + 5000;
					sFormat.tymed = TYMED_FILE;
					sFormat.lindex = -1;
					sFormat.dwAspect = DVASPECT_CONTENT;
					sFormat.ptd = NULL;
					
					STGMEDIUM sMedium;

					string oTest;
					oTest.append("|");
					oTest.append(m_opListViewControl -> GetSelectionSubItem(-1));
					
					sMedium.tymed = TYMED_FILE;
					sMedium.lpszFileName = (unsigned short*)oTest.c_str();
					sMedium.pUnkForRelease = NULL;

					opDataObject -> SetData(&sFormat, &sMedium, TRUE);
			eo << " Data set in DataObject" << ef;
		
					if(opDropSource)
					{
						uint32 test = DoDragDrop(opDataObject, opDropSource, DROPEFFECT_COPY | DROPEFFECT_MOVE, &dwEffect);
						if(test == DRAGDROP_S_DROP)
						{
							eo << " DRAGDROP_S_DROP" << ef;
							/*char vTemp[100];
							char* vpSubItemString = m_opListViewControl -> GetSelectionSubItem(1);
							int vSubItemNumber(atoi(vpSubItemString) + 1);
							m_opListViewControl -> ChangeSubItem(m_opListViewControl -> GetSelectionID(), 1, itoa(vSubItemNumber, vTemp, 10));
							*
							//ChangeInstances();
					//eo << " SubItem changed" << ef;
						}
						else if(test == DRAGDROP_S_CANCEL)
							eo << " DRAGDROP_S_CANCEL" << ef;
						else if(test == E_OUTOFMEMORY)
							eo << " E_OUTOFMEMORY" << ef;
						else 
							eo << " E_UNSPEC" << ef;
					}
					opDataObject -> Release();
//					delete opDataObject();
					opDropSource -> Release();
#endif
				}
		*/
			string oTest;
			oTest.append("|");
			oTest.append(m_opListViewControl -> GetSelectionSubItem(-1));

			if(m_opDragDropData != NULL)
				delete m_opDragDropData;
			m_opDragDropData = EM_new EMDragDropData;

			m_opDragDropData -> m_opString = oTest;
			EMDragDrop::Instance() -> DoDragDrop(m_opListViewControl, m_opDragDropData, EM_LIST_VIEW_CONTROL);

			}
			return true;
		}
		else
			return false; //Someone else send this message so return false
	case EM_DROP:
		if(p_opSender = m_opListViewControl)
		{
			string oFullPath(EMDragDrop::Instance() -> GetData() -> m_opString);
			bool vFromMediaPool(false);

			int32 vEntryIDs[2];
			//EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_IMPORT_MEDIA, const_cast<char*>(oFullPath.c_str()), vEntryIDs);

			//eo << "MediaPoolView Got IDs: " << vEntryIDs[0] << ", " << vEntryIDs[1] << ef;

			//if(vEntryIDs[0] != -1 || vEntryIDs[1] != -1)
			//{
		//		oFullPath.erase(0, oFullPath.find_first_of('|') + 1);

			
			// FIXME: Put the code in these commands in this class, and have the commands call this class member functions/methods. Fixed??
			if(CheckIfExist(oFullPath.c_str()) == false)
			{
				EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_GET_ENTRIES_FROM_MEDIA_POOL_VIEW, const_cast<char*>(oFullPath.c_str()), vEntryIDs, reinterpret_cast<void*>(false));
				EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_TO_MEDIA_POOL_VIEW, const_cast<char*>(oFullPath.c_str()), vEntryIDs);
			}
			//}
			return true;
		}
		else
			return false;
	case EM_LIST_VIEW_ITEM_DELETED:
		if(p_opSender == m_opListViewControl)
		{
			eo << "EM_LIST_VIEW_ITEM_DELETED" << ef;
			string oText("Do you want to remove ");
			oText.append(m_opListViewControl -> GetSelection(false).c_str());
			oText.append(", and all its clips?");
			if(EMMessageBox::Instance() -> DisplayDialog(m_opListViewControl -> GetNativeView(), oText.c_str(), "Remove?", EM_DIALOG_ICON_QUESTION | EM_DIALOG_BUTTON_YES_NO) == EM_DIALOG_YES_BUTTON)
			{
				DeleteAllClipBelongingToFile(m_opListViewControl -> GetSelectionSubItem(-1));
				EraseEntry(m_opListViewControl -> GetSelectionSubItem(-1));
				return true; // We want to delete the entry from the mediapoolview so return true
			}
			return false; // We do not want to delete the entry from the mediapoolview so return false
		}
		else
			return false; //Someone else send this message so return false
	default:
		return false;
	}
}

void MediaPoolView::Show()
{
/*	if(m_opWindow -> Lock())
	{
		m_opWindow -> Show();
		m_opWindow -> Unlock();
	}
*/
}

