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

//-----------------------------------------------------------------------------
// TangoClientXop member : TangoClientXop (constructor)                                    
//-----------------------------------------------------------------------------
TangoClientXop::TangoClientXop (IORecHandle _ioRecHandle) 
  : XDK_Xop(_ioRecHandle)
{

}

//-----------------------------------------------------------------------------
// TangoClientXop member : ~TangoClientXop (destructor)                                   
//-----------------------------------------------------------------------------
TangoClientXop::~TangoClientXop (void) 
{
 
}

//-----------------------------------------------------------------------------
// TangoClientXop member : init                                                            
//-----------------------------------------------------------------------------
int TangoClientXop::init (void) 
{
  if (TangoBinding::init() == kError)
    return kError;

  return kNoError;
}

//-----------------------------------------------------------------------------
// TangoClientXop member : idle                     
//-----------------------------------------------------------------------------   
void TangoClientXop::idle  (void)
{
  MON_OBJ_MANAGER->export_values();
}

//-----------------------------------------------------------------------------
// TangoClientXop member : cleanup                                                       
//-----------------------------------------------------------------------------
void TangoClientXop::cleanup (void) 
{
#if defined (_XDK_DEBUG_)
  XOP_MSG(("Tango-Binding::handling XOP message::CLEANUP"));
#endif
  TangoBinding::cleanup();
}

//-----------------------------------------------------------------------------
// TangoClientXop member : load_experiment                    
//-----------------------------------------------------------------------------   
int TangoClientXop::load_experiment (void)
{	  
#if defined (_XDK_DEBUG_)
  XOP_MSG(("Tango-Binding::handling XOP message::LOAD"));
#endif
  return kNoError;
}

//-----------------------------------------------------------------------------
// TangoClientXop member : new_experiment                                                  
//-----------------------------------------------------------------------------
int TangoClientXop::new_experiment (void)
{
#if defined (_XDK_DEBUG_)
  XOP_MSG(("Tango-Binding::handling XOP message::NEW"));
#endif
  return kNoError;
}

//-----------------------------------------------------------------------------
// TangoClientXop member : clear_modified                     
//-----------------------------------------------------------------------------   
int TangoClientXop::clear_modified (void)
{	  
#if defined (_XDK_DEBUG_)
  XOP_MSG(("Tango-Binding::handling XOP message::CLEAR_MODIFIED"));
#endif
  return XDK_UTILS->build_tango_default_objs();
}