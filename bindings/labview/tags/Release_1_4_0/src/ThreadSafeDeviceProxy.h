// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for LabVIEW
//
// = FILENAME
//   ThreadSafeDeviceProxy.h
//
// = AUTHOR
//   Nicolas Leclercq - SOLEIL
//
// ============================================================================

#ifndef _TS_DEV_PROXY_H_
#define _TS_DEV_PROXY_H_

//=============================================================================
// Class ThreadSafeDeviceProxy (SINGLETON)
//=============================================================================
class ThreadSafeDeviceProxy
{
public:

  ThreadSafeDeviceProxy (const std::string& dev_name) 
    throw (Tango::DevFailed);

	ThreadSafeDeviceProxy (const char * dev_name) 
    throw (Tango::DevFailed);

  virtual ~ThreadSafeDeviceProxy ();

  const std::string status (void) 
    throw (Tango::DevFailed);

	Tango::DevState state (void) 
    throw (Tango::DevFailed);

  int ping (void) 
    throw (Tango::DevFailed);

  Tango::CommandInfoList* command_list_query (void) 
    throw (Tango::DevFailed);

  Tango::AttributeInfoList* attribute_list_query (void) 
    throw (Tango::DevFailed);

  Tango::DeviceAttribute read_attribute (const std::string& attr_name) 
    throw (Tango::DevFailed);

	Tango::DeviceAttribute read_attribute (const char *attr_name) 
    throw (Tango::DevFailed);

  std::vector<Tango::DeviceAttribute>* read_attributes (const std::vector<std::string>& attr_names) 
    throw (Tango::DevFailed);

	void write_attribute (const Tango::DeviceAttribute& attr_value) 
    throw (Tango::DevFailed);

	void write_attributes (const std::vector<Tango::DeviceAttribute>& attr_values) 
    throw (Tango::DevFailed);

  Tango::DeviceData command_inout (const std::string& cmd_name) 
    throw (Tango::DevFailed);

	Tango::DeviceData command_inout (const char * cmd_name) 
    throw (Tango::DevFailed);

  Tango::DeviceData command_inout (const std::string& cmd_name, const Tango::DeviceData &d) 
    throw (Tango::DevFailed);

	Tango::DeviceData command_inout (const char *cmd_name, const Tango::DeviceData &d) 
    throw (Tango::DevFailed);

  int subscribe_event(const string &attr_name, 
                      Tango::EventType event, 
                      Tango::CallBack *cb, 
                      const std::vector<std::string> &filters) 
    throw (Tango::DevFailed);

	void unsubscribe_event(int event_id) 
    throw (Tango::DevFailed);

  Tango::DeviceProxy& unsafe_proxy (void);

private:

  Tango::DeviceProxy  dp_;

  omni_mutex lock_;

  // = Disallow these operations (except for friends).
  //---------------------------------------------------------
  ThreadSafeDeviceProxy (const ThreadSafeDeviceProxy&);
  void operator= (const ThreadSafeDeviceProxy&); 
};

//=============================================================================
// INLINED CODE
//=============================================================================
#if defined (_LV_INLINE_)
  #include "ThreadSafeDeviceProxy.i"
#endif 

#endif // _TS_DEV_PROXY_H_
