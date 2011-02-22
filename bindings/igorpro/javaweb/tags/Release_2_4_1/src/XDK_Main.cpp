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
HOST_IMPORT int main (IORecHandle _ioRecHandle)
{	
  try 
  {
    XDK_Xop::instance = new TangoClientXop(_ioRecHandle);
    if (XDK_Xop::instance == 0) 
    {
      ::SetXOPResult(kMEMORY_ERROR);
      return 0;
    }   
    int result = XDK_Xop::instance->startup();
    if (result == kError)
      ::SetXOPResult(XOP_CANT_INIT);
    else 
      ::SetXOPResult(result);
  }
  catch (...) 
  {
    ::SetXOPResult(XOP_CANT_INIT); 
  }
  
  return 0;
}


 
