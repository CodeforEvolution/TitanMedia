/*******************************************************
* Creator: Richard Kronfalt
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#ifndef __EM_REALTIME_INPUT_DESCRIPTOR
#define __EM_REALTIME_INPUT_DESCRIPTOR

#include "EMInputDescriptor.h"
#include "EMMediaTrack.h"

#include <list>

class EMMediaDataBuffer;
class EMRecording;
class EMProjectDataSaver;
class EMProjectDataLoader;

class EMRealtimeInputDescriptor : public EMInputDescriptor
{
public:
	virtual ~EMRealtimeInputDescriptor();
	EMRecording* GetLastRecording() const;
	void AddRecorder(EMMediaTrack* p_opTrack);
	void RemoveRecorder(EMMediaTrack* p_opTrack);
	virtual bool InitCheckE() = 0;
	virtual bool PrepareToPlayE() = 0;
	virtual bool StartPreviewE() = 0;
	virtual void StartE() = 0;
	virtual void StopE() = 0;
	virtual bool StopPreviewE() = 0;
	virtual bool ClearData() = 0;
	virtual bool SaveData(EMProjectDataSaver* p_opSaver) = 0;
	virtual bool LoadData(EMProjectDataLoader* p_opLoader) = 0;
	virtual void SetArmed(bool p_vShouldArm);

protected:
	EMRealtimeInputDescriptor(EMMediaType p_eType);
	bool m_vIsInitialized;
	list<EMMediaTrack*> oRegisteredRecorders;
	EMRecording* m_opRecording;
};

#endif
