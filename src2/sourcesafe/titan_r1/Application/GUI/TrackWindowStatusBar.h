/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __TRACK_WINDOW_STATUSBAR
#define __TRACK_WINDOW_STATUSBAR

#include "EMListener.h"
#include "EMRect.h"

#include <string>

class EMListenerRepository;
class EMView;

class TrackWindowStatusBar : public EMListener
{
public:
	virtual ~TrackWindowStatusBar();
	void Draw(EMRect p_oClippingRect);
	EMView* GetView();
	static TrackWindowStatusBar* Instance();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void WriteMessage(string p_oMessage);
	
protected:
	TrackWindowStatusBar();

private:
	string m_oMessage;
	static TrackWindowStatusBar* m_opInstance;
	EMView* m_opView;
};

#endif

