/*******************************************************
* Creator: Martin Rudefelt
* Portability: native
*-------------------------------------------------------
*
*******************************************************/

#include "EMGUIUtilities.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGClip.h"
#include "EMGClipRepository.h"
#include "EMGTrack.h"
#include "EMGUIComponent.h"
#include "EMGUIGlobals.h"
#include "EMMediaGlobals.h"
#include "EMMessages.h"
#include "EMResourceRepository.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "ResourceIDs.h" // FIXME: A framework class should not include an application file
#include "SettingIDs.h" // FIXME: A framework class should not include an application file
 
#include <map>
#include <sstream>

EMGUIUtilities* EMGUIUtilities::m_opInstance = NULL;

//const int32 EM_SNAP_THRESHOLD = 10;

EMGUIUtilities::EMGUIUtilities() :
m_eEditMode(EM_CLIP_EDIT_MODE),
m_eSnappingMode(SNAPPING_CLIPS),
m_opSignature(NULL),
m_opSnappingMode(NULL),
m_vFramerate(0),
m_vGridMeasureDivider(0),
m_vGridSize(0),
m_vMeasureSize(0),
m_vMinimumClipSpacing(-1),
m_vSampleRate(0),
m_vSignatureMajor(0),
m_vSignatureMinor(0),
m_vSnapping(false),
m_vSnapThreshold(0),
m_vTempo(0),
m_vTimeZoomScale(0),
m_vUniqueID(0)
{
	EMSettingsRepository::Instance() -> AddListener(this);
}

// Note: A new clip group can be formed by passing the same clip as both parameters
bool EMGUIUtilities::AddToClipGroup(EMGClip* p_opClipGroupOwner, EMGClip* p_opClipToAdd)
{
	if(p_opClipGroupOwner == NULL || p_opClipToAdd == NULL)
		return false;

	if(p_opClipToAdd == p_opClipGroupOwner && p_opClipGroupOwner)
	{
		if(p_opClipGroupOwner -> GetNextInGroup() != NULL)
			return false; // Already belongs to a group

		EMGClipRepository::Instance() -> AddClipGroup(p_opClipGroupOwner);
		p_opClipGroupOwner -> SetFirstInGroup(p_opClipGroupOwner);
		return true;
	}

	EMGClip* opCurrentClip = p_opClipGroupOwner;
	while(opCurrentClip -> GetNextInGroup() != NULL)
		opCurrentClip = opCurrentClip -> GetNextInGroup();
	opCurrentClip -> SetNextInGroup(p_opClipToAdd);
	p_opClipToAdd -> SetFirstInGroup(p_opClipGroupOwner);
	p_opClipToAdd -> SetNextInGroup(NULL);
	return true;
}

void EMGUIUtilities::CalculateSnappingMode()
{
	int64 vMajorNudgeSpacing = static_cast<int64>((m_vSignatureMajor * EM_TICKS_PER_SECOND * 240) / (m_vSignatureMinor * m_vTempo));
	int64 vMinorNudgeSpacing = vMajorNudgeSpacing / m_vSignatureMajor;

	m_vMeasureSize = vMajorNudgeSpacing;

	if(*m_opSnappingMode == "Grid: Meas.")
	{
		m_vGridSize = vMajorNudgeSpacing;
		m_vGridMeasureDivider = 1;
	}
	else if(*m_opSnappingMode == "Grid: 1/2")
	{
		m_vGridSize = vMajorNudgeSpacing / 2;
		m_vGridMeasureDivider = 2;
	}
	else if(*m_opSnappingMode == "Grid: Bar")
	{
		m_vGridSize = vMinorNudgeSpacing;
		m_vGridMeasureDivider = m_vSignatureMajor;
	}
	else if(*m_opSnappingMode == "Grid: 1/8")
	{
		m_vGridSize = vMajorNudgeSpacing / 8;
		m_vGridMeasureDivider = 8;
	}
	else if(*m_opSnappingMode == "Grid: 1/16")
	{
		m_vGridSize = vMajorNudgeSpacing / 16;
		m_vGridMeasureDivider = 16;
	}
	else if(*m_opSnappingMode == "Grid: 1/32")
	{
		m_vGridSize = vMajorNudgeSpacing / 32;
		m_vGridMeasureDivider = 32;
	}
	else if(*m_opSnappingMode == "Grid: 1/64")
	{
		m_vGridSize = vMajorNudgeSpacing / 64;
		m_vGridMeasureDivider = 64;
	}
	else
		m_vGridSize = 1;
	list<string*>* opModes = EMResourceRepository::GetStringList(RES_TB_SNAPPING_MODES);
	int vIndex = 0;
	list<string*>::iterator oIterator = opModes -> begin();
	while(oIterator != opModes -> end())
	{
		if(**oIterator == *m_opSnappingMode)
		{
			m_eSnappingMode = static_cast<SnappingMode>(vIndex);
			return;
		}
		oIterator++;
		vIndex++;
	}

//	EMDebugger("Could not resolve snapping mode");
}

list<EMGClip*> EMGUIUtilities::GetClipGroup(EMGClip* p_opClip, bool* p_vpHiddenClipsExist)
{
	*p_vpHiddenClipsExist = false;

	list<EMGClip*> oClipGroup;
	if(p_opClip -> GetFirstInGroup() == NULL)
	{
		oClipGroup.push_back(p_opClip);
		return oClipGroup;
	}
	
	EMGClip* opCurrentClip = p_opClip -> GetFirstInGroup();
	while(opCurrentClip/* -> GetNextInGroup()*/ != NULL)
	{
		if(opCurrentClip -> GetTrack() -> GetTrackInfo() -> IsHidden(m_eEditMode == EM_CLIP_EDIT_MODE))
			*p_vpHiddenClipsExist = true;

		oClipGroup.push_back(opCurrentClip);
		opCurrentClip = opCurrentClip -> GetNextInGroup();
	}
	//oClipGroup.push_back(opCurrentClip);

	return oClipGroup;
}

list<EMGClip*> EMGUIUtilities::GetClipGroup(list<EMGClip*>* p_opClips, bool* p_vpHiddenClipsExist)
{
	bool vHiddenClipsExistTemp;
	bool vHiddenClipsExist(false);

	list<EMGClip*> oClipGroup;
	list<EMGClip*>::iterator oIterator = p_opClips -> begin();
	while(oIterator != p_opClips -> end())
	{
		oClipGroup.merge(GetClipGroup(*oIterator, &vHiddenClipsExistTemp));
		if(vHiddenClipsExistTemp)
			vHiddenClipsExist = true;

		oIterator++;
	}

	*p_vpHiddenClipsExist = vHiddenClipsExist;

	return oClipGroup;
}

int64 EMGUIUtilities::GetGridSize()
{
	return m_vGridSize;
}

// Returns how many grids goes into one measure
int EMGUIUtilities::GetGridMeasureDivider()
{
	return m_vGridMeasureDivider;
}

int64 EMGUIUtilities::GetMeasureSize()
{
	return m_vMeasureSize;
}

int64 EMGUIUtilities::GetMinimumClipSpacing()
{
	return m_vMinimumClipSpacing;
}

//// This method does not snap to 0 or to the original position (FIXME: Perhaps it should, and perhaps also to the position pointer?)
// p_vSnapAfterClip = true makes the new position be set to 1 microsecond after a clip if
// the position snaps to the end of a clip, else it snaps 1 microsecond before a clip if
// it snaps to the beginning of a clip
bool EMGUIUtilities::GetSnapPosition(int64* p_vpPosition, bool p_vAllowSnapToClip, bool p_vSnapAfterClip, EMGClip* p_opExcludeClip, bool p_vSnapToZero, 
										bool p_vSnapToPositionPointer, int64 p_vOriginalPosition, bool p_vSnapToOriginalPosition)
{
	if(m_vSnapping)
	{
		//int64 vSnapToZeroPos = INT64_MAX;
		int64 vSnapToPositionPointerPos = INT64_MAX;
		int64 vSnapToSnappingModePos = INT64_MAX;
		int64 vCurrentPosition = INT64_MAX;
		int64 vClosestPosition = INT64_MAX;

		if(p_vSnapToPositionPointer)
			vCurrentPosition = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CURRENT_PROJECT_POSITION, NULL)));//InstanceMediaTimer() -> NowTime();

		if(m_vGridSize > 1)
		{
			int64 vLeftSnap = static_cast<int64>(*p_vpPosition / m_vGridSize) * m_vGridSize;
			int64 vRightSnap = vLeftSnap + m_vGridSize;
			if(*p_vpPosition - vLeftSnap < vRightSnap - *p_vpPosition)
				vSnapToSnappingModePos = vLeftSnap;
			else
				vSnapToSnappingModePos = vRightSnap;
		}

		else if(m_eSnappingMode == SNAPPING_CLIPS && p_vAllowSnapToClip)
		{
			list<EMGClip*>::iterator oIterator = EMGClipRepository::Instance() -> GetClips() -> begin();
			int64 vClosestSnap = INT64_MAX;
			EMGClip* opClosestClip = NULL;
			int64 vClipBegin;
			int64 vClipEnd;
			EMGClip* opCurrentClip = NULL;

			while(oIterator != EMGClipRepository::Instance() -> GetClips() -> end())
			{
				opCurrentClip = *oIterator;
				if(opCurrentClip != p_opExcludeClip)
				{
					vClipBegin = opCurrentClip -> GetPosition();
					vClipEnd = vClipBegin + opCurrentClip -> GetLength() - 1;
//;//cout_commented_out_4_release << "EMGUIUtilities::GetSnapPosition: abs(vClipBegin - *p_vpPosition): " << abs(vClipBegin - *p_vpPosition) << ", abs(vClipEnd - *p_vpPosition): " << abs(vClipEnd - *p_vpPosition) << endl;
					if(Abs64(vClipBegin - *p_vpPosition) < vClosestSnap)
					{
						vClosestSnap = Abs64(vClipBegin - *p_vpPosition);
						opClosestClip = opCurrentClip;
					}
					if(Abs64(vClipEnd - *p_vpPosition) < vClosestSnap)
					{
						vClosestSnap = Abs64(vClipEnd - *p_vpPosition);
						opClosestClip = opCurrentClip;
					}
				}

				oIterator++;
			}

			if(vClosestSnap / m_vTimeZoomScale < m_vSnapThreshold)
			{
				vClipBegin = opClosestClip -> GetPosition();
				vClipEnd = vClipBegin + opClosestClip -> GetLength() - 1;
				if(Abs64(vClipBegin - *p_vpPosition) == vClosestSnap)
				{
					if(p_vSnapAfterClip)
						vSnapToSnappingModePos = vClipBegin;
					else
						vSnapToSnappingModePos = vClipBegin - 1;
				}
				else // Here the snap should be right after the end of the clip snapped to (e.g. one frame after)
				{
					if(p_vSnapAfterClip)
						vSnapToSnappingModePos = vClipEnd + 1;
					else
						vSnapToSnappingModePos = vClipEnd;
				}
			}
//;//cout_commented_out_4_release << "EMGUIUtilities::GetSnapPosition: vClosestSnap: " << vClosestSnap << ", opAffectedClips: " << opAffectedClips -> size() << ", EMGClipRepository::Instance() -> GetClips(): " << EMGClipRepository::Instance() -> GetClips() -> size() << endl;
		}

		if(p_vSnapToZero)
		{
			if(*p_vpPosition < 0) // Prevent positions less than 0
			{
				*p_vpPosition = 0;
				return true;
			}
			vClosestPosition = 0;
		}

		if(p_vSnapToPositionPointer)
			if(Abs64(vCurrentPosition - *p_vpPosition) < Abs64(vClosestPosition - *p_vpPosition))
				vClosestPosition = vCurrentPosition;

		if(p_vSnapToOriginalPosition)
			if(Abs64(p_vOriginalPosition - *p_vpPosition) < Abs64(vClosestPosition - *p_vpPosition))
				vClosestPosition = p_vOriginalPosition;

		if(Abs64(vSnapToSnappingModePos - *p_vpPosition) < Abs64(vClosestPosition - *p_vpPosition))
			vClosestPosition = vSnapToSnappingModePos;

		if(Abs64(vClosestPosition - *p_vpPosition) / m_vTimeZoomScale < m_vSnapThreshold)
		{
			*p_vpPosition = vClosestPosition;
			return true;
		}
	}

	return false;
}

const char* EMGUIUtilities::GetTimeString(int64 p_vpTime, EMTimeDisplayMode p_eTimeDisplayMode)
{
	float vFramerate = *(static_cast<float*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_FRAMERATE)));
	switch(p_eTimeDisplayMode)
	{
	case EM_SMPTE:
		{
			int vMinutes = static_cast<int>(p_vpTime / 60000000L);
			int vSeconds = static_cast<int>((p_vpTime % 60000000L) / 1000000L);
			int vFrames = static_cast<int>((p_vpTime % 1000000) * vFramerate / 1000000L);
			sprintf(m_vpTime, "%03d:%02d:%02d", vMinutes, vSeconds, vFrames);
			break;
		}
	case EM_MEASURES:
		{
			int vMeasures = static_cast<int>(p_vpTime / 60000000L/*m_vSignatureMajorSpacing*/)/* + 1*/;
			int vBeats = static_cast<int>((p_vpTime % 60000000L/*m_vSignatureMajorSpacing*/) / 1000000L/*m_vSignatureMinorSpacing*/)/* + 1*/;
			int vFrames = static_cast<int>((p_vpTime % 1000000) * vFramerate / 1000000L);
			sprintf(m_vpTime, "%03d:%02d:%02d"/*"%04d:%01d:%02d"*/, vMeasures, vBeats, vFrames);
			break;
		}
	}
	return m_vpTime;
}

string EMGUIUtilities::GetUniqueClipName(const char* p_vpNameSuggestion, EMGClip* p_opClip)
{
	list<EMGClip*>* opClips = EMGClipRepository::Instance() -> GetClips();
	string oName(p_vpNameSuggestion);
	list<EMGClip*>::iterator oIterator;
	bool vFoundName;
	char vDummyString[10];
	int vInstanceNumber;
	//int vInstanceNumberPos;
	int vLeftBracketPos;
	int vRightBracketPos;
	string oBracketSubString;
	bool vFoundInstanceNr;

	while(true)
	{
		vFoundName = false;
		oIterator = opClips -> begin();
		while(oIterator != opClips -> end())
		{
			if((*oIterator) != p_opClip && oName == (*oIterator) -> GetLabel())
			{
				vFoundName = true;
				break;
			}
			oIterator++;
		}

		if(!vFoundName)
			break;

		vFoundInstanceNr = false;

		vLeftBracketPos = oName.find_last_of('[');
		vRightBracketPos = oName.find_last_of(']');
		if(vLeftBracketPos != string::npos && vRightBracketPos != string::npos && vLeftBracketPos < vRightBracketPos && vRightBracketPos == oName.length() - 1)
		{
			oBracketSubString = oName.substr(vLeftBracketPos, vRightBracketPos);
			if(sscanf(oBracketSubString.c_str(), "[%d]", &vInstanceNumber) == 1)
				vFoundInstanceNr = true;
		}

		if(vFoundInstanceNr)
		{
			// Add 1 to instance nr
			vInstanceNumber++;
			sprintf(vDummyString, " [%d]", vInstanceNumber);
			oName = oName.substr(0, vLeftBracketPos - 1) + vDummyString;
		}

		else
			oName = oName + " [2]"; // The first instance number is 2 since the "original" clip is nr 1
	}

	return oName;
}

uint32 EMGUIUtilities::GetUniqueID()
{
	m_vUniqueID++;
	return m_vUniqueID;
}

string EMGUIUtilities::GetUniqueName()
{
	ostringstream oStream;
	oStream << "UniqueName" << GetUniqueID();
	return oStream.str();
}

EMGUIUtilities* EMGUIUtilities::Instance()
{
	if(m_opInstance == NULL)
		m_opInstance = EM_new EMGUIUtilities();
	return m_opInstance;
}

bool EMGUIUtilities::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	if(p_vMessage == EM_SETTING_UPDATED)
	{
eo << "EMGUIUtilities: EM_SETTING_UPDATED" << ef;
		m_eEditMode = static_cast<EMEditMode>(*(static_cast<int*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_EDIT_MODE))));
		m_vFramerate = *(static_cast<float*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_FRAMERATE)));
		m_vSnapping = *(static_cast<bool*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SNAPPING)));
		m_vTimeZoomScale = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TIME_ZOOM_SCALE)));
		float vTempo = *(static_cast<float*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TEMPO)));
		m_vSnapThreshold = *(static_cast<int32*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SNAP_THRESHOLD)));
		string* opSignature = static_cast<string*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SIGNATURE));
		m_vSampleRate = *(static_cast<int32*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_AUDIO_FRAMERATE)));
		m_vMinimumClipSpacing = EM_TICKS_PER_SECOND / m_vSampleRate + 1;
		string* opSnappingMode = static_cast<string*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SNAPPING_MODE));
//;//cout_commented_out_4_release << "opString: " << opString << endl;
		if(opSnappingMode != m_opSnappingMode || vTempo != m_vTempo || opSignature != m_opSignature)
		{
			m_opSnappingMode = opSnappingMode;
			m_vTempo = vTempo;
			m_opSignature = opSignature;
			sscanf(m_opSignature -> c_str(), "%d/%d", &m_vSignatureMajor, &m_vSignatureMinor);
			CalculateSnappingMode();
		}
		return true;
	}
	return false;
}

// Returns the new clip group owner (if any)
EMGClip* EMGUIUtilities::RemoveFromClipGroup(EMGClip* p_opClipGroupOwner, EMGClip* p_opClipToRemove)
{
	if(p_opClipGroupOwner == NULL)
		return NULL;

	if(p_opClipToRemove == NULL)
		return p_opClipGroupOwner;

	if(p_opClipGroupOwner -> GetFirstInGroup() == NULL)
		return NULL;

	EMGClip* opCurrentClip;

	if(p_opClipToRemove == p_opClipGroupOwner)
	{
		EMGClipRepository::Instance() -> RemoveClipGroup(p_opClipGroupOwner);
		EMGClip* opNewGroupOwner = p_opClipGroupOwner -> GetNextInGroup();
		p_opClipGroupOwner -> SetFirstInGroup(NULL);
		p_opClipGroupOwner -> SetNextInGroup(NULL);
		if(opNewGroupOwner -> GetNextInGroup() == NULL) // If one clip left after removal, no new clip group should be formed
		{
			opNewGroupOwner -> SetFirstInGroup(NULL);
			opNewGroupOwner -> SetNextInGroup(NULL);
			return NULL;
		}

		opCurrentClip = opNewGroupOwner;
		while(opCurrentClip != NULL)
		{
			opCurrentClip -> SetFirstInGroup(opNewGroupOwner);
			opCurrentClip = opCurrentClip -> GetNextInGroup();
		}

		return opNewGroupOwner;
	}

	else
	{
		opCurrentClip = p_opClipGroupOwner -> GetNextInGroup();
		if(opCurrentClip -> GetNextInGroup() == NULL) // If one clip left after removal, no new clip group should be formed
		{
			EMGClipRepository::Instance() -> RemoveClipGroup(p_opClipGroupOwner);
			p_opClipGroupOwner -> SetFirstInGroup(NULL);
			p_opClipGroupOwner -> SetNextInGroup(NULL);
			opCurrentClip -> SetFirstInGroup(NULL);
			opCurrentClip -> SetNextInGroup(NULL);
			return NULL;
		}

		EMGClip* opPreviousClip = p_opClipGroupOwner;
		while(opCurrentClip != NULL)
		{
			if(opCurrentClip == p_opClipToRemove)
			{
				opPreviousClip -> SetNextInGroup(opCurrentClip -> GetNextInGroup());
				opCurrentClip -> SetFirstInGroup(NULL);
				opCurrentClip -> SetNextInGroup(NULL);
				return p_opClipGroupOwner;
			}
			opCurrentClip = opCurrentClip -> GetNextInGroup();
		}
		
		return NULL; // Clip not found
	}
}

void EMGUIUtilities::ResizeToParent(list<EMGUIComponent*>* p_opChildComponents, EMRect p_oOldParentFrame, EMRect p_oNewParentFrame)
{
	uint32 vMode;
	EMRect oChildFrame;
	list<EMGUIComponent*>::iterator oIterator = p_opChildComponents -> begin();
	while(oIterator != p_opChildComponents -> end())
	{
		vMode = (*oIterator) -> GetResizingMode();
		oChildFrame = (*oIterator) -> Frame();
		if(oChildFrame.IsValid() && vMode != 0)
		{
/*
eo << "Resizing (old parent/EM_new parent/old child/EM_new child):" << ef;
p_oOldParentFrame.Print();
p_oNewParentFrame.Print();
oChildFrame.Print();
*/
			if(vMode & EM_FOLLOW_RIGHT)
				oChildFrame.OffsetBy(p_oNewParentFrame.GetWidth() - p_oOldParentFrame.GetWidth(), 0);
			else if(vMode & EM_FOLLOW_H_CENTER)
				oChildFrame.OffsetBy((p_oNewParentFrame.GetWidth() / 2) - (p_oOldParentFrame.GetWidth() / 2), 0);
			else if(vMode & EM_FOLLOW_LEFT_RIGHT)
				oChildFrame.m_vRight += p_oNewParentFrame.GetWidth() - p_oOldParentFrame.GetWidth();
			if(vMode & EM_FOLLOW_BOTTOM)
				oChildFrame.OffsetBy(0, p_oNewParentFrame.GetHeight() - p_oOldParentFrame.GetHeight());
			else if(vMode & EM_FOLLOW_V_CENTER)
				oChildFrame.OffsetBy(0, (p_oNewParentFrame.GetHeight() / 2) - (p_oOldParentFrame.GetHeight() / 2));
			else if(vMode & EM_FOLLOW_TOP_BOTTOM)
				oChildFrame.m_vBottom += p_oNewParentFrame.GetHeight() - p_oOldParentFrame.GetHeight();
			(*oIterator) -> SetFrame(oChildFrame);
//oChildFrame.Print();				
		}
		oIterator++;
	}
}
/*
class CompareClipPositions
{
public:
	int operator() (EMGClip* p_opClipOne, EMGClip* p_opClipTwo)
	{
		return p_opClipOne -> GetPosition() < p_opClipTwo -> GetPosition();
	}
};
*/

double EMGUIUtilities::Round64(double p_vValue)
{
	if(p_vValue - static_cast<int64>(p_vValue) >= 0.5)
		return static_cast<int64>(p_vValue) + 1;
	return static_cast<int64>(p_vValue);
}

// FIXME: Visual C++ doesn't like the standard STL list sort, make a decent alternative
void EMGUIUtilities::SortClips(list<EMGClip*>* p_opClips)
{
	multimap<int64, EMGClip*> oSortingMap;
	list<EMGClip*>::iterator oListIterator = p_opClips -> begin();
	while(oListIterator != p_opClips -> end())
	{
		oSortingMap.insert(pair<int64, EMGClip*>((*oListIterator) -> GetPosition(), *oListIterator));
		oListIterator++;
	}
	multimap<int64, EMGClip*>::iterator oMapIterator = oSortingMap.begin();
	p_opClips -> clear();
	while(oMapIterator != oSortingMap.end())
	{
		p_opClips -> push_back(oMapIterator -> second);
		oMapIterator++;
	}
}

/*
// The time string will be in the format mmm:ss:ff
void EMGUIUtilities::TranslatePosToTime(int64 p_vPosition, char* p_vpTime)
{
	int vMinutes = p_vPosition / (EM_TICKS_PER_SECOND * 60);
	int vSeconds = (p_vPosition / EM_TICKS_PER_SECOND) - (vMinutes * EM_TICKS_PER_SECOND * 60);
	int vFrames = ((p_vPosition / EM_TICKS_PER_SECOND) - static_cast<int32>(p_vPosition / EM_TICKS_PER_SECOND)) * EM_TICKS_PER_SECOND / (EM_TICKS_PER_SECOND / m_vFramerate);
	sprintf(p_vpTime, "%03d:%02d:%02d", vMinutes, vSeconds, vFrames);
}
*/
int64 EMGUIUtilities::TruncateFrame(int64 p_vPosition)
{
	return Round64(Round64(static_cast<double>(p_vPosition * m_vSampleRate) / EM_TICKS_PER_SECOND) * EM_TICKS_PER_SECOND / m_vSampleRate);
	/*
	if(static_cast<float>(p_vPosition) * m_vSampleRate / EM_TICKS_PER_SECOND - static_cast<int64>(p_vPosition * m_vSampleRate / EM_TICKS_PER_SECOND) >= 0.5)
		return static_cast<int64>(p_vPosition * m_vSampleRate / EM_TICKS_PER_SECOND + 1) * EM_TICKS_PER_SECOND / m_vSampleRate;
	else
		return static_cast<int64>(p_vPosition * m_vSampleRate / EM_TICKS_PER_SECOND) * EM_TICKS_PER_SECOND / m_vSampleRate;
	*/
	//return static_cast<int64>(p_vPosition * m_vSampleRate / EM_TICKS_PER_SECOND) * EM_TICKS_PER_SECOND / m_vSampleRate; // Round to nearest media engine frame
/*
	int64 vLeftSnap;
	int64 vRightSnap;
	if(p_vPosition > 0)
	{
		vLeftSnap = static_cast<int64>(((static_cast<int64>((p_vPosition * m_vFramerate) / EM_TICKS_PER_SECOND)) * EM_TICKS_PER_SECOND) / m_vFramerate);
		vRightSnap = static_cast<int64>((((static_cast<int64>((p_vPosition * m_vFramerate) / EM_TICKS_PER_SECOND)) + 1) * EM_TICKS_PER_SECOND) / m_vFramerate);
	}
	else
	{
		vLeftSnap = static_cast<int64>((((static_cast<int64>((p_vPosition * m_vFramerate) / EM_TICKS_PER_SECOND)) - 1) * EM_TICKS_PER_SECOND) / m_vFramerate);
		vRightSnap = static_cast<int64>(((static_cast<int64>((p_vPosition * m_vFramerate) / EM_TICKS_PER_SECOND)) * EM_TICKS_PER_SECOND) / m_vFramerate);
	}
//eo << "EMGUIUtilities::TruncateFrame: p_vPosition: " << (int32) p_vPosition << ", vLeftSnap: " << (int32) vLeftSnap << ", vRightSnap: " << (int32) vRightSnap << ef;
	if(p_vPosition - vLeftSnap < vRightSnap - p_vPosition)
		return vLeftSnap;
	return vRightSnap;
*/
}

int64 EMGUIUtilities::TruncateSample(int64 p_vPosition)
{
	return Round64(Round64(static_cast<double>(p_vPosition * m_vSampleRate) / EM_TICKS_PER_SECOND) * EM_TICKS_PER_SECOND / m_vSampleRate);
	/*
	if(static_cast<float>(p_vPosition) * m_vSampleRate / EM_TICKS_PER_SECOND - static_cast<int64>(p_vPosition * m_vSampleRate / EM_TICKS_PER_SECOND) >= 0.5)
		return static_cast<int64>(p_vPosition * m_vSampleRate / EM_TICKS_PER_SECOND + 1) * EM_TICKS_PER_SECOND / m_vSampleRate;
	else
		return static_cast<int64>(p_vPosition * m_vSampleRate / EM_TICKS_PER_SECOND) * EM_TICKS_PER_SECOND / m_vSampleRate;
	*/
	//return static_cast<int64>(p_vPosition * m_vSampleRate / EM_TICKS_PER_SECOND) * EM_TICKS_PER_SECOND / m_vSampleRate; // Round to nearest media engine frame
/*	
	int64 vLeftSnap;
	int64 vRightSnap;
	if(p_vPosition > 0)
	{
		vLeftSnap = static_cast<int64>(((static_cast<int64>((p_vPosition * m_vSampleRate) / EM_TICKS_PER_SECOND)) * EM_TICKS_PER_SECOND) / m_vSampleRate);
		vRightSnap = static_cast<int64>((((static_cast<int64>((p_vPosition * m_vSampleRate) / EM_TICKS_PER_SECOND)) + 1) * EM_TICKS_PER_SECOND) / m_vSampleRate);
	}
	else
	{
		vLeftSnap = static_cast<int64>((((static_cast<int64>((p_vPosition * m_vSampleRate) / EM_TICKS_PER_SECOND)) - 1) * EM_TICKS_PER_SECOND) / m_vSampleRate);
		vRightSnap = static_cast<int64>(((static_cast<int64>((p_vPosition * m_vSampleRate) / EM_TICKS_PER_SECOND)) * EM_TICKS_PER_SECOND) / m_vSampleRate);
	}
//;//cout_commented_out_4_release << "EMGUIUtilities::TruncateSample: p_vPosition: " << p_vPosition << ", vLeftSnap: " << vLeftSnap << ", vRightSnap: " << vRightSnap << endl;
	if(p_vPosition - vLeftSnap < vRightSnap - p_vPosition)
		return vLeftSnap;
	return vRightSnap;
*/
}



