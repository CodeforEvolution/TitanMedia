#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include <windows.h>

#include <streams.h>

#include "EMDXImageFileInputDescriptor.h"
#include "EMMediaUtility.h"
#include "EMWinMediaUtility.h"
#include "EMMediaDataBuffer.h"
#include "EMMediaFormat.h"
#include "EMReadFrameInterface.h"
#include "EMMediaEngineUIDs.h"
#include "EMScalingUtils.h"
#include "EMWinGDIPlusImageLoaderSuper.h"

EMDXImageFileInputDescriptor::EMDXImageFileInputDescriptor(string p_oFileName) 
	:	EMImageFileInputDescriptor(p_oFileName)
{
}

EMDXImageFileInputDescriptor::EMDXImageFileInputDescriptor(const EMDXImageFileInputDescriptor& p_oOther)
	: EMImageFileInputDescriptor(p_oOther.GetFileName())
{
	
}

EMDXImageFileInputDescriptor::~EMDXImageFileInputDescriptor()
{
}

EMImageFileInputDescriptor* EMDXImageFileInputDescriptor::Clone()
{
	return EM_new EMDXImageFileInputDescriptor(*this);
}


bool EMDXImageFileInputDescriptor::InitCheckE()
{
	HRESULT hr;
	IGraphBuilder* opIGraph = NULL;
	IBaseFilter* opIFilter = NULL;
	EMReadFrameInterface* opIGrab = NULL;
	IPin* opIPin = NULL;
	IPin* opIInPin = NULL;
	IMediaSeeking* opISeek = NULL;
	IMediaControl* opICtrl = NULL;
	EMMediaDataBuffer* opBuffer = NULL;
	int64 vDummy = 0;

/*	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void**)&opIGraph);
	hr = opIGraph ->QueryInterface(IID_IMediaSeeking, (void**)&opISeek);
	hr = opIGraph ->QueryInterface(IID_IMediaControl, (void**)&opICtrl);
*/
	unsigned short *vpFileName = ((EMWinMediaUtility*) EMMediaUtility::Instance()) -> StringToUnicode(m_oFileName.c_str());
	//delete vpFileName;

	//QUICKTIME
	//m_oQuickTimeVideo = new EMWinQuicktimeVideoSuper(m_oFileName.c_str());
	//m_oQuickTimeVideo -> InitCheckE();
	EMWinGDIPlusImageLoaderSuper::Instance() -> PrepareImageFile(vpFileName);

	m_oFormat.m_vWidth = EMWinGDIPlusImageLoaderSuper::Instance() -> GetWidth();
	m_oFormat.m_vHeight = abs(EMWinGDIPlusImageLoaderSuper::Instance() -> GetHeight());
	m_oFormat.m_vNumChannels = 1;
		
	m_oFormat.m_vDepth = 3;
	m_oFormat.m_vBytesPerSample = 3;
	m_oFormat.m_vBytesPerRow = EMWinGDIPlusImageLoaderSuper::Instance() -> GetBytesPerRow();
	m_oFormat.m_vBufferSizeBytes = abs(m_oFormat.m_vBytesPerRow * m_oFormat.m_vHeight);
	m_oFormat.m_vFrameRate = 0;


	m_vpOriginal = new unsigned char[m_oFormat.m_vBufferSizeBytes]; // Allocates memory
	opBuffer = EM_new EMMediaDataBuffer(EM_TYPE_ANY_IMAGE, m_oFormat.m_vBufferSizeBytes, m_vpOriginal);

	opBuffer -> m_oFormat = m_oFormat;

	//m_oQuickTimeVideo->CopyFrameIntoBuffer(0, opBuffer);
	//********
	EMWinGDIPlusImageLoaderSuper::Instance() -> CopyImageIntoBuffer(m_vpOriginal);
	EMWinGDIPlusImageLoaderSuper::Instance() -> CloseImageFile();
	//********
	opBuffer -> m_vSizeUsed = opBuffer -> m_vSizeAvailable;
//	delete m_oQuickTimeVideo;
	//END QUICKTIME
/*
	hr = opIGraph -> RenderFile(vpFileName, NULL); // Allocates memory

	if(hr != S_OK)
	{
		opIGraph ->Release();
		return false;
	}

	opIPin = FindOutputPin(opIGraph);

	opIGrab = ReplaceRenderWithGrab(opIGraph, opIPin);

	if(!GetInternalFormat(opIPin))
	{
		opIPin -> Release();
		opIGraph -> Release();
		return false;
	}

	opIPin -> Release();
	opIPin = NULL;


	m_vpOriginal = new unsigned char[m_oFormat.m_vBufferSizeBytes]; // Allocates memory
	opBuffer = EM_new EMMediaDataBuffer(EM_TYPE_ANY_IMAGE, m_oFormat.m_vBufferSizeBytes, m_vpOriginal);

	opBuffer -> m_oFormat = m_oFormat;


	opIGrab -> InitializeRenderer(NULL, opISeek, opICtrl);

	opICtrl -> Run(); // Allocates memory

	opIGrab -> GrabFrame(opBuffer, vDummy); // Allocates memory
	opBuffer -> m_vSizeUsed = opBuffer -> m_vSizeAvailable;

	opIGrab -> StopGraph();
	opICtrl -> Stop();
*/
	//Tga file writer, (C) Kaneda ;)
/*	unsigned char Header[18];

	memset(&Header, 0, 18); 

	*(WORD*)(Header + 12) = opBuffer -> m_oFormat.m_vWidth;
	*(WORD*)(Header + 14) = opBuffer -> m_oFormat.m_vHeight;
	*(char*)(Header + 16) = 8 * opBuffer -> m_oFormat.m_vDepth;
	*(char*)(Header +  2) = 2;

	FILE* fp = fopen("Original.tga","wb");
	fwrite(&Header, 1, sizeof(Header), fp);
	fwrite(opBuffer -> Data(), 1, opBuffer -> m_vSizeUsed, fp);
	fclose(fp);*/

/*
	opICtrl -> Release();
	opICtrl = NULL;

	opISeek -> Release();
	opISeek = NULL;

	opIGrab -> Release();
	opIGrab = NULL;

	opIGraph -> Release();
	opIGraph = NULL;
*/
	delete opBuffer;

	return true;
}

EMReadFrameInterface* EMDXImageFileInputDescriptor::ReplaceRenderWithGrab(IGraphBuilder* p_opIGraph, IPin* p_opIPin)
{
	HRESULT hr;
	PIN_INFO oInfo;
	IBaseFilter* opIFilter = NULL;
	EMReadFrameInterface* opIGrab = NULL;
	IPin* opIInPin = NULL;

	p_opIPin -> ConnectedTo(&opIInPin);

	p_opIGraph -> Disconnect(p_opIPin);
	p_opIGraph -> Disconnect(opIInPin);

	opIInPin -> QueryPinInfo(&oInfo);

	p_opIGraph -> RemoveFilter(oInfo.pFilter);
	oInfo.pFilter -> Release();

	opIInPin -> Release();
	opIInPin = NULL;

	CoCreateInstance(CLSID_EMReadFrame, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void**)&opIFilter);
	p_opIGraph -> AddFilter(opIFilter, L"EMReadFrame");
	opIFilter -> QueryInterface(IID_EMReadFrameInterface, (void **)&opIGrab);

	hr = opIFilter -> FindPin(L"Input", &opIInPin);

	opIGrab -> UseScaling(true);

	hr = p_opIGraph -> Connect(p_opIPin, opIInPin);

	opIInPin -> Release();
	opIInPin = NULL;
	
	opIFilter -> Release();
	opIFilter = NULL;

	return opIGrab;
}

bool EMDXImageFileInputDescriptor::GetInternalFormat(IPin* p_opIPin)
{
	HRESULT hr;
	AM_MEDIA_TYPE oType;
	
	hr = p_opIPin -> ConnectionMediaType(&oType);

	if(hr != S_OK || oType.formattype != FORMAT_VideoInfo)
	{
		return false;
	}

	VIDEOINFOHEADER* opInfo = reinterpret_cast<VIDEOINFOHEADER*>(oType.pbFormat);

	m_oFormat.m_vWidth = opInfo -> bmiHeader.biWidth;
	m_oFormat.m_vHeight = abs(opInfo -> bmiHeader.biHeight);
	m_oFormat.m_vNumChannels = 1;
		
//	if(opInfo -> bmiHeader.biBitCount == 8)
//	{
		m_oFormat.m_vDepth = 3;
		m_oFormat.m_vBytesPerSample = 3;
		m_oFormat.m_vBytesPerRow = 3 * m_oFormat.m_vWidth;
		m_oFormat.m_vBufferSizeBytes = abs(m_oFormat.m_vBytesPerRow * m_oFormat.m_vHeight);
/*	}
	else
	{
		m_oFormat.m_vDepth = opInfo -> bmiHeader.biBitCount;
		m_oFormat.m_vBytesPerSample = opInfo -> bmiHeader.biBitCount / 8;
		if(opInfo -> bmiHeader.biSizeImage != 0)
		{
			m_oFormat.m_vBytesPerRow = opInfo -> bmiHeader.biSizeImage / m_oFormat.m_vHeight;
			m_oFormat.m_vBufferSizeBytes = opInfo -> bmiHeader.biSizeImage;
		}
		else
		{
			m_oFormat.m_vBytesPerRow = opInfo -> bmiHeader.biWidth * 3; //opInfo -> bmiHeader.biBitCount / 8;
			m_oFormat.m_vBufferSizeBytes = m_oFormat.m_vBytesPerRow * m_oFormat.m_vHeight;
		}
	}*/

	m_oFormat.m_vFrameRate = 0;

//	m_vSwapImage = false; //opInfo -> bmiHeader.biHeight < 0;

	FreeMediaType(oType);

	return true;
}


IPin* EMDXImageFileInputDescriptor::FindOutputPin(IGraphBuilder* p_opIGraph)
{
	bool FoundRenderer = false;
	ULONG fetched;
	IBaseFilter* Renderer;
	IEnumFilters* EnumFilters;
	IPin* InPin;	
	IPin* OutPin = NULL;	
	IEnumPins* EnumPins;
	PIN_DIRECTION oPinDir;

	p_opIGraph -> EnumFilters(&EnumFilters);
	EnumFilters -> Reset();
	while (!FoundRenderer && EnumFilters -> Next(1, &Renderer, &fetched) == S_OK)
	{
		Renderer -> EnumPins(&EnumPins);
		EnumPins -> Reset();

		while (EnumPins -> Next(1, &InPin, &fetched) == S_OK)
		{
			InPin -> QueryDirection(&oPinDir);

			if (oPinDir == PINDIR_OUTPUT)
			{
				break;
			}

			InPin -> Release();
			InPin = NULL;
		}

		if (InPin == NULL)
		{
			EnumPins -> Reset();
			EnumPins -> Next(1, &InPin, &fetched);

			InPin -> ConnectedTo(&OutPin);

			FoundRenderer = true;
		}
		Renderer -> Release();
		Renderer = NULL;

		InPin -> Release();
		InPin = NULL;

		EnumPins -> Release();
		EnumPins = NULL;
	}	

	EnumFilters -> Release();	
	EnumFilters = NULL;

	return OutPin;
}


#endif
