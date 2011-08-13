#include "EMAudioConverter.h"

#ifndef __EM_AUDIO_CONVERTER_CPP
#define __EM_AUDIO_CONVERTER_CPP

#include "EMGlobals.h"
#include "EMMediaFormat.h"
#include "EMMediaUtility.h"
#include "EMWaveFileReader.h"
#include "EMWaveFileWriter.h"
#include "EMMediaFileRepository.h"
#include "EMMediaEngine.h"
#include "EMCommandRepository.h"
#include "CommandIDs.h"

template<class tFromType, class tToType> EMAudioConverter<tFromType, tToType>::EMAudioConverter()
{
}

template<class tFromType, class tToType> EMAudioConverter<tFromType, tToType>::~EMAudioConverter()
{
}

template<class tFromType, class tToType> 
string EMAudioConverter<tFromType, tToType>::ConvertAudio(const string* p_oSourceFileName, const EMMediaFormat* p_opTargetFormat)
{
//	oNoPathFileName += ".converted.wav";
	string oConvertedFileName(EMMediaUtility::Instance() -> MakeConvertFileName((*p_oSourceFileName).c_str())); //ParseFullPath(oNoPathFileName, DIRECTORY_ID_AUDIO_DATA_USED);

//	string oConvertedFileName = *p_oSourceFileName + "_Converted.wav";

	EMWaveFileReader* opSourceFile = EM_new EMWaveFileReader(*p_oSourceFileName);
	EMWaveFileWriter* opTargetFile = EM_new EMWaveFileWriter(oConvertedFileName);

	opSourceFile -> InitCheckE();
	opTargetFile -> InitCheckE();
	
	EMMediaFormat* opSourceFormat = opSourceFile -> GetFormat();

	int64 vTotalSize = opSourceFile -> NumberOfFramesInFile() * p_opTargetFormat -> m_vNumChannels * sizeof(tFromType);

	int64 vNumberOfSourceBytes = opSourceFormat -> m_vBufferSizeBytes;
	int64 vNumberOfSourceSamples = vNumberOfSourceBytes / opSourceFormat -> m_vBytesPerSample;
	int64 vSourceFileBytePosition = 0;
	tFromType* vpSourceData = EM_new tFromType[vNumberOfSourceSamples];

	void* upDataToWrite = NULL;
	int64 vBytesToWrite = 0;

	bool vShouldLoop = true;
	int64 vCount = 0;
	while(vShouldLoop)
	{
		int64 vNumberOfBytesRead = vNumberOfSourceBytes;
		opSourceFile -> ReadData(vpSourceData, vNumberOfBytesRead, vSourceFileBytePosition, 0, opSourceFormat -> m_vBytesPerSample);
		vSourceFileBytePosition += vNumberOfBytesRead;
		if(vNumberOfBytesRead != opSourceFormat -> m_vBufferSizeBytes) //TODO: Fix so we can have 4096 mod file sizes!
			vShouldLoop = false;
		if(vNumberOfBytesRead == 0) 
			break;

		upDataToWrite = vpSourceData;
		vBytesToWrite = vNumberOfBytesRead;

/*		if(opSourceFormat -> m_vNumChannels < p_opTargetFormat -> m_vNumChannels)
		{	//Less channels in source file than the target format requires! Convert from, for instance, mono to stereo here!
			tToType* vpNewPointer = NULL;
			int64 vNumBytesReturnedHere = 0;
			EMMediaFormat oFormatAfterChannelConversion(*opSourceFormat);
			oFormatAfterChannelConversion.m_vNumChannels = p_opTargetFormat -> m_vNumChannels;
			tFromType* vpSourceDataPointer = NULL;
			ChannelConversion(vpSourceData, vBytesToWrite / (sizeof(tFromType) * opSourceFormat -> m_vNumChannels), opSourceFormat, &vpSourceDataPointer, &vNumBytesReturnedHere, &oFormatAfterChannelConversion);
//			upDataToWrite = vpNewPointer;
//			vBytesToWrite = vNumBytesReturnedHere;
		} */


		tToType* vpPointer = NULL;
		int64 vNumBytesReturned = vNumberOfBytesRead;
		if(opSourceFormat -> m_vBytesPerSample != p_opTargetFormat -> m_vBytesPerSample)
		{
			SampleTypeConversion(vpSourceData, vNumberOfBytesRead, &vpPointer, &vNumBytesReturned);
			upDataToWrite = vpPointer;
			vBytesToWrite = vNumBytesReturned;
		}
		else
			vpPointer = (tToType*) vpSourceData;
		
		tToType* vpNewPointer = NULL;
		int64 vNumBytesReturnedHere = 0;
		if(opSourceFormat -> m_vFrameRate != p_opTargetFormat -> m_vFrameRate)
		{
			if(vpPointer == NULL)
				EMDebugger("ERROR! Can't continue, since the source data array is NULL!");
			FrameRateConversion(vpPointer, vNumBytesReturned / (sizeof(tToType) * opSourceFormat -> m_vNumChannels), opSourceFormat, &vpNewPointer, &vNumBytesReturnedHere, p_opTargetFormat);
			upDataToWrite = vpNewPointer;
			vBytesToWrite = vNumBytesReturnedHere;
		}

/*		if((vCount % 10) == 0)
		{
			char vpPercentage[10];
			sprintf(vpPercentage, "%d%%",  (int) (100 * (((float) vSourceFileBytePosition) / vTotalSize)));
			string oString = string("Converting audio data: ") + vpPercentage;
//			EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(COMMAND_WRITE_STATUS_BAR_MESSAGE, const_cast<char*>(oString.c_str()));
		}
		vCount++;*/

		opTargetFile -> WriteData(upDataToWrite, vBytesToWrite);
		delete [] vpNewPointer;
	}

	EMMediaFormat oFormat = *p_opTargetFormat;
	oFormat.m_vBytesPerSample = sizeof(tToType);
	opTargetFile -> WriteHeaderE(&oFormat);
	
	delete opSourceFile;
	delete opTargetFile;
	delete [] vpSourceData;

	EMMediaFileRepository::Instance() -> AddFile(&oConvertedFileName);
	return oConvertedFileName;
}

template<class tFromType, class tToType> 
void EMAudioConverter<tFromType, tToType>::ChannelConversion(tFromType* p_vpSourceArray, 
													int64 p_vNumSourceFrames, 
													const EMMediaFormat* p_opSourceFormat,

													tFromType** p_vpTargetArrayPointer, 
													int64* p_vOutNumTargetBytes,
													const EMMediaFormat* p_opTargetFormat)
{
	int32 vNumSourceChannels = p_opSourceFormat -> m_vNumChannels;
	int32 vNumTargetChannels = p_opTargetFormat -> m_vNumChannels;
	float vFactor = static_cast<float>(vNumTargetChannels) / static_cast<float>(vNumSourceChannels);
	int64 vNumSourceBytes = p_vNumSourceFrames * vNumSourceChannels * sizeof(tFromType);
	int64 vNumTargetBytes = vNumSourceBytes * vFactor;
	
	int64 vNumTargetSamples = vNumTargetBytes / p_opTargetFormat -> m_vBytesPerSample;

	*p_vpTargetArrayPointer = EM_new tFromType[vNumTargetSamples];

	for(int i = 0; i < vNumTargetSamples; i++)
		(*p_vpTargetArrayPointer)[i] = p_vpSourceArray[static_cast<int32>(floor(i / vFactor))];
}

template<class tFromType, class tToType> 
void EMAudioConverter<tFromType, tToType>::SampleTypeConversion(tFromType* p_upSourceArray, int64 p_vSourceNumBytes, tToType** p_upTargetArrayPointer, int64* p_vpOutNumBytes)
{
	tFromType* vpSourceArray = (tFromType*) p_upSourceArray;
	tToType** vpTargetArray = (tToType**) p_upTargetArrayPointer;
	int64 vNumSamples = p_vSourceNumBytes / sizeof(tFromType);

	bool vFromTypeIsSigned = false;
	bool vToTypeIsSigned = false;
	
	//Find out if the types are signed or not... Subtract 1 from 0 and see 
	//what the result is (if it's less than 0 then it's signed - otherwise not).
	if((tFromType) (((tFromType) 0) -1) > 0) 
		vFromTypeIsSigned = false;
	else 
		vFromTypeIsSigned = true;

	if((tToType) (((tToType) 0) -1) > 0) 
		vToTypeIsSigned = false;
	else 
		vToTypeIsSigned = true;



	float vSourceRange = pow(2, 8 * sizeof(tFromType));
	float vSourceMinimum = (vFromTypeIsSigned ? -(vSourceRange / 2) : 0);
	float vSourceMaximum = (vFromTypeIsSigned ? (vSourceRange / 2) -1 : vSourceRange -1);
	float vSourceZeroLevel = (vFromTypeIsSigned ? 0 : (vSourceRange / 2));
	
	float vTargetRange = pow(2, 8 * sizeof(tToType));
	float vTargetMinimum = (vToTypeIsSigned ? -(vTargetRange / 2) : 0);
	float vTargetMaximum = (vToTypeIsSigned ? (vTargetRange / 2) -1 : vTargetRange -1);
	float vTargetZeroLevel = (vToTypeIsSigned ? 0 : (vTargetRange / 2));

	if(vFromTypeIsSigned)
		vSourceRange = (vSourceRange / 2) -1;
	if(vToTypeIsSigned)
		vTargetRange = (vTargetRange / 2) -1;

//	float vSourceSampleZeroLevel = (vFromTypeIsSigned ? vSourceMax / 2 : vSourceMax);
//	float vTargetSampleZeroLevel = (vToTypeIsSigned ? vTargetMax / 2 : vTargetMax);
		
	*p_vpOutNumBytes = vNumSamples * sizeof(tToType);

	if(*vpTargetArray != NULL)
		delete [] *vpTargetArray;
	(*vpTargetArray) = EM_new tToType[vNumSamples];

	for(register int32 vIndex = 0; vIndex < vNumSamples; vIndex++)
	{
/*		tFromType vSourceSample = vpSourceArray[vIndex];
		float vSourceFloatValue = vSourceFloatValue = static_cast<float>(vSourceSample);

		float vR1 = (vSourceFloatValue >= vSourceZeroLevel ? (vSourceFloatValue + 1) / vSourceRange : vSourceFloatValue / vSourceRange);
		float vR2 = vR1 + (vTargetMinimum / vTargetRange) + (vTargetZeroLevel / vTargetRange);
		float vR3 = vR2 * vTargetRange;
		
		(*vpTargetArray)[vIndex] = (tToType) floor(vR3);
*/
//		(*vpTargetArray)[vIndex] = (tToType) floor(vSourceFloatValue * (float) vTargetMax);
//		tToType vTargetSample = (*vpTargetArray)[vIndex];
//		if(vToTypeIsSigned && ! vFromTypeIsSigned)
//			(*vpTargetArray)[vIndex] = (*vpTargetArray)[vIndex] - (vTargetMax / 2);

		(*vpTargetArray)[vIndex] = (tToType) floor((static_cast<float>(vpSourceArray[vIndex]) / vSourceRange) * (float) vTargetRange);
	}
}

template<class tFromType, class tToType> 
void EMAudioConverter<tFromType, tToType>::FrameRateConversion(tToType* p_vpSourceArray, 
													int64 p_vNumSourceFrames, 
													const EMMediaFormat* p_opSourceFormat,

													tToType** p_vpTargetArrayPointer, 
													int64* p_vOutNumTargetBytes,
													const EMMediaFormat* p_opTargetFormat)
//Temporary assumption: The number of channels are always the same in the source and target formats
{
	struct tFrame //This is used so that we can access all samples within one frame at the same time!
	{
		tToType left;
		tToType right;
	};

	float vDivisability = p_opTargetFormat -> m_vFrameRate / p_opSourceFormat -> m_vFrameRate;
	
	const int32 EM_UPSAMPLE = 1;
	const int32 EM_DOWNSAMPLE = 2;
	
	int32 vMode = 0;
	if(vDivisability > 1)
		vMode = EM_UPSAMPLE;
	else
		vMode = EM_DOWNSAMPLE;
	
	int64 vNumTargetFrames = static_cast<int64>(static_cast<float>(p_vNumSourceFrames) * vDivisability);
	
	if(*p_vpTargetArrayPointer != NULL)
		delete [] *p_vpTargetArrayPointer;
	tFrame* vpSourceFrames = (tFrame*) p_vpSourceArray;
	tFrame* vpTargetFrames = EM_new tFrame[vNumTargetFrames];

	int64 vSourceFrameNo = 0;
	for(int64 vTargetFrameNo = 0; vTargetFrameNo < vNumTargetFrames ; vTargetFrameNo++)
	{
		vpTargetFrames[vTargetFrameNo] = vpSourceFrames[vSourceFrameNo];
		while(vTargetFrameNo / p_opTargetFormat -> m_vFrameRate >= vSourceFrameNo / p_opSourceFormat -> m_vFrameRate)
			vSourceFrameNo++;
	}

	*p_vpTargetArrayPointer = (tToType*) vpTargetFrames;
	*p_vOutNumTargetBytes = vNumTargetFrames * p_opTargetFormat -> m_vNumChannels * sizeof(tToType);
}

/*


0                                                                                                   1
0         1         2         3         4         5         6         7         8         9         0
01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890


Source format:
4 frames per second:
#                        #                        #                        #                        #
A                        B                        C                        D                        E

Target format:
10 frames per second:
#         #         #         #         #         #         #         #         #         #         #
A         A         A         B         B         C         C         C         D         D

Target / Source is 10 / 4 = 2.5
That is, for each source frame, there is 2.5 target frames

one source frame is 1/4th of a second long, that is 250 mS
one target frame is 1/10th of a second long, that is 100 mS

source frame no = 0
target frame no = 0

do until source frame no >= number of source frames

	target[target frame no] = source[source frame no]

	target frame no ++
	
	if target frame no / target frame rate >= source frame no / source frame rate
		source frame no ++




Source format:
10 frames per second:
#         #         #         #         #         #         #         #         #         #         #
A         A         A         B         B         C         C         C         D         D

Target format:
4 frames per second:
#                        #                        #                        #                        #
A                        B                        C                        D                        E

Target / Source is 4 / 10 = 0.4
That is, for each source frame, there is 0.4 target frames

one source frame is 1/10th of a second long, that is 100 mS
one target frame is 1/4th of a second long, that is 250 mS

do until target frame no >= number of target frames
	
	target[target frame no] = source[source frame no]
	
	source frame no ++;

	if(source frame no / source frame rate >= target frame no / target frame rate)
		target frame++




*/




















#endif