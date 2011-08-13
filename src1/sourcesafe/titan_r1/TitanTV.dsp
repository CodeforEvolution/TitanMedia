# Microsoft Developer Studio Project File - Name="TitanTV" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=TitanTV - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TitanTV.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TitanTV.mak" CFG="TitanTV - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TitanTV - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TitanTV - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TitanTV - Win32 Release"

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
# ADD CPP /nologo /GB /MT /W3 /GR /GX /O2 /D "_WIN32_DCOM" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D X86_=1 /D "_MT" /D "TITAN_TV_VERSION" /D "EM_BASIC_COMPONENTS" /YX /FD /c
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

!ELSEIF  "$(CFG)" == "TitanTV - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_WIN32_DCOM" /D "TITAN_TV_VERSION" /D "EM_BASIC_COMPONENTS" /FR /FD /LDd /GD /GZ /c
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
# ADD LINK32 .\lib\EMExport_Library.lib .\lib\EMCommon_Library.lib .\lib\EMSettings_Library.lib .\lib\EMCommand_Library.lib .\lib\EMDebug_Library.lib .\lib\EMMediaEngine_Library.lib .\lib\EMNetworkEngine_Library.lib .\lib\EMProjectManager_Library.lib comctl32.lib msimg32.lib .\lib\EMResourceFullD.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386 /nodefaultlib:"nafxcwd.lib" /out:"TitanTV.exe"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "TitanTV - Win32 Release"
# Name "TitanTV - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\titan_tv\ApplicationInit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\EMFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\EMFileManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMKeyboard.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMMessageBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\EMResource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\EMResourceRepository.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\EMThreadRepository.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\EMUniqueID.cpp
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

SOURCE=.\Framework\EMWinFileManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinKeyboard.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMWinMessageBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMwinView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\getdxver.cpp
# End Source File
# Begin Source File

SOURCE=..\titan_tv\MediaManager.cpp
# End Source File
# Begin Source File

SOURCE=..\titan_tv\SelectionManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\SplashWindow.cpp
# End Source File
# Begin Source File

SOURCE=..\titan_tv\TitanTV.cpp
# End Source File
# Begin Source File

SOURCE=..\titan_tv\TVView.cpp
# End Source File
# Begin Source File

SOURCE=..\titan_tv\TVWindow.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Framework\Globals\EMCommandLineIDs.h
# End Source File
# Begin Source File

SOURCE=.\Components\EMFileManager.h
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

SOURCE=.\Components\EMUniqueID.h
# End Source File
# Begin Source File

SOURCE=.\Framework\EMWinFileManager.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Globals\EMWinMessages.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Gui\GUIGlobals.h
# End Source File
# Begin Source File

SOURCE=..\titan_tv\MediaManager.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Application\Messages.h
# End Source File
# Begin Source File

SOURCE=..\titan_tv\ResourceIDs.h
# End Source File
# Begin Source File

SOURCE=..\titan_tv\SelectionManager.h
# End Source File
# Begin Source File

SOURCE=.\Application\SettingIDs.h
# End Source File
# Begin Source File

SOURCE=..\titan_tv\TitanTVGlobals.h
# End Source File
# Begin Source File

SOURCE=..\titan_tv\TVView.h
# End Source File
# Begin Source File

SOURCE=..\titan_tv\TVWindow.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Commands_cpp"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplaySplashScreenText.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandGetApplicationData.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandPostCommand.cpp
# End Source File
# Begin Source File

SOURCE=..\titan_tv\CommandSetDefaultSettings.cpp
# End Source File
# Begin Source File

SOURCE=..\titan_tv\CommandSetDefaultSystemSettings.cpp
# End Source File
# Begin Source File

SOURCE=..\titan_tv\CommandStop.cpp
# End Source File
# End Group
# Begin Group "Commands_h"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\Application\Commands\CommandDisplaySplashScreenText.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\CommandGetApplicationData.h
# End Source File
# Begin Source File

SOURCE=..\titan_tv\CommandSetDefaultSettings.h
# End Source File
# Begin Source File

SOURCE=..\titan_tv\CommandSetDefaultSystemSettings.h
# End Source File
# Begin Source File

SOURCE=..\titan_tv\CommandStop.h
# End Source File
# End Group
# Begin Group "Header Files 2"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=..\titan_tv\ApplicationInit.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\EMDefaultSettingsContainer.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\EMFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMGUIComponent.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMKeyboard.h
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMMessageBox.h
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMMouseCursor.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\EMResource.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\EMResourceRepository.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Components\EMThreadRepository.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMView.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWindow.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\EMWinFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinKeyboard.h
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMWinMessageBox.h
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMWinMouseCursor.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinView.h
# End Source File
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMWinWindow.h
# End Source File
# Begin Source File

SOURCE=.\Application\GUI\SplashWindow.h
# End Source File
# Begin Source File

SOURCE=..\titan_tv\TitanTV.h
# End Source File
# End Group
# Begin Group "Source Files 2"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=..\..\SourceSafe\Titan_r1\Framework\Gui\EMGUIComponent.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMMouseCursor.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\GUI\EMWinMouseCursor.cpp
# End Source File
# End Group
# End Target
# End Project
