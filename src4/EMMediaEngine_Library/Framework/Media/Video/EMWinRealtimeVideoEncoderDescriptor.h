/*******************************************************
* Creator: Jesper Svensson
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_REALTIME_VIDEO_ENCODER_DESCRIPTOR
#define __EM_WIN_REALTIME_VIDEO_ENCODER_DESCRIPTOR

#include "EMRealtimeEncoderDescriptor.h"

//struct IBaseFilter;
#include <dshow.h>

class EMWinRealtimeVideoEncoderDescriptor : public EMRealtimeEncoderDescriptor
{
public:
	EMWinRealtimeVideoEncoderDescriptor(IBaseFilter* p_opSourceFilter);
	~EMWinRealtimeVideoEncoderDescriptor();
	bool InitCheckE();
	bool PrepareToPlayE();
	bool StartPreviewE();	
	void StartE();
	void StopE();
	bool StopPreviewE();
	bool ClearData();
	IBaseFilter* GetEncoderFilter();
	IPin* GetEncoderInPin();
	IPin* GetEncoderOutPin();
	HRESULT DisLinkAndRemoveEncoderFilter(IGraphBuilder* p_opGraphBuilder);
	HRESULT LinkAndCreateEncoderFilter(IPin* p_opSourcePin, IPin* p_opMuxPin, IGraphBuilder* p_opGraphBuilder);
	IPin* GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir);
private:
	IBaseFilter* m_opEncoderFilter;
	IPin* m_EncoderInPin;
	IPin* m_EncoderOutPin;

	IPin* m_opSourceOutPin;
	IPin* m_opMuxInPin;

};

#endif

#endif
