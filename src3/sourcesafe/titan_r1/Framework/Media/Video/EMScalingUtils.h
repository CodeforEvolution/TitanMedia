/*******************************************************
* Creator: Martin Johansson
* Portability: Non_Native
*-------------------------------------------------------
* 
*******************************************************/


#ifndef __EM_SCALING_UTILS
#define __EM_SCALING_UTILS

#include <windows.h>  // Added by: Johan.  To be able to compile.
class EMMediaFormat;

class __declspec(dllexport) EMScalingUtils 
{
public:
	EMScalingUtils();
	~EMScalingUtils();

	//bool InitScaleFunctions(int p_vSrcX, int p_vSrcY, int p_vDstX, int p_vDstY, int p_vBytesPerSourcePixel, int p_vBytesPerDestinationPixel, int p_vSkipBytesSrc, int p_vSkipBytesDst);

	bool InitScaleFunctions(EMMediaFormat* p_opSourceFormat, EMMediaFormat* p_opDestinationFormat);

	void Scale(void *p_opSource, void *p_opDestination);

	void Lock();
	void Release();

protected:
private:
	int *m_pvAddX;
	int *m_pvAddY;

	int m_vDstX;
	int m_vDstY;
	int m_vSrcX;
	int m_vSrcY;
	int m_vBpSP;
	int m_vBpDP;
	int m_vSkipSrc;
	int m_vSkipDst;
	bool m_vConvert;

	bool m_vSwapImage;

	int *CreateAddBuffer(int p_vSrc, int p_vDst, int p_vStepSze);

	const HANDLE m_oMutex;


};

#endif