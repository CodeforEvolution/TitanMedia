/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* The track is not responsible for updating its clips, this
* should instead be handled by the caller
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_G_TRACK
#define __EM_G_TRACK

#include "EMGClipContainer.h"
#include "EMGUIGlobals.h"
#include "EMGTrackInfo.h"
#include "EMMediaGlobals.h"
#include "EMRect.h"

#include <string>

/*
#include "EMGClip.h"
#include "EMGUIGlobals.h"

#include <list>

class EMView;
*/
class EMGClipContainer;
class EMGTrackInfo;
class EMView;

class EMGTrack
{
public:
	virtual ~EMGTrack();
	static EMGTrack* CreateTrack(string p_oTrackName, EMGTrackInfo* p_opTrackInfo, EMGClipContainer* p_opClipContainer);
	bool DecreaseZoomScale();
	EMMediaType GetType();
	EMGClipContainer* GetClipContainer();
	EMGTrackInfo* GetTrackInfo();
	EMZoomScale GetZoomScale();
	bool IncreaseZoomScale();
	void SetZoomScale(EMZoomScale p_eZoomScale);
	
protected:
	EMGTrack(EMGTrackInfo* p_opTrackInfo, EMGClipContainer* p_opClipContainer);

private:
	EMZoomScale m_eEditZoomScale;
	EMZoomScale m_eZoomScale;
	EMGClipContainer* m_opClipContainer;
	EMGTrackInfo* m_opTrackInfo;
//	bool m_vHidden;
};

#endif
