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
 : dp_(0), ownership_(true), name_(dev_name)  
{
  dp_ = new Tango::DeviceProxy(const_cast<std::string&>(dev_name));
  if (dp_ == 0) 
  {
    Tango::Except::throw_exception(_CPTC("out of memory"),
			                             _CPTC("binding error: memory allocation failed"),
			                             _CPTC("ThreadSafeDeviceProxy::ThreadSafeDeviceProxy"));
  }
}

//=============================================================================
// ThreadSafeDeviceProxy::ThreadSafeDeviceProxy 
//=============================================================================
ThreadSafeDeviceProxy::ThreadSafeDeviceProxy (const char * dev_name)
    throw (Tango::DevFailed)
 : dp_(0), ownership_(true), name_(dev_name)    
{
	std::string dev(dev_name);

  dp_ = new Tango::DeviceProxy(dev);
  if (dp_ == 0) 
  {
    Tango::Except::throw_exception(_CPTC("out of memory"),
			                             _CPTC("binding error: memory allocation failed"),
			                             _CPTC("ThreadSafeDeviceProxy::ThreadSafeDeviceProxy"));
  }
}

//=============================================================================
// ThreadSafeDeviceProxy::ThreadSafeDeviceProxy 
//=============================================================================
ThreadSafeDeviceProxy::ThreadSafeDeviceProxy (Tango::DeviceProxy * _dp, bool _ownership)
    throw (Tango::DevFailed)
 : dp_(_dp), ownership_(_ownership)  
{
  if (dp_ == 0) 
  {
    Tango::Except::throw_exception(_CPTC("invalid argument"),
			                             _CPTC("unexpected NULL DeviceProxy"),
			                             _CPTC("ThreadSafeDeviceProxy::ThreadSafeDeviceProxy"));
  }
  this->name_ = dp_->dev_name();
}

//=============================================================================
// ThreadSafeDeviceProxy::~ThreadSafeDeviceProxy 
//=============================================================================
ThreadSafeDeviceProxy::~ThreadSafeDeviceProxy (void)
{
  if (dp_ && ownership_)
    delete dp_;
}

