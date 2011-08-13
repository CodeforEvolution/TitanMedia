/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_INPUT_REPOSITORY
#define __EM_INPUT_REPOSITORY

#include "EMRealtimeInputDescriptor.h"
#include "EMMediaItemContainer.h"
#include "EMRecording.h"
#include "EMListener.h"
#include "EMListenerRepository.h"

#include <list>

class EMMediaSystemInspector;
class EMProjectDataSaver;
class EMProjectDataLoader;
class EMMediaMIDITrack;

class EMInputRepository : public EMMediaItemContainer<EMRealtimeInputDescriptor>, public EMListener
{
public:
	static EMInputRepository* Instance();
	static void Delete();
	list<EMRecording*>* GetLastRecordings();
	bool InitCheckE();
	
	bool StartPreviewE();
	bool StopPreviewE();
	bool PrepareToPlayE();
	bool StartE();
	bool StopE();
	
	int32 FindName(string p_oName);
	void OnItemAdd(); //Inherited from EMMediaItemContainer. Added by Richard 010821
	void OnItemRemove(); //Inherited from EMMediaItemContainer. Added by Richard 010821
	bool ClearData();
	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool LoadData(EMProjectDataLoader* p_opLoader);

	bool LockContainer();
	void UnlockContainer();

	bool SetAsActiveMIDIInput(int32 p_vID, int32 p_vOutputID);

	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);

private:
	EMInputRepository();
	~EMInputRepository();
	int32 m_vID;
	static EMInputRepository* m_opInstance;
	bool m_vIsInitialized;
	list<EMRecording*> m_oRecordings;
};

#endif
