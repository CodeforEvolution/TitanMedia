/*******************************************************
* Creator: Richard Kronfalt
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_REALTIME_VIDEO_INPUT_DESCRIPTOR
#define __EM_BE_REALTIME_VIDEO_INPUT_DESCRIPTOR

#include "EMRealtimeInputDescriptor.h"
#include "EMBeDvConsumerNode.h"

class EMMediaDataBuffer;
class EMProjectDataSaver;
class EMProjectDataLoader;

class EMBeRealtimeVideoInputDescriptor : public EMRealtimeInputDescriptor
{
public:
	EMBeRealtimeVideoInputDescriptor(media_output* p_spMediaOutput);
	~EMBeRealtimeVideoInputDescriptor();
	void ReadBuffer(EMMediaDataBuffer* p_opBuffer, int64 p_vForFrame);
	bool InitCheckE();
	bool PrepareToPlayE();
	void StartE();
	void StopE();
	bool StartPreviewE();
	bool StopPreviewE();
	bool ClearData();
	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool LoadData(EMProjectDataLoader* p_opLoader);
private:
	media_format_family mFormatFamily;
	int32 m_vTakeSequenceNumber;
	bool m_vRecording;
	EMBeDvConsumerNode* m_opNode;
};

#endif

#endif
