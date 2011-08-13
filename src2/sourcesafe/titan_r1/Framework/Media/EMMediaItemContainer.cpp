#include "EMMediaItemContainer.h"

#ifndef __EM_MEDIA_ITEM_CONTAINER_CPP
#define __EM_MEDIA_ITEM_CONTAINER_CPP

#include "EMBeMediaUtility.h"
#include "EMMediaIDManager.h"

#include <algorithm>

template<class T> EMMediaItemContainer<T>::EMMediaItemContainer(EMMediaType p_eType) 
	:	m_eType(p_eType),
		__m_opMediaItemList(NULL)
{
	try
	{
		__m_opMediaItemList = EM_new list<T*>();
		__m_opIterator = __m_opMediaItemList -> begin();
		string oUniqueName = string("EMSemaphore") + EMMediaIDManager::MakeUniqueString();
		__m_opSemaphore = EMSemaphore::CreateEMSemaphore(oUniqueName.c_str());
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in constructor!");
	}
}

template<class T> EMMediaItemContainer<T>::~EMMediaItemContainer()
{
	__m_opSemaphore -> Acquire();
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
		EMDebugger("ERROR! An exception occured while deconstructing EMMediaItemContainer");
	}
	__m_opSemaphore -> Release();
	delete __m_opSemaphore;
}

template<class T> bool EMMediaItemContainer<T>::Add(T* item)
{
	if((m_eType & (item -> GetType())) == 0 && m_eType != 0)
	{
		EMDebugger("ERROR! Object could not be added to item container: Wrong type!");
//		exit(0);
		return false;
	}
	if(Find(item))
	{
		eo << "ERROR! Object could not be added to item container: Already exists in the container!" << ef;
		return false;
	}
	int32 vIDOfPushedBackItem = item -> GetID();
	__m_opMediaItemList -> push_back(item);
	OnItemAdd();
	return true;
}

template<class T> T* EMMediaItemContainer<T>::Current()
{
	if(__m_opMediaItemList == NULL)
		return NULL;
	if(__m_opIterator == __m_opMediaItemList -> end())
		return NULL;
	T* opReturn = *__m_opIterator;
	return opReturn;
}

template<class T> T* EMMediaItemContainer<T>::First()
{
	if(__m_opMediaItemList == NULL)
		return NULL;
	if(__m_opMediaItemList -> size() > 0)
	{
		return __m_opMediaItemList -> front();
	}
	return NULL;
}

template<class T> T* EMMediaItemContainer<T>::First(EMMediaType p_eType)
{
	if(__m_opMediaItemList == NULL)
		return NULL;
	if(__m_opMediaItemList -> size() > 0)
	{
		list<T*>::const_iterator oIterator;
		for(oIterator = __m_opMediaItemList -> begin(); oIterator != __m_opMediaItemList -> end(); oIterator++)
			if((*oIterator) -> GetType() == p_eType)
				return *oIterator;
		return NULL;
	}
	return NULL;
}

template<class T> T* EMMediaItemContainer<T>::Last()
{
	if(__m_opMediaItemList == NULL)
		return NULL;
	if(__m_opMediaItemList -> size() > 0)
	{
		return __m_opMediaItemList -> back();
	}
	return NULL;
}

template<class T> T* EMMediaItemContainer<T>::Last(EMMediaType p_eType)
{
	if(__m_opMediaItemList == NULL)
		return NULL;
	if(__m_opMediaItemList -> size() > 0)
	{
		T* opEvent = NULL;
		list<T*>::const_iterator oIterator;
		for(oIterator = __m_opMediaItemList -> begin(); oIterator != __m_opMediaItemList -> end(); oIterator++)
			if((*oIterator) -> GetType() == p_eType)
				opEvent = *oIterator;
		return opEvent;
	}
	return NULL;
}

template<class T> bool EMMediaItemContainer<T>::Find(T* item)
{
	if(__m_opMediaItemList == NULL) 
		return false;
	list<T*>::iterator oIterator = find(__m_opMediaItemList -> begin(), __m_opMediaItemList -> end(), item);
	if(oIterator != __m_opMediaItemList -> end())
		return true;
	return false;
}

template<class T> T* EMMediaItemContainer<T>::Find(int32 p_vID)
{
	if(__m_opMediaItemList == NULL)
		return NULL;
	list<T*>::const_iterator oIterator;
	for(oIterator = __m_opMediaItemList -> begin(); oIterator != __m_opMediaItemList -> end(); oIterator++)
	{
		if((*oIterator) -> GetID() == p_vID)
		{
			T* opReturn = *oIterator;
			return opReturn;
		}
	}
	return NULL;
}

template<class T> void EMMediaItemContainer<T>::Next()
{
	__m_opIterator++;
}

template<class T> T* EMMediaItemContainer<T>::Remove(T* item)
{
	if(__m_opMediaItemList == NULL)
		return NULL;
	list<T*>::const_iterator oIterator;
	for(oIterator = __m_opMediaItemList -> begin(); oIterator != __m_opMediaItemList -> end(); oIterator++)
		if((*oIterator) == item)
		{
			__m_opMediaItemList -> remove((*oIterator));
			T* opReturn = *oIterator;
			OnItemRemove();
			return opReturn;
		}
	return NULL;
}

template<class T> void EMMediaItemContainer<T>::Rewind()
{
	if(__m_opMediaItemList != NULL)
		__m_opIterator = __m_opMediaItemList -> begin();
}

template<class T> T* EMMediaItemContainer<T>::Remove(int32 p_vID)
{
	if(__m_opMediaItemList == NULL)
		return NULL;

	list<T*>::const_iterator oIterator;
	for(oIterator = __m_opMediaItemList -> begin(); oIterator != __m_opMediaItemList -> end(); oIterator++)
	{
		if((*oIterator) -> GetID() == p_vID)
		{
			__m_opMediaItemList -> remove((*oIterator));
			OnItemRemove();
			return (*oIterator);
		}
	}
	return NULL;
}

template<class T> int64 EMMediaItemContainer<T>::Size()
{
	if(__m_opMediaItemList == NULL)
		return 0;
	int64 vSize = __m_opMediaItemList -> size();
	return vSize;
}

//Please be careful with this, will ya!
template<class T> list<T*>* EMMediaItemContainer<T>::GetList()
{
	return __m_opMediaItemList;
}

template<class T> bool EMMediaItemContainer<T>::LockContainer()
{
	__m_opSemaphore -> Acquire();
	return true;
}

template<class T> void EMMediaItemContainer<T>::UnlockContainer()
{
	__m_opSemaphore -> Release();
}

#endif