// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for LabVIEW
//
// = FILENAME
//   DeviceRepository.h
//
// = AUTHOR
//   Nicolas Leclercq - SOLEIL
//
// ============================================================================

#ifndef _DEVICE_REPOSITORY_H_
#define _DEVICE_REPOSITORY_H_

//=============================================================================
// DEPENDENCIEs
//=============================================================================
#include <map>
#include "StandardHeader.h"
#include "DeviceDescriptor.h"

//=============================================================================
// SHORTCUT
//=============================================================================
#define DEV_REP (DeviceRepository::instance())

//=============================================================================
// CLASS : DeviceRepository 
//=============================================================================
class DeviceRepository 
{
  friend class ObjectManager;

  typedef std::map<std::string, DeviceDescriptor*> DeviceDescriptorMap;
  // DeviceMap : a STL map of <DeviceDescriptor*> indexed by device name

  typedef DeviceDescriptorMap::iterator DeviceDescriptorMapIterator;
  // DeviceMapIterator : a DeviceMap iterator

public:

  static DeviceRepository * instance (void);
  //- Returns the unique instance of <DeviceRepository>.

  DeviceDescriptor * device_desc (const char* device_name)
    throw (Tango::DevFailed);
  // Providing a device name, returns its device descriptor.

  DeviceDescriptor * device_desc (const std::string& device_name)
    throw (Tango::DevFailed);
  // Providing a device name, returns its device descriptor

  DeviceDescriptor * device_desc (Tango::DeviceProxy * device_proxy, bool ownership = false)
    throw (Tango::DevFailed);
  // Providing a device proxy, returns its device descriptor

  void remove_device (const std::string& device_name);
  //- Providing a device name, returns removes it from the repository.

private:
  static void init (void)
    throw (Tango::DevFailed);
  //- Instanciates the singleton.
  
  static void cleanup (void)
    throw (Tango::DevFailed);
  //- Releases the singleton.

  static DeviceRepository * instance_;
  //- The unique instance of <DeviceRepository>.

  void remove_device_i (const std::string& device_name);
  //- Providing a device name, returns removes it from the repository.

  DeviceDescriptorMap dev_map_;
  //- The actual repository (see typedef above).

  omni_mutex lock_;
  //- Use a lock in order to make the repository thread-safe.

  // = Disallow these operations (except for ObjectManager).
  //-------------------------------------------------------
  DeviceRepository();
  DeviceRepository (const DeviceRepository&);
  virtual ~DeviceRepository();
  void operator= (const DeviceRepository&); 
};

#if defined (_LV_INLINE_)
#  include "DeviceRepository.i"
#endif 

#endif // _DEVICE_REPOSITORY_H_

   
   
