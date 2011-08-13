#include "EMWinFactory.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinBitmap.h"
#include "EMWinBrowseWindow.h"
#include "EMWinButton.h"
#include "EMWinCheckBox.h"
#include "EMWinComboBox.h"
#include "EMWinFileWindow.h"
#include "EMWinGroupBox.h"
#include "EMWinLabel.h"
#include "EMWinListBox.h"
#include "EMWinListViewControl.h"
#include "EMWinMenu.h"
#include "EMWinMenuBar.h"
#include "EMWinMenuItem.h"
#include "EMWinMessageBox.h"
#include "EMWinMouseCursor.h"
#include "EMWinPopUpMenu.h"
#include "EMWinProgressBar.h"
#include "EMWinRadioButton.h"
#include "EMWinScrollbar.h"
#include "EMWinTabControl.h"
#include "EMWinTextControl.h"
#include "EMWinToolTip.h"
#include "EMWinTrackbarControl.h"
#include "EMWinTreeViewControl.h"
#include "EMWinView.h"
#include "EMWinWindow.h"

EMBitmap* EMWinFactory::CreateBitmap(void* p_opNativeBitmap)
{
	return EMWinBitmap::CreateBitmap(p_opNativeBitmap);//EM_new EMWinBitmap(p_opNativeBitmap);
}
/*
EMBorder* EMWinFactory::CreateBorder(EMRect p_oFrame, uint32 p_vResizingMode, EMBorderStyle p_vStyle, const char* p_vLabel)
{
	return NULL;
}
*/
EMBrowseWindow* EMWinFactory::CreateBrowseWindow(void* p_vpNativeView, char* p_vpName)
{
	return EM_new EMWinBrowseWindow(p_vpNativeView, p_vpName);
}

EMButton* EMWinFactory::CreateButton(const EMRect p_oFrame, const char* p_vpLabel, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommandID, void* p_vpParentNativeView, bool p_vDefault)
{
	return EM_new EMWinButton(p_oFrame, p_vpLabel, p_vMessage, p_vResizingMode, p_vCommandID, p_vpParentNativeView, p_vDefault);
}

EMCheckBox* EMWinFactory::CreateCheckBox(const EMRect p_oFrame, const char* p_vpName, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand)
{
	return EM_new EMWinCheckBox(p_oFrame, p_vpName, p_vMessage, p_vResizingMode, p_vCommand);
}

EMComboBox* EMWinFactory::CreateComboBox(const EMRect p_oFrame, uint32 p_vMessage, uint32 p_vResizingMode, bool p_vAutoSort)
{
	return EM_new EMWinComboBox(p_oFrame, p_vMessage, p_vResizingMode, p_vAutoSort);
}

EMFileWindow* EMWinFactory::CreateFileWindow(EMFilePanelMode p_eFilePanelMode/*bool p_vIsLoadWindow*/,
								const char* p_vpWindowTitle,
								void* p_opParentNativeView, 
								bool p_vMultipleSelection, 
								const char* p_vpSaveDefaultText,
								vector<string>* p_vpVisibleExtensions,
								const char* p_vpStartDirectory, uint32 p_vMessage)
{
	return EM_new EMWinFileWindow(p_eFilePanelMode, p_vpWindowTitle, p_opParentNativeView, p_vMultipleSelection, p_vpSaveDefaultText, p_vpVisibleExtensions, p_vpStartDirectory, p_vMessage);
}

EMGroupBox* EMWinFactory::CreateGroupBox(const EMRect p_oFrame, const char* p_vpName, uint32 p_vResizingMode)
{
	return EM_new EMWinGroupBox(p_oFrame, p_vpName, p_vResizingMode);
}

EMLabel* EMWinFactory::CreateLabel(const EMRect p_oFrame, const char* p_vpName, uint32 p_vResizingMode,EMAlignment p_eAlignment)
{
	return EM_new EMWinLabel(p_oFrame, p_vpName, p_vResizingMode, p_eAlignment);
}

EMListBox* EMWinFactory::CreateListBox(const EMRect p_oFrame, uint32 p_vMessage, uint32 p_vResizingMode, bool p_vAutoSort, bool p_vMultipleSelection)
{
	return EM_new EMWinListBox(p_oFrame, p_vMessage, p_vResizingMode, p_vAutoSort, p_vMultipleSelection);
}

EMListViewControl* EMWinFactory::CreateListViewControl(const EMRect p_oFrame, const char* p_vpName, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand, EMListViewStyle p_eListViewStyle, bool p_vEnableDragDrop, bool p_vEnableHoverSelect)
{
	return EM_new EMWinListViewControl(p_oFrame, p_vpName, p_vMessage, p_vResizingMode, p_vCommand, p_eListViewStyle, p_vEnableDragDrop, p_vEnableHoverSelect);
}

EMMenu* EMWinFactory::CreateMenu(string* p_opName)
{
	return EM_new EMWinMenu(p_opName);
}

EMMenuBar* EMWinFactory::CreateMenuBar()
{
	return EM_new EMWinMenuBar();
}

EMMenuItem* EMWinFactory::CreateMenuItem(string* p_opName, uint32 p_vCommand)
{
	return EM_new EMWinMenuItem(p_opName, p_vCommand);
}

EMMessageBox* EMWinFactory::CreateMessageBox()
{
	return EMWinMessageBox::Instance();
}

EMMouseCursor* EMWinFactory::CreateMouseCursor(EMMouseCursorType p_eType)
{
	return EM_new EMWinMouseCursor(p_eType);
}

EMPopUpMenu* EMWinFactory::CreatePopUpMenu()
{
	return EM_new EMWinPopUpMenu();
}

EMProgressBar* EMWinFactory::CreateProgressBar(const EMRect p_oFrame, uint32 p_vMessage)
{
	return EM_new EMWinProgressBar(p_oFrame, p_vMessage);
}

EMRadioButton* EMWinFactory::CreateRadioButton(const EMRect p_oFrame, const char* p_vpName, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand, bool p_vEndGroup)
{
	return EM_new EMWinRadioButton(p_oFrame, p_vpName, p_vMessage, p_vResizingMode, p_vCommand, p_vEndGroup);
}

EMScrollbar* EMWinFactory::CreateScrollbar(EMRect p_oFrame, int32 p_vMinValue, int32 p_vMaxValue, EMOrientation p_vOrientation)
{
	return EM_new EMWinScrollbar(p_oFrame, p_vMinValue, p_vMaxValue, p_vOrientation);
}

EMTabControl* EMWinFactory::CreateTabControl(const EMRect p_oFrame, const char* p_vpName, bool p_vMultiLined, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand)
{
	return EM_new EMWinTabControl(p_oFrame, p_vpName, p_vMultiLined, p_vMessage, p_vResizingMode, p_vCommand);
}

EMTextControl* EMWinFactory::CreateTextControl(EMRect p_oFrame, const char* p_vpText, uint32 p_vMessage, uint32 p_vResizingMode, bool p_vAutoClose, EMAlignment p_eAlignment, bool p_vOnlyNumbers, bool p_vPasswordChar)
{
	return EM_new EMWinTextControl(p_oFrame, p_vpText, p_vMessage, p_vResizingMode, p_vAutoClose, p_eAlignment, p_vOnlyNumbers, p_vPasswordChar);
}
/*
EMTextView* EMWinFactory::CreateTextView(EMRect p_oFrame, const char* p_vpName, uint32 p_vResizingMode)
{
	return NULL;
}
*/

EMToolTip* EMWinFactory::CreateToolTip(const EMRect p_oFrame, const char* p_vpLabel, void* p_vpParentNativeView, bool p_vAutomatic)
{
	return EM_new EMWinToolTip(p_oFrame, p_vpLabel, p_vpParentNativeView, p_vAutomatic);
}

EMTrackbarControl* EMWinFactory::CreateTrackbarControl(const EMRect p_oFrame, const char* p_vpName, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand, EMTrackbarTickStyle p_eTickStyle, bool p_vVertical)
{
	return EM_new EMWinTrackbarControl(p_oFrame, p_vpName, p_vMessage, p_vResizingMode, p_vCommand, p_eTickStyle, p_vVertical);
}

EMTreeViewControl* EMWinFactory::CreateTreeViewControl(const EMRect p_oFrame, const char* p_vpName, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommand)
{
	return EM_new EMWinTreeViewControl(p_oFrame, p_vpName, p_vMessage, p_vResizingMode, p_vCommand);
}

EMView* EMWinFactory::CreateView(const EMRect p_oFrame, uint32 p_vResizingMode, bool p_vDoubleBuffered)
{
	return EM_new EMWinView(p_oFrame, p_vResizingMode, p_vDoubleBuffered);
}

EMWindow* EMWinFactory::CreateEMWindow(const EMRect p_oFrame, const char* p_vpName, const EMWindowType p_oType)
{
	return EM_new EMWinWindow(p_oFrame, p_vpName, p_oType);
}

#endif