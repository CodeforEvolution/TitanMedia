/*******************************************************
* Creator: Richard Kronfält
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_MEDIA_ITEM_CONTAINER
#define __EM_MEDIA_ITEM_CONTAINER

#include "EMMediaGlobals.h"
#include "EMSemaphore.h"

#include <list>

template<class T> 
class EMMediaItemContainer
{
public:
	virtual ~EMMediaItemContainer();

	bool Add(T* item);
	T* Current();
	T* First();
	T* Last();
	T* First(EMMediaType p_eType);
	T* Last(EMMediaType p_eType);
	bool Find(T* item);
	T* Find(int32 p_vID);
	void Next();
	T* Remove(T* item);
	T* Remove(int32 p_vID);
	void Rewind();
	int64 Size();
	list<T*>* GetList(); //Be careful with this method, will yah!

	virtual bool LockContainer();
	virtual void UnlockContainer();

	virtual void OnItemAdd() = 0;
	virtual void OnItemRemove() = 0;

protected:
	EMMediaItemContainer(EMMediaType p_eType);

private:
	EMMediaType m_eType;
	list<T*>::const_iterator __m_opIterator;
	list<T*>* __m_opMediaItemList;	
	EMSemaphore* __m_opSemaphore;
};

#include "EMMediaItemContainer.cpp"

#endif

