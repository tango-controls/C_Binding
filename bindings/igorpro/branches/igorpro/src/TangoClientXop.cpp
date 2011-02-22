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
#include "XDK_StandardHeaders.h"
#include "TangoBinding.h"
#include "TangoClientXop.h"

//=============================================================================
// GLOBAL CONSTs
//=============================================================================
const char kNameSep = '.';

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
// TangoClientXop member : cleanup                                                       
//-----------------------------------------------------------------------------
void TangoClientXop::cleanup (void) 
{
  TangoBinding::cleanup();
}

//-----------------------------------------------------------------------------
// TangoClientXop member : clear_modified                     
//-----------------------------------------------------------------------------   
int TangoClientXop::clear_modified (void)
{	  
  return XDK_UTILS->build_tango_default_objs();
}