ECHO OFF

TANGO_BINDING_ROOT=<path-to-your-tango-binding-root-directory>

::HELP IGOR TO FIND THE REQUIRED DLLs
PATH=%TANGO_BINDING_ROOT%\xop;%PATH%

::COULD ALSO CALL YOUR OWN ENV SETUP SCRIPT E.G.:
::CALL %SOLEIL_ROOT%\env\soleil-env-vc8.bat

START /D "C:\Program Files\WaveMetrics\Igor Pro Folder" Igor.exe

