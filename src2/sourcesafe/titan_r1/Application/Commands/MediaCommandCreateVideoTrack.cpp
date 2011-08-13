#include "MediaCommandCreateVideoTrack.h"

MediaCommandCreateVideoTrack::MediaCommandCreateVideoTrack()
{
}

void MediaCommandCreateVideoTrack::ExecuteE()
{
	//;//cout_commented_out_4_release << "MediaCommandCreateVideoTrack" << endl;
	ExecuteCreateTrack(EM_TYPE_ANY_VIDEO, "Untitled video track");
}

void* MediaCommandCreateVideoTrack::ExecuteE(void* p_opParameterOne, void* p_opParameterTwo, void* p_opParameterThree)
{
	//;//cout_commented_out_4_release << "MediaCommandCreateVideoTrack" << endl;
/*	if(p_opParameterOne == NULL)
		EMDebugger("ERROR! Paramater one (oName) to MediaCommandCreateVideoTrack cannot be NULL!");

	string oName = *(static_cast<string*>(p_opParameterOne));
	if(p_opParameterTwo ==NULL)
		EMDebugger("Can't create track with id NULL!!");
	uint32 vPrio = (p_opParameterTwo == NULL ? 0 : *(static_cast<uint32*>(p_opParameterTwo)));
	return ExecuteCreateTrackP(EM_TYPE_ANY_VIDEO, oName, vPrio);
*/

	const char* vpName = static_cast<const char*>(p_opParameterOne);
	uint32 vPrio = (p_opParameterTwo == NULL ? 0 : *(static_cast<uint32*>(p_opParameterTwo)));
	//uint32 vPrio = 0;
	return ExecuteCreateTrackP(EM_TYPE_ANY_VIDEO, vpName, vPrio);

}
