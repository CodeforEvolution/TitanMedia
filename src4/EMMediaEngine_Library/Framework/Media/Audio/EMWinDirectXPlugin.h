/*******************************************************
* Creator: Richard Kronfalt
* Portability: Native
*-------------------------------------------------------
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_WIN_DIRECTX_PLUGIN
#define __EM_WIN_DIRECTX_PLUGIN

#include "EMPlugin.h"
#include "EMMediaDataBuffer.h"

#include <list>
//#include <uuids.h>

class EMWinDXPluginEngine;

class EMWinDirectXPlugin : public EMPlugin
{
public:
	EMWinDirectXPlugin(string p_oPluginName, IBaseFilter* p_opIBaseFilterInstance);
	~EMWinDirectXPlugin();
	string GetName() const;
	IBaseFilter* GetIBaseFilter() const;
	
	list<EMPluginPreset*>* GetPresets();
	bool ActivatePreset(int32 p_vID);

	bool PrepareToPlayE();
	EMMediaDataBuffer* ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList);

	bool ShowDialog();

	bool LoadSetting(EMProjectDataLoader* p_opLoader);
	bool SaveSetting(EMProjectDataSaver* p_opSaver);

	EMPlugin* Clone();

	void* GetNativeEffect() const;

	bool ReadInternalSettings(char** p_vppData, uint32* p_vpLength);
	bool WriteInternalSettings(char* p_vpData, uint32 p_vLength);

protected:
private:
	string m_oName;
	IBaseFilter* m_opIBaseFilterInstance;
	EMWinDXPluginEngine* m_opPluginEngine;
	EMMediaDataBuffer* m_opDryBuffer;
};

#endif
