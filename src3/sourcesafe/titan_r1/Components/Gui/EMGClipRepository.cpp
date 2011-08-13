#include "EMGClipRepository.h"

#include "CommandIDs.h" // FIXME: app class in FW
#include "EMCommandRepository.h"
#include "EMGClip.h"
#include "EMGTrack.h"
#include "EMGTrackRepository.h"
#include "EMGUIUtilities.h"
#include "EMImageBufferRepository.h"
#include "EMMessages.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"
#include "EMView.h"

#include <algorithm>

EMGClipRepository* EMGClipRepository::m_opInstance = NULL;

EMGClipRepository::EMGClipRepository() :
m_vEarliestSelectedPosition(INT64_MAX),
m_vIsDirty(false),
m_vProjectEnd(0)
{
	m_oClipTypes[EM_TYPE_ANY_VIDEO] = NULL;
	m_oClipTypes[EM_TYPE_ANY_AUDIO] = NULL;
	m_oClipTypes[EM_TYPE_MIDI] = NULL;
	m_oClipTypes[EM_TYPE_TRANSITION] = NULL;
}

EMGClipRepository::~EMGClipRepository()
{
	delete m_oClipTypes[EM_TYPE_ANY_VIDEO];
	delete m_oClipTypes[EM_TYPE_ANY_AUDIO];
	delete m_oClipTypes[EM_TYPE_MIDI];
	delete m_oClipTypes[EM_TYPE_TRANSITION];
	
	list<EMGClip*>::iterator oIterator = m_oClipRepository.begin();
	EMGClip* opClip;
	while(oIterator != m_oClipRepository.end()) // Deletes all graphical clips
	{
		opClip = *oIterator;
		EMImageBufferRepository::Instance() -> ClipRemoved(opClip);
		oIterator++;
		delete opClip;
	}
}

void EMGClipRepository::AddClipGroup(EMGClip* p_opClipGroupOwner)
{
	m_oClipGroupOwners.push_back(p_opClipGroupOwner);
	m_oClipGroupOwners.unique();

	m_vIsDirty = true;
}

// This command is used to put back clips when undoing a delete clips operation
void EMGClipRepository::AddClips(list<EMGClip*>* p_opClipList, bool p_vUpdate)
{
	//list<EMGClipContainer*> oAffectedClipContainers;
	list<EMGClip*>::iterator oClipIterator = p_opClipList -> begin();
	while(oClipIterator != p_opClipList -> end())
	{
		// Deselect the clip to be on the safe side
		(*oClipIterator) -> SetSelected(false);
		m_oClipRepository.push_back(*oClipIterator);
		m_oClipsByTypeRepository[(*oClipIterator) -> GetType()].push_back(*oClipIterator);

		// It should be safe to presume that the track previously owning the clip still is alive
		(*oClipIterator) -> GetTrack() -> GetClipContainer() -> AddClip(*oClipIterator);

		// If the clip previously owned a clip group, register that group again
		if((*oClipIterator) -> GetFirstInGroup() == (*oClipIterator))
			AddClipGroup(*oClipIterator);

		oClipIterator++;
	}

	if(p_vUpdate)
		Update(); // FIXME: This is a bit unefficient
}

void EMGClipRepository::CalculateFrame()
{
//;//cout_commented_out_4_release << "EMGClipRepository::CalculateFrame" << endl;
	m_oFrame = EMRect();
	list<EMGClip*>::iterator oIterator = m_oClipRepository.begin();
	while(oIterator != m_oClipRepository.end())
	{
		m_oFrame = m_oFrame | (*oIterator) -> Frame();
		oIterator++;
	}
}

EMRect EMGClipRepository::CalculateFrame(list<EMGClip*>* p_opClips)
{
//;//cout_commented_out_4_release << "EMGClipRepository::CalculateFrame2" << endl;
	EMRect oFrame = EMRect();
	list<EMGClip*>::iterator oIterator = p_opClips -> begin();
	while(oIterator != p_opClips -> end())
	{
		oFrame = oFrame | (*oIterator) -> Frame();
		oIterator++;
	}
	return oFrame;
}

// Should not be used that much since it is somewhat expensive
bool EMGClipRepository::CalculateMovedFrame(uint32 p_vTrackOffset, int64 p_vPositionMovedOffset)
{
	EMRect oMovedTotalRect;
	EMRect oMovedClipRect;
	list<EMGClip*>::iterator oIterator = m_oSelectedClipRepository.begin();
	while(oIterator != m_oSelectedClipRepository.end())
	{
		oMovedClipRect = (*oIterator) -> CalculateFrame(p_vTrackOffset, p_vPositionMovedOffset);
		(*oIterator) -> SetMovedFrame(oMovedClipRect, p_vPositionMovedOffset, p_vTrackOffset);
		(*oIterator) -> CalculateOkToDrop(); // FIXME: Not really logical to have this call here
		//if(!((*oIterator) -> GetTrack() -> GetTrackInfo() -> IsHidden(p_vClipEditMode)))
			oMovedTotalRect = oMovedTotalRect | oMovedClipRect;
		oIterator++;
	}
	m_oMovedFrame = oMovedTotalRect;
	return true; // The frame is successfully calculated
}

// Should not be used that much since it is rather expensive
void EMGClipRepository::CalculateSelectedFrame()
{
//;//cout_commented_out_4_release << "EMGClipRepository::CalculateSelectedFrame" << endl;
//	EMRect oOldFrame = m_oSelectedFrame;
	m_oSelectedFrame = EMRect();
	m_vEarliestSelectedPosition = INT64_MAX; // Should be enough
	m_vLatestSelectedPosition = 0;
	list<EMGClip*>::iterator oIterator = m_oSelectedClipRepository.begin();
	while(oIterator != m_oSelectedClipRepository.end())
	{
		if((*oIterator) -> GetPosition() < m_vEarliestSelectedPosition)
			m_vEarliestSelectedPosition = (*oIterator) -> GetPosition();
		if((*oIterator) -> GetPosition() +  (*oIterator) -> GetLength() - 1 > m_vLatestSelectedPosition)
			m_vLatestSelectedPosition = (*oIterator) -> GetPosition() +  (*oIterator) -> GetLength() - 1;
		//if(!((*oIterator) -> GetTrack() -> GetTrackInfo() -> IsHidden(p_vClipEditMode)))
			m_oSelectedFrame = m_oSelectedFrame | (*oIterator) -> Frame();
		oIterator++;
	}
//	if(m_oSelectedFrame != oOldFrame)
//		Notify(EM_CLIP_FRAME_CHANGED);
}

bool EMGClipRepository::ClearData()
{
;//cout_commented_out_4_release << "EMGClipRepository::ClearData" << endl;
	list<EMGClip*>::iterator oIterator = m_oClipRepository.begin();
	EMGClip* opClip;
	while(oIterator != m_oClipRepository.end()) // Deletes all graphical clips
	{
		opClip = *oIterator;
		//EMImageBufferRepository::Instance() -> ClipRemoved(opClip); This method is not necessary since IBR has its own ClearData
		oIterator++;
		delete opClip;
	}

	m_oClipRepository.clear();
	m_oClipsByTypeRepository.clear();
	m_oClipGroupOwners.clear();
	m_oFrame = EMRect();
	m_oMovedFrame = EMRect();
	m_oSelectedClipRepository.clear();
	m_oSelectedFrame = EMRect();
	m_vEarliestSelectedPosition = INT64_MAX;
	m_vProjectEnd = 0;
;//cout_commented_out_4_release << "EMGClipRepository::ClearData completed" << endl;
	return true;
}

void EMGClipRepository::ClearMovedFrame()
{
	list<EMGClip*>::iterator oIterator = m_oSelectedClipRepository.begin();
	while(oIterator != m_oSelectedClipRepository.end())
	{
		(*oIterator) -> SetMovedFrame(EMRect(), 0, 0);
		oIterator++;
	}
	m_oMovedFrame = EMRect();
}

void EMGClipRepository::CloneClips(list<EMGClip*>* p_opClips, int32 p_vTrackOffset, int64 p_vPositionOffset, list<EMGClip*>* p_opNewClips)
{
	EMRect oOldFrame = CalculateFrame(p_opClips);
	EMRect oNewFrame = EMRect();
	list<EMGClip*> oCloneableClips = *p_opClips; // Makes a copy of the repository since p_opClips will probably change during the update
	list<EMGClip*>::iterator oIterator = oCloneableClips.begin();
	EMGClip* opNewClip;

	while(oIterator != oCloneableClips.end())
	{
		opNewClip = (*oIterator) -> CloneClip(p_vTrackOffset, p_vPositionOffset);
		SetSelected(*oIterator, false);
		SetSelected(opNewClip, true);
		if(opNewClip -> GetPosition() + opNewClip -> GetLength() > m_vProjectEnd)
			m_vProjectEnd = opNewClip -> GetPosition() + opNewClip -> GetLength();
		oNewFrame = oNewFrame | opNewClip -> Frame();
		if(p_opNewClips != NULL)
			p_opNewClips -> push_back(opNewClip);
		oIterator++;
	}
	
	EMGUIUtilities::SortClips(&m_oClipRepository);

	Notify(EM_CLIP_REPOSITORY_UPDATED);
	if(oOldFrame.Intersects(oNewFrame))
	{
		m_oUpdateRect = m_oUpdateRect | oOldFrame | oNewFrame;
		Notify(EM_UPDATE_NEEDED);
	}
	else
	{
		m_oUpdateRect = m_oUpdateRect | oOldFrame;
		Notify(EM_UPDATE_NEEDED);
		m_oUpdateRect = m_oUpdateRect | oNewFrame;
		Notify(EM_UPDATE_NEEDED);
	}
}

void EMGClipRepository::ClearSelectedClips()
{
	list<EMGClip*>::iterator oIterator = m_oSelectedClipRepository.begin();
	while(oIterator != m_oSelectedClipRepository.end())
	{
		(*oIterator) -> SetSelected(false);
		oIterator++;
	}
	m_oSelectedClipRepository.clear();
	m_oSelectedFrame = EMRect();
}

EMGClip* EMGClipRepository::CreateClip(string p_oClipName, EMMediaType p_vType, int64 p_vPosition, int64 p_vLength, EMGTrack* p_opTrack, int32 p_vMediaClipID, EMView* p_opView, bool p_vUpdate)
{
//;//cout_commented_out_4_release << "EMGClipRepository::CreateClip" << endl;
	if(m_oClipTypes[p_vType] == NULL)
		EMDebugger("There is no clip registered for this media type");

	EMGClip* opClip = m_oClipTypes[p_vType] -> CreateInstance(p_oClipName, p_vPosition, p_vLength, p_opTrack, p_vMediaClipID, p_opView);
	opClip -> SetFrame(p_opTrack -> GetClipContainer() -> CalculateClipFrame(opClip));
	p_opTrack -> GetClipContainer() -> AddClip(opClip);
	m_oClipRepository.push_back(opClip);
	m_oClipsByTypeRepository[p_vType].push_back(opClip);
	EMGUIUtilities::SortClips(&m_oClipRepository);
	CalculateFrame();
	if(p_vPosition + p_vLength > m_vProjectEnd)
		m_vProjectEnd = p_vPosition + p_vLength;
	if(p_vUpdate)
		Notify(EM_CLIP_REPOSITORY_UPDATED);
	return opClip;
}

void EMGClipRepository::DeleteClips(list<EMGClip*>* p_opClips, bool p_vRemoveFromMemory, bool p_vNotify)
{
	bool vNewProjectEnd(false);
	
	list<EMGClip*> oDeleteableClips(*p_opClips);
	list<EMGClip*>::iterator oIterator = oDeleteableClips.begin();
	while(oIterator != oDeleteableClips.end())
	{
		if((*oIterator) -> GetFirstInGroup() == (*oIterator))
			RemoveClipGroup(*oIterator);

		m_oClipRepository.remove(*oIterator);
		m_oClipsByTypeRepository[(*oIterator) -> GetType()].remove(*oIterator);
		m_oSelectedClipRepository.remove(*oIterator);
		m_oUpdateRect = m_oUpdateRect | (*oIterator) -> Frame();
		(*oIterator) -> GetTrack() -> GetClipContainer() -> RemoveClip(*oIterator);
		if((*oIterator) -> GetPosition() + (*oIterator) -> GetLength() == m_vProjectEnd)
			vNewProjectEnd = true;
		if(p_vRemoveFromMemory)
			delete (*oIterator);

		oIterator++;
	}

	EMGUIUtilities::SortClips(&m_oClipRepository);
	CalculateFrame();

	if(vNewProjectEnd)
	{
		oIterator = m_oClipRepository.begin();
		m_vProjectEnd = 0;
		while(oIterator != m_oClipRepository.end())
		{
			if((*oIterator) -> GetPosition() + (*oIterator) -> GetLength() > m_vProjectEnd)
				m_vProjectEnd = (*oIterator) -> GetPosition() + (*oIterator) -> GetLength();
			oIterator++;
		}
	}

	EMRect oOldSelectedFrame = SelectedFrame();
	CalculateSelectedFrame();
	if(p_vNotify)
	{
		Notify(EM_CLIP_REPOSITORY_UPDATED);
		if(oOldSelectedFrame != SelectedFrame())
			m_oUpdateRect = m_oUpdateRect | oOldSelectedFrame;
		Notify(EM_UPDATE_NEEDED);
	}
}

void EMGClipRepository::DrawClips(list<EMGClip*>* p_opClips, EMRect p_oUpdateRect, EMView* p_opView, bool p_vClipEditMode, int64 m_vTimeZoomScale, bool p_vDrawContent)
{
//;//cout_commented_out_4_release << "EMGClipRepository::DrawClips (" << p_opClips -> size() << ")" << endl;
//m_oFrame.Print();
//p_oUpdateRect.Print();
//	if(!p_oUpdateRect.Intersects(m_oFrame))
//		return;
	list<EMGClip*>::iterator oIterator = p_opClips -> begin();
	while(oIterator != p_opClips -> end())
	{
		(*oIterator) -> DrawClip(p_oUpdateRect, p_opView, p_vClipEditMode, m_vTimeZoomScale, p_vDrawContent);
		oIterator++;
	}
}

void EMGClipRepository::DrawMovedClips(list<EMGClip*>* p_opClips, const EMRect p_oUpdateRect, EMView* p_opView, int64 p_vPositionMovedOffset, int64 p_vZoomScale, bool p_vDrawContent)
{
//;//cout_commented_out_4_release << "EMGClipRepository::DrawMovedClips" << endl;
	if(!p_oUpdateRect.Intersects(m_oMovedFrame))
		return;
	list<EMGClip*>::iterator oIterator = p_opClips -> begin();
	while(oIterator != p_opClips -> end())
	{
		(*oIterator) -> DrawMovedClip(p_oUpdateRect, p_opView/*, p_vPositionMovedOffset*/, p_vZoomScale, p_vDrawContent);
		oIterator++;
	}
}

EMRect EMGClipRepository::Frame() const
{
	return m_oFrame;
}

EMRect EMGClipRepository::GetAndClearUpdateRect()
{
	EMRect oRect = m_oUpdateRect;
	m_oUpdateRect = EMRect();
	return oRect;
}

EMGClip* EMGClipRepository::GetClip(int32 p_vMediaClipID)
{
	list<EMGClip*>::iterator oIterator = m_oClipRepository.begin();
	while(oIterator != m_oClipRepository.end())
	{
		if((*oIterator) -> GetMediaClipID() == p_vMediaClipID)
			return (*oIterator);
		oIterator++;
	}
	return NULL;
}

EMGClip* EMGClipRepository::GetClip(int32 p_vX, int32 p_vY)
{
//;//cout_commented_out_4_release << "EMGClipRepository::GetClip" << endl;
	list<EMGClip*>::iterator oIterator = m_oClipRepository.begin();
	while(oIterator != m_oClipRepository.end())
	{
		if(((*oIterator) -> Frame()).Contains(p_vX, p_vY))
			return (*oIterator);
		oIterator++;
	}
	return NULL;
}
/*
list<EMGClip*> EMGClipRepository::GetClipGroup(EMGClip* p_opClip, bool* p_vpHiddenClipsExist)
{
	*p_vpHiddenClipsExist = false;

	list<EMGClip*> oClipGroup;
	if(p_opClip -> GetFirstInGroup() == NULL)
	{
		oClipGroup.push_back(p_opClip);
		return oClipGroup;
	}
	
	EMGClip* opCurrentClip = p_opClip -> GetFirstInGroup();
	while(opCurrentClip -> GetNextInGroup() != NULL)
	{
		if(!(opCurrentClip -> GetTrack() -> GetTrackInfo() -> IsHidden(m_eEditMode == EM_CLIP_EDIT_MODE)))
			*p_vpHiddenClipsExist = true;

		oClipGroup.push_back(opCurrentClip);
		opCurrentClip = opCurrentClip -> GetNextInGroup();
	}
	oClipGroup.push_back(opCurrentClip);

	return oClipGroup;
}

list<EMGClip*> EMGClipRepository::GetClipGroup(list<EMGClip*>* p_opClips, bool* p_vpHiddenClipsExist)
{
	bool vHiddenClipsExistTemp;
	bool vHiddenClipsExist(false);

	list<EMGClip*> oClipGroup;
	list<EMGClip*>::iterator oIterator = p_opClips -> begin();
	while(oIterator != p_opClips -> end())
	{
		oClipGroup.merge(GetClipGroup(*oIterator, &vHiddenClipsExistTemp));
		if(vHiddenClipsExistTemp)
			vHiddenClipsExist = true;

		oIterator++;
	}

	*p_vpHiddenClipsExist = vHiddenClipsExist;

	return oClipGroup;
}
*/
list<EMGClip*>* EMGClipRepository::GetClips()
{
	return &m_oClipRepository;
}

void EMGClipRepository::GetClips(EMGTrack* p_opTrack, list<EMGClip*>* p_opClips)
{
//;//cout_commented_out_4_release << "EMGClipRepository::GetClips" << endl;
	p_opClips -> clear(); // Just to make sure it's empty
	list<EMGClip*>::iterator oIterator = m_oClipRepository.begin();
	while(oIterator != m_oClipRepository.end())
	{
		if((*oIterator) -> GetTrack() == p_opTrack)
			p_opClips -> push_back(*oIterator);
		oIterator++;
	}
}

list<EMGClip*>* EMGClipRepository::GetClips(EMMediaType p_eType)
{
list<EMGClip*> oClips = m_oClipsByTypeRepository[p_eType];
	return &m_oClipsByTypeRepository[p_eType];
}

int64 EMGClipRepository::GetEarliestSelectedPosition()
{
	return m_vEarliestSelectedPosition;
}

int64 EMGClipRepository::GetLatestSelectedPosition()
{
	return m_vLatestSelectedPosition;
}

EMGClip* EMGClipRepository::GetNextClipInTrack(EMGClip* p_opClip)
{
	list<EMGClip*>* opClips = p_opClip -> GetTrack() -> GetClipContainer() -> GetClips();
	list<EMGClip*>::iterator oIterator = find(opClips -> begin(), opClips -> end(), p_opClip);
	oIterator++;
	if(oIterator == opClips -> end())
		return NULL;
	return (*oIterator);
}

uint32 EMGClipRepository::GetNrClips() const
{
	return m_oClipRepository.size();
}

uint32 EMGClipRepository::GetNrSelectedClips() const
{
	return m_oSelectedClipRepository.size();
}

EMGClip* EMGClipRepository::GetPreviousClipInTrack(EMGClip* p_opClip)
{
	list<EMGClip*>* opClips = p_opClip -> GetTrack() -> GetClipContainer() -> GetClips();
	list<EMGClip*>::iterator oIterator = find(opClips -> begin(), opClips -> end(), p_opClip);
	if(oIterator == opClips -> begin())
		return NULL;
	oIterator--;
	return (*oIterator);
}

int64 EMGClipRepository::GetProjectEnd() const
{
	return m_vProjectEnd;
}

list<EMGClip*>* EMGClipRepository::GetSelectedClips()
{
	return &m_oSelectedClipRepository;
}

EMGClipRepository* EMGClipRepository::Instance()
{
	if(m_opInstance != NULL)
		return m_opInstance;		
	m_opInstance = EM_new EMGClipRepository();
	return m_opInstance;
}

bool EMGClipRepository::IsDirty()
{
	return m_vIsDirty;
}

void EMGClipRepository::LoadComplete()
{
;//cout_commented_out_4_release << "EMGClipRepository::LoadComplete" << endl;

	// Starting to query the media system for track information
	EMMediaTrackInfo* opMediaTrackInfo;
	EMMediaClipInfo* opMediaClipInfo;
	list<int32> oMediaTrackIDs = *(static_cast<list<int32>*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ALL_TRACK_IDS, NULL, NULL, NULL)));
//eo << "Found " << oMediaTrackIDs.size() << " tracks" << ef;
	list<int32>::iterator oTrackIterator = oMediaTrackIDs.begin();
	list<int32>::iterator oClipIterator;
	EMCommandRepository* opCommandRepository = EMCommandRepository::Instance();
	EMGTrackRepository* opTrackRepository = EMGTrackRepository::Instance();

	// Create graphical clips
	EMGClip* opClip;
	while(oTrackIterator != oMediaTrackIDs.end())
	{
		opMediaTrackInfo = static_cast<EMMediaTrackInfo*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_TRACK_INFO, &(*oTrackIterator), NULL));
		oClipIterator = (opMediaTrackInfo -> m_oMediaClipIDs).begin();
//eo << "Found " << (opMediaTrackInfo -> m_oMediaClipIDs).size() << " clips in track with ID: " << (*oTrackIterator) << ef;
		while(oClipIterator != (opMediaTrackInfo -> m_oMediaClipIDs).end())
		{
			opMediaClipInfo = static_cast<EMMediaClipInfo*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_INFO, &(*oClipIterator), NULL));

			int32 vTrackMediaID = opMediaClipInfo -> m_vMediaTrackID;
			uint32 vIndex;
			for(vIndex = 0; vIndex < opTrackRepository -> GetNrTracks(); vIndex++)
				if(opTrackRepository -> GetTrack(vIndex) -> GetTrackInfo() -> GetMediaTrackID() == vTrackMediaID)
					break;
			if(opMediaClipInfo -> IsAudio())
				opClip = static_cast<EMGClip*>(opCommandRepository -> ExecuteCommand(COMMAND_ADD_G_AUDIO_CLIP, &(opMediaClipInfo -> m_vID), &vIndex, &(opMediaClipInfo -> m_vActiveStart)));
			else if(opMediaClipInfo -> IsVideo())
				opClip = static_cast<EMGClip*>(opCommandRepository -> ExecuteCommand(COMMAND_ADD_G_VIDEO_CLIP, &(opMediaClipInfo -> m_vID), &vIndex, &(opMediaClipInfo -> m_vActiveStart)));
			else if(opMediaClipInfo -> IsMIDI())
				opClip = static_cast<EMGClip*>(opCommandRepository -> ExecuteCommand(COMMAND_ADD_G_MIDI_CLIP, &(opMediaClipInfo -> m_vID), &vIndex, &(opMediaClipInfo -> m_vActiveStart)));
			else // Transition
				opClip = static_cast<EMGClip*>(opCommandRepository -> ExecuteCommand(COMMAND_ADD_G_TRANSITION_CLIP, &(opMediaClipInfo -> m_vID), &vIndex, &(opMediaClipInfo -> m_vActiveStart)));

			opClip -> SetLabel(const_cast<char*>((opMediaClipInfo -> m_oName).c_str()), NULL);

			oClipIterator++;
		}
		oTrackIterator++;
	}

	// Restore clip groups
	EMGClip* opParentClip;
	EMGClip* opFirstClipInGroup;
	EMGClip* opChildClip;
	list<int32>::iterator oIterator = m_oLoadedData.begin();
	bool vNewGroup(true);
eo << "Rebuilding clip groups" << ef;
	while(oIterator != m_oLoadedData.end())
	{
		if(*oIterator == -1)
		{
			opChildClip -> SetNextInGroup(NULL);
			vNewGroup = true;
		}
		else if(vNewGroup)
		{
			opFirstClipInGroup = GetClip(*oIterator);
			opFirstClipInGroup -> SetFirstInGroup(opFirstClipInGroup);
			opChildClip = opFirstClipInGroup;
			vNewGroup = false;
eo << "Group owner: " << opFirstClipInGroup -> GetMediaClipID() << ef;
		}
		else
		{
			opParentClip = opChildClip;
			opChildClip = GetClip(*oIterator);
			opParentClip -> SetNextInGroup(opChildClip);
			opChildClip -> SetFirstInGroup(opFirstClipInGroup);
eo << "Group member: " << opChildClip -> GetMediaClipID() << ef;
		}
		oIterator++;
	}

}

// Loads saved clip groups
bool EMGClipRepository::LoadData(void* p_upDataLoader)
{
	EMProjectDataLoader* opLoader = static_cast<EMProjectDataLoader*>(p_upDataLoader);

	m_oLoadedData.clear();

	uint32 vEntriesToLoad = opLoader -> LoadUInt32();

	for(; vEntriesToLoad > 0; vEntriesToLoad--)
		m_oLoadedData.push_back(opLoader -> LoadInt32());

	return true;
}

void EMGClipRepository::MoveClips(list<EMGClip*>* p_opClips, int32 p_vTrackOffset, int64 p_vPositionOffset)
{
	bool vNewProjectEnd(false);
	
	EMRect oOldFrame = m_oSelectedFrame;
	list<EMGClip*>::iterator oIterator = p_opClips -> begin();
	while(oIterator != p_opClips -> end())
	{
		if((*oIterator) -> GetPosition() + (*oIterator) -> GetLength() == m_vProjectEnd)
			vNewProjectEnd = true;
		(*oIterator) -> MoveClip(p_vTrackOffset, p_vPositionOffset);

		// To keep the clip list of the track owning the clip sorted
		(*oIterator) -> GetTrack() -> GetClipContainer() -> RemoveClip(*oIterator);
		(*oIterator) -> GetTrack() -> GetClipContainer() -> AddClip(*oIterator);

		oIterator++;
	}

	EMGUIUtilities::SortClips(&m_oClipRepository);
	CalculateFrame();
	CalculateSelectedFrame();

	if(vNewProjectEnd)
	{
		oIterator = m_oClipRepository.begin();
		m_vProjectEnd = 0;
		while(oIterator != m_oClipRepository.end())
		{
			if((*oIterator) -> GetPosition() + (*oIterator) -> GetLength() > m_vProjectEnd)
				m_vProjectEnd = (*oIterator) -> GetPosition() + (*oIterator) -> GetLength();
			oIterator++;
		}
	}

	Notify(EM_CLIP_REPOSITORY_UPDATED);
	if(oOldFrame.Intersects(m_oSelectedFrame))
	{
		m_oUpdateRect = m_oUpdateRect | oOldFrame | m_oSelectedFrame;
		Notify(EM_UPDATE_NEEDED);
	}
	else
	{
		m_oUpdateRect = m_oUpdateRect | oOldFrame;
		Notify(EM_UPDATE_NEEDED);
		m_oUpdateRect = m_oUpdateRect | m_oSelectedFrame;
		Notify(EM_UPDATE_NEEDED);
	}
}

void EMGClipRepository::MoveClipsToTempo(list<EMGClip*>* p_opClips, float p_vOldTempo, float p_vNewTempo)
{
	bool vNewProjectEnd(false);
	EMRect oOldFrame = m_oSelectedFrame;
	
	int64 vPositionOffset;
	list<EMGClip*>::iterator oIterator = p_opClips -> begin();
	while(oIterator != p_opClips -> end())
	{
		if((*oIterator) -> GetPosition() + (*oIterator) -> GetLength() == m_vProjectEnd)
			vNewProjectEnd = true;
		vPositionOffset = ((((*oIterator) -> GetPosition()) * p_vOldTempo) / p_vNewTempo) - ((*oIterator) -> GetPosition());
		(*oIterator) -> MoveClip(0, vPositionOffset);

		// To keep the clip list of the track owning the clip sorted
		(*oIterator) -> GetTrack() -> GetClipContainer() -> RemoveClip(*oIterator);
		(*oIterator) -> GetTrack() -> GetClipContainer() -> AddClip(*oIterator);

		(*oIterator) -> UpdateFrame(); // Needed?
		oIterator++;
	}

	EMGUIUtilities::SortClips(&m_oClipRepository);
	CalculateFrame();
	CalculateSelectedFrame();

	// FIXME: Move this routine into a separate method (also used in MoveClips)
	if(vNewProjectEnd)
	{
		oIterator = m_oClipRepository.begin();
		m_vProjectEnd = 0;
		while(oIterator != m_oClipRepository.end())
		{
			if((*oIterator) -> GetPosition() + (*oIterator) -> GetLength() > m_vProjectEnd)
				m_vProjectEnd = (*oIterator) -> GetPosition() + (*oIterator) -> GetLength();
			oIterator++;
		}
	}

	Notify(EM_CLIP_REPOSITORY_UPDATED);

	if(oOldFrame.Intersects(m_oSelectedFrame))
	{
		m_oUpdateRect = m_oUpdateRect | oOldFrame | m_oSelectedFrame;
		Notify(EM_UPDATE_NEEDED);
	}
	else
	{
		m_oUpdateRect = m_oUpdateRect | oOldFrame;
		Notify(EM_UPDATE_NEEDED);
		m_oUpdateRect = m_oUpdateRect | m_oSelectedFrame;
		Notify(EM_UPDATE_NEEDED);
	}

//	m_vIsDirty = true;
}

EMRect EMGClipRepository::MovedFrame() const
{
	return m_oMovedFrame;
}

bool EMGClipRepository::OkToDrop(list<EMGClip*>* p_opClips)
{
//;//cout_commented_out_4_release << "EMGClipRepository::OkToDrop" << endl;
	list<EMGClip*>::iterator oIterator = p_opClips -> begin();
	while(oIterator != p_opClips -> end())
	{
		if(!(*oIterator) -> OkToDrop())
			return false;		
		oIterator++;
	}
	return true;
}
/*
void EMGClipRepository::PreventOverlapping(list<EMGClip*>* p_opClips)
{
	list<EMGTrack*> oTracks;

	list<EMGClip*>::iterator oClipIterator = m_oSelectedClipRepository.begin();
	while(oClipIterator != m_oSelectedClipRepository.end())
	{
		oTracks.push_back((*oClipIterator) -> GetTrack());
		oClipIterator++;
	}
	oTracks.unique();

	bool vClipsHaveMoved(false);
	EMGClip* opPreviousClip;
	list<EMGTrack*>::iterator oTrackIterator = oTracks.begin();
	while(oTrackIterator != oTracks.end())
	{
		if((*oTrackIterator) -> GetClipContainer() -> GetClips() -> size() >= 2)
		{
			opPreviousClip = (*oTrackIterator) -> GetClipContainer() -> GetClips() -> front();
			oClipIterator = (*oTrackIterator) -> GetClipContainer() -> GetClips() -> begin();
			oClipIterator++;
			while(oClipIterator != m_oSelectedClipRepository.end())
			{
				if((*oClipIterator) -> GetPosition() <= opPreviousClip -> GetPosition() + opPreviousClip -> GetLength() - 1)
				{
					vClipsHaveMoved
				}
				oClipIterator++;
			}
		}
		oTrackIterator++;
	}
}
*/
void EMGClipRepository::PrintStatus()
{
	;//cout_commented_out_4_release << "------------------------------------------------------------------" << endl;
	;//cout_commented_out_4_release << "Number of tracks: " << EMGTrackRepository::Instance() -> GetNrTracks() << ", number of clips: " << GetClips() -> size() << endl;
	EMGTrack* opTrack;
	list<EMGClip*>::iterator oIterator;
	for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
	{
		opTrack = EMGTrackRepository::Instance() -> GetTrack(vIndex);
		;//cout_commented_out_4_release << "    Track " << vIndex << ": " << opTrack -> GetTrackInfo() -> GetTrackName() << ", contains " << opTrack -> GetClipContainer() -> GetClips() -> size() << " clips:" << endl;
		oIterator = opTrack -> GetClipContainer() -> GetClips() -> begin();
		while(oIterator != opTrack -> GetClipContainer() -> GetClips() -> end())
		{
			;//cout_commented_out_4_release << "        " << (*oIterator) -> GetLabel() << ", begin: " << (*oIterator) -> GetPosition() << ", end: " << (*oIterator) -> GetPosition() + (*oIterator) -> GetLength() - 1 << ", length: " << (*oIterator) -> GetLength() << endl;
			oIterator++;
		}
	}
	;//cout_commented_out_4_release << "------------------------------------------------------------------" << endl;
}

void EMGClipRepository::ReadyForUse()
{
	m_vIsDirty = false;
}

void EMGClipRepository::RegisterClipType(EMMediaType p_eType, EMGClip* p_opClip)
{
	if(m_oClipTypes[p_eType] != NULL)
		EMDebugger("There are already a clip registered for this media type");
		
	m_oClipTypes[p_eType] = p_opClip;
}

void EMGClipRepository::RemoveClipGroup(EMGClip* p_opClipGroupOwner)
{
//	list<EMGClip*>::iterator oClipIterator = find(p_opClipGroupOwner.begin(), p_opClipGroupOwner.end(), p_opClipGroupOwner);
	m_oClipGroupOwners.remove(p_opClipGroupOwner);
	m_vIsDirty = true;
}

// The created clip groups are saved here
bool EMGClipRepository::SaveData(void* p_upDataSaver)
{
	EMProjectDataSaver* opSaver = static_cast<EMProjectDataSaver*>(p_upDataSaver);

	uint32 vEntriesToSave(0);

	// First calculate how many entries to save
	EMGClip* opClip;
	list<EMGClip*>::iterator oIterator = m_oClipGroupOwners.begin();
	while(oIterator != m_oClipGroupOwners.end())
	{
		opClip = *oIterator;
		vEntriesToSave++;
		while(opClip != NULL)
		{
			opClip = opClip -> GetNextInGroup();
			vEntriesToSave++;
		}
		oIterator++;
	}

	opSaver -> SaveUInt32(vEntriesToSave);
eo << "Saving " << m_oClipGroupOwners.size() << " clip groups" << ef;
	oIterator = m_oClipGroupOwners.begin();
	while(oIterator != m_oClipGroupOwners.end())
	{
		opClip = *oIterator;
		opSaver -> SaveInt32(opClip -> GetMediaClipID());
eo << "Group owner: " << opClip -> GetMediaClipID() << ef;
		while(opClip != NULL)
		{
			opClip = opClip -> GetNextInGroup();
			if(opClip != NULL)
			{
				opSaver -> SaveInt32(opClip -> GetMediaClipID());
eo << "Group member: " << opClip -> GetMediaClipID() << ef;
			}
			else
				opSaver -> SaveInt32(-1);
		}
		oIterator++;
	}

	return true;
}

EMRect EMGClipRepository::SelectedFrame()
{
	return m_oSelectedFrame;
}

// Should only be called by EMGClip and EMGClipRepository
// Does not set the actual position in the clip, that is done elsewhere
void EMGClipRepository::SetClipPosition(EMGClip* p_opClip, int64 p_vPosition, bool p_vTriggerNotifyAndSort)
{
//;//cout_commented_out_4_release << "EMGClipRepository::SetClipPosition" << endl;
	if(p_vTriggerNotifyAndSort)
		EMGUIUtilities::SortClips(&m_oClipRepository);
	CalculateFrame();
//	EMRect oOldFrame = m_oSelectedFrame;
	EMRect oFrame = p_opClip -> Frame();
	int64 vPosition = p_opClip -> GetPosition();
	if(p_opClip -> GetSelected())
	{
		if(vPosition < m_vEarliestSelectedPosition)
		{
			m_vEarliestSelectedPosition = vPosition;
			m_oSelectedFrame.m_vLeft = oFrame.m_vLeft;
		}
		if(oFrame.m_vRight > m_oSelectedFrame.m_vRight)
			m_oSelectedFrame.m_vRight = oFrame.m_vRight;
//		CalculateSelectedFrame();
	}
// FIXME: Calculate an update rect here and notify
//	if(m_oSelectedFrame != oOldFrame && p_vTriggerNotifyAndSort)
//		Notify(EM_CLIP_FRAME_CHANGED);
	m_vIsDirty = true;
}

// Should only be called by EMGClip and EMGClipRepository
void EMGClipRepository::SetClipTrack(EMGClip* p_opClip, EMGTrack* p_opTrack)
{
//;//cout_commented_out_4_release << "EMGClipRepository::SetClipTrack" << endl;
	EMGTrack* opOldTrack = p_opClip -> GetTrack();
	if(p_opTrack != opOldTrack)
	{
		p_opClip -> SetTrack(p_opTrack);
		opOldTrack -> GetClipContainer() -> RemoveClip(p_opClip);//UpdateRepositories();
		p_opTrack -> GetClipContainer() -> AddClip(p_opClip);//UpdateRepositories();
		m_vIsDirty = true;
	}
}

void EMGClipRepository::SetSelected(EMGClip* p_opClip, bool p_vSelected)
{
	if(p_opClip -> GetSelected() == p_vSelected)
		return; // Nothing needs to be changed
	p_opClip -> SetSelected(p_vSelected);
	if(p_vSelected)
		m_oSelectedClipRepository.push_back(p_opClip);
	else
		m_oSelectedClipRepository.remove(p_opClip);
	CalculateSelectedFrame();
	m_vIsDirty = true; // Should this operation need a save?
}

// Do not send the return value of GetSelectedClips in here
void EMGClipRepository::SetSelected(list<EMGClip*>* p_opClips, bool p_vSelected)
{
	list<EMGClip*>::iterator oIterator = p_opClips -> begin();
	while(oIterator != p_opClips -> end())
	{
		if((*oIterator) -> GetSelected() != p_vSelected)
		{
			(*oIterator) -> SetSelected(p_vSelected);
			if(p_vSelected)
				m_oSelectedClipRepository.push_back((*oIterator));
			else
				m_oSelectedClipRepository.remove((*oIterator));
		}
		oIterator++;
	}
	CalculateSelectedFrame();
	m_vIsDirty = true;
}

// Selects all clips within the rect (keeping the already selected ones)
// If p_vIntersectOK is true, all intersecting clips are selected, else
// only clips entirely within the rect are selected
void EMGClipRepository::SetSelected(EMRect p_oRect, bool p_vIntersectOK)
{
	list<EMGClip*>::iterator oIterator = m_oClipRepository.begin();
	while(oIterator != m_oClipRepository.end())
	{
		if(p_vIntersectOK)
		{
			if(p_oRect.Intersects((*oIterator) -> Frame()) && !((*oIterator) -> GetSelected()))
			{
				(*oIterator) -> SetSelected(true);
				m_oSelectedClipRepository.push_back(*oIterator);
			}
			/*
			else if(!(p_oRect.Intersects((*oIterator) -> Frame())) && ((*oIterator) -> GetSelected()))
			{
				(*oIterator) -> SetSelected(false);
				m_oSelectedClipRepository.remove(*oIterator);
			}
			*/
		}
		else
		{
			if(p_oRect.Contains((*oIterator) -> Frame()) && !((*oIterator) -> GetSelected()))
			{
				(*oIterator) -> SetSelected(true);
				m_oSelectedClipRepository.push_back(*oIterator);
			}
			/*
			else if(!(p_oRect.Contains((*oIterator) -> Frame())) && ((*oIterator) -> GetSelected()))
			{
				(*oIterator) -> SetSelected(false);
				m_oSelectedClipRepository.remove(*oIterator);
			}
			*/
		}
		oIterator++;
	}
	CalculateSelectedFrame();
	m_vIsDirty = true;
}

/*
// FIXME: Visual C++ doesn't like the standard list sort, 
void EMGClipRepository::SortClips()
{
//	sort(m_oClipRepository.front(), m_oClipRepository.back(), CompareClipPositions());
//	sort(m_oSelectedClipRepository.front(), m_oSelectedClipRepository.back(), CompareClipPositions());

//	m_oClipRepository.sort(greater<class EMGClip *>());
//	m_oClipRepository.sort(greater_equal<EMGClip*>());//CompareClipPositions());

//	m_oSelectedClipRepository.sort(CompareClipPositions());

// FIXME: This is a temporary solution until a decent clip sorting algorithm has been written
	multimap<int64, EMGClip*> oSortingMap;
	list<EMGClip*>::iterator oListIterator = m_oClipRepository.begin();
	while(oListIterator != m_oClipRepository.end())
	{
//		oSortingMap[(*oListIterator) -> GetPosition()] = *oListIterator;
		oSortingMap.insert(pair<int64, EMGClip*>((*oListIterator) -> GetPosition(), *oListIterator));
		oListIterator++;
	}
	multimap<int64, EMGClip*>::iterator oMapIterator = oSortingMap.begin();
	m_oClipRepository.clear();
	while(oMapIterator != oSortingMap.end())
	{
//eo << "Sorting clips: " << (double) oMapIterator -> first << ef;
		m_oClipRepository.push_back(oMapIterator -> second);
		oMapIterator++;
	}
}
*/
// Returns a list of the new clips created by splitting clips
list<EMGClip*>* EMGClipRepository::SplitClips(list<EMGClip*>* p_opClips, int64 p_vPosition, int64 p_vTimeZoomScale)
{
	list<EMGClip*>::iterator oIterator = p_opClips -> begin();
	list<EMGClip*>* opNewClips = EM_new list<EMGClip*>(/*p_opClips -> size()*/);
	EMGClip* opNewClip;
	while(oIterator != p_opClips -> end())
	{
		opNewClip = (*oIterator) -> SplitClip(p_vPosition, p_vTimeZoomScale);
		opNewClips -> push_back(opNewClip);
		if((*oIterator) -> GetSelected())
			SetSelected(opNewClip, true);
		oIterator++;
	}
	CalculateSelectedFrame();
	EMGUIUtilities::SortClips(&m_oClipRepository);
	m_vIsDirty = true;
	return opNewClips;
}

void EMGClipRepository::Update()
{
	EMRect oOldFrame;
	EMRect oOldSelectedFrame;
	EMGUIUtilities::SortClips(&m_oClipRepository);
	CalculateFrame();
	CalculateSelectedFrame();
	Notify(EM_CLIP_REPOSITORY_UPDATED);
	m_oUpdateRect = m_oUpdateRect | oOldFrame | oOldSelectedFrame | m_oFrame | m_oSelectedFrame;
	Notify(EM_UPDATE_NEEDED);
}

