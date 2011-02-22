// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for LabVIEW
//
// = FILENAME
//   EventManager.cpp
//
// = AUTHOR
//   Nicolas Leclercq - SOLEIL
//
// ============================================================================

//=============================================================================
// DEPENDENCIES
//=============================================================================
#ifndef WIN32
#  include <sys/time.h> 
#endif 
#include "StandardHeader.h"
#include "DeviceRepository.h"
#include "DataAdapter.h"
#include "EventManager.h"
#include "ErrorStack.h"

#if !defined (_LV_INLINE_)
  #include "EventManager.i"
#endif 


//=============================================================================
// STATIC MEMBERS
//=============================================================================
EventManager* EventManager::instance_ = 0;
std::vector<std::string> EventManager::default_periodic_filter;

//*****************************************************************************
//*****************************************************************************
// CLASS: EventCallback
//*****************************************************************************
//*****************************************************************************
//=============================================================================
// EventCallback::EventCallback
//=============================================================================
EventCallback::EventCallback (const std::string& _dev, 
                              const std::string& _attr,
                              LVUserEventRef _lv_evt_refnum)
  : Tango::CallBack(),
    dev_(_dev),
    attr_(_attr),
    lv_evt_refnum_(_lv_evt_refnum),
    tango_evt_refnum_(0)
{
  //- noop
}

//=============================================================================
// EventCallback::EventCallback
//=============================================================================  
EventCallback::~EventCallback (void) 
{
  //- noop
}

//=============================================================================
// EventCallback::push_event
//=============================================================================  
void EventCallback::push_event (Tango::EventData* _evt_data) 
{
  try
  {
    if (_evt_data)
    {
      this->push_lv_event(_evt_data);
    }
  }
  catch (...)
  {
    //- can't do anything here!
  }
}

//=============================================================================
// EventCallback::push_lv_event
//=============================================================================
void EventCallback::push_lv_event (Tango::EventData* _ea)
  throw (Tango::DevFailed)
{
  //- alllocate the event data structure
  UPtr up = ::AZNewPClr(sizeof(LvAttributeValue));
  if (up == 0) {
      Tango::Except::throw_exception(_CPTC("out of memory error"),
                                     _CPTC("binding error: memory allocation failed"),
                                     _CPTC("EventCallback::push_lv_event"));
  }

  LvAttributeValuePtr lvav = reinterpret_cast<LvAttributeValuePtr>(up);

  //- set device name
  long err = ::NumericArrayResize(iB, 1L, (UHandle*)&(lvav->dev_name), this->dev_.size());
  if (err != noErr) {
    Tango::Except::throw_exception(_CPTC("out of memory error"),
                                   _CPTC("binding error: memory allocation failed"),
                                   _CPTC("EventCallback::push_lv_event"));
  }
  LvStringHdl sh = reinterpret_cast<LvStringHdl>(lvav->dev_name);
  (*sh)->length = this->dev_.size();
  ::memcpy((*sh)->data, this->dev_.c_str(), this->dev_.size());

  //- set attr. name
  err = ::NumericArrayResize(iB, 1L, (UHandle*)&(lvav->attr_name), this->attr_.size());
  if (err != noErr) {
    Tango::Except::throw_exception(_CPTC("out of memory error"),
                                   _CPTC("binding error: memory allocation failed"),
                                   _CPTC("EventCallback::push_lv_event"));
  }
  sh = reinterpret_cast<LvStringHdl>(lvav->attr_name);
  (*sh)->length = this->attr_.size();
  ::memcpy((*sh)->data, this->attr_.c_str(), this->attr_.size());

  //- access the Tango::DeviceAttribute
  Tango::DeviceAttribute *da = _ea->attr_value;

  //- in case of error: just set error code and stack then return
  if (_ea->err) 
  {
    lvav->error_code = -1;
    Tango::DevFailed df(_ea->errors);
    ERROR_STACK->devfailed(df, lvav->error_stack);
    ::PostLVUserEvent(this->lv_evt_refnum_, lvav);
    return;
  }

  //- set attribute quality (both value and string)
  std::string s;
  switch (da->quality) 
  {
    case Tango::ATTR_VALID:
	    lvav->quality_val = 0;
      s = "VALID";
	    break;
    case Tango::ATTR_INVALID:
	    lvav->quality_val = 1;
      s = "INVALID";
	    break;
    case Tango::ATTR_ALARM:
	    lvav->quality_val = 2;
      s = "ALARM";
	    break;
    default:
	    lvav->quality_val = -1;
      s = "UNKNOWN";
	    break;
  };
  err = ::NumericArrayResize(iB, 1L, (UHandle*)&(lvav->quality_str), s.size());
  if (err != noErr) {
    Tango::Except::throw_exception(_CPTC("out of memory error"),
                                   _CPTC("binding error: memory allocation failed"),
                                   _CPTC("EventCallback::push_lv_event"));
  }
  sh = reinterpret_cast<LvStringHdl>(lvav->quality_str);
  (*sh)->length = s.size();
  ::memcpy((*sh)->data, s.c_str(), s.size());

  //- set timestamp
  lvav->timestamp = kTIME_OFFSET +
                    (double)da->time.tv_sec +
                    1.E-6 * (double)da->time.tv_usec + 
                    1.E-9 * (double)da->time.tv_nsec;
  //- set dims
  Tango::AttributeDimension r_dim = da->get_r_dimension();
  lvav->dim_x = r_dim.dim_x;
  lvav->dim_y = r_dim.dim_y;

  //- reset error code
  lvav->error_code = 0;
  ERROR_STACK->no_error("TangoBinding::EventManager", lvav->error_stack);
  //- decode data
  try 
  {
    DeviceDescriptor * ddesc = DEV_REP->device_desc(this->dev_);
    int attr_id = ddesc->attr_exists(this->attr_);
    DATA_ADAPTER->decode_attr(ddesc, attr_id, *da, lvav->data);
  }
  catch (const Tango::DevFailed& e) 
  {
    //- set error code 
    lvav->error_code = -1;
    //- set error stack
    ERROR_STACK->devfailed(e, lvav->error_stack); 
  }
  catch (...) 
  {
    //- set error code 
    lvav->error_code = -1;
    //- set error stack 
    ERROR_STACK->unknown("EventManager::push_lv_event", lvav->error_stack);
  }

  ::PostLVUserEvent(this->lv_evt_refnum_, lvav);
}

//*****************************************************************************
//*****************************************************************************
// CLASS: EventManager
//*****************************************************************************
//*****************************************************************************

//=============================================================================
// EventManager::init
//=============================================================================
int EventManager::init (void)
{
  if (EventManager::instance_ != 0)
    return 0;

  EventManager::instance_ = new EventManager;

  return (EventManager::instance_) ? 0 : -1; 
}

//=============================================================================
// EventManager::cleanup
//=============================================================================
void EventManager::cleanup (void)
{
  if (EventManager::instance_) {
    delete EventManager::instance_;
    EventManager::instance_ = 0;
  }
}

//=============================================================================
// EventManager::EventManager 
//=============================================================================
EventManager::EventManager (void) : events_have_been_used_(false)
{
  //- default event filter: all periodic events
  EventManager::default_periodic_filter.push_back(std::string("$counter % 1 == 0"));
}

//=============================================================================
// EventManager::~EventManager 
//=============================================================================
EventManager::~EventManager (void)
{
  this->clear();
}

//=============================================================================
// EventManager::subscribe
//=============================================================================
void EventManager::subscribe (const std::string& dev, 
                              const std::string& attr,
                              LVUserEventRef lv_evt_refnum,
                              Tango::EventType evt_type,
                              const std::vector<std::string>& filters)
  throw (Tango::DevFailed)
{ 
  omni_mutex_lock guard(this->lock_);

  if (this->subscription_exists(lv_evt_refnum)) 
  {
    return;
  }


  DeviceDescriptor * ddesc = DEV_REP->device_desc(dev);
  if (ddesc == 0) {
    std::string d = "failed to subscribe to " + dev + "/" + attr;
    Tango::Except::throw_exception(_CPTC("could not obtain a valid device reference"),
                                   d.c_str(),
			                             _CPTC("EventManager::subscribe"));
  }
   
  EventCallback * cb = new EventCallback(dev, attr, lv_evt_refnum);
  if (cb == 0) {
    std::string d = "failed to subscribe to " + dev + "/" + attr;
    Tango::Except::throw_exception(_CPTC("out of memory error"),
                                   d.c_str(),
			                             _CPTC("EventManager::subscribe"));
  }

  try 
  {
    cb->tango_evt_refnum_ = ddesc->proxy()->subscribe_event(attr, evt_type, cb, filters);
  }
  catch (const Tango::DevFailed& e)
  {
    delete cb;
    std::string d = "failed to subscribe to " + dev + "/" + attr;
    Tango::Except::re_throw_exception(const_cast<Tango::DevFailed&>(e),
                                      _CPTC("TANGO event subscription failed [TANGO exception caught]"),
                                      d.c_str(),
			                                _CPTC("EventManager::subscribe"));
  }
  catch (...) 
  {
    delete cb;
    std::string d = "failed to subscribe to " + dev + "/" + attr;
    Tango::Except::throw_exception(_CPTC("TANGO event subscription failed [unknown error]"),
                                   d.c_str(),
			                             _CPTC("EventManager::subscribe"));
  }

  events_have_been_used_ = true;

  std::pair<EventCallbackMapIterator, bool> result;
  result = this->evt_cb_map_.insert(EventCallbackMap::value_type(lv_evt_refnum, cb));
  if (result.second == 0) {
     delete cb;
     std::string d = "failed to subscribe to " + dev + "/" + attr;
     Tango::Except::throw_exception(_CPTC("internal error"),
			                              d.c_str(),
			                              _CPTC("EventManager::subscribe"));
  }
 
} 

//=============================================================================
// EventManager::unsubscribe
//=============================================================================
void EventManager::unsubscribe (LVUserEventRef lv_evt_refnum)
  throw (Tango::DevFailed)
{ 
  omni_mutex_lock guard(this->lock_);

  if (this->subscription_exists(lv_evt_refnum) == false) 
  {
     Tango::Except::throw_exception(_CPTC("no such subscription"),
			                              _CPTC("could not find any event subscription with the specified refnum"),
			                              _CPTC("EventManager::unsubscribe"));
  }

  EventCallbackMapIterator it = this->evt_cb_map_.find(lv_evt_refnum);
  if (it->second == 0) 
  {
    Tango::Except::throw_exception(_CPTC("internal error [unexpected null CallBack]"),
			                             _CPTC("failed to unsubscribe to Tango::event"),
			                             _CPTC("EventManager::unsubscribe"));
  }

  DeviceDescriptor * ddesc = DEV_REP->device_desc(it->second->device_name());
  if (ddesc == 0) 
  {
    Tango::Except::throw_exception(_CPTC("internal error [unexpected null DeviceDescriptor]"),
			                             _CPTC("failed to unsubscribe to Tango::event"),
			                             _CPTC("EventManager::unsubscribe"));
  }

  try 
  {
    ddesc->proxy()->unsubscribe_event(it->second->tango_evt_refnum_);
  }
  catch (const Tango::DevFailed& e)
  {
    std::string d = "failed to unsubscribe to " + it->second->device_name() 
                  + "/" + it->second->attribute_name();
    Tango::Except::re_throw_exception(const_cast<Tango::DevFailed&>(e),
                                      _CPTC("TANGO exception caught"),
                                      d.c_str(),
			                                _CPTC("EventManager::unsubscribe"));
  }
  catch (...) 
  {
    std::string d = "failed to unsubscribe to " + it->second->device_name() 
                  + "/" + it->second->attribute_name();
    Tango::Except::throw_exception(_CPTC("unknown error"),
                                    d.c_str(),
			                             _CPTC("EventManager::unsubscribe"));
  }

  delete it->second;

  this->evt_cb_map_.erase(it); 
}


//=============================================================================
// EventManager::clear
//=============================================================================
void EventManager::clear (void)
{
  omni_mutex_lock guard(this->lock_);

  DeviceDescriptor * ddesc = 0;
  EventCallbackMapIterator cur = this->evt_cb_map_.begin();
  EventCallbackMapIterator end = this->evt_cb_map_.end();

  for (; cur != end; cur++) 
  {
    if (cur->second) 
    {
      ddesc = DEV_REP->device_desc(cur->second->device_name());
      if (ddesc) 
      {
        try 
        {
          ddesc->proxy()->unsubscribe_event(cur->second->tango_evt_refnum_);
        }
        catch (...) 
        {
          //- ignore exception
        }
      }
      delete cur->second;
      cur->second = 0;
    }
  } 

  this->evt_cb_map_.clear();
}

//=============================================================================
// EventManager::subscription_exists
//=============================================================================
bool EventManager::subscription_exists (LVUserEventRef _lv_evt_refnum)
{ 
  EventCallbackMapIterator it = this->evt_cb_map_.find(_lv_evt_refnum);
  return (it != this->evt_cb_map_.end()) ? true : false;
}

