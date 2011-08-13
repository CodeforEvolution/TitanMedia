#include "MediaCommandMoveMIDIEvents.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMIDIClip.h"
#include "EMMediaMIDIEvent.h"
#include "EMMediaMIDIEventPosition.h"
#include "EMMediaMIDIEventRepository.h"
#include "EMMediaUtility.h"

MediaCommandMoveMIDIEvents::MediaCommandMoveMIDIEvents()
{
}

MediaCommandMoveMIDIEvents::MediaCommandMoveMIDIEvents(list<EMMediaMIDIEvent*>* p_opLastEvents, int64 p_vLastOffset)
	:	m_oLastEvents(*p_opLastEvents),
		m_vLastOffset(p_vLastOffset)
{
}

MediaCommandMoveMIDIEvents::~MediaCommandMoveMIDIEvents()
{
	m_oLastEvents.clear();
}

void* MediaCommandMoveMIDIEvents::ExecuteE(void* p_upMIDIEventList, void* p_upTimeOffsetInMicroSeconds, void*)
{
	list<EMMediaMIDIEvent*>* opEventList = static_cast<list<EMMediaMIDIEvent*>*>(p_upMIDIEventList);
	int64 vTimeOffset = *static_cast<int64*>(p_upTimeOffsetInMicroSeconds);

	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject(); 

	list<EMMIDIClip*> oTouchedClips;

	m_oLastEvents.clear();
	list<EMMediaMIDIEvent*>::iterator oIterator;
	for(oIterator = opEventList -> begin(); oIterator != opEventList -> end(); oIterator++)
	{
		EMMediaMIDIEvent* opEvent = (*oIterator);
		m_oLastEvents.push_back(opEvent);
		opEvent -> GetPosition() -> SetStart(opEvent -> GetPosition() -> GetStart() + vTimeOffset);
		if(find(oTouchedClips.begin(), oTouchedClips.end(), static_cast<EMMIDIClip*>(opEvent -> GetPosition() -> GetClip())) == oTouchedClips.end())
			oTouchedClips.push_back(static_cast<EMMIDIClip*>(opEvent -> GetPosition() -> GetClip()));
	}
	m_vLastOffset = vTimeOffset;

	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);

	//Sort the clip's internal list, as well as the global, potentially massive, event repository ( = not good - fix sometime soon!).
	list<EMMIDIClip*>::iterator oClipIterator;
	for(oClipIterator = oTouchedClips.begin(); oClipIterator != oTouchedClips.end(); oClipIterator++)
		(*oClipIterator) -> SortEvents(); 
	oTouchedClips.clear();
	opProject -> GetMIDIEventRepository() -> SortEvents();
	eo << "Done sorting, moving and everything...!!!!!" << ef;
#ifdef DEBUG
	opProject -> GetMIDIEventRepository() -> Show();
#endif
	return NULL;
}

bool MediaCommandMoveMIDIEvents::RequiresParameters()
{
	return true;
}

bool MediaCommandMoveMIDIEvents::IsUndoable()
{
	return false;
}

EMCommand* MediaCommandMoveMIDIEvents::CloneAndClear()
{
	return EM_new MediaCommandMoveMIDIEvents(&m_oLastEvents, m_vLastOffset);
}

void MediaCommandMoveMIDIEvents::UndoE()
{
	list<EMMediaMIDIEvent*> oList(m_oLastEvents);
	int64 vOffset = -m_vLastOffset;
	ExecuteE(static_cast<void*>(&oList), static_cast<void*>(&vOffset), NULL);
	oList.clear();
}
