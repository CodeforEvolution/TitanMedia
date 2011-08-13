/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __TRACK_WINDOW
#define __TRACK_WINDOW

#include "EMListener.h"
#include "EMMenuBar.h"
#include "EMProjectDataContainer.h"
#include "EMScrollbar.h"
#include "EMSlimButtonView.h"
#include "EMView.h"
#include "EMWindow.h"
#include "TrackInfo.h"
#include "TrackInfoButtonBar.h"
#include "TrackView.h"
//#include "TrackWindowStatusBar.h"
#include "TrackWindowTimeLine.h"
#include "TrackWindowToolBar.h"
#include "UtilityView.h"

#include <memory>

class EMListenerRepository;
class EMResizeTab;

class TrackWindow : public EMListener, public EMProjectDataContainer
{
public:
	TrackWindow();
	~TrackWindow();
	bool ClearData();
	void ExpandTimeLine();
	void ExpandTrackInfo();
	void ExpandUtilityView();
	TrackInfoButtonBar* GetButtonBar();
	EMMenuBar* GetMenuBar();
	TrackWindowTimeLine* GetTimeLine();
	TrackWindowToolBar* GetToolBar();
	TrackInfo* GetTrackInfo();
	TrackView* GetTrackView();
	UtilityView* GetUtilityView();
	EMWindow* GetWindow();
	void Init();
	bool IsDirty();
	void LayoutViews(bool p_vNeedFullLayout = false);
	bool LoadData(void* p_upProjectDataLoader);
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void PrepareToClearData();
	void ReadyForUse();
	bool SaveData(void* p_upProjectDataSaver);;
	
private:
	void BuildMenu();

private:
	auto_ptr<EMView> m_opWindowBackground;
	EMSlimButtonView m_oExpandButton;
	EMSlimButtonView m_oHorizontalZoomInButton;
	EMSlimButtonView m_oHorizontalZoomOutButton;
	auto_ptr<EMWindow> m_opWindow;
	auto_ptr<TrackInfoButtonBar> m_opButtonBar;
	auto_ptr<EMMenuBar> m_opMenuBar;
	EMResizeTab* m_opResizeTab;
	auto_ptr<EMScrollbar> m_opScrollbarH;
	auto_ptr<EMScrollbar> m_opScrollbarV;
//	auto_ptr<TrackWindowStatusBar> m_opStatusBar;
	auto_ptr<TrackWindowTimeLine> m_opTimeLine;
	auto_ptr<TrackWindowToolBar> m_opToolBar;
	auto_ptr<TrackInfo> m_opTrackInfo;
	auto_ptr<TrackView> m_opTrackView;
	UtilityView m_oUtilityView;
	EMSlimButtonView m_oVerticalZoomInButton;
	EMSlimButtonView m_oVerticalZoomOutButton;
	bool m_vIsDirty;
//	bool m_vNeedFullLayout;
};

#endif



/*
#include "EMMediaStructs.h"
#include "EMListener.h"

class EMBorder;
class EMButton;
class EMMenuBar;
class EMMovieWindow;
class EMScrollbar;
class EMTimeline;
class EMTimeView;
class EMTrackView;
class EMView;
class EMWindow;

class TrackWindow : public EMListener
{
public:
	TrackWindow();
	~TrackWindow();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);

private:
	void CreateButtons();
	void CreateCommands();
	void CreateMenu();

private:
	EMButton* m_opButtonDelete;
	EMButton* m_opButtonScaleDown;
	EMButton* m_opButtonScaleUp;
	EMButton* m_opButtonRewind;
	EMButton* m_opButtonPlay;
	EMButton* m_opButtonStop;
	EMView* m_opLogoView;
	EMMenuBar* m_opMenuBar;
	EMScrollbar* m_opScrollbarHorizontal;	
	EMScrollbar* m_opScrollbarVertical;
	EMTimeline* m_opTimeline;
	EMTimeView* m_opTimeView;
	EMBorder* m_opTrackViewBorder;
	EMWindow* m_opWindow;
	EMView* m_opWindowBackground;
};
*/

