# Microsoft Developer Studio Project File - Name="EMWinDirectSoundOutput" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=EMWinDirectSoundOutput - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EMWinDirectSoundOutput.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EMWinDirectSoundOutput.mak" CFG="EMWinDirectSoundOutput - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EMWinDirectSoundOutput - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EMWinDirectSoundOutput - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/EMWinDirectSoundOutput", TJKAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EMWinDirectSoundOutput - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "EMWinDirectSoundOutput_Release"
# PROP Intermediate_Dir "EMWinDirectSoundOutput_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMWINDIRECTSOUNDLIBRARY_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /D "_WIN32_DCOM" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMWINDIRECTSOUNDLIBRARY_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "NDEBUG"
# ADD RSC /l 0x41d /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 winmm.lib .\lib\EMMediaEngine_Library.lib .\lib\EMCommand_Library.lib .\lib\EMSettings_Library.lib .\lib\EMProjectManager_Library.lib .\lib\EMMediaEngine_Library.lib dxguid.lib dsound.lib .\lib\EMCommon_Library.lib .\lib\EMDebug_Library.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /machine:I386 /out:"\SourceSafe\ReleaseBuild\EMWinDirectSoundOutput.dll" /implib:"\SourceSafe\ReleaseBuild\lib\EMWinDirectSoundOutput.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "EMWinDirectSoundOutput - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "EMWinDirectSoundOutput_files"
# PROP BASE Intermediate_Dir "EMWinDirectSoundOutput_files"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "EMWinDirectSoundOutput_files"
# PROP Intermediate_Dir "EMWinDirectSoundOutput_files"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMWINDIRECTSOUNDLIBRARY_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /D "_WIN32_DCOM" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMWINDIRECTSOUNDLIBRARY_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib .\lib\EMMediaEngine_Library.lib .\lib\EMCommand_Library.lib .\lib\EMSettings_Library.lib .\lib\EMProjectManager_Library.lib .\lib\EMMediaEngine_Library.lib dxguid.lib dsound.lib .\lib\EMCommon_Library.lib .\lib\EMDebug_Library.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"EMWinDirectSoundOutput.dll" /implib:"./lib/EMWinDirectSoundOutput.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "EMWinDirectSoundOutput - Win32 Release"
# Name "EMWinDirectSoundOutput - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMDSCache.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMWinDirectSoundPlayback.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMDSCache.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMWinDirectSoundPlayback.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\EMWinDirectSoundOutput.dll
# End Source File
# Begin Source File

SOURCE=.\Lib\EMWinDirectSoundOutput.lib
# End Source File
# End Target
# End Project
