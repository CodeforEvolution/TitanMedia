/*******************************************************
* This code is the copyrighted property of Elegant Media
*-------------------------------------------------------
* Creator: Johan Dunberg
* Component: Framework, GUI
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_TREE_VIEW_CONTROL
#define __EM_TREE_VIEW_CONTROL

#include "EMGUIComponent.h"
#include "EMGUIGlobals.h"

class EMListViewControl;

class EMTreeViewControl : public EMGUIComponent
{
public:
	virtual ~EMTreeViewControl();
//	virtual void AddItem(const char* p_opText) = 0;
//	virtual void AddSubItem(const char* p_opText, int p_vItemId, int p_vColumnt) = 0;
	virtual EMRect Bounds() = 0;
	virtual void DeleteAllItems() = 0;
	virtual void Enable(bool p_vEnable) = 0;
//	virtual string GetSelection() = 0;
	virtual EMListViewControl* GetListView() = 0;
	virtual void Invalidate(EMRect p_oRect) = 0;
	virtual bool IsEnabled() = 0;
	virtual void SetCommand(uint32 p_vCommand) = 0;
	virtual void SetLabel(const char* p_vpLabel) = 0;
	virtual void SetListView(EMListViewControl* p_vListViewControl) = 0;
	virtual void StopUpdate(); // Only call this method when shotdown is unavoidable (EM_QUIT_PHASE_FOUR)

protected:
	EMTreeViewControl();

protected:
	bool __m_vNoUpdate;
};

#endif
