/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_RECT
#define __EM_RECT

class __declspec(dllexport) EMRect
{
public:
	EMRect(int32 p_vLeft, int32 p_vTop, int32 p_vRight, int32 p_vBottom);
	EMRect();
	EMRect(const EMRect& p_oRect);
	bool Contains(int32 p_vX, int32 p_vY);
	bool Contains(EMRect p_oRect);
	int32 GetHeight();
	int32 GetWidth();
	void InsetBy(int32 p_vX, int32 p_vY);
	bool Intersects(const EMRect p_oRect) const;
	bool IsValid() const;
	void OffsetBy(int32 p_vX, int32 p_vY);
	void OffsetTo(int32 p_vX, int32 p_vY);
	EMRect operator |(EMRect p_oMergingRect);
	EMRect operator ^(EMRect p_oRect) const;
	EMRect operator &(EMRect p_oRect) const;
	bool operator ==(EMRect p_oRect) const;
	bool operator !=(EMRect p_oRect) const;
	void Print(); // Debug stuff

public:
	int32 m_vLeft; // Public member variables are allowed in a container class
	int32 m_vTop;
	int32 m_vRight;
	int32 m_vBottom;
};

#endif

