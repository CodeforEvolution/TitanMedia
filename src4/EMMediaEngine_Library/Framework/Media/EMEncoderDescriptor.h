/*******************************************************
* Creator: Jesper Svensson
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#ifndef __EM_ENCODER_DESCRIPTOR
#define __EM_ENCODER_DESCRIPTOR

#include "EMMediaGlobals.h"
#include "EMDescriptor.h"

class EMEncoderDescriptor : public EMDescriptor
{
public:
	virtual ~EMEncoderDescriptor();
	int32 GetID() const;
	EMMediaType GetType() const;
	
protected:
	EMEncoderDescriptor(EMMediaType p_eType);
	
private:
	int32 m_vID;
	EMMediaType m_eType;
};

#endif
