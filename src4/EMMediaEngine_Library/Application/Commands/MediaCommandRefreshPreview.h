/*******************************************************
* Creator: Kaneda
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_COMMAND_REFRESH_PREVIEW
#define __MEDIA_COMMAND_REFRESH_PREVIEW

#include "EMCommand.h"

class MediaCommandRefreshPreview : public EMCommand
{
public:
	MediaCommandRefreshPreview();

	void ExecuteE();

//	void* ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree) {return NULL;};
	bool RequiresParameters();
//	void UndoE();
	bool IsUndoable();
	EMCommand* CloneAndClear();

private:
};

#endif
