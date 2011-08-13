#include "VideoPreviewWindow.h"

#include "EMApplication.h"
#include "EMBitmap.h"
#include "EMFactory.h"
#include "EMGUIGlobals.h"
#include "EMListenerRepository.h"
#include "EMMessages.h"
#include "EMRect.h"
#include "EMView.h"
#include "EMWindow.h"

#include "EMResourceRepository.h" // Debug
#include "ResourceIDs.h" // Debug

#define COLOR_PREVIEW_WINDOW_BACKGROUND EMColor(0, 0, 0, 255)

VideoPreviewWindow::VideoPreviewWindow() :
m_opBitmap(NULL),
m_opWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(50, 50, 770, 626), "Movie Window", EM_WINDOW_NORMAL)),
m_opView(EMFactory::Instance() -> CreateView(m_opWindow -> Bounds(), EM_FOLLOW_LEFT_RIGHT | EM_FOLLOW_TOP_BOTTOM))
{
//	if(m_opView -> LockWindow())
//	{
		m_opWindow -> AddChild(m_opView);
//m_opWindow -> Show();
//m_opWindow -> Hide();
//		m_opView -> UnlockWindow();
//	}

	m_opWindow -> AddListener(this);
	m_opView -> AddListener(this);
}

VideoPreviewWindow::~VideoPreviewWindow()
{
	delete m_opView;
	delete m_opWindow;
}

// Ownership of the bitmap is transferred to this object
void VideoPreviewWindow::DrawBitmap(EMBitmap* p_opBitmap)
{
	m_opBitmap = p_opBitmap;
//	if(m_opView -> LockWindow())
//	{
		m_opView -> DrawBitmap(m_opBitmap, m_opBitmap -> Bounds(), m_opView -> Bounds());
//		m_opView -> DrawBitmapOverlay(m_opBitmap, m_opBitmap -> Bounds(), m_opView -> Bounds());
		m_opView -> Sync();
//		m_opView -> Invalidate(m_opView -> Bounds());
//		m_opView -> UnlockWindow();
//	}
}

void VideoPreviewWindow::HidePreview()
{
	if(m_opView -> LockWindow())
	{
		m_opWindow -> Hide();
		m_opView -> UnlockWindow();
	}
}

bool VideoPreviewWindow::Lock()
{
	return m_opView -> LockWindow();
}

bool VideoPreviewWindow::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	if(p_vMessage == EM_QUIT_REQUESTED)
	{
		EMApplication::QuitApplication();
		return true;
	}
	else if(p_vMessage == EM_DRAW)
	{
//		if(m_opView -> LockWindow())
//		{	
			if(m_opBitmap == NULL)
			{
				m_opView -> SetHighColor(COLOR_PREVIEW_WINDOW_BACKGROUND);
				m_opView -> FillRect(m_opView -> Bounds());
			}
			else
				m_opView -> DrawBitmap(m_opBitmap, m_opBitmap -> Bounds(), m_opView -> Bounds());
//			m_opView -> UnlockWindow();
//		}
		return true;
	}
	return false;
}

void VideoPreviewWindow::ShowPreview()
{
;//cout_commented_out_4_release << "VideoPreviewWindow::ShowPreview: Before locking window" << endl;
	if(m_opView -> LockWindow())
	{
;//cout_commented_out_4_release << "VideoPreviewWindow::ShowPreview: After locking window" << endl;
		m_opWindow -> Show();
		m_opView -> UnlockWindow();
	}
;//cout_commented_out_4_release << "VideoPreviewWindow::ShowPreview: After unlocking window" << endl;
}

void VideoPreviewWindow::Unlock()
{
	m_opView -> UnlockWindow();
}



