/*******************************************************
* Creator: Martin Johansson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_DESTRUCTIVE_NORMALIZE
#define __MEDIA_COMMAND_DESTRUCTIVE_NORMALIZE

#include "MediaCommandDestructiveBase.h"

#include "EMDestructiveNormalize.h"

class MediaCommandDestructiveNormalize : public MediaCommandDestructiveBase
{
public:
	MediaCommandDestructiveNormalize();
	~MediaCommandDestructiveNormalize();

	EMCommand* CloneAndClear();


protected:
	EMDestructivePlugin* GetPlugin(int32 p_vId);

private:
	EMDestructiveNormalize m_oPlugin;
};


#endif
