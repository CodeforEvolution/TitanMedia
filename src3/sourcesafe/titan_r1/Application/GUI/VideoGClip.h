/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __VIDEO_G_CLIP
#define __VIDEO_G_CLIP

#include "FXNode.h"
#include "GClip.h"

#include <vector>

class EMGTrack;
class EMImageChunk;
class EMView;

class VideoGClip : public GClip
{
public:
	VideoGClip(string p_oClipName, int64 p_vPosition, int64 p_vLength, EMGTrack* p_opTrack, int32 p_vMediaClipID, EMView* p_opView);
	virtual ~VideoGClip();
	EMRect CalculateFrame(uint32 p_vTrackOffset, int64 p_vClipOffsetX);
	EMGClip* CloneClip(int32 p_vTrackOffset, int64 p_vPositionOffset);
	EMGClip* CreateInstance(string p_oClipName, int64 p_vPosition, int64 p_vLength, EMGTrack* p_opTrack, int32 p_vMediaClipID, EMView* p_opView);
	void DrawClip(EMRect p_oClippingRect, EMView* p_opView, bool p_vClipEditMode, int64 p_vZoomScale, bool p_vDrawContent);
	void DrawMovedClip(EMRect p_oClippingRect, EMView* p_opView, int64 p_vZoomScale, bool p_vDrawContent);
	void EnableClipFX(EMView* p_opView, int32 p_vMediaTrackID);
	list<FXNode>* GetFXNodes();
	EMMediaType GetType();
//	void MouseMoved(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	bool MoveClip(int32 p_vTrackOffset, int64 p_vPositionOffset);
	void SetFrame(EMRect p_oFrame);
	void SetFramerate(float p_vFramerate);
	void SetLabel(char* p_oLabel, EMView* p_opView);
	void UpdateContent();

private:
	void CalculateNamePosition(EMView* p_opView);
	void DrawContent(int64 p_vFileBeginPosition, int64 p_vFileEndPosition, int64 p_vClipBeginPosition, int64 p_vClipEndPosition, int64 p_vZoomScale, bool p_vClipEdit, bool p_vDrawMovedClip);

private:
	EMRect m_oInsideBorder;
	EMRect m_oLabelBackground;
	list<FXNode>* m_opFXNodes;
	vector<EMImageChunk*>* m_opChunks;
	float m_vAspectRatio;
	float m_vFramerate; // Cached setting
	int32 m_vFrameWidth; // Cached value
	//int32 m_vMediaEffectTrackID;
	bool m_vNamePositionUpdated;
	int m_vSkipRate; // Cached value
	int32 m_vStringLeft;
	int32 m_vStringTop;
	int64 m_vTimeZoomScale; // Cached value
};

#endif
