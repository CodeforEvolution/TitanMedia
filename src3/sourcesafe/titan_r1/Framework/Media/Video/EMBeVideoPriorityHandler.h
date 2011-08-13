/*******************************************************
* Creator: Jesper Svensson
* Portability: Native
*-------------------------------------------------------
*
* Jesper Svensson, 00-12-13
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_VIDEO_PRIORITY_HANDLER
#define __EM_BE_VIDEO_PRIORITY_HANDLER

#include "EMBufferDestination.h"
#include "EMMediaDataBuffer.h"

class EMBeVideoPriorityHandler : public EMBufferDestination
{
public:
	EMBeVideoPriorityHandler();
	~EMBeVideoPriorityHandler();
	bool PrepareToPlayE();
	EMMediaDataBuffer* ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList);

protected:
private:
	BufferMixerFunction m_fpMixFunction;
};

#endif

#endif
