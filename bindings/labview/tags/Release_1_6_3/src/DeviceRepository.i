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
// DeviceRepository::instance
//=============================================================================
LV_INLINE DeviceRepository * DeviceRepository::instance (void)
{
  return DeviceRepository::instance_;
}

//=============================================================================
// DeviceRepository::device_desc
//=============================================================================
LV_INLINE DeviceDescriptor * 
DeviceRepository::device_desc (const char* _device_name)
    throw (Tango::DevFailed)
{
  return this->device_desc(std::string(_device_name));
}
