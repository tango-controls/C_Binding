// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for LabVIEW
//
// = FILENAME
//   DeviceDescriptorriptor.i
//
// = AUTHOR
//   Nicolas Leclercq - SOLEIL
//
// ============================================================================

//=============================================================================
// DeviceDescriptor::proxy
//=============================================================================
LV_INLINE ThreadSafeDeviceProxy * 
DeviceDescriptor::proxy (void) const
{
  return this->dev_proxy_;
}

//=============================================================================
// DeviceDescriptor::operator->
//=============================================================================
LV_INLINE ThreadSafeDeviceProxy * 
DeviceDescriptor::operator-> (void) const
{
  return this->dev_proxy_;
}

//=============================================================================
// DeviceDescriptor::name
//=============================================================================
LV_INLINE const std::string& 
DeviceDescriptor::name (void) const
{
  return this->dev_proxy_->dev_name();
}

//=============================================================================
// DeviceDescriptor::cmd_list
//=============================================================================
LV_INLINE const DeviceDescriptor::MinCmdInfoList& 
DeviceDescriptor::cmd_list (void) const
{
  return this->dev_cmd_list_;
}

//=============================================================================
// DeviceDescriptor::cmd_list
//=============================================================================
LV_INLINE const DeviceDescriptor::MinAttrInfoList& 
DeviceDescriptor::attr_list (void) const
{
  return this->dev_attr_list_;
}

//=============================================================================
// DeviceDescriptor::cmd_exists 
//=============================================================================
LV_INLINE int 
DeviceDescriptor::cmd_exists (const char* _cmd_name)
    throw (Tango::DevFailed)
{
  return this->cmd_exists(std::string(_cmd_name));
}

//=============================================================================
// DeviceDescriptor::attr_exists 
//=============================================================================
LV_INLINE int 
DeviceDescriptor::attr_exists (const char* _attr_name)
    throw (Tango::DevFailed)
{
  return this->attr_exists(std::string(_attr_name));
}

