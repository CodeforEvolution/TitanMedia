/*******************************************************
* This code is the copyrighted property of Elegant Media
*-------------------------------------------------------
* Creator: Johan Dunberg
* Component: Application, Dialog
* Portability: non-native (Uses some native code to get
*			   screen-rect/workarea)
*-------------------------------------------------------
* 
*******************************************************/
#include "EMGlobals.h"

#ifndef __SPLASH_WINDOW
#define __SPLASH_WINDOW

#include "EMListener.h"

#include <string>

class EMFactory;

class EMBitmap;
class EMWindow;
class EMView;

class EMListenerRepository;

class SplashWindow : public EMListener
{
public:
	SplashWindow();
	~SplashWindow();
	void Display();
	void DisplayInfo(const char* p_vVersionNumber, bool p_vIsCrippled);
	void DisplayText(const char* p_vText);
	void Draw();
	void Hide();
	void Init();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void CenterWindow();

private:
	EMBitmap* m_opSplashBitmap;
	EMView* m_opView;
	EMWindow* m_opWindow;
	string m_oText;
	string m_oVersionNumber;
	bool m_vIsCrippled;
};


#endif