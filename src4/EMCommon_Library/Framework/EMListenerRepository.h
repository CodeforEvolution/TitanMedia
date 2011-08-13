/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_LISTENER_REPOSITORY
#define __EM_LISTENER_REPOSITORY

#include <list>

class EMListener;

class __declspec(dllexport) EMListenerRepository
{
	public:
		virtual ~EMListenerRepository();
		virtual void AddListener(EMListener* p_opListener);
		virtual bool ContainsListener(EMListener* p_opListener);
		virtual list<EMListener*>* GetListeners();
		virtual uint16 GetListenerAmount();
		virtual bool Notify(uint32 p_vMessage);
		virtual bool Notify(list<EMListener*>* p_oListeners, uint32 p_vMessage);
		virtual void RemoveListener(EMListener* p_opListener);
		void RemoveListeners();

	protected:
		EMListenerRepository();

	protected:
		list<EMListener*> m_oListenerRepository;
};

#endif
