/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_G_TRACK_INFO
#define __EM_G_TRACK_INFO

#include "EMGUIGlobals.h"
#include "EMListenerRepository.h"
#include "EMMediaGlobals.h"
#include "EMRect.h"

#include <list>
#include <string>

class EMGTrack;
class EMView;
//class MuteButton;
//class SoloButton;

class EMGTrackInfo : public EMListenerRepository
{
public:
//	virtual ~EMGTrackInfo();
//	virtual void CalculateAndSetHidden(bool p_vClipEdit) = 0;
	bool Contains(int32 p_vX, int32 p_vY);
	virtual EMGTrackInfo* CreateInstance() = 0;
	virtual void Draw(EMRect p_oClippingRect, EMView* p_opView) = 0;
	virtual void EditModeChanged(EMEditMode p_eEditMode);
	EMRect Frame();
	int32 GetMediaTrackID() const;
//	virtual MuteButton* GetMuteButton() = 0;
//	virtual SoloButton* GetSoloButton() = 0;
	EMGTrack* GetTrack();
	virtual string GetTrackName() = 0;
	virtual EMMediaType GetType() = 0;
	EMRect GetUpdateRect();
	bool Intersects(EMRect p_oFrame);
//	bool IsClipEdit();
	virtual bool IsHidden(bool p_vClipEdit) = 0;
	virtual bool IsSelected() = 0;
	virtual void LayoutComponents() = 0;
	virtual void MouseDown(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree, EMView* p_opView) = 0;
	virtual void MouseDoubleClick(int32 p_vX, int32 p_vY, EMView* p_opView) = 0;
	virtual void MouseMoved(int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree) = 0;
	virtual void MouseUp(int32 p_vX, int32 p_vY) = 0;
	void OffsetFrame(int32 p_vX, int32 p_vY);
	void SetFrame(EMRect p_oFrame);
	void SetHidden(bool p_vHidden);
	virtual void SetMediaTrackID(int32 p_vMediaTrackID);
	virtual void SetSelected(bool p_vIsSelected, bool p_vUpdate = true) = 0;
//	virtual void SetSignal(list<int32>* p_vSignal) = 0;
	virtual void SetTrack(EMGTrack* p_opTrack);
	virtual void SetTrackName(string p_oName) = 0;

protected:
	EMGTrackInfo();//EMGTrack* p_opTrack);

protected:
	EMRect m_oUpdateRect;
	int32 m_vMediaTrackID;
//	bool m_vClipEdit;

private:
	EMRect m_oFrame;
	EMGTrack* m_opTrack;
	bool m_vHidden;
};

#endif



