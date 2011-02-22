ECHO OFF

TANGO_BINDING_ROOT=s:\bindings\igorpro

::HELP IGOR TO FIND THE REQUIRED DLLs
PATH=%TANGO_BINDING_ROOT%\xop;%PATH%

START /D "C:\Program Files\WaveMetrics\Igor Pro Folder" Igor.exe

