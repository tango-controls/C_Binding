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
 
#define REQUIRED_IGOR_VERSION 620

//-----------------------------------------------------------------------------
// Routine : main
//-----------------------------------------------------------------------------
HOST_IMPORT int main (IORecHandle _ioRecHandle)
{	
  try 
  {
    XDK_Xop::instance = new TangoClientXop(_ioRecHandle);
    if (! XDK_Xop::instance) 
    {
      ::SetXOPResult(kMEMORY_ERROR);
      return EXIT_FAILURE;
    }   

    int result = XDK_Xop::instance->startup();
    if (result == kError)
    {
      ::SetXOPResult(XOP_CANT_INIT);
      return EXIT_FAILURE;
    }   

    ::SetXOPResult(result);
  }
  catch (...) 
  {
    ::SetXOPResult(XOP_CANT_INIT); 
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}


 
