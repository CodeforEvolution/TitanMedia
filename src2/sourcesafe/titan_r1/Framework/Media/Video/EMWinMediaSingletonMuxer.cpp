#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS
#include "EMWinMediaSingletonMuxer.h"
#include "EMWinMediaUtility.h"
#include "EMMediaUtility.h"
#include "EMEncoderRepository.h"
#include "EMWinRealtimeVideoEncoderDescriptor.h"
#include "EMWinRealtimeAudioEncoderDescriptor.h"
#include "EMWinRealtimeRenderToDiskOutputDescriptor.h"
#include "EMMediaFormat.h"

DEFINE_GUID(CLSID_EMWinVideoShuffleOutputFilter, 0x3fc76daf, 0x80e8, 0x11d5, 0x82, 0x59, 0x0, 0x2, 0xb3, 0x18, 0x0, 0x46);
DEFINE_GUID(IID_IEMWinVideoShuffleOutputInterface, 0x3fc76dae, 0x80e8, 0x11d5, 0x82, 0x59, 0x0, 0x2, 0xb3, 0x18, 0x0, 0x46);

EMWinMediaSingletonMuxer* EMWinMediaSingletonMuxer::m_opInstance = NULL;

EMWinMediaSingletonMuxer::EMWinMediaSingletonMuxer()
{
	eo << "EMWinMediaSingletonMuxer::EMWinMediaSingletonMuxer() called by " << (uint32) GetCurrentThreadId() << ef;
	//Initialize some attributes
	m_opGraphBuilder = NULL;
	m_opAviMuxFilter = NULL;
	m_opMuxInPin = NULL;
	m_vVideoIsReady = false;
	m_vAudioIsReady = false;
	m_vpEncoderState = NULL;
	m_opVideoRenderObject = NULL;
	m_opAudioRenderObject = NULL;
	m_opAudioCompressionCapabilityArray = NULL;
	m_opVideoCompressionCapabilityArray = NULL;
	m_vCompressionCapabilityEncoder = -1;
	m_vCompressionCapabilityID = -1;
	m_opFileWriterFilter = NULL;
	m_opEncoderDummyInPin = NULL;
	m_opEncoderDummyOutPin = NULL;
	m_opShuffleDummyFilterOutPin = NULL;
	m_opEncoderDummyFilter = NULL;
	m_opShuffleDummyFilterInterface = NULL;
	m_vErrorStatus = false;
	m_vCurrentlyUsedFamily = EM_FAMILY_QUICKTIME;

}

EMWinMediaSingletonMuxer::~EMWinMediaSingletonMuxer()
{
	if(m_opInstance != NULL)
	{
		if(m_opMuxInPin != NULL)
		{
			m_opMuxInPin -> Release();
			m_opMuxInPin = NULL;
		}

		if(m_opAviMuxFilter != NULL)
		{
			m_opAviMuxFilter -> Release();
			m_opAviMuxFilter = NULL;
		}

		//Removed because of TitanTV(It Crashes here!!)
/*		if(m_opGraphBuilder != NULL)
		{
			m_opGraphBuilder -> Release();
			m_opGraphBuilder = NULL;
		}
*/
		m_opInstance = NULL;

		if(m_vShuffleDummyFilter != NULL)
		{
			m_vShuffleDummyFilter -> Release();
			m_vShuffleDummyFilter = NULL;
		}

		if(m_opDummyGraphBuilder != NULL)
		{
			m_opDummyGraphBuilder -> Release();
			m_opDummyGraphBuilder = NULL;
		}

		if(m_opShuffleDummyFilterInterface != NULL)
		{
			m_opShuffleDummyFilterInterface -> Release();
			m_opShuffleDummyFilterInterface = NULL;
		}

		if(m_opEncoderDummyOutPin != NULL)
		{
			m_opEncoderDummyOutPin -> Release();
			m_opEncoderDummyOutPin = NULL;
		}

		if(m_opFileWriterFilter != NULL)
		{
			m_opFileWriterFilter -> Release();
			m_opFileWriterFilter = NULL;
		}

		if(m_opFileZinkInterface != NULL)
		{
			m_opFileZinkInterface -> Release();
			m_opFileZinkInterface = NULL;
		}
	}

	if(m_opAudioCompressionCapabilityArray != NULL)
	{
		delete [] m_opAudioCompressionCapabilityArray;
	}
}

EMWinMediaSingletonMuxer* EMWinMediaSingletonMuxer::Instance()
{
	if(m_opInstance != NULL)
		return m_opInstance;

	if(m_opInstance == NULL)
		m_opInstance = EM_new EMWinMediaSingletonMuxer();

	EMWinMediaSingletonMuxer* opInstance = m_opInstance;
	//Instansiate a Filter Graph to use (this is done ONCE)
	m_opInstance -> CreateFilterGraph();
	//Instansiate the MuxFilter and the Write to disk filter (this is done ONCE)
	m_opInstance -> CreateFileRendererFilter();

	m_opInstance -> CreateDummyFilterGraph();
	m_opInstance -> m_vErrorString = "--- Not Available ---";
	return opInstance;
}

void EMWinMediaSingletonMuxer::CreateDummyFilterGraph()
{

	// Create a dummy graph to be used for checking encoder properties
	//CoCreate Elegant Medias Producer Filter For Write to Disk

	HRESULT h = S_OK;
	//CoCreate a Filter Graph
	h = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IFilterGraph, (void**) &(m_opDummyGraphBuilder));
	if(FAILED(h))
	{
		EMDebugger("ERROR! Failed to create graph in EMWinMediaSingletonMuxer!");
	}

	h = CoCreateInstance(CLSID_EMWinVideoShuffleOutputFilter, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void**) &(m_vShuffleDummyFilter));
	if(FAILED(h))
	{
		EMDebugger("ERROR! Failed to retrieve interface of own output filter!");
	}

	//Get the Interface of Elegant Medias Producer Filter For Write
	h = m_vShuffleDummyFilter -> QueryInterface(IID_IEMWinVideoShuffleOutputInterface, (void**) &(m_opShuffleDummyFilterInterface));
	if(FAILED(h))
	{
		EMDebugger("ERROR! Failed to retrieve interface of own output filter!");
	}

	//Add this Filter to the Filter Graph
	h = m_opDummyGraphBuilder -> AddFilter(m_vShuffleDummyFilter, L"Elegant Media Producer Filter For Encoder Properties");
	if(FAILED(h))
	{
		EMDebugger("ERROR! Failed to add own producer filter to filter graph!");
	}	
	//End creating it

}

void EMWinMediaSingletonMuxer::TempDisLinkSourceWithEncoder()
{
	HRESULT hr;
	//Disconnect the Encoder In Pin
	if(m_opEncoderDummyInPin != NULL)
	{
		hr = m_opDummyGraphBuilder -> Disconnect(m_opEncoderDummyInPin);
		hr = m_opEncoderDummyInPin -> Release();
		m_opEncoderDummyInPin = NULL;
	}

	//Disconnect the Source Filter Out Pin
	if(m_opShuffleDummyFilterOutPin != NULL)
	{
		hr = m_opDummyGraphBuilder -> Disconnect(m_opShuffleDummyFilterOutPin);
		hr = m_opShuffleDummyFilterOutPin -> Release();
		m_opShuffleDummyFilterOutPin = NULL;
	}
	//If the filter is not present the Filter Graph will just ignore the request
	if(m_opEncoderDummyFilter != NULL)
		hr = m_opDummyGraphBuilder -> RemoveFilter(m_opEncoderDummyFilter);

}

//Used in order for the GetAudioCompressionCapabilities() & GetAudioCompressionCapabilities()
//functions to fetch the right capabilities for the rendering "working format used"
bool EMWinMediaSingletonMuxer::TempLinkSourceWithEncoder(int32 p_vEncoderID)
{
	EMMediaFormat* opMediaFormat = NULL;

	EMRealtimeEncoderDescriptor* opRealtimeEncoderDescriptor = GetEncoderDescriptor(p_vEncoderID);

	if((opRealtimeEncoderDescriptor->GetType() & EM_TYPE_ANY_VIDEO) > 0)
	{

		EMWinRealtimeVideoEncoderDescriptor* opEWRVED = dynamic_cast<EMWinRealtimeVideoEncoderDescriptor*>(opRealtimeEncoderDescriptor);
		m_opEncoderDummyFilter = opEWRVED -> GetEncoderFilter();
		opMediaFormat = new EMMediaFormat(EM_TYPE_ANY_VIDEO);
	} else if((opRealtimeEncoderDescriptor -> GetType() & EM_TYPE_ANY_AUDIO) > 0)
	{
		EMWinRealtimeAudioEncoderDescriptor* opEWRAED = dynamic_cast<EMWinRealtimeAudioEncoderDescriptor*>(opRealtimeEncoderDescriptor);
		m_opEncoderDummyFilter = opEWRAED -> GetEncoderFilter();
		opMediaFormat = new EMMediaFormat(EM_TYPE_ANY_AUDIO);
	}
	m_vActiveDummyType = opRealtimeEncoderDescriptor->GetType();

	//Set the source filter shuffle format
	m_opShuffleDummyFilterInterface -> SetShuffleFormat(opMediaFormat);
	//Destroy object imediately after having used it
	delete opMediaFormat;

	//Add the encoder filter to the Filter Graph
	HRESULT h = m_opDummyGraphBuilder -> AddFilter(m_opEncoderDummyFilter, L"Encoder filter");
	if(FAILED(h))
	{
		if(h == 0x80040295)
			MessageBox(NULL, "This encoder is locked at the moment. Choose another encoder.", "Message", MB_OK);
		return false;
		EMDebugger("ERROR! Failed to add encoder filter to filter graph!");
	}

	m_opEncoderDummyInPin = GetPin(m_opEncoderDummyFilter, PINDIR_INPUT);
	m_opEncoderDummyOutPin = GetPin(m_opEncoderDummyFilter, PINDIR_OUTPUT);

	m_opShuffleDummyFilterOutPin = GetPin(m_vShuffleDummyFilter, PINDIR_OUTPUT);

	//Connect the source with the encoder
	h = m_opDummyGraphBuilder -> ConnectDirect(m_opShuffleDummyFilterOutPin , m_opEncoderDummyInPin, NULL);
	if(h != S_OK)
	{
		m_opEncoderDummyInPin = NULL;
		return false;
	}

	return true;

}


void EMWinMediaSingletonMuxer::CreateFilterGraph()
{
	HRESULT h = S_OK;
	//CoCreate a Filter Graph
	h = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IFilterGraph, (void**) &(m_opGraphBuilder));
	if(FAILED(h))
		MessageBox(NULL, "ERROR! A critical error has occured in the singleton muxer! Could not create DirectX filter graph!", "Error!", MB_OK);

    // Keep a useless clock from being instantiated....
    IMediaFilter *graphF;
    h = m_opGraphBuilder->QueryInterface(IID_IMediaFilter, (void **) &graphF);
    if(SUCCEEDED(h))
    {
        h = graphF->SetSyncSource(NULL);
        graphF->Release();
	}
}
void EMWinMediaSingletonMuxer::SetRenderName(char* p_vpName)
{
	m_oOutputFileName = p_vpName;
	if(m_vCurrentlyUsedFamily == EM_FAMILY_AVI)
	{
		WCHAR *vpFileName = ((EMWinMediaUtility*) EMMediaUtility::Instance()) -> StringToUnicode(m_oOutputFileName.c_str());
		HRESULT hr = DeleteFile(m_oOutputFileName.c_str());
		hr = m_opFileZinkInterface -> SetFileName(vpFileName, NULL);
	}
}

string* EMWinMediaSingletonMuxer::GetRenderName()
{
	return &m_oOutputFileName;
}

void EMWinMediaSingletonMuxer::SetVideoIsReady(bool p_vReadyState)
{
	m_vVideoIsReady = p_vReadyState;
}

void EMWinMediaSingletonMuxer::SetAudioIsReady(bool p_vReadyState)
{
	m_vAudioIsReady = p_vReadyState;
}

bool EMWinMediaSingletonMuxer::AudioIsReady()
{
	return m_vAudioIsReady;
}

bool EMWinMediaSingletonMuxer::VideoIsReady()
{
	return m_vVideoIsReady;
}

bool EMWinMediaSingletonMuxer::AllIsReady()
{
	if(m_vVideoIsReady && m_vAudioIsReady)
		return true;
	else
		return false;
}

void EMWinMediaSingletonMuxer::StoreVideoRealTimeRenderToDiskObject(EMWinRealtimeRenderToDiskOutputDescriptor* m_opObject)
{
	m_opVideoRenderObject = m_opObject;
}

void EMWinMediaSingletonMuxer::StoreAudioRealTimeRenderToDiskObject(EMWinRealtimeRenderToDiskOutputDescriptor* m_opObject)
{
	m_opAudioRenderObject = m_opObject;
}

EMWinRealtimeRenderToDiskOutputDescriptor* EMWinMediaSingletonMuxer::GetVideoRealTimeRenderToDiskObject()
{
	return m_opVideoRenderObject;
}

EMWinRealtimeRenderToDiskOutputDescriptor* EMWinMediaSingletonMuxer::GetAudioRealTimeRenderToDiskObject()
{
	return m_opAudioRenderObject;
}


IGraphBuilder* EMWinMediaSingletonMuxer::GetFilterGraph()
{
	if(m_opGraphBuilder == NULL)
	{
		EMDebugger("ERROR! No Filter Graph implemented in EMWinMediaSingletonMuxer");
	}
	else
	{
		m_opGraphBuilder -> AddRef();
		return m_opGraphBuilder;
	}

	return NULL;
}

IPin* EMWinMediaSingletonMuxer::GetAMuxInPin()
{
	//Get the In Pin for the Mux Filter
	m_opMuxInPin = GetPin(m_opAviMuxFilter, PINDIR_INPUT);
	return m_opMuxInPin;

}

//Find the right codec descriptor by looking for it according to its ID
EMRealtimeEncoderDescriptor* EMWinMediaSingletonMuxer::GetEncoderDescriptor(int32 p_vEncoderID)
{
	EMEncoderRepository* opOutputs = EMEncoderRepository::Instance();
	try
	{
		opOutputs -> LockContainer();
		opOutputs -> Rewind();
		while(opOutputs -> Current() != NULL)
		{
			int32 vOutputID = opOutputs -> Current() -> GetID();
			if(vOutputID == p_vEncoderID || p_vEncoderID == -1) //DIVX FAST MOTION , just temporarily set to 48
			{
				EMRealtimeEncoderDescriptor* opRealtimeEncoderDescriptor = opOutputs -> Find(vOutputID);
				opOutputs -> UnlockContainer();

				return opRealtimeEncoderDescriptor;
			}
			opOutputs -> Next();
		}
		opOutputs -> UnlockContainer();
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetAudioRenderOutput::ExecuteE");
	}

	EMDebugger("ERROR! No such encoder found!");
	
	return NULL;

}

bool EMWinMediaSingletonMuxer::GetEncoderPropertyPages(int32 p_vEncoderID, bool p_vShow)
{
	IBaseFilter *pFilter;
	EMRealtimeEncoderDescriptor* opRealtimeEncoderDescriptor = GetEncoderDescriptor(p_vEncoderID);

	if((opRealtimeEncoderDescriptor->GetType() & EM_TYPE_ANY_VIDEO) > 0)
	{

		EMWinRealtimeVideoEncoderDescriptor* opEWRVED = dynamic_cast<EMWinRealtimeVideoEncoderDescriptor*>(opRealtimeEncoderDescriptor);
		pFilter = opEWRVED -> GetEncoderFilter();
	} else if((opRealtimeEncoderDescriptor->GetType() & EM_TYPE_ANY_AUDIO) > 0)
	{
		EMWinRealtimeAudioEncoderDescriptor* opEWRAED = dynamic_cast<EMWinRealtimeAudioEncoderDescriptor*>(opRealtimeEncoderDescriptor);
		pFilter = opEWRAED -> GetEncoderFilter();
	}
	// Obtain the filter's IBaseFilter interface. (Not shown)
	IAMVfwCompressDialogs* pComprDialog; 


	HRESULT hr = pFilter->QueryInterface(IID_IAMVfwCompressDialogs, (void **)&pComprDialog);
	if(SUCCEEDED(hr))
	{
		hr = pComprDialog -> ShowDialog(VfwCompressDialog_QueryConfig, NULL);
		if(hr == S_OK)
		{

			if(!p_vShow)
				return true;

			pComprDialog -> ShowDialog(VfwCompressDialog_Config, NULL);
		} else
		{
			//NO Dialog exists
			return false;
			pComprDialog -> Release();
		}

	} else
	{
		//GetAudioCompressionCapabilities();
		return false;
	}

	pComprDialog -> Release();
	return true;
}

bool EMWinMediaSingletonMuxer::SetCompressionQuality(double p_vQuality, int32 p_vVideoEncoderID)
{
	IAMVideoCompression* pVideoCompr;
	IBaseFilter *pFilter;
	EMRealtimeEncoderDescriptor* opRealtimeEncoderDescriptor;
	if(p_vVideoEncoderID == 0)
		opRealtimeEncoderDescriptor = GetEncoderDescriptor(m_vVideoEncoderID);
	else
		opRealtimeEncoderDescriptor = GetEncoderDescriptor(p_vVideoEncoderID);

	EMWinRealtimeVideoEncoderDescriptor* opEWRVED = dynamic_cast<EMWinRealtimeVideoEncoderDescriptor*>(opRealtimeEncoderDescriptor);
	pFilter = opEWRVED -> GetEncoderFilter();
	IPin* opEncoderOutPin = GetPin(pFilter, PINDIR_OUTPUT);

	HRESULT hr = opEncoderOutPin -> QueryInterface(IID_IAMVideoCompression, (void **)&pVideoCompr);
	if(SUCCEEDED(hr))
	{
		pVideoCompr -> put_Quality(p_vQuality);
		pVideoCompr -> Release();
		return true;
	}
	return false;
}
void EMWinMediaSingletonMuxer::SetCompressionCapabilityInfo(int32 p_vEncoderID, int32  p_vPropertyID)
{
	m_vCompressionCapabilityEncoder = p_vEncoderID;
	m_vCompressionCapabilityID = p_vPropertyID;
}

void EMWinMediaSingletonMuxer::SetCompressionCapability()
{
	IBaseFilter *pFilter;
	IAMStreamConfig* pStreamCapabilities;
	AUDIO_STREAM_CONFIG_CAPS opConfigCap;
	AM_MEDIA_TYPE* opCapMediaType;// = new AM_MEDIA_TYPE[piCount];
	WAVEFORMATEX* sWaveFormatStructure;
	int piSize;
	EMRealtimeEncoderDescriptor* opRealtimeEncoderDescriptor = GetEncoderDescriptor(m_vCompressionCapabilityEncoder);

	if((opRealtimeEncoderDescriptor->GetType() & EM_TYPE_ANY_VIDEO) > 0)
	{

		EMWinRealtimeVideoEncoderDescriptor* opEWRVED = dynamic_cast<EMWinRealtimeVideoEncoderDescriptor*>(opRealtimeEncoderDescriptor);
		pFilter = opEWRVED -> GetEncoderFilter();
	} else if((opRealtimeEncoderDescriptor->GetType() & EM_TYPE_ANY_AUDIO) > 0)
	{
		EMWinRealtimeAudioEncoderDescriptor* opEWRAED = dynamic_cast<EMWinRealtimeAudioEncoderDescriptor*>(opRealtimeEncoderDescriptor);
		pFilter = opEWRAED -> GetEncoderFilter();
	}
	//pFilter->
	IPin* opEncoderOutPin = GetPin(pFilter, PINDIR_OUTPUT);

	HRESULT hr = opEncoderOutPin -> QueryInterface(IID_IAMStreamConfig, (void **)&pStreamCapabilities);
	hr = pStreamCapabilities -> GetNumberOfCapabilities(&m_vpiCount, &piSize);
	hr = pStreamCapabilities -> GetStreamCaps(m_vCompressionCapabilityID, &opCapMediaType, (BYTE*)&opConfigCap);
	sWaveFormatStructure = reinterpret_cast<WAVEFORMATEX*>(opCapMediaType->pbFormat);
	hr = pStreamCapabilities -> SetFormat(opCapMediaType);

	pStreamCapabilities -> Release();
}

bool EMWinMediaSingletonMuxer::ErrorHasOccured()
{
	return m_vErrorStatus;
}

void EMWinMediaSingletonMuxer::SetErrorHasOccured(bool p_vErrorStatus)
{
	m_vErrorStatus = p_vErrorStatus;
}

char* EMWinMediaSingletonMuxer::GetCompressionCapabilities(int32 p_vEncoderID)
{
	//Temp ... Get some Audio settings
	IAMStreamConfig* pStreamCapabilities;
	int piSize;

	//Make a tmeporary connection with source and encoder
	if(!TempLinkSourceWithEncoder(p_vEncoderID))
	{
		TempDisLinkSourceWithEncoder();
		return (char*)&m_vErrorString;
	}
	
	HRESULT hr = m_opEncoderDummyOutPin -> QueryInterface(IID_IAMStreamConfig, (void **)&pStreamCapabilities);
	if(hr != S_OK)
	{
		TempDisLinkSourceWithEncoder();
		return (char*)&m_vErrorString;
	}
	hr = pStreamCapabilities -> GetNumberOfCapabilities(&m_vpiCount, &piSize);

	if((m_vActiveDummyType & EM_TYPE_ANY_AUDIO) > 0)
	{

		AM_MEDIA_TYPE* opAudioCap;// = new AM_MEDIA_TYPE[piCount];

		WAVEFORMATEX* sWaveFormatStructure;

		if(m_opAudioCompressionCapabilityArray != NULL)
		{
			delete [] m_opAudioCompressionCapabilityArray;
		}
		m_opAudioCompressionCapabilityArray = new string[m_vpiCount+1];
		m_opAudioCompressionCapabilityArray[m_vpiCount] = string("");
		for(int vIndex = 0; vIndex < m_vpiCount; vIndex++)
		{
			hr = pStreamCapabilities -> GetStreamCaps(vIndex, &opAudioCap, (BYTE*)&m_opAudioConfigCap);

			sWaveFormatStructure = reinterpret_cast<WAVEFORMATEX*>(opAudioCap->pbFormat);

			char vSampSec[12];
			char vBitRate[12];

			if(sWaveFormatStructure -> wBitsPerSample == 0)
			{
				itoa(sWaveFormatStructure -> nSamplesPerSec, vSampSec, 10);
			
				float vKiloBit = static_cast<float>((sWaveFormatStructure -> nAvgBytesPerSec * 8.0) / 1000.0);
			
				sprintf(vBitRate, "%.1f", vKiloBit);

				m_opAudioCompressionCapabilityArray[vIndex] = "";
		
				m_opAudioCompressionCapabilityArray[vIndex] += string(vBitRate) + string(" kBit/s, ") + string(vSampSec) +string(" Hz, ");

				if(sWaveFormatStructure->nChannels == 1)
					m_opAudioCompressionCapabilityArray[vIndex] += "Mono";
				else if(sWaveFormatStructure->nChannels == 2)
					m_opAudioCompressionCapabilityArray[vIndex] += "Stereo";
			} else
			{
				itoa(sWaveFormatStructure -> nSamplesPerSec, vSampSec, 10);
			
				itoa(sWaveFormatStructure -> wBitsPerSample, vBitRate, 10);

				m_opAudioCompressionCapabilityArray[vIndex] = "";
		
				m_opAudioCompressionCapabilityArray[vIndex] += string(vSampSec) +string(" Hz, ") + string(vBitRate) + string(" Bit, ");

				if(sWaveFormatStructure->nChannels == 1)
					m_opAudioCompressionCapabilityArray[vIndex] += "Mono";
				else if(sWaveFormatStructure->nChannels == 2)
					m_opAudioCompressionCapabilityArray[vIndex] += "Stereo";

			}
		}
/*		hr = pStreamCapabilities -> GetStreamCaps(3, &opAudioCap, (BYTE*)&m_opAudioConfigCaps[0]);
		sWaveFormatStructure = reinterpret_cast<WAVEFORMATEX*>(opAudioCap->pbFormat);
		hr = pStreamCapabilities -> SetFormat(opAudioCap);
*/

//		hr = pStreamCapabilities -> SetFormat(opAudioCap);
		hr = pStreamCapabilities -> Release();
		TempDisLinkSourceWithEncoder();
		return (char*)m_opAudioCompressionCapabilityArray;

	} else if((m_vActiveDummyType & EM_TYPE_ANY_VIDEO) > 0)
	{
		AM_MEDIA_TYPE* opVideoCap;// = new AM_MEDIA_TYPE[piCount];

		VIDEOINFOHEADER* sVideoFormatStructure;

		if(m_opVideoCompressionCapabilityArray != NULL)
		{
			delete [] m_opVideoCompressionCapabilityArray;
		}

		m_opVideoCompressionCapabilityArray = new string[m_vpiCount+1];
		m_opVideoCompressionCapabilityArray[m_vpiCount] = string("");

		for(int vIndex = 0; vIndex < m_vpiCount; vIndex++)
		{
			hr = pStreamCapabilities -> GetStreamCaps(vIndex, &opVideoCap, (BYTE*)&m_opVideoConfigCap);

			sVideoFormatStructure = reinterpret_cast<VIDEOINFOHEADER*>(opVideoCap -> pbFormat);

			/*
			if(sWaveFormatStructure -> wBitsPerSample == 0)
			{
				itoa(sWaveFormatStructure -> nSamplesPerSec, vSampSec, 10);
			
				float vKiloBit = static_cast<float>((sWaveFormatStructure -> nAvgBytesPerSec * 8.0) / 1000.0);
			
				sprintf(vBitRate, "%.1f", vKiloBit);

				m_opAudioCompressionCapabilityArray[vIndex] = "";
		
				m_opAudioCompressionCapabilityArray[vIndex] += string(vBitRate) + string(" kBit/s, ") + string(vSampSec) +string(" Hz, ");

				if(sWaveFormatStructure->nChannels == 1)
					m_opAudioCompressionCapabilityArray[vIndex] += "Mono";
				else if(sWaveFormatStructure->nChannels == 2)
					m_opAudioCompressionCapabilityArray[vIndex] += "Stereo";
			} else
			{
				itoa(sWaveFormatStructure -> nSamplesPerSec, vSampSec, 10);
			
				itoa(sWaveFormatStructure -> wBitsPerSample, vBitRate, 10);

				m_opAudioCompressionCapabilityArray[vIndex] = "";
		
				m_opAudioCompressionCapabilityArray[vIndex] += string(vSampSec) +string(" Hz, ") + string(vBitRate) + string(" Bit, ");

				if(sWaveFormatStructure->nChannels == 1)
					m_opAudioCompressionCapabilityArray[vIndex] += "Mono";
				else if(sWaveFormatStructure->nChannels == 2)
					m_opAudioCompressionCapabilityArray[vIndex] += "Stereo";

			}
			*/
		}
		//hr = pStreamCapabilities -> SetFormat(opAudioCap);
		//hr = pStreamCapabilities -> Release();
		TempDisLinkSourceWithEncoder();
		return (char*)m_opVideoCompressionCapabilityArray;

	}

	TempDisLinkSourceWithEncoder();
	return (char*)&m_vErrorString;

}

bool EMWinMediaSingletonMuxer::GetFilterPropertyPages(int32 p_vEncoderID, bool p_vShow)
{
	IBaseFilter *pFilter;
	EMRealtimeEncoderDescriptor* opRealtimeEncoderDescriptor = GetEncoderDescriptor(p_vEncoderID);

	if((opRealtimeEncoderDescriptor->GetType() & EM_TYPE_ANY_VIDEO) > 0)
	{

		EMWinRealtimeVideoEncoderDescriptor* opEWRVED = dynamic_cast<EMWinRealtimeVideoEncoderDescriptor*>(opRealtimeEncoderDescriptor);
		pFilter = opEWRVED -> GetEncoderFilter();
	} else if((opRealtimeEncoderDescriptor->GetType() & EM_TYPE_ANY_AUDIO) > 0)
	{
		EMWinRealtimeAudioEncoderDescriptor* opEWRAED = dynamic_cast<EMWinRealtimeAudioEncoderDescriptor*>(opRealtimeEncoderDescriptor);
		pFilter = opEWRAED -> GetEncoderFilter();
	}
		// Obtain the filter's IBaseFilter interface. (Not shown)
	ISpecifyPropertyPages *pSpecify;
	HRESULT hr = pFilter->QueryInterface(IID_ISpecifyPropertyPages, (void **)&pSpecify);

	if (SUCCEEDED(hr))
	{
		if(!p_vShow)
			return true;
		FILTER_INFO FilterInfo;
		pFilter->QueryFilterInfo(&FilterInfo);

		CAUUID caGUID;
		pSpecify->GetPages(&caGUID);

		pSpecify->Release();
            
		OleCreatePropertyFrame(
			NULL,                   // Parent window
			0,                      // x (Reserved)
			0,                      // y (Reserved)
			FilterInfo.achName,     // Caption for the dialog box
			1,                      // Number of filters
			(IUnknown **)&pFilter,  // Pointer to the filter 
			caGUID.cElems,          // Number of property pages
			caGUID.pElems,          // Pointer to property page CLSIDs
			0,                      // Locale identifier
			0,                      // Reserved
			NULL                    // Reserved
		);

		CoTaskMemFree(caGUID.pElems);
		FilterInfo.pGraph->Release();
	} else
	{
		pFilter->Release();
		return false;
	}
	
	pFilter->Release();

	return true;
}
//Creates the Mux Filter in connection to the File Writer
//This has to be created before any connection attempts with the encoder
//and the source filter
void EMWinMediaSingletonMuxer::CreateFileRendererFilter()
{
	//These attributes should be put as private variables later on
	//These attributes are just local method attributes
	HRESULT h = S_OK;
	//End of attributes
	

	//CoCreates the AVI Mux filter
	CoCreateInstance(CLSID_AviDest, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void**) &(m_opAviMuxFilter));
	h = m_opGraphBuilder -> AddFilter(m_opAviMuxFilter, L"Avi Mux Filter");
	if(h != S_OK)
	{
		EMDebugger("ERROR! Failed to Create Avi Mux Filter in EMWinMediaSingletonMuxer!");
	}

	//CoCreates the filewriter filter
	CoCreateInstance(CLSID_FileWriter, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void**) &(m_opFileWriterFilter));
	h = m_opGraphBuilder -> AddFilter(m_opFileWriterFilter, L"Buffers To Disc Writer");
	if(h != S_OK)
	{
		EMDebugger("ERROR! Failed to Create FileWriter Filter in EMWinMediaSingletonMuxer!");
	}

	//Get FileWriters interface (also called IFileSinkFilter)
    h = m_opFileWriterFilter -> QueryInterface(IID_IFileSinkFilter, (void **)&m_opFileZinkInterface);
	if(h != S_OK)
	{
		EMDebugger("ERROR! Failed to retrieve interface of FileSinkFilter from FileWriter Filter in EMWinMediaSingletonMuxer!");
	}

	
	//Connect the two filters
	IPin* opMuxOutPin;
	IPin* opFileWriteInPin;

	opMuxOutPin = GetPin(m_opAviMuxFilter, PINDIR_OUTPUT);
	if(opMuxOutPin == NULL)
	{
		EMDebugger("ERROR! Failed to Find the AVI Mux Filter output Pin in EMWinMediaSingletonMuxer!");
	}

	opFileWriteInPin = GetPin(m_opFileWriterFilter, PINDIR_INPUT);
	if(opFileWriteInPin == NULL)
	{
		EMDebugger("ERROR! Failed to Find the FileWriter Filter input Pin!");
	}

	h =m_opGraphBuilder ->Connect(opMuxOutPin, opFileWriteInPin);
	if(h != S_OK)
	{
		EMDebugger("ERROR! Failed to Connect AVI Mux Filter with the FileWriter Filter in EMWinMediaSingletonMuxer!");
	}

	//We don't need the pins any more, release them (not the same as deletion)
	opMuxOutPin->Release();
	opFileWriteInPin->Release();
	
}

IPin* EMWinMediaSingletonMuxer::GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir) 
{
    BOOL		bFound = FALSE;
    IEnumPins	*pEnum;
    IPin		*pPin;
	IPin		*myDummyPin;
	bool		FoundIt = false;
    pFilter->EnumPins(&pEnum);
    while(pEnum->Next(1, &pPin, 0) == S_OK)
    {
        PIN_DIRECTION PinDirThis;
        pPin->QueryDirection(&PinDirThis);
        if (bFound = (PinDir == PinDirThis))
		{
//			MessageBox(NULL, "Found Matching PIN_DIRECTION", "Looking for a PIN", MB_OK);
			pPin->ConnectedTo(&myDummyPin);
			if(myDummyPin == NULL)
			{
				FoundIt = true;
				break;
			}
		}
        pPin->Release();
//			MessageBox(NULL, "Found A Non Matching PIN_DIRECTION", "Looking for a PIN", MB_OK);
    }
//	if(!FoundIt)
//			MessageBox(NULL, "DIDN'T FIND A MATCHING PIN", "WARNING WARNING!!!", MB_OK);
    pEnum->Release();
    return (bFound ? pPin : 0);  
}



#endif
