#!/bin/sh

#- where is Matlab installed?
IGOR_INSTALL_DIR=/Applications/Developpement

#- Matlab MacOS X binary
IGOR_BIN="$IGOR_INSTALL_DIR/Igor Pro 6.1/Igor Pro.app/Contents/MacOS/Igor Pro"

#- Tango database host and port 
TANGO_HOST=192.168.0.17:10000
export TANGO_HOST

#- launch Matlab
"$IGOR_BIN"&

exit 0