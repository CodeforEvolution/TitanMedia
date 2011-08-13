#include <windows.h>
#include <dshow.h>

#include "EMAudioExtractor.h"
#include "EMWinDXCallbackInterface.h"
#include "EMWinMediaUtility.h"
#include "EMMediaEngineUIDs.h"

#include "EMMediaEngine.h"
#include "EMCommandRepository.h"
#include "CommandIDs.h"

//Quicktime includes
#include "EMWinQuickTimeWriterSuper.h"


EMAudioExtractor* EMAudioExtractor::m_opInstance  = NULL;

EMAudioExtractor::EMAudioExtractor()
  :	m_opIGraph(NULL),
	m_opICtrl(NULL),
	m_opISeek(NULL),
	m_opIEvent(NULL),
	m_opISource(NULL),
	m_opIConvert(NULL),
	m_opIWave(NULL),
	m_opIZink(NULL),
	m_opINull(NULL),
	m_opISplit(NULL),
	m_oFileName("")
{
}

EMAudioExtractor::~EMAudioExtractor()
{
	Clean();
}

EMAudioExtractor* EMAudioExtractor::Instance()
{
	if(m_opInstance == NULL)
		m_opInstance = EM_new EMAudioExtractor();

	return m_opInstance;
}

void EMAudioExtractor::Delete()
{
	if(m_opInstance != NULL)
		delete m_opInstance;

	m_opInstance = NULL;
}


bool EMAudioExtractor::ExtractAudio(const char* p_opSource, const char* p_opDestination)
{
	m_oFileName = string(p_opSource);
	long evCode;
	HRESULT hr;

	if(BuildGraph(p_opSource, p_opDestination) == false)
	{
		Clean();
		//Maybe it is a quicktime file, try extracting it?
		if(EMWinQuickTimeWriterSuper::Instance() -> ExtractAudioFromQuickTimeFile(p_opSource, p_opDestination) == false)
			return false;
		return true;
	}

	if(InitCallback(p_opSource) == false)
	{
		Clean();
		return false;
	}

	eo << "Bringing up dialog..." << ef;
	EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_DISPLAY_AUDIO_EXTRACTION_CONVERSION_DIALOG, "Extracting and converting audio...", const_cast<char*>(string(string("") + string(m_oFileName.c_str()) + string("...")).c_str()), NULL);
	eo << "Bringing up dialog...OK!" << ef;


	hr = m_opICtrl -> Run();
	if(hr < S_OK)
	{
		Clean();
		return false;
	}

	if(m_opIEvent -> WaitForCompletion(INFINITE, &evCode) != S_OK)
	{
		Clean();
		return false;
	}

	m_opICtrl -> Stop();

	EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_CLOSE_AUDIO_EXTRACTION_CONVERSION_DIALOG);
	eo << " *** Dialog Closed" << ef;

	Clean();

	return true;
}


void EMAudioExtractor::CancelExtraction()
{
	if(m_opICtrl != NULL)
		m_opICtrl -> Stop();
}

bool EMAudioExtractor::InitCallback(const char* p_opSource)
{
	HRESULT hr;
	uint32 vCaps;

	EMWinDXCallbackInterface* opICallback = NULL;

	m_opIConvert -> QueryInterface(IID_IEMWinDXCallbackInterface, (void**)&opICallback);
	opICallback -> SetCallback(this);
	opICallback -> Release();
	opICallback = NULL;

	hr = m_opISeek -> SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);
	
	if(hr != S_OK)
		return false;

	m_vDuration = 0;

	m_opISeek -> GetCapabilities(&vCaps);

	if(vCaps & AM_SEEKING_CanGetStopPos)
	{
		m_opISeek -> GetStopPosition(&m_vDuration);
	}

	if(vCaps & AM_SEEKING_CanGetDuration && m_vDuration == 0)
	{
		m_opISeek -> GetDuration(&m_vDuration);
	}

	if(m_vDuration == 0 && !GetDuration(p_opSource))
	{
		return false;
	}

	return true;
}

bool EMAudioExtractor::GetDuration(const char* p_opSource)
{
	HRESULT hr;
	int64 vDummy = 0;

	unsigned short* vpName = dynamic_cast<EMWinMediaUtility*>(EMMediaUtility::Instance()) -> StringToUnicode(p_opSource);

	IGraphBuilder* opIGraph = NULL;
	IMediaSeeking* opISeek = NULL;

	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void**)&opIGraph);

	hr = opIGraph -> RenderFile(vpName, NULL);

	hr = opIGraph -> QueryInterface(IID_IMediaSeeking, (void**) &opISeek);

	hr = opISeek -> SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);
	
	opISeek -> GetDuration(&m_vDuration);
	
	opISeek -> Release();
	opIGraph ->Release();

	delete vpName;
	
/*	IPin* opIPin = GetPin(m_opISource, PINDIR_OUTPUT);

	IAsyncReader* opIAsync = NULL;

	hr = opIPin -> QueryInterface(IID_IAsyncReader, (void**) &opIAsync);

	hr = opIAsync -> Length(&m_vDuration, &vDummy);

	opIAsync -> Release();
	opIPin -> Release();*/

	return hr == S_OK;
}


bool EMAudioExtractor::BuildGraph(const char* p_opSource, const char* p_opDestination)
{
	HRESULT hr;

	Clean();

	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void**) &m_opIGraph);
    hr = m_opIGraph -> QueryInterface(IID_IMediaEvent, (void**) &m_opIEvent);
	hr = m_opIGraph -> QueryInterface(IID_IMediaControl, (void**) &m_opICtrl);
	hr = m_opIGraph -> QueryInterface(IID_IMediaSeeking, (void**) &m_opISeek);

	unsigned short* vpName = dynamic_cast<EMWinMediaUtility*>(EMMediaUtility::Instance()) -> StringToUnicode(p_opSource);

	hr = m_opIGraph -> AddSourceFilter(vpName, L"Source", &m_opISource);
	delete vpName;
	if(hr != S_OK)
		return false;

	hr = CoCreateInstance(CLSID_EMWinAudioConverterFilter, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void**)&m_opIConvert);
	if(hr != S_OK)
		return false;

	hr = CoCreateInstance(CLSID_EMWaveWriter, NULL, CLSCTX_INPROC, IID_IBaseFilter, (LPVOID*) &m_opIWave);
	if(hr != S_OK)
		return false;

	hr = CoCreateInstance(CLSID_FileWriter, NULL, CLSCTX_INPROC, IID_IBaseFilter, (LPVOID*) &m_opIZink);
	if(hr != S_OK)
		return false;

//	hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC, IID_IBaseFilter, (LPVOID*) &m_opINull);
//	if(hr != S_OK)
//		return false;


	hr = m_opIGraph -> AddFilter(m_opIConvert, L"Convert");
	hr |= m_opIGraph -> AddFilter(m_opIWave, L"Wave");
	hr |= m_opIGraph -> AddFilter(m_opIZink, L"Zink");
//	hr |= m_opIGraph -> AddFilter(m_opINull, L"Null");

	if(hr != S_OK)
		return false;

	IFileSinkFilter* opIFile = NULL;
	hr = m_opIZink -> QueryInterface(IID_IFileSinkFilter, (void **)&opIFile);
	if(hr != S_OK)
		return false;

	vpName = ((EMWinMediaUtility*) EMMediaUtility::Instance()) -> StringToUnicode(p_opDestination);
	hr = opIFile -> SetFileName(vpName, NULL);
	opIFile -> Release();
	opIFile = NULL;

	if(hr != S_OK)
		return false;

	if(!Connect(m_opISource, m_opIConvert))
		return false;

	if(!Connect(m_opIConvert, m_opIWave))
		return false;

	if(!Connect(m_opIWave, m_opIZink))
		return false;

	
/*	PIN_INFO oInfo;
	IPin* opITmp = GetPin(m_opISource, PINDIR_OUTPUT);
	IPin* opIIn = NULL;
	hr = opITmp -> ConnectedTo(&opIIn);
	hr = opIIn -> QueryPinInfo(&oInfo);

	opITmp -> Release();
	opIIn -> Release();

	m_opISplit = oInfo.pFilter;

	Connect(m_opISplit, m_opINull);
*/
	return true;
}

bool EMAudioExtractor::CheckForAudio(IPin* p_opIPin)
{
	IEnumMediaTypes* opIEnum = NULL;
	AM_MEDIA_TYPE* opType = NULL;
	unsigned long vDummy;
	bool vHasAudio = false;

	p_opIPin -> EnumMediaTypes(&opIEnum);
	opIEnum -> Reset();
	while(opIEnum -> Next(1, &opType, &vDummy) == S_OK && !vHasAudio)
	{
		vHasAudio = vHasAudio || (opType -> majortype == MEDIATYPE_Audio);
		DeleteMediaType(opType);
	}

	opIEnum -> Release();

	return vHasAudio;
}


IPin* EMAudioExtractor::GetPin(IBaseFilter* p_opIFilter, PIN_DIRECTION p_oDir, bool p_vNotConnected)
{
   IEnumPins*	opIEnum;
   IPin*		opIPin;
   IPin*		opIIn;

    p_opIFilter -> EnumPins(&opIEnum);

    while(opIEnum -> Next(1, &opIPin, 0) == S_OK)
    {
        PIN_DIRECTION PinDirThis;

        opIPin -> QueryDirection(&PinDirThis);		
		opIPin -> ConnectedTo(&opIIn);

		if(p_oDir == PinDirThis  && !(opIIn != NULL && p_vNotConnected))
		{
            break;
		}

		if(opIIn != NULL)
		{
			opIIn -> Release();
			opIIn = NULL;
		}
        
		opIPin -> Release();
		opIPin = NULL;
    }

	if(opIIn != NULL)
	{
		opIIn -> Release();
		opIIn = NULL;
	}

    opIEnum -> Release();
	opIEnum = NULL;

    return opIPin;  
}

bool EMAudioExtractor::Connect(IBaseFilter* p_opISource, IBaseFilter* p_opIDest)
{
	HRESULT hr;
	IPin* opIOut = GetPin(p_opISource, PINDIR_OUTPUT, true);
	if(opIOut == NULL)
		return false;

	IPin* opIIn = GetPin(p_opIDest, PINDIR_INPUT, true);
	if(opIIn != NULL)
	{
		hr = m_opIGraph -> Connect(opIOut, opIIn);
		opIOut -> Release();
		opIIn -> Release();

		return hr == S_OK || hr == VFW_S_PARTIAL_RENDER;
	}

	opIOut -> Release();
	return false;
}

void EMAudioExtractor::Callback(int64 p_vStart, int64 p_vStop)
{
	int64 vStart = 226 * p_vStart;
	int64 vStop = 226 * p_vStop;

//	eo << "EMAudioExtractor::Callback(" << vStart << ", " << vStop <<"); | " << m_vDuration << ef;
	
	float vFloatVal = (static_cast<float>(vStop ) / static_cast<float>(m_vDuration)) * 1000.0;
	uint16 vVal = static_cast<uint16>(vFloatVal);
	EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_SET_VALUE_IN_AUDIO_EXTRACTION_CONVERSION_DIALOG, &vVal, NULL, NULL);
} 


void EMAudioExtractor::Clean()
{
	if(m_opICtrl != NULL)
	{
		m_opICtrl -> Release();
		m_opICtrl = NULL;
	}

	if(m_opISeek != NULL)
	{
		m_opISeek -> Release();
		m_opISeek = NULL;
	}

	if(m_opIEvent != NULL)
	{
		m_opIEvent -> Release();
		m_opIEvent = NULL;
	}

	if(m_opISource != NULL)
	{
		m_opISource -> Release();
		m_opISource = NULL;
	}

	if(m_opIConvert != NULL)
	{
		m_opIConvert -> Release();
		m_opIConvert = NULL;
	}

	if(m_opIWave != NULL)
	{
		m_opIWave -> Release();
		m_opIWave = NULL;
	}

	if(m_opIZink != NULL)
	{
		m_opIZink -> Release();
		m_opIZink = NULL;
	}

	if(m_opINull != NULL)
	{
		m_opINull -> Release();
		m_opINull = NULL;
	}

	if(m_opISplit != NULL)
	{
		m_opISplit -> Release();
		m_opISplit = NULL;
	}

	if(m_opIGraph != NULL)
	{
		m_opIGraph -> Release();
		m_opIGraph = NULL;
	}
}


