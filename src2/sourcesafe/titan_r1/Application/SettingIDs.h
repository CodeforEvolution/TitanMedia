/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __SETTING_IDS
#define __SETTING_IDS

#define SETTING_RELATIVE 1 // bool, if moving clips are relative their original position or not
#define SETTING_SNAPPING_MODE 2 // string, e.g. Snap same
#define SETTING_SNAPPING 3 // bool, if snapping is on/off
#define SETTING_TOOLBOX_MODE 4 // int (an enum really), the currently selected mouse tool
#define SETTING_REGION_BEGIN 5 // int64, beginning of the current region (-1 if no region exists)
#define SETTING_REGION_END 6 // int64, end of the current region (-1 if no region exists)
#define SETTING_LOOP 7 // bool, if looping mode is on or off
#define SETTING_AUDIO_SAMPLE_TYPE 10 //int (an enum really)
#define SETTING_EXPERT_MODE 11 // bool, if expert mode is enabled

#endif

