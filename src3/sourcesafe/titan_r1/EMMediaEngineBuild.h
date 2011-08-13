#ifndef __EM_MEDIA_ENGINE_BUILD
#define __EM_MEDIA_ENGINE_BUILD

//These flags are for debugging purposes, if one would want less processing threads running than the default 3 threads
#define START_AUDIO_PROCESSING 
//#define START_VIDEO_PROCESSING 
//#define START_MIDI_PROCESSING

//#define SEARCH_FOR_MIDI_INPUTS		//Leaves one thread "hanging" when quiting Titan. CreateDevEnum fucks it up!
//#define SEARCH_FOR_MIDI_OUTPUTS
//#define SEARCH_FOR_SMPTE_TRANSITIONS
//#define SEARCH_FOR_NATIVE_VIDEO_EFFECTS
//#define SEARCH_FOR_AUDIO_INPUTS
//#define SEARCH_FOR_VIDEO_INPUTS
#define SEARCH_FOR_AUDIO_OUTPUTS
//#define SEARCH_FOR_VIDEO_OUTPUTS
//#define SEARCH_FOR_DIRECTX_PLUGINS	//Leaves one thread "hanging" when quiting Titan. CreateDevEnum fucks it up!
//#define SEARCH_FOR_AUDIO_ENCODERS
//#define SEARCH_FOR_VIDEO_ENCODERS

#ifdef TRIAL_VERSION
#define ENABLE_SONG_LENGTH_LIMITATION
#define SONG_LENGTH_LIMIT 120000000 //two minutes in microseconds
#endif

#endif