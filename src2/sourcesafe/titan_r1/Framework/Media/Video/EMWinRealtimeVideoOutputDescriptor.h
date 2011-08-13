/*******************************************************
* Creator: Jesper Svensson
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_REALTIME_VIDEO_OUTPUT_DESCRIPTOR
#define __EM_WIN_REALTIME_VIDEO_OUTPUT_DESCRIPTOR

#include "EMRealtimeOutputDescriptor.h"
#include "EMMediaDataBuffer.h"
#include "EMListenerRepository.h"
#include "EMListener.h"
#include "EMWinNodeConnection.h"
#include "EMVideoPreview.h"
#include "EMVideoClipMixer.h"

#include "EMWinRealtimeRenderToDiskOutputDescriptor.h"
#include "EMOutputRepository.h"

#include <list>

class EMProjectDataSaver;
class EMProjectDataLoader;
class EMWinMediaOutputFilter;

class EMWinRealtimeVideoOutputDescriptor : public EMRealtimeOutputDescriptor, public EMListener
{
public:
	EMWinRealtimeVideoOutputDescriptor(IBaseFilter* p_opRenderFilter);
	virtual ~EMWinRealtimeVideoOutputDescriptor();
	bool InitCheckE();
	bool PrepareToPlayE();
	void StartE();
	void StopE();
	EMMediaDataBuffer* ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList);
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
//	BBufferGroup* m_opBufferGroup;
	bool ClearData();
	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool LoadData(EMProjectDataLoader* p_opLoader);
	IPin* GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir);
	bool RegisterAsClockMaster();
	bool UnregisterAsClockMaster();

protected:
	bool m_vIsClockMaster;

	EMVideoPreview* m_opPreviewWindow;
	EMBitmap* m_opNewEMBeBitmapContainer;
	EMBitmap* m_opOldEMBeBitmapContainer;

	void* m_opBuffer;
	HBITMAP m_oBitmap;

private:
	EMRealtimeOutputDescriptor* GetVideoRenderToDiskDestination();

private:
	EMWinRealtimeRenderToDiskOutputDescriptor* m_vVideoRenderToDisk;
	EMVideoClipMixer opSimpleVideoDestination;
	int32 m_vOldWidth;
	int32 m_vOldHeight;
	BITMAPINFO BitmapInfo;
};

#endif

#endif
