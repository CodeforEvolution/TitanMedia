#include "EMDestructiveDXWrapper.h"
#include "EMWaveFileReader.h"
#include "EMWaveFileWriter.h"
#include "EMMediaFormat.h"
#include "EMMediaDataBuffer.h"
#include "EMPluginEntry.h"
#include "EMMediaEngine.h"
#include "EMSettingsRepository.h"
#include "EMSettingIDs.h"
#include "EMWinDirectXPlugin.h"

char* EMDestructiveDXWrapper::m_opTemp = NULL;
uint32 EMDestructiveDXWrapper::m_vLength = 0;

EMDestructiveDXWrapper::EMDestructiveDXWrapper()
	: EMDestructivePlugin("EM DX", EM_DESTRUCTIVE),
	m_opDXEntry(NULL)
{
}

EMDestructiveDXWrapper::EMDestructiveDXWrapper(EMPluginEntry* p_opDXPluginEntry)
	: EMDestructivePlugin("EM DX", EM_DESTRUCTIVE),
	m_opDXEntry(p_opDXPluginEntry)
{
}

EMDestructiveDXWrapper::~EMDestructiveDXWrapper()
{
	//Clean up...
	if(m_opTemp != NULL)
		delete m_opTemp;
}


bool EMDestructiveDXWrapper::DoPlugin(char* p_opDataSource, char* p_opDataDest, int64 p_vLen, int64* p_vpParams, int64 p_vStart, int64 p_vStop)
{
	list<EMMediaDataBuffer*> oList;

	EMPlugin* opPlug = NULL;
	EMMediaDataBuffer oBuffer(EM_TYPE_RAW_AUDIO, static_cast<int64>(*static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_BUFFER_SIZE))), NULL);

	bool vShowDialog = p_vpParams[1] == 0;

	char* opSrc = reinterpret_cast<char*>(p_opDataSource);
	char* opDst = reinterpret_cast<char*>(p_opDataDest);

	char* opBufferData = static_cast<char*>(oBuffer.Data());
	
	try
	{
		opPlug = m_opDXEntry -> InstantiateNativePlugin();

		if(vShowDialog || m_opTemp == NULL)
		{	
			opPlug -> ShowDialog();

			if(m_opTemp != NULL)
				delete m_opTemp;

			bool tmp = dynamic_cast<EMWinDirectXPlugin*>(opPlug) -> ReadInternalSettings(&m_opTemp, &m_vLength);
		}
		else
			bool tmp2 = dynamic_cast<EMWinDirectXPlugin*>(opPlug) -> WriteInternalSettings(m_opTemp, m_vLength);

	}
	catch(...)
	{
		EMDebugger("EMDestructiveDXWrapper::DoPlugin");
	}

	int64 vFrame = 0;
	int64 vLenLeft = p_vLen - 4;

	opPlug -> SetDestination(opPlug);

//	char* tmp[4096];
	char* tmp[4096 * 16];

	while(vLenLeft != 0)
	{
//		oBuffer.SetDataPointer(opDst);
		oBuffer.SetDataPointer(tmp);
		oBuffer.m_vSizeAvailable = __min(vLenLeft, static_cast<int64>(*static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_BUFFER_SIZE))));
		oBuffer.m_vSizeUsed = oBuffer.m_vSizeAvailable;

//		memcpy(opDst, opSrc, oBuffer.m_vSizeUsed);
		memcpy(tmp, opSrc, oBuffer.m_vSizeUsed);

		oList.push_front(&oBuffer);

		oBuffer.SetFrame(vFrame);

		opPlug -> ProcessBufferE(&oList);
		oList.clear();

		memcpy(opDst, tmp, oBuffer.m_vSizeUsed);

		vLenLeft -= oBuffer.m_vSizeAvailable;
		opSrc += oBuffer.m_vSizeAvailable;
		opDst += oBuffer.m_vSizeAvailable;
		vFrame += oBuffer.m_vSizeAvailable / m_vBytesPerFrame;
	}
	
//	delete opPlug;

	return true;
}


EMPlugin* EMDestructiveDXWrapper::Clone()
{
	return EM_new EMDestructiveDXWrapper(m_opDXEntry);
}

void* EMDestructiveDXWrapper::GetNativeEffect() const
{
	return m_opDXEntry -> InstantiateNativePlugin();
}

int EMDestructiveDXWrapper::NeedExtraParams()
{
	return 2;
}