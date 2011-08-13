/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_VIEW_CONTAINER
#define __EM_WIN_VIEW_CONTAINER

#include "EMRect.h"
#include "EMView.h"

#include <list>
#include <memory>
#include <vector>
#include <windows.h>

class EMBitmap;
class EMListener;
class EMWinMouseCursor;
class EMWinWindow;

class EMWinView : public EMView
{
public:
	EMWinView(const EMRect p_oFrame, uint32 p_vResizingMode, bool p_vDoubleBuffered);
	~EMWinView();
	void AddChild(EMGUIComponent* p_opView);
	bool AddToComponent(EMGUIComponent* p_opComponent);
	void AssertWindowLocked() const;
	void BeginPaint();
	EMRect Bounds() const;
	void CalculateStringHeight(const char* p_vpString, int32 &p_vAscent, int32 &p_vDescent);
	int32 CalculateStringWidth(const char* p_vpString);
	static void CleanUp();
	void ConstrainClippingRect(EMRect p_oRect);
	void ConvertToScreenCoordinates(int32 &p_vX, int32 &p_vY);
	void DrawAndUnlockBuffer(EMRect p_oUpdateRect);
	void DrawBitmap(const EMBitmap* p_opBitmap);
	void DrawBitmap(const EMBitmap* p_opBitmap, int32 p_vX, int32 p_vY);
	void DrawBitmap(const EMBitmap* p_opBitmap, EMRect p_oSource, EMRect p_oDestination);
	void DrawBuffer(EMRect p_oUpdateRect);
	void DrawString(int32 p_vX, int32 p_vY, const char* p_vpText);
	void EnableDoubleBuffering(bool p_vEnabled);
	void EndPaint();
	void FillRect(EMRect p_oRect);
	EMRect Frame() const;
	EMRect GetClippingRect() const;
	EMView* GetLastMouseDownView();
	void GetMouse(int32 &p_vX, int32 &p_vY, bool &p_vButtonOne, bool &p_vButtonTwo, bool &p_vButtonThree) const;
	void* GetNativeView() const;
	uint32 GetResizingMode();
	void GetScreenInfo(int32* p_vpWidth, int32* p_vpHeight, int32* p_vpBitDepth);
	HWND GetWindowHandle();
	EMWinWindow* GetWindowWindow();
	void Hide();
	void Invalidate(EMRect p_oRect);
	bool IsDoubleBuffered();
	bool IsHidden();
	bool IsPainting();
	bool LockBuffer();
	bool LockWindow();
	void ReceiveAllMouseEvents(bool vReceiveAll);
	bool RemoveChild(EMGUIComponent* p_opView);
	//static void ResetMouse();
	void ScrollBy(int32 p_vX, int32 p_vY);
	void ScrollTo(int32 p_vX, int32 p_vY);
	void SetBitmap(EMBitmap* p_opBitmap);
	static void SetDefaultCursor(EMMouseCursorType p_eDefaultCursor);
	void SetDefaultColor();
	void SetDrawingMode(EMDrawingMode p_vMode);
	void SetFont(EMFont p_vFont);
	void SetFrame(EMRect p_oFrame);
	void SetHighColor(const EMColor p_vColor);
	void SetLowColor(const EMColor p_vColor);
	void SetMouseCursor(EMMouseCursorType p_eType, bool p_vAlwaysSucceed);
	void SetViewColor(const EMColor p_vColor);
	void SetWindowWindow(EMWinWindow* p_opWindow);
	void Show();
	void StrokeLine(int32 p_vX1, int32 p_vY1, int32 p_vX2, int32 p_vY2);
	void StrokeRect(EMRect p_oRect);
	void Sync();
	void UpdateBufferFrame();
	bool WindowIsLocked();
	void UnlockBuffer();
	void UnlockWindow();

private:
	void BeginPaint(bool p_vTriggeredFromInvalidate);
	void EndPaint(bool p_vTriggeredFromInvalidate);
	static LRESULT CALLBACK GlobalWindowProcedure(HWND p_vWindowHandle, UINT p_vMessage, WPARAM p_vParameterOne, LPARAM p_vParameterTwo);
	//static LRESULT CALLBACK MouseProc(int p_vCode, WPARAM p_vParameterOne, LPARAM p_vParameterTwo);

private:
	EMMouseCursorType m_eCurrentMouseCursor;
	static EMMouseCursorType m_eDefaultMouseCursor;
	EMDrawingMode m_eDrawingMode;
	EMColor m_oBackgroundColor;
	EMRect m_oBounds;
	list<EMGUIComponent*> m_oChildComponents;
	EMRect m_oClippingRect;
	EMRect m_oFrame;
	static vector<EMWinMouseCursor*> m_oMouseCursors;
	HDC m_opBitmapPaintHandle;
	static HFONT m_opBoldFont;
	HRGN m_opClipRegion;
	HBRUSH m_opCurrentBrush;
	HPEN m_opCurrentPen;
	HDC m_opDoubleBufferedPaintHandle;
	HBITMAP m_opDoubleBufferingBitmap;
	static HFONT m_opFixedFont;
	static HHOOK m_opMouseProcHook;
	static HFONT m_opPlainFont;
	HDC m_opPlainPaintHandle;
	HWND  m_opWindowHandle;
	EMWinWindow* m_opWindowWindow; // The actual window that lies on the desktop
	PAINTSTRUCT m_sPaintStruct;
	int m_vBeginPaintCount;
	uint8 m_vCurrentAlpha;
	COLORREF m_vCurrentHighColor;
	EMFont m_eFont;
	bool m_vIsDoubleBuffered;
	//static bool m_vMouseButtonOne;
	//static bool m_vMouseButtonThree;
	//static bool m_vMouseButtonTwo;
	bool m_vMouseCursorIsSet;
	static bool m_vMouseHookIsSet;
//	int16 m_vMouseX;
//	int16 m_vMouseY;
	uint32 m_vResizingMode;
	int32 m_vScrollOffsetX;
	int32 m_vScrollOffsetY;
};

#endif

#endif
