/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_DELETE_CLIP
#define __MEDIA_COMMAND_DELETE_CLIP

#include "EMCommand.h"
#include "EMMediaMIDIEvent.h"

class EMMediaEffectTrack;

class MediaCommandDeleteClip : public EMCommand
{
public:
	MediaCommandDeleteClip();
	MediaCommandDeleteClip(int32 p_vDeletedClipID, int32 p_vClipFxID, const list<EMMediaMIDIEvent*>* p_opDeletedMIDIEvents);
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaClipID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();
	void UndoE();
	EMCommand* CloneAndClear();

private:
	int32 m_vDeletedClipID;
	int32 m_vFxID;
	list<EMMediaMIDIEvent*> m_oDeletedMIDIEvents;
};

#endif
