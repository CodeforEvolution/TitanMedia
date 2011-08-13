#include "EMMediaTrackRepository.h"

#include "EMRealtimeOutputDescriptor.h"
#include "EMMediaTimer.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"
#include "EMCommandRepository.h"
#include "EMMediaItemContainer.h"
#include "CommandIDs.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"
#include "EMMediaMIDITrack.h"
#include "EMMediaEffectTrackRepository.h"

list<EMMediaTrack*>* EMMediaTrackRepository::m_opTracks = NULL;

EMMediaTrackRepository::EMMediaTrackRepository()
	:	EMMediaItemContainer<EMMediaTrack>(EM_TYPE_ANY)
{
	if(m_opTracks == NULL)
	{
		m_opTracks = EM_new list<EMMediaTrack*>();
		m_opTracks -> clear();
	}
	else
		m_opTracks -> clear(); 
}

EMMediaTrackRepository::~EMMediaTrackRepository()
{
	if(m_opTracks != NULL)
	{
		m_opTracks -> clear();
		delete m_opTracks;
		m_opTracks = NULL;
	}
}

void EMMediaTrackRepository::OnItemAdd()
{
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
}

void EMMediaTrackRepository::OnItemRemove()
{
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
}

void EMMediaTrackRepository::AddTrack(EMMediaTrack* p_opTrack)
{
	bool vResult = false;

	LockContainer();
	try
	{
		vResult = Add(p_opTrack);
	}
	catch(...) 
	{
		EMDebugger("ERROR! Caught an exception in EMMediaTrackRepository::AddTrack");
	}
	UnlockContainer();

	if(vResult)
	{
//		m_opTracks -> push_back(p_opTrack);
		if(this == EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks())
			EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks() -> RemoveTrack(p_opTrack);
		else if(this == EMMediaEngine::Instance() -> GetMediaProject() -> GetUnusedTracks())
			EMMediaEngine::Instance() -> GetMediaProject() -> GetUsedTracks() -> RemoveTrack(p_opTrack);
		else 
			EMDebugger("ERROR! Failed to find own track in repository (in EMMediaTrackRepository::AddTrack)");
		if((p_opTrack -> GetType() & EM_TYPE_ANY_AUDIO) > 0)
		{
			Notify(EM_MESSAGE_AUDIO_TRACK_ACTIVATED);
		}
		else if((p_opTrack -> GetType() & EM_TYPE_ANY_VIDEO) > 0)
		{
			Notify(EM_MESSAGE_VIDEO_TRACK_ACTIVATED);
		}
	}
}

void EMMediaTrackRepository::SetAsActiveTrack(EMMediaTrack* p_opTrack)
{
	if(p_opTrack == NULL)
		return;
	if(! Find(p_opTrack))
		return;

	LockContainer();
	Rewind();
	while(Current() != NULL)
	{
		if((Current() -> GetType() & EM_TYPE_MIDI) > 0)
		{
			EMMediaMIDITrack* opMIDITrack = static_cast<EMMediaMIDITrack*>(Current());
			if(Current() -> GetID() != p_opTrack ->GetID())
				opMIDITrack -> SetActive(false);
			else
				opMIDITrack -> SetActive(true);
		}
		Next();
	}
	UnlockContainer();
}

void EMMediaTrackRepository::RemoveTrack(EMMediaTrack* p_opTrack)
{
	LockContainer();
	if(Remove(p_opTrack))
	{
//		m_opTracks -> remove(p_opTrack);
		UnlockContainer();
		if((p_opTrack -> GetType() & EM_TYPE_ANY_AUDIO) > 0)
			Notify(EM_MESSAGE_AUDIO_TRACK_DEACTIVATED);
		else if((p_opTrack -> GetType() & EM_TYPE_ANY_VIDEO) > 0)
			Notify(EM_MESSAGE_VIDEO_TRACK_DEACTIVATED);
		return;
	}
	UnlockContainer();
}

bool EMMediaTrackRepository::LoadData(EMProjectDataLoader* p_opLoader)
{
	if(string(p_opLoader -> LoadString()) != string("EMT"))
	{
		EMDebugger("EMMediaTrackRepository:ERROR! Unknown project file!");
		return false;
	}

	uint32 vNumTracks = p_opLoader -> LoadUInt32();
	for(uint32 vIndex = 0; vIndex < vNumTracks; vIndex++)
	{
		int32 vType = static_cast<int32>(p_opLoader -> LoadUInt32());
		EMMediaType eActualType = EM_TYPE_ANY;
		
		if((vType & EM_TYPE_RAW_AUDIO) == EM_TYPE_RAW_AUDIO) eActualType = EM_TYPE_RAW_AUDIO;
		else if((vType & EM_TYPE_ENCODED_AUDIO) == EM_TYPE_ENCODED_AUDIO) eActualType = EM_TYPE_ENCODED_AUDIO;
		else if((vType & EM_TYPE_ANY_AUDIO) == EM_TYPE_ANY_AUDIO) eActualType = EM_TYPE_ANY_AUDIO;
		else if((vType & EM_TYPE_UNKNOWN_AUDIO) == EM_TYPE_UNKNOWN_AUDIO) eActualType = EM_TYPE_UNKNOWN_AUDIO;
		
		else if((vType & EM_TYPE_RAW_VIDEO) == EM_TYPE_RAW_VIDEO) eActualType = EM_TYPE_RAW_VIDEO;
		else if((vType & EM_TYPE_ENCODED_VIDEO) == EM_TYPE_ENCODED_VIDEO) eActualType = EM_TYPE_ENCODED_VIDEO;
		else if((vType & EM_TYPE_ANY_VIDEO) == EM_TYPE_ANY_VIDEO) eActualType = EM_TYPE_ANY_VIDEO;
		else if((vType & EM_TYPE_UNKNOWN_VIDEO) == EM_TYPE_UNKNOWN_VIDEO) eActualType = EM_TYPE_UNKNOWN_VIDEO;

		else if((vType & EM_TYPE_MIDI) == EM_TYPE_MIDI) eActualType = EM_TYPE_MIDI;
		else if((vType & EM_TYPE_TRANSITION) == EM_TYPE_TRANSITION) eActualType = EM_TYPE_TRANSITION;
		
		else 
			EMDebugger("ERROR! Unknown type saved in project file!");
		
		string oName(p_opLoader -> LoadString());

		EMMediaTrack* opNewTrack = EMMediaTrack::CreateEMTrack(eActualType, oName);
		
		if(! opNewTrack -> InitCheckE())
			EMDebugger("ERROR! EMMediaTrackRepository could not initialize the loaded track!");
		
		if(opNewTrack -> LoadData(p_opLoader))
		{
			AddTrack(opNewTrack);
			EMMediaTrackRepository::m_opTracks -> push_back(opNewTrack);

			//This is where we should load and set the input and the output!
			if(p_opLoader -> LoadBool()) //If we have a track destination
			{
				if(p_opLoader -> LoadBool())
				{	//And the destination is an effect track...
					int32 vEffectTrackID = p_opLoader -> LoadInt32();

					EMMediaEffectTrackRepository* opFX = EMMediaEffectTrackRepository::Instance();
					opFX -> LockContainer();
					EMMediaEffectTrack* opFXTrack = opFX -> Find(vEffectTrackID);
					opFX -> UnlockContainer();

					if(opFXTrack != NULL)
					{
		//				int32 vOutputID = opFXTrack -> GetID();
		//				bool vIsEffect = true;
		//				EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(MEDIA_COMMAND_SET_TRACK_OUTPUT, &m_vID, &vOutputID, &vIsEffect);
						opNewTrack -> SetTrackDestination(opFXTrack);
					}
					else
						EMDebugger("ERROR! Couldn't find the effect track that the track should send to!");
				}
				else
				{	//And the destination is a regular physical output...
					string oRealtimeOutputName = p_opLoader -> LoadString();
				
					EMOutputRepository* opOuts = EMOutputRepository::Instance();
					opOuts -> LockContainer();
					opOuts -> Rewind();
					EMBufferDestination* opFirstDestination = NULL;
					while(opOuts -> Current() != NULL)
					{
						if(opFirstDestination == NULL && (opOuts -> Current() -> GetType() & opNewTrack -> GetType()) > 0)
							opFirstDestination = opOuts -> Current();
						if(opOuts -> Current() -> GetName() == oRealtimeOutputName)
						{
							opNewTrack -> SetTrackDestination(opOuts -> Current());
							int32 vOutputID = opOuts -> Current() -> GetID();
							opOuts -> UnlockContainer();
							int32 vTrackID = opNewTrack -> GetID();
							EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(MEDIA_COMMAND_SET_TRACK_OUTPUT, &vTrackID, &vOutputID, NULL);
							opOuts -> LockContainer();
							break;
						}
						opOuts -> Next();
					}
					opOuts -> UnlockContainer();

					//If the output could not be found, connect the track to the first existing output regardless of its name
					if(opNewTrack -> GetTrackDestination() == NULL && opFirstDestination != NULL)
					{
						int32 vOutputID = opFirstDestination -> GetID();
						int32 vTrackID = opNewTrack -> GetID();
						EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(MEDIA_COMMAND_SET_TRACK_OUTPUT, &vTrackID, &vOutputID, NULL);
		//				SetTrackDestination(opFirstDestination);
					}
				}

				if((opNewTrack -> GetType() & EM_TYPE_MIDI) == 0)
				{
					EMMediaFormat* opFormat = new EMMediaFormat(opNewTrack -> GetType());
					opNewTrack -> InitCheckBufferE(opFormat);
					delete opFormat;
				}
			}

			//If we have an input...
			if(p_opLoader -> LoadBool())
			{
				string oInputName(p_opLoader -> LoadString());
				EMInputRepository* opIns = EMInputRepository::Instance();
				opIns -> LockContainer();
				opIns -> Rewind();
				bool vHaveInput = false;
				while(opIns -> Current() != NULL)
				{
					if((opIns -> Current() -> GetType() & opNewTrack -> GetType()) > 0 && opIns -> Current() -> GetName() == oInputName)
					{
						int32 vInputID = opIns -> Current() -> GetID();
						opIns -> UnlockContainer();
						int32 vTrackID = opNewTrack -> GetID();
						EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(MEDIA_COMMAND_SET_TRACK_INPUT, &vTrackID, &vInputID, NULL);
						opIns -> LockContainer();
						vHaveInput = true;
						break;
					}
					opIns -> Next();
				}

				if(! vHaveInput)
				{
					eo << "WARNING! Track had an input that wasn't found on this system (\"" << oInputName.c_str() << "\")" << ef;
					EMRealtimeInputDescriptor* opInput = NULL;

					if(opNewTrack -> GetType() == EM_TYPE_RAW_AUDIO ||
						opNewTrack -> GetType() == EM_TYPE_ANY_AUDIO)
					{
						opInput = opIns -> First(EM_TYPE_ANY_AUDIO);
					}
					else
						opInput = opIns -> First(opNewTrack -> GetType());

					if(opInput != NULL)
					{
						int32 vInputID = opInput -> GetID();
						opIns -> UnlockContainer();
						int32 vTrackID = opNewTrack -> GetID();
						EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(MEDIA_COMMAND_SET_TRACK_INPUT, &vTrackID, &vInputID, NULL);
						opIns -> LockContainer();
					}
					else
						eo << "WARNING! Titan didn't detect any inputs of this type!" << ef;
				}
				opIns -> UnlockContainer();
			} 

//			m_opTracks -> push_back(opNewTrack);
		}
		else
		{
			delete opNewTrack;
			return false;
		}
	}
 
	if(string(p_opLoader -> LoadString()) != string("EMT"))
	{
		EMDebugger("EMMediaTrackRepository:ERROR(2)! Unknown project file!");
		return false;
	}

	EMRealtimeOutputDescriptor* opOut = EMOutputRepository::Instance() -> FindClockMaster();
	if(opOut != NULL)
	{
		opOut -> RegisterAsClockMaster();
		EMMediaTimer::Instance() -> RegisterClockMaster(opOut);
	}
	
	return true;
}

bool EMMediaTrackRepository::SaveData(EMProjectDataSaver* p_opSaver)
{
	p_opSaver -> SaveString("EMT");
	LockContainer();
	
	uint32 vSize = 0;
	Rewind();
	while(Current() != NULL)
	{
		if(! Current() -> IsObjectDeleted())
			vSize++;
		Next();
	}
	p_opSaver -> SaveUInt32(static_cast<uint32>(vSize));
	
	Rewind();
	while(Current() != NULL)
	{
		if(! Current() -> IsObjectDeleted())
		{
			p_opSaver -> SaveUInt32(static_cast<uint32>(Current() -> GetType()));
			p_opSaver -> SaveString((Current() -> GetName()).c_str());
			Current() -> SaveData(p_opSaver);

			//This is where we should save the input and the output!

			if(Current() -> GetTrackDestination() != NULL && Current() -> GetType() != EM_TYPE_TRANSITION)
			{
				p_opSaver -> SaveBool(true); //Have a track destination!
				if(Current() -> GetTrackDestination() -> GetPosition() != EM_OUTPUT)  //if((m_opTrackDataReceiver -> GetType() & EM_OUTPUT_TYPE_EFFECTSLOT) > 0)
				{
					p_opSaver -> SaveBool(true); //Flag indiacating that the track's destination is an effect track and not a physical output.
					p_opSaver -> SaveInt32(Current() -> GetTrackDestination() -> GetID());
				}
				else
				{
					p_opSaver -> SaveBool(false); //The track's destination is a regular physical output.
					//p_opSaver -> SaveInt32(m_opTrackDataReceiver -> GetID()); //Save the ID of the output, for forward/backward compability
					p_opSaver -> SaveString((static_cast<EMRealtimeOutputDescriptor*>(Current() -> GetTrackDestination()) -> GetName()).c_str()); //Save the name to search for during loading.
				}
			}
			else
				p_opSaver -> SaveBool(false);

			if(Current() -> GetInput() != NULL && Current() -> GetType() != EM_TYPE_TRANSITION)
			{
				p_opSaver -> SaveBool(true);
				p_opSaver -> SaveString(Current() -> GetInput() -> GetName().c_str());
			}
			else
				p_opSaver -> SaveBool(false); 
		}
		Next();
	}
	UnlockContainer();
	p_opSaver -> SaveString("EMT");
	return true;
}

bool EMMediaTrackRepository::ClearData()
{
	LockContainer();
	while(Size() > 0)
	{
		Rewind();
		EMMediaTrack* opTrack = Remove(Current() -> GetID());
//		m_opTracks -> remove(opTrack);
		delete opTrack;
	}
//	m_opTracks -> clear();
	UnlockContainer();
	return true;
}

bool EMMediaTrackRepository::LockContainer()
{
	return EMMediaItemContainer<EMMediaTrack>::LockContainer();
}

void EMMediaTrackRepository::UnlockContainer()
{
	EMMediaItemContainer<EMMediaTrack>::UnlockContainer();
}
