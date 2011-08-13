/*******************************************************
* Creator: Martin Johansson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_DESTRUCTIVE_REVERSE
#define __MEDIA_COMMAND_DESTRUCTIVE_REVERSE

#include "MediaCommandDestructiveBase.h"

#include "EMDestructiveReverse.h"

class MediaCommandDestructiveReverse : public MediaCommandDestructiveBase
{
public:
	MediaCommandDestructiveReverse();
	~MediaCommandDestructiveReverse();

	EMCommand* CloneAndClear();


protected:
	EMDestructivePlugin* GetPlugin(int32 p_vId);

private:
	EMDestructiveReverse m_oPlugin;
};


#endif
