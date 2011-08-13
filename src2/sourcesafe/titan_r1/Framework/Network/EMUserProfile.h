/*******************************************************
* Creator: Martin Johansson
* Portability: Native, very native
*-------------------------------------------------------
* 
* Martin Johansson, 01-08-01
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_USER_PROFILE
#define __EM_USER_PROFILE


#include "srk_defs.h"

class TSonorkIpc;
struct TSonorkUserHandle;

class EMUserProfile
{
public:
	EMUserProfile(TSonorkIpc* p_opIpc);
	~EMUserProfile();

	void UpdateProfile();

	TSonorkUserHandle m_oUserHandle;
protected:
	TSonorkIpc* m_opIpc;

private:
	EMUserProfile();
};

#endif
