/*******************************************************
* Creator: Richard Kronfalt
* Portability: Native
*-------------------------------------------------------
* 
*******************************************************/

#ifndef __EM_FILE_INPUT_DESCRIPTOR
#define __EM_FILE_INPUT_DESCRIPTOR

#include "EMInputDescriptor.h"

#include <string>

class EMMediaDataBuffer;
class EMMediaFormat;

class EMFileInputDescriptor : public EMInputDescriptor
{
public:
	virtual ~EMFileInputDescriptor();
	string GetFileName() const;
	int64 GetNumFrames() const;
	virtual EMMediaFormat* GetFormat() = 0;
	virtual bool InitCheckE() = 0;
	virtual bool ReadFrames(EMMediaDataBuffer** p_opPrimaryBuffer, EMMediaDataBuffer** p_opSecondaryBuffer, int64 p_vMediaFramePosition, int64 p_vOffsetFrames, int64 & p_vOutNumRead, bool p_vSeeking = false) = 0;
	virtual bool SeekAndDisplay(int64 p_vNewMediaFrame) = 0;
	virtual bool SeekTo(int64 p_vNewMediaFrame) = 0;
	int32 GetMediaPoolEntryID() const;
	void SetMediaPoolEntryID(int32 p_vEntryID);

	virtual bool Open();
	virtual bool Close();

protected:
	EMFileInputDescriptor(string p_oFileName, EMMediaType p_eType);
	string m_oFileName;
	int64 m_vNumFrames;
	int32 m_vMediaPoolEntryID;
};

#endif
