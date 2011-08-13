#include "FXNode.h"

#include "AudioGClipContainer.h"
#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMFactory.h"
#include "EMGEffectsRepository.h"
#include "EMGUIGlobals.h"
#include "EMMediaGlobals.h"
#include "EMMenuItem.h"
#include "EMMessages.h"
#include "EMPopUpMenu.h"
#include "EMResourceRepository.h"
#include "EMView.h"
#include "GUIGlobals.h"
#include "ResourceIDs.h"
#include "VideoGClipContainer.h"

#include <map>

const int LOWER_BORDER_SIZE = 3;
const int VALUE_BOX_WIDTH = 22;
const int MENU_ITEM_EMPTY = 1;
const uint32 MESSAGE_VALUE_BOX_CHANGED = EM_MESSAGE_BASE_OFFSET + 1;

EMPopUpMenu* FXNode::m_opPopUpMenu = NULL;

FXNode::FXNode(EMMediaType p_eType, int p_vNodeIndex, EMRect p_oParentFrame, EMView* p_opView, EMGClipContainer* p_opClipContainer, int32 p_vMediaEffectTrackID,
int32 p_vMediaEffectEntryID, int32 p_vMediaEffectID, const char* p_vpEffectName) :
m_eType(p_eType),
m_oInputSignalValueBox(p_opView, EMRect(), 0, 100, 50, 0, NULL, NULL, NULL, true, true, true, true, 0, false),
m_oParentFrame(p_oParentFrame),
m_opClipContainer(p_opClipContainer),
m_opView(p_opView),
m_vCommandIsSet(false),
m_vMediaEffectEntryID(p_vMediaEffectEntryID),
m_vMediaEffectID(p_vMediaEffectID),
m_vMediaEffectTrackID(p_vMediaEffectTrackID),
m_vNodeIndex(-1),
m_vValueBoxMessage(MESSAGE_VALUE_BOX_CHANGED)
{
	if(p_vMediaEffectID != -1)
	{
		EMMediaEffectInfo* opEffectInfo = static_cast<EMMediaEffectInfo*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_EFFECT_INFO, &p_vMediaEffectID));
		m_oInputSignalValueBox.SetValue(opEffectInfo -> m_vDryWetMix * 100);
	}

	if((p_vNodeIndex % 2) > 0)
		m_vIsActive = true;
	else
		m_vIsActive = false;

	SetIndex(p_vNodeIndex);

	if(p_vpEffectName != NULL)
		m_oEffectName = p_vpEffectName;
}

FXNode::~FXNode()
{
//eo << "Deleting FXNode with effect: " << m_oEffectName << ", and index: " << m_vNodeIndex << ef;
	delete m_opPopUpMenu;
	m_opPopUpMenu = NULL;
}

void FXNode::CalculateEffectNamePosition()
{
	if(m_oEffectName == "")
		return;

	int32 vAscent;
	int32 vDescent;
	if(m_opView -> LockWindow())
	{
		m_opView -> CalculateStringHeight(m_oEffectName.c_str(), vAscent, vDescent);
		m_opView -> UnlockWindow();
	}
	int32 vYSpacing = (m_oFrame.GetHeight() - (vAscent + vDescent)) / 2;
	m_vStringTop = m_oFrame.m_vTop + vYSpacing + vAscent;
	m_vStringLeft = m_oFrame.m_vLeft + 2;//m_oFrame.m_vLeft + ((m_oFrame.GetWidth() - m_opView -> CalculateStringWidth(m_vpCurrentValue)) / 2);
}

void FXNode::Draw(EMRect p_oClippingRect, EMView* p_opView)
{
	if(!p_oClippingRect.Intersects(m_oFrame) && !p_oClippingRect.Intersects(m_oInputSignalValueBox.Frame()))
		return;

	EMRect vRect = m_oFrame;

	if(m_vIsActive)
	{
		m_oInputSignalValueBox.Draw(p_oClippingRect);

		if(m_eType == EM_TYPE_ANY_AUDIO)
		{
			p_opView -> SetHighColor(EM_COLOR_AUDIO_VIEW_SHADOW);
			p_opView -> StrokeLine(vRect.m_vLeft, vRect.m_vBottom, vRect.m_vRight, vRect.m_vBottom);
			p_opView -> StrokeLine(vRect.m_vRight, vRect.m_vTop, vRect.m_vRight, vRect.m_vBottom);
			p_opView -> SetHighColor(EM_COLOR_AUDIO_VIEW_HIGHLIGHT);
			p_opView -> StrokeLine(vRect.m_vLeft, vRect.m_vTop, vRect.m_vRight, vRect.m_vTop);
			p_opView -> StrokeLine(vRect.m_vLeft, vRect.m_vTop, vRect.m_vLeft, vRect.m_vBottom);
			p_opView -> SetHighColor(EM_COLOR_AUDIO_VIEW_BACKGROUND);
			vRect.InsetBy(1, 1);
			p_opView -> FillRect(vRect);
		}
		else if(m_eType == EM_TYPE_ANY_VIDEO)
		{
			p_opView -> SetHighColor(EM_COLOR_VIDEO_VIEW_SHADOW);
			p_opView -> StrokeLine(vRect.m_vLeft, vRect.m_vBottom, vRect.m_vRight, vRect.m_vBottom);
			p_opView -> StrokeLine(vRect.m_vRight, vRect.m_vTop, vRect.m_vRight, vRect.m_vBottom);
			p_opView -> SetHighColor(EM_COLOR_VIDEO_VIEW_HIGHLIGHT);
			p_opView -> StrokeLine(vRect.m_vLeft, vRect.m_vTop, vRect.m_vRight, vRect.m_vTop);
			p_opView -> StrokeLine(vRect.m_vLeft, vRect.m_vTop, vRect.m_vLeft, vRect.m_vBottom);
			p_opView -> SetHighColor(EM_COLOR_VIDEO_VIEW_BACKGROUND);
			vRect.InsetBy(1, 1);
			p_opView -> FillRect(vRect);
		}

		if(m_oEffectName != "")
		{
			p_opView -> SetHighColor(EM_COLOR_TEXT);
			p_opView -> ConstrainClippingRect(m_oFrame);
			p_opView -> DrawString(m_vStringLeft, m_vStringTop, m_oEffectName.c_str());
			p_opView -> ConstrainClippingRect(EMRect());
		}
	}
	else
	{
		p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_HIGHLIGHT);
		p_opView -> StrokeRect(vRect);
		p_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_SHADOW);
		vRect.InsetBy(1, 1);
		p_opView -> FillRect(vRect);
	}
}

int32 FXNode::GetEffectTrackID()
{
	return m_vMediaEffectTrackID;
}

bool FXNode::IsActive()
{
	return m_vIsActive;
}

bool FXNode::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	if(p_vMessage == MESSAGE_VALUE_BOX_CHANGED)
	{
		uint32 vValueBoxValue = *(static_cast<int32*>(m_oInputSignalValueBox.GetValue()));
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_EFFECT_DRY_WET_MIX, &m_vMediaEffectID, &m_vMediaEffectTrackID, &vValueBoxValue);
	}

	else // Message from pop-up menu
	{
eo << "FXNode: message from popup menu: " << p_vMessage << ef;
		bool vNeedsUpdate(false);
		uint32 vSlotNr = m_vNodeIndex / 2;

		if(p_vMessage == MENU_ITEM_EMPTY)
		{
			if(m_vIsActive)
			{
eo << "deleting effect with ID: " << m_vMediaEffectEntryID << ", in slot nr: " << vSlotNr << ", node index: " << m_vNodeIndex << ef;
				EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_DELETE_EFFECT, &m_vMediaEffectTrackID, &vSlotNr);
				m_oEffectName = "";
				m_vMediaEffectEntryID = -1;
				m_vIsActive = false;
				vNeedsUpdate = true;
			}
		}	
		else
		{
			string* tmp = EMGEffectsRepository::Instance() -> GetEffectName(m_eType, p_vMessage - 2, &m_vMediaEffectEntryID);

			if(m_vMediaEffectTrackID == -1)
				EMDebugger("The effect track parameter is not set yet");

			if(!m_vIsActive)
			{
eo << "Inserting effect with ID: " << m_vMediaEffectEntryID << ", into slot nr: " << vSlotNr << ", node index: " << m_vNodeIndex << ef;
				m_vMediaEffectID = *(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_INSERT_EFFECT, &m_vMediaEffectEntryID, &m_vMediaEffectTrackID, &vSlotNr)));
			}
			else
			{
eo << "Setting effect with ID: " << m_vMediaEffectEntryID << ", into slot nr: " << vSlotNr << ", node index: " << m_vNodeIndex << ef;
				m_vMediaEffectID = *(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_EFFECT, &m_vMediaEffectEntryID, &m_vMediaEffectTrackID, &vSlotNr)));
			}

			m_oEffectName = tmp -> c_str();
			m_vIsActive = true;
			vNeedsUpdate = true;
		}

		if(vNeedsUpdate)
		{
			if(dynamic_cast<AudioGClipContainer*>(m_opClipContainer))
				dynamic_cast<AudioGClipContainer*>(m_opClipContainer) -> UpdateFXNodes();
			else if(dynamic_cast<VideoGClipContainer*>(m_opClipContainer))
				dynamic_cast<VideoGClipContainer*>(m_opClipContainer) -> UpdateFXNodes();
		}

		CalculateEffectNamePosition();
	}

	return true;
}

bool FXNode::MouseDoubleClicked(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
	if(!m_oFrame.Contains(p_vX, p_vY))
		return false;

	if(!m_vIsActive)
		return false;

	m_opView -> Notify(EM_MOUSE_UP);
eo << "before showing fx dialog" << ef;
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SHOW_EFFECT_DIALOG, &m_vMediaEffectID, &m_vMediaEffectTrackID);
eo << "after showing fx dialog" << ef;
//	EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DISPLAY_PROPERTY_PAGE_DIALOG, &m_vMediaEffectEntryID);
	return true;
}

bool FXNode::MouseDown(EMView* m_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
	if(!m_vCommandIsSet)
	{
		m_oInputSignalValueBox.SetCommand(COMMAND_MESSAGE_SENDER, &m_vValueBoxMessage, this, NULL);
		m_vCommandIsSet = true;
	}

	if(m_oInputSignalValueBox.MouseDown(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree))
	{
		m_opView -> Invalidate(m_oInputSignalValueBox.Frame());
		return true;
	}

	if(!m_oFrame.Contains(p_vX, p_vY))
	{
		return false;
	}

	if(p_vButtonTwo)
	{
		delete m_opPopUpMenu;
		m_opPopUpMenu = EMFactory::Instance() -> CreatePopUpMenu();
		m_opPopUpMenu -> AddListener(this);
		EMMenuItem* opItem;

//		if(m_oValues.size() > 0)
		{
			opItem = EMFactory::Instance() -> CreateMenuItem(EMResourceRepository::GetString(RES_TM_NO_EFFECT)/*EM_new string("No Effect")*/, MENU_ITEM_EMPTY);
			m_opPopUpMenu -> AddItem(opItem);
			if(!m_vIsActive)
				opItem -> SetMarked(true);
			m_opPopUpMenu -> AddSeparatorItem();
			map<int32, string*>* opEffects = EMGEffectsRepository::Instance() -> GetEffectNames(m_eType);
			map<int32, string*>::iterator oIterator = opEffects -> begin();
			int vIndex(2);
			while(oIterator != opEffects -> end())
			{
				opItem = EMFactory::Instance() -> CreateMenuItem(oIterator -> second, vIndex);
				m_opPopUpMenu -> AddItem(opItem);
				if(*(oIterator -> second) == m_oEffectName)
					opItem -> SetMarked(true);
				vIndex++;
				oIterator++;
			}
		}

		int32 vNewX = p_vX;
		int32 vNewY = p_vY;
		m_opView -> ConvertToScreenCoordinates(vNewX, vNewY);
		m_opPopUpMenu -> PopUp(vNewX, vNewY, m_opView);
	}
	return true;
}

bool FXNode::MouseMoved(EMView* m_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree)
{
	if(m_oInputSignalValueBox.MouseMoved(p_vX, p_vY, p_vButtonOne, p_vButtonTwo, p_vButtonThree, true))
	{
		if(m_eType == EM_TYPE_ANY_VIDEO)
		{
			eo << "FXNode::MouseMoved -> MEDIA_COMMAND_REFRESH_PREVIEW" << ef;
			EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_REFRESH_PREVIEW);
		}
		m_opView -> Invalidate(m_oInputSignalValueBox.Frame());
		return true;
	}

	return false;
}

bool FXNode::MouseUp(EMView* m_opView, int32 p_vX, int32 p_vY)
{
	if(m_oInputSignalValueBox.MouseUp(p_vX, p_vY))
	{
		m_opView -> Invalidate(m_oInputSignalValueBox.Frame());
		return true;
	}

	return false;
}

void FXNode::SetIndex(int p_vNodeIndex)
{
	if(p_vNodeIndex == m_vNodeIndex)
		return;

	m_vNodeIndex = p_vNodeIndex;
	UpdateFrame();
	CalculateEffectNamePosition();
}

void FXNode::SetMediaEffectTrackID(int32 p_vMediaEffectTrackID)
{
	m_vMediaEffectTrackID = p_vMediaEffectTrackID;
}

void FXNode::SetParent(EMGClipContainer* p_opClipContainer)
{
	m_opClipContainer = p_opClipContainer;
	m_oParentFrame = p_opClipContainer -> Frame();
	UpdateFrame();
	CalculateEffectNamePosition();
}

void FXNode::UpdateFrame()
{
	int32 vTrackMiddleY = m_oParentFrame.m_vTop + ((m_oParentFrame.GetHeight() - LOWER_BORDER_SIZE) / 2);
	int32 vLeftEdge;
	if((m_vNodeIndex % 2) > 0) // If odd index
		vLeftEdge = FX_VALUE_BOX_X_SPACING * 2 + FX_VALUE_BOX_WIDTH + static_cast<int>(m_vNodeIndex / 2) * FX_ACTIVE_NODE_WIDTH + 
							static_cast<int>(m_vNodeIndex / 2 + 1) * FX_NODE_WIDTH + (1 + m_vNodeIndex) * FX_NODE_X_SPACING;
	else // index is even
		vLeftEdge = FX_VALUE_BOX_X_SPACING * 2 + FX_VALUE_BOX_WIDTH + m_vNodeIndex / 2 * FX_ACTIVE_NODE_WIDTH + 
							m_vNodeIndex / 2 * FX_NODE_WIDTH + (1 + m_vNodeIndex) * FX_NODE_X_SPACING;

	if(m_vIsActive)
	{
		m_oFrame = EMRect(vLeftEdge + VALUE_BOX_WIDTH, vTrackMiddleY - (FX_ACTIVE_NODE_HEIGTH / 2), vLeftEdge + FX_ACTIVE_NODE_WIDTH, vTrackMiddleY + (FX_ACTIVE_NODE_HEIGTH / 2));
		m_oInputSignalValueBox.SetFrame(EMRect(vLeftEdge, vTrackMiddleY - (FX_ACTIVE_NODE_HEIGTH / 2), vLeftEdge + VALUE_BOX_WIDTH - 1, vTrackMiddleY + (FX_ACTIVE_NODE_HEIGTH / 2)));
	}
	else
		m_oFrame = EMRect(vLeftEdge, vTrackMiddleY - (FX_NODE_HEIGTH / 2), vLeftEdge + FX_NODE_WIDTH, vTrackMiddleY + (FX_NODE_HEIGTH / 2));
}


