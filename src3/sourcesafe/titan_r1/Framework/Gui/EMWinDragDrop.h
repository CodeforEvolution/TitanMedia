/*******************************************************
* This code is the copyrighted property of Elegant Media
*-------------------------------------------------------
* Creator: Johan Dunberg
* Component: Framework, GUI
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_DRAG_DROP
#define __EM_WIN_DRAG_DROP

#include "EMDragDrop.h"
class EMWinListViewControl;
class EMWinView;

#include "windows.h"

class MediaPoolView;

typedef pair<void*, EMDragDropClassType> DragDropPair;

class EMWinDragDrop : public EMDragDrop
{
public:
	EMWinDragDrop();
	~EMWinDragDrop();
	EMDragDropData* GetData();
	EMDragDropResult DoDragDrop(void* p_opSource, EMDragDropData* p_opData, EMDragDropClassType p_eSourceClassType);
	EMDragDropResult Drop(EMMouseOver p_eMouseOver){return EM_DRAG_DROP_OK;};
	EMDragDropResult Drop(EMMouseOver p_eMouseOver, EMWinListViewControl* p_opListViewTarget, EMWinView* p_opView);
	EMDragDropResult DragEnter(EMMouseOver p_eMouseOver);
	EMDragDropResult DragLeave();
	EMDragDropResult DragMove(EMMouseOver p_eMouseOver);
	bool IsActive();
//	static EMDragDrop* Instance();
	void ReassignSubProc();
	EMDragDropResult RegisterTarget(void* p_opTarget, EMDragDropClassType p_eTargetClassType);
	void Reset();

	void SetMediaPool(MediaPoolView* p_opMediaPoolView);

private:
	static LRESULT CALLBACK SubclassedWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo);

private:
//	static EMWinDragDrop* m_opInstance;
	WNDPROC m_opDefaultWindowProcedure;
	WNDPROC m_opMainDefaultWindowProcedure;
	WNDPROC m_opListViewDefaultWindowProcedure; // FIXME: Make all target windowproc in a map<pointer, WNDPROC
	WNDPROC m_opViewDefaultWindowProcedure;
	EMWinListViewControl* m_opListViewSource;
	EMWinListViewControl* m_opListViewTarget;
	EMDragDropData* m_opData;
	EMWinView* m_opViewTarget;
	EMDragDropClassType m_eSourceClassType;
	EMDragDropClassType m_eTargetClassType;
	list<DragDropPair> m_oClassType;

	MediaPoolView* m_opMediaPoolView;

	bool m_vNotDropped;
	bool m_vDragging;
	bool m_vDragOverSource;
	bool m_vDragOverTarget;
	bool m_vIsActive;
	bool m_vIsOverSource;
	bool m_vIsOverTarget;
};

#endif

#endif