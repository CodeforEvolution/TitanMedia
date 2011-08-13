/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
* 
* An output descriptor is the representation of some output
* in a chain of media processors. Note that it inherits from
* EMBufferDestination!
*
* The output is physical, in the sence that it actually is
* a BMediaNode of some kind. As you can see, this class 
* therefor has a pointer to a EMBeOutputNode. 
* TODO: Should it have a pointer to an EMBeOutputNode?
* 		..Or should it have a pointer to a more general
*		media node, so it can be used as a descriptor for
*		for instance file-writer nodes as well?
*
* Richard Kronfält, 00-11-28
*******************************************************/

#ifndef __EM_OUTPUT_DESCRIPTOR
#define __EM_OUTPUT_DESCRIPTOR

#include "EMDescriptor.h"
#include "EMBufferDestination.h"
#include "EMMediaDataBuffer.h"

#include <list>

class EMOutputDescriptor : public EMDescriptor, public EMBufferDestination
{
public:
	virtual ~EMOutputDescriptor();
	EMMediaType GetType() const;
	virtual bool InitCheckE() = 0;
	bool IsInitialized() const;
	virtual void StartE() = 0;
	virtual void StopE() = 0;
	virtual bool PrepareToPlayE() = 0;
	virtual EMMediaDataBuffer* ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList) = 0;
	void SetClockMaster(bool p_vFlag);
	bool IsClockMaster() const;
	virtual bool RegisterAsClockMaster() = 0;
	virtual bool UnregisterAsClockMaster() = 0;

	
	void SetRenderOutput(bool p_vFlag);
	bool IsRenderOutput() const;

protected:
	EMOutputDescriptor(EMMediaType p_eType);
	bool m_vIsInitialized;

private:
	EMMediaType m_eType;
	bool m_vIsClockMaster;
	bool m_vIsRenderOutput;
};

#endif
