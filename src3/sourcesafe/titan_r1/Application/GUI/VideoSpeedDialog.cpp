#include "VideoSpeedDialog.h"

#include "CommandIDs.h"
#include "EMButton.h"
#include "EMCommandRepository.h"
#include "EMGClip.h"
#include "EMGClipRepository.h"
#include "EMGUIUtilities.h"
#include "EMFactory.h"
#include "EMLabel.h"
#include "EMMessages.h"
#include "EMTextControl.h"
#include "EMView.h"
#include "EMWindow.h"

#ifdef PLATFORM_WINDOWS
#include <windows.h> // To get modal dialog to work.
#include "EMWinWindow.h" // To get modal dialog to work.
#endif

const uint32 EM_BUTTON_OK = 1;
const uint32 EM_BUTTON_CANCEL = 2;

VideoSpeedDialog::VideoSpeedDialog() :
m_opDialogWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(0, 0, 226, 114), "Video Speed", EM_WINDOW_DIALOG)),
m_opMainView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 226, 114), 0)),
m_opStartLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 2,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 45,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 2 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"Start:", 0)),
m_opStopLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 26,
														  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 45,
														  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 26 + EM_METRICS_NATIVE_LABEL_HEIGHT),
												   "Stop:", 0)),
m_opDurationLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 50,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 45,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 50  + EM_METRICS_NATIVE_LABEL_HEIGHT),
													   "Duration:", 0)),
m_opSpeedLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 74,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 45,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 74 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"Speed:", 0)),
m_opPercentCharacterLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 85,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 74,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 85 + 10,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 74 + EM_METRICS_NATIVE_LABEL_HEIGHT),
															   "%", 0)),
m_opStartTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 50,
																	   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
																	   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 50 + 75,
																	   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																"00:00:00:00", 0, 0, false, EM_ALIGNMENT_RIGHT)),
m_opStopTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 50,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 24,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 50 + 75,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 24 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															   "00:00:00:00", 0, 0, false, EM_ALIGNMENT_RIGHT)),
m_opDurationTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 50,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 48,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 50 + 75,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 48 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																   "00:00:00:00", 0, 0, false, EM_ALIGNMENT_RIGHT)),
m_opSpeedTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 50,
																	   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 72,
																	   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 50 + 30,
																	   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 72 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																"", 0, 0, false, EM_ALIGNMENT_RIGHT)),
m_opOK_Button(EMFactory::Instance() -> CreateButton(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 132,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 42,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 132 + EM_METRICS_NATIVE_BUTTON_WIDTH,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 42 + EM_METRICS_NATIVE_BUTTON_HEIGHT),
													"OK", EM_BUTTON_OK, 0, 0)),
m_opCancelButton(EMFactory::Instance() -> CreateButton(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 132,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 42 + EM_METRICS_NATIVE_BUTTON_HEIGHT + EM_METRICS_NATIVE_BUTTON_SPACE,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 132 + EM_METRICS_NATIVE_BUTTON_WIDTH,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 42 + EM_METRICS_NATIVE_BUTTON_HEIGHT + EM_METRICS_NATIVE_BUTTON_SPACE + EM_METRICS_NATIVE_BUTTON_HEIGHT),
													   "Cancel", EM_BUTTON_CANCEL, 0, 0))
{
	m_opDialogWindow -> AddChild(m_opMainView);
	m_opMainView -> AddChild(m_opStartLabel);
	m_opMainView -> AddChild(m_opStopLabel);
	m_opMainView -> AddChild(m_opDurationLabel);
	m_opMainView ->	AddChild(m_opSpeedLabel);
	m_opMainView -> AddChild(m_opPercentCharacterLabel);
	m_opMainView -> AddChild(m_opStartTextControl);
	m_opMainView -> AddChild(m_opStopTextControl);
	m_opMainView -> AddChild(m_opDurationTextControl);
	m_opMainView -> AddChild(m_opSpeedTextControl);
	m_opMainView -> AddChild(m_opOK_Button);
	m_opMainView -> AddChild(m_opCancelButton);
}

VideoSpeedDialog::~VideoSpeedDialog()
{
	delete m_opDialogWindow;
	delete m_opMainView;
	delete m_opStartLabel;
	delete m_opStopLabel;
	delete m_opDurationLabel;
	delete m_opSpeedLabel;
	delete m_opPercentCharacterLabel;
	delete m_opStartTextControl;
	delete m_opStopTextControl;
	delete m_opDurationTextControl;
	delete m_opSpeedTextControl;
	delete m_opOK_Button;
	delete m_opCancelButton;
}

void VideoSpeedDialog::DisplayDialog()
{
//	float vMagicSpeedOne(-1.0f);
//	float vMagicSpeedTwo(-1.0f);
//	bool vFirstVideoClip(true);
//	bool vMagicSpeedIsSame(true);
	uint32 vVideoClipID(0);
	char vTemp[6];

	m_opClips = EMGClipRepository::Instance() -> GetSelectedClips();

	list<EMGClip*>::iterator oClipIterator = m_opClips -> begin();
	while(oClipIterator != m_opClips -> end())
	{
		if((*oClipIterator) -> GetType() == EM_TYPE_ANY_VIDEO)
		{
			vVideoClipID = (*oClipIterator) -> GetMediaClipID();
/*			if(vFirstVideoClip)
			{
				vFirstVideoClip = false;
				vMagicSpeedOne = *static_cast<float*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_MAGIC_SPEED_VALUE, &vVideoClipID, NULL, NULL));
				vMagicSpeedTwo = vMagicSpeedOne;
			}
			else if(vMagicSpeedIsSame)
				vMagicSpeedTwo = *static_cast<float*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_MAGIC_SPEED_VALUE, &vVideoClipID, NULL, NULL));
			if(vMagicSpeedOne != vMagicSpeedTwo)
				vMagicSpeedIsSame = false;
*/
			break;
		}
		oClipIterator++;
	}
	// Getting Magic Speed
	EMGUIUtilities* opGUIUtilities = EMGUIUtilities::Instance();
	m_opStartTextControl -> SetText(opGUIUtilities -> GetTimeString((*oClipIterator) -> GetPosition(), EM_SMPTE));
	m_opStopTextControl -> SetText(opGUIUtilities -> GetTimeString((*oClipIterator) -> GetPosition() + (*oClipIterator) -> GetLength(), EM_SMPTE));
	m_opDurationTextControl -> SetText(opGUIUtilities -> GetTimeString((*oClipIterator) -> GetLength(), EM_SMPTE));

	m_vMagicSpeed =  *static_cast<float*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_MAGIC_SPEED_VALUE, &vVideoClipID, NULL, NULL));
	m_opSpeedTextControl -> SetText(itoa(m_vMagicSpeed * 100, vTemp, 10));

/*	if(vMagicSpeedIsSame)
	{
		m_vMagicSpeed = vMagicSpeedOne;
		//m_opStartTextControl -> SetText(EMGUIUtilities::Instance() ->GetTimeString(
		m_opSpeedTextControl -> SetText(itoa(m_vMagicSpeed * 100, vTemp, 10));
	}
	else
	{
		m_vMagicSpeed = -1.0f;
		m_opStartTextControl -> SetText(":  :  ");
		m_opStopTextControl -> SetText(":  :  ");
		m_opDurationTextControl -> SetText(":  :  ");
		m_opSpeedTextControl -> SetText("");
	}
*/	

	if(m_opDialogWindow -> Lock())
	{
		m_opDialogWindow -> Show();
#ifdef PLATFORM_WINDOWS
		EnableWindow(dynamic_cast<EMWinWindow*>(m_opDialogWindow) -> GetMainWindowHandle(), FALSE);
		SetFocus(static_cast<HWND>(m_opOK_Button -> GetNativeView()));
#endif
		m_opDialogWindow -> Unlock();
	}
}

void VideoSpeedDialog::HideDialog()
{
	if(m_opDialogWindow -> Lock())
	{
#ifdef PLATFORM_WINDOWS
		EnableWindow(dynamic_cast<EMWinWindow*>(m_opDialogWindow) -> GetMainWindowHandle(), TRUE);
		SetFocus(dynamic_cast<EMWinWindow*>(m_opDialogWindow) -> GetMainWindowHandle());
#endif
		m_opDialogWindow -> Hide();
		m_opDialogWindow -> Unlock();
	}
}

void VideoSpeedDialog::Init()
{
	m_opDialogWindow -> AddListener(this);
	m_opMainView -> AddListener(this);
	if(m_opDialogWindow -> Lock())
	{
		//m_opMainView -> SetViewColor(EMColor(192, 192, 192, 255));
		m_opMainView -> SetDefaultColor();
		m_opDialogWindow -> Unlock();
	}
	m_opOK_Button -> AddListener(this);
	m_opCancelButton -> AddListener(this);
	m_opStartTextControl -> Enable(false);
	m_opStopTextControl -> Enable(false);
	
}

bool VideoSpeedDialog::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
/*	case EM_DRAW:
		if(p_opSender == m_opMainView)
		{
			eo << "EM_DRAW: m_oBackgroundView" << ef;
			m_opMainView -> SetHighColor(EMColor(192, 192, 192, 255));
			m_opMainView -> FillRect(m_opMainView -> Bounds());
		}
		else
			eo << "EM_DRAW: <unknown>" << ef;
		return true;
*/
	case EM_BUTTON_OK:
		{
			eo << "OK button pressed" << ef;
			float vTemp(static_cast<float>(atoi(m_opSpeedTextControl -> Text())) / static_cast<float>(100));
			EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_CHANGE_VIDEO_PLAYBACK_SPEED, &vTemp, NULL, NULL);
			HideDialog();
			return true;
		}
	case EM_BUTTON_CANCEL:
		eo << "Cancel button pressed" << ef;
		HideDialog();
		return true;
	case EM_CLOSE_WINDOW:
		if(p_opSender == m_opDialogWindow)
		{
			eo << "EM_CLOSE_WINDOW" << ef;
			HideDialog();
			return true;
		}
		break;
	}
	return false;
}

void VideoSpeedDialog::MoveDialog()
{
	EMRect oWindowRect(m_opParentWindow -> Frame());
	EMRect oDialogRect(m_opDialogWindow -> Frame());
	float vX(oWindowRect.m_vRight - oWindowRect.GetWidth() / 2 - oDialogRect.GetWidth() / 2);
	float vY(oWindowRect.m_vBottom - oWindowRect.GetHeight() / 2 - oDialogRect.GetHeight() / 2);

	MoveDialog(vX, vY);
}

void VideoSpeedDialog::MoveDialog(float p_vX, float p_vY)
{
	m_opDialogWindow -> MoveTo(p_vX, p_vY);
}

void VideoSpeedDialog::SetParent(EMWindow* p_opWindow)
{
	m_opParentWindow = p_opWindow;
}