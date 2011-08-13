/*******************************************************
* Creator: Martin Rudefelt
* Component: none
* Portability: native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifdef PLATFORM_BEOS

#ifndef __EM_BE_FILE_PANEL_CONTAINER
#define __EM_BE_FILE_PANEL_CONTAINER

#include "EMFileWindow.h"
#include "EMListenerRepository.h"

#include <FilePanel.h>
#include <Looper.h>
#include <string>
#include <vector>

class EMBeFilePanelContainer : public EMFileWindow, public BLooper, public BRefFilter
{
public:
	EMBeFilePanelContainer(EMFilePanelMode p_eFilePanelMode/*bool p_vIsLoadWindow*/, const char* p_vpWindowTitle, 
							bool p_vMultipleSelection, const char* p_vpSaveDefaultText, 
							vector<string>* p_vpVisibleExtensions = NULL, 
							const char* p_vpStartDirectory = NULL, uint32 p_vMessage = 0);
	~EMBeFilePanelContainer();
	EMRect Frame() const;
	void* GetNativeView() const;
	vector<string>* GetPaths();
	bool Filter(const entry_ref* p_spRef, BNode* p_opNode, struct stat* p_spStat, const char* p_vpType);
	bool IsHidden();
	void Hide();
	void MessageReceived(BMessage* p_opMessage); // Needed to catch native messages
	void Show();

private:
	vector<string> m_oFilenames;
	BFilePanel* m_opPanel;
	vector<string>* m_opVisibleExtensions;
};

#endif

#endif
