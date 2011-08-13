#include "EMWinDropTarget.h"
#include "EMApplication.h"

#ifdef PLATFORM_WINDOWS

#include "CommandIDs.h"
#include "EMGClip.h"
#include "EMGClipContainer.h"
#include "EMGTrack.h"
#include "EMGTrackRepository.h"
#include "EMImageBufferRepository.h"
#include "EMListViewControl.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMWinListViewControl.h"
#include "EMWinView.h"
#include "MediaPoolView.h"

MediaPoolView* EMWinDropTarget::m_opMediaPoolView = NULL;

EMWinDropTarget::EMWinDropTarget(void* p_opTargetClass, Target p_eTarget) :
m_eTarget(p_eTarget),
m_vDrop(true),
m_opListViewControl(NULL),
m_opView(NULL),
m_vRefs(1)
{
	m_vOleInitializeResult = OleInitialize(NULL);
	eo << "Drop Target OleInitialize Result:" << ef;
	switch(m_vOleInitializeResult)
	{
	case S_OK:
		eo << "    S_OK" << ef;
		break;
	case S_FALSE:
		eo << "    S_FALSE" << ef;
		break;
	case OLE_E_WRONGCOMPOBJ:
		eo << "    OLE_E_WRONGCOMPOBJ" << ef;
		break;
	case RPC_E_CHANGED_MODE:
		eo << "    RPC_E_CHANGED_MODE" << ef;
		break;
	case E_INVALIDARG:
		eo << "    E_INVALIDARG" << ef;
		break;
	case E_OUTOFMEMORY:
		eo << "    E_OUTOFMEMORY" << ef;
		break;
	case E_UNEXPECTED:
		eo << "    E_UNEXPECTED" << ef;
		break;
	default:
		eo << "    UnKnown result" << ef;
	}
	
	switch(p_eTarget)
	{
//	case EXPLORER_LIST_VIEW_CONTROL:
	case MEDIA_POOL_LIST_VIEW_CONTROL:
		m_opMediaPoolView = static_cast<MediaPoolView*>(p_opTargetClass);
		m_opListViewControl = dynamic_cast<EMWinListViewControl*>(m_opMediaPoolView -> GetListViewControl());
		RegisterDragDrop((static_cast<HWND>(m_opListViewControl -> GetNativeView())), this);
		break;
	case LIST_VIEW_CONTROL:
		m_opListViewControl = static_cast<EMWinListViewControl*>(p_opTargetClass);
		break;
	case VIEW:
		m_opView = static_cast<EMWinView*>(p_opTargetClass);
		break;
	default:
		EMDebugger("Unknown class for DropTarget");
	}
}

EMWinDropTarget::~EMWinDropTarget()
{
	if(m_eTarget == MEDIA_POOL_LIST_VIEW_CONTROL)
		if(FAILED(RevokeDragDrop(static_cast<HWND>(m_opListViewControl -> GetNativeView()))))
			EMDebugger("Faild to RevokeDragDrop!");

	if(m_vOleInitializeResult == S_OK || m_vOleInitializeResult == S_FALSE ||
	  (m_vOleInitializeResult != OLE_E_WRONGCOMPOBJ && m_vOleInitializeResult != E_INVALIDARG && m_vOleInitializeResult != E_OUTOFMEMORY && m_vOleInitializeResult != E_UNEXPECTED))
	{
		eo << "OleUninitialize will be called" << ef;
		OleUninitialize();
	}
}

STDMETHODIMP EMWinDropTarget::DragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
{
	eo << "DragEnter" << ef;

	FORMATETC sFormat;
	sFormat.cfFormat = CF_HDROP + 5000;
	sFormat.tymed = TYMED_FILE;
	sFormat.lindex = -1;
	sFormat.dwAspect = DVASPECT_CONTENT;
	sFormat.ptd = NULL;

	STGMEDIUM sMedium;
	sMedium.tymed = TYMED_FILE;
	sMedium.lpszFileName = 0;
	sMedium.pUnkForRelease = NULL;

	eo << "Drop" << ef;
	if(pDataObj)
	{
		eo << "Get ready for data" << ef;
		pDataObj -> GetData(&sFormat, &sMedium);
		
		if(sMedium.tymed == 0)
		{
			*pdwEffect = DROPEFFECT_NONE;
			m_vDrop = false;
			eo << "No Drop!!" << ef;
		}
		else if(grfKeyState & MK_CONTROL)	
			*pdwEffect = DROPEFFECT_COPY;  // Allways copy??
		else
			*pdwEffect = DROPEFFECT_MOVE;
	}
	return ResultFromScode(S_OK);
}

STDMETHODIMP EMWinDropTarget::DragLeave()
{
	eo << "DragLeave" << ef;
	m_vDrop = true;
	return ResultFromScode(S_OK);
}

STDMETHODIMP EMWinDropTarget::DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
{
	eo << "DragOver" << ef;
	if(!m_vDrop)
	{
		*pdwEffect = DROPEFFECT_NONE;
		eo << "No Drop!!" << ef;
	}
	else if(grfKeyState & MK_CONTROL)	
		*pdwEffect = DROPEFFECT_COPY;  // Allways copy??
	else
		*pdwEffect = DROPEFFECT_MOVE;

	return ResultFromScode(S_OK);
}

STDMETHODIMP EMWinDropTarget::Drop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
{
	//SetCapture(GetParent();
	SetCursor(LoadCursor(NULL, IDC_WAIT));

	FORMATETC sFormat;
	sFormat.cfFormat = CF_HDROP + 5000;
	sFormat.tymed = TYMED_FILE;
	sFormat.lindex = -1;
	sFormat.dwAspect = DVASPECT_CONTENT;
	sFormat.ptd = NULL;

	STGMEDIUM sMedium;
	sMedium.tymed = TYMED_FILE;
	sMedium.lpszFileName = 0;
	sMedium.pUnkForRelease = NULL;

	eo << "Drop" << ef;
	if(pDataObj)
	{
		eo << "Get ready for data" << ef;
		pDataObj -> GetData(&sFormat, &sMedium);
eo << "EMWinDropTarget::Drop(); " << (char*)sMedium.lpszFileName << ef;
		string oFullPath((char*)sMedium.lpszFileName);
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
			if(m_opMediaPoolView && !m_opListViewControl && !vFromMediaPool)
				m_opListViewControl = dynamic_cast<EMWinListViewControl*>(m_opMediaPoolView -> GetListViewControl());
			if(m_opListViewControl)
			{
				WIN32_FIND_DATA sFileData;
				string oPath((char*)sMedium.lpszFileName);
				oPath.erase(oPath.find_last_of('\\'));
				string oSearchFileName((char*)sMedium.lpszFileName);
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
					oLength = itoa(vLength, vTemp, 10);

					vFileSize = sFileData.nFileSizeLow / 1024;
					oFileSize = itoa(vFileSize, vTemp, 10);
					oFileSize.append(" KB");

					vSamplesRate = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_SAMPLE_RATE, &vEntryIDs[0])));
					oSamplesRate = itoa(vSamplesRate, vTemp, 10);
			
					vBitDepth = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_BIT_DEPTH, &vEntryIDs[0])));
					oBitDepth = itoa(vBitDepth, vTemp, 10);

					vChannels = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_NUM_CHANNELS, &vEntryIDs[0])));
					oChannels = itoa(vChannels, vTemp, 10);

					SHFILEINFO sFileInfo;
					const char* vpPath = oFullPathAndFileName.c_str();

					SHGetFileInfo(vpPath, 0, &sFileInfo, sizeof(sFileInfo), SHGFI_TYPENAME);

			//		oFileType = (static_cast<char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_FORMAT_STRING, &vEntryIDs[0])));
					
					m_opListViewControl -> AddItem(oFileName.c_str());
					m_opListViewControl -> AddSubItem("0", 1);
					m_opListViewControl -> AddSubItem(oLength.c_str(), 2);
					m_opListViewControl -> AddSubItem(oFileSize.c_str(), 3);
					m_opListViewControl -> AddSubItem(oSamplesRate.c_str(), 4);
					m_opListViewControl -> AddSubItem(oBitDepth.c_str(), 5);
					m_opListViewControl -> AddSubItem(oChannels.c_str(), 6);
					m_opListViewControl -> AddSubItem(sFileInfo.szTypeName/*oFileType.c_str()*/, 7);
					m_opListViewControl -> AddSubItem("0", 8);
					m_opListViewControl -> AddSubItem("0", 9);
					m_opListViewControl -> AddSubItem(oFullPathAndFileName.c_str(), 10);
					m_opMediaPoolView -> AddEntry(oFullPathAndFileName.c_str(), vEntryIDs[0], EM_TYPE_ANY_AUDIO);
				}
				if(vEntryIDs[1] != -1)
				{
					oFullPathAndFileName = static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME, &vEntryIDs[1]));  //Full path to copied file
					oFileName.append(oFullPathAndFileName);
					oFileName.erase(0, oFileName.find_last_of('\\') + 1);
					oFileName.erase(oFileName.find_last_of('.'));

					vLength = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_DURATION, &vEntryIDs[1])));
					oLength = itoa(vLength, vTemp, 10);

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
					//string oFileType((static_cast<char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_FORMAT_STRING, &vEntryIDs[1]))));
					
					vFrameRate = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_FRAME_RATE, &vEntryIDs[1])));
					oFrameRate = itoa(vFrameRate, vTemp, 10);
		
//					vFrameSize = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_FRAME_SIZE, &vEntryIDs[1])));
//					oFrameSize = itoa(vFrameSize, vTemp, 10);

					m_opListViewControl -> AddItem(oFileName.c_str());
					m_opListViewControl -> AddSubItem("0", 1);
					m_opListViewControl -> AddSubItem(oLength.c_str(), 2);
					m_opListViewControl -> AddSubItem(oFileSize.c_str(), 3);
					m_opListViewControl -> AddSubItem("0"/*oSamplesRate.c_str()*/, 4);
					m_opListViewControl -> AddSubItem("0"/*oBitDepth.c_str()*/, 5);
					m_opListViewControl -> AddSubItem("0"/*oChannels.c_str()*/, 6);
					m_opListViewControl -> AddSubItem(sFileInfo.szTypeName/*oFileType.c_str()*/, 7);
					m_opListViewControl -> AddSubItem(oFrameRate.c_str(), 8);
					m_opListViewControl -> AddSubItem("0"/*oFrameSize.c_str()*/, 9);
					m_opListViewControl -> AddSubItem(oFullPathAndFileName.c_str(), 10);
					m_opMediaPoolView -> AddEntry(oFullPathAndFileName.c_str(), vEntryIDs[1], EM_TYPE_ANY_VIDEO);
				}

			}
			if(m_opMediaPoolView && m_opListViewControl && !vFromMediaPool)
				m_opListViewControl = NULL;

			if(m_opView)
			{
				int32 vMouseX(0);
				int32 vMouseY(0);
				bool vUnUsed1(false);
				bool vUnUsed2(false);
				bool vUnUsed3(false);

				int32 vVideoTrackIndex = -1;
				int32 vAudioTrackIndex = -1;
				if(m_opView ->LockWindow())
				{
					m_opView -> GetMouse(vMouseX, vMouseY, vUnUsed1, vUnUsed2, vUnUsed3);
					
					EMGTrack* opTrack  = NULL;// = EMGTrackRepository::Instance() -> GetTrack(vMouseX, vMouseY);

					if(vEntryIDs[0] != -1)
					{
						EMImageBufferRepository::Instance() -> AudioFileAdded(vEntryIDs[0]);
						opTrack = EMGTrackRepository::Instance() -> GetClosestTrack(vMouseX, vMouseY, EM_TYPE_ANY_AUDIO);
						if(opTrack == NULL)
						{
							EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_AUDIO_TRACK);
							opTrack = EMGTrackRepository::Instance() -> GetClosestTrack(vMouseX, vMouseY, EM_TYPE_ANY_AUDIO);
						}
						vAudioTrackIndex = EMGTrackRepository::Instance() -> GetTrackIndex(opTrack);
			
						int32 vMediaTrackID = opTrack -> GetTrackInfo() -> GetMediaTrackID();
						int64 vPos((*static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TIME_ZOOM_SCALE)) * vMouseX));

						int32* vpMediaClipID = static_cast<int32*>(EMCommandRepository::Instance() ->ExecuteCommand(MEDIA_COMMAND_DROP_MEDIA, &vEntryIDs[0],  &vMediaTrackID, &vPos));
						EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_G_AUDIO_CLIP, vpMediaClipID, &vAudioTrackIndex, &vPos);
						m_opMediaPoolView -> ChangeInstances(m_opMediaPoolView ->GetIndexID(static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME, &vEntryIDs[0]))));
					}
					if(vEntryIDs[1] != -1)
					{
						EMImageBufferRepository::Instance() -> VideoFileAdded(vEntryIDs[1]);
						opTrack = EMGTrackRepository::Instance() -> GetClosestTrack(vMouseX, vMouseY, EM_TYPE_ANY_VIDEO);
						if(opTrack == NULL)
						{
							EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_VIDEO_TRACK);
							opTrack = EMGTrackRepository::Instance() -> GetClosestTrack(vMouseX, vMouseY, EM_TYPE_ANY_VIDEO);
						}
						vVideoTrackIndex = EMGTrackRepository::Instance() -> GetTrackIndex(opTrack);
			
						int32 vMediaTrackID = opTrack -> GetTrackInfo() -> GetMediaTrackID();
						int64 vPos((*static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TIME_ZOOM_SCALE)) * vMouseX));

						int32* vpMediaClipID = static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_DROP_MEDIA, &vEntryIDs[1],  &vMediaTrackID, &vPos));
						EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_G_VIDEO_CLIP, vpMediaClipID, &vVideoTrackIndex, &vPos);
						m_opMediaPoolView -> ChangeInstances(m_opMediaPoolView ->GetIndexID(static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME, &vEntryIDs[1]))));
					}
					m_opView -> UnlockWindow();
				}
			}
		}		
		if(grfKeyState & MK_CONTROL)	
			*pdwEffect = DROPEFFECT_COPY;  // Allways copy??
		else
			*pdwEffect = DROPEFFECT_MOVE;
	}
//	ReleaseCapture();
	SetCursor(LoadCursor(NULL, IDC_ARROW));

	return ResultFromScode(S_OK);
}
STDMETHODIMP EMWinDropTarget::QueryInterface(REFIID p_vReferensInterfaceID, void FAR* FAR* p_oppObject)
{
	if(p_vReferensInterfaceID == IID_IUnknown || p_vReferensInterfaceID == IID_IDropSource)
	{
		*p_oppObject = this;
		++m_vRefs;
		return NOERROR;
	}
	*p_oppObject = NULL;

	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) EMWinDropTarget::AddRef()
{
	return ++m_vRefs;
}
STDMETHODIMP_(ULONG) EMWinDropTarget::Release()
{
	if(--m_vRefs == 0)
	{
		delete this;
		return 0;
	}
	return m_vRefs;

}

#endif