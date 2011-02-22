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

//=============================================================================
// DEPENDENCIES
//=============================================================================
#include "StandardHeader.h"
#include "ThreadSafeDeviceProxy.h"
#include "DeviceRepository.h"

#if !defined (_LV_INLINE_)
  #include "DeviceRepository.i"
#endif 

//=============================================================================
// STATIC MEMBERS
//=============================================================================
DeviceRepository* DeviceRepository::instance_ = 0;

//=============================================================================
// DeviceDesc::DeviceDesc
//=============================================================================
DeviceDesc::DeviceDesc (std::string _dev_name) 
  : dev_name_(_dev_name), 
    dev_proxy_(0), 
    dev_cmd_list_(0), 
    dev_attr_list_(0),
    ownership_(true)
{
	// no-op ctor
}

//=============================================================================
// DeviceDesc::DeviceDesc
//=============================================================================
DeviceDesc::DeviceDesc (ThreadSafeDeviceProxy *_dp, bool _ownership) 
  : dev_name_(_dp->unsafe_proxy().dev_name()), 
    dev_proxy_(_dp), 
    dev_cmd_list_(0), 
    dev_attr_list_(0),
    ownership_(_ownership)
{
	// no-op ctor
}

//=============================================================================
// DeviceDesc::~DeviceDesc 
//=============================================================================
DeviceDesc::~DeviceDesc (void)
{
  this->cmd_list(0);
  this->attr_list(0);
  if (this->dev_proxy_ && this->ownership_) {
    delete this->dev_proxy_;
    this->dev_proxy_ = 0;
  }
}

//=============================================================================
// DeviceDesc::cmd_exists 
//=============================================================================
int DeviceDesc::cmd_exists (const std::string& _cmd_name)
  throw (Tango::DevFailed)
{
  int indx = 0;
  DeviceDesc::MinCmdInfoList::iterator cur = this->dev_cmd_list_.begin();
  DeviceDesc::MinCmdInfoList::iterator end = this->dev_cmd_list_.end();
  while (cur != end) {
    if ((*cur).name == _cmd_name) {
      return indx;
    }
    cur++; indx++;
  }
  Tango::Except::throw_exception(_CPTC("command not found"),
			                           _CPTC("command could not be found in local command list"),
			                           _CPTC("DeviceDesc::cmd_exists"));
  return kError;
}

//=============================================================================
// DeviceDesc::attr_exists 
//=============================================================================
int DeviceDesc::attr_exists (const std::string& _attr_name)
  throw (Tango::DevFailed)
{
  int indx = 0;
  DeviceDesc::MinAttrInfoList::iterator cur = this->dev_attr_list_.begin();
  DeviceDesc::MinAttrInfoList::iterator end = this->dev_attr_list_.end();
  while (cur != end) {
    if ((*cur).name == _attr_name) {
      return indx;
    }
    cur++; indx++;
  }
  Tango::Except::throw_exception(_CPTC("attribute not found"),
			                           _CPTC("attribute could not be found in local attribute list"),
			                           _CPTC("DeviceDesc::attr_exists"));
  return kError;
}

//=============================================================================
// DeviceDesc::is_attr_writable
//=============================================================================
bool DeviceDesc::is_attr_writable (const std::string& _attr_name)
{ 
  return this->is_attr_writable(this->attr_exists(_attr_name));
}

//=============================================================================
// DeviceDesc::is_attr_writable
//=============================================================================
bool DeviceDesc::is_attr_writable(int _attr_id)
{
  if (_attr_id < 0 || static_cast<size_t>(_attr_id) >= this->dev_attr_list_.size())	{
    return false;
  }
  switch (this->dev_attr_list_[_attr_id].writable) {
    case Tango::READ:
    case Tango::READ_WITH_WRITE:
      return false;
    default:
      break;
  }
  return true;
}

//=============================================================================
// DeviceDesc::is_attr_readable
//=============================================================================
bool DeviceDesc::is_attr_readable (const std::string& _attr_name)
{ 
  return this->is_attr_writable(this->attr_exists(_attr_name));
}

//=============================================================================
// DeviceDesc::is_attr_readable
//=============================================================================
bool DeviceDesc::is_attr_readable(int _attr_id)
{
  if (_attr_id < 0 ||static_cast<size_t>(_attr_id) >= this->dev_attr_list_.size())	{
    return false;
  }
  switch (this->dev_attr_list_[_attr_id].writable) {
    case Tango::WRITE:
      return false;
      break;
    default:
      break;
  }
  return true; 
}

//=============================================================================
// DeviceDesc::cmd_list
//=============================================================================
void DeviceDesc::cmd_list (Tango::CommandInfoList* _cmd_list)
{
  this->dev_cmd_list_.clear();
  if (_cmd_list == 0) {
    return;
  }
  DeviceDesc::MinCmdInfoList::size_type s = _cmd_list->size();
  this->dev_cmd_list_.resize(s);
  for (DeviceDesc::MinCmdInfoList::size_type i = 0; i < s; i++) {
    this->dev_cmd_list_[i].name = (*_cmd_list)[i].cmd_name;
    this->dev_cmd_list_[i].in_type = (*_cmd_list)[i].in_type;
    this->dev_cmd_list_[i].out_type = (*_cmd_list)[i].out_type;
  }
  delete _cmd_list;
}

//=============================================================================
// DeviceDesc::attr_list
//=============================================================================
void DeviceDesc::attr_list (Tango::AttributeInfoList* _attr_list)
{
  this->dev_attr_list_.clear();
  if (_attr_list == 0) {
    return;
  }
  DeviceDesc::MinAttrInfoList::size_type s = _attr_list->size();
  this->dev_attr_list_.resize(s);
  for (DeviceDesc::MinAttrInfoList::size_type i = 0; i < s; i++) {
    this->dev_attr_list_[i].name = (*_attr_list)[i].name;
    this->dev_attr_list_[i].writable = (*_attr_list)[i].writable;
    this->dev_attr_list_[i].data_format = (*_attr_list)[i].data_format;
    this->dev_attr_list_[i].data_type = (*_attr_list)[i].data_type;
  }
  delete _attr_list;
}

//=============================================================================
// DeviceRepository::init
//=============================================================================
int DeviceRepository::init (void)
{
  if (DeviceRepository::instance_ != 0)
    return 0;

  DeviceRepository::instance_ = new DeviceRepository;

  return (DeviceRepository::instance_) ? 0 : -1;
}

//=============================================================================
// DeviceRepository::cleanup
//=============================================================================
void DeviceRepository::cleanup (void)
{
  if (DeviceRepository::instance_) {
    delete DeviceRepository::instance_;
    DeviceRepository::instance_ = 0;
  }
}

//=============================================================================
// DeviceRepository::DeviceRepository 
//=============================================================================
DeviceRepository::DeviceRepository (void)
{

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
DeviceDesc* DeviceRepository::device_desc (const std::string& _device_name)
    throw (Tango::DevFailed)
{
  //- critical section
  omni_mutex_lock guard(this->lock_);

  //- convert to lower case
  std::string dev_name(_device_name);
  std::transform(dev_name.begin(), dev_name.end(), dev_name.begin(), ::tolower);
  //- search for <device_name> in the repository
  DeviceDescMapIterator it = this->dev_map_.find(dev_name);
  if (it != this->dev_map_.end() && it->second) {
    return it->second;
  }
  //- search failed, create the descriptor
  DeviceDesc* ddesc = new DeviceDesc(dev_name);
  if (ddesc == 0) {
    Tango::Except::throw_exception(_CPTC("out of memory"),
			                             _CPTC("binding error: memory allocation failed"),
			                             _CPTC("DeviceRepository::device_desc"));
  }
  //- instanciate the device proxy
  try {
    ddesc->dev_proxy_ = new ThreadSafeDeviceProxy(const_cast<std::string&>(dev_name));
    if (ddesc->dev_proxy_ == 0) {
      Tango::Except::throw_exception(_CPTC("out of memory"),
			                               _CPTC("binding error: memory allocation failed"),
			                               _CPTC("DeviceRepository::device_desc"));
    }
	}
	catch (Tango::DevFailed &e) {
		delete ddesc;
    std::string d = "binding error: failed to create proxy for device " + dev_name;
    Tango::Except::re_throw_exception(e,
                                      _CPTC("Tango::DeviceProxy instanciation failed"),
			                                _CPTC(d.c_str()),
			                                _CPTC("DeviceRepository::device_desc"));
	}
  catch (...) {
    delete ddesc;
    Tango::Except::throw_exception(_CPTC("unknown error"),
			                             _CPTC("binding error: unknown exception caught"),
			                             _CPTC("DeviceRepository::device_desc"));
  }
  //- enable transparent reconnection
  ddesc->dev_proxy_->unsafe_proxy().set_transparency_reconnection(true);
  //- get device commands list & info
  try {
    ddesc->cmd_list(ddesc->dev_proxy_->command_list_query());
  }
  catch (Tango::DevFailed &e) {
		delete ddesc;
    Tango::Except::re_throw_exception(e,
                                      _CPTC("command_list_query failed"),
			                                _CPTC("binding error: failed to get device commands list"),
			                                _CPTC("DeviceRepository::device_desc"));
  }
  catch (...) {
    delete ddesc;
    Tango::Except::throw_exception(_CPTC("unknown error"),
			                             _CPTC("binding error: unknown exception caught"),
			                             _CPTC("DeviceRepository::device_desc"));
  }
  //- get device attributes list 
  try {
    ddesc->attr_list(ddesc->dev_proxy_->attribute_list_query());  
  }
  catch (Tango::DevFailed &e) {
    //--TODO::C++ and Java servers have different behavior!!! 
    //--a C++ server return an empty list if it has no attributes
    //--whereas a Java server throw an exception. Both should 
    //--return an empty list. A quick and dirty workaround...
    std::string reason("API_AttrNotFound");
    if (reason != std::string(e.errors[0].reason)) {  
		  delete ddesc;
      Tango::Except::re_throw_exception(e,
                                        _CPTC("attribute_list_query failed"),
			                                  _CPTC("binding error: failed to get device attributes list"),
			                                  _CPTC("DeviceRepository::device_desc"));
    }
    else {
      ddesc->attr_list(0);
    }
  }
  catch (...) {
    delete ddesc;
    Tango::Except::throw_exception(_CPTC("unknown error"),
			                             _CPTC("binding error: failed to get device attributes list"),
			                             _CPTC("DeviceRepository::device_desc"));
  }
  //- store the device descriptor into the repository
  std::pair<DeviceDescMapIterator, bool> result;
  result = this->dev_map_.insert(DeviceDescMap::value_type(dev_name, ddesc));
  if (result.second == 0) {
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
int DeviceRepository::remove_device_i(const std::string& _device_name)
{
  if (_device_name == "*") {
    DeviceDescMapIterator cur = this->dev_map_.begin();
    DeviceDescMapIterator end = this->dev_map_.end();
    for (; cur != end; cur++) {
      if (cur->second) {
	      delete cur->second;
      }
    } 
    this->dev_map_.clear();
  }
  else {
    std::string dev_name(_device_name);
    std::transform(dev_name.begin(), dev_name.end(), dev_name.begin(), ::tolower);
    DeviceDescMapIterator it = this->dev_map_.find(dev_name);
    if (it != this->dev_map_.end()) {
      if (it->second) {
        delete it->second;
      }
      this->dev_map_.erase(dev_name);
    }
  }

  return kNoError;
}


