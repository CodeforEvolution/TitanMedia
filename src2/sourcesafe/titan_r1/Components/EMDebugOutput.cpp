//#ifdef PLATFORM_WINDOWS
#pragma warning(disable : 4786) // Skips boring debug truncate warning
#pragma warning(disable : 4275) // Skips "...needs to have dll-interface..."
#pragma warning(disable : 4251) // Skips "...needs to have dll-interface..."
//#endif //PLATFORM_WINDOWS

#ifndef DEBUG_VERSION
#define DISABLE_OUTPUT
#endif

#include "EMDebugOutput.h"

#include "EMApplication.h"
#include "EMDebug_LibraryVersion.h"
// Since EMGlobals.h" isn't included in the header, it's included here instead
#include "EMGlobals.h"

#include <fstream>

static EMDebugOutput eo;

#ifdef PLATFORM_WINDOWS

#include "EMWinMessages.h"

#include <process.h>

// Global variables aren't nice, but this is an evil file anyway
HWND hwndList(0);
HWND hwndWindow(0);
HANDLE m_vProtectDataSemaphore;
HANDLE m_vLocalStringSemaphore;
string m_oMailBoxString;
list<string*>* m_opMailBoxList;
char m_vMailBoxString[255];
static int m_vCounter(0);
EMDebugOutput* opDebugOutput;
static bool m_vDebugIsActive(false);
static bool m_vWriteDebugLog(false);
#endif //PLATFORM_WINDOWS

const char* LOG_FILE_NAME = "\\titan.log";

#ifndef __EXPORT_EM_DEBUG_LIBRARY
#define __EXPORT_EM_DEBUG_LIBRARY

EMDebugOutput::EMDebugOutput() :
m_vWindowCreated(false)
{
	eo << "DebugOutput Library v0.1Win, Build " << __EMDebug_LibraryVersion << " by " << __EMDebug_LibraryVersion_Owner << " (C) Elegant Media AB 2000-2001" << ef;
#ifdef PLATFORM_WINDOWS
	m_opMailBoxList = new list<string*>(1000);
	opDebugOutput = this;
#endif //PLATFORM_WINDOWS
}

EMDebugOutput::~EMDebugOutput()
{
#ifdef PLATFORM_WINDOWS
	DeleteObject(m_vProtectDataSemaphore);
	if(hwndList != NULL)
		DestroyWindow(hwndList);
	if(hwndWindow != NULL)
		DestroyWindow(hwndWindow);
	delete m_opMailBoxList;
	CloseHandle(m_vProtectDataSemaphore);
	CloseHandle(m_vLocalStringSemaphore);
#endif //PLATFORM_WINDOWS
}

EMDebugOutput& EMDebugOutput::operator<<(short p_vInput)
{
	if(m_vWriteDebugLog)
	{
		ofstream oOutStream(LOG_FILE_NAME, std::ios::out | std::ios::app);
		oOutStream << p_vInput;
	}

	if(!m_vDebugIsActive)
		return (*this);
//#ifdef DISABLE_OUTPUT
//return (*this);
//#endif
	ostringstream oOutputStream;
	oOutputStream << p_vInput;

	long vDummy;
	WaitForSingleObject(m_vLocalStringSemaphore, INFINITE);
	m_oOutputString += oOutputStream.str();
	ReleaseSemaphore(m_vLocalStringSemaphore, 1, &vDummy);
	return (*this);
}

EMDebugOutput& EMDebugOutput::operator<<(const char* p_vpInput)
{
	if(p_vpInput[0] == '*')
	{
		if(strcmp(p_vpInput, "*DISABLE_DEBUG_WINDOW") == 0)
			m_vDebugIsActive = false;
		else if(strcmp(p_vpInput, "*ENABLE_DEBUG_WINDOW") == 0)
			m_vDebugIsActive = true;

		if(strcmp(p_vpInput, "*DISABLE_DEBUG_LOG") == 0)
			m_vWriteDebugLog = false;
		else if(strcmp(p_vpInput, "*ENABLE_DEBUG_LOG") == 0)
			m_vWriteDebugLog = true;

		return (*this);
	}

	if(m_vWriteDebugLog)
	{
		ofstream oOutStream(LOG_FILE_NAME, std::ios::out | std::ios::app);
		oOutStream << p_vpInput;
	}

	if(!m_vDebugIsActive)
		return (*this);
//#ifdef DISABLE_OUTPUT
//return (*this);
//#endif
	long vDummy;
	WaitForSingleObject(m_vLocalStringSemaphore, INFINITE);
	m_oOutputString += string(p_vpInput);
	ReleaseSemaphore(m_vLocalStringSemaphore, 1, &vDummy);
	return (*this);
}

EMDebugOutput& EMDebugOutput::operator<<(unsigned short p_vInput)
{
	if(m_vWriteDebugLog)
	{
		ofstream oOutStream(LOG_FILE_NAME, std::ios::out | std::ios::app);
		oOutStream << p_vInput;
	}

	if(!m_vDebugIsActive)
		return (*this);
//#ifdef DISABLE_OUTPUT
//return (*this);
//#endif
	ostringstream oOutputStream;
	oOutputStream << p_vInput;

	long vDummy;
	WaitForSingleObject(m_vLocalStringSemaphore, INFINITE);
	m_oOutputString += oOutputStream.str();
	ReleaseSemaphore(m_vLocalStringSemaphore, 1, &vDummy);
	return (*this);
}

EMDebugOutput& EMDebugOutput::operator<<(int p_vInput)
{
	if(m_vWriteDebugLog)
	{
		ofstream oOutStream(LOG_FILE_NAME, std::ios::out | std::ios::app);
		oOutStream << p_vInput;
	}

	if(!m_vDebugIsActive)
		return (*this);
//#ifdef DISABLE_OUTPUT
//return (*this);
//#endif
	ostringstream oOutputStream;
	oOutputStream << p_vInput;

	long vDummy;
	WaitForSingleObject(m_vLocalStringSemaphore, INFINITE);
	m_oOutputString += oOutputStream.str();
	ReleaseSemaphore(m_vLocalStringSemaphore, 1, &vDummy);
	return (*this);
}

EMDebugOutput& EMDebugOutput::operator<<(unsigned int p_vInput)
{
	if(m_vWriteDebugLog)
	{
		ofstream oOutStream(LOG_FILE_NAME, std::ios::out | std::ios::app);
		oOutStream << p_vInput;
	}

	if(!m_vDebugIsActive)
		return (*this);
//#ifdef DISABLE_OUTPUT
//return (*this);
//#endif
	ostringstream oOutputStream;
	oOutputStream << p_vInput;

	long vDummy;
	WaitForSingleObject(m_vLocalStringSemaphore, INFINITE);
	m_oOutputString += oOutputStream.str();
	ReleaseSemaphore(m_vLocalStringSemaphore, 1, &vDummy);
	return (*this);
}

EMDebugOutput& EMDebugOutput::operator<<(long p_vInput)
{
	if(m_vWriteDebugLog)
	{
		ofstream oOutStream(LOG_FILE_NAME, std::ios::out | std::ios::app);
		oOutStream << p_vInput;
	}

	if(!m_vDebugIsActive)
		return (*this);
//#ifdef DISABLE_OUTPUT
//return (*this);
//#endif
	ostringstream oOutputStream;
	oOutputStream << p_vInput;

	long vDummy;
	WaitForSingleObject(m_vLocalStringSemaphore, INFINITE);
	m_oOutputString += oOutputStream.str();
	ReleaseSemaphore(m_vLocalStringSemaphore, 1, &vDummy);
	return (*this);
}

EMDebugOutput& EMDebugOutput::operator<<(unsigned long p_vInput)
{
	if(m_vWriteDebugLog)
	{
		ofstream oOutStream(LOG_FILE_NAME, std::ios::out | std::ios::app);
		oOutStream << p_vInput;
	}

	if(!m_vDebugIsActive)
		return (*this);
//#ifdef DISABLE_OUTPUT
//return (*this);
//#endif
	ostringstream* o = NULL; //TEMP, Doesn't do anything
	ostringstream oOutputStream;
	oOutputStream << p_vInput;

	long vDummy;
	WaitForSingleObject(m_vLocalStringSemaphore, INFINITE);
	m_oOutputString += oOutputStream.str();
	ReleaseSemaphore(m_vLocalStringSemaphore, 1, &vDummy);
	return (*this);
}

EMDebugOutput& EMDebugOutput::operator<<(float p_vInput)
{
	if(m_vWriteDebugLog)
	{
		ofstream oOutStream(LOG_FILE_NAME, std::ios::out | std::ios::app);
		oOutStream << p_vInput;
	}

	if(!m_vDebugIsActive)
		return (*this);
//#ifdef DISABLE_OUTPUT
//return (*this);
//#endif
	ostringstream oOutputStream;
	oOutputStream << p_vInput;

	long vDummy;
	WaitForSingleObject(m_vLocalStringSemaphore, INFINITE);
	m_oOutputString += oOutputStream.str();
	ReleaseSemaphore(m_vLocalStringSemaphore, 1, &vDummy);
	return (*this);
}

EMDebugOutput& EMDebugOutput::operator<<(double p_vInput)
{
	if(m_vWriteDebugLog)
	{
		ofstream oOutStream(LOG_FILE_NAME, std::ios::out | std::ios::app);
		oOutStream << p_vInput;
	}

	if(!m_vDebugIsActive)
		return (*this);
//#ifdef DISABLE_OUTPUT
//return (*this);
//#endif
	ostringstream oOutputStream;
	oOutputStream << p_vInput;

	long vDummy;
	WaitForSingleObject(m_vLocalStringSemaphore, INFINITE);
	m_oOutputString += oOutputStream.str();
	ReleaseSemaphore(m_vLocalStringSemaphore, 1, &vDummy);
	return (*this);
}

EMDebugOutput& EMDebugOutput::operator<<(int64 p_vInput)
{
	if(m_vWriteDebugLog)
	{
		ofstream oOutStream(LOG_FILE_NAME, std::ios::out | std::ios::app);
		oOutStream << static_cast<long>(p_vInput);
	}

	if(!m_vDebugIsActive)
		return (*this);
//#ifdef DISABLE_OUTPUT
//return (*this);
//#endif
	ostringstream oOutputStream;
	oOutputStream << double(p_vInput);

	long vDummy;
	WaitForSingleObject(m_vLocalStringSemaphore, INFINITE);
	m_oOutputString += oOutputStream.str();
	ReleaseSemaphore(m_vLocalStringSemaphore, 1, &vDummy);
	return (*this);
}

EMDebugOutput& EMDebugOutput::operator<<(EMDebugOutput& (*p_opFunction)(EMDebugOutput&))
{
	return p_opFunction(*this);
}

EMDebugOutput& EMDebugOutput::EMFlush()
{
	if(m_vWriteDebugLog)
	{
		ofstream oOutStream(LOG_FILE_NAME, std::ios::out | std::ios::app);
		oOutStream << '\n';
	}

	if(!m_vDebugIsActive)
		return (*this);
//#ifdef DISABLE_OUTPUT
//return (*this);
//#endif
	if(!m_vWindowCreated)
		CreateDebugWindow();
	long vDummy;
	WaitForSingleObject(m_vLocalStringSemaphore, INFINITE);
	OutputString(m_oOutputString);
	m_oOutputString = "";
	ReleaseSemaphore(m_vLocalStringSemaphore, 1, &vDummy);

	return (*this);
}

void EMDebugOutput::OutputString(string p_oString)
{
#ifdef PLATFORM_WINDOWS
	WaitForSingleObject(m_vProtectDataSemaphore, INFINITE);
	m_oMailBoxList.push_back(p_oString);
	ReleaseSemaphore(m_vProtectDataSemaphore, 1, NULL);
	bool vFailed;
	if(PostMessage(hwndWindow, NATIVE_MESSAGE_MAIL_SENT, 0, 0) == 0)
		vFailed = true;
	m_vCounter++;
#endif //PLATFORM_WINDOWS
#ifdef PLATFORM_BEOS
	cout << p_oString;
#endif //PLATFORM_BEOS
}

LRESULT CALLBACK DebugWindowProcedure(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

void EMDebugOutput::CreateDebugWindow()
{
	// If BeOS, the default terminal window is used instead of a dedicated debug window
#ifdef PLATFORM_WINDOWS

	static char szAppName[] = "EMDebugOutput" ;
	WNDCLASSEX  wndclass ;

	wndclass.cbSize        = sizeof (wndclass) ;
	wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = DebugWindowProcedure ;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = 0 ;
	wndclass.hInstance     = EMApplication::GetApplicationInstance();//hInstance ;
	wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
	wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1) ;
	wndclass.lpszMenuName  = NULL ;
	wndclass.lpszClassName = szAppName ;
	wndclass.hIconSm       = LoadIcon (NULL, IDI_APPLICATION) ;

	RegisterClassEx (&wndclass) ;

	hwndWindow = CreateWindow (szAppName, "Debug Output",
                          WS_OVERLAPPEDWINDOW,
                          0, 0,
                          500, 1000,
                          NULL, NULL, EMApplication::GetApplicationInstance(), NULL) ;

	ShowWindow (hwndWindow, SW_SHOW) ;
	UpdateWindow (hwndWindow);

	m_vProtectDataSemaphore = CreateSemaphore(NULL, 1, 1, "DebugOutput Semaphore 1");
	m_vLocalStringSemaphore = CreateSemaphore(NULL, 1, 1, "DebugOutput Semaphore 2");
#endif //PLATFORM_WINDOWS
	m_vWindowCreated = true;
}

#ifdef PLATFORM_WINDOWS

LRESULT CALLBACK DebugWindowProcedure(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC         hdc ;
	TEXTMETRIC  tm ;

	switch (iMsg)
	{
	case NATIVE_MESSAGE_MAIL_SENT:
		{
			string oString;
			WaitForSingleObject(m_vProtectDataSemaphore, INFINITE);
			if(opDebugOutput == NULL)
				return 0;
			char vCounterString[20];
			while(opDebugOutput -> m_oMailBoxList.size() > 0)
			{
				oString = opDebugOutput -> m_oMailBoxList.front();
				sprintf(vCounterString, "     (%d)", m_vCounter);
				oString += vCounterString;
				opDebugOutput -> m_oMailBoxList.pop_front();
				SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)(oString.c_str()));
				//delete opString;
				uint32 vSize = SendMessage(hwndList, LB_GETCOUNT, 0, 0);
				SendMessage(hwndList, LB_SETCURSEL, vSize - 1, 0);
				if(vSize >= 1000)
					SendMessage(hwndList, LB_DELETESTRING, 0, 0);
			}
			ReleaseSemaphore(m_vProtectDataSemaphore, 1, NULL);
			return 0;
		}
	case WM_CREATE:
		hdc = GetDC (hwnd) ;
		GetTextMetrics (hdc, &tm) ;
		ReleaseDC (hwnd, hdc) ;

		hwndList = CreateWindow ("listbox", NULL,
			 WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
			tm.tmAveCharWidth, tm.tmHeight * 3,
			tm.tmAveCharWidth * 16 +
			GetSystemMetrics (SM_CXVSCROLL),
			tm.tmHeight * 5,
			hwnd, (HMENU) 1,
			(HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE),
			NULL) ;
		SendMessage(hwndList, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), true);
		return 0 ;

	case WM_COMMAND:
		if(HIWORD(wParam) == LBN_DBLCLK)
		{
			SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)(""));
			SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)("------------------------------------------------------------------------"));
			SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)(""));
			uint32 vSize = SendMessage(hwndList, LB_GETCOUNT, 0, 0);
			SendMessage(hwndList, LB_SETCURSEL, vSize - 1, 0);		
			return 0;
		}
		break;

	case WM_SIZE:
		MoveWindow(hwndList, 0, 0, LOWORD(lParam), HIWORD(lParam), true);
		return 0;

	case WM_SETFOCUS :
		SetFocus (hwndList);
		return 0;

	case WM_DESTROY :
		{
			PostQuitMessage(0);
			return 0 ;
		}
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

#endif //PLATFORM_WINDOWS

//Enters debugger and prints out a message (if supported by the platform)
EM_DEBUG_LIBRARY_DECLSPECIFIER void EMDebugger(const char* p_vpMessage)
{
#ifdef EM_DEBUG
#ifdef PLATFORM_BEOS
	emerr << (char*) p_vpMessage << endl;
	debugger(p_vpMessage);
#endif //PLATFORM_BEOS
#ifdef PLATFORM_WINDOWS

#ifdef DISABLE_OUTPUT
return;
#endif
	OutputDebugString(p_vpMessage);
	MessageBox(NULL, p_vpMessage, "EMDebugger", MB_OK);
	DebugBreak();
#endif //PLATFORM_WINDOWS
#endif //EM_DEBUG
}

EM_DEBUG_LIBRARY_DECLSPECIFIER EMDebugOutput& ef(EMDebugOutput& p_oStream)
{
	return p_oStream.EMFlush();
}

#endif //__EXPORT_EM_DEBUG_LIBRARY
