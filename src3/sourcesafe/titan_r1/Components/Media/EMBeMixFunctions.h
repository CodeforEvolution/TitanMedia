/*******************************************************
* Creator: Richard Kronfalt
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS 

#ifndef __EM_BE_MIX_FUNCTIONS
#define __EM_BE_MIX_FUNCTIONS

#include "EMMediaGlobals.h"
#include "EMMediaDataBuffer.h"
#include <Bitmap.h>

#include <MediaKit.h>
#include <list>

class BMediaRoster;

class EMBeMixFunctions 
{
public:
	~EMBeMixFunctions();
	static EMMediaDataBuffer* VideoPrioritizer(list<EMMediaDataBuffer*>* p_opList);

private:
	EMBeMixFunctions();
	static BBitmap* myPicture;
	static bool agust;
	static bool svaj;
	static float transparency;
	static int64 counter;
};

#endif

#endif
