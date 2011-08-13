/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_MEDIA_FILE_REPOSITORY
#define __EM_MEDIA_FILE_REPOSITORY

#include "EMMediaItemContainer.h"
#include "EMMediaFile.h"

class EMMediaFileRepository : public EMMediaItemContainer<EMMediaFile>
{
public:
	static EMMediaFileRepository* Instance();
	static void Delete();
	bool AddFile(const string* p_opFileName, bool p_vNeverRam = false);
	void RemoveFile(EMMediaFile* p_opFile);
	EMMediaFile* FindFileName(const string* p_opFileName);
	void OnItemAdd(); //Inherited from EMMediaItemContainer. Added by Richard 010821
	void OnItemRemove(); //Inherited from EMMediaItemContainer. Added by Richard 010821

private:
	static EMMediaFileRepository* m_opInstance;
	EMMediaFileRepository();
	~EMMediaFileRepository();
};

#endif
