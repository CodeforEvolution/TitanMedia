#include "VideoPreviewView.h"

#include "CommandIDs.h"
#include "EMBitmap.h"
#include "EMCommandRepository.h"
#include "EMFactory.h"
#include "EMGUIGlobals.h"
#include "EMListenerRepository.h"
#include "EMMessages.h"
#include "EMRect.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMThreadRepository.h"
#include "EMView.h"
#include "EMWindow.h"

#include "EMResourceRepository.h" // Debug
#include "ResourceIDs.h" // Debug

#define COLOR_PREVIEW_WINDOW_BACKGROUND EMColor(0, 0, 0, 255)

VideoPreviewView::VideoPreviewView() :
m_opBitmap(NULL),
m_opView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 1000, 1000), EM_FOLLOW_LEFT | EM_FOLLOW_TOP)),
m_vFramerate(0)
{
}

VideoPreviewView::~VideoPreviewView()
{
	delete m_opView;
}

EMRect VideoPreviewView::Bounds()
{
	return m_oBitmapBounds;
}

void VideoPreviewView::CalculateBitmapBounds()
{
	EMRect oViewBounds(m_opView -> Bounds());
	float vHorizontalRatio = static_cast<float>(m_vOriginalWidth) / oViewBounds.GetWidth();
	float vVerticalRatio = static_cast<float>(m_vOriginalHeight) / oViewBounds.GetHeight();
//	if(vHorizontalRatio > 1 || vVerticalRatio > 1)
	{
		if(vHorizontalRatio > vVerticalRatio)
		{
			m_vWidth = static_cast<int>(/*m_vWidth*/m_vOriginalWidth / vHorizontalRatio);
			m_vHeight = static_cast<int>(m_vOriginalHeight / vHorizontalRatio);
		}
		else
		{
			m_vWidth = static_cast<int>(m_vOriginalWidth / vVerticalRatio);
			m_vHeight = static_cast<int>(m_vOriginalHeight / vVerticalRatio);
		}
	}
	m_oBitmapBounds = EMRect(oViewBounds.GetWidth() / 2 - m_vWidth / 2, 
								oViewBounds.GetHeight() / 2 - m_vHeight / 2, 
								oViewBounds.GetWidth() / 2 + m_vWidth / 2, 
								oViewBounds.GetHeight() / 2 + m_vHeight / 2);
//eo << "VideoPreviewView::CalculateBitmapBounds: vHorizontalRatio: " << vHorizontalRatio << ", vVerticalRatio: " << vVerticalRatio << ", m_vOriginalWidth: " << m_vOriginalWidth << ", m_vOriginalHeight: " << m_vOriginalHeight << ", m_vWidth: " << m_vWidth << ", m_vHeight: " << m_vHeight << ef;
//m_oBitmapBounds.Print();
//oViewBounds.Print();
}

// Used only for debug purposes
void VideoPreviewView::DrawBitmap(EMBitmap* p_opBitmap)
{
	m_opBitmap = p_opBitmap;
	if(m_opBitmap != NULL)
	{
		m_opView -> BeginPaint();
		m_opView -> DrawBitmap(m_opBitmap, m_opBitmap -> Bounds(), m_oBitmapBounds);
		m_opView -> EndPaint();
	}
	else
	{
		m_opView -> BeginPaint();
		m_opView -> SetHighColor(EMColor(0, 0, 0, 255));
		m_opView -> FillRect(m_opView -> Bounds());
		m_opView -> EndPaint();
	}
}

EMView* VideoPreviewView::GetView()
{
	return m_opView;
}

void VideoPreviewView::HidePreview()
{
	m_opView -> Hide();
	if(EMThreadRepository::Instance() -> GetThread(GUI_SERVICE_THREAD_NAME) != NULL)
		EMThreadRepository::Instance() -> GetThread(GUI_SERVICE_THREAD_NAME) -> RemoveListener(this);
}

void VideoPreviewView::Init()
{
	m_opView -> AddListener(this);
	EMSettingsRepository::Instance() -> AddListener(this);
	m_vOriginalWidth = *(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_FORMAT_WIDTH)));
	m_vOriginalHeight = *(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_FORMAT_HEIGHT)));
}

bool VideoPreviewView::Lock()
{
	return m_opView -> LockWindow();
}
/*
void VideoPreviewView::LockIfNeededAndDrawBitmap(EMBitmap* p_opBitmap)
{
//if(p_opBitmap == NULL)
//{
//eo << "VideoPreviewView::LockIfNeededAndDrawBitmap: bitmap is NULL" << ef;
//return;
//}
	m_opBitmap = p_opBitmap;
//eo << "VideoPreviewView::LockIfNeededAndDrawBitmap:";
//m_oBitmapBounds.Print();
//(m_opBitmap -> Bounds()).Print();

	if(m_opView -> WindowIsLocked())
	{
//		return;
eo << "VideoPreviewView: Drawing bitmap async (Invalidate)" << ef;
		m_opView -> Invalidate(m_oBitmapBounds);
	}

	else if(m_opView -> LockWindow())
	{
		if(m_opBitmap != NULL)
		{
			m_opView -> BeginPaint();
			m_opView -> DrawBitmap(m_opBitmap, m_opBitmap -> Bounds(), m_oBitmapBounds);
//			m_opView -> Sync();
			m_opView -> EndPaint();
		}
		else
		{
			m_opView -> BeginPaint();
			m_opView -> SetHighColor(EMColor(0, 0, 0, 255));
			m_opView -> FillRect(m_opView -> Bounds());
			m_opView -> EndPaint();
		}

		m_opView -> UnlockWindow();
	}
}
*/
bool VideoPreviewView::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
	case EM_DRAW:
		//if(m_opBitmap == NULL)
		{
			if(m_opView -> GetClippingRect() == m_oBitmapBounds && m_opBitmap != NULL)
				m_opView -> DrawBitmap(m_opBitmap, m_opBitmap -> Bounds(), m_oBitmapBounds);
			else
			{
				m_opView -> SetHighColor(COLOR_PREVIEW_WINDOW_BACKGROUND);
				if(m_opBitmap == NULL)
					m_opView -> FillRect(m_opView -> Bounds());
				else
				{
					// Only fill with color where there's no bitmap (to reduce flicker), a slight overlap to avoid m_oBitmapBounds flaws
					EMRect oViewBounds(m_opView -> Bounds());
					if(m_oBitmapBounds.m_vTop > 0)
						m_opView -> FillRect(EMRect(0, 0, oViewBounds.m_vRight, m_oBitmapBounds.m_vTop));
					if(m_oBitmapBounds.m_vBottom < oViewBounds.m_vBottom)
						m_opView -> FillRect(EMRect(0, m_oBitmapBounds.m_vBottom, oViewBounds.m_vRight, oViewBounds.m_vBottom));
					if(m_oBitmapBounds.m_vLeft > 0)
						m_opView -> FillRect(EMRect(0, m_oBitmapBounds.m_vTop, m_oBitmapBounds.m_vLeft, m_oBitmapBounds.m_vBottom));
					if(m_oBitmapBounds.m_vRight < oViewBounds.m_vRight)
						m_opView -> FillRect(EMRect(m_oBitmapBounds.m_vRight, m_oBitmapBounds.m_vTop, oViewBounds.m_vRight, m_oBitmapBounds.m_vBottom));

					m_opView -> DrawBitmap(m_opBitmap, m_opBitmap -> Bounds(), m_oBitmapBounds);
				}
			}
		}
		/*
		else
		{
			m_opView -> SetHighColor(COLOR_PREVIEW_WINDOW_BACKGROUND);
			m_opView -> FillRect(m_opView -> Bounds());
			m_opView -> DrawBitmap(m_opBitmap, m_opBitmap -> Bounds(), m_oBitmapBounds);
		}
		*/
		return true;
	case EM_SETTING_UPDATED:
		{
eo << "VideoPreviewView: EM_SETTING_UPDATED" << ef;
			int vWidth = *(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_FORMAT_WIDTH)));
			int vHeight = *(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_FORMAT_HEIGHT)));
			if(vWidth != m_vOriginalWidth || vHeight != m_vOriginalHeight)
			{
				if(m_opView -> LockWindow())
				{
					m_vOriginalWidth = vWidth;
					m_vOriginalHeight = vHeight;
					CalculateBitmapBounds();

					m_opView -> BeginPaint();
					m_opView -> SetHighColor(EMColor(0, 0, 0, 255));
					m_opView -> FillRect(m_opView -> Bounds());
					m_opView -> EndPaint();

					m_opView -> UnlockWindow();
				}
			}
			float vFramerate = *(static_cast<float*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_FRAMERATE)));
			if(vFramerate != m_vFramerate)
			{
				if(m_opView -> LockWindow())
				{
					m_vFramerate = vFramerate;
					m_opView -> Invalidate(m_opView -> Bounds());
					m_opView -> UnlockWindow();
				}
			}
		}
		return true;
	case EM_VIEW_BOUNDS_CHANGED: // Most often due to the user resizing the window
//		if(m_opView -> LockWindow())
		{
			CalculateBitmapBounds();
			m_opView -> Invalidate(m_opView -> Bounds());
//			m_opView -> UnlockWindow();
			Notify(EM_VIEW_BOUNDS_CHANGED);
		}
		return true;
	}
	return false;
}

void VideoPreviewView::ShowPreview()
{
	m_opView -> Show();
	EMThreadRepository::Instance() -> GetThread(GUI_SERVICE_THREAD_NAME) -> AddListener(this);
}

void VideoPreviewView::ThreadRun(EMThread* p_opThread)
{
	bool vNewBitmap;
	EMBitmap* opBitmap = static_cast<EMBitmap*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CURRENT_BITMAP, &vNewBitmap));
	if(vNewBitmap)
		if(m_opView -> LockWindow())
		{
			m_opBitmap = opBitmap;
			m_opView -> BeginPaint();
			m_opView -> DrawBitmap(m_opBitmap, m_opBitmap -> Bounds(), m_oBitmapBounds);
			m_opView -> EndPaint();

			m_opView -> UnlockWindow();
		}
}

void VideoPreviewView::Unlock()
{
	m_opView -> UnlockWindow();
}

