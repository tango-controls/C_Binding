# Microsoft Developer Studio Project File - Name="TangoBindingForIgorPro" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=TangoBindingForIgorPro - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TangoBindingForIgorPro.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TangoBindingForIgorPro.mak" CFG="TangoBindingForIgorPro - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TangoBindingForIgorPro - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TangoBindingForIgorPro - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TangoBindingForIgorPro - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TangoBindingForIgorPro_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "." /I "..\igorxops" /I "$(OMNIORB_INC)" /I "$(TANGO_INC)" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "TangoBindingForIgorPro_EXPORTS" /Fr /FD /c
# SUBTRACT CPP /X /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x409 /i "..\igorxops" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 igor.lib XOPSupport_x86.lib omniORB403_rt.lib omniDynamic403_rt.lib COS403_rt.lib omnithread30_rt.lib tango.lib log4tango.lib kernel32.lib gdi32.lib comctl32.lib user32.lib advapi32.lib ws2_32.lib version.lib /nologo /dll /machine:I386 /out:"tango_binding.xop" /libpath:"..\igorxops" /libpath:"." /libpath:"$(OMNIORB_LIB)" /libpath:"$(TANGO_LIB)/mix"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "TangoBindingForIgorPro - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TangoBindingForIgorPro_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "." /I "..\igorxops" /I "$(OMNIORB_INC)" /I "$(TANGO_INC)" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "TangoBindingForIgorPro_EXPORTS" /Fr /FD /GZ /c
# SUBTRACT CPP /X /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x409 /i "..\igorxops" /d "_DEBUG"
# SUBTRACT RSC /x
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 igor.lib XOPSupport_x86d.lib omniORB403_rtd.lib omniDynamic403_rtd.lib COS403_rtd.lib omnithread30_rtd.lib tangod.lib log4tangod.lib kernel32.lib gdi32.lib comctl32.lib user32.lib advapi32.lib ws2_32.lib version.lib /nologo /dll /debug /machine:I386 /out:"tango_binding.xop" /pdbtype:sept /libpath:"..\igorxops" /libpath:"." /libpath:"$(OMNIORB_LIB)" /libpath:"$(TANGO_LIB)/mix"
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "TangoBindingForIgorPro - Win32 Release"
# Name "TangoBindingForIgorPro - Win32 Debug"
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DevRepository.h
# End Source File
# Begin Source File

SOURCE=.\IgorDevData.h
# End Source File
# Begin Source File

SOURCE=.\TangoBinding.h
# End Source File
# Begin Source File

SOURCE=.\TangoBindingForIgorPro.h
# End Source File
# Begin Source File

SOURCE=.\XDK_Configuration.h
# End Source File
# Begin Source File

SOURCE=.\XDK_Constants.h
# End Source File
# Begin Source File

SOURCE=.\XDK_Functions.h
# End Source File
# Begin Source File

SOURCE=.\XDK_Mld.h
# End Source File
# Begin Source File

SOURCE=.\XDK_Operations.h
# End Source File
# Begin Source File

SOURCE=.\XDK_StandardHeaders.h
# End Source File
# Begin Source File

SOURCE=.\XDK_Utils.h
# End Source File
# Begin Source File

SOURCE=.\XDK_Utils_Win32.h
# End Source File
# Begin Source File

SOURCE=.\XDK_Xop.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DevRepository.cpp
# End Source File
# Begin Source File

SOURCE=.\IgorDevData.cpp
# End Source File
# Begin Source File

SOURCE=.\TangoBinding.cpp
# End Source File
# Begin Source File

SOURCE=.\TangoClientXop.cpp
# End Source File
# Begin Source File

SOURCE=.\XDK_Functions.cpp
# End Source File
# Begin Source File

SOURCE=.\XDK_Main.cpp
# End Source File
# Begin Source File

SOURCE=.\XDK_Mld.cpp
# End Source File
# Begin Source File

SOURCE=.\XDK_Operations.cpp
# End Source File
# Begin Source File

SOURCE=.\XDK_Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\XDK_Utils_Win32.cpp
# End Source File
# Begin Source File

SOURCE=.\XDK_Xop.cpp
# End Source File
# End Group
# Begin Group "Inline files"

# PROP Default_Filter ".i"
# Begin Source File

SOURCE=.\DevRepository.i
# End Source File
# Begin Source File

SOURCE=.\IgorDevData.i
# End Source File
# Begin Source File

SOURCE=.\TangoBinding.i
# End Source File
# Begin Source File

SOURCE=.\XDK_Mld.i
# End Source File
# Begin Source File

SOURCE=.\XDK_Utils.i
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\XDK_ResourceDefs.h
# End Source File
# Begin Source File

SOURCE=.\XDK_Resources.rc
# End Source File
# Begin Source File

SOURCE=.\XDK_Specific_Resources.r
# End Source File
# Begin Source File

SOURCE=".\XDK_STR#_1100_Win32.r"
# End Source File
# Begin Source File

SOURCE=.\XDK_XOPC_1100_Win32.r
# End Source File
# Begin Source File

SOURCE=.\XDK_XOPF_1100_Win32.r
# End Source File
# End Group
# Begin Group "TangoAPI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TangoApi\DevData.cpp
# End Source File
# Begin Source File

SOURCE=.\TangoApi\DevProxy.cpp
# End Source File
# Begin Source File

SOURCE=.\TangoApi\TangoAPI.h
# End Source File
# End Group
# End Target
# End Project
