/*******************************************************
* Creator: Martin Johansson
* Portability: Native
*-------------------------------------------------------
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_WIN_DIRECTX_VIDEO_TRANSITION
#define __EM_WIN_DIRECTX_VIDEO_TRANSITION

#include "EMPlugin.h"
#include "EMMediaDataBuffer.h"
#include "EMVideoTransition.h"
#include "EMWinVideoShuffleOutputInterface.h"
#include "EMReadFrameInterface.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"

#include <list>
//#include <uuids.h>

class EMWinDirectXVideoTransition : public EMVideoTransition
{
public:
	EMWinDirectXVideoTransition(string p_oPluginName, IDirectDraw7* p_opIDD, IMoniker* p_opIMoniker);
	virtual ~EMWinDirectXVideoTransition();

	bool PrepareToPlayE();
	EMMediaDataBuffer* ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList);

	list<EMPluginPreset*>* GetPresets() { m_opPresets -> clear(); return m_opPresets; };
	bool ActivatePreset(int32 p_vID) { return false; };

	virtual bool LoadData(EMProjectDataLoader* p_opLoader);
	virtual bool SaveData(EMProjectDataSaver* p_opSaver);

	virtual EMPlugin* Clone() { return NULL; };
	virtual bool CopyTransParams(EMVideoTransition* p_opTrans) = 0;

protected:
	static const int64 m_vTransTimeSlice;

	IDirectDraw7* m_opIDD;

	IDXEffect* m_opIDxTrans;
	IDXTransform* m_opIEffect;

	IDirectDrawSurface7* m_opIDDInput1;
	IDirectDrawSurface7* m_opIDDInput2;
	IDirectDrawSurface7* m_opIDDOutput;

	IDXSurface* m_opIDXInput1;
	IDXSurface* m_opIDXInput2;
	IDXSurface* m_opIDXOutput;	

protected:
	EMMediaFormat m_oFormat;
	
	bool FormatHasChanged(EMMediaDataBuffer* p_opBuffer);
	bool SetFormat(EMMediaDataBuffer* p_opBuffer);

	bool Setup();
	bool Shutdown();

	IDirectDrawSurface7* CreateDDSurface(int p_vWidth, int p_vHeight, int p_vBpP);

	bool FillSurface(IDirectDrawSurface7* p_opIDXInput, EMMediaDataBuffer* p_opBuffer);
	bool FillBuffert(EMMediaDataBuffer* p_opBuffer, IDirectDrawSurface7* p_opIDXOutput);

	virtual bool UpdateSettings() = 0;
	virtual float CalcTime(EMMediaDataBuffer* p_opBuffer);
	virtual bool NeedReset() = 0;
private:
	IMoniker* m_opIMoniker;
};

#endif
