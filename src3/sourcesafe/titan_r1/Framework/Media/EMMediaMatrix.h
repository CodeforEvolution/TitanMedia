/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_MEDIA_MATRIX
#define __EM_MEDIA_MATRIX

#include "EMBufferDestination.h"

#include <list>

class EMMediaMatrix 
{
public:
	EMMediaMatrix();
	~EMMediaMatrix();
	bool AddToColumn(int64 p_vColNum, EMBufferDestination* p_opSlot);
	bool Connect(int32 p_vSourceID, int32 p_vTargetID);
	int64 ExistsInColumn(const EMBufferDestination* p_opSlot);
	int64 ExistsInColumn(int32 p_vDestinationID);
	list<EMBufferDestination*>* GetColumn(int64 p_vColNum);
	int32 GetID() const;

private:
	//list<list<EMBufferDestination*>*>* m_opSlotColumns;
	list<list<EMBufferDestination*>*> m_oColumns;
	int32 m_vID;
};

#endif
