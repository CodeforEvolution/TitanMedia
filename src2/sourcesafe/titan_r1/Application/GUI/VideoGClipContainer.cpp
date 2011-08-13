#include "VideoGClipContainer.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGTrack.h"
#include "EMGUIGlobals.h"
#include "EMSettingsRepository.h"
#include "EMSlimValueBox.h"
#include "EMView.h"
#include "GUIGlobals.h"
#include "SettingIDs.h"
#include "VideoGClip.h"

const int LOWER_BORDER_SIZE = 3;
const uint32 MESSAGE_FX_CHAIN_CHANGED = 1;

VideoGClipContainer::VideoGClipContainer(EMRect p_oFrame, EMView* p_opView) :
EMGClipContainer(p_oFrame, p_opView),
m_opCurrentFXChain(&m_oTrackFXNodes),
m_opFXValueBox(NULL),
//m_vHasBeenDoubleClicked(false),
m_vMessageFXChainChanged(MESSAGE_FX_CHAIN_CHANGED),
m_vMouseIsDown(false)
{
}

VideoGClipContainer::~VideoGClipContainer()
{
	delete m_opFXValueBox;
}

void VideoGClipContainer::AddClip(EMGClip* p_opClip)
{
	EMGClipContainer::AddClip(p_opClip);
	UpdateValueBox();
}

// This method is called when a project has loaded and the effect chain has to be built
void VideoGClipContainer::BuildFXNodes(list<int32>* p_opMediaEffectIDs)
{
	list<int32>::iterator oMediaEffectIDIterator = p_opMediaEffectIDs -> begin();
	int vNodeIndex(0);
	int32 vMediaTrackID = GetTrack() -> GetTrackInfo() -> GetMediaTrackID();
	int32 vMediaEffectTrackID = *(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_EFFECT_TRACK_ID, &vMediaTrackID)));

	while(oMediaEffectIDIterator != p_opMediaEffectIDs -> end())
	{
		int32 vEffectID = *oMediaEffectIDIterator;
		EMMediaEffectInfo* opEffectInfo = static_cast<EMMediaEffectInfo*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_EFFECT_INFO, &vEffectID));
		// Insert an empty effect node between active effect nodes
		m_oTrackFXNodes.push_back(FXNode(EM_TYPE_ANY_VIDEO, vNodeIndex, Frame(), m_opView, this, vMediaEffectTrackID));
		vNodeIndex++;
		m_oTrackFXNodes.push_back(FXNode(EM_TYPE_ANY_VIDEO, vNodeIndex, Frame(), m_opView, this, vMediaEffectTrackID, opEffectInfo -> m_vEntryID, vEffectID, (opEffectInfo -> m_oPluginName).c_str()));
		vNodeIndex++;
		oMediaEffectIDIterator++;
	}

	// Insert an empty node at the end of the chain
	m_oTrackFXNodes.push_back(FXNode(EM_TYPE_ANY_VIDEO, vNodeIndex, Frame(), m_opView, this, vMediaEffectTrackID));

	if(m_opFXValueBox == NULL)
		InitValueBox();

	SetFrame(Frame());
}

void VideoGClipContainer::ClearClips()
{
	EMGClipContainer::ClearClips();
	UpdateValueBox();
}

EMGClipContainer* VideoGClipContainer::CreateInstance()
{
	return EM_new VideoGClipContainer(EMRect(), m_opView);
}

bool VideoGClipContainer::DeselectAll(EMGClipContainer* p_opNewSelectionOwner)
{
	return true;
}

void VideoGClipContainer::DrawAfterClips(EMRect p_oClippingRect, EMView* p_opView, EMEditMode p_eEditMode)
{
}

void VideoGClipContainer::DrawSelection(EMRect p_oClippingRect, EMView* p_opView)
{
}

void VideoGClipContainer::DrawTrack(EMRect p_oClippingRect, EMView* p_opView, EMEditMode p_eEditMode)
{
	if(!(Frame().Intersects(p_oClippingRect)))
		return;

//	if(m_opView == NULL)
//		m_opView = p_opView; // FIXME: A bit hackisch to assign this value in this late stage

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

	if(p_eEditMode == EM_FX_EDIT_MODE)
	{
		int32 vTrackMiddleY = Frame().m_vTop + ((Frame().GetHeight() - LOWER_BORDER_SIZE) / 2);
		if(m_opFXValueBox == NULL) // Lazy initialization
			InitValueBox();
		m_opFXValueBox -> Draw(p_oClippingRect);
		if(p_oClippingRect.m_vRight >= FX_VALUE_BOX_X_SPACING * 2 + FX_VALUE_BOX_WIDTH)
		{
			p_opView -> SetHighColor(EM_COLOR_VIDEO_VIEW_BACKGROUND);
			p_opView -> StrokeLine(FX_VALUE_BOX_X_SPACING * 2 + FX_VALUE_BOX_WIDTH, vTrackMiddleY, p_oClippingRect.m_vRight, vTrackMiddleY);
		}
		list<FXNode>::iterator oIterator = m_opCurrentFXChain -> begin();
		while(oIterator != m_opCurrentFXChain -> end())
		{
			(*oIterator).Draw(p_oClippingRect, p_opView);
			oIterator++;
		}
	}
}

uint8 VideoGClipContainer::GetLowerBorderSize()
{
	return LOWER_BORDER_SIZE;
}

uint8 VideoGClipContainer::GetUpperBorderSize()
{
	return 0;
}

void VideoGClipContainer::InitValueBox()
{
	int32 vTrackMiddleY = Frame().m_vTop + ((Frame().GetHeight() - LOWER_BORDER_SIZE) / 2);
	m_opFXValueBox = EM_new EMSlimValueBox(m_opView, EMRect(FX_VALUE_BOX_X_SPACING, vTrackMiddleY - (FX_VALUE_BOX_HEIGHT / 2), FX_NODE_X_SPACING + FX_VALUE_BOX_WIDTH, 
								vTrackMiddleY + (FX_VALUE_BOX_HEIGHT / 2)), /*vector<string>(), */0, NULL, NULL, NULL, true, true, true, true, 0, false);
	m_opFXValueBox -> SetCommand(COMMAND_MESSAGE_SENDER, &m_vMessageFXChainChanged, dynamic_cast<EMListener*>(this), NULL);
	UpdateValueBox();

	// If no FX nodes exist (they only exist here if a project has been loaded), then create a default empty node
	if(m_oTrackFXNodes.size() == 0)
	{
		int32 vMediaTrackID = GetTrack() -> GetTrackInfo() -> GetMediaTrackID();
		int32 vMediaEffectTrackID = *(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_EFFECT_TRACK_ID, &vMediaTrackID)));
		m_oTrackFXNodes.push_back(FXNode(EM_TYPE_ANY_VIDEO, 0, Frame(), m_opView, this, vMediaEffectTrackID));
	}
}

bool VideoGClipContainer::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	SetCurrentFXChain(NULL);
	return true;
}

void VideoGClipContainer::MouseDoubleClicked(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMEditMode p_eEditMode, EMGClip* p_opClip)
{
	if(p_eEditMode == EM_CLIP_EDIT_MODE)
	{
	}
	else if(p_eEditMode == EM_FX_EDIT_MODE)
	{
		list<FXNode>::iterator oIterator = m_opCurrentFXChain -> begin();
		while(oIterator != m_opCurrentFXChain -> end())
		{
			if((*oIterator).MouseDoubleClicked(m_opView, p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
				break;
			oIterator++;
		}
	}
}

void VideoGClipContainer::MouseDown(EMView* m_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMEditMode p_eEditMode)
{
	m_vMouseIsDown = true;

	if(p_eEditMode == EM_FX_EDIT_MODE)
	{
		if(m_opFXValueBox -> MouseDown(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
			m_opView -> Invalidate(m_opFXValueBox -> Frame());			

		list<FXNode>::iterator oIterator = m_opCurrentFXChain -> begin();
		while(oIterator != m_opCurrentFXChain -> end())
		{
			if((*oIterator).MouseDown(m_opView, p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
			{
				break;
			}
			oIterator++;
		}
	}
}

void VideoGClipContainer::MouseMoved(EMView* m_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMEditMode p_eEditMode)
{
//eo << "VideoGClipContainer::MouseMoved" << ef;
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

	if(!m_vMouseIsDown)
		return;

	if(p_eEditMode == EM_FX_EDIT_MODE)
	{
		if(m_opFXValueBox -> MouseMoved(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
			m_opView -> Invalidate(m_opFXValueBox -> Frame());

		list<FXNode>::iterator oIterator = m_opCurrentFXChain -> begin();
		while(oIterator != m_opCurrentFXChain -> end())
		{
			if((*oIterator).MouseMoved(m_opView, p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
				break;
			oIterator++;
		}
	}
	else
	{
	}
}

void VideoGClipContainer::MouseUp(EMView* m_opView, int32 p_vX, int32 p_vY, EMEditMode p_eEditMode)
{
	if(!m_vMouseIsDown)
		return;

	m_vMouseIsDown = false;

	if(p_eEditMode == EM_FX_EDIT_MODE)
	{
		if(m_opFXValueBox -> MouseUp(p_vX, p_vY))
			m_opView -> Invalidate(m_opFXValueBox -> Frame());

		list<FXNode>::iterator oIterator = m_opCurrentFXChain -> begin();
		while(oIterator != m_opCurrentFXChain -> end())
		{
			if((*oIterator).MouseUp(m_opView, p_vX, p_vY))
				break;
			oIterator++;
		}
	}
}

void VideoGClipContainer::RemoveClip(EMGClip* p_opClip)
{
	EMGClipContainer::RemoveClip(p_opClip);
	UpdateValueBox();
	SetCurrentFXChain("Track");
}

void VideoGClipContainer::SetCurrentFXChain(const char* p_opString)
{
	if(m_opFXValueBox == NULL) // Lazy initialization
		InitValueBox();

	if(p_opString != NULL)
		m_opFXValueBox -> SetValue(string(p_opString));

	if(string("Track") == static_cast<string*>(m_opFXValueBox -> GetValue()) -> c_str())
		m_opCurrentFXChain = &m_oTrackFXNodes;
	else
	{
		m_opCurrentFXChain = NULL;
		list<EMGClip*>::iterator oIterator = m_oClipRepository.begin();
		while(oIterator != m_oClipRepository.end())
		{
			if((*oIterator) -> GetLabel() == static_cast<string*>(m_opFXValueBox -> GetValue()) -> c_str())
			{
				(dynamic_cast<VideoGClip*>(*oIterator)) -> EnableClipFX(m_opView, GetTrack() -> GetTrackInfo() -> GetMediaTrackID());
				m_opCurrentFXChain = (dynamic_cast<VideoGClip*>(*oIterator)) -> GetFXNodes();
				break;
			}
			oIterator++;
		}
		if(m_opCurrentFXChain == NULL)
			EMDebugger("Could not find clip for FX chain");
	}

	if(m_opView -> LockWindow())
	{
		m_opView -> Invalidate(Frame() & m_opView -> Bounds());
		m_opView -> UnlockWindow();
	}
}

bool VideoGClipContainer::SetFrame(EMRect p_oFrame)
{
	if(EMGClipContainer::SetFrame(p_oFrame))
	{
		if(m_opFXValueBox != NULL)
		{
			EMRect oRect = p_oFrame;
			oRect.m_vBottom -= LOWER_BORDER_SIZE;
			int32 vTrackMiddleY = Frame().m_vTop + ((Frame().GetHeight() - LOWER_BORDER_SIZE) / 2);
			m_opFXValueBox -> SetFrame(EMRect(FX_VALUE_BOX_X_SPACING, vTrackMiddleY - (FX_VALUE_BOX_HEIGHT / 2), FX_NODE_X_SPACING + FX_VALUE_BOX_WIDTH, 
									vTrackMiddleY + (FX_VALUE_BOX_HEIGHT / 2)));
	
			list<FXNode>::iterator oIterator = m_opCurrentFXChain -> begin();
			while(oIterator != m_opCurrentFXChain -> end())
			{
				(*oIterator).SetParent(this);
				oIterator++;
			}
		}

		if(m_opClipContainerWithSelection == this)
			m_oSelectionRect.m_vBottom = Frame().m_vBottom - GetLowerBorderSize();

		return true;
	}
	return false;
}

// Adjusts the number of necessary nodes etc.
void VideoGClipContainer::UpdateFXNodes()
{
	FXNode* opPreviousNode = NULL;
	list<FXNode>::iterator oIterator = m_opCurrentFXChain -> begin();
	bool vIllegalState(false);
	while(oIterator != m_opCurrentFXChain -> end())
	{
		if(opPreviousNode == NULL)
		{
			if((*oIterator).IsActive())
				vIllegalState = true;
		}
		else if(opPreviousNode -> IsActive() == (*oIterator).IsActive())
			vIllegalState = true;

		if(vIllegalState)
		{
			if((*oIterator).IsActive())
			{
				// Add two empty slots, one at each side of the slot becoming active
				list<FXNode>::iterator oBackupIterator = oIterator;
				//int32 vMediaTrackID = GetTrack() -> GetTrackInfo() -> GetMediaTrackID();
				//int32 vMediaEffectTrackID = *(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_EFFECT_TRACK_ID, &vMediaTrackID)));
				int32 vMediaEffectTrackID = (*oIterator).GetEffectTrackID();
				m_opCurrentFXChain -> insert(oIterator, FXNode(EM_TYPE_ANY_VIDEO, 0, Frame(), m_opView, this, vMediaEffectTrackID));
				oIterator = oBackupIterator;
				oIterator++;
				m_opCurrentFXChain -> insert(oIterator, FXNode(EM_TYPE_ANY_VIDEO, 0, Frame(), m_opView, this, vMediaEffectTrackID));
				break;
			}
			else
			{
				list<FXNode>::iterator oBackupIterator = oIterator;
				oBackupIterator--;
				m_opCurrentFXChain -> erase(oBackupIterator);
				oIterator++;
				m_opCurrentFXChain -> erase(oIterator);
				break;
			}
		}
		opPreviousNode = &(*oIterator);
		oIterator++;
	}

	int vNodeIndex(0);
	oIterator = m_opCurrentFXChain -> begin();
	while(oIterator != m_opCurrentFXChain -> end())
	{
		(*oIterator).SetIndex(vNodeIndex);
		vNodeIndex++;
		oIterator++;
	}

	if(m_opView -> LockWindow())
	{
		m_opView -> Invalidate(Frame() & m_opView -> Bounds());
		m_opView -> UnlockWindow();
	}
}
/*
void VideoGClipContainer::UpdateRepositories()
{
	EMGClipContainer::UpdateRepositories();
	UpdateValueBox();
}
*/
void VideoGClipContainer::UpdateValueBox()
{
	if(m_opFXValueBox == NULL)
		return;

	list<string*> oValueBoxValues;
	oValueBoxValues.push_back(EM_new string("Track"));
	list<EMGClip*>::iterator oIterator = m_oClipRepository.begin();
	while(oIterator != m_oClipRepository.end())
	{
		oValueBoxValues.push_back(EM_new string((*oIterator) -> GetLabel()));
		oIterator++;
	}
	m_opFXValueBox -> SetValues(&oValueBoxValues, true);
/*
	vector<string> oValueBoxValues;
	oValueBoxValues.push_back(string("Track"));
	list<EMGClip*>::iterator oIterator = m_oClipRepository.begin();
	while(oIterator != m_oClipRepository.end())
	{
		oValueBoxValues.push_back(string((*oIterator) -> GetLabel()));
eo << "Pushing back label: " << (char*) ((*oIterator) -> GetLabel()).c_str() << ef;
		oIterator++;
	}
	m_opFXValueBox -> SetValues(oValueBoxValues);
*/
}


