#include "UtilityView.h"

#include "AudioInputSignalView.h"
#include "ColaborationView.h"
#include "CommandIDs.h"
#include "EMFactory.h"
#include "EMGUIGlobals.h"
#include "EMMessages.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"
#include "EMResourceRepository.h"
#include "EMVideoPreview.h"
#include "EMView.h"
#include "ExplorerView.h"
#include "MediaPoolView.h"
#include "ResourceIDs.h"
#include "TrackWindow.h"
#include "VideoPreviewView.h"

const int32 UTILITY_BUTTON_SIZE = 34;
const int32 DEFAULT_UTILITY_VIEW_HEIGHT = 176;
const int32 H_RESIZE_BAR_SIZE = 7;

UtilityView::UtilityView() :
m_opView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 100, 100), EM_FOLLOW_LEFT_RIGHT | EM_FOLLOW_BOTTOM, false)),
m_oAudioInputButtonL(m_opView, EMRect((m_opView -> Bounds()).GetWidth() / 2 - UTILITY_BUTTON_SIZE - 1,
								H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE * 3,
								(m_opView -> Bounds()).GetWidth() / 2 - 2,
								H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE * 4 - 1),
					EM_FOLLOW_H_CENTER | EM_FOLLOW_TOP, 
					NULL, NULL, COMMAND_UTILITY_BUTTON_PRESSED, m_opView, NULL, NULL, true, true, true, true,
					EM_COLOR_BUTTON_DEFAULT_OFF, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Displays Audio Input Signals"),
m_oAudioInputButtonR(m_opView, EMRect((m_opView -> Bounds()).GetWidth() / 2 + 2,
								H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE * 3,
								(m_opView -> Bounds()).GetWidth() / 2 + UTILITY_BUTTON_SIZE + 1,
								H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE * 4 - 1),
					EM_FOLLOW_H_CENTER | EM_FOLLOW_TOP, 
					NULL, NULL, COMMAND_UTILITY_BUTTON_PRESSED, m_opView, NULL, NULL, true, true, true, true,
					EM_COLOR_BUTTON_DEFAULT_OFF, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Displays Audio Input Signals"),
m_oFileButtonL(m_opView, EMRect((m_opView -> Bounds()).GetWidth() / 2 - UTILITY_BUTTON_SIZE - 1,
								H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE,
								(m_opView -> Bounds()).GetWidth() / 2 - 2,
								H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE * 2 - 1),
					EM_FOLLOW_H_CENTER | EM_FOLLOW_TOP, 
					NULL, NULL, COMMAND_UTILITY_BUTTON_PRESSED, m_opView, NULL, NULL, true, true, true, true,
					EM_COLOR_BUTTON_DEFAULT_OFF, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Displays The File Explorer"),
m_oFileButtonR(m_opView, EMRect((m_opView -> Bounds()).GetWidth() / 2 + 2,
								H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE,
								(m_opView -> Bounds()).GetWidth() / 2 + UTILITY_BUTTON_SIZE + 1,
								H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE * 2 - 1),
					EM_FOLLOW_H_CENTER | EM_FOLLOW_TOP, 
					NULL, NULL, COMMAND_UTILITY_BUTTON_PRESSED, m_opView, NULL, NULL, true, true, true, true,
					EM_COLOR_BUTTON_DEFAULT_OFF, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Displays The File Explorer"),
m_oInternetButtonL(m_opView, EMRect((m_opView -> Bounds()).GetWidth() / 2 - UTILITY_BUTTON_SIZE - 1,
								H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE * 4,
								(m_opView -> Bounds()).GetWidth() / 2 - 2,
								H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE * 5 - 1),
					EM_FOLLOW_H_CENTER | EM_FOLLOW_TOP, 
					NULL, NULL, COMMAND_UTILITY_BUTTON_PRESSED, m_opView, NULL, NULL, true, true, true, true,
					EM_COLOR_BUTTON_DEFAULT_OFF, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Displays The Collaboration View"),
m_oInternetButtonR(m_opView, EMRect((m_opView -> Bounds()).GetWidth() / 2 + 2,
								H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE * 4,
								(m_opView -> Bounds()).GetWidth() / 2 + UTILITY_BUTTON_SIZE + 1,
								H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE * 5 - 1),
					EM_FOLLOW_H_CENTER | EM_FOLLOW_TOP, 
					NULL, NULL, COMMAND_UTILITY_BUTTON_PRESSED, m_opView, NULL, NULL, true, true, true, true,
					EM_COLOR_BUTTON_DEFAULT_OFF, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Displays The Collaboration View"),
m_oMediaPoolButtonL(m_opView, EMRect((m_opView -> Bounds()).GetWidth() / 2 - UTILITY_BUTTON_SIZE - 1,
								H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE * 2,
								(m_opView -> Bounds()).GetWidth() / 2 - 2,
								H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE * 3 - 1),
					EM_FOLLOW_H_CENTER | EM_FOLLOW_TOP, 
					NULL, NULL, COMMAND_UTILITY_BUTTON_PRESSED, m_opView, NULL, NULL, true, true, true, true,
					EM_COLOR_BUTTON_DEFAULT_OFF, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Displays The Media Pool"),
m_oMediaPoolButtonR(m_opView, EMRect((m_opView -> Bounds()).GetWidth() / 2 + 2,
								H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE * 2,
								(m_opView -> Bounds()).GetWidth() / 2 + UTILITY_BUTTON_SIZE + 1,
								H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE * 3 - 1),
					EM_FOLLOW_H_CENTER | EM_FOLLOW_TOP, 
					NULL, NULL, COMMAND_UTILITY_BUTTON_PRESSED, m_opView, NULL, NULL, true, true, true, true,
					EM_COLOR_BUTTON_DEFAULT_OFF, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Displays The Media Pool"),
m_opAudioInputSignalView(EM_new AudioInputSignalView()),
//m_opColaborationView(EM_new ColaborationView()),
//ExplorerView::Instance()(ExplorerView::Instance()),
m_opLastButtonPressed(NULL),
//MediaPoolView::Instance()(EM_new MediaPoolView()),
m_opVBorder(EMFactory::Instance() -> CreateView(EMRect((m_opView -> Bounds()).GetWidth() / 2 - 1, 3, (m_opView -> Bounds()).GetWidth() / 2 + 1, 100), EM_FOLLOW_H_CENTER | EM_FOLLOW_TOP_BOTTOM, false)),
m_opVideoPreviewView(EM_new VideoPreviewView()),
m_opWindow(NULL),
m_oVideoPreviewButtonL(m_opView, EMRect((m_opView -> Bounds()).GetWidth() / 2 - UTILITY_BUTTON_SIZE - 1,
								H_RESIZE_BAR_SIZE ,
								(m_opView -> Bounds()).GetWidth() / 2 - 2,
								H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE - 1),
					EM_FOLLOW_H_CENTER | EM_FOLLOW_TOP, 
					NULL, NULL, COMMAND_UTILITY_BUTTON_PRESSED, m_opView, NULL, NULL, true, true, true, true,
					EM_COLOR_BUTTON_DEFAULT_OFF, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Displays The Video Preview View"),
m_oVideoPreviewButtonR(m_opView, EMRect((m_opView -> Bounds()).GetWidth() / 2 + 2,
								H_RESIZE_BAR_SIZE ,
								(m_opView -> Bounds()).GetWidth() / 2 + UTILITY_BUTTON_SIZE + 1,
								H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE - 1),
					EM_FOLLOW_H_CENTER | EM_FOLLOW_TOP, 
					NULL, NULL, COMMAND_UTILITY_BUTTON_PRESSED, m_opView, NULL, NULL, true, true, true, true,
					EM_COLOR_BUTTON_DEFAULT_OFF, EM_COLOR_BUTTON_DEFAULT_OFF,
					NULL, NULL, NULL, NULL, "Displays The Video Preview View"),
m_vHeight(DEFAULT_UTILITY_VIEW_HEIGHT),
m_vIsDirty(false),
m_vMouseIsDown(false)
{
}

UtilityView::~UtilityView()
{
	// m_opVideoPreviewView is not deleted here since it is managed by EMVideoPreview::Instance() (deleted by application init)
	ExplorerView::Instance() -> DeleteAllTreeViewItems();
	delete m_opAudioInputSignalView;
//	delete m_opColaborationView;
//	delete ExplorerView::Instance();
//	delete MediaPoolView::Instance();
	delete m_opView;
	delete m_opVBorder;
}

void UtilityView::ButtonPressed(EMSlimButton* p_opButton)
{
	if(p_opButton == m_oAudioInputButtonL.GetButton() ||
		p_opButton == m_oFileButtonL.GetButton() ||
		p_opButton == m_oInternetButtonL.GetButton() ||
		p_opButton == m_oMediaPoolButtonL.GetButton() ||
		p_opButton == m_oVideoPreviewButtonL.GetButton())
	{
		if(p_opButton != m_oAudioInputButtonL.GetButton() && m_oAudioInputButtonL.GetButton() -> GetOn())
		{
			m_oAudioInputButtonL.GetButton() -> SetOn(false);
			m_oAudioInputButtonL.InvalidateButton();
			m_opAudioInputSignalView -> EnableSignalMeters(false);
			m_opAudioInputSignalView -> GetView() -> Hide();
		}
		else if(p_opButton != m_oFileButtonL.GetButton() && m_oFileButtonL.GetButton() -> GetOn())
		{
			m_oFileButtonL.GetButton() -> SetOn(false);
			m_oFileButtonL.InvalidateButton();
			ExplorerView::Instance() -> GetView() -> Hide();
		}
		else if(p_opButton != m_oInternetButtonL.GetButton() && m_oInternetButtonL.GetButton() -> GetOn())
		{
			m_oInternetButtonL.GetButton() -> SetOn(false);
			m_oInternetButtonL.InvalidateButton();
			ColaborationView::Instance() -> GetView() -> Hide();
		}
		else if(p_opButton != m_oMediaPoolButtonL.GetButton() && m_oMediaPoolButtonL.GetButton() -> GetOn())
		{
			m_oMediaPoolButtonL.GetButton() -> SetOn(false);
			m_oMediaPoolButtonL.InvalidateButton();
			MediaPoolView::Instance() -> GetView() -> Hide();
		}
		else if(p_opButton != m_oVideoPreviewButtonL.GetButton() && m_oVideoPreviewButtonL.GetButton() -> GetOn())
		{
			m_oVideoPreviewButtonL.GetButton() -> SetOn(false);
			m_oVideoPreviewButtonL.InvalidateButton();
			m_opVideoPreviewView -> HidePreview();
		}
	}
	else
	{
		if(p_opButton != m_oAudioInputButtonR.GetButton() && m_oAudioInputButtonR.GetButton() -> GetOn())
		{
			m_oAudioInputButtonR.GetButton() -> SetOn(false);
			m_oAudioInputButtonR.InvalidateButton();
			m_opAudioInputSignalView -> EnableSignalMeters(false);
			m_opAudioInputSignalView -> GetView() -> Hide();
		}
		else if(p_opButton != m_oFileButtonR.GetButton() && m_oFileButtonR.GetButton() -> GetOn())
		{
			m_oFileButtonR.GetButton() -> SetOn(false);
			m_oFileButtonR.InvalidateButton();
			ExplorerView::Instance() -> GetView() -> Hide();
		}
		else if(p_opButton != m_oInternetButtonR.GetButton() && m_oInternetButtonR.GetButton() -> GetOn())
		{
			m_oInternetButtonR.GetButton() -> SetOn(false);
			m_oInternetButtonR.InvalidateButton();
			ColaborationView::Instance() -> GetView() -> Hide();
		}
		else if(p_opButton != m_oMediaPoolButtonR.GetButton() && m_oMediaPoolButtonR.GetButton() -> GetOn())
		{
			m_oMediaPoolButtonR.GetButton() -> SetOn(false);
			m_oMediaPoolButtonR.InvalidateButton();
			MediaPoolView::Instance() -> GetView() -> Hide();
		}
		else if(p_opButton != m_oVideoPreviewButtonR.GetButton() && m_oVideoPreviewButtonR.GetButton() -> GetOn())
		{
			m_oVideoPreviewButtonR.GetButton() -> SetOn(false);
			m_oVideoPreviewButtonR.InvalidateButton();
			m_opVideoPreviewView -> HidePreview();
		}
	}

	if(p_opButton == m_oAudioInputButtonL.GetButton())
	{
		if(m_oAudioInputButtonR.GetButton() -> GetOn())
		{
			m_oAudioInputButtonR.GetButton() -> SetOn(false);
			m_oAudioInputButtonR.InvalidateButton();
		}
		m_opAudioInputSignalView -> GetView() -> SetFrame(EMRect(0, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).GetWidth() / 2 - 2 - UTILITY_BUTTON_SIZE, (m_opView -> Bounds()).m_vBottom));
		m_opAudioInputSignalView -> EnableSignalMeters(true);
		if(m_opAudioInputSignalView -> GetView() -> IsHidden())
			m_opAudioInputSignalView -> GetView() -> Show();
	}
	else if(p_opButton == m_oAudioInputButtonR.GetButton())
	{
		if(m_oAudioInputButtonL.GetButton() -> GetOn())
		{
			m_oAudioInputButtonL.GetButton() -> SetOn(false);
			m_oAudioInputButtonL.InvalidateButton();
		}
		m_opAudioInputSignalView -> GetView() -> SetFrame(EMRect((m_opView -> Bounds()).GetWidth() / 2 + 2 + UTILITY_BUTTON_SIZE, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).m_vRight, (m_opView -> Bounds()).m_vBottom));
		m_opAudioInputSignalView -> EnableSignalMeters(true);
		if(m_opAudioInputSignalView -> GetView() -> IsHidden())
			m_opAudioInputSignalView -> GetView() -> Show();
	}
	else if(p_opButton == m_oFileButtonL.GetButton())
	{
		if(m_oFileButtonR.GetButton() -> GetOn())
		{
			m_oFileButtonR.GetButton() -> SetOn(false);
			m_oFileButtonR.InvalidateButton();
		}
		ExplorerView::Instance() -> GetView() -> SetFrame(EMRect(0, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).GetWidth() / 2 - 2 - UTILITY_BUTTON_SIZE, (m_opView -> Bounds()).m_vBottom));
		if(ExplorerView::Instance() -> GetView() -> IsHidden())
			ExplorerView::Instance() -> GetView() -> Show();
	}
	else if(p_opButton == m_oFileButtonR.GetButton())
	{
		if(m_oFileButtonL.GetButton() -> GetOn())
		{
			m_oFileButtonL.GetButton() -> SetOn(false);
			m_oFileButtonL.InvalidateButton();
		}
		ExplorerView::Instance() -> GetView() -> SetFrame(EMRect((m_opView -> Bounds()).GetWidth() / 2 + 2 + UTILITY_BUTTON_SIZE, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).m_vRight, (m_opView -> Bounds()).m_vBottom));
		if(ExplorerView::Instance() -> GetView() -> IsHidden())
			ExplorerView::Instance() -> GetView() -> Show();
	}
	else if(p_opButton == m_oInternetButtonL.GetButton())
	{
		if(m_oInternetButtonR.GetButton() -> GetOn())
		{
			m_oInternetButtonR.GetButton() -> SetOn(false);
			m_oInternetButtonR.InvalidateButton();
		}
		ColaborationView::Instance() -> GetView() -> SetFrame(EMRect(0, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).GetWidth() / 2 - 2 - UTILITY_BUTTON_SIZE, (m_opView -> Bounds()).m_vBottom));
		if(ColaborationView::Instance() -> GetView() -> IsHidden())
			ColaborationView::Instance() -> GetView() -> Show();
	}
	else if(p_opButton == m_oInternetButtonR.GetButton())
	{
		if(m_oInternetButtonL.GetButton() -> GetOn())
		{
			m_oInternetButtonL.GetButton() -> SetOn(false);
			m_oInternetButtonL.InvalidateButton();
		}
		ColaborationView::Instance() -> GetView() -> SetFrame(EMRect((m_opView -> Bounds()).GetWidth() / 2 + 2 + UTILITY_BUTTON_SIZE, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).m_vRight, (m_opView -> Bounds()).m_vBottom));
		if(ColaborationView::Instance() -> GetView() -> IsHidden())
			ColaborationView::Instance() -> GetView() -> Show();
	}
	else if(p_opButton == m_oMediaPoolButtonL.GetButton())
	{
		if(m_oMediaPoolButtonR.GetButton() -> GetOn())
		{
			m_oMediaPoolButtonR.GetButton() -> SetOn(false);
			m_oMediaPoolButtonR.InvalidateButton();
		}
		MediaPoolView::Instance() -> GetView() -> SetFrame(EMRect(0, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).GetWidth() / 2 - 2 - UTILITY_BUTTON_SIZE, (m_opView -> Bounds()).m_vBottom));
		if(MediaPoolView::Instance() -> GetView() -> IsHidden())
			MediaPoolView::Instance() -> GetView() -> Show();
	}
	else if(p_opButton == m_oMediaPoolButtonR.GetButton())
	{
		if(m_oMediaPoolButtonL.GetButton() -> GetOn())
		{
			m_oMediaPoolButtonL.GetButton() -> SetOn(false);
			m_oMediaPoolButtonL.InvalidateButton();
		}
		MediaPoolView::Instance() -> GetView() -> SetFrame(EMRect((m_opView -> Bounds()).GetWidth() / 2 + 2 + UTILITY_BUTTON_SIZE, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).m_vRight, (m_opView -> Bounds()).m_vBottom));
		if(MediaPoolView::Instance() -> GetView() -> IsHidden())
			MediaPoolView::Instance() -> GetView() -> Show();
	}
	else if(p_opButton == m_oVideoPreviewButtonL.GetButton())
	{
		if(m_oVideoPreviewButtonR.GetButton() -> GetOn())
		{
			m_oVideoPreviewButtonR.GetButton() -> SetOn(false);
			m_oVideoPreviewButtonR.InvalidateButton();
		}
		m_opVideoPreviewView -> GetView() -> SetFrame(EMRect(0, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).GetWidth() / 2 - 2 - UTILITY_BUTTON_SIZE, (m_opView -> Bounds()).m_vBottom));
		if(m_opVideoPreviewView -> GetView() -> IsHidden())
			m_opVideoPreviewView -> ShowPreview();
	}
	else if(p_opButton == m_oVideoPreviewButtonR.GetButton())
	{
		if(m_oVideoPreviewButtonL.GetButton() -> GetOn())
		{
			m_oVideoPreviewButtonL.GetButton() -> SetOn(false);
			m_oVideoPreviewButtonL.InvalidateButton();
		}
		m_opVideoPreviewView -> GetView() -> SetFrame(EMRect((m_opView -> Bounds()).GetWidth() / 2 + 2 + UTILITY_BUTTON_SIZE, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).m_vRight, (m_opView -> Bounds()).m_vBottom));
		if(m_opVideoPreviewView -> GetView() -> IsHidden())
			m_opVideoPreviewView -> ShowPreview();
	}
	
	m_vIsDirty = true;
}

bool UtilityView::ClearData()
{
	if(m_oAudioInputButtonL.GetButton() -> GetOn())
	{
		m_oAudioInputButtonL.GetButton() -> SetOn(false);
		m_opAudioInputSignalView -> GetView() -> Hide();
	}
	else if(m_oAudioInputButtonR.GetButton() -> GetOn())
	{
		m_oAudioInputButtonR.GetButton() -> SetOn(false);
		m_opAudioInputSignalView -> GetView() -> Hide();
	}
	if(m_oFileButtonL.GetButton() -> GetOn())
	{
		m_oFileButtonL.GetButton() -> SetOn(false);
		ExplorerView::Instance() -> GetView() -> Hide();
	}
	if(m_oFileButtonR.GetButton() -> GetOn())
	{
		m_oFileButtonR.GetButton() -> SetOn(false);
		ExplorerView::Instance() -> GetView() -> Hide();
	}
	if(m_oInternetButtonL.GetButton() -> GetOn())
	{
		m_oInternetButtonL.GetButton() -> SetOn(false);
		ColaborationView::Instance() -> GetView() -> Hide();
	}
	else if(m_oInternetButtonR.GetButton() -> GetOn())
	{
		m_oInternetButtonR.GetButton() -> SetOn(false);
		ColaborationView::Instance() -> GetView() -> Hide();
	}
	if(m_oMediaPoolButtonL.GetButton() -> GetOn())
	{
		m_oMediaPoolButtonL.GetButton() -> SetOn(false);
		MediaPoolView::Instance() -> GetView() -> Hide();
	}
	else if(m_oMediaPoolButtonR.GetButton() -> GetOn())
	{
		m_oMediaPoolButtonR.GetButton() -> SetOn(false);
		MediaPoolView::Instance() -> GetView() -> Hide();
	}
	if(m_oVideoPreviewButtonL.GetButton() -> GetOn())
	{
		m_oVideoPreviewButtonL.GetButton() -> SetOn(false);
		m_opVideoPreviewView -> HidePreview();
	}
	else if(m_oVideoPreviewButtonR.GetButton() -> GetOn())
	{
		m_oVideoPreviewButtonR.GetButton() -> SetOn(false);
		m_opVideoPreviewView -> HidePreview();
	}

	return true;
}

void UtilityView::Draw(EMRect p_oClippingRect)
{
	m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_HIGHLIGHT);
	m_opView -> StrokeLine(p_oClippingRect.m_vLeft, (m_opView -> Bounds()).m_vTop, p_oClippingRect.m_vRight, (m_opView -> Bounds()).m_vTop);
	m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND);
	m_opView -> FillRect(EMRect(p_oClippingRect.m_vLeft, 1, p_oClippingRect.m_vRight, H_RESIZE_BAR_SIZE - 2));
//	m_opView -> StrokeLine(p_oClippingRect.m_vLeft, (m_opView -> Bounds()).m_vTop + 1, p_oClippingRect.m_vRight, (m_opView -> Bounds()).m_vTop + 1);
	m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_SHADOW);
	m_opView -> StrokeLine(p_oClippingRect.m_vLeft, H_RESIZE_BAR_SIZE - 1, p_oClippingRect.m_vRight, H_RESIZE_BAR_SIZE - 1);
}

void UtilityView::DrawBorder(EMRect p_oClippingRect)
{
	m_opVBorder -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_HIGHLIGHT);
	m_opVBorder -> StrokeLine(0, 0, 0, (m_opView -> Bounds()).m_vBottom);
	m_opVBorder -> SetHighColor(EM_COLOR_VIEW_BACKGROUND);
	m_opVBorder -> StrokeLine(1, 0, 1, (m_opView -> Bounds()).m_vBottom);
	m_opVBorder -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_SHADOW);
	m_opVBorder -> StrokeLine(2, 0, 2, (m_opView -> Bounds()).m_vBottom);
}

int32 UtilityView::GetDefaultHeight()
{
	return DEFAULT_UTILITY_VIEW_HEIGHT;
}

int32 UtilityView::GetHeight()
{
	return m_vHeight;
}

EMVideoPreview* UtilityView::GetVideoPreviewView()
{
	return m_opVideoPreviewView;
}

EMView* UtilityView::GetView()
{
	return m_opView;
}

void UtilityView::Init()
{
	if(m_opView -> LockWindow())
	{
		m_oAudioInputButtonL.GetButton() -> SetBitmaps(EMResourceRepository::GetBitmap(RES_UV_AUDIO_INPUT_ON), EMResourceRepository::GetBitmap(RES_UV_AUDIO_INPUT));
		m_oAudioInputButtonR.GetButton() -> SetBitmaps(EMResourceRepository::GetBitmap(RES_UV_AUDIO_INPUT_ON), EMResourceRepository::GetBitmap(RES_UV_AUDIO_INPUT));
		m_oFileButtonL.GetButton() -> SetBitmaps(EMResourceRepository::GetBitmap(RES_UV_FILE_ON), EMResourceRepository::GetBitmap(RES_UV_FILE));
		m_oFileButtonR.GetButton() -> SetBitmaps(EMResourceRepository::GetBitmap(RES_UV_FILE_ON), EMResourceRepository::GetBitmap(RES_UV_FILE));
		m_oInternetButtonL.GetButton() -> SetBitmaps(EMResourceRepository::GetBitmap(RES_UV_INTERNET_ON), EMResourceRepository::GetBitmap(RES_UV_INTERNET));
		m_oInternetButtonR.GetButton() -> SetBitmaps(EMResourceRepository::GetBitmap(RES_UV_INTERNET_ON), EMResourceRepository::GetBitmap(RES_UV_INTERNET));
		m_oMediaPoolButtonL.GetButton() -> SetBitmaps(EMResourceRepository::GetBitmap(RES_UV_MEDIA_POOL_ON), EMResourceRepository::GetBitmap(RES_UV_MEDIA_POOL));
		m_oMediaPoolButtonR.GetButton() -> SetBitmaps(EMResourceRepository::GetBitmap(RES_UV_MEDIA_POOL_ON), EMResourceRepository::GetBitmap(RES_UV_MEDIA_POOL));
		m_oVideoPreviewButtonL.GetButton() -> SetBitmaps(EMResourceRepository::GetBitmap(RES_UV_VIDEO_PREVIEW_ON), EMResourceRepository::GetBitmap(RES_UV_VIDEO_PREVIEW));
		m_oVideoPreviewButtonR.GetButton() -> SetBitmaps(EMResourceRepository::GetBitmap(RES_UV_VIDEO_PREVIEW_ON), EMResourceRepository::GetBitmap(RES_UV_VIDEO_PREVIEW));

		m_opView -> SetViewColor(EM_COLOR_VIEW_BACKGROUND);
		m_opView -> AddListener(this);
		m_opVBorder -> AddListener(this);
	
		m_oAudioInputButtonL.GetButton() -> SetMode(EM_SLIM_BUTTON_RADIO);
		m_oFileButtonL.GetButton() -> SetMode(EM_SLIM_BUTTON_RADIO);
		m_oInternetButtonL.GetButton() -> SetMode(EM_SLIM_BUTTON_RADIO);
		m_oMediaPoolButtonL.GetButton() -> SetMode(EM_SLIM_BUTTON_RADIO);
		m_oVideoPreviewButtonL.GetButton() -> SetMode(EM_SLIM_BUTTON_RADIO);
		m_oAudioInputButtonR.GetButton() -> SetMode(EM_SLIM_BUTTON_RADIO);
		m_oFileButtonR.GetButton() -> SetMode(EM_SLIM_BUTTON_RADIO);
		m_oInternetButtonR.GetButton() -> SetMode(EM_SLIM_BUTTON_RADIO);
		m_oMediaPoolButtonR.GetButton() -> SetMode(EM_SLIM_BUTTON_RADIO);
		m_oVideoPreviewButtonR.GetButton() -> SetMode(EM_SLIM_BUTTON_RADIO);

		m_opAudioInputSignalView -> Init();
		m_opVideoPreviewView -> Init();

		m_opView -> AddChild(MediaPoolView::Instance() -> GetView());
		MediaPoolView::Instance() -> Init();

		m_opView -> AddChild(ColaborationView::Instance() -> GetView());
		ColaborationView::Instance() -> Init();

		m_opView -> AddChild(ExplorerView::Instance() -> GetView());
		ExplorerView::Instance() -> Init();

		m_opView -> AddChild(m_oAudioInputButtonL.GetView());
		m_opView -> AddChild(m_oFileButtonL.GetView());
		m_opView -> AddChild(m_oInternetButtonL.GetView());
		m_opView -> AddChild(m_oMediaPoolButtonL.GetView());
		m_opView -> AddChild(m_oVideoPreviewButtonL.GetView());
		m_opView -> AddChild(m_oAudioInputButtonR.GetView());
		m_opView -> AddChild(m_oFileButtonR.GetView());
		m_opView -> AddChild(m_oInternetButtonR.GetView());
		m_opView -> AddChild(m_oMediaPoolButtonR.GetView());
		m_opView -> AddChild(m_oVideoPreviewButtonR.GetView());
		m_opView -> AddChild(m_opVBorder);
		m_opView -> AddChild(m_opVideoPreviewView -> GetView());
		m_opView -> AddChild(m_opAudioInputSignalView -> GetView());
	
		m_opVideoPreviewView -> GetView() -> Hide();
		m_opAudioInputSignalView -> GetView() -> Hide();
		MediaPoolView::Instance() -> GetView() -> Hide();
		ColaborationView::Instance() -> GetView() -> Hide();
		ExplorerView::Instance() -> GetView() -> Hide();

		m_opView -> UnlockWindow();
	}
}

bool UtilityView::IsDirty()
{
	return m_vIsDirty;
}

void UtilityView::LayoutViews()
{
	m_oAudioInputButtonL.GetView() -> SetFrame(EMRect((m_opView -> Bounds()).GetWidth() / 2 - UTILITY_BUTTON_SIZE - 1,
											H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE * 3,
											(m_opView -> Bounds()).GetWidth() / 2 - 2,
											H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE * 4 - 1));
	m_oAudioInputButtonR.GetView() -> SetFrame(EMRect((m_opView -> Bounds()).GetWidth() / 2 + 2,
											H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE * 3,
											(m_opView -> Bounds()).GetWidth() / 2 + UTILITY_BUTTON_SIZE + 1,
											H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE * 4 - 1));
	m_oFileButtonL.GetView() -> SetFrame(EMRect((m_opView -> Bounds()).GetWidth() / 2 - UTILITY_BUTTON_SIZE - 1,
											H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE,
											(m_opView -> Bounds()).GetWidth() / 2 - 2,
											H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE * 2 - 1));
	m_oFileButtonR.GetView() -> SetFrame(EMRect((m_opView -> Bounds()).GetWidth() / 2 + 2,
											H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE,
											(m_opView -> Bounds()).GetWidth() / 2 + UTILITY_BUTTON_SIZE + 1,
											H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE * 2 - 1));
	m_oInternetButtonL.GetView() -> SetFrame(EMRect((m_opView -> Bounds()).GetWidth() / 2 - UTILITY_BUTTON_SIZE - 1,
											H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE * 4,
											(m_opView -> Bounds()).GetWidth() / 2 - 2,
											H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE * 5 - 1));
	m_oInternetButtonR.GetView() -> SetFrame(EMRect((m_opView -> Bounds()).GetWidth() / 2 + 2,
											H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE * 4,
											(m_opView -> Bounds()).GetWidth() / 2 + UTILITY_BUTTON_SIZE + 1,
											H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE * 5 - 1));
	m_oMediaPoolButtonL.GetView() -> SetFrame(EMRect((m_opView -> Bounds()).GetWidth() / 2 - UTILITY_BUTTON_SIZE - 1,
											H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE * 2,
											(m_opView -> Bounds()).GetWidth() / 2 - 2,
											H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE * 3 - 1));
	m_oMediaPoolButtonR.GetView() -> SetFrame(EMRect((m_opView -> Bounds()).GetWidth() / 2 + 2,
											H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE * 2,
											(m_opView -> Bounds()).GetWidth() / 2 + UTILITY_BUTTON_SIZE + 1,
											H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE * 3 - 1));
	m_oVideoPreviewButtonL.GetView() -> SetFrame(EMRect((m_opView -> Bounds()).GetWidth() / 2 - UTILITY_BUTTON_SIZE - 1,
											H_RESIZE_BAR_SIZE,
											(m_opView -> Bounds()).GetWidth() / 2 - 2,
											H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE - 1));
	m_oVideoPreviewButtonR.GetView() -> SetFrame(EMRect((m_opView -> Bounds()).GetWidth() / 2 + 2,
											H_RESIZE_BAR_SIZE,
											(m_opView -> Bounds()).GetWidth() / 2 + UTILITY_BUTTON_SIZE + 1,
											H_RESIZE_BAR_SIZE + UTILITY_BUTTON_SIZE - 1));
	m_opVBorder -> SetFrame(EMRect((m_opView -> Bounds()).GetWidth() / 2 - 1, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).GetWidth() / 2 + 1, (m_opView -> Bounds()).m_vBottom));
}

bool UtilityView::LoadData(void* p_upProjectDataLoader)
{
	EMProjectDataLoader* opLoader = static_cast<EMProjectDataLoader*>(p_upProjectDataLoader);

	m_oAudioInputButtonL.GetButton() -> SetOn(opLoader -> LoadBool());
	if(m_oAudioInputButtonL.GetButton() -> GetOn())
	{
		m_opAudioInputSignalView -> GetView() -> SetFrame(EMRect(0, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).GetWidth() / 2 - 2 - UTILITY_BUTTON_SIZE, (m_opView -> Bounds()).m_vBottom));
		m_opAudioInputSignalView -> EnableSignalMeters(true);
		if(m_opAudioInputSignalView -> GetView() -> IsHidden())
			m_opAudioInputSignalView -> GetView() -> Show();
	}

	m_oAudioInputButtonR.GetButton() -> SetOn(opLoader -> LoadBool());
	if(m_oAudioInputButtonR.GetButton() -> GetOn())
	{
		m_opAudioInputSignalView -> GetView() -> SetFrame(EMRect((m_opView -> Bounds()).GetWidth() / 2 + 2 + UTILITY_BUTTON_SIZE, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).m_vRight, (m_opView -> Bounds()).m_vBottom));
		m_opAudioInputSignalView -> EnableSignalMeters(true);
		if(m_opAudioInputSignalView -> GetView() -> IsHidden())
			m_opAudioInputSignalView -> GetView() -> Show();
	}

	m_oFileButtonL.GetButton() -> SetOn(opLoader -> LoadBool());
	if(m_oFileButtonL.GetButton() -> GetOn())
	{
		ExplorerView::Instance() -> GetView() -> SetFrame(EMRect(0, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).GetWidth() / 2 - 2 - UTILITY_BUTTON_SIZE, (m_opView -> Bounds()).m_vBottom));
		if(ExplorerView::Instance() -> GetView() -> IsHidden())
			ExplorerView::Instance() -> GetView() -> Show();
	}

	m_oFileButtonR.GetButton() -> SetOn(opLoader -> LoadBool());
	if(m_oFileButtonR.GetButton() -> GetOn())
	{
		ExplorerView::Instance() -> GetView() -> SetFrame(EMRect((m_opView -> Bounds()).GetWidth() / 2 + 2 + UTILITY_BUTTON_SIZE, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).m_vRight, (m_opView -> Bounds()).m_vBottom));
		if(ExplorerView::Instance() -> GetView() -> IsHidden())
			ExplorerView::Instance() -> GetView() -> Show();
	}

	m_oInternetButtonL.GetButton() -> SetOn(opLoader -> LoadBool());
	if(m_oInternetButtonL.GetButton() -> GetOn())
	{
		ColaborationView::Instance() -> GetView() -> SetFrame(EMRect(0, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).GetWidth() / 2 - 2 - UTILITY_BUTTON_SIZE, (m_opView -> Bounds()).m_vBottom));
		if(ColaborationView::Instance() -> GetView() -> IsHidden())
			ColaborationView::Instance() -> GetView() -> Show();
	}

	m_oInternetButtonR.GetButton() -> SetOn(opLoader -> LoadBool());
	if(m_oInternetButtonR.GetButton() -> GetOn())
	{
		ColaborationView::Instance() -> GetView() -> SetFrame(EMRect((m_opView -> Bounds()).GetWidth() / 2 + 2 + UTILITY_BUTTON_SIZE, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).m_vRight, (m_opView -> Bounds()).m_vBottom));
		if(ColaborationView::Instance() -> GetView() -> IsHidden())
			ColaborationView::Instance() -> GetView() -> Show();
	}

	m_oMediaPoolButtonL.GetButton() -> SetOn(opLoader -> LoadBool());
	if(m_oMediaPoolButtonL.GetButton() -> GetOn())
	{
		MediaPoolView::Instance() -> GetView() -> SetFrame(EMRect(0, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).GetWidth() / 2 - 2 - UTILITY_BUTTON_SIZE, (m_opView -> Bounds()).m_vBottom));
		if(MediaPoolView::Instance() -> GetView() -> IsHidden())
			MediaPoolView::Instance() -> GetView() -> Show();
	}

	m_oMediaPoolButtonR.GetButton() -> SetOn(opLoader -> LoadBool());
	if(m_oMediaPoolButtonR.GetButton() -> GetOn())
	{
		MediaPoolView::Instance() -> GetView() -> SetFrame(EMRect((m_opView -> Bounds()).GetWidth() / 2 + 2 + UTILITY_BUTTON_SIZE, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).m_vRight, (m_opView -> Bounds()).m_vBottom));
		if(MediaPoolView::Instance() -> GetView() -> IsHidden())
			MediaPoolView::Instance() -> GetView() -> Show();
	}

	m_oVideoPreviewButtonL.GetButton() -> SetOn(opLoader -> LoadBool());
	if(m_oVideoPreviewButtonL.GetButton() -> GetOn())
	{
		m_opVideoPreviewView -> GetView() -> SetFrame(EMRect(0, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).GetWidth() / 2 - 2 - UTILITY_BUTTON_SIZE, (m_opView -> Bounds()).m_vBottom));
		if(m_opVideoPreviewView -> GetView() -> IsHidden())
			m_opVideoPreviewView -> ShowPreview();
	}

	m_oVideoPreviewButtonR.GetButton() -> SetOn(opLoader -> LoadBool());
	if(m_oVideoPreviewButtonR.GetButton() -> GetOn())
	{
		m_opVideoPreviewView -> GetView() -> SetFrame(EMRect((m_opView -> Bounds()).GetWidth() / 2 + 2 + UTILITY_BUTTON_SIZE, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).m_vRight, (m_opView -> Bounds()).m_vBottom));
		if(m_opVideoPreviewView -> GetView() -> IsHidden())
			m_opVideoPreviewView -> ShowPreview();
	}

//	m_vIsDirty = false;

	return true;
}

bool UtilityView::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
	case EM_DRAW:
//		if(m_opView -> LockWindow())
		{
			if(static_cast<EMView*>(p_opSender) == m_opView)
			{
				Draw(m_opView -> GetClippingRect());
			}
			else
				DrawBorder(m_opVBorder -> GetClippingRect());
//			m_opView -> UnlockWindow();
		}
		return true;
	case EM_VIEW_BOUNDS_CHANGED:
//		if(m_opView -> LockWindow())
		{
			LayoutViews();
//eo << "UtilityView::MessageReceived: EM_VIEW_BOUNDS_CHANGED" << ef;
			if(!(m_opView -> IsHidden()))
			{
/*
				m_vHeight = (m_opView -> Bounds()).GetHeight();
				if((m_opWindow -> GetWindow() -> Bounds()).GetHeight() - m_vHeight < 200)
				{
					m_vHeight = (m_opWindow -> GetWindow() -> Bounds()).GetHeight() - 200;
					m_opWindow -> LayoutViews(true);
				}
*/
				if(m_oFileButtonL.GetButton() -> GetOn())
					ExplorerView::Instance() -> GetView() -> SetFrame(EMRect(0, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).GetWidth() / 2 - 4 - UTILITY_BUTTON_SIZE, (m_opView -> Bounds()).m_vBottom));
				else if(m_oFileButtonR.GetButton() -> GetOn())
					ExplorerView::Instance() -> GetView() -> SetFrame(EMRect((m_opView -> Bounds()).GetWidth() / 2 + 2 + UTILITY_BUTTON_SIZE, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).m_vRight, (m_opView -> Bounds()).m_vBottom));

				if(m_oVideoPreviewButtonL.GetButton() -> GetOn())
					m_opVideoPreviewView -> GetView() -> SetFrame(EMRect(0, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).GetWidth() / 2 - 4 - UTILITY_BUTTON_SIZE, (m_opView -> Bounds()).m_vBottom));
				else if(m_oVideoPreviewButtonR.GetButton() -> GetOn())
					m_opVideoPreviewView -> GetView() -> SetFrame(EMRect((m_opView -> Bounds()).GetWidth() / 2 + 2 + UTILITY_BUTTON_SIZE, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).m_vRight, (m_opView -> Bounds()).m_vBottom));

				if(m_oAudioInputButtonL.GetButton() -> GetOn())
					m_opAudioInputSignalView -> GetView() -> SetFrame(EMRect(0, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).GetWidth() / 2 - 4 - UTILITY_BUTTON_SIZE, (m_opView -> Bounds()).m_vBottom));
				else if(m_oAudioInputButtonR.GetButton() -> GetOn())
					m_opAudioInputSignalView -> GetView() -> SetFrame(EMRect((m_opView -> Bounds()).GetWidth() / 2 + 2 + UTILITY_BUTTON_SIZE, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).m_vRight, (m_opView -> Bounds()).m_vBottom));

				if(m_oMediaPoolButtonL.GetButton() -> GetOn())
					MediaPoolView::Instance() -> GetView() -> SetFrame(EMRect(0, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).GetWidth() / 2 - 4 - UTILITY_BUTTON_SIZE, (m_opView -> Bounds()).m_vBottom));
				else if(m_oMediaPoolButtonR.GetButton() -> GetOn())
					MediaPoolView::Instance() -> GetView() -> SetFrame(EMRect((m_opView -> Bounds()).GetWidth() / 2 + 2 + UTILITY_BUTTON_SIZE, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).m_vRight, (m_opView -> Bounds()).m_vBottom));

				if(m_oInternetButtonL.GetButton() -> GetOn())
					ColaborationView::Instance() -> GetView() -> SetFrame(EMRect(0, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).GetWidth() / 2 - 4 - UTILITY_BUTTON_SIZE, (m_opView -> Bounds()).m_vBottom));
				else if(m_oInternetButtonR.GetButton() -> GetOn())
					ColaborationView::Instance() -> GetView() -> SetFrame(EMRect((m_opView -> Bounds()).GetWidth() / 2 + 2 + UTILITY_BUTTON_SIZE, H_RESIZE_BAR_SIZE, (m_opView -> Bounds()).m_vRight, (m_opView -> Bounds()).m_vBottom));
			}
//			m_opView -> UnlockWindow();
		}
		return true;
	case EM_MOUSE_DOWN:
		{
			int32 vX;
			int32 vY;
			bool vButtonOne;
			bool vButtonTwo;
			bool vButtonThree;
			m_opView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
			if(vY < H_RESIZE_BAR_SIZE) // If mouse is on the horizontal border
			{
				m_vMouseIsDown = true;
				m_vMouseDownY = vY;
//				m_vOriginalHeight = (m_opView -> Bounds()).GetHeight();
			}
			return true;
		}
	case EM_MOUSE_UP:
		m_vMouseIsDown = false;
		return true;
	case EM_MOUSE_MOVED:
//		if(m_opView -> LockWindow())
		{
			int32 vX;
			int32 vY;
			bool vButtonOne;
			bool vButtonTwo;
			bool vButtonThree;
			m_opView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);

			if(vY < H_RESIZE_BAR_SIZE || m_vMouseIsDown)
				m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_RESIZE_H_BAR);

			if(m_vMouseIsDown)
			{
				int32 vHeight = (m_opView -> Bounds()).GetHeight();
				m_vHeight = vHeight - vY;
				if(m_vHeight < UTILITY_BUTTON_SIZE * 5 + H_RESIZE_BAR_SIZE - 1)
					m_vHeight = UTILITY_BUTTON_SIZE * 5 + H_RESIZE_BAR_SIZE - 1;
				else if((m_opWindow -> GetWindow() -> Bounds()).GetHeight() - m_vHeight < 200)
					m_vHeight = (m_opWindow -> GetWindow() -> Bounds()).GetHeight() - 200;
//;//cout_commented_out_4_release << "m_vHeight: " << m_vHeight << ", vY: " << vY << endl;
				m_opWindow -> LayoutViews(true);
			}
//			m_opView -> UnlockWindow();
		}
		return true;
	}
	return false;
}

void UtilityView::ReadyForUse()
{
	if(m_opView -> LockWindow())
	{
		m_oAudioInputButtonL.GetView() -> Invalidate(m_oAudioInputButtonL.GetView() -> Bounds());
		m_oAudioInputButtonR.GetView() -> Invalidate(m_oAudioInputButtonR.GetView() -> Bounds());
		m_oFileButtonL.GetView() -> Invalidate(m_oFileButtonL.GetView() -> Bounds());
		m_oFileButtonR.GetView() -> Invalidate(m_oFileButtonR.GetView() -> Bounds());
		m_oInternetButtonL.GetView() -> Invalidate(m_oInternetButtonL.GetView() -> Bounds());
		m_oInternetButtonR.GetView() -> Invalidate(m_oInternetButtonR.GetView() -> Bounds());
		m_oMediaPoolButtonL.GetView() -> Invalidate(m_oMediaPoolButtonL.GetView() -> Bounds());
		m_oMediaPoolButtonR.GetView() -> Invalidate(m_oMediaPoolButtonR.GetView() -> Bounds());
		m_oVideoPreviewButtonL.GetView() -> Invalidate(m_oVideoPreviewButtonL.GetView() -> Bounds());
		m_oVideoPreviewButtonR.GetView() -> Invalidate(m_oVideoPreviewButtonR.GetView() -> Bounds());
		//m_opVideoPreviewView -> DrawBitmap(NULL); // Clears the bitmap
		m_opView -> UnlockWindow();
	}

	m_vIsDirty = false;
}

bool UtilityView::SaveData(void* p_upDataSaver)
{
	EMProjectDataSaver* opSaver = static_cast<EMProjectDataSaver*>(p_upDataSaver);

	opSaver -> SaveBool(m_oAudioInputButtonL.GetButton() -> GetOn());
	opSaver -> SaveBool(m_oAudioInputButtonR.GetButton() -> GetOn());
	opSaver -> SaveBool(m_oFileButtonL.GetButton() -> GetOn());
	opSaver -> SaveBool(m_oFileButtonR.GetButton() -> GetOn());
	opSaver -> SaveBool(m_oInternetButtonL.GetButton() -> GetOn());
	opSaver -> SaveBool(m_oInternetButtonR.GetButton() -> GetOn());
	opSaver -> SaveBool(m_oMediaPoolButtonL.GetButton() -> GetOn());
	opSaver -> SaveBool(m_oMediaPoolButtonR.GetButton() -> GetOn());
	opSaver -> SaveBool(m_oVideoPreviewButtonL.GetButton() -> GetOn());
	opSaver -> SaveBool(m_oVideoPreviewButtonR.GetButton() -> GetOn());

//	m_vIsDirty = false;
	
	return true;
}

void UtilityView::SetHeight(int32 p_vHeight)
{
	m_vHeight = p_vHeight;
}

void UtilityView::SetWindow(TrackWindow* p_opWindow)
{
	m_opWindow = p_opWindow;
	ExplorerView::Instance() -> SetMainWindow(m_opWindow -> GetWindow());
}
