/*******************************************************
* Creator: Martin Johansson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_DESTRUCTIVE_CROP
#define __MEDIA_COMMAND_DESTRUCTIVE_CROP

#include "EMCommand.h"
#include "MediaCommandDestructiveBase.h"

#include "EMDestructiveCrop.h"

class MediaCommandDestructiveCrop : public MediaCommandDestructiveBase
{
public:
	MediaCommandDestructiveCrop();
	~MediaCommandDestructiveCrop();

	EMCommand* CloneAndClear();


protected:
	EMDestructivePlugin* GetPlugin(int32 p_vId);

private:
	EMDestructiveCrop m_oPlugin;
};

#endif
