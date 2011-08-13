/*******************************************************
* Creator: Richard Kronfalt
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_MEDIA_SYSTEM_INSPECTOR
#define __EM_WIN_MEDIA_SYSTEM_INSPECTOR

#include "EMMediaGlobals.h"
#include "EMRealtimeInputDescriptor.h"
#include "EMRealtimeOutputDescriptor.h"
#include "EMRealtimeEncoderDescriptor.h"
#include "EMMediaSystemInspector.h"
#include "EMMediaEngineBuild.h"
#include "EMCommandLineIDs.h"

#include <dsound.h>
#include <list>

class EMWinMediaSystemInspector : public EMMediaSystemInspector
{
public:
	~EMWinMediaSystemInspector();

	static uint32 m_vDisableFeaturesFlags;

	list<EMCodecFormat*>* GetCodecs();
	list<EMRealtimeInputDescriptor*>* GetRealtimeAudioInputs();
	list<EMRealtimeInputDescriptor*>* GetRealtimeVideoInputs();
	list<EMRealtimeOutputDescriptor*>* GetRealtimeAudioOutputs();
	list<EMRealtimeOutputDescriptor*>* GetRealtimeVideoOutputs();
	list<EMRealtimeEncoderDescriptor*>* GetRealtimeAudioEncoders();
	list<EMRealtimeEncoderDescriptor*>* GetRealtimeVideoEncoders();
	list<EMRealtimeInputDescriptor*>* GetRealtimeMIDIInputs();
	list<EMRealtimeOutputDescriptor*>* GetRealtimeMIDIOutputs();
	list<EMRealtimeEncoderDescriptor*>* GetEncoders(EMMediaType p_eType);

	void InspectCodecs();
	bool InitializePluginSystem();
	
	string GetType(const AM_MEDIA_TYPE* p_vpType);

	static BOOL CALLBACK DirectSoundAudioDeviceEnumeration(LPGUID lpGUID, LPCTSTR lpszDesc, LPCTSTR lpszDrvName, LPVOID lpContext);

protected:
	EMWinMediaSystemInspector();
	friend EMMediaSystemInspector;

	virtual bool InitializeDirectXPlugins();	//Sets up the native Microsoft DirectX plugins...
	virtual bool InitializeSMPTETransitions();
	virtual bool InitializeChromaTransitions();
	virtual bool InitializeStaticPlugins();		//Own, static plugins that should exist in the system...
	virtual bool InitializeVSTPlugins();		//Sets up the native Steinberg VST2 plugins...

	bool IsFlagEnabled(EMCommandLineIDs p_vID);

private:
	list<EMRealtimeInputDescriptor*>* GetInputs(EMMediaType p_eType);
	list<EMRealtimeOutputDescriptor*>* GetOutputs(EMMediaType p_eType);

	int32 m_vID;
	list<EMCodecFormat*> m_oFormats;
	uint32 m_vCoInitialized;
};

#endif

#endif
