// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for Igor Pro
//
// = FILENAME
//   DevRepository.cpp
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

//=============================================================================
// DEPENDENCIES
//=============================================================================
#include "XDK_StandardHeaders.h"
#include "DevRepository.h"

#if !defined (__XDK_INLINE__)
  #include "DevRepository.i"
#endif 

//=============================================================================
// STATIC MEMBERS
//=============================================================================
DevRepository* DevRepository::instance_ = 0;

//=============================================================================
// DevDescriptor::DevDescriptor
//=============================================================================
DevDescriptor::DevDescriptor (std::string _dev_name) 
  : dev_name_(_dev_name), 
    dev_proxy_(0), 
    dev_df_(0), 
    dev_cmd_list_(0),
    dev_attr_list_(0)
{
  ::MemClear(&this->dev_info_, sizeof(Tango::DeviceInfo));
}

//=============================================================================
// DevDescriptor::~DevDescriptor 
//=============================================================================
DevDescriptor::~DevDescriptor (void)
{
  if (this->dev_proxy_) 
  {
    delete this->dev_proxy_;
    this->dev_proxy_ = 0;
  }

  if (this->dev_cmd_list_) 
  {
    delete this->dev_cmd_list_;
    this->dev_cmd_list_ = 0;
  }  

  if (this->dev_attr_list_) 
  {
    delete this->dev_attr_list_;
    this->dev_attr_list_ = 0;
  }
}

//=============================================================================
// DevDescriptor::cmd_exists 
//=============================================================================
int DevDescriptor::cmd_exists (const std::string& _cmd_name)
{
  int indx = 0;
  Tango::CommandInfoList::iterator cur = this->dev_cmd_list_->begin();
  Tango::CommandInfoList::iterator end = this->dev_cmd_list_->end();
  while (cur != end) 
  {
    if ((*cur).cmd_name == _cmd_name)
      return indx;
    cur++; indx++;
  }
  return -1;
}


//=============================================================================
// DevDescriptor::attr_exists 
//=============================================================================
int DevDescriptor::attr_exists (const std::string& _attr_name)
{
  int indx = 0;
  Tango::AttributeInfoList::iterator cur = this->dev_attr_list_->begin();
  Tango::AttributeInfoList::iterator end = this->dev_attr_list_->end();
  while (cur != end) 
  {
    if ((*cur).name == _attr_name)
      return indx;
    cur++; indx++;
  }
  return -1;
}

//=============================================================================
// DevDescriptor::is_attr_writable
//=============================================================================
bool DevDescriptor::is_attr_writable (const std::string& _attr_name)
{ 
  return this->is_attr_writable(this->attr_exists(_attr_name));
}

//=============================================================================
// DevDescriptor::is_attr_writable
//=============================================================================
bool DevDescriptor::is_attr_writable(int _attr_id)
{
  if (_attr_id < 0 || _attr_id >= this->dev_attr_list_->size())
    return false;

  switch ((*this->dev_attr_list_)[_attr_id].writable) 
  {
    case Tango::READ:
    case Tango::READ_WITH_WRITE:
      return false;
      break;
  }
  return true;
}

//=============================================================================
// DevDescriptor::is_attr_readable
//=============================================================================
bool DevDescriptor::is_attr_readable (const std::string& _attr_name)
{ 
  return this->is_attr_writable(this->attr_exists(_attr_name));
}

//=============================================================================
// DevDescriptor::is_attr_readable
//=============================================================================
bool DevDescriptor::is_attr_readable(int _attr_id)
{
  if (_attr_id < 0 || _attr_id >= this->dev_attr_list_->size())  
    return false;

  switch ((*this->dev_attr_list_)[_attr_id].writable) 
  {
    case Tango::WRITE:
      return false;
      break;
  }
  return true; 
}

//=============================================================================
// DevRepository::init
//=============================================================================
int DevRepository::init (void)
{
  if (DevRepository::instance_ != 0)
    return 0;

  DevRepository::instance_ = new DevRepository;

  return (DevRepository::instance_) ? 0 : -1;
}

//=============================================================================
// DevRepository::cleanup
//=============================================================================
void DevRepository::cleanup (void)
{
  if (DevRepository::instance_) 
  {
    delete DevRepository::instance_;
    DevRepository::instance_ = 0;
  }
}

//=============================================================================
// DevRepository::DevRepository 
//=============================================================================
DevRepository::DevRepository (void)
{

}

//=============================================================================
// DevRepository::~DevRepository 
//=============================================================================
DevRepository::~DevRepository (void)
{
  this->remove_device("*");
  DevRepository::instance_ = 0;
}

//=============================================================================
// DevRepository::device_desc
//=============================================================================
DevDescriptor* DevRepository::device_desc (const std::string& _device_name)
{
  //- search for <device_name> in the repository
  DeviceDescMapIterator it = this->dev_map_.find(_device_name);

  //- found ?
  if (it != this->dev_map_.end() && it->second) 
    return it->second;

  //- search failed, create the descriptor
  DevDescriptor* ddesc = new DevDescriptor(_device_name);
  if (ddesc == 0) 
  {
    XDK_UTILS->set_error("API_MemoryAllocation",
                         "DevDescriptor allocation failed",
                         "DevRepository::device_desc");
    return 0;
  }
  //-- instanciate the device proxy
  try 
  {
    ddesc->dev_proxy_ = new Tango::DeviceProxy(const_cast<std::string&>(_device_name));
    if (ddesc->dev_proxy_ == 0)
    {
      delete ddesc;
      XDK_UTILS->set_error("API_MemoryAllocation",
                           "Tango::DeviceProxy allocation failed",
                           "DevRepository::device_desc");
      return 0;
    }
	}
	catch (const Tango::DevFailed &e) 
  {
		delete ddesc;
    XDK_UTILS->set_error(e);
    std::string d = "failed to create proxy for device " + _device_name;
    XDK_UTILS->push_error("TangoDevProxy instanciation failed",
                          d.c_str(),
                          "DevRepository::device_desc");
		return 0;
	}
  catch (...) 
  {
    delete ddesc;
    XDK_UTILS->set_error("unknown error",
                         "unknown exception caught",
                         "DevRepository::device_desc");
    return 0;
  }
  //- get device commands list & info
  try 
  {
    ddesc->dev_cmd_list_ = ddesc->dev_proxy_->command_list_query();
    ddesc->dev_info_ = ddesc->dev_proxy_->info();
  }
  catch (const Tango::DevFailed &e) 
  {
		delete ddesc;
    XDK_UTILS->set_error(e);
    XDK_UTILS->push_error("command_list_query failed",
                          "failed to get device commands list (Tango::DevFailed exception caught)",
                          "DevRepository::device_desc");
		return 0;
  }
  catch (...) 
  {
    delete ddesc;
    XDK_UTILS->set_error("unknown error",
                         "unknown exception caught",
                         "DevRepository::device_desc");
    return 0;
  }
  //- get device attributes list 
  try 
  {
    ddesc->dev_attr_list_ = ddesc->dev_proxy_->attribute_list_query();  
  }
  catch (const Tango::DevFailed &e) 
  {
		delete ddesc;
    XDK_UTILS->set_error(e);
    XDK_UTILS->push_error("attribute_list_query failed",
                          "failed to get device attributes list (Tango::DevFailed exception caught)",
                          "DevRepository::device_desc");
		return 0;
  }
  catch (...) 
  {
    delete ddesc;
    XDK_UTILS->set_error("unknown error",
                         "failed to get device attributes list (unknown exception caught)",
                         "DevRepository::device_desc");
    return 0;
  }
  //- create device datafolder
  std::string dev_df_name = XDK_UTILS->device_to_df_name(_device_name);
  ddesc->dev_df_ = XDK_UTILS->create_df(dev_df_name);
  if (ddesc->dev_df_ == 0) 
  {
    delete ddesc;
    std::string d = "could not create data folder for device " + _device_name;
    XDK_UTILS->set_error("XOP internal error",
                         d.c_str(),
                         "DevRepository::device_desc");
    return 0;
  }
  //- create dev info df & vars
  DFHndl idf = XDK_UTILS->create_df(dev_df_name + ":info");
  if (idf == 0) 
  {
    delete ddesc;
    std::string d = "could not create <info> datafolder for device " + _device_name;
    XDK_UTILS->set_error("XOP internal error",
                         d.c_str(),
                         "DevRepository::device_desc");
    return 0;
  }
  int err = kNoError;
  do 
  {
    if (XDK_UTILS->create_df_obj(idf, "class", ddesc->dev_info_.dev_class))
      {err = kError; break;}
    if (XDK_UTILS->create_df_obj(idf, "server", ddesc->dev_info_.server_id))
      {err = kError; break;}
    if (XDK_UTILS->create_df_obj(idf, "host", ddesc->dev_info_.server_host))
      {err = kError; break;}
    if (XDK_UTILS->create_df_obj(idf, "version", ddesc->dev_info_.server_version))
      {err = kError; break;}
    if (XDK_UTILS->create_df_obj(idf, "doc_url", ddesc->dev_info_.doc_url))
      {err = kError; break;}
  } while (0);
  if (err != kNoError) 
  {
    delete ddesc;
    std::string d = "could not create <info> variables for device " + _device_name;
    XDK_UTILS->set_error("XOP internal error",
                         d.c_str(),
                         "DevRepository::device_desc");
    return 0;
  }
  //- store the device descriptor into the repository
  std::pair<DeviceDescMapIterator, bool> result;
  result = this->dev_map_.insert(DeviceDescMap::value_type(_device_name, ddesc));
  if (result.second == 0) 
  {
    delete ddesc;
    XDK_UTILS->set_error("XOP internal error",
                         "DeviceMap::insert failed",
                         "DevRepository::device_desc");
    return 0;
  }
#if defined (_XDK_DEBUG_)
  XDK_UTILS->notify("XOP MSG: device %s added to repository", _device_name.c_str());  
#endif
  //- return the device descriptor
  return ddesc;
}

//=============================================================================
// DevRepository::remove_device
//=============================================================================
int DevRepository::remove_device (const std::string& _device_name)
{
  if (_device_name == "*") 
  {
    DeviceDescMapIterator cur = this->dev_map_.begin();
    DeviceDescMapIterator end = this->dev_map_.end();
    for (; cur != end; cur++) 
    {
      if (cur->second) 
        delete cur->second;
    } 
    this->dev_map_.clear();
#if defined (_XDK_DEBUG_)
    XDK_UTILS->notify("XOP MSG: devices repository cleared");  
#endif
  }
  else 
  {
    DeviceDescMapIterator it = this->dev_map_.find(_device_name);
    if (it != this->dev_map_.end()) 
    {
      if (it->second) delete it->second;
      this->dev_map_.erase(_device_name);
#if defined (_XDK_DEBUG_)
    XDK_UTILS->notify("XOP MSG: device %s removed from repository", _device_name.c_str());  
#endif
    }
  }
  return kNoError;
}
