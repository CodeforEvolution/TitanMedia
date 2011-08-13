/*******************************************************
* Creator: Richard Kronfält
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_ITEM_CONTAINER
#define __EM_ITEM_CONTAINER

//#include "EMSemaphore.h"

#include <list>

template<class T> 
class EMItemContainer
{
public:
	EMItemContainer();
	virtual ~EMItemContainer();

	bool Add(T* item);
	T* Current();
	T* First();
	T* Last();
	bool Find(T* item);
	T* Find(uint64 p_oId);
	void Next();
	T* Remove(T* item);
	void Rewind();
	int64 Size();

	virtual bool LockContainer();
	virtual void UnlockContainer();

protected:

private:
	list<T*>::const_iterator __m_opIterator;
	list<T*>* __m_opMediaItemList;	
//	EMSemaphore* __m_opSemaphore;
};

#include "EMItemContainer.cpp"

#endif

