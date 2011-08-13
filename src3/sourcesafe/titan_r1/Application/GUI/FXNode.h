/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* Contains some native code
*******************************************************/

#include "EMGlobals.h"

#ifndef __FX_NODE
#define __FX_NODE

#include "EMListener.h"
#include "EMMediaGlobals.h"
#include "EMRect.h"
#include "EMSlimValueBox.h"

#include <string>

class EMGClipContainer;
class EMListenerRepository;
class EMPopUpMenu;
class EMView;

class FXNode : public EMListener
{
public:
	FXNode(EMMediaType p_eType, int p_vNodeIndex, EMRect p_oParentFrame, EMView* p_opView, EMGClipContainer* p_opClipContainer, int32 p_vMediaEffectTrackID = -1, 
		int32 p_vMediaEffectEntryID = -1, int32 p_vMediaEffectID = -1, const char* p_vpEffectName = NULL);
	~FXNode();
	void CalculateEffectNamePosition();
	void Draw(EMRect p_oClippingRect, EMView* p_opView);
	int32 GetEffectTrackID();
	bool IsActive();
	bool MessageReceived(EMListenerRepository* p_opSender, uint32 p_vMessage);
	bool MouseDown(EMView* m_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	bool MouseDoubleClicked(EMView* p_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	bool MouseMoved(EMView* m_opView, int32 p_vX, int32 p_vY, bool p_vButtonOne, bool p_vButtonTwo, bool p_vButtonThree);
	bool MouseUp(EMView* m_opView, int32 p_vX, int32 p_vY);
	void SetIndex(int p_vNodeIndex);
	void SetMediaEffectTrackID(int32 p_vMediaEffectTrackID);
	void SetParent(EMGClipContainer* p_opClipContainer);

private:	
	void UpdateFrame();

private:
	EMMediaType m_eType;
	string m_oEffectName;
	EMRect m_oFrame; // Does not include the frame of m_oInputSignalValueBox
	EMSlimValueBox m_oInputSignalValueBox;
	EMRect m_oParentFrame;
	EMGClipContainer* m_opClipContainer;
	static EMPopUpMenu* m_opPopUpMenu;
	EMView* m_opView;
	bool m_vCommandIsSet;
	bool m_vIsActive;
	int32 m_vMediaEffectEntryID;
	int32 m_vMediaEffectID;
	int32 m_vMediaEffectTrackID;
	int m_vNodeIndex;
	int32 m_vStringLeft;
	int32 m_vStringTop;
	uint32 m_vValueBoxMessage;
};

#endif

