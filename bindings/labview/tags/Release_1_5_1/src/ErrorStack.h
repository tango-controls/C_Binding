// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for LabVIEW
//
// = FILENAME
//   ErrorStack.h
//
// = AUTHOR
//   Nicolas Leclercq - SOLEIL
//
// ============================================================================

#ifndef _ERROR_STACK_H_
#define _ERROR_STACK_H_

//=============================================================================
// Class ErrorStack [SINGLETON]
//=============================================================================
class ErrorStack
{
  friend class ObjectManager;

public:

  static ErrorStack* instance (void);
  // Returns the unique instance of <ErrorStack>.

  int devfailed (const Tango::DevFailed& _tango_error, LvStringArrayHdl& lv_error_);
  // Handle TANGO exception

  int unknown (const char* o, LvStringArrayHdl& lv_error_);
  // Handle unknown exception

  int no_error (const char* o, LvStringArrayHdl& lv_error_);
  // Handle no error

private:
  static int init (void);
  //- Instanciates the singleton.
  
  static void cleanup (void);
  //- Releases the singleton.

  static ErrorStack* instance_;
  //- The unique instance of <ErrorStack>.

  ErrorStack ();
  //- Ctor

  // = Disallow these operations (except for ObjectManager).
  //-------------------------------------------------------
  ErrorStack (const ErrorStack&);
  virtual ~ErrorStack ();
  void operator= (const ErrorStack&); 
};

//=============================================================================
// MACROS 
//=============================================================================
#define ERROR_STACK ErrorStack::instance()

//=============================================================================
// INLINED CODE
//=============================================================================
#if defined (_LV_INLINE_)
  #include "ErrorStack.i"
#endif 

#endif // _ERROR_STACK_H_
