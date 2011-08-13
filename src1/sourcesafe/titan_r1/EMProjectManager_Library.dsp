# Microsoft Developer Studio Project File - Name="EMProjectManager_Library" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=EMProjectManager_Library - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EMProjectManager_Library.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EMProjectManager_Library.mak" CFG="EMProjectManager_Library - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EMProjectManager_Library - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EMProjectManager_Library - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Titan_r1", BAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EMProjectManager_Library - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "EMProjectManager_Library_Win32Release"
# PROP Intermediate_Dir "EMProjectManager_Library_Win32Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMPROJECTMANAGER_LIBRARY_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMPROJECTMANAGER_LIBRARY_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "NDEBUG"
# ADD RSC /l 0x41d /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 .\lib\EMCommon_Library.lib .\lib\EMCommand_Library.lib .\lib\EMDebug_Library.lib KERNEL32.LIB advapi32.lib /nologo /dll /pdb:none /machine:I386 /out:"\SourceSafe\ReleaseBuild\EMProjectManager_Library.dll" /implib:"\SourceSafe\ReleaseBuild\lib\EMProjectManager_Library.lib"

!ELSEIF  "$(CFG)" == "EMProjectManager_Library - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ProjectManager_Library_Debug"
# PROP Intermediate_Dir "ProjectManager_Library_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMPROJECTMANAGER_LIBRARY_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EM_PROJECT_MANAGER_LIBRARY_API" /D "DEBUG_VERSION" /FR"EMProjectManager_Library_files/" /FD /LDd /GD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 .\lib\EMCommon_Library.lib .\lib\EMCommand_Library.lib .\lib\EMDebug_Library.lib KERNEL32.LIB advapi32.lib user32.lib /nologo /dll /incremental:no /map /debug /machine:I386 /out:"EMProjectManager_Library.dll" /implib:"lib\EMProjectManager_Library.lib" /subsystem:native
# SUBTRACT LINK32 /profile /nodefaultlib /force
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Cmds=EMVersionNumbering.exe \SourceSafe\Titan_r1\lib\ EMProjectManager_LibraryVersion Jesper
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "EMProjectManager_Library - Win32 Release"
# Name "EMProjectManager_Library - Win32 Debug"
# Begin Group "Components"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Components\EMProjectCommandFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\EMProjectCommandFactory.h
# End Source File
# Begin Source File

SOURCE=.\Components\EMProjectDataLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\EMProjectDataLoader.h
# End Source File
# Begin Source File

SOURCE=.\Components\EMProjectDataSaver.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\EMProjectDataSaver.h
# End Source File
# Begin Source File

SOURCE=.\Components\EMProjectManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\EMProjectManager.h
# End Source File
# End Group
# Begin Group "Application"

# PROP Default_Filter ""
# Begin Group "Application / Commands"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Application\Commands\ProjectCommandAddProjectDataContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\ProjectCommandAddProjectDataContainer.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\ProjectCommandCloseProject.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\ProjectCommandCloseProject.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\ProjectCommandCreateProject.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\ProjectCommandCreateProject.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\ProjectCommandGetDirectory.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\ProjectCommandGetDirectory.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\ProjectCommandLoadProject.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\ProjectCommandLoadProject.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\ProjectCommandLoadSystemSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\ProjectCommandLoadSystemSettings.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\ProjectCommandLoadTemplate.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\ProjectCommandLoadTemplate.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\ProjectCommandReadyForUse.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\ProjectCommandReadyForUse.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\ProjectCommandSaveNeeded.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\ProjectCommandSaveNeeded.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\ProjectCommandSaveProject.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\ProjectCommandSaveProject.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\ProjectCommandSaveSystemSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\ProjectCommandSaveSystemSettings.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Application\DirectoryIDs.h
# End Source File
# Begin Source File

SOURCE=.\Application\ProjectManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\ProjectManager.h
# End Source File
# End Group
# Begin Group "Framework"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Framework\EMProjectDataContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\EMProjectDataContainer.h
# End Source File
# End Group
# Begin Group "lib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\lib\EMProjectManager_Library.h
# End Source File
# Begin Source File

SOURCE=.\lib\EMProjectManager_LibraryVersion.h
# End Source File
# Begin Source File

SOURCE=.\lib\EMProjectManager_Library.lib
# End Source File
# End Group
# Begin Source File

SOURCE=.\EMProjectManager_Library.dll
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\ProjectCommandMoveProject.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\ProjectCommandMoveProject.h
# End Source File
# End Target
# End Project
