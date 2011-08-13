#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#include "EMBeRealtimeAudioOutputDescriptor.h"
#include "EMBeAudioOutputNode.h"
#include "EMBeOutputNode.h"
#include "EMBeMediaUtility.h"
#include "EMBeMixFunctions.h"
#include "EMMediaTimer.h"
#include "EMBeMediaTimer.h"
#include "EMException.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"

#include <MediaKit.h>
#include <math.h>

EMBeRealtimeAudioOutputDescriptor::EMBeRealtimeAudioOutputDescriptor(media_input* p_spMediaInput) 
	:	EMRealtimeOutputDescriptor(EM_TYPE_ANY_AUDIO),
		m_opBufferGroup(NULL),
		m_opNode(NULL)
{
	m_opNode = EM_new EMBeAudioOutputNode(p_spMediaInput);
	EMMediaTimer::Instance() -> AddListener(this); //This is where we get the original calls to flush the node from!
	m_opNode -> AddListener(this); //We listen to this node in order to release a semaphore after FLUSH:s have been done. That's it!
	EMBeMediaUtility::push(this, "EMBeRealtimeAudioOutputDescriptor");
}

EMBeRealtimeAudioOutputDescriptor::~EMBeRealtimeAudioOutputDescriptor()
{
	m_opNode -> RemoveListener(this);
	m_opNode -> Shutdown();
	EMBeMediaUtility::GetRosterE() -> ReleaseNode(m_opNode -> Node());
	EMMediaTimer::Instance() -> RemoveListener(this);
	EMBeMediaUtility::pop("EMBeRealtimeAudioOutputDescriptor");
}

bool EMBeRealtimeAudioOutputDescriptor::InitCheckE()
{
	if(m_opNode != NULL && ! m_vIsInitialized)
		if(m_opNode -> Initialize())
			m_vIsInitialized = true;
		else
			EMDebugger("ERROR! Realtime audio output desc could not initialize node!");
	else
		EMDebugger("ERROR! Cannot initialize output desc when the node is NULL!");

	m_opBufferRepository = EMBeBufferRepository::Instance();
	m_opBufferGroup = m_opBufferRepository -> GetAudioBufferGroup();

	if(m_opBufferGroup == NULL)
		EMDebugger("ERROR! Audio output descriptor couldn't get BBufferGroup from BufferRepository!");

	return m_vIsInitialized;
}

bool EMBeRealtimeAudioOutputDescriptor::PrepareToPlayE()
{
	return true;
}

//Here, the EMMediaDataBuffer is converted into a BBuffer, as the final stage of the processing-chain.
//The EMMediaDataBuffers sent in are of course first mixed, and the result is then copied into the
//memory area of a BBuffer.
EMMediaDataBuffer* EMBeRealtimeAudioOutputDescriptor::ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList)
{
	if(p_opBufferList -> size() <= 0)
	{
		emerr << "WARNING! ProcessBufferE received empty media data buffer list!" << endl;
		return NULL;
	}

	//Perform mixing
	EMMediaDataBuffer* opDataBuffer = EMAudioMixFunctions::AudioMixPostFader(p_opBufferList);

	if(opDataBuffer == NULL)
		emerr << "ERROR! Result of mix operation is NULL!" << endl;

	//There, now copy the mixed result into a BBuffer's data area	
	if(m_opBufferGroup != NULL)
	{
		BBuffer* opBeBuffer = NULL;
//		int64 vStart = system_time();
//		while(true)
//		{
//			if((opBeBuffer = m_opBufferGroup -> RequestBuffer(opDataBuffer -> m_vSizeUsed, -1)) != NULL)
//				break;
//			if(system_time() - vStart > 500000)
//				break;
//		}
	
		opBeBuffer = m_opBufferGroup -> RequestBuffer(opDataBuffer -> m_vSizeUsed);
		if(opBeBuffer != NULL)
		{
			memcpy(opBeBuffer -> Data(), opDataBuffer -> Data(), opDataBuffer -> m_vSizeUsed);
			opBeBuffer -> SetSizeUsed(opDataBuffer -> m_vSizeUsed);

			if(m_opNode -> IsRunning() && opDataBuffer -> GetFrame() == EMMediaTimer::Instance() -> AudioThenFrame())
			{
				BBuffer** ptr = &opBeBuffer;
				((int64*) opBeBuffer -> Header() -> user_data)[0] = EMMediaTimer::Instance() -> AudioThenFrame();
				status_t err = write_port(m_opNode -> ControlPort(), EM_PORT_MESSAGE_INCOMING_BUFFER, ptr, sizeof(BBuffer)); //, B_TIMEOUT, B_INFINITE_TIMEOUT);
				if(err != B_OK)	
				{
					;//emout_commented_out_4_release << "ERROR! Buffer addition to node timed out!" << endl;
					opBeBuffer -> Recycle();
				}
			}
			else
			{
				;//emout_commented_out_4_release << "ERROR! Audio node not running, or wrong buffer time-stamp! Recycling buffer!" << endl;
				opBeBuffer -> Recycle();
			}
		}
		else
			;//emout_commented_out_4_release << "ERROR! Buffer request from buffer group timed out!" << endl;
		
		if(opDataBuffer -> m_vShouldBeDeleted)
			delete opDataBuffer;
		else 	
			opDataBuffer -> Recycle();
	}
	else
		EMDebugger("ERROR! EMBeOutputDescriptor doesn't have a BBufferGroup!");
	return NULL;
}

void EMBeRealtimeAudioOutputDescriptor::StartE()
{
	status_t err = EMBeMediaUtility::GetRosterE() -> StartNode(m_opNode -> Node(), 0);
	if(err)
		EMDebugger("ERROR! Could not start audio node!");
}

void EMBeRealtimeAudioOutputDescriptor::StopE()
{
	status_t err = EMBeMediaUtility::GetRosterE() -> StopNode(m_opNode -> Node(), 0, true); 
	if(err)
		emerr << "ERROR! Could not stop node!" << endl;
}

bool EMBeRealtimeAudioOutputDescriptor::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	if(p_vMessage == EM_MESSAGE_START)
	{
		write_port_etc(m_opNode -> ControlPort(), EM_PORT_MESSAGE_FLUSH_QUEUE, NULL, 0, B_TIMEOUT, 25000);
		return true;
	}
	else if(p_vMessage == EM_MESSAGE_STOP)
	{
		acquire_sem(m_vSem);
		write_port_etc(m_opNode -> ControlPort(), EM_PORT_MESSAGE_FLUSH_QUEUE, NULL, 0, B_TIMEOUT, 25000);
		return true;
	}
	else if(p_vMessage == EM_TIMED_EVENT_FLUSH_CASH)
	{
		release_sem(m_vSem);
	}
	else if(p_vMessage == EM_MESSAGE_PAUSE)
	{
//		status_t err = write_port_etc(ControlPort(), EM_NODE_PAUSE, NULL, 0, B_TIMEOUT, 25000);
		return true;
	}
	else if(p_vMessage == EM_MESSAGE_RESUME)
	{
//		status_t err = write_port_etc(ControlPort(), EM_NODE_RESUME, NULL, 0, B_TIMEOUT, 25000);
		return true;
	}
	else if(p_vMessage == EM_MESSAGE_TIME_SEEKED)
	{
		write_port_etc(m_opNode -> ControlPort(), EM_PORT_MESSAGE_FLUSH_QUEUE, NULL, 0, B_TIMEOUT, 25000);
		//TODO: timeout-handling!
		return true;
	}
	
	return false;
}

bool EMBeRealtimeAudioOutputDescriptor::ClearData()
{
	write_port_etc(m_opNode -> ControlPort(), EM_PORT_MESSAGE_RESET, NULL, 0, B_TIMEOUT, 25000);
	return true;
}

bool EMBeRealtimeAudioOutputDescriptor::SaveData(EMProjectDataSaver* p_opSaver)
{
	return true;
}

bool EMBeRealtimeAudioOutputDescriptor::LoadData(EMProjectDataLoader* p_opLoader)
{
	return true;
}

#endif
