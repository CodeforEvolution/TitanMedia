#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#include "EMBeBufferRepository.h"
#include "EMMediaIDManager.h"
#include "EMBeMediaUtility.h"

#include <Bitmap.h>
#include <Buffer.h>
#include <OS.h>

EMBeBufferRepository::EMBeBufferRepository()
	:	m_vAudioIsInitialized(false),
		m_vVideoIsInitialized(false),
		m_opAudioBuffers(NULL),
		m_opVideoBuffers(NULL),
		m_opVideoBuffersRclm(NULL),
		m_vVideoBufferSize(0),
		m_vVideoBufferSizeRclm(0),
		m_opBitmapArray(NULL),
		m_vNumVideoBitmaps(0),
		m_opMediaFormat(NULL),
		m_opBBufferArray(NULL),
		vNextVideoBuffer(0),
		m_vFirstRun(true)
{
//	EMDebugger("EMBeBufferRepository::EMBeBufferRepository()");
	m_vID = EMMediaIDManager::MakeID();
	EMBeMediaUtility::push(this, "EMBeBufferRepository");
	vVideoSemaphore = create_sem(1, "Video buffer array protection sem");		

	//FOR VIDEO

}

EMBeBufferRepository::~EMBeBufferRepository()
{
	//EMDebugger("EMBeBufferRepository::~EMBeBufferRepository()");

	delete m_opAudioBuffers;


	if(m_opVideoBuffersRclm != NULL)
	{
		list<BBuffer*> oDummyBuffersRclm;
		int32 vCount;
		m_opVideoBuffersRclm -> CountBuffers(&vCount);
		for(int32 vIndex = 0; vIndex < vCount; vIndex++)
		{
			BBuffer* opDummy = m_opVideoBuffersRclm -> RequestBuffer(1, -1);
			if(opDummy == NULL)
			{
				vIndex--;
				snooze(5000);
				emerr << "ERROR! Couldn't get video buffer... Waiting!" << endl;
			}
			else
				oDummyBuffersRclm.push_back(opDummy);
		}
				
		while(oDummyBuffersRclm.size() > 0)
		{
			BBuffer* opBuffer = oDummyBuffersRclm.front();
			oDummyBuffersRclm.pop_front();
			opBuffer -> Recycle();
		}
		if(m_opBitmapArray != NULL)
			for(int32 vIndex = 0; vIndex < m_vNumVideoBitmaps; vIndex++)
				delete m_opBitmapArray[vIndex];

		m_opBitmapArray = NULL;
		if(m_opVideoBuffersRclm != NULL)
		{
			delete m_opVideoBuffers;
		}
		m_opVideoBuffersRclm = NULL;
	}


	if(m_opVideoBuffers != NULL)
	{

		list<BBuffer*> oDummyBuffers;
		int32 vCount;
		m_opVideoBuffers -> CountBuffers(&vCount);
		for(int32 vIndex = 0; vIndex < vCount; vIndex++)
		{
			BBuffer* opDummy = m_opVideoBuffers -> RequestBuffer(1, -1);
			if(opDummy == NULL)
			{
				vIndex--;
				snooze(5000);
				emerr << "Couldn't get video buffer... Waiting!" << endl;
			}
			else
				oDummyBuffers.push_back(opDummy);
		}
				
		while(oDummyBuffers.size() > 0)
		{
			BBuffer* opBuffer = oDummyBuffers.front();
			oDummyBuffers.pop_front();
			opBuffer -> Recycle();
		}
		if(m_opBitmapArray != NULL)
			for(int32 vIndex = 0; vIndex < m_vNumVideoBitmaps; vIndex++)
				delete m_opBitmapArray[vIndex];

		m_opBitmapArray = NULL;
		if(m_opVideoBuffers != NULL)
		{
			delete m_opVideoBuffers;
		}
		m_opVideoBuffers = NULL;
	}
	
	delete m_opMediaFormat;

/*	Notify(EM_MESSAGE_BUFFER_DELETION);	
	if(m_opBitmapArrayRclm != NULL)
	{
		for(int32 vIndex = 0; vIndex < m_vNumVideoBitmaps; vIndex++)
		{
			delete m_opBitmapArrayRclm[vIndex];
		}
	}
	m_opBitmapArrayRclm = NULL;
	if(m_opVideoBuffersRclm != NULL)
	{
		delete m_opVideoBuffersRclm;
	}
	m_opVideoBuffersRclm = NULL;

	delete m_opMediaFormat;
*/
	
//	for(int32 vIndex = 0; vIndex < m_vNumVideoBitmaps; vIndex++)
//		delete m_opBitmapArray[vIndex];
	m_opInstance = NULL;
	delete_sem(vVideoSemaphore);
	EMBeMediaUtility::pop("EMBeBufferRepository");
}

BBufferGroup* EMBeBufferRepository::GetAudioBufferGroup()
{
	return m_opAudioBuffers;
}

int32 EMBeBufferRepository::GetID() const
{
	return m_vID;
}

BBufferGroup* EMBeBufferRepository::GetVideoBufferGroup()
{
//	EMDebugger("Buffer group returning video buffers");
	return m_opVideoBuffers;
}

bool EMBeBufferRepository::InitializeAudioE(uint64 p_vBufferSize, int32 p_vNumBuffers)
{
//	EMDebugger("EMBeBufferRepository::InitializeAudioE()");
	if(! m_vAudioIsInitialized)
	{
		//On the third day, God created a BBufferGroup for audio-BBuffers, and there
		//was much joy amongst the system
		m_opAudioBuffers = new BBufferGroup(p_vBufferSize, p_vNumBuffers);
		if(m_opAudioBuffers -> InitCheck() != B_OK)
			return false;
		m_vAudioIsInitialized = true;
	}
	return true;		
}

bool EMBeBufferRepository::ResetVideoE(media_format* p_format)
{
	Notify(EM_MESSAGE_BUFFER_DELETION);	
	if(m_opBitmapArrayRclm != NULL)
	{
		for(int32 vIndex = 0; vIndex < m_vNumVideoBitmaps; vIndex++)
		{
			delete m_opBitmapArrayRclm[vIndex];
		}
	}
	m_opBitmapArrayRclm = NULL;
	if(m_opVideoBuffersRclm != NULL)
	{
		delete m_opVideoBuffersRclm;
	}
	m_opVideoBuffersRclm = NULL;

// NEW
//	uint32 mXSize = p_format -> Width();//720; //with_format.u.raw_video.display.line_width;
//	uint32 mYSize = p_format -> Height();//576; //with_format.u.raw_video.display.line_count;
//	color_space mColorspace = p_format -> ColorSpace(); //with_format.u.raw_video.display.format;

	if(m_opMediaFormat == NULL)
		m_opMediaFormat = new EMMediaFormat(EM_TYPE_ANY_VIDEO);
			
	m_opMediaFormat -> CreateFormat(p_format);
//	m_vVideoBufferSize = m_vVideoBufferSizeRclm;
	
	return true;
}

void EMBeBufferRepository::NotifyVideoFormatChange()
{
	Notify(EM_MESSAGE_BUFFER_SIZE_CHANGED);	
}

void EMBeBufferRepository::NotifyHistoryBufferDelete()
{
	Notify(EM_MESSAGE_BUFFER_DELETION);	
}


int32 EMBeBufferRepository::GetVideoBufferSize()
{
	return m_vVideoBufferSize;
}

EMMediaFormat* EMBeBufferRepository::GetCurrentVideoFormat()
{
	if(m_opMediaFormat == NULL)
		m_opMediaFormat = new EMMediaFormat(EM_TYPE_RAW_VIDEO);
	return m_opMediaFormat;
}


bool EMBeBufferRepository::InitializeVideoE(uint64 p_vBufferSize, int32 p_vNumBuffers, media_format* p_format, bool p_vDeleteFlag = false)
{
//	EMDebugger("EMBeBufferRepository::InitializeVideoE()");
	status_t vAcquireResult = acquire_sem(vVideoSemaphore);	
	if(vAcquireResult != B_NO_ERROR)
		EMDebugger("ERROR! EMBeBufferRepository could not acquire semaphore for video buffer protection!");
	if(! m_vVideoIsInitialized || p_vDeleteFlag)
	{
//		EMDebugger("EMBeBufferRepository::Actually initializing buffers()");
		
		//On the fourth day, God created a BBufferGroup for video-BBuffers, and there
		//was much joy amongst the system

		if(m_opVideoBuffers != NULL)
			m_opVideoBuffersRclm = m_opVideoBuffers;
		
//(		EMDebugger("Buffer reposutory initialize creating new buffer group");
		m_opVideoBuffers = new BBufferGroup();

		if(m_opVideoBuffers -> InitCheck() != B_OK)
		{
			EMDebugger("SOMETHING WONDERFULL HAS HAPPENED... Couldn't create BBufferGroup in EMBeBufferRepository");
		}	

		if(m_opBitmapArray != NULL)
			m_opBitmapArrayRclm = m_opBitmapArray;
	
		m_opBitmapArray = new BBitmap*[p_vNumBuffers];

		for(int32 vIndex = 0; vIndex < p_vNumBuffers; vIndex++)
			m_opBitmapArray[vIndex] = NULL;	

		uint32 mXSize = p_format -> Width();//720; //with_format.u.raw_video.display.line_width;
		uint32 mYSize = p_format -> Height();//576; //with_format.u.raw_video.display.line_count;
		color_space mColorspace = p_format -> ColorSpace(); //with_format.u.raw_video.display.format;
		// and attach the bitmaps to the buffer group
		for(int32 vIndex = 0; vIndex < p_vNumBuffers; vIndex++)
		{
			m_opBitmapArray[vIndex] = new BBitmap(BRect(0, 0, (mXSize-1), (mYSize - 1)), mColorspace, false, true);
			if(m_opBitmapArray[vIndex] -> IsValid())
			{
				sInfo = new buffer_clone_info;						
				if((sInfo -> area = area_for(m_opBitmapArray[vIndex] -> Bits())) == B_ERROR)
				{
					EMDebugger("ERROR! Could not calculate the address space of the memory area of the bitmap!");
				}
			
				sInfo -> offset = 0; 	//The offset into the memory space
				sInfo -> size = (size_t) m_opBitmapArray[vIndex] -> BitsLength();
				sInfo -> flags = vIndex;	//Which bitmap in the array does this buffer correspond to?
				sInfo -> buffer = 0;	//media_buffer_id
				if(m_vVideoBufferSize == 0)
					m_vVideoBufferSize = sInfo -> size;
				else
				{
					m_vVideoBufferSizeRclm = sInfo -> size;
					m_vVideoBufferSize = sInfo -> size;	
				}	
				if(m_opVideoBuffers -> AddBuffer(*sInfo) != B_OK)
				{
					//Error
					emerr << "ERROR! Could not add buffer to buffer group:" << endl;
					return false;
				}
				delete sInfo;
			}
			else 
			{
				//Error
				emerr << "ERROR! The bitmap created is not valid: Size is 0,0 - " << mXSize-1 << "," << mYSize - 1 << endl;
				EMDebugger("ERROR! Bitmap not valid!");
				return false;
			}	
		}
		m_vNumVideoBitmaps = p_vNumBuffers;
			
		m_vVideoIsInitialized = true;
	}

	int32 c = 0;
	m_opVideoBuffers -> CountBuffers(&c);
	release_sem(vVideoSemaphore);

	return true;		
}

BBuffer* EMBeBufferRepository::GetVideoBuffer()
{
	while(true)
	{
		status_t vAcquireResult = acquire_sem(vVideoSemaphore);
		if(vAcquireResult != B_NO_ERROR)
			EMDebugger("ERROR! EMBeBufferRepository could not acquire semaphore for video buffer protection!");

		if(m_opBBufferArray[vNextVideoBuffer] != NULL)
		{
			release_sem(vVideoSemaphore);
			break;
		}
		release_sem(vVideoSemaphore);
		snooze(2000);
	}
		
	status_t vAcquireResult = acquire_sem(vVideoSemaphore);
	if(vAcquireResult != B_NO_ERROR)
		EMDebugger("ERROR! EMBeBufferRepository could not acquire semaphore for video buffer protection!");

	BBuffer* opBuffer = m_opBBufferArray[vNextVideoBuffer];
	m_opBBufferArray[vNextVideoBuffer] = NULL;
	vNextVideoBuffer++;
	if(vNextVideoBuffer == m_vNumVideoBitmaps)
		vNextVideoBuffer = 0;
	
	release_sem(vVideoSemaphore);
	return opBuffer;
}

void EMBeBufferRepository::RecycleVideoBuffer(BBuffer* p_opBuffer)
{
	status_t vAcquireResult = acquire_sem(vVideoSemaphore);
	if(vAcquireResult != B_NO_ERROR)
		EMDebugger("ERROR! EMBeBufferRepository could not acquire semaphore for video buffer protection!");

	if(m_opBBufferArray[p_opBuffer -> Flags()] != NULL)
		EMDebugger("ERROR! Video BBuffer slots confused!");
	
	m_opBBufferArray[p_opBuffer -> Flags()] = p_opBuffer;
	release_sem(vVideoSemaphore);
	return;
}

BBitmap** EMBeBufferRepository::GetVideoBitmaps() const
{
	return m_opBitmapArray;
}

int32 EMBeBufferRepository::GetNumVideoBitmaps() const
{
	return m_vNumVideoBitmaps;
}

#endif
