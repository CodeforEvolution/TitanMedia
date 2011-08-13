/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* This class assumes that all modifying methods are called
* through the class EMGClipRepository (or things will go poo poo)
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_G_CLIP
#define __EM_G_CLIP

#include "EMMediaGlobals.h"
#include "EMRect.h"

#include <string>

class EMGTrack;
class EMView;

class EMGClip
{
public:
	virtual ~EMGClip();
	virtual EMRect CalculateFrame(uint32 p_vTrackOffset, int64 p_vClipOffsetX);
	void CalculateOkToDrop();
	virtual EMGClip* CloneClip(int32 p_vTrackOffset, int64 p_vPositionOffset) = 0;
	virtual EMGClip* CreateInstance(string p_oClipName, int64 p_vPosition, int64 p_vLength, EMGTrack* p_opTrack, int32 p_vMediaClipID, EMView* p_opView) = 0;
	virtual void DrawClip(EMRect p_oClippingRect, EMView* p_opView, bool p_vClipEditMode, int64 p_vZoomScale, bool p_vDrawContent) = 0;
//	virtual void DrawContent(int64 p_vBeginPosition, int64 p_vEndPosition, int64 p_vZoomScale, bool p_vLowRes, bool p_vClipEdit) = 0;
	virtual void DrawMovedClip(EMRect p_oClippingRect, EMView* p_opView, int64 p_vZoomScale, bool p_vDrawContent) = 0;
	EMRect Frame();
	EMGClip* GetFirstInGroup();
	string GetLabel();
	int64 GetLength();
	int32 GetMediaClipID() const;
	bool GetMuted();
	EMGClip* GetNextInGroup();
	int64 GetPosition();
	bool GetSelected() const;
	EMGTrack* GetTrack();
	virtual EMMediaType GetType() = 0;
//	bool IsHidden();
	virtual bool MoveClip(int32 p_vTrackOffset, int64 p_vPositionOffset);
	virtual bool MouseDown(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree) = 0;
	virtual bool MouseMoved(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree) = 0;
	virtual bool MouseUp(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree) = 0;
	EMRect MovedFrame() const;
	bool OkToDrop();
	void SetFirstInGroup(EMGClip* p_opClip);
	virtual void SetFrame(EMRect m_oRect);
	virtual void SetLabel(char* p_oLabel, EMView* p_opView);
//	void SetHidden(bool p_vHidden);
//	virtual void SetMediaClipID(int32 p_vMediaClipID);
	void SetMovedFrame(EMRect p_oFrame, int64 p_vPositionMovedOffset, int32 p_vTrackOffset);
	void SetMuted(bool p_vMuted);
	void SetNextInGroup(EMGClip* p_opClip);
	void SetSelected(bool p_vSelected);
	void SetTrack(EMGTrack* p_opTrack);
	virtual EMGClip* SplitClip(int64 p_vPosition, int64 p_vTimeZoomScale) = 0;
	virtual void UpdateContent() = 0;
	virtual void UpdateFrame() = 0;

protected:
	EMGClip(string p_oClipName, int64 p_vPosition, int64 p_vLength, EMGTrack* p_opTrack, int32 p_vMediaClipID, EMView* p_opView);

protected:
	EMRect m_oFrame;
	EMRect m_oMovedFrame;
	string m_oName;
	EMGClip* m_opFirstInGroup;
	EMGClip* m_opNextInGroup;
	EMGTrack* m_opTrack;
	EMView* m_opView;
	bool m_vHidden;
	int64 m_vLength;
	bool m_vOkToDrop;
	int32 m_vMediaClipID;
	bool m_vMuted;
	int64 m_vPosition; // Cached value
	int64 m_vPositionMovedOffset;
	bool m_vSelected;
	int32 m_vTrackOffset;

/*
	virtual void DrawMovedClip(EMRect p_oClippingRect, EMView* p_opView, int64 p_vPositionMovedOffset);	
	sEMClipInfo GetClipInfo() const;
	string* GetLabel() const;
	int64 GetLength();
//	string* GetPath();
	bool MoveClip(int32 p_vTrackOffset, int64 p_vPositionOffset);
//	virtual void MouseDown(EMGClip* p_opClip, int32 p_vX, int32 p_vY);
//	virtual void MouseUp(int32 p_vX, int32 p_vY);

protected:
	EMGClip(string* p_opClipName, em_media_type p_vType, uint64 p_vPosition, sEMClipInfo p_sClipInfo, EMGTrack* p_opTrack);

protected:
	string* m_opLabel;
//	string* m_opPath;
	EMGTrack* m_opTrack;
	sEMClipInfo m_sClipInfo;
	em_media_type m_vMediaType;
	int64 m_vPosition;
*/
};

#endif
