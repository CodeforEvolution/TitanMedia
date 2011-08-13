#include "EMMediaProject.h"

#include "CommandIDs.h"
#include "EMApplication.h" //For being able to listen for quit-messages!
#include "EMBeBufferRepository.h"
#include "EMBeMediaTimer.h"
#include "EMBufferDestination.h"
#include "EMCommandLineIDs.h"
#include "EMCommandRepository.h"
#include "EMFileInputDescriptor.h"
#include "EMGlobals.h"
#include "EMMediaClipRepository.h"
#include "EMMediaDebugLog.h"
#include "EMMediaEffectTrackRepository.h"
#include "EMMediaEngine.h"
#include "EMMediaEngineBuild.h"
#include "EMMediaIDManager.h"
#include "EMMediaMatrix.h"
#include "EMMediaMIDIEvent.h"
#include "EMMediaMIDIEventRepository.h"
#include "EMMediaPool.h"
#include "EMMediaSignalMeterRepository.h"
#include "EMMediaTimer.h"
#include "EMMediaTrackRepository.h"
#include "EMMediaTransitionTrackRepository.h"
#include "EMMediaUtility.h"
#include "EMMessages.h"
#include "EMOutputRepository.h"
#include "EMRecording.h"
#include "EMRecordingMIDITrackRepository.h"
#include "EMSemaphore.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMSilentAudioSource.h"
#include "EMSilentVideoSource.h"
#include "EMThread.h"
#include "EMThreadListener.h"
#include "EMVideoSynchronizer.h"

#ifdef PLATFORM_WINDOWS
#include "EMWinDXMIDIConsumer.h"
#include "EMWinDXMIDIProducer.h"
#include "EMWinMediaSingletonMuxer.h"
#include "EMWinMediaSystemInspector.h"
#include "EMWinAudioClipRepository.h"
#endif

#include <algorithm> 
#include <iostream>
#include <math.h>

//Defines have been moved to Framework/Globals/EMMediaEngineBuild.h

EMSemaphore* EMMediaProject::m_opAudioProcessingSemaphore = NULL;
EMSemaphore* EMMediaProject::m_opVideoProcessingSemaphore = NULL;

EMMediaProject::EMMediaProject() 
	:	m_vShouldProduceAudio(false),
		m_vShouldProduceVideo(false),
		m_opAudioClipRepository(NULL),
		m_opVideoClipRepository(NULL),
		m_opVideoTransitionClipRepository(NULL),
		m_opVideoTransitionTrackRepository(NULL),
		m_opMIDIClipRepository(NULL),
		m_opEffectTrackRepository(NULL),
		m_opMediaPool(NULL),
		m_vProcessSingleAudioBuffer(0),
		m_vProcessSingleVideoBuffer(0),
		m_opUsedTracks(NULL),
		m_opUnusedTracks(NULL),
		m_vNumAudioSlots(0),
		m_vNumVideoSlots(0),
		m_opPreAllocatedDestinationLists(NULL),
		m_vIsArmed(false),
		m_vIsRenderingAudio(false),
		m_vIsRenderingVideo(false),
		m_vVideoWorkingFrameRate(0),
		vFrameRateChangedSwitch(false),
		m_opMIDIEventRepository(NULL),
		m_opRecordingMIDITrackRepository(NULL),
		m_opAudioThread(NULL),
		m_opVideoThread(NULL),
		m_vAudioIsActive(false),
		m_vVideoIsActive(false),
		m_vIsDirty(false),
		m_vProjectBPM(120),
		m_oProjectSignature("4/4"),
		m_vCurrentLatestEnding(0)
{
	m_vID = EMMediaIDManager::MakeID();
	
	EMMediaTimer::Instance() -> AddListener(this); 
	EMApplication::Instance() -> AddListener(this); //Listen for quit-messages!

	//These two semaphores are used for being sure that the processing threads have died (when deconstructing EMMediaProject).
	m_opSemaphore = EMSemaphore::CreateEMSemaphore("List protection sem", 1, 1);
	m_opAudioThread = EMThread::CreateEMThread("Audio processing thread", EM_THREAD_HIGH_REALTIME_PRIORITY, 0);
	m_opAudioThread -> AddListener(this);
	m_opVideoThread = EMThread::CreateEMThread("Video processing thread", EM_THREAD_MEDIUM_PRIORITY, 0);
	m_opVideoThread -> AddListener(this);


	m_vVideoOn = ((EMWinMediaSystemInspector::m_vDisableFeaturesFlags) & EM_CL_START_VIDEO_PROCESSING) == 0;
	m_vAudioOn = ((EMWinMediaSystemInspector::m_vDisableFeaturesFlags) & EM_CL_START_AUDIO_PROCESSING) == 0;
	m_vMidiOn = ((EMWinMediaSystemInspector::m_vDisableFeaturesFlags) & EM_CL_START_MIDI_PROCESSING) == 0;
}

EMMediaProject::~EMMediaProject()
{
	try
	{
		EMApplication::Instance() -> RemoveListener(this); //Stop listening for quit-messages!
		if(! ClearThreads())
			EMDebugger("ERROR! Failed to clear threads in EMMediaProject!");

		m_opUsedTracks -> RemoveListener(EMOutputRepository::Instance());
		EMMediaTimer::Instance() -> RemoveListener(this);
		delete m_opAudioClipRepository;
		delete m_opVideoClipRepository;
		delete m_opVideoTransitionClipRepository;
		delete m_opVideoTransitionTrackRepository;
		delete m_opMIDIClipRepository;
		delete m_opMIDIEventRepository;
		delete m_opRecordingMIDITrackRepository; 
		delete m_opUsedTracks;
		delete m_opUnusedTracks;
		delete m_opMediaPool;
		EMMediaSignalMeterRepository::Delete();
		EMMediaEffectTrackRepository::Delete();
		if(m_opPreAllocatedDestinationLists != NULL)			//Kaneda - avlusning
		{
			for(int32 vIndex = 0; vIndex < 100; vIndex++)
				delete m_opPreAllocatedDestinationLists[vIndex];
			delete m_opPreAllocatedDestinationLists;
		}
		EMWinDXMIDIProducer::Instance() -> ClearData();
		EMWinDXMIDIConsumer::Instance() -> ClearData();
		
	}
	catch(...)
	{
		EMDebugger("ERROR! Caught an exception while closing EMMediaProject");
	}
}

bool EMMediaProject::ClearThreads()
{
	try
	{
		if(m_opAudioThread != NULL || m_opVideoThread != NULL)
		{
			if(IsPlaying())
			{
				if(IsArmed())
					StopRecordingE();
				else
					StopPlaybackE();
			}
			m_opAudioThread -> RemoveListener(this);
			m_opVideoThread -> RemoveListener(this);
			m_opAudioThread -> Kill();
			m_opVideoThread -> Kill();
			delete m_opAudioThread;
			m_opAudioThread = NULL;
			delete m_opVideoThread;
			m_opVideoThread = NULL;
		 }
	}
	catch(...)
	{
		EMDebugger("ERROR! Caught an exception while clearing threads in EMMediaProject");
		return false;
	}
	return true;
}

void EMMediaProject::OnThreadCreated(EMThread* p_opThread)
{
	if(p_opThread == m_opAudioThread)
	{
		HRESULT vResult = CoInitializeEx(NULL, COINIT_MULTITHREADED);
			if(FAILED(vResult))
				EMDebugger("ERROR! Failed to initialize COM system for audio processing!");

		for(int y = 0; y < 64; y++)
			for(int x = 0; x < 64; x++)
				m_opAudioDestinations[y][x] = NULL;
		for(int i = 0; i < 64; i++)
			m_opAudioBufferList[i] = NULL;
	}
	else if(p_opThread == m_opVideoThread)
	{
		HRESULT vResult = CoInitializeEx(NULL, COINIT_MULTITHREADED);
			if(FAILED(vResult))
				EMDebugger("ERROR! Failed to initialize COM system for video processing!");
	}
}

void EMMediaProject::OnThreadKilled(EMThread* p_opThread)
{
	if(p_opThread == m_opAudioThread)
	{
		CoUninitialize();
	}
	else if(p_opThread == m_opVideoThread)	
	{
		CoUninitialize();
	}
}

void EMMediaProject::ThreadRun(EMThread* p_opThread)
{
	if(p_opThread == m_opAudioThread)
		RunAudio();
	else if(p_opThread == m_opVideoThread)
		RunVideo();
}

int32 EMMediaProject::GetID() const
{
	return m_vID;
}

//Should be called sometime shortly after this class has
//been instantiated, BEFORE it is actually used! Otherwise
//things will get messed up (you'll probably crash and burn).
//Note that this method should NOT be called once before
//each Playback, it's ONCE per LIFETIME of an instance!
bool EMMediaProject::InitCheckE()
{
	try
	{
		//TODO: By the way, note that the silence-generator currently only
		//can produce AUDIO silence. We need to split it up into a base-class
		//with at least two sub-classes, one for audio and one for video buffers 
		//of silence.

		m_vVideoWorkingFrameRate = *(static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FRAMERATE)));
		m_opMediaPool = EM_new EMMediaPool();  //7 threads
		
		m_opAudioClipRepository = EMMediaClipRepository::New(EM_TYPE_ANY_AUDIO);
		m_opVideoClipRepository = EMMediaClipRepository::New(EM_TYPE_ANY_GFX);
		m_opVideoTransitionClipRepository = EMMediaClipRepository::New(EM_TYPE_TRANSITION);
		m_opMIDIClipRepository = EMMediaClipRepository::New(EM_TYPE_MIDI);

//		EMWinDXMIDIConsumer::Instance();
//		EMWinDXMIDIProducer::Instance();

		m_opRecordingMIDITrackRepository = EM_new EMRecordingMIDITrackRepository();
		m_opMIDIEventRepository = EM_new EMMediaMIDIEventRepository();
		m_opMIDIEventRepository -> PreAllocEvents();
	
		m_opUnusedTracks = EM_new EMMediaTrackRepository();
		m_opUsedTracks = EM_new EMMediaTrackRepository();

		m_opEffectTrackRepository = EMMediaEffectTrackRepository::Instance();

		m_opVideoTransitionTrackRepository = EM_new EMMediaTransitionTrackRepository();

		m_opUsedTracks -> AddListener(EMOutputRepository::Instance());
		
		EMMediaSignalMeterRepository::Instance();
		
		PrepareLists();
		
	}
	catch(...)
	{
		delete m_opMediaPool;
		m_opMediaPool = NULL;
		delete m_opAudioClipRepository;
		delete m_opVideoClipRepository;
		delete m_opMIDIClipRepository;
		EMOutputRepository::Delete();
		EMMediaSignalMeterRepository::Delete();
		throw;
	}

	if(m_vAudioOn)
		m_opAudioThread -> StartSuspended();

	if(m_vVideoOn)
		m_opVideoThread -> StartSuspended();

	return true;
}

//This should be called once before each playback. Make sure
//you've called InitCheckE prior to this!
bool EMMediaProject::PrepareToPlayE()
{
	return true;	
}

void EMMediaProject::PrepareLists()
{
	m_opPreAllocatedDestinationLists = EM_new list<EMMediaDataBuffer*>*[100];
	for(int32 vIndex = 0; vIndex < 100; vIndex++)
		m_opPreAllocatedDestinationLists[vIndex] = EM_new list<EMMediaDataBuffer*>();
}

list<EMMediaDataBuffer*>* EMMediaProject::GetList()
{
	m_opSemaphore -> Acquire();
	for(int32 vIndex = 0; vIndex < 100; vIndex++)
		if(m_opPreAllocatedDestinationLists[vIndex] != NULL)
		{
			list<EMMediaDataBuffer*>* opList = m_opPreAllocatedDestinationLists[vIndex];
			m_opPreAllocatedDestinationLists[vIndex] = NULL;
			m_opSemaphore -> Release();
			return opList;
		}
	
	m_opSemaphore -> Release();
	EMDebugger("ERROR! Could not find an available destination list from the preallocated repository of lists!");
	return NULL;
}

void EMMediaProject::RecycleList(list<EMMediaDataBuffer*>* p_opList)
{
	m_opSemaphore -> Acquire();
	for(int32 vIndex = 0; vIndex < 100; vIndex++)
		if(m_opPreAllocatedDestinationLists[vIndex] == NULL)
		{
			p_opList -> clear();
			m_opPreAllocatedDestinationLists[vIndex] = p_opList;
			m_opSemaphore -> Release();
			return;
		}

	m_opSemaphore -> Release();
	EMDebugger("m_vSemaphore! No room to store the list, in the preallocated repository of lists!");
}

uint32 NumBuffers(EMMediaDataBuffer** p_opArray)
{
	uint32 vCount = 0;
	for(uint8 vIndex = 0; vIndex < 64; vIndex++)
		vCount += (p_opArray[vIndex] != NULL ? 1 : 0);
	return vCount;
}
uint32 NumDestinations(EMMediaDataBuffer* (*p_opArray)[64])
{
	uint32 vCount = 0;
	for(uint8 vIndex = 0; vIndex < 64; vIndex++)
		if(p_opArray[vIndex][0] != NULL)
			vCount++;
	return vCount;
}

list<EMMediaDataBuffer* >* EMMediaProject::GetLeastReferencedDestination(list<list<EMMediaDataBuffer* >* >* p_opSets)
{
	if(p_opSets -> size() < 1)
		return NULL;

	list<EMMediaDataBuffer* >* opCurrentlyLeastReferenced = (*p_opSets -> begin());
	int32 vMinimumRefCount = (*p_opSets -> begin()) -> front() -> m_opDestination -> GetProcessingReferenceCount();

	list<list<EMMediaDataBuffer* >* >::const_iterator oListListIterator;
	for(oListListIterator = (p_opSets -> begin())++; oListListIterator != p_opSets -> end(); oListListIterator++)
	{
		if((*oListListIterator) -> front() -> m_opDestination -> GetProcessingReferenceCount() < vMinimumRefCount)
		{
			opCurrentlyLeastReferenced = (*oListListIterator);
			vMinimumRefCount = opCurrentlyLeastReferenced -> front() -> m_opDestination -> GetProcessingReferenceCount();
		}
	}
	return opCurrentlyLeastReferenced;
}

void SplitArrays(EMMediaDataBuffer** p_opBufferArray, EMMediaDataBuffer* (*p_opDestinations)[64])
{
	int32 vIDArray[64];
	for(uint8 vIndex = 0; vIndex < 64; vIndex++)
		vIDArray[vIndex] = -1;

	uint32 vNumElementsArray[64];
	for(vIndex = 0; vIndex < 64; vIndex++)
		vNumElementsArray[vIndex] = 0;

	uint32 vNumDestinations = 0;
	for(vIndex = 0; vIndex < 64 && p_opBufferArray[vIndex] != NULL; vIndex++)
	{
		if(p_opBufferArray[vIndex] -> m_opDestination != NULL)
		{
			int32 vCurrentID = p_opBufferArray[vIndex] -> m_opDestination -> GetID();
			
			bool flag = false;
			for(uint8 vIDIndex = 0; vIDIndex < 64; vIDIndex++)
			{
				if(vIDArray[vIDIndex] == vCurrentID)
				{
					p_opDestinations[vIDIndex][vNumElementsArray[vIDIndex]] = p_opBufferArray[vIndex];
					p_opBufferArray[vIndex] = NULL;
					vNumElementsArray[vIDIndex]++;
					flag = true;
					break;
				}
			}

			if(! flag)
			{
				vIDArray[vNumDestinations] = vCurrentID;
				p_opDestinations[vNumDestinations][vNumElementsArray[vNumDestinations]] = p_opBufferArray[vIndex];
				p_opBufferArray[vIndex] = NULL;
				vNumElementsArray[vNumDestinations]++;
				vNumDestinations++;
			}
		}
	}
}

EMMediaDataBuffer** EMMediaProject::GetLeastReferencedDestination(EMMediaDataBuffer* (*p_opDestinations)[64])
{
	EMMediaDataBuffer** opLeastReferenced = NULL;
	uint32 vLeastRefCount = -1;
	uint32 vPosition = 0;

	for(uint32 vIndex = 0; vIndex < 64; vIndex++)
	{
		if(p_opDestinations[vIndex][0] !=  NULL)
		{
			if(vLeastRefCount == -1 || 
				(p_opDestinations[vIndex][0] -> m_opDestination -> GetProcessingReferenceCount() < vLeastRefCount))
			{
				vLeastRefCount = p_opDestinations[vIndex][0] -> m_opDestination -> GetProcessingReferenceCount();
				opLeastReferenced = p_opDestinations[vIndex];
				vPosition = vIndex;
			}
		}
	}
	return opLeastReferenced;
}
//This ugly little chunk of code splits one unordered list of EMMediaDataBuffers into a list of
//lists! Each resulting list containing only buffers bound for a specific destination.
//It empties the p_opBufferList during this process!
void EMMediaProject::SplitLists(list<EMMediaDataBuffer*>* p_opBufferList, list<list<EMMediaDataBuffer*>*>* p_opSets)
{
	while(p_opBufferList -> size() > 0)
	{
		if(p_opBufferList -> front() -> m_opDestination == NULL)
			EMDebugger("ERROR! Project told to handle a buffer with no destination!");
			
		int32 vDestinationID = p_opBufferList -> front() -> m_opDestination -> GetID();

		bool vFound = false;		
		list<list<EMMediaDataBuffer*>*>::const_iterator opIter;
		for(opIter = p_opSets -> begin(); opIter != p_opSets -> end(); opIter++)
			if((*opIter) -> size() > 0)
				if((*opIter) -> front() -> m_opDestination -> GetID() == vDestinationID)
				{
					(*opIter) -> push_back(p_opBufferList -> front());
					p_opBufferList -> pop_front();
					vFound = true;
					break;
				}

		if(! vFound)
		{
			//create new destination set and add to p_opSets.
			//TODO: Don't do "new" here! Find some better solution! (...Performance, performance, performance!)
			list<EMMediaDataBuffer*>* opNewList = GetList(); //new list<EMMediaDataBuffer*>();
			p_opSets -> push_back(opNewList);
			opNewList -> push_back(p_opBufferList -> front());
			p_opBufferList -> pop_front();
		}
	}
}

//Here's were the action is. See header-documentation for a more general overview of what
//happens here.
void EMMediaProject::RunAudio()
{
	EMMediaFormat* opFormat = EMMediaUtility::Instance() -> GetSystemAudioFormat();
	int64 vAudioBufferSize = opFormat -> m_vBufferSizeBytes;

	EMMediaDataBuffer* opProcessingResult = NULL;
	EMMediaDataBuffer** opBuffersForDestination = NULL; //[64];
	bool vProcessAudio = false;
	bool vShouldIncrementTime = false;
	int64 v = 0;

	m_opAudioClipRepository -> LockContainer();
	int32 vSizeOfRepository = m_opAudioClipRepository -> Size();
	m_opAudioClipRepository -> UnlockContainer();
		
	if((m_vShouldProduceAudio || (m_vProcessSingleAudioBuffer > 0))) // && vSizeOfRepository > 0)
	{	
		//Clear the list of unordered current buffers, and reset the flags
		
		int64 vNow = EMMediaTimer::Instance() -> AudioThenFrame();
		m_vNumAudioSlots = static_cast<int64>(ceil(static_cast<float>(vNow) / static_cast<float>(vAudioBufferSize / (EM_AUDIO_NUM_CHANNELS * EM_AUDIO_SAMPLESIZE))));
		
		//Does the clip repository contain audio at all? And if so,
		//is it time to produce an audio buffer?
		if((! IsRenderingVideo() || (IsRenderingVideo() && (vNow < EMMediaTimer::Instance() -> VideoThenFrame()))) || m_vProcessSingleAudioBuffer > 0)
		{
			vProcessAudio = true;
			vShouldIncrementTime = true;
			m_vNumAudioSlots++;
		}

/* BEGIN CRITICAL SECTION (LOCK AUDIO) */
		LockAudio();

		//If we can produce audio, then we should try to fetch some
		//audio buffers from the clips
		uint32 vNumBuffers = 0;
		if(vProcessAudio)
		{
#ifdef ENABLE_SONG_LENGTH_LIMITATION
			if(vNow < EMMediaUtility::Instance() -> TimeToFrames(SONG_LENGTH_LIMIT, EMMediaUtility::Instance() -> GetSystemAudioFormat()))
			{
#endif
//				try
//				{
				//eo << "Fetching audio buffers for " << (int) vNow << ef;
					vNumBuffers = static_cast<EMWinAudioClipRepository*>(m_opAudioClipRepository) -> GetNextBuffers(m_opAudioBufferList, EM_TYPE_ANY_AUDIO, vNow);	
//				}
//				catch(...)
//				{
//EMMediaDebugLog::Instance() ->Log("RunAudio caught an exception when fetching audio data from the repository. We're suspending the production thread now!");
//					m_opAudioThread -> Suspend(true);
//				}
#ifdef ENABLE_SONG_LENGTH_LIMITATION
			}
			else
			{
				vNumBuffers = 0;
			}
#endif
		}

		if(vNumBuffers > 0)
		{
			//Ok, we should have some buffers now. Loop through the list until it's empty!	
			while(NumBuffers(m_opAudioBufferList) > 0)
			{
				//Clear the destinations list.
				for(uint8 y = 0; y < 64; y++)
					for(uint8 x = 0; x < 64; x++)
						m_opAudioDestinations[y][x] = NULL;
			
				//Split the unordered (m_opAudioBufferList) set of buffers into ordered sets of
				//buffers, one list for each destination (m_opAudioDestinations is a list of list<..> pointers).
				//The m_opAudioBufferList is EMPTY after this call, and is thus ready for result-buffers from the
				//ProcessBuffer calls below.
				SplitArrays(m_opAudioBufferList, m_opAudioDestinations);
				
				if(NumDestinations(m_opAudioDestinations) <= 0)
				{
					for(uint8 x = 0; x < 64; x++)
						if(m_opAudioBufferList[x] != NULL)
						{
							m_opAudioBufferList[x] -> Recycle();
							m_opAudioBufferList[x] = NULL;
						}
					m_opAudioThread -> Suspend(true);
					vShouldIncrementTime = false;
					MessageBox(NULL, "Audio processessing cannot proceed!\n\nReason: At least one audio buffer lacks an internal\nprocessing destination. The reason may be one of (but\nnot limited to) the following:\n\n1\tThe DirectSound drivers for you sound card\n\tis not installed or does not function properly.\n2\tYou have disabled audio using command line parameters.\n3\tYou do not have a sound card installed.\n\nIf none of the above is true (eg if you're sure that audio\nworks in other applications, such as your MP3-player)\nplease let us know at bugs@elegant-media.com. Describe\nthe problem, your system and what you did to encounter\nthis behaviour (or lack thereof). Don't forget to include\nyour name and some means for us to contact you.", "Error!", MB_OK | MB_ICONEXCLAMATION);
					break;
				}

				//While there's more destination-lists				
				while(NumDestinations(m_opAudioDestinations) > 0)
				{
					//Get the first destination-list, only containing buffers with the exact same destination
					opBuffersForDestination = GetLeastReferencedDestination(m_opAudioDestinations);
					EMMediaDataBuffer* opDestinationCurrentlyBeingProcessed[64];
					memset(opDestinationCurrentlyBeingProcessed, 0, sizeof(EMMediaDataBuffer*) * 64);
					for(register uint8 x = 0; x < 64 && opBuffersForDestination[x] != NULL; x++)
					{
						opDestinationCurrentlyBeingProcessed[x] = opBuffersForDestination[x];
						opBuffersForDestination[x] = NULL;
					}
				
					//If it contains buffers, we should process it. This is where the actual Processing is initiated!!
					//Note that the result is stored. The result from the ProcessBuffer call is what should be
					//further routed to other destinations (as set in the resulting EMMediaDataBuffer), or, if NULL,
					//we should do nothing further.
					if(NumBuffers(opDestinationCurrentlyBeingProcessed) > 0)
					{
//						try
//						{
							//Since we sortof stuck in this large while-loop for a while, it might be that the project is
							//told to stop at some time, before we reach the top of the block again (with the m_vShouldProduceAudio check).
							//So.. Do a check here. If we should produce (either continously or one single buffer), we do that.
							//If we should not.. We go through all the lists with buffers ready to be processed, and Recycle all of
							//them. Make sure we don't miss a single buffer - cause we don't want a buffer leak on our hands!
							if(m_vShouldProduceAudio || (m_vProcessSingleAudioBuffer > 0))
							{
								uint32 v = NumBuffers(opDestinationCurrentlyBeingProcessed);
								list<EMMediaDataBuffer*> oList;
								for(register uint8 vI = 0; vI < v; vI++)
									oList.push_back(opDestinationCurrentlyBeingProcessed[vI]);

								UnlockAudio();

								opProcessingResult = opDestinationCurrentlyBeingProcessed[0] -> m_opDestination -> ProcessBufferE(&oList);

//EMMediaDebugLog::Instance() ->Log("RunAudio (1) acquiring the audio protection lock (after ProcessBufferE)");
								LockAudio();
//EMMediaDebugLog::Instance() ->Log("RunAudio (1) now have the audio protection lock (after ProcessBufferE)");

								//If the resulting buffer from the rescent processing has a destination that exists in the
								//"pending for processing" list, then we should add it there (in order to be able to route
								//processing "backwards" into an "earlier" destination (eg an effect track to an effect track).
								bool vStored = false;
								for(register uint8 vY = 0; vY < 64 && opProcessingResult != NULL; vY++)
								{
									if(m_opAudioDestinations[vY][0] != NULL)
									{
										if(m_opAudioDestinations[vY][0] -> m_opDestination -> GetID() == opProcessingResult -> m_opDestination -> GetID())
										{
											for(uint8 vX = 0; vX < 64; vX++)
											{
												if(m_opAudioDestinations[vY][vX] == NULL)
												{
													m_opAudioDestinations[vY][vX] = opProcessingResult;
													opProcessingResult = NULL;
													vStored = true;
													break;
												}
											}
										}
									}
								}
							}
							else
							{
								//Make sure we clear all the arrays (ie make sure we Recycle all buffers) if 
								//we were to stop in the middle of processing!
								for(register uint8 x = 0; x < 64 && opDestinationCurrentlyBeingProcessed[x] != NULL; x++)
								{
									opDestinationCurrentlyBeingProcessed[x] -> Recycle();
									opDestinationCurrentlyBeingProcessed[x] = NULL;
								}
								for(y = 0; y < 64; y++)
									for(x = 0; x < 64; x++)
										if(m_opAudioDestinations[y][x] != NULL)
										{
											m_opAudioDestinations[y][x] -> Recycle();
											m_opAudioDestinations[y][x] = NULL;
										}
								for(x = 0; x < 64; x++)
									if(m_opAudioBufferList[x] != NULL)
									{
										m_opAudioBufferList[x] -> Recycle();
										m_opAudioBufferList[x] = NULL;
									}
								break;
							}
//						}
//						catch(...)
//						{
//							vShouldIncrementTime = false;
							//EMMediaDebugLog::Instance() ->Log("RunAudio (2) acquiring the audio protection lock");
//							LockAudio();
							//EMMediaDebugLog::Instance() ->Log("RunAudio (2) now have the audio protection lock");
//							break;
//						}
					}

					//If the opProcessingResult was other that NULL, then we have another buffer (and hopefully another destination - otherwise
					//we probably have an infinite loop on our hands! :)) to route and process!
					if(opProcessingResult != NULL)
					{
						for(register uint8 x = 0; x < 64; x++)
							if(m_opAudioBufferList[x] == NULL)
							{
								m_opAudioBufferList[x] = opProcessingResult;
								break;
							}
						opProcessingResult = NULL;
					}
				}
			
				//Done with this set of destinations. Continue looping the bufferlist, since it may very well
				//be other than empty now, since we've hopefully received some resulting buffers from the
				//ProcessBuffer-call(s).
				for(y = 0; y < 64; y++)
					for(uint8 x = 0; x < 64 && m_opAudioDestinations[y][x] != NULL; x++)
						m_opAudioDestinations[y][x] = NULL;

				if(! vShouldIncrementTime)
					break;
			}
		}
		else
			::Sleep(1);

		for(register uint8 x = 0; x < 64 && m_opAudioBufferList[x] != NULL; x++)
			m_opAudioBufferList[x] = NULL;

		//EMMediaDebugLog::Instance() ->Log("RunAudio (3) now returning the audio protection lock");
		UnlockAudio();
/* END CRITICAL SECTION (UNLOCK AUDIO) */

		//IF ERROR OCCURED AND RENDERING STOP EVERYTHING!!
		if(IsRenderingAudio())
			if(EMWinMediaSingletonMuxer::Instance() -> ErrorHasOccured())
			{
				SetRenderingAudio(false);
				SetRenderingVideo(false);
				int32 vCommandStop = COMMAND_STOP;
				EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_POST_COMMAND, &vCommandStop);
				EMMediaEngine::Instance() -> GetSettingsRepository() -> Notify(EM_RENDERING_COULD_NOT_START);
				EMWinMediaSingletonMuxer::Instance() -> SetErrorHasOccured(false);
			}

		//Advance the time, measured in audio frames, to the nearest slot (either it's an audio slot or a video slot, or both).
		if(vShouldIncrementTime && vNow == EMMediaTimer::Instance() -> AudioThenFrame() && (m_vProcessSingleAudioBuffer <= 0) && (EMMediaTimer::Instance() -> GetState() == EM_PLAYING || EMMediaTimer::Instance() -> GetState() == EM_RECORDING))
		{
			int64 vNextAudioFrame = static_cast<int64>((m_vNumAudioSlots) * (vAudioBufferSize / (EM_AUDIO_NUM_CHANNELS * EM_AUDIO_SAMPLESIZE)));
			int64 vToAudio = vNextAudioFrame - vNow;
			int64 vIncreaseAmount = 1;
			vIncreaseAmount = vToAudio;
			if(! EMMediaTimer::Instance() -> IncreaseAudioThenFrame(vIncreaseAmount))
			{
				m_vShouldProduceAudio = false;
				
			}
		}
		
		if(m_vProcessSingleAudioBuffer > 0 && ! m_vAudioIsActive)
			m_vProcessSingleAudioBuffer--;
		else
			m_vProcessSingleAudioBuffer = 0;

		if((m_vProcessSingleAudioBuffer == 0 && ! m_vAudioIsActive) || (m_vProcessSingleAudioBuffer == 0 && EMMediaTimer::Instance() -> GetState() == EM_STOPPED))
		{
			//eo << "Now suspending audio production thread..." << ef;
			m_opAudioThread -> Suspend(true);
		}
	}
	else
		::Sleep(1);

}

void EMMediaProject::SeekAndDisplay(int64 p_vNow)
{ 
}

void EMMediaProject::MergeList(list<EMMediaBufferSource*>*opClipList, list<EMMediaClip*>*opVideoList, list<EMMediaClip*>*opTransList)
{
	int32 vVideoPrio;
	int32 vTransPrio;

	EMMediaBufferSource* opSource;

	opClipList -> clear();

	while(opVideoList -> size()!=0 || opTransList -> size()!=0)
	{
		if (opVideoList -> size() != 0)
			vVideoPrio = opVideoList -> front() -> GetTrack() -> GetPriority();	
		else
			vVideoPrio = 1000000;

		if (opTransList -> size() != 0)
			vTransPrio = opTransList -> front() -> GetTrack() -> GetPriority();
		else
			vTransPrio = 1000000;

		if(vVideoPrio < vTransPrio)
		{
			opClipList -> push_back(opVideoList -> front());
			opVideoList -> pop_front();
		}
		else if(vVideoPrio == vTransPrio)
		{
			opClipList -> push_back(opVideoList -> front());
			opVideoList -> pop_front();
			opTransList -> pop_front();
		}
		else
		{
			opSource = opTransList -> front() -> GetTrack() -> GetSilenceGenerator();
//			opSource -> SetDestination(opTransList -> front() -> GetTrack() -> GetTrackDestination());
			opClipList -> push_back(opSource);
			opTransList -> pop_front();			
		}
	}
}

void EMMediaProject::RunBuffer(EMMediaDataBuffer** p_opBuffer, bool p_vIgnoreTransition)
{
	EMDestinationPosition oPos;

	list<EMMediaDataBuffer*> opBufferList;

	oPos = (*p_opBuffer) -> m_opDestination -> GetPosition();

	while(((*p_opBuffer) -> m_opDestination != NULL) && (oPos != EM_OUTPUT) && (oPos != EM_TRANSITION || p_vIgnoreTransition))
	{
		opBufferList.clear();
		opBufferList.push_front(*p_opBuffer);
		*p_opBuffer = (*p_opBuffer) -> m_opDestination -> ProcessBufferE(&opBufferList);
		oPos = (*p_opBuffer) -> m_opDestination -> GetPosition();
	}
}


EMSilentVideoSource* EMMediaProject::GrabAnySilenceGenerator()
{
	EMMediaBufferSource* opRetVal = NULL;
	EMMediaTrack* opTrack = GetNextTrack(NULL);

	if(opTrack != NULL)
		opRetVal = opTrack  -> GetSilenceGenerator();

	return static_cast<EMSilentVideoSource*>(opRetVal);
}

EMMediaTrack* EMMediaProject::GetNextTrack(EMMediaTrack* p_opTrack)
{
	int64 vPrio;
	int64 vCurrentPrio = 100000000;

	EMMediaTrack* opCurrent = NULL;
	EMMediaTrack* opTemp = NULL;
	EMMediaTrackRepository* opUsed = GetUsedTracks();
	EMMediaTrackRepository* opFree = GetUnusedTracks();


	 if(p_opTrack == NULL)
		 vPrio =  -1;
	 else
		 vPrio = p_opTrack -> GetPriority();

	opUsed->LockContainer();
	opUsed->Rewind();

	while(opUsed -> Current() != NULL)
	{
		opTemp = opUsed -> Current();

		if(!(opTemp -> IsObjectDeleted()) &&
			((opTemp -> GetType() & EM_TYPE_ANY_VIDEO) != 0) &&
			(opTemp -> GetPriority() > vPrio) && 
			(opTemp -> GetPriority() < vCurrentPrio))
		{
			opCurrent = opUsed -> Current();
			vCurrentPrio = opCurrent -> GetPriority();
		}
		opUsed->Next();
	}
	opUsed->UnlockContainer();

	opFree->LockContainer();
	opFree->Rewind();

	while(opFree -> Current() != NULL)
	{
		if(((opFree -> Current() -> GetType() & EM_TYPE_ANY_VIDEO) > 0) && opFree -> Current() -> GetPriority() > vPrio && opFree -> Current() -> GetPriority() < vCurrentPrio)
		{
			opCurrent = opFree -> Current();
			vCurrentPrio = opCurrent -> GetPriority();
		}
		opFree->Next();
	}
	opFree->UnlockContainer();

	return opCurrent == NULL ? p_opTrack : opCurrent;
}

void EMMediaProject::RunVideo()
{
	list<EMMediaDataBuffer*>* opBuffersForDestination = NULL;
	list<EMMediaDataBuffer*>* opBufferList = GetList();
	list<list<EMMediaDataBuffer*>*>* opDestinations = EM_new list<list<EMMediaDataBuffer*>*>();
	float vAudioFrameRate = (float) *(static_cast<int32*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_AUDIO_FRAMERATE)));
	EMMediaFormat oSystemAudioFormat(EM_TYPE_ANY_AUDIO);

	list<EMMediaClip*> opVideoList;
	list<EMMediaClip*> opTransList;
	list<EMMediaBufferSource*> opClipList;

	bool vProcessVideo = false;
	bool vShouldIncrementTime = false;
	float vWorkFPS;
	m_opVideoClipRepository -> LockContainer();
	int32 vSizeOfRepository = m_opVideoClipRepository -> Size();
	m_opVideoClipRepository -> UnlockContainer();
		
	if((m_vShouldProduceVideo || (m_vProcessSingleVideoBuffer > 0))) // && vSizeOfRepository > 0 && !m_vIsArmed)
	{
		EMMediaDataBuffer* opProcessingResult = NULL;

		opBufferList -> clear();
		
		int64 vNow;
		if(IsRenderingVideo())
			vNow = EMMediaTimer::Instance() -> VideoThenFrame();
		else
			vNow = EMMediaTimer::Instance() -> NowFrame();

		//Write out the position where vNow is
		//int64 vNowInTime = EMMediaUtility::Instance() -> FramesToTime(vNow, EMMediaUtility::Instance() -> GetSystemAudioFormat());
		//eo<< "vNow says we have played:" << vNowInTime << " micro seconds" << ef;

		if(IsRenderingVideo())
			vWorkFPS = *(static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_RENDER_FRAMERATE)));
		else
			vWorkFPS = *(static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FRAMERATE)));

		m_vNumVideoSlots = static_cast<int64>(vWorkFPS * static_cast<float>(vNow) / vAudioFrameRate);
			
		//Does the clip repository contain audio at all? And if so,
		//is it time to produce an audio buffer?

		//Does the clip repository contain video at all? And if so,
		if((vNow >= 0 && (! IsRenderingAudio())) || (vNow>=0 && (IsRenderingAudio() && (EMMediaTimer::Instance() -> AudioThenFrame() >= vNow))) ) //EMBeMediaUtility::TimeToFrames(static_cast<float>(m_vNumVideoSlots) * 1000000.0 / vWorkFPS, &oSystemAudioFormat) || (m_vProcessSingleVideoBuffer > 0))
		{
			vProcessVideo = true;
			vShouldIncrementTime = true;
			m_vNumVideoSlots++;
		} 

		if(m_opVideoClipRepository -> Size() == 0)
		{
			m_opVideoThread -> Suspend(true);
			vProcessVideo = false;
		}

		if(! LockVideo())
			EMDebugger("ERROR! RunVideo could not acquire video processing semaphore!");

		//If we can produce video, then we should try to fetch some
		//video buffers from the clips, just like we did with audio above

		if(vProcessVideo)
		{
			if(EMVideoSynchronizer::Instance() -> NeedsToSynch() || (m_vProcessSingleVideoBuffer > 0))
			{
				EMVideoSynchronizer::Instance() -> SignalForSeek();
				EMVideoSynchronizer::Instance() -> SetRelativeOffsetTime(EMMediaUtility::Instance() -> FramesToTime(vNow, EMMediaUtility::Instance() -> GetSystemAudioFormat()));
				EMVideoSynchronizer::Instance() -> SetNeedsToSynch(false);
			}
			/*if(söknineg eller start)
			{
				EMVideoSynchronizer::Instance() -> ZeroRealTimeClock();
			}*/

#ifdef ENABLE_SONG_LENGTH_LIMITATION
			if(vNow < EMMediaUtility::Instance() -> TimeToFrames(SONG_LENGTH_LIMIT, EMMediaUtility::Instance() -> GetSystemAudioFormat()))
			{
#endif
	
				//This marked out code is for handling drop frames
				int64 vRealtimeRelativeClock = EMMediaUtility::Instance() -> TimeToFrames(EMVideoSynchronizer::Instance() -> GetSynchClockRelativeRealTime(), EMMediaUtility::Instance() -> GetSystemAudioFormat());
				int64 vRelativeOffsetTime = EMMediaUtility::Instance() -> TimeToFrames(EMVideoSynchronizer::Instance() -> GetRelativeOffsetTime(), EMMediaUtility::Instance() -> GetSystemAudioFormat());	
				int64 p_vTime = vRealtimeRelativeClock + vRelativeOffsetTime;
				//if(m_vProcessSingleVideoBuffer > 0)
					m_opVideoClipRepository -> GetClipsInTimeslot(&opVideoList, vNow);
				//else
				//	m_opVideoClipRepository -> GetClipsInTimeslot(&opVideoList, p_vTime /*vNow*/);

				m_opVideoTransitionClipRepository -> GetClipsInTimeslot(&opTransList, vNow);
				MergeList(&opClipList, &opVideoList, &opTransList);
#ifdef ENABLE_SONG_LENGTH_LIMITATION
			}
			else
			{
				opClipList.clear();
			}
#endif

			if(opClipList.size() !=0)
			{
				list<EMMediaDataBuffer*> opStack;
				EMMediaDataBuffer*  opBuffer;
				EMMediaDataBuffer*  opBufferOld;
				EMMediaBufferSource* opSource;
				EMMediaTrack*		opTrack = NULL;
				EMMediaTrack*		opLastTrack = NULL;

				opStack.clear();

				do
				{
					if(opClipList.size() == 0)
					{
						opTrack = GetNextTrack(opLastTrack);
						opSource = opTrack -> GetSilenceGenerator();
//						opSource -> SetDestination(opTrack -> GetTrackDestination());
//						opSource -> InitCheckBufferE(&(opBuffer -> m_oFormat));

						opBuffer = opSource -> GetBufferAt(vNow, (m_vProcessSingleVideoBuffer > 0));

						RunBuffer(&opBuffer);

						if(opBuffer -> m_opDestination -> GetPosition() == EM_TRANSITION)
							opBuffer -> m_opDestination =  opBuffer -> m_opDestination -> GetDestination();

//						opClipList.push_front(&opSource);
					}
					else
					{
						EMMediaBufferSource* opClip = opClipList.front();
						opClipList.pop_front();

						opLastTrack = opClip -> GetTrack();

						opBuffer = opClip->GetBufferAt(vNow, (m_vProcessSingleVideoBuffer > 0));
						RunBuffer(&opBuffer);
					}

					if(opBuffer -> m_opDestination == NULL)
					{
						EMDebugger("EMMediaProject::RunVideo() - ERROR! Buffer with NULL destination");
						opStack.clear();
					}

					switch(opBuffer->m_opDestination->GetPosition())
					{
					case EM_TRANSITION:
						opStack.push_front(opBuffer);
						break;
					default:
						while(opStack.size() > 0)
						{
							opBufferOld = opStack.front();
							opStack.pop_front();

							opBuffer -> m_opDestination = opBufferOld -> m_opDestination;

							opBufferList->push_front(opBuffer);
							opBufferList->push_front(opBufferOld);
							
							opBuffer = opBufferOld -> m_opDestination ->ProcessBufferE(opBufferList);
							RunBuffer(&opBuffer);
						}

						opBufferList->push_front(opBuffer);
						if(opBuffer -> m_opDestination != NULL)
							opBuffer -> m_opDestination -> ProcessBufferE(opBufferList);
						opBuffer->Recycle();
						
						while(opClipList.size() != 0)
						{
							opClipList.front() -> m_vShouldSeek = true;
							opClipList.pop_front();
						}


						break;

					}
				}
				while(opStack.size()>0);
			}
			else
			{
				EMSilentVideoSource* opSilence = GrabAnySilenceGenerator();
				if (opSilence != NULL)
				{
					EMMediaDataBuffer*   opBuffer = opSilence -> GetBufferAt(vNow, (m_vProcessSingleVideoBuffer > 0));
					RunBuffer(&opBuffer, true);
					opBufferList -> push_front(opBuffer);
					opBuffer -> m_opDestination -> ProcessBufferE(opBufferList);
					opBuffer -> Recycle();
				} else
				{
					vProcessVideo = false;
				}
			}

		}
		else
			::Sleep(1);

		UnlockVideo();

		//IF ERROR OCCURED AND RENDERING STOP EVERYTHING!!
		if(IsRenderingVideo())
		{
			if(EMWinMediaSingletonMuxer::Instance() -> ErrorHasOccured())
			{
				SetRenderingVideo(false);
				SetRenderingAudio(false);
				int32 vCommandStop = COMMAND_STOP;
				EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_POST_COMMAND, &vCommandStop);
				EMMediaEngine::Instance() -> GetSettingsRepository() -> Notify(EM_RENDERING_COULD_NOT_START);
				EMWinMediaSingletonMuxer::Instance() ->SetErrorHasOccured(false);
			}
		}
		else
			::Sleep(4); //Don't sleep if we're rendering!


		if(vShouldIncrementTime && vNow == EMMediaTimer::Instance() -> VideoThenFrame() && (m_vProcessSingleVideoBuffer <= 0))
		{
			//Convert a number of video frames to a number of audio frames!
			//Formula: NumberOfAudioFramesPlayed = AudioFPS * VideoFramesPlayed / VideoFPS
			int64 vAudioFrames = static_cast<int64>(vAudioFrameRate * static_cast<float>(m_vNumVideoSlots) / vWorkFPS);
			vAudioFrames -= vNow;
			
			//Safety! We don't want the video processing to hang because of rounding errors
			if(vAudioFrames <= 0)
				vAudioFrames = static_cast<int64>(ceil(vAudioFrameRate * static_cast<float>(1) / vWorkFPS));

			if(! EMMediaTimer::Instance() -> IncreaseVideoThenFrame(vAudioFrames))
			{
				m_vShouldProduceVideo = false;
			}
		}
		else
			Sleep(1);

		if(m_vProcessSingleVideoBuffer > 0 && ! m_vVideoIsActive)
			m_vProcessSingleVideoBuffer--;
		else
			m_vProcessSingleVideoBuffer = 0;

		if(m_vProcessSingleVideoBuffer == 0 && ! m_vVideoIsActive)
			m_opVideoThread -> Suspend(true);

		//::Sleep(4);
	}	
	else
		::Sleep(3);
	delete opDestinations;
	RecycleList(opBufferList);
}

void EMMediaProject::PlaySingleBuffer()
{ 
	m_vProcessSingleVideoBuffer = 1;
	m_vProcessSingleAudioBuffer = 1;

	if(m_vAudioOn)
//EMMediaDebugLog::Instance() ->Log("Project::PlaySingleBuffer resuming the audio production thread now!");
	m_opAudioThread -> Resume();

	if(m_vVideoOn)
		m_opVideoThread -> Resume();
}

void EMMediaProject::ShouldProduceVideo(bool p_vShouldproduceVideo, bool p_vEmergencyStop)
{
	if(!p_vEmergencyStop)
		LockVideo();
	m_vShouldProduceVideo = p_vShouldproduceVideo;
	if(!p_vEmergencyStop)
		UnlockVideo();
}

void EMMediaProject::ShouldProduceAudio(bool p_vShouldproduceAudio, bool p_vEmergencyStop)
{
	if(!p_vEmergencyStop)
		LockAudio();
	m_vShouldProduceAudio = p_vShouldproduceAudio;
	if(!p_vEmergencyStop)
		UnlockAudio();
}


void EMMediaProject::StartPlaybackE()
{
	LockAudio();
	m_vShouldProduceAudio = true;
	if(m_vAudioOn)
	{
		m_vAudioIsActive = true;
		m_opAudioThread -> Resume();
	}
	UnlockAudio();

	LockVideo();
	m_vShouldProduceVideo = true;
	EMVideoSynchronizer::Instance() -> ZeroRealTimeClock();
	EMVideoSynchronizer::Instance() -> SetNeedsToSynch(true);
	//EMVideoSynchronizer::Instance() -> SetRelativeOffsetTime(0);
	if(m_vVideoOn)
	{
		m_vVideoIsActive = true;
		m_opVideoThread -> Resume();
	}
	UnlockVideo();

	if(m_vMidiOn)
		EMWinDXMIDIProducer::Instance() -> StartPlaybackE();
}

void EMMediaProject::StopPlaybackE()
{
	bool vFlag = false;

	LockAudio();
	m_vShouldProduceAudio = false;
	if(m_vAudioOn)
		vFlag = m_vAudioIsActive = false;
	UnlockAudio();
	if(vFlag)
		m_opAudioThread -> Suspend();


	LockVideo();
	m_vShouldProduceVideo = false;
	if(m_vVideoOn)
		vFlag = m_vVideoIsActive = false;
	UnlockVideo();
	if(vFlag)
		m_opVideoThread -> Suspend();

#ifdef DEBUG
	bool c = EMWinDXMIDIConsumer::Instance() -> __IsAlive();
	bool p = EMWin	DXMIDIProducer::Instance() -> __IsAlive();
#endif

	if(m_vMidiOn)
		EMWinDXMIDIProducer::Instance() -> StopPlaybackE();

	//Do this wheter we were rendering or not
	EMMediaEngine::Instance() -> GetSettingsRepository() -> Notify(EM_RENDERING_HAS_FINNISHED);//, EM_SETTING_INT, EM_new int(p_vWidth), false);

	if(IsRenderingVideo())
	{
		SetRenderingVideo(false);
	}

	if(IsRenderingAudio())
	{
		SetRenderingAudio(false);
	}

}

void EMMediaProject::StartRecordingE()
{
	if(m_opUsedTracks == NULL)
		EMDebugger("Used tracks is NULL");
	if(m_opUnusedTracks == NULL)
		EMDebugger("Unused tracks is NULL");

	m_opUsedTracks -> LockContainer();
	try
	{
		m_opUsedTracks -> Rewind();
		while(m_opUsedTracks -> Current() != NULL)
		{
			if((m_opUsedTracks -> Current() -> GetType() & EM_TYPE_MIDI) > 0)
				m_opRecordingMIDITrackRepository -> AddTrack(static_cast<EMMediaMIDITrack*>(m_opUsedTracks -> Current()));
			m_opUsedTracks -> Current() -> StartRecording();
			m_opUsedTracks -> Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception while starting recording!");
	}
	m_opUsedTracks -> UnlockContainer();

	m_opUnusedTracks -> LockContainer();
	try
	{
		m_opUnusedTracks -> Rewind();
		while(m_opUnusedTracks -> Current() != NULL)
		{
			if((m_opUnusedTracks -> Current() -> GetType() & EM_TYPE_MIDI) > 0)
				m_opRecordingMIDITrackRepository -> AddTrack(static_cast<EMMediaMIDITrack*>(m_opUnusedTracks -> Current()));
			m_opUnusedTracks -> Current() -> StartRecording();
			m_opUnusedTracks -> Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception while starting recording!");
	}
	m_opUnusedTracks -> UnlockContainer();

	m_opMIDIEventRepository -> PreAllocEvents();

//	EMInputRepository::Instance() -> PrepareToPlayE();
//	EMInputRepository::Instance() -> StartE();
	StartPlaybackE();
	
//	m_vStarted = true;
}

void EMMediaProject::StopRecordingE()
{
	if(m_opUsedTracks == NULL)
		EMDebugger("Used tracks is NULL");
	if(m_opUnusedTracks == NULL)
		EMDebugger("Unused tracks is NULL");

	StopPlaybackE();

	list<EMMediaTrack*> oTracksToStop;
	m_opUsedTracks -> LockContainer();
	m_opUsedTracks -> Rewind();
	while(m_opUsedTracks -> Current() != NULL)
	{
		oTracksToStop.push_back(m_opUsedTracks -> Current());
		m_opUsedTracks -> Next();
	}
	m_opUsedTracks -> UnlockContainer();

	m_opUnusedTracks -> LockContainer();
	m_opUnusedTracks -> Rewind();
	while(m_opUnusedTracks -> Current() != NULL)
	{
		oTracksToStop.push_back(m_opUnusedTracks -> Current());
		m_opUnusedTracks -> Next();
	}
	m_opUnusedTracks -> UnlockContainer();

	try
	{
		while(oTracksToStop.size() > 0)
		{
			EMMediaTrack* opTrack = oTracksToStop.front();
			opTrack -> StopRecording();
			if((opTrack -> GetType() & EM_TYPE_MIDI) > 0)
				m_opRecordingMIDITrackRepository -> RemoveTrack(static_cast<EMMediaMIDITrack*>(opTrack));
			oTracksToStop.pop_front();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception while stopping recording!");
	}
	
/*	m_opUsedTracks -> LockContainer();
	try
	{
		m_opUsedTracks -> Rewind();
		while(m_opUsedTracks -> Current() != NULL)
		{
			eo << "WARNING! Need to make sure we only remove tracks that are armed!" << ef;
			m_opUsedTracks -> UnlockContainer(); //TODO: Fix! Temporary unlock only!!!!!
			m_opUsedTracks -> Current() -> StopRecording();
			m_opUsedTracks -> LockContainer();
			if((m_opUsedTracks -> Current() -> GetType() & EM_TYPE_MIDI) > 0)
				m_opRecordingMIDITrackRepository -> RemoveTrack(static_cast<EMMediaMIDITrack*>(m_opUsedTracks -> Current()));
			m_opUsedTracks -> Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception while stopping recording!");
	}
	m_opUsedTracks -> UnlockContainer(); */

/*	m_opUnusedTracks -> LockContainer();
	try
	{
		m_opUnusedTracks -> Rewind();
		while(m_opUnusedTracks -> Current() != NULL)
		{
			eo << "WARNING! Need to make sure we only remove tracks that are armed!" << ef;
			m_opUnusedTracks -> UnlockContainer();  //TODO: Fix! Temporary unlock only!!!!!
			m_opUnusedTracks -> Current() -> StopRecording();
			m_opUnusedTracks -> LockContainer(); 
			if((m_opUnusedTracks -> Current() -> GetType() & EM_TYPE_MIDI) > 0)
				m_opRecordingMIDITrackRepository -> RemoveTrack(static_cast<EMMediaMIDITrack*>(m_opUnusedTracks -> Current()));
			m_opUnusedTracks -> Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception while stopping recording!");
	}
	m_opUnusedTracks -> UnlockContainer(); */

	
	list<EMRecording*>* opRecordings = EMInputRepository::Instance() -> GetLastRecordings();
	while(opRecordings -> size() > 0)
	{
		EMRecording* opCurrentRecording = opRecordings -> front();
		int32 vID = EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool() -> SearchFor(opCurrentRecording -> m_oFileName);
		
//		EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool() -> LockContainer();
		try
		{
			if(EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool() -> Find(vID) -> GetSize() > 0)
			{
				list<EMMediaTrack*>::iterator i;
				for(i = opCurrentRecording -> m_oRecorderTracks.begin(); i != opCurrentRecording -> m_oRecorderTracks.end(); i++)
				{
					int32 vMediaTrackID = (*i) -> GetID();
//					EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool() -> UnlockContainer();
					int32 vpMediaClipID = *static_cast<int32*>(EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(MEDIA_COMMAND_DROP_MEDIA, &(opCurrentRecording -> m_vMediaFileEntryID), &vMediaTrackID, &(opCurrentRecording -> m_vStartTime)));
					uint32 vTrackIndex = *static_cast<uint32*>(EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_GET_TRACK_GUI_INDEX, &vMediaTrackID, NULL, NULL));
					EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_ADD_G_AUDIO_CLIP, &vpMediaClipID, &vTrackIndex, &(opCurrentRecording -> m_vStartTime));
//					EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool() -> LockContainer();


					try
					{
						EMMediaClip* opClip = GetAudioClipRepository() -> Find(vpMediaClipID);

						if(opClip == NULL)
						{
							EMDebugger("EMMediaProject::StopRecordingE - Weird error 1.3");
						}

						string oFullName = opClip -> GetFileName();
						int32 vEntryIds[2];

						vEntryIds [0] = opClip -> GetDescriptor() -> GetMediaPoolEntryID();
						vEntryIds [1] = -1;

						EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_ADD_TO_MEDIA_POOL_VIEW, (void*)oFullName.c_str(), vEntryIds);
						EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_UPDATE_INSTANCE_IN_MEDIA_POOL_VIEW, (void*)oFullName.c_str());

						//eo << "Found sampled file: " << oFullName.c_str() << ef;
					}
					catch(...)
					{
					}
				}
			}
		}
		catch(...)
		{
			EMDebugger("ERROR! Exception in EMMediaProject::StopRecordingE()");
		}
//		EMMediaEngine::Instance() -> GetMediaProject() -> GetMediaPool() -> UnlockContainer();
		opRecordings -> pop_front();
	}


	//******************* Make sure we move track's that are no longer empty to the used-tracks repository *************************
	list<EMMediaTrack*> oTracksToMoveToUsed;
	m_opUnusedTracks -> LockContainer();
	try
	{
		m_opUnusedTracks -> Rewind();
		while(m_opUnusedTracks -> Current() != NULL)
		{
		if(m_opAudioClipRepository -> CountClipsForTrack(m_opUnusedTracks -> Current()) > 0)
				oTracksToMoveToUsed.push_back(m_opUnusedTracks -> Current());
			else if(m_opVideoClipRepository -> CountClipsForTrack(m_opUnusedTracks -> Current()) > 0)
				oTracksToMoveToUsed.push_back(m_opUnusedTracks -> Current());
			else if(m_opVideoTransitionClipRepository -> CountClipsForTrack(m_opUnusedTracks -> Current()) > 0)
				oTracksToMoveToUsed.push_back(m_opUnusedTracks -> Current());
			else if(m_opMIDIClipRepository -> CountClipsForTrack(m_opUnusedTracks -> Current()) > 0)
				oTracksToMoveToUsed.push_back(m_opUnusedTracks -> Current());
			m_opUnusedTracks -> Next();
		}
	}
	catch(...)
	{
		EMDebugger("ERROR! Exception while stopping recording!");
	}
	m_opUnusedTracks -> UnlockContainer();
	while(oTracksToMoveToUsed.size() > 0)
	{
		m_opUnusedTracks -> RemoveTrack(oTracksToMoveToUsed.front());
		m_opUsedTracks -> AddTrack(oTracksToMoveToUsed.front());
		oTracksToMoveToUsed.pop_front();
	}
	//*******************************************************************************************************************************
}

EMMediaClipRepository* EMMediaProject::GetAudioClipRepository() const
{
//	if(m_opAudioClipRepository == NULL)
//		EMDebugger("ERROR! EMMediaProject can't return audio clip repository since it's NULL!");
	return m_opAudioClipRepository;
}

EMMediaClipRepository* EMMediaProject::GetVideoClipRepository() const
{
//	if(m_opVideoClipRepository == NULL)
//		EMDebugger("ERROR! EMMediaProject can't return video clip repository since it's NULL!");
	return m_opVideoClipRepository;
}

EMMediaClipRepository* EMMediaProject::GetVideoTransitionClipRepository() const
{
//	if(m_opVideoClipRepository == NULL)
//		EMDebugger("ERROR! EMMediaProject can't return video clip repository since it's NULL!");
	return m_opVideoTransitionClipRepository;
}

EMMediaTransitionTrackRepository* EMMediaProject::GetVideoTransitionTrackRepository() const
{
	return m_opVideoTransitionTrackRepository;
}

EMMediaClipRepository* EMMediaProject::GetMIDIClipRepository() const
{
//	if(m_opMIDIClipRepository == NULL)
//		EMDebugger("ERROR! EMMediaProject can't return MIDI clip repository since it's NULL!");
	return m_opMIDIClipRepository;
}

EMMediaMIDIEventRepository* EMMediaProject::GetMIDIEventRepository() const
{
	return m_opMIDIEventRepository;
}

EMRecordingMIDITrackRepository* EMMediaProject::GetRecordingMIDITracks() const
{
	return m_opRecordingMIDITrackRepository;
}

bool EMMediaProject::IsPlaying() const
{
	return (m_vAudioIsActive || m_vVideoIsActive); // || (m_vProcessSingleAudioBuffer > 0 || m_vProcessSingleVideoBuffer > 0));
}

EMMediaTrackRepository* EMMediaProject::GetUnusedTracks() const
{
	return m_opUnusedTracks;
}

EMMediaTrackRepository* EMMediaProject::GetUsedTracks() const
{
	return m_opUsedTracks;
}

void EMMediaProject::SetArmed(bool p_vFlag)
{
	m_vIsArmed = p_vFlag;
	SetPreview();
}
void EMMediaProject::SetPreview()
{
	if(m_opUsedTracks == NULL)
		EMDebugger("Used tracks is NULL");
	if(m_opUnusedTracks == NULL)
		EMDebugger("Unused tracks is NULL");

	if(IsArmed())
	{
		//Makes the input repository call all inputs and start previewing
		//(the active ones will then actually start up.. The other will just ignore this operation).
		EMInputRepository::Instance() -> StartPreviewE();
	}
	else
	{
		//Makes the input repository call all inputs and stop previewing
		//(the active ones will then actually start up.. The other will just ignore this operation).
		EMInputRepository::Instance() -> StopPreviewE();
	}
	
/*	m_opUsedTracks -> LockContainer();
	m_opUsedTracks -> Rewind();
	while(m_opUsedTracks -> Current() != NULL)
	{
		if(m_opUsedTracks -> Current() -> GetType() & EM_TYPE_ANY_VIDEO > 0)
		{
			if(m_vIsArmed)
				m_opUsedTracks -> Current() -> StartPreview();
			else
				m_opUsedTracks -> Current() -> StopPreview();				
		}
		m_opUsedTracks -> Next();
	}
	m_opUsedTracks -> UnlockContainer();
	
	m_opUnusedTracks -> LockContainer();
	m_opUnusedTracks -> Rewind();
	while(m_opUnusedTracks -> Current() != NULL)
	{
		if(m_opUnusedTracks -> Current() -> GetType() & EM_TYPE_ANY_VIDEO > 0)
		{
			if(m_vIsArmed)
				m_opUnusedTracks -> Current() -> StartPreview();
			else
				m_opUnusedTracks -> Current() -> StopPreview();				
		}
		m_opUnusedTracks -> Next();
	}
	m_opUnusedTracks -> UnlockContainer(); */
}

bool EMMediaProject::IsArmed() const
{
	return m_vIsArmed;
}

bool EMMediaProject::LockAudio()
{
	if(m_opAudioProcessingSemaphore == NULL)
		m_opAudioProcessingSemaphore = EMSemaphore::CreateEMSemaphore("Audio processing semaphore");
	m_opAudioProcessingSemaphore -> Acquire();
	return true;
}

bool EMMediaProject::LockVideo()
{
	if(m_opVideoProcessingSemaphore == NULL)
		m_opVideoProcessingSemaphore = EMSemaphore::CreateEMSemaphore("Video processing semaphore");
	m_opVideoProcessingSemaphore -> Acquire();
	return true;
}

void EMMediaProject::UnlockAudio()
{
	if(m_opAudioProcessingSemaphore != NULL)
		m_opAudioProcessingSemaphore -> Release();
}

void EMMediaProject::UnlockVideo()
{
	if(m_opVideoProcessingSemaphore != NULL)
		m_opVideoProcessingSemaphore -> Release();
}

void EMMediaProject::SetRenderingAudio(bool p_vFlag)
{
	if(m_opAudioClipRepository -> CountActiveClips() > 0)
	{
		m_vIsRenderingAudio = p_vFlag;
		if(m_vIsRenderingAudio)
		{
			//Check for clip with latest endingtime
			EMMediaClipRepository* opClipRepository = m_opAudioClipRepository;
			opClipRepository -> LockContainer();
			
			opClipRepository -> Rewind();
			while(opClipRepository -> Current() != NULL)
			{
				if((!opClipRepository -> Current() -> IsObjectDeleted()) && ((opClipRepository -> Current() -> GetType() & EM_TYPE_ANY_AUDIO) > 0))
				{
					int64 vPossibleNewEnding = opClipRepository -> Current() -> GetStop();
					if(m_vCurrentLatestEnding < vPossibleNewEnding)
						m_vCurrentLatestEnding = vPossibleNewEnding;
				}
				opClipRepository -> Next();
			}
			opClipRepository -> UnlockContainer();
		}
		else
		{
			m_vCurrentLatestEnding = 0;
		}
	}
	else
		m_vIsRenderingAudio = false;
}

bool EMMediaProject::IsRenderingAudio() const
{
	return m_vIsRenderingAudio;
}

void EMMediaProject::SetVideoWorkingSize(int32 p_vWidth, int32 p_vHeight)
{
	EMMediaEngine::Instance() -> GetSettingsRepository() -> SetSetting(SETTING_VIDEO_FORMAT_WIDTH, EM_SETTING_INT, EM_new int(p_vWidth), false);
	EMMediaEngine::Instance() -> GetSettingsRepository() -> SetSetting(SETTING_VIDEO_FORMAT_HEIGHT, EM_SETTING_INT, EM_new int(p_vHeight), false);
	EMMediaEngine::Instance() -> GetSettingsRepository() -> Notify(EM_MEDIA_ENGINE_VIDEO_WORKSIZE_CHANGED);
}

int64 EMMediaProject::GetProjectEndTimeInFrames()
{
	return m_vCurrentLatestEnding;
}

void EMMediaProject::StopVideoAudioRenderingAtOnce()
{
	m_vIsRenderingVideo = false;
	m_vIsRenderingAudio = false;
}


void EMMediaProject::SetRenderingVideo(bool p_vFlag)
{
	if(m_opVideoClipRepository -> CountActiveClips() > 0)
	{

		m_vIsRenderingVideo = p_vFlag;
		EMMediaEngine::Instance() -> GetSettingsRepository() -> Notify(EM_MEDIA_ENGINE_VIDEO_WORKSIZE_CHANGED);

		if(m_vIsRenderingVideo)
		{
			//Check for clip with latest endingtime
			EMMediaClipRepository* opClipRepository = m_opVideoClipRepository;
			opClipRepository -> LockContainer();
			
			opClipRepository -> Rewind();
			while(opClipRepository -> Current() != NULL)
			{
				if((!opClipRepository -> Current() -> IsObjectDeleted()) && ((opClipRepository -> Current() -> GetType() & EM_TYPE_ANY_VIDEO) > 0 || (opClipRepository -> Current() -> GetType() & EM_TYPE_BMP) > 0))
				{
					int64 vPossibleNewEnding = opClipRepository -> Current() -> GetStop();
					if(m_vCurrentLatestEnding < vPossibleNewEnding)
						m_vCurrentLatestEnding = vPossibleNewEnding;
				}
				opClipRepository -> Next();
			}
			opClipRepository -> UnlockContainer();
		}
		else
		{
			m_vCurrentLatestEnding = 0;
		}
	}
	else
		m_vIsRenderingVideo = false;

/*	if(p_vFlag)
	{
		if(! vFrameRateChangedSwitch)
		{
			//m_vVideoWorkingFrameRate = *(static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_RENDER_FRAMERATE)));
			vFrameRateChangedSwitch = true;
		}

		float vVideoRenderFrameRate = *(static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_RENDER_FRAMERATE)));
		
		//TODO: POSSIBLE MEMORY LEAK!!!!!!!!!!!!!!!!!!! FIX?
		EMMediaEngine::Instance() -> GetSettingsRepository() -> SetSetting(SETTING_VIDEO_FRAMERATE, EM_SETTING_FLOAT, EM_new float(vVideoRenderFrameRate), true);
	}
	else
	{
		if(vFrameRateChangedSwitch)
		{
			//TODO: POSSIBLE MEMORY LEAK!!!!!!!!!!!!!!!!!!! FIX?
			EMMediaEngine::Instance() -> GetSettingsRepository() -> SetSetting(SETTING_VIDEO_FRAMERATE, EM_SETTING_FLOAT, EM_new float(m_vVideoWorkingFrameRate), true);
			vFrameRateChangedSwitch = false;
		}
	}
	*/
}

bool EMMediaProject::IsRenderingVideo() const
{
	
	return m_vIsRenderingVideo;
}

EMMediaPool* EMMediaProject::GetMediaPool() const
{
	return m_opMediaPool;
}

bool EMMediaProject::IsDirty()
{
	return m_vIsDirty;
}

void EMMediaProject::SetDirty(bool p_vFlag)
{
	m_vIsDirty = p_vFlag;
}

bool EMMediaProject::SaveData(EMProjectDataSaver* p_opSaver)
{
	/**************************************************************************************
						
						  WHAT TO SAVE HERE

	1	Media pool and all entry information you can think of
		a)	Each pool entry (type, filename, instance count, id, etc etc etc)

	2	The two track repositories (used- and unused tracks)
		a)	Information about each track
		b)	Where's the track connected
		c)	Which clips belong in the track?
		d)	Pan/Channel/Fader/In/Out information for each track

	3	Audio clip repository
		a)	Don't forget to save if the audio clip is connected to an effect track!

	4	Video clip repository
	5	Video transition clip repository
	6	MIDI clip repository
		a)	All MIDI clips and information about which MIDI events it holds
		
	7	MIDI event repository
		a)	All events
		b)	Information about which media clip the events belong to
		c)	Starting-time information, in relation to the clip start

	8	Effect track repository
		a)	All instantiated effect information for each track, for each effect
		b)	Information about the track itself (where it's connected, from who, its name, etc)

	9	Project state information?
		a)	Possibly if the project is Armed for Recording

	**************************************************************************************/

	m_opVideoTransitionTrackRepository -> SaveData(p_opSaver);
	m_opEffectTrackRepository -> SaveData(p_opSaver);
	m_opMediaPool -> SaveData(p_opSaver);

	m_opUnusedTracks -> SaveData(p_opSaver);
	m_opUsedTracks -> SaveData(p_opSaver);

	m_opMIDIEventRepository -> SaveData(p_opSaver);

	m_opVideoTransitionTrackRepository -> RestoreAfterLoad();
	return true;
}

bool EMMediaProject::LoadData(EMProjectDataLoader* p_opLoader)
{
	m_opVideoTransitionTrackRepository -> LoadData(p_opLoader);
	m_opEffectTrackRepository -> LoadData(p_opLoader);

	m_opMediaPool -> LoadData(p_opLoader);
	m_opUnusedTracks -> LoadData(p_opLoader);
	m_opUsedTracks -> LoadData(p_opLoader);
	m_opMIDIEventRepository -> LoadData(p_opLoader);

	m_opEffectTrackRepository -> RestoreAfterLoad(); 
	m_opVideoTransitionTrackRepository -> RestoreAfterLoad();
	return true;
}

bool EMMediaProject::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{ 
		case EM_MESSAGE_BEGIN_BUFFER_UP:
		{
			if(IsArmed())
				this -> StartRecordingE();
			else
				this -> StartPlaybackE();
			return true; 
		}
		case EM_MESSAGE_BEGIN_PLAYORRECORD:
		{
			return true; 
		}
		case EM_MESSAGE_STOP_PLAYORRECORD:
		{
			if(IsArmed()) 
				this -> StopRecordingE();
			else
				this -> StopPlaybackE();
			return true;
		}
		case EM_QUIT_PHASE_ONE:
			return false;
		case EM_QUIT_PHASE_TWO:
			break;
		case EM_QUIT_PHASE_THREE:
			return ClearThreads();
		case EM_QUIT_PHASE_FOUR:
			break;
		case EM_QUIT_PHASE_FIVE:
			break;
		case EM_QUIT_PHASE_SIX:
			break;
		case EM_QUIT_ABORTED:
			break;
		default:
			break;
	};
	return false;
}