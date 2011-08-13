/*******************************************************
* Creator: Richard Kronfalt
* Portability: Native
*-------------------------------------------------------
* 
* No longer a container class of EMMediaClips. Instead
* used to store variables about the tracks in Titan, and
* also used to create clips for the respective tracks.
*
* Richard Kronfält, 00-11-28
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_MEDIA_TRACK
#define __EM_MEDIA_TRACK

#include "EMMediaGlobals.h"
#include "EMMediaClip.h"
#include "EMMediaItemContainer.h"
#include "EMInputDescriptor.h"
#include "EMMediaBufferSource.h"
#include "EMListenerRepository.h"

#include "EMMessages.h"
//#include "EMVideoPreview.h" //FOR VIDEO-TRACKS

#include <list>

class EMMediaBufferSource;
class EMMediaClipRepository;
class EMMediaTrackOutput;
class EMRealtimeInputDescriptor;
class EMSemaphore;
class EMProjectDataLoader;
class EMProjectDataSaver;
class EMMediaSignalMeter;
class EMFileInputDescriptor;
class EMRubberBandNode;

class EMMediaTrack : public EMMediaBufferSource, public EMListenerRepository, public EMListener
{
public:
	static EMMediaTrack* CreateEMTrack(EMMediaType p_eType, string p_oName, uint32 p_vPrio = 0);
	virtual ~EMMediaTrack();
	virtual uint32 GetPriority() const;
	virtual void SetPriority(uint32 p_vPrio);
	void SeekTo(int64 p_vNewSongTime);
	EMMediaType GetType() const;
	bool InitCheckE();
	virtual int32 CreateClip(int64 p_vAtFrame, string p_oName, EMFileInputDescriptor* p_opSource); //TODO: Make non-native
	EMBufferDestination* GetDestination() const;
	EMBufferDestination* GetTrackDestination() const;
	EMBufferDestination* GetClipDataDestination() const;
	
	virtual void SetTrackDestination(EMBufferDestination* p_opTrackDataReceiver);

	void SetInput(EMRealtimeInputDescriptor* p_opInput); //TODO: Make non-native
	EMRealtimeInputDescriptor* GetInput() const;

	int32 GetFader() const;
	virtual void SetFader(int32 p_vFaderValue);

	int32 GetPan() const;
	virtual void SetPan(int32 p_vPanValue);

	EMMuteState GetMuteState() const;
	virtual void SetMuteState(EMMuteState p_eFlag);

	bool IsSoloed() const;
	virtual void SetSoloed(bool p_vFlag);

	bool IsArmed() const;
	virtual void SetArmed(bool p_vFlag);

	bool IsRender() const;
	void SetRender(bool p_vFlag);

	string GetName() const;
	void SetName(string p_oName);

	virtual EMMediaBufferSource* GetSilenceGenerator() const;
	int64 CountClips();
	EMMediaSignalMeter* GetSignalMeter() const;
	EMBufferDestination* GetClipMixer() const;
	void StartPreview();
	virtual void StartRecording();
	void StopPreview();	
	virtual void StopRecording();
	
	virtual bool LoadData(EMProjectDataLoader* p_opLoader);
	virtual bool SaveData(EMProjectDataSaver* p_opSaver);

	void SetDeleted(bool p_vFlag);
	bool IsObjectDeleted() const;

	void InitializeTrackBuffers();
	virtual bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);

	EMRubberBandNode* AddRubberBandNode(int64* p_vPosAndValue, int p_vParam);
	void DeleteRubberBandNode(EMRubberBandNode* p_opRubberBandNode, int p_vParam);
	void MoveRubberBandNode(EMRubberBandNode* p_opRubberBandNode, int64 p_vPosAndValue[2]);
	list<EMRubberBandNode*>* GetRubberBandNodeObject(int p_vParam);
	int32 GetRubberBandValue(int64 p_vPosition, int p_vParam);

protected:
	EMMediaTrack(EMMediaType p_eType, string p_oName, uint32 p_vPrio = 0);

//private:
	EMMediaType m_eType; //Save
	EMMediaClipRepository* m_opAudioClipRepository;
	EMMediaClipRepository* m_opVideoClipRepository;
	EMMediaClipRepository* m_opVideoTransitionClipRepository;
	EMMediaClipRepository* m_opMIDIClipRepository;
	int32 m_vFaderValue; //Save
	int32 m_vPanValue; //Save
	int64 m_vOffset; //Save
	uint32 m_vPriorityID; //Save
	EMMuteState m_eMuteState; //Save
	bool m_vSoloed; //Save
	bool m_vIsArmed; //Save
	bool m_vIsRender; //Save
	bool m_vBuffersInit;
	string m_oTrackName; //Save
	EMSemaphore* m_opDestinationSemaphore;

	EMMediaBufferSource* m_opSilenceGenerator;
	EMBufferDestination* m_opClipMixer;
	EMMediaSignalMeter* m_opSignalMeter;
	EMBufferDestination* m_opTrackDataReceiver;  //Save
//	EMVideoPreview* m_opVideoPreview;
	EMRealtimeInputDescriptor* m_opRealtimeInput; //Save

	bool m_vIsDeleted; //Save

private:
	int32 m_vOldWidth;
	int32 m_vOldheight;
	float m_vOldFrameRate;
	list<EMRubberBandNode*>* m_opVolumeNodeObject;
	list<EMRubberBandNode*>* m_opPanNodeObject;
	int32 m_vNumberOfVolumeNodeObjects;
	int32 m_vNumberOfPanNodeObjects;
	int64 m_vLowerPositionVolume;
	int32 m_vLowerValueVolume;
	int64 m_vHigherPositionVolume;
	int32 m_vHigherValueVolume;
	int64 m_vLowerPositionPan;
	int32 m_vLowerValuePan;
	int64 m_vHigherPositionPan;
	int32 m_vHigherValuePan;
};

#endif
