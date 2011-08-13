/*******************************************************
* Creator: Richard Kronfalt
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_REALTIME_AUDIO_OUTPUT_DESCRIPTOR
#define __EM_BE_REALTIME_AUDIO_OUTPUT_DESCRIPTOR

#include "EMRealtimeOutputDescriptor.h"
#include "EMMediaDataBuffer.h"
#include "EMListenerRepository.h"
#include "EMListener.h"

#include <list>

class EMProjectDataSaver;
class EMProjectDataLoader;

class EMBeNodeConnection
{
public:
	EMBeNodeConnection() {};
	~EMBeNodeConnection() {};
	media_node m_sProducer;
	media_node m_sConsumer;
	media_output m_sOutput;
	media_input m_sInput;
	media_format m_sFormat;
	media_source m_sSource;
	media_destination m_sDestination;
};

class EMBeRealtimeAudioOutputDescriptor : public EMRealtimeOutputDescriptor, public EMListener
{
public:
	EMBeRealtimeAudioOutputDescriptor(media_input* p_spMediaInput);
	virtual ~EMBeRealtimeAudioOutputDescriptor();
	bool InitCheckE();
	bool PrepareToPlayE();
	void StartE();
	void StopE();
	EMMediaDataBuffer* ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList);
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	BBufferGroup* m_opBufferGroup;
	bool ClearData();
	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool LoadData(EMProjectDataLoader* p_opLoader);
	
private:
	EMBeNodeConnection m_oConnection;
	sem_id m_vSem;
	BBufferProducer* m_opNode;
};

#endif

#endif
