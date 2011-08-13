/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_DEVICE_EXISTS
#define __MEDIA_INFO_COMMAND_DEVICE_EXISTS

#include "EMCommand.h"

class MediaInfoCommandDeviceExists : public EMCommand
{
public:
	MediaInfoCommandDeviceExists();
	~MediaInfoCommandDeviceExists();
	void ExecuteE() {};
	void* ExecuteE(void* p_upMediaType, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();

private:
	bool m_vReturnValue;
};

#endif
