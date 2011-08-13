/*******************************************************
* Creator: Richard Kronfalt
* Portability: Native
*-------------------------------------------------------
* 
* Represents a "clip", i.e. a chunk of media data.
* Contains a descriptor that is the "source" of data,
* i.e. a physical input, or a file.
* It inherits from the EMMediaBufferSource to be able
* to have a EMMediaDataBuffer which it can release on
* demand. When it is released, it is not full of any
* current data. It should rather be filled by someone
* else.
*
* Furthermore, the clip has methods for returning the
* number of audio-frames in the file. Even the length of
* a video clip must be represented by a number of audio
* frames - this because an audio frame is the minimal
* unit of time that we have in the system. Microseconds
* doesn't exist. Remember that! 
*
* Also, the clip knows at which audio frame it should
* start.
*
* TODO: Tons of things. Among other things, implement
* support for Mark-In and Mark-Out of the clip data!
*
* Richard Kronfält, 00-11-28
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_MEDIA_CLIP
#define __EM_MEDIA_CLIP

#include "EMMediaGlobals.h"
#include "EMMediaBufferSource.h"
#include "EMListener.h"

#include <string>

class EMFileInputDescriptor; //TODO: Make non-native
class EMMediaTrack;
class EMMediaEffectTrack;
class EMListenerRepository;

class EMProjectDataLoader;
class EMProjectDataSaver;

class EMMediaClip : public EMMediaBufferSource, public EMListener
{
public:
	virtual ~EMMediaClip();
	virtual EMMediaClip* CloneClip(int64 p_vStartFrame, EMMediaTrack* p_opBelongsToTrack) = 0;
	EMMediaDataBuffer* GetBuffer() const;
	void RecycleAllBuffers() const;
	EMFileInputDescriptor* GetDescriptor() const; //TODO: Make non native!
	string GetName() const;

	string GetFileName() const;
	void SetName(string p_oName);
	virtual EMMediaTrack* GetTrack() const;
	virtual bool InitCheckE() = 0;
	void SetDescriptor(EMFileInputDescriptor* p_opDescriptor); //TODO: Make non-native
	virtual void SetTrack(EMMediaTrack* p_opBelongsToTrack);

	virtual void SetDeleted(bool p_vFlag);
	bool IsObjectDeleted() const;

	bool IsMuted() const;
//	EMMediaDataBuffer* GetBuffer() const;

	void OnItemAdd();
	void OnItemRemove();


	//NEW POSITIONING METHODS
	virtual int64 GetStart(); // const;
	virtual void SetStart(int64 p_vTime);
	virtual int64 GetMediaStart() const;
	
	virtual int64 GetMarkInLength() const;
	virtual void SetMarkInLength(int64 p_vLength);
	
	virtual int64 GetMarkOutLength() const;
	virtual void SetMarkOutLength(int64 p_vLength);
	
	virtual int64 GetStop();
	virtual int64 GetMediaLength() const;

	virtual int64 GetActiveLength(); //Override in MIDI clip to read from other source than file! (midi is stored in RAM)

	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	
	bool SaveData(EMProjectDataSaver* p_opSaver);
	bool LoadData(EMProjectDataLoader* p_opLoader);

	virtual bool IsActiveNow(int64 p_vNow);

//	bool CreateEffectTrack();
	bool HasEffectTrack() const;
	int32 GetEffectTrackID() const;
	void SetEffectTrackID(int32 p_vID);

	virtual void CloseFile();
	virtual void OpenFile();

//	EMMediaDataBuffer* GetBuffer() const;
		
protected:
	EMMediaClip(EMMediaType p_eType, const char* p_vpFileName, int64 p_vStartFrame, EMMediaTrack* p_opBelongsToTrack);
	EMMediaClip(EMMediaTrack* p_opOwner);

//	EMMediaEffectTrack* m_opEffectTrack;
	int32 m_vFxID;

	//NEW POSITIONING ATTRIBUTES
	int64 m_vActiveStart;
	int64 m_vMarkInLength;
	int64 m_vMarkOutLength;
//	int64 m_vMediaLength;

//	int32 m_vID;
//	int64 m_vFirstFrame;
//	int64 m_vMarkInOffset;	//The number of frames (from the beginning of the clip)
//	int64 m_vMarkOutOffset;	//The number of frames (from the END of the clip)

	

	string m_oFileName;
	string m_oName;
	EMFileInputDescriptor* m_opDescriptor; //TODO: Make non-native.
	EMMediaTrack* m_opBelongsToTrack;

	bool m_vIsDeleted;
private:
};

#endif
