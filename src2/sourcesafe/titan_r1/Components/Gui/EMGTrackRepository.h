/*******************************************************
* Creator: Martin Rudefelt
* Component: none
* Portability: non-native
*-------------------------------------------------------
* Contains a list of all graphical tracks in the system. Owns all tracks.
* Operations on tracks listed here should always be done 
* through methods in this class so that cached variables are 
* properly updated. Do not forget to call UpdateCache() in 
* other track repositories that contain some of the tracks 
* available in this repository.
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_G_TRACK_REPOSITORY
#define __EM_G_TRACK_REPOSITORY

#include "EMGUIGlobals.h"
#include "EMListenerRepository.h"
#include "EMMediaGlobals.h"
#include "EMProjectDataContainer.h"
#include "EMRect.h"

#include <map>
#include <string>
#include <vector>

class EMGClipContainer;
class EMGTrack;
class EMGTrackInfo;
class EMView;

/*
#include "EMMediaStructs.h"


class EMView;
*/
class EMGTrackRepository : public EMListenerRepository, public EMProjectDataContainer
{
public:
	~EMGTrackRepository();
	bool ClearData();
	EMGTrack* CreateTrack(string p_oTrackName, EMMediaType p_eType, bool p_vNotify = true);
	void DeleteTrack(EMGTrack* p_opTrack);
	void DeleteTracks(vector<EMGTrack*>* p_opTracks, bool p_vRemoveFromMemory = true);
	EMGTrack* GetClosestTrack(int32 p_vX, int32 p_vY, EMMediaType p_eType, int64 p_vPosition, int64 p_vLength);
	uint32 GetNrTracks();
	uint32 GetNrTracks(EMMediaType p_eType);
	vector<EMGTrack*> GetSelectedTracks();	
	EMGTrack* GetTrack(uint32 p_vIndex); // Try to avoid operating on the track directly, use methods in this class instead if possible
	EMGTrack* GetTrack(int32 p_vX, int32 p_vY);
	int32 GetTrackIndex(EMGTrack* p_opTrack);
	int32 GetTrackIndex(int32 p_vTrackMediaID);
	vector<EMGTrack*>* GetTracks();
	void Init();
	static EMGTrackRepository* Instance();
	bool IsDirty();
	void LoadComplete();
	bool LoadData(void* p_upDataLoader);
	void MoveTracks(vector<EMGTrack*> p_oTracks, uint32 p_vBeforeIndex, uint32 p_vAfterIndex);
	void ReadyForUse();
	void RegisterTrackType(EMMediaType p_eType, EMGTrackInfo* p_opTrackInfo, EMGClipContainer* p_opClipContainer);
	bool SaveData(void* p_upDataSaver);
	void SetTrackOrder(vector<EMGTrack*>* p_opTrackOrder);
	void SetTracks(vector<EMGTrack*>* p_opTracks);
	void TrackSelectionHasChanged();
	void TrackZoomHasChanged();

protected:
	EMGTrackRepository();

private:
	EMRect m_oTrackInfoFrame;
	static EMGTrackRepository* m_opInstance;
	vector<EMGTrack*> m_oTrackRepository;
	map<EMMediaType, EMGTrackInfo*> m_oTrackInfoTypes;
	map<EMMediaType, EMGClipContainer*> m_oClipContainerTypes;
	list<bool> m_oSelectedTracks; // Teporarily stored during project load
	list<int32> m_oTrackOrder;  // Teporarily stored during project load
	list<uint8> m_oTrackSize;  // Teporarily stored during project load
	bool m_vIsDirty;
};

#endif
