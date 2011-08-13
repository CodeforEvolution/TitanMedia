#include "EMMediaFile.h"

#include "EMMediaIDManager.h"
#include "EMBeMediaFile.h"
#include "EMMediaGlobals.h"
//#include "EMWinMediaFile.h"

EMMediaFile* EMMediaFile::Create(const string* p_opName)
{
	EMMediaFile* opNewObject = NULL;
#ifdef PLATFORM_BEOS
	opNewObject = EM_new EMBeMediaFile(p_opName);
#endif
#ifdef PLATFORM_WINDOWS
	opNewObject = EM_new EMBeMediaFile(p_opName);
#endif
	return opNewObject;
}

EMMediaFile::EMMediaFile(const string* p_opName)
	:	m_vDirection(EM_READ),
		m_vIsOpen(false),
		m_oFileName(*p_opName)
{
	m_vID = EMMediaIDManager::MakeID();
//	EMDebugger("Created EMMEdiaFile");
}

EMMediaFile::~EMMediaFile()
{
//	EMDebugger("Deleted EMMEdiaFile");
}

int32 EMMediaFile::GetID() const
{
	return m_vID;
}

int32 EMMediaFile::GetType() const
{
	return 0;
}

bool EMMediaFile::InitCheckE()
{
	//TODO: Perform extraction and conversion here, somehow...
	return true;
}

bool EMMediaFile::IsOpen() const
{
	return m_vIsOpen;
}

void EMMediaFile::SetOpen(bool p_vFlag)
{
	m_vIsOpen = p_vFlag;
}

const string* EMMediaFile::GetFileName()
{
	return &m_oFileName;
}
