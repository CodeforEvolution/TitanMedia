#include "CommandDisplayClipPropertiesDialog.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGClipRepository.h"
#include "EMGClip.h"
#include "EMGTrack.h"
#include "EMGTrackInfo.h"
#include "EMGUIUtilities.h"
#include "EMMediaMidiEvent.h"
#include "EMView.h"
#include "EMWindow.h"
#include "TrackView.h"

#ifdef PLATFORM_WINDOWS	
#include <windows.h>
#endif

CommandDisplayClipPropertiesDialog::CommandDisplayClipPropertiesDialog(TrackView* p_opTrackView) :
m_opTrackView(p_opTrackView),
m_vX(0),
m_vY(0),
m_vOffset(0)
{
	eo << " *** Created Clip Properies Dialog Objects ***" << ef;
}

void CommandDisplayClipPropertiesDialog::CalculatePosition(int32 &p_vX, int32 &p_vY, EMGClip* p_opClip, EMGTrackInfo* p_opTrackInfo, ClipPropertiesDialog* m_opDialog)
{
	EMRect oTrackRect = p_opTrackInfo -> Frame();
	p_vX = m_opTrackView -> GetView() -> Bounds().m_vLeft;//m_vX;//oRect.GetWidth();
	p_vY = oTrackRect.m_vTop + oTrackRect.GetHeight() / 2;
//	if(p_vY == m_vY)
//		m_vOffset += 20;
//	else
//		m_vOffset = 0;
//	if(p_vY > m_vY)
//	{
//		m_vY = p_vY;
//		m_vOffset = 0;
//	}
	m_opTrackView -> GetView() -> ConvertToScreenCoordinates(p_vX, p_vY);
//	p_vX += m_vOffset;
//	p_vY += m_vOffset;
#ifdef PLATFORM_WINDOWS
	RECT sRect;
	//GetWindowRect(GetDesktopWindow(), &sRect);
	SystemParametersInfo(SPI_GETWORKAREA, 0, &sRect, 0);
	int32 test(m_opDialog -> GetDialogWindow() -> Bounds().GetHeight());
	if(p_vY + m_opDialog -> GetDialogWindow() -> Bounds().GetHeight() > sRect.bottom)
		p_vY = sRect.bottom - (m_vOffset + m_opDialog -> GetDialogWindow() -> Bounds().GetHeight());
	if(p_vY < sRect.top)
		p_vY = sRect.top - m_vOffset;
	if(p_vX + m_opDialog -> GetDialogWindow() -> Bounds().GetWidth() > sRect.right)
		p_vX = sRect.right - (m_vOffset + m_opDialog -> GetDialogWindow() -> Bounds().GetWidth());
	if(p_vX < sRect.left)
		p_vX = sRect.left - m_vOffset;

#endif
 
}

void CommandDisplayClipPropertiesDialog::ExecuteE()
{
	EMMediaType eType(EM_TYPE_UNKNOWN);
	EMGClip* opClip;
	EMGTrack* opTrack;
	EMGUIUtilities* opGUIUtilities = EMGUIUtilities::Instance();
	EMCommandRepository* opRepository = EMCommandRepository::Instance();
	int32 vClipID(0);
	int32 vEntryID(-1);
	int64 vValue(0);
	int32 vX(0);
	int32 vY(0);
	string oFileName;
	char vTemp[100];
	list<EMGClip*>* opClips = EMGClipRepository::Instance() -> GetSelectedClips();
	list<EMGClip*>::iterator oClipIterator = opClips -> begin();

	if(opClips -> size() == 0)
		return;

	while(oClipIterator != opClips -> end())
	{
		opClip = *oClipIterator;
		eType = opClip -> GetType();
		opTrack = opClip -> GetTrack();
		switch(eType)
		{
		case EM_TYPE_ANY_AUDIO:
			vClipID = opClip ->GetMediaClipID();
			vEntryID = *static_cast<int32*>(opRepository -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_ENTRY_ID, &vClipID, NULL, NULL));

			m_oAudioDialog.SetNameValue(opClip -> GetLabel());
			m_oAudioDialog.SetTypeValue("Audio");
			m_oAudioDialog.SetBeginValue(opGUIUtilities -> GetTimeString(opClip -> GetPosition(), EM_SMPTE));
			m_oAudioDialog.SetEndValue(opGUIUtilities -> GetTimeString(opClip -> GetPosition() + opClip -> GetLength(), EM_SMPTE));
			m_oAudioDialog.SetDurationValue(opGUIUtilities -> GetTimeString(opClip -> GetLength(), EM_SMPTE));
			
			vValue = *static_cast<int64*>(opRepository -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_NUM_CHANNELS, &vEntryID));
			m_oAudioDialog.SetChannelsValue(_i64toa(vValue, vTemp, 10));
			vValue = *static_cast<int64*>(opRepository -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_NUM_SAMPLES, &vEntryID));
			m_oAudioDialog.SetSamplesValue(_i64toa(vValue, vTemp, 10));

			oFileName = static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME, &vEntryID));  //Full path to copied file
			oFileName.erase(0, oFileName.find_last_of('\\') + 1);
			//oFileName.erase(oFileName.find_last_of('.'));
			m_oAudioDialog.SetFileValue(oFileName);

			vValue = *static_cast<int64*>(opRepository -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_DURATION, &vEntryID));
			m_oAudioDialog.SetFileLengthValue(opGUIUtilities -> GetTimeString(vValue, EM_SMPTE));
			vValue = *static_cast<int64*>(opRepository -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_MARK_IN, &vClipID));
			m_oAudioDialog.SetMarkInValue(opGUIUtilities -> GetTimeString(vValue, EM_SMPTE));
			vValue = *static_cast<int64*>(opRepository -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_MARK_OUT, &vClipID));
			m_oAudioDialog.SetMarkOutValue(opGUIUtilities -> GetTimeString(vValue, EM_SMPTE));

			m_oAudioDialog.Init(vX, vY, AUDIO);
			CalculatePosition(vX, vY, opClip, opTrack -> GetTrackInfo(), &m_oAudioDialog);
			m_oAudioDialog.MoveDialog(vX, vY);
			m_oAudioDialog.DisplayDialog();
			break;
		case EM_TYPE_MIDI:
			{
				char vTemp[100];
				uint32 vEvents(0);
				m_oMidiDialog.SetNameValue(opClip -> GetLabel());
				m_oMidiDialog.SetTypeValue("Midi");
				m_oMidiDialog.SetBeginValue(opGUIUtilities -> GetTimeString(opClip -> GetPosition(), EM_MEASURES));
				m_oMidiDialog.SetEndValue(opGUIUtilities -> GetTimeString(opClip -> GetPosition() + opClip -> GetLength(), EM_MEASURES));
				m_oMidiDialog.SetDurationValue(opGUIUtilities -> GetTimeString(opClip -> GetLength(), EM_MEASURES));
				int32 vMediaClipID(opClip ->GetMediaClipID());
				vEvents = (static_cast<list<EMMediaMIDIEvent*>*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_MIDI_EVENTS_FOR_CLIP, &vMediaClipID, NULL, NULL))) -> size();
				m_oMidiDialog.SetEventValue(string(itoa(vEvents, vTemp, 10)));
			
				m_oMidiDialog.Init(vX, vY, MIDI);
				CalculatePosition(vX, vY, opClip, opTrack -> GetTrackInfo(), &m_oMidiDialog);
				m_oMidiDialog.MoveDialog(vX, vY);
				m_oMidiDialog.DisplayDialog();
				break;
			}
		case EM_TYPE_ANY_VIDEO:
			vClipID = opClip ->GetMediaClipID();
			vEntryID = *static_cast<int32*>(opRepository -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_ENTRY_ID, &vClipID, NULL, NULL));

			m_oVideoDialog.SetNameValue(opClip -> GetLabel());
			m_oVideoDialog.SetTypeValue("Video");
			m_oVideoDialog.SetBeginValue(opGUIUtilities -> GetTimeString(opClip -> GetPosition(), EM_SMPTE));
			m_oVideoDialog.SetEndValue(opGUIUtilities -> GetTimeString(opClip -> GetPosition() + opClip -> GetLength(), EM_SMPTE));
			m_oVideoDialog.SetDurationValue(opGUIUtilities -> GetTimeString(opClip -> GetLength(), EM_SMPTE));

//				vValue = *static_cast<int64*>(opRepository -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_NUM_FRAMES, &vEntryID));
//				m_oVideoDialog.SetSamplesValue(_i64toa(vValue, vTemp, 10));

			oFileName = static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME, &vEntryID));  //Full path to copied file
			oFileName.erase(0, oFileName.find_last_of('\\') + 1);
			//oFileName.erase(oFileName.find_last_of('.'));
			m_oVideoDialog.SetFileValue(oFileName);

			m_oVideoDialog.SetDimentionValue(static_cast<const char*>(opRepository -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_VIDEO_DIMENSION, &vEntryID)));
			vValue = *static_cast<int64*>(opRepository -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_ENTRY_DURATION, &vEntryID));
			m_oVideoDialog.SetFileLengthValue(opGUIUtilities -> GetTimeString(vValue, EM_SMPTE));
			vValue = *static_cast<int64*>(opRepository -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_MARK_IN, &vClipID));
			m_oVideoDialog.SetMarkInValue(opGUIUtilities -> GetTimeString(vValue, EM_SMPTE));
			vValue = *static_cast<int64*>(opRepository -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_MARK_OUT, &vClipID));
			m_oVideoDialog.SetMarkOutValue(opGUIUtilities -> GetTimeString(vValue, EM_SMPTE));

			m_oVideoDialog.Init(vX, vY, VIDEO);
			CalculatePosition(vX, vY, opClip, opTrack -> GetTrackInfo(), &m_oVideoDialog);
			m_oVideoDialog.MoveDialog(vX, vY);
			m_oVideoDialog.DisplayDialog();
			break;
		case EM_TYPE_TRANSITION:
			vClipID = opClip ->GetMediaClipID();
	//		vEntryID = *static_cast<int32*>(opRepository -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_ENTRY_ID, &vClipID, NULL, NULL));

			m_oTransitionDialog.SetNameValue(opClip -> GetLabel());
			m_oTransitionDialog.SetTypeValue("Transition");
			m_oTransitionDialog.SetBeginValue(opGUIUtilities -> GetTimeString(opClip -> GetPosition(), EM_SMPTE));
			m_oTransitionDialog.SetEndValue(opGUIUtilities -> GetTimeString(opClip -> GetPosition() + opClip -> GetLength(), EM_SMPTE));
			m_oTransitionDialog.SetDurationValue(opGUIUtilities -> GetTimeString(opClip -> GetLength(), EM_SMPTE));

			m_oTransitionDialog.Init(vX, vY, TRANSITION);
			CalculatePosition(vX, vY, opClip, opTrack -> GetTrackInfo(), &m_oTransitionDialog);
			m_oTransitionDialog.MoveDialog(vX, vY);
			m_oTransitionDialog.DisplayDialog();
			break;
		}
		vX = 0;
		vY = 0;
		oClipIterator++;
	}
}

bool CommandDisplayClipPropertiesDialog::IsUndoable()
{
	return false;
}

bool CommandDisplayClipPropertiesDialog::RequiresParameters()
{
	return false;
}


