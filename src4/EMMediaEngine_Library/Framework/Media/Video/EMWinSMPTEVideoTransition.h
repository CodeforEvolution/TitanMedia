/*******************************************************
* Creator: Martin Johansson
* Portability: Native
*-------------------------------------------------------
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_WIN_SMPTE_VIDEO_TRANSITION
#define __EM_WIN_SMPTE_VIDEO_TRANSITION

#include "EMWinDirectXVideoTransition.h"
#include "EMMediaDataBuffer.h"
#include "EMWinVideoShuffleOutputInterface.h"
#include "EMReadFrameInterface.h"
#include "EMMediaTransitionDialogIDs.h"

#include "qedit.h"

#include <list>
//#include <uuids.h>

class EMWinSMPTEVideoTransition : public EMWinDirectXVideoTransition
{
public:
	EMWinSMPTEVideoTransition(string p_oPluginName, IDirectDraw7* p_opIDD, IMoniker* p_opIMoniker, int p_vSMPTECode);
	EMWinSMPTEVideoTransition(const EMWinSMPTEVideoTransition& oTrans);
	virtual ~EMWinSMPTEVideoTransition();

//	EMMediaDataBuffer* ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList);

	void SetBorderWidth(long p_vParam);
	void SetBorderColor(long p_vParam);
	void SetBorderSoftness(long p_vParam);
	void SetSMPTECode(long p_vParam);

	long GetBorderWidth();
	long GetBorderColor();
	long GetBorderSoftness();
	long GetSMPTECode();

	void* GetNativeEffect() const { return NULL; };

	virtual bool CopyTransParams(EMVideoTransition* p_opTrans);

	bool LoadData(EMProjectDataLoader* p_opLoader);
	bool SaveData(EMProjectDataSaver* p_opSaver);

	bool LoadSetting(EMProjectDataLoader* p_opLoader);
	bool SaveSetting(EMProjectDataSaver* p_opSaver);

	void SetSettings(int32* p_vpSettings);
	void GetSettings(int32* p_vpSettings);

	void GetDefaultSettings(int32* p_opParam);
protected:
	bool UpdateSettings();
	bool NeedReset();


private:
/*	bool Setup(int p_vWidth, int p_vHeight, int p_vBpP);
	bool Shutdown2();
	IDirectDrawSurface7* CreateDDSurface(int p_vWidth, int p_vHeight, int p_vBpP);

	bool FillSurface(IDirectDrawSurface7* p_opIDXInput, EMMediaDataBuffer* p_opBuffer);
	bool FillBuffert(EMMediaDataBuffer* p_opBuffer, IDirectDrawSurface7* p_opIDXOutput);
*/
private:
	int m_vSMPTECode;

	IDxtJpeg* m_opIDxt;
	EMDialogParameters m_oParams;

};

#endif
