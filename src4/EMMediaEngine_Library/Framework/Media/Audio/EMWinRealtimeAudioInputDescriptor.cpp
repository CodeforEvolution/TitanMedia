#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinRealtimeAudioInputDescriptor.h"
#include "EMMediaDataBuffer.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaTimer.h"
#include "EMMediaTrack.h"
#include "EMMediaUtility.h"
#include "EMWinMediaUtility.h"
#include "EMMediaSignalMeter.h"
#include "EMMediaAudioSignalMeter.h"
#include "EMMediaSignalMeterRepository.h"
#include "EMRecording.h"
#include "EMMediaPool.h"
#include "EMProjectDataSaver.h"
#include "EMProjectDataLoader.h"
//#include "EMWinMediaSystemInspector.h"

#include "EMMediaEngineUIDs.h"

EMWinRealtimeAudioInputDescriptor::EMWinRealtimeAudioInputDescriptor(IBaseFilter* p_opSourceFilter) 
	:	EMRealtimeInputDescriptor(EM_TYPE_ANY_AUDIO),
		m_opSignalMeter(NULL),
		m_vTakeSequenceNumber(0),
		m_vIsHookedUp(false),
		m_vIsArmed(false)
{
	m_vIsInitialized = false;
	m_oConnection.m_opSourceFilter = p_opSourceFilter;

	m_opSignalMeter = EMMediaSignalMeter::CreateEMSignalMeter(EM_TYPE_RAW_AUDIO, true);
}

EMWinRealtimeAudioInputDescriptor::~EMWinRealtimeAudioInputDescriptor() 
{
	if(m_vIsHookedUp)
	{
		m_oConnection.m_opControl -> Stop();
	}

	if(m_oConnection.m_opControl != NULL)
		int32 vRef = m_oConnection.m_opControl -> Release();

/*	IEnumFilters* opFilterEnumerator = NULL;
	m_oConnection.m_opGraphBuilder -> EnumFilters(&opFilterEnumerator);
	
	IBaseFilter* opFilter = NULL;
	uint32 vFetched = 0;

	while(opFilterEnumerator -> Next(1, &opFilter, &vFetched) == S_OK)
	{
		IEnumPins* opPinEnumerator = NULL;
		opFilter -> EnumPins(&opPinEnumerator);

		IPin* opPin = NULL;
		vFetched = 0;

		while(opPinEnumerator -> Next(1, &opPin, &vFetched) == S_OK)
		{
			m_oConnection.m_opGraphBuilder -> Disconnect(opPin);
			opPin -> Release();
		}

		opPinEnumerator -> Release();
	}
	opFilterEnumerator -> Release(); */

	if(m_oConnection.m_opGraphBuilder != NULL)
		int32 vRef = m_oConnection.m_opGraphBuilder -> Release();

//	if(m_oConnection.m_opOutputPin != NULL)
//		int32 vRef = m_oConnection.m_opOutputPin -> Release();

	if(m_oConnection.m_opSourceFilter != NULL)
		int32 vRef = m_oConnection.m_opSourceFilter -> Release();
	
	if(m_oConnection.m_opTargetFilter != NULL)
		int32 vRef = m_oConnection.m_opTargetFilter -> Release();

	if(m_oConnection.m_opTargetFilterInterface != NULL)
		int32 vRef = m_oConnection.m_opTargetFilterInterface -> Release();

	if(m_opSignalMeter != NULL)
		delete m_opSignalMeter;

	delete m_opRecording;
}

bool EMWinRealtimeAudioInputDescriptor::PrepareToPlayE()
{
	return true;
}

bool EMWinRealtimeAudioInputDescriptor::StartPreviewE()
{
	return true;
}

bool EMWinRealtimeAudioInputDescriptor::InitCheckE()
{
	HRESULT h = S_OK;
	IGraphBuilder* opGraphBuilder = NULL;

	if(m_oConnection.m_opSourceFilter == NULL)
	{
		//EMDebugger("ERROR! Cannot initialize output desc when the node is NULL!");
		return false;
	}



	h = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IFilterGraph, (void**) &(m_oConnection.m_opGraphBuilder));
	if(FAILED(h))
	{
		eo << "ERROR! Failed to create graph!" << ef;
		return false;
	}

	h = CoCreateInstance(CLSID_EMWinMediaInputFilter, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void**) &(m_oConnection.m_opTargetFilter));
	if(FAILED(h))
	{
		eo << "ERROR! Failed to CoCreateInstance own target filter (EMWinMediaInputFilter)!" << ef;
		return false;
	}

	h = m_oConnection.m_opTargetFilter -> QueryInterface(IID_IEMWinMediaInputFilter, (void**) &(m_oConnection.m_opTargetFilterInterface));
	if(FAILED(h))
	{
		eo << "ERROR! Failed to retrieve interface of own output filter!" << ef;
		return false;
	}

	h = m_oConnection.m_opGraphBuilder -> AddFilter(m_oConnection.m_opSourceFilter, L"Audio source");
	if(FAILED(h))
	{
		eo << "ERROR! Failed to add renderer filter to graph!" << ef;
		return false;
	}

	h = m_oConnection.m_opGraphBuilder -> AddFilter(m_oConnection.m_opTargetFilter, L"Elegant Media Render Filter");
	if(FAILED(h))
	{
		eo << "ERROR! Failed to add own producer filter to filter graph!" << ef;
		return false;
	}

	//m_oConnection.m_opTargetFilterInterface -> InitializeFilter(EMMediaTimer::Instance(), EMMediaEngine::Instance() -> GetSettingsRepository());
	

	m_oConnection.m_opOutputPin = ((EMWinMediaUtility*) EMMediaUtility::Instance()) -> GetFirstPin(m_oConnection.m_opSourceFilter);

	IAMGraphStreams* opStream = NULL;
	h = m_oConnection.m_opGraphBuilder -> QueryInterface(IID_IAMGraphStreams, (void**)&opStream);

	if(!FAILED(h))
	{
		h = opStream -> SyncUsingStreamOffset(true);
		h = opStream -> SetMaxGraphLatency(40000);

		opStream -> Release();
	}


	h = m_oConnection.m_opGraphBuilder -> Render(m_oConnection.m_opOutputPin);
	if(FAILED(h))
	{
		eo << "ERROR! Failed to render graph!" << ef;
		return false;
	}

	m_oConnection.m_opTargetFilterInterface -> Suspend(true);
	m_oConnection.m_opTargetFilterInterface -> SetSignalMeter(m_opSignalMeter);

	h = m_oConnection.m_opGraphBuilder -> QueryInterface(IID_IMediaControl, (void**) &(m_oConnection.m_opControl));
	if(FAILED(h))
	{
		eo << "ERROR! Could not get control interface for graph!" << ef;
		return false;
	}

	h = m_oConnection.m_opControl -> Pause();
	if(FAILED(h))
	{
		eo << "ERROR! Could not activate the input filter!" << ef;
		return false;
	}

	m_vIsInitialized = true;
	m_vIsHookedUp = true;
	return m_vIsInitialized;
}

void EMWinRealtimeAudioInputDescriptor::StartE()
{
	list<EMMediaTrack*>::const_iterator opListIterator;

	if(oRegisteredRecorders.size() > 0)
	{
		if(m_opRecording != NULL)
			delete m_opRecording;
		m_opRecording = EM_new EMRecording();

		for(opListIterator = oRegisteredRecorders.begin(); opListIterator != oRegisteredRecorders.end(); opListIterator++)
			m_opRecording -> m_oRecorderTracks.push_back(*opListIterator);

		m_opRecording -> m_oFileName = EMMediaUtility::Instance() -> MakeAudioRecordingFileName((m_vTakeSequenceNumber++), GetID());
		m_oConnection.m_opTargetFilterInterface -> SetTargetFile(m_opRecording -> m_oFileName);
		m_opRecording -> m_vStartTime = EMMediaTimer::Instance() -> NowTime();

//		HRESULT vResult = m_oConnection.m_opControl -> Run();


		// Comment out by Johan to be able to compile. One row, the one below.
//		m_oConnection.m_opTargetFilterInterface -> StartOnTime(EMMediaTimer::Instance() -> GetSystemTimeWhenStarted());
		m_oConnection.m_opTargetFilterInterface -> Suspend(false);
/*		if(FAILED(vResult))
		{
			eo << "Error code is: " << (int) vResult << ef;
			MessageBox(NULL, "Failed to activate the audio input! Please report to bugs@elegant-media.com\n(don't forget to list your hardware, platform details, your name, etc).\n\nBy the way, don't assume that even the most trivial bug is reported by someone\nelse.\n\nThank you!", "Error!", MB_OK);
		}*/
	}
}

void EMWinRealtimeAudioInputDescriptor::StopE()
{
	if(oRegisteredRecorders.size() > 0)
	{
		m_oConnection.m_opTargetFilterInterface -> Suspend(true);
//		m_oConnection.m_opControl -> Pause();
		m_oConnection.m_opTargetFilterInterface -> CloseTargetFile();
	
		int32 vpArray[2];
		EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool() -> AddMediaFileE(m_opRecording -> m_oFileName.c_str(), vpArray);
		m_opRecording -> m_vMediaFileEntryID = vpArray[0];
	}
}

bool EMWinRealtimeAudioInputDescriptor::StopPreviewE()
{
	return true;
}

bool EMWinRealtimeAudioInputDescriptor::ClearData()
{
	m_vIsInitialized = false; 
	m_vTakeSequenceNumber = 0;
	return true;
}

bool EMWinRealtimeAudioInputDescriptor::SaveData(EMProjectDataSaver* p_opSaver)
{
	p_opSaver -> SaveUInt32(m_vTakeSequenceNumber);
	return true;
}

bool EMWinRealtimeAudioInputDescriptor::LoadData(EMProjectDataLoader* p_opLoader)
{
	m_vTakeSequenceNumber = p_opLoader -> LoadUInt32();
	return true;
}

void EMWinRealtimeAudioInputDescriptor::SetArmed(bool p_vShouldArm)
{
//	float vDummy[5];

	if(p_vShouldArm && oRegisteredRecorders.size() > 0 && !m_vIsArmed)
	{
		m_vIsArmed = true;
		m_oConnection.m_opTargetFilterInterface -> Suspend(true);
	
		HRESULT vResult = m_oConnection.m_opControl -> Run();
		if(FAILED(vResult))
		{
			eo << "Error code is: " << (int) vResult << ef;
			MessageBox(NULL, "Failed to activate the audio input! Please report to bugs@elegant-media.com\n(don't forget to list your hardware, platform details, your name, etc).\n\nBy the way, don't assume that even the most trivial bug is reported by someone\nelse.\n\nThank you!", "Error!", MB_OK);
		}
		
		return;
	}

	if(!p_vShouldArm && oRegisteredRecorders.size() == 0 && m_vIsArmed)
	{
//		for(int i = 0; i < 5; ++i)
//			vDummy[i] = -1;

		m_vIsArmed = false;
		m_oConnection.m_opControl -> Pause();
		m_oConnection.m_opTargetFilterInterface -> Suspend(false);
		m_opSignalMeter -> OnStop();
//		static_cast<EMMediaAudioSignalMeter*>(m_opSignalMeter) -> Put(vDummy, 5);
		return;
	}
}

EMMediaSignalMeter* EMWinRealtimeAudioInputDescriptor::GetSignalMeter()
{
	return m_opSignalMeter;
}

#endif
