#include "AudioInputSignalView.h"

#include "CommandIDs.h"
#include "EMCommandRepository.h"
#include "EMFactory.h"
#include "EMGUIGlobals.h"
#include "EMListenerRepository.h"
#include "EMMediaGlobals.h"
#include "EMMessages.h"
#include "EMRect.h"
#include "EMThreadRepository.h"
#include "EMView.h"
#include "GUIGlobals.h"

#include <string>
//#include <vector>

const int32 COMPONENT_GROUP_SPACING = 5;
const int32 COMPONENT_SPACING = 2;
const int32 COMPONENT_WIDTH = 100;

AudioInputSignalView::AudioInputSignalView() :
m_opView(EMFactory::Instance() -> CreateView(EMRect(0, 0, 1000, 1000), static_cast<EMResizingFlags>(EM_FOLLOW_LEFT | EM_FOLLOW_TOP))),
m_vSignal(false)
{
	m_opView -> AddListener(this);
	m_opView -> SetViewColor(EM_COLOR_VIEW_BACKGROUND);
}

AudioInputSignalView::~AudioInputSignalView()
{
	delete m_opView;
	string* opInputName;
	while(m_oInputNames.size() != 0)
	{
		opInputName = m_oInputNames.back();
		m_oInputNames.pop_back();
		delete opInputName;
	}
}

void AudioInputSignalView::Draw(EMRect p_oClippingRect)
{
//;//cout_commented_out_4_release << "AudioInputSignalView::Draw: nr inputs: " << m_oLabels.size() << endl;
//m_opView -> SetHighColor(EMColor(255, 0, 0, 255));
//m_opView -> StrokeRect(m_opView -> Bounds());
	list<EMSlimLabel>::iterator oLabelIterator = m_oLabels.begin();
	list<EMSignalMeter>::iterator oSignalMeterIterator = m_oSignalMeters.begin();
	while(oLabelIterator != m_oLabels.end())
	{
		(*oLabelIterator).Draw(p_oClippingRect);
		m_opView -> SetHighColor(EMColor(0, 0, 0, 255));
		m_opView -> StrokeRect((*oLabelIterator).Frame());
		(*oSignalMeterIterator).Draw(p_oClippingRect);
		oLabelIterator++;
		oSignalMeterIterator++;
	}
}

void AudioInputSignalView::EnableSignalMeters(bool p_vEnable)
{
	m_vSignal = p_vEnable;
	if(p_vEnable)
		EMThreadRepository::Instance() -> GetThread(GUI_SERVICE_THREAD_NAME) -> AddListener(this);
	else
		EMThreadRepository::Instance() -> GetThread(GUI_SERVICE_THREAD_NAME) -> RemoveListener(this);
}

EMView* AudioInputSignalView::GetView()
{
	return m_opView;
}

void AudioInputSignalView::Init()
{
	list<int32> oInputIDs;
	string oInputName;
	EMMediaType eMediaType = EMMediaType(EM_TYPE_ANY_AUDIO);
	oInputIDs = *(static_cast<list<int32>* >(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_INPUTS, &eMediaType)));
	list<int32>::iterator oIterator = oInputIDs.begin();
	while(oIterator != oInputIDs.end())
	{
		oInputName = string(static_cast<const char*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_INPUT_NAME, &(*oIterator))));
		m_oInputNames.push_back(EM_new string(string("   ") + oInputName));
		m_oLabels.push_back(EMSlimLabel(m_opView, EMRect(), m_oInputNames.back(), EM_COLOR_AUDIO_VIEW_BACKGROUND));
		m_oSignalMeters.push_back(EMSignalMeter(EMRect(), m_opView));
		oIterator++;
//;//cout_commented_out_4_release << "Input found: " << *opInputName << endl;
	}
}

void AudioInputSignalView::LayoutComponents()
{
	list<EMSlimLabel>::iterator oLabelIterator = m_oLabels.begin();
	list<EMSignalMeter>::iterator oSignalMeterIterator = m_oSignalMeters.begin();
	int vIndex = 0;
	while(oLabelIterator != m_oLabels.end())
	{
		(*oLabelIterator).SetFrame(EMRect(COMPONENT_GROUP_SPACING, 
											EM_METRICS_SIGNAL_METER_HEIGHT * vIndex + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT * vIndex + COMPONENT_GROUP_SPACING * (vIndex + 1) + COMPONENT_SPACING * vIndex, 
											(m_opView -> Bounds()).m_vRight - COMPONENT_GROUP_SPACING,//COMPONENT_GROUP_SPACING + COMPONENT_WIDTH - 1,
											EM_METRICS_SIGNAL_METER_HEIGHT * vIndex + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT * (vIndex + 1) + COMPONENT_GROUP_SPACING * (vIndex + 1) + COMPONENT_SPACING * vIndex - 1));
		(*oSignalMeterIterator).SetFrame(EMRect(COMPONENT_GROUP_SPACING, 
											EM_METRICS_SIGNAL_METER_HEIGHT * vIndex + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT * (vIndex + 1) + COMPONENT_GROUP_SPACING * (vIndex + 1) + COMPONENT_SPACING * (vIndex + 1), 
											(m_opView -> Bounds()).m_vRight - COMPONENT_GROUP_SPACING,//COMPONENT_GROUP_SPACING + COMPONENT_WIDTH - 1,
											EM_METRICS_SIGNAL_METER_HEIGHT * (vIndex + 1) + EM_METRICS_SLIM_EDIT_LABEL_HEIGHT * (vIndex + 1) + COMPONENT_GROUP_SPACING * (vIndex + 1) + COMPONENT_SPACING * (vIndex + 1) - 1));					
		oLabelIterator++;
		oSignalMeterIterator++;
		vIndex++;
	}
}

bool AudioInputSignalView::MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage)
{
	switch(p_vMessage)
	{
	case EM_DRAW:
		Draw(m_opView -> GetClippingRect());
		return true;
	case EM_VIEW_BOUNDS_CHANGED: // Most often due to the user resizing the window
//		if(m_opView -> LockWindow())
		{
			LayoutComponents();
			m_opView -> Invalidate(m_opView -> Bounds());
//			m_opView -> UnlockWindow();
		}
		return true;
	}
	return false;
}

void AudioInputSignalView::ThreadRun(EMThread* p_opThread)
{
	if(!m_vSignal)
		return;

	float* vpSignalMeterValues = static_cast<float*>(EMCommandRepository::Instance() -> ExecuteCommand(MEDIA_INFO_COMMAND_GET_INPUT_SIGNAL_METERS, NULL, NULL, NULL));
	if(vpSignalMeterValues == NULL)
		return;
//eo << "AudioInputSignalView::ThreadRun - Signal: " << ef;
	int vInputIndex(0);
	if(m_opView -> LockWindow())
	{
		m_vpSignalMeterValues = vpSignalMeterValues;
		//float* vpSignal;
		list<EMSignalMeter>::iterator oSignalMeterIterator = m_oSignalMeters.begin();
		while(oSignalMeterIterator != m_oSignalMeters.end())
		{
//eo << "AudioInputSignalView::ThreadRun - Signal: " << vInputIndex << ", " << m_vpSignalMeterValues[vInputIndex * 2] << ef;
			(*oSignalMeterIterator).SetSignal(&(m_vpSignalMeterValues[vInputIndex * 2]));
			vInputIndex++;
			oSignalMeterIterator++;
		}

		m_opView -> UnlockWindow();
	}
}




