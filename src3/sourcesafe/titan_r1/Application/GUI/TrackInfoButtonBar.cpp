#include "TrackInfoButtonBar.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMFactory.h"
#include "EMGUIGlobals.h"
#include "EMMessages.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"
#include "EMResourceRepository.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMView.h"
#include "Messages.h"
#include "ResourceIDs.h"
#include "SettingIDs.h"

TrackInfoButtonBar* TrackInfoButtonBar::m_opInstance = NULL;

TrackInfoButtonBar::TrackInfoButtonBar() :
m_opView(EMFactory::Instance() -> CreateView(EMRect(), 0, false)),
m_oAudioTrackFilterButton(m_opView, 
					EMRect(0, 0, EM_METRICS_SMALL_SLIM_BUTTON_WIDTH - 1, EM_METRICS_SLIM_BUTTON_HEIGHT - 1),
					NULL, NULL,
					0, NULL, NULL, NULL, true, true, true, true, 
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Toggles Display Of Audio Tracks On/Off"),
m_oChannelButton(m_opView, EMRect(),
					NULL, NULL,
					0, NULL, NULL, NULL, true, true, true, true, 
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Toggles Always Display MIDI Channel On/Off"),
m_oDynamicRect(EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 5, 0, EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 7 + EM_METRICS_SMALL_SLIM_BUTTON_WIDTH * 5 - 1, EM_METRICS_SLIM_BUTTON_HEIGHT - 1),
m_oEffectTrackFilterButton(m_opView, 
					EMRect(EM_METRICS_SMALL_SLIM_BUTTON_WIDTH * 4, 0, EM_METRICS_SMALL_SLIM_BUTTON_WIDTH * 5, EM_METRICS_SLIM_BUTTON_HEIGHT - 1),
					NULL, NULL,
					0, NULL, NULL, NULL, true, true, true, true, 
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Toggles Display Of Effect Tracks On/Off"),
m_oEmptySpaceOne(EM_METRICS_SMALL_SLIM_BUTTON_WIDTH * 5, 0, EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 4 - EM_METRICS_SMALL_SLIM_BUTTON_WIDTH - 1, EM_METRICS_SLIM_BUTTON_HEIGHT - 1),
m_oExpandButton(m_opView, 
					EMRect(EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 5 - EM_METRICS_SMALL_SLIM_BUTTON_WIDTH, 0, EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 5 - 1, EM_METRICS_SLIM_BUTTON_HEIGHT - 1),
					NULL, NULL, COMMAND_EXPAND_TRACK_INFO, NULL, NULL, NULL, true, true, true, true,
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Toggles Display Of Track Parameters On/Off"),
m_oInputButton(m_opView, EMRect(),
					NULL, NULL,
					0, NULL, NULL, NULL, true, true, true, true, 
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Toggles Always Display Track Input On/Off"),
m_oKeyOffsetButton(m_opView, EMRect(),
					NULL, NULL,
					0, NULL, NULL, NULL, true, true, true, true, 
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Toggles Always Display MIDI Key Offset On/Off"),
m_oMIDITrackFilterButton(m_opView, 
					EMRect(EM_METRICS_SMALL_SLIM_BUTTON_WIDTH * 2, 0, EM_METRICS_SMALL_SLIM_BUTTON_WIDTH * 3 - 1, EM_METRICS_SLIM_BUTTON_HEIGHT - 1),
					NULL, NULL,
					0, NULL, NULL, NULL, true, true, true, true, 
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Toggles Display Of MIDI Tracks On/Off"),
m_oOutputButton(m_opView, EMRect(),
					NULL, NULL,
					0, NULL, NULL, NULL, true, true, true, true, 
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Toggles Always Display Track Output On/Off"),
m_oPanButton(m_opView, EMRect(),
					NULL, NULL,
					0, NULL, NULL, NULL, true, true, true, true, 
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Toggles Always Display Track Pan On/Off"),
m_oSignalButton(m_opView, 
					EMRect(EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 4 - EM_METRICS_SMALL_SLIM_BUTTON_WIDTH, 0, EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 4 + EM_METRICS_SMALL_SLIM_BUTTON_WIDTH - 1, EM_METRICS_SLIM_BUTTON_HEIGHT - 1),
					NULL, NULL,
					0, NULL, NULL, NULL, true, true, true, true, 
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Toggles Display Of Input Signal On/Off"),
m_oTimeOffsetButton(m_opView, EMRect(),
					NULL, NULL,
					0, NULL, NULL, NULL, true, true, true, true, 
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Toggles Always Display MIDI Time Offset On/Off"),
m_oTransitionTrackFilterButton(m_opView, 
					EMRect(EM_METRICS_SMALL_SLIM_BUTTON_WIDTH * 3, 0, EM_METRICS_SMALL_SLIM_BUTTON_WIDTH * 4 - 1, EM_METRICS_SLIM_BUTTON_HEIGHT - 1),
					NULL, NULL,
					0, NULL, NULL, NULL, true, true, true, true, 
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Toggles Display Of Transition Tracks On/Off"),
m_oVelocityOffsetButton(m_opView, EMRect(),
					NULL, NULL,
					0, NULL, NULL, NULL, true, true, true, true, 
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Toggles Always Display MIDI Velocity Offset On/Off"),
m_oVideoTrackFilterButton(m_opView, 
					EMRect(EM_METRICS_SMALL_SLIM_BUTTON_WIDTH, 0, EM_METRICS_SMALL_SLIM_BUTTON_WIDTH * 2 - 1, EM_METRICS_SLIM_BUTTON_HEIGHT - 1),
					NULL, NULL,
					0, NULL, NULL, NULL, true, true, true, true, 
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Toggles Display Of Video Tracks On/Off"),
m_oVolumeButton(m_opView, EMRect(),
					NULL, NULL,
					0, NULL, NULL, NULL, true, true, true, true, 
					EM_COLOR_BUTTON_DEFAULT_ON, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Toggles Always Display Track Volume On/Off"),
m_vIsDirty(false),
m_vIsExpanded(false)
{
}

TrackInfoButtonBar::~TrackInfoButtonBar()
{
	;//cout_commented_out_4_release << "TrackInfoButtonBar::~TrackInfoButtonBar" << endl;
	delete m_opView;
}

void TrackInfoButtonBar::Draw(EMRect p_oClippingRect)
{
//eo << "TrackInfoButtonBar::Draw:" << ef;
//p_oClippingRect.Print();
//(m_opView -> Bounds()).Print();
	if(m_oEmptySpaceOne.Intersects(p_oClippingRect))
	{
		m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND);
		m_opView -> FillRect(m_oEmptySpaceOne);
	}
	/*
	if(m_oEmptySpaceTwo.Intersects(p_oClippingRect))
	{
		m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND);
		m_opView -> FillRect(m_oEmptySpaceTwo);
	}
	*/
	m_oInputButton.Draw(p_oClippingRect, m_opView);
	m_oOutputButton.Draw(p_oClippingRect, m_opView);
	m_oVolumeButton.Draw(p_oClippingRect, m_opView);
	m_oPanButton.Draw(p_oClippingRect, m_opView);
	m_oChannelButton.Draw(p_oClippingRect, m_opView);
	m_oTimeOffsetButton.Draw(p_oClippingRect, m_opView);
	m_oVelocityOffsetButton.Draw(p_oClippingRect, m_opView);
	m_oKeyOffsetButton.Draw(p_oClippingRect, m_opView);
	m_oAudioTrackFilterButton.Draw(p_oClippingRect, m_opView);
	m_oEffectTrackFilterButton.Draw(p_oClippingRect, m_opView);
	m_oExpandButton.Draw(p_oClippingRect, m_opView);
	m_oMIDITrackFilterButton.Draw(p_oClippingRect, m_opView);
	m_oSignalButton.Draw(p_oClippingRect, m_opView);
	m_oTransitionTrackFilterButton.Draw(p_oClippingRect, m_opView);
	m_oVideoTrackFilterButton.Draw(p_oClippingRect, m_opView);

	EMRect oBounds = m_opView -> Bounds();

	m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_HIGHLIGHT);
	m_opView -> StrokeLine(oBounds.m_vLeft, oBounds.m_vBottom - 2, oBounds.m_vRight, oBounds.m_vBottom - 2);
	m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND);
	m_opView -> StrokeLine(oBounds.m_vLeft, oBounds.m_vBottom - 1, oBounds.m_vRight, oBounds.m_vBottom - 1);
	m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_SHADOW);
	m_opView -> StrokeLine(oBounds.m_vLeft, oBounds.m_vBottom, oBounds.m_vRight, oBounds.m_vBottom);
}

bool TrackInfoButtonBar::GetAudioTrackFilterOn()
{
	return m_oAudioTrackFilterButton.GetOn();
}

bool TrackInfoButtonBar::GetChannelOn()
{
	return m_oChannelButton.GetOn();
}

bool TrackInfoButtonBar::GetEffectTrackFilterOn()
{
	return m_oEffectTrackFilterButton.GetOn();
}

bool TrackInfoButtonBar::GetExpandOn()
{
	return m_oExpandButton.GetOn();
}

bool TrackInfoButtonBar::GetInputOn()
{
	return m_oInputButton.GetOn();
}

bool TrackInfoButtonBar::GetKeyOffsetOn()
{
	return m_oKeyOffsetButton.GetOn();
}

bool TrackInfoButtonBar::GetMIDITrackFilterOn()
{
	return m_oMIDITrackFilterButton.GetOn();
}

bool TrackInfoButtonBar::GetOutputOn()
{
	return m_oOutputButton.GetOn();
}

bool TrackInfoButtonBar::GetPanOn()
{
	return m_oPanButton.GetOn();
}

bool TrackInfoButtonBar::GetSignalOn()
{
	return m_oSignalButton.GetOn();
}

bool TrackInfoButtonBar::GetTimeOffsetOn()
{
	return m_oTimeOffsetButton.GetOn();
}

bool TrackInfoButtonBar::GetTransitionTrackFilterOn()
{
	return m_oTransitionTrackFilterButton.GetOn();
}

bool TrackInfoButtonBar::GetVelocityOffsetOn()
{
	return m_oVelocityOffsetButton.GetOn();
}

EMView* TrackInfoButtonBar::GetView()
{
	return m_opView;
}

bool TrackInfoButtonBar::GetVideoTrackFilterOn()
{
	return m_oVideoTrackFilterButton.GetOn();
}

bool TrackInfoButtonBar::GetVolumeOn()
{
	return m_oVolumeButton.GetOn();
}

void TrackInfoButtonBar::Init()
{
	m_oAudioTrackFilterButton.SetOnString(EMResourceRepository::GetString(RES_TIBB_AUDIO_FILTER));
	m_oAudioTrackFilterButton.SetOffString(EMResourceRepository::GetString(RES_TIBB_AUDIO_FILTER));
	m_oChannelButton.SetOnString(EMResourceRepository::GetString(RES_TIBB_CHANNEL));
	m_oChannelButton.SetOffString(EMResourceRepository::GetString(RES_TIBB_CHANNEL));
	m_oEffectTrackFilterButton.SetOnString(EMResourceRepository::GetString(RES_TIBB_EFFECT_FILTER));
	m_oEffectTrackFilterButton.SetOffString(EMResourceRepository::GetString(RES_TIBB_EFFECT_FILTER));
	m_oExpandButton.SetBitmaps(EMResourceRepository::GetBitmap(RES_EB_EXPAND_HOR_ON), EMResourceRepository::GetBitmap(RES_EB_EXPAND_HOR));
	m_oInputButton.SetOnString(EMResourceRepository::GetString(RES_TIBB_INPUT));
	m_oInputButton.SetOffString(EMResourceRepository::GetString(RES_TIBB_INPUT));
	m_oKeyOffsetButton.SetOnString(EMResourceRepository::GetString(RES_TIBB_KEY_OFFSET));
	m_oKeyOffsetButton.SetOffString(EMResourceRepository::GetString(RES_TIBB_KEY_OFFSET));
	m_oMIDITrackFilterButton.SetOnString(EMResourceRepository::GetString(RES_TIBB_MIDI_FILTER));
	m_oMIDITrackFilterButton.SetOffString(EMResourceRepository::GetString(RES_TIBB_MIDI_FILTER));
	m_oOutputButton.SetOnString(EMResourceRepository::GetString(RES_TIBB_OUTPUT));
	m_oOutputButton.SetOffString(EMResourceRepository::GetString(RES_TIBB_OUTPUT));
	m_oPanButton.SetOnString(EMResourceRepository::GetString(RES_TIBB_PAN));
	m_oPanButton.SetOffString(EMResourceRepository::GetString(RES_TIBB_PAN));
	m_oSignalButton.SetOnString(EMResourceRepository::GetString(RES_TIBB_SIG));
	m_oSignalButton.SetOffString(EMResourceRepository::GetString(RES_TIBB_SIG));
	m_oTimeOffsetButton.SetOnString(EMResourceRepository::GetString(RES_TIBB_CHANNEL));
	m_oTimeOffsetButton.SetOffString(EMResourceRepository::GetString(RES_TIBB_CHANNEL));
	m_oTransitionTrackFilterButton.SetOnString(EMResourceRepository::GetString(RES_TIBB_TRANSITION_FILTER));
	m_oTransitionTrackFilterButton.SetOffString(EMResourceRepository::GetString(RES_TIBB_TRANSITION_FILTER));
	m_oVelocityOffsetButton.SetOnString(EMResourceRepository::GetString(RES_TIBB_VEL_OFFSET));
	m_oVelocityOffsetButton.SetOffString(EMResourceRepository::GetString(RES_TIBB_VEL_OFFSET));
	m_oVideoTrackFilterButton.SetOnString(EMResourceRepository::GetString(RES_TIBB_VIDEO_FILTER));
	m_oVideoTrackFilterButton.SetOffString(EMResourceRepository::GetString(RES_TIBB_VIDEO_FILTER));
	m_oVolumeButton.SetOnString(EMResourceRepository::GetString(RES_TIBB_VOLUME));
	m_oVolumeButton.SetOffString(EMResourceRepository::GetString(RES_TIBB_VOLUME));

//	m_opView -> SetViewColor(EMColor(255, 255, 255, 0)); // Debug
	m_opView -> AddListener(this);
	m_oAudioTrackFilterButton.SetOn(true);
	m_oEffectTrackFilterButton.SetOn(true);
	m_oMIDITrackFilterButton.SetOn(true);
	m_oTransitionTrackFilterButton.SetOn(true);
	m_oVideoTrackFilterButton.SetOn(true);
/*	
	// FIXME: Temporary (just for the first alpha release and CeBIT)
	m_oChannelButton.SetMode(EM_SLIM_BUTTON_DRAW_LABEL);
	m_oInputButton.SetMode(EM_SLIM_BUTTON_DRAW_LABEL);
	m_oKeyOffsetButton.SetMode(EM_SLIM_BUTTON_DRAW_LABEL);
	m_oOutputButton.SetMode(EM_SLIM_BUTTON_DRAW_LABEL);
	m_oPanButton.SetMode(EM_SLIM_BUTTON_DRAW_LABEL);
	m_oSignalButton.SetMode(EM_SLIM_BUTTON_DRAW_LABEL);
	m_oTimeOffsetButton.SetMode(EM_SLIM_BUTTON_DRAW_LABEL);
	m_oVelocityOffsetButton.SetMode(EM_SLIM_BUTTON_DRAW_LABEL);
	m_oVolumeButton.SetMode(EM_SLIM_BUTTON_DRAW_LABEL);
*/
	m_oEffectTrackFilterButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
	
	LayoutButtons();
	m_vExpertMode = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EXPERT_MODE)));
	m_vAudioEnabled = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_AUDIO_ENABLED)));
	m_vVideoEnabled = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_ENABLED)));
	m_vMIDIEnabled = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_MIDI_ENABLED)));

//	if(!m_vExpertMode)
//		m_oSignalButton.SetOn(false);
	m_oSignalButton.SetOn(true);

	m_oAudioTrackFilterButton.SetOn(m_vAudioEnabled);
	m_oVideoTrackFilterButton.SetOn(m_vVideoEnabled);
	m_oTransitionTrackFilterButton.SetOn(m_vVideoEnabled);
	m_oMIDITrackFilterButton.SetOn(m_vMIDIEnabled);

	SetUserMode();
	EMSettingsRepository::Instance() -> AddListener(this);

	Notify(BUTTONBAR_CHANGED);
	Notify(TRACK_FILTERS_CHANGED);
}

TrackInfoButtonBar* TrackInfoButtonBar::Instance()
{
	if(m_opInstance != NULL)
		return m_opInstance;
	m_opInstance = EM_new TrackInfoButtonBar();
	return m_opInstance;
}

bool TrackInfoButtonBar::IsDirty()
{
	return m_vIsDirty;
}

void TrackInfoButtonBar::LayoutButtons()
{
	int32 vCurrentXPos = EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 5;
	
	if(m_oInputButton.GetOn() || m_vIsExpanded)
	{
		m_oInputButton.SetVisible(true);
		m_oInputButton.SetFrame(EMRect(vCurrentXPos, 0, vCurrentXPos + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH - 1, EM_METRICS_SLIM_BUTTON_HEIGHT - 1));
		vCurrentXPos += EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH;
	}
	else
		m_oInputButton.SetVisible(false);
		
	if(m_oOutputButton.GetOn() || m_vIsExpanded)
	{
		m_oOutputButton.SetVisible(true);
		m_oOutputButton.SetFrame(EMRect(vCurrentXPos, 0, vCurrentXPos + EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH - 1, EM_METRICS_SLIM_BUTTON_HEIGHT - 1));
		vCurrentXPos += EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH;
	}
	else
		m_oOutputButton.SetVisible(false);
		
	if(m_oVolumeButton.GetOn() || m_vIsExpanded)
	{
		m_oVolumeButton.SetVisible(true);
		m_oVolumeButton.SetFrame(EMRect(vCurrentXPos, 0, vCurrentXPos + EM_METRICS_SMALL_SLIM_BUTTON_WIDTH - 1, EM_METRICS_SLIM_BUTTON_HEIGHT - 1));
		vCurrentXPos += EM_METRICS_SMALL_SLIM_BUTTON_WIDTH;
	}
	else
		m_oVolumeButton.SetVisible(false);
		
	if(m_oPanButton.GetOn() || m_vIsExpanded)
	{
		m_oPanButton.SetVisible(true);
		m_oPanButton.SetFrame(EMRect(vCurrentXPos, 0, vCurrentXPos + EM_METRICS_SMALL_SLIM_BUTTON_WIDTH - 1, EM_METRICS_SLIM_BUTTON_HEIGHT - 1));
		vCurrentXPos += EM_METRICS_SMALL_SLIM_BUTTON_WIDTH;
	}
	else
		m_oPanButton.SetVisible(false);
		
	if(m_oTimeOffsetButton.GetOn() || m_vIsExpanded)
	{
		m_oTimeOffsetButton.SetVisible(true);
		m_oTimeOffsetButton.SetFrame(EMRect(vCurrentXPos, 0, vCurrentXPos + EM_METRICS_SMALL_SLIM_BUTTON_WIDTH - 1, EM_METRICS_SLIM_BUTTON_HEIGHT - 1));
		vCurrentXPos += EM_METRICS_SMALL_SLIM_BUTTON_WIDTH;
	}
	else
		m_oTimeOffsetButton.SetVisible(false);
		
	if(m_oVelocityOffsetButton.GetOn() || m_vIsExpanded)
	{
		m_oVelocityOffsetButton.SetVisible(true);
		m_oVelocityOffsetButton.SetFrame(EMRect(vCurrentXPos, 0, vCurrentXPos + EM_METRICS_SMALL_SLIM_BUTTON_WIDTH - 1, EM_METRICS_SLIM_BUTTON_HEIGHT - 1));
		vCurrentXPos += EM_METRICS_SMALL_SLIM_BUTTON_WIDTH;
	}
	else
		m_oVelocityOffsetButton.SetVisible(false);
		
	if(m_oKeyOffsetButton.GetOn() || m_vIsExpanded)
	{
		m_oKeyOffsetButton.SetVisible(true);
		m_oKeyOffsetButton.SetFrame(EMRect(vCurrentXPos, 0, vCurrentXPos + EM_METRICS_SMALL_SLIM_BUTTON_WIDTH - 1, EM_METRICS_SLIM_BUTTON_HEIGHT - 1));
		vCurrentXPos += EM_METRICS_SMALL_SLIM_BUTTON_WIDTH;
	}
	else
		m_oKeyOffsetButton.SetVisible(false);

//	m_oEmptySpaceTwo = EMRect(vCurrentXPos, 0, m_oDynamicRect.m_vRight, EM_METRICS_SLIM_BUTTON_HEIGHT - 1);
//	;//cout_commented_out_4_release << "m_oEmptySpaceTwo: ";
//	m_oEmptySpaceTwo.Print();
/*
;//cout_commented_out_4_release << "Bounds: " << endl;
m_oInputButton.Bounds().Print();
m_oOutputButton.Bounds().Print();
m_oVolumeButton.Bounds().Print();
m_oPanButton.Bounds().Print();
m_oTimeOffsetButton.Bounds().Print();
m_oVelocityOffsetButton.Bounds().Print();
m_oKeyOffsetButton.Bounds().Print();
*/
}

bool TrackInfoButtonBar::LoadData(void* p_upProjectDataLoader)
{
	EMProjectDataLoader* opLoader = static_cast<EMProjectDataLoader*>(p_upProjectDataLoader);

	m_oAudioTrackFilterButton.SetOn(opLoader -> LoadBool());
	m_oChannelButton.SetOn(opLoader -> LoadBool());
	m_oEffectTrackFilterButton.SetOn(opLoader -> LoadBool());
	m_oExpandButton.SetOn(opLoader -> LoadBool());
	m_vIsExpanded = m_oExpandButton.GetOn();
	m_oInputButton.SetOn(opLoader -> LoadBool());
	m_oKeyOffsetButton.SetOn(opLoader -> LoadBool());
	m_oMIDITrackFilterButton.SetOn(opLoader -> LoadBool());
	m_oOutputButton.SetOn(opLoader -> LoadBool());
	m_oPanButton.SetOn(opLoader -> LoadBool());
	m_oSignalButton.SetOn(opLoader -> LoadBool());
	m_oTimeOffsetButton.SetOn(opLoader -> LoadBool());
	m_oTransitionTrackFilterButton.SetOn(opLoader -> LoadBool());
	m_oVelocityOffsetButton.SetOn(opLoader -> LoadBool());
	m_oVideoTrackFilterButton.SetOn(opLoader -> LoadBool());
	m_oVolumeButton.SetOn(opLoader -> LoadBool());
	
//	m_vIsDirty = false;

	return true;
}

bool TrackInfoButtonBar::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	int32 vX;
	int32 vY;
	bool vButtonOne;
	bool vButtonTwo;
	bool vButtonThree;

//	;//cout_commented_out_4_release << "TrackInfoButtonBar::MessageReceived" << endl;
	switch(p_vMessage)
	{
	case EM_DRAW:
		Draw(m_opView -> GetClippingRect());
		return true;
	case EM_MOUSE_DOWN:
		m_opView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		MouseDown(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		return true;
	case EM_SETTING_UPDATED:
		{
eo << "TrackInfoButtonBar: EM_SETTING_UPDATED" << ef;
			bool vExpertMode = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EXPERT_MODE)));
			bool vAudioEnabled = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_AUDIO_ENABLED)));
			bool vVideoEnabled = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_ENABLED)));
			bool vMIDIEnabled = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_MIDI_ENABLED)));
			if(vExpertMode != m_vExpertMode || vAudioEnabled != m_vAudioEnabled || vVideoEnabled != m_vVideoEnabled || vMIDIEnabled != m_vMIDIEnabled)
			{
				if(m_opView -> LockWindow())
				{
					bool vFilterChanged(false);

					if(!vExpertMode && m_vExpertMode)
					{
						if(!m_oSignalButton.GetOn())
						{
							m_oSignalButton.SetOn(true);
							bool vEnabled = true;
							EMSettingsRepository::Instance() -> SetSetting(SETTING_SIGNAL_METERS_ENABLED, EM_SETTING_BOOL, &vEnabled, true);
						}
						m_oAudioTrackFilterButton.SetOn(vAudioEnabled);
						m_oVideoTrackFilterButton.SetOn(vVideoEnabled);
						m_oTransitionTrackFilterButton.SetOn(vVideoEnabled);
						m_oMIDITrackFilterButton.SetOn(vMIDIEnabled);
						vFilterChanged = true;
					}
						
					else
					{
						if(vAudioEnabled != m_vAudioEnabled)
						{
							m_oAudioTrackFilterButton.SetOn(vAudioEnabled);
							vFilterChanged = true;
						}
						if(vVideoEnabled != m_vVideoEnabled)
						{
							m_oVideoTrackFilterButton.SetOn(vVideoEnabled);
							m_oTransitionTrackFilterButton.SetOn(vVideoEnabled);
							vFilterChanged = true;
						}
						if(vMIDIEnabled != m_vMIDIEnabled)
						{
							m_oMIDITrackFilterButton.SetOn(vMIDIEnabled);
							vFilterChanged = true;
						}
					}

					if(vFilterChanged)
					{
						Notify(TRACK_FILTERS_CHANGED);
						m_vIsDirty = true;
					}

					m_vExpertMode = vExpertMode;
					m_vAudioEnabled = vAudioEnabled;
					m_vVideoEnabled = vVideoEnabled;
					m_vMIDIEnabled = vMIDIEnabled;

					SetUserMode();
					m_opView -> Invalidate(m_opView -> Bounds());
					m_opView -> UnlockWindow();
				}
				return true;
			}
			return false;
		}
/*
	case EM_MOUSE_MOVED:
		m_opView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		MouseMoved(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		return true;
	case EM_MOUSE_UP:
		m_opView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		MouseUp(vX, vY);
		return true;
*/
	default:
		return false;
	}
}

void TrackInfoButtonBar::MouseDown(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
	bool vHasChanged = false;
	bool vFilterHasChanged = false;
	
	if(m_oAudioTrackFilterButton.MouseDown(p_vX, p_vY))
	{
		m_opView -> Invalidate(m_oAudioTrackFilterButton.Frame());
		vFilterHasChanged = true;
	}
	if(m_oEffectTrackFilterButton.MouseDown(p_vX, p_vY))
	{
		m_opView -> Invalidate(m_oEffectTrackFilterButton.Frame());
		vFilterHasChanged = true;
	}
	if(m_oMIDITrackFilterButton.MouseDown(p_vX, p_vY))
	{
		m_opView -> Invalidate(m_oMIDITrackFilterButton.Frame());
		vFilterHasChanged = true;
	}
	if(m_oTransitionTrackFilterButton.MouseDown(p_vX, p_vY))
	{
		m_opView -> Invalidate(m_oTransitionTrackFilterButton.Frame());
		vFilterHasChanged = true;
	}
	if(m_oSignalButton.MouseDown(p_vX, p_vY))
	{
		m_opView -> Invalidate(m_oSignalButton.Frame());
		bool vEnabled = m_oSignalButton.GetOn();
		EMSettingsRepository::Instance() -> SetSetting(SETTING_SIGNAL_METERS_ENABLED, EM_SETTING_BOOL, &vEnabled, true);
		vHasChanged = true;
	}
	if(m_oVideoTrackFilterButton.MouseDown(p_vX, p_vY))
	{
		m_opView -> Invalidate(m_oVideoTrackFilterButton.Frame());
		vFilterHasChanged = true;
	}
	if(m_oExpandButton.MouseDown(p_vX, p_vY))
	{
		m_opView -> Invalidate(m_oExpandButton.Frame());
		vHasChanged = true;
	}

	if(m_vIsExpanded)
	{
		if(m_oChannelButton.MouseDown(p_vX, p_vY))
		{
			m_opView -> Invalidate(m_oChannelButton.Frame());
			vHasChanged = true;
		}
		if(m_oInputButton.MouseDown(p_vX, p_vY))
		{
			m_opView -> Invalidate(m_oInputButton.Frame());
			vHasChanged = true;
		}
		if(m_oKeyOffsetButton.MouseDown(p_vX, p_vY))
		{
			m_opView -> Invalidate(m_oKeyOffsetButton.Frame());
			vHasChanged = true;
		}
		if(m_oOutputButton.MouseDown(p_vX, p_vY))
		{
			m_opView -> Invalidate(m_oOutputButton.Frame());
			vHasChanged = true;
		}
		if(m_oPanButton.MouseDown(p_vX, p_vY))
		{
			m_opView -> Invalidate(m_oPanButton.Frame());
			vHasChanged = true;
		}
		if(m_oTimeOffsetButton.MouseDown(p_vX, p_vY))
		{
			m_opView -> Invalidate(m_oTimeOffsetButton.Frame());
			vHasChanged = true;
		}
		if(m_oVelocityOffsetButton.MouseDown(p_vX, p_vY))
		{
			m_opView -> Invalidate(m_oVelocityOffsetButton.Frame());
			vHasChanged = true;
		}
		if(m_oVolumeButton.MouseDown(p_vX, p_vY))
		{
			m_opView -> Invalidate(m_oVolumeButton.Frame());
			vHasChanged = true;
		}
	}
	else
	{
		if(m_oChannelButton.MouseDown(p_vX, p_vY))
			vHasChanged = true;
		if(m_oInputButton.MouseDown(p_vX, p_vY))
			vHasChanged = true;
		if(m_oKeyOffsetButton.MouseDown(p_vX, p_vY))
			vHasChanged = true;
		if(m_oOutputButton.MouseDown(p_vX, p_vY))
			vHasChanged = true;
		if(m_oPanButton.MouseDown(p_vX, p_vY))
			vHasChanged = true;
		if(m_oTimeOffsetButton.MouseDown(p_vX, p_vY))
			vHasChanged = true;
		if(m_oVelocityOffsetButton.MouseDown(p_vX, p_vY))
			vHasChanged = true;
		if(m_oVolumeButton.MouseDown(p_vX, p_vY))
			vHasChanged = true;
		LayoutButtons();
		m_opView -> Invalidate(m_oDynamicRect);
	}
	
	if(vHasChanged)
	{
		Notify(BUTTONBAR_CHANGED);
		m_vIsDirty = true;
	}
	if(vFilterHasChanged)
	{
		Notify(TRACK_FILTERS_CHANGED);
		m_vIsDirty = true;
	}
}
/*
void TrackInfoButtonBar::MouseMoved(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
}

void TrackInfoButtonBar::MouseUp(int32 p_vX, int32 p_vY)
{
}
*/
EMRect TrackInfoButtonBar::PreferredBounds()
{
	int32 vWidth = EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH * 5 - 1;//m_oExpandButton.Frame().m_vRight;
	int32 vHeight = EM_METRICS_SLIM_BUTTON_HEIGHT - 1 + 3;

	if(m_oInputButton.GetVisible())
		vWidth += m_oInputButton.Frame().GetWidth() + 1;
	if(m_oOutputButton.GetVisible())
		vWidth += m_oOutputButton.Frame().GetWidth() + 1;
	if(m_oVolumeButton.GetVisible())
		vWidth += m_oVolumeButton.Frame().GetWidth() + 1;
	if(m_oPanButton.GetVisible())
		vWidth += m_oPanButton.Frame().GetWidth() + 1;
	if(m_oChannelButton.GetVisible())
		vWidth += m_oChannelButton.Frame().GetWidth() + 1;
	if(m_oTimeOffsetButton.GetVisible())
		vWidth += m_oTimeOffsetButton.Frame().GetWidth() + 1;
	if(m_oVelocityOffsetButton.GetVisible())
		vWidth += m_oVelocityOffsetButton.Frame().GetWidth() + 1;
	if(m_oKeyOffsetButton.GetVisible())
		vWidth += m_oKeyOffsetButton.Frame().GetWidth() + 1;
	
	vWidth--; // FIXME: Why is this needed?
	
	return EMRect(0, 0, vWidth, vHeight);
}

void TrackInfoButtonBar::ReadyForUse()
{
	if(m_opView -> LockWindow())
	{
		LayoutButtons();
		Notify(BUTTONBAR_CHANGED);
		Notify(TRACK_FILTERS_CHANGED);
		m_opView -> Invalidate(m_opView -> Bounds());
		m_opView -> UnlockWindow();
	}

	m_vIsDirty = false;
}

bool TrackInfoButtonBar::SaveData(void* p_upDataSaver)
{
	EMProjectDataSaver* opSaver = static_cast<EMProjectDataSaver*>(p_upDataSaver);

	opSaver -> SaveBool(m_oAudioTrackFilterButton.GetOn());
	opSaver -> SaveBool(m_oChannelButton.GetOn());
	opSaver -> SaveBool(m_oEffectTrackFilterButton.GetOn());
	opSaver -> SaveBool(m_oExpandButton.GetOn());
	opSaver -> SaveBool(m_oInputButton.GetOn());
	opSaver -> SaveBool(m_oKeyOffsetButton.GetOn());
	opSaver -> SaveBool(m_oMIDITrackFilterButton.GetOn());
	opSaver -> SaveBool(m_oOutputButton.GetOn());
	opSaver -> SaveBool(m_oPanButton.GetOn());
	opSaver -> SaveBool(m_oSignalButton.GetOn());
	opSaver -> SaveBool(m_oTimeOffsetButton.GetOn());
	opSaver -> SaveBool(m_oTransitionTrackFilterButton.GetOn());
	opSaver -> SaveBool(m_oVelocityOffsetButton.GetOn());
	opSaver -> SaveBool(m_oVideoTrackFilterButton.GetOn());
	opSaver -> SaveBool(m_oVolumeButton.GetOn());

//	m_vIsDirty = false;
	
	return true;
}

void TrackInfoButtonBar::SetExpand()
{
	if(EMCommandRepository::Instance() -> TriggeredByBinding())
		m_oExpandButton.ToggleOn();
	m_vIsExpanded = m_oExpandButton.GetOn();

	LayoutButtons();
	m_opView -> Invalidate(m_opView -> Bounds());

	Notify(BUTTONBAR_CHANGED);

	m_vIsDirty = true;
}

void TrackInfoButtonBar::SetUserMode()
{
	if(m_vExpertMode)
	{
		if(m_vAudioEnabled)
			m_oAudioTrackFilterButton.SetMode(EM_SLIM_BUTTON_STAY_DOWN);
		else
			m_oAudioTrackFilterButton.SetMode(EM_SLIM_BUTTON_DRAW_LABEL);
//		m_oEffectTrackFilterButton.SetMode(EM_SLIM_BUTTON_STAY_DOWN);
//		m_oExpandButton.SetMode(EM_SLIM_BUTTON_STAY_DOWN);
		if(m_vMIDIEnabled)
			m_oMIDITrackFilterButton.SetMode(EM_SLIM_BUTTON_STAY_DOWN);
		else
			m_oMIDITrackFilterButton.SetMode(EM_SLIM_BUTTON_DRAW_LABEL);
		m_oSignalButton.SetMode(EM_SLIM_BUTTON_STAY_DOWN);
		if(m_vVideoEnabled)
		{
			m_oTransitionTrackFilterButton.SetMode(EM_SLIM_BUTTON_STAY_DOWN);
			m_oVideoTrackFilterButton.SetMode(EM_SLIM_BUTTON_STAY_DOWN);
		}
		else
		{
			m_oTransitionTrackFilterButton.SetMode(EM_SLIM_BUTTON_DRAW_LABEL);
			m_oVideoTrackFilterButton.SetMode(EM_SLIM_BUTTON_DRAW_LABEL);
		}
	}
	else
	{
		m_oAudioTrackFilterButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
//		m_oEffectTrackFilterButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
//		m_oExpandButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
		m_oMIDITrackFilterButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
		m_oSignalButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
		m_oTransitionTrackFilterButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
		m_oVideoTrackFilterButton.SetMode(EM_SLIM_BUTTON_DRAW_BLANK);
		/*
		if(m_vIsExpanded)
		{
			m_oExpandButton.SetOn(false);
			m_vIsExpanded = false;
			LayoutButtons();
			m_opView -> Invalidate(m_opView -> Bounds());
			Notify(BUTTONBAR_CHANGED);
		}
		*/
	}

	Notify(BUTTONBAR_CHANGED);

	m_vIsDirty = true;
}


