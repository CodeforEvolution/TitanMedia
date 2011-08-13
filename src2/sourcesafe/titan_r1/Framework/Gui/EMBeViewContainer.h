/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_VIEW_CONTAINER
#define __EM_BE_VIEW_CONTAINER

#include "EMRect.h"
#include "EMView.h"

#include <memory>

class EMBeView;
class EMBitmap;
class EMListener;

class BBitmap;

class EMBeViewContainer : public EMView
{
public:
	EMBeViewContainer(const EMRect p_oFrame, uint32 p_vResizingMode, bool p_vDoubleBuffered);
	~EMBeViewContainer();
	void AddChild(EMGUIComponent* p_opView);
	void AssertWindowLocked();
	EMRect Bounds() const;
	void CalculateStringHeight(const char* p_vpString, float &p_vAscent, float &p_vDescent);
	float CalculateStringWidth(const char* p_vpString);
	void ConstrainClippingRect(const EMRect p_oRect);
	void ConvertToScreenCoordinates(float &p_vX, float &p_vY);
	void DrawAndUnlockBuffer(EMRect p_oUpdateRect);
	void DrawBitmap(const EMBitmap* p_opBitmap);
	void DrawBitmap(const EMBitmap* p_opBitmap, float p_vX, float p_vY);
	void DrawBitmap(const EMBitmap* p_opBitmap, EMRect p_oSource, EMRect p_oDestination);
//	void DrawBitmapOverlay(const EMBitmap* p_opBitmap, EMRect p_oSource, EMRect p_oDestination);
	void DrawBuffer(EMRect p_oUpdateRect);
	void DrawString(float p_vX, float p_vY, const char* p_vpText);
	void EnableDoubleBuffering(bool p_vEnabled);
	void FillRect(const EMRect p_oRect);
	EMRect Frame() const;
	EMRect GetClippingRect() const;
	EMView* GetLastMouseDownView();
	void GetMouse(float &p_vX, float &p_vY, bool &p_vButtonOne, bool &p_vButtonTwo, bool &p_vButtonThree) const;
	void* GetNativeView() const;
	void Hide();
	void Invalidate(const EMRect p_oRect);
	bool IsDoubleBuffered();
	bool IsHidden();
	bool LockBuffer();
	bool LockWindow();
	void ReceiveAllMouseEvents(bool vReceiveAll);
	bool RemoveChild(EMGUIComponent* p_opView);
	void ScrollBy(float p_vX, float p_vY);
	void ScrollTo(float p_vX, float p_vY);
	void SetBitmap(EMBitmap* p_opBitmap);
	void SetDrawingMode(EMDrawingMode p_vMode);
	void SetFont(EMFont p_vFont);
	void SetFrame(EMRect p_oFrame);
	void SetHighColor(const EMColor p_vColor);
	void SetLowColor(const EMColor p_vColor);
//	void SetMousePosition(float p_vX, float p_vY);
	void SetViewColor(const EMColor p_vColor);
	void Show();
	void StrokeLine(float p_vX1, float p_vY1, float p_vX2, float p_vY2);
	void StrokeRect(EMRect p_oRect);
	void Sync();
	void UpdateBufferFrame();
	bool WindowIsLocked();
	void UnlockBuffer();
	void UnlockWindow();

private:
	EMBeView* m_opDrawView; // Points to either m_opBufferView or m_opPlainView depeinding on if the view is double buffered or not
	EMBeView* m_opPlainView; // This class does not retain ownership of its native window
	float m_vBufferScrollOffsetX; // Used to simulate double-buffer scrolling
	float m_vBufferScrollOffsetY; // Used to simulate double-buffer scrolling
	static BBitmap* m_opBuffer; // Double buffering stuff
	static EMBeView* m_opBufferView; // Connected to the double-buffer bitmap
//	auto_ptr<BBitmap> m_opBuffer; // Double buffering stuff
//	auto_ptr<EMBeView> m_opBufferView; // Connected to the double-buffer bitmap
	bool m_vIsDoubleBuffered;
	static int m_vExistingDoubleBufferedViews; // Keeps track of when the double buffered stuff should be deleted
};

#endif

#endif
