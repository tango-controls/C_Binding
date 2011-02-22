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
#include "GroupRepository.h"
#include "DataAdapter.h"
#include "EventManager.h"
#include "ObjectManager.h"

//=============================================================================
// STATIC MEMBERS
//=============================================================================
omni_mutex ObjectManager::lock; 
unsigned long ObjectManager::sessions_counter = 0;

#if (_TBFL_HAS_STATIC_OBJ_MANAGER_ == 1)
  ObjectManager  ObjectManager::instance;  
#else
  ObjectManager * ObjectManager::instance = 0;
#endif

//=============================================================================
// WINDOWS DLLMain ROUTINE
//=============================================================================
#if (_TBFL_HAS_STATIC_OBJ_MANAGER_ == 0) 
#if ! defined (_TBFL_HAS_OPEN_CLOSE_SESSION_INTERFACE_) 
#if defined (WIN32)
BOOL APIENTRY DllMain (HANDLE h, DWORD  reason_for_call, LPVOID r)
{
  switch (reason_for_call)
  {
    case DLL_PROCESS_ATTACH:
      try 
      {
        ObjectManager::init();
      }
      catch (...) 
      {
        return FALSE;
      }
      break;
    case DLL_PROCESS_DETACH:
      try 
      {
		    ObjectManager::fini();
      }
      catch (...) 
      {
        //- ignore any error
      }
    default:
      break;
  }
  return TRUE;
}
#elif (defined(_linux_) || defined (__linux__)) && defined(__GNUG__)
//=============================================================================
// GNU-LINUX SHARED LIB INITIALIZATION ROUTINE
//=============================================================================
extern "C" void __attribute__ ((constructor)) init_shared_lib (void);
//-----------------------------------------------------------------------------
void __attribute__ ((constructor)) init_shared_lib (void)
{
  try 
  {
	  ObjectManager::init();
  }
  catch (...) 
  {
    //- ignore any exception
  }
}
//=============================================================================
// GNU-LINUX SHARED LIB CLEANUP ROUTINE
//=============================================================================
extern "C" void __attribute__ ((destructor)) cleanup_shared_lib (void);
//-----------------------------------------------------------------------------
void __attribute__ ((destructor)) cleanup_shared_lib (void)
{
#if defined(_DEBUG)
	std::cout << "***** cleanup_shared_lib <<= *****" << std::endl;
#endif

  try 
  {
	  ObjectManager::fini();
  }
  catch (...) 
  {
    //- ignore any exception
  }
}
#else //- platform: not WIN32 nor GNU-LINUX
# error sorry, no support for your platform
#endif //- platform
#endif //- ! _TBFL_HAS_OPEN_CLOSE_SESSION_INTERFACE_ 
//=============================================================================
// ObjectManager::init
//=============================================================================
void ObjectManager::init (void) throw (Tango::DevFailed)
{
  omni_mutex_lock guard(ObjectManager::lock);

  if (ObjectManager::instance) 
  {
		ObjectManager::sessions_counter++;		
    return; 
	}

  ObjectManager::init_i();
}
//=============================================================================
// ObjectManager::init_i
//=============================================================================
void ObjectManager::init_i (void) throw (Tango::DevFailed)
{
  try 
  {
    ObjectManager::instance = new ObjectManager();
    if (ObjectManager::instance == 0)
      throw std::bad_alloc();
  }
  catch (Tango::DevFailed& df) 
  {
    Tango::Except::re_throw_exception(df,
                                      _CPTC("internal error"),
                                      _CPTC("TANGO binding initialization failed"),
                                      _CPTC("ObjectManager::init"));
  }
  catch (...) 
  {
    Tango::Except::throw_exception(_CPTC("internal error"),
                                   _CPTC("TANGO binding initialization failed"),
                                   _CPTC("ObjectManager::init"));
  }

	ObjectManager::sessions_counter++;
}
//=============================================================================
// ObjectManager::fini
//=============================================================================
void ObjectManager::fini (void) throw(Tango::DevFailed)
{
  omni_mutex_lock guard(ObjectManager::lock);

  if (ObjectManager::instance == 0)
    return;
  
  if (--ObjectManager::sessions_counter == 0)
  	ObjectManager::fini_i();
}
//=============================================================================
// ObjectManager::fini_i
//=============================================================================
void ObjectManager::fini_i (void) throw(Tango::DevFailed)
{
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
#endif //- _TBFL_HAS_STATIC_OBJ_MANAGER_ == 0

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

  //- init the device repository singleton
  DeviceRepository::init();

  //- init the group repository singleton
  GroupRepository::init();

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

    //- release the group repository singleton
    GroupRepository::cleanup();

    //- release the device repository singleton
    DeviceRepository::cleanup();

    //- release the data adapter singleton
    DataAdapter::cleanup();

    //- release the exception converter singleton
    ErrorStack::cleanup();

    //- try to release CORBA stuffs properly
    Tango::ApiUtil::cleanup(); 
  }
  catch (...)
  {
    //- ignore any exception
  }
}

