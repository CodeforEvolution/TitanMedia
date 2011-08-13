/*******************************************************
* Creator: Richard Kronfalt
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_MEDIA_SYSTEM_INSPECTOR
#define __EM_BE_MEDIA_SYSTEM_INSPECTOR

#include "EMMediaGlobals.h"
#include "EMRealtimeInputDescriptor.h"
#include "EMBeRealtimeAudioInputDescriptor.h"
#include "EMBeRealtimeVideoInputDescriptor.h"
#include "EMRealtimeOutputDescriptor.h"
#include "EMBeRealtimeAudioOutputDescriptor.h"
#include "EMBeRealtimeVideoOutputDescriptor.h"

#include <list>

class BMediaRoster;
class EMBeVideoConsumerNode;
class EMBeMediaRenderingNode;

struct Connection
{
	media_node producer, consumer;
	media_source source;
	media_destination destination;
	media_format format;
};

class EMBeMediaSystemInspector : public EMMediaSystemInspector
{
public:
	~EMBeMediaSystemInspector();

	list<EMCodecFormat*>* GetCodecs();
	list<EMRealtimeInputDescriptor*>* GetRealtimeAudioInputs();
	list<EMRealtimeInputDescriptor*>* GetRealtimeVideoInputs();
	list<EMRealtimeOutputDescriptor*>* GetRealtimeAudioOutputs();
	list<EMRealtimeOutputDescriptor*>* GetRealtimeVideoOutputs();
	void InspectCodecs();
	
protected:
	EMBeMediaSystemInspector();
	friend EMMediaSystemInspector;

private:
	list<EMRealtimeInputDescriptor*>* GetInputs(EMMediaType p_eType);
	list<EMRealtimeOutputDescriptor*>* GetOutputs(EMMediaType p_eType);

	BMediaRoster* m_opRoster;
	int32 m_vID;
	EMBeVideoConsumerNode* m_opConsumer;
	Connection mConnection;
	list<EMCodecFormat*> m_oFormats;
	EMBeMediaRenderingNode* m_opRenderNode;
};

#endif

#endif
