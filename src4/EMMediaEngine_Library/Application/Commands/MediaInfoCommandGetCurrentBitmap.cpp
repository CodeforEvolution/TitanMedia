#include "MediaInfoCommandGetCurrentBitmap.h"

#include "EMBitmapBuffer.h"

MediaInfoCommandGetCurrentBitmap::MediaInfoCommandGetCurrentBitmap()
{
}

void* MediaInfoCommandGetCurrentBitmap::ExecuteE(void* p_vpNew, void*, void*)
{
	bool* vpNew = static_cast<bool*>(p_vpNew);
	
	*vpNew = EMBitmapBuffer::Instance() -> IsBitmapNew();

	return EMBitmapBuffer::Instance() -> GetBitmap();
}

bool MediaInfoCommandGetCurrentBitmap::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetCurrentBitmap::IsUndoable()
{
	return false;
}

void MediaInfoCommandGetCurrentBitmap::UndoE()
{

}

EMCommand* MediaInfoCommandGetCurrentBitmap::CloneAndClear()
{
	return EM_new MediaInfoCommandGetCurrentBitmap();
}
