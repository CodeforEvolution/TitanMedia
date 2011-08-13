#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_DIRECTX_PLUGIN_CHAIN
#define __EM_WIN_DIRECTX_PLUGIN_CHAIN

#include "EMPluginChain.h"

struct EMWinPluginWrapperInterface;

class EMWinDirectXPluginChain : public EMPluginChain
{
public:
	EMWinDirectXPluginChain();
	~EMWinDirectXPluginChain();

	bool InitCheckE();
	bool PrepareToAddLast(EMPlugin* p_opPlugin);
	bool PrepareToInsertBefore(EMPlugin* p_opPlugin, EMPlugin* p_opReferencePlugin);

protected:
private:
	EMWinPluginWrapperInterface* m_opDirectXPluginInterface;

};

#endif

#endif