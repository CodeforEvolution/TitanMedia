#include "EMMediaCommandFactory.h"

#include "CommandIDs.h"
#include "MediaCommandTogglePlayback.h"
#include "MediaCommandStartPlayback.h"
#include "MediaCommandStopPlayback.h"
#include "MediaCommandSetTrackPan.h"
#include "MediaCommandSetTrackOutput.h"
#include "MediaCommandSetTrackInput.h"
#include "MediaCommandSetTrackName.h"
#include "MediaCommandSetTrackFader.h"
#include "MediaCommandSetClipOutput.h"
#include "MediaCommandSeek.h"
#include "MediaCommandMoveClip.h"
#include "MediaCommandCloneClip.h"
#include "MediaCommandImportMedia.h"
#include "MediaCommandDropMedia.h"
#include "MediaCommandCreateAudioTrack.h"
#include "MediaCommandCreateVideoTrack.h"
#include "MediaCommandCreateMIDITrack.h"
#include "MediaCommandDeleteClip.h"
#include "MediaCommandDeleteTrack.h"
#include "MediaCommandSetClipMarkIn.h"
#include "MediaCommandSetClipMarkOut.h"
#include "MediaCommandSetTrackMuteState.h"
#include "MediaCommandToggleTrackSolo.h"
#include "MediaCommandToggleTrackArm.h"
#include "MediaCommandPrepareRecording.h"
#include "MediaCommandDeleteVideoClipContent.h"
#include "MediaInfoCommandGetAudioEntryNumChannels.h"
#include "MediaInfoCommandGetAudioEntryNumSamples.h"
#include "MediaInfoCommandGetEntryDuration.h"
#include "MediaInfoCommandGetClipDuration.h"
#include "MediaInfoCommandGetEntryFileName.h"
#include "MediaInfoCommandGetOutputs.h"
#include "MediaInfoCommandGetOutputName.h"
#include "MediaInfoCommandGetInputs.h"
#include "MediaInfoCommandGetInputName.h"
#include "MediaInfoCommandGetAudioTrackIDs.h"
#include "MediaInfoCommandGetSignalMeterValue.h"
#include "MediaInfoCommandGetAudioClipContent.h"
#include "MediaInfoCommandGetAudioClipLowResContent.h"
#include "MediaInfoCommandGetClipNumChannels.h"
#include "MediaInfoCommandGetClipMarkIn.h"
#include "MediaInfoCommandGetClipMarkOut.h"
#include "MediaInfoCommandGetEntryID.h"
#include "MediaInfoCommandGetClipName.h"
#include "MediaInfoCommandGetClipFileName.h"
#include "MediaInfoCommandGetClipPosition.h"
#include "MediaInfoCommandGetVideoClipContent.h"
#include "MediaInfoCommandGetVideoAspectRatio.h"
#include "MediaInfoCommandGetVideoEntryNumFrames.h"
#include "MediaInfoCommandGetAudioEntryFormatString.h"
#include "MediaInfoCommandGetVideoEntryFormatString.h"
#include "MediaInfoCommandIsPlaying.h"
#include "MediaInfoCommandGetAudioRenderOutput.h"
#include "MediaInfoCommandGetVideoRenderOutput.h"
#include "MediaInfoCommandGetTrackInfo.h"
#include "MediaInfoCommandGetClipInfo.h"
#include "MediaInfoCommandGetAllTrackIDs.h"
#include "MediaInfoCommandGetCodecs.h"
#include "MediaCommandSetClipName.h"
#include "MediaCommandPlayBuffer.h"
#include "MediaCommandMoveTrack.h"
#include "MediaCommandCreateEffectTrack.h"
#include "MediaCommandInsertEffect.h"
#include "MediaCommandSetEffectTrackOutput.h"
#include "MediaInfoCommandGetEffectEntryInfo.h"
#include "MediaInfoCommandGetEffectInfo.h"
#include "MediaInfoCommandGetEffectTrackInfo.h"
#include "MediaInfoCommandGetEffectTracks.h"
#include "MediaInfoCommandGetEffectEntries.h"
#include "MediaInfoCommandGetEntryUsageCount.h"
#include "MediaInfoCommandGetAudioEntrySampleRate.h"
#include "MediaInfoCommandGetAudioEntryBitDepth.h"
#include "MediaInfoCommandGetVideoEntryFrameRate.h"
#include "MediaInfoCommandGetVideoEntryFrameSize.h"
#include "MediaInfoCommandGetAllEntryIDs.h"
#include "MediaInfoCommandGetEntryInfo.h"
#include "MediaCommandSetTrackMIDIChannel.h"
#include "MediaCommandSetActiveMIDITrack.h"
#include "MediaInfoCommandGetMIDISignalMeterValue.h"
#include "MediaInfoCommandGetMIDITrackIDs.h"
#include "MediaInfoCommandGetEffectTrackID.h"
#include "MediaCommandSetEffect.h"
#include "MediaCommandAssignVideoTransitionTrack.h"
#include "MediaCommandCreateVideoTransitionTrack.h"
#include "MediaCommandCreateVideoTransitionClip.h"
#include "MediaCommandDeleteVideoTransitionTrack.h"
#include "MediaCommandResizeClip.h"
#include "MediaCommandSetTransition.h"
#include "MediaInfoCommandGetNumberTransitions.h"
#include "MediaInfoCommandGetTransition.h"
#include "MediaInfoCommandGetTransitionIDs.h"
#include "MediaInfoCommandGetTransitionName.h"
#include "MediaCommandSetEffectDryWetMix.h"
#include "MediaCommandLoadEffectPreset.h"
#include "MediaInfoCommandGetEffectPresets.h"
#include "MediaInfoCommandGetNativeDirectXEffect.h"
#include "MediaInfoCommandGetAudioEncoders.h"
#include "MediaInfoCommandGetVideoAVIEncoders.h"
#include "MediaInfoCommandGetNumberAudioEncoders.h"
#include "MediaInfoCommandGetNumberVideoAVIEncoders.h"
#include "MediaInfoCommandGetEncoderName.h"
#include "MediaCommandSetAudioEncoder.h"
#include "MediaCommandSetVideoEncoder.h"
#include "MediaCommandDeleteEffect.h"
#include "MediaCommandSetRenderToDiskOutputFileName.h"
#include "MediaCommandDeleteMIDIEvents.h"
#include "MediaCommandMoveMIDIEvents.h"
#include "MediaInfoCommandGetMIDIEventsForClip.h"
#include "MediaCommandSetRenderingFlags.h"
#include "MediaCommandSetTransitionProperties.h"
#include "MediaInfoCommandGetTransitionProperties.h"
#include "MediaInfoCommandGetDefaultTransitionProperties.h"
#include "MediaInfoCommandGetEncoderProperties.h"
#include "MediaCommandSetEncoderProperty.h"
#include "MediaInfoCommandGetEncoderPropertyPage.h"
#include "MediaInfoCommandHasEncoderPropertyPage.h"
#include "MediaCommandCreateMIDIEvent.h"
#include "MediaInfoCommandGetMIDIEventsForTrack.h"
#include "MediaCommandShowEffectDialog.h"
#include "MediaCommandSetVideoWorkingSize.h"
#include "MediaCommandSetProjectBPM.h"
#include "MediaInfoCommandGetCurrentBitmap.h"
#include "MediaInfoCommandDeviceExists.h"
#include "MediaCommandSetAudioRenderParameters.h"
#include "MediaInfoCommandHasQuality.h"
#include "MediaCommandResizeMIDIEvent.h"
#include "MediaCommandSendMIDIEvent.h"
#include "EMMediaEngine.h"
#include "EMBeMediaUtility.h"   
#include "MediaCommandSetTrackVelocityOffset.h"
#include "MediaCommandSetTrackKeyOffset.h"
#include "MediaCommandResetMIDIDevices.h"
#include "MediaCommandDestructiveEdit.h"
#include "MediaCommandDestructiveReverse.h"
#include "MediaCommandDestructiveCrop.h"
#include "MediaCommandDestructiveSilence.h"
#include "MediaCommandDestructiveNormalize.h"
#include "MediaCommandDestructiveFadeIn.h"
#include "MediaCommandDestructiveFadeOut.h"
#include "MediaCommandRefreshPreview.h"
#include "MediaCommandAddNode.h"
#include "MediaCommandDeleteNode.h"
#include "MediaCommandMoveNode.h"
#include "MediaInfoCommandGetNodes.h"
#include "MediaInfoCommandGetVideoDimension.h"
#include "MediaInfoCommandGetInputSignalMeters.h"
#include "MediaCommandSetMagicSpeedValue.h"
#include "MediaInfoCommandGetMagicSpeedValue.h"
#include "MediaCommandSetEncoderFamily.h"
#include "MediaCommandQuantizeMIDIEvents.h"

#include "MediaInfoCommandGetCurrentProjectPosition.h"
#include "MediaInfoCommandGetCurrentAudioProcessingPosition.h"
#include "MediaInfoCommandGetCurrentVideoProcessingPosition.h"

extern "C" __declspec(dllexport) EMMediaCommandFactory* InstanceMediaCommandFactory() { return EMMediaEngine::Instance() -> GetCommandFactory(); };
extern "C" __declspec(dllexport) void DeleteMediaCommandFactory() { EMMediaCommandFactory::Delete(); };

EMMediaCommandFactory* EMMediaCommandFactory::m_opInstance = NULL;

EMMediaCommandFactory* EMMediaCommandFactory::Instance()
{
	if(m_opInstance == NULL)
		m_opInstance = EM_new EMMediaCommandFactory();
	return m_opInstance;	
	return NULL;
}

void EMMediaCommandFactory::Delete()
{
	delete m_opInstance;
	m_opInstance = NULL;
}

EMMediaCommandFactory::EMMediaCommandFactory()
{
}

EMMediaCommandFactory::~EMMediaCommandFactory()
{
}

EMCommand* EMMediaCommandFactory::CreateMediaCommand(uint32 p_vType)
{
	switch(p_vType)
	{
		case MEDIA_COMMAND_TOGGLE_PLAYBACK:
			return EM_new MediaCommandTogglePlayback();
		case MEDIA_COMMAND_START_PLAYBACK:
			return EM_new MediaCommandStartPlayback();
		case MEDIA_COMMAND_STOP_PLAYBACK:
			return EM_new MediaCommandStopPlayback();
		case MEDIA_COMMAND_SET_TRACK_PAN:
			return EM_new MediaCommandSetTrackPan();
		case MEDIA_COMMAND_SET_TRACK_MUTE_STATE:
			return EM_new MediaCommandSetTrackMuteState();
		case MEDIA_COMMAND_TOGGLE_TRACK_SOLO:
			return EM_new MediaCommandToggleTrackSolo();
		case MEDIA_COMMAND_TOGGLE_TRACK_ARM:
			return EM_new MediaCommandToggleTrackArm();
		case MEDIA_COMMAND_SET_TRACK_OUTPUT:
			return EM_new MediaCommandSetTrackOutput();
		case MEDIA_COMMAND_SET_TRACK_INPUT:
			return EM_new MediaCommandSetTrackInput();
		case MEDIA_COMMAND_SET_TRACK_NAME:
			return EM_new MediaCommandSetTrackName();
		case MEDIA_COMMAND_SET_TRACK_FADER:
			return EM_new MediaCommandSetTrackFader();
		case MEDIA_COMMAND_SET_CLIP_OUTPUT:
			return EM_new MediaCommandSetClipOutput();
		case MEDIA_COMMAND_SEEK:
			return EM_new MediaCommandSeek();
		case MEDIA_COMMAND_MOVE_CLIP:
			return EM_new MediaCommandMoveClip();
		case MEDIA_COMMAND_SET_CLIP_MARK_IN:
			return EM_new MediaCommandSetClipMarkIn();
		case MEDIA_COMMAND_SET_CLIP_MARK_OUT:
			return EM_new MediaCommandSetClipMarkOut();
		case MEDIA_COMMAND_CLONE_CLIP:
			return EM_new MediaCommandCloneClip();
		case MEDIA_COMMAND_IMPORT_MEDIA:
			return EM_new MediaCommandImportMedia();
		case MEDIA_COMMAND_DROP_MEDIA:
			return EM_new MediaCommandDropMedia();
		case MEDIA_COMMAND_CREATE_AUDIO_TRACK:
			return EM_new MediaCommandCreateAudioTrack();
		case MEDIA_COMMAND_CREATE_VIDEO_TRACK:
			return EM_new MediaCommandCreateVideoTrack();
		case MEDIA_COMMAND_DELETE_CLIP:
			return EM_new MediaCommandDeleteClip();
		case MEDIA_COMMAND_DELETE_TRACK:
			return EM_new MediaCommandDeleteTrack();
		case MEDIA_COMMAND_SET_CLIP_NAME:
			return EM_new MediaCommandSetClipName();
		case MEDIA_COMMAND_PLAY_BUFFER:
			return EM_new MediaCommandPlayBuffer();
		case MEDIA_COMMAND_MOVE_TRACK:
			return EM_new MediaCommandMoveTrack();
		case MEDIA_COMMAND_CREATE_EFFECT_TRACK:
			return EM_new MediaCommandCreateEffectTrack();
		case MEDIA_COMMAND_INSERT_EFFECT:
			return EM_new MediaCommandInsertEffect();
		case MEDIA_COMMAND_SET_EFFECT_TRACK_OUTPUT:
			return EM_new MediaCommandSetEffectTrackOutput();
		case MEDIA_COMMAND_DELETE_VIDEO_CLIP_CONTENT:
			return EM_new MediaCommandDeleteVideoClipContent();
		case MEDIA_COMMAND_CREATE_MIDI_TRACK:
			return EM_new MediaCommandCreateMIDITrack();
		case MEDIA_COMMAND_SET_TRACK_MIDI_CHANNEL:
			return EM_new MediaCommandSetTrackMIDIChannel();
		case MEDIA_COMMAND_SET_ACTIVE_MIDI_TRACK:
			return EM_new MediaCommandSetActiveMIDITrack();
		case MEDIA_COMMAND_SET_EFFECT:
			return EM_new MediaCommandSetEffect();
		case MEDIA_COMMAND_CREATE_VIDEO_TRANSITION_TRACK:
			return EM_new MediaCommandCreateVideoTransitionTrack();
		case MEDIA_COMMAND_ASSIGN_VIDEO_TRANSITION_TRACK:
			return EM_new MediaCommandAssignTransitionTrackToVideoTrack();
		case MEDIA_COMMAND_DELETE_VIDEO_TRANSITION_TRACK:
			return EM_new MediaCommandDeleteVideoTransitionTrack();
		case MEDIA_COMMAND_CREATE_VIDEO_TRANSITION_CLIP:
			return EM_new MediaCommandCreateVideoTransitionClip();
		case MEDIA_COMMAND_RESIZE_VIDEO_TRANSITION_CLIP:
			return EM_new MediaCommandResizeClip();
		case MEDIA_COMMAND_SET_VIDEO_TRANSITION:
			return EM_new MediaCommandSetTransition();
		case MEDIA_COMMAND_SET_EFFECT_DRY_WET_MIX:
			return EM_new MediaCommandSetEffectDryWetMix();
		case MEDIA_COMMAND_LOAD_EFFECT_PRESET:
			return EM_new MediaCommandLoadEffectPreset();
		case MEDIA_COMMAND_SET_AUDIO_ENCODER:
			return EM_new MediaCommandSetAudioEncoder();
		case MEDIA_COMMAND_SET_VIDEO_ENCODER:
			return EM_new MediaCommandSetVideoEncoder();
		case MEDIA_COMMAND_DELETE_EFFECT:
			return EM_new MediaCommandDeleteEffect();
		case MEDIA_COMMAND_SET_RENDER_TO_DISK_OUTPUT_FILENAME:
			return EM_new MediaCommandSetRenderToDiskOutputFileName();
		case MEDIA_COMMAND_DELETE_MIDI_EVENTS:
			return EM_new MediaCommandDeleteMIDIEvents();
		case MEDIA_COMMAND_MOVE_MIDI_EVENTS:
			return EM_new MediaCommandMoveMIDIEvents();
		case  MEDIA_COMMAND_SET_RENDERING_FLAG:
			return EM_new MediaCommandSetRenderingFlags();
		case MEDIA_COMMAND_SET_PERSIST_TRANSITION_SETTINGS:
			return EM_new MediaCommandSetTransitionProperties();
		case MEDIA_COMMAND_SET_ENCODER_PROPERTY:
			return EM_new MediaCommandSetEncoderProperty();
		case MEDIA_COMMAND_CREATE_MIDI_EVENT:
			return EM_new MediaCommandCreateMIDIEvent();
		case MEDIA_COMMAND_SHOW_EFFECT_DIALOG:
			return EM_new MediaCommandShowEffectDialog();
		case MEDIA_COMMAND_SET_VIDEO_WORKING_SIZE:
			return EM_new MediaCommandSetVideoWorkingSize();
		case MEDIA_COMMAND_SET_PROJECT_BPM:
			return EM_new MediaCommandSetProjectBPM(); 
		case MEDIA_COMMAND_SET_AUDIO_RENDER_PARAMETERS:
			return EM_new MediaCommandSetAudioRenderParameters();
		case MEDIA_COMMAND_RESIZE_MIDI_EVENT:
			return EM_new MediaCommandResizeMIDIEvent();
		case MEDIA_COMMAND_SEND_MIDI_EVENT:
			return EM_new MediaCommandSendMIDIEvent();
		case MEDIA_COMMAND_SET_TRACK_VELOCITY_OFFSET:
			return EM_new MediaCommandSetTrackVelocityOffset();
		case MEDIA_COMMAND_SET_TRACK_KEY_OFFSET:
			return EM_new MediaCommandSetTrackKeyOffset();
		case MEDIA_COMMAND_RESET_MIDI_DEVICES:
			return EM_new MediaCommandResetMIDIDevices();
		case MEDIA_COMMAND_DESTRUCTIVE_EDIT:
			return EM_new MediaCommandDestructiveEdit();
		case MEDIA_COMMAND_DESTRUCTIVE_REVERSE:
			return EM_new MediaCommandDestructiveReverse();
		case MEDIA_COMMAND_DESTRUCTIVE_CROP:
			return EM_new MediaCommandDestructiveCrop();
		case MEDIA_COMMAND_DESTRUCTIVE_SILENCE:
			return EM_new MediaCommandDestructiveSilence();
		case MEDIA_COMMAND_DESTRUCTIVE_NORMALIZE:
			return EM_new MediaCommandDestructiveNormalize();
		case MEDIA_COMMAND_DESTRUCTIVE_FADE_IN:
			return EM_new MediaCommandDestructiveFadeIn();
		case MEDIA_COMMAND_DESTRUCTIVE_FADE_OUT:
			return EM_new MediaCommandDestructiveFadeOut();		
		case MEDIA_COMMAND_REFRESH_PREVIEW:
			return EM_new MediaCommandRefreshPreview();
		case MEDIA_COMMAND_ADD_NODE:
			return EM_new MediaCommandAddNode();
		case MEDIA_COMMAND_DELETE_NODE:
			return EM_new MediaCommandDeleteNode();
		case MEDIA_COMMAND_MOVE_NODE:
			return EM_new MediaCommandMoveNode();
		case MEDIA_COMMAND_SET_MAGIC_SPEED_VALUE:
			return EM_new MediaCommandSetMagicSpeedValue();
		case MEDIA_COMMAND_SET_ENCODER_FAMILY:
			return EM_new MediaCommandSetEncoderFamily();
		case MEDIA_COMMAND_QUANTIZE_MIDI_EVENTS:
			return EM_new MediaCommandQuantizeMIDIEvents();

		case MEDIA_INFO_COMMAND_GET_ENTRY_DURATION:
			return EM_new MediaInfoCommandGetEntryDuration();
		case MEDIA_INFO_COMMAND_GET_CLIP_DURATION:
			return EM_new MediaInfoCommandGetClipDuration();
		case MEDIA_INFO_COMMAND_GET_ENTRY_FILENAME:
			return EM_new MediaInfoCommandGetEntryFileName();
		case MEDIA_INFO_COMMAND_GET_OUTPUTS:
			return EM_new MediaInfoCommandGetOutputs();
		case MEDIA_INFO_COMMAND_GET_INPUTS:
			return EM_new MediaInfoCommandGetInputs();
		case MEDIA_INFO_COMMAND_GET_OUTPUT_NAME:
			return EM_new MediaInfoCommandGetOutputName();
		case MEDIA_INFO_COMMAND_GET_INPUT_NAME:
			return EM_new MediaInfoCommandGetInputName();
		case MEDIA_INFO_COMMAND_GET_AUDIO_TRACK_IDS:
			return EM_new MediaInfoCommandGetAudioTrackIDs();
		case MEDIA_INFO_COMMAND_GET_SIGNAL_METER_VALUE:
			return EM_new MediaInfoCommandGetSignalMeterValue();
		case MEDIA_COMMAND_PREPARE_RECORDING:
			return EM_new MediaCommandPrepareRecording();
		case MEDIA_INFO_COMMAND_GET_AUDIO_CLIP_HIGHRES_CONTENT:
			return EM_new MediaInfoCommandGetAudioClipContent();
		case MEDIA_INFO_COMMAND_GET_AUDIO_CLIP_LOWRES_CONTENT:
			return EM_new MediaInfoCommandGetAudioClipLowResContent();
		case MEDIA_INFO_COMMAND_GET_CLIP_NUM_CHANNELS:
			return EM_new MediaInfoCommandGetClipNumChannels();
		case MEDIA_INFO_COMMAND_GET_CLIP_MARK_IN:
			return EM_new MediaInfoCommandGetClipMarkIn();
		case MEDIA_INFO_COMMAND_GET_CLIP_MARK_OUT:
			return EM_new MediaInfoCommandGetClipMarkOut();
		case MEDIA_INFO_COMMAND_GET_CLIP_ENTRY_ID:
			return EM_new MediaInfoCommandGetEntryID();
		case MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_NUM_CHANNELS:
			return EM_new MediaInfoCommandGetAudioEntryNumChannels();
		case MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_NUM_SAMPLES:
			return EM_new MediaInfoCommandGetAudioEntryNumSamples();
		case MEDIA_INFO_COMMAND_GET_CLIP_NAME:
			return EM_new MediaInfoCommandGetClipName();
		case MEDIA_INFO_COMMAND_GET_CLIP_FILENAME:
			return EM_new MediaInfoCommandGetClipFileName();
		case MEDIA_INFO_COMMAND_GET_CLIP_POSITION:
			return EM_new MediaInfoCommandGetClipPosition();
		case MEDIA_INFO_COMMAND_GET_VIDEO_CLIP_CONTENT:
			return EM_new MediaInfoCommandGetVideoClipContent();
		case MEDIA_INFO_COMMAND_GET_VIDEO_ASPECT_RATIO:
			return EM_new MediaInfoCommandGetVideoAspectRatio();
		case MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_NUM_FRAMES:
			return EM_new MediaInfoCommandGetVideoEntryNumFrames();
		case MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_FORMAT_STRING:
			return EM_new MediaInfoCommandGetAudioEntryFormatString();
		case MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_FORMAT_STRING:
			return EM_new MediaInfoCommandGetVideoEntryFormatString();
		case MEDIA_INFO_COMMAND_IS_PLAYING:
			return EM_new MediaInfoCommandIsPlaying();
		case MEDIA_INFO_COMMAND_GET_AUDIO_RENDER_OUTPUT:
			return EM_new MediaInfoCommandGetAudioRenderOutput();
		case MEDIA_INFO_COMMAND_GET_VIDEO_RENDER_OUTPUT:
			return EM_new MediaInfoCommandGetVideoRenderOutput();
		case MEDIA_INFO_COMMAND_GET_TRACK_INFO:
			return EM_new MediaInfoCommandGetTrackInfo();
		case MEDIA_INFO_COMMAND_GET_CLIP_INFO:
			return EM_new MediaInfoCommandGetClipInfo();
		case MEDIA_INFO_COMMAND_GET_ALL_TRACK_IDS:
			return EM_new MediaInfoCommandGetAllTrackIDs();
		case MEDIA_INFO_COMMAND_GET_CODECS:
			return EM_new MediaInfoCommandGetCodecs();
		case MEDIA_INFO_COMMAND_GET_EFFECT_ENTRY_INFO:
			return EM_new MediaInfoCommandGetEffectEntryInfo();
		case MEDIA_INFO_COMMAND_GET_EFFECT_INFO:
			return EM_new MediaInfoCommandGetEffectInfo();
		case MEDIA_INFO_COMMAND_GET_EFFECT_TRACK_INFO:
			return EM_new MediaInfoCommandGetEffectTrackInfo();
		case MEDIA_INFO_COMMAND_GET_EFFECT_TRACKS:
			return EM_new MediaInfoCommandGetEffectTracks();
		case MEDIA_INFO_COMMAND_GET_EFFECT_ENTRIES:
			return EM_new MediaInfoCommandGetEffectEntries();
		case MEDIA_INFO_COMMAND_GET_ENTRY_USAGE_COUNT:
			return EM_new MediaInfoCommandGetEntryUsageCount();
		case MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_SAMPLE_RATE:
			return EM_new MediaInfoCommandGetAudioEntrySampleRate();
		case MEDIA_INFO_COMMAND_GET_AUDIO_ENTRY_BIT_DEPTH:
			return EM_new MediaInfoCommandGetAudioEntryBitDepth();
		case MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_FRAME_RATE:
			return EM_new MediaInfoCommandGetVideoEntryFrameRate();
		case MEDIA_INFO_COMMAND_GET_VIDEO_ENTRY_FRAME_SIZE:
			return EM_new MediaInfoCommandGetVideoEntryFrameSize();
		case MEDIA_INFO_COMMAND_GET_ALL_ENTRY_IDS:
			return EM_new MediaInfoCommandGetAllEntryIDs();
		case MEDIA_INFO_COMMAND_GET_ENTRY_INFO:
			return EM_new MediaInfoCommandGetEntryInfo();
		case MEDIA_INFO_COMMAND_GET_MIDI_SIGNAL_METER_VALUE:
			return EM_new MediaInfoCommandGetMIDISignalMeterValue();
		case MEDIA_INFO_COMMAND_GET_MIDI_TRACK_IDS:
			return EM_new MediaInfoCommandGetMIDITrackIDs();
		case MEDIA_INFO_COMMAND_GET_EFFECT_TRACK_ID:
			return EM_new MediaInfoCommandGetEffectTrackID();
		case MEDIA_INFO_COMMAND_GET_VIDEO_TRANSITION:
			return EM_new MediaInfoCommandGetTransition();
		case  MEDIA_INFO_COMMAND_GET_NUMBER_OF_VIDEO_TRANSITIONS:
			return EM_new MediaInfoCommandGetNumberOfTransitions();
		case MEDIA_INFO_COMMAND_GET_VIDEO_TRANSITION_IDS:
			return EM_new MediaInfoCommandGetTransitionIDs();
		case MEDIA_INFO_COMMAND_GET_VIDEO_TRANSITION_NAME:
			return EM_new MediaInfoCommandGetTransitionName();
		case MEDIA_INFO_COMMAND_GET_EFFECT_PRESETS:
			return EM_new MediaInfoCommandGetEffectPresets();
		case MEDIA_INFO_COMMAND_GET_NATIVE_DIRECTX_EFFECT:
			return EM_new MediaInfoCommandGetNativeDirectXEffect();
		case MEDIA_INFO_COMMAND_GET_AUDIO_ENCODERS:
			return EM_new MediaInfoCommandGetAudioEncoders();
		case MEDIA_INFO_COMMAND_GET_VIDEO_AVI_ENCODERS:
			return EM_new MediaInfoCommandGetVideoAVIEncoders();
		case MEDIA_INFO_COMMAND_GET_NUMBER_AUDIO_ENCODERS:
			return EM_new MediaInfoCommandGetNumberAudioEncoders();
		case MEDIA_INFO_COMMAND_GET_NUMBER_VIDEO_AVI_ENCODERS:
			return EM_new MediaInfoCommandGetNumberVideoAVIEncoders();
		case MEDIA_INFO_COMMAND_GET_ENCODER_NAME:
			return EM_new MediaInfoCommandGetEncoderName();
		case MEDIA_INFO_COMMAND_GET_MIDI_EVENTS_FOR_CLIP:
			return EM_new MediaInfoCommandGetMIDIEventsForClip();
		case MEDIA_INFO_COMMAND_GET_PERSIST_TRANSITION_SETTINGS:
			return EM_new MediaInfoCommandGetTransitionProperties();
		case MEDIA_INFO_COMMAND_GET_DEFAULT_TRANSITION_SETTINGS:
			return EM_new MediaInfoCommandGetDefaultTransitionProperties();
		case MEDIA_INFO_COMMAND_GET_ENCODER_PROPERTIES:
			return EM_new MediaInfoCommandGetEncoderProperties();
		case MEDIA_INFO_COMMAND_GET_ENCODER_PROPERTY_PAGE:
			return EM_new MediaInfoCommandGetEncoderPropertyPage();
		case MEDIA_INFO_COMMAND_HAS_ENCODER_PROPERTY_PAGE:
			return EM_new MediaInfoCommandHasEncoderPropertyPage();
		case MEDIA_INFO_COMMAND_GET_MIDI_EVENTS_FOR_TRACK:
			return EM_new MediaInfoCommandGetMIDIEventsForTrack();
		case MEDIA_INFO_COMMAND_GET_CURRENT_BITMAP:
			return EM_new MediaInfoCommandGetCurrentBitmap();
		case MEDIA_INFO_COMMAND_DEVICE_EXISTS:
			return EM_new MediaInfoCommandDeviceExists();
		case MEDIA_INFO_COMMAND_HAS_QUALITY:
			return EM_new MediaInfoCommandHasQuality();
		case MEDIA_INFO_COMMAND_GET_VIDEO_DIMENSION:
			return EM_new MediaInfoCommandGetVideoDimension();
		case MEDIA_INFO_COMMAND_GET_NODES:
			return EM_new MediaInfoCommandGetNodes();
		case MEDIA_INFO_COMMAND_GET_INPUT_SIGNAL_METERS:
			return EM_new MediaInfoCommandGetInputSignalMeters();
		case MEDIA_INFO_COMMAND_GET_MAGIC_SPEED_VALUE:
			return EM_new MediaInfoCommandGetMagicSpeedValue();
		case MEDIA_INFO_COMMAND_GET_CURRENT_PROJECT_POSITION:
			return EM_new MediaInfoCommandGetCurrentProjectPosition();
		case MEDIA_INFO_COMMAND_GET_CURRENT_AUDIO_PROCESSING_POSITION:
			return EM_new MediaInfoCommandGetCurrentAudioProcessingPosition();
		case MEDIA_INFO_COMMAND_GET_CURRENT_VIDEO_PROCESSING_POSITION:
			return EM_new MediaInfoCommandGetCurrentVideoProcessingPosition();

		default:
			eo << "ERROR! MediaCommandFactory failed to find command " << (uint32) p_vType << ef;
			EMDebugger("ERROR! EMMediaCommandFactory could not find that media command!");
	};
	
	return NULL;
}
