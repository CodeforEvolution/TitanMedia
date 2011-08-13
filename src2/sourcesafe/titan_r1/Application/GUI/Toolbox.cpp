#include "Toolbox.h"

#include "EMBitmap.h"
#include "EMFactory.h"
#include "EMGUIGlobals.h"
#include "EMMessages.h"
#include "EMResourceRepository.h"
#include "EMSettingsRepository.h"
#include "EMToolTip.h"
#include "EMView.h"
#include "EMWindow.h"
#include "GUIGlobals.h"
#include "ResourceIDs.h"
#include "SettingIDs.h"
#include "TrackWindowStatusBar.h"

const int TOOLBOX_ICON_SIZE = 20;
const int TOOLBOX_SPACING = 1;
const int TOOLBOX_FRAME = 3;
const int TOOLBOX_COLUMNS = 3;
const int TOOLBOX_ROWS = 3;

Toolbox::Toolbox() :
m_opView(EMFactory::Instance() -> CreateView(EMRect(0, 0, TOOLBOX_ICON_SIZE * 3 + TOOLBOX_SPACING * 2 + TOOLBOX_FRAME * 2 - 1,
									TOOLBOX_ICON_SIZE * 3 + TOOLBOX_SPACING * 2 + TOOLBOX_FRAME * 2 - 1), EM_FOLLOW_LEFT_RIGHT | EM_FOLLOW_TOP_BOTTOM)),
m_opToolTip(EMFactory::Instance() -> CreateToolTip(EMRect(0, 0, TOOLBOX_ICON_SIZE * 3 + TOOLBOX_SPACING * 2 + TOOLBOX_FRAME * 2 - 1,
									TOOLBOX_ICON_SIZE * 3 + TOOLBOX_SPACING * 2 + TOOLBOX_FRAME * 2 - 1), "Gronk!", m_opView, true)),
m_opWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(0, 0, TOOLBOX_ICON_SIZE * 3 + TOOLBOX_SPACING * 2 + TOOLBOX_FRAME * 2 - 1, TOOLBOX_ICON_SIZE * 3 + TOOLBOX_SPACING * 2 + TOOLBOX_FRAME * 2 - 1), "Toolbox Window", EM_WINDOW_POPUP)),
m_vCurrentIcon(-1),
m_vIconsLoaded(false),
m_vOldIcon(-1)
{
	m_opWindow -> AddChild(m_opView);
	m_opView -> AddListener(this);
// Why are these needed in BeOS? (locks the app otherwise when showing the window later on)
//m_opWindow -> Show();
//m_opWindow -> Hide();
}

Toolbox::~Toolbox()
{
	delete m_opToolTip;
	delete m_opView;
	delete m_opWindow;
}

void Toolbox::Draw(EMRect p_oUpdateRect, bool p_vRedrawChangedOnly)
{
	// First make sure that the bitmaps used are loaded (since the Toolbox are briefly displayed 
	// at application start when they aren't loaded)
	if(!m_vIconsLoaded)
		return;
	int32 vX;
	int32 vY;
	bool vButtonOne;
	bool vButtonTwo;
	bool vButtonThree;
	m_opView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
	if(p_vRedrawChangedOnly)
	{
		if(m_vCurrentIcon == 0)
			m_opView -> DrawBitmap(m_oIcons[9], TOOLBOX_FRAME, TOOLBOX_FRAME);
		else if(m_vOldIcon == 0)
			m_opView -> DrawBitmap(m_oIcons[0], TOOLBOX_FRAME, TOOLBOX_FRAME);
		if(m_vCurrentIcon == 1)
			m_opView -> DrawBitmap(m_oIcons[10], TOOLBOX_FRAME + TOOLBOX_ICON_SIZE + TOOLBOX_SPACING, TOOLBOX_FRAME);
		else if(m_vOldIcon == 1)
			m_opView -> DrawBitmap(m_oIcons[1], TOOLBOX_FRAME + TOOLBOX_ICON_SIZE + TOOLBOX_SPACING, TOOLBOX_FRAME);
		if(m_vCurrentIcon == 2)
			m_opView -> DrawBitmap(m_oIcons[11], TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 2 + TOOLBOX_SPACING * 2, TOOLBOX_FRAME);
		else if(m_vOldIcon == 2)
			m_opView -> DrawBitmap(m_oIcons[2], TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 2 + TOOLBOX_SPACING * 2, TOOLBOX_FRAME);
		if(m_vCurrentIcon == 3)
			m_opView -> DrawBitmap(m_oIcons[12], TOOLBOX_FRAME, TOOLBOX_FRAME + TOOLBOX_ICON_SIZE + TOOLBOX_SPACING);
		else if(m_vOldIcon == 3)
			m_opView -> DrawBitmap(m_oIcons[3], TOOLBOX_FRAME, TOOLBOX_FRAME + TOOLBOX_ICON_SIZE + TOOLBOX_SPACING);
		if(m_vCurrentIcon == 4)
			m_opView -> DrawBitmap(m_oIcons[13], TOOLBOX_FRAME + TOOLBOX_ICON_SIZE + TOOLBOX_SPACING, TOOLBOX_FRAME + TOOLBOX_ICON_SIZE + TOOLBOX_SPACING);
		else if(m_vOldIcon == 4)
			m_opView -> DrawBitmap(m_oIcons[4], TOOLBOX_FRAME + TOOLBOX_ICON_SIZE + TOOLBOX_SPACING, TOOLBOX_FRAME + TOOLBOX_ICON_SIZE + TOOLBOX_SPACING);
		if(m_vCurrentIcon == 5)
			m_opView -> DrawBitmap(m_oIcons[14], TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 2 + TOOLBOX_SPACING * 2, TOOLBOX_FRAME + TOOLBOX_ICON_SIZE + TOOLBOX_SPACING);
		else if(m_vOldIcon == 5)
			m_opView -> DrawBitmap(m_oIcons[5], TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 2 + TOOLBOX_SPACING * 2, TOOLBOX_FRAME + TOOLBOX_ICON_SIZE + TOOLBOX_SPACING);
		if(m_vCurrentIcon == 6)
			m_opView -> DrawBitmap(m_oIcons[15], TOOLBOX_FRAME, TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 2 + TOOLBOX_SPACING * 2);
		else if(m_vOldIcon == 6)
			m_opView -> DrawBitmap(m_oIcons[6], TOOLBOX_FRAME, TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 2 + TOOLBOX_SPACING * 2);
		if(m_vCurrentIcon == 7)
			m_opView -> DrawBitmap(m_oIcons[16], TOOLBOX_FRAME + TOOLBOX_ICON_SIZE + TOOLBOX_SPACING, TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 2 + TOOLBOX_SPACING * 2);
		else if(m_vOldIcon == 7)
			m_opView -> DrawBitmap(m_oIcons[7], TOOLBOX_FRAME + TOOLBOX_ICON_SIZE + TOOLBOX_SPACING, TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 2 + TOOLBOX_SPACING * 2);
		if(m_vCurrentIcon == 8)
			m_opView -> DrawBitmap(m_oIcons[17], TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 2 + TOOLBOX_SPACING * 2, TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 2 + TOOLBOX_SPACING * 2);
		else if(m_vOldIcon == 8)
			m_opView -> DrawBitmap(m_oIcons[8], TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 2 + TOOLBOX_SPACING * 2, TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 2 + TOOLBOX_SPACING * 2);
	}
	else
	{
		m_opView -> DrawBitmap(m_opBitmapFrame, 0, 0);
		if(m_vCurrentIcon == 0)
			m_opView -> DrawBitmap(m_oIcons[9], TOOLBOX_FRAME, TOOLBOX_FRAME);
		else
			m_opView -> DrawBitmap(m_oIcons[0], TOOLBOX_FRAME, TOOLBOX_FRAME);
		if(m_vCurrentIcon == 1)
			m_opView -> DrawBitmap(m_oIcons[10], TOOLBOX_FRAME + TOOLBOX_ICON_SIZE + TOOLBOX_SPACING, TOOLBOX_FRAME);
		else
			m_opView -> DrawBitmap(m_oIcons[1], TOOLBOX_FRAME + TOOLBOX_ICON_SIZE + TOOLBOX_SPACING, TOOLBOX_FRAME);
		if(m_vCurrentIcon == 2)
			m_opView -> DrawBitmap(m_oIcons[11], TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 2 + TOOLBOX_SPACING * 2, TOOLBOX_FRAME);
		else
			m_opView -> DrawBitmap(m_oIcons[2], TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 2 + TOOLBOX_SPACING * 2, TOOLBOX_FRAME);
		if(m_vCurrentIcon == 3)
			m_opView -> DrawBitmap(m_oIcons[12], TOOLBOX_FRAME, TOOLBOX_FRAME + TOOLBOX_ICON_SIZE + TOOLBOX_SPACING);
		else
			m_opView -> DrawBitmap(m_oIcons[3], TOOLBOX_FRAME, TOOLBOX_FRAME + TOOLBOX_ICON_SIZE + TOOLBOX_SPACING);
		if(m_vCurrentIcon == 4)
			m_opView -> DrawBitmap(m_oIcons[13], TOOLBOX_FRAME + TOOLBOX_ICON_SIZE + TOOLBOX_SPACING, TOOLBOX_FRAME + TOOLBOX_ICON_SIZE + TOOLBOX_SPACING);
		else
			m_opView -> DrawBitmap(m_oIcons[4], TOOLBOX_FRAME + TOOLBOX_ICON_SIZE + TOOLBOX_SPACING, TOOLBOX_FRAME + TOOLBOX_ICON_SIZE + TOOLBOX_SPACING);
		if(m_vCurrentIcon == 5)
			m_opView -> DrawBitmap(m_oIcons[14], TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 2 + TOOLBOX_SPACING * 2, TOOLBOX_FRAME + TOOLBOX_ICON_SIZE + TOOLBOX_SPACING);
		else
			m_opView -> DrawBitmap(m_oIcons[5], TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 2 + TOOLBOX_SPACING * 2, TOOLBOX_FRAME + TOOLBOX_ICON_SIZE + TOOLBOX_SPACING);
		if(m_vCurrentIcon == 6)
			m_opView -> DrawBitmap(m_oIcons[15], TOOLBOX_FRAME, TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 2 + TOOLBOX_SPACING * 2);
		else
			m_opView -> DrawBitmap(m_oIcons[6], TOOLBOX_FRAME, TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 2 + TOOLBOX_SPACING * 2);
		if(m_vCurrentIcon == 7)
			m_opView -> DrawBitmap(m_oIcons[16], TOOLBOX_FRAME + TOOLBOX_ICON_SIZE + TOOLBOX_SPACING, TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 2 + TOOLBOX_SPACING * 2);
		else
			m_opView -> DrawBitmap(m_oIcons[7], TOOLBOX_FRAME + TOOLBOX_ICON_SIZE + TOOLBOX_SPACING, TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 2 + TOOLBOX_SPACING * 2);
		if(m_vCurrentIcon == 8)
			m_opView -> DrawBitmap(m_oIcons[17], TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 2 + TOOLBOX_SPACING * 2, TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 2 + TOOLBOX_SPACING * 2);
		else
			m_opView -> DrawBitmap(m_oIcons[8], TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 2 + TOOLBOX_SPACING * 2, TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 2 + TOOLBOX_SPACING * 2);
	}
}

int Toolbox::GetCell(int32 p_vX, int32 p_vY)
{
	if(p_vX < TOOLBOX_FRAME || p_vX > TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 3 + TOOLBOX_SPACING * 2 ||
		p_vY < TOOLBOX_FRAME || p_vY > TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 3 + TOOLBOX_SPACING * 2)
		return -1;
		
	if(p_vX < TOOLBOX_FRAME + TOOLBOX_ICON_SIZE)
	{
		if(p_vY < TOOLBOX_FRAME + TOOLBOX_ICON_SIZE)
			return 0;
		if(p_vY < TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 2)
			return 3;
		return 6;
	}
	else if(p_vX < TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 2)
	{
		if(p_vY < TOOLBOX_FRAME + TOOLBOX_ICON_SIZE)
			return 1;
		if(p_vY < TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 2)
			return 4;
		return 7;
	}
	else
	{
		if(p_vY < TOOLBOX_FRAME + TOOLBOX_ICON_SIZE)
			return 2;
		if(p_vY < TOOLBOX_FRAME + TOOLBOX_ICON_SIZE * 2)
			return 5;
		return 8;
	}
}

void Toolbox::Init()
{
	m_opBitmapFrame = EMResourceRepository::GetBitmap(RES_TK_FRAME);
	// The list will contain all possible icons in the toolbox
	m_oIcons.push_back(EMResourceRepository::GetBitmap(RES_TK_EMPTY));
	m_oIcons.push_back(EMResourceRepository::GetBitmap(RES_TK_CUT_PASTE));
	m_oIcons.push_back(EMResourceRepository::GetBitmap(RES_TK_RENAME));
	m_oIcons.push_back(EMResourceRepository::GetBitmap(RES_TK_RESIZE));
	m_oIcons.push_back(EMResourceRepository::GetBitmap(RES_TK_SELECT));
	m_oIcons.push_back(EMResourceRepository::GetBitmap(RES_TK_ERASE));
	m_oIcons.push_back(EMResourceRepository::GetBitmap(RES_TK_EMPTY));//RES_TK_MUTE));
	m_oIcons.push_back(EMResourceRepository::GetBitmap(RES_TK_DRAW));
	m_oIcons.push_back(EMResourceRepository::GetBitmap(RES_TK_EMPTY));

	m_oIcons.push_back(EMResourceRepository::GetBitmap(RES_TK_EMPTY_SEL));
	m_oIcons.push_back(EMResourceRepository::GetBitmap(RES_TK_CUT_PASTE_SEL));
	m_oIcons.push_back(EMResourceRepository::GetBitmap(RES_TK_RENAME_SEL));
	m_oIcons.push_back(EMResourceRepository::GetBitmap(RES_TK_RESIZE_SEL));
	m_oIcons.push_back(EMResourceRepository::GetBitmap(RES_TK_SELECT_SEL));
	m_oIcons.push_back(EMResourceRepository::GetBitmap(RES_TK_ERASE_SEL));
	m_oIcons.push_back(EMResourceRepository::GetBitmap(RES_TK_EMPTY_SEL));//RES_TK_MUTE_SEL));
	m_oIcons.push_back(EMResourceRepository::GetBitmap(RES_TK_DRAW_SEL));
	m_oIcons.push_back(EMResourceRepository::GetBitmap(RES_TK_EMPTY_SEL));
	m_vIconsLoaded = true;
}

bool Toolbox::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	int32 vX;
	int32 vY;
	bool vButtonOne;
	bool vButtonTwo;
	bool vButtonThree;
	
	switch(p_vMessage)
	{
	case EM_MOUSE_UP:
		m_opView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		MouseUp(vX, vY);
		return true;
	case EM_MOUSE_MOVED:
		m_opView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		MouseMoved(vX, vY);
		return true;
	case EM_DRAW:
		m_opView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		m_vCurrentIcon = GetCell(vX, vY);
		Draw(m_opView -> GetClippingRect());
;//cout_commented_out_4_release << "Toolbox::EM_DRAW: cell: " << m_vCurrentIcon << endl;
		return true;
	}
	return false;
}

void Toolbox::MouseMoved(int32 p_vX, int32 p_vY)
{
	int vNewIcon = GetCell(p_vX, p_vY);
	if(vNewIcon != m_vCurrentIcon)
	{
		switch(vNewIcon)
		{
		case 0:
			m_opToolTip -> SetLabel("");
			break;
		case 1:
			m_opToolTip -> SetLabel("Cut/Paste tool");
			break;
		case 2:
			m_opToolTip -> SetLabel("Rename tool");
			break;
		case 3:
			m_opToolTip -> SetLabel("Resize tool");
			break;
		case 4:
			m_opToolTip -> SetLabel("Select tool");
			break;
		case 5:
			m_opToolTip -> SetLabel("Erase tool");
			break;
		case 6:
			m_opToolTip -> SetLabel("");
			break;
		case 7:
			m_opToolTip -> SetLabel("Draw tool");
			break;
		case 8:
			m_opToolTip -> SetLabel("");
			break;
		}

		m_vOldIcon = m_vCurrentIcon;
		m_vCurrentIcon = vNewIcon;
		m_opView -> BeginPaint();
		Draw(EMRect(), true);
		m_opView -> EndPaint();
		/*
		if(m_opParentView -> LockWindow())
		{
			switch(vNewIcon)
			{
			case 0:
				TrackWindowStatusBar::Instance() -> WriteMessage(string(""));//Scrub tool"));
				break;		
			case 1:
				TrackWindowStatusBar::Instance() -> WriteMessage(string("Cut/Paste tool"));
				break;		
			case 2:
				TrackWindowStatusBar::Instance() -> WriteMessage(string("Rename tool"));
				break;		
			case 3:
				TrackWindowStatusBar::Instance() -> WriteMessage(string("Resize tool"));
				break;		
			case 4:
				TrackWindowStatusBar::Instance() -> WriteMessage(string("Select tool"));
				break;		
			case 5:
				TrackWindowStatusBar::Instance() -> WriteMessage(string("Erase tool"));
				break;		
			case 6:
				TrackWindowStatusBar::Instance() -> WriteMessage(string(""));//Mute tool"));
				break;		
			case 7:
				TrackWindowStatusBar::Instance() -> WriteMessage(string("Draw tool"));
				break;		
			default:
				TrackWindowStatusBar::Instance() -> WriteMessage(string(""));
				break;
			}
			m_opParentView -> UnlockWindow();
		}
		*/
	}
}

void Toolbox::MouseUp(int32 p_vX, int32 p_vY)
{
	if(m_opParentView -> LockWindow())
	{
		int vMode;
		switch(m_vCurrentIcon)//GetCell(p_vX, p_vY))
		{
		case 0:
//			TrackWindowStatusBar::Instance() -> WriteMessage(string("Scrub tool selected"));
			vMode = TOOLBOX_SCRUB;
			break;		
		case 1:
			TrackWindowStatusBar::Instance() -> WriteMessage(string("Cut/Paste tool selected"));
			vMode = TOOLBOX_CUT_PASTE;
			break;		
		case 2:
			TrackWindowStatusBar::Instance() -> WriteMessage(string("Rename tool selected"));
			vMode = TOOLBOX_RENAME;
			break;		
		case 3:
			TrackWindowStatusBar::Instance() -> WriteMessage(string("Resize tool selected"));
			vMode = TOOLBOX_RESIZE;
			break;		
		case 4:
			TrackWindowStatusBar::Instance() -> WriteMessage(string("Select tool selected"));
			vMode = TOOLBOX_SELECT;
			break;		
		case 5:
			TrackWindowStatusBar::Instance() -> WriteMessage(string("Erase tool selected"));
			vMode = TOOLBOX_ERASE;
			break;		
		case 6:
//			TrackWindowStatusBar::Instance() -> WriteMessage(string("Mute tool selected"));
//			vMode = TOOLBOX_MUTE;
			break;		
		case 7:
			TrackWindowStatusBar::Instance() -> WriteMessage(string("Draw tool selected"));
			vMode = TOOLBOX_DRAW;
			break;		
		default:
//			TrackWindowStatusBar::Instance() -> WriteMessage(string("Unknown tool selected"));
//			EMSettingsRepository::Instance() -> SetSetting(SETTING_TOOLBOX_MODE, EM_SETTING_INT, EM_new int(TOOLBOX_UNKNOWN));
			m_opWindow -> Hide();
			m_opParentView -> UnlockWindow();
			return;
		}
		EMSettingsRepository::Instance() -> SetSetting(SETTING_TOOLBOX_MODE, EM_SETTING_INT, &vMode);
		m_opParentView -> UnlockWindow();
	}
//	if(m_opWindow -> Lock())
	{
		m_opView -> ReceiveAllMouseEvents(false);
		m_opWindow -> Hide();
//		m_opWindow -> Unlock();
	}
}

void Toolbox::Show(EMView* p_opParentView)
{
	if(m_opWindow -> Lock())
	{
		int32 vX;
		int32 vY;
		bool vButtonOne;
		bool vButtonTwo;
		bool vButtonThree;
		p_opParentView -> GetMouse(vX, vY, vButtonOne, vButtonTwo, vButtonThree);
		p_opParentView -> ConvertToScreenCoordinates(vX, vY);
		m_opParentView = p_opParentView;
		m_opWindow -> MoveTo(vX + (m_opWindow -> Frame()).m_vLeft - ((m_opWindow -> Bounds()).GetWidth() / 2), 
						vY + (m_opWindow -> Frame()).m_vTop - ((m_opWindow -> Bounds()).GetHeight() / 2));
		m_opWindow -> Show();
		m_opView -> ReceiveAllMouseEvents(true);
		m_opWindow -> Unlock();
	}
}