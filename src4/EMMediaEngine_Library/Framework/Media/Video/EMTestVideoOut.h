/*******************************************************
* Creator: Richard Kronfalt
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#ifndef __EM_TEST_VIDEO_OUTPUT
#define __EM_TEST_VIDEO_OUTPUT

#include "EMRealtimeOutputDescriptor.h"
#include "EMMediaDataBuffer.h"
#include "EMListenerRepository.h"
#include "EMListener.h"
#include "EMWinNodeConnection.h"
#include "EMVideoPreview.h"

#include <list>

class EMProjectDataSaver;
class EMProjectDataLoader;
class EMWinMediaOutputFilter;

class EMTestVideoOut
{
public:
	EMTestVideoOut();
	virtual ~EMTestVideoOut();
	bool InitCheckE(EMVideoPreview* m_opPreviewWindow);
	bool PrepareToPlayE();
	void StartE();
	void StopE();
	EMMediaDataBuffer* ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList);
	bool ClearData();
	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool LoadData(EMProjectDataLoader* p_opLoader);
	IPin* GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir);
	void ProcessBufferLikeHell(EMMediaDataBuffer* p_opDataBuffer);

private:
	EMWinNodeConnection m_oConnection;
//	sem_id m_vSem;
//	BBufferProducer* m_opNode;
};

#endif

#endif
