#include "AudioGClip.h"

#include "AudioGClipContainer.h"
#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMGClipContainer.h"
#include "EMGClipRepository.h"
#include "EMGTrack.h"
#include "EMGTrackRepository.h"
#include "EMGUIGlobals.h"
#include "EMGUIUtilities.h"
#include "EMImageBufferRepository.h"
#include "EMMediaGlobals.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMView.h"
#include "GUIGlobals.h"
#include "SettingIDs.h"
  
#include <vector>

#define COLOR_WAVE_LINE EMColor(80, 150, 80, 255)
#define COLOR_WAVE_LINE_SELECTED EMColor(200, 255, 200, 255)
#define COLOR_WAVE EMColor(80, 150, 80, 255)
#define COLOR_WAVE_SELECTED EMColor(200, 255, 200, 255)
#define COLOR_WAVE_LINE_CLIP_EDIT EMColor(255, 0, 0, 255)
#define COLOR_WAVE_CLIP_EDIT EMColor(0, 155, 0, 255)

// Fixme: Fix the text placement calculations
const int32 NAME_X_OFFSET = 3;
const int32 NAME_Y_OFFSET = -2;
const int64 DRAW_LOW_RES_ZOOM_THRESHOLD = 2000;

AudioGClip::AudioGClip(string p_oClipName, int64 p_vPosition, int64 p_vLength, EMGTrack* p_opTrack, int32 p_vMediaClipID, EMView* p_opView) :
GClip(p_oClipName, p_vPosition, p_vLength, p_opTrack, p_vMediaClipID, p_opView),
m_opFXNodes(NULL),
m_vIsInitialized(false),
m_vLowResContent(NULL),
//m_vMediaEffectTrackID(-1),
m_vNamePositionUpdated(false)
{
	if(p_opTrack != NULL)
	{
		m_vAudioSampleRate = *(static_cast<int32*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_AUDIO_FRAMERATE)));
		m_vChannels = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_NUM_CHANNELS, &p_vMediaClipID)));
	}

	if(p_vMediaClipID > 0)
	{
		// If the clip is reconstructed from a saved project or is a clone of another clip, it may have a chain of effects, let's find that out
		EMMediaClipInfo* opMediaClipInfo = static_cast<EMMediaClipInfo*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_INFO, &p_vMediaClipID));
		if(opMediaClipInfo -> m_vMediaEffectTrackID != -1) // It has an effect track
		{
			m_opFXNodes = EM_new list<FXNode>();

			int32 vEffectTrackID = opMediaClipInfo -> m_vMediaEffectTrackID;
			EMMediaEffectTrackInfo* opEffectTrackInfo = static_cast<EMMediaEffectTrackInfo*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_EFFECT_TRACK_INFO, &vEffectTrackID));
			list<int32>* opMediaEffectIDs = &(opEffectTrackInfo -> m_oMediaEffectIDs);
			list<int32>::iterator oMediaEffectIDIterator = opMediaEffectIDs -> begin();
			int vNodeIndex(0);
			int32 vMediaClipID = GetMediaClipID();

			while(oMediaEffectIDIterator != opMediaEffectIDs -> end())
			{
				int32 vEffectID = *oMediaEffectIDIterator;
				EMMediaEffectInfo* opEffectInfo = static_cast<EMMediaEffectInfo*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_EFFECT_INFO, &vEffectID));
				// Insert an empty effect node between active effect nodes
				m_opFXNodes -> push_back(FXNode(EM_TYPE_ANY_AUDIO, vNodeIndex, Frame(), m_opView, GetTrack() -> GetClipContainer(), vEffectTrackID));
				vNodeIndex++;
				m_opFXNodes -> push_back(FXNode(EM_TYPE_ANY_AUDIO, vNodeIndex, Frame(), m_opView, GetTrack() -> GetClipContainer(), vEffectTrackID, opEffectInfo -> m_vEntryID, vEffectID, (opEffectInfo -> m_oPluginName).c_str()));
				vNodeIndex++;
				oMediaEffectIDIterator++;
			}

			// Insert an empty node at the end of the chain
			m_opFXNodes -> push_back(FXNode(EM_TYPE_ANY_AUDIO, vNodeIndex, Frame(), m_opView, GetTrack() -> GetClipContainer(), vEffectTrackID));
		}
	}
}

AudioGClip::~AudioGClip()
{
	delete m_opFXNodes;
}

EMRect AudioGClip::CalculateFrame(uint32 p_vTrackOffset, int64 p_vClipOffsetX)
{
	p_vClipOffsetX = EMGUIUtilities::Instance() -> TruncateSample(p_vClipOffsetX);
	return EMGClip::CalculateFrame(p_vTrackOffset, p_vClipOffsetX);
}

void AudioGClip::CalculateNamePosition(EMView* p_opView)
{
	int32 vAscent;
	int32 vDescent;
	if(p_opView -> LockWindow())
	{
		p_opView -> SetFont(EM_PLAIN);
		p_opView -> CalculateStringHeight(m_oName.c_str(), vAscent, vDescent);
		m_vStringTop = vAscent + NAME_Y_OFFSET;
		m_vStringLeft = NAME_X_OFFSET;
		int vLength = p_opView -> CalculateStringWidth(m_oName.c_str());
		m_oLabelBackground = EMRect(2, 2, 2 + vLength, 12);
		m_vNamePositionUpdated = true;

		p_opView -> UnlockWindow();
	}
/*	
	int32 vYSpacing = (m_oFrame.GetHeight() - (vAscent + vDescent)) / 2;
	m_vStringTop = m_oFrame.m_vTop + vYSpacing + vAscent + SLIM_BUTTON_BORDER_SIZE;
	m_vStringLeft = m_oFrame.m_vLeft + ((m_oFrame.GetWidth() - m_opView -> CalculateStringWidth(m_opOnString -> c_str())) / 2);
*/
}

EMGClip* AudioGClip::CloneClip(int32 p_vTrackOffset, int64 p_vPositionOffset)
{
	EMGTrackRepository* opTrackRepository = EMGTrackRepository::Instance();
	EMGClipRepository* opClipRepository = EMGClipRepository::Instance();
	EMGTrack* opNewTrack = opTrackRepository -> GetTrack(opTrackRepository -> GetTrackIndex(GetTrack()) + p_vTrackOffset);
//	string oLabel = GetLabel() + " (copy)";
	//p_vPositionOffset = EMGUIUtilities::Instance() -> TruncateSample(p_vPositionOffset);
	int64 vNewPosition = GetPosition() + p_vPositionOffset;
	vNewPosition = EMGUIUtilities::Instance() -> TruncateSample(vNewPosition);


	int32 vTargetMediaTrackID = opNewTrack -> GetTrackInfo() -> GetMediaTrackID();
	int32* vpMediaClipID = static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_CLONE_CLIP, &m_vMediaClipID, &vNewPosition, &vTargetMediaTrackID));
	int32 vMediaClipID = *vpMediaClipID;
	EMGClip* opNewClip = opClipRepository -> CreateClip(GetLabel(), GetType(), vNewPosition, GetLength()/*, sNewClipInfo*/, opNewTrack, vMediaClipID, m_opView);

	EMImageFileInfo oInfo = EMImageBufferRepository::Instance() -> GetFileInfo(this);
	EMImageBufferRepository::Instance() -> ClipAdded(opNewClip, oInfo.m_vFileID);

//	opClipRepository -> SetSelected(this, false);
//	opClipRepository -> SetSelected(opNewClip, true);

	return opNewClip;
}

EMGClip* AudioGClip::CreateInstance(string p_oClipName, int64 p_vPosition, int64 p_vLength, EMGTrack* p_opTrack, int32 p_vMediaClipID, EMView* p_opView)
{
	return EM_new AudioGClip(EMGUIUtilities::Instance() -> GetUniqueClipName(p_oClipName.c_str(), NULL), p_vPosition, p_vLength, p_opTrack, p_vMediaClipID, p_opView);
}

void AudioGClip::DrawClip(EMRect p_oClippingRect, EMView* p_opView, bool p_vClipEditMode, int64 p_vZoomScale, bool p_vDrawContent)
{
	if(!p_oClippingRect.Intersects(m_oFrame))
		return;
/*
eo << "Audio clip clipping rect: ";
p_oClippingRect.Print();
m_oFrame.Print();
(m_oFrame & p_oClippingRect).Print();
*/
	if(!m_vNamePositionUpdated)
		CalculateNamePosition(p_opView);

	EMRect oDrawRect = m_oFrame & p_oClippingRect;

	p_opView -> ConstrainClippingRect(m_oFrame & p_oClippingRect);

	if(p_vClipEditMode)
	{
		//p_opView -> SetHighColor(EMColor(255, 255, 255, 255));
		//p_opView -> FillRect(oDrawRect);
		p_opView -> SetHighColor(EMColor(0, 0, 0, 255));
		p_opView -> StrokeRect(m_oFrame);
	}
	else
	{
		if(m_vSelected)
		{
			p_opView -> SetHighColor(EM_COLOR_SELECTED_AUDIO_VIEW_BACKGROUND);
			p_opView -> FillRect(oDrawRect);
		}
		else
		{
			p_opView -> SetHighColor(EM_COLOR_AUDIO_VIEW_BACKGROUND);
			p_opView -> FillRect(oDrawRect);
		}	
	}
	
	if(p_vDrawContent || p_vClipEditMode)
	{
		int32 vLeftPixel;
		int32 vRightPixel;
		if(p_oClippingRect.m_vLeft < m_oFrame.m_vLeft)
			vLeftPixel = m_oFrame.m_vLeft;
		else
			vLeftPixel = p_oClippingRect.m_vLeft;
		if(p_oClippingRect.m_vRight > m_oFrame.m_vRight)
			vRightPixel = m_oFrame.m_vRight;
		else
			vRightPixel = p_oClippingRect.m_vRight;
		if(vLeftPixel > vRightPixel)
			vLeftPixel = vRightPixel;
		if(vLeftPixel != m_oFrame.m_vLeft)
			vLeftPixel--;
		if(vRightPixel == m_oFrame.m_vRight)
			vRightPixel--;

		int32 vMediaClipID = GetMediaClipID();
		int64 vClipMarkIn = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_MARK_IN, &vMediaClipID)));

		int64 vFileBeginPosition = static_cast<int64>(vLeftPixel) * p_vZoomScale - GetPosition() + vClipMarkIn;
		int64 vClipBeginPosition = static_cast<int64>(vLeftPixel) * p_vZoomScale;
		// Buffer one pixel worth of data more than requested since the content drawing routine sometimes
		// needs to look ahead into the next chunk
		int64 vFileEndPosition = static_cast<int64>(vRightPixel + 1) * p_vZoomScale - GetPosition() + vClipMarkIn;
		int64 vClipEndPosition = static_cast<int64>(vRightPixel) * p_vZoomScale;
//		if(vBeginPosition > vEndPosition)
//			vBeginPosition = vEndPosition;


		if(p_vZoomScale > DRAW_LOW_RES_ZOOM_THRESHOLD)
			DrawContent(vFileBeginPosition, vFileEndPosition, vClipBeginPosition, vClipEndPosition, p_vZoomScale, true, p_vClipEditMode);
		else
		{
			// FIXME: since BeginPaint is called previously, another BeginPaint can be called by another thread here when unlocking the semaphore without the first
			//        BeginPaint being finished by an EndPaint, make a better design here (perhaps the same situation is present in other locations?)
			//// The window semaphore is temporarily unlocked to avoid deadlock with the semaphore set in VideoDataAvailable
			//m_opView -> UnlockWindow();
			bool vDataAvailable = EMImageBufferRepository::Instance() -> AudioDataAvailable(this, vFileBeginPosition, vFileEndPosition);
			//m_opView -> LockWindow();
			if(vDataAvailable)
				DrawContent(vFileBeginPosition, vFileEndPosition, vClipBeginPosition, vClipEndPosition, p_vZoomScale, false, p_vClipEditMode);
			else
			{
				//// The window semaphore is temporarily unlocked to avoid deadlock with the semaphore set in Invalidate
				//m_opView -> UnlockWindow();
				EMImageBufferRepository::Instance() -> Invalidate(this, &vClipBeginPosition, &vClipEndPosition, &vFileBeginPosition, &vFileEndPosition);
				//m_opView -> LockWindow();
			}
		}
	}

	if(p_vClipEditMode)
	{
		p_opView -> SetHighColor(EM_COLOR_TEXT);
		p_opView -> SetLowColor(EMColor(200, 200, 200, 255));
		p_opView -> DrawString(m_oFrame.m_vLeft + m_vStringLeft, m_oFrame.m_vTop + m_vStringTop, m_oName.c_str());
	}
	else
	{
		if(m_vSelected)
		{
			p_opView -> SetHighColor(EM_COLOR_SELECTED_AUDIO_VIEW_SHADOW);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vBottom, oDrawRect.m_vRight, m_oFrame.m_vBottom);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vBottom - 1, oDrawRect.m_vRight, m_oFrame.m_vBottom - 1);

			p_opView -> SetHighColor(EM_COLOR_SELECTED_AUDIO_VIEW_HIGHLIGHT);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vTop, oDrawRect.m_vRight, m_oFrame.m_vTop);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vTop + 1, oDrawRect.m_vRight, m_oFrame.m_vTop + 1);

			if(m_oFrame.m_vLeft >= oDrawRect.m_vLeft)
				p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vLeft, m_oFrame.m_vBottom);
			if(m_oFrame.m_vLeft + 1 >= oDrawRect.m_vLeft)
				p_opView -> StrokeLine(m_oFrame.m_vLeft + 1, m_oFrame.m_vTop, m_oFrame.m_vLeft + 1, m_oFrame.m_vBottom - 1);

			p_opView -> SetHighColor(EM_COLOR_SELECTED_AUDIO_VIEW_SHADOW);
			if(m_oFrame.m_vRight <= oDrawRect.m_vRight)
				p_opView -> StrokeLine(m_oFrame.m_vRight, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vBottom);
			if(m_oFrame.m_vRight - 1 <= oDrawRect.m_vRight)
				p_opView -> StrokeLine(m_oFrame.m_vRight - 1, m_oFrame.m_vTop + 1, m_oFrame.m_vRight - 1, m_oFrame.m_vBottom);

			EMRect oLabelBackground(m_oLabelBackground);
			oLabelBackground.OffsetBy(m_oFrame.m_vLeft, m_oFrame.m_vTop);
			if(oLabelBackground.Intersects(p_oClippingRect))
			{
				m_opView -> SetHighColor(EM_COLOR_SELECTED_AUDIO_VIEW_SHADOW);
				p_opView -> FillRect(oLabelBackground);
				p_opView -> SetHighColor(EMColor(255, 255, 255, 255));
				//p_opView -> SetLowColor(EM_COLOR_SELECTED_AUDIO_VIEW_BACKGROUND);
				p_opView -> DrawString(m_oFrame.m_vLeft + m_vStringLeft, m_oFrame.m_vTop + m_vStringTop, m_oName.c_str());
			}
		}
		else
		{
			p_opView -> SetHighColor(EM_COLOR_AUDIO_VIEW_SHADOW);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vBottom, oDrawRect.m_vRight, m_oFrame.m_vBottom);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vBottom - 1, oDrawRect.m_vRight, m_oFrame.m_vBottom - 1);

			p_opView -> SetHighColor(EM_COLOR_AUDIO_VIEW_HIGHLIGHT);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vTop, oDrawRect.m_vRight, m_oFrame.m_vTop);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vTop + 1, oDrawRect.m_vRight, m_oFrame.m_vTop + 1);

			if(m_oFrame.m_vLeft >= oDrawRect.m_vLeft)
				p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vLeft, m_oFrame.m_vBottom);
			if(m_oFrame.m_vLeft + 1 >= oDrawRect.m_vLeft)
				p_opView -> StrokeLine(m_oFrame.m_vLeft + 1, m_oFrame.m_vTop, m_oFrame.m_vLeft + 1, m_oFrame.m_vBottom - 1);

			p_opView -> SetHighColor(EM_COLOR_AUDIO_VIEW_SHADOW);
			if(m_oFrame.m_vRight <= oDrawRect.m_vRight)
				p_opView -> StrokeLine(m_oFrame.m_vRight, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vBottom);
			if(m_oFrame.m_vRight - 1 <= oDrawRect.m_vRight)
				p_opView -> StrokeLine(m_oFrame.m_vRight - 1, m_oFrame.m_vTop + 1, m_oFrame.m_vRight - 1, m_oFrame.m_vBottom);

			EMRect oLabelBackground(m_oLabelBackground);
			oLabelBackground.OffsetBy(m_oFrame.m_vLeft, m_oFrame.m_vTop);
			if(oLabelBackground.Intersects(p_oClippingRect))
			{
				p_opView -> SetHighColor(EM_COLOR_SELECTED_AUDIO_VIEW_SHADOW);
				p_opView -> FillRect(oLabelBackground);
				p_opView -> SetHighColor(EMColor(255, 255, 255, 255));
				//p_opView -> SetLowColor(EM_COLOR_AUDIO_VIEW_BACKGROUND);
				p_opView -> DrawString(m_oFrame.m_vLeft + m_vStringLeft, m_oFrame.m_vTop + m_vStringTop, m_oName.c_str());
			}
		}
		/*
		if(GetMuted())
		{
			p_opView -> SetHighColor(EM_COLOR_SELECTED_AUDIO_VIEW_SHADOW);
			p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vBottom);
			p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vBottom, m_oFrame.m_vRight, m_oFrame.m_vTop);
		}
		*/
	}

	p_opView -> ConstrainClippingRect(EMRect());
}

void AudioGClip::DrawContent(int64 p_vFileBeginPosition, int64 p_vFileEndPosition, int64 p_vClipBeginPosition, int64 p_vClipEndPosition, int64 p_vZoomScale, bool p_vLowRes, bool p_vClipEdit, bool p_vDrawMovedClip)
{
	if(!m_vIsInitialized) // Lazy init
	{
//		m_opView -> UnlockWindow();
		EMImageFileInfo oInfo = EMImageBufferRepository::Instance() -> GetFileInfo(this);
		m_vLowResContent = EMImageBufferRepository::Instance() -> GetLowResAudio(this);
//		m_opView -> LockWindow();
		m_vNrHiResSamples = oInfo.m_vNrHiResSamples;
		m_vNrLoResSamples = oInfo.m_vNrLoResSamples;
		m_vFileLength = oInfo.m_vFileLength;
		m_vIsInitialized = true;
	}
	
	int32 vMediaClipID = GetMediaClipID();
	int64 vClipMarkIn = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_MARK_IN, &vMediaClipID)));

	int vBeginPixel = p_vClipBeginPosition / p_vZoomScale;
	int vEndPixel = p_vClipEndPosition / p_vZoomScale;
/*
	if(vBeginPixel != m_oFrame.m_vLeft)
		vBeginPixel--;
	if(vEndPixel == m_oFrame.m_vRight)
		vEndPixel--;
*/
//;//cout_commented_out_4_release << "vBeginPixel: " << vBeginPixel << ", vEndPixel: " << vEndPixel << ", p_vFileBeginPosition: " << p_vFileBeginPosition << ", p_vFileEndPosition: " << p_vFileEndPosition << endl;
	// Locking repository while using pointers to data in it
	vector<EMImageChunk*>* opChunks = NULL;
	if(!p_vLowRes)
		opChunks = EMImageBufferRepository::Instance() -> GetData(this);
	//EMImageBufferRepository::Instance() -> Lock();
	int vCurrentPixel = vBeginPixel;
	int64 vCurrentSlot;
	ClipEditDisplayMode eDisplayMode;

	if(m_vChannels == 1)
	{
		// FIXME: Add stuff here
	}
	else if(m_vChannels == 2)
	{
		int vLineOne;
		int vLineMiddle;
		int vLineTwo;
		int vMaxWaveHeight;
		if(p_vDrawMovedClip)
		{
			vLineOne = static_cast<int>(m_oMovedFrame.m_vTop + (m_oMovedFrame.GetHeight() / 4));
			vLineMiddle = static_cast<int>(m_oMovedFrame.m_vTop + (m_oMovedFrame.GetHeight() / 2));
			vLineTwo = static_cast<int>(m_oMovedFrame.m_vTop + ((m_oMovedFrame.GetHeight() * 3) / 4));
			vMaxWaveHeight = static_cast<int>(m_oMovedFrame.GetHeight() / 4);
		}
		else
		{
			vLineOne = static_cast<int>(m_oFrame.m_vTop + (m_oFrame.GetHeight() / 4));
			vLineMiddle = static_cast<int>(m_oFrame.m_vTop + (m_oFrame.GetHeight() / 2));
			vLineTwo = static_cast<int>(m_oFrame.m_vTop + ((m_oFrame.GetHeight() * 3) / 4));
			vMaxWaveHeight = static_cast<int>(m_oFrame.GetHeight() / 4);
		}
		if(p_vClipEdit)
		{
			eDisplayMode = static_cast<AudioGClipContainer*>(m_opTrack -> GetClipContainer()) -> GetDisplayMode();
			if(eDisplayMode == CE_DISPLAY_MODE_DEFAULT)
				m_opView -> SetHighColor(COLOR_WAVE_CLIP_EDIT);
			else
				m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_HIGHLIGHT);
		}
		else if(m_vSelected)
			m_opView -> SetHighColor(COLOR_WAVE_SELECTED);
		else
			m_opView -> SetHighColor(COLOR_WAVE);
		int64 vNextSlot;
		int64 vClipPosition = GetPosition();
		int64 vSlotAdvanceConstantOne = p_vZoomScale * m_vAudioSampleRate * m_vChannels;
		int64 vSlotAdvanceConstantTwo = (vClipMarkIn - vClipPosition) * m_vAudioSampleRate * m_vChannels;//((m_vClipMarkIn * m_vAudioSampleRate * m_vChannels) - (vClipPosition * m_vAudioSampleRate * m_vChannels));
		int64 vSlotAdvanceConstantThree;
		if(p_vLowRes)
			vSlotAdvanceConstantThree = EM_TICKS_PER_SECOND * EM_LOW_RES_AUDIO_SKIP_RATE;
		else
			vSlotAdvanceConstantThree = EM_TICKS_PER_SECOND;
		vCurrentSlot = (vCurrentPixel * vSlotAdvanceConstantOne + vSlotAdvanceConstantTwo) / vSlotAdvanceConstantThree;
		if(vCurrentSlot % 2 > 0)
		{
			vCurrentSlot--;
			if(!p_vLowRes)
				if(!((*opChunks)[vCurrentSlot / EM_IMAGE_AUDIO_BUFFER_CHUNK_SIZE]) -> m_vIsValid)
					vCurrentSlot += 2; // So that the first used slot isn't moved to an invalid slot
		}
		int16 vValueOneL;
		int16 vValueTwoL;
		int16 vValueOneR;
		int16 vValueTwoR;
		while(vCurrentPixel <= vEndPixel)
		{
//;//cout_commented_out_4_release << "vNextSlot: " << vNextSlot << ", p_vZoomScale: " << p_vZoomScale << ", m_vClipMarkIn: " << m_vClipMarkIn << ", vClipPosition: " << vClipPosition << ", vCurrentPixel: " << vCurrentPixel << endl;
			vNextSlot = ((vCurrentPixel + 1) * vSlotAdvanceConstantOne + vSlotAdvanceConstantTwo) / vSlotAdvanceConstantThree;
			if(vNextSlot % 2 > 0)
				vNextSlot--;
			if(p_vLowRes)
			{
				vValueOneL = vLineOne - (vMaxWaveHeight * (m_vLowResContent[vCurrentSlot] - 127) / 127);
				vValueOneR = vLineTwo - (vMaxWaveHeight * (m_vLowResContent[vCurrentSlot + 1] - 127) / 127);
				if(vNextSlot >= m_vNrLoResSamples)
				{
					vValueTwoL = vValueOneL;
					vValueTwoR = vValueOneR;
				}
				else
				{
					vValueTwoL = vLineOne - (vMaxWaveHeight * (m_vLowResContent[vNextSlot] - 127) / 127);
					vValueTwoR = vLineTwo - (vMaxWaveHeight * (m_vLowResContent[vNextSlot + 1] - 127) / 127);
				}
			}
			else
			{
if(!((*opChunks)[vCurrentSlot / EM_IMAGE_AUDIO_BUFFER_CHUNK_SIZE]) -> m_vIsValid)
{
;//cout_commented_out_4_release << "vValueOneL chunk is invalid: vCurrentSlot: " << vCurrentSlot << ", vCurrentPixel: " << vCurrentPixel << ", vEndPixel: " << vEndPixel << ", vNextSlot: " << vNextSlot << ", m_vNrHiResSamples: " << m_vNrHiResSamples << ", first slot: " << (vBeginPixel * vSlotAdvanceConstantOne + vSlotAdvanceConstantTwo) / vSlotAdvanceConstantThree << ", last slot: " << (vEndPixel * vSlotAdvanceConstantOne + vSlotAdvanceConstantTwo) / vSlotAdvanceConstantThree << endl;
EMDebugger("vValueOneL chunk is invalid");
return;
}
if(!((*opChunks)[(vCurrentSlot + 1) / EM_IMAGE_AUDIO_BUFFER_CHUNK_SIZE]) -> m_vIsValid)
{
;//cout_commented_out_4_release << "vValueOneR chunk is invalid: vCurrentSlot: " << vCurrentSlot << ", vCurrentPixel: " << vCurrentPixel << ", vEndPixel: " << vEndPixel << ", vNextSlot: " << vNextSlot << ", m_vNrHiResSamples: " << m_vNrHiResSamples << endl;
EMDebugger("vValueOneR chunk is invalid");				
return;
}
				vValueOneL = vLineOne - (vMaxWaveHeight * ((*opChunks)[vCurrentSlot / EM_IMAGE_AUDIO_BUFFER_CHUNK_SIZE]) -> m_vpAudioBufferChunk[vCurrentSlot % EM_IMAGE_AUDIO_BUFFER_CHUNK_SIZE] / 32768);
				vValueOneR = vLineTwo - (vMaxWaveHeight * ((*opChunks)[(vCurrentSlot + 1) / EM_IMAGE_AUDIO_BUFFER_CHUNK_SIZE]) -> m_vpAudioBufferChunk[(vCurrentSlot + 1) % EM_IMAGE_AUDIO_BUFFER_CHUNK_SIZE] / 32768);
				if(vNextSlot >= m_vNrHiResSamples)
				{
					vValueTwoL = vValueOneL;
					vValueTwoR = vValueOneR;
				}
				else
				{
if(!((*opChunks)[vNextSlot / EM_IMAGE_AUDIO_BUFFER_CHUNK_SIZE]) -> m_vIsValid)
{
;//cout_commented_out_4_release << "vValueTwoL chunk is invalid: vCurrentSlot: " << vCurrentSlot << ", vCurrentPixel: " << vCurrentPixel << ", vEndPixel: " << vEndPixel << ", vNextSlot: " << vNextSlot << ", m_vNrHiResSamples: " << m_vNrHiResSamples << endl;
EMDebugger("vValueTwoL chunk is invalid");				
return;
}
if(!((*opChunks)[(vNextSlot + 1) / EM_IMAGE_AUDIO_BUFFER_CHUNK_SIZE]) -> m_vIsValid)
{
;//cout_commented_out_4_release << "vValueTwoR chunk is invalid: vCurrentSlot: " << vCurrentSlot << ", vCurrentPixel: " << vCurrentPixel << ", vEndPixel: " << vEndPixel << ", vNextSlot: " << vNextSlot << ", m_vNrHiResSamples: " << m_vNrHiResSamples << endl;
EMDebugger("vValueTwoR chunk is invalid");				
return;
}
					vValueTwoL = vLineOne - (vMaxWaveHeight * ((*opChunks)[vNextSlot / EM_IMAGE_AUDIO_BUFFER_CHUNK_SIZE]) -> m_vpAudioBufferChunk[vNextSlot % EM_IMAGE_AUDIO_BUFFER_CHUNK_SIZE] / 32768);
					vValueTwoR = vLineTwo - (vMaxWaveHeight * ((*opChunks)[(vNextSlot + 1) / EM_IMAGE_AUDIO_BUFFER_CHUNK_SIZE]) -> m_vpAudioBufferChunk[(vNextSlot + 1) % EM_IMAGE_AUDIO_BUFFER_CHUNK_SIZE] / 32768);
				}
			}
//;//cout_commented_out_4_release << "vValueOneL: " << vValueOneL << ", vValueTwoL: " << vValueTwoL << ", vValueOneR: " << vValueOneR << ", vValueTwoR: " << vValueTwoR << ", vCurrentSlot: " << vCurrentSlot << ", vNextSlot: " << vNextSlot << endl;
			if(p_vDrawMovedClip)
			{
				m_opView -> StrokeLine(vCurrentPixel + static_cast<int>(m_oMovedFrame.m_vLeft - m_oFrame.m_vLeft), vValueOneL, vCurrentPixel + 1 + static_cast<int>(m_oMovedFrame.m_vLeft - m_oFrame.m_vLeft), vValueTwoL);
				m_opView -> StrokeLine(vCurrentPixel + static_cast<int>(m_oMovedFrame.m_vLeft - m_oFrame.m_vLeft), vValueOneR, vCurrentPixel + 1 + static_cast<int>(m_oMovedFrame.m_vLeft - m_oFrame.m_vLeft), vValueTwoR);
			}
			else
			{
				m_opView -> StrokeLine(vCurrentPixel, vValueOneL, vCurrentPixel + 1, vValueTwoL);
				m_opView -> StrokeLine(vCurrentPixel, vValueOneR, vCurrentPixel + 1, vValueTwoR);
			}
//		;//cout_commented_out_4_release << "Stroke line: x1: " << vCurrentPixel << ", y1: " << vLineTwo - ((vMaxWaveHeight * m_vLowResContent[vCurrentSlot + 1]) / 127) << ", x2: " << vCurrentPixel << ", y2: " << vLineTwo - ((vMaxWaveHeight * m_vLowResContent[vCurrentSlot + 3]) / 127) << endl;
			vCurrentSlot = vNextSlot;
			vCurrentPixel++;
		}

		if(p_vDrawMovedClip)
		{
			m_opView -> SetHighColor(COLOR_WAVE_LINE_SELECTED);
			m_opView -> StrokeLine(vBeginPixel + static_cast<int>(m_oMovedFrame.m_vLeft - m_oFrame.m_vLeft), vLineOne, vEndPixel + static_cast<int>(m_oMovedFrame.m_vLeft - m_oFrame.m_vLeft), vLineOne);
			m_opView -> StrokeLine(vBeginPixel + static_cast<int>(m_oMovedFrame.m_vLeft - m_oFrame.m_vLeft), vLineMiddle, vEndPixel + static_cast<int>(m_oMovedFrame.m_vLeft - m_oFrame.m_vLeft), vLineMiddle);
			m_opView -> StrokeLine(vBeginPixel + static_cast<int>(m_oMovedFrame.m_vLeft - m_oFrame.m_vLeft), vLineTwo, vEndPixel + static_cast<int>(m_oMovedFrame.m_vLeft - m_oFrame.m_vLeft), vLineTwo);
		}
		else
		{
			if(p_vClipEdit)
			{
				if(eDisplayMode == CE_DISPLAY_MODE_DEFAULT)
					m_opView -> SetHighColor(COLOR_WAVE_LINE_CLIP_EDIT);
				else
					m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_HIGHLIGHT);
			}
			else if(m_vSelected)
				m_opView -> SetHighColor(COLOR_WAVE_LINE_SELECTED);
			else
				m_opView -> SetHighColor(COLOR_WAVE_LINE);
			m_opView -> StrokeLine(vBeginPixel, vLineOne, vEndPixel, vLineOne);
			m_opView -> StrokeLine(vBeginPixel, vLineMiddle, vEndPixel, vLineMiddle);
			m_opView -> StrokeLine(vBeginPixel, vLineTwo, vEndPixel, vLineTwo);
		}
	}
	//EMImageBufferRepository::Instance() -> Unlock();
}

void AudioGClip::DrawMovedClip(EMRect p_oClippingRect, EMView* p_opView, int64 p_vZoomScale, bool p_vDrawContent)
{
	if(!p_oClippingRect.Intersects(m_oMovedFrame))
		return;

	p_opView -> ConstrainClippingRect(m_oMovedFrame & p_oClippingRect);

	EMRect oDrawRect(m_oMovedFrame & p_oClippingRect);

	p_opView -> SetDrawingMode(EM_OP_ALPHA);
	p_opView -> SetHighColor(EM_COLOR_MOVED_AUDIO_VIEW_BACKGROUND);
	p_opView -> FillRect(oDrawRect);
	
	int32 vMediaClipID = GetMediaClipID();
	int64 vClipMarkIn = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_MARK_IN, &vMediaClipID)));

	if(p_vDrawContent)
	{
		int32 vLeftPixel = m_oFrame.m_vLeft;
		int32 vRightPixel = m_oFrame.m_vRight;
		m_vTimeZoomScale = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TIME_ZOOM_SCALE)));
		int64 vFileBeginPosition = static_cast<int64>(vLeftPixel) * m_vTimeZoomScale - GetPosition() + vClipMarkIn;
		int64 vClipBeginPosition = static_cast<int64>(vLeftPixel) * m_vTimeZoomScale;
		int64 vFileEndPosition = static_cast<int64>(vRightPixel) * m_vTimeZoomScale - GetPosition() + vClipMarkIn;
		int64 vClipEndPosition = static_cast<int64>(vRightPixel) * m_vTimeZoomScale;

//		p_opView -> ConstrainClippingRect(m_oMovedFrame);

		if(m_vTimeZoomScale > DRAW_LOW_RES_ZOOM_THRESHOLD)
			DrawContent(vFileBeginPosition, vFileEndPosition, vClipBeginPosition, vClipEndPosition, m_vTimeZoomScale, true, false, true);
		else if(EMImageBufferRepository::Instance() -> AudioDataAvailable(this, vFileBeginPosition, vFileEndPosition))
			DrawContent(vFileBeginPosition, vFileEndPosition, vClipBeginPosition, vClipEndPosition, m_vTimeZoomScale, false, false, true);
	
//		p_opView -> ConstrainClippingRect(EMRect());
	}

	p_opView -> SetHighColor(EM_COLOR_MOVED_AUDIO_VIEW_SHADOW);
	p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oMovedFrame.m_vBottom, oDrawRect.m_vRight, m_oMovedFrame.m_vBottom);
	if(m_oMovedFrame.m_vRight <= oDrawRect.m_vRight)
		p_opView -> StrokeLine(m_oMovedFrame.m_vRight, m_oMovedFrame.m_vTop, m_oMovedFrame.m_vRight, m_oMovedFrame.m_vBottom);
	p_opView -> SetHighColor(EM_COLOR_MOVED_AUDIO_VIEW_HIGHLIGHT);
	p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oMovedFrame.m_vTop, oDrawRect.m_vRight, m_oMovedFrame.m_vTop);
	if(m_oMovedFrame.m_vLeft >= oDrawRect.m_vLeft)
		p_opView -> StrokeLine(m_oMovedFrame.m_vLeft, m_oMovedFrame.m_vTop, m_oMovedFrame.m_vLeft, m_oMovedFrame.m_vBottom);

	p_opView -> SetHighColor(EM_COLOR_TEXT);
	p_opView -> SetLowColor(EM_COLOR_MOVED_AUDIO_VIEW_BACKGROUND);
//	p_opView -> ConstrainClippingRect(m_oMovedFrame);
//m_opView -> SetHighColor(EM_COLOR_SELECTED_AUDIO_VIEW_SHADOW);
//p_opView -> FillRect(m_oLabelBackground);
	p_opView -> SetHighColor(EMColor(255, 255, 255, 255));
	p_opView -> DrawString(m_oMovedFrame.m_vLeft + m_vStringLeft, m_oMovedFrame.m_vTop + m_vStringTop, m_oName.c_str());
//	p_opView -> ConstrainClippingRect(EMRect());
	p_opView -> SetDrawingMode(EM_OP_COPY);
	/*
	if(GetMuted())
	{
		p_opView -> SetHighColor(EM_COLOR_SELECTED_AUDIO_VIEW_SHADOW);
		p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vBottom);
		p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vBottom, m_oFrame.m_vRight, m_oFrame.m_vTop);
	}
	*/
	p_opView -> ConstrainClippingRect(EMRect());
}

void AudioGClip::EnableClipFX(EMView* p_opView, int32 p_vMediaTrackID)
{
	if(m_opFXNodes == NULL)
	{
		m_opFXNodes = EM_new list<FXNode>();

		int32 vMediaEffectTrackID = *(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_CREATE_EFFECT_TRACK, NULL)));
		EMMediaTrackInfo* opMediaTrackInfo = static_cast<EMMediaTrackInfo*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_TRACK_INFO, &p_vMediaTrackID, NULL));
		int32 vTrackEffectTrackID = opMediaTrackInfo -> m_vOutputID;
//		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_EFFECT_TRACK_OUTPUT, &m_vMediaEffectTrackID, &vTrackEffectTrackID, NULL);
		int32 vMediaClipID = GetMediaClipID();
		EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_COMMAND_SET_CLIP_OUTPUT, &vMediaClipID, &vMediaEffectTrackID, NULL);

		// Create an empty initial FX node
		m_opFXNodes -> push_back(FXNode(EM_TYPE_ANY_AUDIO, 0, m_opTrack -> GetClipContainer() -> Frame(), p_opView, m_opTrack -> GetClipContainer(), vMediaEffectTrackID));
	}
}

list<FXNode>* AudioGClip::GetFXNodes()
{
	return m_opFXNodes;
}

EMMediaType AudioGClip::GetType()
{
	return EM_TYPE_ANY_AUDIO;
}

bool AudioGClip::MoveClip(int32 p_vTrackOffset, int64 p_vPositionOffset)
{
	//p_vPositionOffset = EMGUIUtilities::Instance() -> TruncateSample(p_vPositionOffset);
	bool vResult = EMGClip::MoveClip(p_vTrackOffset, p_vPositionOffset);

	if(m_opFXNodes != NULL)
	{
		list<FXNode>::iterator oIterator = m_opFXNodes -> begin();
		int32 vEffectTrackID;
		EMMediaClipInfo* opClipInfo;
		while(oIterator != m_opFXNodes -> end())
		{
			opClipInfo = static_cast<EMMediaClipInfo*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_INFO, &m_vMediaClipID));
			vEffectTrackID = opClipInfo -> m_vMediaEffectTrackID;
			(*oIterator).SetMediaEffectTrackID(vEffectTrackID);
			oIterator++;
		}
		/*
		list<FXNode>::iterator oIterator = m_opFXNodes -> begin();
		while(oIterator != m_opFXNodes -> end())
		{
			(*oIterator).SetMediaEffectTrackID(m_opTrack -> GetTrackInfo() -> GetMediaTrackID());
			oIterator++;
		}
		*/
	}

	return vResult;
}

void AudioGClip::SetFrame(EMRect p_oFrame)
{
	EMGClip::SetFrame(p_oFrame);

	if(m_opFXNodes != NULL)
	{
		list<FXNode>::iterator oIterator = m_opFXNodes -> begin();
		while(oIterator != m_opFXNodes -> end())
		{
			(*oIterator).SetParent(m_opTrack -> GetClipContainer());
			oIterator++;
		}
	}
}

// FIXME: p_opView is no longer needed
void AudioGClip::SetLabel(char* p_oLabel, EMView* p_opView)
{
	EMGClip::SetLabel(p_oLabel, m_opView);
	m_vNamePositionUpdated = false;
	//CalculateNamePosition(m_opView);
	dynamic_cast<AudioGClipContainer*>(m_opTrack -> GetClipContainer()) -> UpdateValueBox();
}

/*
void AudioGClip::SetMediaClipID(int32 p_vMediaClipID)
{
	if(p_vMediaClipID == m_vMediaClipID)
		return; // Already set

	// Do some clean-up if necessary
	if(m_opFXNodes != NULL)
		delete m_opFXNodes;
	m_vLowResContent = NULL;
	m_vIsInitialized = false;

	if(GetTrack() != NULL)
		m_vChannels = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_NUM_CHANNELS, &p_vMediaClipID)));

	if(p_vMediaClipID > 0)
	{
		// If the clip is reconstructed from a saved project or is a clone of another clip, it may have a chain of effects, let's find that out
		EMMediaClipInfo* opMediaClipInfo = static_cast<EMMediaClipInfo*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_INFO, &p_vMediaClipID));
		if(opMediaClipInfo -> m_vMediaEffectTrackID != -1) // It has an effect track
		{
			m_opFXNodes = EM_new list<FXNode>();

			int32 vEffectTrackID = opMediaClipInfo -> m_vMediaEffectTrackID;
			EMMediaEffectTrackInfo* opEffectTrackInfo = static_cast<EMMediaEffectTrackInfo*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_EFFECT_TRACK_INFO, &vEffectTrackID));
			list<int32>* opMediaEffectIDs = &(opEffectTrackInfo -> m_oMediaEffectIDs);
			list<int32>::iterator oMediaEffectIDIterator = opMediaEffectIDs -> begin();
			int vNodeIndex(0);
			int32 vMediaClipID = GetMediaClipID();

			while(oMediaEffectIDIterator != opMediaEffectIDs -> end())
			{
				int32 vEffectID = *oMediaEffectIDIterator;
				EMMediaEffectInfo* opEffectInfo = static_cast<EMMediaEffectInfo*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_EFFECT_INFO, &vEffectID));
				// Insert an empty effect node between active effect nodes
				m_opFXNodes -> push_back(FXNode(EM_TYPE_ANY_AUDIO, vNodeIndex, Frame(), m_opView, GetTrack() -> GetClipContainer(), vEffectTrackID));
				vNodeIndex++;
				m_opFXNodes -> push_back(FXNode(EM_TYPE_ANY_AUDIO, vNodeIndex, Frame(), m_opView, GetTrack() -> GetClipContainer(), vEffectTrackID, opEffectInfo -> m_vEntryID, vEffectID, (opEffectInfo -> m_oPluginName).c_str()));
				vNodeIndex++;
				oMediaEffectIDIterator++;
			}

			// Insert an empty node at the end of the chain
			m_opFXNodes -> push_back(FXNode(EM_TYPE_ANY_AUDIO, vNodeIndex, Frame(), m_opView, GetTrack() -> GetClipContainer(), vEffectTrackID));
		}
	}
}
*/
/*
void AudioGClip::SetTrack(EMGTrack* p_opTrack)
{
	EMGClip::SetTrack(p_opTrack);
}
*/

void AudioGClip::UpdateContent()
{
	m_vIsInitialized = false;
}

