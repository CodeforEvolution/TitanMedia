#include "EMMediaDestinationRepository.h"

#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"
#include "EMMediaEngine.h"
#include "EMCommandRepository.h"
#include "EMMediaItemContainer.h"
#include "CommandIDs.h"

EMMediaDestinationRepository* EMMediaDestinationRepository::m_opInstance = NULL;

EMMediaDestinationRepository* EMMediaDestinationRepository::Instance()
{
	if(m_opInstance == NULL)
		m_opInstance = EM_new EMMediaDestinationRepository();
	return m_opInstance;
}

void EMMediaDestinationRepository::Delete()
{
	delete m_opInstance; 
}

EMMediaDestinationRepository::EMMediaDestinationRepository()
	:	EMMediaItemContainer<EMBufferDestination>(EM_TYPE_ANY)
{
}

EMMediaDestinationRepository::~EMMediaDestinationRepository()
{
}

void EMMediaDestinationRepository::OnItemAdd()
{
}

void EMMediaDestinationRepository::OnItemRemove()
{
}

bool EMMediaDestinationRepository::SaveData(EMProjectDataSaver* p_opSaver)
{
/*	p_opSaver -> SaveUInt32(static_cast<uint32>(Size()));
	Rewind();
	while(Current() != NULL)
	{
		p_opSaver -> SaveUInt32(static_cast<uint32>(Current() -> GetID()));
		Current() -> SaveData(p_opSaver);
		Next();
	} */
	return true;
}

bool EMMediaDestinationRepository::LoadData(EMProjectDataLoader* p_opLoader)
{
/*	uint32 vSize = p_opLoader -> LoadUInt32();
	
	for(uint32 vIndex = 0; vIndex < vSize; vIndex++)
	{
		int32 vID = static_cast<int32>(p_opLoader -> LoadUInt32());
		;//cout_commented_out_4_release << "Searching for destination with id " << vID << endl;
		EMBufferDestination* opDest = Find(vID);
		if(opDest == NULL)
			EMDebugger("ERROR! Could not find destination with the ID described in the project file!");
		opDest -> LoadData(p_opLoader);
	} */
	return true;
}

bool EMMediaDestinationRepository::ClearData()
{
	return true;
}

