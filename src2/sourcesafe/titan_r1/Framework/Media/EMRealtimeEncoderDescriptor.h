/*******************************************************
* Creator: Jesper Svensson
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#ifndef __EM_REALTIME_ENCODER_DESCRIPTOR
#define __EM_REALTIME_ENCODER_DESCRIPTOR

#include "EMEncoderDescriptor.h"

#include <list>

class EMMediaDataBuffer;

class EMRealtimeEncoderDescriptor : public EMEncoderDescriptor
{
public:
	virtual ~EMRealtimeEncoderDescriptor();
	virtual bool InitCheckE() = 0;
	virtual bool PrepareToPlayE() = 0;
	virtual bool StartPreviewE() = 0;
	virtual void StartE() = 0;
	virtual void StopE() = 0;
	virtual bool StopPreviewE() = 0;
	virtual bool ClearData() = 0;

protected:
	EMRealtimeEncoderDescriptor(EMMediaType p_eType);
	bool m_vIsInitialized;
};

#endif
