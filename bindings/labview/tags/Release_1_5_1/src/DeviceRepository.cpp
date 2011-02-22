// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for LabVIEW
//
// = FILENAME
//   DeviceRepository.cpp
//
// = AUTHOR
//   Nicolas Leclercq - SOLEIL
//
// ============================================================================

#ifndef _DEVICE_REPOSITORY_CPP_
#define _DEVICE_REPOSITORY_CPP_

//=============================================================================
// DEPENDENCIES
//=============================================================================
#include "StandardHeader.h"
#include "DeviceRepository.h"

#if !defined (_LV_INLINE_)
# include "DeviceRepository.i"
#endif 

//=============================================================================
// STATIC MEMBERS
//=============================================================================
DeviceRepository * DeviceRepository::instance_ = 0;

//=============================================================================
// DeviceRepository::cleanup
//=============================================================================
void DeviceRepository::init (void)
    throw (Tango::DevFailed)
{
  if (DeviceRepository::instance_ != 0)
    return;

  try
  {
    DeviceRepository::instance_ = new DeviceRepository;
    if (DeviceRepository::instance_ == 0) 
      throw std::bad_alloc();
  }
  catch (const std::bad_alloc&)
  {
    Tango::Except::throw_exception(_CPTC("out of memory"),
			                             _CPTC("binding error: DeviceRepository instanciation failed"),
			                             _CPTC("DeviceRepository::init_device_repository"));
  }
  catch (Tango::DevFailed& df)
  {
    Tango::Except::re_throw_exception(df,
                                      _CPTC("software error"),
			                                _CPTC("binding error: DeviceRepository instanciation failed [Tango exception caught]"),
			                                _CPTC("DeviceRepository::init_device_repository"));
  }
  catch (...)
  {
    Tango::Except::throw_exception(_CPTC("software error"),
			                             _CPTC("binding error: DeviceRepository instanciation failed [unknown exception caught]"),
			                             _CPTC("DeviceRepository::init_device_repository"));
  }
}

//=============================================================================
// DeviceRepository::cleanup
//=============================================================================
void DeviceRepository::cleanup (void)
    throw (Tango::DevFailed)
{
  if (DeviceRepository::instance_) 
  {
    delete DeviceRepository::instance_;
    DeviceRepository::instance_ = 0;
  }
}

//=============================================================================
// DeviceRepository::DeviceRepository 
//=============================================================================
DeviceRepository::DeviceRepository (void)
{
  //- noop
}

//=============================================================================
// DeviceRepository::~DeviceRepository 
//=============================================================================
DeviceRepository::~DeviceRepository (void)
{
  //- critical section
  omni_mutex_lock guard(this->lock_);

  this->remove_device_i("*");
}

//=============================================================================
// DeviceRepository::device_desc
//=============================================================================
DeviceDescriptor * DeviceRepository::device_desc (const std::string& _device_name)
    throw (Tango::DevFailed)
{
  //- critical section
  omni_mutex_lock guard(this->lock_);

  //- convert to lower case
  std::string dev_name(_device_name);
  std::transform(dev_name.begin(), dev_name.end(), dev_name.begin(), ::tolower);

  //- search for <device_name> in the repository
  DeviceDescriptorMapIterator it = this->dev_map_.find(dev_name);
  if (it != this->dev_map_.end() && it->second) 
    return it->second;

  DeviceDescriptor * ddesc = 0;

  try
  {
    //- search failed, create the descriptor
    ddesc = new DeviceDescriptor(dev_name);
    if (ddesc == 0) 
      throw std::bad_alloc();
  }
  catch (const std::bad_alloc &)
  {
    Tango::Except::throw_exception(_CPTC("out of memory"),
			                             _CPTC("binding error: memory allocation failed"),
			                             _CPTC("DeviceRepository::device_desc"));
  }
  catch (Tango::DevFailed & df)
  {
    Tango::Except::re_throw_exception(df,
                                      _CPTC("object instanciation failed"),
			                                _CPTC("DeviceDescriptor instanciation failed [Tango exception caught]"),
			                                _CPTC("DeviceRepository::device_desc"));
  }
  catch (...)
  {
    Tango::Except::throw_exception(_CPTC("object instanciation failed"),
			                             _CPTC("DeviceDescriptor instanciation failed [unknown error]"),
			                             _CPTC("DeviceRepository::device_desc"));
  }

  //- store the device descriptor into the repository
  std::pair<DeviceDescriptorMapIterator, bool> result;
  result = this->dev_map_.insert(DeviceDescriptorMap::value_type(dev_name, ddesc));
  if (result.second == 0) 
  {
    delete ddesc;
    Tango::Except::throw_exception(_CPTC("internal error"),
			                             _CPTC("binding error: could not insert device into the local repository"),
			                             _CPTC("DeviceRepository::device_desc"));
  }

  //- return the device descriptor
  return ddesc;
}

//=============================================================================
// DeviceRepository::device_desc
//=============================================================================
DeviceDescriptor * DeviceRepository::device_desc (Tango::DeviceProxy * _dp, bool _ownership)
    throw (Tango::DevFailed)
{
  //- critical section
  omni_mutex_lock guard(this->lock_);

  if (! _dp)
  {
    Tango::Except::throw_exception(_CPTC("invalid argument"),
			                             _CPTC("unexpected NULL argument"),
			                             _CPTC("DeviceRepository::device_desc"));
  }

  //- convert to lower case
  std::string dev_name(_dp->dev_name());
  std::transform(dev_name.begin(), dev_name.end(), dev_name.begin(), ::tolower);

  //- search for <device_name> in the repository
  DeviceDescriptorMapIterator it = this->dev_map_.find(dev_name);
  if (it != this->dev_map_.end() && it->second) 
    return it->second;

  //- search failed, create the descriptor
  DeviceDescriptor * ddesc = new DeviceDescriptor(_dp, _ownership);
  if (ddesc == 0) 
  {
    Tango::Except::throw_exception(_CPTC("out of memory"),
			                             _CPTC("binding error: memory allocation failed"),
			                             _CPTC("DeviceRepository::device_desc"));
  }

  //- store the device descriptor into the repository
  std::pair<DeviceDescriptorMapIterator, bool> result;
  result = this->dev_map_.insert(DeviceDescriptorMap::value_type(dev_name, ddesc));
  if (result.second == 0) 
  {
    delete ddesc;
    Tango::Except::throw_exception(_CPTC("internal error"),
			                             _CPTC("binding error: could not insert device into the local repository"),
			                             _CPTC("DeviceRepository::device_desc"));
  }

  //- return the device descriptor
  return ddesc;
}

//=============================================================================
// DeviceRepository::remove_device
//=============================================================================
void DeviceRepository::remove_device (const std::string& _device_name)
{
  //- critical section
  omni_mutex_lock guard(this->lock_);

  this->remove_device_i(_device_name);
}

//=============================================================================
// DeviceRepository::remove_device_i
//=============================================================================
void DeviceRepository::remove_device_i(const std::string& _device_name)
{
  if (_device_name == "*") 
  {
    DeviceDescriptorMapIterator cur = this->dev_map_.begin();
    DeviceDescriptorMapIterator end = this->dev_map_.end();
    for (; cur != end; cur++) 
    {
      if (cur->second)
	      delete cur->second;
    } 
    this->dev_map_.clear();
  }
  else 
  {
    std::string dev_name(_device_name);
    std::transform(dev_name.begin(), dev_name.end(), dev_name.begin(), ::tolower);
    DeviceDescriptorMapIterator it = this->dev_map_.find(dev_name);
    if (it != this->dev_map_.end()) 
    {
      if (it->second)
        delete it->second;
      this->dev_map_.erase(dev_name);
    }
  }
}

#endif // _DEVICE_REPOSITORY_CPP_
