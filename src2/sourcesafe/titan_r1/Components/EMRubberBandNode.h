/*******************************************************
* Creator: Jesper Svensson
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_RUBBER_BAND_NODE
#define __EM_RUBBER_BAND_NODE

class __declspec(dllexport) EMRubberBandNode
{
public:
	EMRubberBandNode();
	~EMRubberBandNode();
	SetPosition(int64 p_vPosition);
	SetValue(int32 p_vValue);
	int64 GetPosition();
	int32 GetValue();

private:
	int64 m_vPosition;
	int32 m_vValue;

};

#endif