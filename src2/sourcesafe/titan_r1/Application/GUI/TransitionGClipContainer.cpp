#include "TransitionGClipContainer.h"

#include "EMGTrack.h"
#include "EMGUIGlobals.h"
#include "EMSettingsRepository.h"
#include "EMView.h"
#include "GUIGlobals.h"
#include "SettingIDs.h"

const int LOWER_BORDER_SIZE = 3;

TransitionGClipContainer::TransitionGClipContainer(EMRect p_oFrame, EMView* p_opView) :
EMGClipContainer(p_oFrame, p_opView)
{
}

EMGClipContainer* TransitionGClipContainer::CreateInstance()
{
	return new TransitionGClipContainer(EMRect(), m_opView);
}

bool TransitionGClipContainer::DeselectAll(EMGClipContainer* p_opNewSelectionOwner)
{
	return true;
}

void TransitionGClipContainer::DrawAfterClips(EMRect p_oClippingRect, EMView* p_opView, EMEditMode p_eEditMode)
{
} 

void TransitionGClipContainer::DrawSelection(EMRect p_oClippingRect, EMView* p_opView)
{
}

void TransitionGClipContainer::DrawTrack(EMRect p_oClippingRect, EMView* p_opView, EMEditMode p_eEditMode)
{
	if(!(Frame().Intersects(p_oClippingRect)))
		return;

	EMRect oRect = Frame();
	oRect.m_vBottom -= LOWER_BORDER_SIZE;
	oRect = oRect & p_oClippingRect;
	if(p_eEditMode == EM_CLIP_EDIT_MODE)
		p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_ULTRA_HIGHLIGHT);//EMColor(100, 100, 100, 255));
	else
		p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND);
	p_opView -> FillRect(oRect);

	p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_HIGHLIGHT);
	p_opView -> StrokeLine(p_oClippingRect.m_vLeft, Frame().m_vBottom - 2, p_oClippingRect.m_vRight, Frame().m_vBottom - 2);
	p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND);
	p_opView -> StrokeLine(p_oClippingRect.m_vLeft, Frame().m_vBottom - 1, p_oClippingRect.m_vRight, Frame().m_vBottom - 1);
	p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_SHADOW);
	p_opView -> StrokeLine(p_oClippingRect.m_vLeft, Frame().m_vBottom, p_oClippingRect.m_vRight, Frame().m_vBottom);
}

uint8 TransitionGClipContainer::GetLowerBorderSize()
{
	return LOWER_BORDER_SIZE;
}

uint8 TransitionGClipContainer::GetUpperBorderSize()
{
	return 0;
}

void TransitionGClipContainer::MouseDoubleClicked(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMEditMode p_eEditMode, EMGClip* p_opClip)
{
}

void TransitionGClipContainer::MouseDown(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMEditMode p_eEditMode)
{
}

void TransitionGClipContainer::MouseMoved(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMEditMode p_eEditMode)
{
	if(p_eEditMode != EM_TRACK_EDIT_MODE && Frame().Contains(p_vX, p_vY))
	{
		ToolboxMode eToolboxMode = static_cast<ToolboxMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TOOLBOX_MODE))));

		switch(eToolboxMode)
		{
			case TOOLBOX_CUT_PASTE:
				m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_CUT_PASTE);
				break;
			case TOOLBOX_RENAME:
				m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_RENAME);
				break;
			case TOOLBOX_RESIZE:
				m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_RESIZE_CLIP);
				break;
			case TOOLBOX_ERASE:
				m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_DELETE);
				break;
			case TOOLBOX_DRAW:
				m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_DRAW);
				break;
			default:
				break;
		}
	}
}

void TransitionGClipContainer::MouseUp(EMView* p_opView, int32 p_vX, int32 p_vY, EMEditMode p_eEditMode)
{
}

