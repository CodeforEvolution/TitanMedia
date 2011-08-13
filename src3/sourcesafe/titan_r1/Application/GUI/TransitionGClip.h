/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __TRANSITION_G_CLIP
#define __TRANSITION_G_CLIP

//#include "EMGClip.h"
#include "GClip.h"

class EMGClip;
class EMGTrack;
class EMView;

class TransitionGClip : public GClip
{
public:
	TransitionGClip(string p_oClipName, int64 p_vPosition, int64 p_vLength, EMGTrack* p_opTrack, int32 p_vMediaClipID, EMView* p_opView);
	virtual ~TransitionGClip();
	EMRect CalculateFrame(uint32 p_vTrackOffset, int64 p_vClipOffsetX);
	EMGClip* CloneClip(int32 p_vTrackOffset, int64 p_vPositionOffset);
	EMGClip* CreateInstance(string p_oClipName, int64 p_vPosition, int64 p_vLength, EMGTrack* p_opTrack, int32 p_vMediaClipID, EMView* p_opView);
	void DrawClip(EMRect p_oClippingRect, EMView* p_opView, bool p_vClipEditMode, int64 p_vZoomScale, bool p_vDrawContent);
	void DrawMovedClip(EMRect p_oClippingRect, EMView* p_opView, int64 p_vZoomScale, bool p_vDrawContent);
	int64 GetLength();
	EMMediaType GetType();
//	void MouseDown(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
//	void MouseMoved(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
//	void MouseUp(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	bool MoveClip(int32 p_vTrackOffset, int64 p_vPositionOffset);
//	void SetFrame(EMRect p_oFrame);
	void SetLabel(char* p_oLabel, EMView* p_opView);
//	void SetMarkInAndMarkOut(int64 p_vMarkIn, int64 p_vMarkOut);
	EMGClip* SplitClip(int64 p_vPosition, int64 p_vTimeZoomScale);
	void UpdateContent();

private:
	void CalculateNamePosition(EMView* p_opView);

private:
	EMRect m_oInsideBorder;
	EMRect m_oLabelBackground;
	bool m_vIsInitialized;
	bool m_vNamePositionUpdated;
	int32 m_vStringLeft;
	int32 m_vStringTop;
};

#endif
