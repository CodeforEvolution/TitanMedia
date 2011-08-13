#include "MediaInfoCommandGetInputSignalMeters.h"

#include "EMInputRepository.h"
#include "EMInputRepository.h"
#include "EMWinRealtimeAudioInputDescriptor.h"
#include "EMMediaSystemInspector.h"
#include "EMMediaSignalMeter.h"
#include "EMMediaInputAudioSignalMeter.h"

MediaInfoCommandGetInputSignalMeters::MediaInfoCommandGetInputSignalMeters()
{
	for(int i = 0; i < 50*2; ++i)
		m_vReturn[i] = -1;
}

void* MediaInfoCommandGetInputSignalMeters::ExecuteE(void*, void*, void*)
{
	//cout << "MediaInfoCommandGetInputSignalMeters" << endl;
	int vCnt = 0;
	float vTmp[5];

	EMInputRepository* opInputs = EMInputRepository::Instance();
	
	opInputs -> LockContainer();
	
	try
	{
		opInputs -> Rewind();
		while(opInputs -> Current() != NULL)
		{
			if((opInputs -> Current() -> GetType() & EM_TYPE_ANY_AUDIO) > 0)
			{
				EMMediaSignalMeter* opMeter = static_cast<EMWinRealtimeAudioInputDescriptor*>(opInputs -> Current()) -> GetSignalMeter();

				static_cast<EMMediaInputAudioSignalMeter*>(opMeter) -> Get(vTmp);

				m_vReturn[vCnt++] = vTmp[0];
				m_vReturn[vCnt++] = vTmp[1];

//				eo << "MediaInfoCommandGetInputSignalMeters::ExecuteE - Value(" << vCnt/2 <<") - " << vTmp[0] << " / " << vTmp[1] << ef;
			}
			opInputs -> Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception in MediaInfoCommandGetInputSignalMeters::ExecuteE");
	}
	opInputs -> UnlockContainer();

	return static_cast<void*>(m_vReturn);
}

bool MediaInfoCommandGetInputSignalMeters::RequiresParameters()
{
	return true;
}

bool MediaInfoCommandGetInputSignalMeters::IsUndoable()
{
	return false;
}
