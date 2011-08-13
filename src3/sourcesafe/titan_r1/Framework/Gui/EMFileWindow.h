/*******************************************************
* Creator: Martin Rudefelt
* Component: none
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_FILE_WINDOW
#define __EM_FILE_WINDOW

#include "EMGUIComponent.h"

#include <string>
#include <vector>

enum EMFilePanelMode
{
	FILE_PANEL_LOAD_FILE,
	FILE_PANEL_LOAD_FOLDER,
	FILE_PANEL_SAVE_FILE
};

class EMFileWindow : public EMGUIComponent
{
public:
	virtual bool IsHidden() = 0;
	virtual vector<string>* GetPaths() = 0;

protected:
	EMFileWindow();
};

#endif
