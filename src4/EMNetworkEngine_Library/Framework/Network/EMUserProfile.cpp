#include "EMUserProfile.h"

#include "srk_ipc.h"

EMUserProfile::EMUserProfile()
  :  m_opIpc(NULL)
{
}

EMUserProfile::EMUserProfile(TSonorkIpc* p_opIpc)
  :  m_opIpc(p_opIpc)
{
	p_opIpc -> GetUserHandle(&m_oUserHandle);
}

EMUserProfile::~EMUserProfile()
{
}


void EMUserProfile::UpdateProfile()
{
	m_opIpc -> GetUserHandle(&m_oUserHandle);
}
