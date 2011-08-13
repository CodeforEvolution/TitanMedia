# Microsoft Developer Studio Project File - Name="EMWinVideoVirtualRenderer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=EMWinVideoVirtualRenderer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EMWinVideoVirtualRenderer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EMWinVideoVirtualRenderer.mak" CFG="EMWinVideoVirtualRenderer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EMWinVideoVirtualRenderer - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EMWinVideoVirtualRenderer - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/EMWinVideoVirtualRenderer", THGAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EMWinVideoVirtualRenderer - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMWinVideoVirtualRenderer_EXPORTS" /YX /FD /c
# ADD CPP /nologo /Gz /MD /W3 /O2 /Ob0 /I "..\..\BaseClasses" /I "..\..\..\..\..\include" /D "INC_OLE2" /D "STRICT" /D _WIN32_WINNT=0x0400 /D "WIN32" /D "_WIN32" /D "_MT" /D "_DLL" /D _X86_=1 /D WINVER=0x0400 /D "NDEBUG" /D try=__try /D except=__except /D leave=__leave /D finally=__finally /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "..\..\BaseClasses" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 ..\..\BaseClasses\release\strmbase.lib quartz.lib winmm.lib msvcrt.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib ole32.lib oleaut32.lib uuid.lib /nologo /entry:"DllEntryPoint@12" /dll /machine:I386 /nodefaultlib /out:"Release/EMWinVideoVirtualRenderer.ax" /libpath:"..\..\..\..\lib"
# SUBTRACT LINK32 /incremental:yes

!ELSEIF  "$(CFG)" == "EMWinVideoVirtualRenderer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "EMWinVideoVirtualRenderer_files"
# PROP Intermediate_Dir "EMWinVideoVirtualRenderer_files"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMWinVideoVirtualRenderer_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\BaseClasses" /I "..\..\..\..\..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D X86_=1 /D "_MT" /D "_DLL" /D "_MBCS" /D "_USRDLL" /D "DEBUG" /D "EMWinVideoVideoVirtualRenderer_EXPORTS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "..\..\BaseClasses" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 .\lib\EMMediaEngine_Library.lib .\lib\EMProjectManager_Library.lib .\lib\EMCommon_Library.lib strmbasd.lib .\lib\EMDebug_Library.lib quartz.lib winmm.lib msvcrt.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib ole32.lib oleaut32.lib uuid.lib /nologo /entry:"DllEntryPoint@12" /dll /pdb:none /debug /machine:I386 /out:"EMVirtualRenderer_Library.dll" /implib:".\lib\EMVirtualRenderer_Library.lib" /libpath:"..\..\..\..\lib"
# Begin Custom Build
OutDir=.\EMWinVideoVirtualRenderer_files
TargetPath=.\EMVirtualRenderer_Library.dll
InputPath=.\EMVirtualRenderer_Library.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "EMWinVideoVirtualRenderer - Win32 Release"
# Name "EMWinVideoVirtualRenderer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinVideoVirtualRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinVideoVirtualRenderer.def
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinVideoVirtualRenderer.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinVideoVirtualRendererFilter.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Video\EMWinVideoVirtualRendererIDs.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\EMVirtualRenderer_Library.dll
# End Source File
# Begin Source File

SOURCE=.\lib\EMVirtualRenderer_Library.lib
# End Source File
# End Target
# End Project
