#include "ClipPropertiesDialog.h"

#include "EMFactory.h"

#include "EMWindow.h"
#include "EMView.h"
#include "EMButton.h"
#include "EMLabel.h"
#include "EMTextControl.h"
#include "EMGroupBox.h"

#include "EMMessages.h"

#ifdef PLATFORM_WINDOWS
#include <windows.h> // To get modal dialog to work.
#include "EMWinWindow.h" // To get modal dialog to work.
#endif

map<EMWindow*, ClipPropertiesDialogComponents*> ClipPropertiesDialog::m_oComponentsMap;
ClipPropertiesDialogComponents* ClipPropertiesDialog::m_opHadFocus = NULL;
uint32 ClipPropertiesDialog::m_vDisplayCount = 0;
bool ClipPropertiesDialog::m_vDoNotChangeLastFocus = false;

ClipPropertiesDialogComponents* ClipPropertiesDialog::m_opFirstDialog = NULL;
ClipPropertiesDialogComponents* ClipPropertiesDialog::m_opCurrentDialog = NULL;
ClipPropertiesDialogComponents* ClipPropertiesDialog::m_opLastDialog = NULL;


ClipPropertiesDialogComponents::ClipPropertiesDialogComponents() :
m_opPrevFocus(NULL),
m_opNextFocus(NULL)
{
}

ClipPropertiesDialogComponents::~ClipPropertiesDialogComponents()
{
	delete m_opNameLabel;
	delete m_opTypeLabel;
	delete m_opBeginLabel;
	delete m_opEndLabel;
	delete m_opDurationLabel;

	delete m_opNameValueLabel;
	delete m_opTypeValueLabel;
	delete m_opBeginValueLabel;
	delete m_opEndValueLabel;
	delete m_opDurationValueLabel;
	
	if(m_vFormat != TRANSITION)
		delete m_opSeparator;

	if(m_vFormat == AUDIO)
	{
		delete m_opChannelsLabel;
		delete m_opSamplesLabel;
		delete m_opFileLabel;
		delete m_opFileLengthLabel;

		delete m_opChannelsValueLabel;
		delete m_opSamplesValueLabel;
		delete m_opFileValueLabel;
		delete m_opFileLengthValueLabel;
	}
	else if(m_vFormat == MIDI)
	{
		delete m_opEventLabel;
		delete m_opEventValueLabel;
	}
	else if(m_vFormat == VIDEO)
	{
		delete m_opDimentionLabel;
		delete m_opBitDepthLabel;
		delete m_opFramesLabel;
		delete m_opAudioInfoLabel;
		delete m_opFileLabel;

		delete m_opDimentionValueLabel;
		delete m_opBitDepthValueLabel;
		delete m_opFramesValueLabel;
		delete m_opAudioInfoValueLabel;
		delete m_opFileValueLabel;
	}

	delete m_opMainView;
	m_opDialogWindow -> RemoveListener(m_opDialog);
	delete m_opDialogWindow;
}


ClipPropertiesDialog::ClipPropertiesDialog(/*float p_vXpoint, float p_vYpoint, const uint32 p_vFormat*/)// :
/*m_opDialogWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(0, 0, 0, 0/*p_vXpoint, p_vYpoint, p_vXpoint + 218, p_vYpoint + 200 + 22*), "Clip Properties", EM_WINDOW_DIALOG)), 
m_opMainView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 0, 0/*0, 0, 218, 200*), false)),
m_opNameLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
														  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 46,
														  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_LABEL_HEIGHT),
												   "Name:", 0)),
m_opTypeLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 16,
														  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 46,
														  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 16 + EM_METRICS_NATIVE_LABEL_HEIGHT),
												   "Type:", 0)),
m_opBeginLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 32,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 46,
														   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 32 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													"Begin:", 0)),
m_opEndLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 48,
														 EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 46,
														 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 48 + EM_METRICS_NATIVE_LABEL_HEIGHT),
												  "End:", 0)),
m_opDurationLabel(EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 64,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 46,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 64 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													   "Duration:", 0)),
m_opSeparator(EMFactory::Instance() -> CreateGroupBox(EMRect(0, 0, 0, 0), "", 0)),

m_opNameValueLabel(EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), "", 0, EM_ALIGNMENT_RIGHT)),
m_opTypeValueLabel(EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), "", 0, EM_ALIGNMENT_RIGHT)),
m_opBeginValueLabel(EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), "", 0, EM_ALIGNMENT_RIGHT)),
m_opEndValueLabel(EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), "", 0, EM_ALIGNMENT_RIGHT)),
m_opDurationValueLabel(EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), "", 0, EM_ALIGNMENT_RIGHT))
*/
//m_opComponents -> m_vFormat(p_vFormat)
{
/*	if(p_vFormat == AUDIO)
	{
		
		m_vFirstTab = 50;
		
		m_opChannelsLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																		EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 70,
																		EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + m_vFirstTab,
																		EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 70 + EM_METRICS_NATIVE_LABEL_HEIGHT),
																 "Channels:", 0);
		m_opSamplesLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																	   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 86,
																	   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + m_vFirstTab,
																	   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 86 + EM_METRICS_NATIVE_LABEL_HEIGHT),
																"Samples:", 0);
		m_opFileLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																	EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 102,
																	EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + m_vFirstTab,
																	EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 102 + EM_METRICS_NATIVE_LABEL_HEIGHT),
															 "File:", 0);

		m_opNameValueLabel -> SetLabel("Drum Loop");
		m_opTypeValueLabel -> SetLabel("Audio");
		m_opBeginValueLabel -> SetLabel("00:29:00");
		m_opEndValueLabel -> SetLabel("00:35:06");

		m_opChannelsValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), "2", 0, EM_ALIGNMENT_RIGHT);
		m_opSamplesValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), "563810", 0, EM_ALIGNMENT_RIGHT);
		m_opFileValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), "drm_loop.wav", 0 ,EM_ALIGNMENT_RIGHT);

		m_vSecondTab = GetMaxStringWidth();
		m_vSecondTab += m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2;
		eo << "m_vSecondTab = " << m_vSecondTab << ef;
		
		m_opChannelsValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
												  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 70,
												  m_vSecondTab,
												  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 70 + EM_METRICS_NATIVE_LABEL_HEIGHT));
		m_opSamplesValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
												 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 86 ,
												 m_vSecondTab,
												 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 86 + EM_METRICS_NATIVE_LABEL_HEIGHT));
		m_opFileValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
											  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 102,
											  m_vSecondTab,
											  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 102 + EM_METRICS_NATIVE_LABEL_HEIGHT));

	}
	else if(p_vFormat == MIDI)
	{
		m_vFirstTab = 34;
		m_opEventLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																	 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 70,
																	 EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + m_vFirstTab,
																	 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 70 + EM_METRICS_NATIVE_LABEL_HEIGHT),
															  "Event:", 0, EM_ALIGNMENT_RIGHT);
		m_opEventValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), "357", 0, EM_ALIGNMENT_RIGHT);

		m_opNameValueLabel -> SetLabel("Trumpet Section 5");
		m_opTypeValueLabel -> SetLabel("MIDI");
		m_opBeginValueLabel -> SetLabel("15:6:4 (00:29:00)");
		m_opEndValueLabel -> SetLabel("16:6:4 (00:35:06)");

		m_vSecondTab = GetMaxStringWidth();
		m_vSecondTab += m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2;
		eo << "m_vSecondTab = " << m_vSecondTab << ef;

		m_opEventValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
											   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 70,
											   m_vSecondTab,
											   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 70 + EM_METRICS_NATIVE_LABEL_HEIGHT));

	}
	else if(p_vFormat == VIDEO)
	{
		m_vFirstTab = 59;
		
		m_opDimentionLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																		 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 70,
																		 EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + m_vFirstTab,
																		 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 70 + EM_METRICS_NATIVE_LABEL_HEIGHT),
																   "Dimentions:", 0);
		m_opBitDepthLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																		EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 86,
																		EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + m_vFirstTab,
																		EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 86 + EM_METRICS_NATIVE_LABEL_HEIGHT),
																 "Bit Depth:", 0);
		m_opFramesLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 102,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + m_vFirstTab,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 102 + EM_METRICS_NATIVE_LABEL_HEIGHT),
															   "Frames:", 0);
		m_opAudioInfoLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																		 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 118,
																		 EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + m_vFirstTab,
																		 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 118 + EM_METRICS_NATIVE_LABEL_HEIGHT),
																  "Audio Info:", 0);
		m_opFileLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																	EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 134,
																	EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + m_vFirstTab,
																	EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 134 + EM_METRICS_NATIVE_LABEL_HEIGHT),
															 "File:", 0);

		m_opDimentionValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), "320 x 200", 0, EM_ALIGNMENT_RIGHT);
		m_opBitDepthValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), "24", 0, EM_ALIGNMENT_RIGHT);
		m_opFramesValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), "1512", 0, EM_ALIGNMENT_RIGHT);
		m_opAudioInfoValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), "MPEG-1 8bit", 0, EM_ALIGNMENT_RIGHT);
		m_opFileValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), "intro.mpg", 0, EM_ALIGNMENT_RIGHT);

		m_opNameValueLabel -> SetLabel("Intro");
		m_opTypeValueLabel -> SetLabel("Video (MPEG)");
		m_opBeginValueLabel -> SetLabel("00:29:00");
		m_opEndValueLabel -> SetLabel("00:35:06");

		m_vSecondTab = GetMaxStringWidth();
		m_vSecondTab += m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2;
		eo << "m_vSecondTab = " << m_vSecondTab << ef;

		m_opDimentionValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
												   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 70,
												   m_vSecondTab,
												   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 70 + EM_METRICS_NATIVE_LABEL_HEIGHT));
		m_opBitDepthValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
												  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 86,
												  m_vSecondTab,
												  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 86 + EM_METRICS_NATIVE_LABEL_HEIGHT));
		m_opFramesValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
												EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 102,
												m_vSecondTab,
												EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 102 + EM_METRICS_NATIVE_LABEL_HEIGHT));
		m_opAudioInfoValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
												   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 118,
												   m_vSecondTab,
												   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 118 + EM_METRICS_NATIVE_LABEL_HEIGHT));
		m_opFileValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
											  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 134,
											  m_vSecondTab,
											  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 134 + EM_METRICS_NATIVE_LABEL_HEIGHT));
	}

	m_opNameValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
										  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
										  m_vSecondTab,
										  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_LABEL_HEIGHT));
	m_opTypeValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
										  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 16,
										  m_vSecondTab,
										  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 16 + EM_METRICS_NATIVE_LABEL_HEIGHT));
	m_opBeginValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
										   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 32,
										   m_vSecondTab,
										   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 32 + EM_METRICS_NATIVE_LABEL_HEIGHT));
	m_opEndValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
										 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 48,
										 m_vSecondTab,
										 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 48 + EM_METRICS_NATIVE_LABEL_HEIGHT));

	m_opDialogWindow -> AddChild(m_opMainView);
	m_opMainView -> AddChild(m_opNameLabel);
	m_opMainView -> AddChild(m_opTypeLabel);
	m_opMainView -> AddChild(m_opBeginLabel);
	m_opMainView -> AddChild(m_opEndLabel);

	m_opMainView -> AddChild(m_opNameValueLabel);
	m_opMainView -> AddChild(m_opTypeValueLabel);
	m_opMainView -> AddChild(m_opBeginValueLabel);
	m_opMainView -> AddChild(m_opEndValueLabel);

	if(p_vFormat == AUDIO)
	{
		if(m_opDialogWindow ->Lock())
		{
			m_opDialogWindow -> SetFrame(EMRect(p_vXpoint, p_vYpoint, m_vSecondTab + 11, 139));
			m_opDialogWindow -> SetTitle("Audio Clip Properies");
			m_opMainView -> SetFrame(EMRect(0, 0, m_vSecondTab + 11, 139));
			m_opDialogWindow -> Unlock();
		}
		m_opMainView -> AddChild(m_opChannelsLabel);
		m_opMainView -> AddChild(m_opSamplesLabel);
		m_opMainView -> AddChild(m_opFileLabel);

		m_opMainView -> AddChild(m_opChannelsValueLabel);
		m_opMainView -> AddChild(m_opSamplesValueLabel);
		m_opMainView -> AddChild(m_opFileValueLabel);
	}
	else if(p_vFormat == MIDI)
	{
		if(m_opDialogWindow ->Lock())
		{
			m_opDialogWindow -> SetFrame(EMRect(p_vXpoint, p_vYpoint, m_vSecondTab + 11, 109));
			m_opDialogWindow -> SetTitle("Midi Clip Properies");
			m_opMainView -> SetFrame(EMRect(0, 0, m_vSecondTab + 11, 109));
			m_opDialogWindow -> Unlock();
		}
		m_opMainView -> AddChild(m_opEventLabel);
		m_opMainView -> AddChild(m_opEventValueLabel);
	}
	else if(p_vFormat == VIDEO)
	{
	
		if(m_opDialogWindow ->Lock())
		{
			m_opDialogWindow -> SetFrame(EMRect(p_vXpoint, p_vYpoint, m_vSecondTab + 11, 172));
			m_opDialogWindow -> SetTitle("Video Clip Properies");
			m_opMainView -> SetFrame(EMRect(0, 0, m_vSecondTab + 11, 172));
			m_opDialogWindow ->Unlock();
		}

		m_opMainView -> AddChild(m_opDimentionLabel);
		m_opMainView -> AddChild(m_opBitDepthLabel);
		m_opMainView -> AddChild(m_opFramesLabel);
		m_opMainView -> AddChild(m_opAudioInfoLabel);
		m_opMainView -> AddChild(m_opFileLabel);

		m_opMainView -> AddChild(m_opDimentionValueLabel);
		m_opMainView -> AddChild(m_opBitDepthValueLabel);
		m_opMainView -> AddChild(m_opFramesValueLabel);
		m_opMainView -> AddChild(m_opAudioInfoValueLabel);
		m_opMainView -> AddChild(m_opFileValueLabel);
	}
*/
}

ClipPropertiesDialog::~ClipPropertiesDialog()
{
}

void ClipPropertiesDialog::AddDialog(ClipPropertiesDialogComponents* p_opAddDialog)
{
	if(m_opFirstDialog == NULL)
	{
		m_opFirstDialog = m_opCurrentDialog = m_opLastDialog = p_opAddDialog;
		return;
	}
	else
	{
		m_opCurrentDialog = p_opAddDialog;
		m_opLastDialog -> m_opNextFocus = m_opCurrentDialog;
		m_opCurrentDialog -> m_opPrevFocus = m_opLastDialog;
		m_opLastDialog = m_opCurrentDialog;
	}
}

void ClipPropertiesDialog::ConvertToScreenCoordinates(int32 &p_vX, int32 &p_vY)
{
	m_opComponents -> m_opMainView -> ConvertToScreenCoordinates(p_vX, p_vY);
}

void ClipPropertiesDialog::DeleteAllControls()
{
/*	delete m_opDialogWindow;
	delete m_opMainView;
	delete m_opNameLabel;
	delete m_opTypeLabel;
	delete m_opBeginLabel;
	delete m_opEndLabel;
	delete m_opDurationLabel;

	delete m_opNameValueLabel;
	delete m_opTypeValueLabel;
	delete m_opBeginValueLabel;
	delete m_opEndValueLabel;
	delete m_opDurationValueLabel;
	
	delete m_opSeparator;

	if(m_opComponents -> m_vFormat == AUDIO)
	{
		delete m_opChannelsLabel;
		delete m_opSamplesLabel;
		delete m_opFileLabel;
		delete m_opFileLengthLabel;

		delete m_opChannelsValueLabel;
		delete m_opSamplesValueLabel;
		delete m_opFileValueLabel;
		delete m_opFileLengthValueLabel;
	}
	else if(m_opComponents -> m_vFormat == MIDI)
	{
		delete m_opEventLabel;
		delete m_opEventValueLabel;
	}
	else if(m_opComponents -> m_vFormat == VIDEO)
	{
		delete m_opDimentionLabel;
		delete m_opBitDepthLabel;
		delete m_opFramesLabel;
		delete m_opAudioInfoLabel;
		delete m_opFileLabel;

		delete m_opDimentionValueLabel;
		delete m_opBitDepthValueLabel;
		delete m_opFramesValueLabel;
		delete m_opAudioInfoValueLabel;
		delete m_opFileValueLabel;
	}
*/
}

void ClipPropertiesDialog::DeleteDialog(ClipPropertiesDialogComponents* p_opDeleteDialog)
{
//	MoveDialog(p_opDeleteDialog);
	
	m_opLastDialog -> m_opPrevFocus -> m_opNextFocus = NULL;
	m_opCurrentDialog = m_opLastDialog -> m_opPrevFocus;
	m_opLastDialog = m_opLastDialog -> m_opPrevFocus;
	/*if(m_opCurrentDialog -> m_opPrevFocus != NULL)
		m_opCurrentDialog -> m_opPrevFocus -> m_opDialogWindow -> SetFocus();
	else if(m_opCurrentDialog -> m_opNextFocus != NULL)
		m_opCurrentDialog -> m_opNextFocus -> m_opDialogWindow -> SetFocus();
	*/
}

void ClipPropertiesDialog::DisplayDialog()
{
	if(m_opComponents -> m_opDialogWindow -> Lock())
	{
		m_opComponents -> m_opDialogWindow -> Show();
#ifdef PLATFORM_WINDOWS
		if(m_vDisplayCount == 0)
			EnableWindow(dynamic_cast<EMWinWindow*>(m_opComponents -> m_opDialogWindow) -> GetMainWindowHandle(), FALSE);
		m_opComponents -> m_opDialogWindow -> SetFocus();
#endif
		m_opComponents -> m_opDialogWindow -> Unlock();
		m_vDisplayCount++;
		eo << "DisplayCount Added = " << m_vDisplayCount << ef;
	}
}

int32 ClipPropertiesDialog::GetMaxStringWidth()
{
	float vMaxWidth = m_opComponents -> m_opNameValueLabel -> GetStringWidth();
	if(vMaxWidth < m_opComponents -> m_opTypeValueLabel -> GetStringWidth())
		vMaxWidth = m_opComponents -> m_opTypeValueLabel -> GetStringWidth();
	if(vMaxWidth < m_opComponents -> m_opBeginValueLabel -> GetStringWidth())
		vMaxWidth = m_opComponents -> m_opBeginValueLabel -> GetStringWidth();
	if(vMaxWidth < m_opComponents -> m_opEndValueLabel -> GetStringWidth())
		vMaxWidth = m_opComponents -> m_opEndValueLabel -> GetStringWidth();
	if(vMaxWidth < m_opComponents -> m_opDurationValueLabel -> GetStringWidth())
		vMaxWidth = m_opComponents -> m_opDurationValueLabel -> GetStringWidth();
	if(m_opComponents -> m_vFormat == AUDIO)
	{
		if(vMaxWidth < m_opComponents -> m_opChannelsValueLabel -> GetStringWidth())
			vMaxWidth = m_opComponents -> m_opChannelsValueLabel -> GetStringWidth();
		if(vMaxWidth < m_opComponents -> m_opSamplesValueLabel -> GetStringWidth())
			vMaxWidth = m_opComponents -> m_opSamplesValueLabel -> GetStringWidth();
		if(vMaxWidth < m_opComponents -> m_opFileValueLabel -> GetStringWidth())
			vMaxWidth = m_opComponents -> m_opFileValueLabel -> GetStringWidth();
		if(vMaxWidth < m_opComponents -> m_opFileLengthValueLabel -> GetStringWidth())
			vMaxWidth = m_opComponents -> m_opFileLengthValueLabel -> GetStringWidth();
	}
	if(m_opComponents -> m_vFormat == MIDI)
		if(vMaxWidth < m_opComponents -> m_opEventValueLabel -> GetStringWidth())
			vMaxWidth = m_opComponents -> m_opEventValueLabel -> GetStringWidth();
	if(m_opComponents -> m_vFormat == VIDEO)
	{
		if(vMaxWidth < m_opComponents -> m_opDimentionValueLabel -> GetStringWidth())
			vMaxWidth = m_opComponents -> m_opDimentionValueLabel -> GetStringWidth();
		if(vMaxWidth < m_opComponents -> m_opBitDepthValueLabel -> GetStringWidth())
			vMaxWidth = m_opComponents -> m_opBitDepthValueLabel -> GetStringWidth();
		if(vMaxWidth < m_opComponents -> m_opFramesValueLabel -> GetStringWidth())
			vMaxWidth = m_opComponents -> m_opFramesValueLabel -> GetStringWidth();
		if(vMaxWidth < m_opComponents -> m_opAudioInfoValueLabel -> GetStringWidth())
			vMaxWidth = m_opComponents -> m_opAudioInfoValueLabel -> GetStringWidth();
		if(vMaxWidth < m_opComponents -> m_opFileValueLabel -> GetStringWidth())
			vMaxWidth = m_opComponents -> m_opFileValueLabel -> GetStringWidth();
		if(vMaxWidth < m_opComponents -> m_opFileLengthValueLabel -> GetStringWidth())
			vMaxWidth = m_opComponents -> m_opFileLengthValueLabel -> GetStringWidth();
	}
	return vMaxWidth;
}

EMWindow* ClipPropertiesDialog::GetDialogWindow()
{
	return m_opComponents -> m_opDialogWindow;
}

void ClipPropertiesDialog::HideDialog(EMWindow* p_opDialogWindow)
{
	if(p_opDialogWindow -> Lock())
	{
		if(--m_vDisplayCount == 0)
		{
#ifdef PLATFORM_WINDOWS
		EnableWindow(dynamic_cast<EMWinWindow*>(p_opDialogWindow) -> GetMainWindowHandle(), TRUE);
		SetFocus(dynamic_cast<EMWinWindow*>(p_opDialogWindow) -> GetMainWindowHandle());
#endif
		m_opFirstDialog = m_opCurrentDialog = m_opLastDialog = NULL;
		}
		else
		{
			m_oComponentsMap[p_opDialogWindow] -> m_opPrevFocus -> m_opDialogWindow -> SetFocus();
			DeleteDialog(m_oComponentsMap[p_opDialogWindow]);
		}

		eo << "DisplayCount after sub. = " << m_vDisplayCount << ef;

		p_opDialogWindow -> Hide();
		p_opDialogWindow -> Unlock();
		
		ClipPropertiesDialogComponents* opComponents = m_oComponentsMap[p_opDialogWindow];
		delete opComponents;
		m_oComponentsMap.erase(p_opDialogWindow);
		eo << "m_oComponentsMap.size() = " << m_oComponentsMap.size() << ef;
	}
}

void ClipPropertiesDialog::Init(float p_vXpoint, float p_vYpoint, const uint32 p_vFormat)
{
	m_opComponents = EM_new ClipPropertiesDialogComponents;
	m_opComponents -> m_opDialogWindow = EMFactory::Instance() -> CreateEMWindow(EMRect(0, 0, 0, 0/*p_vXpoint, p_vYpoint, p_vXpoint + 218, p_vYpoint + 200 + 22*/), m_oNameValue.c_str(), EM_WINDOW_DIALOG); 

	//m_opComponents -> m_opPrevFocus = (*m_oComponentsMap.rbegin()).second -> m_opDialogWindow;
//	m_opComponents -> m_opPrevFocus = m_opHadFocus;
//	m_opHadFocus = m_opComponents -> m_opDialogWindow;
	
	m_oComponentsMap[m_opComponents -> m_opDialogWindow] = m_opComponents; // To be able to delete all controls in all dialogs
	AddDialog(m_opComponents);

	m_opComponents -> m_opMainView = EMFactory::Instance() -> CreateView(EMRect(0, 0, 0, 0/*0, 0, 218, 200*/), false);
	m_opComponents -> m_opNameLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 46,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_LABEL_HEIGHT),
													   "Name:", 0);
	m_opComponents -> m_opTypeLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 16,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 46,
															  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 16 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													   "Type:", 0);
	m_opComponents -> m_opBeginLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
															   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 32,
															   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 46,
															   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 32 + EM_METRICS_NATIVE_LABEL_HEIGHT),
														"Begin:", 0);
	m_opComponents -> m_opEndLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
															 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 48,
															 EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 46,
															 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 48 + EM_METRICS_NATIVE_LABEL_HEIGHT),
													  "End:", 0);
	m_opComponents -> m_opDurationLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 64,
																  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 46,
																  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 64 + EM_METRICS_NATIVE_LABEL_HEIGHT),
														   "Duration:", 0);
	if(p_vFormat != TRANSITION)
		m_opComponents -> m_opSeparator = EMFactory::Instance() -> CreateGroupBox(EMRect(0, 0, 0, 0), "", 0);

	m_opComponents -> m_opNameValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), m_oNameValue.c_str(), 0, EM_ALIGNMENT_RIGHT);
	m_opComponents -> m_opTypeValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), m_oTypeValue.c_str(), 0, EM_ALIGNMENT_RIGHT);
	m_opComponents -> m_opBeginValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), m_oBeginValue.c_str(), 0, EM_ALIGNMENT_RIGHT);
	m_opComponents -> m_opEndValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), m_oEndValue.c_str(), 0, EM_ALIGNMENT_RIGHT);
	m_opComponents -> m_opDurationValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), m_oDurationValue.c_str(), 0, EM_ALIGNMENT_RIGHT);
	m_opComponents -> m_vFormat = p_vFormat;
	if(p_vFormat == AUDIO)
	{
		
		m_vFirstTab = 58;
		
		m_opComponents -> m_opChannelsLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																		EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 86,
																		EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + m_vFirstTab,
																		EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 86 + EM_METRICS_NATIVE_LABEL_HEIGHT),
																 "Channels:", 0);
		m_opComponents -> m_opSamplesLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																	   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 102,
																	   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + m_vFirstTab,
																	   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 102 + EM_METRICS_NATIVE_LABEL_HEIGHT),
																"Samples:", 0);
		m_opComponents -> m_opFileLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																	EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 118,
																	EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + m_vFirstTab,
																	EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 118 + EM_METRICS_NATIVE_LABEL_HEIGHT),
															 "File:", 0);
		m_opComponents -> m_opFileLengthLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 134,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + m_vFirstTab,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 134 + EM_METRICS_NATIVE_LABEL_HEIGHT),
																   "File Length:", 0);
		m_opComponents -> m_opMarkInLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 150,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + m_vFirstTab,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 150 + EM_METRICS_NATIVE_LABEL_HEIGHT),
															   "Mark In:", 0);
		m_opComponents -> m_opMarkOutLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 166,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + m_vFirstTab,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 166 + EM_METRICS_NATIVE_LABEL_HEIGHT),
															   "Mark Out:", 0);

/*		m_opNameValueLabel -> SetLabel("Drum Loop");
		m_opTypeValueLabel -> SetLabel("Audio");
		m_opBeginValueLabel -> SetLabel("00:29:00");
		m_opEndValueLabel -> SetLabel("00:35:06");
		m_opDurationValueLabel -> SetLabel("00:06:06");
*/
		m_opComponents -> m_opChannelsValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), m_oChannelsValue.c_str(), 0, EM_ALIGNMENT_RIGHT);
		m_opComponents -> m_opSamplesValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), m_oSamplesValue.c_str(), 0, EM_ALIGNMENT_RIGHT);
		m_opComponents -> m_opFileValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), m_oFileValue.c_str(), 0 ,EM_ALIGNMENT_RIGHT);
		m_opComponents -> m_opFileLengthValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), m_oFileLengthValue.c_str(), 0 ,EM_ALIGNMENT_RIGHT);
		m_opComponents -> m_opMarkInValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), m_oMarkInValue.c_str(), 0 ,EM_ALIGNMENT_RIGHT);
		m_opComponents -> m_opMarkOutValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), m_oMarkOutValue.c_str(), 0 ,EM_ALIGNMENT_RIGHT);

		m_vSecondTab = GetMaxStringWidth();
		m_vSecondTab += m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2;
		eo << "m_vSecondTab = " << m_vSecondTab << ef;
		
		m_opComponents -> m_opChannelsValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
												  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 86,
												  m_vSecondTab,
												  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 86 + EM_METRICS_NATIVE_LABEL_HEIGHT));
		m_opComponents -> m_opSamplesValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
												 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 102 ,
												 m_vSecondTab,
												 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 102 + EM_METRICS_NATIVE_LABEL_HEIGHT));
		m_opComponents -> m_opFileValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
											  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 118,
											  m_vSecondTab,
											  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 118 + EM_METRICS_NATIVE_LABEL_HEIGHT));
		m_opComponents -> m_opFileLengthValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
													EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 134,
													m_vSecondTab,
													EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 134 + EM_METRICS_NATIVE_LABEL_HEIGHT));
		m_opComponents -> m_opMarkInValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
												EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 150,
												m_vSecondTab,
												EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 150 + EM_METRICS_NATIVE_LABEL_HEIGHT));
		m_opComponents -> m_opMarkOutValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
												 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 166,
												 m_vSecondTab,
												 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 166 + EM_METRICS_NATIVE_LABEL_HEIGHT));

	}
	else if(p_vFormat == MIDI)
	{
		m_vFirstTab = 46;
		m_opComponents -> m_opEventLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																	 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 86,
																	 EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + m_vFirstTab,
																	 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 86 + EM_METRICS_NATIVE_LABEL_HEIGHT),
															  "Events:", 0);
		m_opComponents -> m_opEventValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), m_oEventValue.c_str(), 0, EM_ALIGNMENT_RIGHT);

/*		m_opNameValueLabel -> SetLabel("Trumpet Section 5");
		m_opTypeValueLabel -> SetLabel("MIDI");
		m_opBeginValueLabel -> SetLabel("15:6:4 (00:29:00)");
		m_opEndValueLabel -> SetLabel("16:6:4 (00:35:06)");
		m_opDurationValueLabel -> SetLabel("00:06:06");
*/
		m_vSecondTab = GetMaxStringWidth();
		m_vSecondTab += m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2;
		eo << "m_vSecondTab = " << m_vSecondTab << ef;

		m_opComponents -> m_opEventValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
											   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 86,
											   m_vSecondTab,
											   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 86 + EM_METRICS_NATIVE_LABEL_HEIGHT));

	}
	else if(p_vFormat == VIDEO)
	{
		m_vFirstTab = 59;
		
		m_opComponents -> m_opDimentionLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																		 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 86,
																		 EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + m_vFirstTab,
																		 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 86 + EM_METRICS_NATIVE_LABEL_HEIGHT),
																   "Dimentions:", 0);
		m_opComponents -> m_opBitDepthLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																		EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 102,
																		EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + m_vFirstTab,
																		EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 102 + EM_METRICS_NATIVE_LABEL_HEIGHT),
																 "Bit Depth:", 0);
		m_opComponents -> m_opFramesLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 118,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + m_vFirstTab,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 118 + EM_METRICS_NATIVE_LABEL_HEIGHT),
															   "Frames:", 0);
		m_opComponents -> m_opAudioInfoLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																		 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 134,
																		 EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + m_vFirstTab,
																		 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 134 + EM_METRICS_NATIVE_LABEL_HEIGHT),
																  "Audio Info:", 0);
		m_opComponents -> m_opFileLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																	EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 150,
																	EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + m_vFirstTab,
																	EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 150 + EM_METRICS_NATIVE_LABEL_HEIGHT),
															 "File:", 0);
		m_opComponents -> m_opFileLengthLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 166,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + m_vFirstTab,
																		  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 166 + EM_METRICS_NATIVE_LABEL_HEIGHT),
																   "File Length:", 0);
		m_opComponents -> m_opMarkInLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 182,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + m_vFirstTab,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 182 + EM_METRICS_NATIVE_LABEL_HEIGHT),
															   "Mark In:", 0);
		m_opComponents -> m_opMarkOutLabel = EMFactory::Instance() -> CreateLabel(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 198,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + m_vFirstTab,
																	  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 198 + EM_METRICS_NATIVE_LABEL_HEIGHT),
															   "Mark Out:", 0);

/*		m_opNameValueLabel -> SetLabel("Intro");
		m_opTypeValueLabel -> SetLabel("Video (MPEG)");
		m_opBeginValueLabel -> SetLabel("00:29:00");
		m_opEndValueLabel -> SetLabel("00:35:06");
		m_opDurationValueLabel -> SetLabel("00:06:06");
*/
		m_opComponents -> m_opDimentionValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), m_oDimentionValue.c_str(), 0, EM_ALIGNMENT_RIGHT);
		m_opComponents -> m_opBitDepthValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), m_oBitDepthValue.c_str(), 0, EM_ALIGNMENT_RIGHT);
		m_opComponents -> m_opFramesValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), m_oFramesValue.c_str(), 0, EM_ALIGNMENT_RIGHT);
		m_opComponents -> m_opAudioInfoValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), m_oAudioInfoValue.c_str(), 0, EM_ALIGNMENT_RIGHT);
		m_opComponents -> m_opFileValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), m_oFileValue.c_str(), 0, EM_ALIGNMENT_RIGHT);
		m_opComponents -> m_opFileLengthValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), m_oFileLengthValue.c_str(), 0 ,EM_ALIGNMENT_RIGHT);
		m_opComponents -> m_opMarkInValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), m_oMarkInValue.c_str(), 0 ,EM_ALIGNMENT_RIGHT);
		m_opComponents -> m_opMarkOutValueLabel = EMFactory::Instance() -> CreateLabel(EMRect(0, 0, 0, 0), m_oMarkOutValue.c_str(), 0 ,EM_ALIGNMENT_RIGHT);

		m_vSecondTab = GetMaxStringWidth();
		m_vSecondTab += m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2;
		eo << "m_vSecondTab = " << m_vSecondTab << ef;

		m_opComponents -> m_opDimentionValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
												   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 86,
												   m_vSecondTab,
												   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 86 + EM_METRICS_NATIVE_LABEL_HEIGHT));
		m_opComponents -> m_opBitDepthValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
												  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 102,
												  m_vSecondTab,
												  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 102 + EM_METRICS_NATIVE_LABEL_HEIGHT));
		m_opComponents -> m_opFramesValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
												EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 118,
												m_vSecondTab,
												EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 118 + EM_METRICS_NATIVE_LABEL_HEIGHT));
		m_opComponents -> m_opAudioInfoValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
												   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 134,
												   m_vSecondTab,
												   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 134 + EM_METRICS_NATIVE_LABEL_HEIGHT));
		m_opComponents -> m_opFileValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
											  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 150,
											  m_vSecondTab,
											  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 150 + EM_METRICS_NATIVE_LABEL_HEIGHT));
		m_opComponents -> m_opFileLengthValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
													EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 166,
													m_vSecondTab,
													EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 166 + EM_METRICS_NATIVE_LABEL_HEIGHT));
		m_opComponents -> m_opMarkInValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
												EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 182,
												m_vSecondTab,
												EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 182 + EM_METRICS_NATIVE_LABEL_HEIGHT));
		m_opComponents -> m_opMarkOutValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
												 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 198,
												 m_vSecondTab,
												 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 198 + EM_METRICS_NATIVE_LABEL_HEIGHT));
	}
	else if(p_vFormat == TRANSITION)
	{
		m_vFirstTab = 46;
		m_vSecondTab = GetMaxStringWidth();
		m_vSecondTab += m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2;
		eo << "m_vSecondTab = " << m_vSecondTab << ef;
	}

	m_opComponents -> m_opNameValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
										  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
										  m_vSecondTab,
										  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + EM_METRICS_NATIVE_LABEL_HEIGHT));
	m_opComponents -> m_opTypeValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
										  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 16,
										  m_vSecondTab,
										  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 16 + EM_METRICS_NATIVE_LABEL_HEIGHT));
	m_opComponents -> m_opBeginValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
										   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 32,
										   m_vSecondTab,
										   EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 32 + EM_METRICS_NATIVE_LABEL_HEIGHT));
	m_opComponents -> m_opEndValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
										 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 48,
										 m_vSecondTab,
										 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 48 + EM_METRICS_NATIVE_LABEL_HEIGHT));
	m_opComponents -> m_opDurationValueLabel -> SetFrame(EMRect(m_vFirstTab + EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + 2,
											  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 64,
											  m_vSecondTab,
											  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 64 + EM_METRICS_NATIVE_LABEL_HEIGHT));
	if(p_vFormat != TRANSITION)
		m_opComponents -> m_opSeparator -> SetFrame(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
										 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 75,
										 m_vSecondTab,
										 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 75 + 7));

	m_opComponents -> m_opDialogWindow -> AddChild(m_opComponents -> m_opMainView);
	m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opNameLabel);
	m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opTypeLabel);
	m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opBeginLabel);
	m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opEndLabel);
	m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opDurationLabel);

	m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opNameValueLabel);
	m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opTypeValueLabel);
	m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opBeginValueLabel);
	m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opEndValueLabel);
	m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opDurationValueLabel);

	if(p_vFormat != TRANSITION)
		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opSeparator);

	if(p_vFormat == AUDIO)
	{
		if(m_opComponents -> m_opDialogWindow ->Lock())
		{
			m_opComponents -> m_opDialogWindow -> SetFrame(EMRect(p_vXpoint, p_vYpoint, m_vSecondTab + 11, 202));
//			m_opComponents -> m_opDialogWindow -> SetBounds(EMRect(p_vXpoint, p_vYpoint, m_vSecondTab + 11, 122));
			m_opComponents -> m_opDialogWindow -> SetTitle("Audio Clip Properies");
			m_opComponents -> m_opMainView -> SetFrame(EMRect(0, 0, m_vSecondTab + 11, 202));
			m_opComponents -> m_opDialogWindow -> Unlock();
		}
		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opChannelsLabel);
		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opSamplesLabel);
		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opFileLabel);
		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opFileLengthLabel);
		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opMarkInLabel);
		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opMarkOutLabel);

		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opChannelsValueLabel);
		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opSamplesValueLabel);
		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opFileValueLabel);
		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opFileLengthValueLabel);
		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opMarkInValueLabel);
		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opMarkOutValueLabel);
	}
	else if(p_vFormat == MIDI)
	{
		if(m_opComponents -> m_opDialogWindow ->Lock())
		{
			m_opComponents -> m_opDialogWindow -> SetFrame(EMRect(p_vXpoint, p_vYpoint, m_vSecondTab + 11, 122));
//			m_opComponents -> m_opDialogWindow -> SetBounds(EMRect(p_vXpoint, p_vYpoint, m_vSecondTab + 11, 122));
			m_opComponents -> m_opDialogWindow -> SetTitle("Midi Clip Properies");
			m_opComponents -> m_opMainView -> SetFrame(EMRect(0, 0, m_vSecondTab + 11, 122));
			m_opComponents -> m_opDialogWindow -> Unlock();
		}
		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opEventLabel);
		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opEventValueLabel);
	}
	else if(p_vFormat == VIDEO)
	{
	
		if(m_opComponents -> m_opDialogWindow ->Lock())
		{
			m_opComponents -> m_opDialogWindow -> SetFrame(EMRect(p_vXpoint, p_vYpoint, m_vSecondTab + 11, 234));
//			m_opComponents -> m_opDialogWindow -> SetBounds(EMRect(p_vXpoint, p_vYpoint, m_vSecondTab + 11, 122));
			m_opComponents -> m_opDialogWindow -> SetTitle("Video Clip Properies");
			m_opComponents -> m_opMainView -> SetFrame(EMRect(0, 0, m_vSecondTab + 11, 234));
			m_opComponents -> m_opDialogWindow ->Unlock();
		}

		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opDimentionLabel);
		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opBitDepthLabel);
		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opFramesLabel);
		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opAudioInfoLabel);
		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opFileLabel);
		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opFileLengthLabel);
		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opMarkInLabel);
		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opMarkOutLabel);

		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opDimentionValueLabel);
		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opBitDepthValueLabel);
		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opFramesValueLabel);
		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opAudioInfoValueLabel);
		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opFileValueLabel);
		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opFileLengthValueLabel);
		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opMarkInValueLabel);
		m_opComponents -> m_opMainView -> AddChild(m_opComponents -> m_opMarkOutValueLabel);
	}
	else if(p_vFormat == TRANSITION)
	{
		if(m_opComponents -> m_opDialogWindow ->Lock())
		{
			m_opComponents -> m_opDialogWindow -> SetFrame(EMRect(p_vXpoint, p_vYpoint, m_vSecondTab + 11, 100));
//			m_opComponents -> m_opDialogWindow -> SetBounds(EMRect(p_vXpoint, p_vYpoint, m_vSecondTab + 11, 100));
			m_opComponents -> m_opDialogWindow -> SetTitle("Transition Clip Properies");
			m_opComponents -> m_opMainView -> SetFrame(EMRect(0, 0, m_vSecondTab + 11, 100));
			m_opComponents -> m_opDialogWindow -> Unlock();
		}
	}
	m_opComponents -> m_opMainView -> AddListener(this);
	if(m_opComponents -> m_opDialogWindow -> Lock())
	{
		//m_opComponents -> m_opMainView -> SetViewColor(EMColor(0, 208, 0,/*192, 192, 192,*/ 255));
		m_opComponents -> m_opMainView -> SetDefaultColor();
		m_opComponents -> m_opDialogWindow -> Unlock();
	}
/*	EMRect oWindowFrame(m_opDialogWindow ->Frame());
	m_opSeparator -> SetFrame(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
									 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 75,
									 EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + oWindowFrame.m_vRight - oWindowFrame.m_vLeft - EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE * 2,
									 EM_METRICS_NATIVE_DIALOG_MARGIN_TOP + 75 + 7));
*/
	m_opComponents -> m_opDialogWindow -> AddListener(this);
}

bool ClipPropertiesDialog::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
/*	case EM_DRAW:
		if(p_opSender == m_opMainView)
		{
			eo << "EM_DRAW: m_oBackgroundView" << ef;
			m_opMainView -> SetHighColor(EMColor(0, 255, 0, 255));
			m_opMainView -> FillRect(m_opMainView -> Bounds());
		}
		else
			eo << "EM_DRAW (unknown)" << ef;
		return true;
*/
	case EM_GUI_COMPONENT_GOT_FOCUS:
		m_oComponentsIterator = m_oComponentsMap.begin();
		while(m_oComponentsIterator != m_oComponentsMap.end())
		{
			if(p_opSender == (*m_oComponentsIterator).first)
			{
				if(m_vDoNotChangeLastFocus == false)
					MoveDialog((*m_oComponentsIterator).second);
				m_vDoNotChangeLastFocus = false;
				//m_opHadFocus = (*m_oComponentsIterator).second;
				return true;
			}
			m_oComponentsIterator++;
		}
		return false;
	case EM_CLOSE_WINDOW:
//		if(p_opSender == m_opDialogWindow)
//		{
			m_vDoNotChangeLastFocus = true;
			eo << "EM_CLOSE_WINDOW" << ef;
			HideDialog(dynamic_cast<EMWindow*>(p_opSender));
			return true; 
//		}
		break;

	}
	return false;
}

void ClipPropertiesDialog::MoveDialog(ClipPropertiesDialogComponents* p_opMoveDialog)
{
	m_opCurrentDialog = p_opMoveDialog;

	if(m_opCurrentDialog != m_opLastDialog)
	{
		if(m_opCurrentDialog -> m_opNextFocus != NULL)
			m_opCurrentDialog -> m_opNextFocus -> m_opPrevFocus = m_opCurrentDialog -> m_opPrevFocus;
		if(m_opCurrentDialog -> m_opPrevFocus != NULL)
			m_opCurrentDialog -> m_opPrevFocus -> m_opNextFocus = m_opCurrentDialog -> m_opNextFocus;

		m_opCurrentDialog -> m_opNextFocus = NULL;
		m_opLastDialog -> m_opNextFocus = m_opCurrentDialog;
		m_opCurrentDialog -> m_opPrevFocus = m_opLastDialog;
	}
	m_opLastDialog = m_opCurrentDialog;
}

void ClipPropertiesDialog::MoveDialog(float p_vX, float p_vY)
{
	m_opComponents -> m_opDialogWindow -> MoveTo(p_vX, p_vY);
}

void ClipPropertiesDialog::SetAudioInfoValue(string p_oValue)
{
	m_oAudioInfoValue = p_oValue;
}

void ClipPropertiesDialog::SetBeginValue(string p_oValue)
{
	m_oBeginValue = p_oValue;
}

void ClipPropertiesDialog::SetBitDepthValue(string p_oValue)
{
	m_oBitDepthValue = p_oValue;
}

void ClipPropertiesDialog::SetChannelsValue(string p_oValue)
{
	m_oChannelsValue = p_oValue;
}

void ClipPropertiesDialog::SetDimentionValue(string p_oValue)
{
	m_oDimentionValue = p_oValue;
}

void ClipPropertiesDialog::SetDurationValue(string p_oValue)
{
	m_oDurationValue = p_oValue;
}

void ClipPropertiesDialog::SetEndValue(string p_oValue)
{
	m_oEndValue = p_oValue;
}

void ClipPropertiesDialog::SetEventValue(string p_oValue)
{
	m_oEventValue = p_oValue;
}

void ClipPropertiesDialog::SetFileValue(string p_oValue)
{
	m_oFileValue = p_oValue;
}

void ClipPropertiesDialog::SetFileLengthValue(string p_oValue)
{
	m_oFileLengthValue = p_oValue;
}

void ClipPropertiesDialog::SetFramesValue(string p_oValue)
{
	m_oFramesValue = p_oValue;
}

void ClipPropertiesDialog::SetMarkInValue(string p_oValue)
{
	m_oMarkInValue = p_oValue;
}

void ClipPropertiesDialog::SetMarkOutValue(string p_oValue)
{
	m_oMarkOutValue = p_oValue;
}

void ClipPropertiesDialog::SetNameValue(string p_oValue)
{
	m_oNameValue = p_oValue;
}

void ClipPropertiesDialog::SetSamplesValue(string p_oValue)
{
	m_oSamplesValue = p_oValue;
}

void ClipPropertiesDialog::SetTypeValue(string p_oValue)
{
	m_oTypeValue = p_oValue;
}