#include "EMMediaMatrix.h"

#include "EMMediaIDManager.h"
#include "EMBeMediaUtility.h"

#include <algorithm>

EMMediaMatrix::EMMediaMatrix()
{
	m_vID = EMMediaIDManager::MakeID();
}

EMMediaMatrix::~EMMediaMatrix() //Deleted by MediaProject
{
}

int32 EMMediaMatrix::GetID() const
{
	return m_vID;
}

bool EMMediaMatrix::AddToColumn(int64 p_vColNum, EMBufferDestination* p_opSlot)
{
	if(ExistsInColumn(p_opSlot) >= 0)
		return false;

	if(p_vColNum == m_oColumns.size())
		m_oColumns.push_back(EM_new list<EMBufferDestination*>());
			
	list<EMBufferDestination*>* opTargetColumn = GetColumn(p_vColNum);
	if(opTargetColumn == NULL)
		return false;
	
	opTargetColumn -> push_back(p_opSlot);
	return true;
}

int64 EMMediaMatrix::ExistsInColumn(const EMBufferDestination* p_opSlot)
{
	return ExistsInColumn(p_opSlot -> GetID());
}

int64 EMMediaMatrix::ExistsInColumn(int32 p_vDestinationID)
{
	for(int64 vListNum = 0; vListNum < m_oColumns.size(); vListNum++)
	{
		// Retrieve the current column (e.g. list) from the column-list
		list<EMBufferDestination*>* opCurrentColumn = GetColumn(vListNum);
		if(opCurrentColumn == NULL)
			return -1;
			
		// Search through the current column (e.g. list)
		list<EMBufferDestination*>::const_iterator opSlot = NULL;
		for(opSlot = opCurrentColumn -> begin(); opSlot != opCurrentColumn -> end(); opSlot++)
			if((*opSlot) -> GetID() == p_vDestinationID)
				return vListNum;
	}
	
	return -1;
}

list<EMBufferDestination*>* EMMediaMatrix::GetColumn(int64 p_vColNum)
{
	int64 vListNum = 0;
	list<list<EMBufferDestination*>*>::const_iterator opIterator;
	for(opIterator = m_oColumns.begin(); opIterator != m_oColumns.end(); opIterator++, vListNum++)
		if(vListNum == p_vColNum)
			return (*opIterator);
	
	return NULL;
}

bool EMMediaMatrix::Connect(int32 p_vSourceID, int32 p_vTargetID)
{
	int64 vSourceColNum = ExistsInColumn(p_vSourceID);
	int64 vTargetColNum = ExistsInColumn(p_vTargetID);
	
	if(vSourceColNum == -1 || vTargetColNum == -1)
		EMDebugger("ERROR! Could not find source destination and/or target destination in matrix!");
		
	if(vSourceColNum == vTargetColNum)
		EMDebugger("ERROR! You can't connect a slot in one column to a slot in the same column (feedback)!");
	
	if(vSourceColNum > vTargetColNum)
		EMDebugger("ERROR! You can't connect a slot in one column to a slot in a previous column (loop-back)!");
		
	list<EMBufferDestination*>* opSourceColumn = GetColumn(vSourceColNum);
	list<EMBufferDestination*>* opTargetColumn = GetColumn(vTargetColNum);
	EMBufferDestination* opSource = NULL;
	EMBufferDestination* opTarget = NULL;
	
	list<EMBufferDestination*>::const_iterator opIterator;
	for(opIterator = opSourceColumn -> begin(); opIterator != opSourceColumn -> end(); opIterator++)
	{
		if((*opIterator) -> GetID() == p_vSourceID)
		{
			opSource = (*opIterator);
			break;
		}
	}
	if(opSource == NULL)
		EMDebugger("ERROR! Could not find source-destination in column after all!!");
	
	for(opIterator = opTargetColumn -> begin(); opIterator != opTargetColumn -> end(); opIterator++)
	{
		if((*opIterator) -> GetID() == p_vTargetID)
		{
			opTarget = (*opIterator);
			break;
		}
	}
	if(opSource == NULL)
		EMDebugger("ERROR! Could not find target-destination in column after all!!");

	opSource -> SetDestination(opTarget);
	return true;	
}

















