#include "EMGClipContainer.h"

#include "EMGClipRepository.h"
#include "EMGTrack.h"
#include "EMKeyboard.h"
#include "EMKeyboardGlobals.h"
#include "EMMessages.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"

EMGClipContainer* EMGClipContainer::m_opClipContainerWithSelection = NULL;
EMRect EMGClipContainer::m_oSelectionRect = EMRect();
EMRect EMGClipContainer::m_oOldSelectionRect = EMRect();
//int32 EMGClipContainer::m_vMouseDownX = 0;
//int32 EMGClipContainer::m_vSnappedMouseDownX = 0;
int64 EMGClipContainer::m_vMouseDownXPos = 0;
int64 EMGClipContainer::m_vSelectionLeftXPos = 0;
int64 EMGClipContainer::m_vSelectionRightXPos = 0;

EMGClipContainer::EMGClipContainer(EMRect p_oFrame, EMView* p_opView) :
m_oFrame(p_oFrame),
m_opView(p_opView)
{
//	EMSettingsRepository::Instance() -> AddListener(this);
	m_vTimeZoomScale = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TIME_ZOOM_SCALE)));
}

EMGClipContainer::~EMGClipContainer()
{
	if(m_opClipContainerWithSelection == this)
		m_opClipContainerWithSelection = NULL;
//	EMSettingsRepository::Instance() -> RemoveListener(this);
}

void EMGClipContainer::AddClip(EMGClip* p_opClip)
{
	// Insert the clip in a sorted manner
	if(m_oClipRepository.size() == 0)
		m_oClipRepository.push_front(p_opClip);

	else if(m_oClipRepository.front() -> GetPosition() > p_opClip -> GetPosition())
		m_oClipRepository.push_front(p_opClip);

	else if(m_oClipRepository.back() -> GetPosition() <= p_opClip -> GetPosition())
		m_oClipRepository.push_back(p_opClip);

	else
	{
		list<EMGClip*>::iterator oIterator = m_oClipRepository.begin();
		oIterator++;
		while(oIterator != m_oClipRepository.end())
		{
			if(p_opClip -> GetPosition() < (*oIterator) -> GetPosition())
			{
				m_oClipRepository.insert(oIterator, p_opClip);
				break;
			}
			oIterator++;
		}
	}
}

EMRect EMGClipContainer::CalculateClipFrame(EMGClip* p_opClip)
{
	return CalculateClipFrame(p_opClip, 0);
}

// Used by the clips to calculate their bounds and their moved bounds
EMRect EMGClipContainer::CalculateClipFrame(EMGClip* p_opClip, int64 p_vClipOffsetX)//int32 p_vOffsetX)
{
//	EMTrackView* opView = EMTrackView::Instance();
	int64 vPosition = p_opClip -> GetPosition();
	int32 vLeft = (vPosition + p_vClipOffsetX) / m_vTimeZoomScale;
//	if(vLeft < 0)
//		vLeft = 0; // Due to a rounding error, the clip can end up slightly outside the view -> kaboom!
/*
;//cout_commented_out_4_release << "New clip rect:";
EMRect(vLeft,
	              m_oFrame.m_vTop + GetUpperBorderSize(),
	              opView -> TranslateToPixels(p_opClip -> GetPosition() + 
	              p_opClip -> GetLength()) + p_vOffsetX,
	              m_oFrame.m_vBottom - GetLowerBorderSize()).Print();
*/
	return EMRect(vLeft,
	              m_oFrame.m_vTop + GetUpperBorderSize(),
	              (vPosition + p_vClipOffsetX + p_opClip -> GetLength()) / m_vTimeZoomScale,
	              m_oFrame.m_vBottom - GetLowerBorderSize());
}

void EMGClipContainer::ClearClips()
{
	m_oClipRepository.clear();
}

EMRect EMGClipContainer::Frame()
{
	return m_oFrame;
}

EMRect EMGClipContainer::GetAndClearUpdateRect()
{
	return m_oUpdateRect;
	m_oUpdateRect = EMRect();
}

EMGClip* EMGClipContainer::GetClip(int32 p_vX, int32 p_vY)
{
	list<EMGClip*>::reverse_iterator oIterator = m_oClipRepository.rbegin();
	while(oIterator != m_oClipRepository.rend())
	{
		if(((*oIterator) -> Frame()).Contains(p_vX, p_vY))
			return *oIterator;
		oIterator++;
	}
	return NULL;
}

EMGClipContainer* EMGClipContainer::GetClipContainerWithSelection()
{
	return m_opClipContainerWithSelection;
}

list<EMGClip*>* EMGClipContainer::GetClips()
{
	return &m_oClipRepository;
}

EMGTrack* EMGClipContainer::GetTrack()
{
	return m_opTrack;
}

bool EMGClipContainer::OkToDrop(EMGClip* p_opClip, int64 p_vPosition, int64 p_vLength, bool p_vAlwaysIncludeSelected)
{
	if(p_opClip != NULL) // If p_opClip == NULL, it is assumed that these things are already checked before/after calling this method
	{
		if(p_opClip -> GetType() != m_opTrack -> GetType())
			return false; // Must have clips of a matching type on a track
		
		if(p_opClip -> GetType() == EM_TYPE_ANY_AUDIO || p_opClip -> GetType() == EM_TYPE_MIDI)
			return true; // Audio and MIDI clips are allowed to overlap 
	}

	bool vIsOk = true;
	list<EMGClip*>::iterator oIterator = m_oClipRepository.begin();

	uint32 vModifiers = EMKeyboard::Instance() -> GetModifiers();

	while(oIterator != m_oClipRepository.end())
	{
//eo << "Comparing: " << (int32) p_vPosition << ", " << (int32) (p_vPosition + p_vLength - 1) << "; " << (int32) ((*oIterator) -> GetPosition()) << ", " << (int32) ((*oIterator) -> GetPosition() + (*oIterator) -> GetLength() - 1) << ef;
		if(p_vAlwaysIncludeSelected || !((*oIterator) -> GetSelected()) || (vModifiers & EM_CONTROL_KEY) == EM_CONTROL_KEY)
			if(!(p_vPosition > (*oIterator) -> GetPosition() + (*oIterator) -> GetLength() - 1 || 
			   p_vPosition + p_vLength - 1 < (*oIterator) -> GetPosition()))
			{
				vIsOk = false;
				break;
			}
		oIterator++;
	}
	return vIsOk;
}

void EMGClipContainer::RemoveClip(EMGClip* p_opClip)
{
	m_oClipRepository.remove(p_opClip);
}

// Returns true if the new frame differs from the old one
bool EMGClipContainer::SetFrame(EMRect p_oFrame)
{
	if(p_oFrame == m_oFrame)
		return false;
	m_oFrame = p_oFrame;

	list<EMGClip*>::iterator oIterator = m_oClipRepository.begin();
	while(oIterator != m_oClipRepository.end())
	{
		(*oIterator) -> SetFrame(CalculateClipFrame(*oIterator));
		oIterator++;
	}
	
	return true;
}

void EMGClipContainer::SetTrack(EMGTrack* p_opTrack)
{
	m_opTrack = p_opTrack;
}
/*
void EMGClipContainer::UpdateRepositories()
{
	EMGClipRepository::Instance() -> GetClips(m_opTrack, &m_oClipRepository);
}
*/
void EMGClipContainer::UpdateTimeScale(int64 p_vTimeScale)
{
	m_vTimeZoomScale = p_vTimeScale;
	list<EMGClip*>::iterator oIterator = m_oClipRepository.begin();
	while(oIterator != m_oClipRepository.end())
	{
		(*oIterator) -> SetFrame(CalculateClipFrame(*oIterator));
		oIterator++;
	}
}
