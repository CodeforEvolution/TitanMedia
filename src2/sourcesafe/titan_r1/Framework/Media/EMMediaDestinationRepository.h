/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_MEDIA_DESTINATION_REPOSITORY
#define __EM_MEDIA_DESTINATION_REPOSITORY

#include "EMMediaGlobals.h"
#include "EMMediaItemContainer.h"
#include "EMBufferDestination.h"

class EMProjectDataLoader;
class EMProjectDataSaver;

class EMMediaDestinationRepository : public EMMediaItemContainer<EMBufferDestination>
{
public:
	static EMMediaDestinationRepository* Instance();
	static void Delete();
	~EMMediaDestinationRepository();

	void OnItemAdd(); //Inherited from EMMediaItemContainer. Added by Richard 010821
	void OnItemRemove(); //Inherited from EMMediaItemContainer. Added by Richard 010821
	bool ClearData();
	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool LoadData(EMProjectDataLoader* p_opLoader);

private:
	EMMediaDestinationRepository();
	static EMMediaDestinationRepository* m_opInstance;
};

#endif
