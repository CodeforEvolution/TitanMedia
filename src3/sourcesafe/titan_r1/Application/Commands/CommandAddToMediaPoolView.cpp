#include "CommandAddToMediaPoolView.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGUIUtilities.h"
#include "EMListViewControl.h"
#include "EMMediaGlobals.h"
#include "EMView.h"

#include "MediaPoolView.h"

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#endif

CommandAddToMediaPoolView::CommandAddToMediaPoolView()
{
}

void* CommandAddToMediaPoolView::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
	string oPath(static_cast<char*>(p_opParameterOne));
	int32* vpEntryIDs;
	vpEntryIDs = static_cast<int32*>(p_opParameterTwo);
/*
	vpEntryIDs[0] = -1;
	vpEntryIDs[1] = -1;

	pair<uint32, uint32>* oTemp = MediaPoolView::Instance() -> GetEntry(oPath.c_str());
	if(oTemp -> second == EM_TYPE_ANY_AUDIO)
	{
		vpEntryIDs[0] = oTemp -> first;
		vpEntryIDs[1] = -1;
	}
	else if(oTemp -> second == EM_TYPE_ANY_VIDEO)
	{
		vpEntryIDs[0] = MediaPoolView::Instance() -> GetPairEntry(oTemp -> first);
		vpEntryIDs[1] = oTemp -> first;
	}
	else
	{
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_IMPORT_MEDIA, const_cast<char*>(oPath.c_str()), vpEntryIDs);
*/	if(vpEntryIDs[0] != -1 && vpEntryIDs[1] != -1)
		MediaPoolView::Instance() -> AddEntryPair(vpEntryIDs);		
	if(vpEntryIDs[0] != -1 || vpEntryIDs[1] != -1)
	{
	//	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_IMPORT_MEDIA, const_cast<char*>(oPath.c_str()), vpEntryIDs);
		EMListViewControl* opListViewTarget = MediaPoolView::Instance() -> GetListViewControl();

		WIN32_FIND_DATA sFileData;
	//	string oPath(m_opData -> m_opString);
		string oSearchFileName(oPath.c_str());
		oPath.erase(oPath.find_last_of('\\'));
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

		if(vpEntryIDs[0] != -1)
		{
			oFullPathAndFileName = static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME, &vpEntryIDs[0]));  //Full path to copied file
			oFileName.append(oFullPathAndFileName);
			oFileName.erase(0, oFileName.find_last_of('\\') + 1);
			oFileName.erase(oFileName.find_last_of('.'));

			vLength = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_DURATION, &vpEntryIDs[0])));
			oLength = EMGUIUtilities::Instance() -> GetTimeString(vLength, EM_SMPTE);
			//oLength = itoa(vLength, vTemp, 10);

			vFileSize = sFileData.nFileSizeLow / 1024;
			oFileSize = itoa(vFileSize, vTemp, 10);
			oFileSize.append(" KB");

			vSamplesRate = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_SAMPLE_RATE, &vpEntryIDs[0])));
			oSamplesRate = itoa(vSamplesRate, vTemp, 10);

			vBitDepth = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_BIT_DEPTH, &vpEntryIDs[0])));
			oBitDepth = itoa(vBitDepth, vTemp, 10);

			vChannels = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_NUM_CHANNELS, &vpEntryIDs[0])));
			oChannels = itoa(vChannels, vTemp, 10);

	//		SHFILEINFO sFileInfo;
	//		const char* vpPath = oFullPathAndFileName.c_str();

	//		SHGetFileInfo(vpPath, 0, &sFileInfo, sizeof(sFileInfo), SHGFI_TYPENAME);

			oFileType = (static_cast<char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_FORMAT_STRING, &vpEntryIDs[0])));
			
			opListViewTarget -> AddItem(oFileName.c_str());
			opListViewTarget -> AddSubItem("0", 1);
			opListViewTarget -> AddSubItem(oLength.c_str(), 2);
			opListViewTarget -> AddSubItem(oFileSize.c_str(), 3);
			opListViewTarget -> AddSubItem(oSamplesRate.c_str(), 4);
			opListViewTarget -> AddSubItem(oBitDepth.c_str(), 5);
			opListViewTarget -> AddSubItem(oChannels.c_str(), 6);
			opListViewTarget -> AddSubItem(/*sFileInfo.szTypeName*/oFileType.c_str(), 7);
			opListViewTarget -> AddSubItem("-", 8);
			opListViewTarget -> AddSubItem("-", 9);
			opListViewTarget -> AddSubItem(oFullPathAndFileName.c_str(), 10);
			MediaPoolView::Instance() -> AddEntry(oFullPathAndFileName.c_str(), vpEntryIDs[0], EM_TYPE_ANY_AUDIO, (oPath + EM_FILE_SEPARATOR + oSearchFileName).c_str());
		}
		if(vpEntryIDs[1] != -1)
		{
			oFullPathAndFileName = static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME, &vpEntryIDs[1]));  //Full path to copied file
			oFileName.append(oFullPathAndFileName);
			oFileName.erase(0, oFileName.find_last_of('\\') + 1);
			oFileName.erase(oFileName.find_last_of('.'));

			vLength = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_DURATION, &vpEntryIDs[1])));
			oLength = EMGUIUtilities::Instance() -> GetTimeString(vLength, EM_SMPTE);
			//oLength = itoa(vLength, vTemp, 10);

			vFileSize = sFileData.nFileSizeLow / 1024;
			oFileSize = itoa(vFileSize, vTemp, 10);
			oFileSize.append(" KB");

			//vSamplesRate = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_SAMPLE_RATE, &vpEntryIDs[0])));
			//oSamplesRate = itoa(vSamplesRate, vTemp, 10);

			//vBitDepth = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_BIT_DEPTH, &vpEntryIDs[1])));
			//oBitDepth = itoa(vBitDepth, vTemp, 10);

			//vChannels = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_NUM_CHANNELS, &vpEntryIDs[1])));
			//oChannels = itoa(vChannels, vTemp, 10);

			SHFILEINFO sFileInfo;
			const char* vpPath = oFullPathAndFileName.c_str();

			SHGetFileInfo(vpPath, 0, &sFileInfo, sizeof(sFileInfo), SHGFI_TYPENAME);
			//oFileType = (static_cast<char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_FORMAT_STRING, &vpEntryIDs[1])));
			
			vFrameRate = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_FRAME_RATE, &vpEntryIDs[1])));
			oFrameRate = itoa(vFrameRate, vTemp, 10);

	//		vFrameSize = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_FRAME_SIZE, &vpEntryIDs[1])));
	//		oFrameSize = itoa(vFrameSize, vTemp, 10);

			opListViewTarget -> AddItem(oFileName.c_str());
			opListViewTarget -> AddSubItem("0", 1);
			opListViewTarget -> AddSubItem(oLength.c_str(), 2);
			opListViewTarget -> AddSubItem(oFileSize.c_str(), 3);
			opListViewTarget -> AddSubItem("0"/*oSamplesRate.c_str()*/, 4);
			opListViewTarget -> AddSubItem("0"/*oBitDepth.c_str()*/, 5);
			opListViewTarget -> AddSubItem("0"/*oChannels.c_str()*/, 6);
			opListViewTarget -> AddSubItem(sFileInfo.szTypeName/*oFileType.c_str()*/, 7);
			opListViewTarget -> AddSubItem(oFrameRate.c_str(), 8);
			opListViewTarget -> AddSubItem("0"/*oFrameSize.c_str()*/, 9);
			opListViewTarget -> AddSubItem(oFullPathAndFileName.c_str(), 10);
			MediaPoolView::Instance() -> AddEntry(oFullPathAndFileName.c_str(), vpEntryIDs[1], EM_TYPE_ANY_VIDEO);
		}
//		}
	}
	return NULL;
}

bool CommandAddToMediaPoolView::IsUndoable()
{
	return false;
}

bool CommandAddToMediaPoolView::RequiresParameters()
{
	return true;
}
