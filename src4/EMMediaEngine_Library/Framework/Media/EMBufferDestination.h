/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
* 
* Base class specifying the contents of a buffer destination.
* It includes some basic methods for keeping track of
* the next destination, if it is a pre- or post-fader 
* destination, etc.
*
* Richard Kronfält, 00-11-28
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_BUFFER_DESTINATION
#define __EM_BUFFER_DESTINATION

#include "EMMediaGlobals.h"
#include "EMMediaDataBuffer.h"

#include <list>

class EMSemaphore;
class EMProjectDataSaver;
class EMProjectDataLoader;

class EMBufferDestination 
{
public:
	EMBufferDestination* GetDestination() const;
	int32 GetID() const;
	EMDestinationPosition GetPosition() const;
	int32 GetType() const;
	bool IsPreFader() const;
	virtual bool PrepareToPlayE() = 0;
	virtual EMMediaDataBuffer* ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList) = 0;
	virtual EMMediaDataBuffer* ProcessBufferE(EMMediaDataBuffer** p_opBufferList) { return NULL; };
	void SetDestination(EMBufferDestination* p_opDestination);
	void SetPreFader(bool p_vFlag);
	virtual ~EMBufferDestination();

	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool LoadData(EMProjectDataLoader* p_opLoader);

	void AddProcessingReference();
	void RemoveProcessingReference();
	uint32 GetProcessingReferenceCount();

	string GetDestinationName() const;

protected:
	EMBufferDestination(int32 p_vType, EMDestinationPosition p_ePosition, string p_oDebugDestinationName);
	EMBufferDestination* m_opDestination;

private:
	EMBufferDestination();
	int32 m_vID;
	bool m_vIsPostFader;
	int32 m_vType;
	EMSemaphore* m_opDestinationSemaphore;
	EMDestinationPosition m_ePosition;

	uint32 m_vProcessingReferenceCount;
	string m_oDestinationName;
};

#endif
