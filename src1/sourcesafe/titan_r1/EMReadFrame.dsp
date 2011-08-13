# Microsoft Developer Studio Project File - Name="EMReadFrame" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=EMReadFrame - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EMReadFrame.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EMReadFrame.mak" CFG="EMReadFrame - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EMReadFrame - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EMReadFrame - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/EMReadFrame", UOGAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EMReadFrame - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "EMReadFrame_ReleaseBuild_files"
# PROP Intermediate_Dir "EMReadFrame_ReleaseBuild_files"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMReadFrame_EXPORTS" /YX /FD /c
# ADD CPP /nologo /Gz /MD /W3 /GR /GX /O2 /Ob0 /I "..\..\BaseClasses" /I "..\..\..\..\..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "..\..\BaseClasses" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 .\lib\EMMediaEngine_library.lib .\lib\EMD.\lib\EMMediaEngine_library.lib .\lib\EMDebug_Library.lib strmbase.lib quartz.lib winmm.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib ole32.lib uuid.lib oleaut32.lib /nologo /entry:"DllEntryPoint@12" /dll /pdb:none /machine:I386 /out:"\SourceSafe\ReleaseBuild\EMReadFrame_Library.dll" /implib:"\SourceSafe\ReleaseBuild\lib\EMReadFrame_Library.lib" /libpath:"..\..\..\..\lib" /OPT:NOREF
# Begin Custom Build
OutDir=.\EMReadFrame_ReleaseBuild_files
TargetPath=\SourceSafe\ReleaseBuild\EMReadFrame_Library.dll
InputPath=\SourceSafe\ReleaseBuild\EMReadFrame_Library.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EMReadFrame - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMReadFrame_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\BaseClasses" /I "..\..\..\..\..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D X86_=1 /D "_MT" /D "_DLL" /D "_MBCS" /D "_USRDLL" /D "DEBUG" /D "EMReadFrame_EXPORTS" /D "DEBUG_VERSION" /FR"EMReadFrame_files/" /Fp"EMReadFrame_files/EMReadFrame.pch" /YX /Fo"EMReadFrame_files/" /Fd"EMReadFrame_files/" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "..\..\BaseClasses" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"EMReadFrame_files/EMReadFrame.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 .\lib\EMMediaEngine_library.lib .\lib\EMDebug_Library.lib strmbasd.lib quartz.lib winmm.lib msvcrt.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib ole32.lib oleaut32.lib uuid.lib /nologo /entry:"DllEntryPoint@12" /dll /pdb:none /debug /machine:I386 /out:"EMReadFrame_Library.dll" /implib:".\lib\EMReadFrame_Library.lib" /libpath:"..\..\..\..\lib"
# Begin Custom Build
OutDir=.\Debug
TargetPath=.\EMReadFrame_Library.dll
InputPath=.\EMReadFrame_Library.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Cmds=EMVersionNumbering.exe \SourceSafe\Titan_r1\lib\ EMReadFrame_LibraryVersion Richard 1
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "EMReadFrame - Win32 Release"
# Name "EMReadFrame - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Framework\Media\Video\EMReadFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMReadFrame.def
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMScalingUtils.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Framework\Media\Video\EMReadFrame.h
# End Source File
# Begin Source File

SOURCE=.\lib\EMReadFrame_LibraryVersion.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMReadFrameInterface.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMReadFrameuids.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMScalingUtils.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\EMReadFrame_Library.dll
# End Source File
# Begin Source File

SOURCE=.\lib\EMReadFrame_Library.lib
# End Source File
# End Target
# End Project
