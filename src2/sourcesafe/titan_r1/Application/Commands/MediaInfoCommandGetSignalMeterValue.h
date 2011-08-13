/*******************************************************
* Creator: Richard Kronfält
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"
#include "EMMediaGlobals.h"

#ifndef __MEDIA_INFO_COMMAND_GET_SIGNAL_METER_VALUE
#define __MEDIA_INFO_COMMAND_GET_SIGNAL_METER_VALUE

#include "EMCommand.h"

class MediaInfoCommandGetSignalMeterValue : public EMCommand
{
public:
	MediaInfoCommandGetSignalMeterValue();
	~MediaInfoCommandGetSignalMeterValue();
	void ExecuteE() {};
	void* ExecuteE(void* p_upList, void*, void*);
	bool RequiresParameters();
	bool IsUndoable();
	
private:
	list<float>* m_opZeroValues;
};

#endif
