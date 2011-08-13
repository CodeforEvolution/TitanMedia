/*******************************************************
* Creator: Jesper Svensson
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_WIN_REALTIME_RENDERTODISK_OUTPUT_DESCRIPTOR
#define __EM_WIN_REALTIME_RENDERTODISK_OUTPUT_DESCRIPTOR

#include "EMRealtimeOutputDescriptor.h"
#include "EMMediaDataBuffer.h"
#include "EMListenerRepository.h"
#include "EMListener.h"
#include "EMWinNodeConnection.h"
#include "EMVideoPreview.h"
#include "EMVideoClipMixer.h"
#include "EMWinVideoShuffleOutputInterface.h"
#include "EMWinMediaSingletonMuxer.h"
#include <list>
#include "EMMediaTimer.h"
#include "EMWinQuickTimeWriterSuper.h"
class EMProjectDataSaver;
class EMProjectDataLoader;
class EMWinMediaOutputFilter;

class EMWinRealtimeRenderToDiskOutputDescriptor : public EMRealtimeOutputDescriptor, public EMListener
{
public:
	void CreateFileRendererFilter();
	EMWinRealtimeRenderToDiskOutputDescriptor(IBaseFilter* p_opRenderFilter, EMMediaType p_eEMMediaType);
	virtual ~EMWinRealtimeRenderToDiskOutputDescriptor();
	bool InitCheckE();
	bool PrepareToPlayE();
	void StartE();
	void StopE();
	EMMediaDataBuffer* ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList);
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	bool ClearData();
	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool LoadData(EMProjectDataLoader* p_opLoader);
	IPin* GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir);
	void SetShuffleFormat(EMMediaFormat* p_opMediaFormat);
	bool ConnectWithEncoder(int32 p_vEncoderID);
	bool DisConnectWithEncoder();
	IPin* GetSourceOutPin();
	bool RegisterAsClockMaster();// { return true; };
	bool UnregisterAsClockMaster();// { return true; };
private:
	//EMRealtimeEncoderDescriptor* GetEncoderDescriptor(int32 p_vEncoderID);
private:
	IPin* m_opMuxInPin;
	EMWinNodeConnection m_oConnection;
	EMVideoClipMixer opSimpleVideoDestination;
	EMWinVideoShuffleOutputInterface* m_opSourceFilterInterface;
	bool m_vShuffleFormatSet;
	bool m_vFileRendererCreated;
	bool m_vAllConnectionsMade;
	bool m_vIsRunning;
	EMMediaFormat* m_opMediaFormat;
	EMRealtimeEncoderDescriptor* m_opRealtimeEncoderDescriptor;
	int32 counter;
	EMWinMediaSingletonMuxer* m_opSingletonMuxer;
	EMWinQuickTimeWriterSuper* m_opQuicktimeWriter;
	EMMediaType m_eEMMediaType;
};

#endif

#endif
