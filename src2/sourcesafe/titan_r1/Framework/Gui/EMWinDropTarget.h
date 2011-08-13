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

#ifndef __EM_WIN_DROP_TARGET
#define __EM_WIN_DROP_TARGET

#include <shlobj.h>

enum Target
{
//	EXPLORER_LIST_VIEW_CONTROL,
	MEDIA_POOL_LIST_VIEW_CONTROL,
	LIST_VIEW_CONTROL,
	VIEW
};

class EMWinListViewControl;
class EMWinView;
class MediaPoolView;

class FAR EMWinDropTarget : public IDropTarget
{
public:
	EMWinDropTarget(void* p_opTargetClass, Target p_eTarget);
	~EMWinDropTarget();

	STDMETHOD (DragEnter)( 
            /* [unique][in] */ IDataObject *pDataObj,
            /* [in] */ DWORD grfKeyState,
            /* [in] */ POINTL pt,
            /* [out][in] */ DWORD*pdwEffect);
	STDMETHOD (DragOver)( 
            /* [in] */ DWORD grfKeyState,
            /* [in] */ POINTL pt,
            /* [out][in] */ DWORD *pdwEffect);
	STDMETHOD (DragLeave)( void);
	STDMETHOD (Drop)( 
            /* [unique][in] */ IDataObject *pDataObj,
            /* [in] */ DWORD grfKeyState,
            /* [in] */ POINTL pt,
            /* [out][in] */ DWORD *pdwEffect);
	STDMETHOD(QueryInterface)(REFIID p_vReferensInterfaceID, void FAR* FAR* p_oppObject);
	STDMETHOD_(ULONG, AddRef)(void);
    STDMETHOD_(ULONG, Release)(void);

private:
	bool m_vDrop;
	ULONG m_vRefs;
	long m_vOleInitializeResult;
	HCURSOR m_vCursorHandle;

	EMWinListViewControl* m_opListViewControl;
	EMWinView* m_opView;
	static MediaPoolView* m_opMediaPoolView;
	Target m_eTarget;

};

#endif

#endif