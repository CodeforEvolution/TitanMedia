#include "FileDialogTarget.h"

#include "CommandIDs.h"
#include "EMApplication.h"
#include "EMCommandRepository.h"
#include "EMFactory.h"
#include "EMFileWindow.h"
#include "EMGTrack.h"
#include "EMGTrackInfo.h"
#include "EMGTrackRepository.h"
#include "EMImageBufferRepository.h"
#include "EMMessageBox.h"
#include "EMMessages.h"
#include "EMResourceRepository.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMWindow.h"
#include "ResourceIDs.h"
#include "TrackWindow.h"

#include <string>
#include <vector>

// BEGIN HACK BY JOHAN
#ifdef PLATFORM_WINDOWS
#include <windows.h>
#endif
// END HACK BY JOHAN

FileDialogTarget::FileDialogTarget(TrackWindow* p_opTrackWindow) :
m_opTrackWindow(p_opTrackWindow)
{
}

bool FileDialogTarget::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
/*
	case EM_LOAD_MEDIA_REQUESTED:
		{
			;//cout_commented_out_4_release << "Now calling media command import media" << endl;
			int32 vpIDs[2];	//Prepare array. position 0 will hold eventual audio entry id, and position 1 will hold any eventual video entry id
			int32 vCount = 2;
			EMFileWindow* opFileWindow = dynamic_cast<EMFileWindow*>(p_opSender);
			vector<string> oPaths = *(opFileWindow -> GetPaths());
eo << (char*) oPaths.front().c_str() << ef;
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_IMPORT_MEDIA, const_cast<char*>(oPaths[0].c_str()), vpIDs, &vCount);

			int32 vVideoTrackIndex = -1;
			int32 vAudioTrackIndex = -1;
			uint32 vIndex = 0;
			EMGTrack* opTrack;
			while((vVideoTrackIndex == -1 || vAudioTrackIndex == -1) && vIndex < EMGTrackRepository::Instance() -> GetNrTracks())
			{
				opTrack = EMGTrackRepository::Instance() -> GetTrack(vIndex);
				if(opTrack -> GetTrackInfo() -> GetType() == EM_TYPE_ANY_VIDEO && vVideoTrackIndex == -1)
					vVideoTrackIndex = vIndex;
				if(opTrack -> GetTrackInfo() -> GetType() == EM_TYPE_ANY_AUDIO && vAudioTrackIndex == -1)
					vAudioTrackIndex = vIndex;
				vIndex++;
			}
		
			if(vpIDs[0] != -1 && vAudioTrackIndex != -1)
			{	//If there was an audio entry added
				EMImageBufferRepository::Instance() -> AudioFileAdded(vpIDs[0]);
				int64 vAtTime = 0;
				int32 vMediaTrackID = EMGTrackRepository::Instance() -> GetTrack(vAudioTrackIndex) -> GetTrackInfo() -> GetMediaTrackID();
				int32* vpMediaClipID = static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_DROP_MEDIA, &(vpIDs[0]), &vMediaTrackID, &vAtTime));
				if(m_opTrackWindow -> GetWindow() -> Lock())
				{
					EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_G_AUDIO_CLIP, vpMediaClipID, &vAudioTrackIndex, &vAtTime);
					m_opTrackWindow -> GetWindow() -> Unlock();
				}
			}
			
			if(vpIDs[1] != -1 && vVideoTrackIndex != -1)
			{	//If there was a video entry added
				EMImageBufferRepository::Instance() -> VideoFileAdded(vpIDs[1]);
				int64 vAtTime = 0;
				int32 vMediaTrackID = EMGTrackRepository::Instance() -> GetTrack(vVideoTrackIndex) -> GetTrackInfo() -> GetMediaTrackID();
				int32* vpMediaClipID = static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_DROP_MEDIA, &(vpIDs[1]), &vMediaTrackID, &vAtTime));
				if(m_opTrackWindow -> GetWindow() -> Lock())
				{
					EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_G_VIDEO_CLIP, vpMediaClipID, &vVideoTrackIndex, &vAtTime);
					m_opTrackWindow -> GetWindow() -> Unlock();
				}
			}
			return true;
		}
	case EM_CREATE_PROJECT_REQUESTED:
		{
			if(m_opTrackWindow -> GetWindow() -> Lock())
			{
;//cout_commented_out_4_release << "EM_CREATE_PROJECT_REQUESTED: locked window" << endl;
				// Make sure that no unlocks of the window thread takes place within this block
				EMFileWindow* opFileWindow = dynamic_cast<EMFileWindow*>(p_opSender);
				vector<string> oPaths = *(opFileWindow -> GetPaths());
				string oPath = oPaths[0];
				if(EMSettingsRepository::Instance() -> SettingExists(SETTING_PROJECT_ID))
				{
					int vProjectID = *(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_PROJECT_ID)));
					// Since this version of Titan only supports one concurrent project,
					// we first close the existing project before creating the new project
					EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_CLOSE_PROJECT, &vProjectID);
				}
				EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_CREATE_PROJECT, &oPath);
				string vProjectName = oPath.substr(oPath.find_last_of('/') + 1, oPath.length() - 1);
				string vWindowTitle = string(*(EMResourceRepository::GetString(RES_SR_APPLICATION_NAME))) + " - " + vProjectName;
				m_opTrackWindow -> GetWindow() -> SetTitle(vWindowTitle.c_str());
				m_opTrackWindow -> GetWindow() -> Unlock();
;//cout_commented_out_4_release << "EM_CREATE_PROJECT_REQUESTED: unlocked window" << endl;
			}
			EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_READY_FOR_USE);
			return true;
		}
*/
	case EM_LOAD_PROJECT_REQUESTED:
		{
			// Ask the user if the current project should be saved or not. Maybe even cancel open project!
			if(EMSettingsRepository::Instance() -> SettingExists(SETTING_PROJECT_ID)) // If a project is open
			{
				int vProjectID = *(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_PROJECT_ID)));

				bool vSaveNeeded = reinterpret_cast<bool>(EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_SAVE_NEEDED, NULL, NULL, NULL));
				if(vSaveNeeded)
				{
					switch(EMFactory::Instance() -> CreateMessageBox() -> DisplayDialog(NULL, "Do you want to save changes?", "Save Changes?", EM_DIALOG_ICON_QUESTION | EM_DIALOG_BUTTON_YES_NO_CANCEL))
					{
					case EM_DIALOG_YES_BUTTON:
						{
							EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_SAVE_PROJECT, &vProjectID);
							break;
						}
					case EM_DIALOG_NO_BUTTON:
						break;
					case EM_DIALOG_CANCEL_BUTTON:
						return true;
					}
				}

				EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_CLOSE_PROJECT, &vProjectID);
			}

// Should already be closed here!			
			// Close the current project if it exists (it should exist)
/*			if(EMSettingsRepository::Instance() -> SettingExists(SETTING_PROJECT_ID))
			{
				uint32 vProjectID = *(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_PROJECT_ID)));
				EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_CLOSE_PROJECT, &vProjectID);
			}
*/
			if(m_opTrackWindow -> GetWindow() -> Lock())
			{
				EMFileWindow* opFileWindow = dynamic_cast<EMFileWindow*>(p_opSender);
				vector<string> oPaths = *(opFileWindow -> GetPaths());
				string oPath = oPaths[0];
				uint32 vProjectID = reinterpret_cast<uint32>(EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_LOAD_PROJECT, const_cast<char*>(oPath.c_str())));
				if(vProjectID > 0)
				{
					int vNameBeginPos = oPath.find_last_of(EM_FILE_SEPARATOR, oPath.find_last_of(EM_FILE_SEPARATOR) - 1) + 1;
					int vNameEndPos = oPath.find_last_of(EM_FILE_SEPARATOR) - 1;
					string oProjectName = oPath.substr(vNameBeginPos, vNameEndPos - vNameBeginPos + 1);
					string oWindowTitle = string(EMResourceRepository::GetString(RES_SR_APPLICATION_NAME) -> c_str()) + " " + EMApplication::GetApplicationVersionNumber() +
													" - " + oProjectName;
					m_opTrackWindow -> GetWindow() -> SetTitle(oWindowTitle.c_str());

					EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_CLEAR_UNDO_STACK);		
					EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_SET_UNDO_TEXTS);				

					m_opTrackWindow -> GetWindow() -> Unlock();
				}
				else
				{
					m_opTrackWindow -> GetWindow() -> Unlock();
					EMFactory::Instance() -> CreateMessageBox() -> DisplayDialog(NULL, "Could not load the specified project file.\nPerhaps the file is in an incorrect format or is damaged.", "Load Project Failed", EM_DIALOG_BUTTON_OK | EM_DIALOG_ICON_ERROR);
					EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DISPLAY_NEW_PROJECT_DIALOG);
				}
			}

			EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_READY_FOR_USE);

			return true;
		}
/*
	case EM_SAVE_PROJECT_REQUESTED:
		{
			EMFileWindow* opFileWindow = dynamic_cast<EMFileWindow*>(p_opSender);
			vector<string> oPaths = *(opFileWindow -> GetPaths());
			string oPath = oPaths[0];
			EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_SAVE_PROJECT, &oPath);
			return true;
		}
*/
	}
	return false;
}



