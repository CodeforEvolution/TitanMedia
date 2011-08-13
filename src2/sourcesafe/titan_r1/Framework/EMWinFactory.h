/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_FACTORY
#define __EM_WIN_FACTORY

#include "EMFactory.h"
#include "EMGUIGlobals.h"
#include "EMRect.h"

#include <string>

class EMBitmap;
class EMBorder;
class EMBrowseWindow;
class EMButton;
class EMCheckBox;
class EMComboBox;
class EMFileWindow;
class EMGroupBox;
class EMLabel;
class EMListBox;
class EMListViewControl;
class EMMenu;
class EMMenuBar;
class EMMenuItem;
class EMPopUpMenu;
class EMProgressBar;
class EMRadioButton;
class EMScrollbar;
class EMTabControl;
class EMToolTip;
class EMTrackbarControl;
class EMTextControl;
class EMTextView;
class EMTreeViewControl;
class EMView;
class EMWindow;

class EMWinFactory : public EMFactory
{
public:
#ifndef EM_BASIC_COMPONENTS
//	EMBorder* CreateBorder(EMRect p_oFrame, uint32 p_vResizingMode, EMBorderStyle p_vStyle, const char* p_vLabel);
	EMBrowseWindow* CreateBrowseWindow(void* p_vpNativeView, char* p_vpName);
	EMButton* CreateButton(const EMRect p_oFrame, const char* p_vpLabel, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommandID, void* p_vNativeView, bool p_vDefault);
	EMCheckBox* CreateCheckBox(const EMRect p_oFrame, const char* p_vpName, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand);
	EMComboBox* CreateComboBox(const EMRect p_oFrame, uint32 p_vMessage, uint32 p_vResizingMode, bool p_vAutoSort);
	EMFileWindow* CreateFileWindow(EMFilePanelMode p_eFilePanelMode/*bool p_vIsLoadWindow*/, 
									const char* p_vpWindowTitle,
									void* p_opParentNativeView, 
									bool p_vMultipleSelection, 
									const char* p_vpSaveDefaultText,
									vector<string>* p_vpVisibleExtensions,
									const char* p_vpStartDirectory, uint32 p_vMessage);
	EMGroupBox* CreateGroupBox(const EMRect p_oFrame, const char* p_vpName, uint32 p_vResizingMode);
	EMLabel* CreateLabel(const EMRect p_oFrame, const char* p_vpName, uint32 p_vResizingMode, const EMAlignment p_eAlignment);// = EM_ALIGNMENT_LEFT
	EMListBox* CreateListBox(const EMRect p_oFrame, uint32 p_vMessage, uint32 p_vResizingMode, bool p_vAutoSort, bool p_vMultipleSelection);
	EMListViewControl* CreateListViewControl(const EMRect p_oFrame, const char* p_vpName, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand, EMListViewStyle p_eListViewStyle, bool p_vEnableDragDrop, bool p_vEnableHoverSelect);
	EMMenu* CreateMenu(string* p_opName);
	EMMenuBar* CreateMenuBar();
	EMMenuItem* CreateMenuItem(string* p_opName, uint32 p_vCommand);
	EMMouseCursor* CreateMouseCursor(EMMouseCursorType p_eType);
	EMPopUpMenu* CreatePopUpMenu();
	EMProgressBar* CreateProgressBar(const EMRect p_oFrame, uint32 p_vMessage);
	EMRadioButton* CreateRadioButton(const EMRect p_oFrame, const char* p_vpName, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand, bool p_vEndGroup);
	EMScrollbar* CreateScrollbar(EMRect p_oFrame, int32 p_vMinValue, int32 p_vMaxValue, EMOrientation p_vOrientation);
	EMTabControl* CreateTabControl(const EMRect p_oFrame, const char* p_vpName, bool p_vMultiLined, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand);
//	EMTabControl* CreateTabControl(const char* p_vpName, bool p_vMultiLined, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand);
	EMTextControl* CreateTextControl(EMRect p_oFrame, const char* p_vpText, uint32 p_vMessage, uint32 p_vResizingMode, bool p_vAutoClose, const EMAlignment p_eAlignment, bool p_vOnlyNumbers, bool p_vPasswordChar);
	//EMTextView* CreateTextView(EMRect p_oFrame, const char* p_vpName, uint32 p_vResizingMode);
	EMToolTip* CreateToolTip(const EMRect p_oFrame, const char* p_vpLabel, void* p_vpParentNativeView, bool p_vAutomatic);
	EMTrackbarControl* CreateTrackbarControl(const EMRect p_oFrame, const char* p_vpName, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand, EMTrackbarTickStyle p_eTickStyle, bool p_vVertical);
	EMTreeViewControl* CreateTreeViewControl(const EMRect p_oFrame, const char* p_vpName, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand);
#endif // EM_BASIC_COMPONENTS
	EMBitmap* CreateBitmap(void* p_opNativeBitmap);
	EMMessageBox* CreateMessageBox();
	EMView* CreateView(const EMRect p_oFrame, uint32 p_vResizingMode, bool p_vDoubleBuffered
		);
	EMWindow* CreateEMWindow(const EMRect p_oFrame, const char* p_vpName, const EMWindowType p_oType);
};

#endif

#endif
