/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __AUDIO_G_CLIP
#define __AUDIO_G_CLIP

#include "FXNode.h"
#include "GClip.h"

#include <list>

class EMGClip;
class EMGTrack;
class EMView;

class AudioGClip : public GClip
{
public:
	AudioGClip(string p_oClipName, int64 p_vPosition, int64 p_vLength, EMGTrack* p_opTrack, int32 p_vMediaClipID, EMView* p_opView);
	virtual ~AudioGClip();
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
	void SetLabel(char* p_oLabel, EMView* p_opView);
	//void SetMediaClipID(int32 p_vMediaClipID);
	//void SetTrack(EMGTrack* p_opTrack);
	void UpdateContent();

private:
	void CalculateNamePosition(EMView* p_opView);
	void DrawContent(int64 p_vFileBeginPosition, int64 p_vFileEndPosition, int64 p_vClipBeginPosition, int64 p_vClipEndPosition, int64 p_vZoomScale, bool p_vLowRes, bool p_vClipEdit, bool p_vDrawMovedClip = false);

private:
	EMRect m_oInsideBorder;
	EMRect m_oLabelBackground;
	list<FXNode>* m_opFXNodes;
	int32 m_vAudioSampleRate; // Cached value
	int64 m_vChannels; // Cached value
	bool m_vIsInitialized;
	uint8* m_vLowResContent; // Cached value
	//int32 m_vMediaEffectTrackID;
	bool m_vNamePositionUpdated;
	int64 m_vNrHiResSamples;
	int64 m_vNrLoResSamples;
	int32 m_vStringLeft;
	int32 m_vStringTop;
};

#endif
