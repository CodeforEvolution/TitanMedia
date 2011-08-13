#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include <windows.h>
#include <streams.h>

#include "EMImageFileInputDescriptor.h"
#include "EMMediaUtility.h"
#include "EMWinMediaUtility.h"
#include "EMMediaDataBuffer.h"
#include "EMMediaFormat.h"
#include "EMReadFrameInterface.h"
#include "EMMediaEngineUIDs.h"
#include "EMScalingUtils.h"

EMImageFileInputDescriptor::EMImageFileInputDescriptor(string p_oFileName) 
	:	EMFileInputDescriptor(p_oFileName, EM_TYPE_ANY_IMAGE),
	m_oFormat(EM_TYPE_ANY_IMAGE),
	m_oScaledFormat(EM_TYPE_ANY_IMAGE),
	m_vpOriginal(NULL),
	m_vpScaled(NULL),
	m_vSwapImage(false)
{
	m_vNumFrames = 1;
}

EMImageFileInputDescriptor::~EMImageFileInputDescriptor()
{
	if(m_vpOriginal != NULL)
		delete m_vpOriginal;

	if(m_vpScaled != NULL)
		delete m_vpScaled;
}

EMMediaFormat* EMImageFileInputDescriptor::GetFormat()
{
	return &m_oFormat;
}

bool EMImageFileInputDescriptor::ReadFrames(EMMediaDataBuffer** p_opPrimaryBuffer, EMMediaDataBuffer** p_opSecondaryBuffer, int64 p_vMediaFramePosition, int64 p_vOffsetFrames, int64 & p_vOutNumRead, bool p_vSeeking)
{
	return ReadFrames(*p_opPrimaryBuffer);
}

bool EMImageFileInputDescriptor::ReadFrames(EMMediaDataBuffer* p_opPrimaryBuffer)
{
	if(!(p_opPrimaryBuffer -> m_oFormat == m_oScaledFormat) || m_vpScaled == NULL)
	{
		if(m_vpScaled != NULL)
			delete m_vpScaled;

		m_oScaledFormat = p_opPrimaryBuffer -> m_oFormat;

		m_vpScaled = new unsigned char[p_opPrimaryBuffer -> m_vSizeAvailable];

//		if(m_vSwapImage)
		if(m_oFormat.m_vHeight > 0)
			m_oFormat.m_vHeight *= -1;

		m_oScale.InitScaleFunctions(&m_oFormat, &(p_opPrimaryBuffer -> m_oFormat));

//		if(m_vSwapImage)
//			m_oFormat.m_vHeight *= -1;

//		m_oScale.InitScaleFunctions(m_oFormat.m_vWidth, m_oFormat.m_vHeight , m_oScaledFormat.m_vWidth, m_oScaledFormat.m_vHeight, 3, m_oFormat.m_vBytesPerRow, m_oScaledFormat.m_vBytesPerRow);

		m_oScale.Scale(m_vpOriginal, m_vpScaled);
	}

	memcpy(p_opPrimaryBuffer -> Data(), m_vpScaled, p_opPrimaryBuffer -> m_vSizeAvailable);
	p_opPrimaryBuffer -> m_vSizeUsed = p_opPrimaryBuffer -> m_vSizeAvailable;

	return true;		
}


bool EMImageFileInputDescriptor::SeekTo(int64 p_vNewMediaFrame)
{
	return true;
}

bool EMImageFileInputDescriptor::SeekAndDisplay(int64 p_vNewMediaFrame)
{
	return true;
}



#endif
