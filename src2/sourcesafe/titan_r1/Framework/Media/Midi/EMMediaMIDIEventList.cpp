#include "EMMediaMIDIEventList.h"

#include "EMMediaMIDIEvent.h"
#include "EMProjectDataSaver.h"
#include "EMMediaIDManager.h"
#include "EMSemaphore.h"

EMMediaMIDIEventList::EMMediaMIDIEventList()
	:	m_opFirst(NULL),
		m_opReadPosition(NULL),
		m_opWritePosition(NULL),
		m_opListProtectionSemaphore(NULL),
		m_vLocked(false),
		m_vNumItems(0)
{
	m_opListProtectionSemaphore = EMSemaphore::CreateEMSemaphore(EMMediaIDManager::MakeUniqueString()	, 1, 1);
}

EMMediaMIDIEventList::~EMMediaMIDIEventList()
{
	delete m_opFirst; //Will recursively delete all other items in the linked list.
	delete m_opListProtectionSemaphore;
}

void EMMediaMIDIEventList::Lock()
{
	m_opListProtectionSemaphore -> Acquire();
	m_vLocked = true;
}

void EMMediaMIDIEventList::Unlock()
{
	m_opListProtectionSemaphore -> Release();
	m_vLocked = false;
}

#ifdef _DEBUG
void EMMediaMIDIEventList::Show()
{
	eo << "====================[ EVENTS IN REPOSITORY ]====================" << ef;
	EMMediaMIDIEventItem* opItem = m_opFirst;
	while(opItem != NULL) 
	{
		eo << (int) *(opItem -> m_opEvent -> m_vpStart) << ". Type = " << opItem -> m_opEvent -> m_vBaseType << ". Data1 = " << opItem -> m_opEvent -> m_vData1 << ". Data2 = " << opItem -> m_opEvent -> m_vData2 << ef;
		opItem = opItem -> m_opNext;
	}
}
#endif 

void EMMediaMIDIEventList::Clear()
{
	//First, make sure we remove the event pointers from the items, so we dont delete the events when we clear the list items...
//	_ASSERT(m_vLocked);
	while(m_opFirst != NULL)
	{
		m_opFirst -> m_opEvent = NULL;
		m_opFirst = m_opFirst -> m_opNext;
	}

	delete m_opFirst; //Then delete the items without touching the events (they must be deleted without Clear()ing)
	m_opFirst = NULL;
	m_opReadPosition = NULL;
	m_opWritePosition = NULL;
	m_vNumItems = 0;
}

void EMMediaMIDIEventList::Remove(EMMediaMIDIEvent* p_opEvent)
{
	if(m_opFirst == NULL)
		return;

	EMMediaMIDIEventItem* opItem = m_opFirst;
	while(opItem != NULL)
	{
		EMMediaMIDIEvent* opEvent = opItem -> m_opEvent;
		if(opEvent == p_opEvent)
		{
			if(opItem -> m_opPrevious != NULL && opItem -> m_opNext != NULL)
			{	//if previous is set, and next is also set... (eg this is an item in the middle somewhere)
				opItem -> m_opPrevious -> m_opNext = opItem -> m_opNext;
				opItem -> m_opEvent = NULL;
				delete opItem;
				return;
			}
			else if(opItem == m_opFirst)
			{	//if this is the first item
				if(opItem -> m_opNext != NULL)
				{	//and next is set...
					m_opFirst = opItem -> m_opNext;
					opItem -> m_opEvent = NULL;
					delete opItem;
					return;
				}
				else
				{	//And there is no next (eg there is only 1 event in the list)...
					m_opFirst = NULL;
					opItem -> m_opEvent = NULL;
					delete opItem;
					return;
				}
			}
			else if(opItem -> m_opNext == NULL)
			{	//if this is the last event... (there must be an item prior to this one, 
				//because otherwise this would have been the first event (eg previous if-statement would be true)
				opItem -> m_opPrevious -> m_opNext = NULL;
				opItem -> m_opEvent = NULL;
				delete opItem;
				return;
			}
		}
		opItem = opItem -> m_opNext;
	}
	return;
}

void EMMediaMIDIEventList::Store(EMMediaMIDIEvent* p_opEvent)
{
//	_ASSERT(m_vLocked);
	if(m_opFirst == NULL)
	{
		m_opFirst = m_opReadPosition = EM_new EMMediaMIDIEventItem(p_opEvent, NULL);
		m_opWritePosition = m_opFirst;
	}

	else
	{
		EMMediaMIDIEventItem* opPrevious = NULL;
		if(*(m_opWritePosition -> m_opEvent -> GetStart()) >= *(p_opEvent -> GetStart()))
			opPrevious = FindPreviousTo(p_opEvent);
		else
			opPrevious = m_opWritePosition;

		if(opPrevious == NULL)
		{
			EMMediaMIDIEventItem* opNew = EM_new EMMediaMIDIEventItem(p_opEvent, NULL);
			opNew -> m_opNext = m_opFirst;
			m_opFirst -> m_opPrevious = opNew;
			m_opFirst = opNew;
		}
		else
		{
			EMMediaMIDIEventItem* opNew = EM_new EMMediaMIDIEventItem(p_opEvent, opPrevious);
			opNew -> m_opNext = opPrevious -> m_opNext;
			opPrevious -> m_opNext = opNew;

			if(opNew -> m_opNext != NULL)
				opNew -> m_opNext -> m_opPrevious = opNew;

			m_opWritePosition = opNew;
		}
	}
	m_vNumItems++;
}

bool EMMediaMIDIEventList::SaveData(EMProjectDataSaver* p_opSaver)
{
	int64 vUndeletedEventCount = 0;
	EMMediaMIDIEventItem* opItem = m_opFirst;
	while(opItem != NULL)
	{
		if(! opItem -> m_opEvent -> IsObjectDeleted())
			vUndeletedEventCount++;
		opItem = opItem -> m_opNext;
	}

	//Save the size - but only count the events not marked as deleted!!
	p_opSaver -> SaveUInt64(vUndeletedEventCount);

	opItem = m_opFirst;
	while(opItem != NULL)
	{
		if(! opItem -> m_opEvent -> IsObjectDeleted())
			opItem -> m_opEvent -> SaveData(p_opSaver);
		opItem = opItem -> m_opNext;
	}
	return true;
}

void EMMediaMIDIEventList::Sort() 
{
	EMMediaMIDIEventItem** opArray = EM_new EMMediaMIDIEventItem*[m_vNumItems + 1];
	opArray[m_vNumItems] = NULL;
	EMMediaMIDIEventItem* opCurrent = m_opFirst;
	int64 vPos = 0;
	while(opCurrent != NULL)
	{
		opArray[vPos++] = opCurrent;
		opCurrent = opCurrent -> m_opNext;
	}

	int32 vOuterLoopStart = m_vNumItems - 1;
	for(register int32 vOuter = vOuterLoopStart; vOuter > 0; vOuter--)
	{
		int32 vInnerLoopStart = vOuter - 1;
		for(register int32 vInner = vInnerLoopStart; vInner >= 0; vInner--)
			if(*(opArray[vInner] -> m_opEvent -> m_vpStart) > *(opArray[vOuter] -> m_opEvent -> m_vpStart))
			{
				EMMediaMIDIEventItem* opItem = opArray[vInner];
				opArray[vInner] = opArray[vOuter];
				opArray[vOuter] = opItem;
			}
	}

	EMMediaMIDIEventItem* opLast = NULL;
	vPos = 0;
	while(opArray[vPos] != NULL)
	{
		opArray[vPos] -> m_opPrevious = opLast;
		opLast = opArray[vPos];
		opArray[vPos] -> m_opNext = (opArray[vPos + 1]);
		vPos++;
	}
	m_opFirst = opArray[0];
	delete opArray; 
}

void EMMediaMIDIEventList::MoveEvent(EMMediaMIDIEvent* p_opEvent, int64 p_vNewTime)
{
	//If:
	//	-	The event's new time is unequal to its old time, AND
	//	-	a)	The new time is less than the event's (in its old position) "left" (earlier) neighbour's starting time, or
	//	-	b)	The new time is more than the event's (in its old position) "right" (next) neighbour's starting time
	//
	//	E.g. if the NEW time makes it necessary to move the event-item (which is an item in a sorted linked list).

//	_ASSERT(m_vLocked);
	if(m_opFirst == NULL)
		return;

#ifdef DEBUG
	Show();
#endif

	EMMediaMIDIEventItem* opPrevious = FindPreviousTo(p_opEvent);
	if(opPrevious != NULL && opPrevious -> m_opNext -> m_opNext != NULL)
	{
		EMMediaMIDIEventItem* opThis = opPrevious -> m_opNext;
		if(p_vNewTime != *(p_opEvent -> m_vpStart) &&
			(p_vNewTime < *(opThis -> m_opPrevious -> m_opEvent -> m_vpStart) ||
			p_vNewTime > *(opThis -> m_opNext -> m_opEvent -> m_vpStart)))
		{
			opThis -> m_opPrevious -> m_opNext = opThis -> m_opNext;
			*(opThis -> m_opEvent -> m_vpStart) = p_vNewTime;
			opThis -> m_opEvent = NULL; //Make sure we reset the pointer before we delete the item. We do NOT want the delete to deallocate the event too - just the item!
			if(m_opWritePosition == opThis)
			{
				m_opWritePosition = opThis -> m_opPrevious;
			}
			delete opThis;
			Store(p_opEvent);
		}
	}
	else
	{
		//Either the event is the very first or the very last event, or the event does not exist in the repository (the latter being much less likely)
		if(m_opFirst -> m_opEvent == p_opEvent)
		{
			if(p_vNewTime < *(m_opFirst -> m_opEvent -> m_vpStart))
			{
				*(m_opFirst -> m_opEvent -> m_vpStart) = p_vNewTime;
			}
			else
			{
				if(m_opFirst -> m_opNext != NULL)
				{
					EMMediaMIDIEventItem* opThis = m_opFirst;
					m_opFirst -> m_opEvent = NULL;
					m_opFirst = m_opFirst -> m_opNext;
					delete opThis;

					*(p_opEvent -> m_vpStart) = p_vNewTime;
					Store(p_opEvent);
				}
				else
				{
					*(m_opFirst -> m_opEvent -> m_vpStart) = p_vNewTime;
				}
			}
		}
		else
			EMDebugger("ERROR! Could not find event in event repository!");
	}

#ifdef DEBUG
	Show();
#endif
}

//Algorithm:
/*

  INIT:
  item = first item in repository

  1)	fetch the time of the previous item
		- if previous is null, use time of current item
  2)	fetch the time of the current item
  3)	fetch the time of the next item
		- if next is null, use time of current item

  4)	if current start is later than next item's start, we've found
		an irregularity and need sorting. 
		- Store the current item as X and the next item as Y
		- Go to step 7

  5)	if current start is earlier than the previous item's start, we've found
		an irregularity and need sorting. 
		- Store the previous item as X and the current item as Y
		- Go to step 7

  6)	if start times are in order, set item to point to the next item.
		- Go to step 1

  THE SWAPPING PROCEDURE BEGINS HERE (swap two items in the list)
  7)	store the item prior to X

  8)	store the item after Y

  9)	set X's previous item to point to Y

  10)	set X's next item to point to Y's next item

  11)	set Y's previous item to point to X's previous item

  12)	set Y's next item to point to X

  13)	If X is the root/start of the linked list, then
		change the start to point to Y instead (since we now have swapped X and Y

  14)	set 'item' to point to Y (which now is ahead of X, which it wasn't before).

  15)	Goto step 1
  */
void EMMediaMIDIEventList::Recalculate()
{
//	_ASSERT(m_vLocked);

	EMMediaMIDIEventItem* opItem = m_opFirst;
	while(opItem != NULL)
	{
		int64 vPreviousStart = (opItem -> m_opPrevious != NULL ? *(opItem -> m_opPrevious -> m_opEvent -> m_vpStart) : *(opItem -> m_opEvent -> m_vpStart));
		int64 vCurrentStart = *(opItem -> m_opEvent -> m_vpStart);
		int64 vNextStart = (opItem -> m_opNext != NULL ? *(opItem -> m_opNext -> m_opEvent -> m_vpStart) : *(opItem -> m_opEvent -> m_vpStart));

		EMMediaMIDIEventItem* opX = NULL; 
		EMMediaMIDIEventItem* opY = NULL;

		if(vCurrentStart > vNextStart)
		{
			opX = opItem;
			opY = opItem -> m_opNext;
		}
		else if(vCurrentStart < vPreviousStart)
		{
			opX = opItem -> m_opPrevious;
			opY = opItem;
		}

		if(opX != NULL || opY != NULL)
		{
			EMMediaMIDIEventItem* opOldXp = (opX != NULL ? opX -> m_opPrevious : NULL);
			EMMediaMIDIEventItem* opOldYn = (opY != NULL ? opY -> m_opNext : NULL);

			if(opX != NULL) opX -> m_opPrevious = opY;
			if(opX != NULL) opX -> m_opNext = opOldYn;

			if(opY != NULL) opY -> m_opPrevious = opOldXp;
			if(opY != NULL) opY -> m_opNext = opX;

			if(opOldXp != NULL) opOldXp -> m_opNext = opY;
			if(opOldYn != NULL) opOldYn -> m_opPrevious = opX;

			if(m_opFirst == opX)
				m_opFirst = opY;

			opItem = opY;
		}
		else
			opItem = opItem -> m_opNext;
	}
}

void EMMediaMIDIEventList::Reset()
{
//	_ASSERT(m_vLocked);
	m_opReadPosition = m_opFirst;
}

uint64 EMMediaMIDIEventList::Size()
{
//	_ASSERT(m_vLocked);
	return m_vNumItems;
}

EMMediaMIDIEvent* EMMediaMIDIEventList::First()
{
//	_ASSERT(m_vLocked);
	EMMediaMIDIEventItem* opItem = m_opFirst;
	while(opItem != NULL && opItem -> m_opEvent -> IsObjectDeleted())
		opItem = opItem -> m_opNext;
	return (opItem == NULL ? NULL : opItem -> m_opEvent);
} 

EMMediaMIDIEvent* EMMediaMIDIEventList::Last()
{
	EMMediaMIDIEventItem* opItem = LastItem();
	while(opItem != NULL && opItem -> m_opEvent -> IsObjectDeleted())
		opItem = opItem -> m_opPrevious;
	return (opItem == NULL ? NULL : opItem -> m_opEvent);
}

EMMediaMIDIEventItem* EMMediaMIDIEventList::FirstItem()
{
	return m_opFirst;
}

EMMediaMIDIEventItem* EMMediaMIDIEventList::LastItem()
{
//	_ASSERT(m_vLocked);
	EMMediaMIDIEventItem* opItem = m_opFirst;
	while(opItem != NULL)
	{
		if(opItem -> m_opNext == NULL)
			return opItem;
		opItem = opItem -> m_opNext;
	}
	return NULL;
}

EMMediaMIDIEvent* EMMediaMIDIEventList::operator [](int64 p_vTime)
{
//	_ASSERT(m_vLocked);

	/*while(*m_opReadPosition -> m_opEvent -> m_vpStart > p_vTime)
	{
		if(m_opReadPosition -> m_opPrevious != NULL)
		{
			if(*m_opReadPosition -> m_opPrevious -> m_opEvent -> m_vpStart > p_vTime)
			{
				m_opReadPosition = m_opReadPosition -> m_opPrevious;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}*/

	//m_opReadPosition = m_opFirst; //TEMPORARY!

	while(m_opReadPosition != NULL && (*m_opReadPosition -> m_opEvent -> m_vpStart < p_vTime || m_opReadPosition -> m_opEvent -> IsObjectDeleted() || m_opReadPosition -> m_opEvent -> IsMuted()))
		m_opReadPosition = m_opReadPosition -> m_opNext;
	if(m_opReadPosition != NULL)
	{
		if(m_opReadPosition -> m_opEvent -> IsObjectDeleted() || m_opReadPosition -> m_opEvent -> IsMuted())
			return NULL;
		else 
		{
			EMMediaMIDIEvent* opEvent = m_opReadPosition -> m_opEvent;
			m_opReadPosition = m_opReadPosition -> m_opNext;
			return opEvent;
		}
	}
	return NULL;
}

EMMediaMIDIEventItem* EMMediaMIDIEventList::FindPreviousTo(EMMediaMIDIEvent* p_opEvent)
{
//	_ASSERT(m_vLocked);
	int64 vWantedStart = 0;
	int64 vCurrentStart = 0;

	EMMediaMIDIEventItem* opCurrent = m_opFirst;
	EMMediaMIDIEventItem* opCandidate = NULL;
	for(;opCurrent != NULL;)
	{
		vWantedStart = *(p_opEvent -> m_vpStart);
		vCurrentStart = *(opCurrent -> m_opEvent -> m_vpStart);
		if(vCurrentStart <= vWantedStart && opCurrent -> m_opEvent != p_opEvent)
			opCandidate = opCurrent;
		if(vCurrentStart > vWantedStart)
			if(opCandidate != NULL) 
				return opCandidate;
			else 
				return NULL;
		opCurrent = opCurrent -> m_opNext;
	}
	return opCandidate;
}