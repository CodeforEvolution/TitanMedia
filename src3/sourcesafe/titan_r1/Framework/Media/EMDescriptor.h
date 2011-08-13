/*******************************************************
* Creator: Richard Kronfalt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_DESCRIPTOR
#define __EM_DESCRIPTOR

#include <string>

class EMDescriptor
{
public:
	virtual ~EMDescriptor();
	string GetName() const;
	void SetName(const string p_oName);
protected:
	EMDescriptor();
	
private:
	string m_oName;
	bool m_vIsRenderer;
};

#endif