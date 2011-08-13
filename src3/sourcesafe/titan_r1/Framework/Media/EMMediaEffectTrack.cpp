#include "EMMediaEffectTrack.h"

#include "EMRealtimeOutputDescriptor.h"
#include "EMOutputRepository.h"
#include "EMMediaTrack.h"
#include "EMMediaDataBuffer.h"
#include "EMBufferDestination.h"
#include "EMSemaphore.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"
#include "EMPluginRepository.h"
#include "EMPlugin.h"
#include "EMMediaEffectTrackRepository.h"
#include "EMMediaTimer.h"
#include "EMAudioClipMixer.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMMediaTrackRepository.h"

//Temp
#include "EMWinDirectXPlugin.h"

#include <map>

EMMediaEffectTrack::EMMediaEffectTrack(EMMediaType p_eType)
	:	EMBufferDestination(p_eType, EM_EFFECT, string("EMMediaEffectTrack")),
		m_oName("Untitled effect track"),
		m_opSlots(NULL),
		m_vNumSlots(100),
		m_opOutput(NULL),
		m_opSourceMediaTrack(NULL),
		m_vOutputId(-1),
		m_vIsDeleted(false)
{
	m_opSemaphore = EMSemaphore::CreateEMSemaphore(NULL, 1, 1); //"EffectTrack prot sem", 1, 1);
}

EMMediaEffectTrack::EMMediaEffectTrack(EMMediaEffectTrack& oTrack)
	:	EMBufferDestination(oTrack.GetType(), EM_EFFECT, string("EMMediaEffectTrack")),
	m_oName(oTrack.GetName()),
	m_opSlots(NULL),
	m_vNumSlots(oTrack.m_vNumSlots),
	m_opOutput(oTrack.m_opOutput),
	m_opSourceMediaTrack(oTrack.m_opSourceMediaTrack),
	m_vOutputId(oTrack.m_vOutputId),
	m_vIsDeleted(oTrack.m_vIsDeleted)
{
	m_opSemaphore = EMSemaphore::CreateEMSemaphore(NULL, 1, 1); //"EffectTrack prot sem", 1, 1);

	InitCheckE();

	for(uint32 vIndex(0); vIndex < m_vNumSlots; vIndex++)
	{
		if(oTrack.m_opSlots[vIndex] != NULL)
		{
			EMPlugin* opCurrentPlug = oTrack.m_opSlots[vIndex];
			InsertEffect(opCurrentPlug -> Clone(), vIndex);
		}
	}
}


EMMediaEffectTrack::~EMMediaEffectTrack()
{
	for(uint32 vIndex(0); vIndex < m_vNumSlots; vIndex++)
	{
		delete m_opSlots[vIndex];
		m_opSlots[vIndex] = NULL;
	}
	delete m_opSlots;
	m_opSlots = NULL;
	delete m_opSemaphore;
	m_opSemaphore = NULL;
}

bool EMMediaEffectTrack::InitCheckE()
{
	m_opSlots = EM_new EMPlugin*[m_vNumSlots];
	for(uint32 vSlotIndex = 0; vSlotIndex < m_vNumSlots; vSlotIndex++)
		m_opSlots[vSlotIndex] = NULL;
	return true;	
}

string EMMediaEffectTrack::GetName() const
{
	return m_oName;
}

void EMMediaEffectTrack::SetName(string p_oName)
{
	m_oName = p_oName;
}

bool EMMediaEffectTrack::PrepareToPlayE()
{
	return true;
}

EMMediaDataBuffer* EMMediaEffectTrack::ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList)
{
	list<EMMediaDataBuffer*> opList;
	EMMediaDataBuffer* opBuffer = NULL;

	opBuffer = p_opBufferList -> front();

	if(GetDestination() != NULL && GetDestination() != GetOutput() && !(opBuffer -> m_vIsSilence  && (opBuffer -> m_oFormat.m_eType & EM_TYPE_ANY_VIDEO) != 0))
	{
		m_opSemaphore -> Acquire();
		opBuffer = GetDestination() -> ProcessBufferE(p_opBufferList);
		if(opBuffer != NULL)
		{
			while((opBuffer -> m_opDestination -> GetPosition() == EM_EFFECT) && (opBuffer -> m_opDestination != GetOutput())) //  Type() & EM_OUTPUT_TYPE_EFFECTSLOT) > 0)
			{
				opList.push_front(opBuffer);
				opBuffer -> m_opDestination -> ProcessBufferE(&opList);
				opList.clear();
			}
			opBuffer -> m_opDestination = GetOutput(); //Finally, set the destination of the buffer to be the FX track's output - then we're done!
		}
		m_opSemaphore -> Release();
	}
	else
	{
		m_opSemaphore -> Acquire();
		opBuffer -> m_opDestination = GetOutput();
		m_opSemaphore -> Release();
	}
	return opBuffer;
}

EMBufferDestination* EMMediaEffectTrack::GetNextDestination(uint32 p_vCurrentSlot)
{
	for(uint32 vIndex = p_vCurrentSlot + 1; vIndex < m_vNumSlots; vIndex++)
	{
		if(m_opSlots[vIndex] != NULL)
			return m_opSlots[vIndex];
	}

	//If there were no effect, return the track's destination!
	return GetOutput();
}

EMBufferDestination* EMMediaEffectTrack::GetPreviousDestination(uint32 p_vCurrentSlot)
{
	//If it's the first plugin in the track, then the previous destination (that will send to that plugin)
	//is the effect track itself!
	if(p_vCurrentSlot == 0)
		return this;

	for(uint32 vIndex = p_vCurrentSlot - 1; vIndex >= 0; vIndex--)
	{
		if(m_opSlots[vIndex] != NULL)
			return m_opSlots[vIndex];
	}

	//If there were no effects prior to this plugin, then the track itself is the previous destination.
	return this;
}

bool EMMediaEffectTrack::MakeRoom(uint32 p_vSlotNumber)
{
	if(m_opSlots[m_vNumSlots -1] != NULL)
		return false;

	for(int32 vIndex = m_vNumSlots -1; vIndex > p_vSlotNumber; vIndex--)
	{
		m_opSlots[vIndex] = m_opSlots[vIndex - 1];
		m_opSlots[vIndex - 1] = NULL;
	}
	return true;
}

bool EMMediaEffectTrack::InsertEffect(EMPlugin* p_opPlugin, uint32 p_vSlotNumber)
{
	if(p_opPlugin == NULL)
		return false;
	if(p_vSlotNumber < 0 || p_vSlotNumber >= m_vNumSlots)
		return false;

	m_opSemaphore -> Acquire();
	//Make sure that there is an empty slot where we want the new plugin
	if(! IsSlotEmpty(p_vSlotNumber))
	{
		if(! MakeRoom(p_vSlotNumber))
		{
			m_opSemaphore -> Release();
			EMDebugger("ERROR! No more room for effects!");
			return false;
		}
	}

	//There.. We have an empty spot!
	//Insert the plugin now, and connect it to the surrounding (if any) effects and to the effect track's output.
	m_opSlots[p_vSlotNumber] = p_opPlugin;

	GetPreviousDestination(p_vSlotNumber) -> SetDestination(p_opPlugin);
	p_opPlugin -> SetDestination(GetNextDestination(p_vSlotNumber));
	m_opSemaphore -> Release();
//	p_opPlugin -> ShowDialog();
	return true;
}

bool EMMediaEffectTrack::SetEffect(EMPlugin* p_opPlugin, uint32 p_vSlotNumber)
{
	if(p_opPlugin == NULL)
		return false;
	if(p_vSlotNumber < 0 || p_vSlotNumber >= m_vNumSlots)
		return false;
	if(IsSlotEmpty(p_vSlotNumber))
		return false;

	m_opSemaphore -> Acquire();
	if(p_vSlotNumber > 0)
	{
		EMPlugin* opOldPlugin = m_opSlots[p_vSlotNumber];
		p_opPlugin -> SetDestination(opOldPlugin -> GetDestination());
		m_opSlots[p_vSlotNumber - 1] -> SetDestination(p_opPlugin);
		m_opSlots[p_vSlotNumber] = p_opPlugin;
		delete opOldPlugin;
	}
	else
	{
		EMPlugin* opOldPlugin = m_opSlots[p_vSlotNumber];
		p_opPlugin -> SetDestination(opOldPlugin -> GetDestination());
		SetDestination(p_opPlugin);
		m_opSlots[p_vSlotNumber] = p_opPlugin;
		delete opOldPlugin;
	}
	m_opSemaphore -> Release();
//	m_opSlots[p_vSlotNumber] -> ShowDialog();
	return true;
}

bool EMMediaEffectTrack::DeleteEffect(uint32 p_vSlotNumber)
{
	if(p_vSlotNumber < 0 || p_vSlotNumber >= m_vNumSlots)
		return false;
	if(IsSlotEmpty(p_vSlotNumber))
		return false;

	m_opSemaphore -> Acquire();

	EMPlugin* opOldPlugin = m_opSlots[p_vSlotNumber];
	EMBufferDestination* opSender = GetPreviousDestination(p_vSlotNumber);
	EMBufferDestination* opNextReceiver = GetNextDestination(p_vSlotNumber);
	opSender -> SetDestination(opNextReceiver);
	m_opSlots[p_vSlotNumber] = NULL;
	delete opOldPlugin;
	opOldPlugin = NULL;

	//Move every element after "this" position one step to the "left", since we've removed one slot and made it NULL.
	for(uint32 v = p_vSlotNumber; v < m_vNumSlots - 1; v++)
	{
		if(m_opSlots[v + 1] != NULL && m_opSlots[v] == NULL)
		{
			m_opSlots[v] = m_opSlots[v + 1];
			m_opSlots[v + 1] = NULL;
		}
	}

	m_opSemaphore -> Release();

	return true;
}

bool EMMediaEffectTrack::IsSlotEmpty(uint32 p_vSlotNumber)
{
	return (m_opSlots[p_vSlotNumber] == NULL);
}
/*
bool EMMediaEffectTrack::IsFirstInChain(uint32 p_vSlotNumber)
{
	for(uint32 vSlotIndex = p_vSlotNumber - 1; vSlotIndex >= 0; vSlotIndex--)
		if(m_opSlots[vSlotIndex] != NULL)
			return false;
	return true;
}

bool EMMediaEffectTrack::IsLastInChain(uint32 p_vSlotNumber)
{
	for(uint32 vSlotIndex = p_vSlotNumber + 1; vSlotIndex < m_vNumSlots; vSlotIndex++)
		if(m_opSlots[vSlotIndex] != NULL)
			return false;
	return true;
}

uint32 EMMediaEffectTrack::GetPreviousEffect(uint32 p_vFromSlotIndex)
{
	for(uint32 vSlotIndex = p_vFromSlotIndex - 1; vSlotIndex >= 0; vSlotIndex--)
		if(m_opSlots[vSlotIndex] != NULL)
			return vSlotIndex;
	return -1;
}

uint32 EMMediaEffectTrack::GetNextEffect(uint32 p_vFromSlotIndex)
{
	for(uint32 vSlotIndex = p_vFromSlotIndex + 1; vSlotIndex < m_vNumSlots; vSlotIndex++)
		if(m_opSlots[vSlotIndex] != NULL)
			return vSlotIndex;
	return -1;
}
*/
uint32 EMMediaEffectTrack::GetNumberOfSlots() const
{
	return m_vNumSlots;
}
/*
uint32 EMMediaEffectTrack::GetLastAvailableSlot()
{
	for(uint32 vSlotIndex = m_vNumSlots - 1; vSlotIndex >= 0; vSlotIndex--)
	{
		if(m_opSlots[vSlotIndex] != NULL)
		{
			if(vSlotIndex + 1 >= m_vNumSlots)
			{
				return -1;
			}
			else
			{
				if(m_opSlots[vSlotIndex + 1] == NULL)
				{
					return vSlotIndex + 1;
				}
				else
				{
					return -1;
				}
			}
		}
	}
	return 0; //First slot is available!
}

uint32 EMMediaEffectTrack::GetFirstAvailableSlot()
{
	for(uint32 vSlotIndex = 0; vSlotIndex < m_vNumSlots; vSlotIndex++)
	{
		if(m_opSlots[vSlotIndex] == NULL)
			return vSlotIndex;
	}
	return -1; //No slot available!
}
*/

uint32 EMMediaEffectTrack::GetLastEffect()
{
	for(uint32 vSlotIndex = m_vNumSlots - 1; vSlotIndex >= 0; vSlotIndex--)
	{
		if(m_opSlots[vSlotIndex] != NULL)
			return vSlotIndex;
	}
	return -1;
}

uint32 EMMediaEffectTrack::CountEffects()
{
	uint32 vCount = 0;
	for(uint32 vSlotIndex = m_vNumSlots - 1; vSlotIndex >= 0; vSlotIndex++)
	{
		if(m_opSlots[vSlotIndex] != NULL)
			vCount++;
	}
	return vCount;
}

void EMMediaEffectTrack::DisplaySlots()
{
	for(uint32 vSlotIndex = 0; vSlotIndex < m_vNumSlots; vSlotIndex++)
	{
		if(m_opSlots[vSlotIndex] != NULL)
		{
			eo << "SLOT #" << vSlotIndex << ":\t\t" << m_opSlots[vSlotIndex] -> GetName().c_str() << "\t\t" << m_opSlots[vSlotIndex] -> GetID() << ef;
			if(m_opSlots[vSlotIndex] -> GetDestination() != NULL)
				eo << "     Connected to: \t" << m_opSlots[vSlotIndex] -> GetDestination() -> GetID() << ef;
			else
				eo << "     Connected to: \tn\\a" << ef;
		}
		else
		{
			eo << "SLOT #" << vSlotIndex << ":\t\tn\\a\t\tn\\a" << ef;
			eo << ef;
		}
		eo << ef;
	}
}

bool EMMediaEffectTrack::SetOutput(EMBufferDestination* p_opOutput)
{
	if(p_opOutput == NULL)
		return false;

	m_opSemaphore -> Acquire();

	if(m_opOutput != p_opOutput)
	{
//		this -> SetOutput(p_opOutput);
		m_opOutput = p_opOutput;

		m_vOutputId = p_opOutput -> GetID();

		int32 vLastOccupiedSlot = GetLastEffect();

		if(vLastOccupiedSlot != -1)
			m_opSlots[vLastOccupiedSlot] -> SetDestination(p_opOutput);
		else
			SetDestination(m_opOutput);
	}

	m_opSemaphore -> Release();
	return true;
}

EMBufferDestination* EMMediaEffectTrack::GetOutput() const
{
	return m_opOutput;
}
/*
bool EMMediaEffectTrack::SetSourceMediaTrack(EMMediaTrack* p_opSourceTrack)
{
	if(p_opSourceTrack == NULL)
		return false;

	if(m_opSourceMediaTrack != p_opSourceTrack)
	{
		p_opSourceTrack -> SetTrackDestination(this);
		m_opSourceMediaTrack = p_opSourceTrack;
		eo << "FX track " << GetID() << " now setting media track " << p_opSourceTrack ->GetID() << "'s destination to be self (" << GetID() << ")" << ef;
	}
	return true;
}

EMMediaTrack* EMMediaEffectTrack::GetSourceMediaTrack() const
{
	return m_opSourceMediaTrack;
}
*/
EMPlugin* EMMediaEffectTrack::FindEffect(int32 p_vID)
{
	for(uint32 vSlotIndex = 0; vSlotIndex < m_vNumSlots; vSlotIndex++)
		if(m_opSlots[vSlotIndex] != NULL)
			if(m_opSlots[vSlotIndex] ->GetID() == p_vID)
				return m_opSlots[vSlotIndex];
	return NULL;
}

EMPlugin* EMMediaEffectTrack::GetSlot(uint32 p_vIndex)
{
	if(p_vIndex >= m_vNumSlots || p_vIndex < 0 || m_opSlots == NULL)
		return NULL;
	return m_opSlots[p_vIndex];
}

int32 EMMediaEffectTrack::GetSlotFor(EMPlugin* p_opPlugin)
{
	for(int32 vIndex = 0; vIndex < m_vNumSlots; vIndex++)
		if(m_opSlots[vIndex] == p_opPlugin)
			return vIndex;
	return -1;
}

bool EMMediaEffectTrack::SaveData(EMProjectDataSaver* p_opSaver)
{
	EMBufferDestination::SaveData(p_opSaver);
	p_opSaver -> SaveString("EMMediaEffectTrack_DATABEGIN");
	p_opSaver -> SaveString(m_oName.c_str());

	uint32 vCount = 0;
	for(uint32 vIndex = 0; vIndex < m_vNumSlots; vIndex++)
	{
		if(m_opSlots[vIndex] != NULL)
			vCount++;
	}

	p_opSaver -> SaveUInt32(vCount);

	for(vIndex = 0; vIndex < m_vNumSlots; vIndex++)
	{
		if(m_opSlots[vIndex] != NULL)
		{
			p_opSaver -> SaveUInt32(vIndex);
			p_opSaver -> SaveString((m_opSlots[vIndex] -> GetName()).c_str());
//			p_opSaver -> SaveInt32(m_opSlots[vIndex] -> GetID());
			m_opSlots[vIndex] -> SaveData(p_opSaver);
			m_opSlots[vIndex] -> SaveSetting(p_opSaver);
		}
	}

	if(m_opOutput == NULL)
	{	
		//This is an error anyhow...
		p_opSaver -> SaveInt32(EM_EFFECT);
		p_opSaver -> SaveInt32(-1);
	}
	else
	{
		switch(m_opOutput -> GetPosition())
		{
		case EM_CLIP:
			//AUDIO clip-mixer
			p_opSaver -> SaveInt32(EM_CLIP);
			p_opSaver -> SaveInt32(reinterpret_cast<EMAudioClipMixer*>(m_opOutput) -> GetParentId());
			break;
		case EM_EFFECT:
			//Anuther fx-track
			p_opSaver -> SaveInt32(EM_EFFECT);
			p_opSaver -> SaveInt32(m_opOutput -> GetID()); 
			break;
		case EM_OUTPUT:
			//An output??
			p_opSaver -> SaveInt32(EM_OUTPUT);
			p_opSaver -> SaveString(static_cast<EMOutputDescriptor*>(m_opOutput) -> GetName().c_str()); 
			break;
		default:
			EMDebugger("EMMediaEffectTrack::SaveData - Error code 1");
			break;
		}
	}

	p_opSaver -> SaveString("EMMediaEffectTrack_DATAEND");
	return true;
}

bool EMMediaEffectTrack::LoadData(EMProjectDataLoader* p_opLoader)
{
	string oOutput;

	EMBufferDestination::LoadData(p_opLoader);
	if(string(p_opLoader -> LoadString()) != string("EMMediaEffectTrack_DATABEGIN"))
	{
		EMDebugger("ERROR! Error in project file!");
		return false;
	}
	m_oName = string(p_opLoader -> LoadString());

	uint32 vNumPlugins = p_opLoader -> LoadUInt32();

	for(uint32 vIndex = 0; vIndex < vNumPlugins; vIndex++)
	{
		uint32 vPosition = p_opLoader -> LoadUInt32();

		string oName(p_opLoader -> LoadString());

		EMPluginRepository::Instance() -> LockContainer();
		EMPluginRepository::Instance() -> Rewind();
		while(EMPluginRepository::Instance() -> Current() != NULL)
		{
			EMPluginEntry* opEntry = EMPluginRepository::Instance() -> Current();
			if(opEntry -> GetName() == oName)
			{
				EMPlugin* opPlugin = opEntry -> InstantiateNativePlugin();
				opPlugin -> LoadData(p_opLoader);
				opPlugin -> LoadSetting(p_opLoader);
				InsertEffect(opPlugin, vPosition);
			}
			EMPluginRepository::Instance() -> Next();
		}
		EMPluginRepository::Instance() -> UnlockContainer();
	}

	int32 vType = p_opLoader -> LoadInt32();

	switch(vType)
	{
	case EM_CLIP:
		m_vOutputId = p_opLoader -> LoadInt32();
		SetOutput(NULL);
		break;

	case EM_EFFECT:
		m_vOutputId = p_opLoader -> LoadInt32();
		SetOutput(NULL);
		break;

	case EM_OUTPUT:
		oOutput = p_opLoader -> LoadString();

		EMOutputRepository::Instance() -> LockContainer(); 
		EMOutputRepository::Instance() -> Rewind();
		while(EMOutputRepository::Instance() -> Current() != NULL && EMOutputRepository::Instance() -> Current() -> GetName() != oOutput)
			EMOutputRepository::Instance() -> Next();

		if(EMOutputRepository::Instance() -> Current() != NULL)
		{
			SetOutput(EMOutputRepository::Instance() -> Current());
		}
		else
		{		
			if((GetType() & EM_TYPE_ANY_AUDIO) > 0)
			{
				EMRealtimeOutputDescriptor* opOut = EMOutputRepository::Instance() -> First(EM_TYPE_ANY_AUDIO);
				SetOutput(opOut);
			}
			else if((GetType() & EM_TYPE_ANY_VIDEO) > 0)
			{
				EMRealtimeOutputDescriptor* opOut = EMOutputRepository::Instance() -> First(EM_TYPE_ANY_VIDEO);
				SetOutput(opOut);
			}
		}

		if(GetOutput() != NULL)
			m_vOutputId = GetOutput() -> GetID();
		else
			m_vOutputId = -1;
		EMOutputRepository::Instance() -> UnlockContainer();
		break;
	default:
		EMDebugger("EMMediaEffectTrack::LoadData - Error code 1");
		break;

	}

	if(string(p_opLoader -> LoadString()) != string("EMMediaEffectTrack_DATAEND"))
	{
		EMDebugger("ERROR! Error in project file!");
		return false;
	}
	return true;
}

bool EMMediaEffectTrack::RestoreAfterLoad()
{
	EMMediaEffectTrack* opFXTrack = NULL;
	EMMediaTrack* opTrack = NULL;
	bool vIsFx = true;

	if(m_vOutputId != -1)
	{
		try
		{
			 opFXTrack = EMMediaEffectTrackRepository::Instance() -> Find(m_vOutputId);	
		}
		catch(...)
		{
			return false;
		}

		if(opFXTrack != NULL)
			SetOutput(opFXTrack);
	}

	if(opFXTrack == NULL)
	{
		vIsFx = false;

		try
		{
			opTrack = EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks() -> Find(m_vOutputId);

			if(opTrack == NULL)
				opTrack = EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks() -> Find(m_vOutputId);
		}
		catch(...)
		{
			return false;
		}

		if(opTrack != NULL)
			SetOutput(opTrack -> GetClipDataDestination());
	}


	for(uint32 vIndex = 0; vIndex < m_vNumSlots, m_opSlots[vIndex] == NULL; vIndex++);

	if(vIndex < m_vNumSlots)
	{
		SetDestination(m_opSlots[vIndex]);
	}
	else if(m_opOutput != NULL)
	{
			SetDestination(m_opOutput);
	}

	return opFXTrack != NULL;
}

bool EMMediaEffectTrack::IsObjectDeleted()
{
	return m_vIsDeleted;
}

void EMMediaEffectTrack::SetDeleted(bool p_vState)
{
	m_vIsDeleted = p_vState;
}


