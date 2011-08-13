/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#ifndef __EM_MEDIA_SYSTEM_INSPECTOR
#define __EM_MEDIA_SYSTEM_INSPECTOR

#include "EMMediaGlobals.h"
#include "EMRealtimeInputDescriptor.h"
#include "EMRealtimeOutputDescriptor.h"
#include "EMRealtimeEncoderDescriptor.h"

class EMMediaSystemInspector 
{
public:
	virtual ~EMMediaSystemInspector();
	static void Create();
	static EMMediaSystemInspector* GetInstance();
	static void DeleteAndResetInstance();

	virtual list<EMCodecFormat*>* GetCodecs() = 0;
	virtual list<EMRealtimeInputDescriptor*>* GetRealtimeAudioInputs() = 0;
	virtual list<EMRealtimeInputDescriptor*>* GetRealtimeVideoInputs() = 0;
	virtual list<EMRealtimeEncoderDescriptor*>* GetRealtimeAudioEncoders() = 0;
	virtual list<EMRealtimeEncoderDescriptor*>* GetRealtimeVideoEncoders() = 0;
	virtual list<EMRealtimeOutputDescriptor*>* GetRealtimeAudioOutputs() = 0;
	virtual list<EMRealtimeOutputDescriptor*>* GetRealtimeVideoOutputs() = 0;
	virtual list<EMRealtimeInputDescriptor*>* GetRealtimeMIDIInputs() = 0;
	virtual list<EMRealtimeOutputDescriptor*>* GetRealtimeMIDIOutputs() = 0;

	virtual void InspectCodecs() = 0;
	virtual bool InitializePluginSystem() = 0;
	
protected:
	EMMediaSystemInspector();

private:
	static EMMediaSystemInspector* m_opInstance;
	virtual list<EMRealtimeInputDescriptor*>* GetInputs(EMMediaType p_eType) = 0;
	virtual list<EMRealtimeOutputDescriptor*>* GetOutputs(EMMediaType p_eType) = 0;
};

#endif