#ifndef __EM_RECORDING
#define __EM_RECORDING

#include <list>
#include "EMMediaTrack.h"

class EMRecording
{
public:
	EMRecording();
	~EMRecording();
	int64 m_vStartTime;
	list<EMMediaTrack*> m_oRecorderTracks;
	int32 m_vMediaFileEntryID;
	string m_oFileName;
};

#endif