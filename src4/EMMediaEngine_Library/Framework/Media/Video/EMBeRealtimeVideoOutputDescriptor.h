/*******************************************************
* Creator: Richard Kronfalt
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_REALTIME_VIDEO_OUTPUT_DESCRIPTOR
#define __EM_BE_REALTIME_VIDEO_OUTPUT_DESCRIPTOR

#include "EMRealtimeOutputDescriptor.h"
#include "EMMediaDataBuffer.h"
#include "EMListener.h"

#include <list>

class EMListenerRepository;
class EMProjectDataSaver;
class EMProjectDataLoader;

class EMBeRealtimeVideoOutputDescriptor : public EMRealtimeOutputDescriptor, public EMListener
{
public:
	EMBeRealtimeVideoOutputDescriptor(media_input* p_spMediaInput);
	virtual ~EMBeRealtimeVideoOutputDescriptor();
	bool InitCheckE();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	bool PrepareToPlayE();
	EMMediaDataBuffer* ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList);
	void StartE();
	void StopE();
	bool ClearData();
	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool LoadData(EMProjectDataLoader* p_opLoader);
	
private:
	bool m_vAwaitingNewBufferFormat;
	bool m_ReadyForFormatChange;
	BBufferGroup* m_opBufferGroup;
	BBufferGroup* m_opBufferGroupRclm;
	BBuffer* m_opBuffer;
	BBufferProducer* m_opNode;
};

#endif

#endif
