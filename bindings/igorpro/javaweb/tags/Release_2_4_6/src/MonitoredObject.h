// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for Igor Pro
//
// = FILENAME
//   MonitoredObject.h
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

#ifndef _MONITORED_OBJECT_H_
#define _MONITORED_OBJECT_H_

// ============================================================================
// DEPENDENCIES
// ============================================================================
#include <yat/time/Timer.h>
#include <yat/threading/Mutex.h>

//=============================================================================
// TYPEDEFs
//=============================================================================
typedef struct MonitorClient
{
  //- client counter (for clients requested export to same igor path)
  size_t ref_count;
  //- polling period for <this> client
  unsigned long pp;
  //- full path to igor obj
  std::string path;
  //- suspend/resume flag
  bool suspended;
  //- export timer
  yat::Timer export_timer_;
  //- ctor
  MonitorClient ()
    : ref_count(1), pp(1000), path(""), suspended(false)
  {
    //- noop
  }
  //- ctor
  MonitorClient (const MonitorClient& _src)
    : ref_count(1), pp(_src.pp), path(_src.path), suspended(_src.suspended)
  {
    //- noop
  }
} MonitorClient;
//-----------------------------------------------------------------------------
typedef long ClientId;
//-----------------------------------------------------------------------------
typedef std::map<ClientId, MonitorClient> ClientsMap;
typedef ClientsMap::iterator ClientsMapIt;
//-----------------------------------------------------------------------------

//=============================================================================
// CLASS : MonitoredObject
//=============================================================================
class MonitoredObject 
{
  friend class MonitoredDevice;

protected:
  //- ctor
  MonitoredObject(const std::string& tango_dev_name,
                  const std::string& tango_obj_name);

  //- dtor
  virtual ~MonitoredObject();

  //- suspend monitoring 
  int suspend(ClientId c);

  //- resume monitoring
  int resume(ClientId c);

  //- register client 
  ClientId register_client (unsigned long  pp, 
                            const std::string& igor_obj_name);

  //- unregister client 
  void unregister_client (ClientId c);

  //- client registered
  bool client_registered (ClientId c);

  //- any client?
  bool has_registered_clients ();

  //- change the polling period for the specified client
  int change_polling_period (unsigned long pp, ClientId c = -1);

  //- time to pollling period expiration
  unsigned long num_ms_to_polling_period_expiration ();

  //- device name
  std::string tango_dev_name_;

  //- object name
  std::string tango_obj_name_;

  //- min polling period in ms
  unsigned long min_pp_;

  //- import timer
  yat::Timer import_timer_;

  //- number of errors 
  unsigned long error_counter_;

  //- updated flag
  bool updated_;
  
  //- force value export
  bool force_next_export_;

  //- clients map
  ClientsMap clients_;

  //- client id generator
  static ClientId next_client_id;

private:
  // = Disallow these operations
  //---------------------------------------------
  MonitoredObject (const MonitoredObject&);
  void operator= (const MonitoredObject&); 
};

//=============================================================================
// CLASS : MonitoredAttribute
//=============================================================================
class MonitoredAttribute : public MonitoredObject 
{
  friend class MonitoredDevice;

protected:
  //- ctor
  MonitoredAttribute(const std::string& tango_dev_name,
                     const std::string& tango_obj_name,
                     int attribute_id);

  //- dtor
  virtual ~MonitoredAttribute();

  //- mark attribute as invalid
  void attach_error ();

  //- attach actual value to the underlying attribute
  void attach_value (const Tango::DeviceAttribute & av);

  //- export value or result into associated igor object 
  void export_value ();

  //- the attribute identifier
  int attr_id_;

  //- the attribute value (to be decoded on IDLE)
  Tango::DeviceAttribute value_;

  // = Disallow these operations
  //---------------------------------------------
  MonitoredAttribute (const MonitoredAttribute&);
  void operator= (const MonitoredAttribute&); 
};

//=============================================================================
// CLASS : MonitoredDevice
//=============================================================================
class MonitoredDevice 
{
  friend class MonitoredObjectManager;

  typedef std::map<std::string, MonitoredAttribute*> MonitoredAttributeMap;
  typedef MonitoredAttributeMap::iterator MonitoredAttributeMapIt;

protected:

  //- ctor
  MonitoredDevice (const std::string& tango_dev_name);

  //- dtor
  virtual ~MonitoredDevice ();

  //- any monitored objects
  bool has_monitored_objects () const;

  //- import values
  void import_values (unsigned long ms_to_next_import);

  //- export value or result into associated igor object 
  void export_values ();

  //- add specified attribute to monitor list
  int add_attribute (const std::string& attr_name,
                     const std::string& full_path_to_igor_obj,
                     unsigned long polling_period,
                     long& client_id);

  //- remove specified attribute from monitor list
  int remove_attribute (const std::string& attr_name,
                        long client_id = -1);

  //- change the polling period off the specified object
  int change_polling_period (const std::string& obj_name,
                             unsigned long polling_period,
                             long client_id);

  //- computes the smallest requested polling period
  unsigned long minimum_polling_period ();

  //- suspends a monitor 
  int suspend_monitor (const std::string& obj_name, long client_id = -1);

  //- resumes a monitor 
  int resume_monitor (const std::string& obj_name, long client_id = -1);

  //- device name
  std::string tango_dev_name_;

  //- attribute list
  MonitoredAttributeMap mattr_map_;

#if ! defined(_PER_ATTR_POLLING_PERIOD_)
  //- attribute name list (optimizes read_attributes calls)
  std::vector<std::string> attr_list_;
#endif
};


#if defined (__XDK_INLINE__)
  #include "MonitoredObject.i"
#endif 

#endif // _MONITORED_OBJECT_H_

   
   