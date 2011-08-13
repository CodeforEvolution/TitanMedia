/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* The track is not responsible for updating its clips, this
* should instead be handled by the caller
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_VIDEO_PREVIEW
#define __EM_VIDEO_PREVIEW

#include "EMListenerRepository.h"
#include "EMRect.h"

class EMBitmap;

class EMVideoPreview : public EMListenerRepository
{
public:
	virtual ~EMVideoPreview();
	virtual EMRect Bounds() = 0;
	virtual void DrawBitmap(EMBitmap* p_opBitmap) = 0;
	virtual void HidePreview() = 0;
	static EMVideoPreview* Instance();
	virtual bool Lock() = 0;
	//virtual void LockIfNeededAndDrawBitmap(EMBitmap* p_opBitmap) = 0;
	static void SetInstance(EMVideoPreview* p_opInstance);
	virtual void ShowPreview() = 0;
	virtual void Unlock() = 0;

protected:
	EMVideoPreview();

private:
	static EMVideoPreview* m_opInstance;
};

#endif