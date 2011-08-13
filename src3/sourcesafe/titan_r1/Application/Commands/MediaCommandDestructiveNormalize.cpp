#include "MediaCommandDestructiveNormalize.h"

#include <windows.h>

#include "EMMediaEngine.h"
#include "EMMediaFileRepository.h"
#include "EMMediaProject.h"
#include "EMMediaClipRepository.h"
#include "EMMediaClip.h"
#include "EMDestructiveNormalize.h"
#include "EMFileInputDescriptor.h"
#include "EMWinMediaUtility.h"


MediaCommandDestructiveNormalize::MediaCommandDestructiveNormalize()
{
}

MediaCommandDestructiveNormalize::~MediaCommandDestructiveNormalize()
{
}

EMDestructivePlugin* MediaCommandDestructiveNormalize::GetPlugin(int32 p_vId)
{
	return &m_oPlugin;
}


EMCommand* MediaCommandDestructiveNormalize::CloneAndClear()
{
	MediaCommandDestructiveNormalize* opCmd = EM_new MediaCommandDestructiveNormalize;

	opCmd -> m_oNewName = m_oNewName;
	opCmd -> m_vReturnValue = m_vReturnValue;
	opCmd -> m_vMediaPoolValue = m_vMediaPoolValue;
	opCmd -> m_oPeaksFile = m_oPeaksFile;

	m_oNewName = "";
	m_oPeaksFile = "";
	m_vReturnValue = -1;
	m_vMediaPoolValue = -1;

	return opCmd;
}
