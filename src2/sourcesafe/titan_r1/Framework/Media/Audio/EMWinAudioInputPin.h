#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_AUDIO_INPUT_PIN
#define __EM_WIN_AUDIO_INPUT_PIN

#include "EMWinMediaInputPin.h"

#include <windows.h>

struct IMediaSample;

class EMMediaDataBuffer;
class EMWaveFileWriter;
class EMDirectXFilter;

class __declspec(dllexport) EMWinAudioInputPin : public EMWinMediaInputPin
{
public:
	EMWinAudioInputPin(HRESULT *phr, EMDirectXFilter* p_opParentFilter, string p_oName);
	~EMWinAudioInputPin();

	STDMETHODIMP GetAllocatorRequirements(ALLOCATOR_PROPERTIES *pProps);

	bool ReportFormat(CMediaType* p_opFormat);
	bool OnRun(uint64 p_vStartingSongTime);
	bool OnEndOfStream();
	bool BufferReceived(IMediaSample* p_opMediaSample);

private:
	CCritSec m_oLock;
	EMDirectXFilter* m_opParentFilter;
};

#endif

#endif