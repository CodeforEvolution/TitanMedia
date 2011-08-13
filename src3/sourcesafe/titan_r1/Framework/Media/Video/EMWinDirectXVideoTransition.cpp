#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMMediaEngine.h"
#include "EMWinDirectXVideoTransition.h"
#include "EMWinVideoShuffleOutputFilter.h"
#include "EMMediaSystemInspector.h"
#include "EMWinMediaSystemInspector.h"
#include "EMMediaEngineUIDs.h"
#include "EMPlugin.h"

#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"

#include <streams.h>	
#include <dshow.h>

const int64 EMWinDirectXVideoTransition::m_vTransTimeSlice = 100000000;

EMWinDirectXVideoTransition::EMWinDirectXVideoTransition(string p_oPluginName, IDirectDraw7* p_opIDD, IMoniker* p_opIMoniker)
	:	EMVideoTransition(p_oPluginName, EM_WINDOWS_DIRECTX_VIDEO_TRANSITION), //  EMPlugin(p_oPluginName, EM_TYPE_RAW_VIDEO, EM_WINDOWS_DIRECTX_VIDEO_TRANSITION, EM_TRANSITION),
		m_opIDD(p_opIDD),
		m_opIMoniker(p_opIMoniker),
		m_oFormat(EM_TYPE_RAW_VIDEO)		
{
	m_oFormat.m_eType = EM_TYPE_RAW_VIDEO;
	m_oFormat.m_vHeight = *(static_cast<uint32*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_FORMAT_HEIGHT)));
	m_oFormat.m_vWidth  = *(static_cast<uint32*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_FORMAT_WIDTH)));
	m_oFormat.m_vDepth  = 3;
	m_oFormat.m_vBytesPerRow = m_oFormat.m_vDepth * m_oFormat.m_vWidth;
	m_oFormat.m_vBufferSizeBytes = m_oFormat.m_vHeight * m_oFormat.m_vBytesPerRow;
	m_oFormat.m_vFrameRate = *(static_cast<uint32*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_FRAMERATE)));

	m_opIMoniker -> AddRef();
	m_opIDD -> AddRef();

	m_opIDxTrans = NULL;
	m_opIEffect = NULL;

	m_opIDDInput1 = NULL;
	m_opIDDInput2 = NULL;
	m_opIDDOutput = NULL;

	m_opIDXInput1 = NULL;
	m_opIDXInput2 = NULL;
	m_opIDXOutput = NULL;	

	m_vSwap = false;

//	Setup();
}

EMWinDirectXVideoTransition::~EMWinDirectXVideoTransition()
{
	m_opIMoniker -> Release();
	m_opIDD -> Release();

	Shutdown();
}

bool EMWinDirectXVideoTransition::PrepareToPlayE()
{
	Setup();
	return S_OK;
}

bool EMWinDirectXVideoTransition::FormatHasChanged(EMMediaDataBuffer* p_opBuffer)
{
	EMMediaFormat* opFormat = &(p_opBuffer->m_oFormat);

	if(	m_oFormat.m_vHeight == opFormat -> m_vHeight &&
		m_oFormat.m_vWidth  == opFormat -> m_vWidth &&
		m_oFormat.m_vDepth  == opFormat -> m_vDepth &&
		m_oFormat.m_vBytesPerRow == opFormat -> m_vBytesPerRow &&
		m_oFormat.m_vBufferSizeBytes == opFormat -> m_vBufferSizeBytes)
		return false;

	return true;
}

bool EMWinDirectXVideoTransition::SetFormat(EMMediaDataBuffer* p_opBuffer)
{
	m_oFormat = p_opBuffer -> m_oFormat;
	
	try {
		Shutdown();
		Setup();
	}
	catch(...)
	{
		return false;
	}

	return true;
}

bool EMWinDirectXVideoTransition::FillSurface(IDirectDrawSurface7* p_opIDXInput, EMMediaDataBuffer* p_opBuffer)
{
	HRESULT hr;
	DDSURFACEDESC2 oSurfDesc;

	memset(&oSurfDesc, 0, sizeof(oSurfDesc));
	oSurfDesc.dwSize = sizeof(oSurfDesc);

	hr = p_opIDXInput -> Lock(NULL, &oSurfDesc, DDLOCK_WRITEONLY, NULL);
	memcpy(oSurfDesc.lpSurface, p_opBuffer ->Data(), p_opBuffer -> m_vSizeUsed);
	hr = p_opIDXInput -> Unlock(NULL);
	
	return true;
}

bool EMWinDirectXVideoTransition::FillBuffert(EMMediaDataBuffer* p_opBuffer, IDirectDrawSurface7* p_opIDXOutput)
{
	HRESULT hr;
	DDSURFACEDESC2 oSurfDesc;

	memset(&oSurfDesc, 0, sizeof(oSurfDesc));
	oSurfDesc.dwSize = sizeof(oSurfDesc);

	hr = p_opIDXOutput -> Lock(NULL, &oSurfDesc, DDLOCK_READONLY, NULL);
	memcpy(p_opBuffer -> Data(), oSurfDesc.lpSurface, p_opBuffer -> m_vSizeUsed);
	hr = p_opIDXOutput -> Unlock(NULL);
	
	return true;
}


EMMediaDataBuffer* EMWinDirectXVideoTransition::ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList)
{
	HRESULT hr;

	float vTime;
	int64 vCurrentTime;
	int64 vCurrentFrame;

	EMMediaDataBuffer* opBuffer1;
	EMMediaDataBuffer* opBuffer2;
	EMMediaDataBuffer* opBuffer3;

	opBuffer1 = p_opBufferList -> front();
	p_opBufferList -> pop_front();

	if (p_opBufferList -> size() != 0)
	{
		opBuffer2 = p_opBufferList -> front();
		p_opBufferList -> pop_front();

		if(m_vSwap)
		{
			opBuffer3 = opBuffer2;
			opBuffer2 = opBuffer1;
			opBuffer1 = opBuffer3;
		}

		if (FormatHasChanged(opBuffer1))
			SetFormat(opBuffer1);

		vCurrentTime = opBuffer1 -> GetRelativeTimeToStart();  
		vCurrentFrame = opBuffer1 -> GetFrame();
		vTime = CalcTime(opBuffer1);

		if (vTime == 0)
		{
			opBuffer2 -> Recycle();
		}
		else if (vTime < m_vTransTimeSlice) 
		{	
			FillSurface(m_opIDDInput1, opBuffer1);
			FillSurface(m_opIDDInput2, opBuffer2);

			IUnknown* oInputs[2];
	
			oInputs[0] =  m_opIDXInput1;
			oInputs[1] =  m_opIDXInput2;

			if(NeedReset())
				hr = m_opIEffect -> Setup(oInputs, 2, (IUnknown**)&m_opIDXOutput, 1, 0);
				
			m_opIDxTrans -> put_Progress(vTime);

			hr = m_opIEffect -> Execute(NULL, NULL, NULL);


			FillBuffert(opBuffer1, m_opIDDOutput);

			opBuffer2 -> Recycle();
		}
		else
		{
			opBuffer1 -> Recycle();
			opBuffer1 = opBuffer2;
		}
		opBuffer1 -> SetRelativeTimeToStart(vCurrentTime);
		opBuffer1 -> SetFrame(vCurrentFrame);
	}

	opBuffer1 -> m_vOriginalHasBeenWasted = true;
	opBuffer1 -> m_opDestination = m_opDestination;
	return opBuffer1;
}

IDirectDrawSurface7* EMWinDirectXVideoTransition::CreateDDSurface(int p_vWidth, int p_vHeight, int p_vBpP)
{
	HRESULT hr;

	IDirectDrawSurface7* opISurface = NULL;

	DDSURFACEDESC2 oSurfDesc;

	memset(&oSurfDesc, 0, sizeof(oSurfDesc));
	oSurfDesc.dwSize = sizeof(oSurfDesc);
	oSurfDesc.dwFlags = DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_CAPS;

	oSurfDesc.dwHeight = p_vHeight;
    oSurfDesc.dwWidth = p_vWidth;
	
	oSurfDesc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	
	
	oSurfDesc.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
	oSurfDesc.ddpfPixelFormat.dwFlags = DDPF_RGB;
	oSurfDesc.ddpfPixelFormat.dwRGBBitCount = p_vBpP * 8; 

	oSurfDesc.ddpfPixelFormat.dwRBitMask = 0xff0000; 
	oSurfDesc.ddpfPixelFormat.dwGBitMask = 0x00ff00; 
	oSurfDesc.ddpfPixelFormat.dwBBitMask = 0x0000ff; 

	hr = m_opIDD -> CreateSurface(&oSurfDesc, &opISurface, NULL);

	return opISurface;
}

bool EMWinDirectXVideoTransition::Setup()
{
	HRESULT hr;

	IDXTransformFactory* opIFactory = NULL;
	IDXSurfaceFactory* opISurface = NULL;
	IBindCtx* opIBcJ = NULL;

	if(m_opIEffect != NULL)
		Shutdown();

	int32 vWidth = m_oFormat.m_vWidth;
	int32 vHeight = m_oFormat.m_vHeight;
	int32 vDepth = m_oFormat.m_vDepth;

	m_opIDDInput1 = CreateDDSurface(vWidth, vHeight, vDepth);
	m_opIDDInput2 = CreateDDSurface(vWidth, vHeight, vDepth);
	m_opIDDOutput = CreateDDSurface(vWidth, vHeight, vDepth);
	
	hr = CoCreateInstance(CLSID_DXTransformFactory, NULL, CLSCTX_INPROC, IID_IDXTransformFactory, (void **)&opIFactory);
	
	hr = opIFactory -> QueryService(SID_SDXSurfaceFactory, IID_IDXSurfaceFactory, (void**)&opISurface);
	
	hr = opISurface -> CreateFromDDSurface(m_opIDDInput1, NULL, DXSF_NO_LAZY_DDRAW_LOCK, NULL, IID_IDXSurface, (void**)&m_opIDXInput1);
	hr = opISurface -> CreateFromDDSurface(m_opIDDInput2, NULL, DXSF_NO_LAZY_DDRAW_LOCK, NULL, IID_IDXSurface, (void**)&m_opIDXInput2);
	hr = opISurface -> CreateFromDDSurface(m_opIDDOutput, NULL, DXSF_NO_LAZY_DDRAW_LOCK, NULL, IID_IDXSurface, (void**)&m_opIDXOutput);


	CreateBindCtx(0, &opIBcJ);
	hr = m_opIMoniker -> BindToObject(opIBcJ, NULL, IID_IDXTransform, (void **)&m_opIEffect);
	opIBcJ -> Release();
	opIBcJ = NULL;

	//hr = CoCreateInstance(CLSID_DxtJpeg, NULL, CLSCTX_INPROC, IID_IDXTransform, (void**)&m_opIEffect);
	hr = m_opIEffect -> QueryInterface(IID_IDXEffect, (void**)&m_opIDxTrans);

	UpdateSettings();  //Notify sub-class of change

	IUnknown* oInputs[2];
	
	oInputs[0] =  m_opIDXInput1;
	oInputs[1] =  m_opIDXInput2;
	 
	hr = opIFactory -> InitializeTransform(m_opIEffect, oInputs, 2, (IUnknown**)&m_opIDXOutput, 1, NULL, NULL);

	opIFactory -> Release();
	opISurface -> Release();

	return true;
}

bool EMWinDirectXVideoTransition::Shutdown()
{
	if(m_opIEffect != NULL)
	{
		m_opIEffect -> Release();
		m_opIEffect = NULL;
	}

	if(m_opIDxTrans != NULL)
	{
		m_opIDxTrans -> Release();
		m_opIDxTrans = NULL;
	}

	if(m_opIDXInput1 != NULL)
	{
		m_opIDXInput1 -> Release();
		m_opIDXInput1 = NULL;
	}

	if(m_opIDXInput2 != NULL)
	{
		m_opIDXInput2 -> Release();
		m_opIDXInput2 = NULL;
	}

	if(m_opIDXOutput != NULL)
	{
		m_opIDXOutput -> Release();
		m_opIDXOutput = NULL;
	}

	if(m_opIDDInput1 != NULL)
	{
		m_opIDDInput1 -> Release();
		m_opIDDInput1 = NULL;
	}

	if(m_opIDDInput2 != NULL)
	{
		m_opIDDInput2 -> Release();
		m_opIDDInput2 = NULL;
	}

	if(m_opIDDOutput != NULL)
	{
		m_opIDDOutput -> Release();
		m_opIDDOutput = NULL;
	}

	return true;
}

bool EMWinDirectXVideoTransition::SaveData(EMProjectDataSaver* p_opSaver) 
{
	EMVideoTransition::SaveData(p_opSaver);

	p_opSaver -> SaveFloat(m_oFormat.m_vFrameRate);
	p_opSaver -> SaveInt32(m_oFormat.m_vWidth);
	p_opSaver -> SaveInt32(m_oFormat.m_vHeight);
	p_opSaver -> SaveInt32(m_oFormat.m_vDepth);
	p_opSaver -> SaveInt64(m_oFormat.m_vBufferSizeBytes);
	p_opSaver -> SaveInt32(m_oFormat.m_vNumChannels);
	p_opSaver -> SaveInt32(m_oFormat.m_vBytesPerSample);
	p_opSaver -> SaveInt64(m_oFormat.m_vBytesPerRow);

	
	return true;
}

bool EMWinDirectXVideoTransition::LoadData(EMProjectDataLoader* p_opLoader) 
{
	EMVideoTransition::LoadData(p_opLoader);

	m_oFormat.m_vFrameRate = p_opLoader -> LoadFloat();
	m_oFormat.m_vWidth =	 p_opLoader -> LoadInt32();
	m_oFormat.m_vHeight =	 p_opLoader -> LoadInt32();
	m_oFormat.m_vDepth =	 p_opLoader -> LoadInt32();
	m_oFormat.m_vBufferSizeBytes =	p_opLoader -> LoadInt64();
	m_oFormat.m_vNumChannels =		p_opLoader -> LoadInt32();
	m_oFormat.m_vBytesPerSample =	p_opLoader -> LoadInt32();
	m_oFormat.m_vBytesPerRow =		p_opLoader -> LoadInt64();

	Shutdown();
	Setup();

	return true;
}

float EMWinDirectXVideoTransition::CalcTime(EMMediaDataBuffer* p_opBuffer)
{
	return EMVideoTransition::CalcTime(p_opBuffer);
}


#endif
