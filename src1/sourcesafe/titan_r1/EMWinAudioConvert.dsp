# Microsoft Developer Studio Project File - Name="EMWinAudioConvert" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=EMWinAudioConvert - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EMWinAudioConvert.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EMWinAudioConvert.mak" CFG="EMWinAudioConvert - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EMWinAudioConvert - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EMWinAudioConvert - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/EMWinAudioConvert", VCIAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EMWinAudioConvert - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMWINAUDIOCONVERT_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMWinAudioConverter_EXPORTS" /Fp"EMWinAudioConverter___Win32_Release/EMWinAudioConverter.pch" /YX /Fo"EMWinAudioConverter___Win32_Release/" /Fd"EMWinAudioConverter___Win32_Release/" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "NDEBUG"
# ADD RSC /l 0x41d /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 strmiids.lib Strmbase.lib quartz.lib advapi32.lib OLE32.LIB KERNEL32.LIB USER32.LIB OLEAUT32.LIB WINMM.LIB UUID.LIB MSACM32.LIB GDI32.LIB /nologo /entry:"DllEntryPoint@12" /dll /pdb:none /machine:I386 /out:"\SourceSafe\ReleaseBuild\EMWinAudioConverter.dll" /implib:"\SourceSafe\ReleaseBuild\lib\EMWinAudioConverter.lib"

!ELSEIF  "$(CFG)" == "EMWinAudioConvert - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMWINAUDIOCONVERT_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D X86_=1 /D "_MT" /D "_DLL" /D "_MBCS" /D "_USRDLL" /D "DEBUG" /D "DEBUG_VERSION" /FR"EMWinAudioConverter_files/" /Fp"EMWinAudioConverter_files/EMWinAudioConverter.pch" /YX /Fo"EMWinAudioConverter_files/" /Fd"EMWinAudioConverter_files/" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 strmiids.lib strmbasd.lib quartz.lib advapi32.lib OLE32.LIB MSVCPRTD.LIB MSVCIRTD.LIB KERNEL32.LIB MSVCRTD.LIB USER32.LIB OLEAUT32.LIB WINMM.LIB UUID.LIB MSACM32.LIB GDI32.LIB strmiids.lib strmbase.lib quartz.lib advapi32.lib OLE32.LIB MSVCPRTD.LIB MSVCIRTD.LIB KERNEL32.LIB MSVCRTD.LIB USER32.LIB OLEAUT32.LIB WINMM.LIB UUID.LIB MSACM32.LIB GDI32.LIB strmiids.lib strmbase.lib quartz.lib advapi32.lib OLE32.LIB MSVCPRTD.LIB MSVCIRTD.LIB KERNEL32.LIB MSVCRTD.LIB USER32.LIB OLEAUT32.LIB WINMM.LIB UUID.LIB MSACM32.LIB GDI32.LIB /nologo /entry:"DllEntryPoint@12" /dll /incremental:no /pdb:"EMWinAudioConverter_files/EMWinAudioConverter.pdb" /debug /machine:I386 /out:"EMWinAudioConverter.dll" /implib:"lib/EMWinAudioConverter.lib" /debug:mapped,full /debug:mapped,full /debug:mapped,full
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=.\EMWinAudioConverter.dll
InputPath=.\EMWinAudioConverter.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "EMWinAudioConvert - Win32 Release"
# Name "EMWinAudioConvert - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\EMWinAudioConverter.def
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMWinAudioConverterFilter.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMWinAudioConverterExport.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMWinAudioConverterFilter.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\Audio\EMWinAudioConverterInterface.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Media\EMWinDXCallbackInterface.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\EMWinAudioConverter.dll
# End Source File
# Begin Source File

SOURCE=.\Lib\EMWinAudioConverter.lib
# End Source File
# End Target
# End Project
