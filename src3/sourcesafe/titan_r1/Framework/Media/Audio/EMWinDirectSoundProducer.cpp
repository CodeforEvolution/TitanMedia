#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinDirectSoundProducer.h"

#include "EMWinDirectSoundPlayback.h"
#include "EMThread.h"
#include "EMMediaIDManager.h"
#include "EMDSCache.h"
#include "EMMediaDataBuffer.h"


EMWinDirectSoundProducer::EMWinDirectSoundProducer(GUID* p_upDSDeviceGUID)
	:	m_upDSDeviceGUID(p_upDSDeviceGUID),
		m_opDSCache(NULL),
		m_opDSOutput(NULL),
		m_vIsInitialized(false)
{
	m_opDSCache = new EMDSCache(4096, 15);
	m_opThread = EMThread::CreateEMThread(string(string("DSProducer") + string(EMMediaIDManager::MakeUniqueString())).c_str(), EM_THREAD_HIGH_REALTIME_PRIORITY, 0);
	m_opThread -> AddListener(this);
}

EMWinDirectSoundProducer::~EMWinDirectSoundProducer()
{
	m_opThread -> Kill();
	delete m_opThread;
	m_opDSOutput -> Stop();
	delete m_opDSOutput;
}

bool EMWinDirectSoundProducer::InitCheckE()
{
	if(! m_vIsInitialized)
	{
		m_opDSOutput = EM_new EMWinDirectSoundPlayback(m_upDSDeviceGUID);

		WNDCLASSEX sWindowClass;
		sWindowClass.cbSize = sizeof(WNDCLASSEX);
		sWindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		sWindowClass.lpfnWndProc = WindowProc;
		sWindowClass.cbClsExtra = 0;
		sWindowClass.cbWndExtra = 0;       
		sWindowClass.hInstance = NULL; //hInstance;
		sWindowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		sWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		sWindowClass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
		sWindowClass.lpszMenuName = NULL;
		sWindowClass.lpszClassName = "DummyWindow";
		sWindowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		RegisterClassEx(&sWindowClass);
		HWND upDummyWindowHandle = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "DummyWindow", "DummyWindow", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, 0, 0, 10, 10, NULL, NULL, /*hInstance*/NULL, NULL);

		if(m_opDSOutput -> InitCheckE(upDummyWindowHandle))
		{
			if(m_opDSOutput -> Start())
			{ 
				m_opThread -> Start();
				m_vIsInitialized = true;
			}
		}
	}
	return m_vIsInitialized;
}

bool EMWinDirectSoundProducer::Flush()
{
	m_opDSCache -> PrepareToFlush();
	m_opDSCache -> Signal();
	m_opThread -> Suspend();
	m_opDSCache -> Flush();
	m_opDSCache -> FlushComplete();
	m_opDSOutput -> Flush();
	m_opThread -> Resume();
	return true;
}

bool EMWinDirectSoundProducer::Start()
{
	return true;
}

bool EMWinDirectSoundProducer::Stop()
{
	return true;
}

bool EMWinDirectSoundProducer::ProcessBufferE(EMMediaDataBuffer* p_opBuffer)
{
	if(! m_opDSCache -> Put(static_cast<char*>(p_opBuffer -> Data()), p_opBuffer -> m_vSizeUsed))
	{
		//TODO: Handle this!!
		return false;
	}
	return true;
}

void EMWinDirectSoundProducer::OnThreadCreated(EMThread* p_opThread)
{
	HRESULT vResult = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(vResult))
		MessageBox(NULL, "ERROR! Failed to initialize COM system in ~EMWinMIDISystem!", "Error!", MB_OK);
}

void EMWinDirectSoundProducer::OnThreadKilled(EMThread* p_opThread)
{
	CoUninitialize();
}

void EMWinDirectSoundProducer::ThreadRun(EMThread* p_opThread)
{
	char vpBuffer[4096];
	uint64 vSize = 0;
	if(m_opDSCache -> Get(vpBuffer, &vSize))
	{
		m_opDSOutput -> Deliver(vpBuffer, vSize);
		::Sleep(18);
	}
	else
		::Sleep(5);
}

#endif