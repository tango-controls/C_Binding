@ECHO off

:: ======== TANGO HOST 
set TANGO_HOST=localhost:20000

:: ======== INSTALL_ROOT
set INSTALL_ROOT=%SOLEIL_ROOT%

:: ======== NI-LabVIEW 8 PATHS
set LV8_ROOT=C:\Program Files\National Instruments\LabVIEW 8.6
set LV8_INC=%LV8_ROOT%\cintools
set LV8_LIB=%LV8_ROOT%\cintools

:: ======== TANGO PATHS
set TANGO_INC=%SOLEIL_ROOT%\sw-support\tango\include
set TANGO_LIB=%SOLEIL_ROOT%\sw-support\tango\lib\shared
set TANGO_BIN=%SOLEIL_ROOT%\sw-support\tango\bin
set PATH=%TANGO_BIN%;%PATH%

:: ======== OMNIORB PATHS
set OMNIORB_INC=%SOLEIL_ROOT%\sw-support\omniorb\include
set OMNIORB_LIB=%SOLEIL_ROOT%\sw-support\omniorb\lib
set OMNIORB_BIN=%SOLEIL_ROOT%\sw-support\omniorb\bin
set PATH=%OMNIORB_BIN%;%PATH%

echo "env. setup done"

start /B .\TangoBindingForLabView.sln

