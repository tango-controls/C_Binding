// ============================================================================
//
// = CONTEXT
//   XDK
//
// = FILENAME
//   XDK_Main.cpp
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

//=============================================================================
// DEPENDENCIES
//=============================================================================
#include "XDK_StandardHeaders.h"
#include "TangoClientXop.h"

//-----------------------------------------------------------------------------
// Routine : main
//-----------------------------------------------------------------------------
HOST_IMPORT void main (IORecHandle _ioRecHandle)
{	
  try 
  {
    TangoClientXop* tc_xop = new TangoClientXop(_ioRecHandle);
    if (tc_xop == 0) 
    {
      ::SetXOPResult(kMEMORY_ERROR);
      return;
    }   
    int result = tc_xop->startup();
    if (result == kError)
      ::SetXOPResult(XOP_CANT_INIT);
    else 
      ::SetXOPResult(result);
  }
  catch (...) 
  {
    ::SetXOPResult(XOP_CANT_INIT); 
  }
}

 
