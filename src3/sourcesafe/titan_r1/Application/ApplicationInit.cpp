#include "ApplicationInit.h"

#include "AudioGClip.h"
#include "AudioGClipContainer.h"
#include "AudioGTrackInfo.h"
#include "ColaborationView.h"
#include "CommandActiveMIDITrackStepDown.h"
#include "CommandActiveMIDITrackStepUp.h"
#include "CommandAddAudioTrack.h"
#include "CommandAddGAudioClip.h"
#include "CommandAddGAudioTrack.h"
#include "CommandAddGMIDIClip.h"
#include "CommandAddGMIDITrack.h"
#include "CommandAddGTransitionClip.h"
#include "CommandAddGTransitionTrack.h"
#include "CommandAddGVideoClip.h"
#include "CommandAddGVideoTrack.h"
#include "CommandAddMIDITrack.h"
#include "CommandAddToMediaPoolView.h"
#include "CommandAddTransitionTrack.h"
#include "CommandAddVideoTrack.h"
#include "CommandAudioCrop.h"
#include "CommandAudioDestructiveFX.h"
#include "CommandAudioFadeIn.h"
#include "CommandAudioFadeOut.h"
#include "CommandAudioNormalize.h"
#include "CommandAudioReverse.h"
#include "CommandAudioSilence.h" 
#include "CommandChangeExplorerListViewControlStyle.h"
#include "CommandChangeVideoPlaybackSpeed.h"
#include "CommandClearUndoStack.h"
#include "CommandClipsSelectAll.h"
#include "CommandClipsSelectInvert.h"
#include "CommandClipsSelectNone.h"
#include "CommandCloneClips.h"
#include "CommandCloneMIDIEvents.h"
#include "CommandCloseAudioExtractionConversionDialog.h"
#include "CommandCreateMIDIEvents.h"
#include "CommandCreateProject.h"
#include "CommandCutClips.h"
#include "CommandCutMIDIEvents.h"
#include "CommandDeleteClips.h"
#include "CommandDeleteClipsEventsNodes.h"
#include "CommandDeleteClipsFromList.h"
#include "CommandDeleteEntryFromMediaPoolView.h"
#include "CommandDeleteMIDIEvents.h"
#include "CommandDeleteTracks.h"
#include "CommandDeleteTracksFromList.h"
#include "CommandDisplayAboutDialog.h"
#include "CommandDisplayAudioExtractionConversionDialog.h"
#include "CommandDisplayChangeClipNameDialog.h"
#include "CommandDisplayClipPropertiesDialog.h"
#include "CommandDisplayColorBalanceDialog.h"
#include "CommandDisplayEQDialog.h"
#include "CommandDisplayExportMediaDialog.h"
#include "CommandDisplayExportMediaSettingsDialog.h"
#include "CommandDisplayHSV_ValueDialog.h"
#include "CommandDisplayLoginDialog.h"
#include "CommandDisplayNewProjectDialog.h"
#include "CommandDisplayNormalizeDialog.h"
#include "CommandDisplayProjectSettingsDialog.h"
#include "CommandDisplayPropertyPageDialog.h"
#include "CommandDisplayQuantizeDialog.h"
#include "CommandDisplayRegistrationDialog.h"
#include "CommandDisplaySearchAddUserDialog.h"
#include "CommandDisplaySearchUserDialog.h"
#include "CommandDisplaySetVelocityDialog.h"
#include "CommandDisplaySplashScreenText.h"
#include "CommandDisplaySystemSettingsDialog.h"
#include "CommandDisplayTransitionDialog.h"
#include "CommandDisplayTransposeDialog.h"
#include "CommandDisplayVideoRecordingDialog.h"
#include "CommandDisplayVideoSpeedDialog.h"
#include "CommandExpandTrackInfo.h"
#include "CommandExpandTimeLine.h"
#include "CommandExpandUtilityView.h"
#include "CommandFastForward.h"
#include "CommandGetApplicationData.h"
#include "CommandGetEntriesFromMediaPoolView.h"
#include "CommandGetGTrackIndex.h"
#include "CommandGetTrackGUIIndex.h"
#include "CommandGroupClips.h"
#include "CommandGroupClipsFromList.h"
#include "CommandLoadMedia.h"
#include "CommandAudioLockToTempo.h"
#include "CommandMessageSender.h"
#include "CommandMIDIToggleMetronome.h"
#include "CommandMIDIQuickQuantize.h"
#include "CommandMIDIFixedLength.h"
#include "CommandMoveClips.h"
#include "CommandMoveMIDIEvents.h"
#include "CommandMoveTracks.h"
#include "CommandMuteClips.h"
#include "CommandNewProject.h"
#include "CommandOpenProject.h"
#include "CommandPlay.h"
#include "CommandPostCommand.h"
#include "CommandPrepareRecording.h"
#include "CommandProjectBegin.h"
#include "CommandProjectEnd.h"
#include "CommandQuitApplication.h"
#include "CommandRedo.h"
#include "CommandRegionBegin.h"
#include "CommandRegionEnd.h"
#include "CommandRewind.h"
#include "CommandSaveProject.h"
#include "CommandSetClipEdit.h"
#include "CommandSetDefaultSettings.h"
#include "CommandSetDefaultSystemSettings.h"
#include "CommandSetFXEdit.h"
#include "CommandSetTrackEdit.h"
#include "CommandSetUndoTexts.h"
#include "CommandSetBottomLabelInAudioExtractionConversionDialog.h"
#include "CommandSetValueInAudioExtractionConversionDialog.h"
#include "CommandStop.h"
#include "CommandToggleAutoQuantize.h"
#include "CommandToggleEnableAudio.h"
#include "CommandToggleEnableMIDI.h"
#include "CommandToggleEnableVideo.h"
#include "CommandToggleExpertMode.h"
#include "CommandToggleLoop.h"
#include "CommandTogglePlayback.h"
#include "CommandToggleRelativeMove.h"
#include "CommandToggleSnapping.h"
#include "CommandTracksSelectAll.h"
#include "CommandTracksSelectInvert.h"
#include "CommandTracksSelectNone.h"
#include "CommandToggleViewClipContent.h"
#include "CommandUndo.h"
#include "CommandUngroupClips.h"
#include "CommandUpdateInstanceInMediaPoolView.h"
#include "CommandUtilityButtonPressed.h"
#include "CommandWriteStatusBarMessage.h"
#include "CommandZoomInHorizontal.h"
#include "CommandZoomOutHorizontal.h"
#include "CommandZoomInTracksVertical.h"
#include "CommandZoomOutTracksVertical.h"
#include "CommandZoomInTrackVertical.h"
#include "CommandZoomOutTrackVertical.h"
#include "CommandIDs.h"
#include "DirectoryIDs.h"
#include "EMApplication.h"
#include "EMCommandLineIDs.h"
#include "EMCommandRepository.h"
#include "EMExceptionHandler.h"
#include "EMFactory.h"
#include "EMGClipRepository.h"
#include "EMGEffectsRepository.h"
#include "EMGUIGlobals.h"
#include "EMGUIUtilities.h"
#include "EMGTrackRepository.h"
#include "EMImageBufferRepository.h"
#include "EMKeyboard.h"
#include "EMKeyboardGlobals.h"
#include "EMMediaCommandFactory.h"
#include "EMMessageBox.h"
#include "EMNetworkCommandFactory.h"
#include "EMMediaEngine_Library.h"
#include "EMMediaGlobals.h"
#include "EMMenuBar.h"
#include "EMMessageBox.h"
#include "EMMessages.h"
#include "EMNetworkEngine.h"
#include "EMProjectCommandFactory.h"
#include "EMProjectManager_Library.h"
#include "EMRect.h"
#include "EMResourceRepository.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMSlimEditLabel.h"
#include "EMTextControl.h"
#include "EMThreadRepository.h"
#include "EMVideoPreview.h"
#include "EMWindow.h"

#ifdef PLATFORM_WINDOWS
#include "EMWinView.h"
#include "EMWinWindow.h"
extern DWORD GetDXVersion();
#endif

#include "ExplorerView.h"
#include "FileDialogTarget.h"
#include "GUIGlobals.h"
#include "MediaPoolView.h"
#include "MIDIGClip.h"
#include "MIDIGClipContainer.h"
#include "MIDIGTrackInfo.h"
#include "ProjectManager.h"
#include "ResourceIDs.h"
#include "SettingIDs.h"
#include "SplashWindow.h"
#include "TrackWindowStatusBar.h"
#include "TransitionGClip.h"
#include "TransitionGClipContainer.h"
#include "TransitionGTrackInfo.h"
#include "UndoCommandChangeClipResize.h"
#include "UndoCommandChangeClipSelection.h"
#include "UndoCommandAddRubberBandNodes.h"
#include "UndoCommandDeleteRubberBandNodes.h"
#include "UndoCommandMoveRubberBandNodes.h"
#include "UndoCommandResizeMIDIEvent.h"
#include "UtilityView.h"
#include "VideoGClip.h"
#include "VideoGClipContainer.h"
#include "VideoGTrackInfo.h"

#include <process.h>
#include <string>
#include <stdio.h>
#include "r.h"

ApplicationInit::ApplicationInit() :
m_opTrackWindow(NULL),
m_opResourceRepository(NULL),
m_opFileDialogTarget(NULL)
{
}

ApplicationInit::~ApplicationInit()
{
//	EMThreadRepository::Instance() -> DeleteThread(GUI_SERVICE_THREAD_NAME);
//	int* vpProjectID = EM_new int(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_PROJECT_ID))));
//	EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_CLOSE_PROJECT, vpProjectID);

	delete m_opSplashWindow;

// Cannot have these clean-up methods defined in their base classes since they are static
#ifdef PLATFORM_WINDOWS
	EMWinView::CleanUp();
	EMWinWindow::CleanUp();
#endif

	delete EMGEffectsRepository::Instance();
	delete m_opFileDialogTarget;
	delete EMGUIUtilities::Instance();
//	delete EMExceptionHandler::Instance();
	EMExceptionHandler::DeleteInstance();
	delete EMImageBufferRepository::Instance();
	delete EMVideoPreview::Instance();
	delete TrackWindowStatusBar::Instance();
	// The edit label has a static object that should be deleted
	EMSlimEditLabel::CleanUp();
	delete m_opTrackWindow;
	delete EMKeyboard::Instance();
	delete EMGTrackRepository::Instance();
	delete EMGClipRepository::Instance();
	delete m_opResourceRepository;
//	DeleteMediaCommandFactory();
	delete EMThreadRepository::Instance();
	DeleteMediaEngine();
//	DeleteProjectManager();// FIXME:: Delete the project manager
	delete EMCommandRepository::Instance();
	delete EMSettingsRepository::Instance();
	delete EMFactory::Instance();
	delete EMMessageBox::Instance();
	delete ExplorerView::Instance();
	delete MediaPoolView::Instance();
	delete ColaborationView::Instance();

	EMNetworkEngine::Delete();
}

// Please insert new commands sorted alphabetically
void ApplicationInit::AddCommands()
{
	eo << "Adding commands..." << ef;
	EMCommandRepository* opRepository = EMCommandRepository::Instance();

	opRepository -> AddCommand(EM_new CommandActiveMIDITrackStepDown(m_opTrackWindow -> GetTrackInfo()), COMMAND_ACTIVE_MIDI_TRACK_STEP_DOWN);
	opRepository -> AddCommand(EM_new CommandActiveMIDITrackStepUp(m_opTrackWindow -> GetTrackInfo()), COMMAND_ACTIVE_MIDI_TRACK_STEP_UP);
	opRepository -> AddCommand(EM_new CommandAddAudioTrack(m_opTrackWindow -> GetTrackView(), m_opTrackWindow -> GetTrackInfo()), COMMAND_ADD_AUDIO_TRACK);
	opRepository -> AddCommand(EM_new CommandAddGAudioClip(m_opTrackWindow -> GetTrackView(), m_opTrackWindow -> GetTrackInfo(), NULL), COMMAND_ADD_G_AUDIO_CLIP);
	opRepository -> AddCommand(EM_new CommandAddGAudioTrack(m_opTrackWindow -> GetTrackView(), m_opTrackWindow -> GetTrackInfo()), COMMAND_ADD_G_AUDIO_TRACK);
	opRepository -> AddCommand(EM_new CommandAddGMIDIClip(m_opTrackWindow -> GetTrackView(), m_opTrackWindow -> GetTrackInfo(), NULL), COMMAND_ADD_G_MIDI_CLIP);
	opRepository -> AddCommand(EM_new CommandAddGMIDITrack(m_opTrackWindow -> GetTrackView(), m_opTrackWindow -> GetTrackInfo()), COMMAND_ADD_G_MIDI_TRACK);
	opRepository -> AddCommand(EM_new CommandAddGTransitionClip(m_opTrackWindow -> GetTrackView(), m_opTrackWindow -> GetTrackInfo(), NULL), COMMAND_ADD_G_TRANSITION_CLIP);
	opRepository -> AddCommand(EM_new CommandAddGTransitionTrack(m_opTrackWindow -> GetTrackView(), m_opTrackWindow -> GetTrackInfo()), COMMAND_ADD_G_TRANSITION_TRACK);
	opRepository -> AddCommand(EM_new CommandAddGVideoClip(m_opTrackWindow -> GetTrackView(), m_opTrackWindow -> GetTrackInfo(), NULL), COMMAND_ADD_G_VIDEO_CLIP);
	opRepository -> AddCommand(EM_new CommandAddGVideoTrack(m_opTrackWindow -> GetTrackView(), m_opTrackWindow -> GetTrackInfo()), COMMAND_ADD_G_VIDEO_TRACK);
	opRepository -> AddCommand(EM_new CommandAddMIDITrack(m_opTrackWindow -> GetTrackView(), m_opTrackWindow -> GetTrackInfo()), COMMAND_ADD_MIDI_TRACK);
	opRepository -> AddCommand(EM_new CommandAddToMediaPoolView(), COMMAND_ADD_TO_MEDIA_POOL_VIEW);
	opRepository -> AddCommand(EM_new CommandAddTransitionTrack(m_opTrackWindow -> GetTrackView(), m_opTrackWindow -> GetTrackInfo()), COMMAND_ADD_TRANSITION_TRACK);
	opRepository -> AddCommand(EM_new CommandAddVideoTrack(m_opTrackWindow -> GetTrackView(), m_opTrackWindow -> GetTrackInfo()), COMMAND_ADD_VIDEO_TRACK);
	opRepository -> AddCommand(EM_new CommandAudioCrop(m_opTrackWindow -> GetTrackView()), COMMAND_AUDIO_CROP);
	opRepository -> AddCommand(EM_new CommandAudioDestructiveFX(m_opTrackWindow -> GetTrackView()), COMMAND_AUDIO_DESTRUCTIVE_FX);
	opRepository -> AddCommand(EM_new CommandAudioFadeIn(m_opTrackWindow -> GetTrackView()), COMMAND_AUDIO_FADE_IN);
	opRepository -> AddCommand(EM_new CommandAudioFadeOut(m_opTrackWindow -> GetTrackView()), COMMAND_AUDIO_FADE_OUT);
	opRepository -> AddCommand(EM_new CommandAudioNormalize(m_opTrackWindow -> GetTrackView()), COMMAND_AUDIO_NORMALIZE);
	opRepository -> AddCommand(EM_new CommandAudioReverse(m_opTrackWindow -> GetTrackView()), COMMAND_AUDIO_REVERSE);
	opRepository -> AddCommand(EM_new CommandAudioSilence(m_opTrackWindow -> GetTrackView()), COMMAND_AUDIO_SILENCE);
	opRepository -> AddCommand(EM_new CommandChangeExplorerListViewControlStyle(ExplorerView::Instance() -> GetListViewControl(), EM_LIST_VIEW_STYLE_ICON), COMMAND_CHANGE_EXPLORER_LIST_VIEW_CONTROL_STYLE_TO_ICON);
	opRepository -> AddCommand(EM_new CommandChangeExplorerListViewControlStyle(ExplorerView::Instance() -> GetListViewControl(), EM_LIST_VIEW_STYLE_LIST), COMMAND_CHANGE_EXPLORER_LIST_VIEW_CONTROL_STYLE_TO_LIST);
	opRepository -> AddCommand(EM_new CommandChangeExplorerListViewControlStyle(ExplorerView::Instance() -> GetListViewControl(), EM_LIST_VIEW_STYLE_SMALL_ICON), COMMAND_CHANGE_EXPLORER_LIST_VIEW_CONTROL_STYLE_TO_SMALL_ICON);
	opRepository -> AddCommand(EM_new CommandChangeVideoPlaybackSpeed(m_opTrackWindow -> GetTrackView()), COMMAND_CHANGE_VIDEO_PLAYBACK_SPEED);
	opRepository -> AddCommand(EM_new CommandClearUndoStack(), COMMAND_CLEAR_UNDO_STACK);
	opRepository -> AddCommand(EM_new CommandClipsSelectAll(m_opTrackWindow), COMMAND_CLIPS_SELECT_ALL);
	opRepository -> AddCommand(EM_new CommandClipsSelectInvert(m_opTrackWindow), COMMAND_CLIPS_SELECT_INVERT);
	opRepository -> AddCommand(EM_new CommandClipsSelectNone(m_opTrackWindow), COMMAND_CLIPS_SELECT_NONE);
	opRepository -> AddCommand(EM_new CommandCloneClips(m_opTrackWindow -> GetTrackView()), COMMAND_CLONE_CLIPS);
	opRepository -> AddCommand(EM_new CommandCloneMIDIEvents(m_opTrackWindow -> GetTrackView()), COMMAND_CLONE_MIDI_EVENTS);
	opRepository -> AddCommand(EM_new CommandCloseAudioExtractionConversionDialog(), COMMAND_CLOSE_AUDIO_EXTRACTION_CONVERSION_DIALOG);
	opRepository -> AddCommand(EM_new CommandCreateMIDIEvents(m_opTrackWindow -> GetTrackView()), COMMAND_CREATE_MIDI_EVENTS);
	opRepository -> AddCommand(EM_new CommandCreateProject(m_opFileDialogTarget), COMMAND_CREATE_PROJECT);
	opRepository -> AddCommand(EM_new CommandCutClips(m_opTrackWindow -> GetTrackView()), COMMAND_CUT_CLIPS);
	opRepository -> AddCommand(EM_new CommandCutMIDIEvents(m_opTrackWindow -> GetTrackView()), COMMAND_CUT_MIDI_EVENTS);
	opRepository -> AddCommand(EM_new CommandDeleteClips(), COMMAND_DELETE_CLIPS);
	opRepository -> AddCommand(EM_new CommandDeleteMIDIEvents(m_opTrackWindow -> GetTrackView()), COMMAND_DELETE_MIDI_EVENTS);
	opRepository -> AddCommand(EM_new CommandDeleteClipsEventsNodes(m_opTrackWindow -> GetTrackView()), COMMAND_DELETE_CLIPS_EVENTS_NODES);
	opRepository -> AddCommand(EM_new CommandDeleteClipsFromList(m_opTrackWindow -> GetTrackView()), COMMAND_DELETE_CLIPS_FROM_LIST);
	opRepository -> AddCommand(EM_new CommandDeleteEntryFromMediaPoolView(), COMMAND_DELETE_ENTRY_FROM_MEDIA_POOL_VIEW);
	opRepository -> AddCommand(EM_new CommandDeleteTracks(m_opTrackWindow -> GetTrackView()), COMMAND_DELETE_TRACKS);
	opRepository -> AddCommand(EM_new CommandDeleteTracksFromList(m_opTrackWindow -> GetTrackView()), COMMAND_DELETE_TRACKS_FROM_LIST);
	opRepository -> AddCommand(EM_new CommandDisplayAboutDialog(m_opTrackWindow -> GetWindow()), COMMAND_DISPLAY_ABOUT_DIALOG);
	opRepository -> AddCommand(EM_new CommandDisplayAudioExtractionConversionDialog(m_opTrackWindow -> GetWindow()), COMMAND_DISPLAY_AUDIO_EXTRACTION_CONVERSION_DIALOG);
	opRepository -> AddCommand(EM_new CommandDisplayChangeClipNameDialog(m_opTrackWindow -> GetTrackView()), COMMAND_DISPLAY_CHANGE_CLIP_NAME_DIALOG);
	opRepository -> AddCommand(EM_new CommandDisplayClipPropertiesDialog(m_opTrackWindow -> GetTrackView()), COMMAND_DISPLAY_CLIP_PROPERTIES_DIALOG);
	opRepository -> AddCommand(EM_new CommandDisplayColorBalanceDialog(m_opTrackWindow -> GetWindow()), COMMAND_DISPLAY_COLOR_BALANCE_DIALOG);
	opRepository -> AddCommand(EM_new CommandDisplayEQDialog(m_opTrackWindow -> GetWindow()), COMMAND_DISPLAY_EQ_DIALOG);
	opRepository -> AddCommand(EM_new CommandDisplayExportMediaDialog(m_opTrackWindow -> GetWindow()), COMMAND_DISPLAY_EXPORT_MEDIA_DIALOG);
	opRepository -> AddCommand(EM_new CommandDisplayExportMediaSettingsDialog(m_opTrackWindow -> GetWindow()), COMMAND_DISPLAY_EXPORT_MEDIA_SETTINGS_DIALOG);
	opRepository -> AddCommand(EM_new CommandDisplayHSV_ValueDialog(m_opTrackWindow -> GetWindow()), COMMAND_DISPLAY_HSV_VALUE_DIALOG);
	opRepository -> AddCommand(EM_new CommandDisplayLoginDialog(m_opTrackWindow -> GetWindow()), COMMAND_DISPLAY_LOGIN_DIALOG);
	opRepository -> AddCommand(EM_new CommandDisplayNewProjectDialog(m_opTrackWindow -> GetWindow()), COMMAND_DISPLAY_NEW_PROJECT_DIALOG);
	opRepository -> AddCommand(EM_new CommandDisplayNormalizeDialog(m_opTrackWindow -> GetWindow()), COMMAND_DISPLAY_NORMALIZE_DIALOG);
	opRepository -> AddCommand(EM_new CommandDisplayProjectSettingsDialog(m_opTrackWindow -> GetWindow()), COMMAND_DISPLAY_PROJECT_SETTINGS_DIALOG);
	opRepository -> AddCommand(EM_new CommandDisplayPropertyPageDialog(m_opTrackWindow -> GetWindow()), COMMAND_DISPLAY_PROPERTY_PAGE_DIALOG);
	opRepository -> AddCommand(EM_new CommandDisplayQuantizeDialog(), COMMAND_DISPLAY_QUANTIZE_DIALOG);
	opRepository -> AddCommand(EM_new CommandDisplayRegistrationDialog(m_opTrackWindow -> GetWindow()), COMMAND_DISPLAY_REGISTRATION_DIALOG);
	opRepository -> AddCommand(EM_new CommandDisplaySearchAddUserDialog(m_opTrackWindow -> GetWindow()), COMMAND_DISPLAY_SEARCH_ADD_USER_DIALOG);
	opRepository -> AddCommand(EM_new CommandDisplaySearchUserDialog(m_opTrackWindow -> GetWindow()), COMMAND_DISPLAY_SEARCH_USER_DIALOG);
	opRepository -> AddCommand(EM_new CommandDisplaySetVelocityDialog(), COMMAND_DISPLAY_SET_VELOCITY_DIALOG);
	opRepository -> AddCommand(EM_new CommandDisplaySplashScreenText(m_opSplashWindow), COMMAND_DISPLAY_SPLASH_SCREEN_TEXT);
	opRepository -> AddCommand(EM_new CommandDisplaySystemSettingsDialog(m_opTrackWindow -> GetWindow()), COMMAND_DISPLAY_SYSTEM_SETTINGS_DIALOG);
	opRepository -> AddCommand(EM_new CommandDisplayTransitionDialog(m_opTrackWindow -> GetWindow(), m_opTrackWindow -> GetTrackView() -> GetView()), COMMAND_DISPLAY_TRANSITION_DIALOG);
	opRepository -> AddCommand(EM_new CommandDisplayTransposeDialog(), COMMAND_DISPLAY_TRANSPOSE_DIALOG);
	opRepository -> AddCommand(EM_new CommandDisplayVideoRecordingDialog(), COMMAND_DISPLAY_VIDEO_RECORDING_DIALOG);
	opRepository -> AddCommand(EM_new CommandDisplayVideoSpeedDialog(m_opTrackWindow -> GetWindow()), COMMAND_DISPLAY_VIDEO_SPEED_DIALOG);
	opRepository -> AddCommand(EM_new CommandExpandTimeLine(m_opTrackWindow), COMMAND_EXPAND_TIMELINE);
	opRepository -> AddCommand(EM_new CommandExpandTrackInfo(m_opTrackWindow), COMMAND_EXPAND_TRACK_INFO);
	opRepository -> AddCommand(EM_new CommandExpandUtilityView(m_opTrackWindow), COMMAND_EXPAND_UTILITY_VIEW);
	opRepository -> AddCommand(EM_new CommandFastForward(m_opTrackWindow -> GetToolBar()), COMMAND_FAST_FORWARD);
	opRepository -> AddCommand(EM_new CommandGetApplicationData(), COMMAND_GET_APPLICATION_DATA);
	opRepository -> AddCommand(EM_new CommandGetEntriesFromMediaPoolView(), COMMAND_GET_ENTRIES_FROM_MEDIA_POOL_VIEW);
	opRepository -> AddCommand(EM_new CommandGetGTrackIndex(), COMMAND_GET_G_TRACK_INDEX);
	opRepository -> AddCommand(EM_new CommandGetTrackGUIIndex(), COMMAND_GET_TRACK_GUI_INDEX);
	opRepository -> AddCommand(EM_new CommandGroupClips(m_opTrackWindow -> GetTrackView()), COMMAND_GROUP_CLIPS);
	opRepository -> AddCommand(EM_new CommandGroupClipsFromList(m_opTrackWindow -> GetTrackView()), COMMAND_GROUP_CLIPS_FROM_LIST);
	opRepository -> AddCommand(EM_new CommandLoadMedia(m_opFileDialogTarget), COMMAND_LOAD_MEDIA);
	opRepository -> AddCommand(EM_new CommandAudioLockToTempo(), COMMAND_AUDIO_LOCK_TO_TEMPO);
	opRepository -> AddCommand(EM_new CommandMessageSender(), COMMAND_MESSAGE_SENDER);
	opRepository -> AddCommand(EM_new CommandMIDIFixedLength(m_opTrackWindow), COMMAND_MIDI_FIXED_LENGTH);
	opRepository -> AddCommand(EM_new CommandMIDIQuickQuantize(m_opTrackWindow), COMMAND_MIDI_QUICK_QUANTIZE);
	opRepository -> AddCommand(EM_new CommandMIDIToggleMetronome(m_opTrackWindow -> GetMenuBar()), COMMAND_MIDI_TOGGLE_METRONOME);
	opRepository -> AddCommand(EM_new CommandMoveClips(m_opTrackWindow -> GetTrackView()), COMMAND_MOVE_CLIPS);
	opRepository -> AddCommand(EM_new CommandMoveMIDIEvents(m_opTrackWindow -> GetTrackView()), COMMAND_MOVE_MIDI_EVENTS);
	opRepository -> AddCommand(EM_new CommandMoveTracks(m_opTrackWindow), COMMAND_MOVE_TRACKS);
	opRepository -> AddCommand(EM_new CommandMuteClips(m_opTrackWindow -> GetTrackView()), COMMAND_MUTE_CLIPS);
	opRepository -> AddCommand(EM_new CommandNewProject(), COMMAND_NEW_PROJECT);
	opRepository -> AddCommand(EM_new CommandOpenProject(m_opFileDialogTarget), COMMAND_OPEN_PROJECT);
	opRepository -> AddCommand(EM_new CommandPlay(m_opTrackWindow -> GetToolBar()), COMMAND_PLAY);
	opRepository -> AddCommand(EM_new CommandPostCommand(), COMMAND_POST_COMMAND);
	opRepository -> AddCommand(EM_new CommandPrepareRecording(m_opTrackWindow -> GetToolBar()), COMMAND_PREPARE_RECORDING);
	opRepository -> AddCommand(EM_new CommandProjectBegin(m_opTrackWindow -> GetToolBar()), COMMAND_PROJECT_BEGIN);
	opRepository -> AddCommand(EM_new CommandProjectEnd(m_opTrackWindow -> GetToolBar()), COMMAND_PROJECT_END);
	opRepository -> AddCommand(EM_new CommandQuitApplication(m_opTrackWindow), COMMAND_QUIT_APPLICATION);
	opRepository -> AddCommand(EM_new CommandRedo(), COMMAND_REDO);
	opRepository -> AddCommand(EM_new CommandRegionBegin(m_opTrackWindow -> GetToolBar()), COMMAND_REGION_BEGIN);
	opRepository -> AddCommand(EM_new CommandRegionEnd(m_opTrackWindow -> GetToolBar()), COMMAND_REGION_END);
	opRepository -> AddCommand(EM_new CommandRewind(m_opTrackWindow -> GetToolBar()), COMMAND_REWIND);
	opRepository -> AddCommand(EM_new CommandSaveProject(m_opFileDialogTarget, m_opTrackWindow), COMMAND_SAVE_PROJECT);
	opRepository -> AddCommand(EM_new CommandSetClipEdit(), COMMAND_SET_CLIP_EDIT);
	opRepository -> AddCommand(EM_new CommandSetDefaultSettings(), COMMAND_SET_DEFAULT_SETTINGS);
	opRepository -> AddCommand(EM_new CommandSetDefaultSystemSettings(), COMMAND_SET_DEFAULT_SYSTEM_SETTINGS);
	opRepository -> AddCommand(EM_new CommandSetFXEdit(), COMMAND_SET_FX_EDIT);
	opRepository -> AddCommand(EM_new CommandSetTrackEdit(), COMMAND_SET_TRACK_EDIT);
	opRepository -> AddCommand(EM_new CommandSetUndoTexts(m_opTrackWindow -> GetMenuBar()), COMMAND_SET_UNDO_TEXTS);
	opRepository -> AddCommand(EM_new CommandSetBottomLabelInAudioExtractionConversionDialog(), COMMAND_SET_BOTTOM_LABEL_IN_AUDIO_EXTRACTION_CONVERSION_DIALOG);
	opRepository -> AddCommand(EM_new CommandSetValueInAudioExtractionConversionDialog(), COMMAND_SET_VALUE_IN_AUDIO_EXTRACTION_CONVERSION_DIALOG);
	opRepository -> AddCommand(EM_new CommandStop(m_opTrackWindow -> GetToolBar()), COMMAND_STOP);
	opRepository -> AddCommand(EM_new CommandToggleAutoQuantize(), COMMAND_TOGGLE_AUTO_QUANTIZE);
	opRepository -> AddCommand(EM_new CommandToggleEnableAudio(m_opTrackWindow -> GetMenuBar(), m_opTrackWindow -> GetTrackView()), COMMAND_TOGGLE_ENABLE_AUDIO);
	opRepository -> AddCommand(EM_new CommandToggleEnableMIDI(m_opTrackWindow -> GetMenuBar(), m_opTrackWindow -> GetTrackView()), COMMAND_TOGGLE_ENABLE_MIDI);
	opRepository -> AddCommand(EM_new CommandToggleEnableVideo(m_opTrackWindow -> GetMenuBar(), m_opTrackWindow -> GetTrackView()), COMMAND_TOGGLE_ENABLE_VIDEO);
	opRepository -> AddCommand(EM_new CommandToggleExpertMode(m_opTrackWindow -> GetMenuBar(), m_opTrackWindow -> GetTrackView()), COMMAND_TOGGLE_EXPERT_MODE);
	opRepository -> AddCommand(EM_new CommandToggleLoop(m_opTrackWindow -> GetToolBar()), COMMAND_TOGGLE_LOOP);
	opRepository -> AddCommand(EM_new CommandTogglePlayback(m_opTrackWindow -> GetToolBar()), COMMAND_TOGGLE_PLAYBACK);
	opRepository -> AddCommand(EM_new CommandToggleRelativeMove(), COMMAND_TOGGLE_RELATIVE_MOVE);
	opRepository -> AddCommand(EM_new CommandToggleSnapping(), COMMAND_TOGGLE_SNAPPING);
	opRepository -> AddCommand(EM_new CommandToggleViewClipContent(m_opTrackWindow -> GetMenuBar()), COMMAND_TOGGLE_VIEW_CLIP_CONTENT);
	opRepository -> AddCommand(EM_new CommandTracksSelectAll(m_opTrackWindow), COMMAND_TRACKS_SELECT_ALL);
	opRepository -> AddCommand(EM_new CommandTracksSelectInvert(m_opTrackWindow), COMMAND_TRACKS_SELECT_INVERT);
	opRepository -> AddCommand(EM_new CommandTracksSelectNone(m_opTrackWindow), COMMAND_TRACKS_SELECT_NONE);
	opRepository -> AddCommand(EM_new CommandUndo(), COMMAND_UNDO);
	opRepository -> AddCommand(EM_new CommandUngroupClips(m_opTrackWindow -> GetTrackView()), COMMAND_UNGROUP_CLIPS);
	opRepository -> AddCommand(EM_new CommandUpdateInstanceInMediaPoolView(), COMMAND_UPDATE_INSTANCE_IN_MEDIA_POOL_VIEW);
	opRepository -> AddCommand(EM_new CommandUtilityButtonPressed(m_opTrackWindow -> GetUtilityView()), COMMAND_UTILITY_BUTTON_PRESSED);
	opRepository -> AddCommand(EM_new CommandWriteStatusBarMessage(), COMMAND_WRITE_STATUS_BAR_MESSAGE);
	opRepository -> AddCommand(EM_new CommandZoomInHorizontal(), COMMAND_ZOOM_IN_HORIZONTAL);
	opRepository -> AddCommand(EM_new CommandZoomInTracksVertical(m_opTrackWindow), COMMAND_ZOOM_IN_TRACKS_VERTICAL);
	opRepository -> AddCommand(EM_new CommandZoomInTrackVertical(m_opTrackWindow), COMMAND_ZOOM_IN_TRACK_VERTICAL);
	opRepository -> AddCommand(EM_new CommandZoomOutHorizontal(), COMMAND_ZOOM_OUT_HORIZONTAL);
	opRepository -> AddCommand(EM_new CommandZoomOutTracksVertical(m_opTrackWindow), COMMAND_ZOOM_OUT_TRACKS_VERTICAL);
	opRepository -> AddCommand(EM_new CommandZoomOutTrackVertical(m_opTrackWindow), COMMAND_ZOOM_OUT_TRACK_VERTICAL);
	opRepository -> AddCommand(EM_new UndoCommandAddRubberBandNodes(m_opTrackWindow -> GetTrackView()), UNDO_COMMAND_ADD_RUBBERBAND_NODES);
	opRepository -> AddCommand(EM_new UndoCommandMoveRubberBandNodes(m_opTrackWindow -> GetTrackView()), UNDO_COMMAND_MOVE_RUBBERBAND_NODES);
	opRepository -> AddCommand(EM_new UndoCommandDeleteRubberBandNodes(m_opTrackWindow -> GetTrackView()), UNDO_COMMAND_DELETE_RUBBERBAND_NODES);
	opRepository -> AddCommand(EM_new UndoCommandChangeClipResize(m_opTrackWindow -> GetTrackView(), NULL, NULL, EMRect()), UNDO_COMMAND_CHANGE_CLIP_RESIZE);
	opRepository -> AddCommand(EM_new UndoCommandChangeClipSelection(m_opTrackWindow -> GetTrackView(), NULL), UNDO_COMMAND_CHANGE_CLIP_SELECTION);
	opRepository -> AddCommand(EM_new UndoCommandResizeMIDIEvent(m_opTrackWindow -> GetTrackView()), UNDO_COMMAND_RESIZE_MIDI_EVENT);

	EMMediaCommandFactory* opMediaCommandFactory = InstanceMediaCommandFactory();

	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_ADD_NODE), MEDIA_COMMAND_ADD_NODE);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_ASSIGN_VIDEO_TRANSITION_TRACK), MEDIA_COMMAND_ASSIGN_VIDEO_TRANSITION_TRACK);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_CLONE_CLIP), MEDIA_COMMAND_CLONE_CLIP);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_CREATE_AUDIO_TRACK), MEDIA_COMMAND_CREATE_AUDIO_TRACK);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_CREATE_EFFECT_TRACK), MEDIA_COMMAND_CREATE_EFFECT_TRACK);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_CREATE_MIDI_EVENT), MEDIA_COMMAND_CREATE_MIDI_EVENT);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_CREATE_MIDI_TRACK), MEDIA_COMMAND_CREATE_MIDI_TRACK);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_CREATE_VIDEO_TRACK), MEDIA_COMMAND_CREATE_VIDEO_TRACK);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_CREATE_VIDEO_TRANSITION_CLIP), MEDIA_COMMAND_CREATE_VIDEO_TRANSITION_CLIP);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_CREATE_VIDEO_TRANSITION_TRACK), MEDIA_COMMAND_CREATE_VIDEO_TRANSITION_TRACK);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_DELETE_CLIP), MEDIA_COMMAND_DELETE_CLIP);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_DELETE_EFFECT), MEDIA_COMMAND_DELETE_EFFECT);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_DELETE_MIDI_EVENTS), MEDIA_COMMAND_DELETE_MIDI_EVENTS);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_DELETE_NODE), MEDIA_COMMAND_DELETE_NODE);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_DELETE_TRACK), MEDIA_COMMAND_DELETE_TRACK);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_DELETE_VIDEO_CLIP_CONTENT), MEDIA_COMMAND_DELETE_VIDEO_CLIP_CONTENT);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_DESTRUCTIVE_CROP), MEDIA_COMMAND_DESTRUCTIVE_CROP);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_DESTRUCTIVE_EDIT), MEDIA_COMMAND_DESTRUCTIVE_EDIT);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_DESTRUCTIVE_FADE_IN), MEDIA_COMMAND_DESTRUCTIVE_FADE_IN);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_DESTRUCTIVE_FADE_OUT), MEDIA_COMMAND_DESTRUCTIVE_FADE_OUT);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_DESTRUCTIVE_NORMALIZE), MEDIA_COMMAND_DESTRUCTIVE_NORMALIZE);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_DESTRUCTIVE_REVERSE), MEDIA_COMMAND_DESTRUCTIVE_REVERSE);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_DESTRUCTIVE_SILENCE), MEDIA_COMMAND_DESTRUCTIVE_SILENCE);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_DROP_MEDIA), MEDIA_COMMAND_DROP_MEDIA);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_INSERT_EFFECT), MEDIA_COMMAND_INSERT_EFFECT);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_IMPORT_MEDIA), MEDIA_COMMAND_IMPORT_MEDIA);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_LOAD_EFFECT_PRESET), MEDIA_COMMAND_LOAD_EFFECT_PRESET);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_MOVE_CLIP), MEDIA_COMMAND_MOVE_CLIP);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_MOVE_MIDI_EVENTS), MEDIA_COMMAND_MOVE_MIDI_EVENTS);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_MOVE_NODE), MEDIA_COMMAND_MOVE_NODE);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_MOVE_TRACK), MEDIA_COMMAND_MOVE_TRACK);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_PLAY_BUFFER), MEDIA_COMMAND_PLAY_BUFFER);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_PREPARE_RECORDING), MEDIA_COMMAND_PREPARE_RECORDING);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_QUANTIZE_MIDI_EVENTS), MEDIA_COMMAND_QUANTIZE_MIDI_EVENTS);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_REFRESH_PREVIEW), MEDIA_COMMAND_REFRESH_PREVIEW);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_RESET_MIDI_DEVICES), MEDIA_COMMAND_RESET_MIDI_DEVICES);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_RESIZE_MIDI_EVENT), MEDIA_COMMAND_RESIZE_MIDI_EVENT);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_RESIZE_VIDEO_TRANSITION_CLIP), MEDIA_COMMAND_RESIZE_VIDEO_TRANSITION_CLIP);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SHOW_EFFECT_DIALOG), MEDIA_COMMAND_SHOW_EFFECT_DIALOG);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SEEK), MEDIA_COMMAND_SEEK);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SEND_MIDI_EVENT), MEDIA_COMMAND_SEND_MIDI_EVENT);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_ACTIVE_MIDI_TRACK), MEDIA_COMMAND_SET_ACTIVE_MIDI_TRACK);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_AUDIO_ENCODER), MEDIA_COMMAND_SET_AUDIO_ENCODER);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_AUDIO_RENDER_PARAMETERS), MEDIA_COMMAND_SET_AUDIO_RENDER_PARAMETERS);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_CLIP_MARK_IN), MEDIA_COMMAND_SET_CLIP_MARK_IN);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_CLIP_MARK_OUT), MEDIA_COMMAND_SET_CLIP_MARK_OUT);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_CLIP_NAME), MEDIA_COMMAND_SET_CLIP_NAME);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_CLIP_OUTPUT), MEDIA_COMMAND_SET_CLIP_OUTPUT);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_EFFECT_TRACK_OUTPUT ), MEDIA_COMMAND_SET_EFFECT_TRACK_OUTPUT);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_EFFECT), MEDIA_COMMAND_SET_EFFECT);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_EFFECT_DRY_WET_MIX), MEDIA_COMMAND_SET_EFFECT_DRY_WET_MIX);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_ENCODER_FAMILY), MEDIA_COMMAND_SET_ENCODER_FAMILY);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_ENCODER_PROPERTY), MEDIA_COMMAND_SET_ENCODER_PROPERTY);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_MAGIC_SPEED_VALUE), MEDIA_COMMAND_SET_MAGIC_SPEED_VALUE);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_PERSIST_TRANSITION_SETTINGS), MEDIA_COMMAND_SET_PERSIST_TRANSITION_SETTINGS);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_PROJECT_BPM), MEDIA_COMMAND_SET_PROJECT_BPM);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_RENDER_TO_DISK_OUTPUT_FILENAME), MEDIA_COMMAND_SET_RENDER_TO_DISK_OUTPUT_FILENAME);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_RENDERING_FLAG), MEDIA_COMMAND_SET_RENDERING_FLAG);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_TRACK_FADER), MEDIA_COMMAND_SET_TRACK_FADER);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_TRACK_INPUT), MEDIA_COMMAND_SET_TRACK_INPUT);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_TRACK_KEY_OFFSET), MEDIA_COMMAND_SET_TRACK_KEY_OFFSET);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_TRACK_MIDI_CHANNEL), MEDIA_COMMAND_SET_TRACK_MIDI_CHANNEL);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_TRACK_MUTE_STATE), MEDIA_COMMAND_SET_TRACK_MUTE_STATE);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_TRACK_NAME), MEDIA_COMMAND_SET_TRACK_NAME);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_TRACK_OUTPUT), MEDIA_COMMAND_SET_TRACK_OUTPUT);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_TRACK_PAN), MEDIA_COMMAND_SET_TRACK_PAN);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_TRACK_VELOCITY_OFFSET), MEDIA_COMMAND_SET_TRACK_VELOCITY_OFFSET);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_VIDEO_ENCODER), MEDIA_COMMAND_SET_VIDEO_ENCODER);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_VIDEO_WORKING_SIZE), MEDIA_COMMAND_SET_VIDEO_WORKING_SIZE);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_SET_VIDEO_TRANSITION), MEDIA_COMMAND_SET_VIDEO_TRANSITION);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_START_PLAYBACK), MEDIA_COMMAND_START_PLAYBACK);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_STOP_PLAYBACK), MEDIA_COMMAND_STOP_PLAYBACK);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_TOGGLE_PLAYBACK), MEDIA_COMMAND_TOGGLE_PLAYBACK);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_TOGGLE_TRACK_ARM), MEDIA_COMMAND_TOGGLE_TRACK_ARM);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_COMMAND_TOGGLE_TRACK_SOLO), MEDIA_COMMAND_TOGGLE_TRACK_SOLO);
	
	
	
	

	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_DEVICE_EXISTS), MEDIA_INFO_COMMAND_DEVICE_EXISTS);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_ALL_ENTRY_IDS), MEDIA_INFO_COMMAND_GET_ALL_ENTRY_IDS);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_ALL_TRACK_IDS), MEDIA_INFO_COMMAND_GET_ALL_TRACK_IDS);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_AUDIO_CLIP_HIGHRES_CONTENT), MEDIA_INFO_COMMAND_GET_AUDIO_CLIP_HIGHRES_CONTENT);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_AUDIO_CLIP_LOWRES_CONTENT), MEDIA_INFO_COMMAND_GET_AUDIO_CLIP_LOWRES_CONTENT);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENCODERS), MEDIA_INFO_COMMAND_GET_AUDIO_ENCODERS);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_BIT_DEPTH), MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_BIT_DEPTH);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_FORMAT_STRING), MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_FORMAT_STRING);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_NUM_CHANNELS), MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_NUM_CHANNELS);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_NUM_SAMPLES), MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_NUM_SAMPLES);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_SAMPLE_RATE), MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_SAMPLE_RATE);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_AUDIO_RENDER_OUTPUT), MEDIA_INFO_COMMAND_GET_AUDIO_RENDER_OUTPUT);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_AUDIO_TRACK_IDS), MEDIA_INFO_COMMAND_GET_AUDIO_TRACK_IDS);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_CLIP_DURATION), MEDIA_INFO_COMMAND_GET_CLIP_DURATION);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_CLIP_ENTRY_ID), MEDIA_INFO_COMMAND_GET_CLIP_ENTRY_ID);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_CLIP_FILENAME), MEDIA_INFO_COMMAND_GET_CLIP_FILENAME);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_CLIP_INFO), MEDIA_INFO_COMMAND_GET_CLIP_INFO);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_CLIP_MARK_IN), MEDIA_INFO_COMMAND_GET_CLIP_MARK_IN);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_CLIP_MARK_OUT), MEDIA_INFO_COMMAND_GET_CLIP_MARK_OUT);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_CLIP_NAME), MEDIA_INFO_COMMAND_GET_CLIP_NAME);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_CLIP_NUM_CHANNELS), MEDIA_INFO_COMMAND_GET_CLIP_NUM_CHANNELS);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_CLIP_POSITION), MEDIA_INFO_COMMAND_GET_CLIP_POSITION);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_CODECS), MEDIA_INFO_COMMAND_GET_CODECS);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_CURRENT_AUDIO_PROCESSING_POSITION), MEDIA_INFO_COMMAND_GET_CURRENT_AUDIO_PROCESSING_POSITION);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_CURRENT_BITMAP), MEDIA_INFO_COMMAND_GET_CURRENT_BITMAP);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_CURRENT_PROJECT_POSITION), MEDIA_INFO_COMMAND_GET_CURRENT_PROJECT_POSITION);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_CURRENT_VIDEO_PROCESSING_POSITION), MEDIA_INFO_COMMAND_GET_CURRENT_VIDEO_PROCESSING_POSITION);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_DEFAULT_TRANSITION_SETTINGS), MEDIA_INFO_COMMAND_GET_DEFAULT_TRANSITION_SETTINGS);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_EFFECT_ENTRIES), MEDIA_INFO_COMMAND_GET_EFFECT_ENTRIES);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_EFFECT_ENTRY_INFO), MEDIA_INFO_COMMAND_GET_EFFECT_ENTRY_INFO);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_EFFECT_INFO), MEDIA_INFO_COMMAND_GET_EFFECT_INFO);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_EFFECT_PRESETS), MEDIA_INFO_COMMAND_GET_EFFECT_PRESETS);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_EFFECT_TRACK_ID), MEDIA_INFO_COMMAND_GET_EFFECT_TRACK_ID);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_EFFECT_TRACK_INFO), MEDIA_INFO_COMMAND_GET_EFFECT_TRACK_INFO);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_EFFECT_TRACKS), MEDIA_INFO_COMMAND_GET_EFFECT_TRACKS);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_ENCODER_NAME), MEDIA_INFO_COMMAND_GET_ENCODER_NAME);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_ENCODER_PROPERTY_PAGE), MEDIA_INFO_COMMAND_GET_ENCODER_PROPERTY_PAGE);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_ENCODER_PROPERTIES), MEDIA_INFO_COMMAND_GET_ENCODER_PROPERTIES);	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_ENTRY_DURATION), MEDIA_INFO_COMMAND_GET_ENTRY_DURATION);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME), MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_ENTRY_INFO), MEDIA_INFO_COMMAND_GET_ENTRY_INFO);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_ENTRY_USAGE_COUNT), MEDIA_INFO_COMMAND_GET_ENTRY_USAGE_COUNT);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_INPUT_NAME), MEDIA_INFO_COMMAND_GET_INPUT_NAME);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_INPUT_SIGNAL_METERS), MEDIA_INFO_COMMAND_GET_INPUT_SIGNAL_METERS);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_INPUTS), MEDIA_INFO_COMMAND_GET_INPUTS);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_MAGIC_SPEED_VALUE), MEDIA_INFO_COMMAND_GET_MAGIC_SPEED_VALUE);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_MIDI_EVENTS_FOR_CLIP), MEDIA_INFO_COMMAND_GET_MIDI_EVENTS_FOR_CLIP);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_MIDI_EVENTS_FOR_TRACK), MEDIA_INFO_COMMAND_GET_MIDI_EVENTS_FOR_TRACK);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_MIDI_SIGNAL_METER_VALUE), MEDIA_INFO_COMMAND_GET_MIDI_SIGNAL_METER_VALUE);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_MIDI_TRACK_IDS), MEDIA_INFO_COMMAND_GET_MIDI_TRACK_IDS);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_NATIVE_DIRECTX_EFFECT), MEDIA_INFO_COMMAND_GET_NATIVE_DIRECTX_EFFECT);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_NODES), MEDIA_INFO_COMMAND_GET_NODES);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_NUMBER_AUDIO_ENCODERS), MEDIA_INFO_COMMAND_GET_NUMBER_AUDIO_ENCODERS);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_NUMBER_OF_VIDEO_TRANSITIONS), MEDIA_INFO_COMMAND_GET_NUMBER_OF_VIDEO_TRANSITIONS);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_NUMBER_VIDEO_AVI_ENCODERS), MEDIA_INFO_COMMAND_GET_NUMBER_VIDEO_AVI_ENCODERS);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_OUTPUT_NAME), MEDIA_INFO_COMMAND_GET_OUTPUT_NAME);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_OUTPUTS), MEDIA_INFO_COMMAND_GET_OUTPUTS);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_PERSIST_TRANSITION_SETTINGS), MEDIA_INFO_COMMAND_GET_PERSIST_TRANSITION_SETTINGS);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_SIGNAL_METER_VALUE), MEDIA_INFO_COMMAND_GET_SIGNAL_METER_VALUE);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_TRACK_INFO), MEDIA_INFO_COMMAND_GET_TRACK_INFO);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_VIDEO_ASPECT_RATIO), MEDIA_INFO_COMMAND_GET_VIDEO_ASPECT_RATIO);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_VIDEO_AVI_ENCODERS), MEDIA_INFO_COMMAND_GET_VIDEO_AVI_ENCODERS);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_VIDEO_CLIP_CONTENT), MEDIA_INFO_COMMAND_GET_VIDEO_CLIP_CONTENT);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_VIDEO_DIMENSION), MEDIA_INFO_COMMAND_GET_VIDEO_DIMENSION);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_FORMAT_STRING), MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_FORMAT_STRING);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_FRAME_RATE), MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_FRAME_RATE);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_FRAME_SIZE), MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_FRAME_SIZE);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_NUM_FRAMES), MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_NUM_FRAMES);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_VIDEO_RENDER_OUTPUT), MEDIA_INFO_COMMAND_GET_VIDEO_RENDER_OUTPUT);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_VIDEO_TRANSITION), MEDIA_INFO_COMMAND_GET_VIDEO_TRANSITION);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_VIDEO_TRANSITION_IDS), MEDIA_INFO_COMMAND_GET_VIDEO_TRANSITION_IDS);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_GET_VIDEO_TRANSITION_NAME), MEDIA_INFO_COMMAND_GET_VIDEO_TRANSITION_NAME);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_HAS_ENCODER_PROPERTY_PAGE), MEDIA_INFO_COMMAND_HAS_ENCODER_PROPERTY_PAGE);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_HAS_QUALITY), MEDIA_INFO_COMMAND_HAS_QUALITY);
	opRepository -> AddCommand(opMediaCommandFactory -> CreateMediaCommand(MEDIA_INFO_COMMAND_IS_PLAYING), MEDIA_INFO_COMMAND_IS_PLAYING);

	DeleteMediaCommandFactory();

	EMProjectCommandFactory* opProjectCommandFactory = InstanceProjectCommandFactory();

	opRepository -> AddCommand(opProjectCommandFactory -> CreateCommand(PROJECT_COMMAND_ADD_PROJECT_DATA_CONTAINER), PROJECT_COMMAND_ADD_PROJECT_DATA_CONTAINER);
	opRepository -> AddCommand(opProjectCommandFactory -> CreateCommand(PROJECT_COMMAND_CLOSE_PROJECT), PROJECT_COMMAND_CLOSE_PROJECT);
	opRepository -> AddCommand(opProjectCommandFactory -> CreateCommand(PROJECT_COMMAND_CREATE_PROJECT), PROJECT_COMMAND_CREATE_PROJECT);
	opRepository -> AddCommand(opProjectCommandFactory -> CreateCommand(PROJECT_COMMAND_GET_DIRECTORY), PROJECT_COMMAND_GET_DIRECTORY);
	opRepository -> AddCommand(opProjectCommandFactory -> CreateCommand(PROJECT_COMMAND_LOAD_PROJECT), PROJECT_COMMAND_LOAD_PROJECT);
	opRepository -> AddCommand(opProjectCommandFactory -> CreateCommand(PROJECT_COMMAND_LOAD_TEMPLATE), PROJECT_COMMAND_LOAD_TEMPLATE);
	opRepository -> AddCommand(opProjectCommandFactory -> CreateCommand(PROJECT_COMMAND_READY_FOR_USE), PROJECT_COMMAND_READY_FOR_USE);
	opRepository -> AddCommand(opProjectCommandFactory -> CreateCommand(PROJECT_COMMAND_SAVE_NEEDED), PROJECT_COMMAND_SAVE_NEEDED);
	opRepository -> AddCommand(opProjectCommandFactory -> CreateCommand(PROJECT_COMMAND_SAVE_PROJECT), PROJECT_COMMAND_SAVE_PROJECT);
	opRepository -> AddCommand(opProjectCommandFactory -> CreateCommand(PROJECT_COMMAND_LOAD_SYSTEM_SETTINGS), PROJECT_COMMAND_LOAD_SYSTEM_SETTINGS);
	opRepository -> AddCommand(opProjectCommandFactory -> CreateCommand(PROJECT_COMMAND_SAVE_SYSTEM_SETTINGS), PROJECT_COMMAND_SAVE_SYSTEM_SETTINGS);

	DeleteProjectCommandFactory();


	EMNetworkCommandFactory* opNetworkCommandFactory = InstanceNetworkCommandFactory();

	opRepository -> AddCommand(opNetworkCommandFactory -> CreateNetworkCommand(NETWORK_COMMAND_ADD_USER), NETWORK_COMMAND_ADD_USER);
	opRepository -> AddCommand(opNetworkCommandFactory -> CreateNetworkCommand(NETWORK_COMMAND_DELETE_USER), NETWORK_COMMAND_DELETE_USER);
	opRepository -> AddCommand(opNetworkCommandFactory -> CreateNetworkCommand(NETWORK_COMMAND_FIND_USER), NETWORK_COMMAND_FIND_USER);
	opRepository -> AddCommand(opNetworkCommandFactory -> CreateNetworkCommand(NETWORK_COMMAND_RECEIVE_FILE), NETWORK_COMMAND_RECEIVE_FILE);
	opRepository -> AddCommand(opNetworkCommandFactory -> CreateNetworkCommand(NETWORK_COMMAND_SEND_FILE), NETWORK_COMMAND_SEND_FILE);
	opRepository -> AddCommand(opNetworkCommandFactory -> CreateNetworkCommand(NETWORK_COMMAND_SEND_MESSAGE), NETWORK_COMMAND_SEND_MESSAGE);

	opRepository -> AddCommand(opNetworkCommandFactory -> CreateNetworkCommand(NETWORK_INFO_COMMAND_GET_MESSAGE), NETWORK_INFO_COMMAND_GET_MESSAGE);
	opRepository -> AddCommand(opNetworkCommandFactory -> CreateNetworkCommand(NETWORK_INFO_COMMAND_GET_NEXT_DOWNLOADED_FILE), NETWORK_INFO_COMMAND_GET_NEXT_DOWNLOADED_FILE);
	opRepository -> AddCommand(opNetworkCommandFactory -> CreateNetworkCommand(NETWORK_INFO_COMMAND_GET_NEXT_FILE_INFO), NETWORK_INFO_COMMAND_GET_NEXT_FILE_INFO);
	opRepository -> AddCommand(opNetworkCommandFactory -> CreateNetworkCommand(NETWORK_INFO_COMMAND_GET_NEXT_FOUND_USER), NETWORK_INFO_COMMAND_GET_NEXT_FOUND_USER);
	opRepository -> AddCommand(opNetworkCommandFactory -> CreateNetworkCommand(NETWORK_INFO_COMMAND_GET_USER_INFOS), NETWORK_INFO_COMMAND_GET_USER_INFOS);
	opRepository -> AddCommand(opNetworkCommandFactory -> CreateNetworkCommand(NETWORK_INFO_COMMAND_NUMBER_OF_USERS), NETWORK_INFO_COMMAND_NUMBER_OF_USERS);
	opRepository -> AddCommand(opNetworkCommandFactory -> CreateNetworkCommand(NETWORK_INFO_COMMAND_USER_STATUS_MESSAGE), NETWORK_INFO_COMMAND_USER_STATUS_MESSAGE);

	DeleteNetworkCommandFactory();


	eo << "Commands added" << ef;
}

void ApplicationInit::AddListeners()
{
	eo << "Adding listeners..." << ef;
	m_opTrackWindow -> GetButtonBar() -> AddListener(m_opTrackWindow);
	m_opTrackWindow -> GetButtonBar() -> AddListener(m_opTrackWindow -> GetTrackInfo());
	m_opTrackWindow -> GetButtonBar() -> AddListener(m_opTrackWindow -> GetTrackView());
	EMGTrackRepository::Instance() -> AddListener(m_opTrackWindow -> GetTrackInfo());
	EMGTrackRepository::Instance() -> AddListener(m_opTrackWindow -> GetTrackView());
	EMGClipRepository::Instance() -> AddListener(m_opTrackWindow -> GetTrackView());
	eo << "Listeners added" << ef;
}

void ApplicationInit::InitObjects()
{
	if(m_opTrackWindow -> GetWindow() -> Lock())
	{
		EMGTrackRepository::Instance() -> Init();
		m_opTrackWindow -> Init();
		//EMImageBufferRepository::Instance() -> SetView(m_opTrackWindow -> GetTrackView() -> GetView());
		m_opTrackWindow -> GetWindow() -> Unlock();
	}
}

void ApplicationInit::InstantiateSingletons()
{
	eo << "Creating system services..." << ef;
	EMSettingsRepository::Instance();
	EMGEffectsRepository::Instance();
	EMCommandRepository::Instance();
	EMGUIUtilities::Instance();
	EMFactory::Instance();
	EMImageBufferRepository::Instance();
	EMExceptionHandler::Instance();
	m_opResourceRepository = EM_new EMResourceRepository();
	EMThreadRepository::Instance();
	InstanceMediaEngine();
	EMGClipRepository::Instance();
	EMGTrackRepository::Instance();
	EMKeyboard::Instance();
//	ProjectManager::Instance();
	TrackWindowStatusBar::Instance();
	EMMessageBox::Instance();
	ExplorerView::Instance();
	MediaPoolView::Instance();
	ColaborationView::Instance();
	EMNetworkEngine::Instance();
//#ifdef PLATFORM_WINDOWS
//	EMWinMessagePump::Instance();
//#endif
	eo << "System services created" << ef;
}

bool ApplicationInit::LoadResources()
{
	eo << "Loading resources..." << ef;
	int oID(0);
	uint32 oDirID(DIRECTORY_ID_SYSTEM);
// Let's cheat a little so that we don't need to init all commands before loading the resource file
//	string* opString = static_cast<string*>(EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_GET_DIRECTORY, &oID, &oDirID));
//	m_opResourceRepository -> LoadResources(*opString);
	bool vResult(false);
#ifdef PLATFORM_WINDOWS // Yep, hackorama!
#ifdef _DEBUG
//	char vString[255];
	GetCurrentDirectory(255, m_vDirApplication);
	vResult = m_opResourceRepository -> LoadResources(string(m_vDirApplication) + "\\system");
#endif // _DEBUG
#ifndef _DEBUG
//	char vString[MAX_PATH];
	m_vDirApplication[0] = '\0';
	HRESULT vWinResult;
	HKEY vRegistryKey;

	vWinResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("software\\Elegant Media\\Titan"), 0, KEY_READ, &vRegistryKey);
	if(vWinResult == ERROR_SUCCESS)
	{
		char vpValueName[MAX_PATH];
		uint32 vValueNameSize = MAX_PATH;
		
		uchar vpData[MAX_PATH];
		uint32 vDataSize = MAX_PATH;
		
		uint32  vIndex(0);
		uint32 vType(0);

		while(true)
		{
			vWinResult = RegEnumValue(vRegistryKey, vIndex, vpValueName, &vValueNameSize, NULL, &vType, vpData, &vDataSize);
			if(vWinResult == ERROR_SUCCESS)
			{
				switch(vType)
				{
				case REG_SZ:
					if(strcmpi(vpValueName, "ApplicationPath") == 0)
						memcpy(m_vDirApplication, (char*) vpData, vDataSize); // vpData is the ApplicationPath
					break;
				default:
					break;
				}
			}
			else if(vWinResult == ERROR_NO_MORE_ITEMS)
				break;

			vValueNameSize = MAX_PATH;
			vDataSize = MAX_PATH;
			vIndex++;
		}
		RegCloseKey(vRegistryKey);
	}
	if(m_vDirApplication[0] != '\0')
		vWinResult = m_opResourceRepository -> LoadResources(string(m_vDirApplication) + "\\system");
	else
	{
		GetCurrentDirectory(255, m_vDirApplication);
		vWinResult = m_opResourceRepository -> LoadResources(string(m_vDirApplication) + "\\system");
	}
	vResult = (vWinResult & 0x1) != 0;
#endif // _DEBUG
#endif // PLATFORM_WINDOWS

	eo << "Resources loaded" << ef;
	
	if(!vResult)
	{
		char vpDialogMessage[255];
		sprintf(vpDialogMessage, "The file \"resources.emr\" could not be found/processed correctly at the location\n%s\nThe applicaton will now close", m_vDirApplication);
		EMFactory::Instance() -> CreateMessageBox() -> DisplayDialog(NULL, vpDialogMessage, "File Error", EM_DIALOG_BUTTON_OK | EM_DIALOG_ICON_ERROR);
	}

	return vResult;
}

uint32 ApplicationInit::ProcessCommandLine(const char* p_vpCommandLine, string* p_opProjectFile)
{
	if(p_vpCommandLine == NULL)
		return 0;

	string oCommandLine(p_vpCommandLine);
	uint32 vFlags(0);

	if(oCommandLine.find("/sap") != string::npos)
		vFlags |= EM_CL_START_AUDIO_PROCESSING;

	if(oCommandLine.find("/svp") != string::npos)
		vFlags |= EM_CL_START_VIDEO_PROCESSING;

	if(oCommandLine.find("/smp") != string::npos)
		vFlags |= EM_CL_START_MIDI_PROCESSING;

	if(oCommandLine.find("/sfmi") != string::npos)
		vFlags |= EM_CL_SEARCH_FOR_MIDI_INPUTS;

	if(oCommandLine.find("/sfmo") != string::npos)
		vFlags |= EM_CL_SEARCH_FOR_MIDI_OUTPUTS;

	if(oCommandLine.find("/sfst") != string::npos)
		vFlags |= EM_CL_SEARCH_FOR_SMPTE_TRANSITIONS;

	if(oCommandLine.find("/sfnve") != string::npos)
		vFlags |= EM_CL_SEARCH_FOR_NATIVE_VIDEO_EFFECTS;

	if(oCommandLine.find("/sfai") != string::npos)
		vFlags |= EM_CL_SEARCH_FOR_AUDIO_INPUTS;

	if(oCommandLine.find("/sfvi") != string::npos)
		vFlags |= EM_CL_SEARCH_FOR_VIDEO_INPUTS;

	if(oCommandLine.find("/sfao") != string::npos)
		vFlags |= EM_CL_SEARCH_FOR_AUDIO_OUTPUTS;

	if(oCommandLine.find("/sfvo") != string::npos)
		vFlags |= EM_CL_SEARCH_FOR_VIDEO_OUTPUTS;

	if(oCommandLine.find("/sfdp") != string::npos)
		vFlags |= EM_CL_SEARCH_FOR_DIRECTX_PLUGINS;

	if(oCommandLine.find("/sfae") != string::npos)
		vFlags |= EM_CL_SEARCH_FOR_AUDIO_ENCODERS;

	if(oCommandLine.find("/sfve") != string::npos)
		vFlags |= EM_CL_SEARCH_FOR_VIDEO_ENCODERS;

	if(oCommandLine.find("/dnpd") != string::npos)
		vFlags |= EM_CL_DISPLAY_NEW_PROJECT_DIALOG;

	if(oCommandLine.find("/dss") != string::npos)
		vFlags |= EM_CL_DISPLAY_SPLASH_SCREEN;

	if(oCommandLine.find("/ddw") != string::npos)
	{
		vFlags |= EM_CL_DISPLAY_DEBUG_WINDOW;
		eo << "*ENABLE_DEBUG_WINDOW" << ef;
	}
	else
		eo << "*DISABLE_DEBUG_WINDOW" << ef;

	if(oCommandLine.find("/wdl") != string::npos)
	{
		vFlags |= EM_CL_WRITE_DEBUG_LOG;
		eo << "*ENABLE_DEBUG_LOG" << ef;
	}
	else
		eo << "*DISABLE_DEBUG_LOG" << ef;
	
	if(oCommandLine.find(".tpf") != string::npos)
	{
		int vEndPos = oCommandLine.find(".tpf");
		int vBeginPos = oCommandLine.rfind(' ', vEndPos);
		*p_opProjectFile = oCommandLine.substr(vBeginPos + 1, vEndPos + 4);
	}
	else
		*p_opProjectFile = "";

	return vFlags;
}

void ApplicationInit::RegisterMediaObjects()
{
	eo << "Registering media objects..." << ef;

	EMView* opTrackInfoView = m_opTrackWindow -> GetTrackInfo() -> GetView();
	EMView* opTrackViewView = m_opTrackWindow -> GetTrackView() -> GetView();

	EMGTrackRepository::Instance() -> RegisterTrackType(EM_TYPE_ANY_AUDIO, 
					EM_new AudioGTrackInfo(opTrackInfoView, m_opTrackWindow -> GetButtonBar()), 
					EM_new AudioGClipContainer(EMRect(), opTrackViewView));
	EMGTrackRepository::Instance() -> RegisterTrackType(EM_TYPE_ANY_VIDEO, 
					EM_new VideoGTrackInfo(opTrackInfoView, m_opTrackWindow -> GetButtonBar()), 
					EM_new VideoGClipContainer(EMRect(), opTrackViewView));
	EMGTrackRepository::Instance() -> RegisterTrackType(EM_TYPE_MIDI, 
					EM_new MIDIGTrackInfo(opTrackInfoView, m_opTrackWindow -> GetButtonBar()), 
					EM_new MIDIGClipContainer(EMRect(), opTrackViewView));
	EMGTrackRepository::Instance() -> RegisterTrackType(EM_TYPE_TRANSITION, 
					EM_new TransitionGTrackInfo(opTrackInfoView, m_opTrackWindow -> GetButtonBar()), 
					EM_new TransitionGClipContainer(EMRect(), opTrackViewView));
	EMGClipRepository::Instance() -> RegisterClipType(EM_TYPE_ANY_AUDIO, EM_new AudioGClip(string(), 0, 0, NULL, -1, opTrackViewView));
	EMGClipRepository::Instance() -> RegisterClipType(EM_TYPE_ANY_VIDEO, EM_new VideoGClip(string(), 0, 0, NULL, -1, opTrackViewView));
	EMGClipRepository::Instance() -> RegisterClipType(EM_TYPE_MIDI, EM_new MIDIGClip(string(), 0, 0, NULL, -1, opTrackViewView));
	EMGClipRepository::Instance() -> RegisterClipType(EM_TYPE_TRANSITION, EM_new TransitionGClip(string(), 0, 0, NULL, -1, opTrackViewView));
	eo << "Media objects registered" << ef;
}

/*
The following classes are registered as project data containers (in correct order):
EMSettingsRepository
EMMediaEngine
EMGTrackRepository
EMGClipRepository
TrackInfo
TrackView
TrackWindow
EMImageBufferRepository
TrackWindowToolBar
TrackInfoButtonBar
*/
void ApplicationInit::RegisterProjectDataContainers()
{
	eo << "Registering project data containers..." << ef;
	// It's important that EMSettingsRepository is first, since it sets some default settings when creating a project
	EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_ADD_PROJECT_DATA_CONTAINER, dynamic_cast<EMProjectDataContainer*>(EMSettingsRepository::Instance()));
	
	InitializeMediaEngine(EMSettingsRepository::Instance(), EMCommandRepository::Instance());

	// EMGTrackRepository must come before EMGClipRepository
	EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_ADD_PROJECT_DATA_CONTAINER, dynamic_cast<EMProjectDataContainer*>(EMGTrackRepository::Instance()));
	EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_ADD_PROJECT_DATA_CONTAINER, dynamic_cast<EMProjectDataContainer*>(EMGClipRepository::Instance()));
	// TrackInfo and TrackView should come after the GUI repositories since they clean up the GUI when ClearData() is called
	EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_ADD_PROJECT_DATA_CONTAINER, dynamic_cast<EMProjectDataContainer*>(m_opTrackWindow -> GetTrackInfo()));
	EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_ADD_PROJECT_DATA_CONTAINER, dynamic_cast<EMProjectDataContainer*>(m_opTrackWindow -> GetTrackView()));
	EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_ADD_PROJECT_DATA_CONTAINER, dynamic_cast<EMProjectDataContainer*>(EMImageBufferRepository::Instance()));
	EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_ADD_PROJECT_DATA_CONTAINER, dynamic_cast<EMProjectDataContainer*>(m_opTrackWindow -> GetToolBar()));
	EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_ADD_PROJECT_DATA_CONTAINER, dynamic_cast<EMProjectDataContainer*>(m_opTrackWindow -> GetButtonBar()));
	EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_ADD_PROJECT_DATA_CONTAINER, dynamic_cast<EMProjectDataContainer*>(m_opTrackWindow -> GetUtilityView()));
	EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_ADD_PROJECT_DATA_CONTAINER, dynamic_cast<EMProjectDataContainer*>(m_opTrackWindow -> GetTimeLine()));
	EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_ADD_PROJECT_DATA_CONTAINER, dynamic_cast<EMProjectDataContainer*>(MediaPoolView::Instance()));
	// TrackWindow should be last since it rearranges its views according to their states
	EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_ADD_PROJECT_DATA_CONTAINER, dynamic_cast<EMProjectDataContainer*>(m_opTrackWindow));
	eo << "Registering project data containers complete" << ef;
}

void ApplicationInit::SetKeyBindings()
{
	eo << "Setting keybindings..." << ef;
	EMKeyboard::Instance() -> BindKey('a', 0, COMMAND_ADD_AUDIO_TRACK);
	EMKeyboard::Instance() -> BindKey('x', 0, COMMAND_EXPAND_TRACK_INFO);
//	EMKeyboard::Instance() -> BindKey('e', 0, COMMAND_EXPAND_TIMELINE);
	EMKeyboard::Instance() -> BindKey('+', 0, COMMAND_ZOOM_IN_TRACK_VERTICAL);
	EMKeyboard::Instance() -> BindKey('-', 0, COMMAND_ZOOM_OUT_TRACK_VERTICAL);
	EMKeyboard::Instance() -> BindKey(EM_PAGE_DOWN, EM_SHIFT_KEY, COMMAND_ZOOM_IN_TRACKS_VERTICAL);
	EMKeyboard::Instance() -> BindKey(EM_PAGE_UP, EM_SHIFT_KEY, COMMAND_ZOOM_OUT_TRACKS_VERTICAL);
	EMKeyboard::Instance() -> BindKey(EM_DELETE, 0, COMMAND_DELETE_CLIPS_EVENTS_NODES);
	EMKeyboard::Instance() -> BindKey(EM_DELETE, EM_CONTROL_KEY, COMMAND_DELETE_TRACKS);
	EMKeyboard::Instance() -> BindKey(EM_PAGE_UP, 0, COMMAND_ZOOM_OUT_HORIZONTAL);
	EMKeyboard::Instance() -> BindKey(EM_PAGE_DOWN, 0, COMMAND_ZOOM_IN_HORIZONTAL);
//	EMKeyboard::Instance() -> BindKey('r', 0, COMMAND_TOGGLE_RELATIVE_MOVE);
	EMKeyboard::Instance() -> BindKey('s', 0, COMMAND_TOGGLE_SNAPPING);
	EMKeyboard::Instance() -> BindKey('v', 0, COMMAND_ADD_VIDEO_TRACK);
	// FIXME: Comment out the binding below for the final release
	//EMKeyboard::Instance() -> BindKey('l', EM_CONTROL_KEY, COMMAND_LOAD_MEDIA);
	EMKeyboard::Instance() -> BindKey('l', 0, COMMAND_TOGGLE_LOOP);
	EMKeyboard::Instance() -> BindKey('z', EM_CONTROL_KEY, COMMAND_UNDO);
//	EMKeyboard::Instance() -> BindKey('u', EM_CONTROL_KEY, COMMAND_REDO);
	EMKeyboard::Instance() -> BindKey(EM_HOME, 0, COMMAND_PROJECT_BEGIN);
	EMKeyboard::Instance() -> BindKey(EM_HOME, EM_SHIFT_KEY, COMMAND_REGION_BEGIN);
	EMKeyboard::Instance() -> BindKey(EM_END, 0, COMMAND_PROJECT_END);
	EMKeyboard::Instance() -> BindKey(EM_END, EM_SHIFT_KEY, COMMAND_REGION_END);
	EMKeyboard::Instance() -> BindKey(EM_LEFT_ARROW, 0, COMMAND_REWIND);
	EMKeyboard::Instance() -> BindKey(EM_RIGHT_ARROW, 0, COMMAND_FAST_FORWARD);
	EMKeyboard::Instance() -> BindKey(EM_SPACE, 0, COMMAND_TOGGLE_PLAYBACK);
	// FIXME. Prepare recording -> start recording?
	EMKeyboard::Instance() -> BindKey('*', 0, COMMAND_PREPARE_RECORDING);
	EMKeyboard::Instance() -> BindKey('1', 0, COMMAND_SET_TRACK_EDIT);
	EMKeyboard::Instance() -> BindKey('2', 0, COMMAND_SET_CLIP_EDIT);
	EMKeyboard::Instance() -> BindKey('3', 0, COMMAND_SET_FX_EDIT);
	EMKeyboard::Instance() -> BindKey('a', EM_SHIFT_KEY, COMMAND_TOGGLE_EXPERT_MODE);
//	EMKeyboard::Instance() -> BindKey(EM_TAB, 0, MEDIA_INFO_COMMAND_GET_FILE_TYPE);
	EMKeyboard::Instance() -> BindKey('o', EM_CONTROL_KEY, COMMAND_OPEN_PROJECT);
	EMKeyboard::Instance() -> BindKey('n', EM_CONTROL_KEY, COMMAND_DISPLAY_NEW_PROJECT_DIALOG);
	EMKeyboard::Instance() -> BindKey('s', EM_CONTROL_KEY, COMMAND_SAVE_PROJECT);	
	EMKeyboard::Instance() -> BindKey('t', 0, COMMAND_ADD_TRANSITION_TRACK);
	EMKeyboard::Instance() -> BindKey('m', 0, COMMAND_ADD_MIDI_TRACK);
	EMKeyboard::Instance() -> BindKey('g', EM_CONTROL_KEY, COMMAND_GROUP_CLIPS);
	EMKeyboard::Instance() -> BindKey('u', EM_CONTROL_KEY, COMMAND_UNGROUP_CLIPS);
	EMKeyboard::Instance() -> BindKey(EM_UP_ARROW, 0, COMMAND_ACTIVE_MIDI_TRACK_STEP_UP);
	EMKeyboard::Instance() -> BindKey(EM_DOWN_ARROW, 0, COMMAND_ACTIVE_MIDI_TRACK_STEP_DOWN);
	EMKeyboard::Instance() -> BindKey('a', EM_CONTROL_KEY, COMMAND_CLIPS_SELECT_ALL);
	EMKeyboard::Instance() -> BindKey('a', EM_CONTROL_KEY | EM_SHIFT_KEY, COMMAND_CLIPS_SELECT_INVERT);
	EMKeyboard::Instance() -> BindKey('p', EM_CONTROL_KEY, COMMAND_DISPLAY_CLIP_PROPERTIES_DIALOG);
	EMKeyboard::Instance() -> BindKey('e', EM_CONTROL_KEY, COMMAND_DISPLAY_EXPORT_MEDIA_DIALOG);
	EMKeyboard::Instance() -> BindKey('s', EM_SHIFT_KEY, COMMAND_DISPLAY_PROJECT_SETTINGS_DIALOG);
	//EMKeyboard::Instance() -> BindKey('q', EM_CONTROL_KEY, COMMAND_DISPLAY_QUANTIZE_DIALOG);
	//EMKeyboard::Instance() -> BindKey('v', EM_CONTROL_KEY, COMMAND_DISPLAY_SET_VELOCITY_DIALOG);
	//EMKeyboard::Instance() -> BindKey('t', EM_CONTROL_KEY, COMMAND_DISPLAY_TRANSPOSE_DIALOG);
	EMKeyboard::Instance() -> BindKey('s', EM_CONTROL_KEY, COMMAND_SAVE_PROJECT);
	EMKeyboard::Instance() -> BindKey('c', 0, COMMAND_TOGGLE_VIEW_CLIP_CONTENT);
	EMKeyboard::Instance() -> BindKey('m', EM_CONTROL_KEY, COMMAND_MIDI_TOGGLE_METRONOME);
	EMKeyboard::Instance() -> BindKey('q', EM_SHIFT_KEY, COMMAND_TOGGLE_AUTO_QUANTIZE);
	EMKeyboard::Instance() -> BindKey('q', 0, COMMAND_MIDI_QUICK_QUANTIZE);
	eo << "Keybindings set" << ef;
}

void ApplicationInit::SetUpSystem(const char* p_vpCommandLine)
{
	string oProjectFile;
	uint32 vFlags = ProcessCommandLine(p_vpCommandLine, &oProjectFile);

	if(!LoadResources())
		exit(0);

	m_opSplashWindow = EM_new SplashWindow;
	m_opSplashWindow -> Init();
	m_opSplashWindow -> CenterWindow();
	if((vFlags & EM_CL_DISPLAY_SPLASH_SCREEN) == 0)
		m_opSplashWindow -> Display();

	m_opSplashWindow -> DisplayText("System Check");

	// First check the system if all is ok. If not, exit the application
	char vSystemCheckMessage[255];
	if(!SystemCheck(vSystemCheckMessage))
	{
		char vDialogMessage[300];
		sprintf(vDialogMessage, "Titan could not be started. Reason:\n%s", vSystemCheckMessage);
		EMMessageBox* opMessageBox = EMFactory::Instance() -> CreateMessageBox();
		opMessageBox ->DisplayDialog(NULL, vDialogMessage, "Error", EM_DIALOG_BUTTON_OK | EM_DIALOG_ICON_ERROR);
		exit(0);
		//EMApplication::QuitApplication();
	}

	m_opSplashWindow -> DisplayText("Initialize Singletons");

	InstantiateSingletons();

	EMSettingsRepository::Instance() -> SetSetting(SETTING_DISABLE_FEATURES_FLAGS, EM_SETTING_UINT32, &vFlags, false);

	m_opSplashWindow -> DisplayText("Creating Main Window");

	// Create the track window
	eo << "Creating main window..." << ef;
	m_opTrackWindow = EM_new TrackWindow(); // Must not delete (it locks the app in BeOS), let the OS take care of it
	eo << "Main window created" << ef;

	// Assigning a view to the image buffer repository
	EMImageBufferRepository::Instance() -> SetView(m_opTrackWindow -> GetTrackView() -> GetView());

	// Create a target for the file dialog
	m_opFileDialogTarget = EM_new FileDialogTarget(m_opTrackWindow);

	m_opSplashWindow -> DisplayText("Adding Commands");

	AddCommands();

	m_opSplashWindow -> DisplayText("Setting Key Bindings");

	SetKeyBindings();

	//LoadResources();
	
	//m_opSplashWindow = EM_new SplashWindow;
	//m_opSplashWindow -> Init();
	//m_opSplashWindow -> CenterWindow();
	//m_opSplashWindow -> Display();

	// Creating and setting a video preview destination
	EMVideoPreview::SetInstance(m_opTrackWindow -> GetUtilityView() -> GetVideoPreviewView());

	m_opSplashWindow -> DisplayText("Generating Settings");

	// Setting default project settings and loading system settings
	EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_SET_DEFAULT_SETTINGS);
	// Default system settings are set even if they will be loaded from file, since there could be newly defined settings that aren't previously saved
	EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_SET_DEFAULT_SYSTEM_SETTINGS);
	if(!EMSettingsRepository::Instance() -> LoadSystemSettings())
		EMFactory::Instance() -> CreateMessageBox() -> DisplayDialog(NULL, "System settings could not be found, using default settings.", "Error", EM_DIALOG_BUTTON_OK | EM_DIALOG_ICON_ERROR);
	EMSettingsRepository::Instance() -> Notify(EM_SETTING_UPDATED); // Forcing an update

	// Check if Titan is registered
	bool vIsCrippled;
	bool vDisplayRegistrationDialog = ValidateRegistration(&vIsCrippled);

	m_opSplashWindow -> DisplayInfo(EMApplication::GetApplicationVersionNumber(), vIsCrippled);

	m_opSplashWindow -> DisplayText("Creating GUI Service Thread");

	// Creating service thread
	EMThreadRepository::Instance() -> CreateThread(GUI_SERVICE_THREAD_NAME, EM_THREAD_LOW_PRIORITY, GUI_SERVICE_THREAD_SLEEP_TIME);	

	m_opSplashWindow -> DisplayText("Registering Project Data Containers");

	RegisterProjectDataContainers();

	m_opSplashWindow -> DisplayText("Initializing Main Objects");

	InitObjects();
	
	m_opSplashWindow -> DisplayText("Adding Listeners");

	AddListeners();
	
	m_opSplashWindow -> DisplayText("Register Media Objects");

	RegisterMediaObjects();

	// Starting service thread
	EMThreadRepository::Instance() -> GetThread(GUI_SERVICE_THREAD_NAME) -> Start();

	m_opSplashWindow -> Hide();

//	EMFactory::Instance() -> CreateMessageBox() -> DisplayDialog(NULL, "This is a trial version of Titan v1.0 beta. It is limited to two minutes of media playback time.", "Trial Version Information", EM_DIALOG_BUTTON_OK | EM_DIALOG_ICON_INFOMATION);


	if(vDisplayRegistrationDialog)
		EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DISPLAY_REGISTRATION_DIALOG,  const_cast<char*>(oProjectFile.c_str()), &vFlags, m_opTrackWindow);

	else if(oProjectFile != "")
	{
		if(m_opTrackWindow -> GetWindow() -> Lock())
		{
			uint32 vProjectID = reinterpret_cast<uint32>(EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_LOAD_PROJECT, const_cast<char*>(oProjectFile.c_str())));
			if(vProjectID > 0)
			{
				int vNameBeginPos = oProjectFile.find_last_of(EM_FILE_SEPARATOR, oProjectFile.find_last_of(EM_FILE_SEPARATOR) - 1) + 1;
				int vNameEndPos = oProjectFile.find_last_of(EM_FILE_SEPARATOR) - 1;
				string oProjectName = oProjectFile.substr(vNameBeginPos, vNameEndPos - vNameBeginPos + 1);
				string oWindowTitle = string(EMResourceRepository::GetString(RES_SR_APPLICATION_NAME) -> c_str()) + " - " + oProjectName;
				m_opTrackWindow -> GetWindow() -> SetTitle(oWindowTitle.c_str());

				EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_CLEAR_UNDO_STACK);		
				EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_SET_UNDO_TEXTS);				

				m_opTrackWindow -> GetWindow() -> Unlock();

				EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_READY_FOR_USE);
			}
			else
			{
				m_opTrackWindow -> GetWindow() -> Unlock();
				EMFactory::Instance() -> CreateMessageBox() -> DisplayDialog(NULL, "Could not load the specified project file.\nPerhaps the file is in an incorrect format or is damaged.", "Load Project Failed", EM_DIALOG_BUTTON_OK | EM_DIALOG_ICON_ERROR);
				EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DISPLAY_NEW_PROJECT_DIALOG);
			}
		}
	}
	
	else if((vFlags & EM_CL_DISPLAY_NEW_PROJECT_DIALOG) != 0)
	{
		// Creating a "dummy" project (for debug purposes)
		eo << "Creating dummy project..." << ef;
		string oCreatePath("c:\\titan_project2");
		EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_LOAD_TEMPLATE, const_cast<char*>(oCreatePath.c_str()));
		EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_SAVE_PROJECT);
		eo << "Dummy project created and saved" << ef;
	}

	else//if((vFlags & EM_CL_DISPLAY_NEW_PROJECT_DIALOG) == 0 && oProjectFile == "")
		EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DISPLAY_NEW_PROJECT_DIALOG);

//	delete m_opSplashWindow;
}

// Returns NULL if everything looks ok
bool ApplicationInit::SystemCheck(char* p_vpErrorMessage)
{
	// Create a dummy EMView, check the resolution and bit depth of the screen
	EMView* opDummyView = EMFactory::Instance() -> CreateView(EMRect(), 0, false);
	int32 vWidth;
	int32 vHeight;
	int32 vBitDepth;
	opDummyView -> GetScreenInfo(&vWidth, &vHeight, &vBitDepth);
	delete opDummyView;
	if(vWidth < 800 || vHeight < 600 || vBitDepth < 16)
	{
		strcpy(p_vpErrorMessage, "The current screen resolution must be at least 800x600 with 16 bits per pixel (High Color)");
		return false;
	}

	#ifdef PLATFORM_WINDOWS
	//Check DX version is 8 or more, and check that quicktime 5 or later has been installed
	if( GetDXVersion() <  0x800 )
	{
		strcpy(p_vpErrorMessage, "You need to have DirectX 8.0 or higher");
		return false;
	}
	#endif

	// Media engine system check
	if(!MediaSystemCheck(p_vpErrorMessage))
		return false;
	
	return true;
}

bool ApplicationInit::ValidateRegistration(bool* p_vpIsCrippled)
{
	InitCRC();

	FILE* vTitanRegChunk = NULL;
	FILE* vTitanRegKey = NULL;

	vTitanRegChunk = fopen((string(m_vDirApplication) + "\\TitanRegChunk.trc").c_str(), "r");
	vTitanRegKey = fopen((string(m_vDirApplication) + "\\TitanRegKey.trk").c_str(), "r");

	// Setting if Titan is crippled or not.
	bool vIsCrippled(true);
/*	if(vTitanRegChunk != NULL && vTitanRegKey != NULL)
		vIsCrippled = false;
*/
	//****-Made by Jesper(2001-11-20) for checking that registration is valid
	int codeEnd;
	LPVOID decryptionStart;
	DWORD oldprotect;
	
	typedef LPVOID (* FunctionType)(LPVOID);
	FunctionType ComputeSum;
	int vChecksum = 0;
	int vChecksum2 = 0;
	BYTE* ComputeKey;
	int bytesReadCryptFile = 0;
	int bytesReadKeyFile = 0;
	char* opPtr;
	int lengthToCopy = 512;//codeEnd - codeStart;
	int vMustard = 28;
	ComputeSum = (FunctionType) new BYTE[lengthToCopy];
	ComputeKey = new BYTE[255];
	//Get project path
	int vProjectID(0);
	uint32 vDirectoryID(DIRECTORY_ID_APPLICATION);
	string oAppDirFileName = *static_cast<string*>(EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_GET_DIRECTORY, &vProjectID, &vDirectoryID)) + "\\TitanRegChunk.trc";
	string oAppDirFileName2 = *static_cast<string*>(EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_GET_DIRECTORY, &vProjectID, &vDirectoryID)) + "\\TitanRegKey.trk";

	//Read in the serial key
	FILE* fp3 = fopen(oAppDirFileName2.c_str(),"rb");
	if(fp3 !=NULL)
	{
		bytesReadCryptFile = fread(ComputeKey, 1, 255, fp3);
		fclose(fp3);

		char* key = new char[bytesReadCryptFile+1];
		key[bytesReadCryptFile] = '\0';
		memcpy(key, ComputeKey, bytesReadCryptFile);

		FILE* fp2 = fopen(oAppDirFileName.c_str(),"rb");
		if(fp2 != NULL)
		{
			bytesReadCryptFile = fread(ComputeSum, 1, lengthToCopy, fp2);
			fclose(fp2);
			opPtr = (char*)ComputeSum;

			fp2 = fopen(oAppDirFileName2.c_str(),"rb");
			if(fp2 != NULL)
			{
				bytesReadKeyFile = fread(ComputeKey, 1, 255, fp2);
				fclose(fp2);

				//Calculate checksum
				LPVOID vCheksumPointerVoid = ComputeSum;
				char* vCheksumPointer = (char*) vCheksumPointerVoid;
				for(int index = 0; index < bytesReadCryptFile; index ++)
				{
					vChecksum += (BYTE)vCheksumPointer[index];
				}

				SetCryptKeys(key);

				//Chabbelkod starts here (for the crackers)**************************************
				decryptionStart = (void*)0xff00;
				if(decryptionStart && vChecksum)
					oldprotect = 0xff00 + vChecksum;
				else
					oldprotect = 0x0000;
				decryptionStart = (void*)(oldprotect * lengthToCopy / 4 + (BYTE)&decryptionStart);
				codeEnd = oldprotect;
				for(index = 0; index < 18; index ++)
					codeEnd += (BYTE)vCheksumPointer[index] + vChecksum;
				decryptionStart = (void*)0x00ff;
				//Chabbelkod Stops here**********************************************************

				for(int aroundandaround = 0; aroundandaround < 32; aroundandaround ++)
				{
					DecryptBlock((unsigned char*)opPtr);
					opPtr += 16;
				}
				delete [] key;
				for(index = 0; index < 44; index ++)
				{
					vChecksum2 += (BYTE)vCheksumPointer[index];
				}
				//Chabbelkod starts here (for the crackers)**************************************
				decryptionStart = (void*)0xf0f0;
				if(decryptionStart && vChecksum2)
					oldprotect = 0xf0f + vChecksum2;
				else
					oldprotect = 0x000f;
				decryptionStart = (void*)(oldprotect * lengthToCopy / 4 + (BYTE)&decryptionStart);
				codeEnd = oldprotect;
				for(index = 0; index < 18; index ++)
					codeEnd += (BYTE)vCheksumPointer[index] + vChecksum2;
				decryptionStart = (void*)0xff0f;
				//Chabbelkod Stops here**********************************************************

				 vMustard = (vChecksum2 - 4164);

			}
		}
	}
	//****End of section

	bool vDisplayRegistrationDialog(vTitanRegChunk != NULL && vTitanRegKey == NULL);

	if(vTitanRegChunk != NULL)
		fclose(vTitanRegChunk);
	if(vTitanRegKey != NULL)
		fclose(vTitanRegKey);

	*p_vpIsCrippled = vMustard != 0;
	EMSettingsRepository::Instance() -> SetSetting(SETTING_SOFTWARE_IS_CRIPPLED, EM_SETTING_BOOL, p_vpIsCrippled, false, true);

	return vDisplayRegistrationDialog;
}
