/*******************************************************
* Creator: Jesper Svensson
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/


#ifndef __EM_WIN_QUICKTIME_WRITER_SUPER
#define __EM_WIN_QUICKTIME_WRITER_SUPER

class EMWinQuickTimeWriter;

class EMMediaFormat;
//class EMMediaType;
class EMMediaDataBuffer;

#include <string>

enum EMMediaType;

class EMWinQuickTimeWriterSuper
{
public:
	EMWinQuickTimeWriterSuper();
	~EMWinQuickTimeWriterSuper();
	static EMWinQuickTimeWriterSuper* Instance();

	//Wrapped Functons
	void SetRenderName(char* p_vpName);
	bool InitCheckE(EMMediaType p_vMediaType);
	void GetAudioEncoderIDs(unsigned long* p_vpIDs);
	void GetVideoEncoderIDs(unsigned long* p_vpIDs);
	int GetNumberOfVideoEncoders();
	int GetNumberOfAudioEncoders();
	void SetShuffleFormatForVideo(EMMediaFormat* p_opMediaFormat);
	void SetShuffleFormatForAudio(EMMediaFormat* p_opMediaFormat);
	void SetAudioEncoderToUse(long p_vEncoderID);
	void SetVideoEncoderToUse(long p_vEncoderID);
	void DisConnectWithEncoders();
	void StopWritingToDisk();
	void ProcessVideoBuffer(EMMediaDataBuffer* p_opDataBuffer);
	void ProcessAudioBuffer(EMMediaDataBuffer* p_opDataBuffer);
	void SetCompressionCapabilityInfo(long p_vEncoderID, long p_vPropertyID);
	char* GetCompressionCapabilities(long p_vEncoderID);
	std::string GetEncoderName(long p_vEncoderID);
	bool GetEncoderPropertyPages(long p_vEncoderID, bool p_vShouldShow);
	bool ExtractAudioFromQuickTimeFile(const char* p_opSource, const char* p_opDestination);

private:
	static EMWinQuickTimeWriterSuper* m_opQuickTimeWriterSuper;

	EMWinQuickTimeWriter* m_opQuickTimeWriter;
};

#endif