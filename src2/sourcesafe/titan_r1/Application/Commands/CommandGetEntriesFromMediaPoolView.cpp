#include "CommandGetEntriesFromMediaPoolView.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
//#include "EMGUIUtilities.h"
//#include "EMListViewControl.h"
#include "EMMediaGlobals.h"
//#include "EMView.h"

#include "MediaPoolView.h"

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#endif

CommandGetEntriesFromMediaPoolView::CommandGetEntriesFromMediaPoolView() :
m_vNewEntry(false)
{
}

void* CommandGetEntriesFromMediaPoolView::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
	string oPath(static_cast<char*>(p_opParameterOne));
	int32* vpEntryIDs;
	vpEntryIDs = static_cast<int32*>(p_opParameterTwo);
	bool vPair = reinterpret_cast<bool>(p_opParameterThree);
	
	vpEntryIDs[0] = -1;
	vpEntryIDs[1] = -1;

	pair<uint32, uint32>* oTemp = MediaPoolView::Instance() -> GetEntry(oPath.c_str());
	if(oTemp -> second == EM_TYPE_ANY_AUDIO)
	{
		vpEntryIDs[0] = oTemp -> first;
		if(vPair)
			vpEntryIDs[1] = MediaPoolView::Instance() -> GetPairEntry(oTemp -> first);
		else
			vpEntryIDs[1] = -1;
		m_vNewEntry = false;
	}
	else if(oTemp -> second == EM_TYPE_ANY_VIDEO)
	{
		if(vPair)
			vpEntryIDs[0] = MediaPoolView::Instance() -> GetPairEntry(oTemp -> first);
		else
			vpEntryIDs[0] = -1;
		vpEntryIDs[1] = oTemp -> first;
		m_vNewEntry = false;
	}
	else
	{
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_IMPORT_MEDIA, const_cast<char*>(oPath.c_str()), vpEntryIDs);
		m_vNewEntry = true;
	}

	return &m_vNewEntry;
}

bool CommandGetEntriesFromMediaPoolView::IsUndoable()
{
	return false;
}

bool CommandGetEntriesFromMediaPoolView::RequiresParameters()
{
	return true;
}
