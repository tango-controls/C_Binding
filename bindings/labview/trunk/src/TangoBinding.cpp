// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for LabVIEW
//
// = FILENAME
//   TangoBinding.cpp
//
// = AUTHOR
//   Nicolas Leclercq - SOLEIL
//
// ============================================================================

//=============================================================================
// DEPENDENCIES
//=============================================================================
#include "StandardHeader.h"
#include "ErrorStack.h"
#include "DeviceRepository.h"
#include "GroupRepository.h"
#include "DataAdapter.h"
#include "EventManager.h"
#include "ObjectManager.h" 

//=============================================================================
// SHARED LIB EXPORTED SYMBOLS
//=============================================================================
#if defined(WIN32)
# define LVB_EXPORT extern "C" __declspec (dllexport)
#else
# define LVB_EXPORT extern "C"    
#endif

//=============================================================================
// BINDING C-INTERFACE TO LABVIEW
//=============================================================================
//- Tango ping support --------------------------------------------------------
LVB_EXPORT long ping (const char*, void*, void*);
//- Tango device state/status support  ----------------------------------------
LVB_EXPORT long state (const char*, void*, void*);
LVB_EXPORT long status (const char*, void*, void*);
//- Tango commands support  ---------------------------------------------------
LVB_EXPORT long command_inout (const char*, const char*, void*, void*, void*);
//- Tango read/write attributes support ---------------------------------------
LVB_EXPORT long read_attributes (const char*, void*, void*);
LVB_EXPORT long write_attributes (const char*, void*, void*);
//- Tango events support ------------------------------------------------------
LVB_EXPORT long subscribe_event (const char*,const char*, LVRefNum*, short, const char*, void*);
LVB_EXPORT long unsubscribe_event (LVRefNum *, void*);
//- Tango groups support ------------------------------------------------------
LVB_EXPORT long group_create (const char*, void*, void*, long*, void*);
LVB_EXPORT long group_kill (long, void*);
LVB_EXPORT long group_size (long, long*, void*);
LVB_EXPORT long group_command_inout (long, const char*, void*, void*);
LVB_EXPORT long group_read_attributes (long, void*, void*, void*);
LVB_EXPORT long group_write_attribute (long, void*, void*); 
//- Tango archiving support ---------------------------------------------------
LVB_EXPORT long extract_archived_data (const char*, const char*, unsigned long, void*, void*); 
//- Tango device timeout support ----------------------------------------------
LVB_EXPORT long set_timeout (const char*, unsigned long, void*);
//-----------------------------------------------------------------------------
#if defined(_TBFL_HAS_OPEN_CLOSE_SESSION_INTERFACE_)
//- open/lose session routines (may be removed in a near future)
  LVB_EXPORT long open_lvb_session (void* error_stack_);
  LVB_EXPORT long close_lvb_session (void* error_stack_);
#endif

//=============================================================================
// CONSTS
//=============================================================================
const int kNoError =  0;
const int kError   = -1;

//=============================================================================
// MACROS
//=============================================================================
#define _TO_LV_ERR_STACK(X) reinterpret_cast<LvStringArrayHdl&>(X)

//=============================================================================
// EVENT TYPES
//=============================================================================
enum TangoEventTypes
{
  LV_PERIODIC_EVENT = 0,
  LV_CHANGE_EVENT
};

#if (_TBFL_HAS_STATIC_OBJ_MANAGER_ == 0) && defined(_TBFL_HAS_OPEN_CLOSE_SESSION_INTERFACE_)
//=============================================================================
// BINDING C-INTERFACE TO LABVIEW: open_lvb_session
//=============================================================================
long open_lvb_session (void* error_stack_)
{
  try 
  {
    ObjectManager::init();
  }
  catch (const Tango::DevFailed& e)
  {
    ERROR_STACK->devfailed(e, _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }
  catch (...) 
  {
    ERROR_STACK->unknown("open_lvb_session", _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }

  ERROR_STACK->no_error("open_lvb_session", _TO_LV_ERR_STACK(error_stack_));

  return kNoError;
}

//=============================================================================
// BINDING C-INTERFACE TO LABVIEW: close_lvb_session
//=============================================================================
long close_lvb_session (void* error_stack_)
{
  try 
  {
    ObjectManager::fini();
  }
  catch (const Tango::DevFailed& e)
  {
    ERROR_STACK->devfailed(e, _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }
  catch (...) 
  {
    ERROR_STACK->unknown("close_lvb_session", _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }

  ERROR_STACK->no_error("close_lvb_session", _TO_LV_ERR_STACK(error_stack_));

  return kNoError;
}
#endif

//=============================================================================
// BINDING C-INTERFACE TO LABVIEW: ping
//=============================================================================
long ping (const char* _dev_name, void* argout_, void* error_stack_)
{
  try
  {
    //- get local descriptor
    DeviceDescriptor * ddesc = DEV_REP->device_desc(_dev_name);

    //- ping the device
    int rtt = ddesc->proxy()->ping();

    //- be sure argout_ is valid
    if (argout_ == 0) 
    {
      Tango::Except::throw_exception(_CPTC("internal error"),
                                     _CPTC("unexpected null pointer"),
                                     _CPTC("ping"));
    }

    //- push result into argout_
    *(reinterpret_cast<long*>(argout_)) = rtt;
  }
  catch (const Tango::DevFailed& e)
  {
    ERROR_STACK->devfailed(e, _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }
  catch (...) 
  {
    ERROR_STACK->unknown("ping", _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }

  ERROR_STACK->no_error("ping", _TO_LV_ERR_STACK(error_stack_));

  return kNoError;
}

//=============================================================================
// BINDING C-INTERFACE TO LABVIEW: state
//=============================================================================
long state (const char* _dev_name, void* argout_, void* error_stack_)
{
  try
  {
    //- get local descriptor
    DeviceDescriptor * ddesc = DEV_REP->device_desc(_dev_name);

    short lv_state = 0;

    //- convert from DevState to i16 (Labview calls to DLL don't like enum values)
    switch (ddesc->proxy()->state()) 
    {
      case Tango::ON:
        lv_state = 0;
        break;
      case Tango::OFF:
        lv_state = 1;
        break;
      case Tango::CLOSE:
        lv_state = 2;
        break;
      case Tango::OPEN:
        lv_state = 3;
        break;
      case Tango::INSERT:
        lv_state = 4;
        break;
      case Tango::EXTRACT:
        lv_state = 5;
        break;
      case Tango::MOVING:
        lv_state = 6;
        break;
      case Tango::STANDBY:
        lv_state = 7;
        break;
      case Tango::FAULT:
        lv_state = 8;
        break;
      case Tango::INIT:
        lv_state = 9;
        break;
      case Tango::RUNNING:
        lv_state = 10;
        break;
      case Tango::ALARM:
        lv_state = 11;
        break;
      case Tango::DISABLE:
        lv_state = 12;
        break;
      default:
      case Tango::UNKNOWN:
        lv_state = 13;
        break;
    }

    //- push result into argout_
    *(reinterpret_cast<short*>(argout_)) = lv_state;

  }
  catch (const Tango::DevFailed& e)
  {
    ERROR_STACK->devfailed(e, _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }
  catch (...) 
  {
    ERROR_STACK->unknown("state", _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }

  ERROR_STACK->no_error("state", _TO_LV_ERR_STACK(error_stack_));

  return kNoError;
}

//=============================================================================
// BINDING C-INTERFACE TO LABVIEW: status
//=============================================================================
long status (const char* _dev_name, void* argout_, void* error_stack_)
{
  try
  {
    Tango::DevFailed df;
    bool has_error = false;
    bool has_tango_error = false;

    Tango::DevState state = Tango::UNKNOWN;
    std::string lv_status = "unknown (failed to get state/status from device)";

    try 
    {
      //- get local descriptor
      DeviceDescriptor * ddesc = DEV_REP->device_desc(_dev_name); 

      //- get both state and status from device
      state = ddesc->proxy()->state();
      lv_status = ddesc->proxy()->status();
    }
    catch (const Tango::DevFailed& e) 
    {
      df = e;
      has_error = true;
      has_tango_error = true;
    }
    catch (...) 
    {
      has_error = true;
      has_tango_error = false;
    }

    //- convert from DevState to i16 (Labview calls to DLL don't like enum values)
    std::string lv_state_val;
    std::string lv_state_str;
    switch (state) 
    {
      case Tango::ON:
        lv_state_val = "0"; 
        lv_state_str = "ON";
        break;
      case Tango::OFF:
        lv_state_val = "1"; 
        lv_state_str = "OFF";
        break;
      case Tango::CLOSE:
        lv_state_val = "2"; 
        lv_state_str = "CLOSE";
        break;
      case Tango::OPEN:
        lv_state_val = "3"; 
        lv_state_str = "OPEN";
        break;
      case Tango::INSERT:
        lv_state_val = "4"; 
        lv_state_str = "INSERT";
        break;
      case Tango::EXTRACT:
        lv_state_val = "5"; 
        lv_state_str = "EXTRACT";
        break;
      case Tango::MOVING:
        lv_state_val = "6"; 
        lv_state_str = "MOVING";
        break;
      case Tango::STANDBY:
        lv_state_val = "7"; 
        lv_state_str = "STANDBY";
        break;
      case Tango::FAULT:
        lv_state_val = "8"; 
        lv_state_str = "FAULT";
        break;
      case Tango::INIT:
        lv_state_val = "9"; 
        lv_state_str = "INIT";
        break;
      case Tango::RUNNING:
        lv_state_val = "10"; 
        lv_state_str = "RUNNING";
        break;
      case Tango::ALARM:
        lv_state_val = "11"; 
        lv_state_str = "ALARM";
        break;
      case Tango::DISABLE:
        lv_state_val = "12"; 
        lv_state_str = "DISABLE";
        break;
      default:
      case Tango::UNKNOWN:
        lv_state_val = "13"; 
        lv_state_str = "UNKNOWN";
        break;
    }

    long err = ::NumericArrayResize(LvMemoryAddress, 
                                    1L, 
                                    reinterpret_cast<UHandle*>(&argout_), 
                                    3);
    if (err != noErr) {
      Tango::Except::throw_exception(_CPTC("out of memory"),
                                     _CPTC("binding error: memory allocation failed"),
                                     _CPTC("status"));
    }

    LvStringArrayHdl lvh = _TO_LV_ERR_STACK(argout_);
    (*lvh)->length = 3;
    ::memset((*lvh)->data, 0, (*lvh)->length * sizeof(long));
    size_t l = 0;
    LvStringHdl sh;

    sh = 0;
    l = lv_state_val.size();
    err = ::NumericArrayResize(iB, 1L, reinterpret_cast<UHandle*>(&sh), l);
    if (err != noErr) {
      Tango::Except::throw_exception(_CPTC("out of memory"),
                                     _CPTC("binding error: memory allocation failed"),
                                     _CPTC("status"));
    }
    (*sh)->length = l;
    ::memcpy((*sh)->data, lv_state_val.c_str(), l);
    (*lvh)->data[0] = sh;

    sh = 0;
    l = lv_state_str.size();
    err = ::NumericArrayResize(iB, 1L, reinterpret_cast<UHandle*>(&sh), l);
    if (err != noErr) {
      Tango::Except::throw_exception(_CPTC("out of memory"),
                                     _CPTC("binding error: memory allocation failed"),
                                     _CPTC("status"));
    }
    (*sh)->length = l;
    ::memcpy((*sh)->data, lv_state_str.c_str(), l);
    (*lvh)->data[1] = sh;

    sh = 0;
    l = lv_status.size();
    err = ::NumericArrayResize(iB, 1L, reinterpret_cast<UHandle*>(&sh), l);
    if (err != noErr) {
      Tango::Except::throw_exception(_CPTC("out of memory"),
                                     _CPTC("binding error: memory allocation failed"),
                                     _CPTC("status"));
    }
    (*sh)->length = l;
    ::memcpy((*sh)->data, lv_status.c_str(), l);
    (*lvh)->data[2] = sh;

    if (has_error) 
    {
      if (has_tango_error)
        throw df;
      else
        throw;
    }
  }
  catch (const Tango::DevFailed& e)
  {
    ERROR_STACK->devfailed(e, _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }
  catch (...) 
  {
    ERROR_STACK->unknown("status", _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }

  ERROR_STACK->no_error("status", _TO_LV_ERR_STACK(error_stack_));

  return kNoError;
}

//=============================================================================
// BINDING C-INTERFACE TO LABVIEW: command_inout
//=============================================================================
long command_inout (const char* _dev_name,
                    const char* _cmd_name,
                    void* _argin, 
                    void* argout_,
                    void* error_stack_)
{
  try
  {
    //- cast to actual type
    LvArgIOPtr ai = reinterpret_cast<LvArgIOPtr>(_argin);

    //- cast to actual type
    LvArgIOPtr ao = reinterpret_cast<LvArgIOPtr>(argout_);

    //- get local descriptor
    DeviceDescriptor * ddesc = DEV_REP->device_desc(_dev_name);

    //- is <cmd_name> a valid command for <dev_name>
    int cmd_id = ddesc->cmd_exists(_cmd_name);

    //- get <argin> type for <cmd_name>
    int ai_type = (ddesc->cmd_list())[cmd_id].in_type;

    //- get <argout> type for <cmd_name>
    int ao_type = (ddesc->cmd_list())[cmd_id].out_type;

    //- get <cmd_name> result in a Tango::DeviceData 
    Tango::DeviceData dd_out;

    //- encode <argin> then exec the command 
    if (ai_type != Tango::DEV_VOID) 
    {
      Tango::DeviceData dd_in;
      DATA_ADAPTER->encode_argin(ddesc, cmd_id, ai, dd_in);
      dd_out = ddesc->proxy()->command_inout(_cmd_name, dd_in);
    }
    //- just exec the command 
    else 
    {
      dd_out = ddesc->proxy()->command_inout(_cmd_name);
    }

    //- decode <argout>
    if (ao_type != Tango::DEV_VOID) 
    {
      DATA_ADAPTER->decode_argout(ddesc, cmd_id, dd_out, ao);
    }
  }
  catch (const Tango::DevFailed& e)
  {
    ERROR_STACK->devfailed(e, _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }
  catch (...) 
  {
    ERROR_STACK->unknown("command_inout", _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }

  ERROR_STACK->no_error("command_inout", _TO_LV_ERR_STACK(error_stack_));

  return kNoError;
}

//=============================================================================
// BINDING C-INTERFACE TO LABVIEW: read_attributes
//=============================================================================
long read_attributes (const char* _dev_name,
                      void* values_inout_,
                      void* error_stack_)
{
  std::vector<Tango::DeviceAttribute>* attr_values = 0;

  try 
  {
    //- get local descriptor
    DeviceDescriptor * ddesc = DEV_REP->device_desc(_dev_name);

    //- values_inout_ is a handle to an LvAttributeValueArray (i.e. an array of clusters)
    LvAttributeValueArrayHdl avah = reinterpret_cast<LvAttributeValueArrayHdl>(values_inout_);
    
    //- how many entries do we have in this array?
    long n = (*avah)->length;

    //- build <attribute name> list 
    LvAttributeValuePtr av = 0;

    std::vector<std::string> attr_names;
    attr_names.resize(n);

    //- for each entry in the LvAttributeValueArray...
		long i;
    for (i = 0; i < n; i++)
    {
      //- point to the i-th LvAttributeValue in the array
      av = (*avah)->attr_values + i;
      //- get attribute name 
      LvStringHdl sh = reinterpret_cast<LvStringHdl>(av->attr_name);
      attr_names[i].assign(reinterpret_cast<char*>((*sh)->data), 
                           reinterpret_cast<char*>((*sh)->data) + (*sh)->length);
    }

    //- read attributes
    attr_values = ddesc->proxy()->read_attributes(attr_names);

    //- populate each LvAttributeValue
    for (i = 0; i < n; i++)
    {
      //- point to the i-th LvAttributeValue in the array
      av = (*avah)->attr_values + i;
      //- decode attribute value
      DATA_ADAPTER->tango_to_lv_attribute_value(ddesc, attr_names[i], (*attr_values)[i], av); 
    }

    //- release memory 
    if (attr_values) delete attr_values;
  }
  catch (const Tango::DevFailed& e)
  {
    if (attr_values) delete attr_values;
    ERROR_STACK->devfailed(e, _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }
  catch (...) 
  {
    if (attr_values) delete attr_values;
    ERROR_STACK->unknown("read_attributes", _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }

  ERROR_STACK->no_error("read_attributes", _TO_LV_ERR_STACK(error_stack_));

  return kNoError;
}

//=============================================================================
// BINDING C-INTERFACE TO LABVIEW: write_attributes
//=============================================================================
long write_attributes (const char* _dev_name,
                       void* values_inout_,
                       void* error_stack_)
{
  try 
  {
    //- get local descriptor
    DeviceDescriptor * ddesc = DEV_REP->device_desc(_dev_name);

    //- values_inout_ is a handle to an LvAttributeValueArray (i.e. an array of clusters)
    LvAttributeValueArrayHdl avah = reinterpret_cast<LvAttributeValueArrayHdl>(values_inout_);
    
    //- how many entries do we have in this array?
    long n = (*avah)->length;

    //- some locals
    int attr_id = -1;
    std::string attr_name;
    LvStringHdl sh = 0;
    LvAttributeValuePtr av = 0;

    //- create and resize a vector of TANGO::DeviceAttribute 
    std::vector<Tango::DeviceAttribute> da_list;
    da_list.resize(n);

    //- Get attr/val for each entry in the array
    for (long i = 0; i < n; i++) 
    {
      //- point to the i-th LvAttributeValue in the array
      av = (*avah)->attr_values + i;
      //- get attr name
      sh = reinterpret_cast<LvStringHdl>(av->attr_name);
      attr_name.assign(reinterpret_cast<char*>((*sh)->data), 
                       reinterpret_cast<char*>((*sh)->data) + (*sh)->length);
      //- get <_attr_name> id
      attr_id = ddesc->attr_exists(attr_name);

      //- encode data (may throw an exception)
      try 
      {
        DATA_ADAPTER->encode_attr(ddesc, attr_id, av, da_list[i]);
      }
      catch (const Tango::DevFailed& e) 
      {
        ERROR_STACK->devfailed(e, _TO_LV_ERR_STACK(error_stack_));
        return kError;
      }
      catch (...) 
      {
        ERROR_STACK->unknown("writes_attributes", _TO_LV_ERR_STACK(error_stack_));
        return kError;
      }
    }

    //- write attributes 
    ddesc->proxy()->write_attributes(da_list);
  }
  catch (const Tango::DevFailed& e)
  {
    ERROR_STACK->devfailed(e, _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }
  catch (...) 
  {
    ERROR_STACK->unknown("writes_attributes", _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }

  ERROR_STACK->no_error("writes_attributes", _TO_LV_ERR_STACK(error_stack_));

  return kNoError;
}


//=============================================================================
// BINDING C-INTERFACE TO LABVIEW: subscribe_event
//=============================================================================
long subscribe_event (const char* _dev_name,
                      const char* _attr_name,
                      LVRefNum *_evt_refnum,
                      short lv_evt_type,
                      const char* lv_evt_filters,
                      void *error_stack_)
{
  try
  {
    Tango::EventType evt_type = Tango::PERIODIC_EVENT;
    std::vector<std::string> filters;

    switch (lv_evt_type)
    {
      case LV_CHANGE_EVENT:
        evt_type = Tango::CHANGE_EVENT;
        if (::strlen(lv_evt_filters) == 0)
        {
          Tango::Except::throw_exception(_CPTC("invalid event filters"),
                                         _CPTC("valid event filter string required for CHANGE_EVENT"),
                                         _CPTC("subscribe_event"));
        }
        filters.push_back(std::string(lv_evt_filters));
        break;
      case LV_PERIODIC_EVENT:
        evt_type = Tango::PERIODIC_EVENT;
        if (::strlen(lv_evt_filters) != 0)
        {
          filters.push_back(std::string(lv_evt_filters));
        }
        break;
      default:
        Tango::Except::throw_exception(_CPTC("invalid event type specified"),
                                       _CPTC("invalid or unsupported TANGO event type"),
                                       _CPTC("subscribe_event"));
        break;
    }

    EVT_MANAGER->subscribe(_dev_name, _attr_name, *_evt_refnum, evt_type, filters);
  }
  catch (const Tango::DevFailed& e)
  {
    ERROR_STACK->devfailed(e, _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }
  catch (...) 
  {
    ERROR_STACK->unknown("subscribe_event", _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }

  ERROR_STACK->no_error("subscribe_event", _TO_LV_ERR_STACK(error_stack_));

  return kNoError;
}

//=============================================================================
// BINDING C-INTERFACE TO LABVIEW: unsubscribe_event
//=============================================================================
long unsubscribe_event (LVRefNum *_evt_refnum, void* error_stack_)
{
  try
  {
    EVT_MANAGER->unsubscribe(*_evt_refnum);
  }
  catch (const Tango::DevFailed& e)
  {
    ERROR_STACK->devfailed(e, _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }
  catch (...) 
  {
    ERROR_STACK->unknown("unsubscribe_event", _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }

  ERROR_STACK->no_error("unsubscribe_event", _TO_LV_ERR_STACK(error_stack_));

  return kNoError;
}

//=============================================================================
// BINDING C-INTERFACE TO LABVIEW: group_create
//=============================================================================
long group_create (const char* _group_name,
                   void* _managed_devices,
                   void* _ignored_devices,
                   long* group_id_,
                   void* error_stack_)
{
  try
  {
    //- build vector of <managed device> names
    LvStringArrayHdl h = reinterpret_cast<LvStringArrayHdl>(_managed_devices);
    if (h == 0) 
    {
      Tango::Except::throw_exception(_CPTC("invalid argument"),
                                     _CPTC("binding internal error: unexpected null argument"),
                                     _CPTC("create_group"));
    }
    LvStringHdl sh = 0;
    DeviceList managed_devices((*h)->length);
		long i;
    for (i = 0; i < (*h)->length; i++) 
    {
      sh = (*h)->data[i]; 
      managed_devices[i].assign(reinterpret_cast<char*>((*sh)->data), 
                                reinterpret_cast<char*>((*sh)->data) + (*sh)->length);
    }

    //- build vector of <ignored device> names
    DeviceList ignored_devices(0);
    h = reinterpret_cast<LvStringArrayHdl>(_ignored_devices);
    if (h != 0)
      ignored_devices.resize((*h)->length);

    for (i = 0; i < (*h)->length; i++) 
    {
      sh = (*h)->data[i]; 
      ignored_devices[i].assign(reinterpret_cast<char*>((*sh)->data), 
                                reinterpret_cast<char*>((*sh)->data) + (*sh)->length);
    }

    //- create the group and return its id to the caller
    *group_id_ = GRP_REP->add(_group_name, managed_devices, ignored_devices); 
  }
  catch (const Tango::DevFailed& e)
  {
    ERROR_STACK->devfailed(e, _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }
  catch (...) 
  {
    ERROR_STACK->unknown("group_create", _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }

  ERROR_STACK->no_error("group_create", _TO_LV_ERR_STACK(error_stack_));

  return kNoError;
}

//=============================================================================
// BINDING C-INTERFACE TO LABVIEW: group_kill
//=============================================================================
long group_kill (long _group_id,
                 void* error_stack_)
{
  try
  {
    //- remove group from repository
    GRP_REP->remove(_group_id);
  }
  catch (const Tango::DevFailed& e)
  {
    ERROR_STACK->devfailed(e, _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }
  catch (...) 
  {
    ERROR_STACK->unknown("group_kill", _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }

  ERROR_STACK->no_error("group_kill", _TO_LV_ERR_STACK(error_stack_));

  return kNoError;
}

//=============================================================================
// BINDING C-INTERFACE TO LABVIEW: group_size
//=============================================================================
long group_size (long _group_id,
                 long* group_size_,
                 void* error_stack_)
{
  try
  {
    //- get group from repository
    GroupDescriptor * gdesc = GRP_REP->get(_group_id);
    
    //- return group size to caller
    *group_size_ = static_cast<long>(gdesc->device_list().size());
  }
  catch (const Tango::DevFailed& e)
  {
    ERROR_STACK->devfailed(e, _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }
  catch (...) 
  {
    ERROR_STACK->unknown("group_size", _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }

  ERROR_STACK->no_error("group_size", _TO_LV_ERR_STACK(error_stack_));

  return kNoError;
}

//=============================================================================
// BINDING C-INTERFACE TO LABVIEW: group_read_attributes
//=============================================================================
long group_read_attributes (long _group_id,
                            void* _attributes_names,
                            void* values_inout_,
                            void* error_stack_)
{
  try
  {
    //- get group from repository
    GroupDescriptor * gdesc = GRP_REP->get(_group_id);
  
    //- check returned reference
    if (! gdesc || ! gdesc->group())
    {
      Tango::Except::throw_exception(_CPTC("invalid group identifier"),
                                     _CPTC("invalid group identifier specified [no such group]"),
                                     _CPTC("group_read_attributes"));
    } 

    //- get attribute names from 2nd argument
    LvStringArrayHdl h = reinterpret_cast<LvStringArrayHdl>(_attributes_names);
    if (h == 0) 
    {
      Tango::Except::throw_exception(_CPTC("invalid argument"),
                                     _CPTC("binding internal error: unexpected null argument"),
                                     _CPTC("group_read_attributes"));
    }

    //- vector of attribute names: the attributes to be read
    std::vector<std::string> attributes_names((*h)->length);
    for (size_t i = 0; i < attributes_names.size(); i++)
    {
      LvStringHdl sh = (*h)->data[i]; 
      attributes_names[i].assign(reinterpret_cast<char*>((*sh)->data), 
                                 reinterpret_cast<char*>((*sh)->data) + (*sh)->length);
    }

    //- read attributes on each device in the group (may throw an exception)
    Tango::GroupAttrReplyList arl = gdesc->group()->read_attributes(attributes_names, true);

    //- the number of device in the group
    long gsize = gdesc->device_list().size();

    //- the number of read attributes
    long nattrs = attributes_names.size();

    //- values_inout_ is a handle to an LvAttributeValueArray (i.e. an array of clusters)
    LvAttributeValueArrayHdl avah = reinterpret_cast<LvAttributeValueArrayHdl>(values_inout_);

    //- how many entries do we have in this array?
    long avah_size = (*avah)->length;

    //- be sure we have enough room to store results
    long required_avah_size = gsize * nattrs;
    if (avah_size != required_avah_size)
    {
      Tango::Except::throw_exception(_CPTC("invalid argument"),
                                     _CPTC("wrong size for input array of AttributeValues"),
                                     _CPTC("group_read_attributes"));
    }
    
    LvAttributeValuePtr av = 0;

    //- for each device in the group...
    for (long d = 0; d < gsize; d++)
    {
      //- get local device descriptor
      DeviceDescriptor * ddesc = DEV_REP->device_desc(gdesc->device_list()[d]);
      //- for each read attribute 
      for (long a = 0; a < nattrs; a++)
      {
        //- compute current index in arl (GroupAttrReplyList) 
        long current_index = d * nattrs + a;
        //- point to associated LvAttributeValue 
        av = (*avah)->attr_values + current_index;
        //- decode attribute value
        DATA_ADAPTER->tango_to_lv_attribute_value(ddesc, 
                                                  attributes_names[a], 
                                                  arl[current_index].get_data(), 
                                                  av);
      }
    }
  }
  catch (const Tango::DevFailed& e)
  {
    ERROR_STACK->devfailed(e, _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }
  catch (...) 
  {
    ERROR_STACK->unknown("group_read_attributes", _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }

  ERROR_STACK->no_error("group_read_attributes", _TO_LV_ERR_STACK(error_stack_));

  return kNoError;
}

//=============================================================================
// BINDING C-INTERFACE TO LABVIEW: group_write_attribute
//=============================================================================
long group_write_attribute (long _group_id,
                            void* values_inout_,
                            void* error_stack_)
{
  try
  {
    Tango::Except::throw_exception(_CPTC("missing feature"),
                                   _CPTC("sorry but 'group_write_attribute' is not yet implemented"),
                                   _CPTC("group_write_attribute"));
  }
  catch (const Tango::DevFailed& e)
  {
    ERROR_STACK->devfailed(e, _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }
  catch (...) 
  {
    ERROR_STACK->unknown("group_write_attribute", _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }

  ERROR_STACK->no_error("group_write_attribute", _TO_LV_ERR_STACK(error_stack_));

  return kNoError;
}

//=============================================================================
// BINDING C-INTERFACE TO LABVIEW: group_command_inout
//=============================================================================
long group_command_inout (long _group_id,
													const char * _cmd_name,
                          void* values_inout_,
                          void* error_stack_)
{
  try
  {
    Tango::Except::throw_exception(_CPTC("missing feature"),
                                   _CPTC("sorry but 'group_command_inout' is not yet implemented"),
                                   _CPTC("group_command_inout"));
  }
  catch (const Tango::DevFailed& e)
  {
    ERROR_STACK->devfailed(e, _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }
  catch (...) 
  {
    ERROR_STACK->unknown("group_command_inout", _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }
  
  ERROR_STACK->no_error("group_command_inout", _TO_LV_ERR_STACK(error_stack_));
  
  return kNoError;
}
                                  
//=============================================================================
// BINDING C-INTERFACE TO LABVIEW: set_timeout
//=============================================================================
long set_timeout (const char* _dev_name, unsigned long _tmo, void* error_stack_)
{
  try
  {
    //- avoid too short tmo (min = 1 second)
    if (_tmo < 1000) _tmo = 1000;

    //- get local descriptor for <_dev_name>
    DeviceDescriptor * ddesc = DEV_REP->device_desc(_dev_name);

    //- change timeout on <_dev_name>
    ddesc->proxy()->set_timeout(_tmo);
  }
  catch (const Tango::DevFailed& e)
  {
    ERROR_STACK->devfailed(e, _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }
  catch (...) 
  {
    ERROR_STACK->unknown("set_timeout", _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }

  ERROR_STACK->no_error("set_timeout", _TO_LV_ERR_STACK(error_stack_));

  return kNoError;
}

//=============================================================================
// BINDING C-INTERFACE TO LABVIEW: extract_archived_data
//=============================================================================
long extract_archived_data (const char* _db_extractor_dev_name,
                            const char* _dynamic_attr_name,
                            unsigned long _dynamic_attr_history_len,
                            void* values_inout_,
                            void* error_stack_)
{
  std::vector<Tango::DeviceAttributeHistory>* archived_attr_values = 0;

  try
  {
    //- get local descriptor for <_db_extractor_dev_name>
    DeviceDescriptor * ddesc = DEV_REP->device_desc(_db_extractor_dev_name);

    //- we now need to read <dynamic_attr_name> history on <db_extractor_dev_name> to get the archived data!
    archived_attr_values = ddesc->proxy()->attribute_history(_dynamic_attr_name, _dynamic_attr_history_len);

    //- dyn. attr. workaround: close/(re)open device connection to get the <dynamic_attr_name> data type
    DEV_REP->remove_device(_db_extractor_dev_name);
    ddesc = DEV_REP->device_desc(_db_extractor_dev_name);
    
    //- reinterpret <values_inout_> as a <LvAttributeValueArrayHdl>
    LvAttributeValueArrayHdl avah = reinterpret_cast<LvAttributeValueArrayHdl>(values_inout_);

    //- be sure the input array as correct size (can't resize it from a DLL!)
    if ((*avah)->length != static_cast<long>(_dynamic_attr_history_len))
    {
      Tango::Except::throw_exception(_CPTC("invalid parameter"),
                                     _CPTC("unexpected <destination array> size"),
                                     _CPTC("TangoBinding::extract_archived_data"));
    }

    //- populate each LvAttributeValue
    for (long i = 0; i < static_cast<long>(_dynamic_attr_history_len); i++)
    {
      //- point to the i-th LvAttributeValue in the array
      LvAttributeValuePtr av = (*avah)->attr_values + i;
      //- decode attribute value
      DATA_ADAPTER->tango_to_lv_attribute_value(ddesc, _dynamic_attr_name, (*archived_attr_values)[i], av); 
    }
    
    //- avoid memory leak...
    delete archived_attr_values;
  }
  catch (const Tango::DevFailed& e)
  {
    if (archived_attr_values) delete archived_attr_values;
    ERROR_STACK->devfailed(e, _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }
  catch (...) 
  {
    if (archived_attr_values) delete archived_attr_values;
    ERROR_STACK->unknown("extract_archived_data", _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }

  ERROR_STACK->no_error("extract_archived_data", _TO_LV_ERR_STACK(error_stack_));

  return kNoError;
}
