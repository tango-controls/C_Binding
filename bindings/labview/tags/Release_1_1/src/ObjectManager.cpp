// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for LabVIEW
//
// = FILENAME
//   ObjectManager.cpp
//
// = AUTHOR
//   Nicolas Leclercq - SOLEIL
//
// ============================================================================

//=============================================================================
// DEPENDENCIES
//=============================================================================
#include "StandardHeader.h"
#include "ErrorStack.h"
#include "DeviceRepository.h"
#include "DataAdapter.h"
#include "EventManager.h"
#include "ObjectManager.h"

//=============================================================================
// STATIC MEMBERS
//=============================================================================
omni_mutex ObjectManager::lock; 

#if (TBFL_HAS_STATIC_OBJ_MANAGER == 1)
  ObjectManager  ObjectManager::instance;  
#else
  ObjectManager * ObjectManager::instance = 0;
  long ObjectManager::registered_vi_id = 0;
  ViRepository ObjectManager::vi_repository;
#endif

#if (TBFL_HAS_STATIC_OBJ_MANAGER == 0)
//=============================================================================
// ObjectManager::init
//=============================================================================
void ObjectManager::init (void) throw (Tango::DevFailed)
{
  omni_mutex_lock guard(ObjectManager::lock);

  ObjectManager::init_i();
}

//=============================================================================
// ObjectManager::init_i
//=============================================================================
void ObjectManager::init_i (void) throw (Tango::DevFailed)
{
  if (ObjectManager::instance) 
  {
    return; 
  }

  try 
  {
    ObjectManager::instance = new ObjectManager();
    if (ObjectManager::instance == 0)
      throw std::bad_alloc();
  }
  catch (...) 
  {
    Tango::Except::throw_exception(_CPTC("internal error"),
                                   _CPTC("TANGO binding initialization failed"),
                                   _CPTC("ObjectManager::init"));
  }
}

//=============================================================================
// ObjectManager::fini
//=============================================================================
void ObjectManager::fini (void) throw(Tango::DevFailed)
{
  omni_mutex_lock guard(ObjectManager::lock);

  ObjectManager::fini_i();
}

//=============================================================================
// ObjectManager::fini_i
//=============================================================================
void ObjectManager::fini_i (void) throw(Tango::DevFailed)
{
  if (ObjectManager::instance == 0) 
  {
    return;
  }

  try 
  {
    delete ObjectManager::instance;
    ObjectManager::instance = 0;
  }
  catch (...) 
  {
    Tango::Except::throw_exception(_CPTC("internal error"),
                                   _CPTC("TANGO binding cleanup failed"),
                                   _CPTC("ObjectManager::fini"));
  }
}
#endif // TBFL_HAS_STATIC_OBJ_MANAGER == 0

//=============================================================================
// ObjectManager::register_vi
//=============================================================================
void ObjectManager::register_vi (const std::string& vi_name) 
  throw(Tango::DevFailed)
{
#if (TBFL_HAS_STATIC_OBJ_MANAGER == 0)

  omni_mutex_lock guard(ObjectManager::lock);

  if (ObjectManager::vi_repository.empty())
  {
    ObjectManager::init_i();
  }

  ViRepositoryIterator it = ObjectManager::vi_repository.find(vi_name);
  if (it != ObjectManager::vi_repository.end()) 
  {
    return;
  }

  std::pair<ViRepositoryIterator, bool> result;
  ViRepository::value_type val(vi_name, ObjectManager::registered_vi_id + 1);
  result = ObjectManager::vi_repository.insert(val);
  if (result.second == 0) 
  {
     std::string d = "failed to register VI " + vi_name;
     Tango::Except::throw_exception(_CPTC("internal error"),
			                               d.c_str(),
                                    _CPTC("ObjectManager::register_vi"));
  }

  ObjectManager::registered_vi_id++;

#else

  //- avoid unused arg warning
  do {/*noop*/} while (&vi_name == 0);

#endif //- #if (TBFL_HAS_STATIC_OBJ_MANAGER == 0)
}

//=============================================================================
// ObjectManager::unregister_vi
//=============================================================================
void ObjectManager::unregister_vi (const std::string& vi_name) 
  throw(Tango::DevFailed)
{
#if (TBFL_HAS_STATIC_OBJ_MANAGER == 0)

  omni_mutex_lock guard(ObjectManager::lock);

  ViRepositoryIterator it = ObjectManager::vi_repository.find(vi_name);
  if (it == ObjectManager::vi_repository.end()) 
  {
     Tango::Except::throw_exception(_CPTC("no such VI"),
			                              _CPTC("could not find any registered VI with the specified name"),
			                              _CPTC("ObjectManager::unregister_vi"));
  }

  ObjectManager::vi_repository.erase(it); 

  if (ObjectManager::vi_repository.empty())
  {
    ObjectManager::fini_i();
  }

#else

  //- avoid unused arg warning
  do {/*noop*/} while (&vi_name == 0);

#endif //- #if (TBFL_HAS_STATIC_OBJ_MANAGER == 0)
}

//=============================================================================
// ObjectManager::ObjectManager 
//=============================================================================
ObjectManager::ObjectManager (void)
{
  //- force TANGO client API initialization
  Tango::ApiUtil * u = Tango::ApiUtil::instance();
  if (u == 0) throw std::bad_alloc();
  //- init the exception converter singleton
  ErrorStack::init();
  //- init the data adapter singleton
  DataAdapter::init();
  //- init the devices repository singleton
  DeviceRepository::init();
  //- init the event manager singleton
  EventManager::init(); 
}

//=============================================================================
// ObjectManager::~ObjectManager 
//=============================================================================
ObjectManager::~ObjectManager (void)
{
  try 
  {  
    //- release the event manager singleton
    EventManager::cleanup();
    //- release the devices repository singleton
    DeviceRepository::cleanup();
    //- release the data adapter singleton
    DataAdapter::cleanup();
    //- release the exception converter singleton
    ErrorStack::cleanup();
    //- try to kill CORBA stuffs properly
    Tango::ApiUtil::cleanup();  
  }
  catch (...)
  {
    //- ignore any exception
  }
}

