#include "EMMediaBufferSource.h"

#include "EMBufferDestination.h"
#include "EMMediaDataBuffer.h"
#include "EMMediaIDManager.h"
//#include "EMBeMediaUtility.h"
#include "EMSemaphore.h"
#include "EMMediaUtility.h"
#include "EMMediaTimer.h"

#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"
#include "EMMediaTrack.h"

EMMediaBufferSource::EMMediaBufferSource(EMMediaType p_eType)
	:	m_opBuffers(NULL),
		m_eType(p_eType),
		m_opDestination(NULL),
		m_vNumBuffers(2),
		m_vHasBeenInitialized(false),
		m_vShouldSeek(false)

{
	m_vID = EMMediaIDManager::MakeID();

	char ch[256]; 
	sprintf(ch, "BuffSourceSem_%d\0", m_vID);
	string oName = string(ch);
	m_opDestinationSemaphore = EMSemaphore::CreateEMSemaphore(oName.c_str());	
	
	//MessageBox(NULL, "About to create semaphore", "", MB_OK);
	m_opBufferProtectionSem = EMSemaphore::CreateEMSemaphore();
	
	//MessageBox(NULL, "About to acquire semaphore...", "", MB_OK);
//	m_opBufferProtectionSem -> Acquire();
	//MessageBox(NULL, "About to release!", "", MB_OK);
//	m_opBufferProtectionSem -> Release();
}

EMMediaBufferSource::EMMediaBufferSource()
	:	m_opBuffers(NULL),
		m_vID(-1),
		m_eType(EM_TYPE_UNKNOWN),
		m_vNumBuffers(2),
		m_vHasBeenInitialized(false),
		m_vShouldSeek(false),
		m_opDestination(NULL)
{
	m_vID = EMMediaIDManager::MakeID();

	char ch[256]; 
	sprintf(ch, "BuffSourceSem_%d\0", m_vID);
	string oName = string(ch);
	m_opDestinationSemaphore = EMSemaphore::CreateEMSemaphore(oName.c_str());	
	
	//MessageBox(NULL, "About to create semaphore", "", MB_OK);
	m_opBufferProtectionSem = EMSemaphore::CreateEMSemaphore();
	
	//MessageBox(NULL, "About to acquire semaphore...", "", MB_OK);
//	m_opBufferProtectionSem -> Acquire();
	//MessageBox(NULL, "About to release!", "", MB_OK);
//	m_opBufferProtectionSem -> Release();
}


EMMediaBufferSource::~EMMediaBufferSource()
{
	m_opBufferProtectionSem -> Acquire();

	try
	{
		if(m_opBuffers != NULL)
			for(int32 vIndex = 0; vIndex < m_vNumBuffers; vIndex++)
			{
				//Only delete available buffers. Otherwise, set it's delete flag to true!
				if(m_opBuffers[vIndex] -> m_vAvailable)
					delete m_opBuffers[vIndex];
				else
					m_opBuffers[vIndex] -> m_vShouldBeDeleted = true;
			}

		if(m_opBuffers != NULL)
		{
			delete m_opBuffers;
			m_opBuffers = NULL;
		}
	}
	catch(...)
	{
		MessageBox(NULL, "ERROR! Exception caught in ~EMMediaBufferSource()", "MediaEngine error!", MB_OK);
	}

	m_opBufferProtectionSem -> Release();

	delete m_opBufferProtectionSem;				//Kaneda - Buggfix
	delete m_opDestinationSemaphore;			//Kaneda - Buggfix
}

void EMMediaBufferSource::AcquireBufferProtectionSemaphore()
{
	m_opBufferProtectionSem -> Acquire();

}

void EMMediaBufferSource::ReleaseBufferProtectionSemaphore()
{
	m_opBufferProtectionSem -> Release();

}


void EMMediaBufferSource::RecycleAllBuffers() const
{
	if(! m_vHasBeenInitialized)
		MessageBox(NULL, "ERROR! Tried to recycle buffer into buffer source before it was initialized!", "MediaEngine error!", MB_OK);
		
	if(m_opBuffers == NULL)
		MessageBox(NULL, "ERROR! EMMediaBufferSource has no buffers", "MediaEngine error!", MB_OK);

	m_opBufferProtectionSem -> Acquire();
	try
	{
		for(int32 vIndex = 0; vIndex < m_vNumBuffers; vIndex++)
		{
			//Don't need to zero the buffers (false = don't zero, true = zero the buffers)
			m_opBuffers[vIndex] -> Recycle(false);
		}
	}
	catch(...)
	{
		MessageBox(NULL, "ERROR! Caught an exception in GetBuffer()", "MediaEngine error!", MB_OK);
	}
	m_opBufferProtectionSem -> Release();

}


EMMediaDataBuffer* EMMediaBufferSource::GetBuffer() const
{
	if(! m_vHasBeenInitialized)
		MessageBox(NULL, "ERROR! Tried to get a buffer from buffer source before it was initialized!", "MediaEngine error!", MB_OK);
		
	if(m_opBuffers == NULL)
		MessageBox(NULL, "ERROR! EMMediaBufferSource has no buffers", "MediaEngine error!", MB_OK);
	
	m_opBufferProtectionSem -> Acquire();
	try
	{
		for(int32 vIndex = 0; vIndex < m_vNumBuffers; vIndex++)
		{
			if(m_opBuffers[vIndex] -> m_vAvailable)
			{
				m_opBuffers[vIndex] -> m_vAvailable = false;
				//MessageBox(NULL, "About to GetDestination!", "", MB_OK);
				m_opBuffers[vIndex] -> m_opDestination = GetDestination();
				//MessageBox(NULL, "OK! Got destination!", "", MB_OK);
				m_opBufferProtectionSem -> Release();
				return m_opBuffers[vIndex];
			}
		}
	}
	catch(...)
	{
		MessageBox(NULL, "ERROR! Caught an exception in GetBuffer()", "MediaEngine error!", MB_OK);
	}
	m_opBufferProtectionSem -> Release();
	return NULL;
}

EMMediaDataBuffer* EMMediaBufferSource::GetBufferAt(int64 p_vTime, bool p_vSeeking)
{
	return GetBuffer();
}

EMBufferDestination* EMMediaBufferSource::GetDestination() const
{
	m_opDestinationSemaphore -> Acquire();
	EMBufferDestination* opDest = m_opDestination;
	m_opDestinationSemaphore -> Release();
	return opDest;
}

int32 EMMediaBufferSource::GetID() const
{
	return m_vID;
}

EMMediaType EMMediaBufferSource::GetType() const
{
	return m_eType;
}

void EMMediaBufferSource::SetType(EMMediaType p_eType)
{
	m_eType = p_eType;
}

bool EMMediaBufferSource::InitCheckE(EMMediaFormat* p_opFormat)
{
	bool vRet;
	vRet = InitCheckBufferE(p_opFormat);
	vRet &= InitCheckDestinationsE(p_opFormat);

	return vRet;
}


bool EMMediaBufferSource::InitCheckDestinationsE(EMMediaFormat* p_opFormat)
{
	m_opBufferProtectionSem -> Acquire();

	try
	{
		if(m_opDestination != NULL)
		{
			if(! m_opDestination -> PrepareToPlayE())
			{
				for(int32 vIndex = 0; vIndex < m_vNumBuffers; vIndex++)
					delete m_opBuffers[vIndex];
				delete m_opBuffers;
				m_opBuffers = NULL;
				EMDebugger("ERROR While initializing EMMediaBufferSource!");
				m_opBufferProtectionSem -> Release();
				return false;
			}
			
			//Make sure our buffer has the right next-destination, and the right
			//start-destination. Since our destination IS the start-destination
			//destination and startdestination will be the same, in the buffer.
			for(int32 vIndex = 0; vIndex < m_vNumBuffers; vIndex++)
			{
				if(m_opDestination == NULL)
					EMDebugger("ERROR! My destination is NULL!");
					
				m_opBuffers[vIndex] -> m_opDestination = m_opDestination; //DET BLIR FEL HÄR !!??
				m_opBuffers[vIndex] -> m_opStartDestination = m_opDestination;
			}
		}
		else
			MessageBox(NULL, "ERROR! It's an error to call EMMediaBufferSource::InitCheckE before SetDestination() has been called", "MediaEngine error!", MB_OK);
		m_vHasBeenInitialized = true;
	}
	catch(...)
	{
		MessageBox(NULL, "ERROR! Caught an exception in EMMediaBufferSource::InitCheckDestinationsE()", "MediaEngine error!", MB_OK);
	}
	m_opBufferProtectionSem -> Release();
	
	return true;
}

bool EMMediaBufferSource::InitCheckBufferE(EMMediaFormat* p_opFormat)
{
	m_opBufferProtectionSem -> Acquire();
	try
	{
		if(m_opBuffers != NULL && ((p_opFormat -> m_eType & EM_TYPE_ANY_VIDEO) == 0))
		{
			for(int32 vIndex = 0; vIndex < m_vNumBuffers; vIndex++)
				delete m_opBuffers[vIndex];
			delete [] m_opBuffers;
			m_opBuffers = NULL;
		}
	
		if(m_opBuffers == NULL)
		{
			m_opBuffers = EM_new EMMediaDataBuffer*[m_vNumBuffers];
			for(int32 vIndex = 0; vIndex < m_vNumBuffers; vIndex++)
				m_opBuffers[vIndex] = NULL;

		}
		if((m_eType & EM_TYPE_ANY_AUDIO) > 0)
		{
			EMMediaFormat* opFormat = EMMediaUtility::Instance() -> GetSystemAudioFormat();
			int64 vBufferSize = opFormat -> m_vBufferSizeBytes;

			for(int32 vIndex = 0; vIndex < m_vNumBuffers; vIndex++)
			{
				m_opBuffers[vIndex] = EM_new EMMediaDataBuffer(EM_TYPE_RAW_AUDIO, vBufferSize);
				m_opBuffers[vIndex] -> m_vSizeAvailable = vBufferSize;
				memset(m_opBuffers[vIndex] -> Data(), 0, m_opBuffers[vIndex] -> m_vSizeAvailable);
				m_opBuffers[vIndex] -> m_vAvailable = true;
			}
		}
		else if((m_eType & EM_TYPE_ANY_VIDEO) > 0)
		{
			for(int32 vIndex = 0; vIndex < m_vNumBuffers; vIndex++)
			{
				if(m_opBuffers[vIndex] !=NULL)
				{
					m_opBuffers[vIndex] -> AcquireDataBufferSemaphore();
					m_opBuffers[vIndex] ->ReallocateDataBuffer((p_opFormat -> m_vBytesPerRow * p_opFormat -> m_vHeight));
				} else
				{
					m_opBuffers[vIndex] = EM_new EMMediaDataBuffer(EM_TYPE_RAW_VIDEO, (p_opFormat -> m_vBytesPerRow * p_opFormat -> m_vHeight));
					m_opBuffers[vIndex] -> AcquireDataBufferSemaphore();
				}
				m_opBuffers[vIndex] -> m_vSizeAvailable = (p_opFormat -> m_vBytesPerRow * p_opFormat -> m_vHeight);
				m_opBuffers[vIndex] -> m_oFormat.m_vWidth = p_opFormat -> m_vWidth;
				m_opBuffers[vIndex] -> m_oFormat.m_vHeight = p_opFormat -> m_vHeight;
				m_opBuffers[vIndex] -> m_oFormat.m_vBytesPerSample = p_opFormat -> m_vBytesPerSample;
				m_opBuffers[vIndex] -> m_oFormat.m_vBytesPerRow = p_opFormat -> m_vBytesPerRow;
				m_opBuffers[vIndex] -> m_oFormat.m_vBufferSizeBytes = (p_opFormat -> m_vBytesPerRow * p_opFormat -> m_vHeight);
				m_opBuffers[vIndex] -> m_oFormat.m_vFrameRate = p_opFormat -> m_vFrameRate;
				m_opBuffers[vIndex] -> m_oFormat.m_vDepth = p_opFormat -> m_vDepth;

				m_opBuffers[vIndex] -> m_vAvailable = true;
				m_opBuffers[vIndex] -> ReleaseDataBufferSemaphore();
			}
		}
		else if((m_eType & EM_TYPE_MIDI) > 0)
		{
			for(int32 vIndex = 0; vIndex < m_vNumBuffers; vIndex++)
			{
				m_opBuffers[vIndex] = EM_new EMMediaDataBuffer(EM_TYPE_MIDI, EM_MIDI_BUFFER_SIZE);
				//m_opBuffers[vIndex] -> m_vSizeAvailable = EM_MIDI_BUFFER_SIZE;
				//memset(m_opBuffers[vIndex] -> Data(), 0, m_opBuffers[vIndex] -> m_vSizeAvailable);
				m_opBuffers[vIndex] -> m_vAvailable = true;
			}
		}
		else
			MessageBox(NULL, "ERROR! In EMMediaBufferSource::InitCheckE: Only audio implemented so far!", "MediaEngine error!", MB_OK);
	
		m_vHasBeenInitialized = true;
	}
	catch(...)
	{
		MessageBox(NULL, "ERROR! Caught an exception in EMMediaBufferSource::InitCheckBufferE()", "MediaEngine error!", MB_OK);
	}
	m_opBufferProtectionSem -> Release();
	
	return true;

}
void EMMediaBufferSource::SetDestination(EMBufferDestination* p_opDestination)
{
	m_opDestinationSemaphore -> Acquire();
	if(m_opDestination != NULL)
		m_opDestination -> RemoveProcessingReference();
	m_opDestination = p_opDestination;

	if(m_opDestination != NULL)							//Kaneda - hack/fix
		m_opDestination -> AddProcessingReference();

	m_opDestinationSemaphore -> Release();
}

void EMMediaBufferSource::SetBufferPriority(int32 p_vPriority)
{
	m_opBufferProtectionSem -> Acquire();
	for(int32 vIndex = 0; vIndex < m_vNumBuffers; vIndex++)
		m_opBuffers[vIndex] -> m_vPriority = p_vPriority;
	m_opBufferProtectionSem -> Release();
}

void EMMediaBufferSource::SetBufferFormat(const EMMediaFormat* p_opFormat)
{
	m_opBufferProtectionSem -> Acquire();
	for(int32 vIndex = 0; vIndex < m_vNumBuffers; vIndex++)
		m_opBuffers[vIndex] -> m_oFormat.Copy(p_opFormat);
	m_opBufferProtectionSem -> Release();
}

void EMMediaBufferSource::SetBufferSilence(bool p_vFlag)
{
	m_opBufferProtectionSem -> Acquire();
	for(int32 vIndex = 0; vIndex < m_vNumBuffers; vIndex++)
	{
		m_opBuffers[vIndex] -> m_vIsSilence = p_vFlag;
		m_opBuffers[vIndex] -> m_vSizeUsed = m_opBuffers[vIndex] -> m_vSizeAvailable;
	}
	m_opBufferProtectionSem -> Release();
}

bool EMMediaBufferSource::SaveData(EMProjectDataSaver* p_opSaver)
{
	p_opSaver -> SaveUInt32(static_cast<uint32>(m_eType));
	p_opSaver -> SaveUInt32(static_cast<uint32>(m_vID));
	p_opSaver -> SaveUInt32(static_cast<uint32>(m_vNumBuffers));

	return true;
}

bool EMMediaBufferSource::LoadData(EMProjectDataLoader* p_opLoader)
{
	uint32 vType = p_opLoader -> LoadUInt32();
	EMMediaType eRealType = EM_TYPE_ANY;
	if((vType & EM_TYPE_RAW_AUDIO) == EM_TYPE_RAW_AUDIO) 
		eRealType = EM_TYPE_RAW_AUDIO;
	else if((vType & EM_TYPE_ENCODED_AUDIO) == EM_TYPE_RAW_AUDIO) 
		eRealType = EM_TYPE_ENCODED_AUDIO;
	else if((vType & EM_TYPE_ANY_AUDIO) == EM_TYPE_RAW_AUDIO) 
		eRealType = EM_TYPE_ANY_AUDIO;
	else if((vType & EM_TYPE_RAW_VIDEO) == EM_TYPE_RAW_VIDEO) 
		eRealType = EM_TYPE_RAW_VIDEO;
	else if((vType & EM_TYPE_ENCODED_VIDEO) == EM_TYPE_ENCODED_VIDEO) 
		eRealType = EM_TYPE_ENCODED_VIDEO;
	else if((vType & EM_TYPE_ANY_VIDEO) == EM_TYPE_ANY_VIDEO) 
		eRealType = EM_TYPE_ANY_VIDEO;
	else if((vType & EM_TYPE_TRANSITION) == EM_TYPE_TRANSITION) 
		eRealType = EM_TYPE_TRANSITION;
	else if((vType & EM_TYPE_MIDI) == EM_TYPE_MIDI)
		eRealType = EM_TYPE_MIDI;
	else if((vType & EM_TYPE_BMP) == EM_TYPE_BMP)
		eRealType = EM_TYPE_BMP;
	else if((vType & EM_TYPE_GIF) == EM_TYPE_GIF)
		eRealType = EM_TYPE_GIF;
	else if((vType & EM_TYPE_JPG) == EM_TYPE_JPG)
		eRealType = EM_TYPE_JPG;
	else if((vType & EM_TYPE_TGA) == EM_TYPE_TGA)
		eRealType = EM_TYPE_TGA;
	else 
		EMDebugger("ERROR! Unknown type saved in project file!");


	m_eType = eRealType;
	
	m_vID = static_cast<int32>(p_opLoader -> LoadUInt32());
	
	m_opDestination = NULL;
	m_vNumBuffers = static_cast<int32>(p_opLoader -> LoadUInt32()); 
	return true;
}

EMMediaTrack* EMMediaBufferSource::GetTrack() const
{
	return NULL;
}
