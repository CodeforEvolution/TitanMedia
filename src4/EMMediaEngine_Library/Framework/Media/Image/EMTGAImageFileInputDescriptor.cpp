#include "EMTGAImageFileInputDescriptor.h"

#pragma pack(1)


typedef struct _TGAHeaderSpec
{
	int16 m_vXorigin;
	int16 m_vYorigin;
	int16 m_vWidth;
	int16 m_vHeight;
	int8  m_vBits;
	int8  m_vCaps;
} TGAHeaderSpec;

typedef struct _TGAColorInfo
{
	int16 m_vCMapOrigin;
	int16 m_vCMapLength;
	int8  m_vCMapBits;
} TGAColorInfo;


typedef struct _TGAHeader
{
	int8 m_vIdLen;
	int8 m_vColorType;
	int8 m_vType;
	TGAColorInfo m_oColor;
	TGAHeaderSpec m_oInfo;
} TGAHeader;


EMTGAImageFileInputDescriptor::EMTGAImageFileInputDescriptor(string p_oFileName) 
	:	EMImageFileInputDescriptor(p_oFileName)
{
}

EMTGAImageFileInputDescriptor::EMTGAImageFileInputDescriptor(const EMTGAImageFileInputDescriptor& p_oOther)
	: EMImageFileInputDescriptor(p_oOther.GetFileName())
{
	
}

EMTGAImageFileInputDescriptor::~EMTGAImageFileInputDescriptor()
{
}

bool EMTGAImageFileInputDescriptor::InitCheckE()
{
	TGAHeader oHead;
	long vOffset;
	long vCMLen;
	unsigned char* opBits;
	bool vRet = true;

	int n = sizeof(TGAHeader);

	FILE* fp = fopen(m_oFileName.c_str(), "rb");
	if(fp == NULL)
		return false;

	fread(&oHead, 1, sizeof(TGAHeader), fp);

	m_oFormat.m_vHeight = oHead.m_oInfo.m_vHeight;
	m_oFormat.m_vWidth = oHead.m_oInfo.m_vWidth;
	m_oFormat.m_vDepth = 3;
	m_oFormat.m_vNumChannels = 1;
	m_oFormat.m_vBytesPerRow = m_oFormat.m_vWidth * 3;
	m_oFormat.m_vBufferSizeBytes = m_oFormat.m_vBytesPerRow * m_oFormat.m_vHeight;
	m_oFormat.m_vBytesPerSample = 3;
	m_oFormat.m_vFrameRate = 0;

	switch(oHead.m_vType)
	{
	case 0: //No image data included??
		vRet = false;
		break;
	case 1: ////Uncompressed indexed image
		if(oHead.m_vColorType > 0)
		{	
			vOffset = oHead.m_vIdLen;
			fseek(fp, vOffset, SEEK_CUR);
			
			vOffset = oHead.m_oInfo.m_vWidth * oHead.m_oInfo.m_vHeight * oHead.m_oInfo.m_vBits / 8;
			vCMLen = oHead.m_oColor.m_vCMapLength * oHead.m_oColor.m_vCMapBits / 8;

			opBits = EM_new unsigned char[vOffset + vCMLen];
			fread(opBits, 1, vOffset + vCMLen, fp);

			m_vpOriginal = Convert(opBits + vCMLen, oHead.m_oInfo.m_vBits, opBits, oHead.m_oColor.m_vCMapBits); 
			delete [] opBits;		
		}
		else
			vRet = false;	//No color-map??
		break;

	case 2:	//Uncompressed RGB Image
	case 3: //Uncompressed Black&white
		vOffset = oHead.m_vIdLen;
		if(oHead.m_vColorType > 0)
			vOffset += oHead.m_oColor.m_vCMapLength * oHead.m_oColor.m_vCMapBits / 8;

		fseek(fp, vOffset, SEEK_CUR);

		vOffset = oHead.m_oInfo.m_vWidth * oHead.m_oInfo.m_vHeight * oHead.m_oInfo.m_vBits / 8;

		opBits = EM_new unsigned char[vOffset];
		vCMLen = fread(opBits, 1, vOffset, fp);

		m_vpOriginal = Convert(opBits, oHead.m_oInfo.m_vBits);
		delete [] opBits;		
		break;

	case 9: ////Compressed indexed image
		break;
	case 10:	//Compressed RGB Image
	case 11: //Compressed Black&white
		break;

	default:
		break;
	}

	fclose(fp);

	return vRet;
}

void EMTGAImageFileInputDescriptor::ConvertPixel(unsigned char* p_opSrc, unsigned char* p_opDst, int p_vDepth)
{
	switch(p_vDepth)
	{
	case 15:
	case 16:
		p_opDst[0] = (p_opSrc[0] & 0x1F) << 3;
		p_opDst[1] = (((p_opSrc[0] & 0xE0 ) >> 5) | ((p_opSrc[1] & 0x03 ) << 3)) << 3;
		p_opDst[2] = (((p_opSrc[1] & 0x7C) >> 2)) << 3;
		break;
	case 24:
	case 32:
		p_opDst[0] = p_opSrc[0];
		p_opDst[1] = p_opSrc[1];
		p_opDst[2] = p_opSrc[2];
		break;
	}
}

unsigned char* EMTGAImageFileInputDescriptor::Extract(unsigned char* p_opBits, int p_vDepth, unsigned char* p_opCMap, int p_vCDepth)
{
	int32 i;
	int32 j;
	int16 ndx;
	int16 vAdd;

	unsigned char* opDestination = EM_new unsigned char[m_oFormat.m_vBufferSizeBytes];

	unsigned char* opSrc = p_opBits;
	unsigned char* opCmapSrc;
	unsigned char* opDst = opDestination;

	switch(p_vDepth)
	{
	case 8:
		vAdd = 1;
		break;
	case 15:
	case 16:
		vAdd = 2;
		break;

	case 24:
		vAdd = 3;
		break;

	case 32:
		vAdd = 4;
		break;
	}

	for(i = 0; i < m_oFormat.m_vHeight*m_oFormat.m_vWidth;)
	{
		ndx = *opSrc & 0x7E;

		if(*opSrc & 0x80)
		{	
			++ opSrc;
			if(p_opCMap != NULL)
			{
				opCmapSrc = opSrc;
				opSrc = p_opCMap + *opSrc * p_vCDepth / 8;
			}

			for(j = 0; j <= ndx; ++j, ++i)
			{
				ConvertPixel(opSrc, opDst, p_vDepth);
				opDst += 3;
			}		

			if(p_opCMap != NULL)
				opSrc = opCmapSrc;
			
			opSrc += vAdd;
		}
		else
		{
			++ opSrc;
			
			if(p_opCMap != NULL)
			{
				for(j = 0; j <= ndx; ++j, ++i)
				{
					opCmapSrc = p_opCMap + *opSrc * p_vCDepth / 8;
					ConvertPixel(opCmapSrc, opDst, p_vDepth);
					opDst += 3;
					opSrc += vAdd;
				}							
			}
			else
			{
				for(j = 0; j <= ndx; ++j, ++i)
				{
					ConvertPixel(opSrc, opDst, p_vDepth);
					opDst += 3;
					opSrc += vAdd;
				}			
			}
		}
	}

	return opDestination;
}


unsigned char* EMTGAImageFileInputDescriptor::Convert(unsigned char* p_opBits, int p_vDepth, unsigned char* p_opCMap, int p_vCDepth)
{
	int32 i;
	int16 ndx;

	unsigned char* opDestination = EM_new unsigned char[m_oFormat.m_vBufferSizeBytes];

	unsigned char* opSrc = p_opBits;
	unsigned char* opDst = opDestination;

	switch(p_vDepth)
	{
	case 8:
		if(p_opCMap == NULL)
		{
			for(i = 0; i < m_oFormat.m_vHeight*m_oFormat.m_vWidth; ++i, opDst += 3, ++opSrc)
			{
				opDst[0] = *opSrc;
				opDst[1] = *opSrc;
				opDst[2] = *opSrc;
			}
			
		}
		else
		{
			for(i = 0; i < m_oFormat.m_vHeight*m_oFormat.m_vWidth; ++i, opDst += 3, ++opSrc)
			{
				ndx = opSrc[0];
				ndx *= p_vCDepth / 8;
				ConvertPixel(p_opCMap + ndx, opDst, p_vCDepth);
			}
		}

		break;
	case 15:
	case 16:
		for(i = 0; i < m_oFormat.m_vHeight*m_oFormat.m_vWidth; ++i, opDst += 3, opSrc += 2)
			ConvertPixel(opSrc, opDst, 16);
		break;

	case 24:
		memcpy(opDst, opSrc, m_oFormat.m_vBufferSizeBytes);
		break;

	case 32:
		for(i = 0; i < m_oFormat.m_vHeight*m_oFormat.m_vWidth; ++i, opDst += 3, opSrc += 4)
			ConvertPixel(opSrc, opDst, 32);
		break;
	}

	return opDestination;
}

EMImageFileInputDescriptor* EMTGAImageFileInputDescriptor::Clone()
{
	return EM_new EMTGAImageFileInputDescriptor(*this);
}

#pragma pack()


