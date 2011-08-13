/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_MEDIA_SIGNAL_METER_REPOSITORY
#define __EM_MEDIA_SIGNAL_METER_REPOSITORY

#include "EMMediaItemContainer.h"
#include "EMMediaSignalMeter.h"

class EMMediaSignalMeter;

class EMMediaSignalMeterRepository : public EMMediaItemContainer<EMMediaSignalMeter>
{
public:
	static EMMediaSignalMeterRepository* Instance();
	static void EMMediaSignalMeterRepository::Delete();
	void DeleteSignalMeter(EMMediaSignalMeter* p_opMeter);
	EMMediaSignalMeter* InstanceSignalMeter(EMMediaType p_eType);
	int32 GetID() const;
	void OnItemAdd(); //Inherited from EMMediaItemContainer. Added by Richard 010821
	void OnItemRemove(); //Inherited from EMMediaItemContainer. Added by Richard 010821

private:
	EMMediaSignalMeterRepository();
	~EMMediaSignalMeterRepository();
	static EMMediaSignalMeterRepository* m_opInstance;
	int32 m_vID;
};

#endif
