#include "CommandClipsSelectAll.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMFactory.h"
#include "EMGClip.h"
#include "EMGClipRepository.h"
#include "EMGTrack.h"
#include "EMGUIUtilities.h"
#include "EMMessageBox.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "TrackView.h"
#include "TrackWindow.h"

#include <list>

CommandClipsSelectAll::CommandClipsSelectAll(TrackWindow* p_opTrackWindow) :
m_opTrackWindow(p_opTrackWindow)
{
}

void CommandClipsSelectAll::ExecuteE()
{
	if(m_opTrackWindow -> GetTrackInfo() -> GetView() -> LockWindow())
	{
		EMEditMode eEditMode = static_cast<EMEditMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EDIT_MODE))));
		list<EMGClip*> oClipsToBeSelected;
		list<EMGClip*>::iterator oIterator = EMGClipRepository::Instance() -> GetClips() -> begin();
		bool vHiddenClipsExist;
		while(oIterator != EMGClipRepository::Instance() -> GetClips() -> end())
		{
			if(!((*oIterator) -> GetTrack() -> GetTrackInfo() -> IsHidden(eEditMode == EM_CLIP_EDIT_MODE)))
			{
				if((*oIterator) -> GetFirstInGroup() != NULL)
				{
					EMGUIUtilities::Instance() -> GetClipGroup(*oIterator, &vHiddenClipsExist);
					if(vHiddenClipsExist)
					{
						EMMessageBox* opMessageBox = EMFactory::Instance() -> CreateMessageBox();
						opMessageBox -> DisplayDialog(NULL, "Can not select all visible clips since at least\none of the clips is grouped with one or more hidden clips.", "Hidden Clips In Selection", EM_DIALOG_BUTTON_OK);
						m_opTrackWindow -> GetTrackInfo() -> GetView() -> UnlockWindow();
						return;
					}
				}
				oClipsToBeSelected.push_back(*oIterator);
			}

			oIterator++;
		}

		EMCommandRepository::Instance() -> ExecuteCommand(UNDO_COMMAND_CHANGE_CLIP_SELECTION, EMGClipRepository::Instance() -> GetSelectedClips(), NULL, NULL);
		EMGClipRepository::Instance() -> SetSelected(&oClipsToBeSelected, true);
		m_opTrackWindow -> GetTrackView() -> EnableMenuItems();
		m_opTrackWindow -> GetTrackView() -> GetView() -> Invalidate(m_opTrackWindow -> GetTrackView() -> GetView() -> Bounds());

		m_opTrackWindow -> GetTrackInfo() -> GetView() -> UnlockWindow();
	}
}

bool CommandClipsSelectAll::IsUndoable()
{
	return false;
}

bool CommandClipsSelectAll::RequiresParameters()
{
	return false;
}

