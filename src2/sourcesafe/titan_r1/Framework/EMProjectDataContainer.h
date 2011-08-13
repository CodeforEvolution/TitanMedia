/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_PROJECT_DATA_CONTAINER
#define __EM_PROJECT_DATA_CONTAINER

class __declspec(dllexport) EMProjectDataContainer
{
public:
	virtual ~EMProjectDataContainer();
	virtual bool ClearData();
	virtual bool IsDirty();
	virtual void LoadComplete();
	virtual bool LoadData(void* p_upProjectDataLoader);
	virtual void PrepareToClearData();
	virtual void PrepareToLoadData();
	virtual void PrepareToSaveData();
	virtual void ProjectClosed();
	virtual void ProjectCreated(int p_vProjectID);
	virtual void ReadyForUse();
	virtual void SaveComplete();
	virtual bool SaveData(void* p_upProjectDataSaver);

protected:
	EMProjectDataContainer();
};

#endif