#include "EMWinDragDrop.h"

#ifdef PLATFORM_WINDOWS

#include "EMGUIComponent.h"
#include "EMWinWindow.h"
#include "EMWinView.h"
#include "EMWinListViewControl.h"
#include "EMCommandRepository.h"

#include "CommandIDs.h"
#include "EMGClip.h"
#include "EMGClipContainer.h"
#include "EMGClipRepository.h"
#include "EMGTrack.h"
#include "EMGTrackRepository.h"
#include "EMGUIUtilities.h"
#include "EMImageBufferRepository.h"
#include "EMKeyboardGlobals.h"
#include "EMListViewControl.h"
#include "EMMessages.h"
#include "EMMouseCursor.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMWinListViewControl.h"
#include "MediaPoolView.h"


//#include <windows.h>
#include <process.h>

//EMWinDragDrop* EMWinDragDrop::m_opInstance = NULL;

EMWinDragDrop::EMWinDragDrop() :
m_opListViewSource(NULL),
m_opListViewTarget(NULL),
m_opViewTarget(NULL),
m_vNotDropped(true),
m_vDragging(false),
m_vDragOverSource(true),
m_vDragOverTarget(true),
m_vIsActive(false),
m_vIsOverSource(false),
m_vIsOverTarget(true),
m_opData(NULL)
{

}

EMWinDragDrop::~EMWinDragDrop()
{
//		SetWindowLong(reinterpret_cast<HWND>(m_opListViewSource -> GetNativeView()), GWL_WNDPROC, reinterpret_cast<LONG>(m_opDefaultWindowProcedure));
//		SetWindowLong(reinterpret_cast<HWND>(m_opListViewTarget -> GetNativeView()), GWL_WNDPROC, reinterpret_cast<LONG>(m_opDefaultWindowProcedure));
}

EMDragDropData* EMWinDragDrop::GetData()
{
	return m_opData;
}

EMDragDropResult EMWinDragDrop::DoDragDrop(void* p_opSource, EMDragDropData* p_opData, EMDragDropClassType p_eSourceClassType)
{
	m_vIsActive = true;
	m_opData = p_opData;
	m_eSourceClassType = p_eSourceClassType;
	if(p_eSourceClassType == EM_LIST_VIEW_CONTROL)
	{
		m_opListViewSource = static_cast<EMWinListViewControl*>(p_opSource);
//		SetCapture(reinterpret_cast<HWND>(m_opListViewSource ->GetNativeView()));
		m_opDefaultWindowProcedure = reinterpret_cast<WNDPROC>(GetWindowLong(reinterpret_cast<HWND>(m_opListViewSource ->GetNativeView()), GWL_WNDPROC));
		m_opMainDefaultWindowProcedure = reinterpret_cast<WNDPROC>(GetWindowLong(m_opListViewSource -> GetWindowWindow() -> GetMainWindowHandle(), GWL_WNDPROC));
		SetWindowLong(reinterpret_cast<HWND>(m_opListViewSource -> GetNativeView()), GWL_WNDPROC, reinterpret_cast<LONG>(SubclassedWindowProcedure));
		SetWindowLong(m_opListViewSource -> GetWindowWindow() -> GetMainWindowHandle(), GWL_WNDPROC, reinterpret_cast<LONG>(SubclassedWindowProcedure));
	}

	list<DragDropPair>::iterator oClassIterator = m_oClassType.begin();

	while(oClassIterator != m_oClassType.end())
	{
		if(oClassIterator -> second == EM_LIST_VIEW_CONTROL)
		{
			m_opListViewTarget = static_cast<EMWinListViewControl*>(oClassIterator -> first);
			if(m_opListViewTarget == m_opListViewSource)
			{
	//			m_opListViewTarget = NULL;
				oClassIterator++;
				continue;
			}
			m_opListViewDefaultWindowProcedure = reinterpret_cast<WNDPROC>(GetWindowLong(reinterpret_cast<HWND>(m_opListViewTarget -> GetNativeView()), GWL_WNDPROC));
			SetWindowLong(reinterpret_cast<HWND>(m_opListViewTarget -> GetNativeView()), GWL_WNDPROC, reinterpret_cast<LONG>(SubclassedWindowProcedure));
		}
		if(oClassIterator -> second == EM_VIEW)
		{
			m_opViewTarget = static_cast<EMWinView*>(oClassIterator -> first);
			m_opViewDefaultWindowProcedure = reinterpret_cast<WNDPROC>(GetWindowLong(reinterpret_cast<HWND>(m_opViewTarget -> GetNativeView()), GWL_WNDPROC));
			SetWindowLong(reinterpret_cast<HWND>(m_opViewTarget -> GetNativeView()), GWL_WNDPROC, reinterpret_cast<LONG>(SubclassedWindowProcedure));
		}
		oClassIterator++;
	}
	m_vDragging = true;

	return EM_DRAG_DROP_OK;
}

EMDragDropResult EMWinDragDrop::Drop(EMMouseOver p_eMouseOver, EMWinListViewControl* p_opListViewTarget, EMWinView* p_opView)
{

//	ReleaseCapture();
	SetCursor(LoadCursor(NULL, IDC_WAIT));

	m_vNotDropped = false;
	Reset();

	switch(p_eMouseOver)
	{
	case EM_SOURCE:
		eo << "Drop at Source" << ef;
		break;
	case EM_TARGET:
		{
			if(m_opData)
			{
				if(p_opListViewTarget)
					p_opListViewTarget -> Notify(EM_DROP);
				if(p_opView)
				{
					bool vUnUsed1(false);
					bool vUnUsed2(false);
					bool vUnUsed3(false);
					if(p_opView -> LockWindow())
					{
						p_opView -> GetMouse(m_opData -> m_vMouseX, m_opData -> m_vMouseY, vUnUsed1, vUnUsed2, vUnUsed3);
						p_opView -> UnlockWindow();
					}
					p_opView -> Notify(EM_DROP);
				}
			}
			else
				EMDebugger("No data to drop!");
/*			EMWinListViewControl* opListViewTarget = p_opListViewTarget;
			EMWinView* opView = p_opView;
			eo << "Drop at Target" << ef;
			if(m_opData)
			{
				eo << "Get ready for data" << ef;
				if(opListViewTarget != m_opMediaPoolView -> GetListViewControl() && opView == NULL)
				{
					opListViewTarget -> Notify(EM_DROP);
					break;
				}
				int32 vMouseX(0);
				int32 vMouseY(0);
				if(opView)
				{
					bool vUnUsed1(false);
					bool vUnUsed2(false);
					bool vUnUsed3(false);
					if(opView -> LockWindow())
					{
						opView -> GetMouse(vMouseX, vMouseY, vUnUsed1, vUnUsed2, vUnUsed3);
						opView -> UnlockWindow();
					}
						
				}

		//		pDataObj -> GetData(&sFormat, &sMedium);
		//eo << "EMWinDropTarget::Drop(); " << (char*)sMedium.lpszFileName << ef;
				//string oFullPath((char*)sMedium.lpszFileName);
				string oFullPath(m_opData -> m_opString);
				bool vFromMediaPool(false);

				int32 vEntryIDs[2];
				if(oFullPath.find_first_of('|') == -1)
					EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_IMPORT_MEDIA, const_cast<char*>(oFullPath.c_str()), vEntryIDs);
				else
				{
					vFromMediaPool = true;
					oFullPath.erase(0, oFullPath.find_first_of('|') + 1);
					pair<uint32, uint32>* oTemp = m_opMediaPoolView -> GetEntry(oFullPath.c_str());
					if(oTemp -> second == EM_TYPE_ANY_AUDIO)
					{
						vEntryIDs[0] = oTemp -> first;
						vEntryIDs[1] = -1;
					}
					else
					{
						vEntryIDs[0] = -1;
						vEntryIDs[1] = oTemp -> first;
					}
				}
				eo << "Got IDs: " << vEntryIDs[0] << ", " << vEntryIDs[1] << ef;

				if(vEntryIDs[0] != -1 || vEntryIDs[1] != -1)
				{
					if(m_opMediaPoolView && !opListViewTarget && !vFromMediaPool)
						opListViewTarget = dynamic_cast<EMWinListViewControl*>(m_opMediaPoolView -> GetListViewControl());
					if(opListViewTarget)
					{
						m_opData -> m_vEntryIDs[0] = vEntryIDs[0];
						m_opData -> m_vEntryIDs[1] = vEntryIDs[1];
						opListViewTarget -> Notify(EM_DROP);
						//if(opListViewTarget -> Notify(EM_DROP) == false)
							//EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_TO_MEDIA_POOL_VIEW, const_cast<char*>(m_opData -> m_opString.c_str()), vEntryIDs);
/*							WIN32_FIND_DATA sFileData;
						string oPath(m_opData -> m_opString);
						oPath.erase(oPath.find_last_of('\\'));
						string oSearchFileName(m_opData -> m_opString);
						oSearchFileName.erase(0, oSearchFileName.find_last_of('\\') + 1);

						SetCurrentDirectory(oPath.c_str());
						HANDLE hHandle(FindFirstFile(oSearchFileName.c_str(), &sFileData));
						//cout << test.cFileName << endl;
						FindClose(hHandle);

						string oFileName;
						string oFullPathAndFileName;
						int64 vLength;
						string oLength;
						int32 vFileSize;
						string oFileSize;
						int64 vSamplesRate;
						string oSamplesRate;
						int64 vBitDepth;
						string oBitDepth;
						int64 vChannels;
						string oChannels;
						string oFileType;
						int64 vFrameRate;
						string oFrameRate;
			//			int64 vFrameSize;
			//			string oFrameSize;

						char vTemp[255];

						if(vEntryIDs[0] != -1)
						{
							oFullPathAndFileName = static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME, &vEntryIDs[0]));  //Full path to copied file
							oFileName.append(oFullPathAndFileName);
							oFileName.erase(0, oFileName.find_last_of('\\') + 1);
							oFileName.erase(oFileName.find_last_of('.'));

							vLength = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_DURATION, &vEntryIDs[0])));
							oLength = EMGUIUtilities::Instance() -> GetTimeString(vLength, EM_SMPTE);
							//oLength = itoa(vLength, vTemp, 10);

							vFileSize = sFileData.nFileSizeLow / 1024;
							oFileSize = itoa(vFileSize, vTemp, 10);
							oFileSize.append(" KB");

							vSamplesRate = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_SAMPLE_RATE, &vEntryIDs[0])));
							oSamplesRate = itoa(vSamplesRate, vTemp, 10);
					
							vBitDepth = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_BIT_DEPTH, &vEntryIDs[0])));
							oBitDepth = itoa(vBitDepth, vTemp, 10);

							vChannels = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_NUM_CHANNELS, &vEntryIDs[0])));
							oChannels = itoa(vChannels, vTemp, 10);

					//		SHFILEINFO sFileInfo;
					//		const char* vpPath = oFullPathAndFileName.c_str();

					//		SHGetFileInfo(vpPath, 0, &sFileInfo, sizeof(sFileInfo), SHGFI_TYPENAME);

							oFileType = (static_cast<char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_FORMAT_STRING, &vEntryIDs[0])));
							
							opListViewTarget -> AddItem(oFileName.c_str());
							opListViewTarget -> AddSubItem("0", 1);
							opListViewTarget -> AddSubItem(oLength.c_str(), 2);
							opListViewTarget -> AddSubItem(oFileSize.c_str(), 3);
							opListViewTarget -> AddSubItem(oSamplesRate.c_str(), 4);
							opListViewTarget -> AddSubItem(oBitDepth.c_str(), 5);
							opListViewTarget -> AddSubItem(oChannels.c_str(), 6);
							opListViewTarget -> AddSubItem(/*sFileInfo.szTypeName*oFileType.c_str(), 7);
							opListViewTarget -> AddSubItem("0", 8);
							opListViewTarget -> AddSubItem("0", 9);
							opListViewTarget -> AddSubItem(oFullPathAndFileName.c_str(), 10);
							m_opMediaPoolView -> AddEntry(oFullPathAndFileName.c_str(), vEntryIDs[0], EM_TYPE_ANY_AUDIO);
						}
						if(vEntryIDs[1] != -1)
						{
							oFullPathAndFileName = static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME, &vEntryIDs[1]));  //Full path to copied file
							oFileName.append(oFullPathAndFileName);
							oFileName.erase(0, oFileName.find_last_of('\\') + 1);
							oFileName.erase(oFileName.find_last_of('.'));

							vLength = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_DURATION, &vEntryIDs[1])));
							oLength = EMGUIUtilities::Instance() -> GetTimeString(vLength, EM_SMPTE);
							//oLength = itoa(vLength, vTemp, 10);

							vFileSize = sFileData.nFileSizeLow / 1024;
							oFileSize = itoa(vFileSize, vTemp, 10);
							oFileSize.append(" KB");

							//vSamplesRate = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_SAMPLE_RATE, &vEntryIDs[0])));
							//oSamplesRate = itoa(vSamplesRate, vTemp, 10);
					
							//vBitDepth = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_BIT_DEPTH, &vEntryIDs[1])));
							//oBitDepth = itoa(vBitDepth, vTemp, 10);

							//vChannels = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_NUM_CHANNELS, &vEntryIDs[1])));
							//oChannels = itoa(vChannels, vTemp, 10);

							SHFILEINFO sFileInfo;
							const char* vpPath = oFullPathAndFileName.c_str();

							SHGetFileInfo(vpPath, 0, &sFileInfo, sizeof(sFileInfo), SHGFI_TYPENAME);
							//oFileType = (static_cast<char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_FORMAT_STRING, &vEntryIDs[1])));
							
							vFrameRate = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_FRAME_RATE, &vEntryIDs[1])));
							oFrameRate = itoa(vFrameRate, vTemp, 10);
				
	//						vFrameSize = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_FRAME_SIZE, &vEntryIDs[1])));
	//						oFrameSize = itoa(vFrameSize, vTemp, 10);

							opListViewTarget -> AddItem(oFileName.c_str());
							opListViewTarget -> AddSubItem("0", 1);
							opListViewTarget -> AddSubItem(oLength.c_str(), 2);
							opListViewTarget -> AddSubItem(oFileSize.c_str(), 3);
							opListViewTarget -> AddSubItem("0"/*oSamplesRate.c_str()*, 4);
							opListViewTarget -> AddSubItem("0"/*oBitDepth.c_str()*, 5);
							opListViewTarget -> AddSubItem("0"/*oChannels.c_str()*, 6);
							opListViewTarget -> AddSubItem(sFileInfo.szTypeName/*oFileType.c_str(), 7);
							opListViewTarget -> AddSubItem(oFrameRate.c_str(), 8);
							opListViewTarget -> AddSubItem("0"/*oFrameSize.c_str()*, 9);
							opListViewTarget -> AddSubItem(oFullPathAndFileName.c_str(), 10);
							m_opMediaPoolView -> AddEntry(oFullPathAndFileName.c_str(), vEntryIDs[1], EM_TYPE_ANY_VIDEO);
						}
*
					}
					if(opView)
					{
/*						float vMouseX(0);
						float vMouseY(0);
						bool vUnUsed1(false);
						bool vUnUsed2(false);
						bool vUnUsed3(false);
*
						int32 vAudioTrackIndex = -1;
						int32 vVideoTrackIndex = -1;
						
						if(opView ->LockWindow())
						{
//								m_opViewTarget -> GetMouse(vMouseX, vMouseY, vUnUsed1, vUnUsed2, vUnUsed3);
							
							EMGTrack* opAudioTrack  = NULL;
							EMGTrack* opVideoTrack  = NULL;
							int32 vAudioMediaClipID = NULL;
							int32 vVideoMediaClipID = NULL;

							if(vEntryIDs[0] != -1)
							{
								EMImageBufferRepository::Instance() -> AudioFileAdded(vEntryIDs[0]);
								opAudioTrack = EMGTrackRepository::Instance() -> GetClosestTrack(vMouseX, vMouseY, EM_TYPE_ANY_AUDIO, 0, 0);
								if(opAudioTrack == NULL)
								{
									EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_AUDIO_TRACK);
									opAudioTrack = EMGTrackRepository::Instance() -> GetClosestTrack(vMouseX, vMouseY, EM_TYPE_ANY_AUDIO, 0, 0);
								}
								vAudioTrackIndex = EMGTrackRepository::Instance() -> GetTrackIndex(opAudioTrack);
					
								int32 vAudioMediaTrackID = opAudioTrack -> GetTrackInfo() -> GetMediaTrackID();
								int64 vPos((*static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TIME_ZOOM_SCALE)) * vMouseX));

								vAudioMediaClipID = *static_cast<int32*>(EMCommandRepository::Instance() ->ExecuteCommand(MEDIA_COMMAND_DROP_MEDIA, &vEntryIDs[0],  &vAudioMediaTrackID, &vPos));
								EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_G_AUDIO_CLIP, &vAudioMediaClipID, &vAudioTrackIndex, &vPos);
								m_opMediaPoolView -> ChangeInstances(m_opMediaPoolView ->GetIndexID(static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME, &vEntryIDs[0]))));
							}
							if(vEntryIDs[1] != -1)
							{
								int64 vPos((*static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TIME_ZOOM_SCALE)) * vMouseX));
								int64 vLength(*(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_DURATION, &vEntryIDs[1]))));
								EMImageBufferRepository::Instance() -> VideoFileAdded(vEntryIDs[1]);
								opVideoTrack = EMGTrackRepository::Instance() -> GetClosestTrack(vMouseX, vMouseY, EM_TYPE_ANY_VIDEO, vPos, vLength);
								if(opVideoTrack == NULL)
								{
									eo << "No track found or not enough space to drop clip. First time" << ef;
									EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_VIDEO_TRACK);
									opVideoTrack = EMGTrackRepository::Instance() -> GetClosestTrack(vMouseX, vMouseY, EM_TYPE_ANY_VIDEO, vPos, vLength);
								}
								vVideoTrackIndex = EMGTrackRepository::Instance() -> GetTrackIndex(opVideoTrack);
					
								int32 vVideoMediaTrackID = opVideoTrack -> GetTrackInfo() -> GetMediaTrackID();

								vVideoMediaClipID = *static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_DROP_MEDIA, &vEntryIDs[1],  &vVideoMediaTrackID, &vPos));
								if(EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_G_VIDEO_CLIP, &vVideoMediaClipID, &vVideoTrackIndex, &vPos) == NULL)
								{
									EMDebugger("How did you get here? Better do something about it.");
								/*	eo << "No track found or not enough space to drop clip. Second time" << ef;
									EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_VIDEO_TRACK);
									opVideoTrack = EMGTrackRepository::Instance() -> GetClosestTrack(vMouseX, vMouseY, EM_TYPE_ANY_VIDEO, vPos, vLength);
									vVideoTrackIndex = EMGTrackRepository::Instance() -> GetTrackIndex(opVideoTrack);
									EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_G_VIDEO_CLIP, &vVideoMediaClipID, &vVideoTrackIndex, &vPos);
									*
								}
								m_opMediaPoolView -> ChangeInstances(m_opMediaPoolView -> GetIndexID(static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME, &vEntryIDs[1]))));
							}
							if(opAudioTrack && opVideoTrack)
							{
								list<EMGClip*> oClipsToGroup;
								oClipsToGroup.push_back(EMGClipRepository::Instance() -> GetClip(vAudioMediaClipID));
								oClipsToGroup.push_back(EMGClipRepository::Instance() -> GetClip(vVideoMediaClipID));
								EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_GROUP_CLIPS_FROM_LIST, &oClipsToGroup, NULL, NULL);
							}
							opView -> UnlockWindow();
						}
					}
				}		
//					if(grfKeyState & MK_CONTROL)	
//						*pdwEffect = DROPEFFECT_COPY;  // Allways copy??
//					else
//						*pdwEffect = DROPEFFECT_MOVE;
			}
			break;
*/		}

	case EM_SOMEWHERE_ELSE:
		eo << "Drop Somewhere Else" << ef;
		break;
	}
//	m_vNotDropped = false;
//	Reset();

	m_vIsActive = false;

	reinterpret_cast<EMWinView*>(GetWindowLong(GetParent(static_cast<HWND>(m_opListViewSource -> GetNativeView())), GWL_USERDATA)) -> SetDefaultCursor(EM_MOUSE_CURSOR_ARROW);
	reinterpret_cast<EMWinView*>(GetWindowLong(GetParent(static_cast<HWND>(m_opListViewSource -> GetNativeView())), GWL_USERDATA)) -> SetMouseCursor(EM_MOUSE_CURSOR_ARROW, false);
	//SetCursor(LoadCursor(NULL, IDC_ARROW));
	return EM_DRAG_DROP_OK;
}

EMDragDropResult EMWinDragDrop::DragEnter(EMMouseOver p_eMouseOver)
{
	DWORD vFlags(0);
	DWORD vX(0);
	DWORD vY(0);
	DWORD vData(0);
	DWORD vExtra(0);

	if(GetKeyState(VK_LBUTTON) < 0) 
		;//eo << "Mouse button down" << ef;
	else
	{
		//eo << "Mouse button up" << ef;
		return EM_DRAG_DROP_FAILED;
	}

	switch(p_eMouseOver)
	{
	case EM_SOURCE:
		eo << "Enter Source" << ef;
		//SetCursor(LoadCursor(NULL, IDC_NO));
//		reinterpret_cast<EMWinView*>(GetWindowLong(GetParent(static_cast<HWND>(m_opListViewSource -> GetNativeView())), GWL_USERDATA)) -> SetMouseCursor(EM_MOUSE_CURSOR_DROP_NOT_TARGET);
		reinterpret_cast<EMWinView*>(GetWindowLong(GetParent(static_cast<HWND>(m_opListViewSource -> GetNativeView())), GWL_USERDATA)) -> SetDefaultCursor(EM_MOUSE_CURSOR_DROP_NOT_TARGET);
		m_vIsOverSource = true;
		m_vIsOverTarget = false;
		break;
	case EM_TARGET:
		eo << "Enter Target" << ef;
		if(m_opListViewTarget)
		{
		//	eo << "Enter ListView" << ef;
			//SetCursor(LoadCursor(NULL, IDC_APPSTARTING));
			//reinterpret_cast<EMWinView*>(GetWindowLong(GetParent(static_cast<HWND>(m_opListViewSource -> GetNativeView())), GWL_USERDATA)) -> SetDefaultCursor(EM_MOUSE_CURSOR_ARROW);
			//reinterpret_cast<EMWinView*>(GetWindowLong(GetParent(static_cast<HWND>(m_opListViewTarget -> GetNativeView())), GWL_USERDATA)) -> SetMouseCursor(EM_MOUSE_CURSOR_DROP_TARGET);
			reinterpret_cast<EMWinView*>(GetWindowLong(GetParent(static_cast<HWND>(m_opListViewTarget -> GetNativeView())), GWL_USERDATA)) -> SetDefaultCursor(EM_MOUSE_CURSOR_DROP_TARGET);
			m_opListViewTarget -> Notify(EM_DRAG_ENTER);
		}
		if(m_opViewTarget)
		{
		//	eo << "Enter View" << ef;
			//reinterpret_cast<EMWinView*>(GetWindowLong(GetParent(static_cast<HWND>(m_opListViewSource -> GetNativeView())), GWL_USERDATA)) -> SetDefaultCursor(EM_MOUSE_CURSOR_ARROW);
			//m_opViewTarget -> SetMouseCursor(EM_MOUSE_CURSOR_DROP_TARGET);
			m_opViewTarget -> SetDefaultCursor(EM_MOUSE_CURSOR_DROP_TARGET);
			m_opViewTarget -> Notify(EM_DRAG_ENTER);
		}
		m_vIsOverSource = false;
		m_vIsOverTarget = true;
		break;
	case EM_SOMEWHERE_ELSE:
//		reinterpret_cast<EMWinView*>(GetWindowLong(GetParent(static_cast<HWND>(m_opListViewSource -> GetNativeView())), GWL_USERDATA)) -> SetMouseCursor(EM_MOUSE_CURSOR_DROP_NOT_TARGET);
		//reinterpret_cast<EMWinView*>(GetWindowLong(GetParent(static_cast<HWND>(m_opListViewSource -> GetNativeView())), GWL_USERDATA)) -> SetDefaultCursor(EM_MOUSE_CURSOR_DROP_NOT_TARGET);
//		SetCursor(LoadCursor(NULL, IDC_NO));
		break;
	}
	return EM_DRAG_DROP_OK;
}

EMDragDropResult EMWinDragDrop::DragLeave()
{
	eo << "DragLeave" << ef;
//	SetCursor(LoadCursor(NULL, IDC_NO));
 	reinterpret_cast<EMWinView*>(GetWindowLong(GetParent(static_cast<HWND>(m_opListViewSource -> GetNativeView())), GWL_USERDATA)) -> SetDefaultCursor(EM_MOUSE_CURSOR_DROP_NOT_TARGET);
	m_vIsOverSource = false;
	m_vIsOverTarget = false;
	if(m_opListViewTarget)
		m_opListViewTarget -> Notify(EM_DRAG_LEAVE);
	if(m_opViewTarget)
		m_opViewTarget -> Notify(EM_DRAG_LEAVE);

	return EM_DRAG_DROP_OK;
}

EMDragDropResult EMWinDragDrop::DragMove(EMMouseOver p_eMouseOver)
{
	if(GetKeyState(VK_LBUTTON) < 0) 
		;//eo << "Mouse button down" << ef;
	else
	{
		//eo << "Mouse button up" << ef;
		return EM_DRAG_DROP_FAILED;
	}
	
	switch(p_eMouseOver)
	{
	case EM_SOURCE:
		//eo << "DragMove -> EM_SOURCE" << ef;
	//	SetCursor(LoadCursor(NULL, IDC_NO));
//		reinterpret_cast<EMWinView*>(GetWindowLong(GetParent(static_cast<HWND>(m_opListViewSource -> GetNativeView())), GWL_USERDATA)) -> SetMouseCursor(EM_MOUSE_CURSOR_DROP_NOT_TARGET, true);
		break;
	case EM_TARGET:
		//eo << "DragMove -> EM_TARGET" << ef;
//		SetCursor(LoadCursor(NULL, IDC_APPSTARTING));
		if(m_opListViewTarget)
		{
		//	eo << "In ListView" << ef;
			//SetCursor(LoadCursor(NULL, IDC_APPSTARTING));
//			reinterpret_cast<EMWinView*>(GetWindowLong(GetParent(static_cast<HWND>(m_opListViewTarget -> GetNativeView())), GWL_USERDATA)) -> SetMouseCursor(EM_MOUSE_CURSOR_DROP_TARGET, true);
		}
		if(m_opViewTarget)
		{
		//	eo << "In View" << ef;
			//m_opViewTarget -> SetMouseCursor(EM_MOUSE_CURSOR_DROP_TARGET);
			//m_opViewTarget -> SetDefaultCursor(EM_MOUSE_CURSOR_DROP_TARGET);
			//SetCursor(LoadCursor(NULL, IDC_APPSTARTING));
		}

		break;
	case EM_SOMEWHERE_ELSE:
		//eo << "DragMove -> EM_SOMEWHERE_ELSE" << ef;
		//SetCursor(LoadCursor(NULL, IDC_NO));
		break;
	}
	return EM_DRAG_DROP_OK;
}

/*EMDragDrop* EMWinDragDrop::Instance()
{
	if(m_opInstance == NULL)
		m_opInstance = EM_new EMWinDragDrop;
	return m_opInstance;
}
*/

bool EMWinDragDrop::IsActive()
{
	return m_vIsActive;
}

void EMWinDragDrop::ReassignSubProc()
{
//	eo << "ReassignSubProc" << ef;
	if(m_vDragging)
	{
		SetWindowLong(reinterpret_cast<HWND>(m_opListViewSource -> GetNativeView()), GWL_WNDPROC, reinterpret_cast<LONG>(SubclassedWindowProcedure));
		if(m_eTargetClassType == EM_LIST_VIEW_CONTROL)
			if(m_opListViewTarget != NULL)
				SetWindowLong(reinterpret_cast<HWND>(m_opListViewTarget -> GetNativeView()), GWL_WNDPROC, reinterpret_cast<LONG>(SubclassedWindowProcedure));
		if(m_eTargetClassType == EM_VIEW)
			SetWindowLong(reinterpret_cast<HWND>(m_opViewTarget -> GetNativeView()), GWL_WNDPROC, reinterpret_cast<LONG>(SubclassedWindowProcedure));
		SetWindowLong(m_opListViewSource -> GetWindowWindow() -> GetMainWindowHandle(), GWL_WNDPROC, reinterpret_cast<LONG>(SubclassedWindowProcedure));
		
		list<DragDropPair>::iterator oClassIterator = m_oClassType.begin();

		while(oClassIterator != m_oClassType.end())
		{
			if(oClassIterator -> second == EM_LIST_VIEW_CONTROL)
			{
				m_opListViewTarget = static_cast<EMWinListViewControl*>(oClassIterator -> first);
				SetWindowLong(reinterpret_cast<HWND>(m_opListViewTarget -> GetNativeView()), GWL_WNDPROC, reinterpret_cast<LONG>(SubclassedWindowProcedure));
			}
			if(oClassIterator -> second == EM_VIEW)
			{
				m_opViewTarget = static_cast<EMWinView*>(oClassIterator -> first);
				m_opViewDefaultWindowProcedure = reinterpret_cast<WNDPROC>(GetWindowLong(reinterpret_cast<HWND>(m_opViewTarget -> GetNativeView()), GWL_WNDPROC));
				SetWindowLong(reinterpret_cast<HWND>(m_opViewTarget -> GetNativeView()), GWL_WNDPROC, reinterpret_cast<LONG>(SubclassedWindowProcedure));
			}
			oClassIterator++;
		}
	}
}

EMDragDropResult EMWinDragDrop::RegisterTarget(void* p_opTarget, EMDragDropClassType p_eTargetClassType)
{
	DragDropPair oTemp(p_opTarget, p_eTargetClassType);
	m_oClassType.push_back(oTemp);
/*	m_eTargetClassType = p_eTargetClassType;
	if(p_eTargetClassType == EM_LIST_VIEW_CONTROL)
	{
		m_opListViewTarget = static_cast<EMWinListViewControl*>(p_opTarget);
		//SetWindowLong(reinterpret_cast<HWND>(m_opListViewTarget -> GetNativeView()), GWL_WNDPROC, reinterpret_cast<LONG>(SubclassedWindowProcedure));
	}
	if(p_eTargetClassType == EM_VIEW)
	{
		m_opViewTarget = static_cast<EMWinView*>(p_opTarget);
		//SetWindowLong(reinterpret_cast<HWND>(m_opViewTarget -> GetNativeView()), GWL_WNDPROC, reinterpret_cast<LONG>(SubclassedWindowProcedure));
	}
*/
	return EM_DRAG_DROP_OK;
}

void EMWinDragDrop::Reset()
{
//	eo << "Reset ListView Source Window Proc." << ef;
		SetWindowLong(reinterpret_cast<HWND>(m_opListViewSource -> GetNativeView()), GWL_WNDPROC, reinterpret_cast<LONG>(m_opDefaultWindowProcedure));

		list<DragDropPair>::iterator oClassIterator = m_oClassType.begin();

		while(oClassIterator != m_oClassType.end())
		{
			if(oClassIterator -> second == EM_LIST_VIEW_CONTROL)
			{
				if(oClassIterator -> first != m_opListViewSource)
				{
//					eo << "Reset ListView Target Window Proc." << ef;
					SetWindowLong(reinterpret_cast<HWND>(static_cast<EMWinListViewControl*>(oClassIterator -> first) -> GetNativeView()), GWL_WNDPROC, reinterpret_cast<LONG>(m_opListViewDefaultWindowProcedure));
				}
			}
			if(oClassIterator -> second == EM_VIEW)
			{
//				eo << "Reset View Target Window Proc." << ef;
				SetWindowLong(reinterpret_cast<HWND>(static_cast<EMWinView*>(oClassIterator -> first) -> GetNativeView()), GWL_WNDPROC, reinterpret_cast<LONG>(m_opViewDefaultWindowProcedure));
			}
			oClassIterator++;
		}

/*		if(m_eTargetClassType == EM_LIST_VIEW_CONTROL)
			SetWindowLong(reinterpret_cast<HWND>(m_opListViewTarget -> GetNativeView()), GWL_WNDPROC, reinterpret_cast<LONG>(m_opDefaultWindowProcedure));
		if(m_eTargetClassType == EM_VIEW)
			SetWindowLong(reinterpret_cast<HWND>(m_opViewTarget -> GetNativeView()), GWL_WNDPROC, reinterpret_cast<LONG>(m_opDefaultWindowProcedure));
*/		SetWindowLong(m_opListViewSource -> GetWindowWindow() -> GetMainWindowHandle(), GWL_WNDPROC, reinterpret_cast<LONG>(m_opMainDefaultWindowProcedure));
}

void EMWinDragDrop::SetMediaPool(MediaPoolView* p_opMediaPoolView)
{
	m_opMediaPoolView = p_opMediaPoolView;
}


LRESULT CALLBACK EMWinDragDrop::SubclassedWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo)
{
//eo << "MouseDrag Button subclassing" << ef;
	EMWinListViewControl* opListView = NULL;
	EMWinView* opView = NULL;

	EMWinDragDrop* oThis = dynamic_cast<EMWinDragDrop*>(m_opInstance);	

	EMGUIComponent* opComponent = reinterpret_cast<EMWinListViewControl*>(GetWindowLong(p_vWindowHandle, GWL_USERDATA));
	if(dynamic_cast<EMWinListViewControl*>(opComponent))
	{
		opListView = dynamic_cast<EMWinListViewControl*>(opComponent);
		oThis -> m_opListViewTarget = opListView;
		oThis -> m_opViewTarget = NULL;
	}
	if(dynamic_cast<EMWinView*>(opComponent))
	{
		opView = dynamic_cast<EMWinView*>(opComponent);
		oThis -> m_opViewTarget = opView;
		oThis -> m_opListViewTarget = NULL;
	}
		//opView = reinterpret_cast<EMWinView*>(GetWindowLong(p_vWindowHandle, GWL_USERDATA));

//	eo << (int)opListView << " " << p_vMessage << ef;

	if(opListView == oThis -> m_opListViewSource && opListView != NULL)
	{
//		 eo << "opListView == Source" << ef; 
		switch(p_vMessage)
		{
		case WM_LBUTTONDOWN:
//			eo << (int)opListView << "ListView Source: WM_LBUTTONDOWN" << ef;
			oThis -> Reset();
			oThis -> ReassignSubProc();
			oThis -> m_vIsOverSource = true;
			return true;
		case WM_MOUSEMOVE:
//			eo << (int)opListView << "ListView Source: EM_MOUSEMOVE" << ef;
			oThis -> m_vDragging = false;
			oThis -> m_vDragOverSource = true;
			if(oThis -> m_vIsOverSource == true)
				oThis -> DragMove(EM_SOURCE);
			else
				if(oThis -> DragEnter(EM_SOURCE) == EM_DRAG_DROP_FAILED)
					oThis -> Drop(EM_SOMEWHERE_ELSE, NULL, NULL);
			return CallWindowProc(oThis -> m_opDefaultWindowProcedure, p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
			//break;
		case WM_LBUTTONUP:
		//case WM_NCHITTEST:
//			eo << (int)opListView << "ListView Source: WM_LBUTTONUP" << ef;
			oThis -> Drop(EM_SOURCE, NULL, NULL);
			return true;
		//case WM_KEYDOWN:
		case WM_CHAR:
		//case EM_GUI_COMPONENT_ESC_PRESSED:
			eo << "DragDrop: ListView source -> WM_CHAR" << ef;
			if(static_cast<int>(p_vParameterOne) == EM_ESCAPE)
				oThis -> Drop(EM_SOMEWHERE_ELSE, NULL, NULL);
			return true;
		default:
			return CallWindowProc(oThis -> m_opDefaultWindowProcedure, p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
		}
	}
	else if(opListView == oThis -> m_opListViewTarget && opListView != NULL)
	{
//		eo << "opListView == Target" << ef;
		switch(p_vMessage)
		{
		case WM_MOUSEMOVE:
//		eo << (int)opListView << "ListView Target: WM_MOUSEMOVE" << ef;
			oThis -> m_vDragOverTarget = true;
			if(oThis -> m_vIsOverTarget == true)
				oThis -> DragMove(EM_TARGET);
			else
				if(oThis -> DragEnter(EM_TARGET) == EM_DRAG_DROP_FAILED)
					oThis -> Drop(EM_SOMEWHERE_ELSE, NULL, NULL);
			return CallWindowProc(oThis -> m_opListViewDefaultWindowProcedure, p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
			//break;
		case WM_LBUTTONUP:
		//case WM_NCHITTEST:
//			eo << (int)opListView << "ListView Target: WM_LBUTTONUP" << ef;
			oThis -> Drop(EM_TARGET, opListView, NULL);
			return true;
/*		case WM_KEYDOWN:
		case WM_CHAR:
			eo << "DragDrop: WM_KEYDOWN || WM_CHAR" << ef;
			oThis -> Drop(EM_SOMEWHERE_ELSE, NULL, NULL);
			return true;
*/		default:
			return CallWindowProc(oThis -> m_opListViewDefaultWindowProcedure, p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
		}
	}
	else if(opView == oThis -> m_opViewTarget && opView != NULL)
	{
//		eo << "opView == Target" << ef;
		switch(p_vMessage)
		{
		case WM_MOUSEMOVE:
//		eo << (int)opView << "View Target: WM_MOUSEMOVE" << ef;
//		eo << "View Targer: WM_MOUSEMOVE -> KeyPressed = " << p_vParameterOne << ef;
			oThis -> m_vDragOverTarget = true;
			if(oThis -> m_vIsOverTarget == true)
				oThis -> DragMove(EM_TARGET);
			else
				if(oThis -> DragEnter(EM_TARGET) == EM_DRAG_DROP_FAILED)
					oThis -> Drop(EM_SOMEWHERE_ELSE, NULL, NULL);
			return CallWindowProc(oThis -> m_opViewDefaultWindowProcedure, p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
			//break;
		case WM_LBUTTONUP:
		//case WM_NCHITTEST:
//			eo << (int)opView << "View Target: WM_LBUTTONUP" << ef;
			oThis -> Drop(EM_TARGET, NULL, opView);
			return true;
/*		case WM_KEYDOWN:
		case WM_CHAR:
			eo << "DragDrop: WM_KEYDOWN || WM_CHAR" << ef;
			oThis -> Drop(EM_SOMEWHERE_ELSE, NULL, NULL);
			return true;
*/
		default:
			return CallWindowProc(oThis -> m_opViewDefaultWindowProcedure, p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
		}
	}
	else
	{
		switch(p_vMessage)
		{
		case WM_SETCURSOR:
			{
	//			eo << (int)opListView << "DragDrop: WM_SETCURSOR" << ef;
				int32 vHitMessage = LOWORD(p_vParameterTwo);
				int32 vMessage = HIWORD(p_vParameterTwo);
//				eo << "WM_SETCURSOR -> " << vHitMessage << ef;
				switch(vMessage)
				{
				case WM_MOUSEMOVE:
//					eo << (int)opListView << "DragDrop: WM_SETCURSOR -> WM_MOUSEMOVE" << ef;
					if(oThis -> m_vDragOverSource == false && oThis -> m_vDragOverTarget == false)
					{
						if(oThis -> m_vIsOverSource == true || oThis -> m_vIsOverTarget == true)
							oThis -> DragLeave();
						//oThis -> DragMove(EM_SOMEWHERE_ELSE);
					}
					oThis -> m_vDragOverSource = false;
					oThis -> m_vDragOverTarget = false;
					if(oThis -> DragMove(EM_SOMEWHERE_ELSE) == EM_DRAG_DROP_FAILED)
						oThis -> Drop(EM_SOMEWHERE_ELSE, NULL, NULL);
	//				if(vHitMessage != HTERROR)
	//				{
		//				eo << "Button Not pressed" << ef;
						//oThis -> Reset();
	//				}
	//				else
		//				eo << "Button Pressed" << ef;
				//return true;
				break;
				case WM_LBUTTONUP:
//					eo << (int)opListView << "DragDrop: WM_SETCURSOR -> WM_BUTTONUP" << ef;
					if(oThis -> m_vIsOverSource == true)
					{
						eo << "Scould not get here! Or?\n@EMWinDragDrop::SubclassedWindowProcedure\nWM_SETCURSOR -> WM_LBUTTONUP\nIsOverSource == true" << ef;
					//	oThis -> Drop(EM_SOURCE, NULL, NULL);
					}
					else if(oThis -> m_vIsOverTarget == true)
					{
						eo << "Scould not get here! Or?\n@EMWinDragDrop::SubclassedWindowProcedure\nWM_SETCURSOR -> WM_LBUTTONUP\nIsOverTarget == true" << ef;
					//	oThis -> Drop(EM_TARGET, NULL, NULL);
					}
					else
						oThis -> Drop(EM_SOMEWHERE_ELSE, NULL, NULL);
					return false;
/*				case WM_KEYDOWN:
				case WM_CHAR:
					eo << "DragDrop: WM_KEYDOWN || WM_CHAR" << ef;
					oThis -> Drop(EM_SOMEWHERE_ELSE, NULL, NULL);
					return true;
*/
//				default:
//					return false;
				}
			}
		}
		return CallWindowProc(oThis -> m_opMainDefaultWindowProcedure, p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
	}
	//FIXME: Does not working correct. Only works some times
	/*if(p_vMessage == WM_KEYDOWN || p_vMessage == WM_CHAR)
	{
		eo << "DragDrop: WM_KEYDOWN || WM_CHAR" << ef;
		oThis -> Drop(EM_SOMEWHERE_ELSE, NULL, NULL);
	}*/
	return CallWindowProc(oThis -> m_opDefaultWindowProcedure, p_vWindowHandle, p_vMessage, p_vParameterOne, p_vParameterTwo);
}

#endif

/* 
eo << "WM_CAPTURECHANGED" << WM_CAPTURECHANGED << ef;
eo << "WM_LBUTTONDBLCLK" << WM_LBUTTONDBLCLK << ef;
eo << "WM_LBUTTONDOWN" << WM_LBUTTONDOWN << ef;
eo << "WM_LBUTTONUP" << WM_LBUTTONUP << ef;
eo << "WM_MBUTTONDBLCLK" << WM_MBUTTONDBLCLK << ef;
eo << "WM_MBUTTONDOWN" << WM_MBUTTONDOWN << ef;  
eo << "WM_MBUTTONUP" << WM_MBUTTONUP << ef;
eo << "WM_MOUSEACTIVATE" << WM_MOUSEACTIVATE << ef;
eo << "WM_MOUSEMOVE" << WM_MOUSEMOVE << ef;
//eo << "WM_MOUSEWHEEL" << WM_MOUSEWHEEL << ef;
eo << "WM_NCHITTEST" << WM_NCHITTEST << ef;
eo << "WM_NCLBUTTONDBLCLK" << WM_NCLBUTTONDBLCLK << ef;
eo << "WM_NCLBUTTONDOWN" << WM_NCLBUTTONDOWN << ef;
eo << "WM_NCLBUTTONUP" << WM_NCLBUTTONUP << ef;
eo << "WM_NCMBUTTONDBLCLK" << WM_NCMBUTTONDBLCLK << ef;
eo << "WM_NCMBUTTONDOWN" << WM_NCMBUTTONDOWN << ef;
eo << "WM_NCMBUTTONUP" << WM_NCMBUTTONUP << ef;
eo << "WM_NCMOUSEMOVE" << WM_NCMOUSEMOVE << ef;
eo << "WM_NCRBUTTONDBLCLK" << WM_NCRBUTTONDBLCLK << ef;
eo << "WM_NCRBUTTONDOWN" << WM_NCRBUTTONDOWN << ef;
eo << "WM_NCRBUTTONUP" << WM_NCRBUTTONUP << ef;
eo << "WM_RBUTTONDBLCLK" << WM_RBUTTONDBLCLK << ef;
eo << "WM_RBUTTONDOWN" << WM_RBUTTONDOWN << ef;
eo << "WM_RBUTTONUP" << WM_RBUTTONUP << ef;
*/

/*
//EMView.h
static void DragDropCursorActive(bool p_vActive);

static bool m_vDragDropActive;

//EMView.cpp
bool EMView::m_oDragDropActive = false;

void EMView::DragDropCursorActive(bool p_vActive)
{
	m_oDragDropActive = p_vActive;
}
*/