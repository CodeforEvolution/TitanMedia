#ifndef __EM_PLUGIN_CHAIN
#define __EM_PLUGIN_CHAIN

#include "EMMediaItemContainer.h"
#include "EMPlugin.h"
#include "EMBufferDestination.h"
#include "EMMediaDataBuffer.h"
#include "EMAudioMixFunctions.h"

struct EMWinPluginWrapperInterface;

class EMPluginChain : public EMMediaItemContainer<EMPlugin>, public EMBufferDestination
{
public:
	virtual ~EMPluginChain();

	EMMediaType GetType() const;
	EMPluginType GetPluginType() const;
	int32 GetID() const;
	virtual bool InitCheckE() = 0;
	virtual bool PrepareToAddLast(EMPlugin* p_opPlugin) = 0;
	virtual bool PrepareToInsertBefore(EMPlugin* p_opPlugin, EMPlugin* p_opReferencePlugin) = 0;

	bool IsInitialized() const;

	bool PrepareToPlayE();
	EMMediaDataBuffer* ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList);

	bool AddEffectLast(EMPlugin* p_opPluginToAdd);
	bool InsertEffectBefore(EMPlugin* p_opPluginToInsert, int32 p_vReferencePluginID);

protected:
	EMPluginChain(EMMediaType p_vMediaType, EMPluginType p_ePluginType);
	void SetInitialized();

private:
	EMMediaType m_vType;
	EMPluginType m_ePluginType;
	int32 m_vID;
	bool m_vIsInitialized;
	BufferMixerFunction m_fpMixFunction;
	EMWinPluginWrapperInterface* m_opDirectXPluginInterface;
};

#endif