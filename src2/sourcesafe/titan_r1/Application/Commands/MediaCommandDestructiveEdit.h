/*******************************************************
* Creator: Martin Johansson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_DESTRUCTIVE_EDIT
#define __MEDIA_COMMAND_DESTRUCTIVE_EDIT

#include "EMCommand.h"

#include "MediaCommandDestructiveBase.h"

class EMDestructivePlugin;

class MediaCommandDestructiveEdit : public MediaCommandDestructiveBase
{
public:
	MediaCommandDestructiveEdit();
	~MediaCommandDestructiveEdit();

	EMCommand* CloneAndClear();

protected:
	EMDestructivePlugin* GetPlugin(int32 p_vId);

private:
	EMDestructivePlugin* m_opPlug;
	bool m_vWrapped;
};

#endif
