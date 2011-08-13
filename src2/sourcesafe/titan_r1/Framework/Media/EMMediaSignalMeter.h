/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
* 
	time = 
	0	1	2	3	4	n
	Slot content = 
	[ ]	[ ]	[ ]	[ ]	[ ] . . . [ ]
	 4	 5	 9	 2	
		 R	 		 W
		 e			 r
		 a			 i
		 d			 t
		 			 e
		 			 
	Readpos  = 1 % maxlength
	Writepos = 4 % maxlength

*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_MEDIA_SIGNAL_METER
#define __EM_MEDIA_SIGNAL_METER

#include "EMMediaGlobals.h"

#include "EMMediaSignalMeterRepository.h"
#include "EMBufferDestination.h"
#include "EMListener.h"
#include "EMListenerRepository.h"

class EMMediaSignalMeter : public EMBufferDestination, public EMListener
{
public:
	static EMMediaSignalMeter* CreateEMSignalMeter(EMMediaType p_eType, bool p_vIsInputMeter = false);

	virtual ~EMMediaSignalMeter();
	int32 GetID() const;
	EMMediaType GetType() const;

	virtual bool PrepareToPlayE();
	virtual EMMediaDataBuffer* ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList);
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);

	virtual void OnStart();
	virtual void OnStop();
	virtual void OnSeek();

protected:
	EMMediaSignalMeter(EMMediaType p_eType);

	bool m_vSignalMetersActive;

private:
	EMMediaType m_eType;
	int32 m_vID;

};

#endif
