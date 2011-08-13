#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#include "EMBeMediaSystemInspector.h"

#include "EMBeRealtimeInputDescriptor.h"
#include "EMBeRealtimeAudioInputDescriptor.h"
#include "EMBeRealtimeVideoInputDescriptor.h"
#include "EMBeRealtimeOutputDescriptor.h"
#include "EMBeRealtimeAudioOutputDescriptor.h"
#include "EMBeRealtimeVideoOutputDescriptor.h"
#include "EMBeAnalogRealtimeVideoInputDescriptor.h"
#include "EMMediaIDManager.h"
#include "EMDescriptor.h"
#include "EMBeMediaUtility.h"
#include "EMBeVideoConsumerNode.h"
//#include "EMBeVideoDisplayer.h"
#include "EMBeVideoOutputNode.h"
#include "EMBeMediaRenderingNode.h"

#include <MediaKit.h>
#include <iostream>
#include <string>

EMBeMediaSystemInspector::EMBeMediaSystemInspector()
	:	EMMediaSystemInsepctor(),
		m_opConsumer(NULL),
		m_opRenderNode(NULL)
{
	EMBeMediaUtility::GetRosterE();
	m_vID = EMMediaIDManager::MakeID();
	m_opRenderNode = EMBeMediaRenderingNode::Instance();
	m_opRenderNode -> Initialize();
	InspectCodecs();
}

EMBeMediaSystemInspector::~EMBeMediaSystemInspector()
{
//	EMDebugger("Now in SystemInspector's destructor!");
	if(m_opConsumer != NULL)
	{
		EMBeMediaUtility::GetRosterE() -> ReleaseNode(m_opConsumer -> Node());
		EMBeMediaUtility::pop("EMBeMediaSystemInspector");
		EMBeMediaUtility::GetRosterE() -> ReleaseNode(m_opRenderNode -> Node());;
	}
}

list<EMBeRealtimeInputDescriptor*>* EMBeMediaSystemInspector::GetRealtimeAudioInputs()
{
	list<EMBeRealtimeInputDescriptor*>* opList1 = GetInputs(EM_TYPE_RAW_AUDIO);
	return opList1;
}

list<EMBeRealtimeInputDescriptor*>* EMBeMediaSystemInspector::GetRealtimeVideoInputs()
{
	list<EMBeRealtimeInputDescriptor*>* opList2 = GetInputs(EM_TYPE_ANY_VIDEO);
	return opList2;
}

list<EMBeRealtimeOutputDescriptor*>* EMBeMediaSystemInspector::GetRealtimeAudioOutputs()
{
//	list<EMBeRealtimeOutputDescriptor*>* opList1 = new list<EMBeRealtimeOutputDescriptor*>();
	list<EMBeRealtimeOutputDescriptor*>* opList1 = GetOutputs(EM_TYPE_RAW_AUDIO);
	return opList1;
}

list<EMBeRealtimeOutputDescriptor*>* EMBeMediaSystemInspector::GetRealtimeVideoOutputs()
{
//	list<EMBeRealtimeOutputDescriptor*>* opList1 = new list<EMBeRealtimeOutputDescriptor*>();
	list<EMBeRealtimeOutputDescriptor*>* opList1 = GetOutputs(EM_TYPE_RAW_VIDEO);
	return opList1;
}

list<EMBeRealtimeInputDescriptor*>* EMBeMediaSystemInspector::GetInputs(EMMediaType p_eType)
{
	status_t vErrorCode;
	dormant_node_info spNodeInfo[20];
	live_node_info spLiveNodeInfo[20];
	int32 vNumNodes = 20;
	media_format sFormat;
	media_output spOutputs[20];

	media_node sNode;
	list<EMBeRealtimeInputDescriptor*>* opList = NULL;

	opList = new list<EMBeRealtimeInputDescriptor*>();

	if(p_eType == EM_TYPE_RAW_AUDIO)
		sFormat.type = B_MEDIA_RAW_AUDIO;
	else if(p_eType == EM_TYPE_ENCODED_AUDIO)
		sFormat.type = B_MEDIA_ENCODED_AUDIO;
	else if(p_eType == EM_TYPE_RAW_VIDEO)
		sFormat.type = B_MEDIA_RAW_VIDEO;
	else if(p_eType == EM_TYPE_ENCODED_VIDEO)
		sFormat.type = B_MEDIA_ENCODED_VIDEO;
	else if(p_eType == EM_TYPE_ANY)
		sFormat.type = B_MEDIA_MULTISTREAM;


	EMBeRealtimeInputDescriptor* opDescriptor = NULL;

	if((p_eType & EM_TYPE_ANY_VIDEO) > 0)
	{



	//Checking for digital video inputs
		dormant_node_info dni[5];
		live_node_info lni[5];
		media_output* spOutput = NULL;
		// get dormant node info for DV Input Node
		int32			inOutNumNodes = 5;
		int32			linOutNumNodes = 5;
		media_format	tryFormat;
		bool isNotInAliveList = true;

		tryFormat.type = B_MEDIA_ENCODED_VIDEO;
		tryFormat.u.encoded_video = media_encoded_video_format::wildcard;

		vErrorCode = EMBeMediaUtility::GetRosterE() ->GetLiveNodes(lni, &linOutNumNodes, NULL, &tryFormat, NULL, B_PHYSICAL_INPUT);
		for(int32 vIndex = 0; vIndex < linOutNumNodes; vIndex++)
		{
			// find free output from video source
			int32 cnt = 0;
			spOutput = new media_output;

			vErrorCode = EMBeMediaUtility::GetRosterE() -> GetFreeOutputsFor(lni[vIndex].node, spOutput, 1, &cnt, B_MEDIA_ENCODED_VIDEO);
			if (vErrorCode < B_OK || cnt < 1) {
				cout<<"ERROR! Video stream 1 is busy!"<<endl;
			}
			else
			{
				opDescriptor = new EMBeRealtimeVideoInputDescriptor(spOutput);
				opDescriptor -> SetName(string(lni[vIndex].name));
				opList -> push_back(opDescriptor);
			}
		}


		vErrorCode = EMBeMediaUtility::GetRosterE() -> GetDormantNodes(dni, &inOutNumNodes, NULL, &tryFormat, NULL, B_PHYSICAL_INPUT);

		if (vErrorCode < B_OK)
			;
		else
		{
			for(int32 vIndex = 0; vIndex < inOutNumNodes; vIndex++)
			{
				for(int32 vAliveInputsIndex = 0; vAliveInputsIndex < linOutNumNodes; vAliveInputsIndex++)
				{
					if (strcmp(dni[vIndex].name, lni[vAliveInputsIndex].name) == 0)
					{
						isNotInAliveList = false;
					}
				}
				if(isNotInAliveList)
				{
					try
					{
						status_t err = EMBeMediaUtility::GetRosterE()->InstantiateDormantNode(dni[vIndex], &mConnection.producer, B_FLAVOR_IS_LOCAL);
						if (err < B_OK) {
						;//cout_commented_out_4_release<< "ERROR INSTANSIATING DORMANTNODE:" << strerror(err) << endl;
							EMDebugger("ERROR! roster->InstantiateDormantNode");
							return NULL;
						}
	
						// find free output from video source
						int32 cnt = 0;
						spOutput = new media_output;

						err = EMBeMediaUtility::GetRosterE() -> GetFreeOutputsFor(mConnection.producer, spOutput, 1, &cnt, B_MEDIA_ENCODED_VIDEO);
						if (err < B_OK || cnt < 1) {
							cout<<"ERROR! Video stream 1 is busy!"<<endl;
						}
						else
						{
							opDescriptor = new EMBeRealtimeVideoInputDescriptor(spOutput);
							opDescriptor -> SetName(string(dni[vIndex].name));
							opList -> push_back(opDescriptor);
						}
					}
					catch(...)
					{
						emerr << "EXCEPTION! Error! Got an exception while getting outchannels from the physical input!" << endl;
						delete spOutput;
						delete opDescriptor;
						delete opList;
						return NULL;
					}
				}
				isNotInAliveList = true;
			}
		}
		//END Checking for digital video inputs


		///Checking for analog video inputs bt848
		;//cout_commented_out_4_release<<"TITAN will now look for an analog VideoInput-device" << endl;


		live_node_info lniAnalogue[5];

		media_output* spOutputAnalogue = NULL;
		int32 linOutNumNodesAnalogue = 5;
		media_format	tryFormatAnalogue;
		tryFormatAnalogue.type = B_MEDIA_RAW_VIDEO;
		tryFormatAnalogue.u.raw_video = media_raw_video_format::wildcard;

		vErrorCode = EMBeMediaUtility::GetRosterE() ->GetLiveNodes(lniAnalogue, &linOutNumNodesAnalogue, NULL, &tryFormatAnalogue, NULL, B_PHYSICAL_INPUT);
		for(int32 vIndex = 0; vIndex < linOutNumNodesAnalogue; vIndex++)
		{
			// find free output from video source
			int32 cnt = 0;
			spOutputAnalogue = new media_output;
			vErrorCode = EMBeMediaUtility::GetRosterE() -> GetFreeOutputsFor(lniAnalogue[vIndex].node, spOutputAnalogue, 1, &cnt, B_MEDIA_RAW_VIDEO);
			if (vErrorCode < B_OK || cnt < 1) {
				cout<<"ERROR! Video stream is busy!"<<endl;
				//return NULL;
			}
			else
			{
				opDescriptor = new EMBeAnalogRealtimeVideoInputDescriptor(spOutputAnalogue);
				opDescriptor -> SetName(string(lniAnalogue[vIndex].name));
				opList -> push_back(opDescriptor);
			}
		}
		
		dormant_node_info dniAnalogue[5];
		// get dormant node info for DV Input Node
		int32 inOutNumNodesAnalogue = 5;
		isNotInAliveList = true;
		tryFormatAnalogue.type = B_MEDIA_RAW_VIDEO;
		tryFormatAnalogue.u.raw_video = media_raw_video_format::wildcard;
		vErrorCode = EMBeMediaUtility::GetRosterE() -> GetDormantNodes(dniAnalogue, &inOutNumNodesAnalogue, NULL, &tryFormatAnalogue, NULL, B_PHYSICAL_INPUT);

		if (vErrorCode < B_OK)
			;
		else
		{
			for(int32 vIndex = 0; vIndex < inOutNumNodesAnalogue; vIndex++)
			{
				for(int32 vAliveInputsIndex = 0; vAliveInputsIndex < linOutNumNodesAnalogue; vAliveInputsIndex++)
				{
					if (strcmp(dniAnalogue[vIndex].name, lniAnalogue[vAliveInputsIndex].name) == 0)
					{
						isNotInAliveList = false;
					}
				}
				if(isNotInAliveList)
				{
					try
					{
						status_t err = EMBeMediaUtility::GetRosterE()->InstantiateDormantNode(dniAnalogue[vIndex], &mConnection.producer, B_FLAVOR_IS_LOCAL);
						if (err < B_OK) {
						;//cout_commented_out_4_release<< "ERROR INSTANSIATING DORMANTNODE:" << strerror(err) << endl;
							cout<< "ERROR_CODE:" << strerror(err) << endl;
							EMDebugger("ERROR! roster->InstantiateDormantNode");
							return NULL;
						}

						// find free output from video source
						int32 cnt = 0;
						spOutputAnalogue = new media_output;

						err = EMBeMediaUtility::GetRosterE() -> GetFreeOutputsFor(mConnection.producer, spOutputAnalogue, 1, &cnt, B_MEDIA_RAW_VIDEO);
						if (err < B_OK || cnt < 1) {
							cout<<"ERROR! Video stream is busy!"<<endl;
							//return NULL;
						}
						else
						{
							opDescriptor = new EMBeAnalogRealtimeVideoInputDescriptor(spOutputAnalogue);
							opDescriptor -> SetName(string(dniAnalogue[vIndex].name));
							opList -> push_back(opDescriptor);
						}
					}
					catch(...)
					{
						emerr << "EXCEPTION! Error! Got an exception while getting outchannels from the physical input!" << endl;
						delete spOutputAnalogue;
						delete opDescriptor;
						delete opList;
						return NULL;
					}
				}
				isNotInAliveList = true;
			}

		}
		//END Checking for analogue video inputs


/*		status_t status = EMBeMediaUtility::GetRosterE() -> GetVideoInput(&mConnection.producer);
		if (status != B_OK)
		{
			;//cout_commented_out_4_release<<"Can't find an analog video input:" << status << endl;
		}
		else
		{
				try
				{	
					// find free producer output
					int32 cnt = 0;
					spOutput = new media_output;

					status = EMBeMediaUtility::GetRosterE() -> GetFreeOutputsFor(mConnection.producer, spOutput, 1,  &cnt, B_MEDIA_RAW_VIDEO);
					if (status != B_OK || cnt < 1)
					{
						status = B_RESOURCE_UNAVAILABLE;
						;//cout_commented_out_4_release<< "Can't find an available video stream:" << status << endl;
						EMDebugger("Can't find an available video stream!");
						return NULL;		
					}
					opDescriptor = new EMBeAnalogRealtimeVideoInputDescriptor(spOutput);
					string oName = string(spOutput -> name);
					if(oName == "")
						oName = "Unknown analog video input";
					opDescriptor -> SetName(oName);
					opList -> push_back(opDescriptor);
					
				}
				catch(...)
				{
					EMDebugger("Something went very wrong when trying to allocate analog video inputs!?");
				}
		}
		///Finished checking for analog video inputs	
	}
*/
	} else if((p_eType & EM_TYPE_ANY_AUDIO) > 0)
	{
		//*****************************************
		//This is where the LIVE NODES are fetched
		//*****************************************
		/*vErrorCode = EMBeMediaUtility::GetRosterE() -> GetLiveNodes(spLiveNodeInfo, &vNumNodes, NULL, NULL, NULL, B_PHYSICAL_INPUT);
		for(int32 vIndex1 = 0; vIndex1 < vNumNodes; vIndex1++)
		{
			sNode = spLiveNodeInfo[vIndex1].node;
			vErrorCode = EMBeMediaUtility::GetRosterE() -> GetAllOutputsFor(sNode, spOutputs, 20, &vNumOutputs);
			for(int32 vIndex2 = 0; vIndex2 < vNumOutputs; vIndex2++)
			{
				if(spOutputs[vIndex2].format.type == sFormat.type)
				{
					media_output* spOutput = NULL;
					try
					{
						spOutput = new media_output;
						*spOutput = spOutputs[vIndex2];
						if((p_eType & EM_TYPE_ANY_AUDIO) > 0)
						{
							opDescriptor = new EMBeRealtimeAudioInputDescriptor(spOutput);
						}
						else
						{
							EMDebugger("ERROR! In EMBeMediaSystemInspector::GetInputs() - Unknown media type!");
						}		
		
						if(opDescriptor -> InitCheckE())
						{
							opDescriptor -> SetName(string(spOutput -> name));
							opList -> push_back(opDescriptor);
						}
						else
							emerr << "WARNING! Found an audio input that could not be initialized, so we're skipping it!" << endl;
					} 
					catch(...)
					{
						emerr << "EXCEPTION! Error! Got an exception while getting outchannels from the physical input!" << endl;
						delete spOutput;
						delete opDescriptor;
						delete opList;
						return NULL;
					}
				}
			}
		}*/


		//********************************************
		//This is where the DORMANT NODES are fetched
		//********************************************
		vNumNodes = 20;
		media_format sWantedFormat;
		sWantedFormat.type = B_MEDIA_RAW_AUDIO;
		sWantedFormat.u.raw_audio = media_raw_audio_format::wildcard;

		vErrorCode = EMBeMediaUtility::GetRosterE() -> GetDormantNodes(spNodeInfo, &vNumNodes, NULL, &sWantedFormat, NULL, B_PHYSICAL_INPUT, 0);
		for(int32 vIndex1 = 0; vIndex1 < vNumNodes; vIndex1++)
		{
			vErrorCode = EMBeMediaUtility::GetRosterE() -> InstantiateDormantNode(spNodeInfo[vIndex1], &sNode);

//			EMBeMediaUtility::GetRosterE() -> GetAudioInput(&sNode);
			if(vErrorCode == B_OK)
			{
//				vErrorCode = EMBeMediaUtility::GetRosterE() -> GetAllOutputsFor(sNode, spOutputs, 20, &vNumOutputs);
//				for(int32 vIndex2 = 0; vIndex2 < vNumOutputs; vIndex2++)
//				{
//					if(spOutputs[vIndex2].format.type == sWantedFormat.type)
//					{
//						media_output* spOutput = NULL;
//						try
//						{
//							spOutput = new media_output;
//							*spOutput = spOutputs[vIndex2];
							if((p_eType & EM_TYPE_ANY_AUDIO) > 0)
							{
//								opDescriptor = new EMBeRealtimeAudioInputDescriptor(spOutput);
								opDescriptor = new EMBeRealtimeAudioInputDescriptor(&sNode); //TODO: Make sure the desc Releases the node!
							}
							else
							{
								EMDebugger("ERROR! In EMBeMediaSystemInspector::GetInputs() - Unknown media type!");
							}		
							
							if(opDescriptor -> InitCheckE())
							{
								opDescriptor -> SetName(string(spNodeInfo[vIndex1].name));
								opList -> push_back(opDescriptor);
							}
							else
							{
								delete opDescriptor;
								break;
							}
//						} 
//						catch(...)
//						{
//							emerr << "EXCEPTION! Error! Got an exception while getting outchannels from the physical input!" << endl;
//							delete spOutput;
//							delete opDescriptor;
//							delete opList;
//							return NULL;
//						}
//					}
//				}
			}
			else
				EMDebugger("ERROR! Could not instantiate dormant node!");
		
//			EMBeMediaUtility::GetRosterE() -> ReleaseNode(sNode);
			break; //TODO: Remove this statement!!!!!
		}
	}
	
	return opList;
}

list<EMBeRealtimeOutputDescriptor*>* EMBeMediaSystemInspector::GetOutputs(EMMediaType p_eType)
{
	status_t vErrorCode;
	dormant_node_info spDormantNodeInfo[20];
	live_node_info spLiveNodeInfo[20];

//	int32 vNumDormantNodes = 20;
//	int32 vNumLiveNodes = 20;
	media_format sFormat;
	media_input spInputs[20];
	int32 vNumInputs = 20;
	media_node sNode;
	list<EMBeRealtimeOutputDescriptor*>* opList = NULL;

	opList = new list<EMBeRealtimeOutputDescriptor*>();

	if(p_eType == EM_TYPE_RAW_AUDIO)
		sFormat.type = B_MEDIA_RAW_AUDIO;
	else if(p_eType == EM_TYPE_ENCODED_AUDIO)
		sFormat.type = B_MEDIA_ENCODED_AUDIO;
	else if(p_eType == EM_TYPE_RAW_VIDEO)
		sFormat.type = B_MEDIA_RAW_VIDEO;
	else if(p_eType == EM_TYPE_ENCODED_VIDEO)
		sFormat.type = B_MEDIA_ENCODED_VIDEO;
	else if(p_eType == EM_TYPE_ANY)
		sFormat.type = B_MEDIA_MULTISTREAM;

	if((p_eType & EM_TYPE_RAW_VIDEO) > 0)
	{
		if(m_opConsumer == NULL) 
			m_opConsumer = EMBeVideoConsumerNode::Instance();
			
		if(m_opConsumer == NULL)
		{
			emerr << "ERROR! Could not instantiate video consumer!" << endl;
			return NULL;
		}

		media_node sVideoNode;
		media_output sMediaFileNodeOutput;
		int32 vVideoInputCount;
		media_input sVideoInput;
	
		sVideoNode = m_opConsumer -> Node();

		vErrorCode = B_OK;
		vErrorCode = EMBeMediaUtility::GetRosterE() -> GetFreeInputsFor(sVideoNode, &sVideoInput, 1, &vVideoInputCount, B_MEDIA_RAW_VIDEO);
		if(vErrorCode != B_OK)
		{
			emerr << "ERROR! Could not find any free inputs for video consumer!" << endl;
			return NULL;
		}
		
//		char vpName[64] = "Titan video preview window";
//		memcpy(sVideoInput.name, vpName, 26);
				
		EMBeRealtimeOutputDescriptor* opDescriptor = NULL;
		opDescriptor = new EMBeRealtimeVideoOutputDescriptor(&sVideoInput);
		if(! opDescriptor -> InitCheckE())
		{
			emerr << "ERROR! Could not initialize video output!" << endl;
			delete opDescriptor;
		}
		else
		{
			opDescriptor -> SetName(string(sVideoInput.name));
			opList -> push_back(opDescriptor);
		}

		//***********************************
		//*
		//* Fetching rendering node for video
		//*
		//***********************************
		vErrorCode = B_OK;
		int32 vCount = 1;
		media_input sRenderInput;

		vErrorCode = EMBeMediaUtility::GetRosterE() -> GetFreeInputsFor(m_opRenderNode -> Node(), &sRenderInput, 1, &vCount, B_MEDIA_RAW_VIDEO);
		if(vErrorCode != B_OK)
		{
			EMDebugger("ERROR! Could not find any free video inputs for video render node!");
			return NULL;
		}

//		char vpName2[64] = "Titan video rendering node";
//		memcpy(sVideoInput.name, vpName2, 26);

		opDescriptor = NULL;
		opDescriptor = new EMBeRealtimeVideoOutputDescriptor(&sRenderInput);
		if(! opDescriptor -> InitCheckE())
		{
			emerr << "ERROR! Could not initialize video output connected to render node!" << endl;
			delete opDescriptor;
		}
		else
		{
			opDescriptor -> SetName(string(sRenderInput.name));
			opDescriptor -> SetRenderOutput(true);
			;//cout_commented_out_4_release << "Video Render output has ID = " << opDescriptor -> GetID() << endl;
			opList -> push_back(opDescriptor);
		}

	}
	else if((p_eType & EM_TYPE_ANY_AUDIO) > 0)
	{
		//vErrorCode = m_opRoster -> GetDormantNodes(spDormantNodeInfo, &vNumDormantNodes, &sFormat, NULL, NULL, 0, 0);
		//vErrorCode = m_opRoster -> GetLiveNodes(spLiveNodeInfo, &vNumLiveNodes, &sFormat, NULL, NULL, B_BUFFER_CONSUMER);
		//for(int32 vIndex1 = 0; vIndex1 < vNumLiveNodes; vIndex1++)
		//{
		//	sNode = spLiveNodeInfo[vIndex1].node;
		//	vErrorCode = m_opRoster -> GetFreeInputsFor(spLiveNodeInfo[vIndex1].node, spInputs, 20, &vNumInputs);
		
			media_node am;
			EMBeMediaUtility::GetRosterE() -> GetAudioMixer(&am);
			vErrorCode = EMBeMediaUtility::GetRosterE() -> GetFreeInputsFor(am, spInputs, 1, &vNumInputs);

			for(int32 vIndex2 = 0; vIndex2 < vNumInputs; vIndex2++)
			{
				if(spInputs[vIndex2].format.type == sFormat.type)
				{
					media_input* spInput = NULL;
					EMBeRealtimeOutputDescriptor* opDescriptor = NULL;
					try
					{
						spInput = new media_input;
						*spInput = spInputs[vIndex2];
						if((p_eType & EM_TYPE_ANY_AUDIO) > 0)
						{
							opDescriptor = new EMBeRealtimeAudioOutputDescriptor(spInput);
						}
						else
						{
							EMDebugger("ERROR! In EMBeMediaSystemInspector::GetOutputs() - Unknown media type!");
						}

						if(! opDescriptor -> InitCheckE())
						{
							emerr << "ERROR! Could not initialize audio output!" << endl;
							delete opDescriptor;
						}
						else
						{
							opDescriptor -> SetName(string("System audio mixer" /*spInput -> name*/ ));
							opList -> push_back(opDescriptor);
						}
					} 
					catch(...)
					{
						emerr << "EXCEPTION! Error! Got an exception while getting outchannels from the physical input!" << endl;
						delete spInput;
						delete opDescriptor;
						delete opList;
						return NULL;
					}
					break;
				}
			}


			//***********************************
			//*
			//* Fetching rendering node for audio
			//*
			//***********************************
			am = m_opRenderNode -> Node();
			vErrorCode = EMBeMediaUtility::GetRosterE() -> GetFreeInputsFor(am, spInputs, 2, &vNumInputs, B_MEDIA_RAW_AUDIO);
			for(int32 vIndex2 = 0; vIndex2 < vNumInputs; vIndex2++)
			{
				if(spInputs[vIndex2].format.type == sFormat.type)
				{
					media_input* spInput = NULL;
					EMBeRealtimeOutputDescriptor* opDescriptor = NULL;
					try
					{
						spInput = new media_input;
						*spInput = spInputs[vIndex2];
						if((p_eType & EM_TYPE_ANY_AUDIO) > 0)
						{
							opDescriptor = new EMBeRealtimeAudioOutputDescriptor(spInput);
						}
						else
						{
							EMDebugger("ERROR! In EMBeMediaSystemInspector::GetOutputs() - Unknown media type!");
						}
				
						if(! opDescriptor -> InitCheckE())
						{
							emerr << "ERROR! Could not initialize audio output connected to the rendering node!" << endl;
							delete opDescriptor;
						}
						else
						{
							opDescriptor -> SetName(string(spInput -> name));
							opDescriptor -> SetRenderOutput(true);
							;//cout_commented_out_4_release << "Audio Render output has ID = " << opDescriptor -> GetID() << endl;
							opList -> push_back(opDescriptor);
						}
					} 
					catch(...)
					{
						emerr << "EXCEPTION! Error! Got an exception while getting outchannels from the physical input!" << endl;
						delete spInput;
						delete opDescriptor;
						delete opList;
						return NULL;
					}
					break;
				}
				else
					emerr << "ERROR! The input found doesn't match the format needed!" << endl;
			}
			
	}
	return opList;
}
list<EMCodecFormat*>* EMBeMediaSystemInspector::GetCodecs()
{
	return &m_oFormats;
}

void EMBeMediaSystemInspector::InspectCodecs()
{
	int32 vCookie1 = 0;

	media_file_format sFileFormat;
	while(get_next_file_format(&vCookie1, &sFileFormat) == B_OK)
	{
		;//cout_commented_out_4_release<< "FAMILY:" << sFileFormat.short_name << endl;
		EMCodecFormat* opNewFormat = new EMCodecFormat();
		
		opNewFormat -> m_oFormatShortName = sFileFormat.short_name;
		opNewFormat -> m_oFormatLongName = sFileFormat.pretty_name;
		
		if(((sFileFormat.capabilities & media_file_format::B_KNOWS_RAW_AUDIO) >0) || ((sFileFormat.capabilities & media_file_format::B_KNOWS_ENCODED_AUDIO) >0))
			opNewFormat -> m_vRawAudio = sFileFormat.capabilities;
		if(((sFileFormat.capabilities & media_file_format::B_KNOWS_RAW_VIDEO) >0) || ((sFileFormat.capabilities & media_file_format::B_KNOWS_ENCODED_VIDEO) >0))
			opNewFormat -> m_vRawVideo = sFileFormat.capabilities;

		media_format sFormat;
		media_format sOutFormat;
		media_codec_info sCodec;

		if(opNewFormat -> m_vRawAudio)
		{
			memset(&sFormat, 0, sizeof(media_format));
			sFormat.type = B_MEDIA_RAW_AUDIO;
			sOutFormat.type = B_MEDIA_ENCODED_AUDIO;
			//sFormat.u.raw_audio = media_raw_audio_format::wildcard;
			//sFormat.u.encoded_audio = media_encoded_audio_format::wildcard;
			int32 vCookie2 = 0;
			while(get_next_encoder(&vCookie2, &sFileFormat, &sFormat, &sOutFormat, &sCodec) == B_OK)
			{
				opNewFormat -> m_oRawAudioCodecShortNames.push_back(EMCodecInfo(string(sCodec.pretty_name), string(sCodec.short_name)));
				
				
				memset(&sFormat, 0, sizeof(media_format));
				sFormat.type = B_MEDIA_RAW_AUDIO;
				sFormat.u.raw_audio = media_raw_audio_format::wildcard;
			}
		}

		if(opNewFormat -> m_vRawVideo)
		{
			memset(&sFormat, 0, sizeof(media_format));
			sFormat.type = B_MEDIA_RAW_VIDEO;
			sFormat.u.raw_video = media_raw_video_format::wildcard;
			int32 vCookie2 = 0;
			while(get_next_encoder(&vCookie2, &sFileFormat, &sFormat, &sFormat, &sCodec) == B_OK)
			{
				opNewFormat -> m_oRawVideoCodecShortNames.push_back(EMCodecInfo(string(sCodec.pretty_name), string(sCodec.short_name)));
				memset(&sFormat, 0, sizeof(media_format));
				sFormat.type = B_MEDIA_RAW_VIDEO;
				sFormat.u.raw_video = media_raw_video_format::wildcard;
			}
		}

		if(opNewFormat -> m_vRawAudio || 
			opNewFormat -> m_vRawVideo)
			m_oFormats.push_back(opNewFormat);
		else
			delete opNewFormat;
	}
	;//cout_commented_out_4_release << endl << endl << endl << endl;
	;//cout_commented_out_4_release << "OK! Found " << m_oFormats.size() << " formats!" << endl;
	list<EMCodecFormat*>::const_iterator oIter;
	for(oIter = m_oFormats.begin(); oIter != m_oFormats.end(); oIter++)
	{
		;//cout_commented_out_4_release << endl << " " << (*oIter) -> m_oFormatLongName << " (" << (*oIter) -> m_oFormatShortName << ") can handle the following:" << endl;
		;//cout_commented_out_4_release << " *************************************************************" << endl;
		if((*oIter) -> m_vRawAudio)
		{
			;//cout_commented_out_4_release << "  Available audio codecs:" << endl;
			;//cout_commented_out_4_release << "  -----------------------" << endl;
			list<EMCodecInfo>::const_iterator oCodecIterator;
			for(oCodecIterator = (*oIter) -> m_oRawAudioCodecShortNames.begin(); oCodecIterator != (*oIter) -> m_oRawAudioCodecShortNames.end(); oCodecIterator++)
				if((*oCodecIterator).m_oShortName.length() <= 14)
					;//cout_commented_out_4_release << "   - " << (*oCodecIterator).m_oShortName << " \t\t\t\t" << (*oCodecIterator).m_oLongName << endl;
				else
					;//cout_commented_out_4_release << "   - " << (*oCodecIterator).m_oShortName << " \t\t\t" << (*oCodecIterator).m_oLongName << endl;
			;//cout_commented_out_4_release << endl;
		}

		if((*oIter) -> m_vRawVideo)
		{
			;//cout_commented_out_4_release << "  Available video codecs:" << endl;
			;//cout_commented_out_4_release << "  -----------------------" << endl;
			list<EMCodecInfo>::const_iterator oCodecIterator;
			for(oCodecIterator = (*oIter) -> m_oRawVideoCodecShortNames.begin(); oCodecIterator != (*oIter) -> m_oRawVideoCodecShortNames.end(); oCodecIterator++)
				if((*oCodecIterator).m_oShortName.length() <= 14)
					;//cout_commented_out_4_release << "   - " << (*oCodecIterator).m_oShortName << " \t\t\t\t" << (*oCodecIterator).m_oLongName << endl;
				else
					;//cout_commented_out_4_release << "   - " << (*oCodecIterator).m_oShortName << " \t\t\t" << (*oCodecIterator).m_oLongName << endl;
			;//cout_commented_out_4_release << endl;
		}
	}
	;//cout_commented_out_4_release << endl << endl << endl << endl;
}


#endif