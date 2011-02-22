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
// TYPEDEFs
//=============================================================================
#if (TBFL_HAS_STATIC_OBJ_MANAGER == 0) && defined(USE_VI_REGISTRATION)  
typedef std::map<const std::string, long> ViRepository;
typedef ViRepository::iterator ViRepositoryIterator;
#endif

//=============================================================================
// CLASS : ObjectManager [SINGLETON]
//=============================================================================
class ObjectManager
{
  //- just make some compilers happy: give ObjectManager a friend
  friend class DummyClass;

public:

#if (TBFL_HAS_STATIC_OBJ_MANAGER == 0)

  static void init (void) 
    throw (Tango::DevFailed);

  static void fini (void) 
    throw (Tango::DevFailed);

# if defined(USE_VI_REGISTRATION)  

  static void register_vi (const std::string& vi_name) 
    throw (Tango::DevFailed);

  static void unregister_vi (const std::string& vi_name) 
    throw (Tango::DevFailed);

# endif

#endif

private:

  static omni_mutex lock;

#if (TBFL_HAS_STATIC_OBJ_MANAGER == 0)

  static void init_i (void) 
    throw (Tango::DevFailed);

  static void fini_i (void) 
    throw (Tango::DevFailed);

  static ObjectManager * instance;

#if defined(USE_VI_REGISTRATION)  
  static long registered_vi_id;
  static ViRepository vi_repository;
#endif

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

   
   
