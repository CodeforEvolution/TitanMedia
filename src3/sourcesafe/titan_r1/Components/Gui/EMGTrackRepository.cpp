#include "EMGTrackRepository.h"

#include "AudioGClipContainer.h" // FIXME: app class in FW
#include "CommandIDs.h" // FIXME: app class in FW
#include "EMCommandRepository.h"
#include "EMGClipContainer.h"
#include "EMGClipRepository.h"
#include "EMGTrack.h"
#include "EMGTrackInfo.h"
#include "EMGUIGlobals.h"
#include "EMMediaGlobals.h"
#include "EMMessages.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMView.h"
#include "TransitionGTrackInfo.h" // FIXME: app class in FW
#include "VideoGClipContainer.h" // FIXME: app class in FW
#include "VideoGTrackInfo.h" // FIXME: app class in FW

#include <algorithm>

EMGTrackRepository* EMGTrackRepository::m_opInstance = NULL;

EMGTrackRepository::EMGTrackRepository() :
m_vIsDirty(false)
{
}

EMGTrackRepository::~EMGTrackRepository()
{
	delete m_oTrackInfoTypes[EM_TYPE_ANY_VIDEO];
	delete m_oTrackInfoTypes[EM_TYPE_ANY_AUDIO];
	delete m_oTrackInfoTypes[EM_TYPE_MIDI];
	delete m_oTrackInfoTypes[EM_TYPE_TRANSITION];

	delete m_oClipContainerTypes[EM_TYPE_ANY_VIDEO];
	delete m_oClipContainerTypes[EM_TYPE_ANY_AUDIO];
	delete m_oClipContainerTypes[EM_TYPE_MIDI];
	delete m_oClipContainerTypes[EM_TYPE_TRANSITION];
}


bool EMGTrackRepository::ClearData()
{
eo << "EMGTrackRepository::ClearData" << ef;
	vector<EMGTrack*>::iterator oIterator = m_oTrackRepository.begin();
	EMGTrack* opTrack;
	while(oIterator != m_oTrackRepository.end())
	{
		opTrack = *oIterator;
		oIterator++;
		opTrack -> GetClipContainer() -> ClearClips();
		delete opTrack;
	}
	m_oTrackRepository.clear();
	return true;
;//cout_commented_out_4_release << "EMGTrackRepository::ClearData completed" << endl;
}

EMGTrack* EMGTrackRepository::CreateTrack(string p_oTrackName, EMMediaType p_eType, bool p_vNotify)
{
	if(m_oTrackInfoTypes[p_eType] == NULL || m_oClipContainerTypes[p_eType] == NULL)
		EMDebugger("There are no classes registered for this media type");
		
//	EMColor sColor = EMTrackView::Instance() -> CalculateTrackColor(vIndex);
	EMGTrack* opTrack = EMGTrack::CreateTrack(/*EM_new EMGTrack(*/p_oTrackName, m_oTrackInfoTypes[p_eType] -> CreateInstance(), m_oClipContainerTypes[p_eType] -> CreateInstance());
//	EMGClipRepository::Instance() -> AddListener(opTrack -> GetClipContainer());
	m_oTrackRepository.push_back(opTrack);
	if(p_vNotify)
		Notify(EM_TRACK_REPOSITORY_UPDATED);
//	CalculateClipContainerFrame();
//	CalculateTrackInfoFrame();
	m_vIsDirty = true;
	
	return opTrack;
}

void EMGTrackRepository::DeleteTrack(EMGTrack* p_opTrack)
{
//	EMGClipRepository::Instance() -> RemoveListener(p_opTrack -> GetClipContainer());
	m_oTrackRepository.erase(find(m_oTrackRepository.begin(), m_oTrackRepository.end(), p_opTrack));
	delete p_opTrack;
//	CalculateClipContainerFrame();
//	CalculateTrackInfoFrame();
	Notify(EM_TRACK_REPOSITORY_UPDATED);
	m_vIsDirty = true;
}

void EMGTrackRepository::DeleteTracks(vector<EMGTrack*>* p_opTracks, bool p_vRemoveFromMemory)
{
	EMGTrack* opTrack;
	vector<EMGTrack*> oTracksToDelete = *p_opTracks;
//	int32 vMediaTrackID;
	for(uint32 vIndex = 0; vIndex < oTracksToDelete.size(); vIndex++)
	{
		opTrack = oTracksToDelete[vIndex];
//		vMediaTrackID = opTrack -> GetTrackInfo() -> GetMediaTrackID();
//		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_DELETE_TRACK, &vMediaTrackID);
		m_oTrackRepository.erase(find(m_oTrackRepository.begin(), m_oTrackRepository.end(), oTracksToDelete[vIndex]));
		if(p_vRemoveFromMemory)
			delete opTrack;
	}
	Notify(EM_TRACK_REPOSITORY_UPDATED);

	m_vIsDirty = true;
}

EMGTrack* EMGTrackRepository::GetClosestTrack(int32 p_vX, int32 p_vY, EMMediaType p_eType, int64 p_vPosition, int64 p_vLength)
{
	if(m_oTrackRepository.size() == 0)
		return NULL;

	bool vClipEdit = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EDIT_MODE))) == EM_CLIP_EDIT_MODE;
	vector<EMGTrack*>::iterator oIterator = m_oTrackRepository.begin();

	while(oIterator != m_oTrackRepository.end())
	{
		if(((*oIterator) -> GetClipContainer() -> Frame()).Contains(p_vX, p_vY) && !((*oIterator) -> GetTrackInfo() -> IsHidden(vClipEdit)))
			if((*oIterator) -> GetTrackInfo() -> GetType() == p_eType && (p_eType != EM_TYPE_ANY_VIDEO || (*oIterator) -> GetClipContainer() -> OkToDrop(NULL, p_vPosition, p_vLength, true)))
				return (*oIterator);
		oIterator++;
	}

	oIterator = m_oTrackRepository.begin();

	EMRect oFrame;
	uint32 vClosestY = UINT32_MAX;
	EMGTrack* opClosestTrack = NULL;

	while(oIterator != m_oTrackRepository.end())
	{
		if((*oIterator) -> GetTrackInfo() -> GetType() == p_eType && (p_eType != EM_TYPE_ANY_VIDEO || (*oIterator) -> GetClipContainer() -> OkToDrop(NULL, p_vPosition, p_vLength, true)))
		{
			oFrame = (*oIterator) -> GetClipContainer() -> Frame();
			if(abs(static_cast<int>(oFrame.m_vTop - p_vY)) < vClosestY)
			{
				vClosestY = abs(static_cast<int>(oFrame.m_vTop - p_vY));
				opClosestTrack = *oIterator;
			}
			if(abs(static_cast<int>(oFrame.m_vBottom - p_vY)) < vClosestY)
			{
				vClosestY = abs(static_cast<int>(oFrame.m_vTop - p_vY));
				opClosestTrack = *oIterator;
			}
		}
		oIterator++;
	}

	return opClosestTrack;
}

uint32 EMGTrackRepository::GetNrTracks()
{
	return m_oTrackRepository.size();
}

uint32 EMGTrackRepository::GetNrTracks(EMMediaType p_eType)
{
	uint32 vTrackCounter(0);
	vector<EMGTrack*>::iterator oIterator = m_oTrackRepository.begin();
	while(oIterator != m_oTrackRepository.end())
	{
		if((*oIterator) -> GetTrackInfo() -> GetType() == p_eType)
			vTrackCounter++;
		oIterator++;
	}

	return vTrackCounter;
}

vector<EMGTrack*> EMGTrackRepository::GetSelectedTracks()
{
	vector<EMGTrack*> oSelectedTracks;
	vector<EMGTrack*>::iterator oIterator = m_oTrackRepository.begin();
	while(oIterator != m_oTrackRepository.end())
	{
		if((*oIterator) -> GetTrackInfo() -> IsSelected())
			oSelectedTracks.push_back(*oIterator);
		oIterator++;
	}
	return oSelectedTracks;
}

EMGTrack* EMGTrackRepository::GetTrack(uint32 p_vIndex)
{
	if(p_vIndex >= m_oTrackRepository.size())
		return NULL;
	return m_oTrackRepository[p_vIndex];
}

EMGTrack* EMGTrackRepository::GetTrack(int32 p_vX, int32 p_vY)
{
	bool vClipEdit = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EDIT_MODE))) == EM_CLIP_EDIT_MODE;
	vector<EMGTrack*>::iterator oIterator = m_oTrackRepository.begin();
	while(oIterator != m_oTrackRepository.end())
	{
		if(((*oIterator) -> GetClipContainer() -> Frame()).Contains(p_vX, p_vY) && !((*oIterator) -> GetTrackInfo() -> IsHidden(vClipEdit)))
		{
//eo << "Track (clip container) found at: (" << p_vX << ", " << p_vY << ")" << ef;
			return *oIterator;
		}
		oIterator++;
	}
	oIterator = m_oTrackRepository.begin();
	while(oIterator != m_oTrackRepository.end())
	{
		if(((*oIterator) -> GetTrackInfo() -> Frame()).Contains(p_vX, p_vY) && !((*oIterator) -> GetTrackInfo() -> IsHidden(vClipEdit)))
		{
//eo << "Track (track info) found at: (" << p_vX << ", " << p_vY << ")" << ef;
			return *oIterator;
		}
		oIterator++;
	}
//eo << "No track found (GRONK!!!)" << ef;
	;//cout_commented_out_4_release << "****** GRONK!!! ******: (" <<  p_vX << ", " <<  p_vY << ") != ";
	return NULL;
}

int32 EMGTrackRepository::GetTrackIndex(EMGTrack* p_opTrack)
{
	vector<EMGTrack*>::iterator oIterator = m_oTrackRepository.begin();
	int32 vIndex = 0;
	while(oIterator != m_oTrackRepository.end())
	{
		if(*oIterator == p_opTrack)
			return vIndex;
		oIterator++;
		vIndex++;
	}
	return -1; // Throw exception instead?
}

int32 EMGTrackRepository::GetTrackIndex(int32 p_vTrackMediaID)
{
	vector<EMGTrack*>::iterator oIterator = m_oTrackRepository.begin();
	int32 vIndex = 0;
	while(oIterator != m_oTrackRepository.end())
	{
		if((*oIterator) -> GetTrackInfo() -> GetMediaTrackID() == p_vTrackMediaID)
			return vIndex;
		oIterator++;
		vIndex++;
	}
	return -1; // Throw exception instead?
}

vector<EMGTrack*>* EMGTrackRepository::GetTracks()
{
	return &m_oTrackRepository;
}

void EMGTrackRepository::Init()
{
	Notify(EM_TRACK_REPOSITORY_UPDATED);
}

EMGTrackRepository* EMGTrackRepository::Instance()
{
	if(m_opInstance != NULL)
		return m_opInstance;		
	m_opInstance = EM_new EMGTrackRepository();
	return m_opInstance;
}

bool EMGTrackRepository::IsDirty()
{
	return m_vIsDirty;
}

void EMGTrackRepository::LoadComplete()
{
eo << "EMGTrackRepository::LoadComplete" << ef;

	// Starting to query the media system for track information
	EMMediaTrackInfo* opMediaTrackInfo;
	list<int32> oMediaTrackIDs = *(static_cast<list<int32>*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ALL_TRACK_IDS, NULL, NULL, NULL)));
	EMGTrack* opTrack;
	EMCommandRepository* opCommandRepository = EMCommandRepository::Instance();
	EMGTrack* opPreviousTrack = NULL;

	list<int32>::iterator oOrderIterator = m_oTrackOrder.begin();
	list<bool>::iterator oSelectedIterator = m_oSelectedTracks.begin();
	list<uint8>::iterator oSizeIterator = m_oTrackSize.begin();

	while(oOrderIterator != m_oTrackOrder.end())
	{
		opMediaTrackInfo = static_cast<EMMediaTrackInfo*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_TRACK_INFO, &(*oOrderIterator), NULL));
		if(opMediaTrackInfo -> IsAudio())
			opTrack = static_cast<EMGTrack*>(opCommandRepository -> ExecuteCommand(COMMAND_ADD_G_AUDIO_TRACK, &(opMediaTrackInfo -> m_oName), &(opMediaTrackInfo -> m_vID)));
		else if(opMediaTrackInfo -> IsVideo())
			opTrack = static_cast<EMGTrack*>(opCommandRepository -> ExecuteCommand(COMMAND_ADD_G_VIDEO_TRACK, &(opMediaTrackInfo -> m_oName), &(opMediaTrackInfo -> m_vID)));
		else if(opMediaTrackInfo -> IsMIDI())
			opTrack = static_cast<EMGTrack*>(opCommandRepository -> ExecuteCommand(COMMAND_ADD_G_MIDI_TRACK, &(opMediaTrackInfo -> m_oName), &(opMediaTrackInfo -> m_vID)));
		else if(opPreviousTrack != NULL) // Transition track (previous track must be a valid video track)
		{
			opTrack = static_cast<EMGTrack*>(opCommandRepository -> ExecuteCommand(COMMAND_ADD_G_TRANSITION_TRACK, &(opMediaTrackInfo -> m_oName), &(opMediaTrackInfo -> m_vID)));

			if(opTrack != NULL)
			{
				// Set the parent video track for transition tracks and vice versa, assume that transition tracks always are ordered one step after its parent video track
				dynamic_cast<VideoGTrackInfo*>(opPreviousTrack -> GetTrackInfo()) -> SetTransitionTrack(opTrack);
				dynamic_cast<TransitionGTrackInfo*>(opTrack -> GetTrackInfo()) -> SetVideoTrack(opPreviousTrack);
			}
		}

		if(opTrack != NULL)
		{
			opTrack -> GetTrackInfo() -> SetSelected(*oSelectedIterator);
			opTrack -> SetZoomScale(static_cast<EMZoomScale>(*oSizeIterator));
;//cout_commented_out_4_release << "Setting track parameters for track: " << opMediaTrackInfo -> m_oName << ", size: " << static_cast<int16>(*oSizeIterator) << ", selected: " << (*oSelectedIterator) << endl;

			// Let's build up the effects
			if(opMediaTrackInfo -> IsAudio() || opMediaTrackInfo -> IsVideo())
			{
				int32 vEffectTrackID = *(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_EFFECT_TRACK_ID, &(*oOrderIterator))));
				EMMediaEffectTrackInfo* opEffectTrackInfo = static_cast<EMMediaEffectTrackInfo*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_EFFECT_TRACK_INFO, &vEffectTrackID));
				if(opMediaTrackInfo -> IsAudio())
					dynamic_cast<AudioGClipContainer*>(opTrack -> GetClipContainer()) -> BuildFXNodes(&(opEffectTrackInfo -> m_oMediaEffectIDs));
				else if(opMediaTrackInfo -> IsVideo())
					dynamic_cast<VideoGClipContainer*>(opTrack -> GetClipContainer()) -> BuildFXNodes(&(opEffectTrackInfo -> m_oMediaEffectIDs));
			}
		}

		opPreviousTrack = opTrack;

		oSelectedIterator++;
		oSizeIterator++;
		oOrderIterator++;
	}
}

bool EMGTrackRepository::LoadData(void* p_upDataLoader)
{
	EMProjectDataLoader* opLoader = static_cast<EMProjectDataLoader*>(p_upDataLoader);
	m_oTrackSize.clear();
	m_oSelectedTracks.clear();
	m_oTrackOrder.clear();
	uint32 vIndexEnd = opLoader -> LoadUInt32();
eo << "EMGTrackRepository::LoadData:" << ef;
	for(uint32 vIndex = 0; vIndex < vIndexEnd; vIndex++)
	{
		m_oTrackSize.push_back(opLoader -> LoadUInt8());
		m_oSelectedTracks.push_back(opLoader -> LoadBool());
		m_oTrackOrder.push_back(opLoader -> LoadInt32());
eo << "  " << m_oTrackSize.back() << ", " << m_oSelectedTracks.back() << ", " << m_oTrackOrder.back() << ef;
	}

	return true;
}

// p_vBeforeIndex: All tracks before this index that are not amongst the moved tracks will be first
// p_vBeforeIndex: All tracks after this index that are not amongst the moved tracks will be last
void EMGTrackRepository::MoveTracks(vector<EMGTrack*> p_oTracks, uint32 p_vBeforeIndex, uint32 p_vAfterIndex)
{
	vector<EMGTrack*> oNewRepository;
	uint32 vIndex;
//;//cout_commented_out_4_release << "EMGTrackRepository::MoveTracks: p_oTracks.size " << p_oTracks.size() << ", p_vBeforeIndex " << p_vBeforeIndex << ", p_vAfterIndex: " << p_vAfterIndex << ", m_oTrackRepository.size " << m_oTrackRepository.size() << endl;
	if(!(p_vBeforeIndex == 0 && p_vAfterIndex == 0))
		for(vIndex = 0; vIndex <= p_vBeforeIndex; vIndex++)
		{
			if(find(p_oTracks.begin(), p_oTracks.end(), m_oTrackRepository[vIndex]) == p_oTracks.end())
			{
				oNewRepository.push_back(m_oTrackRepository[vIndex]);
//;//cout_commented_out_4_release << "1 Pushing back: " << m_oTrackRepository[vIndex] -> GetTrackInfo() -> GetTrackName() << endl;
			}
		}
	for(vIndex = 0; vIndex < p_oTracks.size(); vIndex++)
	{
		oNewRepository.push_back(p_oTracks[vIndex]);
//;//cout_commented_out_4_release << "2 Pushing back: " << p_oTracks[vIndex] -> GetTrackInfo() -> GetTrackName() << endl;
	}
	for(vIndex = p_vAfterIndex; vIndex < m_oTrackRepository.size(); vIndex++)
	{
		if(find(p_oTracks.begin(), p_oTracks.end(), m_oTrackRepository[vIndex]) == p_oTracks.end())
		{
			oNewRepository.push_back(m_oTrackRepository[vIndex]);
//;//cout_commented_out_4_release << "3 Pushing back: " << m_oTrackRepository[vIndex] -> GetTrackInfo() -> GetTrackName() << endl;
		}
	}

	m_oTrackRepository = oNewRepository;

	// Notify the media engine of the changes (so video priorities works)
	int32 vMediaTrackID;
	for(vIndex = 0; vIndex < m_oTrackRepository.size(); vIndex++)
	{
		vMediaTrackID = m_oTrackRepository[vIndex] -> GetTrackInfo() -> GetMediaTrackID();
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_MOVE_TRACK, &vMediaTrackID, &vIndex);
	}

	Notify(EM_TRACK_REPOSITORY_UPDATED);

	m_vIsDirty = true;
}

void EMGTrackRepository::ReadyForUse()
{
eo << "EMGTrackRepository::ReadyForUse" << ef;
//	Notify(EM_TRACK_REPOSITORY_UPDATED);
	m_vIsDirty = false;
}

void EMGTrackRepository::RegisterTrackType(EMMediaType p_eType, EMGTrackInfo* p_opTrackInfo, EMGClipContainer* p_opClipContainer)
{
	if(m_oTrackInfoTypes[p_eType] != NULL || m_oClipContainerTypes[p_eType] != NULL)
		EMDebugger("There are already classes registered for this media type");
		
	m_oTrackInfoTypes[p_eType] = p_opTrackInfo;
	m_oClipContainerTypes[p_eType] = p_opClipContainer;
}

bool EMGTrackRepository::SaveData(void* p_upDataSaver)
{
eo << "EMGTrackRepository::SaveData" << ef;
	EMProjectDataSaver* opSaver = static_cast<EMProjectDataSaver*>(p_upDataSaver);
	opSaver -> SaveUInt32(m_oTrackRepository.size());

	vector<EMGTrack*>::iterator oIterator = m_oTrackRepository.begin();
	while(oIterator != m_oTrackRepository.end())
	{
;//cout_commented_out_4_release << "Saving track size: " << static_cast<uint8>((*oIterator) -> GetZoomScale()) << endl;
		opSaver -> SaveUInt8(static_cast<uint8>((*oIterator) -> GetZoomScale()));
		opSaver -> SaveBool((*oIterator) -> GetTrackInfo() -> IsSelected());
		opSaver -> SaveInt32((*oIterator) -> GetTrackInfo() -> GetMediaTrackID());
		oIterator++;
	}

	return true;
}

void EMGTrackRepository::SetTrackOrder(vector<EMGTrack*>* p_opTrackOrder)
{
	if(p_opTrackOrder -> size() != m_oTrackRepository.size())
		EMDebugger("Conflicting track repository sizes");

	m_oTrackRepository = *p_opTrackOrder;
	int32 vMediaTrackID;
	for(uint32 vIndex = 0; vIndex < m_oTrackRepository.size(); vIndex++)
	{
		vMediaTrackID = m_oTrackRepository[vIndex] -> GetTrackInfo() -> GetMediaTrackID();
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_MOVE_TRACK, &vMediaTrackID, &vIndex);
	}

	m_vIsDirty = true;
}

void EMGTrackRepository::SetTracks(vector<EMGTrack*>* p_opTracks)
{
	m_oTrackRepository = *p_opTracks;
	Notify(EM_TRACK_REPOSITORY_UPDATED);

	m_vIsDirty = true;
}

void EMGTrackRepository::TrackSelectionHasChanged()
{
	m_vIsDirty = true;
}

void EMGTrackRepository::TrackZoomHasChanged()
{
	m_vIsDirty = true;
}

