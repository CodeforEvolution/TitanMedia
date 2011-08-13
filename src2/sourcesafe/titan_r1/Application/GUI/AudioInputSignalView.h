/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __AUDIO_INPUT_SIGNAL_VIEW
#define __AUDIO_INPUT_SIGNAL_VIEW

#include "EMListener.h"
#include "EMRect.h"
#include "EMSignalMeter.h"
#include "EMSlimLabel.h"
#include "EMThreadListener.h"

#include <list>

class EMListenerRepository;
class EMView;

class AudioInputSignalView : public EMListener, public EMThreadListener
{
public:	
	AudioInputSignalView();
	~AudioInputSignalView();
	void EnableSignalMeters(bool p_vEnable);
	void Draw(EMRect p_oClippingRect);
	EMView* GetView();
	void Init();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void ThreadRun(EMThread* p_opThread);

private:
	void LayoutComponents();

private:
	list<string*> m_oInputNames;
	list<EMSlimLabel> m_oLabels;
	EMView* m_opView;
	list<EMSignalMeter> m_oSignalMeters;
	//list<list<float>* > m_oSignalValues;
	float* m_vpSignalMeterValues; // Points to a [50}[2] matrix
	bool m_vSignal;
};

#endif



