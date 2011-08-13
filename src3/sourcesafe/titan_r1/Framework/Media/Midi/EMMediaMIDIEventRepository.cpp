#include "EMMediaMIDIEventRepository.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMRealtimeMIDIOutputDescriptor.h"
#include "EMMediaMIDIEventPosition.h"
#include "EMMIDIClip.h"
#include "EMMediaUtility.h"
#include "EMMediaMIDITrack.h"
#include "EMSettingsRepository.h"
#include "EMSettingIDs.h"
#include "EMProjectDataSaver.h"
#include "EMProjectDataLoader.h"
#include "EMMediaMIDIEventList.h"
#include "EMMediaTimer.h"
#include "EMMediaTrackRepository.h"
#include "EMMIDIGlobals.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMMediaEngineBuild.h"

#include <math.h>

#define EM_QUANTIZE_WHOLE			1.0
#define EM_QUANTIZE_HALF			2.0
#define EM_QUANTIZE_HALF_TR			3.0
#define EM_QUANTIZE_QUARTER			4.0
#define EM_QUANTIZE_QUARTER_TR		6.0
#define EM_QUANTIZE_EIGHTH			8.0
#define EM_QUANTIZE_EIGHTH_TR		12.0
#define EM_QUANTIZE_SIXTEENTH		16.0
#define EM_QUANTIZE_SIXTEENTH_TR	24.0
#define EM_QUANTIZE_THIRDYSECOND	32.0
#define EM_QUANTIZE_THIRDYSECOND_TR	48.0
#define EM_QUANTIZE_SIXTYFOURTH		64.0
#define EM_QUANTIZE_SIXTYFOURTH_TR	96.0

EMMediaMIDIEventRepository::EMMediaMIDIEventRepository()
	:	m_vHasBeenPrepared(false),
		m_vNumPreAllocEvents(0),
		m_oppPreAllocatedEvents(NULL),
		m_vPreAllocGetPointer(0),
		m_vCurrentBPM(120),
		m_opSilentEvent(NULL),
		m_vSongSignatureMajor(1),
		m_vSongSignatureMinor(4),
		m_vAutoQuantizeMode(1.0),
		m_oAutoQuantizeMode(""),
		m_vAutoQuantizeEnabled(false),
		m_vAutoQuantizeNoteLengthsToo(false)
{
	m_opEventList = EM_new EMMediaMIDIEventList();
	m_opSilentEvent = EM_new EMMediaMIDIEvent();

	m_vCurrentBPM = *static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_TEMPO));
	string oSignature = *static_cast<string*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_SIGNATURE));
	int32 vSignatureMajor = 0;
	int32 vSignatureMinor = 0;
	sscanf(oSignature.c_str(), "%d/%d", &m_vSongSignatureMajor, &m_vSongSignatureMinor);
	m_vAutoQuantizeEnabled = *(static_cast<bool*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_AUTO_QUANTIZE_ENABLED)));
	m_oAutoQuantizeMode = string(const_cast<const char*>(static_cast<string*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_AUTO_QUANTIZE_MODE)) -> c_str()));
	m_vAutoQuantizeNoteLengthsToo = *static_cast<bool*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_AUTO_QUANTIZE_LENGTH_ENABLED));
	if(m_oAutoQuantizeMode == "1")
		m_vAutoQuantizeMode = 1.0;
	else if(m_oAutoQuantizeMode == "1/2")
		m_vAutoQuantizeMode = 2.0;
	else if(m_oAutoQuantizeMode == "1/2 tr")
		m_vAutoQuantizeMode = 3.0;
	else if(m_oAutoQuantizeMode == "1/4")
		m_vAutoQuantizeMode = 4.0;
	else if(m_oAutoQuantizeMode == "1/4 tr")
		m_vAutoQuantizeMode = 6.0;
	else if(m_oAutoQuantizeMode == "1/8")
		m_vAutoQuantizeMode = 8.0;
	else if(m_oAutoQuantizeMode == "1/8 tr")
		m_vAutoQuantizeMode = 12.0;
	else if(m_oAutoQuantizeMode == "1/16")
		m_vAutoQuantizeMode = 16.0;
	else if(m_oAutoQuantizeMode == "1/16 tr")
		m_vAutoQuantizeMode = 24.0;
	else if(m_oAutoQuantizeMode == "1/32")
		m_vAutoQuantizeMode = 32.0;
	else if(m_oAutoQuantizeMode == "1/32 tr")
		m_vAutoQuantizeMode = 48.0;
	else if(m_oAutoQuantizeMode == "1/64")
		m_vAutoQuantizeMode = 64.0;
	else if(m_oAutoQuantizeMode == "1/64 tr")
		m_vAutoQuantizeMode = 96.0;
	else
		m_vAutoQuantizeMode = 1.0;

	EMMediaEngine::Instance() -> GetSettingsRepository() -> AddListener(this);
}

EMMediaMIDIEventRepository::~EMMediaMIDIEventRepository()
{
	EMMediaEngine::Instance() -> GetSettingsRepository() -> RemoveListener(this);
	delete m_opEventList;
	for(uint64 vPos = 0; vPos < m_vNumPreAllocEvents; vPos++) 
		delete m_oppPreAllocatedEvents[vPos];
	m_vNumPreAllocEvents = 0;
	delete [] m_oppPreAllocatedEvents;
	m_oppPreAllocatedEvents = NULL;
	delete m_opSilentEvent;
}

#ifdef _DEBUG
void EMMediaMIDIEventRepository::Show()
{
	m_opEventList -> Show(); //DEBUGGING
}
#endif

void EMMediaMIDIEventRepository::OnItemAdd()
{
	//Actually, we should set the project's dirty flag, since the project IS dirty when MIDI events are added...
	//However, since this method is called so many times (when you finish a recording, for instance) we don't
	//put the make-dirty-stuff here - but instead put it in the EMMediaMIDITrack's method where the recording
	//is finalized...
}

void EMMediaMIDIEventRepository::OnItemRemove()
{
	//Same story here!
}

uint64 EMMediaMIDIEventRepository::Size()
{
	m_opEventList -> Lock();
	uint64 vSize = m_opEventList -> Size();
	m_opEventList -> Unlock();
	return vSize;
}

void EMMediaMIDIEventRepository::DiscardAllEvents()
{
}

void EMMediaMIDIEventRepository::ResetReadPosition()
{
	m_opEventList -> Lock();
	m_opEventList -> Reset();
	m_opEventList -> Unlock();
}

bool EMMediaMIDIEventRepository::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	if(p_vMessage == EM_SETTING_UPDATED)
	{
		float vBPM = *static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_TEMPO));
		string oSignature = *static_cast<string*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_SIGNATURE));
		int32 vSignatureMajor = 0;
		int32 vSignatureMinor = 0;
		sscanf(oSignature.c_str(), "%d/%d", &vSignatureMajor, &vSignatureMinor);
		bool vAutoQuantizeEnabled = *(static_cast<bool*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_AUTO_QUANTIZE_ENABLED)));
		string oQuantizeMode = string(const_cast<const char*>(static_cast<string*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_AUTO_QUANTIZE_MODE)) -> c_str()));
		bool vAutoQuantizeNoteLengthsToo = *static_cast<bool*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_MIDI_AUTO_QUANTIZE_LENGTH_ENABLED));

		if(vBPM != m_vCurrentBPM ||
			m_vSongSignatureMajor != vSignatureMajor ||
			m_vSongSignatureMinor != vSignatureMinor ||
			m_vAutoQuantizeEnabled != vAutoQuantizeEnabled ||
			m_oAutoQuantizeMode != oQuantizeMode ||
			m_vAutoQuantizeNoteLengthsToo != vAutoQuantizeNoteLengthsToo)
		{
			m_vCurrentBPM = vBPM;
			m_vSongSignatureMajor = vSignatureMajor;
			m_vSongSignatureMinor = vSignatureMinor;
			m_vAutoQuantizeEnabled = vAutoQuantizeEnabled;
			m_oAutoQuantizeMode = oQuantizeMode;
			m_vAutoQuantizeNoteLengthsToo = vAutoQuantizeNoteLengthsToo;
			if(m_oAutoQuantizeMode == "1")
				m_vAutoQuantizeMode = 1.0;
			else if(m_oAutoQuantizeMode == "1/2")
				m_vAutoQuantizeMode = 2.0;
			else if(m_oAutoQuantizeMode == "1/2 tr")
				m_vAutoQuantizeMode = 3.0;
			else if(m_oAutoQuantizeMode == "1/4")
				m_vAutoQuantizeMode = 4.0;
			else if(m_oAutoQuantizeMode == "1/4 tr")
				m_vAutoQuantizeMode = 6.0;
			else if(m_oAutoQuantizeMode == "1/8")
				m_vAutoQuantizeMode = 8.0;
			else if(m_oAutoQuantizeMode == "1/8 tr")
				m_vAutoQuantizeMode = 12.0;
			else if(m_oAutoQuantizeMode == "1/16")
				m_vAutoQuantizeMode = 16.0;
			else if(m_oAutoQuantizeMode == "1/16 tr")
				m_vAutoQuantizeMode = 24.0;
			else if(m_oAutoQuantizeMode == "1/32")
				m_vAutoQuantizeMode = 32.0;
			else if(m_oAutoQuantizeMode == "1/32 tr")
				m_vAutoQuantizeMode = 48.0;
			else if(m_oAutoQuantizeMode == "1/64")
				m_vAutoQuantizeMode = 64.0;
			else if(m_oAutoQuantizeMode == "1/64 tr")
				m_vAutoQuantizeMode = 96.0;
			else
				m_vAutoQuantizeMode = 1.0;
		}
	}
	return true;
}

void EMMediaMIDIEventRepository::Store(EMMediaMIDIEvent* p_opEvent)
{
	m_opEventList -> Lock();

	if((p_opEvent -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_ON || p_opEvent -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_OFF) && m_vAutoQuantizeEnabled && ((m_vAutoQuantizeNoteLengthsToo) || (! m_vAutoQuantizeNoteLengthsToo && p_opEvent -> m_vBaseType != EM_MIDI_EVENT_TYPE_NOTE_OFF)))
	{
		//First, calculate the length of one measure in the current signature and tempo...
		//TODO: Make this calculation only once per setting-update (ie put it in MessageReceived instead!)
		int64 vMicrosecondsPerMeasure = 60000000.0 * m_vSongSignatureMajor * m_vSongSignatureMinor / (4 * m_vCurrentBPM);

		//Fetch the event's current - unquantized - time...
		int64 vTime = *p_opEvent -> m_vpStart;

		//Then, based on the measure-duration find out which measure this event lies within... (floating point number with los grande decimalos)
		float vMeasureNumber = m_vCurrentBPM * vTime / (15000000.0 * m_vSongSignatureMajor * m_vSongSignatureMinor);
		
		//Then, remove the integer part of the number, and find out how far inside the measure this note is... This length is the time that we should
		//quantize to the quantize-note length/step...
		//TODO: Make portions of this calculation only once per setting-update (ie put it in MessageReceived instead!)
		int64 vIntoMeasure = (vMeasureNumber - floor(vMeasureNumber)) * 60000000.0 * m_vSongSignatureMajor * m_vSongSignatureMinor / (4 * m_vCurrentBPM);

		//Then, based on the length of one quantize-note (in this tempo and signature), calculate which note is the closed to this event's time...
		//...And set that note number as the "target note"...
		//TODO: Make portions of this calculation only once per setting-update (ie put it in MessageReceived instead!)
		float vQuantizeNoteNumber = (vIntoMeasure * m_vCurrentBPM * m_vSongSignatureMinor * m_vAutoQuantizeMode) / (4 * 4 * 60000000.0);
		if(ceil(vQuantizeNoteNumber) - vQuantizeNoteNumber > 0.5)
			vQuantizeNoteNumber = floor(vQuantizeNoteNumber);
		else
			vQuantizeNoteNumber = ceil(vQuantizeNoteNumber);

		//There. We know which measure this note is within.. And we know wich quantize-note inside that measure that is the closest to the event...
		//..All we need to do is multiply the measure-number and the quantize-note-number with their respective durations, and we find the new
		//quantized time that we should assign to this event!
		//TODO: Make this calculation only once per setting-update (ie put it in MessageReceived instead!)
		int64 vMicrosecondsPerQuantizeNote = 4 * 4 * 60000000.0 / (m_vCurrentBPM * m_vSongSignatureMinor * m_vAutoQuantizeMode);
		int64 vNewTime = floor(vMeasureNumber) * vMicrosecondsPerMeasure + vQuantizeNoteNumber * vMicrosecondsPerQuantizeNote;

		//If we're not quantizing the note off:s start time we must move it the same amount that we move the note on, since the duration must
		//stay the same even if the note on moves...
		if(! m_vAutoQuantizeNoteLengthsToo && p_opEvent -> GetBaseEventType() == EM_MIDI_EVENT_TYPE_NOTE_ON && p_opEvent -> m_opNoteOff != NULL)
		{
			int64 vQuantizeOffset = vNewTime - (*p_opEvent -> m_vpStart);
			(*(p_opEvent -> m_opNoteOff-> m_vpStart)) += vQuantizeOffset;
		}
		*p_opEvent -> m_vpStart = vNewTime;

		//Make sure that (if it's a NOTE ON) the NOTE OFF event really comes AFTER the note on!
		if(p_opEvent -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_ON && *(p_opEvent -> m_vpStart) >= *(p_opEvent -> m_opNoteOff -> m_vpStart))
			*(p_opEvent -> m_opNoteOff -> m_vpStart) = *(p_opEvent -> m_vpStart) + 1;
	}

	m_opEventList -> Store(p_opEvent);
	m_opEventList -> Unlock();
}

void EMMediaMIDIEventRepository::RemoveEvent(EMMediaMIDIEvent* p_opEvent)
{
	m_opEventList -> Lock();
	m_opEventList -> Store(p_opEvent);
	m_opEventList -> Unlock();
}

EMMediaMIDIEvent* EMMediaMIDIEventRepository::GetNextEvent(int64 p_vSongTime)
{
	m_opEventList -> Lock();
#ifdef ENABLE_SONG_LENGTH_LIMITATION
	EMMediaMIDIEvent* opEvent = (p_vSongTime < SONG_LENGTH_LIMIT ? (*m_opEventList)[p_vSongTime] : NULL);
#else
	EMMediaMIDIEvent* opEvent = (*m_opEventList)[p_vSongTime];
#endif
	m_opEventList -> Unlock();

	if(opEvent == NULL)
	{
		*(m_opSilentEvent -> m_vpStart) = EMMediaTimer::Instance() -> NowTime() + 1000000;

		EMRealtimeMIDIOutputDescriptor* opOutput = NULL;
		if(EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks() -> First(EM_TYPE_MIDI) != NULL)
			opOutput = static_cast<EMRealtimeMIDIOutputDescriptor*>(EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks() -> First(EM_TYPE_MIDI) -> GetTrackDestination());
		else if(EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks() -> First(EM_TYPE_MIDI) != NULL)
			opOutput = static_cast<EMRealtimeMIDIOutputDescriptor*>(EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks() -> First(EM_TYPE_MIDI) -> GetTrackDestination());
		else
		{
			opOutput = static_cast<EMRealtimeMIDIOutputDescriptor*>(EMOutputRepository::Instance() -> First(EM_TYPE_MIDI));
		}
		m_opSilentEvent -> GetPosition() -> SetDestination(opOutput);
		
		return m_opSilentEvent; //Just an event to fool the producer into moving time forward!
	}

	return opEvent;
}

#define EM_MEDIA_MIDI_PREALLOC_BUFFER 2097152 //Allocate 2 MB of MIDI buffers
bool EMMediaMIDIEventRepository::PreAllocEvents()
{
	if(m_vNumPreAllocEvents == 0)
	{
		uint64 vNum = (EM_MEDIA_MIDI_PREALLOC_BUFFER - (m_vNumPreAllocEvents * sizeof(EMMediaMIDIEvent))) / sizeof(EMMediaMIDIEvent);
		m_oppPreAllocatedEvents = new EMMediaMIDIEvent*[vNum];
		for(uint64 vPos = 0; vPos < vNum; vPos++)
			m_oppPreAllocatedEvents[vPos] = EM_new EMMediaMIDIEvent();
		m_vNumPreAllocEvents = vNum;
		m_vPreAllocGetPointer = 0;
	}
	else
	{
		for(uint64 vPos = 0; vPos < m_vPreAllocGetPointer; vPos++)
			m_oppPreAllocatedEvents[vPos] = EM_new EMMediaMIDIEvent();
		m_vPreAllocGetPointer = 0;
	}
	return true;
}

EMMediaMIDIEvent* EMMediaMIDIEventRepository::AllocEvent()
{
	if((int64) m_vPreAllocGetPointer == ((int64) m_vNumPreAllocEvents * 0.75))
		MessageBox(NULL, "WARNING! Preallocated buffer of MIDI events is running dangerously low (<25% left)", "Warning", MB_OK | MB_ICONWARNING);
	if((int64) m_vPreAllocGetPointer == ((int64) m_vNumPreAllocEvents * 0.90))
		MessageBox(NULL, "WARNING! Preallocated buffer MIDI events is running dangerously low (<10% left)", "Warning", MB_OK | MB_ICONWARNING);
	if((int64) m_vPreAllocGetPointer == ((int64) m_vNumPreAllocEvents * 0.99))
		MessageBox(NULL, "WARNING! Preallocated buffer MIDI events is running dangerously low (<1% left)", "Warning", MB_OK | MB_ICONWARNING);
	
	if(m_vPreAllocGetPointer > (m_vNumPreAllocEvents - 1))
		return EM_new EMMediaMIDIEvent();
	
	EMMediaMIDIEvent* opEvent = m_oppPreAllocatedEvents[m_vPreAllocGetPointer];
	m_oppPreAllocatedEvents[m_vPreAllocGetPointer] = NULL;
	m_vPreAllocGetPointer++;
	return opEvent;
}

EMMediaMIDIEvent* EMMediaMIDIEventRepository::FindNoteOnFor(EMMediaMIDIEvent* p_opNoteOffEvent)
{
	m_opEventList -> Lock();

	EMMediaMIDIEventItem* opItem = m_opEventList -> FirstItem();
	EMMediaMIDIEventItem* opCandidate = NULL;

	while(opItem != NULL)
	{
		if(opItem -> m_opEvent -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_ON && opItem -> m_opEvent -> m_vData1 == p_opNoteOffEvent -> m_vData1 && static_cast<EMMIDIClip*>(opItem -> m_opEvent -> GetPosition() -> GetClip()) -> GetTrack() == static_cast<EMMIDIClip*>(p_opNoteOffEvent -> GetPosition() -> GetClip()) -> GetTrack() && *(opItem -> m_opEvent -> m_vpStart) <= *(p_opNoteOffEvent -> m_vpStart))
		{
			opCandidate = opItem;
		}
		opItem = opItem -> m_opNext;
	}
	m_opEventList -> Unlock();
	return (opCandidate != NULL ? opCandidate -> m_opEvent : NULL);
}

void EMMediaMIDIEventRepository::AdjustToBPM(uint32 p_vNewBPM)
{
	float vNewBPM = static_cast<float>(p_vNewBPM);
	float vOldBPM = static_cast<float>(m_vCurrentBPM);

	m_opEventList -> Lock();
	EMMediaMIDIEventItem* opItem = m_opEventList -> FirstItem();
	while(opItem != NULL)
	{
		(*(opItem -> m_opEvent -> m_vpStart)) = static_cast<int64>(static_cast<float>(*(opItem -> m_opEvent -> m_vpStart)) * vOldBPM / vNewBPM);
		opItem = opItem -> m_opNext;
	}
	m_vCurrentBPM = vNewBPM;
	m_opEventList -> Unlock();
}

void EMMediaMIDIEventRepository::SortEvents()
{
	m_opEventList -> Lock();
	m_opEventList -> Recalculate();
	m_opEventList -> Unlock();
}

bool EMMediaMIDIEventRepository::SaveData(EMProjectDataSaver* p_opSaver)
{
	m_opEventList -> Lock();
	p_opSaver -> SaveString("EMT");
	bool vResult = m_opEventList -> SaveData(p_opSaver);
	m_opEventList -> Unlock();
	return vResult;
}

//Make sure that all MIDI clips are loaded prior to loading event data 
//(coz' the EMMediaMIDIEventPosition tries to search for the parent MIDI clip when loaded).
bool EMMediaMIDIEventRepository::LoadData(EMProjectDataLoader* p_opLoader)
{ 
	if(string(p_opLoader -> LoadString()) != string("EMT"))
		return false;

	list<EMMediaMIDIEvent*> oOns;

	m_opEventList -> Lock();
	int64 vNumEvents = p_opLoader -> LoadUInt64();

	char vNum[10];
	_i64toa(vNumEvents, vNum, 10);

	for(int64 vIndex = 0; vIndex < vNumEvents; vIndex++)
	{
		EMMediaMIDIEvent* opEvent = EM_new EMMediaMIDIEvent();
		opEvent -> LoadData(p_opLoader);
		if(opEvent -> m_vNoteOffID != -1)
			oOns.push_back(opEvent);
		else if(opEvent -> m_vBaseType == EM_MIDI_EVENT_TYPE_NOTE_OFF)
		{
			list<EMMediaMIDIEvent*>::iterator oOn;
			for(oOn = oOns.begin(); oOn != oOns.end(); oOn++)
			{
				if((*oOn) -> m_vNoteOffID == opEvent -> GetID())
				{
					(*oOn) -> m_opNoteOff = opEvent;
					EMMediaMIDIEvent* opOnEventToRemove = *oOn;
					oOns.remove(opOnEventToRemove);
					opOnEventToRemove -> m_vNoteOffID = -1;
					break;
				}
			}
		}

		int64 vTime = *opEvent -> m_vpStart;
		char vChar1[10];
		char vChar2[10];
		_i64toa(vTime, vChar1, 10);
		uint8 vBaseType = opEvent -> GetBaseEventType();
		itoa((int) vBaseType, vChar2, 10);
		char vpEventNum[10];
		_i64toa(vIndex, vpEventNum, 10);
		m_opEventList -> Store(opEvent);
	}
	m_opEventList -> Unlock();

	SortEvents();

	return true;
}

void EMMediaMIDIEventRepository::MoveEvent(EMMediaMIDIEvent* p_opEvent, int64 p_vNewTime)
{
	m_opEventList -> Lock();
	m_opEventList -> MoveEvent(p_opEvent, p_vNewTime);	
	m_opEventList -> Unlock();
}

EMMediaMIDIEvent* EMMediaMIDIEventRepository::FindPreviousEvent(int64 p_vTime, EMMediaMIDITrack* p_opTrack, EMMIDIEventType p_eType, EMMIDIControlChangeType p_eControlChangeType)
{
	m_opEventList -> Lock();
	EMMediaMIDIEventItem* opItem = m_opEventList -> FirstItem();
	EMMediaMIDIEventItem* opSearchStartItem = NULL;
	EMMediaMIDIEvent* opEventFound = NULL;

	//Make bools to tell what we're searching for...
	bool vSearchTrack = (p_opTrack != NULL);
	bool vSearchType = (p_eType != EM_MIDI_EVENT_TYPE_UNKNOWN);
	bool vSearchCCType = (p_eControlChangeType != EM_MIDI_CONTROL_CHANGE_TYPE_UNKNOWN);

	//First, locate the very first event in the repository that comes ON or AFTER the specified time!
	while(opItem != NULL)
	{
		if(*opItem -> m_opEvent -> m_vpStart > p_vTime)
		{
			opSearchStartItem = opItem;
			break;
		}
		opItem = opItem -> m_opNext;
	}

	//There, now that we (possibly) have that event (see comment above), we should iterate backwards until we find an event
	//that meets the search critera (if any)
	bool vFirstCondition = false;
	bool vSecondCondition = false;
	bool vThirdCondition = false;
	if(opSearchStartItem != NULL)
	{
		if(opSearchStartItem -> m_opPrevious != NULL)
		{	//We have to have the previous event, since the start event is AT or AFTER the specified time!
			opItem = opSearchStartItem -> m_opPrevious;
			while(opItem != NULL)
			{
				//Now, we should check our search criterias. First off is the "parent track" matching...
				if(vSearchTrack && static_cast<EMMIDIClip*>(opItem -> m_opEvent -> GetPosition() -> GetClip()) != NULL && 
					static_cast<EMMIDIClip*>(opItem -> m_opEvent -> GetPosition() -> GetClip()) -> GetTrack() == p_opTrack)
					vFirstCondition = true;

				//Now, we move on and check the other critera - the event type
				if(vSearchType && opItem -> m_opEvent -> m_vBaseType == p_eType)
					vSecondCondition = true;

				//Finally, if we're looking for a certain control change type...
				if(vSearchCCType && opItem -> m_opEvent -> m_vBaseType == EM_MIDI_EVENT_TYPE_CONTROL_CHANGE && 
					opItem -> m_opEvent -> m_vData1 == p_eControlChangeType)
					vThirdCondition = true;

				//There... Are we done?
				if(vSearchTrack == vFirstCondition && vSearchType == vSecondCondition && vSearchCCType == vThirdCondition)
				{
					opEventFound = opItem -> m_opEvent;
					break;
				}
				else
					; //No criterias matched... So continue the search (iterate one step backwards - if possible)!

				opItem = opItem -> m_opPrevious;
			}
		}
	}
	m_opEventList -> Unlock();
	return opEventFound;
}
