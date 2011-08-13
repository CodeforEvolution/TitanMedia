#include "MediaInfoCommandGetVideoQTEncoders.h"

//#include "EMMediaEngine.h"
#include "EMWinQuickTimeWriterSuper.h"
//#include <list>

MediaInfoCommandGetVideoQTEncoders::MediaInfoCommandGetVideoQTEncoders()
{
}

void* MediaInfoCommandGetVideoQTEncoders::ExecuteE(void* p_upIDs, void*, void*)
{
	int cnt = 0;
	uint32* vpIDs = static_cast<uint32*>(p_upIDs);

	EMWinQuickTimeWriterSuper::Instance() -> GetEncoderIDs(static_cast<uint32*>(vpIDs));
	//EMWinQuickTimeWriter::Instance()->static_cast<char*>(p_vName));
	return NULL;
}

bool MediaInfoCommandGetVideoQTEncoders::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetVideoQTEncoders::IsUndoable()
{
	return false;
}