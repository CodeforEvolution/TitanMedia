/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-native
*-------------------------------------------------------
* 
*******************************************************/

#ifndef __EM_REALTIME_OUTPUT_DESCRIPTOR
#define __EM_REALTIME_OUTPUT_DESCRIPTOR

#include "EMOutputDescriptor.h"
#include "EMMediaDataBuffer.h"

#include <list>

class EMProjectDataLoader;
class EMProjectDataSaver;

class EMRealtimeOutputDescriptor : public EMOutputDescriptor
{
public:
	virtual ~EMRealtimeOutputDescriptor();
	void* GetMediaInput() const;
	virtual EMMediaDataBuffer* ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList) = 0;
	virtual bool PrepareToPlayE() = 0;
	virtual void StartE() = 0;
	virtual void StopE() = 0;
	virtual bool ClearData() = 0;
	virtual bool SaveData(EMProjectDataSaver* p_opSaver) = 0;
	virtual bool LoadData(EMProjectDataLoader* p_opLoader) = 0;
	
protected:
	EMRealtimeOutputDescriptor(EMMediaType p_eType);

private:
};

#endif

