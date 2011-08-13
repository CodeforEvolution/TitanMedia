# Microsoft Developer Studio Project File - Name="EMDebug_Library" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=EMDebug_Library - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EMDebug_Library.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EMDebug_Library.mak" CFG="EMDebug_Library - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EMDebug_Library - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EMDebug_Library - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Titan_r1", BAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EMDebug_Library - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "EMDebugLibrary_ReleaseBuild_files"
# PROP Intermediate_Dir "EMDebugLibrary_ReleaseBuild_files"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMDEBUG_LIBRARY_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMDEBUG_LIBRARY_EXPORTS" /D "__EM_DEBUG_LIBRARY_IS_BUILDING" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "NDEBUG"
# ADD RSC /l 0x41d /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 .\lib\EMCommon_Library.lib gdi32.lib winmm.lib comdlg32.lib comctl32.lib USER32.LIB /nologo /dll /pdb:none /machine:I386 /out:"\SourceSafe\ReleaseBuild\EMDebug_Library.dll" /implib:"C:\SourceSafe\ReleaseBuild\lib\EMDebug_Library.lib"

!ELSEIF  "$(CFG)" == "EMDebug_Library - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "EMDebug_Library___Win32_Debug"
# PROP BASE Intermediate_Dir "EMDebug_Library___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "EMDebug_Library_files"
# PROP Intermediate_Dir "EMDebug_Library_files"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMDEBUG_LIBRARY_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "__EM_DEBUG_LIBRARY_IS_BUILDING" /D "DEBUG_VERSION" /FR /Fo"Release/" /Fd"Release/" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 .\lib\EMCommon_Library.lib gdi32.lib winmm.lib comdlg32.lib comctl32.lib /nologo /subsystem:console /dll /pdb:none /debug /machine:IX86 /out:"EMDebug_Library.dll" /implib:"lib\EMDebug_Library.lib" /defaultlib:user32.lib
# SUBTRACT LINK32 /map
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Cmds=EMVersionNumbering.exe \SourceSafe\Titan_r1\lib\ EMDebug_LibraryVersion Jesper
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "EMDebug_Library - Win32 Release"
# Name "EMDebug_Library - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Components\EMDebugOutput.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\lib\EMDebug_LibraryVersion.h
# End Source File
# Begin Source File

SOURCE=.\Components\EMDebugOutput.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\EMDebug_Library.def
# End Source File
# Begin Source File

SOURCE=.\EMDebug_Library.dll
# End Source File
# Begin Source File

SOURCE=.\lib\EMDebug_Library.lib
# End Source File
# End Target
# End Project
