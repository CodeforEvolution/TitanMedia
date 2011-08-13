/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_COMMAND_LINE_IDS
#define __EM_COMMAND_LINE_IDS

// After each constant follows its command line flag equivalent

enum EMCommandLineIDs
{
	EM_CL_START_AUDIO_PROCESSING = 0x00000001, // sap
	EM_CL_START_VIDEO_PROCESSING = 0x00000002, // svp
	EM_CL_START_MIDI_PROCESSING = 0x00000004, // smp
	EM_CL_SEARCH_FOR_MIDI_INPUTS = 0x00000008, // sfmi
	EM_CL_SEARCH_FOR_MIDI_OUTPUTS = 0x00000010, // sfmo
	EM_CL_SEARCH_FOR_SMPTE_TRANSITIONS	= 0x00000020, // sfst
	EM_CL_SEARCH_FOR_NATIVE_VIDEO_EFFECTS = 0x00000040, // sfnve
	EM_CL_SEARCH_FOR_AUDIO_INPUTS	= 0x00000080, // sfai
	EM_CL_SEARCH_FOR_VIDEO_INPUTS = 0x00000100, // sfvi
	EM_CL_SEARCH_FOR_AUDIO_OUTPUTS = 0x00000200, // sfao
	EM_CL_SEARCH_FOR_VIDEO_OUTPUTS = 0x00000400, // sfvo
	EM_CL_SEARCH_FOR_DIRECTX_PLUGINS = 0x00000800, // sfdp
	EM_CL_SEARCH_FOR_AUDIO_ENCODERS = 0x00001000, // sfae
	EM_CL_SEARCH_FOR_VIDEO_ENCODERS = 0x00002000, // sfve
	EM_CL_DISPLAY_NEW_PROJECT_DIALOG = 0x0004000, // dnpd
	EM_CL_DISPLAY_SPLASH_SCREEN = 0x0008000, // dss
	EM_CL_DISPLAY_DEBUG_WINDOW = 0x0010000, // ddw
	EM_CL_WRITE_DEBUG_LOG = 0x0020000, // wdl
};

/*
/sap - Disable audio processing
/svp - Disable video processing
/smp - Disable MIDI processing
/sfmi - Disable search for MIDI inputs
/sfmo - Disable search for MIDI outputs
/sfst - Disable search for video transitions
/sfnve - Disable search for video effects
/sfai - Disable search for audio inputs
/sfvi - Disable search for video inputs
/sfao - Disable search for audio outputs
/sfvo - Disable search for video outputs
/sfdp - Disable search for DirectX plugins
/sfao - Disable search for audio encoders
/sfvo - Disable search for video encoders
/dnpd - Disable new project doalog (could crash Titan)
/dss - Disable splash screen
/ddw - Display debug window
/wdl - Write debug log ( <x>:\titan.log )
*/

#endif