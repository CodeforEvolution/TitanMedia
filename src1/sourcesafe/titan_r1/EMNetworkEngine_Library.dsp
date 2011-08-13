# Microsoft Developer Studio Project File - Name="EMNetworkEngine_Library" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=EMNetworkEngine_Library - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EMNetworkEngine_Library.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EMNetworkEngine_Library.mak" CFG="EMNetworkEngine_Library - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EMNetworkEngine_Library - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EMNetworkEngine_Library - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/EMNetworkEngine_Library", UQHAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EMNetworkEngine_Library - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMNETWORKENGINE_LIBRARY_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "__EM_NETWORK_ENGINE_LIBRARY_IS_BUILDING" /D "SONORK_CLIENT_BUILD" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMNETWORKENGINE_LIBRARY_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "NDEBUG"
# ADD RSC /l 0x41d /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 ws2_32.lib .\lib\srkcryptlibvccmt.lib .\lib\EMCommand_Library.lib .\lib\EMCommon_Library.lib .\lib\EMDebug_Library.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"\SourceSafe\ReleaseBuild\EMNetworkEngine_Library.dll" /implib:"\SourceSafe\ReleaseBuild\lib\EMNetworkEngine_Library.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "EMNetworkEngine_Library - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Network_Debug"
# PROP Intermediate_Dir "Network_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMNETWORKENGINE_LIBRARY_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EMNETWORKENGINE_LIBRARY_EXPORTS" /D "SONORK_CLIENT_BUILD" /D "__EM_NETWORK_ENGINE_LIBRARY_IS_BUILDING" /D "DEBUG_VERSION" /FR"EMNetworkEngine_Library_files/" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib .\lib\srkcryptlibvccmt.lib .\lib\EMCommand_Library.lib .\lib\EMCommon_Library.lib .\lib\EMDebug_Library.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /pdb:none /debug /machine:I386 /out:"EMNetworkEngine_Library.dll" /implib:"./lib/EMNetworkEngine_Library.lib"

!ENDIF 

# Begin Target

# Name "EMNetworkEngine_Library - Win32 Release"
# Name "EMNetworkEngine_Library - Win32 Debug"
# Begin Group "Framework"

# PROP Default_Filter ""
# Begin Group "Framework / Network"

# PROP Default_Filter ""
# Begin Group "Framework / Network / Sonork"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Framework\Network\Sonork\sonorkvcapi.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\sonorkvcapi.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_atom_db.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_atom_db.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_bf_file.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_bf_file.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_cfg_names.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_cfg_names.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_client.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_client.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_client_defs.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_codec_atom.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_codec_atom.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_codec_file.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_codec_file.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_codec_io.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_codec_io.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_crypt.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_crypt_context.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_crypt_context.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_data_lists.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_data_lists.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_data_packet.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_data_packet.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_data_types.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_data_types.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_defs.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_defs.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_dragdrop.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_dragdrop.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_email_codec.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_email_codec.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_event_handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_event_handler.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_ext_user_data.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_ext_user_data.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_file_io.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_file_io.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_language.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_language.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_netio.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_netio.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_pairvalue.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_pairvalue.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_referrer_id.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_referrer_id.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_services.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_services.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_stream_lr.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_stream_lr.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_string_loader.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_string_loader.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_svr_login_packet.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_svr_login_packet.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_sys_string.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_sys_string.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_task_atom.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_task_atom.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_tcpio.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_tcpio.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_udpio.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_udpio.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_url_codec.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_url_codec.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_winregkey.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\Sonork\srk_winregkey.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Framework\Network\EMItemContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\EMItemContainer.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\EMNetworkFileHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\EMNetworkFileHandler.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\EMNetworkFileInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\EMNetworkFileInfo.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\EMNetworkMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\EMNetworkMessage.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\EMNetworkMessageQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\EMNetworkMessageQueue.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\EMNetworkMessageSender.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\EMNetworkMessageSender.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\EMNetworkRecvFileInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\EMNetworkRecvFileInfo.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\EMNetworkSearchItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\EMNetworkSearchItem.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\EMNetworkSendFileInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\EMNetworkSendFileInfo.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\EMNetworkTask.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\EMNetworkUserDatabase.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\EMNetworkUserDatabase.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\EMNetworkUserHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\EMNetworkUserHandler.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\EMUserInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Framework\Network\EMUserInfo.h
# End Source File
# End Group
# Begin Group "Framework / Globals"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Framework\Globals\EMGlobals.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Globals\EMMessages.h
# End Source File
# Begin Source File

SOURCE=.\Framework\Globals\EMNetworkGlobals.h
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

SOURCE=.\Application\Commands\NetworkCommandAddUser.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\NetworkCommandAddUser.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\NetworkCommandDeleteUser.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\NetworkCommandDeleteUser.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\NetworkCommandFindUser.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\NetworkCommandFindUser.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\NetworkCommandReceiveFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\NetworkCommandReceiveFile.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\NetworkCommandSendFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\NetworkCommandSendFile.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\NetworkCommandSendMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\NetworkCommandSendMessage.h
# End Source File
# End Group
# Begin Group "Info commands"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Application\Commands\NetworkInfoCommandGetNextDownloadedFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\NetworkInfoCommandGetNextDownloadedFile.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\NetworkInfoCommandGetNextFileInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\NetworkInfoCommandGetNextFileInfo.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\NetworkInfoCommandGetNextFoundUser.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\NetworkInfoCommandGetNextFoundUser.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\NetworkInfoCommandGetNextMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\NetworkInfoCommandGetNextMessage.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\NetworkInfoCommandGetNextUserStatusMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\NetworkInfoCommandGetNextUserStatusMessage.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\NetworkInfoCommandGetNumberOfUsers.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\NetworkInfoCommandGetNumberOfUsers.h
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\NetworkInfoCommandGetUserInfos.cpp
# End Source File
# Begin Source File

SOURCE=.\Application\Commands\NetworkInfoCommandGetUserInfos.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "Components"

# PROP Default_Filter ""
# Begin Group "Components / Network"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Components\Network\EMNetworkCommandFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\Network\EMNetworkCommandFactory.h
# End Source File
# Begin Source File

SOURCE=.\Components\Network\EMNetworkEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\Components\Network\EMNetworkEngine.h
# End Source File
# End Group
# End Group
# Begin Group "lib"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=.\EMNetworkEngine_Library.dll
# End Source File
# End Target
# End Project
