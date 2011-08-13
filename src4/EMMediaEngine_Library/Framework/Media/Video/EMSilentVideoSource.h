/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
* 
* This class inherits from buffer source, and is used
* for producing silence. It is instantiated from the 
* EMMediaProject class (namely in the Run()-method) and
* supplies it with silent video buffers for sending 
* downstream when there's no "real" audio to 
* produce/process.
*
* TODO: Allow for holding multiple buffers, so we can
* send several buffers downstream (i.e. to all destinations)
* if there's no audio to process.
*
* Jesper Svensson, 00-12-12
*******************************************************/

#include "EMGlobals.h"
#ifndef __EM_SILENT_VIDEO_SOURCE
#define __EM_SILENT_VIDEO_SOURCE

#include "EMMediaBufferSource.h"
#include "EMListenerRepository.h"
#include "EMListener.h"

class EMSilentVideoSource : public EMMediaBufferSource, public EMListener
{
public:
	EMSilentVideoSource();
	~EMSilentVideoSource();
//	EMMediaDataBuffer* GetBuffer() const;
	bool InitCheckE(EMMediaFormat* p_opFormat);

	EMMediaTrack* GetTrack() const;
	void SetTrack(EMMediaTrack* p_opTrack);

	virtual EMMediaDataBuffer* GetBufferAt(int64 p_vTime, bool p_vSeeking);
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);


protected:
	EMMediaFormat m_oFormat;
	EMMediaTrack* m_opTrack;

	int32 m_vColor;

	unsigned char m_vR;
	unsigned char m_vG;
	unsigned char m_vB;
private:
	bool m_vNeedToSyncWithTime;
	int m_vOldWidth;
	int m_vOldheight;
};

#endif
