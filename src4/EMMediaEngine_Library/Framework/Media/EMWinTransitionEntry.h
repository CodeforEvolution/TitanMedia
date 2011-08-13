/*******************************************************
* Creator: Martin Johansson
* Portability: Native
*-------------------------------------------------------
*
* Represents the existance of a native transition, in the
* the system. It holds native information such as the
* ability to actually instantiate the plugin into a 
* system dependent object. In windows (this file) this
* means having a CLSID that describes the filter's registry
* information, and code for fetching it as a COM object,
* etc, etc.
*
*******************************************************/

#ifndef __EM_WIN_TRANSITION_ENTRY
#define __EM_WIN_TRANSITION_ENTRY

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMMediaGlobals.h"
#include "EMPluginEntry.h"

#include <streams.h>
#include <string>

class EMPlugin;
struct IBaseFilter;

class EMWinTransitionEntry : public EMPluginEntry
{
public:
	EMWinTransitionEntry(string p_oPluginName, IMoniker* p_opIMoniker, IDirectDraw7* p_opIDD, EMMediaType p_vType);
	virtual ~EMWinTransitionEntry();

	EMPlugin* InstantiateNativePlugin();
	virtual string GetUniqueID();

	virtual void* GetDefaultProperties() = 0;

protected:
	virtual EMPlugin* WrapFilter(IMoniker* p_opIMoniker, IDirectDraw7* p_opIDD) = 0; //IBaseFilter* p_opIFilter) = 0;
	virtual string MakeUnique(string& p_oName) = 0;

protected:
	string m_oUniqueID;

private:
	IMoniker* m_opIMoniker;
	IDirectDraw7* m_opIDD;
};

#endif

#endif