#include "EMGlobals.h"

#ifndef __EM_MEDIA_DATA_BUFFER
#define __EM_MEDIA_DATA_BUFFER

#include "EMMediaGlobals.h"
#include "EMMediaFormat.h"

class EMBufferDestination;
class EMSemaphore;

class __declspec(dllexport) EMMediaDataBuffer 
{

public:
	EMMediaDataBuffer(EMMediaType p_eType, uint64 p_vSize);
	EMMediaDataBuffer(EMMediaType p_eType, uint64 p_vSize, void* p_opDataPointer);
	~EMMediaDataBuffer();

	void* BackBuffer();
	void* Data();
	void* SwapBuffer(void* p_opBuffer);

	void Recycle(bool p_vReset = false);

	void SwapBuffer();
	void RestoreBuffer();
	void SetDataPointer(void* p_opDataPointer);
	int32 GetID() const;
	void AcquireDataBufferSemaphore();
	void ReleaseDataBufferSemaphore();
	void ReallocateDataBuffer(int64 p_vSize);
	void SetFrame(int64 p_vFrame);
	int64 GetFrame() const;
	void SetRelativeTimeToStart(int64 p_vFrame);
	int64 GetRelativeTimeToStart() const;
	void SetRelativeTimeToStartForSynchClock(int64 p_vFrame);
	int64 GetRelativeTimeToStartForSynchClock();
public:
	EMBufferDestination* m_opDestination;
	EMBufferDestination* m_opStartDestination; //For resetting destination upon Recycle
	int64 m_vIsFinished;
	int64 m_vIsSilence;
	int64 m_vSizeAvailable;
	int64 m_vSizeUsed;
	int32 m_vFader;
	int32 m_vPan;
	int32 m_vPriority;
	bool m_vAvailable;
	EMMediaFormat m_oFormat;
	bool m_vShouldBeDeleted;
	bool m_vDiscon;
	bool m_vOriginalHasBeenWasted;
	int32 m_vBelongsToUniqueID;

private:
	int64 m_vFrame;
	int64 m_vFrameRelative;
	int64 m_vFrameRelativeForSynchClock;
	unsigned char* m_vpData;
	unsigned char* m_vpBackBuffer;
	bool m_vSwapped;
	bool m_vValidBackBuffer;
	void* m_opDataPointer;
	int32 m_vID;
	EMSemaphore* m_opDataBufferProtectionSemaphore;
};

#endif