/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __FILE_DIALOG_TARGET
#define __FILE_DIALOG_TARGET

#include "EMListener.h"

class EMListenerRepository;
class TrackWindow;

class FileDialogTarget : public EMListener
{
public:
	FileDialogTarget(TrackWindow* p_opTrackWindow);
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);

private:
	TrackWindow* m_opTrackWindow;
};

#endif


