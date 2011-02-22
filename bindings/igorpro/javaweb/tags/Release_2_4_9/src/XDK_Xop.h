// ============================================================================
//
// = CONTEXT
//   XDK
//
// = FILENAME
//   XDK_Xop.h
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

#ifndef _CLASS_XDK_XOP_H_
#define _CLASS_XDK_XOP_H_

//=============================================================================
// Class : XDK_Xop (ABSTRACT)
//=============================================================================
class XDK_Xop
{
public:
  XDK_Xop (IORecHandle _ioRecHandle);
  // Ctor
    
  virtual ~XDK_Xop ();
  // Dtor

  int startup ();
  // Initializes the XOP then calls <init>.

  virtual int init () = 0;
  // XOP initialization.

  virtual void cleanup () = 0;
  // CLEANUP msg handler.

  #if defined(_XOP_NEEDS_IDLE_)
    virtual void idle () = 0;
    // IDLE  msg handler. 
  #endif

  virtual int new_experiment ();
  // NEW msg handler : no-op default behaviour.

  virtual int load_experiment ();
  // LOAD msg handler : no-op default behaviour.

  virtual int clear_modified ();
  // CLEAR_MODIFIED msg handler : no-op default behaviour.

  static void process_igor_message ();
  // Disptacher des messages d'Igor. 

  void quit ();
  // Terminaison (sur reception msg CLEANUP)

  static XDK_Xop * instance;
  // Instance unique de la classe XDK_Xop.

private:
  //-- METHODES INTERDITES
  XDK_Xop (const XDK_Xop&);
  void operator= (const XDK_Xop&); 
};

#endif // _CLASS_XDK_XOP_H_

   
   