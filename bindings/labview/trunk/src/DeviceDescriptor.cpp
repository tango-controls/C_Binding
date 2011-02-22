// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for LabVIEW
//
// = FILENAME
//   DeviceDescriptorriptor.cpp
//
// = AUTHOR
//   Nicolas Leclercq - SOLEIL
//
// ============================================================================

//=============================================================================
// DEPENDENCIES
//=============================================================================
#include "StandardHeader.h"
#include "DeviceDescriptor.h"

#if !defined (_LV_INLINE_)
# include "DeviceDescriptor.i"
#endif 

//=============================================================================
// DeviceDescriptor::DeviceDescriptor
//=============================================================================
DeviceDescriptor::DeviceDescriptor (const std::string& _dev_name) 
  : dev_cmd_list_(0), 
    dev_attr_list_(0),
    dev_proxy_ (0)
{
  //- instanciate the device proxy
  try 
  {
    this->dev_proxy_ = new ThreadSafeDeviceProxy(_dev_name);
    if (this->dev_proxy_ == 0) 
    {
      Tango::Except::throw_exception(_CPTC("out of memory"),
			                               _CPTC("binding error: memory allocation failed"),
			                               _CPTC("DeviceDescriptor::DeviceDescriptor"));
    }
	}
	catch (Tango::DevFailed &e) 
  {
    std::string d = "binding error: failed to create proxy for device " + _dev_name;
    Tango::Except::re_throw_exception(e,
                                      _CPTC("Tango::DeviceProxy instanciation failed"),
			                                _CPTC(d.c_str()),
			                                _CPTC("DeviceDescriptor::DeviceDescriptor"));
	}
  catch (...) 
  {
    Tango::Except::throw_exception(_CPTC("unknown error"),
			                             _CPTC("binding error: unknown exception caught"),
			                             _CPTC("DeviceDescriptor::DeviceDescriptor"));
  }

  //- enable transparent reconnection
  this->dev_proxy_->unsafe_proxy().set_transparency_reconnection(true);

  //- get device attributes and commands info
  this->get_device_interface();
}

//=============================================================================
// DeviceDescriptor::DeviceDescriptor 
//=============================================================================
DeviceDescriptor::DeviceDescriptor (Tango::DeviceProxy * _dp, bool _ownership)
  : dev_cmd_list_(0), 
    dev_attr_list_(0),
    dev_proxy_ (0)
{
  //- instanciate the device proxy
  try 
  {
    this->dev_proxy_ = new ThreadSafeDeviceProxy(_dp, _ownership);
    if (this->dev_proxy_ == 0) 
    {
      Tango::Except::throw_exception(_CPTC("out of memory"),
			                               _CPTC("binding error: memory allocation failed"),
			                               _CPTC("DeviceDescriptor::DeviceDescriptor"));
    }
	}
	catch (Tango::DevFailed &e) 
  {
    std::string d = "binding error: failed to create proxy for device " + _dp->dev_name();
    Tango::Except::re_throw_exception(e,
                                      _CPTC("Tango::DeviceProxy instanciation failed"),
			                                _CPTC(d.c_str()),
			                                _CPTC("DeviceDescriptor::DeviceDescriptor"));
	}
  catch (...) 
  {
    Tango::Except::throw_exception(_CPTC("unknown error"),
			                             _CPTC("binding error: unknown exception caught"),
			                             _CPTC("DeviceDescriptor::DeviceDescriptor"));
  }

  //- enable transparent reconnection
  //- _dp->set_transparency_reconnection(true);

  //- get device attributes and commands info
  this->get_device_interface();
}

//=============================================================================
// DeviceDescriptor::~DeviceDescriptor 
//=============================================================================
DeviceDescriptor::~DeviceDescriptor (void)
{
  this->dev_cmd_list_.clear();
  this->dev_attr_list_.clear();
  this->disconnect();
}

//=============================================================================
// DeviceDescriptor::disconnect
//=============================================================================
void DeviceDescriptor::disconnect (void)
{
  if (this->dev_proxy_) 
  {
    delete this->dev_proxy_;
    this->dev_proxy_ = 0;
  }
}

//=============================================================================
// DeviceDescriptor::get_device_interface
//=============================================================================
void DeviceDescriptor::get_device_interface (void)
  throw (Tango::DevFailed) 
{
  if (! this->dev_proxy_)
  {
    Tango::Except::throw_exception(_CPTC("binding error"),
                                   _CPTC("unexpected NULL device proxy"),
			                             _CPTC("DeviceDescriptor::get_device_interface"));
  }

  //- get device commands list & info
  try 
  {
    this->cmd_list(this->dev_proxy_->command_list_query());
  }
  catch (Tango::DevFailed &e) 
  {
    Tango::Except::re_throw_exception(e,
                                      _CPTC("command_list_query failed"),
			                                _CPTC("binding error: failed to get device commands list"),
			                                _CPTC("DeviceDescriptor::get_device_interface"));
  }
  catch (...) 
  {
    Tango::Except::throw_exception(_CPTC("unknown error"),
			                             _CPTC("binding error: unknown exception caught"),
			                             _CPTC("DeviceDescriptor::get_device_interface"));
  }

  //- get device attributes list 
  try 
  {
    this->attr_list(this->dev_proxy_->attribute_list_query());  
  }
  catch (Tango::DevFailed &e) 
  {
    //--TODO::C++ and Java servers have different behavior!!! 
    //--a C++ server return an empty list if it has no attributes
    //--whereas a Java server throw an exception. Both should 
    //--return an empty list. A quick and dirty workaround...
    std::string reason("API_AttrNotFound");
    if (reason != std::string(e.errors[0].reason)) 
    {  
      Tango::Except::re_throw_exception(e,
                                        _CPTC("attribute_list_query failed"),
			                                  _CPTC("binding error: failed to get device attributes list"),
			                                  _CPTC("DeviceDescriptor::get_device_interface"));
    }
    else 
    {
      this->attr_list(0);
    }
  }
  catch (...) 
  {
    Tango::Except::throw_exception(_CPTC("unknown error"),
			                             _CPTC("binding error: failed to get device attributes list"),
			                             _CPTC("DeviceDescriptor::get_device_interface"));
  }
}

//=============================================================================
// DeviceDescriptor::cmd_exists 
//=============================================================================
int DeviceDescriptor::cmd_exists (const std::string& _cmd_name)
  throw (Tango::DevFailed)
{
  int indx = 0;
  DeviceDescriptor::MinCmdInfoList::iterator cur = this->dev_cmd_list_.begin();
  DeviceDescriptor::MinCmdInfoList::iterator end = this->dev_cmd_list_.end();
  while (cur != end) 
  {
    if ((*cur).name == _cmd_name)
      return indx;
    cur++; 
    indx++;
  }
  Tango::Except::throw_exception(_CPTC("command not found"),
			                           _CPTC("command could not be found in local command list"),
			                           _CPTC("DeviceDescriptor::cmd_exists"));
  return NO_SUCH_CMD;
}

//=============================================================================
// DeviceDescriptor::attr_exists 
//=============================================================================
int DeviceDescriptor::attr_exists (const std::string& _attr_name)
  throw (Tango::DevFailed)
{
  int indx = 0;
  DeviceDescriptor::MinAttrInfoList::iterator cur = this->dev_attr_list_.begin();
  DeviceDescriptor::MinAttrInfoList::iterator end = this->dev_attr_list_.end();
  while (cur != end) 
  {
    if ((*cur).name == _attr_name)
      return indx;
    cur++; 
    indx++;
  }
  Tango::Except::throw_exception(_CPTC("attribute not found"),
			                           _CPTC("attribute could not be found in local attribute list"),
			                           _CPTC("DeviceDescriptor::attr_exists"));
  return NO_SUCH_ATTR;
}

//=============================================================================
// DeviceDescriptor::is_attr_writable
//=============================================================================
bool DeviceDescriptor::is_attr_writable (const std::string& _attr_name)
{ 
  return this->is_attr_writable(this->attr_exists(_attr_name));
}

//=============================================================================
// DeviceDescriptor::is_attr_writable
//=============================================================================
bool DeviceDescriptor::is_attr_writable (int _attr_id)
{
  if (_attr_id < 0 || static_cast<size_t>(_attr_id) >= this->dev_attr_list_.size())	
    return false;

  switch (this->dev_attr_list_[_attr_id].writable) 
  {
    case Tango::READ:
    case Tango::READ_WITH_WRITE:
      return false;
    default:
      break;
  }

  return true;
}

//=============================================================================
// DeviceDescriptor::is_attr_readable
//=============================================================================
bool DeviceDescriptor::is_attr_readable (const std::string& _attr_name)
{ 
  return this->is_attr_writable(this->attr_exists(_attr_name));
}

//=============================================================================
// DeviceDescriptor::is_attr_readable
//=============================================================================
bool DeviceDescriptor::is_attr_readable (int _attr_id)
{
  if (_attr_id < 0 ||static_cast<size_t>(_attr_id) >= this->dev_attr_list_.size())
    return false;
 
  switch (this->dev_attr_list_[_attr_id].writable) 
  {
    case Tango::WRITE:
      return false;
      break;
    default:
      break;
  }

  return true; 
}

//=============================================================================
// DeviceDescriptor::cmd_list
//=============================================================================
void DeviceDescriptor::cmd_list (Tango::CommandInfoList* _cmd_list)
{
  this->dev_cmd_list_.clear();

  if (_cmd_list == 0)
    return;

  DeviceDescriptor::MinCmdInfoList::size_type s = _cmd_list->size();

  this->dev_cmd_list_.resize(s);

  for (DeviceDescriptor::MinCmdInfoList::size_type i = 0; i < s; i++) 
  {
    this->dev_cmd_list_[i].name = (*_cmd_list)[i].cmd_name;
    this->dev_cmd_list_[i].in_type = (*_cmd_list)[i].in_type;
    this->dev_cmd_list_[i].out_type = (*_cmd_list)[i].out_type;
  }

  delete _cmd_list;
}

//=============================================================================
// DeviceDescriptor::attr_list
//=============================================================================
void DeviceDescriptor::attr_list (Tango::AttributeInfoList* _attr_list)
{
  this->dev_attr_list_.clear();

  if (_attr_list == 0)
    return;

  DeviceDescriptor::MinAttrInfoList::size_type s = _attr_list->size();

  this->dev_attr_list_.resize(s);

  for (DeviceDescriptor::MinAttrInfoList::size_type i = 0; i < s; i++) 
  {
    this->dev_attr_list_[i].name = (*_attr_list)[i].name;
    this->dev_attr_list_[i].writable = (*_attr_list)[i].writable;
    this->dev_attr_list_[i].data_format = (*_attr_list)[i].data_format;
    this->dev_attr_list_[i].data_type = (*_attr_list)[i].data_type;
  }

  delete _attr_list;
}

