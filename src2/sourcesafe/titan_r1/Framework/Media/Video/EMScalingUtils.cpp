#include <windows.h>
#include "EMScalingUtils.h"
#include "EMGlobals.h"

#include "EMMediaFormat.h"
	
EMScalingUtils::EMScalingUtils() :
	m_pvAddX(0),
	m_pvAddY(0),
	m_oMutex( CreateSemaphore(NULL, 1, 1, NULL) )
{
}

EMScalingUtils::~EMScalingUtils()
{
	if (m_pvAddX != 0) 
		delete [] m_pvAddX;

	if (m_pvAddY != 0) 
		delete [] m_pvAddY;

	CloseHandle(m_oMutex);
}

int *EMScalingUtils::CreateAddBuffer(int p_vSrc, int p_vDst, int p_vStepSze) 
{
	int tmp = 0;

	int *pBuff = new int[p_vDst];
	int *pPtr = pBuff;

	float k = (float)p_vSrc / (float)p_vDst;

	for (int i = 0; i < p_vDst; ++i, ++pPtr) 
	{
		*pPtr = (int)(k*(i+1) - (int)(k*i));
		*pPtr *= p_vStepSze;

		tmp += *pPtr;

	}

	return pBuff;
}

/*bool EMScalingUtils::InitScaleFunctions(int p_vSrcX, int p_vSrcY, int p_vDstX, int p_vDstY, int p_vBytesPerSourcePixel, int p_vBytesPerDestinationPixel, int p_vSkipBytesSrc, int p_vSkipBytesDst)
{
	Lock();

	m_vSrcX = p_vSrcX;
	m_vSrcY = abs(p_vSrcY);
	m_vDstX = p_vDstX;
	m_vDstY = abs(p_vDstY);
	m_vBpSP = p_vBytesPerSourcePixel;
	m_vBpDP = p_vBytesPerDestinationPixel;
	m_vSkipSrc = p_vSkipBytesSrc;
	m_vSkipDst = p_vSkipBytesDst;
	m_vSwapImage = (p_vDstY * p_vSrcY) < 0;
	m_vConvert = m_vBpSP != m_vBpDP;

	if(m_vSwapImage)
		m_vSkipDst = -m_vSkipDst;

	if (m_pvAddX != 0) 
		delete [] m_pvAddX;

	if (m_pvAddY != 0)
		delete [] m_pvAddY;

	m_pvAddX = CreateAddBuffer(m_vSrcX, m_vDstX, m_vBpSP);
	m_pvAddY = CreateAddBuffer(m_vSrcY, m_vDstY, 1);

	Release();

	if (m_pvAddX == 0 || m_pvAddY == 0)
		return false;

	return true;
}
*/

bool EMScalingUtils::InitScaleFunctions(EMMediaFormat* p_opSourceFormat, EMMediaFormat* p_opDestinationFormat)
{
	Lock();

	m_vSrcX = p_opSourceFormat -> m_vWidth;
	m_vSrcY = abs(p_opSourceFormat -> m_vHeight);

	m_vDstX = p_opDestinationFormat -> m_vWidth;
	m_vDstY = abs(p_opDestinationFormat -> m_vHeight);

	m_vBpSP = p_opSourceFormat -> m_vDepth;
	m_vBpDP = p_opDestinationFormat -> m_vDepth;

	m_vSkipSrc = p_opSourceFormat -> m_vBytesPerRow;
	m_vSkipDst = p_opDestinationFormat -> m_vBytesPerRow;
	m_vSwapImage = (p_opSourceFormat -> m_vHeight * p_opDestinationFormat -> m_vHeight) < 0;
	m_vConvert = m_vBpSP != m_vBpDP;

	if(m_vSwapImage)
		m_vSkipDst = -m_vSkipDst;

	if (m_pvAddX != 0) 
		delete [] m_pvAddX;

	if (m_pvAddY != 0)
		delete [] m_pvAddY;

	m_pvAddX = CreateAddBuffer(m_vSrcX, m_vDstX, m_vBpSP);
	m_pvAddY = CreateAddBuffer(m_vSrcY, m_vDstY, 1);

	Release();

	if (m_pvAddX == 0 || m_pvAddY == 0)
		return false;

	return true;
}


void EMScalingUtils::Lock() 
{
	WaitForSingleObject( m_oMutex, INFINITE);
}
void EMScalingUtils::Release()
{
	ReleaseSemaphore( m_oMutex, 1, NULL);
}

void EMScalingUtils::Scale(void *p_opSource, void *p_opDestination)
{
	Lock();
	if(m_vSrcX == m_vDstX && m_vSrcY == m_vDstY && m_vSkipSrc == m_vSkipDst && !m_vSwapImage)
	{
		int64 vImageSize = m_vSrcY * m_vSkipSrc;
		memcpy(p_opDestination, p_opSource, vImageSize);
		Release();
		return;
	}

	unsigned char *pSrcOld;
	unsigned char *pDstOld;
	unsigned char *pSrc = static_cast<unsigned char*>(p_opSource);
	unsigned char *pDst = static_cast<unsigned char*>(p_opDestination) - (m_vSwapImage ?  m_vSkipDst * (m_vDstY - 1) : 0);

	int *pAddY = m_pvAddY;

	if (m_pvAddX == 0 || m_pvAddY == 0)
		MessageBox(NULL, "No preAllocated Scaling data!!!","Going down",MB_OK);

	for (int y = 0; y < m_vDstY; ++y) 
	{
		int *pAddX = m_pvAddX;

		pSrcOld = pSrc;
		pDstOld = pDst;

		for (int x = 0; x < m_vDstX; ++x)
		{
			if(m_vConvert)
				if(m_vBpSP == 2 && m_vBpDP == 3)
				{
					//MEDIASUBTYPE_RGB565	
//					pDst[0] = (pSrc[0] & 0x1F) << 3;
//					pDst[1] = (((pSrc[0] & 0xE0 ) >> 5) | ((pSrc[1] & 0x07 ) << 3)) << 3;
//					pDst[2] = (((pSrc[1] & 0xF8) >> 3)) << 3;

					//MEDIASUBTYPE_RGB555  
					pDst[0] = (pSrc[1] & 0x1F) << 3;
					pDst[1] = (((pSrc[1] & 0xE0 ) >> 5) | ((pSrc[0] & 0x03 ) << 3)) << 3;
					pDst[2] = (((pSrc[0] & 0x7C) >> 2)) << 3;

				}
				else if(m_vBpSP == 4 && m_vBpDP == 3)
					for (int i = 0; i < m_vBpDP; ++i) 
						pDst[i] = pSrc[i];
				else
			;//		EMDebugger("Error!! Can not convert between unknown pixel formats!");
			else
				for (int i = 0; i < m_vBpSP; ++i) 
					pDst[i] = pSrc[i];
			

			pDst += m_vBpDP;
			pSrc += *pAddX;
			++ pAddX;
		}
		pDst = pDstOld + m_vSkipDst;
		pSrc = pSrcOld + m_vSkipSrc*(*pAddY);
		++ pAddY;
	}

	Release();
}