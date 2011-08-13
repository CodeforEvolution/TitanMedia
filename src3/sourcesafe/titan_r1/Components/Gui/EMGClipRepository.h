/*******************************************************
* Creator: Martin Rudefelt
* Component: none
* Portability: non-native
*-------------------------------------------------------
* Contains a list of all graphical clips in the system. Owns all clips.
* Operations on clips listed here should always be done 
* through methods in this class so that cached variables are 
* properly updated. Do not forget to call UpdateCache() in 
* other clip repositories that contain some of the clips 
* available in this repository.
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_G_CLIP_REPOSITORY
#define __EM_G_CLIP_REPOSITORY

#include "EMListenerRepository.h"
#include "EMMediaGlobals.h"
#include "EMProjectDataContainer.h"
#include "EMRect.h"

#include <list>
#include <map>
#include <string>

class EMGClip;
class EMGTrack;
class EMView;

class EMGClipRepository : public EMListenerRepository, public EMProjectDataContainer
{
public:
	~EMGClipRepository();
	void AddClipGroup(EMGClip* p_opClipGroupOwner);
	void AddClips(list<EMGClip*>* p_opClipList, bool p_vUpdate = true);
	void CalculateFrame();
	bool CalculateMovedFrame(uint32 p_vTrackOffset, int64 p_vPositionMovedOffset);
	void CalculateSelectedFrame();
	bool ClearData();
	void ClearMovedFrame();
	void ClearSelectedClips();
	void CloneClips(list<EMGClip*>* p_opClips, int32 p_vTrackOffset, int64 p_vPositionOffset, list<EMGClip*>* p_opNewClips = NULL);
	EMGClip* CreateClip(string p_oClipName, EMMediaType p_vType, int64 p_vPosition, int64 p_vLength, EMGTrack* p_opTrack, int32 p_vMediaClipID, EMView* p_opView, bool p_vUpdate = true);
	void DeleteClips(list<EMGClip*>* p_opClips, bool p_vRemoveFromMemory = true, bool p_vNotify = true);
	void DrawClips(list<EMGClip*>* p_opClips, EMRect p_oUpdateRect, EMView* p_opView, bool p_vClipEditMode, int64 m_vTimeZoomScale, bool p_vDrawContent);
	void DrawMovedClips(list<EMGClip*>* p_opClips, const EMRect p_oUpdateRect, EMView* p_opView, int64 m_vPositionMovedOffset, int64 p_vZoomScale, bool p_vDrawContent);
	EMRect Frame() const;
	EMRect GetAndClearUpdateRect();
	EMGClip* GetClip(int32 p_vX, int32 p_vY); // Try to avoid operating on the clip directly, use methods in this class instead if possible
	//list<EMGClip*> GetClipGroup(EMGClip* p_opClip, bool* p_vpHiddenClipsExist);
	//list<EMGClip*> GetClipGroup(list<EMGClip*>* p_opClips, bool* p_vpHiddenClipsExist);
	list<EMGClip*>* GetClips();
	EMGClip* GetClip(int32 p_vMediaClipID);
	void GetClips(EMGTrack* p_opTrack, list<EMGClip*>* p_opClips);
	list<EMGClip*>* GetClips(EMMediaType p_eType);
	int64 GetEarliestSelectedPosition();
	int64 GetLatestSelectedPosition();
	EMGClip* GetNextClipInTrack(EMGClip* p_opClip);
	uint32 GetNrClips() const;
	uint32 GetNrSelectedClips() const;
	EMGClip* GetPreviousClipInTrack(EMGClip* p_opClip);
	int64 GetProjectEnd() const;
	list<EMGClip*>* GetSelectedClips();
	static EMGClipRepository* Instance();
	bool IsDirty();
	void LoadComplete();
	bool LoadData(void* p_upDataLoader);
	void MoveClips(list<EMGClip*>* p_opClips, int32 p_vTrackOffset, int64 p_vPositionOffset);
	void MoveClipsToTempo(list<EMGClip*>* p_opClips, float p_vOldTempo, float p_vNewTempo);
	EMRect MovedFrame() const;
	bool OkToDrop(list<EMGClip*>* p_opClips);
	//void PreventOverlapping(list<EMGClip*>* p_opClips);
	void PrintStatus(); // Debug
	void ReadyForUse();
	void RegisterClipType(EMMediaType p_eType, EMGClip* p_opClip);
	void RemoveClipGroup(EMGClip* p_opClipGroupOwner);
	bool SaveData(void* p_upDataSaver);
	EMRect SelectedFrame();
	void SetClipPosition(EMGClip* p_opClip, int64 p_vPosition, bool p_vTriggerNotifyAndSort = true);
	void SetClipTrack(EMGClip* p_opClip, EMGTrack* p_opTrack);
	void SetSelected(EMGClip* p_opClip, bool p_vSelected);
	void SetSelected(list<EMGClip*>* p_opClips, bool p_vSelected);
	void SetSelected(EMRect p_oRect, bool p_vIntersectOK);
	list<EMGClip*>* SplitClips(list<EMGClip*>* p_opClips, int64 p_vPosition, int64 p_vTimeZoomScale);
	void Update();

protected:
	EMGClipRepository();
	
private:
	EMRect CalculateFrame(list<EMGClip*>* p_opClips);
	//void SortClips();

private:
	list<EMGClip*> m_oClipGroupOwners;
	list<EMGClip*> m_oClipRepository;
	map<EMMediaType, list<EMGClip*> > m_oClipsByTypeRepository;
	map<EMMediaType, EMGClip*> m_oClipTypes;
	EMRect m_oFrame; // Cached frame of the clips
	list<int32> m_oLoadedData; // Temporary load cache
	EMRect m_oMovedFrame; // Cached frame of the moved clips (if any)
	static EMGClipRepository* m_opInstance;
	list<EMGClip*> m_oSelectedClipRepository;
	EMRect m_oSelectedFrame; // Cached frame of the selected clips (if any)
	EMRect m_oUpdateRect;
	int64 m_vEarliestSelectedPosition;
	bool m_vIsDirty;
	int64 m_vLatestSelectedPosition; // The position + length - 1 of the last selected clip
	int64 m_vProjectEnd;
};

#endif
