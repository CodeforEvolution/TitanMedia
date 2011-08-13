/*******************************************************
* Creator: Martin Johansson
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_AUDIO_EXTRACTOR
#define __EM_AUDIO_EXTRACTOR

#include "EMWinDXCallbackInterface.h"

#include <dshow.h>

class __declspec(dllexport) EMAudioExtractor : public EMCallback
{
public:
	~EMAudioExtractor();

	static EMAudioExtractor* Instance();
	static void Delete();

	bool ExtractAudio(const char* p_opSource, const char* p_opDestination);
	void CancelExtraction();

	void Callback(int64 p_vStart, int64 p_vStop);
	
protected:
	EMAudioExtractor();

	IGraphBuilder*	m_opIGraph;
	IMediaControl*	m_opICtrl;
	IMediaSeeking*	m_opISeek;
	IMediaEvent*	m_opIEvent;

	IBaseFilter*	m_opISource;
	IBaseFilter*	m_opIConvert;
	IBaseFilter*	m_opIWave;
	IBaseFilter*	m_opIZink;
	IBaseFilter*	m_opINull;
	IBaseFilter*	m_opISplit;

	int64 m_vDuration;

protected:
	bool BuildGraph(const char* p_opSource, const char* p_opDestination);
	void Clean();
	bool InitCallback(const char* p_opSource);

	bool GetDuration(const char* p_opSource);

	IPin* GetPin(IBaseFilter* p_opIFilter, PIN_DIRECTION p_oDir, bool p_vNotConnected = false);
	bool CheckForAudio(IPin* p_opIPin);
	bool Connect(IBaseFilter* p_opISource, IBaseFilter* p_opIDest);
	bool ExtractAudioFromQuickTimeFile(const char* p_opSource, const char* p_opDestination);
private:
	static EMAudioExtractor* m_opInstance;
	string m_oFileName;
};

#endif
