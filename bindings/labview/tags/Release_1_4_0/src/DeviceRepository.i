// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for LabVIEW
//
// = FILENAME
//   DeviceRepository.i
//
// = AUTHOR
//   Nicolas Leclercq - SOLEIL
//
// ============================================================================

//=============================================================================
// DeviceDesc::name
//=============================================================================
LV_INLINE const std::string& DeviceDesc::name (void) const
{
  return this->dev_name_;
}

//=============================================================================
// DeviceDesc::proxy
//=============================================================================
LV_INLINE ThreadSafeDeviceProxy* DeviceDesc::proxy (void) const
{
  return this->dev_proxy_;
}

//=============================================================================
// DeviceDesc::cmd_list
//=============================================================================
LV_INLINE const DeviceDesc::MinCmdInfoList& DeviceDesc::cmd_list (void) const
{
  return this->dev_cmd_list_;
}

//=============================================================================
// DeviceDesc::attr_list
//=============================================================================
LV_INLINE const DeviceDesc::MinAttrInfoList& DeviceDesc::attr_list (void) const
{
  return this->dev_attr_list_;
}

//=============================================================================
// DeviceDesc::cmd_exists 
//=============================================================================
LV_INLINE int DeviceDesc::cmd_exists (const char* _cmd_name)
    throw (Tango::DevFailed)
{
  return this->cmd_exists(std::string(_cmd_name));
}

//=============================================================================
// DeviceDesc::attr_exists 
//=============================================================================
LV_INLINE int DeviceDesc::attr_exists (const char* _attr_name)
    throw (Tango::DevFailed)
{
  return this->attr_exists(std::string(_attr_name));
}

//=============================================================================
// DeviceRepository::instance
//=============================================================================
LV_INLINE DeviceRepository* DeviceRepository::instance (void)
{
  return DeviceRepository::instance_;
}

//=============================================================================
// DeviceRepository::device_desc
//=============================================================================
LV_INLINE DeviceDesc* DeviceRepository::device_desc (const char* device_name)
    throw (Tango::DevFailed)
{
  return this->device_desc(std::string(device_name));
}

