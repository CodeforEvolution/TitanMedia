/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
*
*******************************************************/

#include "EMGlobals.h"

#ifndef __EM_SETTING_IDS
#define __EM_SETTING_IDS

#define FRAMEWORK_PROJECT_SETTING_ID_OFFSET 1000
#define FRAMEWORK_SYSTEM_SETTING_ID_OFFSET 2000

// Project settings
#define SETTING_VIDEO_FRAMERATE FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 1 // float, e.g. 24 frames/second
#define SETTING_TEMPO FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 2 // float, e.g. 120.000 bpm
#define SETTING_TIME_ZOOM_SCALE FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 3 // int64, The horizontal zoom in the track view
#define SETTING_SIGNATURE FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 4 // string, e.g. "3/4"
#define SETTING_AUDIO_FRAMERATE FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 5 // int32, e.g. 44100kHz
#define SETTING_EDIT_MODE FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 6 // int, currently three modes: track, clip and FX edit mode
#define SETTING_PROJECT_ID FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 7 // int, the ID of the project connected to the settings repository
#define SETTING_AUDIO_NUM_CHANNELS FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 8 // int32
#define SETTING_AUDIO_ENABLED FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 9 // bool
#define SETTING_VIDEO_ENABLED FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 10 // bool
#define SETTING_MIDI_ENABLED FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 11 // bool
#define SETTING_VIDEO_PREVIEW FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 12 // void* (can be casted to a EMVideoPreview object)
#define SETTING_VIDEO_FORMAT_WIDTH FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 14 // int, the width of the video work format
#define SETTING_VIDEO_FORMAT_HEIGHT FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 15 // int, the height of the video work format
#define SETTING_VIDEO_RENDER_FORMAT_HEIGHT FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 16 // int, the height of the rendered video work format
#define SETTING_VIDEO_RENDER_FORMAT_WIDTH FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 17 // int, the width of the rendered video work format
#define SETTING_VIDEO_RENDER_FRAMERATE FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 18 // float, e.g. 24 frames/second
#define SETTING_VIDEO_RENDER_CODEC FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 19 // string, e.g. "Intel Indeo"
#define SETTING_AUDIO_RENDER_CODEC FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 20 // string, e.g. "Raw Audio"
#define SETTING_RENDER_CODEC_FAMILY FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 21 // string, e.g. "Avi"
#define SETTING_RENDER_CODEC_QUALITY FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 22 // The render quality from 1 to 100 percent (but what's the data type Jesper?)
#define SETTING_MIDI_METRONOME_ENABLED FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 23 // bool, if an audible metronome should be hear when recording
#define SETTING_VIEW_CLIP_CONTENT_ENABLED FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 24 // bool, if the contents of clips should be displayed
#define SETTING_SIGNAL_METERS_ENABLED FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 25 // bool, if the signal meters for the tracks are active
#define SETTING_AUDIO_LOCKED_TO_TEMPO FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 26 // bool, if audio clips move when the tempo changes
#define SETTING_VIDEO_LOCKED_TO_TEMPO FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 27 // bool, if video clips move when the tempo changes
#define SETTING_VIDEO_RENDER_QUALITY FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 28 // int, the video render quality
#define SETTING_MIDI_AUTO_QUANTIZE_ENABLED FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 29 //bool, true if auto-quantize is enabled
#define SETTING_MIDI_AUTO_QUANTIZE_MODE FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 30 //string, "1/4", "1/8tr", etc
#define SETTING_MIDI_AUTO_QUANTIZE_LENGTH_ENABLED FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 31 //bool, true if auto-quantize should quantize note-lengths as well as start-times.
#define SETTING_DISABLE_FEATURES_FLAGS FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 32 //int32 (number of measures of count-in. 0 = no count in (ie disabled))
#define SETTING_VIDEO_BACKGROUND_COLOR FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 33 //int32 (the video silence color, 0x00RRGGBB. 0x00000000 = black, 0x00ffffff = white)
#define SETTING_USE_PREDEFINED_VIDEO_FORMAT FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 34 //bool, use predefine video format in project settings dialog or not
#define SETTING_USE_ASPECT_RATIOS FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 35 //bool, use aspect ratios in project settings dialog or not
#define SETTING_USE_PREDEFINED_VIDEO_FRAMERATE FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 36 //bool, use predefine video framerate in project settings dialog or not
#define SETTING_ASPECT_RATIOS FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 37 //int, which aspect rations to use (in project settings dialog aspect rations combobox)
#define SETTING_MIDI_METRONOME_USE_PC_SPEAKER FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 38 //bool, enabled/disabled
#define SETTING_MIDI_METRONOME_USE_MIDI_NOTE FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 39 //bool, enabled/disabled
#define SETTING_MIDI_METRONOME_MIDI_DEVICE FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 40 //string (name of media output)
#define SETTING_MIDI_METRONOME_MIDI_CHANNEL FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 41 //int32 (0-15)
#define SETTING_MIDI_METRONOME_MIDI_NOTE_HIGH_KEY FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 42 //int (0-127)
#define SETTING_MIDI_METRONOME_MIDI_NOTE_HIGH_VELOCITY FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 43 //int (0-127)
#define SETTING_MIDI_METRONOME_MIDI_NOTE_HIGH_DURATION FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 44 //int64, microseconds
#define SETTING_MIDI_METRONOME_MIDI_NOTE_LOW_KEY FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 45 //int (0-127)
#define SETTING_MIDI_METRONOME_MIDI_NOTE_LOW_VELOCITY FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 46 //int (0-127)
#define SETTING_MIDI_METRONOME_MIDI_NOTE_LOW_DURATION FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 47 //int64 (microseconds)
#define SETTING_MIDI_METRONOME_COUNT_IN FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 48 //int32 (number of measures of count-in. 0 = no count in (ie disabled))
#define SETTING_MIDI_METRONOME_DURING_PLAYBACK FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 49 //bool (true if metronome is on during playback)
#define SETTING_MIDI_METRONOME_DURING_RECORDING FRAMEWORK_PROJECT_SETTING_ID_OFFSET + 50 //bool (true if metronome is on during recording)

// System settings
#define SETTING_SNAP_THRESHOLD FRAMEWORK_SYSTEM_SETTING_ID_OFFSET + 1 // int32, how many max pixels difference when snapping
#define SETTING_NETWORK_HOST FRAMEWORK_SYSTEM_SETTING_ID_OFFSET + 2 //string, Network port to use for TCP
#define SETTING_NETWORK_USE_TCP FRAMEWORK_SYSTEM_SETTING_ID_OFFSET + 3 //bool, if true use TCP else use UDP
#define SETTING_NETWORK_TCP_PORT FRAMEWORK_SYSTEM_SETTING_ID_OFFSET + 4 //int32, Network port to use for TCP
#define SETTING_NETWORK_UDP_PORT FRAMEWORK_SYSTEM_SETTING_ID_OFFSET + 5 //int32, Network port to use for UDP
#define SETTING_NETWORK_USE_INTERNET FRAMEWORK_SYSTEM_SETTING_ID_OFFSET + 6 //bool, if true connect to Intenet Server else connect to Intranet(LAN) server
#define SETTING_NETWORK_USER_ID FRAMEWORK_SYSTEM_SETTING_ID_OFFSET + 7 //int64, User ID
#define SETTING_NETWORK_USER_PASSWORD FRAMEWORK_SYSTEM_SETTING_ID_OFFSET + 8 //string, Password. If SETTING_NETWORK_SAVE_PASSWORD = false password is empty
#define SETTING_NETWORK_SAVE_PASSWORD FRAMEWORK_SYSTEM_SETTING_ID_OFFSET + 9 //bool, Network port to use for TCP
#define SETTING_AUDIO_PROCESSING_AHEAD FRAMEWORK_SYSTEM_SETTING_ID_OFFSET + 10 //int32, the "processing-ahead" time for audio processing, in milliseconds
#define SETTING_AUDIO_BUFFER_SIZE FRAMEWORK_SYSTEM_SETTING_ID_OFFSET + 11 //int32, the buffer size for the internal audio buffers, in bytes
#define SETTING_SOFTWARE_IS_CRIPPLED FRAMEWORK_SYSTEM_SETTING_ID_OFFSET + 12 // bool, If the software has limited functionality, e.g. a non-registered shareware version
#define SETTING_REGISTRATION_USER_ID FRAMEWORK_SYSTEM_SETTING_ID_OFFSET + 13 // string, e.g. the name of the user used to verify the serial number
#define SETTING_REGISTRATION_SERIAL_NUMBER FRAMEWORK_SYSTEM_SETTING_ID_OFFSET + 14 // int64, the number that, in combination with the user id, registers Titan

#endif

