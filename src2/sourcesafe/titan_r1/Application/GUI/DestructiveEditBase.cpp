#include "DestructiveEditBase.h"

#include "AudioGClip.h"
#include "AudioGClipContainer.h"
#include "CommandIDs.h"
#include "EMCommand.h"
#include "EMCommandRepository.h"
#include "EMGClip.h"
#include "EMGClipContainer.h"
#include "EMGClipRepository.h"
#include "EMGTrackRepository.h"
#include "EMGUIUtilities.h"
#include "EMRect.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "MediaPoolView.h"
#include "SettingIDs.h"
#include "TrackView.h"
#include "TrackWindow.h"

DestructiveEditBase::DestructiveEditBase(TrackView* p_opTrackView) :
m_opTrackView(p_opTrackView)
{
}

DestructiveEditBase::DestructiveEditBase(TrackView* p_opTrackView, list<AudioGClip*> p_oAffectedClips, list<AudioGClip*> p_oNewClips, list<EMCommand*> p_oUndoCommands) :
m_oAffectedClips(p_oAffectedClips),
m_oNewClips(p_oNewClips),
m_opTrackView(p_opTrackView),
m_oUndoCommands(p_oUndoCommands)
{
}

void DestructiveEditBase::DestructiveEdit(uint32 p_vDestructiveEditMediaCommandID, int32 p_vEffectEntryID, bool vFillSpaceWithClonedClips, int64 p_vDialogParameter)
{
	EMEditMode eEditMode = static_cast<EMEditMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EDIT_MODE))));

	if(eEditMode == EM_FX_EDIT_MODE)
		return;

	if(m_opTrackView -> GetView() -> LockWindow())
	{		
		int64 vSelectionBeginPos;
		int64 vSelectionEndPos;

		// Calculate affected (original) clips and the selection area (íf there's no selection, the entire clips are affected)
		if(eEditMode == EM_TRACK_EDIT_MODE)
		{
			vSelectionBeginPos = *(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_REGION_BEGIN)));
			vSelectionEndPos = *(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_REGION_END)));
			/*
			if(vSelectionBeginPos == -1 || vSelectionEndPos == -1)
			{
				// No use to crop a clip to the same size as before
				m_opTrackView -> GetView() -> UnlockWindow();
				return;
			}
			*/
			list<EMGClip*>* opSelectedClips = EMGClipRepository::Instance() -> GetSelectedClips();
			list<EMGClip*>::iterator oIterator = opSelectedClips -> begin();
			while(oIterator != opSelectedClips -> end())
			{
				if((*oIterator) -> GetType() == EM_TYPE_ANY_AUDIO)
					m_oAffectedClips.push_back(dynamic_cast<AudioGClip*>(*oIterator));

				oIterator++;
			}
		}
		else
		{
			EMGClipContainer* opContainer = EMGClipContainer::GetClipContainerWithSelection();

			if(opContainer == NULL)
			{
				m_opTrackView -> GetView() -> UnlockWindow();
				return;
			}

			AudioGClipContainer* opAudioContainer;

			if(!(opAudioContainer = dynamic_cast<AudioGClipContainer*>(opContainer)))
			{
				m_opTrackView -> GetView() -> UnlockWindow();
				return;
			}

			m_oAffectedClips = *(opAudioContainer -> GetSelectedClips(&vSelectionBeginPos, &vSelectionEndPos));
		}

		int64 vTimeZoomScale = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TIME_ZOOM_SCALE)));
		EMRect oInvalidateRect;
		list<AudioGClip*>::iterator oIterator = m_oAffectedClips.begin();
		int32 vOriginalMediaClipID;
		int32 vNewMediaClipID;
		int32 vNewMediaClipTwoID;
		int32 vNewMediaClipThreeID;
		int64 vClipBeginPos;
		int64 vClipEndPos;
		int64 vAffectedClipBeginPos;
		int64 vAffectedClipEndPos;
		uint32 vTrackIndex;
		AudioGClip* opNewClipOne = NULL;
		AudioGClip* opNewClipTwo = NULL;
		AudioGClip* opNewClipThree = NULL;
		list<EMGClip*> oClipsToClone;
		int64 vClonePositionOffset;
		int32 vCloneTrackOffset(0);
		int64 vClipMarkIn;
		int64 vClipMarkOut;
		int64 vNewClipMarkIn;
		int64 vNewClipMarkOut;
		bool vClipIsSelected;
		const char* vpClipFileName;
		int32* vpClipEntryID;
		uint32 vClipCounter(0);

		while(oIterator != m_oAffectedClips.end())
		{
			oInvalidateRect = oInvalidateRect & (*oIterator) -> Frame();

			vOriginalMediaClipID = (*oIterator) -> GetMediaClipID();
			vClipMarkIn = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_MARK_IN, &vOriginalMediaClipID)));
			vClipMarkOut = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_MARK_OUT, &vOriginalMediaClipID)));
			vClipBeginPos = (*oIterator) -> GetPosition();
			vClipEndPos = (*oIterator) -> GetPosition() + (*oIterator) -> GetLength() - 1;
			vClipIsSelected = (*oIterator) -> GetSelected();

			if(eEditMode == EM_TRACK_EDIT_MODE && (vSelectionBeginPos == -1 || vSelectionEndPos == -1))
			{
				vAffectedClipBeginPos = vClipBeginPos;
				vAffectedClipEndPos = vClipEndPos;
			}
			else
			{
				if(vSelectionBeginPos < vClipBeginPos)
					vAffectedClipBeginPos = vClipBeginPos;
				else
					vAffectedClipBeginPos = vSelectionBeginPos;
				if(vSelectionEndPos > vClipEndPos)
					vAffectedClipEndPos = vClipEndPos;
				else
					vAffectedClipEndPos = vSelectionEndPos;
			}

			int64 vpParameters[4];
			vpParameters[0] = vSelectionBeginPos;//vAffectedClipBeginPos;
			vpParameters[1] = vSelectionEndPos;//vAffectedClipEndPos;
			vpParameters[2] = p_vDialogParameter;
			vpParameters[3] = vClipCounter;

			vNewMediaClipID = *(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommandNoUndo(p_vDestructiveEditMediaCommandID, &vOriginalMediaClipID, &p_vEffectEntryID, vpParameters)));
			m_oUndoCommands.push_back(EMCommandRepository::Instance() -> GetUndoClone(p_vDestructiveEditMediaCommandID));

			vTrackIndex = EMGTrackRepository::Instance() -> GetTrackIndex((*oIterator) -> GetTrack());

			// Each original clip can be affected in four different ways

			if(vSelectionBeginPos == -1 || vSelectionEndPos == -1 || (vSelectionBeginPos <= vClipBeginPos && vSelectionEndPos >= vClipEndPos) || !vFillSpaceWithClonedClips) // The whole clip is affected
			{
				opNewClipOne = static_cast<AudioGClip*>(EMCommandRepository::Instance() -> ExecuteCommandNoUndo(COMMAND_ADD_G_AUDIO_CLIP, &vNewMediaClipID, &vTrackIndex, &vAffectedClipBeginPos));
				m_oUndoCommands.push_back(EMCommandRepository::Instance() -> GetUndoClone(COMMAND_ADD_G_AUDIO_CLIP));
				m_oNewClips.push_back(opNewClipOne);

				if((*oIterator) -> GetFirstInGroup() != NULL)
				{
					EMGUIUtilities::Instance() -> AddToClipGroup((*oIterator) -> GetFirstInGroup(), opNewClipOne);
					EMGUIUtilities::Instance() -> RemoveFromClipGroup((*oIterator) -> GetFirstInGroup(), *oIterator);
				}

				EMGClipRepository::Instance() -> SetSelected(opNewClipOne, vClipIsSelected);

eo << "Destructive edit - Entire clip replaced: Clip 1 (processed) - ID: " << vNewMediaClipID << ", position: " << opNewClipOne -> GetPosition() << ", length: " << opNewClipOne -> GetLength() << ef;
			}

			else if(vSelectionBeginPos <= vClipBeginPos && vSelectionEndPos < vClipEndPos) // The beginning of the clip is affected
			{
				opNewClipOne = static_cast<AudioGClip*>(EMCommandRepository::Instance() -> ExecuteCommandNoUndo(COMMAND_ADD_G_AUDIO_CLIP, &vNewMediaClipID, &vTrackIndex, &vClipBeginPos));
				m_oUndoCommands.push_back(EMCommandRepository::Instance() -> GetUndoClone(COMMAND_ADD_G_AUDIO_CLIP));
				m_oNewClips.push_back(opNewClipOne);

				oClipsToClone.clear();
				oClipsToClone.push_back(*oIterator);
				vClonePositionOffset = vAffectedClipEndPos - vAffectedClipBeginPos;
				opNewClipTwo = dynamic_cast<AudioGClip*>((static_cast<list<EMGClip*>*>(EMCommandRepository::Instance() -> ExecuteCommandNoUndo(COMMAND_CLONE_CLIPS, &vCloneTrackOffset, &vClonePositionOffset, &oClipsToClone))) -> front());
				m_oUndoCommands.push_back(EMCommandRepository::Instance() -> GetUndoClone(COMMAND_CLONE_CLIPS));
				m_oNewClips.push_back(opNewClipTwo);

				vNewMediaClipTwoID = opNewClipTwo -> GetMediaClipID();
				vNewClipMarkIn = vClipMarkIn + (vAffectedClipEndPos - vAffectedClipBeginPos);
				EMCommandRepository::Instance() -> ExecuteCommandNoUndo(MEDIA_COMMAND_SET_CLIP_MARK_IN, &vNewMediaClipTwoID, &vNewClipMarkIn);
				delete EMCommandRepository::Instance() -> GetUndoClone(MEDIA_COMMAND_SET_CLIP_MARK_IN);
				opNewClipTwo -> SetFrame(EMRect((opNewClipTwo -> Frame()).m_vLeft, (opNewClipTwo -> Frame()).m_vTop, ((*oIterator) -> Frame()).m_vRight, (opNewClipTwo -> Frame()).m_vBottom));

				if((*oIterator) -> GetFirstInGroup() != NULL)
				{
					EMGUIUtilities::Instance() -> AddToClipGroup((*oIterator) -> GetFirstInGroup(), opNewClipOne);
					EMGUIUtilities::Instance() -> AddToClipGroup((*oIterator) -> GetFirstInGroup(), opNewClipTwo);
					EMGUIUtilities::Instance() -> RemoveFromClipGroup((*oIterator) -> GetFirstInGroup(), *oIterator);
				}
				else
				{
					EMGUIUtilities::Instance() -> AddToClipGroup(opNewClipOne, opNewClipOne);
					EMGUIUtilities::Instance() -> AddToClipGroup(opNewClipOne, opNewClipTwo);
				}

				list<EMGClip*> oClips;
				oClips.push_back(opNewClipOne);
				oClips.push_back(opNewClipTwo);
				EMGClipRepository::Instance() -> SetSelected(&oClips, vClipIsSelected);

eo << "Destructive edit - 2 new clips: Clip 1 (processed) - ID: " << vNewMediaClipID << ", position: " << opNewClipOne -> GetPosition() << ", length: " << 
opNewClipOne -> GetLength() << ". Clip 2 (cloned) - ID: " << vNewMediaClipTwoID << ", position: " << opNewClipTwo -> GetPosition() << 
", length: " << opNewClipTwo -> GetLength() << ef;
			}

			else if(vSelectionBeginPos > vClipBeginPos && vSelectionEndPos >= vClipEndPos) // The end of the clip is affected
			{
				opNewClipOne = static_cast<AudioGClip*>(EMCommandRepository::Instance() -> ExecuteCommandNoUndo(COMMAND_ADD_G_AUDIO_CLIP, &vNewMediaClipID, &vTrackIndex, &vSelectionBeginPos));
				m_oUndoCommands.push_back(EMCommandRepository::Instance() -> GetUndoClone(COMMAND_ADD_G_AUDIO_CLIP));
				m_oNewClips.push_back(opNewClipOne);

				oClipsToClone.clear();
				oClipsToClone.push_back(*oIterator);
				vClonePositionOffset = 0;
				opNewClipTwo = dynamic_cast<AudioGClip*>((static_cast<list<EMGClip*>*>(EMCommandRepository::Instance() -> ExecuteCommandNoUndo(COMMAND_CLONE_CLIPS, &vCloneTrackOffset, &vClonePositionOffset, &oClipsToClone))) -> front());
				m_oUndoCommands.push_back(EMCommandRepository::Instance() -> GetUndoClone(COMMAND_CLONE_CLIPS));
				m_oNewClips.push_back(opNewClipTwo);

				vNewMediaClipTwoID = opNewClipTwo -> GetMediaClipID();
				vNewClipMarkOut = vClipMarkOut - (vAffectedClipEndPos - vAffectedClipBeginPos);
				EMCommandRepository::Instance() -> ExecuteCommandNoUndo(MEDIA_COMMAND_SET_CLIP_MARK_OUT, &vNewMediaClipTwoID, &vNewClipMarkOut);
				delete EMCommandRepository::Instance() -> GetUndoClone(MEDIA_COMMAND_SET_CLIP_MARK_OUT);
				opNewClipTwo -> SetFrame(EMRect((opNewClipTwo -> Frame()).m_vLeft, (opNewClipTwo -> Frame()).m_vTop, (vAffectedClipBeginPos - 1) / vTimeZoomScale, (opNewClipTwo -> Frame()).m_vBottom));

				if((*oIterator) -> GetFirstInGroup() != NULL)
				{
					EMGUIUtilities::Instance() -> AddToClipGroup((*oIterator) -> GetFirstInGroup(), opNewClipOne);
					EMGUIUtilities::Instance() -> AddToClipGroup((*oIterator) -> GetFirstInGroup(), opNewClipTwo);
					EMGUIUtilities::Instance() -> RemoveFromClipGroup((*oIterator) -> GetFirstInGroup(), *oIterator);
				}
				else
				{
					EMGUIUtilities::Instance() -> AddToClipGroup(opNewClipOne, opNewClipOne);
					EMGUIUtilities::Instance() -> AddToClipGroup(opNewClipOne, opNewClipTwo);
				}

				list<EMGClip*> oClips;
				oClips.push_back(opNewClipOne);
				oClips.push_back(opNewClipTwo);
				EMGClipRepository::Instance() -> SetSelected(&oClips, vClipIsSelected);

eo << "Destructive edit - 2 new clips: Clip 1 (processed) - ID: " << vNewMediaClipID << ", position: " << opNewClipOne -> GetPosition() << ", length: " << 
opNewClipOne -> GetLength() << ". Clip 2 (cloned) - ID: " << vNewMediaClipTwoID << ", position: " << opNewClipTwo -> GetPosition() << 
", length: " << opNewClipTwo -> GetLength() << ef;
			}

			else if(vSelectionBeginPos > vClipBeginPos && vSelectionEndPos < vClipEndPos) // The middle of the clip is affected
			{
				opNewClipOne = static_cast<AudioGClip*>(EMCommandRepository::Instance() -> ExecuteCommandNoUndo(COMMAND_ADD_G_AUDIO_CLIP, &vNewMediaClipID, &vTrackIndex, &vSelectionBeginPos));
				m_oUndoCommands.push_back(EMCommandRepository::Instance() -> GetUndoClone(COMMAND_ADD_G_AUDIO_CLIP));
				m_oNewClips.push_back(opNewClipOne);

				oClipsToClone.clear();
				oClipsToClone.push_back(*oIterator);
				vClonePositionOffset = 0;
				opNewClipTwo = dynamic_cast<AudioGClip*>((static_cast<list<EMGClip*>*>(EMCommandRepository::Instance() -> ExecuteCommandNoUndo(COMMAND_CLONE_CLIPS, &vCloneTrackOffset, &vClonePositionOffset, &oClipsToClone))) -> front());
				m_oUndoCommands.push_back(EMCommandRepository::Instance() -> GetUndoClone(COMMAND_CLONE_CLIPS));
				m_oNewClips.push_back(opNewClipTwo);

				vNewMediaClipTwoID = opNewClipTwo -> GetMediaClipID();
				vNewClipMarkOut = vClipMarkOut - (vClipEndPos - vAffectedClipBeginPos);
				EMCommandRepository::Instance() -> ExecuteCommandNoUndo(MEDIA_COMMAND_SET_CLIP_MARK_OUT, &vNewMediaClipTwoID, &vNewClipMarkOut);
				delete EMCommandRepository::Instance() -> GetUndoClone(MEDIA_COMMAND_SET_CLIP_MARK_OUT);
				opNewClipTwo -> SetFrame(EMRect((opNewClipTwo -> Frame()).m_vLeft, (opNewClipTwo -> Frame()).m_vTop, (vAffectedClipBeginPos - 1) / vTimeZoomScale, (opNewClipTwo -> Frame()).m_vBottom));

				oClipsToClone.clear();
				oClipsToClone.push_back(*oIterator);
				vClonePositionOffset = vAffectedClipEndPos - vClipBeginPos;
				opNewClipThree = dynamic_cast<AudioGClip*>((static_cast<list<EMGClip*>*>(EMCommandRepository::Instance() -> ExecuteCommandNoUndo(COMMAND_CLONE_CLIPS, &vCloneTrackOffset, &vClonePositionOffset, &oClipsToClone))) -> front());
				m_oUndoCommands.push_back(EMCommandRepository::Instance() -> GetUndoClone(COMMAND_CLONE_CLIPS));
				m_oNewClips.push_back(opNewClipThree);

				vNewMediaClipThreeID = opNewClipThree -> GetMediaClipID();
				vNewClipMarkIn = vClipMarkIn + (vAffectedClipEndPos - vClipBeginPos);
				EMCommandRepository::Instance() -> ExecuteCommandNoUndo(MEDIA_COMMAND_SET_CLIP_MARK_IN, &vNewMediaClipThreeID, &vNewClipMarkIn);
				delete EMCommandRepository::Instance() -> GetUndoClone(MEDIA_COMMAND_SET_CLIP_MARK_IN);
				opNewClipThree -> SetFrame(EMRect((opNewClipThree -> Frame()).m_vLeft, (opNewClipThree -> Frame()).m_vTop, ((*oIterator) -> Frame()).m_vRight, (opNewClipThree -> Frame()).m_vBottom));

				if((*oIterator) -> GetFirstInGroup() != NULL)
				{
					EMGUIUtilities::Instance() -> AddToClipGroup((*oIterator) -> GetFirstInGroup(), opNewClipOne);
					EMGUIUtilities::Instance() -> AddToClipGroup((*oIterator) -> GetFirstInGroup(), opNewClipTwo);
					EMGUIUtilities::Instance() -> AddToClipGroup((*oIterator) -> GetFirstInGroup(), opNewClipThree);
					EMGUIUtilities::Instance() -> RemoveFromClipGroup((*oIterator) -> GetFirstInGroup(), *oIterator);
				}
				else
				{
					EMGUIUtilities::Instance() -> AddToClipGroup(opNewClipOne, opNewClipOne);
					EMGUIUtilities::Instance() -> AddToClipGroup(opNewClipOne, opNewClipTwo);
					EMGUIUtilities::Instance() -> AddToClipGroup(opNewClipOne, opNewClipThree);
				}

				list<EMGClip*> oClips;
				oClips.push_back(opNewClipOne);
				oClips.push_back(opNewClipTwo);
				oClips.push_back(opNewClipThree);
				EMGClipRepository::Instance() -> SetSelected(&oClips, vClipIsSelected);

eo << "Destructive edit - 3 new clips: Clip 1 (processed) - ID: " << vNewMediaClipID << ", position: " << opNewClipOne -> GetPosition() << ", length: " << 
opNewClipOne -> GetLength() << ". Clip 2 (cloned) - ID: " << vNewMediaClipTwoID << ", position: " << opNewClipTwo -> GetPosition() << 
", length: " << opNewClipTwo -> GetLength() << ". Clip 3 (cloned) - ID: " << vNewMediaClipThreeID << ", position: " << opNewClipThree -> GetPosition() <<
", length: " << opNewClipThree -> GetLength() << ef; 
			}
			// Update media pool
			vpClipEntryID = static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_ENTRY_ID, &vNewMediaClipID));
			vpClipFileName = static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME, vpClipEntryID));
			int32 vEntryIDs[2] = {*vpClipEntryID, -1};
			// FIXME: Need full path and file name for this to work.
			EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_ADD_TO_MEDIA_POOL_VIEW, const_cast<char*>(vpClipFileName), &vEntryIDs);
			MediaPoolView::Instance() -> ChangeInstances(MediaPoolView::Instance() -> GetIndexID(vpClipFileName));
			if(opNewClipTwo != NULL)
			{
				vpClipEntryID = static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_ENTRY_ID, &vNewMediaClipTwoID));
				vpClipFileName = static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME, &vpClipEntryID));
				MediaPoolView::Instance() -> ChangeInstances(MediaPoolView::Instance() -> GetIndexID(vpClipFileName));
			}
			if(opNewClipThree != NULL)
			{
				vpClipEntryID = static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_ENTRY_ID, &vNewMediaClipThreeID));
				vpClipFileName = static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME, &vpClipEntryID));
				MediaPoolView::Instance() -> ChangeInstances(MediaPoolView::Instance() -> GetIndexID(vpClipFileName));
			}

			vClipCounter++;
			oIterator++;
		}

		// Remove all original clips from the GUI
		EMCommandRepository::Instance() -> ExecuteCommandNoUndo(COMMAND_DELETE_CLIPS_FROM_LIST, &m_oAffectedClips);
		m_oUndoCommands.push_back(EMCommandRepository::Instance() -> GetUndoClone(COMMAND_DELETE_CLIPS_FROM_LIST));

		m_opTrackView -> GetView() -> Invalidate(oInvalidateRect);

		m_opTrackView -> GetView() -> UnlockWindow();
	}
}


