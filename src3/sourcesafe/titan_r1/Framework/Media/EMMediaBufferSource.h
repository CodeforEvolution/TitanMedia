/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
* 
* This little pervert forces itself upon children!
*
* To be less humouristic, it allows its children to 
* have EMMediaDataBuffers (or actually, ONE) by supplying
* a pre-allocated EMMediaDataBuffer, and a public Get-
* method for it, together with Get- and SetDestination 
* methods for controlling which destination the buffer
* should be sent to first.
*
* This class is typically inherited by starting-points in
* the media-stream chain, such as EMMediaClip. Probably,
* classes such as EMBeFileInputDescriptor, or even
* EMBeInputDescriptor should inherit from this class
* as well.
* 
* Richard Kronfält, 00-11-28
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_MEDIA_BUFFER_SOURCE
#define __EM_MEDIA_BUFFER_SOURCE

#include "EMMediaGlobals.h"
#include "EMMediaFormat.h"

class EMMediaDataBuffer;
class EMBufferDestination;
class EMSemaphore;
class EMProjectDataLoader;
class EMProjectDataSaver;
class EMMediaTrack;

class EMMediaBufferSource 
{
public:
	virtual ~EMMediaBufferSource();
	int32 GetID() const;
	virtual EMMediaDataBuffer* GetBuffer() const;
	virtual EMMediaDataBuffer* GetBufferAt(int64 p_vTime, bool p_vSeeking);
	EMBufferDestination* GetDestination() const;
	EMMediaType GetType() const;
	void RecycleAllBuffers() const;
	void SetType(EMMediaType p_eType);
	virtual bool InitCheckE(EMMediaFormat* p_opFormat);
	void SetDestination(EMBufferDestination* p_opDestination);
	void SetBufferPriority(int32 p_vPriority);
	void SetBufferFormat(const EMMediaFormat* p_opFormat);
	void SetBufferSilence(bool p_vFlag);
	bool InitCheckDestinationsE(EMMediaFormat* p_opFormat);
	bool InitCheckBufferE(EMMediaFormat* p_opFormat);
	virtual bool SaveData(EMProjectDataSaver* p_opSaver);
	virtual bool LoadData(EMProjectDataLoader* p_opLoader);

	virtual EMMediaTrack* GetTrack() const;
	void AcquireBufferProtectionSemaphore();
	void ReleaseBufferProtectionSemaphore();
	bool m_vShouldSeek;

protected:
	EMMediaBufferSource(EMMediaType p_eType);
	EMMediaBufferSource(); //Only used for loading
	EMMediaDataBuffer** m_opBuffers;
	int32 m_vID;
	bool m_vHasBeenInitialized;
	int32 m_vNumBuffers;

private:
	EMMediaType m_eType;
	EMBufferDestination* m_opDestination;
	EMSemaphore* m_opDestinationSemaphore;
	EMSemaphore* m_opBufferProtectionSem;
};

#endif
