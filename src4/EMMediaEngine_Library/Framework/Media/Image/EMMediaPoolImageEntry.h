/*******************************************************
* Creator: Martin Johansosn
* Portability: Native
*-------------------------------------------------------
* 
* This class simply represents a media file.
* It exists in one instance per "physical" file.
*
* Martin Johansson, 2001-09-12
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_MEDIA_POOL_IMAGE_ENTRY
#define __EM_MEDIA_POOL_IMAGE_ENTRY

#include "EMMediaPoolEntry.h"
#include "EMMediaFormat.h"

class EMProjectDataSaver;
class EMProjectDataLoader;
class EMMediaFormat;
class EMMediaDataBuffer;
class EMImageFileInputDescriptor;

#include <dshow.h>

class EMMediaPoolImageEntry : public EMMediaPoolEntry 
{
public:
	EMMediaPoolImageEntry(string p_oFileName, EMMediaType p_eType);
	EMMediaPoolImageEntry(EMProjectDataLoader* p_opLoader, uint32 vUndecidedType);

	bool SaveData(EMProjectDataSaver* p_opSaver);

	virtual ~EMMediaPoolImageEntry();

	virtual EMMediaFormat* GetFormat() const;
	virtual int64 GetLength() const;
	virtual int64 GetSize() const ;
	virtual int64 GetFrameRate() const;
	virtual string GetFormatString();

	EMImageFileInputDescriptor* CloneDescriptor();
	
	float* GetAspectRatio();

	int64* NumberOfFramesInWFPS(float p_vFramerateCurrentFPS);

	EMBitmap* GetImage(int16 p_vHeight);

	virtual bool InitCheckE();

	virtual int64 BitDepth() const;

protected:
	IPin* FindOutputPin(IGraphBuilder* p_opIGraph);

protected:
	EMMediaFormat m_oFormat;
	EMMediaFormat* m_opFormat;
	float m_vAspect;

	EMImageFileInputDescriptor* m_opDesc;
	EMMediaDataBuffer* m_opBuffer;

	int64 m_vOne;
};

#endif
