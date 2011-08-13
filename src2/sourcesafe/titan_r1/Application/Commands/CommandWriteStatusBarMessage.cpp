#include "CommandWriteStatusBarMessage.h"

#include "EMView.h"
#include "TrackWindowStatusBar.h"
#include "EMGUIGlobals.h"
#include "EMThread.h"
#include "EMThreadRepository.h"

CommandWriteStatusBarMessage::CommandWriteStatusBarMessage()
{
}

void* CommandWriteStatusBarMessage::ExecuteE(void* p_upParameterOne, void*, void*)
{
	if(TrackWindowStatusBar::Instance() -> GetView() -> LockWindow())
	{
		strcpy(m_vpMessageBuffer, static_cast<const char*>(p_upParameterOne));
		EMThreadRepository::Instance() -> GetThread(GUI_SERVICE_THREAD_NAME) -> AddListener(this);

		TrackWindowStatusBar::Instance() -> GetView() -> UnlockWindow();
	}

	return NULL;
/*
	bool vIsLocked(false); // FIXME: this temporary lock must be designed around
	if(TrackWindowStatusBar::Instance() -> GetView() -> WindowIsLocked())
		vIsLocked = true;
	else
		TrackWindowStatusBar::Instance() -> GetView() -> LockWindow();

//	if(TrackWindowStatusBar::Instance() -> GetView() -> LockWindow())
	{
		string oMessage(static_cast<const char*>(p_upParameterOne));
		TrackWindowStatusBar::Instance() -> WriteMessage(oMessage);
//		TrackWindowStatusBar::Instance() -> GetView() -> UnlockWindow();
	}

	if(!vIsLocked)
		TrackWindowStatusBar::Instance() -> GetView() -> UnlockWindow();

	return NULL;
*/
}

bool CommandWriteStatusBarMessage::IsUndoable()
{
	return false;
}

bool CommandWriteStatusBarMessage::RequiresParameters()
{
	return true;
}

void CommandWriteStatusBarMessage::ThreadRun(EMThread* p_opThread)
{
	if(TrackWindowStatusBar::Instance() -> GetView() -> LockWindow())
	{
		string oMessage(m_vpMessageBuffer);
		TrackWindowStatusBar::Instance() -> WriteMessage(oMessage);
		EMThreadRepository::Instance() -> GetThread(GUI_SERVICE_THREAD_NAME) -> RemoveListener(this);

		TrackWindowStatusBar::Instance() -> GetView() -> UnlockWindow();
	}
}
