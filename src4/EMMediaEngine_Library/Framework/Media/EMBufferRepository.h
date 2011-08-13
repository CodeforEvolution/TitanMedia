/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
*******************************************************/

#ifndef __EM_BUFFER_REPOSITORY
#define __EM_BUFFER_REPOSITORY

#include "EMMediaGlobals.h"
#include "EMMediaFormat.h"
#include "EMListenerRepository.h"

class EMBufferRepository : public EMListenerRepository
{
public:
	static EMBufferRepository* Instance();
	static void Delete();
	virtual ~EMBufferRepository();
	int32 GetID() const;
	EMMediaFormat* GetCurrentVideoFormat();
	virtual int32 GetNumVideoBitmaps() const = 0;
	virtual void NotifyHistoryBufferDelete() = 0;
	virtual int32 GetVideoBufferSize() = 0;
	
protected:
	EMBufferRepository();
	static EMBufferRepository* m_opInstance;

	int32 m_vID;
	bool m_vAudioIsInitialized;
	bool m_vVideoIsInitialized;
};

#endif
