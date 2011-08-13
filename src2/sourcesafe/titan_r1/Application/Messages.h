/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __MESSAGES
#define __MESSAGES

#include "EMMessages.h"

const uint32 BUTTONBAR_CHANGED = EM_MESSAGE_BASE_OFFSET + 1; // Covers all but the track filter buttons
const uint32 TRACK_FILTERS_CHANGED = EM_MESSAGE_BASE_OFFSET + 2;

#endif