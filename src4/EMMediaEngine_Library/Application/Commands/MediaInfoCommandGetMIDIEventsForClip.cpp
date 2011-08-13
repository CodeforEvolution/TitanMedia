#include "MediaInfoCommandGetMIDIEventsForClip.h"

#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMIDIClip.h"
#include "EMMediaClipRepository.h"
#include "EMMediaMIDIEvent.h"

MediaInfoCommandGetMIDIEventsForClip::MediaInfoCommandGetMIDIEventsForClip()
{
}

void* MediaInfoCommandGetMIDIEventsForClip::ExecuteE(void* p_upMediaClipID, void*, void*)
{
	int32 vClipID = *static_cast<int32*>(p_upMediaClipID);

	m_oReturnValue.clear();

	EMMediaEngine* opEngine = EMMediaEngine::Instance();
	EMMediaProject* opProject = opEngine -> GetMediaProject();
	EMMediaClipRepository* opClips = opProject -> GetMIDIClipRepository();
	EMMIDIClip* opClip = static_cast<EMMIDIClip*>(opClips -> Find(vClipID));
 
	list<EMMediaMIDIEvent*> oEventList;
	opClip -> GetAllMIDIEvents(&oEventList);
	list<EMMediaMIDIEvent*>::iterator oEventIterator;
	for(oEventIterator = oEventList.begin(); oEventIterator != oEventList.end(); oEventIterator++) 
	{
		EMMediaMIDIEvent* opEvent = (*oEventIterator);
		if(! opEvent -> IsObjectDeleted())
		{
			if(opEvent -> m_vBaseType == 144)
			{
				if(opEvent -> m_opNoteOff == NULL)
					eo << "Error! Found a NOTE ON that didn't have a correct m_opNoteOff pointer" << ef;
				if(*(opEvent -> m_opNoteOff) -> m_vpStart == NULL)
					eo << "Error! Found a NOTE ON with a NOTE OFF that didn't have a correct m_vpStart pointer" << ef;
			}
			m_oReturnValue.push_back(opEvent);
		}
	}
	return static_cast<void*>(&m_oReturnValue);
}

bool MediaInfoCommandGetMIDIEventsForClip::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetMIDIEventsForClip::IsUndoable()
{
	return false;
}