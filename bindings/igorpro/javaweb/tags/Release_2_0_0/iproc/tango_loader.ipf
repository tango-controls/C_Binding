#pragma rtGlobals = 2
#pragma version = 1.0
#pragma IgorVersion = 6.0

//==============================================================================
// tango_loader.ipf
//------------------------------------------------------------------------------
// N.Leclercq - SOLEIL
//==============================================================================

//==============================================================================
// DEPENDENCIES
//==============================================================================
#include "tango_panel"
#include "tango_monitor"
#include "tango_code_generator"

//==============================================================================
// menu TANGO
//==============================================================================
Menu "Tango"
	"TANGO Browser...",Execute/P/Q "tango_panel()"
	"-"
	"Kill all Monitors",Execute/P/Q "tmon_kill_all_monitors()"
end

//==============================================================================
// Igor Hook: tango_start_new_hook
//==============================================================================
static function IgorStartOrNewHook(igorApplicationNameStr)
 String igorApplicationNameStr
 SetIgorHook IgorBeforeNewHook=tango_before_new_hook
 SetIgorHook IgorQuitHook=tango_quit_hook
 tools_df_make("root:tango:common", 0);
 print "Tango-Binding::welcome!"
 tango_load_prefs()
end

//==============================================================================
// Igor Hook: tango_before_new_hook
//==============================================================================
function tango_before_new_hook (app)
 String app
 tmon_kill_all_monitors()
 tango_monitor_stop("*","*")
 tango_save_prefs()
end

//==============================================================================
// Igor Hook: tango_quit_hook
//==============================================================================
function tango_quit_hook (app)
 String app
 tmon_kill_all_monitors()
 tango_monitor_stop("*","*")
 tango_save_prefs()
end

