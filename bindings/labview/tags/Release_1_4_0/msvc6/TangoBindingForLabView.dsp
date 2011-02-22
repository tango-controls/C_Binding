# Microsoft Developer Studio Project File - Name="TangoBindingForLabView" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=TangoBindingForLabView - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TangoBindingForLabView.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TangoBindingForLabView.mak" CFG="TangoBindingForLabView - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TangoBindingForLabView - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TangoBindingForLabView - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TangoBindingForLabView - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TANGOBINDINGFORLABVIEW_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "..\src" /I "$(OMNIORB_INC)" /I "$(TANGO_INC)" /I "$(LV_INC)" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TANGO_HAS_DLL" /D "LOG4TANGO_HAS_DLL" /D "HAS_TANGO_5_OR_SUP" /FR /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 labview.lib omniORB405_rt.lib omniDynamic405_rt.lib omnithread30_rt.lib COS405_rt.lib tango.lib log4tango.lib kernel32.lib gdi32.lib comctl32.lib user32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /dll /machine:I386 /out:"../tango/tango_binding.dll" /libpath:"$(LV_LIB)" /libpath:"$(OMNIORB_LIB)" /libpath:"$(TANGO_LIB)\shared" /IGNORE:4089 /IGNORE:4049
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "TangoBindingForLabView - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TANGOBINDINGFORLABVIEW_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\src" /I "$(OMNIORB_INC)" /I "$(TANGO_INC)" /I "$(LV_INC)" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TANGO_HAS_DLL" /D "LOG4TANGO_HAS_DLL" /D "HAS_TANGO_5_OR_SUP" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 labview.lib omniORB405_rtd.lib omniDynamic405_rtd.lib omnithread30_rtd.lib COS405_rtd.lib tangod.lib log4tangod.lib kernel32.lib gdi32.lib comctl32.lib user32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /dll /debug /machine:I386 /out:"../tango/tango_binding.dll" /pdbtype:sept /libpath:"$(LV_LIB)" /libpath:"$(OMNIORB_LIB)" /libpath:"$(TANGO_LIB)\shared" /IGNORE:4049
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "TangoBindingForLabView - Win32 Release"
# Name "TangoBindingForLabView - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\DataAdapter.cpp
# End Source File
# Begin Source File

SOURCE=..\src\DeviceRepository.cpp
# End Source File
# Begin Source File

SOURCE=..\src\Endianness.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ErrorStack.cpp
# End Source File
# Begin Source File

SOURCE=..\src\EventManager.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ObjectManager.cpp
# End Source File
# Begin Source File

SOURCE=..\src\TangoBinding.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ThreadSafeDeviceProxy.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\src\DataAdapter.h
# End Source File
# Begin Source File

SOURCE=..\src\DeviceRepository.h
# End Source File
# Begin Source File

SOURCE=..\src\Endianness.h
# End Source File
# Begin Source File

SOURCE=..\src\ErrorStack.h
# End Source File
# Begin Source File

SOURCE=..\src\EventManager.h
# End Source File
# Begin Source File

SOURCE=..\src\LvTypes.h
# End Source File
# Begin Source File

SOURCE=..\src\ObjectManager.h
# End Source File
# Begin Source File

SOURCE=..\src\StandardHeader.h
# End Source File
# Begin Source File

SOURCE=..\src\TargetPlatform.h
# End Source File
# Begin Source File

SOURCE=..\src\ThreadSafeDeviceProxy.h
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter ".i"
# Begin Source File

SOURCE=..\src\DataAdapter.i
# End Source File
# Begin Source File

SOURCE=..\src\DeviceRepository.i
# End Source File
# Begin Source File

SOURCE=..\src\Endianness.i
# End Source File
# Begin Source File

SOURCE=..\src\ErrorStack.i
# End Source File
# Begin Source File

SOURCE=..\src\EventManager.i
# End Source File
# Begin Source File

SOURCE=..\src\ThreadSafeDeviceProxy.i
# End Source File
# End Group
# End Target
# End Project
