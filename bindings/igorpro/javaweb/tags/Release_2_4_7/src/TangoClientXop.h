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
#include <yat/threading/Utilities.h>
#include "XDK_Xop.h"

//=============================================================================
//  CLASS :  TangoClientXop (PSEUDO-SINGLETON)
//=============================================================================
class TangoClientXop : public XDK_Xop
{
public:
  TangoClientXop (IORecHandle _ioRecHandle);
  // Ctor

  virtual ~TangoClientXop ();
  // Dtor.

  int init ();
  // Initialization

  void cleanup ();
  // CLEANUP msg handler.

  virtual int new_experiment ();
  // NEW msg handler

  virtual int load_experiment ();
  // LOAD msg handler

  virtual int clear_modified ();
  // CLEAR_MODIFIED msg handler.

  virtual void idle ();
  // IDLE  msg handler. 

private:   
  //- store Igor's main thread id (debug purpose)
  static yat::ThreadUID main_thread_id;
  
  TangoClientXop (const TangoClientXop&);
  void operator= (const TangoClientXop&); 
};

#endif // _TANGO_CLIENT_XOP_H_

   
   