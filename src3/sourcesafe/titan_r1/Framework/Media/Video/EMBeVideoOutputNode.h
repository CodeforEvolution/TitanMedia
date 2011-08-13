/*******************************************************
* Creator: Jesper Svensson
* Portability: Native
*-------------------------------------------------------
* 
* Audio-specific buffer producing media node.
*
* Inherits from EMBeOutputNode, and implements the needed
* methods for complementing its parent and thus makes a
* complete media node - in this case, an video producing
* node.
*
* Jesper Svensson, 00-12-13
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_VIDEO_OUTPUT_NODE
#define __EM_BE_VIDEO_OUTPUT_NODE

#include "EMBeOutputNode.h"
#include "EMBeVideoConsumerNode.h"
#include "EMBeDvConsumerNode.h"

#include "EMBeRealtimeAudioOutputDescriptor.h"

class EMBeNodeConnection;

class EMBeVideoOutputNode : public EMBeOutputNode
{
public:
	EMBeVideoOutputNode(media_input* p_spPhysicalOutInput);
	~EMBeVideoOutputNode();
//	bool CheckOutputFormat(media_format* p_spFinalFormat);
	bool Initialize();
	bool InitializeVideoProducer();
	bool Shutdown();
	bool SetUpDVNodes();
	void InitializePreferredFormat(media_format* p_spInOutFormat);
	status_t CheckFinalFormat(media_format* p_spInOutFormat);
	int64 GetBufferDuration() const;

protected:
private:
	EMBeNodeConnection m_oConnection;

	media_input* m_spPhysicalOutInput;
	port_id m_vPortID;
	
	//For VideoConsumer
	EMBeVideoConsumerNode* m_opConsumer;
	//EMPacketRepository* m_opMemoryRepository;
	BMediaRoster* m_opRoster;
	BView* m_opView;
	media_input m_sInput;

	media_node m_sMediaFileNode;
	media_output m_sOutput;
	media_node m_sTimeSourceNode; 

	bool m_vVideoIsEncoded;
	int32 m_vAudioInputCount;
	bool m_vClipIsRunning;
	bigtime_t m_vDuration;

	bigtime_t m_vStartTime;	
	int32 m_vVideoInputCount;

	char* m_vpFileName;

	bool m_IsStarted;
	bool m_vIsInitialized;
	static int64 m_vTimeWhenStarted;
//TEST ATTRIBUTES
	entry_ref m_sRef;
	BMediaTrack *vidtrack;
	uint32 bufsize;
	char *bitmap;
	media_format m_sFormat;

//FOR DV CONSUMER NODE
	EMBeDvConsumerNode*		m_DvConsumerNode;
	media_node				timesourceNode;

	
};

#endif

#endif