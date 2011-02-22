@ECHO off

:: ======== VC++ 8 ======== 
SET PSDK=C:\Program Files\Microsoft Platform SDK for Windows Server 2003 R2
CALL "C:\Program Files\Microsoft Visual Studio 8\VC\bin\vcvars32.bat"

:: ======== PYTHON PATHS ======== 
SET PYTHON_ROOT=c:\Python25
SET PATH=%PYTHON_ROOT%;%PATH%
SET PYTHONPATH=%SOLEIL_ROOT%\bindings\python\python25;%PYTHONPATH%

:: ======== MAVEN ======== 
SET M2_ROOT=C:\Documents and Settings\ncl\.m2\repository

:: ======== OMNIORB ======== 
SET OMNIORB_ROOT=%M2_ROOT%\org\omnigroup\omniORB-x86-Windows-msvc-shared-release\4.1.4\nar
SET OMNIORB_INC=%OMNIORB_ROOT%\include\x86-Windows-msvc
SET OMNIORB_LIB=%OMNIORB_ROOT%\lib\x86-Windows-msvc\shared
::------------------------------------------------------------------------
SET OMNIORB_LIB_LIST=omnithread34_rt.lib omniORB414_rt.lib COS414_rt.lib omniDynamic414_rt.lib
SET PATH=%OMNIORB_LIB%;%PATH%

:: ======== TANGO ======== 
SET TANGO_ROOT=%M2_ROOT%\org\tango\tango-cpp-api-x86-Windows-msvc-shared-release\7.2.1-1\nar
SET TANGO_INC=%TANGO_ROOT%\include\x86-Windows-msvc
SET TANGO_LIB=%TANGO_ROOT%\lib\x86-Windows-msvc\shared
SET TANGO_LIB_LIST=tango721.lib log4tango.lib
SET PATH=%TANGO_LIB%;%PATH%

:: ======== YAT PATHS ======== 
set YAT_ROOT=%M2_ROOT%\fr\soleil\lib\YAT-x86-Windows-msvc-static-release\1.5.4\nar
set YAT_INC=%YAT_ROOT%\include
set YAT_LIB=%YAT_ROOT%\lib\x86-Windows-msvc\static
set YAT_LIB_LIST=YAT-x86-Windows-msvc-static-release-1.5.4.lib

:: ======== YAT FOR TANGO PATHS ======== 
set YAT_FOR_TANGO_ROOT=%M2_ROOT%\fr\soleil\lib\YAT4Tango-x86-Windows-msvc-static-release\1.4.8\nar
set YAT_FOR_TANGO_INC=%YAT_FOR_TANGO_ROOT%\include
set YAT_FOR_TANGO_LIB=%YAT_FOR_TANGO_ROOT%\lib\x86-Windows-msvc\static
set YAT_FOR_TANGO_LIB_LIST=YAT4Tango-x86-Windows-msvc-static-release-1.4.8.lib

:: ======== IGOR PRO XOP
set XOP_ROOT=C:\Soleil\bindings\igorpro\xopsupport-v6
set XOP_INC=%XOP_ROOT%
set XOP_RC=%XOP_INC%
set XOP_LIB=%XOP_ROOT%
set XOP_LIB_LIST=igor.lib xopsupport_x86.lib