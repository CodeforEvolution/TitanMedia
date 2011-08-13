#include "EMGlobals.h"

#ifdef PLATFORM_WINDOWS

#include "EMWinMediaUtility.h"
#include "EMMediaFormat.h"
#include "EMCommandRepository.h"
#include "EMSettingsRepository.h"
#include "CommandIDs.h"
#include "DirectoryIDs.h"
#include "EMMediaEngine.h"
#include "EMSettingIDs.h"
#include "EMMediaUtility.h"
#include "EMWinMediaUtility.h"
#include "EMSemaphore.h"

#include <iostream>
#include <string>
#include <math.h>
#include <algorithm>
//#include <string.h>
#include <map>
#include <streams.h>

list<char*>* EMWinMediaUtility::classes = EM_new list<char*>();
list<int64>* EMWinMediaUtility::num = EM_new list<int64>();
list<map<int64, string> >* EMWinMediaUtility::instances = new list<map<int64, string> >();
//IDirectMusic8* EMWinMediaUtility::m_opDirectMusicInstance = NULL;

//EMMediaFormat* EMWinMediaUtility::m_opSystemAudioFormat = NULL;
//EMMediaFormat* EMWinMediaUtility::m_opSystemVideoFormat = NULL;

void EMWinMediaUtility::push(const void* ptr, char* name)
{
	//status_t vAcquireResult = acquire_sem(vSem);

	map<int64, string> oAddressNameMap;
	oAddressNameMap[(int64) ptr] = string(name);
	instances -> push_back(oAddressNameMap);
//	push(name);
//}
//
//void EMWinMediaUtility::push(char* name)
//{

	//if(vAcquireResult != B_NO_ERROR)
	//	EMDebugger("ERROR! EMWinMediaUtility could not acquire semaphore for push-protection!");

	list<char*>::const_iterator names;
	list<int64>::iterator nums;
	names = classes -> begin();
	nums = num -> begin();
	bool found = false;
	for(; names != classes -> end() && nums != num -> end(); names++, nums++)
	{
		if(strcmp((*names), name) == 0)
		{
			(*nums)++;
			found = true;
			break;
		}
	}

	if(! found)
	{
		num -> push_back(1);
		classes -> push_back(name);
	}
	//release_sem(vSem);
}

void EMWinMediaUtility::pop(char* name)
{
	//status_t vAcquireResult = acquire_sem(vSem);
	//if(vAcquireResult != B_NO_ERROR)
	//	EMDebugger("ERROR! EMWinMediaUtility could not acquire semaphore for push-protection!");
//	classes -> remove(name);

	list<char*>::const_iterator names;
	list<int64>::iterator nums;
	names = classes -> begin();
	nums = num -> begin();
	bool found = false;
	for(; names != classes -> end() && nums != num -> end(); names++, nums++)
	{
		if(strcmp((*names), name) == 0)
		{
			(*nums)--;
			found = true;
			break;
		}
	}

//	if(! found)
//		nums -> push_back(1);

	//release_sem(vSem);
}

void EMWinMediaUtility::show()
{
	//status_t vAcquireResult = acquire_sem(vSem);
	//if(vAcquireResult != B_NO_ERROR)
	//	EMDebugger("ERROR! EMWinMediaUtility could not acquire semaphore for push-protection!");

	;//cout_commented_out_4_release << endl << endl;
	;//cout_commented_out_4_release << "** Classes currently instantiated:" << endl;
	list<char*>::const_iterator i;
	list<int64>::const_iterator j = num -> begin();
	int64 sum = 0;
	for(i = classes -> begin(); i != classes -> end() && j != num -> end(); i++, j++)
	{
		;//cout_commented_out_4_release << "** " << (*j) << " instance(s) of class " << (*i) << endl;
		sum += (*j);
	}
	;//cout_commented_out_4_release << "** Total number of instances left is " << sum << endl;
	;//cout_commented_out_4_release << "** End of list" << endl;
	
	
/*	;//cout_commented_out_4_release << endl << endl;
	;//cout_commented_out_4_release << "** Classes and their object addresses:" << endl;

	list<map<int64, string> >::iterator it;
	for(it = instances -> begin(); it != instances -> end(); it++)
	{
		map<int64, string>::iterator mapIter;
		mapIter = (*it).begin();
		;//cout_commented_out_4_release << "** " << (*mapIter).first << " named \"" << (*mapIter).second << endl;
	}
	
	;//cout_commented_out_4_release << "** End of list" << endl; */
	//release_sem(vSem);
}

EMWinMediaUtility::EMWinMediaUtility() 
	:	EMMediaUtility(),
		m_opSystemAudioFormat(NULL),
		m_opSystemVideoFormat(NULL),
		m_opDirectMusicInstance(NULL),
		m_opDirectMusicProtection(NULL)
{
	m_opDirectMusicProtection = EMSemaphore::CreateEMSemaphore("DxMusic8 protection sem", 1, 1);
}

EMWinMediaUtility::~EMWinMediaUtility()
{
	m_opDirectMusicProtection -> Acquire();
	m_opDirectMusicInstance -> Release();
	m_opDirectMusicInstance = NULL;
	m_opDirectMusicProtection -> Release();
	delete m_opDirectMusicProtection;
	ClearData();
}

void* EMWinMediaUtility::GetRosterE()
{
	return NULL;
}

/*void EMWinMediaUtility::DisplayBuffer(BBuffer* opBuffer)
{
	;//emout_commented_out_4_release << "== BBuffer contents ==" << endl;
	;//emout_commented_out_4_release << "   Size available: " << opBuffer -> SizeAvailable() << endl;
	;//emout_commented_out_4_release << "   Size used: " << opBuffer -> SizeUsed() << endl;
	;//emout_commented_out_4_release << "   ID: " << opBuffer -> ID() << endl;
	;//emout_commented_out_4_release << "   Flags: " << opBuffer -> Flags() << endl;
	;//emout_commented_out_4_release << "   Start time: " << opBuffer -> Header() -> start_time << endl;
	;//emout_commented_out_4_release << "   Type: " << static_cast<int>(opBuffer -> Header() -> type) << endl;
	;//emout_commented_out_4_release << "   First sample: " << static_cast<signed short*>(opBuffer -> Data())[0] << endl;
	;//emout_commented_out_4_release << "" << endl;
}*/

double Round(double p_vNumber)
{
	double vInteger = floor(p_vNumber);
	double vDecimals = p_vNumber - vInteger;
	if(vDecimals >= 0.5)
		return vInteger + 1;
	return vInteger;
}

int64 EMWinMediaUtility::FramesToTime(int64 p_vFrames, const EMMediaFormat* p_opFormat)
{	
	if((p_opFormat -> m_eType & EM_TYPE_RAW_AUDIO) > 0)
		return static_cast<int64>(Round((1000000.0 * static_cast<double>(p_vFrames)) / static_cast<double>(p_opFormat -> m_vFrameRate)));
	else if((p_opFormat -> m_eType & EM_TYPE_RAW_VIDEO) > 0)
		return static_cast<int64>(Round(static_cast<double>(p_vFrames) * 1000000.0 / static_cast<double>(p_opFormat -> m_vFrameRate)));
	else
		EMDebugger("ERROR! FramesToTime is only implemented for audio and vide, this far!");
	return 0;
}

int64 EMWinMediaUtility::TimeToFrames(int64 p_vTime, const EMMediaFormat* p_opFormat)
{
	if((p_opFormat -> m_eType & EM_TYPE_RAW_AUDIO) > 0)
		return static_cast<int64>(Round((p_vTime * static_cast<double>(static_cast<double>(p_opFormat -> m_vFrameRate)) / 1000000.0)));
	else if((p_opFormat -> m_eType & EM_TYPE_RAW_VIDEO) > 0)
		return static_cast<int64>(Round((static_cast<double>(p_vTime) * static_cast<double>(p_opFormat -> m_vFrameRate) / 1000000.0)));
	else
		EMDebugger("ERROR! TimeToFrames is only implemented for audio and video, this far!");
	return 0;
}

int64 EMWinMediaUtility::BytesToFrames(int64 p_vBytes, const EMMediaFormat* p_opFormat)
{
	if((p_opFormat -> m_eType & EM_TYPE_RAW_AUDIO) > 0)
		return static_cast<int64>(static_cast<float>(p_vBytes) / (static_cast<float>(p_opFormat -> m_vNumChannels) * p_opFormat -> m_vBytesPerSample));
	else if((p_opFormat -> m_eType & EM_TYPE_RAW_VIDEO) > 0)
		return static_cast<int64>(floor(static_cast<float>(p_vBytes) / (static_cast<float>(p_opFormat -> m_vHeight) * static_cast<float>(p_opFormat -> m_vBytesPerRow))));
	else
		EMDebugger("ERROR! BytesToFrames is only implemented for audio and video, this far!");
	return 0;
}

int64 EMWinMediaUtility::FramesToBytes(int64 p_vFrames, const EMMediaFormat* p_opFormat)
{
	if((p_opFormat -> m_eType & EM_TYPE_RAW_AUDIO) > 0)
		return p_vFrames * (p_opFormat -> m_vNumChannels * p_opFormat -> m_vBytesPerSample);
	else
		EMDebugger("ERROR! FramesToBytes is only implemented for audio, this far!");
	return 0;
}

int64 EMWinMediaUtility::FramesPerBuffer(const EMMediaFormat* p_opFormat)
{
	if((p_opFormat -> m_eType & EM_TYPE_RAW_AUDIO) > 0)
		return p_opFormat -> m_vBufferSizeBytes / (p_opFormat -> m_vBytesPerSample * p_opFormat -> m_vNumChannels);
	else if((p_opFormat -> m_eType & EM_TYPE_RAW_VIDEO) > 0)
		return 1;
	else
		EMDebugger("ERROR! FramesPerBuffer is only implemented for audio and video, this far!");
	return 0;
}

/*int64 EMWinMediaUtility::FramesInBuffer(BBuffer* p_opBuffer, const EMMediaFormat* p_opFormat, EMMediaType p_eReturnType)
{
	if((p_opFormat -> m_eType & EM_TYPE_RAW_AUDIO) > 0)
	{
		int64 vFrames = 0;
		if((p_eReturnType & p_opFormat -> m_eType) > 0)
			vFrames = static_cast<int64>((p_opBuffer -> SizeUsed()) / ((p_opFormat -> m_vBytesPerSample) * p_opFormat -> m_vNumChannels));
		else
		{
			if(m_opSystemAudioFormat == NULL) m_opSystemAudioFormat = EM_new EMMediaFormat(EM_TYPE_RAW_AUDIO);
			if(m_opSystemVideoFormat == NULL) m_opSystemVideoFormat = EM_new EMMediaFormat(EM_TYPE_RAW_VIDEO);
			
			vFrames = TimeToFrames(FramesToTime((static_cast<int64>((p_opBuffer -> SizeUsed()) / ((p_opFormat -> m_vBytesPerSample) * p_opFormat -> m_vNumChannels))), m_opSystemAudioFormat), m_opSystemVideoFormat);
		}
		return vFrames;
	}
	else if((p_opFormat -> m_eType & EM_TYPE_RAW_VIDEO) > 0)
	{
		int64 vFrames = 0;
		if((p_eReturnType & p_opFormat -> m_eType) > 0)
			vFrames = 1;
		else
			vFrames = TimeToFrames(FramesToTime(1, p_opFormat), m_opSystemAudioFormat);
		return vFrames;
	}
	else
		EMDebugger("ERROR! FramesInBuffer is only implemented for audio and video, this far!");
	return 0;
} */

int64 EMWinMediaUtility::BytesPerFrame(const EMMediaFormat* p_opFormat)
{
	if((p_opFormat -> m_eType & EM_TYPE_RAW_AUDIO) > 0)
		return static_cast<int64>(p_opFormat -> m_vNumChannels * p_opFormat -> m_vBytesPerSample);
	else if((p_opFormat -> m_eType & EM_TYPE_RAW_VIDEO) > 0)
		return p_opFormat -> m_vHeight * p_opFormat -> m_vBytesPerRow;
	else
		EMDebugger("ERROR! BytesPerFrame is only implemented for audio and video, this far!");
	return 0;
}

float EMWinMediaUtility::FramesPerSecond(const EMMediaFormat* p_opFormat)
{
	if((p_opFormat -> m_eType & EM_TYPE_RAW_AUDIO) > 0)
		return p_opFormat -> m_vFrameRate;
	else if((p_opFormat -> m_eType & EM_TYPE_RAW_VIDEO) > 0)
	{
		//Should be calculated through the preferences and the acctual file videofile field_rate
		return p_opFormat -> m_vFrameRate; //Static
	}
	else
		EMDebugger("ERROR! FramesPerSecond is only implemented for audio and video, this far!");
	return 0;
}

int64 EMWinMediaUtility::FindMaxNum(signed short* array, int64 length, int64 num)
{
	signed short* max = EM_new signed short[num];
	for(register int i = 0; i < num; i++)
		max[i] = 0;
		
	for(i = 0; i < length; i++)
	{
		for(register int j = num - 1; j >= 0; j--)
		{
			if(array[i] > max[j])
			{
				max[j] = array[i];
				break;
			}
		}
	}
	
	signed short val = max[0];
	delete [] max;
	return static_cast<int64>(val);
}

const char* EMWinMediaUtility::GetStringBeforeLast(const char* p_vpSourceString, char p_vDelimiter)
{
	string oSourceString(p_vpSourceString);
	int32 vSeekedPos = -1;
	int32 vCharPosition = 0;
	for(string::iterator oStringIterator = oSourceString.begin(); oStringIterator != oSourceString.end(); oStringIterator++, vCharPosition++)
		if((*oStringIterator) == p_vDelimiter) vSeekedPos = vCharPosition;
	if(vSeekedPos == -1) 
	{
		m_oDummyString = "";
		return m_oDummyString.c_str();
	}
	m_oDummyString = oSourceString.substr(0, vSeekedPos).c_str();
	return m_oDummyString.c_str();
}

const char* EMWinMediaUtility::GetStringAfterLast(const char* p_vpSourceString, char p_vDelimiter)
{
	string oSourceString(p_vpSourceString);
	int32 vSeekedPos = -1;
	int32 vCharPosition = 0;
	for(string::iterator oStringIterator = oSourceString.begin(); oStringIterator != oSourceString.end(); oStringIterator++, vCharPosition++)
		if((*oStringIterator) == p_vDelimiter) vSeekedPos = vCharPosition;
	if(vSeekedPos == -1) 
	{
		m_oDummyString = "";
		return m_oDummyString.c_str();
	}
	m_oDummyString = oSourceString.substr(vSeekedPos + 1, oSourceString.size() - (vSeekedPos + 1)).c_str();
	return m_oDummyString.c_str();
}

const char* EMWinMediaUtility::GetFileName(const char* p_vpFullPath)
{	
	return GetStringAfterLast(p_vpFullPath, '\\');
}

const char* EMWinMediaUtility::GetPathName(const char* p_vpFullPath)
{
	return GetStringBeforeLast(p_vpFullPath, '\\');
}

const char* EMWinMediaUtility::ParseFullPath(const char* p_vpFileName, uint32 p_vDirectoryID)
{
	EMMediaEngine* e = EMMediaEngine::Instance();
	EMSettingsRepository* s = EMMediaEngine::Instance() -> GetSettingsRepository();

	//Does setting_project_id exist in the settings repository?! ... Find out tomorrow!!

	int vProjectID = *(static_cast<int*>(s -> GetSetting(SETTING_PROJECT_ID)));
	string oPath = *static_cast<string*>(EMMediaEngine::Instance() -> GetCommandRepository() -> ExecuteCommand(PROJECT_COMMAND_GET_DIRECTORY, &vProjectID, &p_vDirectoryID));
	oPath += string("\\") + string(p_vpFileName);
	m_oDummyString = oPath.c_str();
	return m_oDummyString.c_str();
}

const char* EMWinMediaUtility::MakePeaksFileName(const char* p_vpSourceFileName)
{
	string oSourceFile(GetFileName(p_vpSourceFileName));
	string oSourceFileNoExt(GetStringBeforeLast(oSourceFile.c_str(), '.'));
	string oSourceFileExt(GetStringAfterLast(oSourceFile.c_str(), '.'));
	string oResultFile = oSourceFileNoExt + string("_WaveFormPeaks.") + oSourceFileExt;
	string oResultFullPath(ParseFullPath(oResultFile.c_str(), DIRECTORY_ID_AUDIO_DATA_PEAKS));
	m_oDummyString = oResultFullPath.c_str();
	return m_oDummyString.c_str();
}

const char* EMWinMediaUtility::MakeExtractFileName(const char* p_vpSourceFileName)
{
	string oSourceFile(GetFileName(p_vpSourceFileName));
	string oSourceFileNoExt(GetStringBeforeLast(oSourceFile.c_str(), '.'));
	string oResultFile = oSourceFileNoExt + string("_ExtractedAudio.wav");
	string oResultFullPath(ParseFullPath(oResultFile.c_str(), DIRECTORY_ID_AUDIO_DATA_USED));
	m_oDummyString = oResultFullPath.c_str();
	return m_oDummyString.c_str();
}

const char* EMWinMediaUtility::MakeConvertFileName(const char* p_vpSourceFileName)
{
	string oSourceFile(GetFileName(p_vpSourceFileName));
	string oSourceFileNoExt(GetStringBeforeLast(oSourceFile.c_str(), '.'));
	string oSourceFileExt(GetStringAfterLast(oSourceFile.c_str(), '.'));
	string oResultFile = oSourceFileNoExt + string("_ConvertedAudio.") + oSourceFileExt;
	string oResultFullPath(ParseFullPath(oResultFile.c_str(), DIRECTORY_ID_AUDIO_DATA_USED));
	m_oDummyString = oResultFullPath.c_str();
	return m_oDummyString.c_str();
}

const char* EMWinMediaUtility::MakeRenderFileName(uint32 p_vSequenceNumber)
{
	return "NONAME-FIXTHIS!";
}

const char* EMWinMediaUtility::MakeAudioRecordingFileName(uint32 p_vSequenceNumber, int32 p_vUniqueID)
{
	string oZeroes = "";
	if(p_vSequenceNumber < 10)
		oZeroes += "0000";
	else if(p_vSequenceNumber < 100)
		oZeroes += "000";
	else if(p_vSequenceNumber < 1000)
		oZeroes += "00";
	else if(p_vSequenceNumber < 10000)
		oZeroes += "0";

	char vpSeqNumber[32];
	sprintf(vpSeqNumber, "%d", (int) p_vSequenceNumber);

	char vpIDNumber[32];
	sprintf(vpIDNumber, "%d", (int) p_vUniqueID);

	string oFile = string(vpIDNumber) + "take" + oZeroes + string(vpSeqNumber) + string(".wav");
	m_oDummyString = string(ParseFullPath(oFile.c_str(), DIRECTORY_ID_AUDIO_DATA_USED));
	return m_oDummyString.c_str();
}

const char* EMWinMediaUtility::MakeVideoRecordingFileName(uint32 p_vSequenceNumber, int32 p_vUniqueID)
{
	string oZeroes = "";
	if(p_vSequenceNumber < 10)
		oZeroes += "0000";
	else if(p_vSequenceNumber < 100)
		oZeroes += "000";
	else if(p_vSequenceNumber < 1000)
		oZeroes += "00";
	else if(p_vSequenceNumber < 10000)
		oZeroes += "0";

	char vpSeqNumber[32];
	sprintf(vpSeqNumber, "%d", (int) p_vSequenceNumber);

	char vpIDNumber[32];
	sprintf(vpIDNumber, "%d", (int) p_vUniqueID);

	string oFile = string(vpIDNumber) + "footage" + oZeroes + string(vpSeqNumber);
	m_oDummyString = ParseFullPath(oFile.c_str(), DIRECTORY_ID_VIDEO_DATA);
	return m_oDummyString.c_str();
}

EMMediaFormat* EMWinMediaUtility::GetSystemAudioFormat()
{
	if(m_opSystemAudioFormat == NULL) m_opSystemAudioFormat = EM_new EMMediaFormat(EM_TYPE_RAW_AUDIO);
	return m_opSystemAudioFormat;
}

EMMediaFormat* EMWinMediaUtility::GetSystemVideoFormat()
{
	if(m_opSystemVideoFormat == NULL) m_opSystemVideoFormat = EM_new EMMediaFormat(EM_TYPE_RAW_VIDEO);
	return m_opSystemVideoFormat;
}

void EMWinMediaUtility::ClearData()
{
	delete m_opSystemAudioFormat;
	m_opSystemAudioFormat = NULL;
	delete m_opSystemVideoFormat;
	m_opSystemVideoFormat = NULL;
}

IPin* EMWinMediaUtility::GetFirstPin(IBaseFilter* p_opFilterToExamine)
{
	IEnumPins* opPinEnumerator = NULL;
	HRESULT h = p_opFilterToExamine -> EnumPins(&opPinEnumerator);
	if(FAILED(h))
		EMDebugger("ERROR! Failed to enumerate pins of filter!");

	IPin* opPinInterface = NULL;
	PIN_DIRECTION PinDirThis;
	while(opPinEnumerator -> Next(1, &opPinInterface, 0) == S_OK)
    {
        h = opPinInterface -> QueryDirection(&PinDirThis);
        if(FAILED(h))
        {
            opPinInterface -> Release();
			opPinInterface = NULL;
            continue;
        }
        opPinInterface -> Release();
        // Does the pin's direction match the requested direction?
/*        if(PinDirThis == PINDIR_OUTPUT)
        {
            PIN_INFO sPinInformation = {0};

            // Direction matches, so add pin name to listbox
            h = opPinInterface -> QueryPinInfo(&sPinInformation);
            if(SUCCEEDED(h))
            {
            }

            // The pininfo structure contains a reference to an IBaseFilter,
            // so you must release its reference to prevent resource a leak.
            sPinInformation.pFilter -> Release();
        }
		else
			eo << "Direction is input!" << ef; */
		break;
    }
    opPinEnumerator -> Release();
	return opPinInterface;
}

WCHAR* EMWinMediaUtility::StringToUnicode(const char* p_vpString)
{
	string oString(p_vpString);
	WCHAR* wFile = EM_new WCHAR[oString.length()+1];
	for(int i = 0; i < oString.length()+1; i++)
		wFile[i] = 0;

	LPCSTR s = oString.c_str();
	MultiByteToWideChar(CP_ACP, 0, oString.c_str(), -1, wFile, oString.length());
//	eo << "THIS IS A HACK! Don't allocate memory here!!!!!" << ef;
	return wFile;
}

const char* EMWinMediaUtility::UnicodeToString(const WCHAR* p_vpName)
{
	int vStrLen = wcslen(p_vpName);

	char* dest = EM_new char[vStrLen+1];
	for(int i = 0; i < vStrLen+1; i++)
		dest[i] = '\0';
	WideCharToMultiByte(CP_ACP, 0, p_vpName, -1, dest, vStrLen, NULL, NULL);
	m_oDummyString = const_cast<const char*>(dest);
	return m_oDummyString.c_str();
}

void EMWinMediaUtility::SetDirectMusicInstance(IDirectMusic8* p_opInstance)
{
	m_opDirectMusicProtection ->Acquire();
	ASSERT(m_opDirectMusicInstance == NULL);
	m_opDirectMusicInstance = p_opInstance;
	m_opDirectMusicProtection ->Release();
}

IDirectMusic8* EMWinMediaUtility::GetDirectMusicInstance() const
{
	m_opDirectMusicProtection ->Acquire();
	ASSERT(m_opDirectMusicInstance != NULL);
	m_opDirectMusicProtection ->Release();
	return m_opDirectMusicInstance;
}

bool EMWinMediaUtility::DirectMusicObjectExists() const
{
	m_opDirectMusicProtection ->Acquire();
	if(m_opDirectMusicInstance == NULL)
	{
		m_opDirectMusicProtection ->Release();
		return false;
	}
	m_opDirectMusicProtection ->Release();
	return true;
}

#endif
