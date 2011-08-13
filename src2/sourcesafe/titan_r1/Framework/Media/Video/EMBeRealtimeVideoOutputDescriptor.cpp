#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#include "EMBeRealtimeVideoOutputDescriptor.h"
#include "EMBeMediaUtility.h"
#include "EMBeVideoOutputNode.h"
#include "EMBeMixFunctions.h"
#include "EMBeMediaTimer.h"
#include "EMBeBufferRepository.h"
#include "EMMediaFormat.h"
#include "EMListener.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"

#include <MediaKit.h>

EMBeRealtimeVideoOutputDescriptor::EMBeRealtimeVideoOutputDescriptor(media_input* p_spMediaInput)
	:	EMRealtimeOutputDescriptor(EM_TYPE_ANY_VIDEO), 
		m_vAwaitingNewBufferFormat(false) , 
		m_ReadyForFormatChange(false), 
		m_opBufferGroup(NULL),
		m_opBuffer(NULL),
		m_opNode(NULL)
{
	m_opNode = new EMBeVideoOutputNode(p_spMediaInput);
	EMMediaTimer::Instance() -> AddListener(this);
	EMBeMediaUtility::push(this, "EMBeRealtimeVideoOutputDescriptor");
}

EMBeRealtimeVideoOutputDescriptor::~EMBeRealtimeVideoOutputDescriptor()
{
	m_opNode -> Shutdown();
	EMMediaTimer::Instance() -> RemoveListener(this);
	EMBeMediaUtility::GetRosterE() -> ReleaseNode(m_opNode -> Node());
	EMBeMediaUtility::pop("EMBeRealtimeVideoOutputDescriptor");
/*	if(m_opBuffer != NULL)
		m_opBuffer -> Recycle();
*/
}

bool EMBeRealtimeVideoOutputDescriptor::PrepareToPlayE()
{
	m_opBufferRepository = EMBeBufferRepository::Instance();
	
	//TODO: Init video stuff instead
//	if(! m_opBufferRepository -> InitializeVideoE(304*168*8/2, 10))
//	{
//		EMDebugger("ERROR! Could not initialize buffer repository for video!");
//		return false;
//	}
	
	return true;
}


EMMediaDataBuffer* EMBeRealtimeVideoOutputDescriptor::ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList)
{
	EMMediaDataBuffer* opTarget = p_opBufferList -> front();

//	EMMediaFormat oEMMediaFormat = opTarget -> m_oFormat;
	if(m_opBufferGroup == NULL)
		m_opBufferGroup = m_opBufferRepository -> GetVideoBufferGroup();

	media_format oMediaFormat;
	oMediaFormat.type = B_MEDIA_RAW_VIDEO;		
	oMediaFormat.u.raw_video.display.line_width = opTarget -> m_oFormat.m_vWidth;
	oMediaFormat.u.raw_video.display.line_count = opTarget -> m_oFormat.m_vHeight;
 	oMediaFormat.u.raw_video.display.format = static_cast<color_space>(opTarget -> m_oFormat.m_vDepth);
 	oMediaFormat.u.raw_video.display.bytes_per_row = opTarget -> m_oFormat.m_vBytesPerRow;

	if((opTarget -> m_oFormat.m_vWidth != EMBeBufferRepository::Instance() -> GetCurrentVideoFormat() -> m_vWidth || opTarget -> m_oFormat.m_vHeight != EMBeBufferRepository::Instance() -> GetCurrentVideoFormat() -> m_vHeight) && !m_vAwaitingNewBufferFormat && !m_ReadyForFormatChange)
	{
		;//cout_commented_out_4_release << "Have format: " << EMBeBufferRepository::Instance() -> GetCurrentVideoFormat() -> m_vWidth << " X " << EMBeBufferRepository::Instance() -> GetCurrentVideoFormat() -> m_vHeight << endl;
		;//cout_commented_out_4_release << "And new format: " << opTarget -> m_oFormat.m_vWidth << " X " << opTarget -> m_oFormat.m_vHeight << endl;
		EMBeBufferRepository::Instance() -> InitializeVideoE(0, 2, &oMediaFormat, true);
		m_vAwaitingNewBufferFormat = true;
	}

	if(m_opBufferRepository != NULL)
	{
		if(! m_vAwaitingNewBufferFormat)
			m_opBufferGroup = m_opBufferRepository -> GetVideoBufferGroup();
		if(m_opBufferGroup == NULL)
			EMDebugger("ERROR! No video buffer group!");
	}
	else
		EMDebugger("ERROR! BufferRepository is NULL, in EMBeRealtimeVideoOutputDescriptor!");

	if(p_opBufferList -> size() <= 0)
		return NULL;

	//Perform mixing

	opTarget = EMBeMixFunctions::VideoPrioritizer(p_opBufferList);

	if(opTarget == NULL)
		emerr << "ERROR! Result of mix operation is NULL!" << endl;


	//There, now copy the mixed result into a BBuffer's data area	
	if(m_opBufferGroup != NULL)
	{

		if(m_vAwaitingNewBufferFormat)
		{
			m_opBufferGroupRclm = m_opBufferGroup;
			m_opBufferGroup = m_opBufferRepository -> GetVideoBufferGroup();

			m_vAwaitingNewBufferFormat = false;
			m_ReadyForFormatChange = true;
			m_opBufferRepository -> NotifyVideoFormatChange();
		}

		EMMediaEngine::Instance() -> GetMediaProject() -> UnlockVideo();
		while(true)
		{
			m_opBuffer = m_opBufferGroup -> RequestBuffer(1, -1);
			if(m_opBuffer != NULL)
				break;
			snooze(2000);
		}
		EMMediaEngine::Instance() -> GetMediaProject() -> LockVideo();

		if(m_ReadyForFormatChange)
		{
			//if(m_opBufferGroupRclm -> ReclaimAllBuffers() == B_OK) // <- KOLLA DENNA RADEN !!
			//{
				int32 vCount = 0;
				list<BBuffer*> oDummyBuffers;
				m_opBufferGroupRclm -> CountBuffers(&vCount);
				EMMediaEngine::Instance() -> GetMediaProject() -> UnlockVideo();
				for(int32 vIndex = 0; vIndex < vCount; vIndex++)
				{
					m_opBufferRepository -> NotifyHistoryBufferDelete();
					BBuffer* opDummy = m_opBufferGroupRclm -> RequestBuffer(1, -1);
					if(opDummy == NULL)
					{
						vIndex--;
//						EMMediaEngine::Instance() -> GetMediaProject() -> UnlockVideo();
						snooze(5000);
//						if(! EMMediaEngine::Instance() -> GetMediaProject() -> LockVideo())
//						{
//							break;
//						}
					}
					else
						oDummyBuffers.push_back(opDummy);
				}
				EMMediaEngine::Instance() -> GetMediaProject() -> LockVideo();
				
				while(oDummyBuffers.size() > 0)
				{
					BBuffer* opBuffer = oDummyBuffers.front();
					oDummyBuffers.pop_front();
					opBuffer -> Recycle();
				}
				
				m_opBufferRepository -> ResetVideoE(&oMediaFormat);
				m_ReadyForFormatChange = false;
			//}
				
		}

		if(m_opBuffer != NULL)
		{
			;//cout_commented_out_4_release << "Before crash..." << endl;
			memcpy(m_opBuffer -> Data(), opTarget -> Data(), opTarget -> m_vSizeUsed);
			m_opBuffer -> SetSizeUsed(opTarget -> m_vSizeUsed);

			if(m_opNode -> IsRunning() /*&& opTarget -> m_vFrame == EMMediaTimer::Instance() -> VideoThenFrame() */)
			{
				((int64*) m_opBuffer -> Header() -> user_data)[0] = EMMediaTimer::Instance() -> VideoThenFrame();
				BBuffer** ptr = &m_opBuffer;

				//Store the format in the user-data area of the BBuffer (the only place where we can store it)				
				char* vpUserData = (char*) m_opBuffer -> Header() -> user_data;
				memcpy(vpUserData, &(opTarget -> m_oFormat), sizeof(EMMediaFormat));
				
				status_t err = write_port_etc(m_opNode -> ControlPort(), EM_PORT_MESSAGE_INCOMING_BUFFER, ptr, sizeof(BBuffer), B_TIMEOUT, 25000);
				if(err != B_OK)
				{
					m_opBuffer -> Recycle();
					EMDebugger("ERROR! Buffer addition to node timed out!");
				}
			}	
			else
				m_opBuffer -> Recycle();
			if(opTarget -> m_vShouldBeDeleted)
				delete opTarget;
			else
				opTarget -> Recycle();
		}
		//else
		//	EMDebugger("ERROR! Buffer was NULL in video output descriptor!");
	}
	else
		EMDebugger("ERROR! No BBufferGroup set in EMBeRealtimeVideoOutputDescriptor!");
	return NULL;
}

void EMBeRealtimeVideoOutputDescriptor::StartE()
{
	EMBeMediaUtility::GetRosterE() -> StartNode(m_opNode -> Node(), 0); 
}

void EMBeRealtimeVideoOutputDescriptor::StopE()
{
	EMBeMediaUtility::GetRosterE() -> StopNode(m_opNode -> Node(), 0, true); 
}

bool EMBeRealtimeVideoOutputDescriptor::InitCheckE()
{
	if(m_opNode != NULL)
		if(m_opNode -> Initialize())
			m_vIsInitialized = true;
	if(! m_vIsInitialized)
		emerr << "ERROR! Tried to initialize an output descriptor with a NULL node!" << endl;

	return m_vIsInitialized;
}

bool EMBeRealtimeVideoOutputDescriptor::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	if(p_vMessage == EM_MESSAGE_STOP)
	{
		return true;
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

bool EMBeRealtimeVideoOutputDescriptor::ClearData()
{
	write_port_etc(m_opNode -> ControlPort(), EM_PORT_MESSAGE_RESET, NULL, 0, B_TIMEOUT, 25000);
	return true;
}

bool EMBeRealtimeVideoOutputDescriptor::SaveData(EMProjectDataSaver* p_opSaver)
{
	return true;
}

bool EMBeRealtimeVideoOutputDescriptor::LoadData(EMProjectDataLoader* p_opLoader)
{
	return true;
}

#endif
