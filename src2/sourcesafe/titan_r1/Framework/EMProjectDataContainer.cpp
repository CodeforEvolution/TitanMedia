#include "EMProjectDataContainer.h"

EMProjectDataContainer::EMProjectDataContainer()
{
}

EMProjectDataContainer::~EMProjectDataContainer()
{
}

// Is called when a project data container should clear all
// project related data (probably only when closing a project)
bool EMProjectDataContainer::ClearData()
{
	return true;
}

// Is called to see if any data needs to be saved, e.g. when quitting without first saving
bool EMProjectDataContainer::IsDirty()
{
	return false;
}

// Is called when data has been loaded by all EMProjectDataContainer objects
void EMProjectDataContainer::LoadComplete()
{
}

// Provides a way to load data from the previously saved project file
// ProjectCreated() should be called before this method is called
// Should return false only if data for some reason could not be loaded
bool EMProjectDataContainer::LoadData(void* p_upProjectDataLoader)
{
	return true;
}

// Called before ClearData()
void EMProjectDataContainer::PrepareToClearData()
{
}

// Called before LoadData()
void EMProjectDataContainer::PrepareToLoadData()
{
}

// Called before SaveData()
void EMProjectDataContainer::PrepareToSaveData()
{
}

// Is called when a project is discarded. Before this, ClearData() is called
void EMProjectDataContainer::ProjectClosed()
{
}

// Is called when a new project is defined (and assigned a unique ID)
void EMProjectDataContainer::ProjectCreated(int p_vProjectID)
{
}

// Is called after a project operation is completed and signals
// that all data is available and can be presented to the user
void EMProjectDataContainer::ReadyForUse()
{
}

// Is called when data has been loaded by all EMProjectDataContainer objects
void EMProjectDataContainer::SaveComplete()
{
}

// Provides a way to save data to the project file
// Should return false only if data for some reason could not be saved
bool EMProjectDataContainer::SaveData(void* p_upProjectDataSaver)
{
	return true;
}


