/*******************************************************
* Creator: Martin Rudefelt
* Portability: non-native
*-------------------------------------------------------
* 
*******************************************************/

#include "EMGlobals.h"

#ifndef __COMMAND_IDS
#define __COMMAND_IDS

#include "EMCommandRepository.h"

#define PROJECT_COMMAND_OFFSET 1000
#define MEDIA_INFO_COMMAND_OFFSET 1000
#define NETWORK_COMMAND_OFFSET 3000

const uint32 COMMAND_EXPAND_TRACK_INFO = COMMAND_BASE_OFFSET + 1;
const uint32 COMMAND_EXPAND_TIMELINE = COMMAND_BASE_OFFSET + 2;
const uint32 COMMAND_ADD_G_AUDIO_TRACK = COMMAND_BASE_OFFSET + 3;
const uint32 COMMAND_ADD_G_AUDIO_CLIP = COMMAND_BASE_OFFSET + 4;
const uint32 COMMAND_ZOOM_IN_TRACK_VERTICAL = COMMAND_BASE_OFFSET + 5;
const uint32 COMMAND_ZOOM_OUT_TRACK_VERTICAL = COMMAND_BASE_OFFSET + 6;
const uint32 COMMAND_MOVE_CLIPS = COMMAND_BASE_OFFSET + 7;
const uint32 COMMAND_CLONE_CLIPS = COMMAND_BASE_OFFSET + 8;
const uint32 COMMAND_DELETE_CLIPS = COMMAND_BASE_OFFSET + 9;
const uint32 COMMAND_ZOOM_IN_HORIZONTAL = COMMAND_BASE_OFFSET + 10;
const uint32 COMMAND_ZOOM_OUT_HORIZONTAL = COMMAND_BASE_OFFSET + 11;
const uint32 COMMAND_TOGGLE_RELATIVE_MOVE = COMMAND_BASE_OFFSET + 12;
const uint32 COMMAND_TOGGLE_SNAPPING = COMMAND_BASE_OFFSET + 13;
const uint32 COMMAND_MOVE_TRACKS = COMMAND_BASE_OFFSET + 14;
const uint32 COMMAND_ADD_G_VIDEO_TRACK = COMMAND_BASE_OFFSET + 15;
const uint32 COMMAND_ADD_G_VIDEO_CLIP = COMMAND_BASE_OFFSET + 16;
const uint32 COMMAND_LOAD_AUDIO = COMMAND_BASE_OFFSET + 17;
const uint32 COMMAND_LOAD_VIDEO = COMMAND_BASE_OFFSET + 18;
const uint32 COMMAND_DELETE_CLIPS_FROM_LIST = COMMAND_BASE_OFFSET + 19;
const uint32 COMMAND_TOGGLE_LOOP = COMMAND_BASE_OFFSET + 20;
const uint32 COMMAND_UNDO = COMMAND_BASE_OFFSET + 21;
const uint32 COMMAND_REDO = COMMAND_BASE_OFFSET + 22;
const uint32 COMMAND_PLAY = COMMAND_BASE_OFFSET + 23;
const uint32 COMMAND_STOP = COMMAND_BASE_OFFSET + 24;
const uint32 COMMAND_PROJECT_BEGIN = COMMAND_BASE_OFFSET + 25;
const uint32 COMMAND_PROJECT_END = COMMAND_BASE_OFFSET + 26;
const uint32 COMMAND_REGION_BEGIN = COMMAND_BASE_OFFSET + 27;
const uint32 COMMAND_REGION_END = COMMAND_BASE_OFFSET + 28;
const uint32 COMMAND_REWIND = COMMAND_BASE_OFFSET + 29;
const uint32 COMMAND_FAST_FORWARD = COMMAND_BASE_OFFSET + 30;
const uint32 COMMAND_ZOOM_IN_TRACKS_VERTICAL = COMMAND_BASE_OFFSET + 31;
const uint32 COMMAND_ZOOM_OUT_TRACKS_VERTICAL = COMMAND_BASE_OFFSET + 32;
const uint32 COMMAND_PREPARE_RECORDING = COMMAND_BASE_OFFSET + 33;
const uint32 COMMAND_SET_CLIP_EDIT = COMMAND_BASE_OFFSET + 34;
const uint32 COMMAND_SET_TRACK_EDIT = COMMAND_BASE_OFFSET + 35;
const uint32 COMMAND_SET_FX_EDIT = COMMAND_BASE_OFFSET + 36;
const uint32 COMMAND_TOGGLE_ENABLE_MIDI = COMMAND_BASE_OFFSET + 37;
const uint32 COMMAND_TOGGLE_EXPERT_MODE = COMMAND_BASE_OFFSET + 38;
const uint32 COMMAND_TOGGLE_AUDIO_SIGNAL_WINDOW = COMMAND_BASE_OFFSET + 39;
const uint32 COMMAND_EXPAND_UTILITY_VIEW = COMMAND_BASE_OFFSET + 40;
const uint32 COMMAND_UTILITY_BUTTON_PRESSED = COMMAND_BASE_OFFSET + 41;
const uint32 COMMAND_GET_G_TRACK_INDEX = COMMAND_BASE_OFFSET + 42;

const uint32 MEDIA_COMMAND_TOGGLE_PLAYBACK = MEDIA_COMMAND_BASE_OFFSET + 1;
const uint32 MEDIA_COMMAND_START_PLAYBACK = MEDIA_COMMAND_BASE_OFFSET + 2;
const uint32 MEDIA_COMMAND_STOP_PLAYBACK = MEDIA_COMMAND_BASE_OFFSET + 3;
const uint32 MEDIA_COMMAND_TOGGLE_TRACK_MUTE = MEDIA_COMMAND_BASE_OFFSET + 4;
const uint32 MEDIA_COMMAND_TOGGLE_TRACK_SOLO = MEDIA_COMMAND_BASE_OFFSET + 5;
const uint32 MEDIA_COMMAND_SET_TRACK_PAN = MEDIA_COMMAND_BASE_OFFSET + 6;
const uint32 MEDIA_COMMAND_SET_TRACK_OUTPUT = MEDIA_COMMAND_BASE_OFFSET + 7;
const uint32 MEDIA_COMMAND_SET_TRACK_INPUT = MEDIA_COMMAND_BASE_OFFSET + 8;
const uint32 MEDIA_COMMAND_SET_TRACK_NAME = MEDIA_COMMAND_BASE_OFFSET + 9;
const uint32 MEDIA_COMMAND_SET_TRACK_FADER = MEDIA_COMMAND_BASE_OFFSET + 10;
const uint32 MEDIA_COMMAND_SET_CLIP_OUTPUT = MEDIA_COMMAND_BASE_OFFSET + 11;
const uint32 MEDIA_COMMAND_SEEK = MEDIA_COMMAND_BASE_OFFSET + 12;
const uint32 MEDIA_COMMAND_MOVE_CLIP = MEDIA_COMMAND_BASE_OFFSET + 13;
const uint32 MEDIA_COMMAND_IMPORT_MEDIA = MEDIA_COMMAND_BASE_OFFSET + 14;
const uint32 MEDIA_COMMAND_DROP_MEDIA = MEDIA_COMMAND_BASE_OFFSET + 15;
const uint32 MEDIA_COMMAND_CREATE_AUDIO_TRACK = MEDIA_COMMAND_BASE_OFFSET + 16;
const uint32 MEDIA_COMMAND_CREATE_VIDEO_TRACK = MEDIA_COMMAND_BASE_OFFSET + 17;
const uint32 MEDIA_COMMAND_DELETE_CLIP = MEDIA_COMMAND_BASE_OFFSET + 18;
const uint32 MEDIA_COMMAND_DELETE_TRACK = MEDIA_COMMAND_BASE_OFFSET + 19;
const uint32 MEDIA_COMMAND_CLONE_CLIP = MEDIA_COMMAND_BASE_OFFSET + 20;
const uint32 MEDIA_COMMAND_PREPARE_RECORDING = MEDIA_COMMAND_BASE_OFFSET + 21;
const uint32 MEDIA_COMMAND_TOGGLE_TRACK_ARM = MEDIA_COMMAND_BASE_OFFSET + 22;
const uint32 MEDIA_COMMAND_SET_CLIP_MARK_IN = MEDIA_COMMAND_BASE_OFFSET + 23;
const uint32 MEDIA_COMMAND_SET_CLIP_MARK_OUT = MEDIA_COMMAND_BASE_OFFSET + 24;
const uint32 MEDIA_COMMAND_SET_CLIP_NAME = MEDIA_COMMAND_BASE_OFFSET + 25;

const uint32 MEDIA_INFO_COMMAND_GET_MEDIA_FILE_DURATION = MEDIA_COMMAND_BASE_OFFSET + MEDIA_INFO_COMMAND_OFFSET + 1;
const uint32 MEDIA_INFO_COMMAND_GET_OUTPUTS = MEDIA_COMMAND_BASE_OFFSET + MEDIA_INFO_COMMAND_OFFSET + 2;
const uint32 MEDIA_INFO_COMMAND_GET_INPUTS = MEDIA_COMMAND_BASE_OFFSET + MEDIA_INFO_COMMAND_OFFSET + 3;
const uint32 MEDIA_INFO_COMMAND_GET_OUTPUT_NAME = MEDIA_COMMAND_BASE_OFFSET + MEDIA_INFO_COMMAND_OFFSET + 4;
const uint32 MEDIA_INFO_COMMAND_GET_INPUT_NAME = MEDIA_COMMAND_BASE_OFFSET + MEDIA_INFO_COMMAND_OFFSET + 5;
const uint32 MEDIA_INFO_COMMAND_GET_MEDIA_TRACK_IDS = MEDIA_COMMAND_BASE_OFFSET + MEDIA_INFO_COMMAND_OFFSET + 6;
const uint32 MEDIA_INFO_COMMAND_GET_SIGNAL_METER_VALUE = MEDIA_COMMAND_BASE_OFFSET + MEDIA_INFO_COMMAND_OFFSET + 7;
const uint32 MEDIA_INFO_COMMAND_GET_CLIP_ENTRY_ID = MEDIA_COMMAND_BASE_OFFSET + MEDIA_INFO_COMMAND_OFFSET + 8;
const uint32 MEDIA_INFO_COMMAND_GET_CLIP_NUM_CHANNELS = MEDIA_COMMAND_BASE_OFFSET + MEDIA_INFO_COMMAND_OFFSET + 9;
const uint32 MEDIA_INFO_COMMAND_GET_CLIP_MARK_IN = MEDIA_COMMAND_BASE_OFFSET + MEDIA_INFO_COMMAND_OFFSET + 10;
const uint32 MEDIA_INFO_COMMAND_GET_CLIP_MARK_OUT = MEDIA_COMMAND_BASE_OFFSET + MEDIA_INFO_COMMAND_OFFSET + 11;
const uint32 MEDIA_INFO_COMMAND_GET_CLIP_DURATION = MEDIA_COMMAND_BASE_OFFSET + MEDIA_INFO_COMMAND_OFFSET + 12;
const uint32 MEDIA_INFO_COMMAND_GET_CLIP_NAME = MEDIA_COMMAND_BASE_OFFSET + MEDIA_INFO_COMMAND_OFFSET + 13;
const uint32 MEDIA_INFO_COMMAND_GET_CLIP_FILENAME = MEDIA_COMMAND_BASE_OFFSET + MEDIA_INFO_COMMAND_OFFSET + 14;
const uint32 MEDIA_INFO_COMMAND_GET_CLIP_POSITION = MEDIA_COMMAND_BASE_OFFSET + MEDIA_INFO_COMMAND_OFFSET + 15;
const uint32 MEDIA_INFO_COMMAND_GET_AUDIO_CLIP_HIGHRES_CONTENT = MEDIA_COMMAND_BASE_OFFSET + MEDIA_INFO_COMMAND_OFFSET + 16;
const uint32 MEDIA_INFO_COMMAND_GET_AUDIO_CLIP_LOWRES_CONTENT = MEDIA_COMMAND_BASE_OFFSET + MEDIA_INFO_COMMAND_OFFSET + 17;
const uint32 MEDIA_INFO_COMMAND_GET_AUDIO_FILE_NUM_CHANNELS = MEDIA_COMMAND_BASE_OFFSET + MEDIA_INFO_COMMAND_OFFSET + 18;
const uint32 MEDIA_INFO_COMMAND_GET_AUDIO_FILE_NUM_SAMPLES = MEDIA_COMMAND_BASE_OFFSET + MEDIA_INFO_COMMAND_OFFSET + 19;
const uint32 MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME = MEDIA_COMMAND_BASE_OFFSET + MEDIA_INFO_COMMAND_OFFSET + 20;
const uint32 MEDIA_INFO_COMMAND_GET_VIDEO_CLIP_CONTENT = MEDIA_COMMAND_BASE_OFFSET + MEDIA_INFO_COMMAND_OFFSET + 21;
const uint32 MEDIA_INFO_COMMAND_GET_VIDEO_ASPECT_RATIO = MEDIA_COMMAND_BASE_OFFSET + MEDIA_INFO_COMMAND_OFFSET + 22;
const uint32 MEDIA_INFO_COMMAND_GET_VIDEO_FILE_NUM_FRAMES = MEDIA_COMMAND_BASE_OFFSET + MEDIA_INFO_COMMAND_OFFSET + 23;
const uint32 MEDIA_INFO_COMMAND_GET_FILE_TYPE = MEDIA_COMMAND_BASE_OFFSET + MEDIA_INFO_COMMAND_OFFSET + 24;
const uint32 MEDIA_INFO_COMMAND_IS_PLAYING = MEDIA_COMMAND_BASE_OFFSET + MEDIA_INFO_COMMAND_OFFSET + 25;

const uint32 PROJECT_COMMAND_CREATE_PROJECT = COMMAND_BASE_OFFSET + PROJECT_COMMAND_OFFSET + 1;
const uint32 PROJECT_COMMAND_LOAD_PROJECT = COMMAND_BASE_OFFSET + PROJECT_COMMAND_OFFSET + 2;
const uint32 PROJECT_COMMAND_SAVE_PROJECT = COMMAND_BASE_OFFSET + PROJECT_COMMAND_OFFSET + 3;
const uint32 PROJECT_COMMAND_CLOSE_PROJECT = COMMAND_BASE_OFFSET + PROJECT_COMMAND_OFFSET + 4;
const uint32 PROJECT_COMMAND_GET_DIRECTORY = COMMAND_BASE_OFFSET + PROJECT_COMMAND_OFFSET + 5;

const uint32 NETWORK_COMMAND_CONNECT_TCP = NETWORK_COMMAND_OFFSET + 1;
const uint32 NETWORK_COMMAND_CHAT_STARTUP = NETWORK_COMMAND_OFFSET + 2;
const uint32 NETWORK_COMMAND_CHAT_CREATE = NETWORK_COMMAND_OFFSET + 3;
const uint32 NETWORK_COMMAND_PING = NETWORK_COMMAND_OFFSET + 4;
const uint32 NETWORK_COMMAND_TERMINATE_MASTER_SOCKET = NETWORK_COMMAND_OFFSET + 5;
const uint32 NETWORK_COMMAND_SEND_STRING = NETWORK_COMMAND_OFFSET + 6;
const uint32 NETWORK_COMMAND_STARTUP = NETWORK_COMMAND_OFFSET + 7;
const uint32 NETWORK_COMMAND_CLEANUP = NETWORK_COMMAND_OFFSET + 8;
const uint32 NETWORK_COMMAND_SET_HOST = NETWORK_COMMAND_OFFSET + 9;

#endif

