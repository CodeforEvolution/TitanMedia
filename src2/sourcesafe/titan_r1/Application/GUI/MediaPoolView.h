/*******************************************************
* This code is the copyrighted property of Elegant Media
*-------------------------------------------------------
* Creator: Johan Dunberg
* Component: Application
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/
#include "EMGlobals.h"

#ifndef __MEDIA_POOL_VIEW
#define __MEDIA_POOL_VIEW

#include "EMListener.h"
#include "EMProjectDataContainer.h"
#include <map>

#ifdef PLATFORM_WINDOWS
#include <shlobj.h>
#endif
#include <vector>
typedef pair<uint32, uint32> Pair;
typedef pair<int32, int32> EntryPair;

class EMView;
class EMListViewControl;
class EMDragDropData;

class MediaPoolView : public EMListener, public EMProjectDataContainer
{
public:
	~MediaPoolView();
	void AddEntry(const char* p_vpFileName, const uint32 p_vEntry, const uint32 p_vType, const char* p_vpRealFileName = NULL);
	void AddEntryPair(int32* p_vEntryIDs);
	void ChangeInstances(uint32 p_vRow); // Set -1 to get highest value possible
	bool ClearData();
	void DeleteAllClipBelongingToFile(const char* p_vpFileName); //p_vpFileName must be full path to file
	//pair<uint32, uint32>* EraseEntry(const char* p_vpFileName); //p_vpFileName must be full path to file
	void EraseEntry(const char* p_vpFileName, bool p_vForceErase = false); //p_vpFileName must be full path to file
	pair<uint32, uint32>* GetEntry(const char* p_vpFileName); //p_vpFileName must be full path to file
	int32 GetPairEntry(int32 p_vEntryID);
	uint32 GetIndexID(const char* p_vpFileName); //p_vpFileName must be full path to file
	EMListViewControl* GetListViewControl();
	EMView* GetView();
	void Hide();
	void Init();
	static MediaPoolView* Instance();
	void LoadComplete();
	bool Lock();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void Show();
	void Unlock();

protected:
	MediaPoolView();
	void AddEntry(const char* p_vpFileName, pair<uint32, uint32>* p_vpEntry);
	bool CheckIfExist(const char* p_vFullpathFilename);
	
private:
	static MediaPoolView* m_opInstance;
	map<string, Pair> m_oEntries;
	map<uint32, string> m_oPathsMappedToEntries;
	map<string, string> m_oRealAudioPathToPath;
	EMListViewControl* m_opListViewControl;
	EMView* m_opView;
	list<EntryPair> m_oVideoWithAudioEntries;
//	Pair m_vSelectionTempEntry;
	string m_oSelectionTempString;
	EMDragDropData* m_opDragDropData;
	Pair m_oNoValueFound;
};

#endif