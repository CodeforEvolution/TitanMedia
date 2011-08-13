/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __G_CLIP
#define __G_CLIP

#include "EMGClip.h"
#include "GUIGlobals.h"

#include <string>

class EMGTrack;
class EMView;

class GClip : public EMGClip
{
public:
	GClip(string p_oClipName, int64 p_vPosition, int64 p_vLength, EMGTrack* p_opTrack, int32 p_vMediaClipID, EMView* p_opView);
	//int64 GetLength();
	bool MouseDown(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	bool MouseMoved(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	bool MouseUp(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	EMGClip* SplitClip(int64 p_vPosition, int64 p_vTimeZoomScale);
	void UpdateFrame();

protected:
	//ToolboxMode m_eToolboxMode;
	EMRect m_oOriginalFrame;
//	int64 m_vClipMarkIn; // Cached value
//	int64 m_vClipMarkOut; // Cached value
	int64 m_vFileLength;
	int32 m_vMouseDownX;
	int32 m_vMouseDownY;
	bool m_vMouseIsDown;
	int64 m_vOriginalLength;
	int64 m_vOriginalMarkIn;
	int64 m_vOriginalMarkOut;
	int64 m_vOriginalPosition;
	bool m_vResizeLeft;
	bool m_vResizeRight;
	int64 m_vTimeZoomScale; // Cached value

private:
	EMGClip* m_opNextClip; // Used for resizing purposes
	EMGClip* m_opPreviousClip; // Used for resizing purposes
};

#endif