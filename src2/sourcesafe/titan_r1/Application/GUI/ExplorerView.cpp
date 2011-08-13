#include "ExplorerView.h"

#include "EMApplication.h"
#include "EMDragDrop.h"
#include "EMFactory.h"
#include "EMListViewControl.h"
#include "EMMessages.h"
#include "EMMouseCursor.h"
#include "EMTreeViewControl.h"
#include "EMView.h"
#include "EMWindow.h"
#include "MediaPoolView.h"

ExplorerView* ExplorerView::m_opInstance = NULL;

ExplorerView::ExplorerView() :
//m_opDialogWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(400, 200, 400 + 500, 200 + 400), "Export Media", EM_WINDOW_DIALOG)), 
m_opView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 500, 400), false)),
m_opTreeViewControl(EMFactory::Instance() ->CreateTreeViewControl(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
																  250,
																  400 - EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM),
														   "ListViewTest", 0, 0, 0)),
m_opListViewControl(EMFactory::Instance() ->CreateListViewControl(EMRect(250/*EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE*/,
																  EM_METRICS_NATIVE_DIALOG_MARGIN_TOP,
																  500 - EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
																  400 - EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM),
														   "ListViewTest", 0, 0, 0, EM_LIST_VIEW_STYLE_LIST, false)),
m_vMiddle(150),
m_vMouseX(0),
m_vMouseY(0),
m_vMouseLeft(false),
m_vMouseMiddle(false),
m_vMouseRight(false),
m_vMouseDown(false),
m_vInit(false),
//m_opDragDrop(NULL),
m_opDragDropData(NULL)
{
}

ExplorerView::~ExplorerView()
{
	delete m_opTreeViewControl;
	delete m_opListViewControl;
	delete m_opView;
	m_opInstance = NULL;
}

void ExplorerView::DeleteAllTreeViewItems()
{
	m_opTreeViewControl -> DeleteAllItems();
	m_opListViewControl -> DeleteAllItems();
}

EMListViewControl* ExplorerView::GetListViewControl()
{
	return m_opListViewControl;
}

EMView* ExplorerView::GetView()
{
	return m_opView;
}

void ExplorerView::Hide()
{
}

void ExplorerView::Init()
{
	if(!m_vInit)
	{
		m_opTreeViewControl -> SetListView(m_opListViewControl);
		m_opListViewControl -> SetTreeView(m_opTreeViewControl);
		m_opView -> AddChild(m_opListViewControl);
		m_opView -> AddChild(m_opTreeViewControl);
		m_opView -> AddListener(this);
		m_opListViewControl -> AddListener(this);
		EMApplication::Instance() -> AddListener(this);
		m_vInit = true;
	}
}

ExplorerView* ExplorerView::Instance()
{
	if(m_opInstance == NULL)
		m_opInstance = EM_new ExplorerView();
	return m_opInstance;
}

void ExplorerView::LayoutViews()
{
	EMRect oRect(m_opView -> Bounds());

	if(m_vMiddle < 75)
		m_vMiddle = 75;
	if(m_vMiddle > m_opView -> Bounds().GetWidth() - 75)
		m_vMiddle = m_opView -> Bounds().GetWidth() - 75;
//	eo << "LayoutViews() m_vMiddle = " << m_vMiddle << ef;

	m_opTreeViewControl -> SetFrame(EMRect(0, 0, m_vMiddle - 3, oRect.m_vBottom));
	m_opListViewControl -> SetFrame(EMRect(m_vMiddle + 3, 0, oRect.m_vRight, oRect.m_vBottom));
}

bool ExplorerView::Lock()
{
	return m_opView -> LockWindow();
}

bool ExplorerView::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
	case EM_DRAW:
		if(p_opSender == m_opView)
		{
//			eo << "EM_DRAW: m_oBackgroundView" << ef;
			m_opView -> SetHighColor(EMColor(192, 192, 192, 255));
			m_opView -> FillRect(m_opView -> Bounds());
		}
		else
			eo << "EM_DRAW (unknown)" << ef;
		return true;
	case EM_VIEW_BOUNDS_CHANGED: // Most often due to the user resizing the window
		LayoutViews();
//		m_opView -> Invalidate(m_opView -> Bounds());
		return true;
	case EM_MOUSE_DOWN:
		eo << "EM_MOUSE_DOWN Explorer" << ef;
		m_vMouseDown = true;
		return true;
	case EM_MOUSE_MOVED:
//		eo << "EM_MOUSE_MOVED Explorer" << ef;
		if(p_opSender == m_opView)
		{
			m_opView -> GetMouse(m_vMouseX, m_vMouseY, m_vMouseLeft, m_vMouseMiddle, m_vMouseRight);
			if(m_vMouseX >= m_opTreeViewControl -> Frame().m_vRight && m_vMouseX <= m_opListViewControl -> Frame().m_vLeft)
			{
				m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_RESIZE_V_BAR);
			//	if(m_vMouseLeft)
			//		m_vMouseDown = true;
			}
			if(m_vMouseDown)
			{
				eo << "EM_MOUSE_MOVED Explorer moving" << ef;
				m_opView -> SetMouseCursor(EM_MOUSE_CURSOR_RESIZE_V_BAR);
				m_vMiddle = m_vMouseX;
				eo << "MessageReceived() m_vMiddle = " << m_vMiddle << ef;
				LayoutViews();
//				m_opView -> Invalidate(m_opView -> Bounds());
			}
			return true;
		}
		return false;
	case EM_MOUSE_UP:
		eo << "EM_MOUSE_UP Explorer" << ef;
		m_vMouseDown = false;
		return true;
	case EM_BEGIN_DRAG:
		eo << "ExplorerView -> EM_BEGIN_DRAG" << ef;
		if(p_opSender == m_opListViewControl)
		{
			//m_opDragDrop = EMWinDragDrop::Instance();
			if(m_opDragDropData != NULL)
				delete m_opDragDropData;
			m_opDragDropData = EM_new EMDragDropData;

			m_opDragDropData -> m_opString = m_opListViewControl -> GetSelection(true);
			EMDragDrop::Instance() -> DoDragDrop(m_opListViewControl, m_opDragDropData, EM_LIST_VIEW_CONTROL);
		}
		return true;
	case EM_QUIT_PHASE_FOUR: // When getting this message shutdown is unavoidable. Stop updating EMTree- and EMListViewControl.
		eo << "EM_QUIT_PHASE_FOUR" << ef;
		m_opTreeViewControl -> StopUpdate();
		m_opListViewControl -> StopUpdate();
		return true;
	}
	return false;
}

void ExplorerView::SetMainWindow(EMWindow* p_opMainWindow)
{
	m_opMainWindow = p_opMainWindow;
}

void ExplorerView::Show()
{
/*	if(m_opWindow -> Lock())
	{
		m_opWindow -> Show();
		m_opWindow -> Unlock();
	}
*/
}