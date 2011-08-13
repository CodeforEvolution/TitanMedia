# Microsoft Developer Studio Project File - Name="EMMediaEngine_Library" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=EMMEDIAENGINE_LIBRARY - WIN32 DEBUG
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EMMediaEngine_Library.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EMMediaEngine_Library.mak" CFG="EMMEDIAENGINE_LIBRARY - WIN32 DEBUG"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EMMediaEngine_Library - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EMMediaEngine_Library - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EMMediaEngine_Library - Win32 Trial Version Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Titan_r1", BAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EMMediaEngine_Library - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "EMMediaEngine_Library_files_RELEASE"
# PROP Intermediate_Dir "EMMediaEngine_Library_files_RELEASE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMMEDIAENGINE_LIBRARY_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /Ob2 /D "_WIN32_DCOM" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMMEDIAENGINE_LIBRARY_EXPORTS" /D "__EM_MEDIA_ENGINE_LIBRARY_IS_BUILDING" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "NDEBUG"
# ADD RSC /l 0x41d /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 qedit.lib gdiplus.lib qtmlClient.lib Vfw32.lib dsound.lib MAPI32.LIB SHELL32.LIB .\lib\EMWinDirectSoundOutput.lib .\lib\EMWinAudioConverter.lib .\lib\EMWinDXPluginEngine.lib .\lib\EMCommon_Library.lib .\lib\EMSettings_Library.lib .\lib\EMCommand_Library.lib .\lib\EMDebug_Library.lib .\lib\EMProjectManager_Library.lib .\lib\EMWinMediaInputFilter.lib .\lib\EMWinVideoShuffleOutputFilter.lib dxguid.lib ddraw.lib strmiids.lib strmbase.lib ksuser.lib advapi32.lib OLE32.LIB kernel32.lib USER32.LIB GDI32.LIB OLEAUT32.LIB WINMM.LIB /nologo /dll /pdb:none /map /machine:I386 /nodefaultlib:"nafxcwd.lib" /out:"\SourceSafe\ReleaseBuild\EMMediaEngine_Library.dll" /implib:"\SourceSafe\ReleaseBuild\lib\EMMediaEngine_Library.lib"

!ELSEIF  "$(CFG)" == "EMMediaEngine_Library - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "EMMediaEngine_Library_files"
# PROP Intermediate_Dir "EMMediaEngine_Library_files"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMMEDIAENGINE_LIBRARY_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /D "_WIN32_DCOM" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_USRDLL" /D "__EM_MEDIA_ENGINE_LIBRARY_IS_BUILDING" /D "DEBUG_VERSION" /FR /YX /FD /LDd /GD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 gdiplus.lib qtmlClient.lib Vfw32.lib .\lib\EMWinDirectSoundOutput.lib dsound.lib dxguid.lib MAPI32.LIB SHELL32.LIB .\lib\EMWinAudioConverter.lib .\lib\EMWinDXPluginEngine.lib .\lib\EMCommon_Library.lib .\lib\EMSettings_Library.lib .\lib\EMCommand_Library.lib .\lib\EMDebug_Library.lib .\lib\EMProjectManager_Library.lib .\lib\EMWinMediaInputFilter.lib .\lib\EMWinVideoShuffleOutputFilter.lib dxguid.lib ddraw.lib strmiids.lib strmbasd.lib ksuser.lib advapi32.lib OLE32.LIB kernel32.lib USER32.LIB GDI32.LIB OLEAUT32.LIB WINMM.LIB /nologo /dll /incremental:no /debug /machine:I386 /nodefaultlib:"nafxcwd.lib" /out:"EMMediaEngine_Library.dll  " /implib:"lib\EMMediaEngine_Library.lib" /subsystem:native
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Cmds=EMVersionNumbering.exe \SourceSafe\Titan_r1\lib\ EMMediaEngine_LibraryVersion Richard 1
# End Special Build Tool

!ELSEIF  "$(CFG)" == "EMMediaEngine_Library - Win32 Trial Version Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "EMMediaEngine_Library___Win32_Trial_Version_Release"
# PROP BASE Intermediate_Dir "EMMediaEngine_Library___Win32_Trial_Version_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "EMMediaEngine_Library_files_RELEASE"
# PROP Intermediate_Dir "EMMediaEngine_Library_files_RELEASE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /Ob2 /D "_WIN32_DCOM" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMMEDIAENGINE_LIBRARY_EXPORTS" /D "__EM_MEDIA_ENGINE_LIBRARY_IS_BUILDING" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /Ob2 /D "_WIN32_DCOM" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMMEDIAENGINE_LIBRARY_EXPORTS" /D "__EM_MEDIA_ENGINE_LIBRARY_IS_BUILDING" /D "TRIAL_VERSION" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "NDEBUG"
# ADD RSC /l 0x41d /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 MAPI32.LIB SHELL32.LIB .\lib\EMWinAudioConverter.lib .\lib\EMWinDXAudioOutput.lib .\lib\EMWinDXPluginEngine.lib .\lib\EMCommon_Library.lib .\lib\EMSettings_Library.lib .\lib\EMCommand_Library.lib .\lib\EMDebug_Library.lib .\lib\EMProjectManager_Library.lib .\lib\EMWinMediaInputFilter.lib .\lib\EMWinMediaOutputFilter.lib .\lib\EMWinVideoShuffleOutputFilter.lib dxguid.lib ddraw.lib strmiids.lib strmbase.lib ksuser.lib advapi32.lib OLE32.LIB kernel32.lib USER32.LIB GDI32.LIB OLEAUT32.LIB WINMM.LIB /nologo /dll /pdb:none /machine:I386 /nodefaultlib:"nafxcwd.lib" /out:"EMMediaEngine_Library.dll" /implib:".\lib\EMMediaEngine_Library.lib"
# ADD LINK32 gdiplus.lib qtmlClient.lib vfw32.lib MAPI32.LIB SHELL32.LIB .\lib\EMWinDirectSoundOutput.lib .\lib\EMWinAudioConverter.lib .\lib\EMWinDXPluginEngine.lib .\lib\EMCommon_Library.lib .\lib\EMSettings_Library.lib .\lib\EMCommand_Library.lib .\lib\EMDebug_Library.lib .\lib\EMProjectManager_Library.lib .\lib\EMWinMediaInputFilter.lib .\lib\EMWinVideoShuffleOutputFilter.lib dxguid.lib ddraw.lib strmiids.lib strmbase.lib ksuser.lib advapi32.lib OLE32.LIB kernel32.lib USER32.LIB GDI32.LIB OLEAUT32.LIB WINMM.LIB /nologo /dll /pdb:none /machine:I386 /nodefaultlib:"nafxcwd.lib" /out:"\SourceSafe\ReleaseBuild\EMMediaEngine_Library.dll" /implib:"\SourceSafe\ReleaseBuild\lib\EMMediaEngine_Library.lib"

!ENDIF 

# Begin Target

# Name "EMMediaEngine_Library - Win32 Release"
# Name "EMMediaEngine_Library - Win32 Debug"
# Name "EMMediaEngine_Library - Win32 Trial Version Release"
# Begin Group "Components"

# PROP Default_Filter ""
# Begin Group "Components / Media"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Components\Media\EMAudioClip.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMAudioClip.h
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMAudioExtractor.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMAudioExtractor.h
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMBeMixFunctions.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMBeMixFunctions.h
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMMediaClip.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMMediaClip.h
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMMediaCommandFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMMediaCommandFactory.h
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMMediaDebugLog.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMMediaDebugLog.h
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMMediaEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMMediaEngine.h
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMMediaFileManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMMediaFileManager.h
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMMediaIDManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMMediaIDManager.h
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMMediaMetronome.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMMediaMetronome.h
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMMediaMIDITrack.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMMediaMIDITrack.h
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMMediaTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMMediaTimer.h
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMMediaTrack.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMMediaTrack.h
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMMediaVideoTransitionClip.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMMediaVideoTransitionClip.h
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMMediaVideoTransitionTrack.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMMediaVideoTransitionTrack.h
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMMIDIClip.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMMIDIClip.h
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMVideoClip.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\Media\EMVideoClip.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinMediaTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinMediaTimer.h
# End Source File
# End Group
# End Group
# Begin Group "Application"

# PROP Default_Filter ""
# Begin Group "Application / Commands"

# PROP Default_Filter ""
# Begin Group "Actions"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandAddNode.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandAddNode.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandAssignVideoTransitionTrack.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandAssignVideoTransitionTrack.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandCloneClip.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandCloneClip.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandCreateAudioTrack.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandCreateAudioTrack.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandCreateEffectTrack.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandCreateEffectTrack.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandCreateMIDIEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandCreateMIDIEvent.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandCreateMIDITrack.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandCreateMIDITrack.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandCreateTrack.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandCreateTrack.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandCreateVideoTrack.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandCreateVideoTrack.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandCreateVideoTransitionClip.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandCreateVideoTransitionClip.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandCreateVideoTransitionTrack.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandCreateVideoTransitionTrack.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDeleteClip.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDeleteClip.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDeleteEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDeleteEffect.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDeleteMIDIEvents.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDeleteMIDIEvents.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDeleteNode.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDeleteNode.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDeleteTrack.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDeleteTrack.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDeleteVideoClipContent.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDeleteVideoClipContent.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDeleteVideoTransitionTrack.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDeleteVideoTransitionTrack.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDestructiveBase.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDestructiveBase.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDestructiveCrop.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDestructiveCrop.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDestructiveEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDestructiveEdit.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDestructiveFadeIn.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDestructiveFadeIn.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDestructiveFadeOut.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDestructiveFadeOut.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDestructiveNormalize.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDestructiveNormalize.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDestructiveReverse.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDestructiveReverse.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDestructiveSilence.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDestructiveSilence.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDropMedia.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandDropMedia.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandImportMedia.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandImportMedia.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandInsertEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandInsertEffect.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandLoadEffectPreset.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandLoadEffectPreset.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandMoveClip.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandMoveClip.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandMoveMIDIEvents.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandMoveMIDIEvents.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandMoveNode.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandMoveNode.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandMoveTrack.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandMoveTrack.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandPlayBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandPlayBuffer.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandPrepareRecording.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandPrepareRecording.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandQuantizeMIDIEvents.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandQuantizeMIDIEvents.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandRefreshPreview.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandRefreshPreview.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandResetMIDIDevices.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandResetMIDIDevices.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandResizeClip.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandResizeClip.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandResizeMIDIEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandResizeMIDIEvent.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSeek.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSeek.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSendMIDIEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSendMIDIEvent.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetActiveMIDITrack.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetActiveMIDITrack.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetAudioEncoder.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetAudioEncoder.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetAudioRenderParameters.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetAudioRenderParameters.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetClipMarkIn.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetClipMarkIn.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetClipMarkInUndoable.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetClipMarkInUndoable.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetClipMarkOut.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetClipMarkOut.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetClipMarkOutUndoable.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetClipMarkOutUndoable.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetClipName.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetClipName.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetClipOutput.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetClipOutput.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetEffect.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetEffectDryWetMix.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetEffectDryWetMix.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetEffectTrackOutput.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetEffectTrackOutput.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetEncoderFamily.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetEncoderFamily.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetEncoderProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetEncoderProperty.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetMagicSpeedValue.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetMagicSpeedValue.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetProjectBPM.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetProjectBPM.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetRenderingFlags.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetRenderingFlags.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetRenderToDiskOutputFileName.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetRenderToDiskOutputFileName.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetTrackFader.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetTrackFader.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetTrackInput.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetTrackInput.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetTrackKeyOffset.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetTrackKeyOffset.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetTrackMIDIChannel.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetTrackMIDIChannel.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetTrackMuteState.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetTrackMuteState.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetTrackName.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetTrackName.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetTrackOutput.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetTrackOutput.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetTrackPan.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetTrackPan.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetTrackVelocityOffset.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetTrackVelocityOffset.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetTransition.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetTransition.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetTransitionProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetTransitionProperties.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetVideoEncoder.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetVideoEncoder.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetVideoWorkingSize.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandSetVideoWorkingSize.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandShowEffectDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandShowEffectDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandStartPlayback.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandStartPlayback.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandStopPlayback.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandStopPlayback.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandTogglePlayback.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandTogglePlayback.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandToggleTrackArm.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandToggleTrackArm.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandToggleTrackSolo.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaCommandToggleTrackSolo.h
# End Source File
# End Group
# Begin Group "Info commands"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandDeviceExists.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandDeviceExists.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetAllEntryIDs.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetAllEntryIDs.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetAllTrackIDs.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetAllTrackIDs.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetAudioClipContent.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetAudioClipContent.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetAudioClipLowResContent.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetAudioClipLowResContent.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetAudioEncoders.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetAudioEncoders.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetAudioEntryBitDepth.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetAudioEntryBitDepth.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetAudioEntryFormatString.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetAudioEntryFormatString.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetAudioEntryNumChannels.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetAudioEntryNumChannels.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetAudioEntryNumSamples.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetAudioEntryNumSamples.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetAudioEntrySampleRate.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetAudioEntrySampleRate.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetAudioRenderOutput.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetAudioRenderOutput.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetAudioTrackIDs.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetAudioTrackIDs.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetClipDuration.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetClipDuration.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetClipFileName.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetClipFileName.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetClipInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetClipInfo.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetClipMarkIn.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetClipMarkIn.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetClipMarkOut.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetClipMarkOut.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetClipName.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetClipName.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetClipNumChannels.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetClipNumChannels.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetClipPosition.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetClipPosition.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetCodecs.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetCodecs.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetCurrentAudioProcessingPosition.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetCurrentAudioProcessingPosition.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetCurrentBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetCurrentBitmap.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetCurrentProjectPosition.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetCurrentProjectPosition.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetCurrentVideoProcessingPosition.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetCurrentVideoProcessingPosition.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetDefaultTransitionProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetDefaultTransitionProperties.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEffectEntries.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEffectEntries.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEffectEntryInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEffectEntryInfo.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEffectInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEffectInfo.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEffectPresets.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEffectPresets.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEffectTrackID.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEffectTrackID.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEffectTrackInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEffectTrackInfo.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEffectTracks.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEffectTracks.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEncoderName.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEncoderName.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEncoderProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEncoderProperties.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEncoderPropertyPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEncoderPropertyPage.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEntryDuration.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEntryDuration.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEntryFileName.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEntryFileName.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEntryID.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEntryID.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEntryInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEntryInfo.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEntryUsageCount.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetEntryUsageCount.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetInputName.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetInputName.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetInputs.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetInputs.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetInputSignalMeters.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetInputSignalMeters.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetMagicSpeedValue.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetMagicSpeedValue.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetMIDIEventsForClip.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetMIDIEventsForClip.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetMIDIEventsForTrack.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetMIDIEventsForTrack.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetMIDISignalMeterValue.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetMIDISignalMeterValue.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetMIDITrackIDs.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetMIDITrackIDs.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetNativeDirectXEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetNativeDirectXEffect.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetNodes.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetNodes.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetNumberAudioEncoders.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetNumberAudioEncoders.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetNumberTransitions.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetNumberTransitions.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetNumberVideoAVIEncoders.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetNumberVideoAVIEncoders.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetOutputName.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetOutputName.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetOutputs.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetOutputs.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetSignalMeterValue.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetSignalMeterValue.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetTrackInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetTrackInfo.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetTransition.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetTransition.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetTransitionIDs.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetTransitionIDs.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetTransitionName.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetTransitionName.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetTransitionProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetTransitionProperties.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetVideoAspectRatio.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetVideoAspectRatio.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetVideoAVIEncoders.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetVideoAVIEncoders.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetVideoClipContent.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetVideoClipContent.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetVideoDimension.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetVideoDimension.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetVideoEntryFormatString.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetVideoEntryFormatString.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetVideoEntryFrameRate.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetVideoEntryFrameRate.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetVideoEntryFrameSize.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetVideoEntryFrameSize.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetVideoEntryNumFrames.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetVideoEntryNumFrames.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetVideoRenderOutput.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandGetVideoRenderOutput.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandHasEncoderPropertyPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandHasEncoderPropertyPage.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandHasQuality.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandHasQuality.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandIsPlaying.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\MediaInfoCommandIsPlaying.h
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\Application\DirectoryIDs.h
# End Source File
# Begin Source File

SOURCE=.\Application\Messages.h
# End Source File
# Begin Source File

SOURCE=.\Application\ResourceIDs.h
# End Source File
# Begin Source File

SOURCE=.\Application\SettingIDs.h
# End Source File
# End Group
# Begin Group "Framework"

# PROP Default_Filter ""
# Begin Group "Framework / Media"

# PROP Default_Filter ""
# Begin Group "Framework / Media / Audio"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMAudioClipMixer.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMAudioClipMixer.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMAudioConverter.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMAudioConverter.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMAudioFileInputDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMAudioFileInputDescriptor.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMAudioMixFunctions.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMAudioMixFunctions.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMAudioSignalMeter.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMAudioSignalMeter.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMBeAudioClipRepository.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMBeAudioClipRepository.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMBeAudioConsumerNode.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMBeAudioConsumerNode.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMBeAudioFileInputDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMBeAudioFileInputDescriptor.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMBeAudioOutputNode.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMBeAudioOutputNode.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMBeRealtimeAudioInputDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMBeRealtimeAudioInputDescriptor.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMBeRealtimeAudioOutputDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMBeRealtimeAudioOutputDescriptor.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMDestructiveCrop.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMDestructiveCrop.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMDestructiveDXWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMDestructiveDXWrapper.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMDestructiveFadeIn.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMDestructiveFadeIn.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMDestructiveFadeOut.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMDestructiveFadeOut.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMDestructiveNormalize.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMDestructiveNormalize.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMDestructiveReverse.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMDestructiveReverse.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMDestructiveSilence.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMDestructiveSilence.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMEqualizerInterface.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMEqualizerPlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMEqualizerPlugin.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMMediaPoolAudioEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMMediaPoolAudioEntry.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMSilentAudioSource.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMSilentAudioSource.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMWaveFileReader.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMWaveFileReader.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMWaveFileWriter.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMWaveFileWriter.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMWinAudioClipRepository.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMWinAudioClipRepository.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMWinAudioDiskWriter.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMWinAudioDiskWriter.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMWinDirectXPlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMWinDirectXPlugin.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMWinMemoryMappedWaveReader.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMWinMemoryMappedWaveReader.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMWinMemoryMappedWaveWriter.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMWinMemoryMappedWaveWriter.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMWinRealtimeAudioEncoderDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMWinRealtimeAudioEncoderDescriptor.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMWinRealtimeAudioInputDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMWinRealtimeAudioInputDescriptor.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMWinRealtimeAudioOutputDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMWinRealtimeAudioOutputDescriptor.h
# End Source File
# End Group
# Begin Group "Framework / Media / Video"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Framework\Media\Video\BlockFIFO.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\BlockFIFO.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMBarnTransition.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMBarnTransition.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMBrightnessFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMBrightnessFilter.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMContrastFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMContrastFilter.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMHSVFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMHSVFilter.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMMediaPoolVideoEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMMediaPoolVideoEntry.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMMediaTransitionTrackRepository.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMMediaTransitionTrackRepository.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMNegativeFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMNegativeFilter.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMNoTransition.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMNoTransition.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMRGBFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMRGBFilter.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMScalingUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMSilentVideoSource.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMSilentVideoSource.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMSimpleVideoEffectDestination.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMSimpleVideoEffectDestination.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMSimpleVideoEffectPlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMSimpleVideoEffectPlugin.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMVideoClipMixer.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMVideoClipMixer.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMVideoEffectFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMVideoEffectFilter.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMVideoFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMVideoFile.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMVideoFileInputDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMVideoFileInputDescriptor.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMVideoMixFunctions.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMVideoMixFunctions.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMVideoSynchronizer.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMVideoSynchronizer.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMVideoTransition.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMVideoTransition.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinChromaVideoTransition.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinChromaVideoTransition.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinDirectXVideoTransition.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinDirectXVideoTransition.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinDXVideoTransition.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinDXVideoTransition.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinGDIPlusImageLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinGDIPlusImageLoader.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinGDIPlusImageLoaderSuper.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinGDIPlusImageLoaderSuper.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinMediaSingletonMuxer.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinMediaSingletonMuxer.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinQuicktimeVideo.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinQuicktimeVideo.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinQuicktimeVideoSuper.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinQuicktimeVideoSuper.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinQuickTimeWriter.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinQuickTimeWriter.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinQuickTimeWriterSuper.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinQuickTimeWriterSuper.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinRealtimeRenderToDiskOutputDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinRealtimeRenderToDiskOutputDescriptor.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinRealtimeVideoEncoderDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinRealtimeVideoEncoderDescriptor.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinRealtimeVideoOutputDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinRealtimeVideoOutputDescriptor.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinSMPTEVideoTransition.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinSMPTEVideoTransition.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinSMPTEVideoTransitionNames.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinVideoClipRepository.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinVideoClipRepository.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinVideoFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinVideoFile.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinVideoTransitionClipRepository.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinVideoTransitionClipRepository.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWipeTransition.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWipeTransition.h
# End Source File
# End Group
# Begin Group "Framework / Media / MIDI"

# PROP Default_Filter "*.cpp;*.h"
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMMediaMIDIEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMMediaMIDIEvent.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMMediaMIDIEventList.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMMediaMIDIEventList.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMMediaMIDIEventListener.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMMediaMIDIEventListener.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMMediaMIDIEventPosition.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMMediaMIDIEventPosition.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMMediaMIDIEventRepository.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMMediaMIDIEventRepository.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMMediaMIDISignalMeter.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMMediaMIDISignalMeter.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMMIDISystem.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMMIDISystem.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMRealtimeMIDIInputDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMRealtimeMIDIInputDescriptor.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMRealtimeMIDIOutputDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMRealtimeMIDIOutputDescriptor.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMRecordingMIDITrackRepository.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMRecordingMIDITrackRepository.h
# End Source File
# Begin Source File

SOURCE=.\Components\EMRubberBandNode.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\EMRubberBandNode.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMWinDXMIDIConsumer.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMWinDXMIDIConsumer.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMWinDXMIDIProducer.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMWinDXMIDIProducer.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMWinMIDIClipRepository.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMWinMIDIClipRepository.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMWinMIDISystem.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMWinMIDISystem.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMWinRealtimeMIDIInputDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMWinRealtimeMIDIInputDescriptor.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMWinRealtimeMIDIOutputDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Midi\EMWinRealtimeMIDIOutputDescriptor.h
# End Source File
# End Group
# Begin Group "Framework / Media / Image"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Framework\Media\Image\EMDXImageFileInputDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Image\EMDXImageFileInputDescriptor.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Image\EMImageClip.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Image\EMImageClip.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Image\EMImageFileInputDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Image\EMImageFileInputDescriptor.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Image\EMMediaPoolImageEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Image\EMMediaPoolImageEntry.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Image\EMTGAImageFileInputDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Image\EMTGAImageFileInputDescriptor.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Framework\Media\EMBeMediaFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMBeMediaFile.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMBitmapBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMBitmapBuffer.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMBufferDestination.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMBufferDestination.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMBufferRepository.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMBufferRepository.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMDescriptor.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMDestructivePlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMDestructivePlugin.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMEncoderDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMEncoderDescriptor.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMEncoderRepository.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMEncoderRepository.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMFileInputDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMFileInputDescriptor.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMInputDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMInputDescriptor.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMInputRepository.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMInputRepository.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaAudioSignalMeter.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaAudioSignalMeter.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaBufferSource.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaBufferSource.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaClipRepository.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaClipRepository.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaDataBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaDataBuffer.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaEffectTrack.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaEffectTrack.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaEffectTrackRepository.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaEffectTrackRepository.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaFile.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaFileRepository.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaFileRepository.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaFormat.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaFormat.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaInputAudioSignalMeter.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaInputAudioSignalMeter.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaItemContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaItemContainer.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaPool.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaPool.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaPoolEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaPoolEntry.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaProject.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaProject.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaSignalMeter.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaSignalMeter.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaSignalMeterRepository.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaSignalMeterRepository.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaSystemInspector.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaSystemInspector.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaTrackRepository.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMMediaTrackRepository.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMNativePlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMNativePlugin.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMNativePluginEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMNativePluginEntry.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMOutputDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMOutputDescriptor.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMOutputRepository.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMOutputRepository.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMPlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMPlugin.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMPluginEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMPluginEntry.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMPluginRepository.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMPluginRepository.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMRealtimeEncoderDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMRealtimeEncoderDescriptor.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMRealtimeInputDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMRealtimeInputDescriptor.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMRealtimeOutputDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMRealtimeOutputDescriptor.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMRecording.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMRecording.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMVideoBitmapScaler.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMVideoBitmapScaler.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinChromaTransitionEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinChromaTransitionEntry.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinDXTransitionEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinDXTransitionEntry.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinMediaFileManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinMediaFileManager.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinMediaMetronome.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinMediaMetronome.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinMediaSystemInspector.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinMediaSystemInspector.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinMediaUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinMediaUtility.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinNodeConnection.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinPluginEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinPluginEntry.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinSMPTETransitionEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinSMPTETransitionEntry.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinTransitionEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinTransitionEntry.h
# End Source File
# End Group
# Begin Group "Framework / Globals"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Components\Media\EMConstants.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Globals\EMGlobals.h
# End Source File
# Begin Source File

SOURCE=.\EMMediaEngineBuild.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Globals\EMMediaEngineUIDs.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Globals\EMMediaGlobals.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Globals\EMMediaTransitionDialogIDs.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Globals\EMMessages.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Globals\EMMIDIGlobals.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Globals\EMSettingIDs.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Framework\EMMediaUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\EMMediaUtility.h
# End Source File
# End Group
# Begin Group "lib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\lib\EMMediaEngine_Library.h
# End Source File
# Begin Source File

SOURCE=.\lib\EMMediaEngine_LibraryVersion.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\EMMediaEngine_Library.dll
# End Source File
# Begin Source File

SOURCE=.\lib\EMMediaEngine_Library.lib
# End Source File
# End Target
# End Project
