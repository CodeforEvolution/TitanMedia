#include "MediaInfoCommandGetEffectTracks.h"

#include "EMMediaEffectTrackRepository.h"
#include "EMMediaEffectTrack.h"

MediaInfoCommandGetEffectTracks::MediaInfoCommandGetEffectTracks()
{
}

void* MediaInfoCommandGetEffectTracks::ExecuteE(void*, void*, void*)
{
	m_oReturnValue.clear();
	EMMediaEffectTrackRepository* opRep = EMMediaEffectTrackRepository::Instance();

	opRep -> LockContainer();
	try
	{
		opRep -> Rewind();
		while(opRep -> Current() != NULL)
		{
			if(!opRep -> Current() -> IsObjectDeleted())
				m_oReturnValue.push_back(opRep -> Current()-> GetID());

			opRep -> Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetEffectTracks::ExecuteE");
	}
	opRep -> UnlockContainer();
	return static_cast<void*>(&m_oReturnValue);
}

bool MediaInfoCommandGetEffectTracks::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetEffectTracks::IsUndoable()
{
	return false;
}