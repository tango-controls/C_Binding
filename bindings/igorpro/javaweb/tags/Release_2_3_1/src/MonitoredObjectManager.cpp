// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for Igor Pro
//
// = FILENAME
//   MonitoredObjectManager.cpp
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================


// ============================================================================
// DEPENDENCIES
// ============================================================================
#include "XDK_StandardHeaders.h"
#include <yat/XString.h>
#include "DevRepository.h"
#include "MonitoredObject.h"
#include "MonitoredObjectManager.h"

#if !defined (__XDK_INLINE__)
  #include "MonitoredObjectManager.i"
#endif 

// ============================================================================
// SOME USER DEFINED MESSAGES
// ============================================================================
#define kSTART_MONITORING (yat::FIRST_USER_MSG + 1000)
#define kSTOP_MONITORING  (yat::FIRST_USER_MSG + 1001)
//-----------------------------------------------------------------------------

// ============================================================================
// SINGLETON
// ============================================================================
MonitoredObjectManager * MonitoredObjectManager::instance_ = 0;

// ============================================================================
// MonitoredObjectManager::init <STATIC MEMBER>
// ============================================================================
int MonitoredObjectManager::init ()
{
	//- already instanciated?
	if (MonitoredObjectManager::instance_)
		return kNoError;

	try
	{
		//- instanciate
		MonitoredObjectManager::instance_ = new MonitoredObjectManager();
		if (MonitoredObjectManager::instance_ == 0)
			throw std::bad_alloc();

    //- start the task
    try
    {
      MonitoredObjectManager::instance_->go (kDEFAULT_MSG_TMO_MSECS);
    }
    catch (...)
    {
      XDK_UTILS->push_error("failed to start the monitor manager",
                            "the monitor manager thread refused to start",
                            "MonitoredObjectManager::add_attribute");
      return kError;
    }
	}
	catch (const std::bad_alloc&)
	{
		//- THROW_DEVFAILED(_CPTC("OUT_OF_MEMORY"),
		//- 								_CPTC("galil::MonitoredObjectManager::instance_ allocation failed"),
		//- 								_CPTC("galil::MonitoredObjectManager::init")); 
    return kError;
	}
	catch (Tango::DevFailed& df)
	{
		//- RETHROW_DEVFAILED(df,
		//- 									_CPTC("SOFTWARE_ERROR"),
		//- 									_CPTC("galil::MonitoredObjectManager::instance_ initialisation failed"),
		//- 									_CPTC("galil::MonitoredObjectManager::init")); 
    return kError;
	}
	catch (const yat::Exception& ye)
	{		
	  //- _YAT_TO_TANGO_EXCEPTION(ye, te);
	  //- throw te;  
    return kError;
	}
	catch (...)
	{
		//- THROW_DEVFAILED(_CPTC("UNKNOWN_ERROR"),
		//- 								_CPTC("galil::MonitoredObjectManager::instance_ initialisation failed [unknown exception]"),
		//- 								_CPTC("galil::MonitoredObjectManager::init")); 
    return kError;
	}

  return kNoError;
}

// ============================================================================
// MonitoredObjectManager::cleanup  <STATIC MEMBER>
// ============================================================================
void MonitoredObjectManager::cleanup ()
{
	try
	{
		//- check
		if (! MonitoredObjectManager::instance_)
			return;
 
		//- ask the underlying yat::Task to exit
		MonitoredObjectManager::instance_->exit();

		//- reset singleton 
		MonitoredObjectManager::instance_ = 0; 

		//- NEVER CALL DELETE ON <MonitoredObjectManager::instance_>
		//- see yat::Task impl for more info...
	}
	catch (...)
  {

  }
}

// ============================================================================
// MonitoredObjectManager::MonitoredObjectManager
// ============================================================================
MonitoredObjectManager::MonitoredObjectManager ()
{
	//- disable timeout msgs
	this->enable_timeout_msg(false);

	//- disable periodic msgs
  this->set_periodic_msg_period(0);
	this->enable_periodic_msg(false);
}

// ============================================================================
// MonitoredObjectManager::~MonitoredObjectManager
// ============================================================================
MonitoredObjectManager::~MonitoredObjectManager ()
{
  this->remove_attribute("*", "*"); 
}

// ============================================================================
// MonitoredObjectManager::add_attribute
// ============================================================================
int MonitoredObjectManager::add_attribute (const std::string& dev_name,
                                           const std::string& attr_name,
                                           const std::string& full_path_to_igor_obj,
                                           unsigned long polling_period,
                                           long& client_id)
{
  //- invalid client id
  client_id = -1;

  {
    //- enter critical section
    yat::AutoMutex<yat::Mutex> guard(this->m_lock);

    MonitoredDevice * md = 0;

    //- search for <obj> in the repository
    MonitoredDeviceMapIt it = this->mdev_map_.find(dev_name);
  
    //- found ?
    if (it != this->mdev_map_.end()) 
    {
    //-#if defined (_XDK_DEBUG_)
    //-      XDK_UTILS->notify("Tango-Binding::%s already has monitored attributes", dev_name.c_str());  
    //-#endif
      md = it->second;

      PollingPeriodMSetIt ppit = 
              this->pp_mset_.find(md->minimum_polling_period());

      if (ppit != this->pp_mset_.end())
          this->pp_mset_.erase(ppit);
    }
    else
    {
      //- search failed, create the descriptor
      md = new MonitoredDevice(dev_name);
      if (md == 0) 
      {
        XDK_UTILS->set_error("API_MemoryAllocation",
                             "MonitoredDevice allocation failed",
                             "MonitoredObjectManager::add_attribute");
        return kError;
      }
      //- store the MonitoredAttribute into the repository
      std::pair<MonitoredDeviceMapIt, bool> result;
      result = this->mdev_map_.insert(MonitoredDeviceMap::value_type(dev_name, md));
      if (result.second == 0) 
      {
        XDK_UTILS->set_error("XOP internal error",
                             "MonitoredDeviceMap::insert failed",
                             "MonitoredObjectManager::add_attribute");
        return kError;
      }
    }

    //- register attribute
    md->add_attribute(attr_name,
                      full_path_to_igor_obj,
                      polling_period,
                      client_id);

    //- push the requested polling period into our multiset
    this->pp_mset_.insert(md->minimum_polling_period());

    //- set the Task period msg period to the lowest polling period
    this->set_periodic_msg_period(*(this->pp_mset_.begin()));

//-#if defined (_XDK_DEBUG_)
//-    XDK_UTILS->notify("Tango-Binding::%s added to monitored device list", dev_name.c_str());  
//-    XDK_UTILS->notify("Tango-Binding::periodic msg period set to %d msecs", this->get_periodic_msg_period());  
//-#endif
  }

	this->enable_periodic_msg(true);

  return kNoError;
}

// ============================================================================
// MonitoredObjectManager::remove_attribute
// ============================================================================
int MonitoredObjectManager::remove_attribute (const std::string& dev_name,
                                              const std::string& attr_name,
                                              long client_id)
{
  //- enter critical section
  yat::AutoMutex<yat::Mutex> guard(this->m_lock);

  if (dev_name == "*") 
  {
    MonitoredDeviceMapIt cur = this->mdev_map_.begin();
    MonitoredDeviceMapIt end = this->mdev_map_.end();
    for (; cur != end; cur++) 
      delete cur->second;
    this->mdev_map_.clear();
#if defined (_XDK_DEBUG_)
    XDK_UTILS->notify("Tango-Binding::monitored device list cleared");  
#endif
  }
  else 
  {
    MonitoredDeviceMapIt it = this->mdev_map_.find(dev_name);
    if (it != this->mdev_map_.end()) 
    {
      PollingPeriodMSetIt ppit = 
              this->pp_mset_.find(it->second->minimum_polling_period());

      if (ppit != this->pp_mset_.end())
          this->pp_mset_.erase(ppit);

      it->second->remove_attribute(attr_name, client_id);

      if (! it->second->has_monitored_objects())
      {
        delete it->second;
        this->mdev_map_.erase(it);
#if defined (_XDK_DEBUG_)
        XDK_UTILS->notify("Tango-Binding::%s removed from monitored device list", dev_name.c_str());  
#endif
      }
      else
      {
        this->pp_mset_.insert(it->second->minimum_polling_period());
      }
    }
  }

  if (this->mdev_map_.empty())
  {
    this->pp_mset_.clear();
    this->enable_periodic_msg(false);
#if defined (_XDK_DEBUG_)
    XDK_UTILS->notify("Tango-Binding::periodic msg disabled");  
#endif 
  }
  else
  {
    this->set_periodic_msg_period(*(this->pp_mset_.begin()));
#if defined (_XDK_DEBUG_)
    XDK_UTILS->notify("Tango-Binding::periodic msg period set to %d msecs", this->get_periodic_msg_period());  
#endif
  }

  return kNoError;
}

// ============================================================================
// MonitoredObjectManager::suspend_monitor
// ============================================================================
int MonitoredObjectManager::suspend_monitor (const std::string& dev_name,
                                             const std::string& obj_name,
                                             long client_id)
{
  //- enter critical section
  yat::AutoMutex<yat::Mutex> guard(this->m_lock);

  //- search for <device_name> in the repository
  MonitoredDeviceMapIt it = this->mdev_map_.find(dev_name);

  //- found ?
  if (it != this->mdev_map_.end()) 
    it->second->suspend_monitor(obj_name, client_id);

  return kNoError;  
}

// ============================================================================
// MonitoredObjectManager::resume_monitor
// ============================================================================
int MonitoredObjectManager::resume_monitor (const std::string& dev_name,
                                            const std::string& obj_name,
                                            long client_id)
{
  //- enter critical section
  yat::AutoMutex<yat::Mutex> guard(this->m_lock);

  //- search for <device_name> in the repository
  MonitoredDeviceMapIt it = this->mdev_map_.find(dev_name);

  //- found ?
  if (it != this->mdev_map_.end()) 
    it->second->resume_monitor(obj_name, client_id);

  return kNoError;  
}

// ============================================================================
// MonitoredObjectManager::change_monitor_period
// ============================================================================
int MonitoredObjectManager::change_monitor_period (const std::string& dev_name,
                                                   const std::string& attr_name,
                                                   unsigned long polling_period,
                                                   long client_id)
{
  {
    //- enter critical section
    yat::AutoMutex<yat::Mutex> guard(this->m_lock);

    //- search for <obj> in the repository
    MonitoredDeviceMapIt it = this->mdev_map_.find(dev_name);
  
    //- found ?
    if (it != this->mdev_map_.end()) 
    {
        PollingPeriodMSetIt ppit = 
            this->pp_mset_.find(it->second->minimum_polling_period());

        if (ppit != this->pp_mset_.end())
          this->pp_mset_.erase(ppit);

        it->second->change_polling_period(attr_name, polling_period, client_id);

        this->pp_mset_.insert(it->second->minimum_polling_period());

        this->set_periodic_msg_period(*(this->pp_mset_.begin()));
    }
  }

//-#if defined (_XDK_DEBUG_)
//-  XDK_UTILS->notify("Tango-Binding::periodic msg period set to %d msecs", 
//-                    this->get_periodic_msg_period());  
//-#endif

  return kNoError;
}

// ============================================================================
// MonitoredObjectManager::handle_message
// ============================================================================
void MonitoredObjectManager::handle_message (yat::Message& _msg)
	throw (yat::Exception)
{
#if defined (_XDK_DEBUG_)
  static yat::Timestamp last_ts;
#endif

  try
  {
	  //- handle msg
	  switch (_msg.type())
	  {
		  //- TASK_INIT ----------------------
		  case yat::TASK_INIT:
			  {
				  //- "initialization" code goes here
			  } 
			  break;
		  //- TASK_EXIT ----------------------
		  case yat::TASK_EXIT:
			  {
				  //- "release" code goes here
			  }
			  break;
		  //- TASK_PERIODIC ------------------
		  case yat::TASK_PERIODIC:
      {
//-#if defined (_XDK_DEBUG_)
//-          yat::Timestamp now;
//-          _GET_TIME(now);
//-          XDK_UTILS->notify("MonitoredObjectManager::handle_message::TASK_PERIODIC::elapsed::%.2f msecs::pmp::%d msecs",
//-                            _ELAPSED_MSEC(last_ts, now),
//-                            this->get_periodic_msg_period()); 
//-          _GET_TIME(last_ts);
//-#endif
          this->import_values();
			  }
			  break;
			//- default ------------------------
		  default:
			  break;
	  }
	}
	catch (...)
	{		

	}
}

// ============================================================================
// MonitoredObjectManager::import_values
// ============================================================================
void MonitoredObjectManager::import_values ()
{
  //- enter critical section
  yat::AutoMutex<yat::Mutex> guard(this->m_lock);

  if (this->mdev_map_.empty())
    return;

  MonitoredDeviceMapIt cur = this->mdev_map_.begin();
  MonitoredDeviceMapIt end = this->mdev_map_.end();
  for (; cur != end; cur++)
    cur->second->import_values(this->get_periodic_msg_period());
}

// ============================================================================
// MonitoredObjectManager::export_values
// ============================================================================
void MonitoredObjectManager::export_values ()
{
  //- enter critical section
  yat::AutoMutex<yat::Mutex> guard(this->m_lock);

  //- something to export?
  if (this->mdev_map_.empty())
    return;

  MonitoredDeviceMapIt cur = this->mdev_map_.begin();
  MonitoredDeviceMapIt end = this->mdev_map_.end();
  for (; cur != end; cur++)
    cur->second->export_values();
}

