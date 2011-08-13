#include "CommandAddGAudioTrack.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGClipContainer.h"
#include "EMGTrack.h"
#include "EMGTrackRepository.h"
#include "EMMediaGlobals.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMView.h"
#include "GTrackInfo.h"
#include "TrackInfo.h"
#include "TrackView.h"
#include "TrackWindowStatusBar.h"

#include <list>
#include <string>
#include <vector>

CommandAddGAudioTrack::CommandAddGAudioTrack(TrackView* p_opTrackView, TrackInfo* p_opTrackInfo) :
m_opTrack(NULL),
m_opTrackInfo(p_opTrackInfo),
m_opTrackView(p_opTrackView)
{
}

CommandAddGAudioTrack::CommandAddGAudioTrack(TrackView* p_opTrackView, TrackInfo* p_opTrackInfo, EMGTrack* p_opTrack) :
m_opTrack(p_opTrack),
m_opTrackInfo(p_opTrackInfo),
m_opTrackView(p_opTrackView)
{
}

CommandAddGAudioTrack::~CommandAddGAudioTrack()
{
}

EMCommand* CommandAddGAudioTrack::CloneAndClear()
{
	CommandAddGAudioTrack* opCommand = new CommandAddGAudioTrack(m_opTrackView, m_opTrackInfo, m_opTrack);
	m_opTrack = NULL;
	return opCommand;
}

void* CommandAddGAudioTrack::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	bool vIsCrippled = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SOFTWARE_IS_CRIPPLED)));
	if(vIsCrippled && EMGTrackRepository::Instance() -> GetNrTracks(EM_TYPE_ANY_AUDIO) >= 2)
		return NULL;

//		string oTrackName(*(static_cast<string*>(p_upParameterOne)));
	string oTrackName(static_cast<string*>(p_upParameterOne) -> c_str());
	int32 vMediaTrackID = *(static_cast<int32*>(p_upParameterTwo));

	m_opTrack = EMGTrackRepository::Instance() -> CreateTrack(oTrackName, EM_TYPE_ANY_AUDIO, false);
/*
	EMMediaType eType = EM_TYPE_ANY_AUDIO;
	list<int32> oList;
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_INPUTS, &eType, &oList, NULL);
	if(oList.size() <= 0)
		emerr << "WARNING! CommandAddGAudioTrack could not find any audio inputs in the system! Input will not be set for audio track!" << endl;
	else
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_TRACK_INPUT, &vMediaTrackID, &(oList.front()), NULL);

	oList.clear();
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_OUTPUTS, &eType, &oList, NULL);
	if(oList.size() <= 0)
		emerr << "WARNING! CommandAddGAudioTrack could not find any audio outputs in the system! Output will not be set for audio track!" << endl;
	else
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_TRACK_OUTPUT, &vMediaTrackID, &(oList.front()), NULL);
*/

	// Check if any track is soloed, then this track should be muted
	m_opTrack -> GetTrackInfo() -> SetMediaTrackID(vMediaTrackID);
	m_opTrack -> GetTrackInfo() -> SetTrack(m_opTrack); // FIXME: the track is set before this as well, should only be set once
	m_opTrack -> GetClipContainer() -> UpdateTimeScale(*(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TIME_ZOOM_SCALE))));

	GTrackInfo* opTrackInfo;
	for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
	{
		opTrackInfo = dynamic_cast<GTrackInfo*>(EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo());
		if(opTrackInfo -> GetSoloButton() -> GetOn())
			dynamic_cast<GTrackInfo*>(m_opTrack -> GetTrackInfo()) -> GetMuteButton() -> SetOnBySolo(true);
	}
	
	char vOutput[80];
	sprintf(vOutput, "Audio track created");
	TrackWindowStatusBar::Instance() -> WriteMessage(string(vOutput));

	return m_opTrack;
}

const char* CommandAddGAudioTrack::GetShortDescription()
{
	return "Add Audio Track";
}

bool CommandAddGAudioTrack::IsUndoable()
{
	return true;
}

bool CommandAddGAudioTrack::RequiresParameters()
{
	return true;
}

void CommandAddGAudioTrack::UndoE()
{
	if(m_opTrackView -> GetView() -> LockWindow())
	{
		vector<EMGTrack*> opTrackList;
		opTrackList.push_back(m_opTrack);
		EMCommandRepository::Instance() -> ExecuteCommandNoUndo(COMMAND_DELETE_TRACKS_FROM_LIST, &opTrackList);
		delete EMCommandRepository::Instance() -> GetUndoClone(COMMAND_DELETE_TRACKS_FROM_LIST);
		m_opTrackView -> GetView() -> UnlockWindow();
	}
	/*
	if(m_opTrackView -> GetView() -> LockWindow())
	{
		list<EMGTrack*>
		EMCommandRepository::Instance() -> ExecuteCommand(COMMAND_DELETE_CLIPS_FROM_LIST, 
		m_opTrackView -> GetView() -> UnlockWindow();
	}
	*/
	/*
	uint32 vPosition = EMGTrackRepository::Instance() -> GetTrackIndex(m_opTrack);
	if(vPosition != 0)
		vPosition--;
	EMGTrackRepository::Instance() -> DeleteTrack(m_opTrack);
	m_opTrackView -> CalculateAndUpdateAll();//CalculateTrackFramesAndUpdate(m_opTrack);
	*/
}
