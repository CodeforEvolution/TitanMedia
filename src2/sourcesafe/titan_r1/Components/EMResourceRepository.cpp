#include "EMResourceRepository.h"

#include "EMBitmap.h"
#include "EMCommandRepository.h"
#include "EMFactory.h"

#ifdef PLATFORM_BEOS
#include <Bitmap.h>
#include <Rect.h>
#endif
#ifdef PLATFORM_WINDOWS
#include "EMWinBitmap.h"
#include "EMWinWindow.h"
#include <windows.h>
#endif

#include <fstream>
#include <list>
#include <memory>

const char* INPUT_FILE_ID = "EM_RES";
//const char* RESOURCE_PATH = "System/resources.emr"; 

//Since code doesn't recompile after a preprocessor define has changed, it didn't help
//to make a #ifdef TRIAL_VERSION etc routine here. Instead, I made two static libraries
//that export two different definitions of RESOURCE_FILE. One library called EMResourceFull.lib
//and one called EMResourceTrial.lib.  EMResourceFile.h simply declares the constant as external
//and makes the search for this constant wait until the linking procedure.
//In the project settings for Titan_r1 the link include list is changed accordingly for
//a full-version and a trial-version build (one includes EMResourceFull.lib and one EMResourceTrial.lib).
//All files for the EMResources project can be found in /lib/, including the .dsp and .lib file(s).
//																				/Richard, 010914
#ifdef TRIAL_VERSION
const char* RESOURCE_FILE = "ResourcesTrial.emr";
#endif
#ifdef FULL_VERSION
const char* RESOURCE_FILE = "Resources.emr";
#endif
#ifdef TITAN_TV_VERSION
const char* RESOURCE_FILE = "titan_tv_resources.emr";
#endif
//#include "EMResourceFile.h"

map<uint32, EMResource*> EMResourceRepository::m_oResources;

EMResourceRepository::~EMResourceRepository()
{
	// Deleting all resources
	map<uint32, EMResource*>::iterator oIterator = m_oResources.begin();
	while(oIterator != m_oResources.end())
	{
		delete oIterator -> second;
		oIterator++;
	}
}

string EMResourceRepository::FixString(string p_oString)
{
	while(p_oString.find("\\t") != string::npos)
		p_oString.replace(p_oString.find("\\t"), 2, "\t");

	return p_oString;
}

string* EMResourceRepository::GetString(uint32 p_vID)
{
	return static_cast<string*>((m_oResources[p_vID]) -> m_opResource);
}

list<string*>* EMResourceRepository::GetStringList(uint32 p_vID)
{
	return static_cast<list<string*>*>((m_oResources[p_vID]) -> m_opResource);
}

EMBitmap* EMResourceRepository::GetBitmap(uint32 p_vID)
{
	return static_cast<EMBitmap*>((m_oResources[p_vID]) -> m_opResource);
}

bool EMResourceRepository::LoadResources(string p_oPath)
{
	string oFilePath;
	if(p_oPath == "")
		oFilePath = RESOURCE_FILE;
	else
		oFilePath = p_oPath + EM_FILE_SEPARATOR + RESOURCE_FILE;
//oFilePath += "xx";	
eo << "EMResourceRepository::LoadResources: " << oFilePath.c_str() << ef;
	ifstream oIn;
	oIn.open(oFilePath.c_str(), std::ios::in | std::ios::binary);
	if(!oIn)
	{
eo << "Could not open the compiled resource file" << ef;
		return false;
	}

	char vFileID[7];
	oIn.read(vFileID, 6);
	vFileID[6] = '\0';
	string oFileID(vFileID);
	if(oFileID != INPUT_FILE_ID)
	{
eo << "The resource file is of an unknown format (" << oFileID.c_str() << ")" << ef;
		return false;
	}
	
	int vCurrentType;
	int vLength;
	int vSize;
	char vString[1000];
	string* opString;
	string oString;
	list<string*>* opStrings;
	int32 vBitmapSize;
	int vColorSpace;
	int32 vIndex;
	int32 vDimensionX;
	int32 vDimensionY;
	EMBitmap* opEMBitmap = NULL;
#ifdef PLATFORM_BEOS
	BBitmap* opBitmap = NULL;
	color_space eColorSpace;
#endif
#ifdef PLATFORM_WINDOWS
	HBITMAP opBitmap = NULL;
//	EMColorSpace eColorSpace;
#endif

	int vID;
//vID = 0;
	try
	{
		while(oIn)
		{
//vID++;
			if(oIn.peek() == EOF) // Is there a better solution than to peek? (is this used BTW?)
				break;

			oIn.read((/*unsigned*/ char*) &vCurrentType, sizeof vCurrentType);
			switch(vCurrentType)
			{
			case 1:
				oIn.read((/*unsigned*/ char*) &vID, sizeof vID);
				oIn.read((/*unsigned*/ char*) &vLength, sizeof vLength);	
				oIn.read((/*unsigned*/ char*) vString, vLength);
				vString[vLength] = '\0';
				opString = EM_new string(FixString(string(vString)));
				m_oResources[vID] = EM_new EMResource(EM_STRING, vID, opString);
				break;
			case 2:
				oIn.read((/*unsigned*/ char*) &vID, sizeof vID);
				oIn.read((/*unsigned*/ char*) &vSize, sizeof vSize);
				opStrings = EM_new list<string*>;
				for(vIndex = 0; vIndex < vSize; vIndex++)
				{
					oIn.read((/*unsigned*/ char*) &vLength, sizeof vLength);	
					oIn.read((/*unsigned*/ char*) vString, vLength);
					vString[vLength] = '\0';
//					oString = vString;
//					opStrings -> push_back(oString);
					opString = EM_new string(vString);
					opStrings -> push_back(opString);
				}
				m_oResources[vID] = EM_new EMResource(EM_STRING_LIST, vID, opStrings);
				break;
#ifdef PLATFORM_BEOS
			case 3:
				oIn.read((/*unsigned*/ char*) &vID, sizeof vID);
				oIn.read((/*unsigned*/ char*) &vColorSpace, sizeof vColorSpace);
				switch(vColorSpace)
				{
				case 1:
					eColorSpace = B_GRAY1;
					break;
				case 2:
					eColorSpace = B_GRAY8;
					break;
				case 3:
					eColorSpace = B_CMAP8;
					break;
				case 4:
					eColorSpace = B_RGB15;
					break;
				case 5:
					eColorSpace = B_RGBA15;
					break;
				case 6:
					eColorSpace = B_RGB16;
					break;
				case 7:
					eColorSpace = B_RGB32;
					break;
				default:
eo << "Resource file is probably corrupt" << ef;
					return;
				}
				oIn.read((/*unsigned*/ char*) &vDimensionX, sizeof vDimensionX);
				oIn.read((/*unsigned*/ char*) &vDimensionY, sizeof vDimensionY);
				oIn.read((/*unsigned*/ char*) &vBitmapSize, sizeof vBitmapSize);
				try
				{
					opBitmap = EM_new BBitmap(BRect(0, 0, vDimensionX, vDimensionY), eColorSpace);
					vpArray = static_cast<unsigned char*>(opBitmap -> Bits());
					oIn.read(vpArray, vBitmapSize);
					opEMBitmap = EMFactory::Instance() -> CreateBitmap(opBitmap);
					m_oResources[vID] = EM_new EMResource(EM_BITMAP, vID, opEMBitmap);
				}
				catch(bad_alloc)
				{
					delete opBitmap;
					delete opEMBitmap;
					throw;
				}
				break;
#endif
#ifdef PLATFORM_WINDOWS
			case 3:
				oIn.read((/*unsigned*/ char*) &vID, sizeof vID);
				// In the windows version, colorspaces are not really supported
				oIn.read((/*unsigned*/ char*) &vColorSpace, sizeof vColorSpace);
				oIn.read((/*unsigned*/ char*) &vDimensionX, sizeof vDimensionX);
				oIn.read((/*unsigned*/ char*) &vDimensionY, sizeof vDimensionY);
				oIn.read((/*unsigned*/ char*) &vBitmapSize, sizeof vBitmapSize);
/*
{
BITMAPINFO sBitmapInfo;
sBitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
sBitmapInfo.bmiHeader.biWidth = 50;
sBitmapInfo.bmiHeader.biHeight = 50;
sBitmapInfo.bmiHeader.biPlanes = 1;
sBitmapInfo.bmiHeader.biBitCount = vColorSpace;
sBitmapInfo.bmiHeader.biCompression = BI_RGB;
void* upBitmapBits;
HBITMAP opNativeBitmap = CreateDIBSection(NULL, &sBitmapInfo, DIB_RGB_COLORS, &upBitmapBits, NULL, NULL);
m_oResources[vID] = EM_new EMResource(EM_BITMAP, vID, opEMBitmap);
char* xxx = new char[vBitmapSize];
oIn.read((char*) xxx, vBitmapSize);//vSize);
delete xxx;
break;
}
*/
				try
				{
					BITMAPINFO sBitmapInfo;
					sBitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
					sBitmapInfo.bmiHeader.biWidth = vDimensionX + 1;
					sBitmapInfo.bmiHeader.biHeight = 1 * (vDimensionY + 1);
					sBitmapInfo.bmiHeader.biPlanes = 1;
					sBitmapInfo.bmiHeader.biBitCount = vColorSpace;
					sBitmapInfo.bmiHeader.biCompression = BI_RGB;
					void* upBitmapBits;

					HBITMAP opNativeBitmap = CreateDIBSection(NULL, &sBitmapInfo, DIB_RGB_COLORS, &upBitmapBits, NULL, NULL);
					if(opNativeBitmap == NULL)
						eo << "resource: bitmap is NULL" << ef;
//					uint32 vSize(sBitmapInfo.bmiHeader.biBitCount / 8 * (1) * sBitmapInfo.bmiHeader.biHeight * sBitmapInfo.bmiHeader.biWidth);
					oIn.read((char*) upBitmapBits, vBitmapSize);//vSize);
					opEMBitmap = EMWinBitmap::CreateBitmap(opNativeBitmap);//new EMWinBitmap(opNativeBitmap);//EMFactory::Instance() -> CreateBitmap(opNativeBitmap);
//					(dynamic_cast<EMWinBitmap*>(opEMBitmap)) -> SetBits(upBitmapBits);
					m_oResources[vID] = EM_new EMResource(EM_BITMAP, vID, opEMBitmap);
				}
				catch(bad_alloc)
				{
					//delete opBitmap;
					delete opEMBitmap;
					throw;
				}
				break;
#endif
			default:
eo << "Found unknown resource: type: " << vCurrentType << ef;
				return false;
			}
		}
	}
	catch(...)
	{
eo << "Exception caught when reading/parsing resource file" << ef;
		return false;
	}
eo << "All resources loaded" << ef;

	return true;
}



