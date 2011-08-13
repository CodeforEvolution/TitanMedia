/*******************************************************
* This code is the copyrighted property of Elegant Media
*-------------------------------------------------------
* Creator: Johan Dunberg
* Component: Application
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/
#include "EMGlobals.h"

#ifndef __EXPLORER_VIEW
#define __EXPLORER_VIEW

#include "EMListener.h"

class EMView;
class EMWindow;
class EMListViewControl;
class EMTreeViewControl;

//class EMDragDrop;
class EMDragDropData;

class ExplorerView : public EMListener
{
public:
	ExplorerView();
	~ExplorerView();
	void DeleteAllTreeViewItems();
	EMListViewControl* GetListViewControl();
	EMView* GetView();
	void Hide();
	void Init();
	static ExplorerView* Instance();
	void LayoutViews();
	bool Lock();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void SetMainWindow(EMWindow* p_opMainWindow);
	void Show();
	void Unlock();

private:
	static ExplorerView* m_opInstance;
	EMListViewControl* m_opListViewControl;
	EMWindow* m_opMainWindow;
	EMTreeViewControl* m_opTreeViewControl;
	EMView* m_opView;
	bool m_vInit;
	int32 m_vMiddle;
	bool m_vMouseDown;
	bool m_vMouseLeft;
	bool m_vMouseMiddle;
	bool m_vMouseRight;
	int32 m_vMouseX;
	int32 m_vMouseY;

//	EMDragDrop* m_opDragDrop;
	EMDragDropData* m_opDragDropData;
};

#endif