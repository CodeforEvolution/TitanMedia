# Microsoft Developer Studio Project File - Name="Titan_r1" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Titan_r1 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Titan_r1.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Titan_r1.mak" CFG="Titan_r1 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Titan_r1 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Titan_r1 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Titan_r1 - Win32 Trial Version Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Titan_r1", BAAAAAAA"
# PROP Scc_LocalPath "..\..\SOURCESAFE\TITAN_R1"
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Titan_r1 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /D "_WIN32_DCOM" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D X86_=1 /D "_MT" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "NDEBUG"
# ADD RSC /l 0x41d /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 .\lib\EMCommon_Library.lib .\lib\EMSettings_Library.lib .\lib\EMCommand_Library.lib .\lib\EMDebug_Library.lib .\lib\EMMediaEngine_Library.lib .\lib\EMNetworkEngine_Library.lib .\lib\EMProjectManager_Library.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib msimg32.lib .\lib\EMResourceFull.lib winmm.lib /nologo /subsystem:windows /machine:I386 /out:"\SourceSafe\ReleaseBuild\Titan_r1.exe"

!ELSEIF  "$(CFG)" == "Titan_r1 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GR /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_WIN32_DCOM" /FR /FD /LDd /GD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 .\lib\EMExport_Library.lib .\lib\EMCommon_Library.lib .\lib\EMSettings_Library.lib .\lib\EMCommand_Library.lib .\lib\EMDebug_Library.lib .\lib\EMMediaEngine_Library.lib .\lib\EMNetworkEngine_Library.lib .\lib\EMProjectManager_Library.lib comctl32.lib msimg32.lib .\lib\EMResourceFullD.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386 /nodefaultlib:"nafxcwd.lib" /out:"Titan_r1.exe"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Titan_r1 - Win32 Trial Version Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Titan_r1___Win32_Trial_Version_Release"
# PROP BASE Intermediate_Dir "Titan_r1___Win32_Trial_Version_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_WIN32_DCOM" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /D "_WIN32_DCOM" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D X86_=1 /D "_MT" /D "TRIAL_VERSION" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "NDEBUG"
# ADD RSC /l 0x41d /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 .\lib\EMCommon_Library.lib .\lib\EMSettings_Library.lib .\lib\EMCommand_Library.lib .\lib\EMDebug_Library.lib .\lib\EMMediaEngine_Library.lib .\lib\EMNetworkEngine_Library.lib .\lib\EMProjectManager_Library.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib msimg32.lib /nologo /subsystem:windows /machine:I386 /out:"\SourceSafe\ReleaseBuild\Titan_r1.exe"
# ADD LINK32 .\lib\EMCommon_Library.lib .\lib\EMSettings_Library.lib .\lib\EMCommand_Library.lib .\lib\EMDebug_Library.lib .\lib\EMMediaEngine_Library.lib .\lib\EMNetworkEngine_Library.lib .\lib\EMProjectManager_Library.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib msimg32.lib EMResourceTrial.lib /nologo /subsystem:windows /machine:I386 /out:"\SourceSafe\ReleaseBuild\Titan_r1.exe"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Cmds=del \SourceSafe\ReleaseBuild\Titan_r1.exe
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Titan_r1 - Win32 Release"
# Name "Titan_r1 - Win32 Debug"
# Name "Titan_r1 - Win32 Trial Version Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\ApplicationInit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\AudioGClip.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\AudioGClipContainer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\AudioGTrackInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\AudioInputSignalView.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\DestructiveEditBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\EMFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMGClip.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMGClipContainer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMGClipRepository.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\GUI\EMGEffectsRepository.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\GUI\EMGRubberBand.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMGTrack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMGTrackInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMGTrackRepository.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMGUIComponent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMGUIUtilities.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMImageBufferRepository.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMKeyboard.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMMenu.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMMenuBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMMenuItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMMouseCursor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMPopUpMenu.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\EMResource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\EMResourceRepository.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMScrollbar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMSignalMeter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMSlimButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMSlimButtonView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMSlimEditLabel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMSlimLabel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMSlimValueBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMTextControl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\EMThreadRepository.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMVideoPreview.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWindow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\EMWinFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinKeyboard.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinMenu.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinMenuBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinMenuItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMWinMouseCursor.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMWinPopUpMenu.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinScrollbar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinTextControl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMwinView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinWindow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\FileDialogTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\FXNode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\GClip.cpp
# End Source File
# Begin Source File

SOURCE=.\ThirdParty\getdxver.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\GTrackInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\MIDIGClip.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\MIDIGClipContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\MIDIGTrackInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\MuteButton.cpp
# End Source File
# Begin Source File

SOURCE=.\Crypto\r.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\SelectionButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\SoloButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\TimeDisplay.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Titan_r1.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\Toolbox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\TrackInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\TrackInfoButtonBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\TrackView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\TrackWindow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\TrackWindowStatusBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\TrackWindowTimeLine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\TrackWindowToolBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\TransitionGClip.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\TransitionGClipContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\TransitionGTrackInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\UtilityView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\VideoGClip.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\VideoGClipContainer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\VideoGTrackInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\VideoPreviewView.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Application\GUI\DestructiveEditBase.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Globals\EMCommandLineIDs.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Globals\EMGlobals.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Globals\EMGUIGlobals.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Globals\EMKeyboardGlobals.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Globals\EMMessages.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Globals\EMWinMessages.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\GUIGlobals.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Messages.h
# End Source File
# Begin Source File

SOURCE=.\Crypto\r.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\ResourceIDs.h
# End Source File
# Begin Source File

SOURCE=.\Application\SettingIDs.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Commands_cpp"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\Application\Commands\CommandActiveMIDITrackStepDown.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandActiveMIDITrackStepUp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandAddAudioTrack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandAddGAudioClip.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandAddGAudioTrack.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAddGMIDIClip.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAddGMIDITrack.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAddGTransitionClip.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAddGTransitionTrack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandAddGVideoClip.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandAddGVideoTrack.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAddMIDITrack.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAddTransitionTrack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandAddVideoTrack.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAudioCrop.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAudioDestructiveFX.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAudioDestructiveFX.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAudioFadeIn.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAudioFadeOut.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAudioLockToTempo.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAudioNormalize.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAudioNormalize.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAudioReverse.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAudioSilence.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandChangeVideoPlaybackSpeed.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandChangeVideoPlaybackSpeed.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandClearUndoStack.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandClipsSelectAll.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandClipsSelectInvert.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandClipsSelectNone.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandCloneClips.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandCloneMIDIEvents.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandCreateMIDIEvents.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandCreateMIDIEvents.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandCreateProject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandCutClips.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandCutMIDIEvents.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandCutMIDIEvents.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandDeleteClips.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDeleteClipsEventsNodes.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandDeleteClipsFromList.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDeleteEntryFromMediapoolView.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDeleteEntryFromMediapoolView.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDeleteMIDIEvents.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandDeleteTracks.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDeleteTracksFromList.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayEQDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayEQDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplaySearchAddUserDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplaySearchAddUserDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplaySearchUserDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplaySearchUserDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplaySplashScreenText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandExpandTimeLine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandExpandTrackInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandExpandUtilityView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandFastForward.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandGetApplicationData.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandGetGTrackIndex.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandGetTrackGUIIndex.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandGroupClips.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandGroupClipsFromList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandLoadMedia.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandMessageSender.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandMIDIFixedLength.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandMIDIQuickQuantize.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandMIDIToggleMetronome.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandMoveClips.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandMoveMIDIEvents.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandMoveTracks.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandMuteClips.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandNewProject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandOpenProject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandPlay.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandPostCommand.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandPostCommand.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandPrepareRecording.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandProjectBegin.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandProjectEnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandQuitApplication.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandRedo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandRegionBegin.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandRegionEnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandRewind.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandSaveProject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandSetClipEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandSetDefaultSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandSetDefaultSystemSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandSetDefaultSystemSettings.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandSetFXEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandSetTrackEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandSetUndoTexts.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandStop.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandToggleAutoQuantize.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandToggleEnableAudio.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandToggleEnableMIDI.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandToggleEnableVideo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandToggleExpertMode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandToggleLoop.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandTogglePlayback.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandToggleRelativeMove.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandToggleSnapping.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandToggleViewClipContent.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandTracksSelectAll.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandTracksSelectInvert.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandTracksSelectNone.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandUndo.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandUngroupClips.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandUpdateInstanceInMediapoolView.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandUpdateInstanceInMediapoolView.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandUtilityButtonPressed.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandWriteStatusBarMessage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandZoomInHorizontal.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandZoomInTracksVertical.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandZoomInTrackVertical.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandZoomOutHorizontal.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandZoomOutTracksVertical.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandZoomOutTrackVertical.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\UndoCommandAddRubberBandNodes.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\UndoCommandChangeClipResize.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\UndoCommandChangeClipSelection.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\UndoCommandDeleteRubberBandNodes.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\UndoCommandMoveRubberBandNodes.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\UndoCommandResizeMIDIEvent.cpp
# End Source File
# End Group
# Begin Group "Commands_h"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\Application\Commands\CommandActiveMIDITrackStepDown.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandActiveMIDITrackStepUp.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandAddAudioTrack.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandAddGAudioClip.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandAddGAudioTrack.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAddGMIDIClip.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAddGMIDITrack.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAddGTransitionClip.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAddGTransitionTrack.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandAddGVideoClip.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandAddGVideoTrack.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAddMIDITrack.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAddToMediaPoolView.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAddTransitionTrack.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandAddVideoTrack.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAudioCrop.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAudioFadeIn.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAudioFadeOut.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAudioLockToTempo.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAudioReverse.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandAudioSilence.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandChangeExplorerListViewControlStyle.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandClearUndoStack.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandClipsSelectAll.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandClipsSelectInvert.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandClipsSelectNone.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandCloneClips.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandCloneMIDIEvents.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandCreateProject.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandCutClips.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandDeleteClips.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDeleteClipsEventsNodes.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandDeleteClipsFromList.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDeleteMIDIEvents.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandDeleteTracks.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDeleteTracksFromList.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayAboutDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayChangeClipNameDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayClipPropertiesDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayExportMediaDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayExportMediaSettingsDialog.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandDisplayNewProjectDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayNormalizeDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayProjectSettingsDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayPropertyPageDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayQuantizeDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplaySetVelocityDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplaySplashScreenText.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplaySystemSettingsDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayTransitionDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayTransposeDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayVideoRecordingDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayVideoSpeedDialog.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandExpandTimeLine.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandExpandTrackInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandExpandUtilityView.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandFastForward.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandGetApplicationData.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandGetEntriesFromMediaPoolView.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandGetGTrackIndex.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandGetTrackGUIIndex.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandGroupClips.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandGroupClipsFromList.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandLoadMedia.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandMessageSender.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandMIDIFixedLength.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandMIDIQuickQuantize.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandMIDIToggleMetronome.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandMoveClips.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandMoveMIDIEvents.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandMoveTracks.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandMuteClips.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandNewProject.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandOpenProject.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandPlay.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandPrepareRecording.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandProjectBegin.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandProjectEnd.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandQuitApplication.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandRedo.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandRegionBegin.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandRegionEnd.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandRewind.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandSaveProject.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandSetClipEdit.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandSetDefaultSettings.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandSetFXEdit.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandSetTrackEdit.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandSetUndoTexts.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandStop.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandToggleAutoQuantize.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandToggleEnableAudio.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandToggleEnableMIDI.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandToggleEnableVideo.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandToggleExpertMode.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandToggleLoop.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandTogglePlayback.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandToggleRelativeMove.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandToggleSnapping.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandToggleViewClipContent.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandTracksSelectAll.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandTracksSelectInvert.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandTracksSelectNone.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandUndo.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandUngroupClips.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandUtilityButtonPressed.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandWriteStatusBarMessage.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandZoomInHorizontal.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandZoomInTracksVertical.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandZoomInTrackVertical.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandZoomOutHorizontal.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandZoomOutTracksVertical.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandZoomOutTrackVertical.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\UndoCommandAddRubberBandNodes.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\UndoCommandChangeClipResize.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\UndoCommandChangeClipSelection.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\UndoCommandDeleteRubberBandNodes.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\UndoCommandMoveRubberBandNodes.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\UndoCommandResizeMIDIEvent.h
# End Source File
# End Group
# Begin Group "Dialogs_cpp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Application\GUI\AboutDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\AboutDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\AudioExtractionConversionDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\AudioExtractionConversionDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\ChangeClipNameDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\ChangeClipNameDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\ClipPropertiesDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\ClipPropertiesDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\ColorBalanceDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\ColorBalanceDialog.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Gui\EMTrackbarControl.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Gui\EMTrackbarControl.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Gui\EMWinTrackbarControl.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Gui\EMWinTrackbarControl.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\EQDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\EQDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\ExportAudioSettingsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\ExportAudioSettingsDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\ExportMediaDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\ExportMediaDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\ExportMediaSettingsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\ExportMediaSettingsDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\HSV_ValueDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\HSV_ValueDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\LoginDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\LoginDialog.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\NewProjectDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\NewProjectDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\NormalizeDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\NormalizeDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\ProjectSettingsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\ProjectSettingsDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\PropertyPageDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\PropertyPageDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\QuantizeDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\QuantizeDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\RegistrationDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\RegistrationDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\SearchAddUserDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\SearchAddUserDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\SearchUserDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\SearchUserDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\SetVelocityDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\SetVelocityDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\SplashWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\SplashWindow.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\SystemSettingsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\SystemSettingsDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\TransitionDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\TransitionDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\TransposeDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\TransposeDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\VideoRecordingDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\VideoRecordingDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\VideoSpeedDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\VideoSpeedDialog.h
# End Source File
# End Group
# Begin Group "Header Files 2"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\ApplicationInit.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\AudioGClip.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\AudioGClipContainer.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\AudioGTrackInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\AudioInputSignalView.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\ColaborationView.h
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMBrowseWindow.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMButton.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMCheckBox.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMComboBox.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\EMDefaultSettingsContainer.h
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMDragDrop.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\EMFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMFileWindow.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMGClip.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMGClipContainer.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMGClipRepository.h
# End Source File
# Begin Source File

SOURCE=.\Components\GUI\EMGEffectsRepository.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMGroupBox.h
# End Source File
# Begin Source File

SOURCE=.\Components\GUI\EMGRubberBand.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMGTrack.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMGTrackInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMGTrackRepository.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMGUIComponent.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMGUIUtilities.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMImageBufferRepository.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMKeyboard.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMLabel.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMListBox.h
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMListViewControl.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMMenu.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMMenuBar.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMMenuItem.h
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMMessageBox.h
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMMouseCursor.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMPopUpMenu.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Gui\EMProgressBar.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Gui\EMRadioButton.h
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMResizeTab.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\EMResource.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\EMResourceRepository.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMScrollbar.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMSignalMeter.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMSlimButton.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMSlimButtonView.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMSlimEditLabel.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMSlimLabel.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMSlimLabelEdit.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMSlimValueBox.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Gui\EMTabControl.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMTextControl.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMTextView.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\EMThreadRepository.h
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMTreeViewControl.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\Gui\EMVideoPreview.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMView.h
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMWinBrowseWindow.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinButton.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinCheckBox.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinComboBox.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWindow.h
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMWinDragDrop.h
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMWinDropSource.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\EMWinFactory.h
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMWinFileWindow.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinGroupBox.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinKeyboard.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinLabel.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinListBox.h
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMWinListViewControl.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinMenu.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinMenuBar.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinMenuItem.h
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMWinMessageBox.h
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMWinMouseCursor.h
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMWinPopUpMenu.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Gui\EMWinProgressBar.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Gui\EMWinRadioButton.h
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMWinResizeTab.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinScrollbar.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Gui\EMWinTabControl.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinTextControl.h
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMWinTreeViewControl.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinView.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinWindow.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\ExplorerView.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\FileDialogTarget.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\FXNode.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\GClip.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\GTrackInfo.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\MediaPoolView.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\MIDIGClip.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\MIDIGClipContainer.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\MIDIGTrackInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\MuteButton.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\SelectionButton.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\SoloButton.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\TimeDisplay.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Titan_r1.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\Toolbox.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\TrackInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\TrackInfoButtonBar.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\TrackView.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\TrackWindow.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\TrackWindowStatusBar.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\TrackWindowTimeLine.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\TrackWindowToolBar.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\TransitionGClip.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\TransitionGClipContainer.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\TransitionGTrackInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\UtilityView.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\VideoGClip.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\VideoGClipContainer.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\VideoGTrackInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\VideoPreviewView.h
# End Source File
# End Group
# Begin Group "Source Files 2"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\Application\GUI\ColaborationView.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMBrowseWindow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMCheckBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMDragDrop.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMFileWindow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMGroupBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMLabel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMListViewControl.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMMessageBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Gui\EMProgressBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Gui\EMRadioButton.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMResizeTab.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Gui\EMTabControl.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMTreeViewControl.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMWinBrowseWindow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinCheckBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMWinDragDrop.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMWinDropSource.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMWinFileWindow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinGroupBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinLabel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMWinListViewControl.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMWinMessageBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Gui\EMWinProgressBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Gui\EMWinRadioButton.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMWinResizeTab.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Gui\EMWinTabControl.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMWinTreeViewControl.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\ExplorerView.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\MediaPoolView.cpp
# End Source File
# End Group
# Begin Group "Commands_cpp 2"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\Application\Commands\CommandAddToMediaPoolView.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandChangeExplorerListViewControlStyle.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandCloseAudioExtractionConversionDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandCloseAudioExtractionConversionDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayAboutDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayAudioExtractionConversionDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayAudioExtractionConversionDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayChangeClipNameDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayClipPropertiesDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayColorBalanceDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayColorBalanceDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayExportMediaDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayExportMediaSettingsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayHSV_ValueDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayHSV_ValueDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayLoginDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayLoginDialog.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Commands\CommandDisplayNewProjectDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayNormalizeDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayProjectSettingsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayPropertyPageDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayQuantizeDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayRegistrationDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayRegistrationDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplaySetVelocityDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplaySplashWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplaySplashWindow.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplaySystemSettingsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayTransitionDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayTransposeDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayVideoRecordingDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplayVideoSpeedDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandGetEntriesFromMediaPoolView.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandSetBottomLabelInAudioExtractionConversionDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandSetBottomLabelInAudioExtractionConversionDialog.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandSetValueInAudioExtractionConversionDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandSetValueInAudioExtractionConversionDialog.h
# End Source File
# End Group
# End Target
# End Project
