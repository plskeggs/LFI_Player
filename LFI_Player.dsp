# Microsoft Developer Studio Project File - Name="LFI_Player" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=LFI_Player - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LFI_Player.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LFI_Player.mak" CFG="LFI_Player - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LFI_Player - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "LFI_Player - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LFI_Player - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib serial.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"LIBC.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "LFI_Player - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib serial.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "LFI_Player - Win32 Release"
# Name "LFI_Player - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Abstract.cpp
# End Source File
# Begin Source File

SOURCE=.\AbstractControl.cpp
# End Source File
# Begin Source File

SOURCE=.\Cheap3D.cpp
# End Source File
# Begin Source File

SOURCE=.\Cheap3DDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\Cheap3DView.cpp
# End Source File
# Begin Source File

SOURCE=.\ColoredStatusBarCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CommandLine.cpp
# End Source File
# Begin Source File

SOURCE=.\Device_BSOFT_RGB.cpp
# End Source File
# Begin Source File

SOURCE=.\Device_BSOFT_XYI.cpp
# End Source File
# Begin Source File

SOURCE=.\Device_EasyLase.cpp
# End Source File
# Begin Source File

SOURCE=.\Device_EzAudDac.cpp
# End Source File
# Begin Source File

SOURCE=.\Device_GWS.cpp
# End Source File
# Begin Source File

SOURCE=.\Device_None.cpp
# End Source File
# Begin Source File

SOURCE=.\Device_RIYA.cpp
# End Source File
# Begin Source File

SOURCE=.\FontList.cpp
# End Source File
# Begin Source File

SOURCE=.\Frame.cpp
# End Source File
# Begin Source File

SOURCE=.\FrameSeq.cpp
# End Source File
# Begin Source File

SOURCE=.\Gem_pc.cpp
# End Source File
# Begin Source File

SOURCE=.\JobBlock.cpp
# End Source File
# Begin Source File

SOURCE=.\LFI_Player.rc
# End Source File
# Begin Source File

SOURCE=.\LiveControl.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MidiBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\MidiIn.cpp
# End Source File
# Begin Source File

SOURCE=.\PageComSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\Point.cpp
# End Source File
# Begin Source File

SOURCE=.\PointBuff.cpp
# End Source File
# Begin Source File

SOURCE=.\Preferences.cpp
# End Source File
# Begin Source File

SOURCE=.\PrefProppageBeamView.cpp
# End Source File
# Begin Source File

SOURCE=.\PrefProppageColor.cpp
# End Source File
# Begin Source File

SOURCE=.\PrefProppageDisplayTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\PrefProppageIldaRecording.cpp
# End Source File
# Begin Source File

SOURCE=.\PrefProppageLiveControl.cpp
# End Source File
# Begin Source File

SOURCE=.\PrefProppageOutput.cpp
# End Source File
# Begin Source File

SOURCE=.\RichCommEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\Routines.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectPort.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TextScroll.cpp
# End Source File
# Begin Source File

SOURCE=.\Track.cpp
# End Source File
# Begin Source File

SOURCE=.\TrackLst.cpp
# End Source File
# Begin Source File

SOURCE=.\View.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Abstract.h
# End Source File
# Begin Source File

SOURCE=.\AbstractControl.h
# End Source File
# Begin Source File

SOURCE=.\Cheap3D.h
# End Source File
# Begin Source File

SOURCE=.\Cheap3DDoc.h
# End Source File
# Begin Source File

SOURCE=.\Cheap3DView.h
# End Source File
# Begin Source File

SOURCE=.\ColoredStatusBarCtrl.h
# End Source File
# Begin Source File

SOURCE=.\CommandLine.h
# End Source File
# Begin Source File

SOURCE=.\Compile_Switch.h
# End Source File
# Begin Source File

SOURCE=.\Device_BSOFT_RGB.h
# End Source File
# Begin Source File

SOURCE=.\Device_BSOFT_XYI.h
# End Source File
# Begin Source File

SOURCE=.\Device_EasyLase.h
# End Source File
# Begin Source File

SOURCE=.\Device_EzAudDac.h
# End Source File
# Begin Source File

SOURCE=.\Device_GWS.h
# End Source File
# Begin Source File

SOURCE=.\Device_None.h
# End Source File
# Begin Source File

SOURCE=.\Device_RIYA.h
# End Source File
# Begin Source File

SOURCE=.\FontList.h
# End Source File
# Begin Source File

SOURCE=.\Frame.h
# End Source File
# Begin Source File

SOURCE=.\FrameSeq.h
# End Source File
# Begin Source File

SOURCE=.\Gem_pc.h
# End Source File
# Begin Source File

SOURCE=.\Globals.h
# End Source File
# Begin Source File

SOURCE=.\JobBlock.h
# End Source File
# Begin Source File

SOURCE=.\LiveControl.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MidiBuffer.h
# End Source File
# Begin Source File

SOURCE=.\MidiIn.h
# End Source File
# Begin Source File

SOURCE=.\PageComSettings.h
# End Source File
# Begin Source File

SOURCE=.\Palette.h
# End Source File
# Begin Source File

SOURCE=.\Point.h
# End Source File
# Begin Source File

SOURCE=.\PointBuff.h
# End Source File
# Begin Source File

SOURCE=.\Preferences.h
# End Source File
# Begin Source File

SOURCE=.\PrefProppageBeamView.h
# End Source File
# Begin Source File

SOURCE=.\PrefProppageColor.h
# End Source File
# Begin Source File

SOURCE=.\PrefProppageDisplayTimer.h
# End Source File
# Begin Source File

SOURCE=.\PrefProppageIldaRecording.h
# End Source File
# Begin Source File

SOURCE=.\PrefProppageLiveControl.h
# End Source File
# Begin Source File

SOURCE=.\PrefProppageOutput.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RichCommEdit.h
# End Source File
# Begin Source File

SOURCE=.\Routines.h
# End Source File
# Begin Source File

SOURCE=.\SelectPort.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TextScroll.h
# End Source File
# Begin Source File

SOURCE=.\Track.h
# End Source File
# Begin Source File

SOURCE=.\TrackLst.h
# End Source File
# Begin Source File

SOURCE=.\View.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\LazerusDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\LFI_Player.ico
# End Source File
# Begin Source File

SOURCE=.\res\LFI_Player.rc2
# End Source File
# Begin Source File

SOURCE=.\RES\LFI_PlayerDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
