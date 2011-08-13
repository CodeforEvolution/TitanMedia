#include "TrackWindowStatusBar.h"

#include "EMFactory.h"
#include "EMGUIGlobals.h"
#include "EMMessages.h"
#include "EMResourceRepository.h"
#include "EMView.h"
#include "ResourceIDs.h"

TrackWindowStatusBar* TrackWindowStatusBar::m_opInstance = NULL;

TrackWindowStatusBar::TrackWindowStatusBar() :
m_opView(EMFactory::Instance() -> CreateView(EMRect(), /*EM_FOLLOW_LEFT_RIGHT |*/ EM_FOLLOW_BOTTOM, false))
{
	m_opView -> AddListener(this);
}

TrackWindowStatusBar::~TrackWindowStatusBar()
{
	delete m_opView;
}

void TrackWindowStatusBar::Draw(EMRect p_oClippingRect)
{
	m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_SHADOW);
	m_opView -> StrokeLine(0, m_opView -> Bounds().m_vBottom, m_opView -> Bounds().m_vRight, m_opView -> Bounds().m_vBottom);
	m_opView -> StrokeLine(m_opView -> Bounds().m_vRight, 0, m_opView -> Bounds().m_vRight, m_opView -> Bounds().m_vBottom);
	m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_ULTRA_HIGHLIGHT);
	m_opView -> StrokeLine(0, 0, m_opView -> Bounds().m_vRight, 0);
	m_opView -> StrokeLine(0, 0, 0, m_opView -> Bounds().m_vBottom);
	m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_HIGHLIGHT);
	m_opView -> FillRect(EMRect(1, 1, m_opView -> Bounds().m_vRight - 1, m_opView -> Bounds().m_vBottom - 1)/* & p_oClippingRect*/);
	m_opView -> SetHighColor(EM_COLOR_TEXT);
	m_opView -> SetLowColor(EM_COLOR_VIEW_BACKGROUND_HIGHLIGHT);
	m_opView -> DrawString(5, 10, m_oMessage.c_str()); // FIXME: Replace magic numbers
}

EMView* TrackWindowStatusBar::GetView()
{
	return m_opView;
}

TrackWindowStatusBar* TrackWindowStatusBar::Instance()
{
	if(m_opInstance == NULL)
		m_opInstance = EM_new TrackWindowStatusBar();
	return m_opInstance;
}

bool TrackWindowStatusBar::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
	case EM_DRAW:
		Draw(m_opView -> GetClippingRect());
		return true;
	case EM_VIEW_BOUNDS_CHANGED:
		m_opView -> Invalidate(m_opView -> Bounds());
		return true;
	default:
		return false;
	}
}

void TrackWindowStatusBar::WriteMessage(string p_oMessage)
{
	m_oMessage = p_oMessage.c_str();
	m_opView -> Invalidate(EMRect(1, 1, m_opView -> Bounds().m_vRight - 1, m_opView -> Bounds().m_vBottom - 1));
}