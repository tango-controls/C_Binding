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
  friend HOST_IMPORT void main (IORecHandle);

protected:
  XDK_Xop (IORecHandle _ioRecHandle);
  // Constructeur
    
  virtual ~XDK_Xop (void);
  // Destructeur.

  int startup (void);
  // Initializes the XOP then calls <init>.

  virtual int init (void) = 0;
  // XOP initialization.

  virtual void cleanup (void) = 0;
  // CLEANUP msg handler.

  #if defined(_XOP_NEEDS_IDLE_)
    virtual void idle (void) = 0;
    // IDLE  msg handler. 
  #endif

  virtual int new_experiment (void);
  // NEW msg handler : no-op default behaviour.

  virtual int load_experiment (void);
  // LOAD msg handler : no-op default behaviour.

  virtual int clear_modified (void);
  // CLEAR_MODIFIED msg handler : no-op default behaviour.

private:
  static XDK_Xop* instance;
  // Instance unique de la classe XDK_Xop.

  static void process_igor_message (void);
  // Disptacher des messages d'Igor. 

  void quit (void);
  // Terminaison (sur reception msg CLEANUP)

  //-- METHODES INTERDITES
  XDK_Xop (const XDK_Xop&);
  void operator= (const XDK_Xop&); 
};

#endif // _CLASS_XDK_XOP_H_

   
   