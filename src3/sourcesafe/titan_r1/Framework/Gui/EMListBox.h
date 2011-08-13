/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_LIST_BOX
#define __EM_LIST_BOX

#include "EMGUIComponent.h"

#include <string>

class EMListBox : public EMGUIComponent
{
public:
	virtual ~EMListBox();
	virtual bool AddString(string p_oItem) = 0;
	virtual void Clear() = 0;
	virtual void Enable(bool p_vEnable) = 0;
	virtual int GetNrItems() = 0;
	virtual string GetSelection() = 0;
	virtual int GetSelectionIndex() = 0;
	virtual bool InsertString(string p_oItem, int p_vIndex) = 0;
	virtual bool IsEnabled() = 0;
	virtual void RemoveString(int p_vIndex) = 0;
	virtual void SelectItem(int p_vIndex) = 0;

protected:
	EMListBox();
};

#endif


