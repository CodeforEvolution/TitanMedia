#include "EMWinAudioDiskWriter.h"
#include "EMWinMediaUtility.h"
#include "EMAudioMixFunctions.h"

EMWinAudioDiskWriter* EMWinAudioDiskWriter::m_opInstance = NULL;



EMWinAudioDiskWriter::EMWinAudioDiskWriter()
  : m_oName("d:\\dummy.wav"),
	m_vIsRunning(false),
	m_opIGraph(NULL),
	m_opICtrl(NULL),
	m_opBaseConverter(NULL),
	m_opBaseWave(NULL),
	m_opBaseShuffle(NULL),
	m_opShuffleOutput(NULL),
	m_opConverterInput(NULL),
	m_opConverterOutput(NULL),
	m_opWaveInput(NULL),
	m_opWaveOutput(NULL),
	m_opWriterInput(NULL),
	m_opIBaseWriter(NULL),
	m_opIFileSink(NULL),
	m_opIShuffle(NULL),
	m_opConverter(NULL)

{
	HRESULT hr;

	hr = CoCreateInstance(CLSID_EMWinAudioConverterFilter, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void**)&m_opBaseConverter);
	if(FAILED(hr))
	{
		EMDebugger("Error: Couldn't find audio converter!!");
	}

	hr = m_opBaseConverter -> QueryInterface(IID_IEMWinAudioConverterInterface, (void**)&m_opConverter);
	if(FAILED(hr))
	{
		EMDebugger("Error: Couldn't find audio converter!!");
	}

	hr = CoCreateInstance(CLSID_EMWaveWriter, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void**)&m_opBaseWave);
	if(FAILED(hr))
	{
		EMDebugger("Error: Couldn't find wave destination!!");
	}
	
	hr = CoCreateInstance(CLSID_EMWinVideoShuffleOutputFilter, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void**)&m_opBaseShuffle);
	if(FAILED(hr))
	{
		EMDebugger("Error: Couldn't find graph writer!!");
	}
	
	hr = m_opBaseShuffle -> QueryInterface(IID_IEMWinVideoShuffleOutputInterface, (void**)&m_opIShuffle);
	if(FAILED(hr))
	{
		EMDebugger("Error: Couldn't find graph writer!!");
	}

	hr = CoCreateInstance(CLSID_FileWriter, NULL, CLSCTX_INPROC, IID_IBaseFilter, (LPVOID*) &m_opIBaseWriter);
	if(FAILED(hr))
	{
		EMDebugger("ERROR! No file writer!");
	}

	hr = m_opIBaseWriter -> QueryInterface(IID_IFileSinkFilter, (void **)&m_opIFileSink);
	if(FAILED(hr))
	{
		EMDebugger("ERROR! Failed to retrieve interface of FileSinkFilter!");
	}

	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void**) &m_opIGraph);
	if(FAILED(hr))
	{
		EMDebugger("ERROR! No graph!");
	}

	hr = m_opIGraph -> QueryInterface(IID_IMediaControl, (void**)&m_opICtrl);
	if(FAILED(hr))
	{
		EMDebugger("ERROR! No graph!");
	}

	hr = m_opIGraph -> AddFilter(m_opBaseShuffle, L"Shuffle");
	
	m_opShuffleOutput = GetPin(m_opBaseShuffle, PINDIR_OUTPUT);

	hr = m_opIGraph -> AddFilter(m_opBaseConverter, L"Convert");

	m_opConverterOutput = GetPin(m_opBaseConverter, PINDIR_OUTPUT);
	m_opConverterInput = GetPin(m_opBaseConverter, PINDIR_INPUT);
	
	hr = m_opIGraph -> AddFilter(m_opBaseWave, L"WaveDest");

	m_opWaveInput = GetPin(m_opBaseWave, PINDIR_INPUT);
	m_opWaveOutput = GetPin(m_opBaseWave, PINDIR_OUTPUT);
	
	hr = m_opIGraph -> AddFilter(m_opIBaseWriter, L"Sink");

	m_opWriterInput = GetPin(m_opIBaseWriter, PINDIR_INPUT);

}

EMWinAudioDiskWriter::~EMWinAudioDiskWriter()
{
	int vCnt; 
	HRESULT hr;

	if(m_vIsRunning)
		Stop();

	if(m_opICtrl != NULL)
	{
		hr = m_opICtrl -> Stop();
		OAFilterState uState;
		while(m_opICtrl -> GetState(50, &uState) == VFW_S_STATE_INTERMEDIATE)
			::Sleep(5);
	}

	if(m_opShuffleOutput != NULL)
	{
		hr = m_opShuffleOutput -> Disconnect();
		vCnt = m_opShuffleOutput -> Release();
		m_opShuffleOutput = NULL;
	}

	if(m_opConverterOutput != NULL)
	{
		hr = m_opConverterOutput -> Disconnect();
		vCnt = m_opConverterOutput -> Release();
		m_opConverterOutput = NULL;
	}

	if(m_opConverterInput != NULL)
	{
		hr = m_opConverterInput -> Disconnect();
		vCnt = m_opConverterInput -> Release();
		m_opConverterInput = NULL;
	}

	if(m_opWaveInput != NULL)
	{
		hr = m_opWaveInput -> Disconnect();
		vCnt = m_opWaveInput -> Release();
		m_opWaveInput = NULL;
	}

	if(m_opWaveOutput != NULL)
	{
		hr = m_opWaveOutput -> Disconnect();
		vCnt = m_opWaveOutput -> Release();
		m_opWaveOutput = NULL;
	}

	if(m_opWriterInput != NULL)
	{
		hr = m_opWriterInput -> Disconnect();
		vCnt = m_opWriterInput -> Release();
		m_opWriterInput = NULL;
	}

	if(m_opIShuffle != NULL)
	{
		vCnt = m_opIShuffle -> Release();
		m_opIShuffle = NULL;
	}

	if(m_opConverter != NULL)
	{	
		vCnt = m_opConverter -> Release();
		m_opConverter = NULL;
	}

	if(m_opBaseConverter != NULL)
	{
		hr = m_opIGraph -> RemoveFilter(m_opBaseConverter);
		vCnt = m_opBaseConverter -> Release();
		m_opBaseConverter = NULL;
	}

	if(m_opBaseWave != NULL)
	{
		hr = m_opIGraph -> RemoveFilter(m_opBaseWave);
		vCnt = m_opBaseWave -> Release();
		m_opBaseWave = NULL;
	}

	if(m_opBaseShuffle != NULL)
	{
		hr = m_opIGraph -> RemoveFilter(m_opBaseShuffle);		
		vCnt = m_opBaseShuffle -> Release();
		m_opBaseShuffle = NULL;
	}

	if(m_opIFileSink != NULL)
	{
		vCnt = m_opIFileSink -> Release();
		m_opIFileSink = NULL;
	}

	if(m_opIBaseWriter != NULL)
	{
		hr = m_opIGraph -> RemoveFilter(m_opIBaseWriter);	
		vCnt = m_opIBaseWriter -> Release();
		m_opIBaseWriter = NULL;
	}

	if(m_opICtrl != NULL)
	{
		vCnt = m_opICtrl -> Release();
		m_opICtrl = NULL;
	}

	if(m_opIGraph != NULL)
	{
		vCnt = m_opIGraph -> Release();
		m_opIGraph = NULL;
	}
}

IPin* EMWinAudioDiskWriter::GetPin(IBaseFilter* p_opFilter, PIN_DIRECTION p_oPinDir)
{
	int vCnt;
	IPin* opFirst = NULL;

	IEnumPins* opPinEnumerator = NULL;
	if(p_opFilter -> EnumPins(&opPinEnumerator) == S_OK)
	{
		IPin* opPin = NULL;
		opPinEnumerator -> Reset();
		while(opPinEnumerator -> Next(1, &opPin, NULL) == S_OK && opFirst == NULL)
		{
			PIN_DIRECTION sPinDir;
			if(opPin -> QueryDirection(&sPinDir) == S_OK)
			{
				if(sPinDir == p_oPinDir)
					opFirst = opPin;
			}

			if(opFirst == NULL)
			{
				vCnt = opPin -> Release();
				opPin = NULL;
			}
		}

		vCnt = opPinEnumerator -> Release();
		opPinEnumerator = NULL;
	}

	return opFirst;
}


EMWinAudioDiskWriter* EMWinAudioDiskWriter::Instance()
{	
	if(m_opInstance == NULL)
		m_opInstance = EM_new EMWinAudioDiskWriter();

	return m_opInstance;
}


void EMWinAudioDiskWriter::Delete()
{
	if(m_opInstance != NULL)
		delete m_opInstance;

	m_opInstance = NULL;
}

bool EMWinAudioDiskWriter::IsRunning()
{
	return m_vIsRunning;
}


bool EMWinAudioDiskWriter::InitConverter(int p_vSampleRate, int p_vBits, bool p_vIsMono)
{
	if(m_opConverter != NULL && !m_vIsRunning)
		return S_OK == m_opConverter -> SetSampleParams(p_vSampleRate, p_vBits, p_vIsMono);

	return false;
}

bool EMWinAudioDiskWriter::SetRenderName(char* p_opFileName)
{
	m_oName = p_opFileName;

	return true;
}


bool EMWinAudioDiskWriter::Start()
{
	HRESULT hr;
	unsigned short* vpOutput = ((EMWinMediaUtility*) EMMediaUtility::Instance()) -> StringToUnicode(m_oName.c_str());

	hr = m_opIFileSink -> SetFileName(vpOutput, NULL);
	if(FAILED(hr))
	{
		EMDebugger("ERROR! Couldn't open file...");
	}

	EMMediaFormat oFormat(EM_TYPE_RAW_AUDIO);
	m_opIShuffle -> SetShuffleFormat(&oFormat);

	hr = m_opIGraph -> ConnectDirect(m_opShuffleOutput, m_opConverterInput, NULL);
	hr = m_opIGraph -> ConnectDirect(m_opConverterOutput, m_opWaveInput, NULL);
	hr = m_opIGraph -> ConnectDirect(m_opWaveOutput, m_opWriterInput, NULL);

	m_vIsRunning = true;

	delete [] vpOutput;

	hr = m_opICtrl -> Run();

	return S_OK == hr;
}



bool EMWinAudioDiskWriter::Stop()
{
	HRESULT hr;

	if(m_vIsRunning)
	{
		hr = m_opICtrl -> Stop();

		hr = m_opIGraph -> Disconnect(m_opShuffleOutput);
		hr = m_opIGraph -> Disconnect(m_opConverterInput);
		hr = m_opIGraph -> Disconnect(m_opConverterOutput);
		hr = m_opIGraph -> Disconnect(m_opWaveInput);
		hr = m_opIGraph -> Disconnect(m_opWaveOutput);
		hr = m_opIGraph -> Disconnect(m_opWriterInput);

		//hr = m_opIFileSink -> SetFileName("", NULL);

		m_vIsRunning = false;
	}

	return true;
}

EMMediaDataBuffer* EMWinAudioDiskWriter::ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList)
{
	EMMediaDataBuffer* opBuffer = EMAudioMixFunctions::AudioMixPostFader(p_opBufferList);

	try
	{
		m_opIShuffle -> ProcessBuffer(opBuffer);	
	}
	catch(...)
	{
	}

	return opBuffer;
}