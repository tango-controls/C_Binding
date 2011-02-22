// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for LabVIEW
//
// = FILENAME
//   DeviceDescriptorriptor.h
//
// = AUTHOR
//   Nicolas Leclercq - SOLEIL
//
// ============================================================================

#ifndef _DEVICE_DESC_H_
#define _DEVICE_DESC_H_

//=============================================================================
// DEPENDENCIEs
//=============================================================================
#include "StandardHeader.h"
#include "ThreadSafeDeviceProxy.h"

//=============================================================================
// DEFINEs
//=============================================================================
#define NO_SUCH_ATTR (int)-1
#define NO_SUCH_CMD  (int)-1

//=============================================================================
// CLASS : DeviceDescriptor
//=============================================================================
class DeviceDescriptor 
{
  friend class DeviceRepository;

public:

  typedef struct 
  {
	  string name;
	  long in_type;
	  long out_type;
  } MinCmdInfo;
  // Used to store locally the mininum required cmd info.

  typedef std::vector<MinCmdInfo> MinCmdInfoList;
  // Define what is a list of <MinCmdInfo>;

  typedef struct 
  {
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

  const MinCmdInfoList& cmd_list (void) const;
  // Returns the device commands list.

  const MinAttrInfoList& attr_list (void) const;
  // Returns the device attributes list.

  int cmd_exists (const std::string& cmd_name)
    throw (Tango::DevFailed);
  // If <cmd_name> is supported by the device, returns its 
  // indice in <cmd_list>,NO_SUCH_CMD otherwise.

  int cmd_exists (const char* cmd_name)
    throw (Tango::DevFailed);
  // If <cmd_name> is supported by the device, returns its 
  // indice in <cmd_list>, NO_SUCH_CMD otherwise.

  int attr_exists (const std::string& attr_name)
    throw (Tango::DevFailed);
  // If <attr_name> is an attribute of the device, returns its 
  // indice in <attr_list>, NO_SUCH_ATTR otherwise.

  int attr_exists (const char* attr_name)
    throw (Tango::DevFailed);
  // If <attr_name> is an attribute of the device, returns its 
  // indice in <attr_list>, NO_SUCH_ATTR otherwise.

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

  ThreadSafeDeviceProxy * operator-> (void) const;
  // Returns the underlying device proxy

  ThreadSafeDeviceProxy * proxy (void) const;
  // Returns the underlying device proxy

private:
  DeviceDescriptor (const std::string& _dev_name);
  // Ctor

  DeviceDescriptor (Tango::DeviceProxy * dp, bool ownership);
  // Ctor

  void get_device_interface (void)
    throw (Tango::DevFailed);
  // Request then store localy the device interface 

  void cmd_list (Tango::CommandInfoList* cmd_list);
  // Set the device commands list

  void attr_list (Tango::AttributeInfoList* attr_list);
  // Set the device attributes list

  virtual void disconnect (void);
  // Release the underlying proxy keeping only the attrs and cmds info 

  MinCmdInfoList dev_cmd_list_;
  // The device command list

  MinAttrInfoList dev_attr_list_;
  // The device attribute list

  ThreadSafeDeviceProxy * dev_proxy_;
  //- The underlying device proxy

  // = Disallow these operations (except for DeviceRepository).
  //-----------------------------------------------------------
  DeviceDescriptor (const DeviceDescriptor&);
  virtual ~DeviceDescriptor();
  void operator= (const DeviceDescriptor&); 
};

//=============================================================================
// INLINED CODE
//=============================================================================
#if defined (_LV_INLINE_)
  #include "DeviceDescriptor.i"
#endif 

#endif // _DEVICE_DESC_H_

   
   
