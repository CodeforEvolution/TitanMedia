#include "CommandAddGVideoTrack.h"

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

#include <string>

CommandAddGVideoTrack::CommandAddGVideoTrack(TrackView* p_opTrackView, TrackInfo* p_opTrackInfo) :
m_opTrack(NULL),
m_opTrackInfo(p_opTrackInfo),
m_opTrackView(p_opTrackView)
{
}

CommandAddGVideoTrack::~CommandAddGVideoTrack()
{
}

EMCommand* CommandAddGVideoTrack::CloneAndClear()
{
	CommandAddGVideoTrack* opCommand = new CommandAddGVideoTrack(m_opTrackView, m_opTrackInfo);
	opCommand -> SetTrack(m_opTrack);
	m_opTrack = NULL;
	return opCommand;
}

void* CommandAddGVideoTrack::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	bool vIsCrippled = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SOFTWARE_IS_CRIPPLED)));
	if(vIsCrippled && EMGTrackRepository::Instance() -> GetNrTracks(EM_TYPE_ANY_VIDEO) >= 2)
		return NULL;

	string oTrackName(static_cast<string*>(p_upParameterOne) -> c_str());
	int32 vMediaTrackID = *(static_cast<int32*>(p_upParameterTwo));

	m_opTrack = EMGTrackRepository::Instance() -> CreateTrack(oTrackName, EM_TYPE_ANY_VIDEO, false);
/*
	EMMediaType eType = EM_TYPE_ANY_VIDEO;
	list<int32> oList;
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_INPUTS, &eType, &oList, NULL);
	if(oList.size() <= 0)
		emerr << "WARNING! CommandAddGVideoTrack could not find any video inputs in the system! Input will not be set for video track!" << endl;
	else
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_TRACK_INPUT, &vMediaTrackID, &(oList.front()), NULL);

	oList.clear();
	EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_OUTPUTS, &eType, &oList, NULL);
	if(oList.size() <= 0)
		emerr << "WARNING! CommandAddGVideoTrack could not find any video outputs in the system! Output will not be set for video track!" << endl;
	else
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_TRACK_OUTPUT, &vMediaTrackID, &(oList.front()), NULL);
*/
//		int32 vOutputID = 20;//11;//20;
//		int32 vInputID = 19;
//		;//cout_commented_out_4_release << "Now setting input for track by executing the MEDIA_COMMAND_SET_TRACK_INPUT command!" << endl;
//		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_TRACK_INPUT, &vMediaTrackID, &vInputID, NULL);
//		;//cout_commented_out_4_release << "!Now setting output for track by executing the MEDIA_COMMAND_SET_TRACK_OUTPUT command!" << endl;
//		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_TRACK_OUTPUT, &vMediaTrackID, &vOutputID, NULL);
	
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
	sprintf(vOutput, "Video track created");
	TrackWindowStatusBar::Instance() -> WriteMessage(string(vOutput));

	return m_opTrack;
}

const char* CommandAddGVideoTrack::GetShortDescription()
{
	return "Add Video Track";
}

bool CommandAddGVideoTrack::IsUndoable()
{
	return true;
}

bool CommandAddGVideoTrack::RequiresParameters()
{
	return true;
}

void CommandAddGVideoTrack::SetTrack(EMGTrack* p_opTrack)
{
	m_opTrack = p_opTrack;
}

void CommandAddGVideoTrack::UndoE()
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
	uint32 vPosition = EMGTrackRepository::Instance() -> GetTrackIndex(m_opTrack);
	if(vPosition != 0)
		vPosition--;
	EMGTrackRepository::Instance() -> DeleteTrack(m_opTrack);
	m_opTrackView -> CalculateAndUpdateAll();
*/
}
