#include "QuantizeDialog.h"

#include "EMFactory.h"

#include "EMWindow.h"
#include "EMView.h"
#include "EMButton.h"
#include "EMLabel.h"
#include "EMTextControl.h"
#include "EMCheckBox.h"

#include "EMMessages.h"

const uint32 EM_BUTTON_OK = 1;
const uint32 EM_BUTTON_CANCEL = 2;
const uint32 EM_CHECKBOX_QUANTIZE_NOTE_BEGIN = 3;
const uint32 EM_CHECKBOX_QUANTIZE_NOTE_END = 4;

QuantizeDialog::QuantizeDialog() :
m_opDialogWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(0, 0, 194, 127), "Quantize", EM_WINDOW_DIALOG)), 
m_opMainView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 194, 127), false)),
m_opOK_Button(EMFactory::Instance() -> CreateButton(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 99,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 54,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 99 + EM_METRICS_NATIVE_BUTTON_WIDTH,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 54 + EM_METRICS_NATIVE_BUTTON_HEIGHT),
													"OK", EM_BUTTON_OK, 0, 0)),
m_opCancelButton(EMFactory::Instance() -> CreateButton(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 99,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 82,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 99 + EM_METRICS_NATIVE_BUTTON_WIDTH,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 82 + EM_METRICS_NATIVE_BUTTON_HEIGHT),
													   "Cancel", EM_BUTTON_CANCEL, 0, 0)),
m_opSwingLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 42,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 31,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 42 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"Swing:", 0)),
m_opSwingTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 61,
																	   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 39,
																	   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 61 + 25,
																	   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 39 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																"0", 0, 0, false, EM_ALIGNMENT_RIGHT)),
m_opStrengthLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 65,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 45,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 65 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													   "Strength:", 0)),
m_opStrengthTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 61,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 62,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 61 + 25,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 62 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																   "0", 0, 0, false, EM_ALIGNMENT_RIGHT)),
m_opRandomizeLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
															   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 88,
															   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 58,
															   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 88 + EM_METRICS_NATIVE_LABEL_HEIGHT),
														"Randomize:", 0)),
m_opRandomizeTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 61,
																		   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 85,
																		   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 61 + 25,
																		   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 85 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																	"0", 0, 0, false, EM_ALIGNMENT_RIGHT)),
m_opQuantizeNoteBeginCheckBox(EMFactory::Instance() -> CreateCheckBox(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																			 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
																			 EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 118,
																			 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_CHECKBOX_HEIGHT),
																	   "Quantize Note Begin", EM_CHECKBOX_QUANTIZE_NOTE_BEGIN, 0, 0)),
m_opQuantizeNoteEndCheckBox(EMFactory::Instance() -> CreateCheckBox(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																		   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 17,
																		   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 118,
																		   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 17 + EM_METRICS_NATIVE_CHECKBOX_HEIGHT),
																	"Quantize Note End", EM_CHECKBOX_QUANTIZE_NOTE_END, 0, 0))
{
		m_opDialogWindow -> AddChild(m_opMainView);
		m_opMainView -> AddChild(m_opOK_Button);
		m_opMainView -> AddChild(m_opCancelButton);
		m_opMainView -> AddChild(m_opSwingLabel);
		m_opMainView -> AddChild(m_opSwingTextControl);
		m_opMainView -> AddChild(m_opStrengthLabel);
		m_opMainView -> AddChild(m_opStrengthTextControl);
		m_opMainView -> AddChild(m_opRandomizeLabel);
		m_opMainView -> AddChild(m_opRandomizeTextControl);
		m_opMainView -> AddChild(m_opQuantizeNoteBeginCheckBox);
		m_opMainView -> AddChild(m_opQuantizeNoteEndCheckBox);
}

QuantizeDialog::~QuantizeDialog()
{
	delete m_opDialogWindow;
	delete m_opMainView;
	delete m_opOK_Button;
	delete m_opCancelButton;
	delete m_opSwingLabel;
	delete m_opSwingTextControl;
	delete m_opStrengthLabel;
	delete m_opStrengthTextControl;
	delete m_opRandomizeLabel;
	delete m_opRandomizeTextControl;
	delete m_opQuantizeNoteBeginCheckBox;
	delete m_opQuantizeNoteEndCheckBox;
}

void QuantizeDialog::DisplayDialog()
{
	if(m_opDialogWindow -> Lock())
	{
		m_opDialogWindow -> Show();
		m_opDialogWindow -> Unlock();
	}
}

void QuantizeDialog::HideDialog()
{
	if(m_opDialogWindow -> Lock())
	{
		m_opDialogWindow -> Hide();
		m_opDialogWindow -> Unlock();
	}
}

void QuantizeDialog::Init()
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
	m_opQuantizeNoteBeginCheckBox -> AddListener(this);
	m_opQuantizeNoteEndCheckBox -> AddListener(this);
}

bool QuantizeDialog::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
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
		m_opDialogWindow -> Hide();
		return true;
	case EM_BUTTON_CANCEL:
		eo << "Cancel button pressed" << ef;
		m_opDialogWindow -> Hide();
		return true;
	case EM_CHECKBOX_QUANTIZE_NOTE_BEGIN:
		eo << "Checkbox Begin checked/unchecked" << ef;
		return true;
	case EM_CHECKBOX_QUANTIZE_NOTE_END:
		eo << "Checkbox End checked/unchecked" << ef;
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

void QuantizeDialog::MoveDialog(float p_vX, float p_vY)
{
	m_opDialogWindow -> MoveTo(p_vX, p_vY);
}