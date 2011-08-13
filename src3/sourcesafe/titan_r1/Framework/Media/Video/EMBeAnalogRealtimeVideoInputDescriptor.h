/*******************************************************
* Creator: Richard Kronfalt
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_ANALOG_REALTIME_VIDEO_INPUT_DESCRIPTOR
#define __EM_BE_ANALOG_REALTIME_VIDEO_INPUT_DESCRIPTOR

#include "EMBeRealtimeInputDescriptor.h"
#include "EMBeAnalogConsumerNode.h"

class EMMediaDataBuffer;
class EMProjectDataSaver;
class EMProjectDataLoader;

class EMBeAnalogRealtimeVideoInputDescriptor : public EMBeRealtimeInputDescriptor
{
public:
	EMBeAnalogRealtimeVideoInputDescriptor(media_output* p_spMediaOutput);
	~EMBeAnalogRealtimeVideoInputDescriptor();
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
	bool ResetAnalogConnectionAndStart();
private:
	media_format_family mFormatFamily;
};

#endif

#endif
