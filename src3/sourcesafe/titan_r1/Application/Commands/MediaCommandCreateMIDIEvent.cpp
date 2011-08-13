#include "MediaCommandCreateMIDIEvent.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMIDIClip.h"
#include "EMMediaMIDIEvent.h"
#include "EMMediaMIDIEventPosition.h"
#include "EMMediaMIDIEventRepository.h"
#include "EMMediaMIDITrack.h"
#include "EMMediaUtility.h"
#include "EMCommandRepository.h"
#include "EMMediaClipRepository.h"
#include "EMMediaTrackRepository.h"
#include "EMRealtimeMIDIOutputDescriptor.h"
#include "CommandIDs.h"

MediaCommandCreateMIDIEvent::MediaCommandCreateMIDIEvent()
{
}

MediaCommandCreateMIDIEvent::MediaCommandCreateMIDIEvent(list<EMMediaMIDIEvent*>* p_opRecentlyCreatedEvents)
{
	m_oRecentlyCreatedEvents.clear();
	list<EMMediaMIDIEvent*>::iterator oIter;
	for(oIter = p_opRecentlyCreatedEvents -> begin(); oIter != p_opRecentlyCreatedEvents -> end(); oIter++)
		m_oRecentlyCreatedEvents.push_back(*oIter);
}

/*

param1:
int32	vTrackID
int32	vClipID
int32	vNumEvents

param2:
int64	vType
int64	vData1
int64	vData2
int64	vNumBytes
int64	vStart
...
*/

void* MediaCommandCreateMIDIEvent::ExecuteE(void* p_upSeries1, void* p_upSeries2, void* p_upUndoCommand)
{

	//If the MediaClipID is -1 then we should find out which clip to add the event to, ourselves.
	//If the MediaClipID isn't -1, then we should, obviously, use that clip (if we find it) to add the event to.

	//The way we're finding out ourselves is:
	/*
	1.)		Look through the MIDI clip repository for a MIDI clip that is intersected by the new event (start time of event is within clip boundaries).
			Also, make sure that the currently investigated clip is on the same track as the one we're adding the MIDI event to (decided by vTrackID).
			If we find such a clip, just break and use that clip - ie use the very first clip we can find, in the clip repository.
			Now jump to §4.

	2.)		Look through all MIDI clips that is on the track with ID = vTrackID (see above). If we find a clip that ends ealier than the new event
			should start, calculate the distance from that clip's end to the event's start. If this distance is a max/min (we're dealing with
			negative as well as positive max/mins.. The clip with the distance (from clip end [if previous to event] or from clip start [if after event]
			closest to "zero" will be the clip that we add the event to.
			Now jump to §4.

	3.)		We didn't find any clip at all, so we'll have to create one, add it to the correct track (decided by vTrackID - see above).
			Make sure we add the clip to the MIDI clip repository.

	4.)		Add the event to the MIDI clip we have now...

	5.)		Sort the clip's event repository

	6.)		If we create a new clip, we're going to call COMMAND_ADD_G_MIDI_CLIP (but first find out which GUI track index that correlates to our media track ID).

	7.)		Return the new event.

  */

	m_oReturnValue.clear();
	m_oRecentlyCreatedEvents.clear();

	list<EMMediaMIDIEvent*> oLingeringNoteOns;

	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();
	EMMIDIClip* opSelectedMIDIClip = NULL;

	int32* vpParameterSeries1 = static_cast<int32*>(p_upSeries1);
	int64* vpParameterSeries2 = static_cast<int64*>(p_upSeries2);
	
	
	int32 vTargetTrackID = vpParameterSeries1[0];
	int32 vTargetClipID = vpParameterSeries1[1];
	int32 vNumEvents = vpParameterSeries1[2];
	
	EMMediaTrack* opTrack = opProject -> GetUsedTracks() -> Find(vTargetTrackID);
	if(opTrack == NULL)
		opTrack = opProject -> GetUnusedTracks() -> Find(vTargetTrackID);
	if(opTrack == NULL)
		EMDebugger("ERROR! Failed to find track!");

	EMMediaClipRepository* opClips = opProject -> GetMIDIClipRepository();
	bool vHadToCreateNewClip = false;

	int64 vStartTimeOfFirstEvent = vpParameterSeries2[4];
	uint64 vStartFrameOfFirstEvent = EMMediaUtility::Instance() -> TimeToFrames(vStartTimeOfFirstEvent, EMMediaUtility::Instance() -> GetSystemAudioFormat());
	
	int32 vEventNum = 0;
	int32 vOffset = 0;
	while(vEventNum < vNumEvents)
	{
		uint8 vpMIDIData[3];
		vpMIDIData[0] = vpParameterSeries2[vOffset + 0];
		vpMIDIData[1] = vpParameterSeries2[vOffset + 1];
		vpMIDIData[2] = vpParameterSeries2[vOffset + 2];

		EMMediaMIDIEvent* opMIDIEvent = EM_new EMMediaMIDIEvent();
		opMIDIEvent -> Set(vpParameterSeries2[vOffset + 4], vpParameterSeries2[vOffset + 3], vpMIDIData);

		eo << "CREATING NEW EVENT: " << ef;
		eo << (opMIDIEvent -> GetBaseEventType() == EM_MIDI_EVENT_TYPE_NOTE_ON ? "NOTE ON" : "NOTE OFF") << " time = " << (int32) (*opMIDIEvent->m_vpStart) << ef;

		vOffset += 5; //Parameter2 comes in series of 5 int64:s - 5 variables for each event to create
		vEventNum++;

//BEGIN OF ROUTINE FOR LOCATING MIDI CLIP
		if(vTargetClipID != -1 && opSelectedMIDIClip == NULL)
		{
			EMMediaClipRepository* opClips = opProject -> GetMIDIClipRepository();
			EMMIDIClip* opMIDIClip = static_cast<EMMIDIClip*>(opClips -> Find(vTargetClipID));
			if(opMIDIClip == NULL)
			{
				MessageBox(NULL, "Unknown MIDI clip ID in CreateMIDIEvent!", "Error!", MB_OK);
				return NULL; 
			}
			opSelectedMIDIClip = opMIDIClip;
		}
		else if(opSelectedMIDIClip == NULL)
		{   
			//Locate the correct clip to use...
			opClips -> LockContainer();
			opClips -> Rewind();		
			while(opClips -> Current() != NULL)
			{
				EMMIDIClip* opClip = static_cast<EMMIDIClip*>(opClips -> Current());
				if(opClip -> GetTrack() -> GetID() == vTargetTrackID && ! opClip -> IsObjectDeleted())
				{
					int64 x = opClip -> GetStart();
					if(x <= vStartFrameOfFirstEvent)
					{
						int64 y = opClip -> GetStop();
						if(y >= vStartFrameOfFirstEvent)
						{	//If we've found a clip that is intersected by this new event... 
							opSelectedMIDIClip = opClip;
							break;
						}
					}
				}
				opClips -> Next();
			}

			if(opSelectedMIDIClip == NULL)
			{
				int64 vMinDistance = 0;
				opClips -> Rewind();
				int64 vDiff = 0;
				bool vFirstRun = true;

				while(opClips -> Current() != NULL)
				{
					EMMIDIClip* opClip = static_cast<EMMIDIClip*>(opClips -> Current());
					
					if(opClip -> GetTrack() -> GetID() == vTargetTrackID && ! opClip -> IsObjectDeleted() && opClip -> GetStop() <= vStartFrameOfFirstEvent)
						vDiff = opClip -> GetStop() - vStartFrameOfFirstEvent; //Negative number!

					if(opClip -> GetTrack() -> GetID() == vTargetTrackID && ! opClip -> IsObjectDeleted() && opClip -> GetStart() >= vStartFrameOfFirstEvent)
						vDiff = opClip -> GetStop() - vStartFrameOfFirstEvent; //Negative number!

					if(opClip -> GetTrack() -> GetID() == vTargetTrackID && ! opClip -> IsObjectDeleted() && vFirstRun)
					{
						vMinDistance = vDiff;
						opSelectedMIDIClip = opClip;
						vFirstRun = false;
					}
					else if(opClip -> GetTrack() -> GetID() == vTargetTrackID && ! opClip -> IsObjectDeleted() && vDiff < 0 && vDiff > vMinDistance)
					{
						vMinDistance = vDiff;
						opSelectedMIDIClip = opClip;
					}
					else if(opClip -> GetTrack() -> GetID() == vTargetTrackID && ! opClip -> IsObjectDeleted() && vDiff >= 0 && vDiff < vMinDistance)
					{
						vMinDistance = vDiff;
						opSelectedMIDIClip = opClip;
					}
					opClips -> Next();
				}
			}
			opClips -> UnlockContainer();
		}

		if(opSelectedMIDIClip == NULL)
		{
			//No clip found... Create a new one!
			EMMIDIClip* opNewMIDIClip = EM_new EMMIDIClip(vStartFrameOfFirstEvent, static_cast<EMMediaMIDITrack*>(opTrack));
			opNewMIDIClip -> SetDestination(opTrack -> GetClipDataDestination());
			if(! opNewMIDIClip -> InitCheckE())
				EMDebugger("ERROR! Failed to create MIDI clip!");
			opNewMIDIClip -> SetStart(vStartFrameOfFirstEvent);
			opClips -> LockContainer();
			opClips -> Add(opNewMIDIClip);
			opClips -> UnlockContainer();
			opSelectedMIDIClip = opNewMIDIClip;
			vHadToCreateNewClip = true;
		}
//END OF ROUTINE FOR LOCATING MIDI CLIP

		opMIDIEvent -> GetPosition() -> SetDestination(static_cast<EMRealtimeMIDIOutputDescriptor*>(opSelectedMIDIClip -> GetTrack() -> GetTrackDestination()));
		opSelectedMIDIClip -> AddEvent(opMIDIEvent);  
		opMIDIEvent -> GetPosition() -> SetClip(opSelectedMIDIClip);
		if(opMIDIEvent -> GetBaseEventType() == EM_MIDI_EVENT_TYPE_NOTE_ON)
			oLingeringNoteOns.push_back(opMIDIEvent);
		else if(opMIDIEvent -> GetBaseEventType() == EM_MIDI_EVENT_TYPE_NOTE_OFF)
		{
			list<EMMediaMIDIEvent*>::iterator oI;
			EMMediaMIDIEvent* opNoteOnCandidate = NULL;
			int64 vDistance = -1;
			for(oI = oLingeringNoteOns.begin(); oI != oLingeringNoteOns.end(); oI++)
			{
				if((vDistance == -1 || (*opMIDIEvent -> m_vpStart - *((*oI) -> m_vpStart)) < vDistance) && (*oI) -> m_vData1 == opMIDIEvent -> m_vData1)
				{
					vDistance = (*opMIDIEvent -> m_vpStart - *((*oI) -> m_vpStart));
					opNoteOnCandidate = *oI;
				}
			}
			if(opNoteOnCandidate != NULL)
			{
				oLingeringNoteOns.remove(opNoteOnCandidate);
				opNoteOnCandidate -> m_opNoteOff = opMIDIEvent;
				opNoteOnCandidate -> m_vNoteOffID = opMIDIEvent -> GetID();

				//Actually store the events in the event repository! (we can't store the NoteOn until now, since it has to have a valid note-off pointer first).
				opProject -> GetMIDIEventRepository() -> Store(opMIDIEvent);
				opProject -> GetMIDIEventRepository() -> Store(opNoteOnCandidate);
			}
			else
				EMDebugger("ERROR! Attempted to create a NOTE OFF without having created a preceeding NOTE ON for that key!");
		}
		else
		{
			opProject -> GetMIDIEventRepository() -> Store(opMIDIEvent);
		}

		m_oReturnValue.push_back(opMIDIEvent);
		m_oRecentlyCreatedEvents.push_back(opMIDIEvent);
	}

	if(vHadToCreateNewClip)
	{
		int32 vMediaClipID = opSelectedMIDIClip -> GetID();
		int32 vMediaTrackID = opTrack -> GetID();
		int32 vGTrackIndex = *static_cast<int32*>(opEngine -> GetCommandRepository() -> ExecuteCommand(COMMAND_GET_TRACK_GUI_INDEX, &vMediaTrackID, NULL, NULL));
		opEngine -> GetCommandRepository() -> ExecuteCommandNoUndo(COMMAND_ADD_G_MIDI_CLIP, &vMediaClipID, &vGTrackIndex, &vStartTimeOfFirstEvent);
		*(reinterpret_cast<EMCommand**>(p_upUndoCommand)) = opEngine -> GetCommandRepository() -> GetUndoClone(COMMAND_ADD_G_MIDI_CLIP);
	}

	opSelectedMIDIClip -> SortEvents(); 
	opProject -> GetMIDIEventRepository() -> SortEvents();

	return static_cast<void*>(&m_oReturnValue);
}

bool MediaCommandCreateMIDIEvent::RequiresParameters()
{
	return true;
}

bool MediaCommandCreateMIDIEvent::IsUndoable()
{
	return false;
}

void MediaCommandCreateMIDIEvent::UndoE()
{
	list<EMMediaMIDIEvent*>::iterator oIter;
	for(oIter = m_oRecentlyCreatedEvents.begin(); oIter != m_oRecentlyCreatedEvents.end(); oIter++)
	{
		EMMediaMIDIEvent* opEvent = (*oIter);
		EMMIDIClip* opClip = static_cast<EMMIDIClip*>(opEvent -> GetPosition() -> GetClip());
		if(opEvent -> GetBaseEventType() != EM_MIDI_EVENT_TYPE_NOTE_OFF)
		{
			opEvent -> SetDeleted(true);
		}
		if(opClip -> CountEvents() == 0)
		{
			opClip -> SetDeleted(true);
		}
	}
	
}

EMCommand* MediaCommandCreateMIDIEvent::CloneAndClear()
{
	return EM_new MediaCommandCreateMIDIEvent(&m_oRecentlyCreatedEvents);
}
