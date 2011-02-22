# Microsoft Developer Studio Project File - Name="TangoBindingForMatlab" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=TangoBindingForMatlab - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TangoBindingForMatlab.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TangoBindingForMatlab.mak" CFG="TangoBindingForMatlab - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TangoBindingForMatlab - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TangoBindingForMatlab - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TangoBindingForMatlab - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TangoBindingForMatlab___Win32_Debug"
# PROP BASE Intermediate_Dir "TangoBindingForMatlab___Win32_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "objs/debug"
# PROP Intermediate_Dir "objs/debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "$(OMNIORB_INC)" /I "$(TANGO_ROOT)\tango\win32\include" /D "_DEBUG" /D "_MATLAB_r13_" /D "MATLAB_MEX_FILE" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "IBMPC" /D "MSVC" /D "MSWIND" /D "__STDC__" /D "LOG4TANGO_HAS_DLL" /D "Use_stub_in_nt_dll" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "." /I "$(OMNIORB_INC)" /I "$(TANGO_INC)" /I "$(MATLAB_INC)" /D "_DEBUG" /D "_HAS_NO_MWSIZE_TYPE_" /D "_MATLAB_R14_" /D "MATLAB_MEX_FILE" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "IBMPC" /D "MSVC" /D "MSWIND" /D "__STDC__" /D "LOG4TANGO_HAS_DLL" /D "TANGO_HAS_DLL" /D "HAS_TANGO_5_OR_SUP" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /i "C:/MATLAB7/extern/include" /d "_DEBUG"
# SUBTRACT RSC /x
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 omniORB401_rtd.lib omniDynamic401_rtd.lib omnithread30_rtd.lib tangod.lib log4tangod.lib comctl32.lib version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libmex.lib libmx.lib ws2_32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../mex-file/win32/r13/msvc6/debug/tango_binding.dll" /pdbtype:sept /libpath:"$(OMNIORB_LIB)" /libpath:"$(TANGO_ROOT)\tango\win32\lib\msvc6\shared\debug" /EXPORT:mexFunction
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 omniORB407_rtd.lib omniDynamic407_rtd.lib COS407_rtd.lib omnithread32_rtd.lib tangod.lib log4tangod.lib comctl32.lib version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libmex.lib libmx.lib ws2_32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../mex-file/tango_binding.dll" /pdbtype:sept /libpath:"$(OMNIORB_LIB)" /libpath:"$(TANGO_LIB)\shared" /libpath:"$(MATLAB_LIB)" /EXPORT:mexFunction
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "TangoBindingForMatlab - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "TangoBindingForMatlab___Win32_Release"
# PROP BASE Intermediate_Dir "TangoBindingForMatlab___Win32_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "objs/release"
# PROP Intermediate_Dir "objs/release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /I "$(OMNIORB_INC)" /I "$(TANGO_ROOT)\tango\win32\include" /D "NDEBUG" /D "_MATLAB_r13_" /D "MATLAB_MEX_FILE" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "IBMPC" /D "MSVC" /D "MSWIND" /D "__STDC__" /D "LOG4TANGO_HAS_DLL" /D "Use_stub_in_nt_dll" /FR /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "." /I "$(OMNIORB_INC)" /I "$(TANGO_INC)" /I "$(MATLAB_INC)" /D "NDEBUG" /D "_HAS_NO_MWSIZE_TYPE_" /D "_MATLAB_R14_" /D "MATLAB_MEX_FILE" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "IBMPC" /D "MSVC" /D "MSWIND" /D "__STDC__" /D "LOG4TANGO_HAS_DLL" /D "TANGO_HAS_DLL" /D "HAS_TANGO_5_OR_SUP" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /i "C:/MATLAB7/extern/include" /d "NDEBUG"
# SUBTRACT RSC /x
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 omniORB401_rt.lib omniDynamic401_rt.lib omnithread30_rt.lib tango.lib log4tango.lib comctl32.lib version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libmex.lib libmx.lib ws2_32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../mex-file/win32/r13/msvc6/release/tango_binding.dll" /libpath:"$(OMNIORB_LIB)" /libpath:"$(TANGO_ROOT)\tango\win32\lib\msvc6\shared\release" /EXPORT:mexFunction
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 omniORB407_rt.lib omniDynamic407_rt.lib COS407_rt.lib omnithread32_rt.lib tango.lib log4tango.lib comctl32.lib version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libmex.lib libmx.lib ws2_32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../mex-file/tango_binding.dll" /libpath:"$(OMNIORB_LIB)" /libpath:"$(TANGO_LIB)\shared" /libpath:"$(MATLAB_LIB)" /EXPORT:mexFunction
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "TangoBindingForMatlab - Win32 Debug"
# Name "TangoBindingForMatlab - Win32 Release"
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\DataAdapter.h
# End Source File
# Begin Source File

SOURCE=..\DevRepository.h
# End Source File
# Begin Source File

SOURCE=..\GroupRepository.h
# End Source File
# Begin Source File

SOURCE=..\MexArgs.h
# End Source File
# Begin Source File

SOURCE=..\MexFile.h
# End Source File
# Begin Source File

SOURCE=..\MexUtils.h
# End Source File
# Begin Source File

SOURCE=..\StandardHeader.h
# End Source File
# Begin Source File

SOURCE=..\TangoBinding.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\DataAdapter.cpp
# End Source File
# Begin Source File

SOURCE=..\DevRepository.cpp
# End Source File
# Begin Source File

SOURCE=..\GroupRepository.cpp
# End Source File
# Begin Source File

SOURCE=..\main.cpp
# End Source File
# Begin Source File

SOURCE=..\MexArgs.cpp
# End Source File
# Begin Source File

SOURCE=..\MexFile.cpp
# End Source File
# Begin Source File

SOURCE=..\MexUtils.cpp
# End Source File
# Begin Source File

SOURCE=..\TangoBinding.cpp
# End Source File
# End Group
# Begin Group "Inline Files"

# PROP Default_Filter "i"
# Begin Source File

SOURCE=..\DataAdapter.i
# End Source File
# Begin Source File

SOURCE=..\DevRepository.i
# End Source File
# Begin Source File

SOURCE=..\GroupRepository.i
# End Source File
# Begin Source File

SOURCE=..\MexArgs.i
# End Source File
# Begin Source File

SOURCE=..\MexUtils.i
# End Source File
# Begin Source File

SOURCE=..\TangoBinding.i
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\TangoBinding.rc
# ADD BASE RSC /l 0x40c /i "\tango-bindings\matlab" /i "\dev\tango-cvs\cvs-sourceforge\matlab_binding" /i "\dev\matlab_binding"
# SUBTRACT BASE RSC /i "C:/MATLAB7/extern/include"
# ADD RSC /l 0x40c /i "\tango-bindings\matlab" /i "\dev\tango-cvs\cvs-sourceforge\matlab_binding" /i "$(MATLAB_RC)"
# SUBTRACT RSC /i "C:/MATLAB7/extern/include"
# End Source File
# End Group
# End Target
# End Project
