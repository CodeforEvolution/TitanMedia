/*******************************************************
* This code is the copyrighted property of Elegant Media
*-------------------------------------------------------
* Creator: Johan Dunberg
* Component: Command
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/
#include "EMGlobals.h"

#ifndef __COMMAND_DISPLAY_CLIP_PROPERTIES_DIALOG
#define __COMMAND_DISPLAY_CLIP_PROPERTIES_DIALOG

#include "EMCommand.h"
#include "ClipPropertiesDialog.h"

class EMGClip;
class EMGTrackInfo;
class TrackView;

class CommandDisplayClipPropertiesDialog : public EMCommand
{
public:
	CommandDisplayClipPropertiesDialog(TrackView* p_opTrackView);
	void CalculatePosition(int32 &p_vX, int32 &p_vY, EMGClip* p_opClip, EMGTrackInfo* p_opTrackInfo, ClipPropertiesDialog* m_opDialog);
	void ExecuteE();
	void* ExecuteE(void*, void*, void* ){return NULL;};
	bool IsUndoable();
	bool RequiresParameters();

private:
	ClipPropertiesDialog m_oAudioDialog;
	ClipPropertiesDialog m_oMidiDialog;
	TrackView* m_opTrackView;
	ClipPropertiesDialog m_oVideoDialog;
	ClipPropertiesDialog m_oTransitionDialog;
	int32 m_vX;
	int32 m_vY;
	int32 m_vOffset;
};

#endif
