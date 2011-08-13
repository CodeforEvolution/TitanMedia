#include "MediaCommandDeleteVideoClipContent.h"
#include "EMGlobals.h"

#include "EMBitmap.h"
#include "EMWinBitmap.h"


#include <iostream>
#include <list>

MediaCommandDeleteVideoClipContent::MediaCommandDeleteVideoClipContent()
{
}

void* MediaCommandDeleteVideoClipContent::ExecuteE(void* p_opBitmap, void*, void*)
{
	
	EMWinBitmap* opBitmap = static_cast<EMWinBitmap*>(p_opBitmap);
	
	HBITMAP opNativeBitmap = static_cast<HBITMAP>( opBitmap->GetNativeBitmap());
	
	DeleteObject(opNativeBitmap);

	return NULL;

}

bool MediaCommandDeleteVideoClipContent::RequiresParameters()
{
	return true;
}

bool MediaCommandDeleteVideoClipContent::IsUndoable()
{
	return false;
}