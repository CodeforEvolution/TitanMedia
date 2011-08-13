/*******************************************************
* Creator: Martin Johansson
* Portability: Native
*-------------------------------------------------------
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_WIN_DX_VIDEO_TRANSITION
#define __EM_WIN_DX_VIDEO_TRANSITION

#include "EMWinDirectXVideoTransition.h"
#include "EMMediaDataBuffer.h"
#include "EMWinVideoShuffleOutputInterface.h"
#include "EMReadFrameInterface.h"

class EMWinDXVideoTransition : public EMWinDirectXVideoTransition
{
public:
	EMWinDXVideoTransition(string p_oPluginName, IDirectDraw7* p_opIDD, IMoniker* p_opIMoniker);
	EMWinDXVideoTransition(const EMWinDXVideoTransition& oTrans);
	virtual ~EMWinDXVideoTransition();

	void* GetNativeEffect() const { return NULL; };

	virtual bool CopyTransParams(EMVideoTransition* p_opTrans);

	virtual bool LoadData(EMProjectDataLoader* p_opLoader);
	virtual bool SaveData(EMProjectDataSaver* p_opSaver);

	void SetSettings(int32* p_vpSettings);
	void GetSettings(int32* p_vpSettings);

	bool LoadSetting(EMProjectDataLoader* p_opLoader);
	bool SaveSetting(EMProjectDataSaver* p_opSaver);

protected:
	bool UpdateSettings();
	float CalcTime(EMMediaDataBuffer* p_opBuffer);
	bool NeedReset();

private:
	bool m_vBackup;

};

#endif
