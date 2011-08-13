/*******************************************************
* Creator: Jesper Svensson
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_ENCODER_REPOSITORY
#define __EM_ENCODER_REPOSITORY

#include "EMRealtimeEncoderDescriptor.h"
#include "EMMediaItemContainer.h"
#include "EMListener.h"

#include <list>

class EMListenerRepository;

class EMEncoderRepository : public EMMediaItemContainer<EMRealtimeEncoderDescriptor>, public EMListener
{
public:
	static EMEncoderRepository* Instance();
	static void Delete();
	bool InitCheckE();
	void OnItemAdd(); //Inherited from EMMediaItemContainer. Added by Richard 010821
	void OnItemRemove(); //Inherited from EMMediaItemContainer. Added by Richard 010821
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	
private:
	EMEncoderRepository();
	~EMEncoderRepository();
	int32 m_vID;
	static EMEncoderRepository* m_opInstance;
	bool m_vIsInitialized;
};

#endif
