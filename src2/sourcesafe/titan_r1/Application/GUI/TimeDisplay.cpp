#include "TimeDisplay.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMFactory.h"
#include "EMMediaGlobals.h"
#include "EMMessages.h"
#include "EMSettingIDs.h"
#include "EMSettingsRepository.h"
#include "EMThreadRepository.h"
#include "EMToolTip.h"
#include "EMView.h"
#include "GUIGlobals.h"

TimeDisplay::TimeDisplay(EMView* p_opView, EMRect p_oFrame, EMTimeDisplayMode p_eMode) :
m_eDrawingMode(TIME_DISPLAY_DRAW_NORMAL),
m_eTimeDisplayMode(p_eMode),
m_oFrame(p_oFrame),
m_oInsideBorder(p_oFrame),
m_opSignature(NULL),
m_opView(p_opView),
m_vFramerate(0),
m_vPosition(-1),
m_vSignatureMajor(4),
m_vSignatureMajorSpacing(0),
m_vSignatureMinor(4),
m_vSignatureMinorSpacing(0),
m_vTempo(0),
m_vTimeTop(0),
m_vTimeLeft(0)
{
	if(p_eMode == EM_SMPTE)
		m_opToolTip = EMFactory::Instance() -> CreateToolTip(p_oFrame, "Displays Current Position in MINUTES:SECONDS:FRAMES", p_opView -> GetNativeView(), true);
	else
		m_opToolTip = EMFactory::Instance() -> CreateToolTip(p_oFrame, "Displays Current Position in MEASURES:BARS:FRAMES", p_opView -> GetNativeView(), true);
}

TimeDisplay::~TimeDisplay()
{
	delete m_opToolTip;
}
 
void TimeDisplay::CalculatePositions()
{
	int32 vAscent;
	int32 vDescent;
	
	if(m_opView -> LockWindow())
	{
		m_opView -> SetFont(EM_PLAIN);//EM_FIXED);
		m_opView -> CalculateStringHeight("xxxxxxxxx", vAscent, vDescent);
		int32 vYSpacing =  ((m_oFrame.GetHeight() - (vAscent + vDescent)) / 2);
		vYSpacing += 1; // FIXME: Why is this needed?
		m_vTimeTop = vYSpacing + vAscent;
		m_vTimeLeft = (m_oFrame.GetWidth() - m_opView -> CalculateStringWidth("xxxxxxxxx")) / 2;
		m_opView -> SetFont(EM_PLAIN);
		m_opView -> UnlockWindow();
	}
}

void TimeDisplay::Draw(EMRect p_oClippingRect)
{
	if(!m_oFrame.Intersects(p_oClippingRect))
		return;

	if(m_eDrawingMode == TIME_DISPLAY_DRAW_NORMAL)
	{
		m_opView -> SetHighColor(EMColor(245, 245, 255, 255));
		m_opView -> StrokeLine(m_oFrame.m_vRight, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vBottom);
		m_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vBottom, m_oFrame.m_vRight, m_oFrame.m_vBottom);
		m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_SHADOW);
		m_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vLeft, m_oFrame.m_vBottom);
		m_opView -> StrokeLine(m_oFrame.m_vLeft, m_oFrame.m_vTop, m_oFrame.m_vRight, m_oFrame.m_vTop);
		DrawContent(p_oClippingRect);
	}
	else
	{
		m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND);
		m_opView -> FillRect(m_oFrame);
	}
}

void TimeDisplay::DrawContent(EMRect p_oClippingRect)
{
	m_opView -> SetHighColor(EM_COLOR_VIEW_BACKGROUND_HIGHLIGHT);
	m_opView -> FillRect(m_oInsideBorder & p_oClippingRect);
	m_opView -> SetHighColor(EM_COLOR_TEXT);
	m_opView -> SetLowColor(EM_COLOR_VIEW_BACKGROUND_HIGHLIGHT);
	//m_opView -> SetFont(EM_FIXED);
	m_opView -> DrawString(m_oFrame.m_vLeft + m_vTimeLeft, m_oFrame.m_vTop + m_vTimeTop, m_vpTime);
	m_opView -> SetFont(EM_PLAIN);
}

EMRect TimeDisplay::Frame()
{
	return m_oFrame;
}

void TimeDisplay::Init()
{
	CalculatePositions();
	m_vFramerate = *(static_cast<float*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_FRAMERATE)));
	m_vpTime[0] = '\0';
	EMThreadRepository::Instance() -> GetThread(GUI_SERVICE_THREAD_NAME) -> AddListener(this);
	MessageReceived(NULL, EM_SETTING_UPDATED); // To force an update of all cached settings
	EMSettingsRepository::Instance() -> AddListener(this);
	m_oInsideBorder.InsetBy(1, 1);
}

bool TimeDisplay::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	string* opSignature;
	float vTempo;

	switch(p_vMessage)
	{
	case EM_SETTING_UPDATED:
eo << "TimeDisplay: EM_SETTING_UPDATED" << ef;
		m_vFramerate = *(static_cast<float*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_VIDEO_FRAMERATE)));
		vTempo = *(static_cast<float*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_TEMPO)));
//;//cout_commented_out_4_release << "TimeDisplay::MessageReceived: vTempo: " << vTempo << endl;
		if(vTempo != m_vTempo)
		{
			m_vTempo = vTempo;
			m_vSignatureMajorSpacing = static_cast<int64>((m_vSignatureMajor * EM_TICKS_PER_SECOND * 240) / (m_vSignatureMinor * m_vTempo));
			m_vSignatureMinorSpacing = m_vSignatureMajorSpacing / m_vSignatureMajor;
			m_vPosition = -1; // Force an update
		}
		opSignature = static_cast<string*>(EMSettingsRepository::Instance() -> GetSetting(SETTING_SIGNATURE));
//;//cout_commented_out_4_release << "TimeDisplay::MessageReceived: opSignature: " << *opSignature << endl;
		if(opSignature != m_opSignature)
		{
			m_opSignature = opSignature;
			sscanf(m_opSignature -> c_str(), "%d/%d", &m_vSignatureMajor, &m_vSignatureMinor);
			m_vSignatureMajorSpacing = static_cast<int64>((m_vSignatureMajor * EM_TICKS_PER_SECOND * 240) / (m_vSignatureMinor * m_vTempo));
			m_vSignatureMinorSpacing = m_vSignatureMajorSpacing / m_vSignatureMajor;
			m_vPosition = -1; // Force an update
		}
//;//cout_commented_out_4_release << "TimeDisplay::MessageReceived: m_vSignatureMajor: " << m_vSignatureMajor << ", m_vSignatureMinor: " << m_vSignatureMinor << endl;
		return true;
	default:
		return false;
	}
}

void TimeDisplay::SetFrame(EMRect p_oFrame)
{
	if(m_oFrame == p_oFrame)
		return;
		
	m_oFrame = p_oFrame;
	CalculatePositions();
}

void TimeDisplay::SetMode(TimeDisplayDrawingMode p_eMode)
{
	m_eDrawingMode = p_eMode;
}

void TimeDisplay::ThreadRun(EMThread* p_opThread)
{
//;//cout_commented_out_4_release << "TimeDisplay::ThreadRun" << endl;
	if(m_eDrawingMode == TIME_DISPLAY_DRAW_BLANK)
		return;

	int64 vNewPosition = *(static_cast<int64*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_CURRENT_PROJECT_POSITION, NULL)));
	if(vNewPosition != m_vPosition)
	{
		m_vPosition = vNewPosition;
		switch(m_eTimeDisplayMode)
		{
		case EM_SMPTE:
			{
				int vMinutes = static_cast<int>(m_vPosition / 60000000L);
				int vSeconds = static_cast<int>((m_vPosition % 60000000L) / 1000000L);
				int vFrames = static_cast<int>((m_vPosition % 1000000) * m_vFramerate / 1000000L);
				sprintf(m_vpTime, "%03d:%02d:%02d", vMinutes, vSeconds, vFrames);
				if(m_opView -> LockWindow())
				{
					m_opView -> BeginPaint();
					DrawContent(m_oInsideBorder);
					m_opView -> EndPaint();
					m_opView -> UnlockWindow();
				}
				break;
			}
		case EM_MEASURES:
			{
				int vMeasures = static_cast<int>(m_vPosition / m_vSignatureMajorSpacing) + 1;
				int vBeats = static_cast<int>((m_vPosition % m_vSignatureMajorSpacing) / m_vSignatureMinorSpacing) + 1;
				int vFrames = static_cast<int>((m_vPosition % 1000000) * m_vFramerate / 1000000L);
				sprintf(m_vpTime, "%04d:%01d:%02d", vMeasures, vBeats, vFrames);
				if(m_opView -> LockWindow())
				{
					m_opView -> BeginPaint();
					DrawContent(m_oInsideBorder);
					m_opView -> EndPaint();
					m_opView -> UnlockWindow();
				}
				break;
			}
		}
	}
}
