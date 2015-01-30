# Microsoft Developer Studio Project File - Name="smpeg" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=smpeg - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "smpeg.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "smpeg.mak" CFG="smpeg - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "smpeg - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "smpeg - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "smpeg - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "NOCONTROLS" /D "THREADED_AUDIO" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib SDL.lib /nologo /subsystem:windows /dll /machine:I386

!ELSEIF  "$(CFG)" == "smpeg - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "..\\" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "NOCONTROLS" /D "THREADED_AUDIO" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 wsock32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib SDL.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "smpeg - Win32 Release"
# Name "smpeg - Win32 Debug"
# Begin Source File

SOURCE=..\audio\bitwindow.cpp
# End Source File
# Begin Source File

SOURCE=..\video\decoders.cpp
# End Source File
# Begin Source File

SOURCE=..\video\decoders.h
# End Source File
# Begin Source File

SOURCE=..\video\dither.h
# End Source File
# Begin Source File

SOURCE=..\audio\filter.cpp
# End Source File
# Begin Source File

SOURCE=..\audio\filter_2.cpp
# End Source File
# Begin Source File

SOURCE=..\video\floatdct.cpp
# End Source File
# Begin Source File

SOURCE=..\video\gdith.cpp
# End Source File
# Begin Source File

SOURCE=..\audio\hufftable.cpp
# End Source File
# Begin Source File

SOURCE=..\video\jrevdct.cpp
# End Source File
# Begin Source File

SOURCE=..\video\motionvec.cpp
# End Source File
# Begin Source File

SOURCE=..\MPEG.cpp
# End Source File
# Begin Source File

SOURCE=..\MPEG.h
# End Source File
# Begin Source File

SOURCE=..\MPEGaction.h
# End Source File
# Begin Source File

SOURCE=..\audio\MPEGaudio.cpp
# End Source File
# Begin Source File

SOURCE=..\MPEGaudio.h
# End Source File
# Begin Source File

SOURCE=..\MPEGerror.h
# End Source File
# Begin Source File

SOURCE=..\MPEGfilter.c
# End Source File
# Begin Source File

SOURCE=..\MPEGfilter.h
# End Source File
# Begin Source File

SOURCE=..\audio\mpeglayer1.cpp
# End Source File
# Begin Source File

SOURCE=..\audio\mpeglayer2.cpp
# End Source File
# Begin Source File

SOURCE=..\audio\mpeglayer3.cpp
# End Source File
# Begin Source File

SOURCE=..\MPEGlist.cpp
# End Source File
# Begin Source File

SOURCE=..\MPEGlist.h
# End Source File
# Begin Source File

SOURCE=..\MPEGring.cpp
# End Source File
# Begin Source File

SOURCE=..\MPEGring.h
# End Source File
# Begin Source File

SOURCE=..\MPEGstream.cpp
# End Source File
# Begin Source File

SOURCE=..\MPEGstream.h
# End Source File
# Begin Source File

SOURCE=..\MPEGsystem.cpp
# End Source File
# Begin Source File

SOURCE=..\MPEGsystem.h
# End Source File
# Begin Source File

SOURCE=..\audio\mpegtable.cpp
# End Source File
# Begin Source File

SOURCE=..\audio\mpegtoraw.cpp
# End Source File
# Begin Source File

SOURCE=..\video\MPEGvideo.cpp
# End Source File
# Begin Source File

SOURCE=..\MPEGvideo.h
# End Source File
# Begin Source File

SOURCE=..\video\parseblock.cpp
# End Source File
# Begin Source File

SOURCE=..\video\proto.h
# End Source File
# Begin Source File

SOURCE=..\video\readfile.cpp
# End Source File
# Begin Source File

SOURCE=..\smpeg.cpp
# End Source File
# Begin Source File

SOURCE=..\smpeg.h
# End Source File
# Begin Source File

SOURCE=..\video\util.cpp
# End Source File
# Begin Source File

SOURCE=..\video\util.h
# End Source File
# Begin Source File

SOURCE=..\video\vhar128.cpp
# End Source File
# Begin Source File

SOURCE=..\video\vhar128.h
# End Source File
# Begin Source File

SOURCE=..\video\video.cpp
# End Source File
# Begin Source File

SOURCE=..\video\video.h
# End Source File
# End Target
# End Project
