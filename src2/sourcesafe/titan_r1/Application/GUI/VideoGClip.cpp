#include "VideoGClip.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
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
#include "VideoGClipContainer.h"

#include "EMResourceRepository.h" // Debug
#include "ResourceIDs.h" // Debug

// Fixme: Fix the text placement calculations
const int32 NAME_X_OFFSET = 3;//5;
const int32 NAME_Y_OFFSET = -2;//5; 

VideoGClip::VideoGClip(string p_oClipName, int64 p_vPosition, int64 p_vLength, EMGTrack* p_opTrack, int32 p_vMediaClipID, EMView* p_opView) :
GClip(p_oClipName, p_vPosition, p_vLength, p_opTrack, p_vMediaClipID, p_opView),
m_opChunks(NULL),
m_opFXNodes(NULL),
//m_vMediaEffectTrackID(-1),
m_vNamePositionUpdated(false)
{
	if(p_opTrack != NULL)
		m_vFramerate = *(static_cast<float*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_FRAMERATE)));

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
				m_opFXNodes -> push_back(FXNode(EM_TYPE_ANY_VIDEO, vNodeIndex, Frame(), m_opView, p_opTrack -> GetClipContainer(), vEffectTrackID));
				vNodeIndex++;
				m_opFXNodes -> push_back(FXNode(EM_TYPE_ANY_VIDEO, vNodeIndex, Frame(), m_opView, p_opTrack -> GetClipContainer(), vEffectTrackID, opEffectInfo -> m_vEntryID, vEffectID, (opEffectInfo -> m_oPluginName).c_str()));
				vNodeIndex++;
				oMediaEffectIDIterator++;
			}

			// Insert an empty node at the end of the chain
			m_opFXNodes -> push_back(FXNode(EM_TYPE_ANY_VIDEO, vNodeIndex, Frame(), m_opView, p_opTrack -> GetClipContainer(), vEffectTrackID));
		}
	}
}

VideoGClip::~VideoGClip()
{
	delete m_opFXNodes;
}

EMRect VideoGClip::CalculateFrame(uint32 p_vTrackOffset, int64 p_vClipOffsetX)
{
//;//cout_commented_out_4_release << "VideoGClip::CalculateFrame: p_vClipOffsetX: " << p_vClipOffsetX << endl;
	p_vClipOffsetX = EMGUIUtilities::Instance() -> TruncateFrame(p_vClipOffsetX);
//;//cout_commented_out_4_release << "VideoGClip::CalculateFrame: p_vClipOffsetX: " << p_vClipOffsetX << endl;
//EMGClip::CalculateFrame(p_vTrackOffset, p_vClipOffsetX).Print();
	return EMGClip::CalculateFrame(p_vTrackOffset, p_vClipOffsetX);
}

void VideoGClip::CalculateNamePosition(EMView* p_opView)
{
	int32 vAscent;
	int32 vDescent;
	p_opView -> SetFont(EM_PLAIN);
	p_opView -> CalculateStringHeight(m_oName.c_str(), vAscent, vDescent);
	m_vStringTop = vAscent + NAME_Y_OFFSET;
	m_vStringLeft = NAME_X_OFFSET;
	int vLength = p_opView -> CalculateStringWidth(m_oName.c_str());
	m_oLabelBackground = EMRect(2, 2, 2 + vLength, 12);
	m_vNamePositionUpdated = true;
/*	
	int32 vYSpacing = (m_oFrame.GetHeight() - (vAscent + vDescent)) / 2;
	m_vStringTop = m_oFrame.m_vTop + vYSpacing + vAscent + SLIM_BUTTON_BORDER_SIZE;
	m_vStringLeft = m_oFrame.m_vLeft + ((m_oFrame.GetWidth() - m_opView -> CalculateStringWidth(m_opOnString -> c_str())) / 2);
*/
}

EMGClip* VideoGClip::CloneClip(int32 p_vTrackOffset, int64 p_vPositionOffset)
{
	EMGTrackRepository* opTrackRepository = EMGTrackRepository::Instance();
	EMGClipRepository* opClipRepository = EMGClipRepository::Instance();
	EMGTrack* opNewTrack = opTrackRepository -> GetTrack(opTrackRepository -> GetTrackIndex(GetTrack()) + p_vTrackOffset);
	//p_vPositionOffset = EMGUIUtilities::Instance() -> TruncateFrame(p_vPositionOffset);
	int64 vNewPosition = GetPosition() + p_vPositionOffset;
	vNewPosition = EMGUIUtilities::Instance() -> TruncateFrame(vNewPosition);


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

EMGClip* VideoGClip::CreateInstance(string p_oClipName, int64 p_vPosition, int64 p_vLength, EMGTrack* p_opTrack, int32 p_vMediaClipID, EMView* p_opView)
{
	return EM_new VideoGClip(EMGUIUtilities::Instance() -> GetUniqueClipName(p_oClipName.c_str(), NULL), p_vPosition, p_vLength, p_opTrack, p_vMediaClipID, p_opView);
}

void VideoGClip::DrawClip(EMRect p_oClippingRect, EMView* p_opView, bool p_vClipEditMode, int64 p_vZoomScale, bool p_vDrawContent)
{
//;//cout_commented_out_4_release << "VideoGClip::DrawClip: " << p_opView << "; " << m_opView << endl;
	if(!p_oClippingRect.Intersects(m_oFrame))
		return;

	if(!m_vNamePositionUpdated)
		CalculateNamePosition(p_opView);

	if(m_opChunks == NULL) // Lazy init
	{
		m_opChunks = EMImageBufferRepository::Instance() -> GetData(this);
		EMImageBufferRepository::Instance() -> GetFrameWidthAndSkipRate(this, m_oFrame.GetHeight(), &m_vFrameWidth, &m_vSkipRate);
		EMImageFileInfo oInfo = EMImageBufferRepository::Instance() -> GetFileInfo(this);
		m_vFileLength = oInfo.m_vFileLength;
		m_vAspectRatio = oInfo.m_vAspectRatio;
	}

	EMRect oDrawRect(m_oFrame & p_oClippingRect);

	if(!p_vClipEditMode)
	{
		if(m_vSelected && p_vDrawContent)
			p_opView -> SetHighColor(EMColor(130, 130, 130, 255));
		else if(m_vSelected && !p_vDrawContent)
			p_opView -> SetHighColor(EM_COLOR_SELECTED_VIDEO_VIEW_BACKGROUND);
		else if(p_vDrawContent)
			p_opView -> SetHighColor(EMColor(190, 190, 190, 255));
		else
			p_opView -> SetHighColor(EM_COLOR_VIDEO_VIEW_BACKGROUND);
		p_opView -> FillRect(oDrawRect);
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
		int32 vMediaClipID = GetMediaClipID();
		int64 vClipMarkIn = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_MARK_IN, &vMediaClipID)));
		int64 vFileBeginPosition = static_cast<int64>(vLeftPixel) * p_vZoomScale - GetPosition() + vClipMarkIn;
		int64 vClipBeginPosition = static_cast<int64>(vLeftPixel) * p_vZoomScale;
		int64 vFileEndPosition = static_cast<int64>(vRightPixel) * p_vZoomScale - GetPosition() + vClipMarkIn;
		int64 vClipEndPosition = static_cast<int64>(vRightPixel) * p_vZoomScale;
		//// The window semaphore is temporarily unlocked to avoid deadlock with the semaphore set in VideoDataAvailable
		//m_opView -> UnlockWindow();
		bool vDataAvailable = EMImageBufferRepository::Instance() -> VideoDataAvailable(this, vFileBeginPosition, vFileEndPosition, m_oFrame.GetHeight());
		//m_opView -> LockWindow();
		DrawContent(vFileBeginPosition, vFileEndPosition, vClipBeginPosition, vClipEndPosition, p_vZoomScale, p_vClipEditMode, false);
		if(!vDataAvailable)
			EMImageBufferRepository::Instance() -> Invalidate(this, &vClipBeginPosition, &vClipEndPosition, &vFileBeginPosition, &vFileEndPosition);
	}

	p_opView -> ConstrainClippingRect(m_oFrame & p_oClippingRect);

	if(p_vClipEditMode)
	{
		p_opView -> SetHighColor(EMColor(0, 0, 0, 255));
		p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vTop, oDrawRect.m_vRight, m_oFrame.m_vTop);
		p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vBottom, oDrawRect.m_vRight, m_oFrame.m_vBottom);
		if(m_oFrame.m_vLeft >= oDrawRect.m_vLeft)
			p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vLeft, m_oFrame.m_vBottom);
		if(m_oFrame.m_vRight <= oDrawRect.m_vRight)
			p_opView -> StrokeLine(m_oFrame.m_vRight, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vBottom);

		p_opView -> ConstrainClippingRect(m_oFrame & p_oClippingRect);

		EMRect oLabelBackground(m_oLabelBackground);
		oLabelBackground.OffsetBy(m_oFrame.m_vLeft, m_oFrame.m_vTop);
		if(oLabelBackground.Intersects(p_oClippingRect))
		{
			p_opView -> SetHighColor(EMColor(255, 255, 255, 255));
			p_opView -> FillRect(oLabelBackground);
			p_opView -> SetHighColor(EM_COLOR_TEXT);
			//p_opView -> SetLowColor(EMColor(200, 200, 200, 255));
			p_opView -> DrawString(m_oFrame.m_vLeft + m_vStringLeft, m_oFrame.m_vTop + m_vStringTop, m_oName.c_str());
		}
	}

	else
	{
		if(m_vSelected)
		{
			p_opView -> SetHighColor(EM_COLOR_SELECTED_VIDEO_VIEW_SHADOW);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vBottom, oDrawRect.m_vRight, m_oFrame.m_vBottom);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vBottom - 1, oDrawRect.m_vRight, m_oFrame.m_vBottom - 1);

			p_opView -> SetHighColor(EM_COLOR_SELECTED_VIDEO_VIEW_HIGHLIGHT);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vTop, oDrawRect.m_vRight, m_oFrame.m_vTop);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vTop + 1, oDrawRect.m_vRight, m_oFrame.m_vTop + 1);

			if(m_oFrame.m_vLeft >= oDrawRect.m_vLeft)
				p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vLeft, m_oFrame.m_vBottom);
			if(m_oFrame.m_vLeft + 1 >= oDrawRect.m_vLeft)
				p_opView -> StrokeLine(m_oFrame.m_vLeft + 1, m_oFrame.m_vTop, m_oFrame.m_vLeft + 1, m_oFrame.m_vBottom - 1);

			p_opView -> SetHighColor(EM_COLOR_SELECTED_VIDEO_VIEW_SHADOW);
			if(m_oFrame.m_vRight <= oDrawRect.m_vRight)
				p_opView -> StrokeLine(m_oFrame.m_vRight, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vBottom);
			if(m_oFrame.m_vRight - 1 <= oDrawRect.m_vRight)
				p_opView -> StrokeLine(m_oFrame.m_vRight - 1, m_oFrame.m_vTop + 1, m_oFrame.m_vRight - 1, m_oFrame.m_vBottom);
		}

		else
		{
			p_opView -> SetHighColor(EM_COLOR_VIDEO_VIEW_SHADOW);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vBottom, oDrawRect.m_vRight, m_oFrame.m_vBottom);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vBottom - 1, oDrawRect.m_vRight, m_oFrame.m_vBottom - 1);

			p_opView -> SetHighColor(EM_COLOR_VIDEO_VIEW_HIGHLIGHT);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vTop, oDrawRect.m_vRight, m_oFrame.m_vTop);
			p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oFrame.m_vTop + 1, oDrawRect.m_vRight, m_oFrame.m_vTop + 1);

			if(m_oFrame.m_vLeft >= oDrawRect.m_vLeft)
				p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vLeft, m_oFrame.m_vBottom);
			if(m_oFrame.m_vLeft + 1 >= oDrawRect.m_vLeft)
				p_opView -> StrokeLine(m_oFrame.m_vLeft + 1, m_oFrame.m_vTop, m_oFrame.m_vLeft + 1, m_oFrame.m_vBottom - 1);

			p_opView -> SetHighColor(EM_COLOR_VIDEO_VIEW_SHADOW);
			if(m_oFrame.m_vRight <= oDrawRect.m_vRight)
				p_opView -> StrokeLine(m_oFrame.m_vRight, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vBottom);
			if(m_oFrame.m_vRight - 1 <= oDrawRect.m_vRight)
				p_opView -> StrokeLine(m_oFrame.m_vRight - 1, m_oFrame.m_vTop + 1, m_oFrame.m_vRight - 1, m_oFrame.m_vBottom);
		}

		EMRect oLabelBackground(m_oLabelBackground);
		oLabelBackground.OffsetBy(m_oFrame.m_vLeft, m_oFrame.m_vTop);
		if(oLabelBackground.Intersects(p_oClippingRect))
		{
			p_opView -> SetHighColor(EM_COLOR_SELECTED_VIDEO_VIEW_SHADOW);
			p_opView -> FillRect(oLabelBackground);
			p_opView -> SetHighColor(EMColor(255, 255, 255, 255));
			p_opView -> DrawString(m_oFrame.m_vLeft + m_vStringLeft, m_oFrame.m_vTop + m_vStringTop, m_oName.c_str());
		}
	}
	/*
	if(GetMuted() && !p_vClipEditMode)
	{
		if(!p_vDrawContent)
		{
			p_opView -> SetDrawingMode(EM_OP_COPY);
			p_opView -> SetHighColor(EM_COLOR_SELECTED_VIDEO_VIEW_BACKGROUND);
		}
		else
			p_opView -> SetHighColor(EMColor(0, 0, 0, 255));
		p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vBottom);
		p_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vBottom, m_oFrame.m_vRight, m_oFrame.m_vTop);
	}
	*/
	//p_opView -> SetDrawingMode(EM_OP_COPY);

	p_opView -> ConstrainClippingRect(EMRect());
}

void VideoGClip::DrawContent(int64 p_vFileBeginPosition, int64 p_vFileEndPosition, int64 p_vClipBeginPosition, int64 p_vClipEndPosition, int64 p_vZoomScale, bool p_vClipEdit, bool p_vDrawMovedClip)
{
	//EMImageBufferRepository::Instance() -> Lock();

	int32 vMediaClipID = GetMediaClipID();
	int64 vClipMarkIn = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_MARK_IN, &vMediaClipID)));

	int32 vCurrentPixelX;
	EMImageChunk* opCurrentChunk = NULL;

	uint32 vBeginChunkIndex;
	uint32 vEndChunkIndex;

	if(m_opChunks -> size() == 1) // Assume that if it only contains one chunk, it is a "picture clip" (bmp, jpg...)
	{
		vBeginChunkIndex = 0;
		vEndChunkIndex = UINT32_MAX;
		opCurrentChunk = (*m_opChunks)[0];
		vCurrentPixelX = m_oFrame.m_vLeft;//(static_cast<int32>((vCurrentChunkIndex * EM_TICKS_PER_SECOND) / m_vFramerate) + GetPosition()) / p_vZoomScale;
	}
	else
	{
		vBeginChunkIndex = EMImageBufferRepository::Instance() -> GetVideoChunkIndex(p_vFileBeginPosition, m_vSkipRate, m_opChunks);
		vEndChunkIndex = EMImageBufferRepository::Instance() -> GetVideoChunkIndex(p_vFileEndPosition, m_vSkipRate, m_opChunks);
		opCurrentChunk = (*m_opChunks)[vBeginChunkIndex];
		vCurrentPixelX = (opCurrentChunk -> m_vBeginPosition - vClipMarkIn + GetPosition()) / p_vZoomScale;
	}

	uint32 vCurrentChunkIndex = vBeginChunkIndex;

	if(p_vDrawMovedClip)
		m_opView -> ConstrainClippingRect(m_oMovedFrame & m_opView -> Bounds());
	else
		m_opView -> ConstrainClippingRect(m_oFrame & m_opView -> Bounds());

	while(vCurrentChunkIndex <= vEndChunkIndex && vCurrentPixelX <= m_oFrame.m_vRight)
	{
		if(m_opChunks -> size() > 1) // Assume that if it only contains one chunk, it is a "picture clip" (bmp, jpg...)
			opCurrentChunk = (*m_opChunks)[vCurrentChunkIndex];

if(opCurrentChunk -> m_opVideoBufferChunk == NULL && opCurrentChunk -> m_vIsValid)
{
EMDebugger("Trying to draw valid bitmap that is NULL");
return;
}

		if(p_vDrawMovedClip)
		{
			if(opCurrentChunk -> m_vIsValid)
			{
				m_opView -> SetDrawingMode(EM_OP_ALPHA);
				m_opView -> SetHighColor(EMColor(255, 255, 255, 100));
				m_opView -> DrawBitmap(opCurrentChunk -> m_opVideoBufferChunk, vCurrentPixelX + (m_oMovedFrame.m_vLeft - m_oFrame.m_vLeft), m_oMovedFrame.m_vTop);
			}
			else
			{
				m_opView -> SetHighColor(EM_COLOR_SELECTED_VIDEO_VIEW_SHADOW);
				m_opView -> StrokeRect(EMRect(vCurrentPixelX, m_oFrame.m_vTop, vCurrentPixelX + m_oFrame.GetHeight() * m_vAspectRatio,  m_oFrame.m_vBottom));
			}

			m_opView -> SetDrawingMode(EM_OP_INVERT);
			char vFrameString[6];
			sprintf(vFrameString, "%04ld", vCurrentChunkIndex);
			m_opView -> DrawString(vCurrentPixelX + 2 + (m_oMovedFrame.m_vLeft - m_oFrame.m_vLeft), m_oMovedFrame.m_vBottom - 2, vFrameString);

			m_opView -> StrokeLine(vCurrentPixelX + (m_oMovedFrame.m_vLeft - m_oFrame.m_vLeft), m_oMovedFrame.m_vTop, vCurrentPixelX + (m_oMovedFrame.m_vLeft - m_oFrame.m_vLeft), m_oMovedFrame.m_vBottom);

			m_opView -> SetDrawingMode(EM_OP_COPY);
		}
		else
		{
			if(opCurrentChunk -> m_vIsValid)
				m_opView -> DrawBitmap(opCurrentChunk -> m_opVideoBufferChunk, vCurrentPixelX, m_oFrame.m_vTop);
			else
			{
				m_opView -> SetHighColor(EM_COLOR_SELECTED_VIDEO_VIEW_SHADOW);
				m_opView -> StrokeRect(EMRect(vCurrentPixelX, m_oFrame.m_vTop, vCurrentPixelX + m_oFrame.GetHeight() * m_vAspectRatio,  m_oFrame.m_vBottom));
			}

			m_opView -> SetDrawingMode(EM_OP_INVERT);
			char vFrameString[6];
			sprintf(vFrameString, "%04ld", vCurrentChunkIndex);
			m_opView -> DrawString(vCurrentPixelX + 2, m_oFrame.m_vBottom - 2, vFrameString);

			m_opView -> StrokeLine(vCurrentPixelX, m_oFrame.m_vTop, vCurrentPixelX, m_oFrame.m_vBottom);

			m_opView -> SetDrawingMode(EM_OP_COPY);
		}

		vCurrentPixelX += m_vFrameWidth;
		vCurrentChunkIndex += m_vSkipRate;
	}
	m_opView -> ConstrainClippingRect(EMRect());
	//EMImageBufferRepository::Instance() -> Unlock();
}

void VideoGClip::DrawMovedClip(EMRect p_oClippingRect, EMView* p_opView, int64 p_vZoomScale, bool p_vDrawContent)
{
	if(!p_oClippingRect.Intersects(m_oMovedFrame))
		return;

	p_opView -> SetDrawingMode(EM_OP_ALPHA);
	p_opView -> SetHighColor(EM_COLOR_MOVED_VIDEO_VIEW_BACKGROUND);
	p_opView -> FillRect(m_oMovedFrame);

	if(p_vDrawContent)
	{
		int32 vMediaClipID = GetMediaClipID();
		int64 vClipMarkIn = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_MARK_IN, &vMediaClipID)));
		int64 vClipMarkOut = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_MARK_OUT, &vMediaClipID)));

		m_vTimeZoomScale = *(static_cast<int64*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TIME_ZOOM_SCALE)));
		int64 vFileBeginPosition = static_cast<int64>(p_oClippingRect.m_vLeft - (m_oMovedFrame.m_vLeft - m_oFrame.m_vLeft)) * m_vTimeZoomScale - GetPosition() + vClipMarkIn;
		int64 vClipBeginPosition = static_cast<int64>(p_oClippingRect.m_vLeft) * m_vTimeZoomScale;
		int64 vFileEndPosition = static_cast<int64>(p_oClippingRect.m_vRight - (m_oMovedFrame.m_vLeft - m_oFrame.m_vLeft)) * m_vTimeZoomScale - GetPosition() + vClipMarkIn;
		int64 vClipEndPosition = static_cast<int64>(p_oClippingRect.m_vRight) * m_vTimeZoomScale;
		if(vFileBeginPosition < vClipMarkIn)
		{
			vFileBeginPosition += vClipMarkIn - vFileBeginPosition;
			vClipBeginPosition += vClipMarkIn - vFileBeginPosition;
		}
		if(vFileEndPosition > vClipMarkOut)
		{
			vFileEndPosition -= vFileEndPosition - vClipMarkOut;
			vClipEndPosition -= vFileEndPosition - vClipMarkOut;
		}

		//// The window semaphore is temporarily unlocked to avoid deadlock with the semaphore set in VideoDataAvailable
		//m_opView -> UnlockWindow();
		bool vDataAvailable = EMImageBufferRepository::Instance() -> VideoDataAvailable(this, vFileBeginPosition, vFileEndPosition, m_oFrame.GetHeight());
		//m_opView -> LockWindow();
		if(vDataAvailable)
			DrawContent(vFileBeginPosition, vFileEndPosition, vClipBeginPosition, vClipEndPosition, p_vZoomScale, false, true);
		else
		{
//			DrawEmptyContent(vFileBeginPosition, vFileEndPosition, vClipBeginPosition, vClipEndPosition, p_vZoomScale, false);
			EMImageBufferRepository::Instance() -> Invalidate(this, &vClipBeginPosition, &vClipEndPosition, &vFileBeginPosition, &vFileEndPosition);
		}
//		DrawContent(vFileBeginPosition, vFileEndPosition, vClipBeginPosition, vClipEndPosition, m_vTimeZoomScale, false, true);
	}

	p_opView -> ConstrainClippingRect(m_oMovedFrame & p_oClippingRect);

	EMRect oDrawRect(m_oMovedFrame & p_oClippingRect);

	p_opView -> SetHighColor(EM_COLOR_MOVED_VIDEO_VIEW_SHADOW);
	p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oMovedFrame.m_vBottom, oDrawRect.m_vRight, m_oMovedFrame.m_vBottom);
	if(m_oMovedFrame.m_vRight <= oDrawRect.m_vRight)
		p_opView -> StrokeLine(m_oMovedFrame.m_vRight, m_oMovedFrame.m_vTop, m_oMovedFrame.m_vRight, m_oMovedFrame.m_vBottom);
	p_opView -> SetHighColor(EM_COLOR_MOVED_VIDEO_VIEW_HIGHLIGHT);
	p_opView -> StrokeLine(oDrawRect.m_vLeft, m_oMovedFrame.m_vTop, oDrawRect.m_vRight, m_oMovedFrame.m_vTop);
	if(m_oMovedFrame.m_vLeft >= oDrawRect.m_vLeft)
		p_opView -> StrokeLine(m_oMovedFrame.m_vLeft, m_oMovedFrame.m_vTop, m_oMovedFrame.m_vLeft, m_oMovedFrame.m_vBottom);

//	m_opView -> SetDrawingMode(EM_OP_INVERT);
	p_opView -> SetHighColor(EM_COLOR_TEXT);
	p_opView -> SetLowColor(EM_COLOR_MOVED_VIDEO_VIEW_BACKGROUND);
//p_opView -> SetHighColor(EM_COLOR_SELECTED_VIDEO_VIEW_SHADOW);
//p_opView -> FillRect(m_oLabelBackground);
//p_opView -> SetHighColor(EMColor(255, 255, 255, 255));
	p_opView -> DrawString(m_oMovedFrame.m_vLeft + m_vStringLeft, m_oMovedFrame.m_vTop + m_vStringTop, m_oName.c_str());

	p_opView -> ConstrainClippingRect(EMRect());

	//p_opView -> SetDrawingMode(EM_OP_COPY);
}

void VideoGClip::EnableClipFX(EMView* p_opView, int32 p_vMediaTrackID)
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
		m_opFXNodes -> push_back(FXNode(EM_TYPE_ANY_VIDEO, 0, m_opTrack -> GetClipContainer() -> Frame(), p_opView, m_opTrack -> GetClipContainer(), vMediaEffectTrackID));
	}
}

list<FXNode>* VideoGClip::GetFXNodes()
{
	return m_opFXNodes;
}

EMMediaType VideoGClip::GetType()
{
	return EM_TYPE_ANY_VIDEO;
}

bool VideoGClip::MoveClip(int32 p_vTrackOffset, int64 p_vPositionOffset)
{
	//p_vPositionOffset = EMGUIUtilities::Instance() -> TruncateFrame(p_vPositionOffset);
	bool vResult = EMGClip::MoveClip(p_vTrackOffset, p_vPositionOffset);

	if(m_opChunks != NULL)
		EMImageBufferRepository::Instance() -> GetFrameWidthAndSkipRate(this, m_oFrame.GetHeight(), &m_vFrameWidth, &m_vSkipRate);

	if(m_opFXNodes != NULL)
	{
		list<FXNode>::iterator oIterator = m_opFXNodes -> begin();
		int32 vEffectTrackID;
		EMMediaClipInfo* opClipInfo;
		while(oIterator != m_opFXNodes -> end())
		{
			opClipInfo = static_cast<EMMediaClipInfo*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CLIP_INFO, &m_vMediaClipID));
			vEffectTrackID = opClipInfo -> m_vMediaEffectTrackID;//*(static_cast<int32*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_EFFECT_TRACK_ID, &vMediaTrackID)));
			(*oIterator).SetMediaEffectTrackID(vEffectTrackID);
			oIterator++;
		}
	}

	return vResult;
}

void VideoGClip::SetFrame(EMRect p_oFrame)
{
	EMGClip::SetFrame(p_oFrame);

	if(m_opChunks != NULL)
		EMImageBufferRepository::Instance() -> GetFrameWidthAndSkipRate(this, m_oFrame.GetHeight(), &m_vFrameWidth, &m_vSkipRate);

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

void VideoGClip::SetFramerate(float p_vFramerate)
{
	m_vFramerate = p_vFramerate;
	m_opChunks = NULL; // EMImageBufferRepository is modifying this content when changing the framerate
//	if(m_opChunks != NULL)
//		EMImageBufferRepository::Instance() -> GetFrameWidthAndSkipRate(this, m_oFrame.GetHeight(), &m_vFrameWidth, &m_vSkipRate);
;//cout_commented_out_4_release << "VideoGClip::SetFramerate: " << p_vFramerate << ", " << m_vFrameWidth << ", " << m_vSkipRate << endl;
}

// FIXME: p_opView is no longer needed
void VideoGClip::SetLabel(char* p_oLabel, EMView* p_opView)
{
	EMGClip::SetLabel(p_oLabel, m_opView);
	m_vNamePositionUpdated = false;
	//CalculateNamePosition(m_opView);
	dynamic_cast<VideoGClipContainer*>(m_opTrack -> GetClipContainer()) -> UpdateValueBox();
}

void VideoGClip::UpdateContent()
{
	m_opChunks = NULL;
}

