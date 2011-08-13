/*******************************************************
* Creator: MartOut Johansson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_DESTRUCTIVE_FADE_OUT
#define __MEDIA_COMMAND_DESTRUCTIVE_FADE_OUT

#include "MediaCommandDestructiveBase.h"

#include "EMDestructiveFadeOut.h"

class MediaCommandDestructiveFadeOut : public MediaCommandDestructiveBase
{
public:
	MediaCommandDestructiveFadeOut();
	~MediaCommandDestructiveFadeOut();

	EMCommand* CloneAndClear();

protected:
	EMDestructivePlugin* GetPlugin(int32 p_vId);

private:
	EMDestructiveFadeOut m_oPlugin;
};

#endif