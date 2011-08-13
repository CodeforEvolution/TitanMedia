#include "VideoRecordingDialog.h"

#include "EMFactory.h"

#include "EMWindow.h"
#include "EMView.h"
#include "EMTabControl.h"
#include "EMButton.h"
#include "EMLabel.h"
#include "EMTextControl.h"

#include "EMMessages.h"

VideoRecordingDialog::VideoRecordingDialog() :
m_opDialogWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(0, 0, 200, 200), "Video Recording", EM_WINDOW_DIALOG)),
m_opMainView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 200, 200), 0)),
m_opTabControl(EMFactory::Instance() -> CreateTabControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
																EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 180,
																EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 125),
														 "TabControl", true, 0, 0, 0)),

m_opMarkTabView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 0, 0), 0)),
m_opInLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
													    EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 2,
														EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 45,
														EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 2 + EM_METRICS_NATIVE_LABEL_HEIGHT),
												 "In:", 0)),
m_opOutLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 24,
														 EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 45,
														 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 24 + EM_METRICS_NATIVE_LABEL_HEIGHT),
												  "Out:", 0)),
m_opDurationLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 46,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 45,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 46 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													  "Duration:", 0)),
m_opInTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 50,
																	EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
																	EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 50 + 75,
																	EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															 "00:00:00:00", 0, 0, false, EM_ALIGNMENT_RIGHT)),															 
m_opOutTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 50,
																	 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 3 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT,
																	 EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 50 + 75,
																	 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 3 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
															  "00:00:00:00", 0, 0, false, EM_ALIGNMENT_RIGHT)),
m_opDurationTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 50,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + (3 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT) * 2,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 50 + 75,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + (3 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT) * 2 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																   "00:00:00:00", 0, 0, false, EM_ALIGNMENT_RIGHT)),

m_opFileTabView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 0, 0), 0)),
m_opFilenameLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 70,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_LABEL_HEIGHT),
													   "Filename:", 0)),
m_opFilenameTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 16,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 150,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 16 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																   "Edit", 0, 0, false)),
m_opCopyRightLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
															   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 40,
															   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 70,
															   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 40 + EM_METRICS_NATIVE_LABEL_HEIGHT),
														"Copyright:", 0)),
m_opCopyRightTextControl(EMFactory::Instance() -> CreateTextControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																		   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 56,
																		   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 150,
																		   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 56 + EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT),
																	"Edit", 0, 0, false))
{
	m_opDialogWindow -> AddChild(m_opMainView);
	m_opMainView -> AddChild(m_opTabControl);

	m_opTabControl -> AddChild(m_opMarkTabView);
	m_opMarkTabView -> AddChild(m_opInLabel);
	m_opMarkTabView -> AddChild(m_opOutLabel);
	m_opMarkTabView -> AddChild(m_opDurationLabel);
	m_opMarkTabView -> AddChild(m_opInTextControl);
	m_opMarkTabView -> AddChild(m_opOutTextControl);
	m_opMarkTabView -> AddChild(m_opDurationTextControl);

	m_opTabControl -> AddChild(m_opFileTabView);
	m_opFileTabView -> AddChild(m_opFilenameLabel);
	m_opFileTabView -> AddChild(m_opFilenameTextControl);
	m_opFileTabView -> AddChild(m_opCopyRightLabel);
	m_opFileTabView -> AddChild(m_opCopyRightTextControl);
}

VideoRecordingDialog::~VideoRecordingDialog()
{
	delete m_opDialogWindow;
	delete m_opMainView;
	delete m_opTabControl;
	
	delete m_opMarkTabView;
	delete m_opInLabel;
	delete m_opOutLabel;
	delete m_opDurationLabel;
	delete m_opInTextControl;
	delete m_opOutTextControl;
	delete m_opDurationTextControl;

	delete m_opFileTabView;
	delete m_opFilenameLabel;
	delete m_opFilenameTextControl;
	delete m_opCopyRightLabel;
	delete m_opCopyRightTextControl;
}

void VideoRecordingDialog::DisplayDialog()
{
	if(m_opDialogWindow -> Lock())
	{
		m_opDialogWindow -> Show();
		m_opDialogWindow -> Unlock();
	}
}

void VideoRecordingDialog::HideDialog()
{
	if(m_opDialogWindow -> Lock())
	{
		m_opDialogWindow -> Hide();
		m_opDialogWindow -> Unlock();
	}
}

void VideoRecordingDialog::Init()
{
	if(m_opDialogWindow -> Lock())
	{
		//m_opMainView -> SetViewColor(EMColor(192, 192, 192, 255));
		m_opMainView -> SetDefaultColor();
//		m_opMarkTabView -> SetViewColor(EMColor(192, 192, 192, 255));
		//m_opFileTabView -> SetViewColor(EMColor(192, 192, 192, 255));
		m_opFileTabView -> SetDefaultColor();
		m_opDialogWindow -> Unlock();
	}

	m_opTabControl -> AddTab("Mark", m_opMarkTabView);
	m_opTabControl -> AddTab("File", m_opFileTabView);


}

bool VideoRecordingDialog::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	return false;
}

void VideoRecordingDialog::MoveDialog(float p_vX, float p_vY)
{
	m_opDialogWindow -> MoveTo(p_vX, p_vY);
}