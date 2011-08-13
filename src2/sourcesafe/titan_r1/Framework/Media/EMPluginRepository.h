/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
*
* EMPluginRepository contains all plugin information in
* the system. It doesn't hold any instantiated 
* ready-to-use plugins, but rather descriptions about
* each plugin and the possibility to get those
* representations (EMPluginEntry:s) and instantiate the
* platform-dependent plugin from there.
*
* There should be exactly one (1) plugin repository for
* the entire system.
*
*******************************************************/

#ifndef __EM_PLUGIN_REPOSITORY
#define __EM_PLUGIN_REPOSITORY

#include "EMGlobals.h"

#include "EMMediaItemContainer.h"
#include "EMPluginEntry.h"
#include "EMListener.h"

class EMListenerRepository;

class EMPluginRepository : public EMMediaItemContainer<EMPluginEntry>, public EMListener
{
public:
	static EMPluginRepository* Instance();
	static void Delete();
	void OnItemAdd(); //Inherited from EMMediaItemContainer. Added by Richard 010821
	void OnItemRemove(); //Inherited from EMMediaItemContainer. Added by Richard 010821
	void ResetEntries();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);

protected:
	EMPluginRepository();
	virtual ~EMPluginRepository();

private:
	static EMPluginRepository* m_opInstance;
};

#endif