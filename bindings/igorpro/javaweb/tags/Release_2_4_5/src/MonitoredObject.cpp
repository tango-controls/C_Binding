// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for Igor Pro
//
// = FILENAME
//   MonitoredObject.cpp
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

// ============================================================================
// DEPENDENCIES
// ============================================================================
#include <iostream>
#include "XDK_StandardHeaders.h"
#include <yat/utils/XString.h>
#include "TangoBinding.h"
#include "DevRepository.h"
#include "DataCodec.h"
#include "MonitoredObject.h"

// ============================================================================
// STATICs
// ============================================================================
ClientId MonitoredObject::next_client_id = -1;

// ============================================================================
// MonitoredObject::MonitoredObject
// ============================================================================
MonitoredObject::MonitoredObject(const std::string& _tdev_name,
                                 const std::string& _tobj_name)
	: tango_dev_name_ (_tdev_name),
    tango_obj_name_ (_tobj_name),
    min_pp_ (0x0FFFFFFF),
    error_counter_ (0),
    updated_ (false)
{
  //- noop
}

// ============================================================================
// MonitoredObject::~MonitoredObject
// ============================================================================
MonitoredObject::~MonitoredObject ()
{
  //- noop
}

// ============================================================================
// MonitoredObject::register_client
// ============================================================================
ClientId MonitoredObject::register_client (unsigned long _pp,
                                           const std::string& _iop)
{
  //- any client with same <_iop>?
  ClientsMapIt it = this->clients_.begin();
  ClientsMapIt end = this->clients_.end();
  for (; it != end; it++)
  {
    if (_iop == it->second.path)
    {
      it->second.ref_count++;
#if defined (_VERBOSE_)
  		std::cout << "Tango-Binding::MonitoredObject::register_client::"
      					<< "client exists for same dev/attr/export-path - ref.count:"
 								<< it->second.ref_count
                << " - returning existing cid"
                << std::endl;  
#endif
      return it->first;
    }
  }

  //- inc. the client id
  ClientId cid = ++MonitoredObject::next_client_id;

  //- create a new monitor client
  MonitorClient mc;
  mc.pp = _pp;
  mc.path = _iop;

  //- store the MonitoredAttribute into the repository
  std::pair<ClientsMapIt, bool> result;
  result = this->clients_.insert(ClientsMap::value_type(cid, mc));
  if (result.second == 0) 
  {
    XDK_UTILS->set_error("XOP internal error",
                         "ClientsMap::insert failed",
                         "MonitoredObject::register_client");
    return kError;
  }

  //- store the minimum polling period into the dedicated member
  if (_pp < this->min_pp_)
    this->min_pp_ = _pp;

  return cid;
}

// ============================================================================
// MonitoredObject::unregister_client 
// ============================================================================
void MonitoredObject::unregister_client (ClientId _c)
{
  //- find client
  ClientsMapIt it = this->clients_.find(_c);

  //- no such client
  if (it == this->clients_.end()) 
    return;

  //- store polling period
  unsigned long pp = it->second.pp;

  //- dec ref. counter 
  it->second.ref_count--;

#if defined (_VERBOSE_)
  std::cout << "Tango-Binding::MonitoredObject::unregister_client - ref.cpount:" 
  					<< it->second.ref_count
            << std::endl;
#endif

  //- if still has clients for same igor path, return
  if (it->second.ref_count)
    return;

#if defined (_VERBOSE_)
  std::cout << "Tango-Binding::MonitoredObject::unregister_client"
  					<< " - no more clients - removing item"
            << std::endl; 
#endif

  //- otherwise, remove client from list
  this->clients_.erase(it);

  //- no more client, return 
  if (this->clients_.empty())
    return;

  //- otherwise, compute new min polling period
  if (pp == this->min_pp_)
  {
    ClientsMapIt it = this->clients_.begin();
    ClientsMapIt end = this->clients_.end();
    this->min_pp_ = it->second.pp;
    for (++it; it != end; ++it)
    {
      if (it->second.pp < this->min_pp_)
        this->min_pp_ = it->second.pp;
    }
  }
}

// ============================================================================
// MonitoredObject::client_registered
// ============================================================================
bool MonitoredObject::client_registered (ClientId _c)
{
  ClientsMapIt it = this->clients_.find(_c);

  return it != this->clients_.end();
}

// ============================================================================
// MonitoredObject::has_registered_clients
// ============================================================================
bool MonitoredObject::has_registered_clients (void)
{
  return ! this->clients_.empty();
}

//=============================================================================
// MonitoredObject::suspend
//=============================================================================
int MonitoredObject::suspend (ClientId _c)
{
  if (_c == -1)
  {
    ClientsMapIt it = this->clients_.begin();
    ClientsMapIt end = this->clients_.end();
    for (; it != end; ++it)
      it->second.suspended = true;
  }
  else
  {
    ClientsMapIt it = this->clients_.find(_c);
    if (it != this->clients_.end()) 
      it->second.suspended = true;
  }

  return kNoError;
}

//=============================================================================
// MonitoredObject::resume
//=============================================================================
int MonitoredObject::resume (ClientId _c)
{
  if (_c == -1)
  {
    ClientsMapIt it = this->clients_.begin();
    ClientsMapIt end = this->clients_.end();
    for (; it != end; ++it)
      it->second.suspended = false;
  }
  else
  {
    ClientsMapIt it = this->clients_.find(_c);
    if (it != this->clients_.end()) 
      it->second.suspended = false;
  }

  return kNoError;
}

//=============================================================================
// MonitoredObject::change_polling_period
//=============================================================================
int MonitoredObject::change_polling_period (unsigned long _pp, ClientId _c)
{
  if (_c == -1)
  {
    ClientsMapIt it = this->clients_.begin();
    ClientsMapIt end = this->clients_.end();
    for (; it != end; ++it)
      it->second.pp = _pp;
    this->min_pp_ = _pp;
  }
  else
  {
    ClientsMapIt it = this->clients_.find(_c);
    ClientsMapIt end = this->clients_.end();
    if (it != end) 
    {
      it->second.pp = _pp;
      it = this->clients_.begin();
      this->min_pp_ = it->second.pp;
      for (++it; it != end; ++it)
      {
        if (it->second.pp < this->min_pp_)
          this->min_pp_ = it->second.pp;
      }
    }
  }

  return kNoError;
}

// ============================================================================
// MonitoredAttribute::MonitoredAttribute
// ============================================================================
MonitoredAttribute::MonitoredAttribute(const std::string& _tdev_name,
                                       const std::string& _tobj_name,
                                       int _attr_id)
	: MonitoredObject(_tdev_name, _tobj_name),
    attr_id_ (_attr_id)
{
  //- noop
}

// ============================================================================
// MonitoredAttribute::~MonitoredAttribute
// ============================================================================
MonitoredAttribute::~MonitoredAttribute ()
{
  //- noop
}

// ============================================================================
// MonitoredAttribute::attach_error
// ============================================================================
void MonitoredAttribute::attach_error ()
{
#if defined (_VERBOSE_)
	std::cout << "MonitoredAttribute::attach_error to "
  					<< this->tango_dev_name_
            << "/"
            << this->tango_obj_name_
            << " [err-cnt:"
            << this->error_counter_
            << "]"
            << std::endl;
#endif 

  //- if (! this->error_counter_)
  //- {
  	this->value_ = Tango::DeviceAttribute();
    this->updated_ = true;
  //- }
  
  this->error_counter_++;

  this->import_timer_.restart();
}

// ============================================================================
// MonitoredAttribute::attach_value 
// ============================================================================
void MonitoredAttribute::attach_value (const Tango::DeviceAttribute & _av)
{
  if (  
        const_cast<Tango::DeviceAttribute&>(_av).has_failed() 
      ||
        const_cast<Tango::DeviceAttribute&>(_av).is_empty()
     )
  {
    this->attach_error();
  }
  else
  {
#if defined (_VERBOSE_)
	std::cout << "MonitoredAttribute::attach_value to "
  					<< this->tango_dev_name_
            << "/"
            << this->tango_obj_name_
            << " [err-cnt:"
            << this->error_counter_
            << "]"
            << std::endl;
#endif 
    this->value_ = _av;
    this->error_counter_ = 0;
    this->updated_ = true;
    this->import_timer_.restart();
  }
}

// ============================================================================
// MonitoredAttribute::export_value
// ============================================================================
void MonitoredAttribute::export_value ()
{
  //- is attribute value updated ?
	if (! this->updated_)
		return;

  //- attribute idenfifier
  int attr_id;

  //- get dev desc
  DevDescriptor * ddesc = DEV_REP->device_desc(this->tango_dev_name_);
  if (ddesc == 0) 
  {
    //- use the stored attribute id
    attr_id = this->attr_id_;
    this->attach_error();
  }
  else
  {
    //- get actual attr_id (may have change if device restarted after interface changes)
    attr_id = ddesc->attr_exists(this->tango_obj_name_);
    if (attr_id == kError) 
    {
      //- use the stored attribute id
      attr_id = this->attr_id_;
    	this->attach_error();
    }
  }

  ClientsMapIt it = this->clients_.begin();
  ClientsMapIt end = this->clients_.end();

  for (; it != end; ++it)
  {
    if (! it->second.suspended && it->second.export_timer_.elapsed_msec() >= (0.9 * it->second.pp))
    {
#if defined (_VERBOSE_)
  		std::cout << "Tango-Binding::exporting monitor value for "
            		<< this->tango_dev_name_
                << "/"
            		<< this->tango_obj_name_
                << " to "
                << it->second.path
                << std::endl;  
			yat::Timer t;
#endif

      //- decode attribute value and export it to Igor
			try 
      {
      	DATA_CODEC->decode_attr(ddesc, 
                                it->second.path, 
                                attr_id, 
                                this->value_,
                                true,
                                true);
      }
      catch (...) 
      {
        //- noop
      }                   
      
#if defined (_VERBOSE_)
  		std::cout << "DATA_CODEC::decode_attr took "
      					<< t.elapsed_msec()
                << " msecs"
      					<< std::endl;
#endif

      this->updated_ = false;
        
      it->second.export_timer_.restart();
		}
	}
}

// ============================================================================
// MonitoredDevice::MonitoredDevice
// ============================================================================
MonitoredDevice::MonitoredDevice (const std::string& _tdev_name)
	: tango_dev_name_(_tdev_name)
{

}

// ============================================================================
// MonitoredDevice::~MonitoredDevice
// ============================================================================
MonitoredDevice::~MonitoredDevice ()
{
  this->remove_attribute("*", -1); 
}

// ============================================================================
// MonitoredDevice::add_attribute
// ============================================================================
int MonitoredDevice::add_attribute (const std::string& attr_name,
                                    const std::string& full_path_to_igor_obj,
                                    unsigned long polling_period,
                                    long& client_id)
{
#if defined (_VERBOSE_)
	std::cout << "Tango-Binding::adding "
            << attr_name
            << " to " 
            << this->tango_dev_name_
            << " monitored attribute list"
            << std::endl;  
#endif

  //- build unique key
  std::string obj = this->tango_dev_name_ + "/" + attr_name;

  //- invalid client id
  client_id = -1;

  {
    //- search for <attr_name> in the monitored attribute repository
    MonitoredAttributeMapIt it = this->mattr_map_.find(attr_name);
  
    //- found ?
    if (it != this->mattr_map_.end() && it->second) 
    {
#if defined (_VERBOSE_)
			std::cout << "Tango-Binding::"
      					<< obj
      					<< " is already monitored - registering new client..."
            		<< std::endl;  

#endif

      //- register client
      client_id = it->second->register_client(polling_period, full_path_to_igor_obj);
      return kNoError;  
    }
  }

  //- be sure the attribute belongs to the device
  DevDescriptor* ddesc = DEV_REP->device_desc(this->tango_dev_name_);
  if (ddesc == 0) 
  {
    std::string r = "failed to monitor " + obj;
    XDK_UTILS->push_error(r.c_str(),
                          "could not obtain a valid device reference",
                          "MonitoredDevice::add_attribute");
    return kError;
  }

  //- is <attr_name> an attribute of <dev_name>
  int attr_id = ddesc->attr_exists(attr_name);
  if (attr_id == kError) 
  {
    std::string d = obj + " is not a valid attribute";
    XDK_UTILS->set_error("API_AttrNotFound",
                         d.c_str(),
                         "MonitoredDevice::add_attribute");
    return kError;
  }

  //- search failed, create the descriptor
  MonitoredAttribute * ma = new MonitoredAttribute(tango_dev_name_,
                                                   attr_name,
                                                   attr_id);
  if (ma == 0) 
  {
    XDK_UTILS->set_error("API_MemoryAllocation",
                         "MonitoredAttribute allocation failed",
                         "MonitoredDevice::add_attribute");
    return kError;
  }

  {
    //- store the MonitoredAttribute into the repository
    std::pair<MonitoredAttributeMapIt, bool> result;
    result = this->mattr_map_.insert(MonitoredAttributeMap::value_type(attr_name, ma));
    if (result.second == 0) 
    {
      XDK_UTILS->set_error("XOP internal error",
                           "MonitoredAttributeMap::insert failed",
                           "MonitoredDevice::add_attribute");
      return kError;
    }

    //- register new client
    client_id = ma->register_client(polling_period, full_path_to_igor_obj);

#if ! defined(_PER_ATTR_POLLING_PERIOD_)
    //- add to attribute name list (optimizes read_attributes calls)
    this->attr_list_.push_back(attr_name);
#endif
  }

  //- read the attribute
  this->import_values(0);

  return kNoError;
}

// ============================================================================
// MonitoredDevice::remove_attribute
// ============================================================================
int MonitoredDevice::remove_attribute (const std::string& attr_name,
                                       long client_id)
{
#if defined (_VERBOSE_)
		std::cout << "Tango-Binding::about to remove monitored attribute "
    					<< "<"
              << attr_name
              << "> for client "
              << client_id
              << " on device "
              << this->tango_dev_name_
              << std::endl;
#endif

  if (attr_name == "*")
  {
    MonitoredAttributeMapIt cur, end;
    if (client_id != -1)
    {
      for (cur = this->mattr_map_.begin(); cur != this->mattr_map_.end(); ++cur)
        delete cur->second;
      this->mattr_map_.clear();
#if ! defined(_PER_ATTR_POLLING_PERIOD_)  
      this->attr_list_.clear();
#endif
    }
    else
    {
      std::vector<std::string> attr_to_be_removed;
      for (cur = this->mattr_map_.begin(); cur != this->mattr_map_.end(); ++cur) 
      {
        cur->second->unregister_client(client_id);
        if (! cur->second->has_registered_clients())
          attr_to_be_removed.push_back(cur->first);
      }
      for (size_t i = 0; i < attr_to_be_removed.size(); i++)   
      {
        MonitoredAttributeMapIt it = this->mattr_map_.find(attr_to_be_removed[i]);
        if (it != this->mattr_map_.end())
        {
          delete cur->second;
          this->mattr_map_.erase(it);
        }
#if ! defined(_PER_ATTR_POLLING_PERIOD_)  
        std::vector<std::string>::iterator vit = this->attr_list_.begin();
        std::vector<std::string>::iterator eov = this->attr_list_.end();
        for (; vit != eov; vit++)
        {
          if (attr_to_be_removed[i] == *vit)
          {
            this->attr_list_.erase(vit);
            break;
          }
        }
#endif
      }
    } 
#if defined (_VERBOSE_)
		std::cout << "Tango-Binding::removed all monitored attributes for client "
    					<< client_id
              << " on device " 
              << this->tango_dev_name_
              << std::endl;
#endif
  }
  else 
  {
#if defined (_VERBOSE_)
		std::cout << "Tango-Binding::removing "
    					<< attr_name
              << " from "
              << this->tango_dev_name_
              << " monitored attribute list for client "
              << client_id
              << std::endl; 
#endif
    MonitoredAttributeMapIt it = this->mattr_map_.find(attr_name);
    if (it != this->mattr_map_.end()) 
    {
      it->second->unregister_client(client_id);

      if (! it->second->has_registered_clients() || client_id == -1)
      {
        delete it->second;
        this->mattr_map_.erase(it);
#if ! defined(_PER_ATTR_POLLING_PERIOD_)  
        std::vector<std::string>::iterator vit = this->attr_list_.begin();
        std::vector<std::string>::iterator eov = this->attr_list_.end();
        for (; vit != eov; vit++)
        {
          if (attr_name == *vit)
          {
            this->attr_list_.erase(vit);
            break;
          }
        }
#endif
#if defined (_VERBOSE_)
				std::cout << "Tango-Binding::monitored attribute "
        					<< attr_name
                  << " removed from "
                  << this->tango_dev_name_
                  << " monitored attributes list"
              		<< std::endl; 
#endif
      }
      else
      {
#if defined (_VERBOSE_)
				std::cout << "Tango-Binding::monitored attribute "
        					<< attr_name
                  << "/"
                  << this->tango_dev_name_
                  << " has client (kept in list)"
              		<< std::endl;
#endif      
      }
    }
  }

  return kNoError;
}

// ============================================================================
// MonitoredDevice::minimum_polling_period
// ============================================================================
unsigned long MonitoredDevice::minimum_polling_period ()
{
  MonitoredAttributeMapIt cur = this->mattr_map_.begin();
  MonitoredAttributeMapIt end = this->mattr_map_.end();

  unsigned long mpp = 0xFFFFFFFF;

  for (; cur != end; ++cur) 
  {
    if (cur->second->min_pp_ < mpp)
      mpp = cur->second->min_pp_;
  } 

  return mpp;
}

// ============================================================================
// MonitoredDevice::change_polling_period
// ============================================================================
int MonitoredDevice::change_polling_period (const std::string& obj_name,
                                            unsigned long polling_period,
                                            long client_id)
{
  //- search for <obj> in the repository
  MonitoredAttributeMapIt it = this->mattr_map_.find(obj_name);

  //- found ?
  if (it != this->mattr_map_.end()) 
    it->second->change_polling_period(polling_period, client_id);

  return kNoError;
}

// ============================================================================
// MonitoredDevice::suspend_monitor
// ============================================================================
int MonitoredDevice::suspend_monitor (const std::string& obj_name,
                                      long client_id)
{
  //- search for <obj_name> in the repository
  MonitoredAttributeMapIt it = this->mattr_map_.find(obj_name);

  //- found ?
  if (it != this->mattr_map_.end()) 
    it->second->suspend(client_id);

  return kNoError;  
}

// ============================================================================
// MonitoredDevice::resume_monitor
// ============================================================================
int MonitoredDevice::resume_monitor (const std::string& obj_name,
                                     long client_id)
{
  //- search for <obj_name> in the repository
  MonitoredAttributeMapIt it = this->mattr_map_.find(obj_name);

  //- found ?
  if (it != this->mattr_map_.end()) 
    it->second->resume(client_id);

  return kNoError;  
}

// ============================================================================
// MonitoredDevice::import_values
// ============================================================================
void MonitoredDevice::import_values (unsigned long ms_to_next_import)
{
#if defined(_PER_ATTR_POLLING_PERIOD_)
  //- actual attribute list to read
  std::vector<std::string> attr_list;

  //- build list of attributes to read
  MonitoredAttributeMapIt cur = this->mattr_map_.begin();
  MonitoredAttributeMapIt end = this->mattr_map_.end();
  for (; cur != end; ++cur) 
  {
      if (
          ! ms_to_next_import
            || 
          cur->second->num_ms_to_polling_period_expiration() < (ms_to_next_import / 2)
         )
      attr_list.push_back(cur->first);
  }
#else
  //- we need this later
  MonitoredAttributeMapIt cur;
  //- attribute list to read
  std::vector<std::string>& attr_list = this->attr_list_; 
#endif

  //- get device descriptor
  DevDescriptor * ddesc = DEV_REP->device_desc(this->tango_dev_name_);
  if (ddesc == 0) 
  {
    for (size_t a = 0; a < attr_list.size(); a++)
    {
      cur = this->mattr_map_.find(attr_list[a]);
      cur->second->attach_error();
    }
  }
  else
  {
    //- read attributes 
    try
    {
      std::vector<Tango::DeviceAttribute> * values;
      values = ddesc->proxy()->read_attributes(attr_list);
      for (size_t a = 0; a < attr_list.size(); a++)
      {
        cur = this->mattr_map_.find(attr_list[a]);
        cur->second->attach_value((*values)[a]);
      }
      delete values;
    }
    catch (...) 
    {
      for (size_t a = 0; a < attr_list.size(); a++)
      {
        cur = this->mattr_map_.find(attr_list[a]);
        cur->second->attach_error();
      }
	  } 
  }
}

// ============================================================================
// MonitoredDevice::export_values
// ============================================================================
void MonitoredDevice::export_values ()
{
  MonitoredAttributeMapIt cur = this->mattr_map_.begin();
  MonitoredAttributeMapIt end = this->mattr_map_.end();

  for (; cur != end; ++cur) 
    cur->second->export_value();
}
