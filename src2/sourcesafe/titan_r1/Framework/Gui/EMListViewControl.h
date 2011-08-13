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

#ifndef __EM_LIST_VIEW_CONTROL
#define __EM_LIST_VIEW_CONTROL

#include "EMGUIComponent.h"
#include "EMGUIGlobals.h"
#include "EMListener.h"

const uint32 CHANGE_LIST_VIEW_CONTROL_STYLE_TO_ICON = 1;
const uint32 CHANGE_LIST_VIEW_CONTROL_STYLE_TO_SMALL_ICON = 2;
const uint32 CHANGE_LIST_VIEW_CONTROL_STYLE_TO_LIST = 3;
const uint32 CHANGE_LIST_VIEW_CONTROL_STYLE_TO_REPORT = 4;

class EMTreeViewControl;

class EMListViewControl : public EMGUIComponent, public EMListener
{
public:
	virtual ~EMListViewControl();
	virtual void AddColumn(char* p_opName, uint32 p_vWidth, EMAlignment p_eAlignment = EM_ALIGNMENT_LEFT) = 0;
	virtual void AddItem(const char* p_opText) = 0;
	virtual void AddSubItem(const char* p_opText, int p_vColumnt) = 0;
	virtual void AddSubItem(const char* p_opText, int p_vItemId, int p_vColumnt) = 0;
	virtual EMRect Bounds() = 0;
	virtual void ChangeSubItem(uint32 p_vItemID, uint16 p_vColumn, const char* p_vpChangeItTo) = 0;
	virtual void DeleteAllItems() = 0;
	virtual bool DeleteItem(int p_vItemID) = 0;
	virtual void DeleteSelectedItem() = 0;
	virtual void Enable(bool p_vEnable) = 0;
	virtual	void EnableHoverSelect(bool p_vEnable, int32 p_vHoverTime = -1) = 0; // -1 = Default hovertime is set.
	virtual int32 GetItem(const char* p_vpUniqueStringIndex) = 0;
	virtual int GetItemCount() = 0;
	virtual string GetSelection(bool FullPath = false) = 0;
	virtual uint32 GetSelectionID() = 0;
	virtual char* GetSelectionSubItem(int16 p_vColumn) = 0; //If p_vColumn == -1 UniqueIndexColumn is used;
	virtual char* GetSubItem(uint32 p_vItem, int16 p_vColumn) = 0; //If p_vColumn == -1 UniqueIndexColumn is used;
	virtual EMTreeViewControl* GetTreeView() = 0;
	virtual void Invalidate(const EMRect p_oRect) = 0;
	virtual bool IsDragEnabled() = 0;
	virtual bool IsEnabled() = 0;
	virtual void Scroll(int32 p_vItemID) = 0; // If p_vItemID == -1 last item is selected and will only scroll if PrevItem bottom is out of bounds
	virtual void SetCommand(uint32 p_vCommand) = 0;
	virtual void SetLabel(const char* p_vpLabel) = 0;
	virtual void SetTreeView(EMTreeViewControl* p_vTreeView) = 0;
	virtual void SetUniqueIndexColumn(int16 p_vUniqueIndexColumn) = 0; // If set to -1 sort will not work and no unique index column will be set
	virtual void StopUpdate(); // Only call this method when shotdown is unavoidable (EM_QUIT_PHASE_FOUR)
	virtual void SwitchView(EMListViewStyle p_eListViewStyle) = 0;


protected:
	EMListViewControl();

protected:
	bool __m_vNoUpdate;
};

#endif
