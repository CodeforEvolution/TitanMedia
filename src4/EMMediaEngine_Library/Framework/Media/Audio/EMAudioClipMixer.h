/*******************************************************
* Creator: Richard Kronfalt
* Portability: Native
*-------------------------------------------------------
*
* This class is your average mixer. It takes a list of
* buffers (EMMediaDataBuffer) and adds them together and
* returns the result. 
*
* To be more specific, it takes the first buffer in the
* list and uses that as the destination for the additions.
* It loops through the remaining lists, adds each sample
* to the corresponding sample in the destination, and
* then Recycles the source.
*
* It also knows the next destination (since it inherits
* from EMBeBufferDestination, it is a destination as well
* as knows the next destination) so it updates the
* destination of the returned buffer.
*
* This class is typically instantiated once for each
* EMMediaTrack, and used for mixing overlapping (within
* the track) audio clips.
*
* Richard Kronfält, 00-11-28
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_AUDIO_CLIP_MIXER
#define __EM_AUDIO_CLIP_MIXER

#include "EMBufferDestination.h"
#include "EMMediaDataBuffer.h"

class EMAudioClipMixer : public EMBufferDestination
{
public:
	EMAudioClipMixer(int32 p_vParentId);
	~EMAudioClipMixer();
	bool PrepareToPlayE();
	EMMediaDataBuffer* ProcessBufferE(list<EMMediaDataBuffer*>* p_opBufferList);
	EMMediaDataBuffer* ProcessBufferE(EMMediaDataBuffer** p_opBufferList);

	int32 GetParentId();

protected:
	EMAudioClipMixer();

private:
	BufferMixerFunction m_fpMixFunction;
	int32 m_vParentId;

};

#endif
