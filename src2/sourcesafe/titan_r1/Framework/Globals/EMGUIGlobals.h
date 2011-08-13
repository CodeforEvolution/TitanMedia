/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_GUI_GLOBALS
#define __EM_GUI_GLOBALS

class EMGUIComponent;

// Here follows defines for most (all?) of the colors used within the GUI
// Text
#define EM_COLOR_TEXT EMColor(0, 0, 0, 170)
#define EM_COLOR_TEXT_BACKGROUND EMColor(255, 255, 255, 255)
// Windows and views

#define EM_COLOR_VIEW_BACKGROUND EMColor(150, 150, 200, 255)
#define EM_COLOR_VIEW_BACKGROUND_ULTRA_HIGHLIGHT EMColor(255, 255, 255, 255)
#define EM_COLOR_VIEW_BACKGROUND_HIGHLIGHT EMColor(200, 200, 250, 255)
#define EM_COLOR_VIEW_BACKGROUND_SHADOW EMColor(100, 100, 150, 255)
/*
#define EM_COLOR_VIEW_BACKGROUND EMColor(212, 208, 200, 255)
#define EM_COLOR_VIEW_BACKGROUND_ULTRA_HIGHLIGHT EMColor(255, 255, 255, 255)
#define EM_COLOR_VIEW_BACKGROUND_HIGHLIGHT EMColor(230, 230, 230, 255)
#define EM_COLOR_VIEW_BACKGROUND_SHADOW EMColor(128, 128, 128, 255)
*/
#define EM_COLOR_WINDOW_BACKGROUND EMColor(200, 200, 200, 255)
// TrackWindow
#define EM_COLOR_ODD_TRACK EMColor(200, 200, 200, 255)
#define EM_COLOR_EVEN_TRACK EMColor(230, 230, 230, 255)
#define EM_COLOR_SELECTED_CLIPS EMColor(0, 0, 0, 255)
// EMSlimButton
#define EM_COLOR_BUTTON_BORDER EMColor(0, 0, 0, 255)
#define EM_COLOR_BUTTON_DEFAULT_BACKGROUND EMColor(230, 230, 230, 255)
#define EM_COLOR_BUTTON_BACKGROUND_DISABLED EMColor(200, 200, 200, 255)
#define EM_COLOR_BUTTON_TEXT_DISABLED EMColor(100, 100, 100, 255)
#define EM_COLOR_BUTTON_TEXT_ENABLED EMColor(0, 0, 0, 255)
#define EM_COLOR_BUTTON_DEFAULT_ON EMColor(150, 255, 150, 255)
#define EM_COLOR_BUTTON_DEFAULT_OFF EMColor(200, 200, 250, 255)
// EMSlimValueBox
#define EM_COLOR_VALUE_BOX_BORDER EMColor(0, 0, 0, 255)
#define EM_COLOR_VALUE_BOX_BACKGROUND EMColor(200, 200, 250, 255)
#define EM_COLOR_VALUE_BOX_BACKGROUND_DISABLED EMColor(200, 200, 200, 255)
#define EM_COLOR_VALUE_BOX_TEXT EMColor(0, 0, 0, 255)
#define EM_COLOR_VALUE_BOX_TEXT_DISABLED EMColor(100, 100, 100, 255)
#define EM_COLOR_VALUE_BOX_MAX_LIMIT_MARK EMColor(150, 150, 190, 255)
#define EM_COLOR_VALUE_BOX_MIN_LIMIT_MARK EMColor(150, 150, 190, 255)
#define EM_COLOR_VALUE_CHANGING EMColor(0, 0, 255, 255)
// Misc
//#define EM_COLOR_SELECTION_BOX EMColor(0, 0, 0, 255)
#define EM_COLOR_INVERTED_DRAWING EMColor(0, 0, 0, 255)

// Here follows some of the standard GUI metrics used
#define EM_METRICS_SLIM_BUTTON_HEIGHT 14
#define EM_METRICS_TINY_SLIM_BUTTON_WIDTH 14
#define EM_METRICS_SMALL_SLIM_BUTTON_WIDTH 22
#define EM_METRICS_MEDIUM_SLIM_BUTTON_WIDTH 44
#define EM_METRICS_LARGE_SLIM_BUTTON_WIDTH 88
#define EM_METRICS_SCROLLBAR_HEIGHT 14 // Correct value? 
#define EM_METRICS_SLIM_EDIT_LABEL_HEIGHT 14
#define EM_METRICS_SIGNAL_METER_HEIGHT 14
#define EM_METRICS_LABEL_HEIGHT 12

const int32 MAX_VIEW_PIXEL_LIMIT = 0x7FFFF000;//0x7FFFFFFF; // The "unlimited" width or height of a view
const int MAX_LABEL_CHARACTER_SIZE = 30;

#ifdef PLATFORM_WINDOWS
// Here follows some of the standard Windows GUI metrics used
#define EM_METRICS_NATIVE_BUTTON_HEIGHT 22
#define EM_METRICS_NATIVE_BUTTON_WIDTH 74
#define EM_METRICS_NATIVE_BUTTON_SPACE 6
#define EM_METRICS_NATIVE_CHECKBOX_HEIGHT 15
#define EM_METRICS_NATIVE_RADIO_BUTTON_HEIGHT 15
#define EM_METRICS_NATIVE_LABEL_HEIGHT 15
#define EM_METRICS_NATIVE_TEXT_CONTROL_HEIGHT 19
#define EM_METRICS_NATIVE_SCROLLBAR_HEIGHT 15
#define EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE 10
#define EM_METRICS_NATIVE_DIALOG_MARGIN_TOP 11
#define EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM 11
#define EM_METRICS_NATIVE_PROGRESS_BAR_HEIGHT 22
#define EM_METRICS_NATIVE_TRACKBAR_CONTROL_HEIGHT 20
// The margins for GroupBox in Windows
#define EM_METRICS_NATIVE_GROUP_BOX_MARGIN_SIDE 7
#define EM_METRICS_NATIVE_GROUP_BOX_MARGIN_TOP 15
#define EM_METRICS_NATIVE_GROUP_BOX_MARGIN_B0TTOM 8
// The margins for TabControl in Windows
#define EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_SIDE 5
#define EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_TOP 6
#define EM_METRICS_NATIVE_TAB_CONTROL_MARGIN_B0TTOM 6

#endif

// Why must I have GUI_SERVICE_THREAD_NAME as a define instead of a const 
// (linker complains about multiple declarations)?
//const char* GUI_SERVICE_THREAD_NAME = "GUI Service Thread";
#define GUI_SERVICE_THREAD_NAME "GUI Service Thread"
const int32 GUI_SERVICE_THREAD_SLEEP_TIME = 25000; // 25/100th of a second

// Max length for text in a TextControl
#define EM_MAX_STRING_BUFFER_SIZE 255

class EMKey
{
public:
	EMGUIComponent* m_opOrigin;
	char m_vKey;
	bool m_vKeyStatus;
	uint32 m_vModifiers;
};

enum EMZoomScale
{
	EM_SMALLEST,
	EM_MICROSCOPIC,
	EM_TINY,
	EM_SMALL,
	EM_NORMAL,
	EM_LARGE,
	EM_HUGE,
	EM_COLOSSAL,
	EM_LARGEST
};

enum EMOrientation
{
	EM_VERTICAL,
	EM_HORIZONTAL
};

enum EMBorderStyle
{
	EM_BORDER_PLAIN,
	EM_BORDER_PLAIN_SHADOWED,
	EM_BORDER_BEVELED,
	EM_BORDER_NONE
};
/*
enum EMSnap
{
	EM_SNAP_OFF,
	EM_SNAP_TIME,
	EM_SNAP_CLIP
};
*/
enum EMFont
{
	EM_PLAIN,
	EM_BOLD,
	EM_FIXED
};

enum EMWindowType
{
	EM_WINDOW_NORMAL,
	EM_WINDOW_NUDGE, // Like normal but with a resize "square" in one corner
	EM_WINDOW_UNRESIZABLE, // Like normal window but cannot be resized
	EM_WINDOW_TOOLBOX,
	EM_WINDOW_POPUP,
	EM_WINDOW_POPUP_BORDER,
	EM_WINDOW_DIALOG,
	EM_WINDOW_DIALOG_NONMODAL,
	EM_WINDOW_SPLASH,
	EM_WINDOW_UNKNOWN
};

enum EMResizingFlags
{
	EM_FOLLOW_NONE = 0x0,
	EM_FOLLOW_LEFT = 0x00000001,
	EM_FOLLOW_RIGHT = 0x00000002,
	EM_FOLLOW_H_CENTER = 0x00000004,
	EM_FOLLOW_TOP = 0x00000008,
	EM_FOLLOW_BOTTOM = 0x00000010,
	EM_FOLLOW_V_CENTER = 0x00000020,
	EM_FOLLOW_LEFT_RIGHT = 0x00000040,
	EM_FOLLOW_TOP_BOTTOM = 0x00000080
};

enum EMDrawingMode 
{
	EM_OP_COPY,
	EM_OP_ALPHA,
	EM_OP_INVERT,
	EM_OP_UNKNOWN
};

class EMColor
{
public:
	EMColor()
	{
		m_vRed = 0;
		m_vGreen = 0;
		m_vBlue = 0;
		m_vAlpha = 0;
	}
	EMColor(uint8 p_vRed, uint8 p_vGreen, uint8 p_vBlue, uint8 p_vAlpha)
	{
		m_vRed = p_vRed;
		m_vGreen = p_vGreen;
		m_vBlue = p_vBlue;
		m_vAlpha = p_vAlpha;
	}

public:
	uint8 m_vRed;
	uint8 m_vGreen;
	uint8 m_vBlue;
	uint8 m_vAlpha;
};

enum EMColorSpace
{
	EM_LE_GRAY1 = 0x0001,
	EM_LE_GRAY8 = 0x0002,
	EM_LE_CMAP8 = 0x0004,
	EM_LE_RGB15 = 0x0010,
	EM_LE_RGBA15 = 0x2010,
	EM_LE_16 = 0x0005, // Little endian, 16 bit color
	EM_LE_32 = 0x0008,
	EM_COLORSPACE_UNKNOWN
};

enum EMEditMode
{
	EM_TRACK_EDIT_MODE,
	EM_CLIP_EDIT_MODE,
	EM_FX_EDIT_MODE
};

enum EMComponentType
{
	EM_COMPONENT_WINDOW,
	EM_COMPONENT_VIEW,
	EM_COMPONENT_SCROLLBAR
};

enum EMAlignment
{
	EM_ALIGNMENT_LEFT,
	EM_ALIGNMENT_CENTER,
	EM_ALIGNMENT_RIGHT
};

enum EMListViewStyle
{
	EM_LIST_VIEW_STYLE_ICON,
	EM_LIST_VIEW_STYLE_SMALL_ICON,
	EM_LIST_VIEW_STYLE_LIST,
	EM_LIST_VIEW_STYLE_REPORT
};

enum EMTimeDisplayMode
{
	EM_SMPTE,
	EM_MEASURES
};

enum EMTrackbarTickStyle
{
	EM_NO_TICKS,
	EM_AUTO_TICKS,		// Display one tick for each incerement in its range of values
	EM_BOTTOM_TICKS,	// Only horizontal style
	EM_TOP_TICKS,		// Only horizontal style
	EM_LEFT_TICKS,		// Only vertical style
	EM_RIGHT_TICKS,		// Only vertical style
	EM_BOTH_TICKS
	
};

#endif









