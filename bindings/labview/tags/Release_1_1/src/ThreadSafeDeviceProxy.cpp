// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for LabVIEW
//
// = FILENAME
//   ThreadSafeDeviceProxy.cpp
//
// = AUTHOR
//   Nicolas Leclercq - SOLEIL
//
// ============================================================================

//=============================================================================
// DEPENDENCIES
//=============================================================================
#include "StandardHeader.h"
#include "ThreadSafeDeviceProxy.h"

#if !defined (_LV_INLINE_)
  #include "ThreadSafeDeviceProxy.i"
#endif 

//=============================================================================
// ThreadSafeDeviceProxy::ThreadSafeDeviceProxy 
//=============================================================================
ThreadSafeDeviceProxy::ThreadSafeDeviceProxy (const std::string& dev_name)
    throw (Tango::DevFailed)
 : dp_(const_cast<std::string&>(dev_name))  
{
  //- noop ctor
}

//=============================================================================
// ThreadSafeDeviceProxy::ThreadSafeDeviceProxy 
//=============================================================================
ThreadSafeDeviceProxy::ThreadSafeDeviceProxy (const char * dev_name)
    throw (Tango::DevFailed)
 : dp_(dev_name)  
{
  //- noop ctor
}

//=============================================================================
// ThreadSafeDeviceProxy::~ThreadSafeDeviceProxy 
//=============================================================================
ThreadSafeDeviceProxy::~ThreadSafeDeviceProxy (void)
{
  //- noop dtor
}

