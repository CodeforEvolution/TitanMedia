/*******************************************************
* Creator: Martin Johansson
* Portability: Non Native
*-------------------------------------------------------
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_VIDEO_TRANSITION
#define __EM_VIDEO_TRANSITION

#include "EMNativePlugin.h"
#include "EMMediaDataBuffer.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"

#include <list>

class EMVideoTransition : public EMNativePlugin
{
public:
	virtual ~EMVideoTransition();
	string GetName() const;

	virtual int64 GetStartTime();
	virtual int64 GetStopTime();
	
	virtual void SetStartTime(int64 p_vTime);
	virtual void SetStopTime(int64 p_vTime);

	virtual void SwapInputs(bool p_vSwap);

	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool LoadData(EMProjectDataLoader* p_opLoader);

	virtual bool CopyTransParams(EMVideoTransition* p_opTrans) = 0;

	virtual void SetSettings(int32* p_vpSettings) = 0;
	virtual void GetSettings(int32* p_vpSettings) = 0;

protected:
	EMVideoTransition(string p_oPluginName, EMPluginType p_ePluginType);

	virtual float CalcTime(EMMediaDataBuffer* p_opBuffer);

	string m_oName;

	int64 m_vStart;
	int64 m_vStop;

	bool m_vSwap;
private:

};

#endif
