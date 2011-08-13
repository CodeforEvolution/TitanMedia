# Microsoft Developer Studio Project File - Name="EMWinDXPluginEngine" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=EMWinDXPluginEngine - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EMWinDXPluginEngine.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EMWinDXPluginEngine.mak" CFG="EMWinDXPluginEngine - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EMWinDXPluginEngine - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EMWinDXPluginEngine - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/EMWinDXPluginEngine", UOGAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EMWinDXPluginEngine - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "EMWinDXPluginEngine___Win32_Release"
# PROP BASE Intermediate_Dir "EMWinDXPluginEngine___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "EMWinDXPluginEngine___Win32_Release"
# PROP Intermediate_Dir "EMWinDXPluginEngine___Win32_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMWINDXPLUGINENGINE_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMWINDXPLUGINENGINE_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "NDEBUG"
# ADD RSC /l 0x41d /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 .\lib\EMCommand_Library.lib strmiids.lib Strmbase.lib quartz.lib .\lib\EMMediaEngine_Library.lib .\lib\EMProjectManager_Library.lib .\lib\EMCommon_Library.lib .\lib\EMWinMediaInputFilter.lib .\lib\EMSettings_Library.lib .\lib\EMDebug_Library.lib advapi32.lib OLE32.LIB KERNEL32.LIB USER32.LIB OLEAUT32.LIB WINMM.LIB UUID.LIB MSACM32.LIB GDI32.LIB /nologo /entry:"DllEntryPoint@12" /dll /pdb:none /machine:I386 /out:"\SourceSafe\ReleaseBuild\EMWinDXPluginEngine.dll" /implib:"\SourceSafe\ReleaseBuild\lib\EMWinDXPluginEngine.lib"
# Begin Custom Build - Performing registration
OutDir=.\EMWinDXPluginEngine___Win32_Release
TargetPath=\SourceSafe\ReleaseBuild\EMWinDXPluginEngine.dll
InputPath=\SourceSafe\ReleaseBuild\EMWinDXPluginEngine.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "EMWinDXPluginEngine - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "EMWinDXPluginEngine___Win32_Debug"
# PROP BASE Intermediate_Dir "EMWinDXPluginEngine___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "EMWinDXPluginEngine_files"
# PROP Intermediate_Dir "EMWinDXPluginEngine_files"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMWINDXPLUGINENGINE_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D X86_=1 /D "_MT" /D "_DLL" /D "_MBCS" /D "_USRDLL" /D "DEBUG" /D "DEBUG_VERSION" /FR /YX /Fd"EMWinDirectXPluginWrapper_files/" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 .\lib\EMCommand_Library.lib .\lib\EMMediaEngine_Library.lib .\lib\EMProjectManager_Library.lib .\lib\EMCommon_Library.lib .\lib\EMWinMediaInputFilter.lib .\lib\EMSettings_Library.lib .\lib\EMDebug_Library.lib strmiids.lib strmbasd.lib quartz.lib advapi32.lib OLE32.LIB MSVCPRTD.LIB MSVCIRTD.LIB KERNEL32.LIB MSVCRTD.LIB USER32.LIB OLEAUT32.LIB WINMM.LIB UUID.LIB MSACM32.LIB GDI32.LIB /nologo /entry:"DllEntryPoint@12" /dll /incremental:no /pdb:"EMWinDXPluginEngine___Win32_Debug/EMWinDirectXPluginWrapper.pdb" /debug /machine:I386 /out:"EMWinDXPluginEngine.dll" /implib:"lib/EMWinDXPluginEngine.lib" /debug:mapped,full
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Performing registration
OutDir=.\EMWinDXPluginEngine_files
TargetPath=.\EMWinDXPluginEngine.dll
InputPath=.\EMWinDXPluginEngine.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Cmds=EMVersionNumbering.exe \SourceSafe\Titan_r1\lib\ EMWinDXPluginEngineVersion Richard 1
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "EMWinDXPluginEngine - Win32 Release"
# Name "EMWinDXPluginEngine - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Framework\Media\EMWinDXPluginEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinDXPluginEngineFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinDXPluginEngineInputPin.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinDXPluginEngineOutputPin.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\EMWinDXPluginEngine.def
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinDXPluginEngine.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinDXPluginEngineExport.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinDXPluginEngineFilter.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinDXPluginEngineInputPin.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinDXPluginEngineInterface.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinDXPluginEngineOutputPin.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\EMWinDXPluginEngine.dll
# End Source File
# Begin Source File

SOURCE=.\lib\EMWinDXPluginEngineVersion.h
# End Source File
# Begin Source File

SOURCE=.\lib\EMWinDXPluginEngine.lib
# End Source File
# End Target
# End Project
