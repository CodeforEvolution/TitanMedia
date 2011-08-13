#include "EMListenerRepository.h"
#include "EMListener.h"

#include <algorithm>

EMListenerRepository::EMListenerRepository()
{
}

EMListenerRepository::~EMListenerRepository()
{
}

// Adds a listener to the linked list of listeners
void EMListenerRepository::AddListener(EMListener* p_opListener)
{
	list<EMListener*>::iterator oIterator = find(m_oListenerRepository.begin(), m_oListenerRepository.end(), p_opListener);
	if(oIterator != m_oListenerRepository.end())
		return; // The listener is already added
	m_oListenerRepository.push_back(p_opListener);
}

// Returns true if the repository contains the listener
bool EMListenerRepository::ContainsListener(EMListener* p_opListener)
{
	list<EMListener*>::iterator oIterator = find(m_oListenerRepository.begin(), m_oListenerRepository.end(), p_opListener);
	if(oIterator == m_oListenerRepository.end())
		return false;
	return true;
}

list<EMListener*>* EMListenerRepository::GetListeners()
{
	return &m_oListenerRepository;
}

uint16 EMListenerRepository::GetListenerAmount()
{
	return m_oListenerRepository.size();
}

bool EMListenerRepository::Notify(uint32 p_vMessage)
{
	// Special case to allow deletion of this object from the receiver and listener removal	
	if(m_oListenerRepository.size() == 1)
	{
		EMListener* opListener = *(m_oListenerRepository.begin());
		return opListener -> MessageReceived(this, p_vMessage);
	}

	bool vHandled = false;
	list<EMListener*>::iterator oIterator = m_oListenerRepository.begin();
	while(oIterator != m_oListenerRepository.end())
	{
		if((*oIterator) -> MessageReceived(this, p_vMessage))
			vHandled = true;
		oIterator++;
	}
	if(vHandled) // If any listener handled the broadcasted message
	{
		return true;
	}
	else
		return false;
}

// It's up to the caller to insure that the list of listeners is valid
bool EMListenerRepository::Notify(list<EMListener*>* p_oListeners, uint32 p_vMessage)
{
	bool vHandled = false;
	list<EMListener*>::iterator oIterator = p_oListeners -> begin();
	while(oIterator != p_oListeners -> end())
	{
		if((*oIterator) -> MessageReceived(this, p_vMessage))
			vHandled = true;
		oIterator++;
	}
	if(vHandled) // If any listener handled the broadcasted message
	{
		return true;
	}
	else
		return false;
}

// Removes the listener from the repository
void EMListenerRepository::RemoveListener(EMListener* p_opListener)
{
	m_oListenerRepository.remove(p_opListener);
}

void EMListenerRepository::RemoveListeners()
{
	m_oListenerRepository.clear();
}