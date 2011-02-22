@ECHO off

:: ======== TANGO HOST 

set TANGO_HOST=localhost:20000

:: ======== INSTALL_ROOT
set INSTALL_ROOT=%SOLEIL_ROOT%

:: ======== NI-LabVIEW 7 PATHS

set LV7_ROOT=C:\Program Files\National Instruments\LabVIEW 7.1

set LV7_INC=%LV7_ROOT%\cintools

set LV7_LIB=%LV7_ROOT%\cintools

:: ======== NI-LabVIEW 8 PATHS

set LV8_ROOT=C:\Program Files\National Instruments\LabVIEW 8.2

set LV8_INC=%LV8_ROOT%\cintools

set LV8_LIB=%LV8_ROOT%\cintools

:: ======== TANGO PATHS

set TANGO_ROOT=%INSTALL_ROOT%\tango

set TANGO_INC=%TANGO_ROOT%\include

set TANGO_LIB=%TANGO_ROOT%\lib

set TANGO_BIN=%TANGO_ROOT%\bin

set PATH=%TANGO_BIN%;%PATH%

:: ======== OMNIORB PATHS

set OMNIORB_ROOT=%INSTALL_ROOT%\omniorb

set OMNIORB_INC=%OMNIORB_ROOT%\include

set OMNIORB_LIB=%OMNIORB_ROOT%\lib\x86_win32

set OMNIORB_BIN=%OMNIORB_ROOT%\bin\x86_win32

set PATH=%OMNIORB_BIN%;%PATH%


echo "env. setup done"

start /B .\TangoBindingForLabView.dsw

