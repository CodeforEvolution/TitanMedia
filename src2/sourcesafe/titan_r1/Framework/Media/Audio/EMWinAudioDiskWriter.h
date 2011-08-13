/*******************************************************
* Creator: Martin Johansson
* Portability: Native
*-------------------------------------------------------
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_WIN_AUDIO_DISK_WRITER
#define __EM_WIN_AUDIO_DISK_WRITER

#include "EMMediaDataBuffer.h"
#include "EMWinAudioConverterInterface.h"
#include "EMMediaDataBuffer.h"
#include "EMWinVideoShuffleOutputInterface.h"
#include "EMWinAudioConverterInterface.h"

#include <list>
#include <dshow.h>

class EMWinAudioDiskWriter
{
public:
	~EMWinAudioDiskWriter();

	static EMWinAudioDiskWriter* Instance();
	static void Delete();

	bool IsRunning();

	bool InitConverter(int p_vSampleRate, int p_vBits, bool p_vIsMono);
	bool SetRenderName(char* p_opFileName);

	bool Start();
	bool Stop();

	EMMediaDataBuffer* ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList);

protected:
	EMWinAudioDiskWriter();
	IPin* GetPin(IBaseFilter* p_opFilter, PIN_DIRECTION p_oPinDir);

protected:
	bool m_vIsRunning;

	string m_oName;

private:
	IGraphBuilder* m_opIGraph;
	IMediaControl* m_opICtrl;

	IBaseFilter* m_opBaseConverter;
	IBaseFilter* m_opBaseWave;
	IBaseFilter* m_opBaseShuffle;

	IPin* m_opShuffleOutput;
	IPin* m_opConverterInput;
	IPin* m_opConverterOutput;
	IPin* m_opWaveInput;
	IPin* m_opWaveOutput;
	IPin* m_opWriterInput;

	IBaseFilter* m_opIBaseWriter;
	IFileSinkFilter* m_opIFileSink;

	EMWinVideoShuffleOutputInterface* m_opIShuffle;
	EMWinAudioConverterInterface* m_opConverter;

	static EMWinAudioDiskWriter* m_opInstance;
};

#endif
