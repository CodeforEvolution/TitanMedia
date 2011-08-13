/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __TOOLBOX
#define __TOOLBOX

#include "EMListener.h"
#include "EMRect.h"

#include <vector>

class EMBitmap;
class EMListenerRepository;
class EMToolTip;
class EMView;
class EMWindow;

class Toolbox : public EMListener
{
public:
	Toolbox();
	~Toolbox();
	void Init();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	void Show(EMView* p_opParentView);
	
private:
	void Draw(EMRect p_oUpdateRect, bool p_vRedrawChangedOnly = false);
	int GetCell(int32 p_vX, int32 p_vY);
	void MouseMoved(int32 p_vX, int32 p_vY);
	void MouseUp(int32 p_vX, int32 p_vY);

private:
	EMBitmap* m_opBitmapFrame;
	vector<EMBitmap*> m_oIcons;
	EMView* m_opParentView;
	EMToolTip* m_opToolTip;
	EMView* m_opView;
	EMWindow* m_opWindow;
	int m_vCurrentIcon;
	bool m_vIconsLoaded;
	int m_vOldIcon;
};

#endif