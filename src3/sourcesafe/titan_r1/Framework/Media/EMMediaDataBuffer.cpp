#include "EMMediaDataBuffer.h"
#include "EMMediaIDManager.h"
#include "EMMediaGlobals.h"
#include "EMMediaFormat.h"
#include "EMMediaMIDIEvent.h"
#include "EMSemaphore.h"

EMMediaDataBuffer::EMMediaDataBuffer(EMMediaType p_eType, uint64 p_vSize)
	:	m_opDestination(NULL),
		m_opStartDestination(NULL),
		m_vIsFinished(false),
		m_vIsSilence(false),
		m_vSizeUsed(0),
		m_vFader(127),
		m_vPan(0),
		m_vPriority(1),
		m_oFormat(p_eType),
		m_vShouldBeDeleted(false),
		m_vFrameRelative(0),
		m_vFrame(0),
		m_vpBackBuffer(0),
		m_opDataPointer(NULL),
		m_vDiscon(FALSE),
		m_vSwapped(false),
		m_vOriginalHasBeenWasted(false)
{
	m_vID = EMMediaIDManager::MakeID();
	
	if((p_eType & EM_TYPE_MIDI) > 0)
	{
		m_vpData = reinterpret_cast<unsigned char*>(EM_new list<EMMediaMIDIEvent*>());
		m_vSizeAvailable = 0;
	}
	else
	{
		m_vpData = EM_new unsigned char[p_vSize];
		m_vSizeAvailable = p_vSize;
		memset(m_vpData, 255, m_vSizeAvailable);
	}
	char xxx[200];
	sprintf(xxx, "MEDIABUFFERDATASEMAPHORE_%d\0", (int32) this);
	m_opDataBufferProtectionSemaphore = EMSemaphore::CreateEMSemaphore(xxx);

}

EMMediaDataBuffer::EMMediaDataBuffer(EMMediaType p_eType, uint64 p_vSize, void* p_opDataPointer)
	:	m_opDestination(NULL),
		m_opStartDestination(NULL),
		m_vIsFinished(false),
		m_vIsSilence(false),
		m_vSizeUsed(0),
		m_vFader(127),
		m_vPan(0),
		m_vPriority(1),
		m_oFormat(p_eType),
		m_vShouldBeDeleted(false),
		m_vFrame(0),
		m_vpBackBuffer(0),
		m_opDataPointer(NULL),
		m_vDiscon(FALSE),
		m_vSwapped(false),
		m_vOriginalHasBeenWasted(false)
{
	m_vID = EMMediaIDManager::MakeID();
	m_vpData = NULL;
	m_opDataPointer = p_opDataPointer;
	m_vSizeAvailable = p_vSize;
	char xxx[200];
	sprintf(xxx, "MEDIABUFFERDATASEMAPHORE_%d\0", (int32) this);
	m_opDataBufferProtectionSemaphore = EMSemaphore::CreateEMSemaphore(xxx);
}


EMMediaDataBuffer::~EMMediaDataBuffer()
{
	if (m_vpBackBuffer != NULL)
		delete m_vpBackBuffer;
	
	if(m_vpData != NULL)
		delete m_vpData;
}

int32 EMMediaDataBuffer::GetID() const
{
	return m_vID;
}

void EMMediaDataBuffer::SetDataPointer(void* p_opDataPointer)
{
	m_opDataPointer = p_opDataPointer;
}

void* EMMediaDataBuffer::SwapBuffer(void* p_opBuffer)
{
	void* opRet;

	if(m_vpData != NULL)
	{
		opRet = m_vpData;
		m_vpData = static_cast<unsigned char*>(p_opBuffer);
	}
	else if(m_opDataPointer != NULL)
	{
		opRet = m_opDataPointer;
		m_opDataPointer = p_opBuffer;
	}
	else
		opRet = NULL;

	return opRet;
}


void EMMediaDataBuffer::Recycle(bool p_vReset)
{
	m_opDestination = m_opStartDestination;
	m_vSizeUsed = 0;
	if(p_vReset)
		memset(Data(), 0, m_vSizeAvailable);
	m_vAvailable = true; 
}

void EMMediaDataBuffer::ReallocateDataBuffer(int64 p_vSize)
{
	delete m_vpData;
	m_vpData = EM_new unsigned char[p_vSize];

	if(m_vpBackBuffer != NULL)
	{
		delete m_vpBackBuffer;
		m_vpBackBuffer = NULL;
	}

	m_vSizeAvailable = p_vSize;
}

void* EMMediaDataBuffer::BackBuffer()
{
	if (m_vpBackBuffer == NULL)
		m_vpBackBuffer = EM_new unsigned char[m_vSizeAvailable];

	return m_vpBackBuffer;
}

void* EMMediaDataBuffer::Data()
{
	if(m_vpData != NULL)
		return static_cast<void*>(m_vpData);
	else 
		return m_opDataPointer;
}

void EMMediaDataBuffer::SetFrame(int64 p_vFrame)
{
	m_vFrame = p_vFrame;
}

void EMMediaDataBuffer::SetRelativeTimeToStart(int64 p_vFrame)
{
/*	if(p_vFrame > 16000000)
	{
		int arne = 0;
	}*/
	m_vFrameRelative = p_vFrame;
}

void EMMediaDataBuffer::SetRelativeTimeToStartForSynchClock(int64 p_vFrame)
{
	m_vFrameRelativeForSynchClock = p_vFrame;
}

int64 EMMediaDataBuffer::GetRelativeTimeToStartForSynchClock()
{
	return m_vFrameRelativeForSynchClock;
}


void EMMediaDataBuffer::SwapBuffer()
{
	if (m_vpBackBuffer != 0)
	{
		unsigned char* vpTmp = m_vpBackBuffer;
		m_vpBackBuffer = m_vpData;
		m_vpData = vpTmp;
		m_vSwapped = !m_vSwapped;
		m_vOriginalHasBeenWasted = true;
	}
}

void EMMediaDataBuffer::RestoreBuffer()
{
	if(m_vSwapped)
		SwapBuffer();
}


int64 EMMediaDataBuffer::GetRelativeTimeToStart() const
{
	return m_vFrameRelative;
}

int64 EMMediaDataBuffer::GetFrame() const
{
	return m_vFrame;
}

void EMMediaDataBuffer::AcquireDataBufferSemaphore()
{
	m_opDataBufferProtectionSemaphore -> Acquire ();
}

void EMMediaDataBuffer::ReleaseDataBufferSemaphore()
{
	m_opDataBufferProtectionSemaphore -> Release();
}