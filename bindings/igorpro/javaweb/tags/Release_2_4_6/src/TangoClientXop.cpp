// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for Igor Pro
//
// = FILENAME
//   TangoClientXop.cpp
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

//=============================================================================
// DEPENDENCIES
//=============================================================================
#include <yat/time/Timer.h>
#include "XDK_StandardHeaders.h"
#include "TangoBinding.h"
#include "MonitoredObjectManager.h"
#include "TangoClientXop.h"

//=============================================================================
// GLOBAL CONSTs
//=============================================================================
const char kNameSep = '#';

//=============================================================================
// GLOBAL CONSTs
//=============================================================================
yat::ThreadUID TangoClientXop::main_thread_id = YAT_INVALID_THREAD_UID;

//-----------------------------------------------------------------------------
// TangoClientXop member : TangoClientXop (constructor)                                    
//-----------------------------------------------------------------------------
TangoClientXop::TangoClientXop (IORecHandle _ioRecHandle) 
  : XDK_Xop(_ioRecHandle)
{
  //- noop
}

//-----------------------------------------------------------------------------
// TangoClientXop member : ~TangoClientXop (destructor)                                   
//-----------------------------------------------------------------------------
TangoClientXop::~TangoClientXop () 
{
  //- noop
}

//-----------------------------------------------------------------------------
// TangoClientXop member : init                                                            
//-----------------------------------------------------------------------------
int TangoClientXop::init () 
{
  TangoClientXop::main_thread_id = yat::ThreadingUtilities::self(); 
  
  if (TangoBinding::init() == kError)
    return kError;

  return kNoError;
}

//-----------------------------------------------------------------------------
// TangoClientXop member : idle                     
//-----------------------------------------------------------------------------   
void TangoClientXop::idle  ()
{
  static yat::Timer t;
  
  //- limit idle calls frequency to 10 Hz
  if (! MON_OBJ_MANAGER->has_objects_to_monitor() || (t.elapsed_msec() < 50))
    return;
  
#if defined (_VERBOSE_)
  std::cout << "TangoClientXop::idle::called @ " 
            << 1. / t.elapsed_sec() 
            << " Hz" 
            << std::endl;
#endif
  
  t.restart();
  
  if (TangoClientXop::main_thread_id != yat::ThreadingUtilities::self())
    XOP_MSG(("Tango-Binding::warning <IDLE> activity NOT triggered by main thread!"));
  
  MON_OBJ_MANAGER->export_values();
}

//-----------------------------------------------------------------------------
// TangoClientXop member : cleanup                                                       
//-----------------------------------------------------------------------------
void TangoClientXop::cleanup () 
{
#if defined (_XDK_DEBUG_)
  XOP_MSG(("Tango-Binding::handling XOP message::CLEANUP"));
#endif
  TangoBinding::cleanup();
}

//-----------------------------------------------------------------------------
// TangoClientXop member : load_experiment                    
//-----------------------------------------------------------------------------   
int TangoClientXop::load_experiment ()
{	  
#if defined (_XDK_DEBUG_)
  XOP_MSG(("Tango-Binding::handling XOP message::LOAD"));
#endif
  return kNoError;
}

//-----------------------------------------------------------------------------
// TangoClientXop member : new_experiment                                                  
//-----------------------------------------------------------------------------
int TangoClientXop::new_experiment ()
{
#if defined (_XDK_DEBUG_)
  XOP_MSG(("Tango-Binding::handling XOP message::NEW"));
#endif
  return kNoError;
}

//-----------------------------------------------------------------------------
// TangoClientXop member : clear_modified                     
//-----------------------------------------------------------------------------   
int TangoClientXop::clear_modified ()
{	  
#if defined (_XDK_DEBUG_)
  XOP_MSG(("Tango-Binding::handling XOP message::CLEAR_MODIFIED"));
#endif
  return XDK_UTILS->build_tango_default_objs();
}