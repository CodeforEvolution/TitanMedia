/*******************************************************
* Creator: Richard Kronfalt
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#ifndef __EM_INPUT_DESCRIPTOR
#define __EM_INPUT_DESCRIPTOR

#include "EMMediaGlobals.h"
#include "EMDescriptor.h"

class EMInputDescriptor : public EMDescriptor
{
public:
	virtual ~EMInputDescriptor();
	int32 GetID() const;
	EMMediaType GetType() const;
	
protected:
	EMInputDescriptor(EMMediaType p_eType);
	
private:
	int32 m_vID;
	EMMediaType m_eType;
};

#endif
