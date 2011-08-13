#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_DX_PLUGIN_ENGINE
#define __EM_WIN_DX_PLUGIN_ENGINE

#include <streams.h>

class EMMediaDataBuffer;
struct EMWinDXPluginEngineInterface;

class __declspec(dllexport) EMWinDXPluginEngine
{
public:
	EMWinDXPluginEngine(const char* p_vpName);
	~EMWinDXPluginEngine();
	
	bool InitCheckE(IBaseFilter* p_opFilter);
	bool ShowDialog();
	bool ProcessBuffer(EMMediaDataBuffer* p_opBuffer);

private:
	EMWinDXPluginEngineInterface* m_opSourceFilterInterface;
	EMWinDXPluginEngineInterface* m_opTargetFilterInterface;

	IFilterGraph2* m_opFilterGraph;
	IBaseFilter* m_opSourceFilter;
	IBaseFilter* m_opDirectXPlugin;
	IBaseFilter* m_opTargetFilter;
	IPin* m_opSourceFilterOutputPin;
	IPin* m_opDirectXFilterInputPin;
	IPin* m_opDirectXFilterOutputPin;
	IPin* m_opTargetFilterInputPin;
	IMediaControl* m_opGraphControl;
	bool m_vIsHookedUp;
	string m_oName;
};

#endif

#endif