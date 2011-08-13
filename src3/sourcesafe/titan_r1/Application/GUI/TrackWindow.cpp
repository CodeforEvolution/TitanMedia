#include "TrackWindow.h"

#include "CommandIDs.h"
#include "EMApplication.h"
#include "EMCommandRepository.h"
#include "EMFactory.h"
#include "EMGEffectsRepository.h"
#include "EMGUIGlobals.h"
#include "EMKeyboard.h"
#include "EMListenerRepository.h"
#include "EMMenu.h"
#include "EMMenuItem.h"
#include "EMMessages.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"
#include "EMResizeTab.h"
#include "EMResourceRepository.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMThreadRepository.h"
#include "Messages.h"
#include "ResourceIDs.h"
#include "SettingIDs.h"
#include "TrackWindowStatusBar.h"
#include "UtilityView.h"

// ****** MAJOR HACK!!!! by Johan to get LoadTemplate/NewProject to work as it should. I.e. scrollbar not outside window. This is bad hack!! ******
#ifdef PLATFORM_WINDOWS
#include <windows.h>
#endif

#define TOOL_BAR_HEIGHT 38

TrackWindow::TrackWindow() :
m_opWindowBackground(EMFactory::Instance() -> CreateView(EMRect(0, 0, 1150, 1150), EM_FOLLOW_LEFT_RIGHT | EM_FOLLOW_TOP_BOTTOM, false)),
m_oExpandButton(m_opWindowBackground.get(), EMRect(), EM_FOLLOW_LEFT | EM_FOLLOW_BOTTOM, 
					NULL, NULL, COMMAND_EXPAND_UTILITY_VIEW, NULL, NULL, NULL, true, true, true, true,
					EM_COLOR_BUTTON_DEFAULT_OFF, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Toggles Display Of The Utility View On/Off"),
m_oHorizontalZoomInButton(m_opWindowBackground.get(), EMRect(), EM_FOLLOW_RIGHT | EM_FOLLOW_BOTTOM, 
					NULL, NULL, COMMAND_ZOOM_IN_HORIZONTAL, NULL, NULL, NULL, true, true, true, true,
					EM_COLOR_BUTTON_DEFAULT_OFF, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Increases Time Resolution (Page Down)"),
m_oHorizontalZoomOutButton(m_opWindowBackground.get(), EMRect(), EM_FOLLOW_RIGHT | EM_FOLLOW_BOTTOM, 
					NULL, NULL, COMMAND_ZOOM_OUT_HORIZONTAL, NULL, NULL, NULL, true, true, true, true,
					EM_COLOR_BUTTON_DEFAULT_OFF, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Decreases Time Resolution (Page Up)"),
m_opWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(10, 21, 795, 500), "Le Window Grande", EM_WINDOW_NORMAL)),
m_opButtonBar(TrackInfoButtonBar::Instance()),
m_opMenuBar(EMFactory::Instance() -> CreateMenuBar()),
m_opScrollbarH(EMFactory::Instance() -> CreateScrollbar(EMRect(0, 1000, 1000, 1000), 0, 0, EM_HORIZONTAL)),
m_opScrollbarV(EMFactory::Instance() -> CreateScrollbar(EMRect(1000, 0, 1000, 1000), 0, 0, EM_VERTICAL)),
m_opTimeLine(EM_new TrackWindowTimeLine()),
m_opToolBar(EM_new TrackWindowToolBar()),
m_opTrackInfo(EM_new TrackInfo()),
m_opTrackView(EM_new TrackView()),
m_oVerticalZoomInButton(m_opWindowBackground.get(), EMRect(), EM_FOLLOW_RIGHT | EM_FOLLOW_BOTTOM, 
					NULL, NULL, COMMAND_ZOOM_IN_TRACKS_VERTICAL, NULL, NULL, NULL, true, true, true, true,
					EM_COLOR_BUTTON_DEFAULT_OFF, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Increases Track Height (Shift-Page Down)"),
m_oVerticalZoomOutButton(m_opWindowBackground.get(), EMRect(), EM_FOLLOW_RIGHT | EM_FOLLOW_BOTTOM, 
					NULL, NULL, COMMAND_ZOOM_OUT_TRACKS_VERTICAL, NULL, NULL, NULL, true, true, true, true,
					EM_COLOR_BUTTON_DEFAULT_OFF, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Decreases Track Height (Shift-Page Up)"),
m_vIsDirty(false)
{
	m_opResizeTab = EMResizeTab::Create(m_opWindowBackground -> GetNativeView(), m_opWindowBackground -> Frame());
	EMMenuBar::SetInstance(m_opMenuBar.get());
}

TrackWindow::~TrackWindow()
{
//delete m_opMenuBar.release();
	//delete m_opMenuBar;
	delete m_opResizeTab;
	eo << "TrackWindow::~TrackWindow" << ef;
}

void TrackWindow::BuildMenu()
{
	EMFactory* opFactory = EMFactory::Instance();
	EMMenu* opMenu;
	EMMenu* opSubMenu;
	EMMenuItem* opMenuItem;
	
	// File Header
	opMenu = opFactory -> CreateMenu(EMResourceRepository::GetString(RES_TM_FILE));
	m_opMenuBar -> AddItem(opMenu);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_NEW_PROJECT), COMMAND_DISPLAY_NEW_PROJECT_DIALOG);
	opMenu -> AddItem(opMenuItem);
//return;
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_OPEN_PROJECT), COMMAND_OPEN_PROJECT);
	opMenu -> AddItem(opMenuItem);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_SAVE_PROJECT), COMMAND_SAVE_PROJECT);
	opMenu -> AddItem(opMenuItem);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_EXPORT_MEDIA), COMMAND_DISPLAY_EXPORT_MEDIA_DIALOG);
	opMenu -> AddItem(opMenuItem);
	opMenu -> AddSeparatorItem();
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_ABOUT), COMMAND_DISPLAY_ABOUT_DIALOG);
	opMenu -> AddItem(opMenuItem);
	//opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_REGISTER), COMMAND_DISPLAY_REGISTRATION_DIALOG);
	//opMenu -> AddItem(opMenuItem);
	opMenu -> AddSeparatorItem();
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_QUIT), COMMAND_QUIT_APPLICATION);
	opMenu -> AddItem(opMenuItem);
	
	// Edit header
	opMenu = opFactory -> CreateMenu(EMResourceRepository::GetString(RES_TM_EDIT));
	m_opMenuBar -> AddItem(opMenu);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_UNDO), COMMAND_UNDO);
	opMenu -> AddItem(opMenuItem);
	opMenuItem -> SetEnabled(false);
	opMenu -> AddSeparatorItem();
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_PROJECT_SETTINGS), COMMAND_DISPLAY_PROJECT_SETTINGS_DIALOG);
	opMenu -> AddItem(opMenuItem);
	//opMenuItem -> SetEnabled(false);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_SYSTEM_SETTINGS), COMMAND_DISPLAY_SYSTEM_SETTINGS_DIALOG);
	opMenu -> AddItem(opMenuItem);
	//opMenuItem -> SetEnabled(false);
/*
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_REDO), COMMAND_REDO);
	opMenu -> AddItem(opMenuItem);
	opMenu -> AddSeparatorItem();
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_CUT), 0);
	opMenu -> AddItem(opMenuItem);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_COPY), 0);
	opMenu -> AddItem(opMenuItem);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_PASTE), 0);
	opMenu -> AddItem(opMenuItem);
*/
	
	// Transport header
	opMenu = opFactory -> CreateMenu(EMResourceRepository::GetString(RES_TM_TRANSPORT));
	m_opMenuBar -> AddItem(opMenu);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_PLAY), MEDIA_COMMAND_START_PLAYBACK);
	opMenu -> AddItem(opMenuItem);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_STOP), MEDIA_COMMAND_STOP_PLAYBACK);
	opMenu -> AddItem(opMenuItem);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_RECORD), COMMAND_PREPARE_RECORDING);
	opMenu -> AddItem(opMenuItem);
	opMenuItem -> SetEnabled(false);
	opMenu -> AddSeparatorItem();
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_GO_TO_BEGINNING), COMMAND_PROJECT_BEGIN);
	opMenu -> AddItem(opMenuItem);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_GO_TO_END), COMMAND_PROJECT_END);
	opMenu -> AddItem(opMenuItem);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_GO_TO_SELECTION), COMMAND_REGION_BEGIN);
	opMenu -> AddItem(opMenuItem);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_GO_TO_SELECTION_END), COMMAND_REGION_END);
	opMenu -> AddItem(opMenuItem);
	opMenu -> AddSeparatorItem();
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_LOOP), COMMAND_TOGGLE_LOOP);
	opMenu -> AddItem(opMenuItem);
	opMenuItem -> SetEnabled(false);
	
	// Track header
	opMenu = opFactory -> CreateMenu(EMResourceRepository::GetString(RES_TM_TRACK));
	m_opMenuBar -> AddItem(opMenu);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_CREATE_AUDIO_TRACK), COMMAND_ADD_AUDIO_TRACK);
	opMenu -> AddItem(opMenuItem);
	if(!(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_AUDIO_ENABLED)))))
		opMenuItem -> SetEnabled(false);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_CREATE_VIDEO_TRACK), COMMAND_ADD_VIDEO_TRACK);
	opMenu -> AddItem(opMenuItem);
	if(!(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_ENABLED)))))
		opMenuItem -> SetEnabled(false);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_CREATE_MIDI_TRACK), COMMAND_ADD_MIDI_TRACK);
	opMenu -> AddItem(opMenuItem);
	if(!(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_MIDI_ENABLED)))))
		opMenuItem -> SetEnabled(false);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_CREATE_TRANSITION_TRACK), COMMAND_ADD_TRANSITION_TRACK);
	opMenu -> AddItem(opMenuItem);
	opMenuItem -> SetEnabled(false);
	if(!(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_ENABLED)))))
		opMenuItem -> SetEnabled(false);
	opMenu -> AddSeparatorItem();
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_SELECT_ALL_TRACKS), COMMAND_TRACKS_SELECT_ALL);
	opMenu -> AddItem(opMenuItem);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_SELECT_NO_TRACKS), COMMAND_TRACKS_SELECT_NONE);
	opMenu -> AddItem(opMenuItem);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_INVERT_TRACK_SELECTION), COMMAND_TRACKS_SELECT_INVERT);
	opMenu -> AddItem(opMenuItem);
	opMenu -> AddSeparatorItem();
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_DELETE_TRACK), COMMAND_DELETE_TRACKS);
	opMenu -> AddItem(opMenuItem);
	opMenuItem -> SetEnabled(false);
	
	// Clip header
	opMenu = opFactory -> CreateMenu(EMResourceRepository::GetString(RES_TM_CLIP));
	m_opMenuBar -> AddItem(opMenu);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_CLIP_PROPERTIES), COMMAND_DISPLAY_CLIP_PROPERTIES_DIALOG);
	opMenu -> AddItem(opMenuItem);
	opMenuItem -> SetEnabled(false);
	opMenu -> AddSeparatorItem();
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_SELECT_ALL_CLIPS), COMMAND_CLIPS_SELECT_ALL);
	opMenu -> AddItem(opMenuItem);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_SELECT_NO_CLIPS), COMMAND_CLIPS_SELECT_NONE);
	opMenu -> AddItem(opMenuItem);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_INVERT_CLIP_SELECTION), COMMAND_CLIPS_SELECT_INVERT);
	opMenu -> AddItem(opMenuItem);
	opMenu -> AddSeparatorItem();
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_GROUP_CLIPS), COMMAND_GROUP_CLIPS);
	opMenu -> AddItem(opMenuItem);
	opMenuItem -> SetEnabled(false);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_UNGROUP_CLIPS), COMMAND_UNGROUP_CLIPS);
	opMenu -> AddItem(opMenuItem);
	opMenuItem -> SetEnabled(false);
	opMenu -> AddSeparatorItem();
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_DELETE_CLIPS), COMMAND_DELETE_CLIPS);
	opMenu -> AddItem(opMenuItem);
	opMenuItem -> SetEnabled(false);
	
	// View header
	opMenu = opFactory -> CreateMenu(EMResourceRepository::GetString(RES_TM_VIEW));
	m_opMenuBar -> AddItem(opMenu);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_CLIP_CONTENTS), COMMAND_TOGGLE_VIEW_CLIP_CONTENT);
	opMenu -> AddItem(opMenuItem);
	if(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIEW_CLIP_CONTENT_ENABLED))))
		opMenuItem -> SetMarked(true);
	
	// Audio header
	opMenu = opFactory -> CreateMenu(EMResourceRepository::GetString(RES_TM_AUDIO));
	m_opMenuBar -> AddItem(opMenu);
	if(!(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_AUDIO_ENABLED)))))
		opMenu -> SetEnabled(false);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_LOCK_AUDIO_TO_TEMPO), COMMAND_AUDIO_LOCK_TO_TEMPO);
	opMenu -> AddItem(opMenuItem);
	opMenu -> AddSeparatorItem();
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_REVERSE_AUDIO), COMMAND_AUDIO_REVERSE);
	opMenu -> AddItem(opMenuItem);
	//opMenuItem -> SetEnabled(false);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_CROP_AUDIO_SELECTION), COMMAND_AUDIO_CROP);
	opMenu -> AddItem(opMenuItem);
	//opMenuItem -> SetEnabled(false);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_SILENCE_AUDIO), COMMAND_AUDIO_SILENCE);
	opMenu -> AddItem(opMenuItem);
	//opMenuItem -> SetEnabled(false);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_NORMALIZE_AUDIO), COMMAND_AUDIO_NORMALIZE);
	opMenu -> AddItem(opMenuItem);
	//opMenuItem -> SetEnabled(false);
	opMenu -> AddSeparatorItem();
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_FADE_IN_LINEAR), COMMAND_AUDIO_FADE_IN);
	opMenu -> AddItem(opMenuItem);
	//opMenuItem -> SetEnabled(false);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_FADE_OUT_LINEAR), COMMAND_AUDIO_FADE_OUT);
	opMenu -> AddItem(opMenuItem);
	//opMenuItem -> SetEnabled(false);
	opMenu -> AddSeparatorItem();
	opSubMenu = opFactory -> CreateMenu(EMResourceRepository::GetString(RES_TM_AUDIO_PLUGIN));
	opMenu -> AddItem(opSubMenu);
	opSubMenu -> SetEnabled(false);
	// Add all found Audio DirectX plugins to this menu
	map<int32, string*>* opEffectNames = EMGEffectsRepository::Instance() -> GetEffectNames(EM_TYPE_ANY_AUDIO);
	map<int32, string*>::iterator oFXIterator = opEffectNames -> begin();
	while(oFXIterator != opEffectNames -> end())
	{
		opMenuItem = opFactory -> CreateMenuItem(oFXIterator -> second, oFXIterator -> first + COMMAND_NOT_TRIGGERED_MENU_ITEM_OFFSET);
		opSubMenu -> AddItem(opMenuItem);
		oFXIterator++;
	}
	
	// Video header
	//opMenu = opFactory -> CreateMenu(EMResourceRepository::GetString(RES_TM_VIDEO));
	//m_opMenuBar -> AddItem(opMenu);
	//if(!(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_ENABLED)))))
	//	opMenu -> SetEnabled(false);
//	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_LOCK_VIDEO_TO_TEMPO), 666);
//	opMenu -> AddItem(opMenuItem);
//	opMenu -> AddSeparatorItem();
	//opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_CHANGE_PLAYBACK_SPEED), COMMAND_DISPLAY_VIDEO_SPEED_DIALOG);
	//opMenu -> AddItem(opMenuItem);
	//opMenuItem -> SetEnabled(false);

	// MIDI header
	opMenu = opFactory -> CreateMenu(EMResourceRepository::GetString(RES_TM_MIDI));
	m_opMenuBar -> AddItem(opMenu);
	if(!(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_MIDI_ENABLED)))))
		opMenu -> SetEnabled(false);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_METRONOME), COMMAND_MIDI_TOGGLE_METRONOME);
	opMenu -> AddItem(opMenuItem);
	if(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_MIDI_METRONOME_ENABLED))))
		opMenuItem -> SetMarked(true);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_QUICK_QUANTIZE), COMMAND_MIDI_QUICK_QUANTIZE);
	opMenu -> AddItem(opMenuItem);
	/*
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_QUANTIZE), COMMAND_DISPLAY_QUANTIZE_DIALOG);
	opMenu -> AddItem(opMenuItem);
	opMenuItem -> SetEnabled(false);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_FIXED_LENGTH), COMMAND_MIDI_FIXED_LENGTH);
	opMenu -> AddItem(opMenuItem);
	opMenuItem -> SetEnabled(false);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_SET_VELOCITY), COMMAND_DISPLAY_SET_VELOCITY_DIALOG);
	opMenu -> AddItem(opMenuItem);
	opMenuItem -> SetEnabled(false);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_TRANSPOSE), COMMAND_DISPLAY_TRANSPOSE_DIALOG);
	opMenu -> AddItem(opMenuItem);
	opMenuItem -> SetEnabled(false);
	*/
	opMenu -> AddSeparatorItem();
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_RESET_DEVICES), MEDIA_COMMAND_RESET_MIDI_DEVICES);
	opMenu -> AddItem(opMenuItem);

	//Using header
	opMenu = opFactory -> CreateMenu(EMResourceRepository::GetString(RES_TM_USING));
	m_opMenuBar -> AddItem(opMenu);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_USING_AUDIO), COMMAND_TOGGLE_ENABLE_AUDIO);
	opMenu -> AddItem(opMenuItem);
	if(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_AUDIO_ENABLED))))
		opMenuItem -> SetMarked(true);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_USING_VIDEO), COMMAND_TOGGLE_ENABLE_VIDEO);
	opMenu -> AddItem(opMenuItem);
	if(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_ENABLED))))
		opMenuItem -> SetMarked(true);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_USING_MIDI), COMMAND_TOGGLE_ENABLE_MIDI);
	opMenu -> AddItem(opMenuItem);
	if(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_MIDI_ENABLED))))
		opMenuItem -> SetMarked(true);
	opMenuItem = opFactory -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_USING_ADVANCED_EDITING), COMMAND_TOGGLE_EXPERT_MODE);
	opMenu -> AddItem(opMenuItem);
	if(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EXPERT_MODE))))
		opMenuItem -> SetMarked(true);
}

bool TrackWindow::ClearData()
{
;//cout_commented_out_4_release << "TrackWindow::ClearData" << endl;
	m_oExpandButton.GetButton() -> SetOn(true);
	m_oUtilityView.SetHeight(m_oUtilityView.GetDefaultHeight());
	// The track window gets the honor of clearing the undo stack
	EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_CLEAR_UNDO_STACK);
	return true;
}

void TrackWindow::ExpandTimeLine()
{
//	;//cout_commented_out_4_release << "TrackWindow::ExpandTimeLine" << endl;
	m_opTimeLine -> SetExpand();
	LayoutViews(true);
}

void TrackWindow::ExpandTrackInfo()
{
//	;//cout_commented_out_4_release << "TrackWindow::ExpandTrackInfo" << endl;
	m_opButtonBar -> SetExpand();
}

void TrackWindow::ExpandUtilityView()
{
	if((m_oUtilityView.GetView()) -> IsHidden())
		(m_oUtilityView.GetView()) -> Show();
	else
		(m_oUtilityView.GetView()) -> Hide();
	LayoutViews(true);
}

TrackInfoButtonBar* TrackWindow::GetButtonBar()
{
	return m_opButtonBar.get();
}

EMMenuBar* TrackWindow::GetMenuBar()
{
	return m_opMenuBar.get();
}

TrackWindowTimeLine* TrackWindow::GetTimeLine()
{
	return m_opTimeLine.get();
}

TrackWindowToolBar* TrackWindow::GetToolBar()
{
	return m_opToolBar.get();
}

TrackInfo* TrackWindow::GetTrackInfo()
{
	return m_opTrackInfo.get();
}

TrackView* TrackWindow::GetTrackView()
{
	return m_opTrackView.get();
}

UtilityView* TrackWindow::GetUtilityView()
{
	return &m_oUtilityView;
}

EMWindow* TrackWindow::GetWindow()
{
	return m_opWindow.get();
}

void TrackWindow::Init()
{
	EMApplication::Instance() -> AddListener(this);
	EMSettingsRepository::Instance() -> AddListener(this);

	m_opTrackInfo -> Init();
	m_opTrackView -> Init();

	m_oExpandButton.GetButton() -> SetBitmaps(EMResourceRepository::GetBitmap(RES_EB_EXPAND_VER_ON), EMResourceRepository::GetBitmap(RES_EB_EXPAND_VER));
	m_oHorizontalZoomInButton.GetButton() -> SetBitmaps(EMResourceRepository::GetBitmap(RES_ZB_ZOOM_IN_ON), EMResourceRepository::GetBitmap(RES_ZB_ZOOM_IN));
	m_oHorizontalZoomOutButton.GetButton() -> SetBitmaps(EMResourceRepository::GetBitmap(RES_ZB_ZOOM_OUT_ON), EMResourceRepository::GetBitmap(RES_ZB_ZOOM_OUT));
	m_oVerticalZoomInButton.GetButton() -> SetBitmaps(EMResourceRepository::GetBitmap(RES_ZB_ZOOM_IN_ON), EMResourceRepository::GetBitmap(RES_ZB_ZOOM_IN));
	m_oVerticalZoomOutButton.GetButton() -> SetBitmaps(EMResourceRepository::GetBitmap(RES_ZB_ZOOM_OUT_ON), EMResourceRepository::GetBitmap(RES_ZB_ZOOM_OUT));

	char vpTitle[80];
	sprintf(vpTitle, "%s %s", EMResourceRepository::GetString(RES_SR_APPLICATION_NAME) -> c_str(), EMApplication::GetApplicationVersionNumber());
	m_opWindow -> SetTitle(vpTitle);

	m_opButtonBar -> Init();

	m_oUtilityView.SetWindow(this);

	m_opWindow -> AddChild(m_opMenuBar.get());
	m_opWindow -> AddChild(m_opWindowBackground.get());
	m_opWindowBackground -> AddChild(m_opScrollbarH.get());
	m_opWindowBackground -> AddChild(m_opScrollbarV.get());
	m_opWindowBackground -> AddChild(m_oHorizontalZoomInButton.GetView());
	m_opWindowBackground -> AddChild(m_oHorizontalZoomOutButton.GetView());
	m_opWindowBackground -> AddChild(m_oVerticalZoomInButton.GetView());
	m_opWindowBackground -> AddChild(m_oVerticalZoomOutButton.GetView());
	m_opWindowBackground -> AddChild(m_opTrackView -> GetView());
	m_opWindowBackground -> AddChild(m_opButtonBar -> GetView());
	m_opWindowBackground -> AddChild(m_opTimeLine -> GetView());
	m_opWindowBackground -> AddChild(m_opToolBar -> GetView());
	m_opWindowBackground -> AddChild(TrackWindowStatusBar::Instance() -> GetView());
	m_opWindowBackground -> AddChild(m_opTrackInfo -> GetView());
	m_opWindowBackground -> AddChild(m_oExpandButton.GetView());
	m_opWindowBackground -> AddChild(m_oUtilityView.GetView());
	m_opWindowBackground -> AddChild(m_opResizeTab -> GetView());
	
	m_opTimeLine -> Init();
	m_opToolBar -> Init();
	m_opButtonBar -> Init();
	m_oUtilityView.Init();

	if(m_opWindowBackground -> LockWindow())
	{
		m_opWindowBackground -> SetViewColor(/*EMColor(255, 0, 0, 255)*/EM_COLOR_VIEW_BACKGROUND);
		m_opWindowBackground -> UnlockWindow();
	}

//	m_opWindow -> SetSizeLimits(770, 1600, 500, 1200);//(m_opToolBar -> CalculateMinimumBounds()).GetWidth()
	m_opWindow -> SetSizeLimits(770, 1800, 500, 1440);

	m_opMenuBar -> AddListener(this);
	BuildMenu();
	
	m_oHorizontalZoomInButton.GetButton() -> SetMode(EM_SLIM_BUTTON_FOLLOW_MOUSE);
	m_oHorizontalZoomOutButton.GetButton() -> SetMode(EM_SLIM_BUTTON_FOLLOW_MOUSE);
	m_oVerticalZoomInButton.GetButton() -> SetMode(EM_SLIM_BUTTON_FOLLOW_MOUSE);
	m_oVerticalZoomOutButton.GetButton() -> SetMode(EM_SLIM_BUTTON_FOLLOW_MOUSE);

	(m_oExpandButton.GetButton()) -> SetOn(true);

	m_opWindowBackground -> AddListener(this);
	m_opWindow -> AddListener(this);
	m_opScrollbarH -> AddListener(m_opTrackView.get());
	m_opScrollbarV -> AddListener(m_opTrackView.get());
	m_opScrollbarV -> AddListener(m_opTrackInfo.get());
	m_opScrollbarH -> AddListener(m_opTimeLine.get());
	m_opTrackView -> GetView() -> AddListener(m_opTimeLine.get());
	m_opMenuBar -> AddListener(this);
	m_opResizeTab -> GetView() -> AddListener(this);

	m_opTrackView -> SetScrollbars(m_opScrollbarH.get(), m_opScrollbarV.get());
	m_opTrackInfo -> SetScrollbar(m_opScrollbarV.get());

	m_opWindow -> Show();
	m_opMenuBar -> InitComponent();

	if(m_opWindowBackground -> LockWindow())
	{
		LayoutViews(true);
		m_opWindowBackground -> UnlockWindow();
	}

}

bool TrackWindow::IsDirty()
{
	return m_vIsDirty;
}

// FIXME: Some of the magic numbers here are temporary offsets. The reason
// why they are needed should be sorted out
void TrackWindow::LayoutViews(bool p_vNeedFullLayout)
{
//	if(m_opWindow -> Lock())
	{
		EMRect oWinBounds = m_opWindow -> Bounds();
/*
		if(!oWinBounds.IsValid())
			return;
		if(oWinBounds.GetWidth() <= 0 || oWinBounds.GetHeight() <= 0)
			return;
*/
		m_opWindowBackground -> SetFrame(EMRect(0, 0, oWinBounds.GetWidth(), oWinBounds.GetHeight()));

		int32 vOldUtilityViewHeight = (m_oUtilityView.GetView() -> Bounds()).GetHeight();

		if(p_vNeedFullLayout)
		{
			m_opToolBar -> GetView() -> SetFrame(EMRect(0, 
											(m_opMenuBar -> Frame()).m_vBottom + 1, 
											oWinBounds.m_vRight, 
											(m_opMenuBar -> Frame()).m_vBottom + 1 + TOOL_BAR_HEIGHT - 1));

			m_opTimeLine -> GetView() -> SetFrame(EMRect((m_opButtonBar -> PreferredBounds()).m_vRight + 1,
											(m_opToolBar -> GetView() -> Frame()).m_vBottom + 1,
											oWinBounds.m_vRight - EM_METRICS_SCROLLBAR_HEIGHT,
											(m_opToolBar -> GetView() -> Frame()).m_vBottom + 1 + (m_opTimeLine -> PreferredBounds()).GetHeight()));								

			EMRect oButtonBarFrame(0,
									(m_opTimeLine -> GetView() -> Frame()).m_vBottom - (m_opButtonBar -> PreferredBounds()).GetHeight(),
									(m_opButtonBar -> PreferredBounds()).m_vRight,
									(m_opTimeLine -> GetView() -> Frame()).m_vBottom);

			
			if((m_opButtonBar -> GetView() -> Frame()).m_vBottom >= oButtonBarFrame.m_vBottom) // If the button bar hasn't increased its height, set its frame before the TrackInfo view
				m_opButtonBar -> GetView() -> SetFrame(oButtonBarFrame);

			if((m_oUtilityView.GetView()) -> IsHidden())
			{
				m_opTrackInfo -> GetView() -> SetFrame(EMRect(0, 
							oButtonBarFrame.m_vBottom + 1, 
							oButtonBarFrame.m_vRight + EM_METRICS_TINY_SLIM_BUTTON_WIDTH,
							oWinBounds.m_vBottom - EM_METRICS_SCROLLBAR_HEIGHT));

				m_opTrackView -> GetView() -> SetFrame(EMRect(m_opTrackInfo -> GetView() -> Frame().m_vRight + 1, 
												oButtonBarFrame.m_vBottom + 1, 
												oWinBounds.m_vRight - EM_METRICS_SCROLLBAR_HEIGHT, 
												oWinBounds.m_vBottom - EM_METRICS_SCROLLBAR_HEIGHT));
	
				m_opScrollbarV -> SetFrame(EMRect((m_opTrackView -> GetView() -> Frame()).m_vRight + 1, 
										oButtonBarFrame.m_vBottom + 1,
										oWinBounds.m_vRight + 1,
										oWinBounds.m_vBottom - (EM_METRICS_SCROLLBAR_HEIGHT * 3)));
			}
			else
			{
				EMRect oOldUtilityBounds = (m_oUtilityView.GetView()) -> Bounds();
				(m_oUtilityView.GetView()) -> SetFrame(EMRect(0, oWinBounds.m_vBottom - m_oUtilityView.GetHeight() - EM_METRICS_SCROLLBAR_HEIGHT, oWinBounds.m_vRight, oWinBounds.m_vBottom - EM_METRICS_SCROLLBAR_HEIGHT));
//eo << "App and Utility view frame: " << ef;
				EMRect oUtilityBounds = (m_oUtilityView.GetView()) -> Bounds();
				if(oUtilityBounds != oOldUtilityBounds)
					m_vIsDirty = true;
//oWinBounds.Print();
//oUtilityBounds.Print();		
EMRect oUtilityFrame = (m_oUtilityView.GetView()) -> Frame();
				m_opTrackInfo -> GetView() -> SetFrame(EMRect(0, 
							oButtonBarFrame.m_vBottom + 1, 
							oButtonBarFrame.m_vRight + EM_METRICS_TINY_SLIM_BUTTON_WIDTH,
							oWinBounds.m_vBottom - EM_METRICS_SCROLLBAR_HEIGHT - 1 - oUtilityBounds.GetHeight()));

				m_opTrackView -> GetView() -> SetFrame(EMRect(m_opTrackInfo -> GetView() -> Frame().m_vRight + 1, 
												oButtonBarFrame.m_vBottom + 1, 
												oWinBounds.m_vRight - EM_METRICS_SCROLLBAR_HEIGHT, 
												oWinBounds.m_vBottom - EM_METRICS_SCROLLBAR_HEIGHT - 1 - oUtilityBounds.GetHeight()));
	
				m_opScrollbarV -> SetFrame(EMRect((m_opTrackView -> GetView() -> Frame()).m_vRight + 1, 
										oButtonBarFrame.m_vBottom + 1,
										oWinBounds.m_vRight + 1,
										oWinBounds.m_vBottom - (EM_METRICS_SCROLLBAR_HEIGHT * 3) - 1 - oUtilityBounds.GetHeight()));
			}
		
			if((m_opButtonBar -> GetView() -> Frame()).m_vBottom < oButtonBarFrame.m_vBottom) // If the button bar has increased its height, set its frame after the TrackInfo view
				m_opButtonBar -> GetView() -> SetFrame(oButtonBarFrame);

			TrackWindowStatusBar::Instance() -> GetView() -> SetFrame(EMRect(EM_METRICS_TINY_SLIM_BUTTON_WIDTH, 
											oWinBounds.m_vBottom - EM_METRICS_SCROLLBAR_HEIGHT + 1,
											(m_opTrackInfo -> GetView() -> Frame()).m_vRight, 
											oWinBounds.m_vBottom));
		
			m_oExpandButton.SetFrame(EMRect(0, 
										oWinBounds.m_vBottom - EM_METRICS_SCROLLBAR_HEIGHT + 1, 
										EM_METRICS_TINY_SLIM_BUTTON_WIDTH - 1, 
										oWinBounds.m_vBottom));

			m_opScrollbarH -> SetFrame(EMRect((m_opTrackView -> GetView() -> Frame()).m_vLeft, 
									oWinBounds.m_vBottom - EM_METRICS_SCROLLBAR_HEIGHT + 1,// - 1, 
									(m_opTrackView -> GetView() -> Frame()).m_vRight - (EM_METRICS_SCROLLBAR_HEIGHT * 2), 
									oWinBounds.m_vBottom + 1));
		}
	
		m_oHorizontalZoomInButton.SetFrame(EMRect((m_opTrackView -> GetView() -> Frame()).m_vRight - (EM_METRICS_SCROLLBAR_HEIGHT * 2) + 1, 
								oWinBounds.m_vBottom - EM_METRICS_SCROLLBAR_HEIGHT + 1,// - 1, 
								(m_opTrackView -> GetView() -> Frame()).m_vRight - EM_METRICS_SCROLLBAR_HEIGHT, 
								oWinBounds.m_vBottom));

		m_oHorizontalZoomOutButton.SetFrame(EMRect((m_opTrackView -> GetView() -> Frame()).m_vRight - EM_METRICS_SCROLLBAR_HEIGHT + 1, 
								oWinBounds.m_vBottom - EM_METRICS_SCROLLBAR_HEIGHT + 1,//- 1, 
								(m_opTrackView -> GetView() -> Frame()).m_vRight, 
								oWinBounds.m_vBottom));

		if(vOldUtilityViewHeight < (m_oUtilityView.GetView() -> Bounds()).GetHeight()) // If the utility view "moves down"
		{
			m_oVerticalZoomInButton.SetFrame(EMRect((m_opTrackView -> GetView() -> Frame()).m_vRight + 1, 
									(m_opScrollbarV -> Frame()).m_vBottom + 1,
									oWinBounds.m_vRight + 1,
									(m_opScrollbarV -> Frame()).m_vBottom + EM_METRICS_SCROLLBAR_HEIGHT));

			m_oVerticalZoomOutButton.SetFrame(EMRect((m_opTrackView -> GetView() -> Frame()).m_vRight + 1, 
									(m_opScrollbarV -> Frame()).m_vBottom + EM_METRICS_SCROLLBAR_HEIGHT + 1,
									oWinBounds.m_vRight,
									(m_opScrollbarV -> Frame()).m_vBottom + (EM_METRICS_SCROLLBAR_HEIGHT * 2)));
		}
		else
		{
			m_oVerticalZoomOutButton.SetFrame(EMRect((m_opTrackView -> GetView() -> Frame()).m_vRight + 1, 
									(m_opScrollbarV -> Frame()).m_vBottom + EM_METRICS_SCROLLBAR_HEIGHT + 1,
									oWinBounds.m_vRight,
									(m_opScrollbarV -> Frame()).m_vBottom + (EM_METRICS_SCROLLBAR_HEIGHT * 2)));

			m_oVerticalZoomInButton.SetFrame(EMRect((m_opTrackView -> GetView() -> Frame()).m_vRight + 1, 
									(m_opScrollbarV -> Frame()).m_vBottom + 1,
									oWinBounds.m_vRight + 1,
									(m_opScrollbarV -> Frame()).m_vBottom + EM_METRICS_SCROLLBAR_HEIGHT));
		}

		if(vOldUtilityViewHeight != (m_oUtilityView.GetView() -> Bounds()).GetHeight())
		{
			// These are invalidated since they get messed up if resizing the utility view
			m_oVerticalZoomOutButton.GetView() -> Invalidate(m_oVerticalZoomOutButton.GetView() -> Bounds());
			m_oVerticalZoomInButton.GetView() -> Invalidate(m_oVerticalZoomInButton.GetView() -> Bounds());
		}

		m_opResizeTab -> GetView() -> SetFrame(EMRect(oWinBounds.m_vRight - EM_METRICS_SCROLLBAR_HEIGHT + 1,
													  oWinBounds.m_vBottom - EM_METRICS_SCROLLBAR_HEIGHT + 1,
													  oWinBounds.m_vRight,
													  oWinBounds.m_vBottom));
								
//		m_opWindow -> Unlock();
	}
		// And now for the easy and bad GUI solution - redraw everything (FIXME)
//		m_opWindowBackground -> Invalidate(m_opWindowBackground -> Bounds());
//		m_opTrackInfo -> GetView() -> Invalidate(m_opTrackInfo -> GetView() -> Bounds());
}	

bool TrackWindow::LoadData(void* p_upProjectDataLoader)
{
	EMProjectDataLoader* opLoader = static_cast<EMProjectDataLoader*>(p_upProjectDataLoader);
	m_opWindow -> SetMaximized(opLoader -> LoadBool());
	int32 vLeft = opLoader -> LoadInt32();
	int32 vTop = opLoader -> LoadInt32();
	int32 vRight = opLoader -> LoadInt32();
	int32 vBottom = opLoader -> LoadInt32();
	m_oExpandButton.GetButton() -> SetOn(opLoader -> LoadBool());
	m_oUtilityView.SetHeight(opLoader -> LoadInt32());
	EMRect vLoadedFrame(vLeft, vTop, vRight, vBottom);
	m_opWindow -> ConvertFromWorkAreaToWindow(vLoadedFrame);
	m_opWindow -> SetFrame(vLoadedFrame, false);
	// ****** MAJOR HACK!!!! by Johan to get LoadTemplate/NewProject to work as it should. I.e. scrollbar not outside window. This is bad hack!! ******
#ifdef PLATFORM_WINDOWS
	SendMessage(static_cast<HWND>(m_opWindow -> GetNativeView()), WM_SIZE, 0, 0);
#endif
//	m_opWindow -> Notify(EM_VIEW_BOUNDS_CHANGED);
//eo << "TrackWindow::LoadData - new frame: ";
//(m_opWindow -> Frame()).Print();
	m_opScrollbarH -> SetValue(opLoader -> LoadInt32());
	m_opScrollbarV -> SetValue(opLoader -> LoadInt32());

	return true;
}

bool TrackWindow::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
//;//cout_commented_out_4_release << "TrackWindow::MessageReceived: " << p_vMessage << endl;
	switch(p_vMessage)
	{
	case EM_CLOSE_WINDOW:
		EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_QUIT_APPLICATION);
		return true;

	case EM_QUIT_PHASE_TWO:
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_STOP_PLAYBACK);
		return true;

	case EM_QUIT_PHASE_THREE:
		EMThreadRepository::Instance() -> DeleteThread(GUI_SERVICE_THREAD_NAME);// -> Suspend();
		return true;

	case EM_QUIT_PHASE_FOUR:
		// Assumes that the user has already been given the option to save the project data
		if(EMSettingsRepository::Instance() -> SettingExists(SETTING_PROJECT_ID)) // If a project is open
		{
			int vProjectID = *(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_PROJECT_ID)));
			EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_CLOSE_PROJECT, &vProjectID);
		}
		return true;

	case EM_QUIT_PHASE_FIVE:
		m_opWindow -> Hide();
		return true;
/*
	case EM_QUIT_REQUESTED:
		{
			eo << " * Try to close Main Window * " << ef;
			m_opWindow -> Hide();
			eo << " * Main Window should be closed * " << ef;
//m_opWindow -> Unlock(); // FIXME: Why are two unlocks needed here, are there times where two unlocks may be a problem?
m_opWindow -> Unlock();
			;//cout_commented_out_4_release << "TrackWindow: QuitRequested" << endl;
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_STOP_PLAYBACK);
			EMThreadRepository::Instance() -> DeleteThread(GUI_SERVICE_THREAD_NAME);
			// Closes the active project (if it exists)
			if(EMSettingsRepository::Instance() -> SettingExists(SETTING_PROJECT_ID))
			{
				uint32 vProjectID = *(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_PROJECT_ID)));
				EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_CLOSE_PROJECT, &vProjectID);
			}
			EMApplication::InitiateQuitApplication();
			m_opWindow -> Lock();
			return true;
		}
*/
	case BUTTONBAR_CHANGED:
		LayoutViews(true);
		return true;
	case EM_VIEW_BOUNDS_CHANGED: // This window listens to the background view
		{
			if((GetWindow() -> Bounds()).GetHeight() == 0) // FIXME: This is if the windows is minimized, shouldn't receive EM_VIEW_BOUNDS_CHANGED at all then
				return true;
			EMRect oUtilityBounds(m_oUtilityView.GetView() -> Bounds());
			int32 vHeight = oUtilityBounds.GetHeight();
			if((GetWindow() -> Bounds()).GetHeight() - vHeight < 200)
				m_oUtilityView.SetHeight((GetWindow() -> Bounds()).GetHeight() - 200);
			LayoutViews(true);
/*	
			EMRect oUtilityBounds(m_oUtilityView.GetView() -> Bounds());
			int32 vHeight = oUtilityBounds.GetHeight();
			if((GetWindow() -> Bounds()).GetHeight() - vHeight < 200)
			{
				m_oUtilityView.SetHeight((GetWindow() -> Bounds()).GetHeight() - 200);
//				oUtilityBounds.m_vTop += 200 - vHeight;
//				m_oUtilityView -> GetView() -> SetFrame(oUtilityBounds);
				LayoutViews(true);
			}
*/
			m_vIsDirty = true;
			return false; // FIXME: Should NOT return false!
		}
	case EM_SETTING_UPDATED:
eo << "TrackWindow: EM_SETTING_UPDATED" << ef;
		if(m_opWindow -> Lock())
		{
			bool vSetting = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_AUDIO_ENABLED)));
			EMMenuBar::Instance() -> SetMarked(RES_TM_USING, RES_TM_USING_AUDIO, vSetting);
			EMMenuBar::Instance() -> SetEnabled(RES_TM_TRACK, RES_TM_CREATE_AUDIO_TRACK, vSetting);
			EMMenuBar::Instance() -> SetEnabled(RES_TM_AUDIO, vSetting);

			vSetting = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_ENABLED)));
			EMMenuBar::Instance() -> SetMarked(RES_TM_USING, RES_TM_USING_VIDEO, vSetting);
			EMMenuBar::Instance() -> SetEnabled(RES_TM_TRACK, RES_TM_CREATE_VIDEO_TRACK, vSetting);
			//EMMenuBar::Instance() -> SetEnabled(RES_TM_VIDEO, vSetting);

			vSetting = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_MIDI_ENABLED)));
			EMMenuBar::Instance() -> SetMarked(RES_TM_USING, RES_TM_USING_MIDI, vSetting);
			EMMenuBar::Instance() -> SetEnabled(RES_TM_TRACK, RES_TM_CREATE_MIDI_TRACK, vSetting);
			EMMenuBar::Instance() -> SetEnabled(RES_TM_MIDI, vSetting);

			vSetting = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EXPERT_MODE)));
			EMMenuBar::Instance() -> SetMarked(RES_TM_USING, RES_TM_USING_ADVANCED_EDITING, vSetting);

			vSetting = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_AUDIO_LOCKED_TO_TEMPO)));
			EMMenuBar::Instance() -> SetMarked(RES_TM_AUDIO, RES_TM_LOCK_AUDIO_TO_TEMPO, vSetting);

			vSetting = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_MIDI_METRONOME_ENABLED)));
			EMMenuBar::Instance() -> SetMarked(RES_TM_MIDI, RES_TM_METRONOME, vSetting);

			m_opWindow -> Unlock();
			return true;
		}
	case EM_MENU_ITEM_TRIGGERED:
		{
			if(EMMenuBar::Instance() -> GetLastTriggeredMenuItem() -> GetCommand() >= COMMAND_NOT_TRIGGERED_MENU_ITEM_OFFSET)
			{
				eo << "EM_MENU_ITEM_TRIGGERED: " << EMMenuBar::Instance() -> GetLastTriggeredMenuItem() -> GetCommand() << ef;
				int32 vEffectEntryID = EMMenuBar::Instance() -> GetLastTriggeredMenuItem() -> GetCommand() - COMMAND_NOT_TRIGGERED_MENU_ITEM_OFFSET;
				EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_AUDIO_DESTRUCTIVE_FX, &vEffectEntryID);
			}

			return true;
		}
	default:
		return false;
	}
}

void TrackWindow::PrepareToClearData()
{
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_STOP_PLAYBACK);
}

void TrackWindow::ReadyForUse()
{
	if(m_opWindow -> Lock())
	{
		if(m_oExpandButton.GetButton() -> GetOn())
		{
			if(m_oUtilityView.GetView() -> IsHidden())
				m_oUtilityView.GetView() -> Show();
		}
		else
		{
			if(!(m_oUtilityView.GetView() -> IsHidden()))
				m_oUtilityView.GetView() -> Hide();
		}

		LayoutViews(true);
/*
		string* opMenuName = EMResourceRepository::GetString(RES_TM_MIDI);
		string* opItemName = EMResourceRepository::GetString(RES_TM_DISABLE_MIDI);
		EMMenu* opMenu = m_opMenuBar -> GetMenu(opMenuName);
		EMMenuItem* opMenuItem = opMenu -> GetMenuItem(opItemName);
		opMenuItem -> SetMarked(!(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_MIDI_ENABLED)))));

		opMenuName = EMResourceRepository::GetString(RES_TM_MODE);
		string* opItemNameOne = EMResourceRepository::GetString(RES_TM_NOVICE_MODE);
		string* opItemNameTwo = EMResourceRepository::GetString(RES_TM_EXPERT_MODE);
		opMenu = m_opMenuBar -> GetMenu(opMenuName);
		EMMenuItem* opMenuItemOne = opMenu -> GetMenuItem(opItemNameOne);
		EMMenuItem* opMenuItemTwo = opMenu -> GetMenuItem(opItemNameTwo);
		opMenuItemOne -> SetMarked(!(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EXPERT_MODE)))));
		opMenuItemTwo -> SetMarked(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EXPERT_MODE))));
*/			
		m_opWindowBackground -> Invalidate(m_opWindowBackground -> Bounds());
		// The utility view expansion button may need a redraw
		m_oExpandButton.GetView() -> Invalidate(m_oExpandButton.GetView() -> Bounds());
		m_opWindow -> Unlock();
	}

	m_vIsDirty = false;
}

bool TrackWindow::SaveData(void* p_upDataSaver)
{
	EMProjectDataSaver* opSaver = static_cast<EMProjectDataSaver*>(p_upDataSaver);
//	if(m_opWindow -> Lock())
	{
		opSaver -> SaveBool(m_opWindow -> IsMaximized());

eo << "TrackWindow::SaveData - frame: ";
(m_opWindow -> Frame()).Print();
		EMRect oFrame(m_opWindow -> Frame());
		opSaver -> SaveInt32(oFrame.m_vLeft);
		opSaver -> SaveInt32(oFrame.m_vTop);
		opSaver -> SaveInt32(oFrame.m_vRight);
		opSaver -> SaveInt32(oFrame.m_vBottom);

eo << "Saving window frame:";
oFrame.Print();
		opSaver -> SaveBool(m_oExpandButton.GetButton() -> GetOn()); // The state of the UtilityView
		opSaver -> SaveInt32(m_oUtilityView.GetHeight()); // The height of the UtilityView
		opSaver -> SaveInt32(m_opScrollbarH -> GetValue());
		opSaver -> SaveInt32(m_opScrollbarV -> GetValue());
//		m_opWindow -> Unlock();
	}

	return true;
}

