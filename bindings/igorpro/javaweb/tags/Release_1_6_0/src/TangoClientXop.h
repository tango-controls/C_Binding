// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for Igor Pro
//
// = FILENAME
//   TangoClientXop.h
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

#ifndef _TANGO_CLIENT_XOP_H_
#define _TANGO_CLIENT_XOP_H_

//=============================================================================
// DEPENDENCIES
//=============================================================================
#include "XDK_Xop.h"

//=============================================================================
//  CLASS :  TangoClientXop (PSEUDO-SINGLETON)
//=============================================================================
class TangoClientXop : public XDK_Xop
{
  friend HOST_IMPORT void main (IORecHandle); 

protected:
  TangoClientXop (IORecHandle _ioRecHandle);
  // Ctor
  virtual ~TangoClientXop (void);
  // Dtor.
  int init (void);
  // Initialization
  void cleanup (void);
  // Cleanup
  virtual int clear_modified (void);
  // CLEAR_MODIFIED msg handler.

private:   
  TangoClientXop (const TangoClientXop&);
  void operator= (const TangoClientXop&); 
};

#endif // _TANGO_CLIENT_XOP_H_

   
   