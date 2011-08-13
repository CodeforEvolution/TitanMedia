#ifndef __EM_MEDIA_POOL_CPP
#define __EM_MEDIA_POOL_CPP

#include "EMMediaPool.h"

#include "EMMediaIDManager.h"
#include "EMFileInputDescriptor.h"

#include "EMAudioFileInputDescriptor.h"
#include "EMImageFileInputDescriptor.h"

#ifdef PLATFORM_BEOS
#include "EMBeVideoFileInputDescriptor.h"
#endif
#ifdef PLATFORM_WINDOWS
#include "EMVideoFileInputDescriptor.h"
#endif

#include "EMMediaUtility.h"
#include "EMMediaPoolEntry.h"
#include "EMMediaPoolAudioEntry.h"
#include "EMMediaPoolVideoEntry.h"
#include "EMMediaPoolImageEntry.h"
#include "EMWaveFileReader.h"
#include "EMWaveFileWriter.h"
#include "EMMediaFileManager.h"
#include "EMWinMediaFileManager.h"
#include "EMMediaFileRepository.h"
#include "EMMediaEngine.h"
#include "EMMediaFormat.h"
#include "EMProjectDataSaver.h"
#include "EMProjectDataLoader.h"
#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "DirectoryIDs.h"
#include "EMException.h"
#include "EMExceptionHandler.h"
#include "EMMediaEngine.h"
#include "EMMediaProject.h"

#include <windows.h>

EMMediaPool::EMMediaPool() 
	:	EMMediaItemContainer<EMMediaPoolEntry>(EM_TYPE_ANY),
		m_opFileManager(NULL)
{
	m_vID = EMMediaIDManager::MakeID();
	m_opFileManager = EMMediaFileManager::Instance();
}

EMMediaPool::~EMMediaPool()
{
	EMMediaFileManager::Delete();
	EMMediaFileRepository::Delete();
}

void EMMediaPool::OnItemAdd()
{
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
}

void EMMediaPool::OnItemRemove()
{
	EMMediaEngine::Instance() -> GetMediaProject() -> SetDirty(true);
}

void EMMediaPool::AddMediaFileE(string p_oFileName, int32* p_vIDs, bool p_vExtract)
{
	string oFileName = p_oFileName;
	EMMediaPoolEntry* opEntry = NULL; 
	p_vIDs[0] = p_vIDs[1] = -1;
	
	EMMediaFormat oOriginalVideoFormat(EM_TYPE_ANY_VIDEO);
	EMMediaFormat oOriginalAudioFormat(EM_TYPE_ANY_AUDIO);
	EMMediaFormat oOriginalFormat(EM_TYPE_ANY_VIDEO);

//	((EMWinMediaFileManager*) EMMediaFileManager::Instance()) ->FindOriginalFormat(p_oFileName, &oOriginalFormat);

	//Find the file type
	int32 vActualType = 0;
	try 
	{
		vActualType = EMMediaFileManager::Instance() -> GetFileTypeForE(&oFileName);
	}
	catch(...)
	{
		string oException("There were problems checking the file type!");
		throw EM_new EMException(EM_DIALOG_ALWAYS, &oException);
	}

	//If it's an unknown file type (other than media file)	
	if(vActualType > 0)
	{
		EMMediaFileRepository::Instance() -> AddFile(&oFileName);
	}
	else
	{
		string oException("Unsupported file type! Cannot do anything with the file");
		throw EM_new EMException(EM_DIALOG_ALWAYS, &oException);
	}

	if((vActualType & EM_TYPE_ANY_IMAGE) > 0)
	{
		opEntry = EM_new EMMediaPoolImageEntry(oFileName, (EMMediaType)vActualType);

		if(!opEntry -> InitCheckE())
			EMDebugger("ERROR! Could not initiate image entry!");

		oOriginalFormat.Copy(opEntry -> GetFormat());
		p_vIDs[1] = opEntry -> GetID();

		LockContainer();
		try
		{
			Add(opEntry);
		}
		catch(...)
		{
			EMDebugger("ERROR! Caught an exception in EMMediaPool::AddMediaFileE (1)");
		}
		UnlockContainer();
	}

	//If it's a video file, we can add it directly
	if((vActualType & EM_TYPE_ANY_VIDEO) > 0)
	{
		//Add video entry for the file name
		if(EMMediaFileManager::Instance() -> CheckFileForVideoFormat(&oFileName))
		{
			opEntry = EM_new EMMediaPoolVideoEntry(oFileName);

			if(! opEntry -> InitCheckE())
				EMDebugger("ERROR! Could not initiate video entry!");

//			MessageBox(NULL, "DEBUG OUTPUT: Import step 2 completed", "Message", MB_OK);
			oOriginalFormat.Copy(opEntry -> GetFormat());

			p_vIDs[1] = opEntry -> GetID();
//			EMDebugger(string(string("MediaPool adding entry for file \"") + oFileName + string("\"")).c_str());
			LockContainer();
			try
			{
				Add(opEntry);
			}
			catch(...)
			{
				EMDebugger("ERROR! Caught an exception in EMMediaPool::AddMediaFileE (1)");
			}
			UnlockContainer();
		} 
		else if((vActualType & EM_TYPE_ANY_AUDIO) ==0)
		{
			string oException("Unknown video format! Cannot load video");
			EMExceptionHandler::Instance() -> HandleException(*(EM_new EMException(EM_DIALOG_ALWAYS, &oException)));
		}
		else
		{
			vActualType ^= (vActualType & EM_TYPE_ANY_VIDEO);
		}
	}
	
	 //Further, if it's encoded audio possibly inside a video-file, we need to extract it into a new file!
	if((vActualType & EM_TYPE_ANY_AUDIO) > 0)
	{
		string oExtractedAudioFileName = EMMediaFileManager::Instance() -> ExtractAudioE(&oFileName);
		
		if(oExtractedAudioFileName != "Noname")
		{
			EMMediaFileRepository::Instance() -> AddFile(&oExtractedAudioFileName);
			oFileName = oExtractedAudioFileName;
		} 
		else
		{
			if((vActualType & EM_TYPE_ANY_VIDEO) == 0)
				MessageBox(NULL, "File contained an unsupported or invalid audio format!", "Unsupported audio format!", MB_OK);

			oFileName = "Noname";
		}
	} 
	else
	{
		oFileName = "Noname";
	}

	//If it was audio in some form...
	if((vActualType & EM_TYPE_ANY_AUDIO) > 0 && oFileName != "Noname")
	{
		//In any case, as long as it's an audio file we need to create a peaks file for it
		string oPeaksFile(EMMediaFileManager::Instance() -> CreatePeaksFileE(&oFileName));

		EMMediaFileRepository::Instance() -> AddFile(&oPeaksFile);

		//Finally, add the file to the media pool as an audio entry
		opEntry = EM_new EMMediaPoolAudioEntry(oFileName);
		if(! opEntry -> InitCheckE())
			EMDebugger("ERROR! Could not initialize audio entry!");
		p_vIDs[0] = opEntry -> GetID();
		
		LockContainer();
		try
		{
			Add(opEntry);
		} 
		catch(...)
		{
			EMDebugger("ERROR! Caught an exception in EMMediaPool::AddMediaFileE (2)");
		}
		UnlockContainer();
	} 
}

EMFileInputDescriptor* EMMediaPool::InstantiateEntryE(int32 p_vMediaPoolEntryID)
{
	EMMediaPoolEntry* opEntry = NULL;

	LockContainer();
	try
	{
		opEntry = Find(p_vMediaPoolEntryID);
	}
	catch(...)
	{
		EMDebugger("ERROR! Caught an exception in EMMediaPool::InstantiateEntryE");
	}
	UnlockContainer();
	if(opEntry == NULL)
	{
		return NULL; //TODO: Throw exception?
	}

	EMFileInputDescriptor* opDescriptor = NULL;
	
	if((opEntry -> GetType() & EM_TYPE_ANY_AUDIO) > 0)
	{
		opDescriptor = EM_new EMAudioFileInputDescriptor(opEntry -> GetFileName());
	}
	else if((opEntry -> GetType() & EM_TYPE_ANY_VIDEO) > 0)
	{
#ifdef PLATFORM_BEOS
		opDescriptor = EMBeVideoFileInputDescriptor(opEntry -> GetFileName());
#endif
#ifdef PLATFORM_WINDOWS
		opDescriptor = EM_new EMVideoFileInputDescriptor(opEntry -> GetFileName());
#endif
	}
	else if((opEntry -> GetType() & EM_TYPE_ANY_IMAGE) > 0)
	{
		opDescriptor = static_cast<EMMediaPoolImageEntry*>(opEntry) -> CloneDescriptor();
	}
	else
	{
		EMDebugger("Only raw audio and raw video is implemented so far!");
	}

	if(! opDescriptor -> Open())
	{
		delete opDescriptor;
		return NULL; //TODO: Throw exception?
	}
	
	opDescriptor -> SetName(opEntry -> GetFileName());
	opDescriptor -> SetMediaPoolEntryID(p_vMediaPoolEntryID);
	opEntry -> IncreaseUsed();
	
	return opDescriptor;
}

int32 EMMediaPool::GetID() const
{
	return m_vID;
}

int32 EMMediaPool::SearchFor(string p_oFileName, EMMediaType p_eType)
{
	LockContainer();
	try
	{
		Rewind();
		while(Current() != NULL)
		{
			if(Current() -> GetFileName() == p_oFileName && ((Current() -> GetType() & p_eType) > 0))
			{
				int32 vID = Current() -> GetID();
				UnlockContainer();
				return vID;
			}
			Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Caught an exception in EMMediaPool::SearchFor");
	}
	UnlockContainer();
	return -1;
}

int32 EMMediaPool::GetFileTypeForE(const string* p_opFileName)
{
	return (int32) EM_TYPE_RAW_AUDIO; //m_opFileManager -> GetFileTypeForE(p_opFileName); 
}

bool EMMediaPool::SaveData(EMProjectDataSaver* p_opProjectDataSaver)
{
	/*
		What's important to save?
			-	The number of files in the pool?
			-	The relative file name of each? (relative to the project folder)
			-	The type of each?
			-	The byte size of each?
			-	The number of times each file is used in the project?
			-	The identification number of each entry?et
			-	...?
	*/
	
	p_opProjectDataSaver -> SaveString("MEDIAPOOL - Begin");

	uint32 vNumFiles = 0;

	LockContainer();
	try
	{
		vNumFiles = static_cast<uint32>(Size());
	}
	catch(...)
	{
		EMDebugger("ERROR! Caught an exception in EMMediaPool::SaveData (1)");
	}
	UnlockContainer();
	
	p_opProjectDataSaver -> SaveUInt32(vNumFiles);
	
	LockContainer();
	try
	{
		Rewind();
		while(Current() != NULL)
		{
			uint32 vType = static_cast<uint32>(Current() -> GetType());
			p_opProjectDataSaver -> SaveUInt32(vType);

	//		EMDebugger(string(string("MediaPool about to save data for entry \"") + Current() -> GetFileName() + string("\"")).c_str());
			Current() -> SaveData(p_opProjectDataSaver);
			
	//		p_opProjectDataSaver -> SaveString(Current() -> GetFileName());
	//		p_opProjectDataSaver -> SaveUInt32(static_cast<uint32>(Current() -> GetID()));
			
			Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Caught an exception in EMMediaPool::SaveData (2)");
	}
	UnlockContainer();

	p_opProjectDataSaver -> SaveString("MEDIAPOOL - End");

	return true;
}

bool EMMediaPool::LoadData(EMProjectDataLoader* p_opProjectDataLoader)
{
//	if(string(p_opProjectDataLoader -> LoadString()) != string("MEDIAPOOL"))
//		EMDebugger("ERROR! Couldn't find sync in project file! Error when starting to load media pool stuff!");
	
	string Test1 = p_opProjectDataLoader -> LoadString();

	uint32 vNumFiles = p_opProjectDataLoader -> LoadUInt32();
//	EMDebugger("About to start adding files to pool!");
	
	for(uint32 vIndex = 0; vIndex < vNumFiles; vIndex++)
	{
//		string oFileName(p_opProjectDataLoader -> LoadString());
//		int32 vCorrectID = static_cast<int32>(p_opProjectDataLoader -> LoadUInt32());
		
//		int32 vpIDs[2];
//		vpIDs[0] = vpIDs[1] = -1;
//		AddMediaFileE(oFileName, vpIDs, false);
//		;//cout_commented_out_4_release << "Ok! File added to media pool. Got back id " << vpIDs[0] << " and id " << vpIDs[1] << " when it should actually be " << vCorrectID << endl;
		
		EMMediaPoolEntry* opEntry = NULL;
		uint32 vType = p_opProjectDataLoader -> LoadUInt32();
	
		if((vType & EM_TYPE_ANY_AUDIO) > 0)
		{
			opEntry = EM_new EMMediaPoolAudioEntry(p_opProjectDataLoader, vType);
		}
		else if((vType & EM_TYPE_ANY_VIDEO) > 0)
		{
			opEntry = EM_new EMMediaPoolVideoEntry(p_opProjectDataLoader, vType);
		}
		else if((vType & EM_TYPE_ANY_IMAGE) > 0)
		{
			opEntry = EM_new EMMediaPoolImageEntry(p_opProjectDataLoader, vType);
		}
		else
			EMDebugger("ERROR! Unknown type in project file!");
		LockContainer();
		try
		{
			if(! opEntry -> InitCheckE())
				EMDebugger("ERROR! Could not initialize audio entry!");
			Add(opEntry);
		}
		catch(...)
		{
			EMDebugger("ERROR! Caught an exception in EMMediaPool::LoadData");
		}
		UnlockContainer();
	}

	string Test2 = p_opProjectDataLoader -> LoadString();

	return true;
}

bool EMMediaPool::ClearData()
{
	return true;
}

#endif