// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for LabVIEW
//
// = FILENAME
//   ThreadSafeDeviceProxy.i
//
// = AUTHOR
//   Nicolas Leclercq - SOLEIL
//
// ============================================================================

//=============================================================================
// ThreadSafeDeviceProxy::state
//=============================================================================
LV_INLINE const std::string
ThreadSafeDeviceProxy::status (void) 
  throw (Tango::DevFailed)
{
  omni_mutex_lock guard(this->lock_);
  return this->dp_.status();
}

//=============================================================================
// ThreadSafeDeviceProxy::state
//=============================================================================
LV_INLINE Tango::DevState
ThreadSafeDeviceProxy::state (void) 
  throw (Tango::DevFailed)
{
  omni_mutex_lock guard(this->lock_);
  return this->dp_.state();
}

//=============================================================================
// ThreadSafeDeviceProxy::ping
//=============================================================================
LV_INLINE int 
ThreadSafeDeviceProxy::ping (void) 
  throw (Tango::DevFailed)
{
  omni_mutex_lock guard(this->lock_);
  return this->dp_.ping();
}

//=============================================================================
// ThreadSafeDeviceProxy::command_list_query
//=============================================================================
LV_INLINE Tango::CommandInfoList* 
ThreadSafeDeviceProxy::command_list_query (void) 
  throw (Tango::DevFailed)
{
  omni_mutex_lock guard(this->lock_);
  return this->dp_.command_list_query();
}

//=============================================================================
// ThreadSafeDeviceProxy::attribute_list_query
//=============================================================================
LV_INLINE Tango::AttributeInfoList* 
ThreadSafeDeviceProxy::attribute_list_query (void) 
  throw (Tango::DevFailed)
{
  omni_mutex_lock guard(this->lock_);
  return this->dp_.attribute_list_query();
}

//=============================================================================
// ThreadSafeDeviceProxy::read_attribute
//=============================================================================
LV_INLINE Tango::DeviceAttribute
ThreadSafeDeviceProxy::read_attribute (const std::string& attr_name) 
  throw (Tango::DevFailed)
{
  omni_mutex_lock guard(this->lock_);
  return this->dp_.read_attribute(const_cast<std::string&>(attr_name));
}

//=============================================================================
// ThreadSafeDeviceProxy::read_attribute
//=============================================================================
LV_INLINE Tango::DeviceAttribute
ThreadSafeDeviceProxy::read_attribute (const char *attr_name) 
  throw (Tango::DevFailed)
{
  omni_mutex_lock guard(this->lock_);
  return this->dp_.read_attribute(attr_name);
}

  Tango::DeviceAttribute read_attributes(const std::vector<std::string>& attr_names) 
    throw (Tango::DevFailed);

//=============================================================================
// ThreadSafeDeviceProxy:read_attributes
//=============================================================================
LV_INLINE std::vector<Tango::DeviceAttribute>*
ThreadSafeDeviceProxy::read_attributes (const std::vector<std::string>& attr_names) 
  throw (Tango::DevFailed)
{
  omni_mutex_lock guard(this->lock_);
  return this->dp_.read_attributes(const_cast<std::vector<std::string>&>(attr_names));
}

//=============================================================================
// ThreadSafeDeviceProxy::write_attribute
//=============================================================================
LV_INLINE void
ThreadSafeDeviceProxy::write_attribute (const Tango::DeviceAttribute& attr_value) 
  throw (Tango::DevFailed)
{
  omni_mutex_lock guard(this->lock_);
  this->dp_.write_attribute(const_cast<Tango::DeviceAttribute&>(attr_value));
}

//=============================================================================
// ThreadSafeDeviceProxy::write_attributes
//=============================================================================
LV_INLINE void
ThreadSafeDeviceProxy::write_attributes (const std::vector<Tango::DeviceAttribute>& attr_values) 
  throw (Tango::DevFailed)
{
  omni_mutex_lock guard(this->lock_);
  this->dp_.write_attributes(const_cast<std::vector<Tango::DeviceAttribute>&>(attr_values));
}

//=============================================================================
// ThreadSafeDeviceProxy::command_inout
//=============================================================================
LV_INLINE Tango::DeviceData
ThreadSafeDeviceProxy::command_inout (const std::string& cmd_name) 
  throw (Tango::DevFailed)
{
  omni_mutex_lock guard(this->lock_);
  return this->dp_.command_inout(const_cast<std::string&>(cmd_name));
}

//=============================================================================
// ThreadSafeDeviceProxy::command_inout
//=============================================================================
LV_INLINE Tango::DeviceData
ThreadSafeDeviceProxy::command_inout (const char * cmd_name) 
  throw (Tango::DevFailed)
{
  omni_mutex_lock guard(this->lock_);
  return this->dp_.command_inout(cmd_name);
}

//=============================================================================
// ThreadSafeDeviceProxy::command_inout
//=============================================================================
LV_INLINE Tango::DeviceData
ThreadSafeDeviceProxy::command_inout (const std::string& cmd_name, 
                                      const Tango::DeviceData &d) 
  throw (Tango::DevFailed)
{
  omni_mutex_lock guard(this->lock_);
  return this->dp_.command_inout(const_cast<std::string&>(cmd_name),
                                  const_cast<Tango::DeviceData &>(d));
}

//=============================================================================
// ThreadSafeDeviceProxy::command_inout
//=============================================================================
LV_INLINE Tango::DeviceData
ThreadSafeDeviceProxy::command_inout (const char * cmd_name, 
                                      const Tango::DeviceData &d) 
  throw (Tango::DevFailed)
{
  omni_mutex_lock guard(this->lock_);
  return this->dp_.command_inout(cmd_name,
                                  const_cast<Tango::DeviceData &>(d));
}

//=============================================================================
// ThreadSafeDeviceProxy::command_inout
//=============================================================================
LV_INLINE int
ThreadSafeDeviceProxy::subscribe_event(const std::string &attr_name, 
                                       Tango::EventType event, 
                                       Tango::CallBack *cb, 
                                       const vector<string> &filters) 
  throw (Tango::DevFailed)
{
  omni_mutex_lock guard(this->lock_);
  return this->dp_.subscribe_event(attr_name, event, cb, filters);
}


//=============================================================================
// ThreadSafeDeviceProxy::unsubscribe_event
//=============================================================================
LV_INLINE void
ThreadSafeDeviceProxy::unsubscribe_event (int event_id)
  throw (Tango::DevFailed)
{
  omni_mutex_lock guard(this->lock_);
  this->dp_.unsubscribe_event(event_id);
}

//=============================================================================
// ThreadSafeDeviceProxy::unsafe_proxy
//=============================================================================
LV_INLINE Tango::DeviceProxy&
ThreadSafeDeviceProxy::unsafe_proxy (void)
{
  return this->dp_;
}


