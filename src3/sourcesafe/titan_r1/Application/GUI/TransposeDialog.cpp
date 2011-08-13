#include "TransposeDialog.h"

#include "EMFactory.h"

#include "EMWindow.h"
#include "EMView.h"
#include "EMButton.h"
#include "EMLabel.h"
#include "EMTextControl.h"

#include "EMMessages.h"

const uint32 EM_BUTTON_OK = 1;
const uint32 EM_BUTTON_CANCEL = 2;

TransposeDialog::TransposeDialog() :
m_opDialogWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(0, 0, 188, 73), "Transpose", EM_WINDOW_DIALOG)), 
m_opMainView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 188, 73), false)),
m_opOK_Button(EMFactory::Instance() -> CreateButton(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 94,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 94 + EM_METRICS_NATIVE_BUTTON_WIDTH,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_BUTTON_HEIGHT),
													"OK", EM_BUTTON_OK, 0, 0)),
m_opCancelButton(EMFactory::Instance() -> CreateButton(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 94,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_BUTTON_HEIGHT + EM_METRICS_NATIVE_BUTTON_SPACE,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 94 + EM_METRICS_NATIVE_BUTTON_WIDTH,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_BUTTON_HEIGHT + EM_METRICS_NATIVE_BUTTON_SPACE + EM_METRICS_NATIVE_BUTTON_HEIGHT),
													   "Cancel", EM_BUTTON_CANCEL, 0, 0)),
m_opLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
													  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 2,
													  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 56,
													  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 2 + EM_METRICS_NATIVE_LABEL_HEIGHT),
											   "Transpose:", 0)),
m_opTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 59,
																  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
																  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 59 + 25,
																  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
														   "0", 0, 0, false, EM_ALIGNMENT_RIGHT))
{
	m_opDialogWindow -> AddChild(m_opMainView);
	m_opMainView -> AddChild(m_opOK_Button);
	m_opMainView -> AddChild(m_opCancelButton);
	m_opMainView -> AddChild(m_opLabel);
	m_opMainView -> AddChild(m_opTextControl);
}
TransposeDialog::~TransposeDialog()
{
	delete m_opDialogWindow;
	delete m_opMainView;
	delete m_opOK_Button;
	delete m_opCancelButton;
	delete m_opLabel;
	delete m_opTextControl;
}

void TransposeDialog::DisplayDialog()
{
	if(m_opDialogWindow -> Lock())
	{
		m_opDialogWindow -> Show();
		m_opDialogWindow -> Unlock();
	}
}

void TransposeDialog::HideDialog()
{
	if(m_opDialogWindow -> Lock())
	{
		m_opDialogWindow -> Hide();
		m_opDialogWindow -> Unlock();
	}
}

void TransposeDialog::Init()
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
}

bool TransposeDialog::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
/*	case EM_DRAW:
		if(p_opSender == m_opMainView)
		{
			eo << "EM_DRAW: m_oBackgroundView" << ef;
			m_opMainView -> SetHighColor(EMColor(0, 192, 0, 255));
			m_opMainView -> FillRect(m_opMainView -> Bounds());
		}
		else
			eo << "EM_DRAW (unknown)" << ef;
		return true;
*/	case EM_BUTTON_OK:
		eo << "OK button pressed" << ef;
	//	eo << "Getting TextControl value: " << m_opTextControl -> Text() << ef;
		m_opDialogWindow -> Hide();
		return true;
	case EM_BUTTON_CANCEL:
		eo << "Cancel button pressed" << ef;
		m_opDialogWindow -> Hide();
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

void TransposeDialog::MoveDialog(float p_vX, float p_vY)
{
	m_opDialogWindow -> MoveTo(p_vX, p_vY);
}