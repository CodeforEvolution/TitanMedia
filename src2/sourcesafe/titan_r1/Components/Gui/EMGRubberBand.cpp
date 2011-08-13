#include "EMGRubberBand.h"

#include "CommandIDs.h" // FIXME: application file in the framework
#include "EMCommandRepository.h"
#include "EMKeyboard.h"
#include "EMKeyboardGlobals.h"
#include "EMMessages.h"
#include "EMSettingsRepository.h"
#include "EMView.h"
#include "GUIGlobals.h"
#include "SettingIDs.h" // FIXME: application header in the framework

const int32 NODE_WIDTH = 9; // Should be an odd value
const int32 LINE_HIT_THRESHOLD = 7; //How far a mouse down outside a line should be considered hitting the line

EMGRubberBand::EMGRubberBand(EMView* p_opView, int32 p_vMaxValue, int32 p_vMinValue, int32 p_vDefaultValue, EMRubberBandDrawMode p_eDrawMode) :
m_eDrawMode(p_eDrawMode),
m_opCurrentNode(NULL),
m_opNextNode(NULL),
m_opPreviousNode(NULL),
m_opView(p_opView),
m_vAllowCreateNewNodes(true),
m_vAllowMoveNodesX(true),
m_vAllowMoveNodesY(true),
m_vDefaultValue(p_vDefaultValue),
m_vMaxValue(p_vMaxValue),
m_vMinValue(p_vMinValue),
m_vMouseIsDown(false),
m_vPositionOffset(0),
m_vTimeZoomScale(-1)
{
}

EMGRubberBand::~EMGRubberBand()
{
	ClearNodes(false);
}

// This procedure is used to put back deleted nodes in an undo operation
void EMGRubberBand::AddNodes(list<EMGRubberBandNode*>* p_opNodes)
{
	EMGRubberBandNode* opNode;
	list<EMGRubberBandNode*>::iterator oNewIterator = p_opNodes -> begin();
	while(oNewIterator != p_opNodes -> end())
	{
		opNode = *oNewIterator;

		// Make sure that it's not selected
		opNode -> m_vSelected = false;

		if(m_oNodes.size() == 0)
			m_oNodes.push_front(opNode);

		else if(m_oNodes.front() -> m_vPosition > opNode -> m_vPosition)
			m_oNodes.push_front(opNode);

		else if(m_oNodes.back() -> m_vPosition <= opNode -> m_vPosition)
			m_oNodes.push_back(opNode);

		else
		{
			list<EMGRubberBandNode*>::iterator oCurrentIterator = m_oNodes.begin();
			oCurrentIterator++;
			while(oCurrentIterator != m_oNodes.end())
			{
				if(opNode -> m_vPosition < (*oCurrentIterator) -> m_vPosition)
				{
					m_oNodes.insert(oCurrentIterator, opNode);
					break;
				}
				oCurrentIterator++;
			}
		}

		oNewIterator++;
	}
}

/*
// Assumes that at least one node exists and that at least one node is selected
void EMGRubberBand::CalculatePreviousAndNextNodes()
{
	if(m_oNodes.size() == 1)
	{
		m_opPreviousNode = NULL;
		m_opNextNode = NULL;
		return;
	}

	list<EMGRubberBandNode*>::iterator oIterator = m_oNodes.begin();
	while(oIterator != m_oNodes.end())
	{
		if(*oIterator == m_oSelectedNodes.front())
		{
			if(*oIterator == m_oNodes.front())
				m_opPreviousNode = NULL;
			else
			{
				oIterator--;
				m_opPreviousNode = *oIterator;
				oIterator++;
			}
		}

		if(*oIterator == m_oSelectedNodes.back())
		{
			if(*oIterator == m_oNodes.back())
				m_opNextNode = NULL;
			else
			{
				oIterator++;
				m_opNextNode = *oIterator;
			}
			return;
		}

		oIterator++;
	}

	EMDebugger("Seems like no nodes are selected (or no nodes exist at all)");
}
*/
void EMGRubberBand::ClearNodes(bool p_vNotify)
{
	m_oAffectedNodes = m_oNodes;

	if(p_vNotify)
	{
		Notify(EM_RUBBERBAND_NODES_CHANGED);
		Notify(EM_RUBBERBAND_NODES_DELETED);
	}

	EMGRubberBandNode* opNode;
	list<EMGRubberBandNode*>::iterator oIterator = m_oNodes.begin();
	while(oIterator != m_oNodes.end())
	{
		opNode = *oIterator;
		oIterator++;
		delete opNode;
	}

	m_oNodes.clear();
	m_oSelectedNodes.clear();
}

// Creates a new node and inserts it (sorted) into m_oNodes
// It is assumed that the created node gets a unique position
// It is assumed that a node does not get added twice
EMGRubberBandNode* EMGRubberBand::CreateNode(int64 p_vPosition, int32 p_vValue, void* p_opDataLink, bool p_vNotify, bool p_vCreateUndo)
{
	p_vPosition += m_vPositionOffset;

	EMGRubberBandNode* opNode = EM_new EMGRubberBandNode(p_vPosition, false, p_vValue, p_opDataLink);

	if(m_oNodes.size() == 0)
		m_oNodes.push_front(opNode);

	else if(m_oNodes.front() -> m_vPosition > p_vPosition)
		m_oNodes.push_front(opNode);

	else if(m_oNodes.back() -> m_vPosition <= p_vPosition)
		m_oNodes.push_back(opNode);

	else
	{
		list<EMGRubberBandNode*>::iterator oIterator = m_oNodes.begin();
		oIterator++;
		while(oIterator != m_oNodes.end())
		{
			if(p_vPosition < (*oIterator) -> m_vPosition)
			{
				m_oNodes.insert(oIterator, opNode);
				break;
			}
			oIterator++;
		}
	}

	if(p_vCreateUndo)
	{
		list<EMGRubberBandNode*> oAddedNodes;
		oAddedNodes.push_back(opNode);
		EMCommandRepository::Instance() -> ExecuteCommand(UNDO_COMMAND_ADD_RUBBERBAND_NODES, this, &oAddedNodes);
	}

	m_oAffectedNodes.clear();
	m_oAffectedNodes.push_back(opNode);

	if(p_vNotify)
	{
		Notify(EM_RUBBERBAND_NODES_CHANGED);
		Notify(EM_RUBBERBAND_NODES_ADDED);
	}

//eo << "New node created - position: " << (double) p_vPosition << ", value: " << p_vValue << ", total nr nodes: " << m_oNodes.size() << ef;
	return opNode;
}
/*
// Assumes that the node isn't in the middle of a selection (but can be at the ends of a selection)
void EMGRubberBand::DeleteNode(EMGRubberBandNode* p_opNode)
{
	EMGRubberBandNode* opNode = p_opNode;
	m_oNodes.remove(p_opNode);
	m_oSelectedNodes.remove(p_opNode);
	delete opNode;
}
*/
void EMGRubberBand::DeleteNodes(list<EMGRubberBandNode*>* p_opNodes, bool p_vCreateUndo, bool p_vNotify)
{
	m_oAffectedNodes = *p_opNodes;

	if(p_vNotify)
	{
		Notify(EM_RUBBERBAND_NODES_CHANGED);
		Notify(EM_RUBBERBAND_NODES_DELETED);
	}

	if(p_vCreateUndo)
		EMCommandRepository::Instance() -> ExecuteCommand(UNDO_COMMAND_DELETE_RUBBERBAND_NODES, this, p_opNodes);

	list<EMGRubberBandNode*>::iterator oIterator = p_opNodes -> begin();
	EMGRubberBandNode* opNode;
	while(oIterator != p_opNodes -> end())
	{
		opNode = *oIterator;
		oIterator++;
		m_oNodes.remove(opNode);
		m_oSelectedNodes.remove(opNode);
	}
}

void EMGRubberBand::DeselectAllNodes()
{
	list<EMGRubberBandNode*>::iterator oIterator = m_oSelectedNodes.begin();
	while(oIterator != m_oSelectedNodes.end())
	{
		(*oIterator) -> m_vSelected = false;
		oIterator++;
	}

	m_oSelectedNodes.clear();
}

void EMGRubberBand::Draw(EMRect p_oClippingRect)
{
	if(!m_oFrame.IsValid() || m_vTimeZoomScale == -1)
		EMDebugger("The rubberband frame or zoom scale is not set yet");

	if(!p_oClippingRect.Intersects(m_oFrame))
		return;

	if(m_eDrawMode == EM_RUBBERBAND_DRAW_BLANK)
		return;

	EMRect oDrawRect(m_oFrame & p_oClippingRect);
	oDrawRect.InsetBy(-1 * NODE_WIDTH, 0/*-1 * NODE_WIDTH*/); // To surely encompass partially redrawed nodes
	if(oDrawRect.m_vLeft < 0)
		oDrawRect.m_vLeft = 0;
	if(oDrawRect.m_vTop < 0)
		oDrawRect.m_vTop = 0;

	// Draw the default value (if default value != lowest or highest value) as a red line
	if(m_vDefaultValue != m_vMaxValue && m_vDefaultValue != m_vMinValue)
	{
		m_opView -> SetHighColor(EMColor(255, 0, 0, 255));
		m_opView -> StrokeLine(oDrawRect.m_vLeft, GetPixelYFromValue(m_vDefaultValue), oDrawRect.m_vRight, GetPixelYFromValue(m_vDefaultValue));
	}

	m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_SHADOW);

	if(m_oNodes.size() == 0)
	{
		// No nodes added, just draw a straight line
		m_opView -> StrokeLine(oDrawRect.m_vLeft, GetPixelYFromValue(m_vDefaultValue), oDrawRect.m_vRight, GetPixelYFromValue(m_vDefaultValue));
		return;
	}

	m_vCurrentPixelX = (m_oNodes.front() -> m_vPosition + m_vPositionOffset) / m_vTimeZoomScale;
	m_vCurrentPixelY = GetPixelYFromValue(m_oNodes.front() -> m_vValue);

	// If the line before the first node is visible
	if(m_vCurrentPixelX + (NODE_WIDTH / 2) > oDrawRect.m_vLeft)
	{
		if(m_vCurrentPixelX > oDrawRect.m_vRight && m_eDrawMode != EM_RUBBERBAND_DRAW_BARS)
		{
			// The first node is not reached, just draw a straight line
			m_opView -> StrokeLine(oDrawRect.m_vLeft, m_vCurrentPixelY, oDrawRect.m_vRight, m_vCurrentPixelY);
			return;
		}

		// The first node is reached, draw a straight line to it and draw the node
		if(m_eDrawMode != EM_RUBBERBAND_DRAW_BARS)
			m_opView -> StrokeLine(oDrawRect.m_vLeft, m_vCurrentPixelY, m_vCurrentPixelX, m_vCurrentPixelY);
		else
			m_opView -> StrokeLine(m_vCurrentPixelX, m_vCurrentPixelY, m_vCurrentPixelX, m_oFrame.m_vBottom);
		DrawNode(m_vCurrentPixelX, m_vCurrentPixelY, m_oNodes.front()); // If visible, draw the first node
	}

	list<EMGRubberBandNode*>::iterator oIterator = m_oNodes.begin();
	oIterator++; // The first node is already drawn

	while(oIterator != m_oNodes.end())
	{
		m_vPreviousPixelX = m_vCurrentPixelX;
		m_vPreviousPixelY = m_vCurrentPixelY;
		m_vCurrentPixelX = ((*oIterator) -> m_vPosition + m_vPositionOffset) / m_vTimeZoomScale;
		m_vCurrentPixelY = GetPixelYFromValue((*oIterator) -> m_vValue);

		if(m_vPreviousPixelX > oDrawRect.m_vRight)
			return; // The rest of the lines and nodes are not reached and thus not drawn

		if(m_vPreviousPixelX + (NODE_WIDTH / 2) < oDrawRect.m_vLeft && m_vCurrentPixelX - (NODE_WIDTH / 2) > oDrawRect.m_vRight)
		{
			// The previous and next node is not in range, just draw a line between them
			if(m_eDrawMode == EM_RUBBERBAND_DRAW_LINES)
				m_opView -> StrokeLine(oDrawRect.m_vLeft, 
								m_vPreviousPixelY + (m_vCurrentPixelY -  m_vPreviousPixelY) * (static_cast<float>(oDrawRect.m_vLeft - m_vPreviousPixelX) / (m_vCurrentPixelX - m_vPreviousPixelX)),
								oDrawRect.m_vRight,
								m_vPreviousPixelY + (m_vCurrentPixelY -  m_vPreviousPixelY) * (static_cast<float>(oDrawRect.m_vRight - m_vPreviousPixelX) / (m_vCurrentPixelX - m_vPreviousPixelX)));
			else if(m_eDrawMode == EM_RUBBERBAND_DRAW_SAMPLEHOLD)
				m_opView -> StrokeLine(oDrawRect.m_vLeft, m_vPreviousPixelY, oDrawRect.m_vRight, m_vPreviousPixelY);
			return;
		}

		// If the previous node is not in range, just draw a line from it to the current node and draw the current node
		if(m_vPreviousPixelX < oDrawRect.m_vLeft)
		{
			if(m_eDrawMode == EM_RUBBERBAND_DRAW_LINES)
				m_opView -> StrokeLine(oDrawRect.m_vLeft, 
								m_vPreviousPixelY + (m_vCurrentPixelY -  m_vPreviousPixelY) * (static_cast<float>(oDrawRect.m_vLeft - m_vPreviousPixelX) / (m_vCurrentPixelX - m_vPreviousPixelX)),
								m_vCurrentPixelX,
								m_vCurrentPixelY);
			else if(m_eDrawMode == EM_RUBBERBAND_DRAW_SAMPLEHOLD)
			{
				m_opView -> StrokeLine(oDrawRect.m_vLeft, m_vPreviousPixelY, m_vCurrentPixelX, m_vPreviousPixelY);
				m_opView -> StrokeLine(m_vCurrentPixelX, m_vPreviousPixelY, m_vCurrentPixelX, m_vCurrentPixelY);
			}
			else if(m_eDrawMode == EM_RUBBERBAND_DRAW_BARS)
				m_opView -> StrokeLine(m_vCurrentPixelX, m_vCurrentPixelY, m_vCurrentPixelX, m_oFrame.m_vBottom);

			DrawNode(m_vCurrentPixelX, m_vCurrentPixelY, *oIterator);
		}
		
		// The previous node is in range but not the current node, draw a line between the previous node and the left side of the drawing frame
		else if(m_vCurrentPixelX > oDrawRect.m_vRight)
		{
			if(m_eDrawMode == EM_RUBBERBAND_DRAW_LINES)
				m_opView -> StrokeLine(m_vPreviousPixelX, 
								m_vPreviousPixelY, 
								oDrawRect.m_vRight,
								m_vPreviousPixelY + (m_vCurrentPixelY -  m_vPreviousPixelY) * (static_cast<float>(oDrawRect.m_vRight - m_vPreviousPixelX) / (m_vCurrentPixelX - m_vPreviousPixelX)));
			else if(m_eDrawMode == EM_RUBBERBAND_DRAW_SAMPLEHOLD)
				m_opView -> StrokeLine(m_vPreviousPixelX, m_vPreviousPixelY, oDrawRect.m_vRight, m_vPreviousPixelY);
			else if(m_eDrawMode == EM_RUBBERBAND_DRAW_BARS)
				m_opView -> StrokeLine(m_vPreviousPixelX, m_vPreviousPixelY, m_vPreviousPixelX, m_oFrame.m_vBottom);
		}

		// The previous and current nodes are both in range, draw a line between them and draw the current node
		else
		{
			if(m_eDrawMode == EM_RUBBERBAND_DRAW_LINES)
				m_opView -> StrokeLine(m_vPreviousPixelX, m_vPreviousPixelY, m_vCurrentPixelX, m_vCurrentPixelY);
			else if(m_eDrawMode == EM_RUBBERBAND_DRAW_SAMPLEHOLD)
			{
				m_opView -> StrokeLine(m_vPreviousPixelX, m_vPreviousPixelY, m_vCurrentPixelX, m_vPreviousPixelY);
				m_opView -> StrokeLine(m_vCurrentPixelX, m_vPreviousPixelY, m_vCurrentPixelX, m_vCurrentPixelY);
			}
			else if(m_eDrawMode == EM_RUBBERBAND_DRAW_BARS)
			{
				m_opView -> StrokeLine(m_vPreviousPixelX, m_vPreviousPixelY, m_vPreviousPixelX, m_oFrame.m_vBottom);
				m_opView -> StrokeLine(m_vCurrentPixelX, m_vCurrentPixelY, m_vCurrentPixelX, m_oFrame.m_vBottom);
			}

			DrawNode(m_vCurrentPixelX, m_vCurrentPixelY, *oIterator);
		}

		oIterator++;
	}

	// If the line after the last node is visible
	if(m_vCurrentPixelX < oDrawRect.m_vRight && m_eDrawMode != EM_RUBBERBAND_DRAW_BARS)
	{
		if(m_vCurrentPixelX < oDrawRect.m_vLeft)
		{
			// The last node is not reached, just draw a straight line
			m_opView -> StrokeLine(oDrawRect.m_vLeft, m_vCurrentPixelY, oDrawRect.m_vRight, m_vCurrentPixelY);
			return;
		}

		// The last node is reached, draw a straight line from it
		m_opView -> StrokeLine(m_vCurrentPixelX, m_vCurrentPixelY, oDrawRect.m_vRight, m_vCurrentPixelY);
	}
}

void EMGRubberBand::DrawNode(int32 p_vPixelX, int32 p_vPixelY, EMGRubberBandNode* p_opNode)
{
	m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_SHADOW);
	m_opView -> StrokeRect(EMRect(p_vPixelX - (NODE_WIDTH / 2), p_vPixelY - (NODE_WIDTH / 2), p_vPixelX + (NODE_WIDTH / 2), p_vPixelY + (NODE_WIDTH / 2)));
	if(p_opNode -> m_vSelected)
		m_opView -> SetHighColor(EM_COLOR_BUTTON_DEFAULT_ON);
	else
		m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_HIGHLIGHT);
	m_opView -> FillRect(EMRect(p_vPixelX - (NODE_WIDTH / 2) + 1, p_vPixelY - (NODE_WIDTH / 2) + 1, p_vPixelX + (NODE_WIDTH / 2) - 1, p_vPixelY + (NODE_WIDTH / 2) - 1));
	m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_SHADOW);
}

list<EMGRubberBandNode*>* EMGRubberBand::GetAffectedNodes()
{
	return &m_oAffectedNodes;
}

list<EMGRubberBandNode*>* EMGRubberBand::GetNodes()
{
	return &m_oNodes;
}

list<EMGRubberBandNode*>* EMGRubberBand::GetNodes(EMRect p_oSelectionRect)
{
//	p_oSelectionRect.m_vTop = m_oFrame.m_vTop;
//	p_oSelectionRect.m_vBottom = m_oFrame.m_vBottom;

	m_oReturnedNodes.clear();

	list<EMGRubberBandNode*>::iterator oIterator = m_oNodes.begin();
	while(oIterator != m_oNodes.end())
	{
		if(p_oSelectionRect.Contains((*oIterator) -> m_vPosition / m_vTimeZoomScale, GetPixelYFromValue((*oIterator) -> m_vValue)))
			m_oReturnedNodes.push_back(*oIterator);

		oIterator++;
	}
	return &m_oReturnedNodes;
}

int32 EMGRubberBand::GetPixelYFromValue(int32 p_vValue)
{
	if(p_vValue <= m_vMinValue)
		return m_oFrame.m_vBottom;

	if(p_vValue >= m_vMaxValue)
		return m_oFrame.m_vTop;

	return (1 - (static_cast<float>(p_vValue - m_vMinValue) / (m_vMaxValue - m_vMinValue))) * (m_oFrame.m_vBottom - m_oFrame.m_vTop) + m_oFrame.m_vTop;
}

list<EMGRubberBandNode*>* EMGRubberBand::GetSelectedNodes()
{
	return &m_oSelectedNodes;
}

int32 EMGRubberBand::GetValueFromPixelY(int32 p_vPixelY)
{
	if(p_vPixelY <= m_oFrame.m_vTop)
		return m_vMaxValue;

	if(p_vPixelY >= m_oFrame.m_vBottom)
		return m_vMinValue;

	return (1 - (static_cast<float>(p_vPixelY - m_oFrame.m_vTop) / (m_oFrame.m_vBottom - m_oFrame.m_vTop))) * (m_vMaxValue - m_vMinValue) + m_vMinValue;
}

void EMGRubberBand::InvalidateAll()
{
	EMRect oInvalidateFrame = m_oFrame;
	oInvalidateFrame.InsetBy(-1 * NODE_WIDTH / 2, 0/*-1 * NODE_WIDTH / 2*/);
	oInvalidateFrame = oInvalidateFrame & m_opView -> Bounds();
	m_opView -> Invalidate(oInvalidateFrame); // Quick 'n dirty redraw
}

// Check if any node or line is hit. If a line is hit, create a new node there
// Returns true if a node is hit or a new node is created (and thus hit)
bool EMGRubberBand::MouseDown(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, ToolboxMode p_eToolboxMode)
{
	if(!m_oFrame.Contains(p_vX, p_vY))
		return false;

	if(!p_vButtonOne) // Currently only the left mouse button does anything
		return false;

	ToolboxMode eToolboxMode = static_cast<ToolboxMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TOOLBOX_MODE))));

	if(eToolboxMode == TOOLBOX_CUT_PASTE || eToolboxMode == TOOLBOX_RENAME || eToolboxMode == TOOLBOX_RESIZE)
		return false;

	m_opCurrentNode = NULL;

	//m_vNodeIsAdded = false;
	m_vPositionsChanged = false;
	m_vValuesChanged = false;

	m_vMouseDownPos = p_vX * m_vTimeZoomScale;

	// If there are no nodes, let's check the default line
	if(m_oNodes.size() == 0)
	{
		if(eToolboxMode == TOOLBOX_ERASE)
			return false;

		m_vCurrentPixelY = GetPixelYFromValue(m_vDefaultValue);
		if(((p_vY >= m_vCurrentPixelY - LINE_HIT_THRESHOLD && p_vY <= m_vCurrentPixelY + LINE_HIT_THRESHOLD) || eToolboxMode == TOOLBOX_DRAW) && m_vAllowCreateNewNodes)
		{
			m_opCurrentNode = CreateNode(p_vX * m_vTimeZoomScale, GetValueFromPixelY(p_vY), NULL, true, false);
			m_opCurrentNode -> m_vSelected = true;
			m_oSelectedNodes.push_back(m_opCurrentNode);
			//CalculatePreviousAndNextNodes();
			InvalidateAll();
			m_vMouseIsDown = true;
			//m_vNodeIsAdded = true;
			PrepareToMove();
			return true;
		}
		return false;
	}

	// Let's check the line after the last node (last node == first node if there's only one node)
	m_vCurrentPixelX = (m_oNodes.back() -> m_vPosition + m_vPositionOffset) / m_vTimeZoomScale;
	if(p_vX > m_vCurrentPixelX + (NODE_WIDTH / 2))
	{
		if(eToolboxMode == TOOLBOX_ERASE)
			return false;

		m_vCurrentPixelY = GetPixelYFromValue(m_oNodes.back() -> m_vValue);
		if((p_vY >= m_vCurrentPixelY - LINE_HIT_THRESHOLD && p_vY <= m_vCurrentPixelY + LINE_HIT_THRESHOLD) || eToolboxMode == TOOLBOX_DRAW)
		{
			if(m_vAllowCreateNewNodes)
			{
				DeselectAllNodes();
				m_opCurrentNode = CreateNode(p_vX * m_vTimeZoomScale, GetValueFromPixelY(p_vY), NULL, true, false);
				m_opCurrentNode -> m_vSelected = true;
				m_oSelectedNodes.push_back(m_opCurrentNode);
				//CalculatePreviousAndNextNodes();
				InvalidateAll();
				m_vMouseIsDown = true;
				//m_vNodeIsAdded = true;
				PrepareToMove();
				return true;
			}
			else if(m_eDrawMode == EM_RUBBERBAND_DRAW_SAMPLEHOLD)
			{
				m_opCurrentNode = m_oNodes.back();
				m_opCurrentNode -> m_vSelected = true;
				//CalculatePreviousAndNextNodes();
				InvalidateAll();
				m_vMouseIsDown = true;
				PrepareToMove();
				return true;
			}
		}
		return false;
	}

	// Let's check the first node
	m_vCurrentPixelX = (m_oNodes.front() -> m_vPosition + m_vPositionOffset) / m_vTimeZoomScale;
	if(p_vX >= m_vCurrentPixelX - (NODE_WIDTH / 2) && p_vX <= m_vCurrentPixelX + (NODE_WIDTH / 2))
	{
		m_vCurrentPixelY = GetPixelYFromValue(m_oNodes.front() -> m_vValue);
		if(p_vY >= m_vCurrentPixelY - (NODE_WIDTH / 2) && p_vY <= m_vCurrentPixelY + (NODE_WIDTH / 2))
		{
			if(eToolboxMode == TOOLBOX_ERASE)
			{
				if(m_vAllowCreateNewNodes)
				{
					list<EMGRubberBandNode*> oNodesToDelete;
					oNodesToDelete.push_back(m_oNodes.front());
					DeleteNodes(&oNodesToDelete, false);
					InvalidateAll();
				}
				return false;
			}
	
			m_opCurrentNode = m_oNodes.front();
			if(!m_opCurrentNode -> m_vSelected)
			{
				DeselectAllNodes();
				m_opCurrentNode -> m_vSelected = true;
				m_oSelectedNodes.push_back(m_opCurrentNode);
			}
			//CalculatePreviousAndNextNodes();
			InvalidateAll();
			m_vMouseIsDown = true;
			PrepareToMove();
			return true;
		}
	}

	// Let's check the line before the first node
	if(p_vX < m_vCurrentPixelX - (NODE_WIDTH / 2))
	{
		if(eToolboxMode == TOOLBOX_ERASE)
			return false;

		m_vCurrentPixelY = GetPixelYFromValue(m_oNodes.front() -> m_vValue);
		if((p_vY >= m_vCurrentPixelY - LINE_HIT_THRESHOLD && p_vY <= m_vCurrentPixelY + LINE_HIT_THRESHOLD) || eToolboxMode == TOOLBOX_DRAW)
		{
			if(m_vAllowCreateNewNodes)
			{
				DeselectAllNodes();
				m_opCurrentNode = CreateNode(p_vX * m_vTimeZoomScale, GetValueFromPixelY(p_vY), NULL, true, false);
				m_opCurrentNode -> m_vSelected = true;
				m_oSelectedNodes.push_back(m_opCurrentNode);
				//CalculatePreviousAndNextNodes();
				InvalidateAll();
				m_vMouseIsDown = true;
				//m_vNodeIsAdded = true;
				PrepareToMove();
				return true;
			}
			else if(m_eDrawMode == EM_RUBBERBAND_DRAW_SAMPLEHOLD && eToolboxMode != TOOLBOX_DRAW)
			{
				m_opCurrentNode = m_oNodes.front();
				if(!m_opCurrentNode -> m_vSelected)
				{
					DeselectAllNodes();
					m_opCurrentNode -> m_vSelected = true;
					m_oSelectedNodes.push_back(m_opCurrentNode);
				}
				//CalculatePreviousAndNextNodes();
				InvalidateAll();
				m_vMouseIsDown = true;
				PrepareToMove();
				return true;
			}
		}
		return false;
	}

	// Let's check all other nodes + the lines between them
	list<EMGRubberBandNode*>::iterator oIterator = m_oNodes.begin();
	m_opPreviousNode = *oIterator;
	m_vCurrentPixelX = ((*oIterator) -> m_vPosition + m_vPositionOffset) / m_vTimeZoomScale;
	m_vCurrentPixelY = GetPixelYFromValue((*oIterator) -> m_vValue);
	oIterator++;
	int32 vLinePixelY;
	while(oIterator != m_oNodes.end())
	{
		m_vPreviousPixelX = m_vCurrentPixelX;
		m_vPreviousPixelY = m_vCurrentPixelY;
		m_vCurrentPixelX = ((*oIterator) -> m_vPosition + m_vPositionOffset) / m_vTimeZoomScale;
		m_vCurrentPixelY = GetPixelYFromValue((*oIterator) -> m_vValue);

		// Let's check the node first
		if(p_vX >= m_vCurrentPixelX - (NODE_WIDTH / 2) && p_vX <= m_vCurrentPixelX + (NODE_WIDTH / 2))
		{
			if(p_vY >= m_vCurrentPixelY - (NODE_WIDTH / 2) && p_vY <= m_vCurrentPixelY + (NODE_WIDTH / 2))
			{
				if(eToolboxMode == TOOLBOX_ERASE)
				{
					if(m_vAllowCreateNewNodes)
					{
						list<EMGRubberBandNode*> oNodesToDelete;
						oNodesToDelete.push_back(*oIterator);
						DeleteNodes(&oNodesToDelete, false);
						InvalidateAll();
					}
					return false;
				}
		
				m_opCurrentNode = *oIterator;
				if(!m_opCurrentNode -> m_vSelected)
				{
					DeselectAllNodes();
					m_opCurrentNode -> m_vSelected = true;
					m_oSelectedNodes.push_back(m_opCurrentNode);
				}
				//CalculatePreviousAndNextNodes();
				InvalidateAll();
				m_vMouseIsDown = true;
				PrepareToMove();
				return true;
			}
		}

		// The current node is not hit, let's check the line between the current and the previous node
		if(p_vX > m_vPreviousPixelX && p_vX < m_vCurrentPixelX)
		{
			if(m_eDrawMode == EM_RUBBERBAND_DRAW_LINES)
				vLinePixelY = m_vPreviousPixelY + (m_vCurrentPixelY -  m_vPreviousPixelY) * (static_cast<float>(p_vX - m_vPreviousPixelX) / (m_vCurrentPixelX - m_vPreviousPixelX));
			else // EM_RUBBERBAND_DRAW_SAMPLEHOLD
				vLinePixelY = m_vPreviousPixelY;
			if((p_vY >= vLinePixelY - LINE_HIT_THRESHOLD && p_vY <= vLinePixelY + LINE_HIT_THRESHOLD) || eToolboxMode == TOOLBOX_DRAW)
			{
				if(eToolboxMode == TOOLBOX_ERASE)
					return false;

				if(m_vAllowCreateNewNodes)
				{
					m_opNextNode = *oIterator;
					DeselectAllNodes();
					m_opCurrentNode = CreateNode(p_vX * m_vTimeZoomScale, GetValueFromPixelY(p_vY), NULL, true, false);
					m_opCurrentNode -> m_vSelected = true;
					m_oSelectedNodes.push_back(m_opCurrentNode);
					//CalculatePreviousAndNextNodes();
					InvalidateAll();
					m_vMouseIsDown = true;
					PrepareToMove();
					//m_vNodeIsAdded = true;
					return true;
				}
				else if(m_eDrawMode == EM_RUBBERBAND_DRAW_SAMPLEHOLD && eToolboxMode != TOOLBOX_DRAW)
				{
					m_opCurrentNode = m_opPreviousNode;
					if(!m_opCurrentNode -> m_vSelected)
					{
						DeselectAllNodes();
						m_opCurrentNode -> m_vSelected = true;
						m_oSelectedNodes.push_back(m_opCurrentNode);
					}
					//CalculatePreviousAndNextNodes();
					InvalidateAll();
					m_vMouseIsDown = true;
					PrepareToMove();
					return true;
				}
			}
		}

		m_opPreviousNode = *oIterator;
		oIterator++;
	}

	return false;
}

bool EMGRubberBand::MouseMoved(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, ToolboxMode p_eToolboxMode)
{
	ToolboxMode eToolboxMode = static_cast<ToolboxMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TOOLBOX_MODE))));

	if(eToolboxMode == TOOLBOX_ERASE && p_vButtonOne)
	{
		if(m_vAllowCreateNewNodes)
		{
			int32 m_vCurrentPixelX;
			int32 m_vCurrentPixelY;
			list<EMGRubberBandNode*> oNodesToDelete;
			list<EMGRubberBandNode*>::iterator oIterator = m_oNodes.begin();
			while(oIterator != m_oNodes.end())
			{
				m_vCurrentPixelX = ((*oIterator) -> m_vPosition + m_vPositionOffset) / m_vTimeZoomScale;
				m_vCurrentPixelY = GetPixelYFromValue((*oIterator) -> m_vValue);
				if(p_vX >= m_vCurrentPixelX - (NODE_WIDTH / 2) && p_vX <= m_vCurrentPixelX + (NODE_WIDTH / 2) &&
					p_vY >= m_vCurrentPixelY - (NODE_WIDTH / 2) && p_vY <= m_vCurrentPixelY + (NODE_WIDTH / 2))
					oNodesToDelete.push_back(*oIterator);

				oIterator++;
			}

			if(oNodesToDelete.size() > 0)
			{
				DeleteNodes(&oNodesToDelete, false);
				InvalidateAll();
			}
		}

		return false;
	}

	if(!m_vMouseIsDown)
		return false;
//eo << "current node - value: " << m_opCurrentNode -> m_vValue  << ", position: " << m_opCurrentNode -> m_vPosition << ", start pos: " << m_opCurrentNode -> m_vStartPosition << ef;
	// Let's limit the x value
	int64 vPositionOffset;
	if(m_vAllowMoveNodesX)
	{

		int64 vPreviousPosition(0);
		int64 vNextPosition;//(INT64_MAX);
		vPositionOffset = (p_vX * m_vTimeZoomScale) - m_vMouseDownPos;
		EMGRubberBandNode* opCurrentNode;
		list<EMGRubberBandNode*>::iterator oIterator = m_oNodes.begin();
		while(oIterator != m_oNodes.end())
		{
			opCurrentNode = *oIterator;

			// If the node isn't selected, skip it but set the previous position to it
			if(!(opCurrentNode -> m_vSelected)) 
			{
				vPreviousPosition = opCurrentNode -> m_vPosition;
				oIterator++;
				continue;
			}
//eo << "vPositionOffset: " << vPositionOffset << ", vPreviousPosition: " << vPreviousPosition << ", opCurrentNode -> m_vStartPosition: " << opCurrentNode -> m_vStartPosition << ef;
			// Check if the node position is less than the previous node
			if(opCurrentNode -> m_vStartPosition + vPositionOffset < vPreviousPosition)
				vPositionOffset = vPreviousPosition - opCurrentNode -> m_vStartPosition;

			oIterator++;

			if(oIterator == m_oNodes.end())
				vNextPosition = m_oFrame.m_vRight * m_vTimeZoomScale;
			else if(!((*oIterator) -> m_vSelected)) 
				vNextPosition = (*oIterator) -> m_vPosition;
			else
				continue;

//eo << "vPositionOffset: " << vPositionOffset << ", vNextPosition: " << vNextPosition << ", opCurrentNode -> m_vStartPosition: " << opCurrentNode -> m_vStartPosition << ef;
			if(opCurrentNode -> m_vStartPosition + vPositionOffset > vNextPosition)
				vPositionOffset = vNextPosition - opCurrentNode -> m_vStartPosition;
		}
//eo << "Final offset: " << vPositionOffset << ef;
		//vPosition = m_vMouseDownPos + vPositionOffset;

//eo << "mouse pos: " << (double) p_vX * m_vTimeZoomScale << ", vOriginalLeftPosition: " << (double) vOriginalLeftPosition << ", vOriginalRightPosition: " << (double) vOriginalRightPosition << ", vLeftPosition: " << (double) vLeftPosition << ", vRightPosition: " << (double) vRightPosition << ", vPosition: " << (double) vPosition << ef;
	}
	else
		vPositionOffset = 0;
		//vPosition = m_opCurrentNode -> m_vPosition;

	int32 vValue;
	if(m_vAllowMoveNodesY)
		vValue =  GetValueFromPixelY(p_vY);
	else
		vValue = m_opCurrentNode -> m_vValue;

//eo << "Mouse moved - position: " << (double) vPosition << ", value: " << GetValueFromPixelY(p_vY) << ", p_vX: " << p_vX << ", p_vY: " << p_vY << ef;

	//int64 vPositionOffset = vPosition - m_opCurrentNode -> m_vPosition;
	int32 vValueOffset = vValue - m_opCurrentNode -> m_vStartValue;

	//m_opCurrentNode -> m_vPosition = vPosition;
//eo << "Position before: " << m_opCurrentNode -> m_vPosition << ", " << m_opCurrentNode -> m_vStartPosition << ef;
	m_opCurrentNode -> m_vPosition = m_opCurrentNode -> m_vStartPosition + vPositionOffset;
//eo << "Position after: " << m_opCurrentNode -> m_vPosition << ", " << m_opCurrentNode -> m_vStartPosition << ef;
	m_opCurrentNode -> m_vValue = vValue;

	uint32 vModifiers = EMKeyboard::Instance() -> GetModifiers();

	// Now update all other nodes with their moved value
	float vRelativeChange;
	bool vUseRelativeMaxMin(false);
	if((vModifiers & EM_SHIFT_KEY) == EM_SHIFT_KEY)
	{
		if(m_opCurrentNode -> m_vStartValue != m_vDefaultValue)
			vRelativeChange = static_cast<float>(m_opCurrentNode -> m_vValue - m_vDefaultValue) / (m_opCurrentNode -> m_vStartValue - m_vDefaultValue); // Change in percent of original value
		else if(vValueOffset > 0)
		{
			vUseRelativeMaxMin = true;
			vRelativeChange = -1;
		}
		else
		{
			vUseRelativeMaxMin = true;
			vRelativeChange = 1;
		}
	}
	list<EMGRubberBandNode*>::iterator oIterator = m_oSelectedNodes.begin();
	while(oIterator != m_oSelectedNodes.end())
	{
		if(*oIterator != m_opCurrentNode)
		{
			// If shift is pressed, a percentual change occurs, otherwize all nodes are moved the same amount
			if((vModifiers & EM_SHIFT_KEY) == EM_SHIFT_KEY)
			{
				if(!vUseRelativeMaxMin)
					(*oIterator) -> m_vValue = ((*oIterator) -> m_vStartValue - m_vDefaultValue) * vRelativeChange + m_vDefaultValue;
				else
				{
					if(vRelativeChange == 1)
						(*oIterator) -> m_vValue = m_vMaxValue;
					else
						(*oIterator) -> m_vValue = m_vMinValue;
				}
			}
			else
				(*oIterator) -> m_vValue = (*oIterator) -> m_vStartValue + vValueOffset;

			if((*oIterator) -> m_vValue > m_vMaxValue)
				(*oIterator) -> m_vValue = m_vMaxValue;
			else if((*oIterator) -> m_vValue < m_vMinValue)
				(*oIterator) -> m_vValue = m_vMinValue;

//			(*oIterator) -> m_vPosition += vPositionOffset;
			(*oIterator) -> m_vPosition = (*oIterator) -> m_vStartPosition + vPositionOffset;
		}
		oIterator++;
	}

	EMRect oInvalidateFrame = m_oFrame;
	oInvalidateFrame.InsetBy(-1 * NODE_WIDTH, -1 * NODE_WIDTH);
	oInvalidateFrame = oInvalidateFrame & m_opView -> Bounds();
	m_opView -> Invalidate(oInvalidateFrame); // Quick 'n dirty redraw

	if(vPositionOffset != 0)
		m_vPositionsChanged = true;
	if(vValueOffset != 0)
		m_vValuesChanged = true;

	return true;
}

bool EMGRubberBand::MouseUp(int32 p_vX, int32 p_vY, ToolboxMode p_eToolboxMode)
{
	m_vMouseIsDown = false;

	if(m_opCurrentNode != NULL)
	{
		/*
		EMRect oInvalidateFrame = m_oFrame;
		oInvalidateFrame.InsetBy(-1 * NODE_WIDTH, -1 * NODE_WIDTH);
		oInvalidateFrame = oInvalidateFrame & m_opView -> Bounds();
		m_opView -> Invalidate(oInvalidateFrame); // Quick 'n dirty redraw
		*/
		InvalidateAll();
		m_opCurrentNode = NULL;
	}

	//EMCommandRepository::Instance() -> ExecuteCommand(UNDO_COMMAND_MOVE_RUBBERBAND_NODES, this, &m_oSelectedNodes);

	// Send appropriate messages. Update: EM_RUBBERBAND_VALUES_CHANGED is no longer used
	if(/*m_vNodeIsAdded || */m_vPositionsChanged || m_vValuesChanged)
	{
		m_oAffectedNodes = m_oSelectedNodes;
		Notify(EM_RUBBERBAND_NODES_CHANGED);
		//if(m_vNodeIsAdded)
		//	Notify(EM_RUBBERBAND_NODES_ADDED);
		//if(m_vPositionsChanged)
			Notify(EM_RUBBERBAND_POSITIONS_CHANGED);
		//if(m_vValuesChanged)
			//Notify(EM_RUBBERBAND_VALUES_CHANGED);
	}

	return true;
}

// Sets the start value and position of all selected nodes to the actual value
void EMGRubberBand::PrepareToMove()
{
	list<EMGRubberBandNode*>::iterator oIterator = m_oSelectedNodes.begin();
	while(oIterator != m_oSelectedNodes.end())
	{
		(*oIterator) -> m_vStartValue = (*oIterator) -> m_vValue;
		(*oIterator) -> m_vStartPosition = (*oIterator) -> m_vPosition;
		oIterator++;
	}
}

void EMGRubberBand::SetDefaultValue(int32 p_vDefaultValue)
{
	m_vDefaultValue = p_vDefaultValue;
}

void EMGRubberBand::SetDrawMode(EMRubberBandDrawMode p_eDrawMode)
{
	m_eDrawMode = p_eDrawMode;
}

void EMGRubberBand::SetFrame(EMRect p_oFrame)
{
	m_oFrame = p_oFrame;
}

void EMGRubberBand::SetMinMax(int32 p_vMin, int32 p_vMax)
{
	m_vMinValue = p_vMin;
	m_vMaxValue = p_vMax;
}

void EMGRubberBand::SetPermissions(bool p_vAllowCreateNewNodes, bool p_vAllowMoveNodesX, bool p_vAllowMoveNodesY)
{
	m_vAllowCreateNewNodes = p_vAllowCreateNewNodes;
	m_vAllowMoveNodesX = p_vAllowMoveNodesX;
	m_vAllowMoveNodesY = p_vAllowMoveNodesY;
}

void EMGRubberBand::SetPositionOffset(int64 p_vPositionOffset)
{
	m_vPositionOffset = p_vPositionOffset;
}

// Returns the number of selected nodes
// Currently, the selection becomes a homogenous range of nodes
uint32 EMGRubberBand::SetSelected(list<EMGRubberBandNode*>* p_opNodes, bool p_vDeselectOtherNodes)
{
	if(p_vDeselectOtherNodes)
		DeselectAllNodes();

	list<EMGRubberBandNode*>::iterator oIterator = p_opNodes -> begin();
	while(oIterator != p_opNodes -> end())
	{
		(*oIterator) -> m_vSelected = true;
		m_oSelectedNodes.push_back(*oIterator);

		oIterator++;
	}

	// If not cleared first, previously selected nodes are now probably in the list twice, fix that
	if(!p_vDeselectOtherNodes)
		m_oSelectedNodes.unique();

	return m_oSelectedNodes.size();
}

void EMGRubberBand::SetTimeZoomScale(int64 p_vTimeZoomScale)
{
	m_vTimeZoomScale = p_vTimeZoomScale;
}

