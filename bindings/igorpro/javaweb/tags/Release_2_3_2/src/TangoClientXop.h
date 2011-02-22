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
  // CLEANUP msg handler.

  virtual int new_experiment (void);
  // NEW msg handler

  virtual int load_experiment (void);
  // LOAD msg handler

  virtual int clear_modified (void);
  // CLEAR_MODIFIED msg handler.

  virtual void idle (void);
  // IDLE  msg handler. 

private:   
  TangoClientXop (const TangoClientXop&);
  void operator= (const TangoClientXop&); 
};

#endif // _TANGO_CLIENT_XOP_H_

   
   