#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#include "EMBeRealtimeAudioInputDescriptor.h"
#include "EMBeAudioConsumerNode.h"
#include "EMMediaDataBuffer.h"
#include "EMBeFileInputDescriptor.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaTimer.h"
#include "EMMediaTrack.h"
#include "EMBeMediaUtility.h"
#include "EMMediaSignalMeter.h"
#include "EMMediaSignalMeterRepository.h"
#include "EMRecording.h"
#include "EMMediaPool.h"
#include "EMProjectDataSaver.h"
#include "EMProjectDataLoader.h"

#include <MediaKit.h>


EMBeRealtimeAudioInputDescriptor::EMBeRealtimeAudioInputDescriptor(const media_node* p_spInputNode) 
	:	EMRealtimeInputDescriptor(EM_TYPE_ANY_AUDIO),
		m_opSignalMeter(NULL),
		m_vTakeSequenceNumber(0),
		m_opNode(NULL)
{
	memcpy(&m_sSystemInputNode, p_spInputNode, sizeof(media_node));
	EMBeMediaUtility::push(this, "EMBeRealtimeAudioInputDescriptor");
	m_vIsInitialized = false;
}

EMBeRealtimeAudioInputDescriptor::~EMBeRealtimeAudioInputDescriptor()
{
//	EMDebugger("EMBeRealtimeAudioInputDescriptor received capital punishment! You can press G in the terminal, madderphakker!");

	if(m_opNode != NULL)
	{
		if(! m_opNode -> Shutdown())
			EMDebugger("ERROR! Turns out the audio consumer node couldn't disconnect properly!");

		EMBeMediaUtility::GetRosterE() -> StopNode(m_opNode -> Node(), true);
//		m_opNode -> SetDestination(NULL);
		EMBeMediaUtility::GetRosterE() -> ReleaseNode(m_opNode -> Node());
	}

	//Return the dormant node that we borrowed from the system!	
	EMBeMediaUtility::GetRosterE() -> ReleaseNode(m_sSystemInputNode);

	EMBeMediaUtility::pop("EMBeRealtimeAudioInputDescriptor");
	delete m_opRecording;
}

bool EMBeRealtimeAudioInputDescriptor::PrepareToPlayE()
{
	return true;
}

bool EMBeRealtimeAudioInputDescriptor::StartPreviewE()
{
	return true;
}

void EMBeRealtimeAudioInputDescriptor::ReadBuffer(EMMediaDataBuffer* p_opBuffer, int64 p_vForFrame)
{
}

bool EMBeRealtimeAudioInputDescriptor::InitCheckE()
{
//	snooze(1000000);
	if(m_vIsInitialized)
		return true;

//	if(m_opNode != NULL)
//	{
	media_output sOutputToUse; // = media_output::wildcard;
	memset(&sOutputToUse, 0, sizeof(media_output));

//snooze(1000000);
	
	int32 vNumOutputs = 5;
	media_output spOutputs[5];

	status_t vErrorCode = EMBeMediaUtility::GetRosterE() -> GetAllOutputsFor(m_sSystemInputNode, spOutputs, 5, &vNumOutputs);
	if(vErrorCode != B_OK)
	{
		EMDebugger("ERROR! Could not get any free outgoing ports from the system's audio input node!");
	}
	
//	snooze(1000000);

	media_format sWantedFormat;
	sWantedFormat.type = B_MEDIA_RAW_AUDIO;
	sWantedFormat.u.raw_audio = media_raw_audio_format::wildcard;

	if(vNumOutputs <= 0)
	{
		;//emout_commented_out_4_release << "WARNING! There were no available data outputs ports from your system's audio-input node! You won't be able to record! ASSHOLE!" << endl;
		return false;
	}

	for(int32 vIndex = 0; vIndex < vNumOutputs; vIndex++)
	{
		if(spOutputs[vIndex].format.type == sWantedFormat.type)
		{
			memcpy(&sOutputToUse, &(spOutputs[vIndex]), sizeof(media_output));
			break;
		}
	}

	m_opNode = EM_new EMBeAudioConsumerNode(&sOutputToUse);

	if(m_opNode -> Initialize())
		m_vIsInitialized = true;
	else
		return false;

	if(! m_vIsInitialized)
	{
		emerr << "ERROR! Tried to initialize an audio input descriptor with a NULL node!" << endl;
		return false;
	}

	EMMediaSignalMeter* opMeter = EMMediaSignalMeterRepository::Instance() -> InstanceSignalMeter(EM_TYPE_RAW_AUDIO);
	if(opMeter == NULL)
		EMDebugger("ERROR!");

	if(m_opNode != NULL)
	{
		m_opNode -> SetDestination(opMeter);
		EMMediaFormat oFormat(EM_TYPE_RAW_AUDIO);
		m_opNode -> InitCheckE(&oFormat);
	}

	return m_vIsInitialized;
}

void EMBeRealtimeAudioInputDescriptor::StartE()
{
	list<EMMediaTrack*>::const_iterator opListIterator;

	if(oRegisteredRecorders.size() > 0)
	{
		if(m_opRecording != NULL)
			delete m_opRecording;
		m_opRecording = EM_new EMRecording();

		for(opListIterator = oRegisteredRecorders.begin(); opListIterator != oRegisteredRecorders.end(); opListIterator++)
			m_opRecording -> m_oRecorderTracks.push_back(*opListIterator);

		m_opRecording -> m_oFileName = EMBeMediaUtility::MakeAudioRecordingFileName((m_vTakeSequenceNumber++), GetID());
		if(m_opNode != NULL)
			m_opNode -> SetTargetFile(m_opRecording -> m_oFileName.c_str());
		m_opRecording -> m_vStartTime = EMMediaTimer::Instance() -> NowTime();

		if(m_opNode != NULL)
			EMBeMediaUtility::GetRosterE() -> StartNode(m_opNode -> Node(), 0); 
			
	}
	else
		;//emerr << "WARNING! Can't start, since audio input " << GetID() << " has no recorders!" << endl;
}

void EMBeRealtimeAudioInputDescriptor::StopE()
{
	
	if(oRegisteredRecorders.size() > 0)
	{
		if(m_opNode != NULL)
		{
			EMBeMediaUtility::GetRosterE() -> StopNode(m_opNode -> Node(), true);
			m_opNode -> CloseTargetFile();
		}
	
		int32 vpArray[2];
		;//cout_commented_out_4_release << "OK! Recording done. The filename is \"" << m_opRecording -> m_oFileName << "\"" << endl;
		EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool() -> AddMediaFileE(m_opRecording -> m_oFileName.c_str(), vpArray);
		;//cout_commented_out_4_release << "OK! File added!" << endl;
		m_opRecording -> m_vMediaFileEntryID = vpArray[0];
		;//cout_commented_out_4_release << "The new file got id " << vpArray[0] << endl;
	}
	else
		; //emerr << "WARNING! Can't stop, since audio input " << GetID() << " has no recorders!" << endl;
}

bool EMBeRealtimeAudioInputDescriptor::StopPreviewE()
{
	return true;
}

bool EMBeRealtimeAudioInputDescriptor::ClearData()
{
	m_vIsInitialized = false; 
	m_vTakeSequenceNumber = 0;
	return true;
}

bool EMBeRealtimeAudioInputDescriptor::SaveData(EMProjectDataSaver* p_opSaver)
{
	p_opSaver -> SaveUInt32(m_vTakeSequenceNumber);
	return true;
}

bool EMBeRealtimeAudioInputDescriptor::LoadData(EMProjectDataLoader* p_opLoader)
{
	m_vTakeSequenceNumber = p_opLoader -> LoadUInt32();
	return true;
}

#endif
