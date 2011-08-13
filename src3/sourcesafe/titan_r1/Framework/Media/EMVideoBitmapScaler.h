#ifndef __EM_VIDEO_BITMAP_SCALER
#define __EM_VIDEO_BITMAP_SCALER

#include "EMMediaFormat.h"

class EMRowValues
{
public:
	EMRowValues(){};
	long x_i;
	float p;
	float p1;
};

class EMVideoBitmapScaler
{

public:
	EMVideoBitmapScaler();
	~EMVideoBitmapScaler();
	void* Scale(void *src, EMMediaFormat *srcFormat, EMMediaFormat *destFormat);

private:
	EMRowValues **m_opR, **m_opRptr;
	float m_vxfac, m_vyfac;
	long m_vDrows;
	long m_vDcols;
	int32 m_vLocalHeight;
	int32 m_vLocalWidth;
	int32 m_vLocalSourceHeight;
	int32 m_vLocalSourceWidth;
	unsigned char* m_opDestinationBitMap;

};

#endif
