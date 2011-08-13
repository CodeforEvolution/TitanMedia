/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_OUTPUT_REPOSITORY
#define __EM_OUTPUT_REPOSITORY

#include "EMRealtimeOutputDescriptor.h"
#include "EMMediaItemContainer.h"
#include "EMListener.h"

class EMMediaSystemInspector;
class EMListenerRepository;
class EMProjectDataSaver;
class EMProjectDataLoader;

class EMOutputRepository : public EMMediaItemContainer<EMRealtimeOutputDescriptor>, public EMListener
{
public:
	static EMOutputRepository* Instance();
	static void Delete();
	bool InitCheckE();
	bool PrepareToPlayE();
	int32 GetDefaultOutput(EMMediaType p_eType);
	bool StartE();
	bool StopE();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	EMRealtimeOutputDescriptor* FindClockMaster();
	EMRealtimeOutputDescriptor* SearchForName(string p_oName);

	void OnItemAdd(); //Inherited from EMMediaItemContainer. Added by Richard 010821
	void OnItemRemove(); //Inherited from EMMediaItemContainer. Added by Richard 010821
	bool ClearData();
	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool LoadData(EMProjectDataLoader* p_opLoader);

	bool SetAsActiveMIDIOutput(int32 p_vID);

private:
	static EMOutputRepository* m_opInstance;
	virtual ~EMOutputRepository();
	EMOutputRepository();
	int32 m_vID;
	bool m_vIsInitialized;
};

#endif
