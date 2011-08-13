/*******************************************************
* This code is the copyrighted property of Elegant Media
*-------------------------------------------------------
* Creator: Johan Dunberg
* Component: Framework, GUI
* Portability: non - native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_DRAG_DROP
#define __EM_DRAG_DROP

enum EMDragDropResult
{
	EM_DRAG_DROP_OK,
	EM_DRAG_DROP_FAILED,
	EM_DRAG_DROP_WRONG_TARGET
};

enum EMDragDropClassType
{
	EM_VIEW,
	EM_LIST_VIEW_CONTROL
};

enum EMMouseOver
{
	EM_SOURCE,
	EM_TARGET,
	EM_SOMEWHERE_ELSE
};

class EMDragDropData
{
public:
	string m_opString;
	int32 m_vEntryIDs[2];  // First Audio, Second Video
	int32 m_vMouseX;
	int32 m_vMouseY;
};

class EMDragDrop
{
public:
	~EMDragDrop();
	virtual EMDragDropData* GetData() = 0;
	virtual EMDragDropResult DoDragDrop(void* p_opSource, EMDragDropData* p_opData, EMDragDropClassType p_eSourceClassType) = 0;
	virtual EMDragDropResult Drop(EMMouseOver p_eMouseOver) = 0;
	virtual EMDragDropResult DragEnter(EMMouseOver p_eMouseOver) = 0;
	virtual EMDragDropResult DragLeave() = 0;
	virtual EMDragDropResult DragMove(EMMouseOver p_eMouseOver) = 0;
	virtual bool IsActive() = 0; // return true if DragDrop is activated
	static EMDragDrop* Instance();
	virtual EMDragDropResult RegisterTarget(void* p_opTarget, EMDragDropClassType p_eTargetClassType) = 0;

protected:
	EMDragDrop();

protected:
	static EMDragDrop* m_opInstance;
};

#endif