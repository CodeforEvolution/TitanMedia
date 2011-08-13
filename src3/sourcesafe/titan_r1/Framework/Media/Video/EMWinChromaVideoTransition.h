/*******************************************************
* Creator: Martin Johansson
* Portability: Native
*-------------------------------------------------------
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_WIN_CHROMA_VIDEO_TRANSITION
#define __EM_WIN_CHROMA_VIDEO_TRANSITION

#include "EMWinDirectXVideoTransition.h"
#include "EMMediaDataBuffer.h"
#include "EMWinVideoShuffleOutputInterface.h"
#include "EMReadFrameInterface.h"
#include "EMMediaTransitionDialogIDs.h"

#include "qedit.h"

#include <list>
//#include <uuids.h>

class EMWinChromaVideoTransition : public EMWinDirectXVideoTransition
{
public:
	EMWinChromaVideoTransition(string p_oPluginName, IDirectDraw7* p_opIDD, IMoniker* p_opIMoniker);;
	EMWinChromaVideoTransition(const EMWinChromaVideoTransition& oTrans);
	virtual ~EMWinChromaVideoTransition();

	void* GetNativeEffect() const { return NULL; };

	virtual bool CopyTransParams(EMVideoTransition* p_opTrans);

	virtual bool LoadData(EMProjectDataLoader* p_opLoader);
	virtual bool SaveData(EMProjectDataSaver* p_opSaver);

	void SetSettings(int32* p_vpSettings);
	void GetSettings(int32* p_vpSettings);

	bool LoadSetting(EMProjectDataLoader* p_opLoader);
	bool SaveSetting(EMProjectDataSaver* p_opSaver);

	void GetDefaultSettings(int32* p_opParam);
protected:
	IDxtKey* m_opIDxt;
protected:
	bool UpdateSettings();
	float CalcTime(EMMediaDataBuffer* p_opBuffer);
	bool NeedReset();
private:
	EMDialogParameters m_oParams;
};

#endif
