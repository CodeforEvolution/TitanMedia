#include "EMGlobals.h"

//#ifdef PLATFORM_BEOS

#include "EMBeMediaFile.h"
#include "EMMediaFile.h"
#include "EMBeMediaUtility.h"
#include "EMMediaFileManager.h"

#ifdef CSTYLE_FILE
#include <stdio.h>
#endif

EMBeMediaFile::EMBeMediaFile(const string* p_opName)
	:	EMMediaFile(p_opName),
		m_opInFile(NULL),
		m_opCStyleInFile(NULL),
		m_opOutFile(NULL)
{
//	m_vSemaphore = create_sem(1, "Media file semaphore");
}

EMBeMediaFile::~EMBeMediaFile()
{
//	delete_sem(m_vSemaphore);
}
 
bool EMBeMediaFile::Close()
{
	EMMediaFileManager::Lock();

#ifndef CSTYLE_FILE
	if(m_opInFile != NULL)
		m_opInFile -> close();
#else
	if(m_opCStyleInFile != NULL)
		fclose(m_opCStyleInFile);
#endif
	if(m_opOutFile != NULL)
		m_opOutFile -> close();
	
	EMMediaFileManager::Unlock();
	return false;
}

bool EMBeMediaFile::Open(int32 p_vDirection)
{
	if(IsOpen() && 
		((p_vDirection == EM_READ && m_opCStyleInFile != NULL) ||
		 (p_vDirection == EM_WRITE && m_opInFile != NULL)))
	{
		return true;
	}
		
//	status_t vAcquireResult = acquire_sem(m_vSemaphore);
//	if(vAcquireResult != B_NO_ERROR)
//		EMDebugger("ERROR! EMBeMediaFile could not acquire semaphore for file protection!");
	EMMediaFileManager::Lock();
	
	if(p_vDirection == EM_READ)
	{
#ifndef CSTYLE_FILE
		if(m_opInFile != NULL)
		{
			if(m_opInFile -> is_open())
				m_opInFile -> close();
			delete m_opInFile;
		}	
		m_opInFile = new ifstream(GetFileName() -> c_str(), ios::in | ios::binary);
#else
		if(m_opCStyleInFile != NULL)
		{
			fclose(m_opCStyleInFile);
			m_opCStyleInFile = 0;
		}
		m_opCStyleInFile = fopen(GetFileName() -> c_str(), "rb");
#endif
	}
	else if(p_vDirection == EM_WRITE)
	{
		if(m_opOutFile != NULL)
		{
			if(m_opOutFile -> is_open())
				m_opOutFile -> close();
			delete m_opOutFile;
		}	
		m_opOutFile = new ofstream(GetFileName() -> c_str(), ios::out | ios::binary | ios::trunc);
//		if(! (m_opInFile -> is_open()))
//		{
//		}
	}
	
	if(m_opCStyleInFile == NULL && m_opInFile == NULL)
		;

	m_vPosition = 0;
	SetOpen(true);
	EMMediaFileManager::Unlock();
	return true;
}

int64 EMBeMediaFile::Read(void* p_upBuffer, int64 p_vLength, int64 p_vStartPosition)
{
	EMMediaFileManager::Lock();
	size_t vNum = 0;
#ifndef CSTYLE_FILE
	if(m_opInFile != NULL)
#else

	if(m_opCStyleInFile != NULL)
#endif
	{
#ifndef CSTYLE_FILE
		if(! m_opInFile -> is_open())
			EMDebugger("ERROR! Attempted to read media file while it was closed!");
#endif

#ifndef CSTYLE_FILE
		if(p_vStartPosition != m_opInFile -> tellg()) //m_vPosition)
			m_opInFile -> seekg(p_vStartPosition, ios::beg);
#else
		if(p_vStartPosition != ftell(m_opCStyleInFile))
			fseek(m_opCStyleInFile, p_vStartPosition, SEEK_SET);
#endif
		char* vpArray = (char*) p_upBuffer;		
		vNum = 0;

#ifndef CSTYLE_FILE
		register int16 vIndex = 0;
		int16 vMax = (int16) p_vLength;
		for(vIndex = 0; vIndex < vMax && m_opInFile -> good(); vIndex++)
			(*(vpArray++)) = m_opInFile -> get();
		vNum = vIndex;
#else
		vNum = fread(vpArray, 1, p_vLength, m_opCStyleInFile);
#endif
		
#ifndef CSTYLE_FILE
		if(m_opInFile -> eof() || m_opInFile -> fail())
		{
			m_opInFile -> close();
			m_opInFile -> open(GetFileName() -> c_str(), ios::in | ios::bin);
		}
#else
		if(feof(m_opCStyleInFile))
			freopen(GetFileName() -> c_str(), "rb", m_opCStyleInFile); 
#endif
		m_vPosition = p_vStartPosition + vNum;
	}
	else
		EMDebugger("ERROR! Can't read from file if the ifstream is NULL!");
	EMMediaFileManager::Unlock();
	return vNum;
}

void EMBeMediaFile::Write(void* p_upBuffer, int64 p_vLength)
{
//	status_t vAcquireResult = acquire_sem(m_vSemaphore);
//	if(vAcquireResult != B_NO_ERROR)
//		EMDebugger("ERROR! EMBeMediaFile could not acquire semaphore for file protection!");
	EMMediaFileManager::Lock();
	if(m_opOutFile != NULL)
	{
		if(! m_opOutFile -> is_open())
		{
			EMDebugger("ERROR! Can't write to file when it's closed!");
			EMMediaFileManager::Unlock();
			return;
		}
		
		int n = m_opOutFile -> tellp();
		m_opOutFile -> write((char*) p_upBuffer, p_vLength);
		m_vPosition += p_vLength;
	}
	else
		EMDebugger("Can't write to file if the ofstream is NULL!");
	EMMediaFileManager::Unlock();
}

bool EMBeMediaFile::SeekFromEnd(int64 p_vByteNumber)
{
//	status_t vAcquireResult = acquire_sem(m_vSemaphore);
//	if(vAcquireResult != B_NO_ERROR)
//		EMDebugger("ERROR! EMBeMediaFile could not acquire semaphore for file protection!");
	EMMediaFileManager::Lock();

#ifndef CSTYLE_FILE
	if(m_opInFile != NULL)
		m_opInFile -> seekg(p_vByteNumber, ios::end);
#else
	if(m_opCStyleInFile != NULL)
		fseek(m_opCStyleInFile, p_vByteNumber, SEEK_END);
#endif

	if(m_opOutFile != NULL)
	{
		m_opOutFile -> flush();
		m_opOutFile -> seekp(p_vByteNumber, ios::end);
//		m_vPosition = m_opOutFile -> tellp();
	}
	EMMediaFileManager::Unlock();
	return true;
}

bool EMBeMediaFile::SeekFromStart(int64 p_vByteNumber)
{
//	status_t vAcquireResult = acquire_sem(m_vSemaphore);
//	if(vAcquireResult != B_NO_ERROR)
//		EMDebugger("ERROR! EMBeMediaFile could not acquire semaphore for file protection!");
	EMMediaFileManager::Lock();

#ifndef CSTYLE_FILE
	if(m_opInFile != NULL)
		m_opInFile -> seekg(p_vByteNumber, ios::beg);
#else
	if(m_opCStyleInFile != NULL)
		fseek(m_opCStyleInFile, p_vByteNumber, SEEK_SET);
#endif	

	if(m_opOutFile != NULL)
	{
		m_opOutFile -> flush();
		m_opOutFile -> seekp(p_vByteNumber, ios::beg);
	}
//	m_vPosition = p_vByteNumber;
	EMMediaFileManager::Unlock();
	return true;
}

int64 EMBeMediaFile::GetPosition() const
{
	EMMediaFileManager::Lock();
	int64 vPosition = 0;
#ifndef CSTYLE_FILE
	if(m_opInFile != NULL)
		vPosition = m_opInFile -> tellg();
#else
	if(m_opCStyleInFile != NULL)
		vPosition = ftell(m_opCStyleInFile);
#endif
	if(m_opOutFile != NULL)
		vPosition = m_opOutFile -> tellp();
	EMMediaFileManager::Unlock();
	return vPosition;
}

int64 EMBeMediaFile::GetLength()
{
	int64 vCurrentPosition = 0;
#ifndef CSTYLE_FILE
	if(m_opInFile != NULL)
	{
		vCurrentPosition = m_opInFile -> tellg();
		m_opInFile -> seekg(0, ios::end);
		int64 vEndByte = m_opInFile -> tellg();
		m_opInFile -> seekg(vCurrentPosition, ios::beg);
		return vEndByte;
	}
#else
	if(m_opCStyleInFile != NULL)
	{
		vCurrentPosition = ftell(m_opCStyleInFile);
		fseek(m_opCStyleInFile, 0, SEEK_END);
		int64 vEndByte = ftell(m_opCStyleInFile);
		fseek(m_opCStyleInFile, vCurrentPosition, SEEK_SET);
		return vEndByte;
	}
#endif
	else if(m_opOutFile != NULL)
	{
		vCurrentPosition = m_opOutFile -> tellp();
		m_opOutFile -> seekp(0, ios::end);
		int64 vEndByte = m_opOutFile -> tellp();
		m_opOutFile -> seekp(vCurrentPosition, ios::beg);
		return vEndByte;
	}
	else
		EMDebugger("ERROR! Can't get length of NULL file!");
	return 0;
}

//#endif

