#include "EMGUIComponent.h"

EMGUIComponent::EMGUIComponent()
{
}

EMGUIComponent::~EMGUIComponent()
{
}

// This method is called in AddChild() from an EMView or an EMWindow
// to allow components to prepare to be added (or even add themselves
// as for MS Windows components)
// Returns false if AddToComponent fails for some reason
bool EMGUIComponent::AddToComponent(EMGUIComponent* p_opComponent)
{
	return true;
}

// This method is called when parent component want to know if "this"
// component can have focus at all i.e. when Tab-key is pressed
bool EMGUIComponent::CanGetTab()
{
	return false;
}

uint32 EMGUIComponent::GetResizingMode()
{
	return 0;
}

// This method is called after the component has been attached to a view
// It makes it possible for the component to initialize itself if
// necessary
void EMGUIComponent::InitComponent()
{
}

// This method is called when parent component want to know if "this"
// component can have focus now i.e. when Tab-key is pressed and component
// is Enabled.
// This method is also called when something/someone want to know if 
// "this" component enabled or not.
bool EMGUIComponent::IsEnabled()
{
	return true;
}

// This method is called when parent component want to know if "this"
// component can have focus now i.e. when Tab-key is pressed and component
// is Enabled.
// This method is also called when something/someone want to know if 
// "this" component visible or not.
bool EMGUIComponent::IsVisible()
{
	return true;
}

void EMGUIComponent::SetFocus()
{
}
/*
void EMGUIComponent::SetFrame(EMRect p_oFrame)
{
}
*/
