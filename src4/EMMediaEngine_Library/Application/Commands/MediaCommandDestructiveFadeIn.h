/*******************************************************
* Creator: Martin Johansson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_DESTRUCTIVE_FADE_IN
#define __MEDIA_COMMAND_DESTRUCTIVE_FADE_IN

#include "MediaCommandDestructiveBase.h"

#include "EMDestructiveFadeIn.h"

class MediaCommandDestructiveFadeIn : public MediaCommandDestructiveBase
{
public:
	MediaCommandDestructiveFadeIn();
	~MediaCommandDestructiveFadeIn();

	EMCommand* CloneAndClear();


protected:
	EMDestructivePlugin* GetPlugin(int32 p_vId);

private:
	EMDestructiveFadeIn m_oPlugin;
};

#endif
