// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for LabVIEW
//
// = FILENAME
//   ObjectManager.h
//
// = AUTHOR
//   Nicolas Leclercq - SOLEIL
//
// ============================================================================

#ifndef _OBJ_MANAGER_H_
#define _OBJ_MANAGER_H_

//=============================================================================
// CLASS : ObjectManager [SINGLETON]
//=============================================================================
class ObjectManager
{
  //- just make some compilers happy: give ObjectManager a friend
  friend class DummyClass;

public:

#if (_TBFL_HAS_STATIC_OBJ_MANAGER_ == 0)

  static void init (void) 
    throw (Tango::DevFailed);

  static void fini (void) 
    throw (Tango::DevFailed);
#endif

private:

  static omni_mutex lock;

#if (_TBFL_HAS_STATIC_OBJ_MANAGER_ == 0)

  static void init_i (void) 
    throw (Tango::DevFailed);

  static void fini_i (void) 
    throw (Tango::DevFailed);

  static unsigned long sessions_counter;

  static ObjectManager * instance;

#else

  static ObjectManager instance;

#endif

  // = Disallow these operations (except for friend classes).
  //---------------------------------------------------------
  ObjectManager(void);
  virtual ~ObjectManager(void);
  ObjectManager (const ObjectManager&);
  void operator= (const ObjectManager&); 
};

#endif // _OBJ_MANAGER_H_

   
   
