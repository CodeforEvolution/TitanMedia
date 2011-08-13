#include "CommandAddGMIDITrack.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGClipContainer.h"
#include "EMGTrack.h"
#include "EMGTrackRepository.h"
#include "EMMediaGlobals.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMView.h"
#include "MIDIGTrackInfo.h"
#include "TrackInfo.h"
#include "TrackView.h"
#include "TrackWindowStatusBar.h"

#include <string>

CommandAddGMIDITrack::CommandAddGMIDITrack(TrackView* p_opTrackView, TrackInfo* p_opTrackInfo) :
m_opTrack(NULL),
m_opTrackInfo(p_opTrackInfo),
m_opTrackView(p_opTrackView)
{
}

CommandAddGMIDITrack::~CommandAddGMIDITrack()
{
}

EMCommand* CommandAddGMIDITrack::CloneAndClear()
{
	CommandAddGMIDITrack* opCommand = new CommandAddGMIDITrack(m_opTrackView, m_opTrackInfo);
	opCommand -> SetTrack(m_opTrack);
	m_opTrack = NULL;
	return opCommand;
}

void* CommandAddGMIDITrack::ExecuteE(void* p_upParameterOne, void* p_upParameterTwo, void* p_upParameterThree)
{
	bool vIsCrippled = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SOFTWARE_IS_CRIPPLED)));
	if(vIsCrippled && EMGTrackRepository::Instance() -> GetNrTracks(EM_TYPE_MIDI) >= 2)
		return NULL;

	string oTrackName(static_cast<string*>(p_upParameterOne) -> c_str());
	int32 vMediaTrackID = *(static_cast<int32*>(p_upParameterTwo));

	m_opTrack = EMGTrackRepository::Instance() -> CreateTrack(oTrackName, EM_TYPE_MIDI, false);
	m_opTrack -> GetTrackInfo() -> SetMediaTrackID(vMediaTrackID);
	m_opTrack -> GetTrackInfo() -> SetTrack(m_opTrack); // FIXME: the track is set before this as well, should only be set once
	m_opTrack -> GetClipContainer() -> UpdateTimeScale(*(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TIME_ZOOM_SCALE))));

	if(dynamic_cast<MIDIGTrackInfo*>(m_opTrack -> GetTrackInfo()) -> GetActiveTrackInfo() == NULL)
		dynamic_cast<MIDIGTrackInfo*>(m_opTrack -> GetTrackInfo()) -> SetActive(true);

	GTrackInfo* opTrackInfo;
	for(uint32 vIndex = 0; vIndex < EMGTrackRepository::Instance() -> GetNrTracks(); vIndex++)
	{
		opTrackInfo = dynamic_cast<GTrackInfo*>(EMGTrackRepository::Instance() -> GetTrack(vIndex) -> GetTrackInfo());
		if(opTrackInfo -> GetSoloButton() -> GetOn())
			dynamic_cast<GTrackInfo*>(m_opTrack -> GetTrackInfo()) -> GetMuteButton() -> SetOnBySolo(true);
	}		
		
	char vOutput[80];
	sprintf(vOutput, "MIDI track created");
	TrackWindowStatusBar::Instance() -> WriteMessage(string(vOutput));

	return m_opTrack;
}

const char* CommandAddGMIDITrack::GetShortDescription()
{
	return "Add MIDI Track";
}

bool CommandAddGMIDITrack::IsUndoable()
{
	return true;
}

bool CommandAddGMIDITrack::RequiresParameters()
{
	return true;
}

void CommandAddGMIDITrack::SetTrack(EMGTrack* p_opTrack)
{
	m_opTrack = p_opTrack;
}

void CommandAddGMIDITrack::UndoE()
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
	m_opTrackView -> CalculateAndUpdateAll();//CalculateTrackFramesAndUpdate(m_opTrack);
*/
}
