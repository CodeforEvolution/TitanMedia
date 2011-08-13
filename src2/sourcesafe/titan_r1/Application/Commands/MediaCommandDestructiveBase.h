/*******************************************************
* Creator: Martin Johansson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_DESTRUCTIVE_BASE
#define __MEDIA_COMMAND_DESTRUCTIVE_BASE

#include "EMCommand.h"

class EMMediaClip;
class EMDestructivePlugin;

class MediaCommandDestructiveBase : public EMCommand
{
public:
	MediaCommandDestructiveBase();
	~MediaCommandDestructiveBase();

	void* ExecuteE(void* p_upClipID, void* p_upFxID, void* p_upStartStop);
	void ExecuteE() {};
	bool RequiresParameters();
	bool IsUndoable();
	void UndoE();

protected:
	list<EMMediaClip*> GetClips(int64 p_vStart, int64 p_vStop, int64* p_vpParams);
	EMMediaClip* GetClip(int32 p_vID);

	virtual EMDestructivePlugin* GetPlugin(int32 p_vId) = 0;
protected:
	string m_oNewName;
	string m_oPeaksFile;
	int32 m_vReturnValue;
	int32 m_vMediaPoolValue;
};

#endif
