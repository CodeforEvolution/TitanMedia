#include "EMBeFilePanelContainer.h"

#ifdef PLATFORM_BEOS

#include "CommandIDs.h"

#include "EMCommandRepository.h"
#include "EMImageBufferRepository.h"

//Temp - Added by Rille
#include "EMGTrack.h"
#include "EMGTrackRepository.h"
//End temp

#include "EMListener.h"
#include "EMMessages.h"
#include "EMRect.h"

#include <Directory.h>
#include <Entry.h>
#include <FindDirectory.h>
#include <Message.h>
#include <Messenger.h>
#include <Node.h>
#include <Path.h>
#include <Rect.h>
#include <string>
#include <Window.h>

#define BE_DIRECTORY "application/x-vnd.Be-directory"

// Opens a standard BeOS file-handling window
EMBeFilePanelContainer::EMBeFilePanelContainer(EMFilePanelMode p_eFilePanelMode/*bool p_vIsLoadWindow*/, const char* p_vpWindowTitle, 
												bool p_vMultipleSelection, const char* p_vpSaveDefaultText, 
												vector<string>* p_opVisibleExtensions, 
												const char* p_vpStartDirectory, uint32 p_vMessage)
{
	file_panel_mode vType;
	BDirectory	oDir;
	BPath oPath;
	BEntry oEntry;
	Run();

	m_opVisibleExtensions = p_opVisibleExtensions;

	uint32 vNodeFlavor;

	if(p_eFilePanelMode == FILE_PANEL_SAVE_FILE)
	{
		vType = B_SAVE_PANEL;
		vNodeFlavor = B_FILE_NODE;
	}
	else
	{
		vType = B_OPEN_PANEL;
		if(p_eFilePanelMode == FILE_PANEL_LOAD_FILE)
			vNodeFlavor = B_FILE_NODE;
		else
			vNodeFlavor = B_DIRECTORY_NODE;
	}
	
	if(p_vpStartDirectory == NULL)
		find_directory(B_USER_DIRECTORY, &oPath, true); // Home directory
	else
		oPath.SetTo(p_vpStartDirectory);
	oDir.SetTo(oPath.Path());
	oDir.GetEntry(&oEntry);
	BMessenger oHandler(this);//dynamic_cast<BHandler*>(this));

	BMessage* opMessage = NULL;
	if(p_vMessage != 0)
		opMessage = new BMessage(p_vMessage);
		
	m_opPanel = new BFilePanel(vType, NULL, NULL, vNodeFlavor, 
								p_vMultipleSelection, opMessage, this, true, true);

	if(opMessage != NULL)
		delete opMessage;

	if(p_vpWindowTitle != NULL)
		m_opPanel -> Window() -> SetTitle(p_vpWindowTitle);
	m_opPanel -> SetPanelDirectory(&oEntry);
	if(p_vpSaveDefaultText != NULL && vType == B_SAVE_PANEL)
		m_opPanel -> SetSaveText(p_vpSaveDefaultText);
	m_opPanel -> SetTarget(oHandler);
	m_opPanel -> Window() -> Show();
}

// m_vpVisibleExtensions is owned by this class
EMBeFilePanelContainer::~EMBeFilePanelContainer()
{
	delete m_opPanel;
	if(m_opVisibleExtensions != NULL)
		delete m_opVisibleExtensions;
}

EMRect EMBeFilePanelContainer::Frame() const
{
	BRect oRect = m_opPanel -> Window() -> Frame();
	return EMRect(oRect.left, oRect.top, oRect.right, oRect.bottom);
}

void* EMBeFilePanelContainer::GetNativeView() const
{
	return (void*) m_opPanel;
}

vector<string>* EMBeFilePanelContainer::GetPaths()
{
	return &m_oFilenames;
}

// Filters filenames so that only files with certain extensions are shown
bool EMBeFilePanelContainer::Filter(const entry_ref* p_spRef, BNode* p_opNode, struct stat* p_spStat, const char* p_vpType)
{
	if(m_opVisibleExtensions == NULL)
		return true; // All extensions are allowed
	if(strcmp(p_vpType, BE_DIRECTORY) == 0) // Ok to display directories
		return true;

	string oName(p_spRef -> name);
	string::size_type vPos = oName.rfind('.');
	if(vPos == string::npos) // Extension cannot be found
		return false;
	if(vPos == oName.length() - 1)
		return false; // The dot is the last character in the filename
	string oExt = oName.substr(vPos + 1);

	for(uint16 vIndex = 0; vIndex < m_opVisibleExtensions -> size(); vIndex++)
		if((*m_opVisibleExtensions)[vIndex] == oExt)
			return true;
	return false;
}

void EMBeFilePanelContainer::Hide()
{
	m_opPanel -> Window() -> Hide();
}

bool EMBeFilePanelContainer::IsHidden()
{
	return !(m_opPanel -> IsShowing());
}

// Translates and passes on native messages
void EMBeFilePanelContainer::MessageReceived(BMessage* p_opMessage)
{
	uint32 vMessage;
	entry_ref oRef;
	const char* opFilename;
	string oFullPath;
	int vIndex = 0;
	BPath oPath;
	BEntry oEntry;

	switch(p_opMessage -> what)
	{
	case B_SAVE_REQUESTED:
		vMessage = EM_SAVE_REQUESTED;
		m_oFilenames.clear();
		if(p_opMessage -> FindRef("directory", &oRef) == B_OK)
		{
			opFilename = p_opMessage -> FindString("name", (int32) 0);
			oEntry.SetTo(&oRef, true);
			oPath.SetTo(&oEntry);
			oFullPath = oPath.Path();
			oFullPath += "/";
			oFullPath += opFilename;
			m_oFilenames.push_back(oFullPath);
		}
		break;
	case B_CANCEL:
		vMessage = EM_FILE_CANCEL;
		break;
	case B_REFS_RECEIVED:
	case EM_LOAD_MEDIA_REQUESTED:
	case EM_LOAD_PROJECT_REQUESTED:
		if(p_opMessage -> what == B_REFS_RECEIVED)
			vMessage = EM_LOAD_REQUESTED;
		else
			vMessage = p_opMessage -> what;
		m_oFilenames.clear();
		while(p_opMessage -> FindRef("refs", vIndex, &oRef) == B_OK)			
		{
			oEntry.SetTo(&oRef, true);
			oPath.SetTo(&oEntry);
			oFullPath = oPath.Path();
			m_oFilenames.push_back(oFullPath);
			vIndex++;
		}
		break;
	case EM_CREATE_PROJECT_REQUESTED:
	case EM_SAVE_PROJECT_REQUESTED:
		vMessage = EM_CREATE_PROJECT_REQUESTED;
		m_oFilenames.clear();
		if(p_opMessage -> FindRef("directory", &oRef) == B_OK)
		{
			opFilename = p_opMessage -> FindString("name", (int32) 0);
			oEntry.SetTo(&oRef, true);
			oPath.SetTo(&oEntry);
			oFullPath = oPath.Path();
			oFullPath += "/";
			oFullPath += opFilename;
			m_oFilenames.push_back(oFullPath);
		}
/*
		while(p_opMessage -> FindRef("refs", vIndex, &oRef) == B_OK)			
		{
			oEntry.SetTo(&oRef, true);
			oPath.SetTo(&oEntry);
			oFullPath = oPath.Path();
			m_oFilenames.push_back(oFullPath);
			vIndex++;
		}
*/
		break;
	default:
		vMessage = p_opMessage -> what;
	}
	
	if(!Notify(vMessage))
		BHandler::MessageReceived(p_opMessage);
}

void EMBeFilePanelContainer::Show()
{
	m_opPanel -> Window() -> Show();
}

#endif
