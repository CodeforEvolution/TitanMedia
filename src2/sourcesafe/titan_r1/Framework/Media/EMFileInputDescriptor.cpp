#include "EMFileInputDescriptor.h"

EMFileInputDescriptor::EMFileInputDescriptor(string p_oFileName, EMMediaType p_eType) 
	:	EMInputDescriptor(p_eType),
		m_oFileName(p_oFileName),
		m_vNumFrames(0),
		m_vMediaPoolEntryID(-1)
{
}

EMFileInputDescriptor::~EMFileInputDescriptor()
{
}

string EMFileInputDescriptor::GetFileName() const
{
	return m_oFileName;
}

int64 EMFileInputDescriptor::GetNumFrames() const
{
	return m_vNumFrames;
}

int32 EMFileInputDescriptor::GetMediaPoolEntryID() const
{
	return m_vMediaPoolEntryID;
}

void EMFileInputDescriptor::SetMediaPoolEntryID(int32 p_vEntryID)
{
	m_vMediaPoolEntryID = p_vEntryID;
}

bool EMFileInputDescriptor::Open()
{
	return true;
}


bool EMFileInputDescriptor::Close()
{
	return true;
}
