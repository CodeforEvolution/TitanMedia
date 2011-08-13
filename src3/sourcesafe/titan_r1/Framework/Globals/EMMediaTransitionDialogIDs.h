#ifndef __EM_MEDIA_TRANSITION_DIALOG_IDS
#define __EM_MEDIA_TRANSITION_DIALOG_IDS

#include "EMGlobals.h"

#pragma pack( push, ENTER_EM_MEDIA_TRANSITION_DIALOG_IDS )

#pragma pack(1)

enum EMMediaTransitionDialogIDs
{
	EM_TRANSITION_FADER,
	EM_TRANSITION_WIPE,
	EM_TRANSITION_KEY,
	EM_TRANSITION_DX
};

enum EMMediaColorKeyIDs
{
	EM_KEY_COLOR,
	EM_KEY_HUE,
	EM_KEY_LUMINANCE,
	EM_KEY_BLUE,
	EM_KEY_ALPHA
};

class EMWipeParameters
{
public:
	uint8 m_vR;
	uint8 m_vG;
	uint8 m_vB;
	int32 m_vSoftness;
	int32 m_vWidth;
	int32 m_vOffsetX;
	int32 m_vOffsetY;
	int32 m_vReplicateX;
	int32 m_vReplicateY;
};

class EMKeyParameters
{
public:
	EMMediaColorKeyIDs m_vColorID;
	uint8 m_vR;
	uint8 m_vG;
	uint8 m_vB;
	int m_vHue;
	int m_vLuminance;
	int m_vTolerance;
	bool m_vInvert;
};


class EMDialogParameters
{
public:
	EMMediaTransitionDialogIDs m_vID;

	union __Params
	{
		EMWipeParameters m_vWipe;
		EMKeyParameters m_vKey;
	} m_vParams;

	bool m_vSwap;
};

#pragma pack(pop, ENTER_EM_MEDIA_TRANSITION_DIALOG_IDS )

#endif
