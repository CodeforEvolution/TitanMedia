/*******************************************************
* Creator: Martin Johansson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_DESTRUCTIVE_SILENCE
#define __MEDIA_COMMAND_DESTRUCTIVE_SILENCE

#include "MediaCommandDestructiveBase.h"

#include "EMDestructiveSilence.h"

class MediaCommandDestructiveSilence : public MediaCommandDestructiveBase
{
public:
	MediaCommandDestructiveSilence();
	~MediaCommandDestructiveSilence();

	EMCommand* CloneAndClear();

protected:
	EMDestructivePlugin* GetPlugin(int32 p_vId);

private:
	EMDestructiveSilence m_oPlugin;
};

#endif
