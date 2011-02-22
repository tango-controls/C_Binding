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
#include "ThreadSafeDeviceProxy.h"

//=============================================================================
// FORWARD DECLARATION
//=============================================================================
class DeviceDesc;

//=============================================================================
// TYPEDEFs
//=============================================================================
typedef std::map<std::string, DeviceDesc*> DeviceDescMap;
// DeviceMap : a STL map of <DeviceProxy*> indexed by device name.
typedef DeviceDescMap::iterator DeviceDescMapIterator;
// DeviceMapIterator : a DeviceMap iterator;

//=============================================================================
// DEFINEs
//=============================================================================
#define DEV_REP (DeviceRepository::instance())

//=============================================================================
// CLASS : DeviceDesc
//=============================================================================
class DeviceDesc 
{
  friend class DeviceRepository;

public:

  typedef struct {
	  string name;
	  long in_type;
	  long out_type;
  } MinCmdInfo;
  // Used to store locally the mininum required cmd info.

  typedef std::vector<MinCmdInfo> MinCmdInfoList;
  // Define what is a list of <MinCmdInfo>;

  typedef struct {
	  string name;
	  Tango::AttrWriteType writable;
	  Tango::AttrDataFormat data_format;
	  int data_type;
  } MinAttrInfo;
  // Used to store locally the mininum required attr info.

  typedef std::vector<MinAttrInfo> MinAttrInfoList;
  // Define what is a list of <MinAttrInfo>;

  const std::string& name (void) const;
  // Returns the device name.

  ThreadSafeDeviceProxy* proxy (void) const;
  // Returns the device proxy.

  const MinCmdInfoList& cmd_list (void) const;
  // Returns the device commands list.

  const MinAttrInfoList& attr_list (void) const;
  // Returns the device attributes list.

  int cmd_exists (const std::string& cmd_name)
    throw (Tango::DevFailed);
  // If <cmd_name> is supported by the device, returns its 
  // indice in <cmd_list>, -1 otherwise.

  int cmd_exists (const char* cmd_name)
    throw (Tango::DevFailed);
  // If <cmd_name> is supported by the device, returns its 
  // indice in <cmd_list>, -1 otherwise.

  int attr_exists (const std::string& attr_name)
    throw (Tango::DevFailed);
  // If <attr_name> is an attribute of the device, returns its 
  // indice in <attr_list>, -1 otherwise.

  int attr_exists (const char* attr_name)
    throw (Tango::DevFailed);
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
  DeviceDesc (std::string _dev_name);
  // Ctor.

  DeviceDesc(ThreadSafeDeviceProxy* _dp, bool _ownership = true);
  // Ctor.

  void cmd_list (Tango::CommandInfoList* cmd_list);
  // Set the device commands list.

  void attr_list (Tango::AttributeInfoList* attr_list);
  // Set the device attributes list.

  std::string dev_name_;
  // The device name.

  ThreadSafeDeviceProxy* dev_proxy_;
  // The device proxy

  MinCmdInfoList dev_cmd_list_;
  // The device command list.

  MinAttrInfoList dev_attr_list_;
  // The device attribute list.

  bool ownership_;
  //- Do this instance has ownership of the underlying <Tango::DeviceProxy>?

  // = Disallow these operations (except for DeviceRepository).
  //-----------------------------------------------------------
  DeviceDesc (const DeviceDesc&);
  virtual ~DeviceDesc();
  void operator= (const DeviceDesc&); 
};

//=============================================================================
// CLASS : DeviceRepository (SINGLETON)
//=============================================================================
class DeviceRepository 
{
  friend class ObjectManager;

public:
  static DeviceRepository* instance (void);
  // Returns the unique instance of <DeviceRepository>.
    
  DeviceDesc* device_desc (const char* device_name)
    throw (Tango::DevFailed);
  // Providing a device name, returns its device proxy or NULL on error.

  DeviceDesc* device_desc (const std::string& device_name)
    throw (Tango::DevFailed);
  // Providing a device name, returns its device proxy or NULL on error.

  void remove_device (const std::string& device_name);
  //- Providing a device name, returns removes it from the repository.

private:

  static int init (void);
  //- Instanciates the singleton.
  
  static void cleanup (void);
  //- Releases the singleton.

  static DeviceRepository* instance_;
  //- The unique instance of <DeviceRepository>.

  int remove_device_i (const std::string& device_name);
  //- Providing a device name, returns removes it from the repository.

  DeviceDescMap dev_map_;
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

//=============================================================================
// INLINED CODE
//=============================================================================
#if defined (_LV_INLINE_)
  #include "DeviceRepository.i"
#endif 

#endif // _DEVICE_REPOSITORY_H_

   
   
