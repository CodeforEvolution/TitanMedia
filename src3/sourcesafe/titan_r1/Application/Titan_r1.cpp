/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* Contains lots of native code
*******************************************************/

#include "EMGlobals.h"

#include "Titan_r1.h"

#include "EMApplication.h"
#include "EMExceptionHandler.h"

#ifdef PLATFORM_BEOS

Titan_r1::Titan_r1() :
BApplication("application/x-vnd.Titan")
{
}

Titan_r1::~Titan_r1()
{
}

void Titan_r1::ReadyToRun()
{
	// Redirect bad_alloc exception handling
	set_new_handler(&(EMExceptionHandler::NewHandler));

	m_oInit.SetUpSystem();
}

int main()
{
	Titan_r1* opTitan = EM_new Titan_r1();
	try
	{
		opTitan -> Run();
	}
	catch(bad_alloc)
	{
		// Do stuff
	}
	catch(...)
	{
		// Do stuff
	}
	delete opTitan;
}

#endif

#ifdef PLATFORM_WINDOWS

#include "EMWinView.h"
#include "EMWinWindow.h"

#include <new.h>
#include <windows.h>
#include <tlhelp32.h>
#include "crtdbg.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR lpCmd, int nShow)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// Check if Titan already is running
	char vpThisAppName[] = "Titan_r1.exe";
	int vNumberOfTitanRunning(0);
	bool vValid(true);
	PROCESSENTRY32 sProcessInfo;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS|TH32CS_SNAPMODULE , 0);

	sProcessInfo.dwSize = sizeof(PROCESSENTRY32);

	vValid = Process32First(hSnapshot, &sProcessInfo) != 0;
	while(vValid)
	{
		char* opNextApp = strrchr(sProcessInfo.szExeFile, '\\');

		if(opNextApp == NULL)
			opNextApp = sProcessInfo.szExeFile;
		else
			++opNextApp;
				
		if(_stricmp(vpThisAppName, opNextApp) == 0)
			++vNumberOfTitanRunning;
		vValid = Process32Next(hSnapshot, &sProcessInfo) != 0;
	}

	CloseHandle(hSnapshot);

	if(vNumberOfTitanRunning > 1)
	{
		MessageBox(NULL, "You already have \"Elegant Media Titan\" running on this system.", "Is running", MB_OK | MB_ICONINFORMATION);
		return -1;
	}

	// Check if the OS supports Titan
	HMODULE hMod = GetModuleHandle ("ole32.dll") ;
	FARPROC fp = GetProcAddress (hMod, "CoCreateInstanceEx") ;
	if(fp == NULL)
	{
		MessageBox (NULL, "It seems as though your operating system does\nnot support the technology required by this\nproduct. This product is developed and tested\nonly for Microsoft Windows 98 and Microsoft\nWindows 2000 with DirectX 8 or higher installed.\n\nYou will not be able to run Elegant Media Titan\non this machine with the current operating system!\n\nIf you have any questions, please send\nan email to support@elegant-media.com", "Unrecoverable error", MB_OK);
		return -1 ;
	}

	HRESULT vResult = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(vResult))
		EMDebugger("ERROR! Failed to initialize COM system in WinMain!");

	EMApplication::SetApplicationInstance(hInst);

	// Redirect bad_alloc exception handling
	//_set_new_handler(&EMExceptionHandler::NewHandler));

	//SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

	ApplicationInit oInit;
	oInit.SetUpSystem(lpCmd);

	return EMWinWindow::StartMessageLoop();;
}

#endif

