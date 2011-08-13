/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_VIEW
#define __EM_VIEW

#include "EMGUIComponent.h"
#include "EMGUIGlobals.h"
#include "EMMouseCursor.h"
#include "EMRect.h"

class EMBitmap;

class EMView : public EMGUIComponent
{
public:
	virtual ~EMView();
	virtual void AddChild(EMGUIComponent* p_opView) = 0;
	virtual void AssertWindowLocked() const = 0;
	virtual void BeginPaint() = 0;
	virtual EMRect Bounds() const = 0;
	virtual void CalculateStringHeight(const char* p_vpString, int32 &p_vAscent, int32 &p_vDescent) = 0;
	virtual int32 CalculateStringWidth(const char* p_vpString) = 0;
	virtual void ConstrainClippingRect(EMRect p_oRect) = 0;
	virtual void ConvertToScreenCoordinates(int32 &p_vX, int32 &p_vY) = 0;
	virtual void DrawAndUnlockBuffer(EMRect p_oUpdateRect) = 0;
	virtual void DrawBitmap(const EMBitmap* p_opBitmap) = 0;
	virtual void DrawBitmap(const EMBitmap* p_opBitmap, int32 p_vX, int32 p_vY) = 0;
	virtual void DrawBitmap(const EMBitmap* p_opBitmap, EMRect p_oSource, EMRect p_oDestination) = 0;
//	virtual void DrawBitmapOverlay(const EMBitmap* p_opBitmap, EMRect p_oSource, EMRect p_oDestination) = 0;
	virtual void DrawBuffer(EMRect p_oUpdateRect) = 0;
	virtual void DrawString(int32 p_vX, int32 p_vY, const char* p_vpText) = 0;
	virtual void EnableDoubleBuffering(bool p_vEnabled) = 0;
	virtual void EndPaint() = 0;
	virtual void FillRect(EMRect p_oRect) = 0;
	virtual EMRect GetClippingRect() const = 0;
	virtual EMView* GetLastMouseDownView() = 0;
	virtual void GetMouse(int32 &p_vX, int32 &p_vY, bool &p_vButtonOne, bool &p_vButtonTwo, bool &p_vButtonThree) const = 0;
	virtual void GetScreenInfo(int32* p_vpWidth, int32* p_vpHeight, int32* p_vpBitDepth) = 0;
	virtual void Invalidate(EMRect p_oRect) = 0;
	virtual bool IsDoubleBuffered() = 0;
	virtual bool IsHidden() = 0;
	virtual bool IsPainting() = 0;
	virtual bool LockBuffer() = 0;
	virtual bool LockWindow() = 0;
	virtual void ReceiveAllMouseEvents(bool vReceiveAll) = 0;
	virtual bool RemoveChild(EMGUIComponent* p_opView) = 0;
	virtual void ScrollBy(int32 p_vX, int32 p_vY) = 0;
	virtual void ScrollTo(int32 p_vX, int32 p_vY) = 0;
//	virtual void SetBitmap(EMBitmap* p_opBitmap) = 0;
	virtual void SetDefaultColor() = 0;
	virtual void SetDrawingMode(EMDrawingMode p_vMode) = 0;
	virtual void SetFont(EMFont p_vFont) = 0;
//	virtual void SetFrame(EMRect p_oBounds) = 0;
	virtual void SetHighColor(const EMColor p_vColor) = 0;
	virtual void SetLowColor(const EMColor p_vColor) = 0;
	virtual void SetMouseCursor(EMMouseCursorType p_eType, bool p_vAlwaysSucceed = false) = 0;
//	virtual void SetMousePosition(int32 p_vX, int32 p_vY) = 0;
	virtual void SetViewColor(const EMColor p_vColor) = 0;
	virtual void StrokeLine(int32 p_vX1, int32 p_vY1, int32 p_vX2, int32 p_vY2) = 0;
	virtual void StrokeRect(EMRect p_oRect) = 0;
	virtual void Sync() = 0;
	virtual void UpdateBufferFrame() = 0;
	virtual bool WindowIsLocked() = 0;
	virtual void UnlockBuffer() = 0;
	virtual void UnlockWindow() = 0;
	
protected:
	EMView();
};

#endif
