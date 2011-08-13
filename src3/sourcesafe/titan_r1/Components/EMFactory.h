/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_FACTORY
#define __EM_FACTORY

#include "EMGUIGlobals.h"
#include "EMRect.h"
#include "EMFileWindow.h" // Contains an enum this file needs to know of
#include "EMMouseCursor.h" // Contains an enum this file needs to know of

#include <string>
#include <vector>

class EMBitmap;
class EMBorder;
class EMBrowseWindow;
class EMButton;
class EMCheckBox;
class EMComboBox;
class EMGroupBox;
class EMLabel;
class EMListBox;
class EMListViewControl;
class EMMenu;
class EMMenuBar;
class EMMenuItem;
class EMMessageBox;
class EMPopUpMenu;
class EMProgressBar;
class EMRadioButton;
class EMScrollbar;
class EMTabControl;
class EMTextControl;
class EMTextView;
class EMToolTip;
class EMTrackbarControl;
class EMTreeViewControl;
class EMView;
class EMWindow;

class EMFactory
{
public:
	virtual ~EMFactory();
	virtual EMBitmap* CreateBitmap(void* p_opNativeBitmap) = 0;
//	virtual EMBorder* CreateBorder(EMRect p_oFrame, uint32 p_vResizingMode, EMBorderStyle p_vStyle, const char* p_vLabel) = 0;
	virtual EMBrowseWindow* CreateBrowseWindow(void* p_vpNativeView, char* p_vpName) = 0;
	virtual EMButton* CreateButton(const EMRect p_oFrame, const char* p_vpLabel, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommandID, void* p_vpParentNativeView = NULL, bool p_vDefault = false) = 0;
	virtual EMCheckBox* CreateCheckBox(const EMRect p_oFrame, const char* p_vpName, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand) = 0;
	virtual EMComboBox* CreateComboBox(const EMRect p_oFrame, uint32 p_vMessage, uint32 p_vResizingMode, bool p_vAutoSort) = 0;
	virtual EMFileWindow* CreateFileWindow(EMFilePanelMode p_eFilePanelMode/*bool p_vIsLoadWindow*/,
											const char* p_vpWindowTitle,
											void* p_opParentNativeView, 
											bool p_vMultipleSelection, 
											const char* p_vpSaveDefaultText,
											vector<string>* p_vpVisibleExtensions,
											const char* p_vpStartDirectory, uint32 p_vMessage) = 0;
	virtual EMGroupBox* CreateGroupBox(const EMRect p_oFrame, const char* p_vpName, uint32 p_vResizingMode) = 0;
	virtual EMLabel* CreateLabel(const EMRect p_oFrame, const char* p_vpName, uint32 p_vResizingMode, const EMAlignment p_eAlignment = EM_ALIGNMENT_LEFT) = 0;
	virtual EMListBox* CreateListBox(const EMRect p_oFrame, uint32 p_vMessage, uint32 p_vResizingMode, bool p_vAutoSort, bool p_vMultipleSelection) = 0;
	virtual EMListViewControl* CreateListViewControl(const EMRect p_oFrame, const char* p_vpName, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand, EMListViewStyle p_eListViewStyle, bool p_vEnableDragDrop = false, bool p_vEnableHoverSelect = false) = 0;
	virtual EMMenu* CreateMenu(string* p_opName) = 0;
	virtual EMMenuBar* CreateMenuBar() = 0;
	virtual EMMenuItem* CreateMenuItem(string* p_opName, uint32 p_vCommand) = 0;
	virtual EMMessageBox* CreateMessageBox() = 0;
	virtual EMMouseCursor* CreateMouseCursor(EMMouseCursorType p_eType) = 0;
	virtual EMPopUpMenu* CreatePopUpMenu() = 0;
	virtual EMProgressBar* CreateProgressBar(const EMRect p_oFrame, uint32 p_vMessage) = 0;
	virtual EMRadioButton* CreateRadioButton(const EMRect p_oFrame, const char* p_vpName, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand, bool p_vEndGroup) = 0;
	virtual EMScrollbar* CreateScrollbar(EMRect p_oFrame, int32 p_vMinValue, int32 p_vMaxValue, EMOrientation p_vOrientation) = 0;
	virtual EMTabControl* CreateTabControl(const EMRect p_oFrame, const char* p_vpName, bool p_vMultiLined, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand) = 0;
	virtual EMTextControl* CreateTextControl(EMRect p_oFrame, const char* p_vpText, uint32 p_vMessage, uint32 p_vResizingMode, bool p_vAutoClose, const EMAlignment p_eAlignment = EM_ALIGNMENT_LEFT, bool p_vOnlyNumber = false, bool p_vPasswordChar = false) = 0;
//	virtual EMTextView* CreateTextView(EMRect p_oFrame, const char* p_vpName, uint32 p_vResizingMode) = 0;
	virtual EMToolTip* CreateToolTip(const EMRect p_oFrame, const char* p_vpLabel, void* p_vpParentNativeView, bool p_vAutomatic) = 0;
	virtual EMTrackbarControl* CreateTrackbarControl(const EMRect p_oFrame, const char* p_vpName, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand, EMTrackbarTickStyle p_eTickStyle = EM_NO_TICKS, bool p_vVertical = false) = 0;
	virtual EMTreeViewControl* CreateTreeViewControl(const EMRect p_oFrame, const char* p_vpName, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand) = 0;
	virtual EMView* CreateView(const EMRect p_oFrame, uint32 p_vResizingMode, bool p_vDoubleBuffered = false) = 0;
	virtual EMWindow* CreateEMWindow(const EMRect p_oFrame, const char* p_vpName, const EMWindowType p_oType) = 0;
	static EMFactory* Instance();

protected:
	EMFactory();

private:
	static EMFactory* m_opInstance;
};

#endif


