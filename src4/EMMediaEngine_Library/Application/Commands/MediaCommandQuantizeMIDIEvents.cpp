#include "MediaCommandQuantizeMIDIEvents.h"

#include "EMMIDIGlobals.h"
#include "EMMediaMIDIEvent.h"
#include "EMMediaEngine.h"
#include "EMSettingsRepository.h"
#include "EMSettingIDs.h"
#include "EMMIDIClip.h"
#include "EMMediaMIDIEventPosition.h"
#include "EMMediaProject.h"
#include "EMMediaMIDIEventRepository.h"

#include <math.h>
#include <algorithm>

MediaCommandQuantizeMIDIEvents::MediaCommandQuantizeMIDIEvents(const map<EMMediaMIDIEvent*, int64>* p_opUndoInformation)
{
	if(p_opUndoInformation != NULL)
		m_oUndoInformation = (*p_opUndoInformation);
}

void* MediaCommandQuantizeMIDIEvents::ExecuteE(void* p_upEventList, void*, void*)
{
	m_oUndoInformation.clear();
	list<EMMIDIClip*> oTouchedClips;

	list<EMMediaMIDIEvent*>* opEvents = static_cast<list<EMMediaMIDIEvent*>*>(p_upEventList);

	double vBPM = (double) (*static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_TEMPO)));
	string oSignature = *static_cast<string*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_SIGNATURE));
	int32 vSignatureMajor = 0;
	int32 vSignatureMinor = 0;
	sscanf(oSignature.c_str(), "%d/%d", &vSignatureMajor, &vSignatureMinor);
	string oQuantizeMode = string(const_cast<const char*>(static_cast<string*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_AUTO_QUANTIZE_MODE)) -> c_str()));
	bool vAutoQuantizeNoteLengthsToo = *static_cast<bool*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_AUTO_QUANTIZE_LENGTH_ENABLED));

	double vAutoQuantizeMode = 1.0;

	if(oQuantizeMode == "1")
		vAutoQuantizeMode = 1.0;
	else if(oQuantizeMode == "1/2")
		vAutoQuantizeMode = 2.0;
	else if(oQuantizeMode == "1/2 tr")
		vAutoQuantizeMode = 3.0;
	else if(oQuantizeMode == "1/4")
		vAutoQuantizeMode = 4.0;
	else if(oQuantizeMode == "1/4 tr")
		vAutoQuantizeMode = 6.0;
	else if(oQuantizeMode == "1/8")
		vAutoQuantizeMode = 8.0;
	else if(oQuantizeMode == "1/8 tr")
		vAutoQuantizeMode = 12.0;
	else if(oQuantizeMode == "1/16")
		vAutoQuantizeMode = 16.0;
	else if(oQuantizeMode == "1/16 tr")
		vAutoQuantizeMode = 24.0;
	else if(oQuantizeMode == "1/32")
		vAutoQuantizeMode = 32.0;
	else if(oQuantizeMode == "1/32 tr")
		vAutoQuantizeMode = 48.0;
	else if(oQuantizeMode == "1/64")
		vAutoQuantizeMode = 64.0;
	else if(oQuantizeMode == "1/64 tr")
		vAutoQuantizeMode = 96.0;
	else
		vAutoQuantizeMode = 1.0;

	int64 vMicrosecondsPerMeasure = 60000000.0 * vSignatureMajor * vSignatureMinor / (4.0 * vBPM);

	list<EMMediaMIDIEvent*>::iterator oEventIterator;
	for(oEventIterator = opEvents -> begin(); oEventIterator != opEvents -> end(); oEventIterator++)
	{
		EMMediaMIDIEvent* opCurrentEvent = (*oEventIterator);

		if((opCurrentEvent -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_ON || opCurrentEvent -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_OFF) && vAutoQuantizeNoteLengthsToo || (! vAutoQuantizeNoteLengthsToo && opCurrentEvent -> m_vBaseType != EM_MIDI_EVENT_TYPE_NOTE_OFF))
		{
			//Fetch the event's current - unquantized - time...
			int64 vOriginalTime = *opCurrentEvent -> m_vpStart;

			//Then, based on the measure-duration find out which measure this event lies within... (floating point number with los grande decimalos)
			double vMeasureNumber = vBPM * vOriginalTime / (15000000.0 * vSignatureMajor * vSignatureMinor);
			
			//Then, remove the integer part of the number, and find out how far inside the measure this note is... This length is the time that we should
			//quantize to the quantize-note length/step...
			//TODO: Make portions of this calculation only once per setting-update (ie put it in MessageReceived instead!)
			int64 vIntoMeasure = (vMeasureNumber - floor(vMeasureNumber)) * 60000000.0 * vSignatureMajor * vSignatureMinor / (4.0 * vBPM);

			//Then, based on the length of one quantize-note (in this tempo and signature), calculate which note is the closed to this event's time...
			//...And set that note number as the "target note"...
			//TODO: Make portions of this calculation only once per setting-update (ie put it in MessageReceived instead!)
			double vQuantizeNoteNumber = (vIntoMeasure * vBPM * vSignatureMinor * vAutoQuantizeMode) / (4.0 * 4.0 * 60000000.0);
			if(ceil(vQuantizeNoteNumber) - vQuantizeNoteNumber > 0.5)
				vQuantizeNoteNumber = floor(vQuantizeNoteNumber);
			else
				vQuantizeNoteNumber = ceil(vQuantizeNoteNumber);

			//There. We know which measure this note is within.. And we know wich quantize-note inside that measure that is the closest to the event...
			//..All we need to do is multiply the measure-number and the quantize-note-number with their respective durations, and we find the new
			//quantized time that we should assign to this event!
			//TODO: Make this calculation only once per setting-update (ie put it in MessageReceived instead!)
			int64 vMicrosecondsPerQuantizeNote = 4.0 * 4.0 * 60000000.0 / (vBPM * vSignatureMinor * vAutoQuantizeMode);
			int64 vNewTime = floor(vMeasureNumber) * vMicrosecondsPerMeasure + vQuantizeNoteNumber * vMicrosecondsPerQuantizeNote;

			m_oUndoInformation[opCurrentEvent] = vOriginalTime;

			//If we're not quantizing the note off:s start time we must move it the same amount that we move the note on, since the duration must
			//stay the same even if the note on moves...
			if(! vAutoQuantizeNoteLengthsToo && opCurrentEvent -> GetBaseEventType() == EM_MIDI_EVENT_TYPE_NOTE_ON && opCurrentEvent -> m_opNoteOff != NULL)
			{
				int64 vQuantizeOffset = vNewTime - (*opCurrentEvent -> m_vpStart);
				(*(opCurrentEvent -> m_opNoteOff -> m_vpStart)) += vQuantizeOffset;
				m_oUndoInformation[opCurrentEvent -> m_opNoteOff] = *(opCurrentEvent -> m_opNoteOff -> m_vpStart);
			}

			*opCurrentEvent -> m_vpStart = vNewTime;

			//Make sure that (if it's a NOTE ON) the NOTE OFF event really comes AFTER the note on!
			if(opCurrentEvent -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_ON && *(opCurrentEvent -> m_vpStart) >= *(opCurrentEvent -> m_opNoteOff -> m_vpStart))
				*(opCurrentEvent -> m_opNoteOff -> m_vpStart) = *(opCurrentEvent -> m_vpStart) + 1;

			if(find(oTouchedClips.begin(), oTouchedClips.end(), static_cast<EMMIDIClip*>(opCurrentEvent -> GetPosition() -> GetClip())) == oTouchedClips.end())
				oTouchedClips.push_back(static_cast<EMMIDIClip*>(opCurrentEvent -> GetPosition() -> GetClip()));
		}
	}

	while(oTouchedClips.size() > 0)
	{
		oTouchedClips.front() -> SortEvents();
		oTouchedClips.pop_front();
	}
	EMMediaEngine::Instance() -> GetMediaProject() -> GetMIDIEventRepository() -> SortEvents();

	return NULL;
}

bool MediaCommandQuantizeMIDIEvents::RequiresParameters()
{
	return true;
}

void MediaCommandQuantizeMIDIEvents::UndoE()
{
	list<EMMIDIClip*> oTouchedClips;

	map<EMMediaMIDIEvent*, int64>::iterator oMapIterator;
	for(oMapIterator = m_oUndoInformation.begin(); oMapIterator != m_oUndoInformation.end(); oMapIterator++)
	{
		EMMediaMIDIEvent* opEvent = (*oMapIterator).first;
		int64 vOriginalTime = (*oMapIterator).second;

		*opEvent -> m_vpStart = vOriginalTime;

		if(find(oTouchedClips.begin(), oTouchedClips.end(), static_cast<EMMIDIClip*>(opEvent -> GetPosition() -> GetClip())) == oTouchedClips.end())
			oTouchedClips.push_back(static_cast<EMMIDIClip*>(opEvent -> GetPosition() -> GetClip()));
	}

	while(oTouchedClips.size() > 0)
	{
		oTouchedClips.front() -> SortEvents();
		oTouchedClips.pop_front();
	}
	EMMediaEngine::Instance() -> GetMediaProject() -> GetMIDIEventRepository() -> SortEvents();
}

bool MediaCommandQuantizeMIDIEvents::IsUndoable()
{
	return false;
}

EMCommand* MediaCommandQuantizeMIDIEvents::CloneAndClear()
{
	return EM_new MediaCommandQuantizeMIDIEvents(const_cast<const map<EMMediaMIDIEvent*, int64>*>(&m_oUndoInformation));
}

