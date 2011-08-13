#include "AboutDialog.h"

#include "EMApplication.h"
#include "EMBitmap.h"
#include "EMButton.h"
#include "EMFactory.h"
#include "EMMessages.h"
#include "EMResourceRepository.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMTextControl.h"
#include "EMView.h"
#include "EMWindow.h"
#include "ResourceIDs.h"

#ifdef PLATFORM_WINDOWS
#include <windows.h> // To get modal dialog to work.
#include "EMWinWindow.h" // To get modal dialog to work.
#endif

const uint32 EM_BUTTON_CLOSE = 1;
const uint32 EM_BUTTON_HOMEPAGE = 2;
const uint32 EM_BUTTON_PURCHASE = 3;

AboutDialog::AboutDialog() :
m_opDialogWindow(EMFactory::Instance() -> CreateEMWindow(EMRect(0, 0, 475, 388), "About Titan", EM_WINDOW_DIALOG)), 
m_opMainView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 475, 388), false)),
m_opCloseButton(EMFactory::Instance() -> CreateButton(EMRect(391,
														   388 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
														   391 + EM_METRICS_NATIVE_BUTTON_WIDTH - 1,
														   388 - EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM),
													"Close", EM_BUTTON_CLOSE, 0, 0, NULL, true)),
m_opHomepageButton(EMFactory::Instance() -> CreateButton(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH + EM_METRICS_NATIVE_BUTTON_SPACE - 1,
														   388 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
														   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH * 2 + EM_METRICS_NATIVE_BUTTON_SPACE - 1,
														   388 - EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM),
													"Web Page", EM_BUTTON_HOMEPAGE, 0, 0, NULL, false)),
m_opPurchaseButton(EMFactory::Instance() -> CreateButton(EMRect(EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE,
														   388 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
														   EM_METRICS_NATIVE_DIALOG_MARGIN_SIDE + EM_METRICS_NATIVE_BUTTON_WIDTH - 1,
														   388 - EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM),
													"Purchase", EM_BUTTON_PURCHASE, 0, 0, NULL, false))
/*
													m_opCloseButton(EMFactory::Instance() ->CreateButton(EMRect(475 / 2 - EM_METRICS_NATIVE_BUTTON_WIDTH / 2,
														   388 - (EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM + EM_METRICS_NATIVE_BUTTON_HEIGHT),
														   475 / 2 + EM_METRICS_NATIVE_BUTTON_WIDTH / 2,
														   388 - EM_METRICS_NATIVE_DIALOG_MARGIN_BOTTOM),
													"Close", EM_BUTTON_CLOSE, 0, 0, NULL, true))
*/
{
	m_opDialogWindow -> AddChild(m_opMainView);
	m_opMainView -> AddChild(m_opCloseButton);
	m_opMainView -> AddChild(m_opHomepageButton);
	m_opMainView -> AddChild(m_opPurchaseButton);
}

AboutDialog::~AboutDialog()
{
	delete m_opDialogWindow;
	delete m_opMainView;
	delete m_opCloseButton;
	delete m_opHomepageButton;
	delete m_opPurchaseButton;
}

void AboutDialog::DisplayDialog()
{
	if(m_opDialogWindow -> Lock())
	{
		m_opDialogWindow -> Show();
		vDraw = true;
#ifdef PLATFORM_WINDOWS
		EnableWindow(dynamic_cast<EMWinWindow*>(m_opDialogWindow) -> GetMainWindowHandle(), FALSE);
		SetFocus(static_cast<HWND>(m_opCloseButton -> GetNativeView()));
#endif
		m_opDialogWindow -> Unlock();
	}
}

void AboutDialog::HideDialog()
{
	if(m_opDialogWindow -> Lock())
	{
#ifdef PLATFORM_WINDOWS
		EnableWindow(dynamic_cast<EMWinWindow*>(m_opDialogWindow) -> GetMainWindowHandle(), TRUE);
		SetFocus(dynamic_cast<EMWinWindow*>(m_opDialogWindow) -> GetMainWindowHandle());
#endif
		vDraw = false;
		m_opDialogWindow -> Hide();
		m_opDialogWindow -> Unlock();
	}
}

void AboutDialog::Init()
{
	m_opSplashBitmap = EMResourceRepository::GetBitmap(RES_SPLASH);
	vDraw = false;
	m_opDialogWindow -> AddListener(this);
	m_opMainView -> AddListener(this);
	if(m_opDialogWindow -> Lock())
	{
		//m_opMainView -> SetViewColor(EMColor(192, 192, 192, 255));
		m_opMainView -> SetDefaultColor();
		m_opDialogWindow -> Unlock();
	}
	m_opCloseButton -> AddListener(this);
	m_opHomepageButton -> AddListener(this);
	m_opPurchaseButton -> AddListener(this);
}

bool AboutDialog::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
	case EM_DRAW:
		if(p_opSender == m_opMainView && vDraw)
		{
			m_opMainView -> DrawBitmap(m_opSplashBitmap, 0, 0);

			char vpVersionString[80];
			sprintf(vpVersionString, "Version %s", EMApplication::GetApplicationVersionNumber());
			m_opMainView -> SetHighColor(EMColor(0, 0, 0, 255));
			m_opMainView -> DrawString(380, 20, vpVersionString);
			if(*(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SOFTWARE_IS_CRIPPLED))))
				m_opMainView -> DrawString(380, 30, "Unregistered");

			return true;
		}
		return false;
	case EM_BUTTON_HOMEPAGE:
#ifdef PLATFORM_WINDOWS
		ShellExecute(GetActiveWindow(), "open", "explorer", "http:\\\\www.elegant-media.com", NULL, SW_SHOWNORMAL);
#endif
		HideDialog();
		return true;
	case EM_BUTTON_PURCHASE:
#ifdef PLATFORM_WINDOWS
		ShellExecute(GetActiveWindow(), "open", "explorer", "http:\\\\www.elegant-media.com\\sales.html", NULL, SW_SHOWNORMAL);
#endif
		HideDialog();
		return true;
	case EM_GUI_COMPONENT_ENTER_PRESSED:
	case EM_GUI_COMPONENT_ESC_PRESSED:
	case EM_BUTTON_CLOSE:
		eo << "Close button pressed" << ef;
	//	eo << "Getting TextControl value: " << m_opTextControl -> Text() << ef;
		HideDialog();
		return true;
	case EM_CLOSE_WINDOW:
		if(p_opSender == m_opDialogWindow)
		{
			eo << "EM_CLOSE_WINDOW" << ef;
			HideDialog();
			return true;
		}
		break;
	}
	return false;
}

void AboutDialog::MoveDialog()
{
	EMRect oWindowRect(m_opParentWindow -> Frame());
	EMRect oDialogRect(m_opDialogWindow -> Frame());
	float vX(oWindowRect.m_vRight - oWindowRect.GetWidth() / 2 - oDialogRect.GetWidth() / 2);
	float vY(oWindowRect.m_vBottom - oWindowRect.GetHeight() / 2 - oDialogRect.GetHeight() / 2);

	MoveDialog(vX, vY);
}

void AboutDialog::MoveDialog(float p_vX, float p_vY)
{
	m_opDialogWindow -> MoveTo(p_vX, p_vY);
}

void AboutDialog::SetParent(EMWindow* p_opWindow)
{
	m_opParentWindow = p_opWindow;
}
