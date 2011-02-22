@ECHO off



:: ======== SOLEIL ROOT

set SOLEIL_ROOT=c:\soleil-win32-4.3.0.e



:: ======== NI-LabVIEW PATHS

set LV_ROOT=C:\Program Files\National Instruments\LabVIEW 7.1

set LV_INC=%LV_ROOT%\cintools

set LV_LIB=%LV_ROOT%\cintools



:: ======== TANGO PATHS

set TANGO_ROOT=%SOLEIL_ROOT%\tango

set TANGO_INC=%TANGO_ROOT%\include

set TANGO_LIB=%TANGO_ROOT%\lib

set TANGO_BIN=%TANGO_ROOT%\bin

set PATH=%TANGO_BIN%;%PATH%



:: ======== OMNIORB PATHS

set OMNIORB_ROOT=%SOLEIL_ROOT%\omniorb

set OMNIORB_INC=%OMNIORB_ROOT%\include

set OMNIORB_LIB=%OMNIORB_ROOT%\lib\x86_win32

set OMNIORB_BIN=%OMNIORB_ROOT%\bin\x86_win32

set PATH=%OMNIORB_BIN%;%PATH%



echo "env. setup done"



.\TangoBindingForLabView.dsw

