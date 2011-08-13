/*******************************************************
* This code is the copyrighted property of Elegant Media
*-------------------------------------------------------
* Creator: Johan Dunberg
* Component: Framework
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/
#include "EMGlobals.h"

#ifndef __EM_MESSAGE_BOX
#define __EM_MESSAGE_BOX

class EMWindow;

enum EM_MESSAGE_BOX_FLAGS
{
	EM_DIALOG_BUTTON_ABORT_RETRY_IGNORE = 1,
	EM_DIALOG_BUTTON_OK					= 2,
	EM_DIALOG_BUTTON_OK_CANCEL			= 4,
	EM_DIALOG_BUTTON_RETRY_CANCEL		= 8,
	EM_DIALOG_BUTTON_YES_NO				= 16,
	EM_DIALOG_BUTTON_YES_NO_CANCEL		= 32,
	EM_DIALOG_ICON_WARNING				= 64,
	EM_DIALOG_ICON_INFORMATION			= 128,
	EM_DIALOG_ICON_QUESTION				= 256,
	EM_DIALOG_ICON_ERROR				= 512
};

enum EM_MESSAGE_BOX_RESULT
{
	EM_DIALOG_ABORT_BUTTON,
	EM_DIALOG_CANCEL_BUTTON,
	EM_DIALOG_IGNORE_BUTTON,
	EM_DIALOG_NO_BUTTON,
	EM_DIALOG_OK_BUTTON,
	EM_DIALOG_RETRY_BUTTON,
	EM_DIALOG_YES_BUTTON
};

enum Results  // Temporary put here. Put it in the Folder/Disk management class when it exist!!
{
	RESULT_OK,
	RESULT_NOT_OK,
	RESULT_NO_BUTTON,
	RESULT_CANCEL_BUTTON,
};

class EMMessageBox
{
public:
	virtual uint32 DisplayDialog(void* p_opNativeView, const char* p_vMessage, const char* p_vCaption, uint32 p_vFlags) = 0;
	static EMMessageBox* Instance();

protected:
	EMMessageBox();

private:
	static EMMessageBox* m_opInstance;
};

#endif
