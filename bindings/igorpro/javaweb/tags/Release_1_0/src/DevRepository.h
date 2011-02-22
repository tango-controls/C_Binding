// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for Igor Pro
//
// = FILENAME
//   DevRepository.h
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

#ifndef _DEVICE_REPOSITORY_H_
#define _DEVICE_REPOSITORY_H_

//=============================================================================
// DEPENDENCIEs
//=============================================================================
#include <map>

//=============================================================================
// FORWARD DECLARATION
//=============================================================================
class TangoApi::DevProxy;
class DevDescriptor;

//=============================================================================
// TYPEDEFs
//=============================================================================
typedef std::map<std::string, DevDescriptor*> DeviceDescMap;
// DeviceMap : a STL map of <DeviceProxy*> indexed by device name.
typedef DeviceDescMap::iterator DeviceDescMapIterator;
// DeviceMapIterator : a DeviceMap iterator;

//=============================================================================
// DEFINEs
//=============================================================================
#define DEV_REP (DevRepository::instance())

//=============================================================================
// CLASS : DevDescriptor
//=============================================================================
class DevDescriptor
{
  friend class DevRepository;

public:
  const std::string& name (void) const;
  // Returns the device name.

  TangoApi::DevProxy* proxy (void) const;
  // Returns the device proxy.

  const TangoApi::DevCmdInfoList& cmd_list (void) const;
  // Returns the device commands list.

  const TangoApi::DevAttrInfoList& attr_list (void) const;
  // Returns the device attributes list.

  DataFolderHandle data_folder (void) const;
  // Returns the device data folder.

  int cmd_exists (const std::string& cmd_name);
  // If <cmd_name> is supported by the device, returns its 
  // indice in <cmd_list>, -1 otherwise.

  int cmd_exists (const char* cmd_name);
  // If <cmd_name> is supported by the device, returns its 
  // indice in <cmd_list>, -1 otherwise.

  int attr_exists (const std::string& attr_name);
  // If <attr_name> is an attribute of the device, returns its 
  // indice in <attr_list>, -1 otherwise.

  int attr_exists (const char* attr_name);
  // If <attr_name> is an attribute of the device, returns its 
  // indice in <attr_list>, -1 otherwise.

  bool is_attr_writable (const std::string& _attr_name);
  // Returns <true> if the attribute named <_attr_name> is 
  // writable, <false> otherwise.

  bool is_attr_writable (int attr_id);
  // Returns <true> if the attribute named <_attr_name> is 
  // writable, <false> otherwise.

  bool is_attr_readable (const std::string& _attr_name);
  // Returns <true> if the attribute named <_attr_name> is 
  // readable, <false> otherwise.

  bool is_attr_readable (int attr_id);
  // Returns <true> if the attribute named <_attr_name> is 
  // readable, <false> otherwise.

private:
  DevDescriptor (std::string _dev_name);
  // Ctor.

  std::string dev_name_;
  // The device name.

  TangoApi::DevProxy* dev_proxy_;
  // The device proxy

  TangoApi::DevCmdInfoList dev_cmd_list_;
  // The device commands list.

  TangoApi::DevAttrInfoList dev_attr_list_;
  // The device commands list.

  TangoApi::DevInfo dev_info_;
  // The device information.

  DataFolderHandle dev_df_;
  // The device datafolder.

  // = Disallow these operations (except for DevRepository).
  //---------------------------------------------------------
  DevDescriptor (const DevDescriptor&);
  virtual ~DevDescriptor();
  void operator= (const DevDescriptor&); 
};

//=============================================================================
// CLASS : DevRepository (SINGLETON)
//=============================================================================
class DevRepository 
{
  friend class TangoBinding;

public:
  static DevRepository* instance (void);
  // Returns the unique instance of <DevRepository>.
    
  DevDescriptor* device_desc (const char* device_name);
  // Providing a device name, returns its device proxy or NULL on error.

  DevDescriptor* device_desc (const std::string& device_name);
  // Providing a device name, returns its device proxy or NULL on error.

private:
  int remove_device (const std::string& device_name);
  // Providing a device name, returns removes it from the repository.

  static int init (void);
  //- Instanciates the singleton.
  
  static void cleanup (void);
  //- Releases the singleton.

  static DevRepository* instance_;
  //- The unique instance of <DevRepository>.

  DeviceDescMap dev_map_;
  //- The actual repository (see typedef above).

  // = Disallow these operations (except for TangoClientXop).
  //---------------------------------------------------------
  DevRepository();
  DevRepository (const DevRepository&);
  virtual ~DevRepository();
  void operator= (const DevRepository&); 
};

#if defined (__XDK_INLINE__)
  #include "DevRepository.i"
#endif 

#endif // _DEVICE_REPOSITORY_H_

   
   