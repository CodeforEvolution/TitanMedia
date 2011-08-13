#include "EMBeVideoOutputNode.h"

#include "EMSettingsRepository.h"
#include "EMSettingIDs.h"
#include "EMMediaEngine.h"

#ifdef PLATFORM_BEOS

#include "EMBeMediaUtility.h"

#include <iostream>

EMBeVideoOutputNode::EMBeVideoOutputNode(media_input* p_spPhysicalOutInput)
	:	BMediaNode("Titan stream mixer node"),
		EMBeOutputNode(B_MEDIA_RAW_VIDEO),
		m_spPhysicalOutInput(p_spPhysicalOutInput)
{
	if(m_spPhysicalOutInput -> format.type != B_MEDIA_RAW_VIDEO)
		EMDebugger("ERROR! We can't use an input that is non-raw video!");
		
	status_t err = EMBeMediaUtility::GetRosterE() -> RegisterNode(this);
	if(err)
		EMDebugger("ERROR! Could not register video output producer node!");
	EMBeMediaUtility::push(this, "EMBeVideoOutputNode");
}

EMBeVideoOutputNode::~EMBeVideoOutputNode()
{
	EMBeMediaUtility::pop("EMBeVideoOutputNode");
}

bool EMBeVideoOutputNode::Shutdown()
{
	status_t e = EMBeMediaUtility::GetRosterE() -> StopNode(Node(), true);
	if(e != B_OK)
		EMDebugger((string(string("ERROR! Could not stop Titan video output node: ") + string(strerror(e)))).c_str());
	status_t vErrorCode = EMBeMediaUtility::GetRosterE() -> Disconnect(m_oConnection.m_sProducer.node, m_oConnection.m_sOutput.source, m_oConnection.m_sConsumer.node, m_oConnection.m_sInput.destination); //.source, m_oConnection.m_sInput.destination, &m_oConnection.m_sFormat, &m_oConnection.m_sOutput, &m_oConnection.m_sInput);
	if(vErrorCode != B_OK)
		EMDebugger("ERROR! Could not disconnect Titan's video output from the video consumer!");
	EMBeOutputNode::Shutdown();
	return true;
}

bool EMBeVideoOutputNode::Initialize()
{
	if(! EMBeOutputNode::Initialize())
	{
		EMDebugger("ERROR! Could not initialize the video node's parent class!");
		return false;
	}

	media_node mTimeSource;
	BMediaRoster* r = BMediaRoster::Roster();
	m_oConnection.m_sProducer = Node();
	m_oConnection.m_sConsumer = m_spPhysicalOutInput -> node;
	r -> GetTimeSource(&mTimeSource);
	r -> SetTimeSourceFor(m_oConnection.m_sProducer.node, mTimeSource.node);
	BTimeSource* opTimeSource = EMBeMediaUtility::GetRosterE() -> MakeTimeSourceFor(m_oConnection.m_sProducer);
/*	if(EMBeMediaUtility::GetRosterE() -> SetTimeSourceFor(m_oConnection.m_sConsumer.node, opTimeSource -> Node().node) != B_OK)
		EMDebugger("ERROR! Could not set time source for consumer node!");
	EMBeMediaUtility::GetRosterE() -> StartNode(opTimeSource -> Node(), 0);
	opTimeSource -> Release();
*/
	// got the nodes; now we find the endpoints of the connection
	int32 count = 1;
	status_t err = r -> GetFreeOutputsFor(m_oConnection.m_sProducer, &m_oConnection.m_sOutput, 1, &count);
	m_oConnection.m_sInput = *m_spPhysicalOutInput;
		
	// got the endpoints; now we connect it!
	m_oConnection.m_sFormat = m_spPhysicalOutInput -> format;
	m_oConnection.m_sFormat.type = B_MEDIA_RAW_VIDEO;	
	m_oConnection.m_sFormat.u.raw_video = media_raw_video_format::wildcard;
	err = r -> Connect(m_oConnection.m_sOutput.source, m_oConnection.m_sInput.destination, &m_oConnection.m_sFormat, &m_oConnection.m_sOutput, &m_oConnection.m_sInput);
	if(err != B_OK)
		emerr << "ERROR! Could not connect video nodes: " << strerror(err) << endl;

	// the inputs and outputs might have been reassigned during the
	// nodes' negotiation of the Connect().  That's why we wait until
	// after Connect() finishes to save their contents.
	m_oConnection.m_sSource = m_oConnection.m_sOutput.source;
	m_oConnection.m_sDestination = m_oConnection.m_sOutput.destination;
	EMBeMediaUtility::GetRosterE() -> SetRunModeNode(m_oConnection.m_sProducer, BMediaNode::B_INCREASE_LATENCY);

	return true;
}

bool
EMBeVideoOutputNode::SetUpDVNodes()
{
	dormant_node_info	dni;
	dormant_node_info	dni2;
	
	// find TimeSource
	status_t err = EMBeMediaUtility::GetRosterE()->GetTimeSource(&timesourceNode);
	if (err < B_OK) {
		EMDebugger("Can't get TimeSource!");
		return false;
	}
	;//_commented_out_4_release_printf("TimeSource found: %ld (%lx)\n", timesourceNode.node, timesourceNode.port);

	// instantiate a video consumer
/*	m_DvConsumerNode = new EMBeDvConsumerNode();
	if (!m_DvConsumerNode) {
		EMDebugger("ERROR: can't create a video consumer node !");
		return false;
	}
*/
	// register the node
	// make sure the Media Roster knows that we're using the node
/*
	err = EMBeMediaUtility::GetRosterE()->GetNodeFor(m_DvConsumerNode->Node().node, &mConnection.consumer);
	if (err < B_OK) {
		EMDebugger("Can't get node for the video consumer");
		return false;
	}

	// get dormant node info for DV Input Node
	int32			inOutNumNodes = 1;
	media_format	tryFormat;

	tryFormat.type = B_MEDIA_ENCODED_VIDEO;
	tryFormat.u.encoded_video = media_encoded_video_format::wildcard;

	err = EMBeMediaUtility::GetRosterE()->GetDormantNodes(
		&dni, &inOutNumNodes, NULL, &tryFormat,
		"Default DV Input", B_BUFFER_PRODUCER
	);

	if (err < B_OK) {
		EMDebugger("error: roster->GetDormantNodes(DV)");
		return false;
	}

	// instantiate a copy of the node for our use
	err = EMBeMediaUtility::GetRosterE()->InstantiateDormantNode(dni, &mConnection.producer, B_FLAVOR_IS_LOCAL);
	if (err < B_OK) {
		EMDebugger("error: roster->InstantiateDormantNode");
		return false;
	}

*/


/*
///BIG FUCKING TEST
	// get dormant node info for DV Input Node
	int32			inOutNumNodes2 = 1;
	media_format	tryFormat2;

	tryFormat2.type = B_MEDIA_ENCODED_VIDEO;
	tryFormat2.u.encoded_video = media_encoded_video_format::wildcard;

	err = EMBeMediaUtility::GetRosterE()->GetDormantNodes(
		&dni2, &inOutNumNodes2, &tryFormat2, NULL,
		"Default DV Output", B_BUFFER_CONSUMER
	);

	if (err < B_OK) {
		error("error: roster->GetDormantNodes(DV OUPUTNODE FAILED)", err);
		return false;
	} else ;//_commented_out_4_release_printf("SUCESSS!!");

	// instantiate a copy of the node for our use
	err = EMBeMediaUtility::GetRosterE()->InstantiateDormantNode(dni2, &mConnection2.producer, B_FLAVOR_IS_LOCAL);
	if (err < B_OK) {
		error("error: roster->InstantiateDormantNode", err);
		return false;
	}
	BParameterWeb * aWeb2 = NULL;

	err = EMBeMediaUtility::GetRosterE()->GetParameterWebFor(mConnection2.producer, &aWeb2);
	if (err < B_OK) {
		error("can't get parameter web", err);
		return false;
	}
	err = SetupTransportPanel(aWeb2);
	if (err < B_OK) {
		error("can't setup transport panel", err);
		return false;
	}

//////////////////ENDS HERE
*/
	;//_commented_out_4_release_printf("OK: roster->InstantiateDormantNode(%s)\n", dni.name);

/*
	BParameterWeb * aWeb = NULL;

	err = EMBeMediaUtility::GetRosterE()->GetParameterWebFor(mConnection.producer, &aWeb);
	if (err < B_OK) {
		EMDebugger("can't get parameter web");
		return false;
	}

	err = SetupTransportPanel(aWeb);
	if (err < B_OK) {
		error("can't setup transport panel", err);
		return false;
	}
*/
/*
	media_output	m_from;
	media_input		m_to;

	// find free output from video source
	int32 cnt = 0;
	err = EMBeMediaUtility::GetRosterE()->GetFreeOutputsFor(mConnection.producer, &m_from, 1, &cnt, B_MEDIA_ENCODED_VIDEO);
	if (err < B_OK || cnt < 1) {
		EMDebugger("Video stream 1 is busy!");
		return false;
	}

	// find free input to video window
	cnt = 0;
	err = EMBeMediaUtility::GetRosterE()->GetFreeInputsFor(mConnection.consumer, &m_to, 1, &cnt, B_MEDIA_ENCODED_VIDEO);
	if (err < B_OK || cnt < 1) {
		EMDebugger("The video output is busy!");
		return false;
	}

	// connect them
	media_format format;
	format.type = B_MEDIA_ENCODED_VIDEO;
	format.u.encoded_video = media_encoded_video_format::wildcard;

	err = EMBeMediaUtility::GetRosterE()->Connect(m_from.source, m_to.destination, &format, &m_from, &m_to);
	if (err < B_OK) {
		EMDebugger("Couldn't connect video input 1 to video display!");
		return false;
	}

	mConnection.format = format;
	mConnection.source = m_from.source;
	mConnection.destination = m_to.destination;
//	mConnected = true;

	// Set time source for output node
	err = EMBeMediaUtility::GetRosterE()->SetTimeSourceFor(mConnection.consumer.node, timesourceNode.node);
	if (err < B_OK) {
		EMDebugger("Couldn't set TimeSource for video display!");
		return false;
	}

	// Set time source for video input
	err = EMBeMediaUtility::GetRosterE()->SetTimeSourceFor(mConnection.producer.node, timesourceNode.node);
	if (err < B_OK) {
		EMDebugger("Couldn't set TimeSource for video input!");
		return false;
	}
*/	
/*
//START NODE
	err = EMBeMediaUtility::GetRosterE()->SetRunModeNode(mConnection.consumer, BMediaNode::B_RECORDING);
	if (err < B_OK) {
		EMDebugger("ERROR: MediaRoster->SetRunModeNode() !");
		return false;
	}

	err = EMBeMediaUtility::GetRosterE()->StartNode(mConnection.producer, 0);
	if (err < B_OK) {
		EMDebugger("Couldn't start video input!");
		return false;
	}

	err = EMBeMediaUtility::GetRosterE()->StartNode(mConnection.consumer, 0);
	if (err < B_OK) {
		EMDebugger("Couldn't start video displayer!");
		return false;
	}
	
*/
	return true;
}


bool EMBeVideoOutputNode::InitializeVideoProducer()
{
	status_t vErrorCode; 
	media_format sTryFormat; 
	dormant_node_info m_sNodeInfo; 
//	int32 m_vNodeCount; 
	int32 vNum;
	media_output sProducerOutput;

	/* Find the time source */
	vErrorCode = EMBeMediaUtility::GetRosterE()->GetTimeSource(&m_sTimeSourceNode);
	if (vErrorCode != B_OK)
	{
		emerr << "ERROR! Can't get a time source: " << vErrorCode << endl;
		return false;
	}
	
	/* Find free producer output */
	vErrorCode = EMBeMediaUtility::GetRosterE() -> GetFreeOutputsFor(Node(), &sProducerOutput, 1, &vNum);
	if (vErrorCode != B_OK || vNum < 1)
	{
		vErrorCode = B_RESOURCE_UNAVAILABLE;
		emerr << "ERROR! Could not find any free inputs for producer!" << endl;
		return false;
	}

	sTryFormat = sProducerOutput.format;

//	sTryFormat.type = B_MEDIA_RAW_VIDEO; 
//	sTryFormat.u.raw_video = media_raw_video_format::wildcard; 
	media_format sFormat = m_spPhysicalOutInput -> format;
	/* Connect producer with COnsumer */
	vErrorCode = EMBeMediaUtility::GetRosterE() -> Connect(sProducerOutput.source, m_spPhysicalOutInput->destination, &sTryFormat, &sProducerOutput, m_spPhysicalOutInput);
	if (vErrorCode != B_OK)
	{
		emerr << "ERROR! Can't connect consumer to producer: " << strerror(vErrorCode) << endl;
		return false;
	}

/*	
//	fMediaRoster->SetProducerRate(fProducerNode, 1, 20);
	// set time sources 
	status = fMediaRoster->SetTimeSourceFor(fProducerNode.node, fTimeSourceNode.node);
	if (status != B_OK) {
		ErrorAlert("Can't set the timesource for the video source", status);
		return status;
	}
	
	status = fMediaRoster->SetTimeSourceFor(fVideoConsumer->ID(), fTimeSourceNode.node);
	if (status != B_OK) {
		ErrorAlert("Can't set the timesource for the video window", status);
		return status;
	}
	
	// figure out what recording delay to use 
	latency = 0;
	status = fMediaRoster->GetLatencyFor(fProducerNode, &latency);
	status = fMediaRoster->SetProducerRunModeDelay(fProducerNode, latency);

	// start the nodes 
	initLatency = 0;
	status = fMediaRoster->GetInitialLatencyFor(fProducerNode, &initLatency);
	if (status < B_OK) {
		ErrorAlert("error getting initial latency for fCaptureNode", status);	
	}
	initLatency += estimate_max_scheduling_latency();
	
	timeSource = fMediaRoster->MakeTimeSourceFor(fProducerNode);
	bool running = timeSource->IsRunning();
*/

}

/*bool EMBeVideoOutputNode::CheckOutputFormat(media_format* p_spFinalFormat)
{
	if(p_spFinalFormat -> type != GetConnectedFormat().type)
	{
		p_spFinalFormat -> type = GetConnectedFormat().type;
		emerr << "ERROR! Suggested format TYPE is not compatible!" << endl;
		return false;
	}
	if(p_spFinalFormat -> u.raw_video.field_rate > GetConnectedFormat().u.raw_video.field_rate)
	{
		emerr << "ERROR! Suggested format FRAME RATE (" << p_spFinalFormat -> u.raw_video.field_rate << " vs our " << GetConnectedFormat().u.raw_video.field_rate << ") out is over our capabilities!" << endl;
		p_spFinalFormat -> u.raw_video.field_rate = GetConnectedFormat().u.raw_video.field_rate;
		return false;
	}

	return true;
}*/

void EMBeVideoOutputNode::InitializePreferredFormat(media_format* p_spInOutFormat)
{
	float vWorkFPS = *(static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FRAMERATE)));

	p_spInOutFormat -> type = B_MEDIA_RAW_VIDEO;
	p_spInOutFormat -> u.raw_video = media_raw_video_format::wildcard;
	p_spInOutFormat -> u.raw_video = media_raw_video_format::wildcard;
	p_spInOutFormat -> u.raw_video.display.line_width = m_spPhysicalOutInput -> format.Width();
	if(p_spInOutFormat -> u.raw_video.display.line_width == 0)
	{
//		EMDebugger("ÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖ????"); //FIIIIIIIIIIIXXXXXXXXXXXX
		p_spInOutFormat -> u.raw_video.display.line_width = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FORMAT_WIDTH)));
		p_spInOutFormat -> u.raw_video.display.line_count = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FORMAT_HEIGHT)));
		p_spInOutFormat -> u.raw_video.display.format = B_RGB32;
	}
	else
	{
		p_spInOutFormat -> u.raw_video.display.line_count = m_spPhysicalOutInput -> format.Height();
		p_spInOutFormat -> u.raw_video.display.format = m_spPhysicalOutInput -> format.u.raw_video.display.format;
	}
	p_spInOutFormat -> u.raw_video.field_rate = vWorkFPS;
}

status_t EMBeVideoOutputNode::CheckFinalFormat(media_format* p_spInOutFormat)
{
	float vWorkFPS = *(static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FRAMERATE)));

	if(p_spInOutFormat -> type != B_MEDIA_RAW_VIDEO)
	{
		p_spInOutFormat -> type = GetConnectedFormat().type;
		emerr << "ERROR! Unknown type in video node's CheckFinalFormat!" << endl;
		return B_MEDIA_BAD_FORMAT;
	}
	if(p_spInOutFormat -> u.raw_video.field_rate > vWorkFPS)
	{
		emerr << "ERROR! Suggested format FRAME RATE (" << p_spInOutFormat -> u.raw_video.field_rate << " vs our " << vWorkFPS << ") out is over our capabilities!" << endl;
		p_spInOutFormat -> u.raw_video.field_rate = vWorkFPS;
	}
	return B_OK;
}

int64 EMBeVideoOutputNode::GetBufferDuration() const
{
	EMMediaFormat oFormat(EM_TYPE_RAW_VIDEO);
	return EMBeMediaUtility::FramesToTime(1, &oFormat);
}

#endif
