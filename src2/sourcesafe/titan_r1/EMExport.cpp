#include "EMExport.h"
#include "windows.h"
#include "CommandIDs.h"
#include "EMMediaEngine.h"
#include "EMCommandRepository.h"
#include "DirectoryIDs.h"
#include "r.h"

EMExport::EMExport()
{
}

EMExport::~EMExport()
{
}

bool EMExport::Export(bool m_vVideo, bool m_vAudio, const char* vpFullPath)
{


	int codeStart;
	int codeEnd;
	LPVOID decryptionStart;
	LPVOID countAddress;
	DWORD oldprotect;
	
	typedef LPVOID (* FunctionType)(LPVOID);
	FunctionType ComputeSum;
	int vChecksum = 0;
	int vChecksum2 = 0;
	BYTE* ComputeKey;
	int bytesReadCryptFile = 0;
	int bytesReadKeyFile = 0;
	char* opPtr;

	decryptionStart = Counter(false);
	InitCRC();

	//****Unmark this section to write some chunk of titan reg code
/*	_asm mov dword ptr countAddress,offset [countit]
	_asm mov codeStart, offset[countit]
	_asm mov codeEnd, offset[countitEnd]
	int lengthToCopy = codeEnd - codeStart;
	ComputeSum = (FunctionType) new BYTE[512];//lengthToCopy];
	ComputeKey = new BYTE[255];
	memcpy(ComputeSum, countAddress, 512);//lengthToCopy);

	//Get project path
	int vProjectID(0);//(*static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_PROJECT_ID)));
	uint32 vDirectoryID(DIRECTORY_ID_APPLICATION);
	string oAppDirFileName = *static_cast<string*>(EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_GET_DIRECTORY, &vProjectID, &vDirectoryID)) + "\\TitanRegChunk.trc";
	string oAppDirFileName2 = *static_cast<string*>(EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_GET_DIRECTORY, &vProjectID, &vDirectoryID)) + "\\TitanRegKey.trk";


	FILE* fp2 = fopen(oAppDirFileName.c_str(),"wb");
	//Read in the serial key
	FILE* fp3 = fopen(oAppDirFileName2.c_str(),"rb");
	bytesReadCryptFile = fread(ComputeKey, 1, 255, fp3);

	char* key = new char[bytesReadCryptFile+1];
	key[bytesReadCryptFile] = '\0';
	memcpy(key, ComputeKey, bytesReadCryptFile);


	opPtr = (char*)ComputeSum;


	SetCryptKeys(key);
	delete [] key;
	delete [] ComputeKey;
	for(int aroundandaround = 0; aroundandaround < 32; aroundandaround ++)
	{
		EncryptBlock((unsigned char*)opPtr);
		opPtr += 16;
	}
	fwrite(ComputeSum, 1, 512, fp2);
	fclose(fp2);
	delete (LPBYTE)ComputeSum;
	return false;*/
	//****End of section

	//****Unmark this for release
	int lengthToCopy = 512;//codeEnd - codeStart;

	ComputeSum = (FunctionType) new BYTE[lengthToCopy];
	ComputeKey = new BYTE[255];
	//Get project path
	int vProjectID(0);//(*static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_PROJECT_ID)));
	uint32 vDirectoryID(DIRECTORY_ID_APPLICATION);
	string oAppDirFileName = *static_cast<string*>(EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_GET_DIRECTORY, &vProjectID, &vDirectoryID)) + "\\TitanRegChunk.trc";
	string oAppDirFileName2 = *static_cast<string*>(EMCommandRepository::Instance() -> ExecuteCommand(PROJECT_COMMAND_GET_DIRECTORY, &vProjectID, &vDirectoryID)) + "\\TitanRegKey.trk";

	//Read in the serial key
	FILE* fp3 = fopen(oAppDirFileName2.c_str(),"rb");
	if(fp3 != NULL)
	{
		bytesReadCryptFile = fread(ComputeKey, 1, 255, fp3);
		fclose(fp3);
	}
	else
		return false;

	char* key = new char[bytesReadCryptFile+1];
	key[bytesReadCryptFile] = '\0';
	memcpy(key, ComputeKey, bytesReadCryptFile);

	FILE* fp2 = fopen(oAppDirFileName.c_str(),"rb");
	if(fp2 != NULL)
	{
		bytesReadCryptFile = fread(ComputeSum, 1, lengthToCopy, fp2);
		fclose(fp2);
	}
	else
		return false;

	opPtr = (char*)ComputeSum;

	fp2 = fopen(oAppDirFileName2.c_str(),"rb");
	if(fp2 != NULL)
	{
		bytesReadKeyFile = fread(ComputeKey, 1, 255, fp2);
		fclose(fp2);
	}
	else
		return false;

	//Calculate checksum
	LPVOID vCheksumPointerVoid = ComputeSum;
	char* vCheksumPointer = (char*) vCheksumPointerVoid;
	for(int index = 0; index < bytesReadCryptFile; index ++)
	{
		vChecksum += (BYTE)vCheksumPointer[index];
	}

	SetCryptKeys(key);

	//Chabbelkod starts here (for the crackers)**************************************
	decryptionStart = Counter(false);
	if(decryptionStart && vChecksum)
		oldprotect = 0xff00 + vChecksum;
	else
		oldprotect = 0x0000;
	decryptionStart = (void*)(oldprotect * lengthToCopy / 4 + (BYTE)&decryptionStart);
	codeEnd = oldprotect;
	for(index = 0; index < 18; index ++)
		codeEnd += (BYTE)vCheksumPointer[index] + vChecksum;
	decryptionStart = Counter(false);
	//Chabbelkod Stops here**********************************************************

	for(int aroundandaround = 0; aroundandaround < 32; aroundandaround ++)
	{
		DecryptBlock((unsigned char*)opPtr);
		opPtr += 16;
	}
	delete [] key;
	for(index = 0; index < 44; index ++)
	{
		vChecksum2 += (BYTE)vCheksumPointer[index];
	}
	//Chabbelkod starts here (for the crackers)**************************************
	decryptionStart = Counter(false);
	if(decryptionStart && vChecksum2)
		oldprotect = 0xf0f + vChecksum2;
	else
		oldprotect = 0x000f;
	decryptionStart = (void*)(oldprotect * lengthToCopy / 4 + (BYTE)&decryptionStart);
	codeEnd = oldprotect;
	for(index = 0; index < 18; index ++)
		codeEnd += (BYTE)vCheksumPointer[index] + vChecksum2;
	decryptionStart = Counter(false);
	//Chabbelkod Stops here**********************************************************

	if(vChecksum2 != 4164)
		return false;
	else
	{

	}
	//****End of section



	EMCommandRepository* opCmds = EMMediaEngine::Instance() -> GetCommandRepository();
	_asm mov edi, eax //<-- Line is neccessary as we use edi in our chunk of protection

	LPVOID arneanka = opCmds;

	void*(EMCommandRepository::*kaksig)(uint32, void*, void*, void*);

	kaksig = &EMCommandRepository::ExecuteCommand;

	_asm mov eax, kaksig
	LPVOID opExecuteCommend;
	_asm mov opExecuteCommend, eax

	_asm nop

	LPVOID sum = ComputeSum(opExecuteCommend);
	_asm nop

	opCmds -> ExecuteCommand(MEDIA_COMMAND_START_PLAYBACK);

	decryptionStart = Counter(false);

	delete (LPBYTE)ComputeSum;
	delete ComputeKey;
	return true;

//Should be marked out for release
countit:
/*	_asm push ebp
	//Here you can use the two values how ever you like
	LPVOID opCmdsPointer;
	LPVOID trickyStartPointerAdress;
	DWORD trickyStartPointer;
	DWORD trickyEndPointer;
	DWORD lengthOfTrickyPart;
	int counter;
	//Remember that ebp hold the current stack(changing this will alter the whole stackpointer community)
	
	_asm mov ebp, esp
	_asm mov ebx, dword ptr [ebp+8]
	//End using the values
	_asm pop ebp

	_asm push 0
	_asm push 0
	_asm mov edx, dword ptr[ebp+10h]
	_asm push edx
	_asm push 223h
	//_asm mov ecx, dword ptr[ebp-24h]
	_asm mov ecx, edi
	_asm call ebx

	_asm push 0
	_asm lea ecx, [ebp+0Ch]
	_asm push ecx
	_asm lea edx, [ebp+8]
	_asm push edx
	_asm push 226h
	//_asm mov ecx, dword ptr[ebp-24h]
	_asm mov ecx, edi
	_asm call ebx
	_asm ret
*/
countitEnd:

	return true;
}

LPVOID EMExport::Counter(bool executeORnot)
{
	LPVOID countAddress;

	EMCommandRepository* opCmds = EMMediaEngine::Instance() -> GetCommandRepository();
	if(opCmds != NULL)
	{
		if(!executeORnot)
		{
			FILE* fp2 = fopen("TitanRegChunk.trc","rb");
			if(fp2 != NULL)
			{
				_asm mov dword ptr countAddress,offset [ThisIsWhere]
				fclose(fp2);
			}
			return countAddress;
		}

	ThisIsWhere:
		if(executeORnot)
		{
			int totalwaste = (int) countAddress;
			if(totalwaste != 0)
			{
				MessageBox(NULL, "Why haven't you figured the scheme out yet(what are you doing here? This is so totally wrong!) Have a beer and you'll manage it, says Cadaber :)", "Message for the cracker", NULL);
				return (void*)0x7f0020;
			} else
				return (void*)0x7fd657;
		}
	}
	return NULL;
}

//HERE FOLLOWS THE CRYPTATION ALGORITHM

