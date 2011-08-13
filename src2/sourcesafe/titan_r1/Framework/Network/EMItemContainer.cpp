#include "EMItemContainer.h"

#ifndef __EM_ITEM_CONTAINER_CPP
#define __EM_ITEM_CONTAINER_CPP

#include <algorithm>

template<class T> EMItemContainer<T>::EMItemContainer() 
	:	__m_opMediaItemList(NULL)
{
	try
	{
		__m_opMediaItemList = EM_new list<T*>();
		__m_opIterator = __m_opMediaItemList -> begin();
//		__m_opSemaphore = EMSemaphore::CreateEMSemaphore();
	}
	catch(...)
	{
//		EMDebugger("ERROR! Exception in constructor!");
	}
}

template<class T> EMItemContainer<T>::~EMItemContainer()
{
//	__m_opSemaphore -> Acquire();
	try
	{
		list<T*>::const_iterator oIterator;
		for(oIterator = __m_opMediaItemList -> begin(); oIterator != __m_opMediaItemList -> end(); oIterator++)
			delete (*oIterator);
		
		delete __m_opMediaItemList;
		__m_opMediaItemList = NULL;
	}
	catch(...)
	{
//		EMDebugger("ERROR! An exception occured while deconstructing EMItemContainer");
	}
//	__m_opSemaphore -> Release();
//	delete __m_opSemaphore;
}

template<class T> bool EMItemContainer<T>::Add(T* item)
{
	if(Find(item))
	{
//		eo << "ERROR! Object could not be added to item container: Already exists in the container!" << ef;
		return false;
	}
	__m_opMediaItemList -> push_back(item);
	return true;
}

template<class T> T* EMItemContainer<T>::Current()
{
	if(__m_opMediaItemList == NULL)
		return NULL;
	if(__m_opIterator == __m_opMediaItemList -> end())
		return NULL;
	T* opReturn = *__m_opIterator;
	return opReturn;
}

template<class T> T* EMItemContainer<T>::First()
{
	if(__m_opMediaItemList == NULL)
		return NULL;
	if(__m_opMediaItemList -> size() > 0)
	{
		return __m_opMediaItemList -> front();
	}
	return NULL;
}

template<class T> T* EMItemContainer<T>::Last()
{
	if(__m_opMediaItemList == NULL)
		return NULL;
	if(__m_opMediaItemList -> size() > 0)
	{
		return __m_opMediaItemList -> back();
	}
	return NULL;
}

template<class T> bool EMItemContainer<T>::Find(T* item)
{
	if(__m_opMediaItemList == NULL) 
		return false;
	list<T*>::iterator oIterator = find(__m_opMediaItemList -> begin(), __m_opMediaItemList -> end(), item);
	if(oIterator != __m_opMediaItemList -> end())
		return true;
	return false;
}

template<class T> T* EMItemContainer<T>::Find(uint64 p_oId)
{
	list<T*>::iterator oIterator = __m_opMediaItemList -> begin();

	while((oIterator != __m_opMediaItemList -> end()) && ((*oIterator) -> GetId() != p_oId))
		++oIterator;

	if(oIterator == __m_opMediaItemList -> end())
		return NULL;

	return *oIterator;
}



template<class T> void EMItemContainer<T>::Next()
{
	__m_opIterator++;
}

template<class T> T* EMItemContainer<T>::Remove(T* item)
{
	if(__m_opMediaItemList == NULL)
		return NULL;
	list<T*>::const_iterator oIterator;
	for(oIterator = __m_opMediaItemList -> begin(); oIterator != __m_opMediaItemList -> end(); oIterator++)
		if((*oIterator) == item)
		{
			__m_opMediaItemList -> remove((*oIterator));
			T* opReturn = *oIterator;
			return opReturn;
		}
	return NULL;
}

template<class T> void EMItemContainer<T>::Rewind()
{
	if(__m_opMediaItemList != NULL)
		__m_opIterator = __m_opMediaItemList -> begin();
}

template<class T> int64 EMItemContainer<T>::Size()
{
	if(__m_opMediaItemList == NULL)
		return 0;
	int64 vSize = __m_opMediaItemList -> size();
	return vSize;
}

template<class T> bool EMItemContainer<T>::LockContainer()
{
//	__m_opSemaphore -> Acquire();
	return true;
}

template<class T> void EMItemContainer<T>::UnlockContainer()
{
//	__m_opSemaphore -> Release();
}

#endif