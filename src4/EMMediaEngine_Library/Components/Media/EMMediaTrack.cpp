#include "EMMediaTrack.h"
#include "EMMediaClip.h"
#include "EMOutputRepository.h"
#include "EMMediaDataBuffer.h"
#include "EMAudioClipMixer.h"
#include "EMMediaSignalMeterRepository.h"
#include "EMMediaSignalMeter.h"
#include "EMSilentAudioSource.h"
#include "EMSilentVideoSource.h"
#include "EMMediaTimer.h"
#include "EMSemaphore.h"
#include "EMMediaEffectTrackRepository.h"
#include "EMGlobals.h"
#include "EMMediaProject.h"
#include "EMVideoClip.h"
#include "EMAudioClip.h"
#include "EMImageClip.h"
#include "EMMIDIClip.h"
#include "EMMediaVideoTransitionClip.h"
#include "EMSettingIDs.h" 
#include "EMSettingsRepository.h"
#include "EMMediaMIDITrack.h"
#include "EMMediaEngine.h"
#include "EMMediaClipRepository.h"
#include "EMFileInputDescriptor.h"
#include "EMRealtimeInputDescriptor.h"
#include "EMProjectDataLoader.h"
#include "EMProjectDataSaver.h"
#include "EMMediaVideoTransitionTrack.h"
#include "EMCommandRepository.h"
#include "CommandIDs.h" 
#include "EMRubberBandNode.h"

EMMediaTrack* EMMediaTrack::CreateEMTrack(EMMediaType p_eType, string p_oName, uint32 p_vPrio)
{
	if((p_eType & EM_TYPE_MIDI) > 0)
		return EM_new EMMediaMIDITrack(p_oName);
	else if((p_eType & EM_TYPE_TRANSITION) > 0)
		return EM_new EMMediaVideoTransitionTrack(p_oName);
	else
		return new EMMediaTrack(p_eType, p_oName, p_vPrio);
}

EMMediaTrack::EMMediaTrack(EMMediaType p_eType, string p_oName = "Untitled track", uint32 p_vPrio)
	:	EMMediaBufferSource(p_eType),
		m_eType(p_eType),
		m_opAudioClipRepository(NULL),
		m_opVideoClipRepository(NULL),
		m_opVideoTransitionClipRepository(NULL),
		m_vFaderValue(100),
		m_vPanValue(0),
		m_vOffset(0),
		m_eMuteState(EM_MUTE_OFF),
		m_vSoloed(false),
		m_vIsArmed(false),
		m_vIsRender(false),
		m_oTrackName(p_oName),
		m_opSilenceGenerator(NULL),
		m_opClipMixer(NULL),
		m_opSignalMeter(NULL),
		m_opTrackDataReceiver(NULL),
		m_opRealtimeInput(NULL),
		m_vBuffersInit(false),
		m_vIsDeleted(false),
		m_opVolumeNodeObject(NULL),
		m_opPanNodeObject(NULL),
		m_vNumberOfVolumeNodeObjects(0),
		m_vNumberOfPanNodeObjects(0)

//		m_opVideoPreview(NULL)
{
	if((m_eType & EM_TYPE_ANY_AUDIO) > 0)
	{
		m_opSignalMeter = EMMediaSignalMeterRepository::Instance() -> InstanceSignalMeter(m_eType);
		m_opClipMixer = EM_new EMAudioClipMixer(GetID());
		m_opClipMixer -> SetDestination(m_opSignalMeter);
		m_opClipMixer -> PrepareToPlayE();
		m_opSilenceGenerator = EM_new EMSilentAudioSource();

		//Rubberband audio node objects (Static at the moment for pan and volume)
		m_opVolumeNodeObject = EM_new list<EMRubberBandNode*>();
		m_opPanNodeObject = EM_new list<EMRubberBandNode*>();

	} 
	else if((m_eType & EM_TYPE_ANY_VIDEO) > 0)
	{
		m_opSilenceGenerator = EM_new EMSilentVideoSource();
		static_cast<EMSilentVideoSource*>(m_opSilenceGenerator) -> SetTrack(this);
		//m_opVideoPreview = static_cast<EMVideoPreview*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_PREVIEW));
		//m_opVideoPreview -> AddListener(this);
		EMMediaEngine::Instance() -> GetSettingsRepository() -> AddListener(this);

	}
	else if((m_eType & EM_TYPE_MIDI) > 0)
	{
		m_opSignalMeter = EMMediaSignalMeterRepository::Instance() -> InstanceSignalMeter(EM_TYPE_MIDI);
		m_opSilenceGenerator = NULL;
	}

	m_vPriorityID = p_vPrio;
	m_opDestinationSemaphore = EMSemaphore::CreateEMSemaphore();
	EMMediaTimer::Instance() -> AddListener(this);
}

EMMediaTrack::~EMMediaTrack()
{
	EMMediaTimer::Instance() -> RemoveListener(this);
	if((GetType() & EM_TYPE_ANY_VIDEO) > 0)
	{
		//if(m_opVideoPreview != NULL)
			//m_opVideoPreview -> RemoveListener(this);
		EMMediaEngine::Instance() -> GetSettingsRepository() -> RemoveListener(this);
	}
	delete dynamic_cast<EMAudioClipMixer*>(m_opClipMixer);
	delete m_opSilenceGenerator;
	delete m_opDestinationSemaphore;
}

void EMMediaTrack::DeleteRubberBandNode(EMRubberBandNode* p_opRubberBandNode, int p_vParam)
{
	switch(p_vParam)
	{
		case 1:
			{

				m_opVolumeNodeObject -> remove(p_opRubberBandNode);
				delete p_opRubberBandNode;
				m_vNumberOfVolumeNodeObjects--; //Decrease node counter for volume

			}
			break;

		case 2:
			{
				m_opPanNodeObject -> remove(p_opRubberBandNode);
				delete p_opRubberBandNode;
				m_vNumberOfPanNodeObjects++; //Decrease node counter for volume
			}
			break;
	}

}

void EMMediaTrack::MoveRubberBandNode(EMRubberBandNode* p_opRubberBandNode, int64 p_vPosAndValue[2])
{
	p_opRubberBandNode -> SetPosition(p_vPosAndValue[0]);
	p_opRubberBandNode -> SetValue(p_vPosAndValue[1]);

	//Fast hack to reset fast calculation in GetRubberBandValue(...) (Not thread safe)
	m_vLowerPositionVolume = -1;
	m_vHigherPositionVolume = -1;
	m_vLowerPositionPan = -1;
	m_vHigherPositionPan = -1;
	//End fast hack

}

list<EMRubberBandNode*>* EMMediaTrack::GetRubberBandNodeObject(int p_vParam)
{
	switch(p_vParam)
	{
		case 1:
			{
				return m_opVolumeNodeObject;
			}
			break;

		case 2:
			{
				return m_opPanNodeObject;
			}
			break;
	}

	return NULL;
}


EMRubberBandNode* EMMediaTrack::AddRubberBandNode(int64* p_vPosAndValue, int p_vParam)
{
	switch(p_vParam)
	{
		case 1:
			{
				EMRubberBandNode* opVolumeNode = EM_new EMRubberBandNode();
				opVolumeNode -> SetPosition(p_vPosAndValue[0]);
				opVolumeNode -> SetValue(p_vPosAndValue[1]);

				list<EMRubberBandNode*>::iterator oIterator = m_opVolumeNodeObject -> begin();
				m_vNumberOfVolumeNodeObjects++; //Increase nodeobject counter for volume
				if(oIterator == m_opVolumeNodeObject -> end())
				{
					m_opVolumeNodeObject -> insert(oIterator, opVolumeNode);
				}
				while(oIterator != m_opVolumeNodeObject -> end())
				{
					if(p_vPosAndValue[0] >= (*oIterator) ->GetPosition())
					{
						oIterator++;
						if(oIterator == m_opVolumeNodeObject -> end())
						{
							m_opVolumeNodeObject -> insert(oIterator, opVolumeNode);
							break;
						}
						if(p_vPosAndValue[0] <= (*oIterator) ->GetPosition())
						{
							m_opVolumeNodeObject -> insert(oIterator, opVolumeNode);
							break;
						}
					}
					else if(p_vPosAndValue[0] < (*oIterator) ->GetPosition())
					{
						m_opVolumeNodeObject -> insert(oIterator, opVolumeNode);
						break;
					} 
					else
						oIterator++;
				}

				return opVolumeNode;
			}
			break;

		case 2:
			{
				EMRubberBandNode* opPanNode = EM_new EMRubberBandNode();
				opPanNode -> SetPosition(p_vPosAndValue[0]);
				opPanNode -> SetValue(p_vPosAndValue[1]);

				list<EMRubberBandNode*>::iterator oIterator = m_opPanNodeObject -> begin();
				m_vNumberOfPanNodeObjects++; //Increase nodeobject counter for panning
				if(oIterator == m_opPanNodeObject -> end())
				{
					m_opPanNodeObject -> insert(oIterator, opPanNode);
				}
				while(oIterator != m_opPanNodeObject -> end())
				{
					if(p_vPosAndValue[0] >= (*oIterator) ->GetPosition())
					{
						oIterator++;
						if(oIterator == m_opPanNodeObject -> end())
						{
							m_opPanNodeObject -> insert(oIterator, opPanNode);
							break;
						}
						if(p_vPosAndValue[0] <= (*oIterator) ->GetPosition())
						{
							m_opPanNodeObject -> insert(oIterator, opPanNode);
							break;
						}
					}
					else if(p_vPosAndValue[0] < (*oIterator) ->GetPosition())
					{
						m_opPanNodeObject -> insert(oIterator, opPanNode);
						break;
					}
					else
						oIterator++;
				}

				return opPanNode;
			}
			break;

	}

	return NULL;
}

//myVolume = GetRubberBandValue(1000000

int32 EMMediaTrack::GetRubberBandValue(int64 p_vPosition, int p_vParam)
{
	if(p_vParam == 1)
	{
		if(p_vPosition >= m_vLowerPositionVolume && p_vPosition <= m_vHigherPositionVolume)
		{
			float vVolumeValueMultiplier = ((float)m_vLowerValueVolume - (float)m_vHigherValueVolume) / ((float)m_vLowerPositionVolume - (float)m_vHigherPositionVolume);
			int32 vReturnValue =static_cast<int32>(vVolumeValueMultiplier*((float)p_vPosition) + (float)m_vLowerValueVolume - vVolumeValueMultiplier*(float)m_vLowerPositionVolume);

			return vReturnValue;
		}

		list<EMRubberBandNode*>::iterator oIterator = m_opVolumeNodeObject -> begin();
		while(oIterator != m_opVolumeNodeObject -> end())
		{
			if(p_vPosition >= (*oIterator) ->GetPosition())
			{
				m_vLowerPositionVolume = (*oIterator) -> GetPosition();
				m_vLowerValueVolume = (*oIterator) -> GetValue();
				oIterator++;
				if(oIterator == m_opVolumeNodeObject -> end())
				{
					return m_vLowerValueVolume;
					break;
				}
				if(p_vPosition <= (*oIterator) ->GetPosition())
				{
					m_vHigherPositionVolume = (*oIterator) -> GetPosition();
					m_vHigherValueVolume = (*oIterator) -> GetValue();

					float vVolumeValueMultiplier = ((float)m_vLowerValueVolume - (float)m_vHigherValueVolume) / ((float)m_vLowerPositionVolume - (float)m_vHigherPositionVolume);
					int32 vReturnValue =static_cast<int32>(vVolumeValueMultiplier*((float)p_vPosition) + (float)m_vLowerValueVolume - vVolumeValueMultiplier*(float)m_vLowerPositionVolume);

					return vReturnValue;
					break;
				}
			}
			else if(p_vPosition < (*oIterator) ->GetPosition())
			{
				m_vHigherPositionVolume = -1;
				m_vHigherValueVolume = -1;
				m_vLowerPositionVolume = (*oIterator) -> GetPosition();
				m_vLowerValueVolume = (*oIterator) -> GetValue();
				return m_vLowerValueVolume;
				break;
			} 
			else
				oIterator++;
		}

		return -1;
	}
	else if(p_vParam == 2)
	{
		if(p_vPosition >= m_vLowerPositionPan && p_vPosition <= m_vHigherPositionPan)
		{
			float vPanValueMultiplier = ((float)m_vLowerValuePan - (float)m_vHigherValuePan) / ((float)m_vLowerPositionPan - (float)m_vHigherPositionPan);
			int32 vReturnValue =static_cast<int32>(vPanValueMultiplier*((float)p_vPosition) + (float)m_vLowerValuePan - vPanValueMultiplier*(float)m_vLowerPositionPan);

			return vReturnValue;
		}
		list<EMRubberBandNode*>::iterator oIterator = m_opPanNodeObject -> begin();
		while(oIterator != m_opPanNodeObject -> end())
		{
			if(p_vPosition >= (*oIterator) ->GetPosition())
			{
				m_vLowerPositionPan = (*oIterator) -> GetPosition();
				m_vLowerValuePan = (*oIterator) -> GetValue();
				oIterator++;
				if(oIterator == m_opPanNodeObject -> end())
				{
					return m_vLowerValuePan;
					break;
				}
				if(p_vPosition <= (*oIterator) ->GetPosition())
				{
					m_vHigherPositionPan = (*oIterator) -> GetPosition();
					m_vHigherValuePan = (*oIterator) -> GetValue();
					float vPanValueMultiplier = ((float)m_vLowerValuePan - (float)m_vHigherValuePan) / ((float)m_vLowerPositionPan - (float)m_vHigherPositionPan);
					int32 vReturnValue =static_cast<int32>(vPanValueMultiplier*((float)p_vPosition) + (float)m_vLowerValuePan - vPanValueMultiplier*(float)m_vLowerPositionPan);

					return vReturnValue;
					break;
				}
			}
			else if(p_vPosition < (*oIterator) ->GetPosition())
			{
				m_vHigherPositionPan = -1;
				m_vHigherValuePan = -1;
				m_vLowerPositionPan = (*oIterator) -> GetPosition();
				m_vLowerValuePan = (*oIterator) -> GetValue();
				return m_vLowerValuePan;
				break;
			} 
			else
				oIterator++;
		}
		return -1;

	}

	return -1;
}

bool EMMediaTrack::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	
	if(p_vMessage == EM_SETTING_UPDATED)
	{
		EMMediaFormat oMediaFormat(EM_TYPE_RAW_VIDEO);
		oMediaFormat.m_vWidth = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FORMAT_WIDTH)));//(m_opVideoPreview -> Bounds()).GetHeight();
		oMediaFormat.m_vHeight  = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FORMAT_HEIGHT)));//(m_opVideoPreview -> Bounds()).GetWidth();
		oMediaFormat.m_vBytesPerSample = 3;
		oMediaFormat.m_vFrameRate = *(static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FRAMERATE)));

		//while(((oMediaFormat.m_vHeight * oMediaFormat.m_vBytesPerSample) % 4) != 0)
		//	oMediaFormat.m_vHeight--;

		if(m_vOldWidth != oMediaFormat.m_vWidth || m_vOldheight != oMediaFormat.m_vHeight || m_vOldFrameRate != oMediaFormat.m_vFrameRate)
		{
			oMediaFormat.m_vBytesPerRow  = oMediaFormat.m_vWidth*3;
			EMMediaBufferSource::InitCheckBufferE(&oMediaFormat);
			m_vBuffersInit = true;
		}
		m_vOldWidth = oMediaFormat.m_vWidth;
		m_vOldheight = oMediaFormat.m_vHeight;
		m_vOldFrameRate = oMediaFormat.m_vFrameRate;
	}
	else if(p_vMessage == EM_VIEW_BOUNDS_CHANGED)
	{
		return true;
	}
	else if(p_vMessage == EM_MEDIA_ENGINE_VIDEO_WORKSIZE_CHANGED)
	{
		if((GetType() & EM_TYPE_ANY_VIDEO) > 0)
		{
			//POSSIBLE MEMORY LEAK
			EMMediaFormat oMediaFormat(EM_TYPE_RAW_VIDEO);
			if(EMMediaEngine::Instance() -> GetMediaProject() -> IsRenderingVideo())
			{
				oMediaFormat.m_vWidth = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_RENDER_FORMAT_WIDTH)));//(m_opVideoPreview -> Bounds()).GetHeight();
				oMediaFormat.m_vHeight  = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_RENDER_FORMAT_HEIGHT)));//(m_opVideoPreview -> Bounds()).GetWidth();
				oMediaFormat.m_vBytesPerSample = 3;
				oMediaFormat.m_vFrameRate = *(static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_RENDER_FRAMERATE)));
				
				//while(((oMediaFormat.m_vHeight * oMediaFormat.m_vBytesPerSample) % 4) != 0)
				//	oMediaFormat.m_vHeight--;
				
				if(m_vOldWidth != oMediaFormat.m_vWidth || m_vOldheight != oMediaFormat.m_vHeight || m_vOldFrameRate != oMediaFormat.m_vFrameRate )
				{
					oMediaFormat.m_vBytesPerRow  = oMediaFormat.m_vWidth*3;
					EMMediaBufferSource::InitCheckBufferE(&oMediaFormat);
					m_vBuffersInit = true;
				}
			}
			else
			{
				oMediaFormat.m_vWidth = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FORMAT_WIDTH)));//(m_opVideoPreview -> Bounds()).GetHeight();
				oMediaFormat.m_vHeight  = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FORMAT_HEIGHT)));//(m_opVideoPreview -> Bounds()).GetWidth();
				oMediaFormat.m_vBytesPerSample = 3;
				oMediaFormat.m_vFrameRate = *(static_cast<float*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FRAMERATE)));

				//while(((oMediaFormat.m_vHeight * oMediaFormat.m_vBytesPerSample) % 4) != 0)
				//	oMediaFormat.m_vHeight--;


				if(m_vOldWidth != oMediaFormat.m_vWidth || m_vOldheight != oMediaFormat.m_vHeight || m_vOldFrameRate != oMediaFormat.m_vFrameRate )
				{
					oMediaFormat.m_vBytesPerRow  = oMediaFormat.m_vWidth*3;
					EMMediaBufferSource::InitCheckBufferE(&oMediaFormat);
					m_vBuffersInit = true;
				}
			}
			m_vOldWidth = oMediaFormat.m_vWidth;
			m_vOldheight = oMediaFormat.m_vHeight;
			m_vOldFrameRate = oMediaFormat.m_vFrameRate;
		}
		return true; 
	}
	return false;
}

uint32 EMMediaTrack::GetPriority() const
{
	return m_vPriorityID;
}

void EMMediaTrack::SetPriority(uint32 p_vPrio)
{
	m_vPriorityID = p_vPrio;
}


EMMediaType EMMediaTrack::GetType() const
{
	return m_eType;
}

bool EMMediaTrack::InitCheckE()
{
	if(EMMediaEngine::Instance() -> GetMediaProject() == NULL)
	{
		eo << "ERROR! Sorry, but no project exists in memory, so the track can't be initialized!" << ef;
		return false;
	}
		
	m_opAudioClipRepository = EMMediaEngine::Instance() -> GetMediaProject() -> GetAudioClipRepository();
	m_opVideoClipRepository = EMMediaEngine::Instance() -> GetMediaProject() -> GetVideoClipRepository();
	m_opVideoTransitionClipRepository = EMMediaEngine::Instance() -> GetMediaProject() -> GetVideoTransitionClipRepository();
	m_opMIDIClipRepository = EMMediaEngine::Instance() -> GetMediaProject() -> GetMIDIClipRepository();

	if(m_opAudioClipRepository == NULL)
		EMDebugger("ERROR! Audio clip repository is NULL!");

	if(m_opVideoClipRepository == NULL)
		EMDebugger("ERROR! Video clip repository is NULL!");

	if(m_opVideoTransitionClipRepository == NULL)
		EMDebugger("ERROR! Video clip repository is NULL!");

	if(! m_opAudioClipRepository -> InitCheckE())
		return false;

	if(! m_opVideoClipRepository -> InitCheckE())
		return false;

	return true;
}

//This is only for video clips, and is currently done when a clip is moved to an empty
//track which buffers have not yet been initialized
void EMMediaTrack::InitializeTrackBuffers()
{
	if(!m_vBuffersInit)
	{
		EMMediaFormat oMediaFormat(GetType());
		oMediaFormat.m_vWidth = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FORMAT_WIDTH)));//(m_opVideoPreview -> Bounds()).GetHeight();
		oMediaFormat.m_vHeight  = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FORMAT_HEIGHT)));//(m_opVideoPreview -> Bounds()).GetWidth();
		while(((oMediaFormat.m_vHeight * oMediaFormat.m_vBytesPerSample) % 4) != 0)
			oMediaFormat.m_vHeight--;
		oMediaFormat.m_vBytesPerSample = 3;
		oMediaFormat.m_vBytesPerRow  = oMediaFormat.m_vWidth*3;
		EMMediaBufferSource::InitCheckBufferE(&oMediaFormat);
	}

}

int32 EMMediaTrack::CreateClip(int64 p_vAtFrame, string p_oName, EMFileInputDescriptor* p_opSource) //TODO: Make non-native
{
	EMMediaClip* opNewClip = NULL;
	try
	{
		if ((GetType() & EM_TYPE_ANY_VIDEO) > 0 && !m_vBuffersInit)
		{
			//EMVideoPreview* opVideoPreview = static_cast<EMVideoPreview*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_PREVIEW));
			EMMediaFormat oMediaFormat(GetType());
			oMediaFormat.m_vWidth = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FORMAT_WIDTH)));//(m_opVideoPreview -> Bounds()).GetHeight();
			oMediaFormat.m_vHeight  = *(static_cast<int*>(EMMediaEngine::Instance() -> GetSettingsRepository() -> GetSetting(SETTING_VIDEO_FORMAT_HEIGHT)));//(m_opVideoPreview -> Bounds()).GetWidth();
			while(((oMediaFormat.m_vHeight * oMediaFormat.m_vBytesPerSample) % 4) != 0)
				oMediaFormat.m_vHeight--;
			oMediaFormat.m_vBytesPerSample = 3;
			oMediaFormat.m_vBytesPerRow  = oMediaFormat.m_vWidth*3;
		
			EMMediaBufferSource::InitCheckBufferE(&oMediaFormat);
			m_vBuffersInit = true;
		}


		if ((p_opSource -> GetType() & EM_TYPE_ANY_VIDEO) > 0)
			opNewClip = EM_new EMVideoClip(m_eType, p_oName.c_str(), p_vAtFrame, this);
		else if((p_opSource -> GetType() & EM_TYPE_ANY_AUDIO) > 0)
			opNewClip = EM_new EMAudioClip(m_eType, p_oName.c_str(), p_vAtFrame, this);
		else if((p_opSource -> GetType() & EM_TYPE_ANY_IMAGE) >0)
			opNewClip = EM_new EMImageClip(p_opSource -> GetType(), p_oName.c_str(), p_vAtFrame, this);
		else
			MessageBox(NULL,"Wrong media","EMMediaTrack::CreateClip() - ERROR!!",MB_OK);

		if((p_opSource -> GetType() & EM_TYPE_ANY_IMAGE) == 0)
		{
			opNewClip -> SetMarkInLength(0);
			opNewClip -> SetMarkOutLength(0);
		}

		opNewClip -> SetDescriptor(p_opSource);
		opNewClip -> SetStart(p_vAtFrame);
		
		AddListener(opNewClip);

		if(p_opSource -> GetNumFrames() <= 0)
			return -1;
		
		if((m_eType & EM_TYPE_ANY_AUDIO) > 0)
			//We should be able to mix overlapping clips if it's an audio-track (so connect it to clipmixer).
			opNewClip -> SetDestination(m_opClipMixer);
		else if((m_eType & EM_TYPE_ANY_VIDEO) > 0)
			//We don't need to "mix" overlapping videoclips in case of video-track. So, connect it directly to the track's "real" output
			opNewClip -> SetDestination(m_opTrackDataReceiver);
		else if((m_eType & EM_TYPE_MIDI) > 0)
		{
			eo << "ERROR! EMMediaTrack::CreateClip not yet implemented!" << ef;
		}
		else
			EMDebugger("ERROR! In EMMediaTrack::CreateClip() - Unknown type!");
			
		if(! opNewClip -> InitCheckE())
		{
			RemoveListener(opNewClip);
			delete opNewClip;
			return -1;
		}
		
		if((opNewClip -> GetType() & EM_TYPE_ANY_AUDIO) > 0)
		{
			m_opAudioClipRepository -> LockContainer();
			try
			{
				m_opAudioClipRepository -> Add(opNewClip);
			}
			catch(...)
			{
				EMDebugger("ERROR! Caught an exception in EMMediaTrack::CreateClip (1)");
			}
			m_opAudioClipRepository -> UnlockContainer();
		}
		else if((opNewClip -> GetType() & EM_TYPE_ANY_VIDEO) > 0 || (opNewClip -> GetType() & EM_TYPE_ANY_IMAGE) > 0)
		{
			m_opVideoClipRepository -> LockContainer();
			try
			{
				m_opVideoClipRepository -> Add(opNewClip);
			}
			catch(...)
			{
				EMDebugger("ERROR! Caught an exception in EMMediaTrack::CreateClip (2)");
			}
			m_opVideoClipRepository -> UnlockContainer();
		}
		else
			EMDebugger("!");
	}
	catch(...)
	{
		delete opNewClip;
		return -1;
	}
	return opNewClip -> GetID();
}

//Audio specific. Haven't really decided wether or not to 
//split the track into a base-class and two/three children, for audio,
//video (and MIDI?) ... Or if we should keep it in one class and
//just ignore some methods depending on the format.
void EMMediaTrack::SetFader(int32 p_vFaderValue)
{
	if(p_vFaderValue > 127)
		m_vFaderValue = 127;
	else if(p_vFaderValue < 0)
		m_vFaderValue = 0;
	else
		m_vFaderValue = p_vFaderValue;
}

void EMMediaTrack::SetPan(int32 p_vPanValue)
{
	if(p_vPanValue > 63)
		m_vPanValue = 63;
	else if(p_vPanValue < -63)
		m_vPanValue = -63;
	else
		m_vPanValue = p_vPanValue;
}

int32 EMMediaTrack::GetFader() const
{
	return m_vFaderValue;
}

int32 EMMediaTrack::GetPan() const
{
	return m_vPanValue;
}

EMBufferDestination* EMMediaTrack::GetDestination() const
{
	return GetClipDataDestination();
}

EMBufferDestination* EMMediaTrack::GetTrackDestination() const
{
	m_opDestinationSemaphore -> Acquire();
	EMBufferDestination* opDestination = m_opTrackDataReceiver;
	m_opDestinationSemaphore -> Release();
	return opDestination;
}

EMBufferDestination* EMMediaTrack::GetClipDataDestination() const
{
	if((m_eType & EM_TYPE_ANY_AUDIO) > 0)
		return m_opClipMixer;
	else if((m_eType & EM_TYPE_ANY_VIDEO) > 0)
		return m_opTrackDataReceiver;
	else if((m_eType & EM_TYPE_MIDI) > 0)
		return m_opTrackDataReceiver; //The track's destination is also each MIDI clip's destination
	else if((m_eType & EM_TYPE_TRANSITION) > 0)
		return m_opTrackDataReceiver;

	return m_opSignalMeter;
}

//The destination here is where the actual output of the track should
//be sent. I.e. this is AFTER the overlapping-clip mixer has done it's
//work, so really a track has two destinations:
//First the clip mixer, and then the actual output.

/*
	AUDIO TRACK CONNECTION SCHEME
	
 			|---------------------- Inside the EMMediaTrack --------------------|--- Outside the track --- - -

  			  Clip		---->		Clip mixer 		---->		Signal meter   ----> Track data receiver - - - > Effects, Physical output / render output, etc
			  Silence	----> ----'^


	VIDEO TRACK CONNECTION SCHEME
			
			|---------------------- Inside the EMMediaTrack --------------------|--- Outside the track --- - -

			  Clip		---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----> Track data receiver - - - > Effects, Physical output / render output, etc
			  Silence	----------'^
*/

void EMMediaTrack::SetTrackDestination(EMBufferDestination* p_opTrackDataReceiver)
{
	if(p_opTrackDataReceiver != NULL)
	{
		m_opDestinationSemaphore -> Acquire();
		m_opTrackDataReceiver = p_opTrackDataReceiver;

		//If this is an audio-track we should be able to mix overlapping clips. The output of the overlapclip-mixer should then be set to the "real" destination.
		if((m_eType & EM_TYPE_ANY_AUDIO) > 0)
		{
			if(m_opTrackDataReceiver != NULL)
				m_opSignalMeter -> SetDestination(m_opTrackDataReceiver);
		}
		m_opDestinationSemaphore -> Release();
	}
	else EMDebugger("ERROR! Tried to set NULL as a track's destination!");
 
	//If this is an audio track (ie there's a clip mixer)
	if(m_opClipMixer != NULL)
		m_opSilenceGenerator -> SetDestination(m_opClipMixer);
	else if((GetType() & EM_TYPE_ANY_VIDEO) > 0)
		m_opSilenceGenerator -> SetDestination(m_opTrackDataReceiver);
	else if((GetType() & EM_TYPE_ANY_AUDIO) > 0)
		m_opSilenceGenerator -> SetDestination(m_opTrackDataReceiver);

	if((GetType() & EM_TYPE_TRANSITION)==0)
	{
		if(GetType() != EM_TYPE_MIDI)
		{
			EMMediaFormat* opFormat = EM_new EMMediaFormat(m_eType);
			if(GetType() != EM_TYPE_ANY_AUDIO)
				m_opSilenceGenerator -> InitCheckBufferE(opFormat);
			m_opSilenceGenerator -> InitCheckE(opFormat);
			delete opFormat;
		}
	}

	//Notify our listeners (eg the clips within this track) that our destination has changed!	
	Notify(EM_MESSAGE_TRACK_DEST_CHANGE);
}

EMMuteState EMMediaTrack::GetMuteState() const
{
	return m_eMuteState;
}

void EMMediaTrack::SetMuteState(EMMuteState p_eFlag)
{
	m_eMuteState = p_eFlag;
}

bool EMMediaTrack::IsSoloed() const
{
	return m_vSoloed;
}

void EMMediaTrack::SetSoloed(bool p_vFlag)
{
	m_vSoloed = p_vFlag;
}

bool EMMediaTrack::IsArmed() const
{
	return m_vIsArmed;
}

void EMMediaTrack::SetArmed(bool p_vFlag)
{
	if(m_opRealtimeInput == NULL)
	{
		m_vIsArmed = p_vFlag;
		return;
	}
		
	m_vIsArmed = p_vFlag;
	
	m_opRealtimeInput -> RemoveRecorder(this);
	if(m_vIsArmed)
		m_opRealtimeInput -> AddRecorder(this);

	
	m_opRealtimeInput -> SetArmed(m_vIsArmed);
}

bool EMMediaTrack::IsRender() const
{
	return m_vIsRender;
}

void EMMediaTrack::SetRender(bool p_vFlag)
{
	if(p_vFlag)
		;//cout_commented_out_4_release << "Track " << GetID() << " is now RENDERING!" << endl;
	else
		;//cout_commented_out_4_release << "Track " << GetID() << " is now sending its data to a regular output (NOT rendering)!" << endl;
	m_vIsRender = p_vFlag;
}

string EMMediaTrack::GetName() const
{
	return m_oTrackName;
}

void EMMediaTrack::SetName(string p_oName)
{
	m_oTrackName = p_oName;
}

EMMediaBufferSource* EMMediaTrack::GetSilenceGenerator() const
{
	return m_opSilenceGenerator;
}

int64 EMMediaTrack::CountClips()
{
	int64 vCount = 0;
	vCount += m_opAudioClipRepository -> CountClipsForTrack(this);
	vCount += m_opVideoClipRepository -> CountClipsForTrack(this);
	return vCount;
}

EMMediaSignalMeter* EMMediaTrack::GetSignalMeter() const
{
	return m_opSignalMeter;
}

EMBufferDestination* EMMediaTrack::GetClipMixer() const
{
	return m_opClipMixer;
}

void EMMediaTrack::SetInput(EMRealtimeInputDescriptor* p_opInput) //Make non-native?
{
	if(p_opInput == NULL)
	{
		eo << "WARNING! EMMediaTrack was told to use NULL as input!" << ef;
		m_opRealtimeInput = NULL;
	}
	else
	{
/*		if(! (p_opInput -> InitCheckE()))
		{
			eo << "ERROR! Could not initialize realtime input for track!" << ef;	
			return;
		}*/
	}
	
	if(m_opRealtimeInput != NULL)
		m_opRealtimeInput -> RemoveRecorder(this);
		
	m_opRealtimeInput = p_opInput;
	if(IsArmed() && m_opRealtimeInput != NULL)
		m_opRealtimeInput -> AddRecorder(this);
}

EMRealtimeInputDescriptor* EMMediaTrack::GetInput() const
{
	return m_opRealtimeInput;
}

void EMMediaTrack::StartRecording()
{
//	if(! IsArmed())
//		return;
//	if(m_opRealtimeInput == NULL)
//		return;
//	m_opRealtimeInput -> PrepareToPlayE();
//	m_opRealtimeInput -> StartE();
}

void EMMediaTrack::StartPreview()
{
//	if(! IsArmed())
//		return;
//	
//	if(m_opRealtimeInput == NULL)
//	{
//		return;
//	}
//	m_opRealtimeInput -> PreviewE();
}

void EMMediaTrack::StopPreview()
{
//	if(m_opRealtimeInput == NULL)
//		return;
//	m_opRealtimeInput -> StopPreviewE();
}


void EMMediaTrack::StopRecording()
{
//	if(m_opRealtimeInput == NULL)
//		return;
//	m_opRealtimeInput -> StopE();
}

bool EMMediaTrack::LoadData(EMProjectDataLoader* p_opLoader)
{
	if(p_opLoader -> LoadString() != string("EMT"))
	{
		EMDebugger("ERROR! Error in project file!");
		return false;
	}

	m_vID = static_cast<int32>(p_opLoader -> LoadUInt32());
	m_vFaderValue = static_cast<int32>(p_opLoader -> LoadUInt32());
	m_vPanValue = static_cast<int32>(p_opLoader -> LoadUInt32());
	m_vOffset = static_cast<int64>(p_opLoader -> LoadUInt64());
	m_vPriorityID = static_cast<int32>(p_opLoader -> LoadUInt32());
	m_eMuteState = static_cast<EMMuteState>(p_opLoader -> LoadUInt32());
	m_vSoloed = p_opLoader -> LoadBool();
	m_vIsArmed = p_opLoader -> LoadBool();
	m_vIsRender = p_opLoader -> LoadBool();
	m_oTrackName = string(p_opLoader -> LoadString());

	EMMediaClipRepository* opAudioClips = EMMediaEngine::Instance() -> GetMediaProject() -> GetAudioClipRepository();
	EMMediaClipRepository* opVideoClips = EMMediaEngine::Instance() -> GetMediaProject() -> GetVideoClipRepository();
	EMMediaClipRepository* opMIDIClips = EMMediaEngine::Instance() -> GetMediaProject() -> GetMIDIClipRepository();
	EMMediaClipRepository* opVideoTransitionClips = EMMediaEngine::Instance() -> GetMediaProject() -> GetVideoTransitionClipRepository();

	uint64 vNumAudioClipsInTrack = p_opLoader -> LoadUInt64();
	uint64 vNumVideoClipsInTrack = p_opLoader -> LoadUInt64();
	uint64 vNumImageClipsInTrack = p_opLoader -> LoadUInt64();
	uint64 vNumMIDIClipsInTrack = p_opLoader -> LoadUInt64();
	uint64 vNumVideoTransitionClipsInTrack = p_opLoader -> LoadUInt64();

	for(uint64 vIndex = 0; vIndex < vNumAudioClipsInTrack; vIndex++)
	{
		EMMediaClip* opNewClip = EM_new EMAudioClip(this);
		if(! opNewClip -> LoadData(p_opLoader))
		{
			delete opNewClip;
			return false;
		}
		AddListener(opNewClip);

		if(opNewClip -> HasEffectTrack())
			dynamic_cast<EMMediaEffectTrack*>(opNewClip -> GetDestination()) -> SetOutput(GetClipDataDestination());
		else
			opNewClip -> SetDestination(GetClipDataDestination());

		opNewClip -> InitCheckE();
		opAudioClips -> LockContainer();
		try
		{
			opAudioClips -> Add(opNewClip);
		}
		catch(...)
		{
			EMDebugger("ERROR! Caught an exception in EMMediaTrack::LoadData (1)");
		}
		opAudioClips -> UnlockContainer();
	}

	for(vIndex = 0; vIndex < vNumVideoClipsInTrack; vIndex++)
	{
		EMMediaClip* opNewClip = EM_new EMVideoClip(this);
		if(! opNewClip -> LoadData(p_opLoader))
		{
			delete opNewClip;
			return false;
		}
		AddListener(opNewClip);
//		opNewClip -> SetDestination(GetClipDataDestination());
		opNewClip -> InitCheckE();
		opVideoClips -> LockContainer();
		try
		{
			opVideoClips -> Add(opNewClip);
		}
		catch(...)
		{
			EMDebugger("ERROR! Caught an exception in EMMediaTrack::LoadData (2)");
		}
		opVideoClips -> UnlockContainer();
	}

	for(vIndex = 0; vIndex < vNumImageClipsInTrack; vIndex++)
	{
		EMMediaClip* opNewClip = EM_new EMImageClip(this);
		if(! opNewClip -> LoadData(p_opLoader))
		{
			delete opNewClip;
			return false;
		}
		AddListener(opNewClip);
//		opNewClip -> SetDestination(GetClipDataDestination());
		opNewClip -> InitCheckE();
		opVideoClips -> LockContainer();
		try
		{
			opVideoClips -> Add(opNewClip);
		}
		catch(...)
		{
			EMDebugger("ERROR! Caught an exception in EMMediaTrack::LoadData (2b)");
		}
		opVideoClips -> UnlockContainer();
	}


	for(vIndex = 0; vIndex < vNumMIDIClipsInTrack; vIndex++)
	{
		EMMediaClip* opNewClip = EM_new EMMIDIClip(this);
		if(! opNewClip -> LoadData(p_opLoader))
		{
			delete opNewClip;
			return false;
		}
		AddListener(opNewClip);
		opNewClip -> SetDestination(GetClipDataDestination());
		opNewClip -> InitCheckE();
		opMIDIClips -> LockContainer();
		try
		{
			opMIDIClips -> Add(opNewClip);
		}
		catch(...)
		{
			EMDebugger("ERROR! Caught an exception in EMMediaTrack::LoadData (3)");
		}
		opMIDIClips -> UnlockContainer();
	}

	//Load Volume node objects
	m_vNumberOfVolumeNodeObjects = static_cast<int32>(p_opLoader -> LoadUInt32());

	list<EMRubberBandNode*>::iterator oIterator;
	
	if(m_opVolumeNodeObject != NULL)
		 oIterator = m_opVolumeNodeObject -> begin();

	for(int32 vIndexNodeCounter = 0; vIndexNodeCounter < m_vNumberOfVolumeNodeObjects; vIndexNodeCounter++)
	{

		int64 vPos = static_cast<int64>(p_opLoader -> LoadUInt64());
		int32 vVolume = static_cast<int32>(p_opLoader -> LoadUInt32());

		EMRubberBandNode* opVolumeNode = EM_new EMRubberBandNode();
		opVolumeNode -> SetPosition(vPos);
		opVolumeNode -> SetValue(vVolume);

		m_opVolumeNodeObject -> insert(oIterator, opVolumeNode);		
	}

	//Load pan node objects
	m_vNumberOfPanNodeObjects = static_cast<int32>(p_opLoader -> LoadUInt32());

	if(m_opPanNodeObject != NULL)
		oIterator = m_opPanNodeObject -> begin();
	for(vIndexNodeCounter = 0; vIndexNodeCounter < m_vNumberOfPanNodeObjects; vIndexNodeCounter++)
	{

		int64 vPos = static_cast<int64>(p_opLoader -> LoadUInt64());
		int32 vPan = static_cast<int32>(p_opLoader -> LoadUInt32());

		EMRubberBandNode* opPanNode = EM_new EMRubberBandNode();
		opPanNode -> SetPosition(vPos);
		opPanNode -> SetValue(vPan);

		m_opPanNodeObject -> insert(oIterator, opPanNode);		
	}
	return true;
}

bool EMMediaTrack::SaveData(EMProjectDataSaver* p_opSaver)
{
	//Make sure we don't save tracks that have been deleted!
	if(! m_vIsDeleted)
	{

		p_opSaver -> SaveString("EMT");

		p_opSaver -> SaveUInt32(static_cast<uint32>(GetID()));
		p_opSaver -> SaveUInt32(static_cast<uint32>(m_vFaderValue));
		p_opSaver -> SaveUInt32(static_cast<uint32>(m_vPanValue));
		p_opSaver -> SaveUInt64(static_cast<uint64>(m_vOffset));
		p_opSaver -> SaveUInt32(static_cast<uint32>(m_vPriorityID));
		p_opSaver -> SaveUInt32(static_cast<uint32>(m_eMuteState));
		p_opSaver -> SaveBool(m_vSoloed);
		p_opSaver -> SaveBool(m_vIsArmed);
		p_opSaver -> SaveBool(m_vIsRender);
		p_opSaver -> SaveString(m_oTrackName.c_str());

		EMMediaClipRepository* opAudioClips = EMMediaEngine::Instance() -> GetMediaProject() -> GetAudioClipRepository();
		EMMediaClipRepository* opVideoClips = EMMediaEngine::Instance() -> GetMediaProject() -> GetVideoClipRepository();
		EMMediaClipRepository* opMIDIClips = EMMediaEngine::Instance() -> GetMediaProject() -> GetMIDIClipRepository();
		EMMediaClipRepository* opVideoTransitionsClips = EMMediaEngine::Instance() -> GetMediaProject() -> GetVideoTransitionClipRepository();

		p_opSaver -> SaveUInt64(static_cast<uint64>(opAudioClips -> CountClipsForTrack(this)));
		p_opSaver -> SaveUInt64(static_cast<uint64>(opVideoClips -> CountClipsForTrack(this, EM_TYPE_ANY_VIDEO)));
		p_opSaver -> SaveUInt64(static_cast<uint64>(opVideoClips -> CountClipsForTrack(this, EM_TYPE_ANY_IMAGE)));
		p_opSaver -> SaveUInt64(static_cast<uint64>(opMIDIClips -> CountClipsForTrack(this)));
		p_opSaver -> SaveUInt64(static_cast<uint64>(opVideoTransitionsClips -> CountClipsForTrack(this)));
		
		opAudioClips -> LockContainer();
		try
		{
			opAudioClips -> Rewind();
			while(opAudioClips -> Current() != NULL)
			{
				//Save the clips that belong to "this" track
				if(opAudioClips -> Current() -> GetTrack() -> GetID() == GetID() && ! opAudioClips -> Current() -> IsObjectDeleted())
					opAudioClips -> Current() -> SaveData(p_opSaver);
				opAudioClips -> Next();
			}
		}
		catch(...)
		{
			EMDebugger("ERROR! Caught an exception in EMMediaTrack::SaveData (1)");
		}
		opAudioClips -> UnlockContainer();

		opVideoClips -> LockContainer();
		try
		{
			opVideoClips -> Rewind();
			while(opVideoClips -> Current() != NULL)
			{
				//Save the clips that belong to "this" track
				if(opVideoClips -> Current() -> GetTrack() -> GetID() == GetID() && ! opVideoClips -> Current() -> IsObjectDeleted() && (opVideoClips -> Current() -> GetType() & EM_TYPE_ANY_VIDEO) != 0)
					opVideoClips -> Current() -> SaveData(p_opSaver);
				opVideoClips -> Next();
			}
		}
		catch(...)
		{
			EMDebugger("ERROR! Caught an exception in EMMediaTrack::SaveData (2)");
		}
		opVideoClips -> UnlockContainer();

		opVideoClips -> LockContainer();
		try
		{
			opVideoClips -> Rewind();
			while(opVideoClips -> Current() != NULL)
			{
				//Save the clips that belong to "this" track
				if(opVideoClips -> Current() -> GetTrack() -> GetID() == GetID() && ! opVideoClips -> Current() -> IsObjectDeleted() && (opVideoClips -> Current() -> GetType() & EM_TYPE_ANY_IMAGE) != 0)
					opVideoClips -> Current() -> SaveData(p_opSaver);
				opVideoClips -> Next();
			}
		}
		catch(...)
		{
			EMDebugger("ERROR! Caught an exception in EMMediaTrack::SaveData (2b)");
		}
		opVideoClips -> UnlockContainer();


		opMIDIClips -> LockContainer();
		try
		{
			opMIDIClips -> Rewind();
			while(opMIDIClips -> Current() != NULL)
			{
				//Save the clips that belong to "this" track
				if(opMIDIClips -> Current() -> GetTrack() -> GetID() == GetID() && ! opMIDIClips -> Current() -> IsObjectDeleted())
					opMIDIClips -> Current() -> SaveData(p_opSaver);
				opMIDIClips -> Next();
			}
		}
		catch(...)
		{
			EMDebugger("ERROR! Caught an exception in EMMediaTrack::SaveData (3)");
		}
		opMIDIClips -> UnlockContainer();

		//Save volume node objects
		list<EMRubberBandNode*>::iterator oIterator;
		if(m_opVolumeNodeObject != NULL)
			 oIterator = m_opVolumeNodeObject -> begin();

		p_opSaver -> SaveUInt32(static_cast<uint32>(m_vNumberOfVolumeNodeObjects));
		for(int32 vIndexNodeCounter = 0; vIndexNodeCounter < m_vNumberOfVolumeNodeObjects; vIndexNodeCounter++)
		{
				p_opSaver -> SaveUInt64(static_cast<uint64>((*oIterator)->GetPosition()));
				p_opSaver -> SaveUInt32(static_cast<uint32>((*oIterator)->GetValue()));					
				oIterator++;
		}
		//Save pan node objects
		if(m_opPanNodeObject != NULL)
			oIterator = m_opPanNodeObject -> begin();
		p_opSaver -> SaveUInt32(static_cast<uint32>(m_vNumberOfPanNodeObjects));
		for(vIndexNodeCounter = 0; vIndexNodeCounter < m_vNumberOfPanNodeObjects; vIndexNodeCounter++)
		{
				p_opSaver -> SaveUInt64(static_cast<uint64>((*oIterator)->GetPosition()));
				p_opSaver -> SaveUInt32(static_cast<uint32>((*oIterator)->GetValue()));					
				oIterator++;
		}
	}
	return true;
}

void EMMediaTrack::SetDeleted(bool p_vFlag)
{
	m_vIsDeleted = p_vFlag;
}

bool EMMediaTrack::IsObjectDeleted() const
{
	return m_vIsDeleted;
}
