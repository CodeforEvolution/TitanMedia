#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#include "EMBeAnalogRealtimeVideoInputDescriptor.h"
#include "EMProjectDataSaver.h"
#include "EMProjectDataLoader.h"
#include "EMMediaFormat.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMMediaEngine.h"
#include "EMException.h"
#include "EMExceptionHandler.h"
#include "CommandIDs.h"
#include <MediaKit.h>

EMBeAnalogRealtimeVideoInputDescriptor::EMBeAnalogRealtimeVideoInputDescriptor(media_output* p_spMediaOutput) 
: EMBeRealtimeInputDescriptor(p_spMediaOutput, EM_TYPE_ANY_VIDEO)
{
	/* create the video consumer node */
	m_AnalogConsumerNode = new EMBeAnalogConsumerNode("ANALOG CONSUMER NODE", NULL, 0, p_spMediaOutput);
	
	if (!m_AnalogConsumerNode) {
		EMDebugger("ERROR: can't create a video consumer node !");
	}

	EMBeMediaUtility::push(this, "EMBeAnalogRealtimeVideoInputDescriptor");
}

EMBeAnalogRealtimeVideoInputDescriptor::~EMBeAnalogRealtimeVideoInputDescriptor()
{

	//EMBeMediaUtility::GetRosterE() -> ReleaseNode(m_DvConsumerNode -> Node());
	;//cout_commented_out_4_release<<"FIRST STOPPING PRODUCER!"<<endl;
	status_t err = EMBeMediaUtility::GetRosterE() -> StopNode(m_AnalogConsumerNode -> Node(), true);
	if (err < B_OK) {
		;//cout_commented_out_4_release<< "mMediaRoster->Disconnect" << err << endl;
	}

	thread_info t;
	get_thread_info(find_thread(NULL), &t);
	;//cout_commented_out_4_release << "Stop performed by " << t.name << endl;
	
	;//cout_commented_out_4_release<<"Disconnecting"<<endl;
	;//cout_commented_out_4_release<<"mIn.destination.port:"<<m_spMediaOutput -> destination.port<<endl;
	;//cout_commented_out_4_release<<"mIn.source.port:"<<m_spMediaOutput -> source.port<<endl;

	err = EMBeMediaUtility::GetRosterE()->Disconnect(m_spMediaOutput -> node.node, m_spMediaOutput -> source, m_AnalogConsumerNode -> Node().node, m_spMediaOutput -> destination);
	if (err < B_OK) {
		;//cout_commented_out_4_release<< "mMediaRoster->Disconnect" << strerror(err) << endl;
	}

	delete m_AnalogConsumerNode;
	//m_AnalogConsumerNode -> SetDestination(NULL);
	if (m_spMediaOutput -> node != media_node::null) 
	{
		EMBeMediaUtility::GetRosterE()->ReleaseNode(m_spMediaOutput -> node);
		m_spMediaOutput -> node = media_node::null;
	}

	EMBeMediaUtility::pop("EMBeAnalogRealtimeVideoInputDescriptor");

}

void EMBeAnalogRealtimeVideoInputDescriptor::ReadBuffer(EMMediaDataBuffer* p_opBuffer, int64 p_vForFrame)
{
}

bool EMBeAnalogRealtimeVideoInputDescriptor::InitCheckE()
{
	if(m_vIsInitialized)
		return true;

	if(m_AnalogConsumerNode != NULL)
	{
		media_format	format;

		format.type = B_MEDIA_RAW_VIDEO;
		format.u.raw_video.field_rate = 0;
		format.u.raw_video.interlace = 1;
		format.u.raw_video.first_active = 0;
		format.u.raw_video.last_active = 0;
		format.u.raw_video.orientation = B_VIDEO_TOP_LEFT_RIGHT;
		format.u.raw_video.pixel_width_aspect = 1;
		format.u.raw_video.pixel_height_aspect = 1;
		format.u.raw_video.display.format = B_RGB32;
		format.u.raw_video.display.line_width = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_RENDER_FORMAT_WIDTH)));//640;//352;
		format.u.raw_video.display.line_count = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_RENDER_FORMAT_HEIGHT))); //480;//264;
		format.u.raw_video.field_rate = *(static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_RENDER_FRAMERATE)));

		format.u.raw_video.display.bytes_per_row =format.u.raw_video.display.line_width * 4;//640*4;
		format.u.raw_video.display.pixel_offset = 0;
		format.u.raw_video.display.line_offset = 0;

		if(m_AnalogConsumerNode -> Initialize(format))
			m_vIsInitialized = true;
		else
		{
			m_vIsInitialized = false;
			return false;
		}
	} 
	else
		EMDebugger("ERROR! Problem!");

	if(! m_vIsInitialized)
		emerr << "ERROR! Tried to initialize analog video !" << endl;
	
	return m_vIsInitialized;
}

bool EMBeAnalogRealtimeVideoInputDescriptor::PrepareToPlayE()
{
	return true;
}

void EMBeAnalogRealtimeVideoInputDescriptor::StartE()
{
	if(oRegisteredRecorders.size() > 0)
	{
		;//cout_commented_out_4_release << "Now Starting EMBeAnalogRealtimeVideoInputDescriptor since one or more track(s) has it as input!" << endl;
		if(ResetAnalogConnectionAndStart())
		{
			m_AnalogConsumerNode -> PrepareForRecording();
		}
	}
	else
		;//cout_commented_out_4_release << "Shouldn't start EMBeAnalogRealtimeVideoInputDescriptor since no track has it as input!" << endl;
	
}

bool EMBeAnalogRealtimeVideoInputDescriptor:: ResetAnalogConnectionAndStart()
{
	status_t err = EMBeMediaUtility::GetRosterE() -> StopNode(m_AnalogConsumerNode -> Node(), true);
	if (err < B_OK) 
	{
		emerr << "ERROR! mMediaRoster->Disconnect: " << err << endl;
	}

	thread_info t;
	get_thread_info(find_thread(NULL), &t);
	
	err = EMBeMediaUtility::GetRosterE() -> Disconnect(m_spMediaOutput -> node.node, m_spMediaOutput -> source, m_AnalogConsumerNode -> Node().node, m_spMediaOutput -> destination);
	if (err < B_OK) 
	{
		emerr << "ERROR! mMediaRoster->Disconnect: " << strerror(err) << endl;
	}
	;//cout_commented_out_4_release<<"Finished starting new"<<endl;
	m_vIsInitialized = false;
	if(!InitCheckE())
	{
			string oException("Couldn't start analog video capturing with the current settings!?");
			EMExceptionHandler::Instance() -> HandleException(*(new EMException(EM_DIALOG_ALWAYS, &oException)));
			return false;
	} 
	else
	{

		status_t status = EMBeMediaUtility::GetRosterE() -> StartNode(m_spMediaOutput -> node, 0);// perf);
		if(status != B_OK)
			EMDebugger("Couldn't start Producer analog node for some erason");

		status = EMBeMediaUtility::GetRosterE() -> StartNode(m_AnalogConsumerNode -> Node(), 0); 
		if(status != B_OK)
			EMDebugger("Couldn't start Consumer analog node for some erason");


	}
	return true;
}

bool EMBeAnalogRealtimeVideoInputDescriptor::StartPreviewE()
{
	if(m_vIsInitialized)
	{
		if(oRegisteredRecorders.size() > 0)
		{
			;//cout_commented_out_4_release << "Now actually starting the NODE for EMBeAnalogRealtimeVideoInputDescriptor (through StopPreviewE) since one or more track(s) has it as input!" << endl;
			status_t status = EMBeMediaUtility::GetRosterE() -> StartNode(m_spMediaOutput -> node, 0);// perf);
			if(status != B_OK)
				EMDebugger("Couldn't start Producer analog node for some erason");

			status = EMBeMediaUtility::GetRosterE() -> StartNode(m_AnalogConsumerNode -> Node(), 0); 
			if(status != B_OK)
				EMDebugger("Couldn't start Consumer analog node for some erason");
		}
		else
			;//cout_commented_out_4_release << "Shouldn't start the NODE in EMBeAnalogRealtimeVideoInputDescriptor (through StartPreviewE), since no track had it as input!" << endl;
	} 
	else if(oRegisteredRecorders.size() > 0)
	{	
		;//cout_commented_out_4_release << "EMBeAnalogRealtimeVideoInputDescriptor::StartPreviewE() is not initialized so calling ResetAnalogConnectionAndStart() instead" << endl;
		ResetAnalogConnectionAndStart();	
	}
	return true;

}

bool EMBeAnalogRealtimeVideoInputDescriptor::StopPreviewE()
{
	if(m_vIsInitialized)
	{
		;//cout_commented_out_4_release << "Now actually stopping the NODE for EMBeAnalogRealtimeVideoInputDescriptor (through StopPreviewE) since one or more track(s) has it as input!" << endl;
		EMBeMediaUtility::GetRosterE() -> StopNode(m_AnalogConsumerNode -> Node(), 0, true);
	}
	return true;
}

void EMBeAnalogRealtimeVideoInputDescriptor::StopE()
{
	if(oRegisteredRecorders.size() > 0)
	{
		string oString = string("Finishing recording AV... please hold");
//		EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_WRITE_STATUS_BAR_MESSAGE, &oString);	
		;//cout_commented_out_4_release << "Now stopping EMBeAnalogRealtimeVideoInputDescriptor since one or more track(s) has it as input!" << endl;
		m_AnalogConsumerNode -> CloseFile();
		StopPreviewE();
		oString = string("Recording AV finished.");
//		EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_WRITE_STATUS_BAR_MESSAGE, &oString);	
	}
	else
		;//cout_commented_out_4_release << "Shouldn't stop EMBeAnalogRealtimeVideoInputDescriptor since no track had it as input!" << endl;

}

bool EMBeAnalogRealtimeVideoInputDescriptor::ClearData()
{
//	m_vTakeSequenceNumber = 0;
	return true;
}

bool EMBeAnalogRealtimeVideoInputDescriptor::SaveData(EMProjectDataSaver* p_opSaver)
{
//	p_opSaver -> SaveUInt32(m_vTakeSequenceNumber);
	return true;
}

bool EMBeAnalogRealtimeVideoInputDescriptor::LoadData(EMProjectDataLoader* p_opLoader)
{
//	m_vTakeSequenceNumber = p_opLoader -> LoadUInt32();
	return true;
}

#endif
