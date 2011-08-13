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

#ifndef __EM_WIN_CHROMA_TRANSITION_ENTRY
#define __EM_WIN_CHROMA_TRANSITION_ENTRY

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMMediaGlobals.h"
#include "EMWinTransitionEntry.h"
#include "EMMediaTransitionDialogIDs.h"

#include <streams.h>

class EMPlugin;

class EMWinChromaTransitionEntry : public EMWinTransitionEntry
{
public:
	EMWinChromaTransitionEntry(string p_oPluginName, IMoniker* p_opIMoniker, IDirectDraw7* p_opIDD, EMMediaType p_vType);
	virtual ~EMWinChromaTransitionEntry();

	virtual void* GetDefaultProperties();

protected:
	EMPlugin* WrapFilter(IMoniker* p_opIMoniker, IDirectDraw7* p_opIDD);//	EMPlugin* WrapFilter(IBaseFilter* p_opIFilter);
	string MakeUnique(string& m_oName);

private:
	EMDialogParameters m_oDefaultParams;
};

#endif
#endif