#include "EMBeFactory.h"

#ifdef PLATFORM_BEOS

#include "EMBeBitmapContainer.h"
#include "EMBeBoxContainer.h"
#include "EMBeButtonContainer.h"
#include "EMBeFilePanelContainer.h"
#include "EMBeMenuContainer.h"
#include "EMBeMenuBarContainer.h"
#include "EMBeMenuItemContainer.h"
#include "EMBePopUpMenuContainer.h"
#include "EMBeScrollbarContainer.h"
#include "EMBeTextControlContainer.h"
#include "EMBeTextViewContainer.h"
#include "EMBeViewContainer.h"
#include "EMBeWindowContainer.h"

#include <string>
#include <vector>

EMBitmap* EMBeFactory::CreateBitmap(const void* p_opNativeBitmap)
{
	return new EMBeBitmapContainer(p_opNativeBitmap);
}

EMBorder* EMBeFactory::CreateBorder(EMRect p_oFrame, uint32 p_vResizingMode, EMBorderStyle p_vStyle, const char* p_vLabel)
{
	return new EMBeBoxContainer(p_oFrame, p_vResizingMode, p_vStyle, p_vLabel);
}

EMButton* EMBeFactory::CreateButton(const EMRect p_oFrame, const char* p_vpLabel, uint32 p_vMessage, uint32 p_vResizingMode, uint32 p_vCommandID)
{
	return new EMBeButtonContainer(p_oFrame, p_vpLabel, p_vMessage, p_vResizingMode, p_vCommandID);
}

EMFileWindow* EMBeFactory::CreateFileWindow(EMFilePanelMode p_eFilePanelMode/*bool p_vIsLoadWindow*/, 
											const char* p_vpWindowTitle,
											bool p_vMultipleSelection, 
											const char* p_vpSaveDefaultText,
											vector<string>* p_vpVisibleExtensions,
											const char* p_vpStartDirectory, uint32 p_vMessage)
{
	return new EMBeFilePanelContainer(p_eFilePanelMode, p_vpWindowTitle,
										p_vMultipleSelection, p_vpSaveDefaultText,
										p_vpVisibleExtensions, p_vpStartDirectory, p_vMessage);
}

EMMenu* EMBeFactory::CreateMenu(string* p_opName)
{
	return new EMBeMenuContainer(p_opName);
}

EMMenuBar* EMBeFactory::CreateMenuBar()
{
//	return NULL;
	return new EMBeMenuBarContainer();
}

EMMenuItem* EMBeFactory::CreateMenuItem(string* p_opName, uint32 p_vCommand)
{
	return new EMBeMenuItemContainer(p_opName, p_vCommand);
}

EMPopUpMenu* EMBeFactory::CreatePopUpMenu()
{
	return new EMBePopUpMenuContainer();
}

EMScrollbar* EMBeFactory::CreateScrollbar(EMRect p_oFrame, float p_vMinValue, float p_vMaxValue, EMOrientation p_vOrientation)
{
	return new EMBeScrollbarContainer(p_oFrame, p_vMinValue, p_vMaxValue, p_vOrientation);
}

EMTextControl* EMBeFactory::CreateTextControl(EMRect p_oFrame, const char* p_vpText, uint32 p_vResizingMode)
{
	return new EMBeTextControlContainer(p_oFrame, p_vpText, p_vResizingMode);
}

EMTextView* EMBeFactory::CreateTextView(EMRect p_oFrame, const char* p_vpName, uint32 p_vResizingMode)
{
	return new EMBeTextViewContainer(p_oFrame, p_vpName, p_vResizingMode);
}

EMView* EMBeFactory::CreateView(const EMRect p_oFrame, uint32 p_vResizingMode, bool p_vDoubleBuffered)
{
	return new EMBeViewContainer(p_oFrame, p_vResizingMode, p_vDoubleBuffered);
}

EMWindow* EMBeFactory::CreateWindow(const EMRect p_oFrame, const char* p_vpName, const EMWindowType p_oType)
{
	return new EMBeWindowContainer(p_oFrame, p_vpName, p_oType);
}

#endif
