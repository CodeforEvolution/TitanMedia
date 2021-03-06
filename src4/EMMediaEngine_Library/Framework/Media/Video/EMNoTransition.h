/*******************************************************
* Creator: Martin Johansson
* Portability: Non-Native
*-------------------------------------------------------
*
* This class is your average Wipe. It takes a list of
* buffers (EMMediaDataBuffer) and adds them together and
* returns the result. The transparancy of a clip is
* determined by EMMediaDataBuffer::m_vFader
*
* To be more specific, it takes the first buffer in the
* list and uses that as the destination for the additions.
* It loops through the remaining lists, adds each sample
* to the corresponding sample in the destination, and
* then Recycles the source.
*
* It also knows the next destination (since it inherits
* from EMBeBufferDestination, it is a destination as well
* as knows the next destination) so it updates the
* destination of the returned buffer.
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_NO_TRANSITION
#define __EM_NO_TRANSITION

#include "EMVideoTransition.h"
#include "EMMediaDataBuffer.h"
#include "EMMediaTransitionDialogIDs.h"

class EMNoTransition : public EMVideoTransition
{
public:
	EMNoTransition();
	~EMNoTransition();
	bool PrepareToPlayE();
	EMMediaDataBuffer* ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList);

	virtual list<EMPluginPreset*>* GetPresets() {return NULL;};
	virtual bool ActivatePreset(int32 p_vID) {return true;};
	bool CopyTransParams(EMVideoTransition* p_opTrans);

	virtual void SetSettings(int32* p_vpSettings);
	virtual void GetSettings(int32* p_vpSettings);

	bool LoadData(EMProjectDataLoader* p_opLoader);
	bool SaveData(EMProjectDataSaver* p_opSaver);

	bool LoadSetting(EMProjectDataLoader* p_opLoader);
	bool SaveSetting(EMProjectDataSaver* p_opSaver);

	virtual void* GetNativeEffect() const {return NULL;};
	virtual void* GetDefaultProperties();

	virtual EMPlugin* Clone();

protected:
private:
	EMDialogParameters m_oDefaultParams;
};

#endif