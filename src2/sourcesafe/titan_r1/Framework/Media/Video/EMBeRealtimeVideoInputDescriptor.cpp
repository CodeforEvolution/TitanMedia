#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#include "EMBeRealtimeVideoInputDescriptor.h"
#include "EMProjectDataSaver.h"
#include "EMProjectDataLoader.h"
#include "EMMediaEngine.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include <MediaKit.h>
#include <interface/Alert.h>
#include "CommandIDs.h"

EMBeRealtimeVideoInputDescriptor::EMBeRealtimeVideoInputDescriptor(media_output* p_spMediaOutput) 
	:	EMRealtimeInputDescriptor(EM_TYPE_ANY_VIDEO),
		m_vTakeSequenceNumber(0),
		m_vRecording(false)
{
	// instantiate a video consumer
	m_opNode = new EMBeDvConsumerNode(p_spMediaOutput);
	if (!m_opNode) {
		EMDebugger("ERROR: can't create a video consumer node !");
	}

	EMBeMediaUtility::push(this, "EMBeRealtimeVideoInputDescriptor");
}

EMBeRealtimeVideoInputDescriptor::~EMBeRealtimeVideoInputDescriptor()
{

	//EMBeMediaUtility::GetRosterE() -> ReleaseNode(m_opNode -> Node());
	EMBeMediaUtility::GetRosterE() -> StopNode(m_opNode -> Node(), 0, true);
	m_opNode -> DisconnectNode();

//TODO: Check and see why we can't deallocate this object without everything crashing down...
/*	if(m_opNode != NULL)
		delete m_opNode;
*/
	m_opNode -> SetDestination(NULL);


	;//cout_commented_out_4_release<< "Checking to see if producer needs to be released" << endl;
	if (m_spMediaOutput -> node != media_node::null) {
		;//cout_commented_out_4_release<<"releasing ProducerNode!"<<endl;;
		EMBeMediaUtility::GetRosterE()->ReleaseNode(m_spMediaOutput -> node);
		m_spMediaOutput -> node = media_node::null;
	}

	EMBeMediaUtility::pop("EMBeRealtimeVideoInputDescriptor");
}

void EMBeRealtimeVideoInputDescriptor::ReadBuffer(EMMediaDataBuffer* p_opBuffer, int64 p_vForFrame)
{
}

bool EMBeRealtimeVideoInputDescriptor::InitCheckE()
{
	if(m_vIsInitialized)
		return true;

	if(m_opNode != NULL)
	{
		if(m_opNode -> Initialize())
			m_vIsInitialized = true;
	}

	if(! m_vIsInitialized)
		emerr << "ERROR! Tried to initialize a video input descriptor with a NULL node!" << endl;

//	EMMediaSignalMeter* opMeter = EMMediaSignalMeterRepository::Instance() -> InstanceSignalMeter(EM_TYPE_RAW_AUDIO);
//	m_opNode -> SetDestination(opMeter);
//	m_opNode -> InitCheckE();
	
	return m_vIsInitialized;
}

bool EMBeRealtimeVideoInputDescriptor::PrepareToPlayE()
{
 return true;
}

void EMBeRealtimeVideoInputDescriptor::StartE()
{
	if(oRegisteredRecorders.size() > 0)
	{
		;//cout_commented_out_4_release << "Now starting EMBeRealtimeVideoInputDescriptor since one or more track(s) has it as input!" << endl;
		string* m_opFamilyName = static_cast<string*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_RENDER_CODEC_FAMILY));
		string oFileName = EMBeMediaUtility::MakeVideoRecordingFileName((m_vTakeSequenceNumber++), GetID());
		//mFormatFamily = B_AVI_FORMAT_FAMILY;
		if (strcmp(m_opFamilyName -> c_str(), "unknown") == 0)
			(new BAlert("", "Format Family not valid for ouput chose another one", "So Be It"))->Go();
		else
		{
			if(m_opNode -> SetFileName(oFileName, m_opFamilyName) < B_OK) 
			{
				EMDebugger("ERROR: mDvFileWriter -> SetFileName !");
			}

			//The node is allready started, so dont start it!
			m_opNode -> PrepareForRecording();
			m_opNode -> StartRecording();
			m_vRecording = true;
		}
	}
	else
		;//cout_commented_out_4_release << "Shouldn't start EMBeRealtimeVideoInputDescriptor since no track has it as input!" << endl;
//	EMBeMediaUtility::GetRosterE() -> StartNode(m_opNode -> Node(), 0); 
//	m_opNode -> StartVideoDecoder();
}

bool EMBeRealtimeVideoInputDescriptor::StartPreviewE()
{
	if(oRegisteredRecorders.size() > 0)
	{
		;//cout_commented_out_4_release << "Now actually starting the NODE for EMBeRealtimeVideoInputDescriptor (through StopPreviewE) since one or more track(s) has it as input!" << endl;
		EMBeMediaUtility::GetRosterE() -> StartNode(m_opNode -> Node(), 0);
	}
	else
		;//cout_commented_out_4_release << "Shouldn't start the NODE in EMBeRealtimeVideoInputDescriptor (through StartPreviewE), since no track had it as input!" << endl;
		
	return true;
}

bool EMBeRealtimeVideoInputDescriptor::StopPreviewE()
{
	if(! m_vRecording)
	{
		;//cout_commented_out_4_release << "Now actually stopping the NODE for EMBeRealtimeVideoInputDescriptor (through StopPreviewE) since one or more track(s) has it as input!" << endl;
		if(m_opNode -> IsRecording())
		{
			m_opNode -> StopRecording();
			m_opNode -> FinishRecording();
		}
		EMBeMediaUtility::GetRosterE() -> StopNode(m_opNode -> Node(), 0, true);
	}
	else
		m_vRecording = false;
	return true;
}

void EMBeRealtimeVideoInputDescriptor::StopE()
{
	if(oRegisteredRecorders.size() > 0)
	{
		;//cout_commented_out_4_release << "Now stopping EMBeRealtimeVideoInputDescriptor since one or more track(s) has it as input!" << endl;
		if(m_opNode -> IsRecording())
		{
			string oString = string("Finishing recording DV... please hold");
//			EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_WRITE_STATUS_BAR_MESSAGE, &oString);	
			m_opNode -> StopRecording();
			m_opNode -> FinishRecording();
			StopPreviewE();
			oString = string("Recording DV finished.");
//			EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_WRITE_STATUS_BAR_MESSAGE, &oString);	
		}
	}
	else
		;//cout_commented_out_4_release << "Shouldn't stop EMBeRealtimeVideoInputDescriptor since no track had it as input!" << endl;
}

bool EMBeRealtimeVideoInputDescriptor::ClearData()
{
	m_vTakeSequenceNumber = 0;
	return true;
}

bool EMBeRealtimeVideoInputDescriptor::SaveData(EMProjectDataSaver* p_opSaver)
{
	p_opSaver -> SaveUInt32(m_vTakeSequenceNumber);
	return true;
}

bool EMBeRealtimeVideoInputDescriptor::LoadData(EMProjectDataLoader* p_opLoader)
{
	m_vTakeSequenceNumber = p_opLoader -> LoadUInt32();
	return true;
}

#endif
