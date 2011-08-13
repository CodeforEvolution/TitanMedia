#include "EMGlobals.h"
#include "EMMediaGlobals.h"
#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_VIDEO_OUTPUT_PIN
#define __EM_WIN_VIDEO_OUTPUT_PIN

#include "EMWinMediaOutputPin.h"

#include <windows.h>

class EMMediaDataBuffer;
struct IMediaSample;

class EMWinVideoOutputPin : public EMWinMediaOutputPin
{
public:
	EMWinVideoOutputPin (HRESULT *phr, CDynamicSource* p_opParentFilter);
	~EMWinVideoOutputPin();

	bool ReportFormat(CMediaType* p_opFormat);

	uint32 ReportNumBuffers();
	uint32 ReportBufferSize();
	EMMediaType ReportFormatType();
	bool OnActive();
	bool OnDisconnect();
	VIDEOINFOHEADER* m_sVideoFormatStructure;
};

#endif

#endif d