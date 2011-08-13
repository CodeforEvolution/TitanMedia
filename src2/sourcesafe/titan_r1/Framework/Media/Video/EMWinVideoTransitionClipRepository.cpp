#include "EMWinVideoTransitionClipRepository.h"

#include "EMMediaTrack.h"

EMWinVideoTransitionClipRepository::EMWinVideoTransitionClipRepository()
	: EMMediaClipRepository(EM_TYPE_TRANSITION)
{

}

EMWinVideoTransitionClipRepository::~EMWinVideoTransitionClipRepository()
{

}

void EMWinVideoTransitionClipRepository::GetNextBuffers(list<EMMediaDataBuffer*>* p_opList, EMMediaType p_eType, int64 p_vTimeNow, bool p_vSeeking)
{
	EMDebugger("EMWinVideoTransitionClipRepository::GetNextBuffers: Why oh why do you you call me!?!?");
	return;
}

bool EMWinVideoTransitionClipRepository::InitCheckE()
{
	return true;
}

EMMediaClip *EMWinVideoTransitionClipRepository::GetTransitionsInTimeslot(int64 p_vTime, EMMediaTrack *p_opTrack)
{
	EMMediaClip* opClip = NULL;

	LockContainer();
	Rewind();

	while (Current() != NULL)
	{
		if (static_cast<EMVideoTransitionClip*>(Current()) -> IsActiveNow(p_vTime) && Current() -> GetTrack() -> GetID() == p_opTrack -> GetID())
		{
			opClip = Current();
			break;
		}
		Next();
	}

	UnlockContainer();

	return opClip;	
}

bool EMWinVideoTransitionClipRepository::GetClipsInTimeslot(list<EMMediaClip*>* p_opList, int64 p_vTime)
{
	p_opList -> clear();

	LockContainer();
	Rewind();

	while (Current() != NULL)
	{
		if(Current() -> GetTrack() -> GetMuteState() == EM_MUTE_OFF)
		{
			if(static_cast<EMVideoTransitionClip*>(Current()) -> IsActiveNow(p_vTime))
			{
				EMMediaClip* opClip = Current();
				int32 vPrio = opClip -> GetTrack() ->GetPriority();

				list<EMMediaClip*>::iterator i;
				

				for(i = p_opList -> begin(); i != p_opList -> end(); ++i)
				{
					int32 vPrio2 = (*i) -> GetTrack() -> GetPriority();

					if(vPrio2 > vPrio)
						break;		
				}

				p_opList -> insert(i, opClip);
			}
		}
		Next();
	}

	UnlockContainer();

	return p_opList -> size() != 0;	
}
