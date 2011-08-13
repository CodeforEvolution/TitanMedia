/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
* 
* This class is a container class for EMMediaEntry:s.
* It thereby contains a number of files, which can be
* instantiated (i.e. added to the track window)
* by calling InstantiateEntryE(...) and pass the ID
* of the EMMediaEntry in question.
*
* Each time a media entry is instantiated a counter in
* the entry is increased. And each time a clip is deleted
* from the track window, the corresponding counter is
* decreased. This to keep track of how many times a 
* certain file is used.
*
* Richard Kronfält, 2000-12-02
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_MEDIA_POOL
#define __EM_MEDIA_POOL

#include "EMMediaItemContainer.h"
#include "EMMediaPoolEntry.h"

class EMFileInputDescriptor;
class EMMediaPoolAudioEntry;
class EMSettingsRepository;
class EMMediaFileManager;
class EMProjectDataLoader;
class EMProjectDataSaver;



class EMMediaPool : public EMMediaItemContainer<EMMediaPoolEntry>
{
public:
	EMMediaPool();
	~EMMediaPool();
	void AddMediaFileE(string p_oFileName, int32* p_vIDs, bool p_vExtract = true);
	int32 SearchFor(string p_oFileName, EMMediaType p_eType = EM_TYPE_ANY);
	int32 GetID() const;
	EMFileInputDescriptor* InstantiateEntryE(int32 p_vMediaPoolEntryID);
//	bool CreatePeaksFile(EMMediaPoolAudioEntry* p_opEntry);
	int32 GetFileTypeForE(const string* p_opFileName);

	void OnItemAdd(); //Inherited from EMMediaItemContainer. Added by Richard 010821
	void OnItemRemove(); //Inherited from EMMediaItemContainer. Added by Richard 010821

	//Project load/save methods..	
	bool ClearData();
	bool SaveData(EMProjectDataSaver* p_opProjectDataSaver);
	bool LoadData(EMProjectDataLoader* p_opProjectDataLoader);

private:
	int32 m_vID;
	EMMediaFileManager* m_opFileManager;
};

#endif
