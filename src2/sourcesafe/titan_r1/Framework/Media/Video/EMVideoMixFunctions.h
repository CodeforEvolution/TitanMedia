/*******************************************************
* Creator: Jesper Svensson
* Portability: Non-Native
*-------------------------------------------------------
* Created first: 2001-06-21
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_VIDEO_MIX_FUNCTIONS
#define __EM_VIDEO_MIX_FUNCTIONS

#include "EMMediaGlobals.h"
#include "EMMediaDataBuffer.h"

#include <list>

class EMVideoMixFunctions 
{
public:
	~EMVideoMixFunctions();
	static EMMediaDataBuffer* VideoPrioritizer(list<EMMediaDataBuffer*>* p_opList);
private:
	EMVideoMixFunctions();
};

#endif
