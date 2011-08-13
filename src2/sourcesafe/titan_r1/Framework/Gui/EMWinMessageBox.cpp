#include "EMWinMessageBox.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinWindow.h"

EMWinMessageBox::EMWinMessageBox()
{
}

uint32 EMWinMessageBox::DisplayDialog(void* p_opNativeView, const char* p_vMessage, const char* p_vCaption, uint32 p_vFlags)
{
	uint32 vFlags = 0;
	if(p_vFlags & EM_DIALOG_BUTTON_ABORT_RETRY_IGNORE)
		vFlags |= MB_ABORTRETRYIGNORE;
	if(p_vFlags & EM_DIALOG_BUTTON_OK)
		vFlags |= MB_OK;
	if(p_vFlags & EM_DIALOG_BUTTON_OK_CANCEL)
		vFlags |= MB_OKCANCEL;
	if(p_vFlags & EM_DIALOG_BUTTON_RETRY_CANCEL)
		vFlags |= MB_RETRYCANCEL;
	if(p_vFlags & EM_DIALOG_BUTTON_YES_NO)
		vFlags |= MB_YESNO;
	if(p_vFlags & EM_DIALOG_BUTTON_YES_NO_CANCEL)
		vFlags |= MB_YESNOCANCEL;
	if(p_vFlags & EM_DIALOG_ICON_WARNING)
		vFlags |= MB_ICONWARNING;
	if(p_vFlags & EM_DIALOG_ICON_INFORMATION)
		vFlags |= MB_ICONINFORMATION;
	if(p_vFlags & EM_DIALOG_ICON_QUESTION)
		vFlags |= MB_ICONQUESTION;
	if(p_vFlags & EM_DIALOG_ICON_ERROR)
		vFlags |= MB_ICONERROR;

	HWND opParentWindow;
	if(p_opNativeView == NULL) // If NULL, take the current active window instead
	{
		opParentWindow = GetForegroundWindow();
	}
	else
		opParentWindow = static_cast<HWND>(p_opNativeView);

	uint32 vWinResult(MessageBox(opParentWindow, p_vMessage, p_vCaption, vFlags));
	uint32 vResult = 0;

	switch(vWinResult)
	{
	case IDABORT:
		vResult = EM_DIALOG_ABORT_BUTTON;
		break;
	case IDCANCEL:
		vResult = EM_DIALOG_CANCEL_BUTTON;
		break;
	case IDIGNORE:
		vResult = EM_DIALOG_IGNORE_BUTTON;
		break;
	case IDNO:
		vResult = EM_DIALOG_NO_BUTTON;
		break;
	case IDOK:
		vResult = EM_DIALOG_OK_BUTTON;
		break;
	case IDRETRY:
		vResult = EM_DIALOG_RETRY_BUTTON;
		break;
	case IDYES:
		vResult = EM_DIALOG_YES_BUTTON;
		break;
	};
	return vResult;
}
#endif