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
#include "DataAdapter.h"
#include "EventManager.h"
#include "ObjectManager.h" 

//-----------------------------------------------------------------------------
// CONSTS
//-----------------------------------------------------------------------------
const int kNoError =  0;
const int kError   = -1;

//=============================================================================
// MACROS
//=============================================================================
#define _TO_LV_ERR_STACK(X) reinterpret_cast<LvStringArrayHdl&>(X)

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// WIN32 DLL EXPORT MACRO
//-----------------------------------------------------------------------------
#if defined(WIN32)
# define LVB_EXPORT extern "C" __declspec (dllexport)
#else
# define LVB_EXPORT extern "C"   
#endif

//=============================================================================
// BINDING C-INTERFACE TO LABVIEW
//=============================================================================
LVB_EXPORT long register_vi (const char*, void*);
LVB_EXPORT long unregister_vi (const char*, void*);
LVB_EXPORT long ping (const char*, void*, void*);
LVB_EXPORT long state (const char*, void*, void*);
LVB_EXPORT long status (const char*, void*, void*);
LVB_EXPORT long command_inout (const char*, const char*, void*, void*, void*);
LVB_EXPORT long read_attributes (const char*, void*, void*);
LVB_EXPORT long write_attributes (const char*, void*, void*);
LVB_EXPORT long subscribe_event (const char*,const char*, LVRefNum*, void*);
LVB_EXPORT long unsubscribe_event (LVRefNum *, void*);

//=============================================================================
// WINDOWS DLL HELL: DllMain
//=============================================================================
#if (TBFL_HAS_STATIC_OBJ_MANAGER == 0) 
# if defined(WIN32)
BOOL APIENTRY DllMain (HANDLE h, DWORD  reason_for_call, LPVOID r)
{
  switch (reason_for_call)
  {
    case DLL_PROCESS_ATTACH:
      try 
      {
        ObjectManager::init();
      }
      catch (...) 
      {
        return FALSE;
      }
      break;
    case DLL_PROCESS_DETACH:
      try 
      {
        if (EVT_MANAGER->events_have_been_used() == false)
        {
          ObjectManager::fini();
        }
      }
      catch (...) 
      {
        //- ignore any error
      }
    default:
      break;
  }
  return TRUE;
}

# elif (defined(_linux_) || defined (__linux__)) && defined(__GNUG__)

//=============================================================================
// SHARED LIB INITIALIZATION ROUTINE
//=============================================================================
extern "C" void __attribute__ ((constructor)) init_shared_lib (void);
//-----------------------------------------------------------------------------
void __attribute__ ((constructor)) init_shared_lib (void)
{
  //- ::printf("***** -> init_shared_lib *****");

  try 
  {
    ObjectManager::init();
  }
  catch (...) 
  {
    //- ignore any exception
  }

  //- ::printf("***** <- init_shared_lib *****");
}

//=============================================================================
// SHARED LIB CLEANUP
//=============================================================================
extern "C" void __attribute__ ((destructor)) cleanup_shared_lib (void);
//-----------------------------------------------------------------------------
void __attribute__ ((destructor)) cleanup_shared_lib (void)
{
  //- ::printf("***** -> cleanup_shared_lib *****");

  try 
  {
    ObjectManager::fini();
  }
  catch (...) 
  {
    //- ignore any exception
  }

  //- ::printf("***** <- cleanup_shared_lib *****");
}

# endif

#endif //- (TBFL_HAS_STATIC_OBJ_MANAGER == 0)

//=============================================================================
// BINDING C-INTERFACE TO LABVIEW: register_vi
//=============================================================================
long register_vi (const char* _vi_name, void* error_stack_)
{
  try
  {
    ObjectManager::register_vi(_vi_name);
  }
  catch (const Tango::DevFailed& e)
  {
    if (error_stack_)
      ERROR_STACK->devfailed(e, _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }
  catch (...) 
  {
    if (error_stack_)
      ERROR_STACK->unknown("register_vi", _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }

  if (error_stack_)
    ERROR_STACK->no_error("register_vi", _TO_LV_ERR_STACK(error_stack_));

  return kNoError;
}

//=============================================================================
// BINDING C-INTERFACE TO LABVIEW: unregister_vi
//=============================================================================
long unregister_vi (const char* _vi_name, void* error_stack_)
{
  try
  {
    ObjectManager::unregister_vi(_vi_name);
  }
  catch (const Tango::DevFailed& e)
  {
    if (error_stack_)
      ERROR_STACK->devfailed(e, _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }
  catch (...) 
  {
    if (error_stack_)
      ERROR_STACK->unknown("unregister_vi", _TO_LV_ERR_STACK(error_stack_));
    return kError;
  }

  if (error_stack_)
    ERROR_STACK->no_error("unregister_vi", _TO_LV_ERR_STACK(error_stack_));

  return kNoError;
}

//=============================================================================
// BINDING C-INTERFACE TO LABVIEW: ping
//=============================================================================
long ping (const char* _dev_name, void* argout_, void* error_stack_)
{
  try
  {
    //- get local descriptor
    DeviceDesc * ddesc = DEV_REP->device_desc(_dev_name);

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
    DeviceDesc * ddesc = DEV_REP->device_desc(_dev_name);

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
      DeviceDesc * ddesc = DEV_REP->device_desc(_dev_name); 

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

    long err = ::NumericArrayResize(iL, 
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
    DeviceDesc * ddesc = DEV_REP->device_desc(_dev_name);

    //- is <cmd_name> a valid command for <dev_name>
    int cmd_id = ddesc->cmd_exists(_cmd_name);

    //- get <argin> type for <cmd_name>
    int ai_type = (ddesc->cmd_list())[cmd_id].in_type;

    //- get <argout> type for <cmd_name>
    int ao_type = (ddesc->cmd_list())[cmd_id].out_type;

    //- get <cmd_name> result in a Tango::DeviceData 
    Tango::DeviceData dd_out;

    //- encode argin then exec the command 
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

    //- get <argout> type for <cmd_name>
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
    DeviceDesc * ddesc = DEV_REP->device_desc(_dev_name);

    //- values_inout_ is a handle to an LvAttributeValueArray (i.e. an array of clusters)
    LvAttributeValueArrayHdl avah = reinterpret_cast<LvAttributeValueArrayHdl>(values_inout_);
    
    //- how many entries do we have in this array?
    long n = (*avah)->length;

    //- build <attribute name> list 
    std::vector<std::string> attr_names;
    attr_names.resize(n);
    //- build <attribute id> list 
    std::vector<int> attr_ids;
    attr_ids.resize(n);

    //- some locals
    long i = 0;
    long err = 0;
    LvStringHdl sh = 0;
    LvAttributeValuePtr av = 0;

    //- for each entry in the LvAttributeValueArray...
    for (i = 0; i < n; i++)
    {
      //- point to the i-th LvAttributeValue in the array
      av = (*avah)->attr_values + i;
      //- get attribute name 
      sh = reinterpret_cast<LvStringHdl>(av->attr_name);
      attr_names[i].assign(reinterpret_cast<char*>((*sh)->data), 
                           reinterpret_cast<char*>((*sh)->data) + (*sh)->length);
      //- get attribute id (may throw an exception)
      attr_ids[i] = ddesc->attr_exists(attr_names[i]);
    }

    //- read attributes
    attr_values = ddesc->proxy()->read_attributes(attr_names);

    //- fill each LvAttributeValue
    std::string s;
    for (i = 0; i < n; i++)
    {
      //- point to the i-th LvAttributeValue in the array
      av = (*avah)->attr_values + i;
      //- set attribute quality (both value and string)
      switch ((*attr_values)[i].quality) {
        case Tango::ATTR_VALID:
	        av->quality_val = 0;
          s = "VALID";
	        break;
        case Tango::ATTR_INVALID:
	        av->quality_val = 1;
          s = "INVALID";
	        break;
        case Tango::ATTR_ALARM:
	        av->quality_val = 2;
          s = "ALARM";
	        break;
        default:
	        av->quality_val = -1;
          s = "UNKNOWN";
	        break;
      };
      err = ::NumericArrayResize(iB, 1L, (UHandle*)&(av->quality_str), s.size());
      if (err != noErr) {
        Tango::Except::throw_exception(_CPTC("out of memory error"),
                                       _CPTC("binding error: memory allocation failed"),
                                       _CPTC("read_attributes"));
      }
      sh = reinterpret_cast<LvStringHdl>(av->quality_str);
      (*sh)->length = s.size();
      ::memcpy((*sh)->data, s.c_str(), s.size());
      //- set timestamp
      av->timestamp = kTIME_OFFSET +
                      (double)((*attr_values)[i].time.tv_sec) + 
				              1.E-6 * (double)((*attr_values)[i].time.tv_usec) + 
				              1.E-9 * (double)((*attr_values)[i].time.tv_nsec);  
      //- set dims
#if defined(HAS_TANGO_5_OR_SUP)
      Tango::AttributeDimension r_dim = (*attr_values)[i].get_r_dimension();
      av->dim_x = r_dim.dim_x;
      av->dim_y = r_dim.dim_y;
#else
      av->dim_x = (*attr_values)[i].dim_x;
      av->dim_y = (*attr_values)[i].dim_y; 
#endif
      //- set individual error code 
      av->error_code = 0;
      //- set individual error stack
      ERROR_STACK->no_error("read_attributes", av->error_stack); 
      //- decode data (may throw an exception)
      try 
      {
        DATA_ADAPTER->decode_attr(ddesc, attr_ids[i], (*attr_values)[i], av->data);
      }
      catch (const Tango::DevFailed& e) 
      {
        //- set individual error code 
        av->error_code = -1;
        //- set individual error stack
        ERROR_STACK->devfailed(e, av->error_stack); 
      }
      catch (...) 
      {
        //- set individual error code 
        av->error_code = -1;
        //- set individual error stack 
        ERROR_STACK->unknown("read_attributes", av->error_stack);
      }
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
    DeviceDesc * ddesc = DEV_REP->device_desc(_dev_name);

    //- values_inout_ is a handle to an LvAttributeValueArray (i.e. an array of clusters)
    LvAttributeValueArrayHdl avah = reinterpret_cast<LvAttributeValueArrayHdl>(values_inout_);
    
    //- how many entries do we have in this array?
    long n = (*avah)->length;

    //- some locals
    int attr_id = -1;
    std::string attr_name;
    LvStringHdl sh = 0;
    LvAttributeValuePtr av = 0;

    //- create and resize a TANGO DeviceAttribute vector 
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
        DATA_ADAPTER->encode_attr(ddesc, attr_id, av->data, da_list[i]);
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
                      void *error_stack_)
{
  try
  {
    /*   
    Tango::EventType evt_type = Tango::CHANGE_EVENT;
    std::vector<std::string> filters;
    filters.push_back(std::string("$abs_change >= 1 or $abs_change <= -1"));
    */

    Tango::EventType evt_type = Tango::PERIODIC_EVENT;
    std::vector<std::string> filters;

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
long unsubscribe_event(LVRefNum *_evt_refnum, void* error_stack_)
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
