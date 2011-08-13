/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_G_RUBBERBAND
#define __EM_G_RUBBERBAND

#include "EMListenerRepository.h"
#include "EMRect.h"
#include "GUIGlobals.h" // FIXME: application class in the framework

#include <list>
//#include <utility>

enum EMRubberBandDrawMode
{
	EM_RUBBERBAND_DRAW_LINES,
	EM_RUBBERBAND_DRAW_BARS,
	EM_RUBBERBAND_DRAW_SAMPLEHOLD,
	EM_RUBBERBAND_DRAW_BLANK
};

class EMView;

// Container class
class EMGRubberBandNode
{
public:
	EMGRubberBandNode() : m_vPosition(-1), m_vSelected(false), m_vValue(0), m_opDataLink(NULL) {};
	EMGRubberBandNode(int64 p_vPosition, bool p_vSelected, int32 p_vValue, void* p_opDataLink = NULL) : 
		m_vPosition(p_vPosition), m_vSelected(p_vSelected), m_vValue(p_vValue), m_opDataLink(p_opDataLink) {};

public:
	int64 m_vPosition;
	int64 m_vStartPosition;
	bool m_vSelected;
	int32 m_vStartValue; // The value of the node when it is starting to be moved (to make nice relative moves possible)
	int32 m_vValue;
	void* m_opDataLink;
};

class EMGRubberBand : public EMListenerRepository
{
public:
	EMGRubberBand(EMView* p_opView, int32 p_vMaxValue, int32 p_vMinValue, int32 p_vDefaultValue, EMRubberBandDrawMode p_eDrawMode);
	~EMGRubberBand();
	void AddNodes(list<EMGRubberBandNode*>* p_opNodes);
	void ClearNodes(bool p_vNotify = true);
	EMGRubberBandNode* CreateNode(int64 p_vPosition, int32 p_vValue, void* p_opDataLink = NULL, bool p_vNotify = true, bool p_vCreateUndo = true);
	void DeleteNodes(list<EMGRubberBandNode*>* p_opNodes, bool p_vCreateUndo = true, bool p_vNotify = true);
	void DeselectAllNodes();
	void Draw(EMRect p_oClippingRect);
	list<EMGRubberBandNode*>* GetAffectedNodes();
	list<EMGRubberBandNode*>* GetNodes();
	list<EMGRubberBandNode*>* GetNodes(EMRect p_oSelectionRect);
	list<EMGRubberBandNode*>* GetSelectedNodes();
	void InvalidateAll();
//	void MouseDoubleClicked(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMEditMode p_eEditMode);
	bool MouseDown(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, ToolboxMode p_eToolboxMode);
	bool MouseMoved(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, ToolboxMode p_eToolboxMode);
	bool MouseUp(int32 p_vX, int32 p_vY, ToolboxMode p_eToolboxMode);
	void SetDefaultValue(int32 p_vDefaultValue);
	void SetDrawMode(EMRubberBandDrawMode p_eDrawMode);
	void SetFrame(EMRect p_oFrame);
	void SetMinMax(int32 p_vMin, int32 p_vMax);
	void SetPermissions(bool p_vAllowCreateNewNodes, bool p_vAllowMoveNodesX, bool p_vAllowMoveNodesY);
	void SetPositionOffset(int64 p_vPositionOffset);
	uint32 SetSelected(list<EMGRubberBandNode*>* p_opNodes, bool p_vDeselectOtherNodes);
	void SetTimeZoomScale(int64 p_vTimeZoomScale);

private:
	//void CalculatePreviousAndNextNodes();
	//void DeleteNode(EMGRubberBandNode* p_opNode);
	void DrawNode(int32 p_vPixelX, int32 p_vPixelY, EMGRubberBandNode* p_opNode);
	int32 GetPixelYFromValue(int32 p_vValue);
	int32 GetValueFromPixelY(int32 p_vPixelY);
	void PrepareToMove();

private:
	EMRubberBandDrawMode m_eDrawMode;
	EMRect m_oFrame;
	list<EMGRubberBandNode*> m_oAffectedNodes; // Contains the nodes affected by the last operation
	list<EMGRubberBandNode*> m_oNodes; // Must always be sorted
	list<EMGRubberBandNode*> m_oReturnedNodes; // Used in GetNodes(...)
	list<EMGRubberBandNode*> m_oSelectedNodes; // Must always be sorted
	EMGRubberBandNode* m_opCurrentNode;
	EMGRubberBandNode* m_opNextNode;
	EMGRubberBandNode* m_opPreviousNode;
	EMView* m_opView;
	bool m_vAllowCreateNewNodes;
	bool m_vAllowMoveNodesX;
	bool m_vAllowMoveNodesY;
	int32 m_vCurrentPixelX;
	int32 m_vCurrentPixelY;
	int32 m_vDefaultValue;
	int32 m_vMaxValue;
	int32 m_vMinValue;
	int64 m_vMouseDownPos;
	bool m_vMouseIsDown;
	//bool m_vNodeIsAdded;
	bool m_vNodeIsDeleted;
	bool m_vPositionsChanged;
	int32 m_vPreviousPixelX;
	int64 m_vPositionOffset;
	int32 m_vPreviousPixelY;
	int64 m_vTimeZoomScale;
	bool m_vValuesChanged;
};

#endif



