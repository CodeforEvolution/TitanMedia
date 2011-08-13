/*******************************************************
* Creator: Jesper Svensson
* Component: none
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_CREATE_TRACK
#define __MEDIA_COMMAND_CREATE_TRACK

#include "EMCommand.h"


class MediaCommandCreateTrack : public EMCommand
{
public:
	MediaCommandCreateTrack();
	~MediaCommandCreateTrack();

	virtual void ExecuteE() = 0;
	virtual void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree) = 0;
	bool RequiresParameters();
	bool IsUndoable();

protected:
	void ExecuteCreateTrack(EMMediaType p_eType, const char* p_vpName, uint32 p_vPrio = 0);
	void* ExecuteCreateTrackP(EMMediaType p_eType, const char* p_vpName, uint32 p_vPrio = 0);
private:
	int32 m_vReturnValue;
};

#endif
