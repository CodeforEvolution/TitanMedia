#include "AudioGClipContainer.h"

#include "AudioGClip.h"
#include "CommandIDs.h"
#include "EMGRubberBand.h"
#include "EMGTrack.h"
#include "EMGUIGlobals.h"
#include "EMGUIUtilities.h"
#include "EMKeyboard.h"
#include "EMKeyboardGlobals.h"
#include "EMMediaGlobals.h"
#include "EMRubberBandNode.h"
#include "EMSettingsRepository.h"
#include "EMSlimValueBox.h"
#include "EMView.h"
#include "GTrackInfo.h"
#include "GUIGlobals.h"
#include "SettingIDs.h"

const int LOWER_BORDER_SIZE = 3;
const uint32 MESSAGE_FX_CHAIN_CHANGED = 1;
 
AudioGClipContainer::AudioGClipContainer(EMRect p_oFrame, EMView* p_opView) :
m_eDisplayMode(CE_DISPLAY_MODE_DEFAULT),
EMGClipContainer(p_oFrame, p_opView),
m_opCurrentFXChain(&m_oTrackFXNodes),
m_opFXValueBox(NULL),
m_opRubberBand(NULL),
m_opSelectionButton(NULL),
m_vHasBeenDoubleClicked(false),
m_vMessageFXChainChanged(MESSAGE_FX_CHAIN_CHANGED),
m_vMouseIsDown(false)
{
}

AudioGClipContainer::~AudioGClipContainer()
{
	delete m_opFXValueBox;
	delete m_opRubberBand;
}

void AudioGClipContainer::AddClip(EMGClip* p_opClip)
{
	EMGClipContainer::AddClip(p_opClip);
	UpdateValueBox();
}

// This method is called when a project has loaded and the effect chain has to be built
void AudioGClipContainer::BuildFXNodes(list<int32>* p_opMediaEffectIDs)
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
		m_oTrackFXNodes.push_back(FXNode(EM_TYPE_ANY_AUDIO, vNodeIndex, Frame(), m_opView, this, vMediaEffectTrackID));
		vNodeIndex++;
		m_oTrackFXNodes.push_back(FXNode(EM_TYPE_ANY_AUDIO, vNodeIndex, Frame(), m_opView, this, vMediaEffectTrackID, opEffectInfo -> m_vEntryID, vEffectID, (opEffectInfo -> m_oPluginName).c_str()));
		vNodeIndex++;
		oMediaEffectIDIterator++;
	}

	// Insert an empty node at the end of the chain
	m_oTrackFXNodes.push_back(FXNode(EM_TYPE_ANY_AUDIO, vNodeIndex, Frame(), m_opView, this, vMediaEffectTrackID));

	if(m_opFXValueBox == NULL)
		InitValueBox();

	SetFrame(Frame());
}

void AudioGClipContainer::ChangeDisplayMode()
{
	if(m_eDisplayMode == CE_DISPLAY_MODE_AUDIO_VOLUME || m_eDisplayMode == CE_DISPLAY_MODE_AUDIO_PAN)
	{
		int32 vParameterType;
		if(m_eDisplayMode == CE_DISPLAY_MODE_AUDIO_VOLUME)
		{
			m_opRubberBand -> SetMinMax(0, 127);
			m_opRubberBand -> SetDefaultValue(127);
			vParameterType = 1; // 1 = volume, 2 = pan
		}
		else if(m_eDisplayMode == CE_DISPLAY_MODE_AUDIO_PAN)
		{
			m_opRubberBand -> SetMinMax(-64, 64);
			m_opRubberBand -> SetDefaultValue(0);
			vParameterType = 2; // 1 = volume, 2 = pan
		}

		m_opRubberBand -> ClearNodes(false);

		int32 vMediaTrackID = GetTrack() -> GetTrackInfo() -> GetMediaTrackID();
		list<EMRubberBandNode*>* opMediaNodes = static_cast<list<EMRubberBandNode*>*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_NODES, &vMediaTrackID, &vParameterType));
		list<EMRubberBandNode*>::iterator oIterator = opMediaNodes -> begin();
		while(oIterator != opMediaNodes -> end())
		{
			m_opRubberBand -> CreateNode((*oIterator) -> GetPosition(), (*oIterator) -> GetValue(), *oIterator, false, false);
			oIterator++;
		}
	}

	m_opView -> Invalidate(Frame() & m_opView -> Bounds());
}

void AudioGClipContainer::ClearClips()
{
	EMGClipContainer::ClearClips();
	UpdateValueBox();
}

EMGClipContainer* AudioGClipContainer::CreateInstance()
{
	return new AudioGClipContainer(EMRect(), m_opView);
}

bool AudioGClipContainer::DeselectAll(EMGClipContainer* p_opNewSelectionOwner)
{
	EMRect oInvalidateRect = m_oSelectionRect & m_opView -> Bounds();
	m_oSelectionRect = EMRect();
	m_opView -> Invalidate(oInvalidateRect);

	m_opRubberBand -> DeselectAllNodes();

	if(p_opNewSelectionOwner != this)
		m_opClipContainerWithSelection = NULL;

	return true;
}

void AudioGClipContainer::DrawAfterClips(EMRect p_oClippingRect, EMView* p_opView, EMEditMode p_eEditMode)
{
	if(p_eEditMode == EM_CLIP_EDIT_MODE && m_eDisplayMode != CE_DISPLAY_MODE_DEFAULT)
		m_opRubberBand -> Draw(p_oClippingRect);
}

void AudioGClipContainer::DrawSelection(EMRect p_oClippingRect, EMView* p_opView)
{
	if(this != m_opClipContainerWithSelection)
		return;

	if(m_eDisplayMode != CE_DISPLAY_MODE_DEFAULT) // Used for the rubberband
	{
		if(p_oClippingRect.IsValid())
		{
			if(p_oClippingRect.Intersects(m_oSelectionRect))
			{
				m_opView -> SetDrawingMode(EM_OP_INVERT);
				m_opView -> StrokeRect(m_oSelectionRect);
				m_opView -> SetDrawingMode(EM_OP_COPY);
			}
		}
		else
		{
			m_opView -> BeginPaint();
			m_opView -> EnableDoubleBuffering(false);
			m_opView -> SetDrawingMode(EM_OP_INVERT);
			if(m_oOldSelectionRect.IsValid())
				m_opView -> StrokeRect(m_oOldSelectionRect);
			if(m_oSelectionRect.IsValid())
				m_opView -> StrokeRect(m_oSelectionRect);
			m_oOldSelectionRect = m_oSelectionRect;
			m_opView -> SetDrawingMode(EM_OP_COPY);
			m_opView -> EnableDoubleBuffering(true);
			m_opView -> EndPaint();
		}
	}

	else // Used for selection parts of an audio clip
	{
		EMRect oDrawRect = m_oSelectionRect & p_oClippingRect;
		p_opView -> SetDrawingMode(EM_OP_INVERT);
		p_opView -> FillRect(oDrawRect);
		p_opView -> SetDrawingMode(EM_OP_COPY);
	}
}

void AudioGClipContainer::DrawTrack(EMRect p_oClippingRect, EMView* p_opView, EMEditMode p_eEditMode)
{
	if(!(Frame().Intersects(p_oClippingRect)))
		return;

	if(m_opRubberBand == NULL)
	{
		// Lazy init
		m_opRubberBand = EM_new EMGRubberBand(m_opView, 127, 0, 64, EM_RUBBERBAND_DRAW_LINES);
		m_opRubberBand -> SetFrame(EMRect(Frame().m_vLeft, Frame().m_vTop, Frame().m_vRight, Frame().m_vBottom - LOWER_BORDER_SIZE));
		m_opRubberBand -> SetTimeZoomScale(m_vTimeZoomScale);
		m_opRubberBand -> SetPermissions(true, true, true);
		m_opRubberBand -> AddListener(this);
	}

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
			p_opView -> SetHighColor(EM_COLOR_AUDIO_VIEW_BACKGROUND);
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

ClipEditDisplayMode AudioGClipContainer::GetDisplayMode()
{
	return m_eDisplayMode;
}

uint8 AudioGClipContainer::GetLowerBorderSize()
{
	return LOWER_BORDER_SIZE;
}

EMGRubberBand* AudioGClipContainer::GetRubberband()
{
	return m_opRubberBand;
}

list<AudioGClip*>* AudioGClipContainer::GetSelectedClips(int64* p_vpSelectionBegin, int64* p_vpSelectionEnd)
{
	if(m_opClipContainerWithSelection != this)
		return NULL;

	*p_vpSelectionBegin = m_vSelectionLeftXPos;
	*p_vpSelectionEnd = m_vSelectionRightXPos;

	m_oAffectedClips.clear();
	list<EMGClip*>::iterator oIterator = m_oClipRepository.begin();
	int64 vClipBeginPos;
	int64 vClipEndPos;
	while(oIterator != m_oClipRepository.end())
	{
		vClipBeginPos = (*oIterator) -> GetPosition();
		vClipEndPos = (*oIterator) -> GetPosition() + (*oIterator) -> GetLength() - 1;
		if(m_vSelectionLeftXPos < vClipEndPos && m_vSelectionRightXPos > vClipBeginPos)
			m_oAffectedClips.push_back(dynamic_cast<AudioGClip*>(*oIterator));

		oIterator++;
	}

	return &m_oAffectedClips;
}

uint8 AudioGClipContainer::GetUpperBorderSize()
{
	return 0;
}

void AudioGClipContainer::InitValueBox()
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
		m_oTrackFXNodes.push_back(FXNode(EM_TYPE_ANY_AUDIO, 0, Frame(), m_opView, this, vMediaEffectTrackID));
	}
}

bool AudioGClipContainer::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
	case SELECTION_BUTTON_DISPLAY_MODE_CHANGED:
		m_eDisplayMode = m_opSelectionButton -> GetDisplayMode();
		ChangeDisplayMode();
		return true;
	case EM_RUBBERBAND_NODES_ADDED:
		{
			list<EMGRubberBandNode*>* opAffectedNodes = m_opRubberBand -> GetAffectedNodes();
			if(opAffectedNodes -> size() == 0)
				return true; // Nothing to do

			list<EMCommand*> oMediaUndoCommands;
			int64 vpPosAndValue[2];
			int32 vMediaTrackID = GetTrack() -> GetTrackInfo() -> GetMediaTrackID();
			int32 vParameterType; // 1 = volume, 2 = pan
			if(m_eDisplayMode == CE_DISPLAY_MODE_AUDIO_VOLUME)
				vParameterType = 1;
			else
				vParameterType = 2;
			EMRubberBandNode* opMediaNode;
			list<EMRubberBandNode*> oMediaNodes;
			list<EMGRubberBandNode*>::iterator oNodeIterator = opAffectedNodes -> begin();
			while(oNodeIterator != opAffectedNodes -> end())
			{
				vpPosAndValue[0] = (*oNodeIterator) -> m_vPosition;
				vpPosAndValue[1] = (*oNodeIterator) -> m_vValue;

				opMediaNode = static_cast<EMRubberBandNode*>(EMCommandRepository::Instance() -> ExecuteCommandNoUndo(MEDIA_COMMAND_ADD_NODE, vpPosAndValue, &vMediaTrackID, &vParameterType));
				oMediaNodes.push_back(opMediaNode);
				oMediaUndoCommands.push_back(EMCommandRepository::Instance() -> GetUndoClone(MEDIA_COMMAND_ADD_NODE));

				(*oNodeIterator) -> m_opDataLink = opMediaNode;

				oNodeIterator++;
			}

			EMCommandRepository::Instance() -> ExecuteCommand(UNDO_COMMAND_ADD_RUBBERBAND_NODES, m_opRubberBand, opAffectedNodes, &oMediaUndoCommands);

			return true;
		}
	case EM_RUBBERBAND_NODES_DELETED:
		{
			list<EMGRubberBandNode*>* opAffectedNodes = m_opRubberBand -> GetAffectedNodes();
			if(opAffectedNodes -> size() == 0)
				return true; // Nothing to do

			int32 vMediaTrackID = GetTrack() -> GetTrackInfo() -> GetMediaTrackID();
			int32 vParameterType; // 1 = volume, 2 = pan
			if(m_eDisplayMode == CE_DISPLAY_MODE_AUDIO_VOLUME)
				vParameterType = 1;
			else
				vParameterType = 2;
			list<EMCommand*> oMediaUndoCommands;
			list<EMGRubberBandNode*>::iterator oNodeIterator = opAffectedNodes -> begin();
			while(oNodeIterator != opAffectedNodes -> end())
			{
				EMCommandRepository::Instance() -> ExecuteCommandNoUndo(MEDIA_COMMAND_DELETE_NODE, (*oNodeIterator) -> m_opDataLink, &vMediaTrackID, &vParameterType);
				oMediaUndoCommands.push_back(EMCommandRepository::Instance() -> GetUndoClone(MEDIA_COMMAND_DELETE_NODE));
				oNodeIterator++;
			}

			EMCommandRepository::Instance() -> ExecuteCommand(UNDO_COMMAND_DELETE_RUBBERBAND_NODES, m_opRubberBand, opAffectedNodes, &oMediaUndoCommands);

			return true;
		}
	case EM_RUBBERBAND_POSITIONS_CHANGED: // Is sent when either values or positions of nodes have changed
		{
			list<EMGRubberBandNode*>* opAffectedNodes = m_opRubberBand -> GetAffectedNodes();
			if(opAffectedNodes -> size() == 0)
				return true; // Nothing to do

			int64 vpPosAndValue[2];
			/*
			int32 vParameterType; // 1 = volume, 2 = pan
			if(m_eDisplayMode == CE_DISPLAY_MODE_AUDIO_VOLUME)
				vParameterType = 1;
			else
				vParameterType = 2;
				*/
			int32 vMediaTrackID = GetTrack() -> GetTrackInfo() -> GetMediaTrackID();
			list<EMCommand*> oMediaUndoCommands;
			list<EMGRubberBandNode*>::iterator oNodeIterator = opAffectedNodes -> begin();
			while(oNodeIterator != opAffectedNodes -> end())
			{
				vpPosAndValue[0] = (*oNodeIterator) -> m_vPosition;
				vpPosAndValue[1] = (*oNodeIterator) -> m_vValue;

				EMCommandRepository::Instance() -> ExecuteCommandNoUndo(MEDIA_COMMAND_MOVE_NODE, (*oNodeIterator) -> m_opDataLink, vpPosAndValue, &vMediaTrackID);
				oMediaUndoCommands.push_back(EMCommandRepository::Instance() -> GetUndoClone(MEDIA_COMMAND_MOVE_NODE));

				oNodeIterator++;
			}

			EMCommandRepository::Instance() -> ExecuteCommand(UNDO_COMMAND_MOVE_RUBBERBAND_NODES, m_opRubberBand, opAffectedNodes, &oMediaUndoCommands);

			return true;
		}
		default:
			// Assume that the remaining message is from the selection button
			SetCurrentFXChain(NULL); 
			return true;
	}
//	return false;
}

void AudioGClipContainer::MouseDoubleClicked(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMEditMode p_eEditMode, EMGClip* p_opClip)
{
	if(p_eEditMode == EM_CLIP_EDIT_MODE)
	{
		if(p_opClip != NULL)
		{
			m_vSelectionLeftXPos = p_opClip -> GetPosition();
			m_vSelectionRightXPos = p_opClip -> GetPosition() + p_opClip -> GetLength() - 1;
			m_oSelectionRect = EMRect(m_vSelectionLeftXPos / m_vTimeZoomScale, Frame().m_vTop, m_vSelectionRightXPos / m_vTimeZoomScale, Frame().m_vBottom - GetLowerBorderSize());
			p_opView -> Invalidate(m_oSelectionRect | m_oOldSelectionRect);
			m_vHasBeenDoubleClicked = true;
		}
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

void AudioGClipContainer::MouseDown(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMEditMode p_eEditMode)
{
	if(p_eEditMode == EM_TRACK_EDIT_MODE)
		return;

	if(!Frame().Contains(p_vX, p_vY))
		return;

	if(m_opClipContainerWithSelection != NULL && m_opClipContainerWithSelection != this) // First, remove any previous track selection if the selection isn't in this track (if possible)
	{
		if(!(m_opClipContainerWithSelection -> DeselectAll(this)))
			return;

		//m_opClipContainerWithSelection = this;//NULL;
	}
	m_opClipContainerWithSelection = this;

	m_vMouseIsDown = true;
	m_vMouseDownXPos = p_vX * m_vTimeZoomScale;
	if(/*p_eEditMode != EM_CLIP_EDIT_MODE || */m_eDisplayMode == CE_DISPLAY_MODE_DEFAULT)
		EMGUIUtilities::Instance() -> GetSnapPosition(&m_vMouseDownXPos, true, false, NULL, true, true);
	m_vMouseDownY = p_vY;

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

		return;
	}

	ToolboxMode eToolboxMode = static_cast<ToolboxMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TOOLBOX_MODE))));

	if(/*p_eEditMode == EM_CLIP_EDIT_MODE && */m_eDisplayMode != CE_DISPLAY_MODE_DEFAULT)
	{
		if(m_opRubberBand -> MouseDown(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree, eToolboxMode))
			return;
	}
/*
	if(m_opClipContainerWithSelection != NULL) // First, remove any previous track selection (if possible)
	{
		if(!(m_opClipContainerWithSelection -> DeselectAll(this)))
			return;
	}
*/
	if(eToolboxMode == TOOLBOX_SELECT)
	{
		//m_opClipContainerWithSelection = this;
	
		if(m_eDisplayMode != CE_DISPLAY_MODE_DEFAULT)
		{
			m_oSelectionRect = EMRect(p_vX, p_vY, p_vX, p_vY);
			m_oOldSelectionRect = EMRect();
			DrawSelection(EMRect(), p_opView);
		}
	}
}

void AudioGClipContainer::MouseMoved(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMEditMode p_eEditMode)
{
//eo << "AudioGClipContainer::MouseMoved" << ef;
	if(p_eEditMode == EM_TRACK_EDIT_MODE)
		return;

	if(m_vHasBeenDoubleClicked)
		return;

	ToolboxMode eToolboxMode = static_cast<ToolboxMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TOOLBOX_MODE))));

	if(Frame().Contains(p_vX, p_vY))
	{
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

		return;
	}

	if(eToolboxMode == TOOLBOX_SELECT)
	{
		if(p_vX < 0)
			p_vX = 0;
		if(p_vY > Frame().m_vBottom - GetLowerBorderSize())
			p_vY = Frame().m_vBottom - GetLowerBorderSize();
		else if(p_vY < Frame().m_vTop)
			p_vY = Frame().m_vTop;

		int64 vMouseXPos = p_vX * m_vTimeZoomScale;
		if(m_eDisplayMode == CE_DISPLAY_MODE_DEFAULT)
		{
			EMGUIUtilities::Instance() -> GetSnapPosition(&vMouseXPos, true, false, NULL, true, true);
			m_oOldSelectionRect = m_oSelectionRect;
			if(m_vMouseDownXPos < vMouseXPos)
			{
				m_oSelectionRect = EMRect(m_vMouseDownXPos / m_vTimeZoomScale, Frame().m_vTop, vMouseXPos / m_vTimeZoomScale, Frame().m_vBottom - GetLowerBorderSize());
				m_vSelectionLeftXPos = m_vMouseDownXPos;
				m_vSelectionRightXPos = vMouseXPos;
			}
			else
			{
				m_oSelectionRect = EMRect(vMouseXPos / m_vTimeZoomScale, Frame().m_vTop, m_vMouseDownXPos / m_vTimeZoomScale, Frame().m_vBottom - GetLowerBorderSize());
				m_vSelectionLeftXPos = vMouseXPos;
				m_vSelectionRightXPos = m_vMouseDownXPos;
			}
			p_opView -> Invalidate(m_oSelectionRect | m_oOldSelectionRect);
		}

		else
		{
			if(m_oSelectionRect.IsValid())
			{
				if(m_vMouseDownXPos / m_vTimeZoomScale < p_vX)
				{
					if(m_vMouseDownY <= p_vY)
						m_oSelectionRect = EMRect(m_vMouseDownXPos / m_vTimeZoomScale, m_vMouseDownY, p_vX, p_vY);
					else
						m_oSelectionRect = EMRect(m_vMouseDownXPos / m_vTimeZoomScale, p_vY, p_vX, m_vMouseDownY);
					m_vSelectionLeftXPos = m_vMouseDownXPos;
					m_vSelectionRightXPos = vMouseXPos;
				}
				else
				{
					if(m_vMouseDownY <= p_vY)
						m_oSelectionRect = EMRect(p_vX, m_vMouseDownY, m_vMouseDownXPos / m_vTimeZoomScale, p_vY);
					else
						m_oSelectionRect = EMRect(p_vX, p_vY, m_vMouseDownXPos / m_vTimeZoomScale, m_vMouseDownY);
					m_vSelectionLeftXPos = vMouseXPos;
					m_vSelectionRightXPos = m_vMouseDownXPos;
				}

				//m_oSelectionRect = m_oSelectionRect & (m_opView -> Bounds());

				DrawSelection(EMRect(), p_opView);

				return;
			}
			else
				m_opRubberBand -> MouseMoved(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree, eToolboxMode);
		}
	}

	else if(eToolboxMode == TOOLBOX_ERASE)
	{
		if(m_eDisplayMode != CE_DISPLAY_MODE_DEFAULT)
			m_opRubberBand -> MouseMoved(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree, eToolboxMode);
	}
}

void AudioGClipContainer::MouseUp(EMView* p_opView, int32 p_vX, int32 p_vY, EMEditMode p_eEditMode)
{
	if(p_eEditMode == EM_TRACK_EDIT_MODE)
		return;

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

		return;
	}

	if(m_opClipContainerWithSelection != this)
		return;

	ToolboxMode eToolboxMode = static_cast<ToolboxMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TOOLBOX_MODE))));

	uint32 vModifiers = EMKeyboard::Instance() -> GetModifiers();
	
	if(eToolboxMode == TOOLBOX_SELECT)
	{
		int64 vMouseXPos = p_vX * m_vTimeZoomScale;

		if(m_eDisplayMode == CE_DISPLAY_MODE_DEFAULT)
		{
			EMGUIUtilities::Instance() -> GetSnapPosition(&vMouseXPos, true, false, NULL, true, true);
			if(static_cast<int>(vMouseXPos / m_vTimeZoomScale) == static_cast<int>(m_vMouseDownXPos / m_vTimeZoomScale))
			{
				if(!m_vHasBeenDoubleClicked)
				{
					m_oOldSelectionRect = m_oSelectionRect;
					m_opClipContainerWithSelection = NULL;
					p_opView -> Invalidate(m_oSelectionRect | m_oOldSelectionRect);
				}
				//else
				//	m_vHasBeenDoubleClicked = false;
			}
			m_vHasBeenDoubleClicked = false;
		}

		else
		{
			if(m_oSelectionRect.IsValid())
			{
				if((vModifiers & EM_SHIFT_KEY) == EM_SHIFT_KEY)
					m_opRubberBand -> SetSelected(m_opRubberBand -> GetNodes(m_oSelectionRect), false);	
				else
					m_opRubberBand -> SetSelected(m_opRubberBand -> GetNodes(m_oSelectionRect), true);
				m_oSelectionRect = EMRect();
			}
			else
				m_opRubberBand -> MouseUp(p_vX, p_vY, eToolboxMode);

			m_opView -> Invalidate(Frame() & m_opView -> Bounds());
			DrawSelection(EMRect(), p_opView);
		}
	}
}

void AudioGClipContainer::RemoveClip(EMGClip* p_opClip)
{
	EMGClipContainer::RemoveClip(p_opClip);
	UpdateValueBox();
	SetCurrentFXChain("Track");
}

void AudioGClipContainer::SetCurrentFXChain(const char* p_opString)
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
				(dynamic_cast<AudioGClip*>(*oIterator)) -> EnableClipFX(m_opView, GetTrack() -> GetTrackInfo() -> GetMediaTrackID());
				m_opCurrentFXChain = (dynamic_cast<AudioGClip*>(*oIterator)) -> GetFXNodes();
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

bool AudioGClipContainer::SetFrame(EMRect p_oFrame)
{
	if(EMGClipContainer::SetFrame(p_oFrame))
	{
		if(m_opRubberBand != NULL)
			m_opRubberBand -> SetFrame(EMRect(p_oFrame.m_vLeft, p_oFrame.m_vTop, p_oFrame.m_vRight, p_oFrame.m_vBottom - LOWER_BORDER_SIZE));

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
		{
			m_oSelectionRect.m_vTop = Frame().m_vTop;
			m_oSelectionRect.m_vBottom = Frame().m_vBottom - GetLowerBorderSize();
		}

		return true;
	}

	return false;
}

void AudioGClipContainer::SetTrack(EMGTrack* p_opTrack)
{
	EMGClipContainer::SetTrack(p_opTrack);
	m_opSelectionButton = dynamic_cast<GTrackInfo*>(p_opTrack -> GetTrackInfo()) -> GetSelectionButton();
	m_opSelectionButton -> AddListener(this);
}

// Adjusts the number of necessary nodes etc.
void AudioGClipContainer::UpdateFXNodes()
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
				m_opCurrentFXChain -> insert(oIterator, FXNode(EM_TYPE_ANY_AUDIO, 0, Frame(), m_opView, this, vMediaEffectTrackID));
				oIterator = oBackupIterator;
				oIterator++;
				m_opCurrentFXChain -> insert(oIterator, FXNode(EM_TYPE_ANY_AUDIO, 0, Frame(), m_opView, this, vMediaEffectTrackID));
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

void AudioGClipContainer::UpdateValueBox()
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
		oIterator++;
	}
	m_opFXValueBox -> SetValues(oValueBoxValues);
*/
}
/*
void AudioGClipContainer::UpdateRepositories()
{
	EMGClipContainer::UpdateRepositories();
	UpdateValueBox();
}
*/
void AudioGClipContainer::UpdateTimeScale(int64 p_vTimeScale)
{
//	m_oOldSelectionRect = m_oSelectionRect;
/*
	int64 vOldLeftXPos;
	int64 vOldRightXPos;
	if(m_opClipContainerWithSelection == this)
	{
		vOldLeftXPos = m_oSelectionRect.m_vLeft * m_vTimeZoomScale;
		vOldRightXPos = m_oSelectionRect.m_vRight * m_vTimeZoomScale;
	}
*/
	EMGClipContainer::UpdateTimeScale(p_vTimeScale);

	if(m_opRubberBand != NULL)
		m_opRubberBand -> SetTimeZoomScale(p_vTimeScale);

	if(m_opClipContainerWithSelection == this)
	{
		m_oSelectionRect.m_vLeft = m_vSelectionLeftXPos / m_vTimeZoomScale;
		m_oSelectionRect.m_vRight = m_vSelectionRightXPos / m_vTimeZoomScale;
	}
}

