/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_FAST_FORWARD
#define __COMMAND_FAST_FORWARD

#include "EMCommand.h"
#include "EMSemaphore.h"
#include "EMThreadListener.h"

class TrackWindowToolBar;

class CommandFastForward : public EMCommand, public EMThreadListener
{
public:
	CommandFastForward(TrackWindowToolBar* p_opToolBar);
	~CommandFastForward();
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ) { return NULL; };
	bool IsUndoable();
	bool RequiresParameters();
	void ThreadRun(EMThread* p_opThread);
	bool TriggerOnKeyRelease();

private:
	EMSemaphore* m_opSemaphore;
	TrackWindowToolBar* m_opToolBar;
	//bool m_vIsPlaying;
	bool m_vKeyPressed;
};

#endif
