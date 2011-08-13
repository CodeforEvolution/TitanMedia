/*******************************************************
* Creator: Richard Kronfalt
* Portability: Non-Native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_AUDIO_CONVERTER
#define __EM_AUDIO_CONVERTER

class EMMediaFormat;

template <class tFromType, class tToType> 
class EMAudioConverter 
{
public:
	EMAudioConverter();
	~EMAudioConverter();
	static string ConvertAudio(const string* p_oSourceFileName, const EMMediaFormat* p_opTargetFormat);
	static void SampleTypeConversion(tFromType* p_upSourceArray, int64 p_vSourceNumBytes = 0, tToType** p_upTargetArrayPointer = NULL, int64* p_vpOutNumBytes = NULL);
	static void FrameRateConversion(tToType* p_vpSourceArray, int64 p_vNumSourceFrames, const EMMediaFormat* p_opSourceFormat, tToType** p_vpTargetArrayPointer, int64* p_vOutNumTargetFrames, const EMMediaFormat* p_opTargetFormat);
	static void ChannelConversion(tFromType* p_vpSourceArray, int64 p_vNumSourceFrames, const EMMediaFormat* p_opSourceFormat, tFromType** p_vpTargetArrayPointer, int64* p_vOutNumTargetBytes, const EMMediaFormat* p_opTargetFormat);

protected:          
private: 
};

#include "EMAudioConverter.cpp"

#endif
