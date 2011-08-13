/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_NATIVE_DIRECTX_EFFECT
#define __MEDIA_INFO_COMMAND_GET_NATIVE_DIRECTX_EFFECT

#include "EMCommand.h"

struct IBaseFilter;

class MediaInfoCommandGetNativeDirectXEffect : public EMCommand
{
public:
	MediaInfoCommandGetNativeDirectXEffect();
	~MediaInfoCommandGetNativeDirectXEffect();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaEffectID, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	void DeallocateArrays();
	IBaseFilter* m_opReturnValue;
};

#endif
