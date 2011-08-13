/*******************************************************
* Creator: Kaneda
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_CURRENT_BITMAP
#define __MEDIA_INFO_COMMAND_GET_CURRENT_BITMAP

#include "EMCommand.h"

#include "EMBitmapBuffer.h"

class MediaInfoCommandGetCurrentBitmap : public EMCommand
{
public:
	MediaInfoCommandGetCurrentBitmap();
	void ExecuteE() {};
	void* ExecuteE(void*, void*, void*);

	bool RequiresParameters();
	bool IsUndoable();
	void UndoE();
	EMCommand* CloneAndClear();

private:
};

#endif