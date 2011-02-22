#!/bin/sh

#- Igor version
IGOR_VERS=6.2

#- where is Matlab installed?
IGOR_INSTALL_DIR=/Applications/Developpement

#- Matlab MacOS X binary
IGOR_BIN="$IGOR_INSTALL_DIR/Igor Pro $IGOR_VERS/Igor Pro.app/Contents/MacOS/Igor Pro"

#- Tango database host and port 
TANGO_HOST=192.168.0.16:10000
export TANGO_HOST

#- launch Matlab
"$IGOR_BIN"&

exit 0