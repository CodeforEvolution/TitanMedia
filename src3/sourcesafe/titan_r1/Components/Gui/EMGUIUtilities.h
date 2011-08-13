/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_GUI_UTILITIES
#define __EM_GUI_UTILITIES

#include "EMGUIGlobals.h"
#include "EMListener.h"
#include "EMRect.h"
#include "GUIGlobals.h" // FIXME: A framework class should not include an application file

#include <list>
#include <string>

class EMGClip;
class EMGUIComponent;
class EMListenerRepository;

class EMGUIUtilities : public EMListener
{
public:
	EMGUIUtilities();
	//string CalculateCopyString(string p_oOriginalString);
	static inline int64 Abs64(int64 p_vValue) { return p_vValue >= 0 ? p_vValue : -1 * p_vValue; };
	bool AddToClipGroup(EMGClip* p_opClipGroupOwner, EMGClip* p_opClipToAdd);
	list<EMGClip*> GetClipGroup(EMGClip* p_opClip, bool* p_vpHiddenClipsExist);
	list<EMGClip*> GetClipGroup(list<EMGClip*>* p_opClips, bool* p_vpHiddenClipsExist);
	int GetGridMeasureDivider();
	int64 GetGridSize();
	int64 GetMeasureSize();
	int64 GetMinimumClipSpacing();
	bool GetSnapPosition(int64* p_vpPosition, bool p_vAllowSnapToClip, bool p_vSnapAfterClip, EMGClip* p_opExcludeClip = NULL, bool p_vSnapToZero = true, 
							bool p_vSnapToPositionPointer = true, int64 p_vOriginalPosition = -1, bool p_vSnapToOriginalPosition = false);
	const char* GetTimeString(int64 p_vpTime, EMTimeDisplayMode p_eTimeDisplayMode);
	string GetUniqueClipName(const char* p_vpNameSuggestion, EMGClip* p_opClip);
	uint32 GetUniqueID();
	string GetUniqueName();
	static EMGUIUtilities* Instance();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	EMGClip* RemoveFromClipGroup(EMGClip* p_opClipGroupOwner, EMGClip* p_opClipToRemove);
	void ResizeToParent(list<EMGUIComponent*>* p_opChildComponents, EMRect p_oOldParentFrame, EMRect p_oNewParentFrame);
	double Round64(double p_vValue);
	static void SortClips(list<EMGClip*>* p_opClips);
	//void TranslatePosToTime(int64 p_vPosition, char* p_vpTime);
	int64 TruncateFrame(int64 p_vPosition);
	int64 TruncateSample(int64 p_vPosition);

private:
	void CalculateSnappingMode();

private:
	EMEditMode m_eEditMode;
	SnappingMode m_eSnappingMode; // Cached setting
	static EMGUIUtilities* m_opInstance;
	string* m_opSignature; // Cached setting
	string* m_opSnappingMode; // Cached setting
	float m_vFramerate; // Cached setting
	int m_vGridMeasureDivider;
	int64 m_vGridSize;
	int64 m_vMeasureSize;
	int64 m_vMinimumClipSpacing;
	char m_vpTime[20];
	int32 m_vSampleRate; // Cached setting
	int m_vSignatureMajor; // Cached setting
	int m_vSignatureMinor; // Cached setting
	bool m_vSnapping;
	int32 m_vSnapThreshold; // Cached setting
	float m_vTempo; // Cached setting
	int64 m_vTimeZoomScale; // Cached setting
	uint32 m_vUniqueID;
};

#endif