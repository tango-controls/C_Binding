// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for Igor Pro
//
// = FILENAME
//   MonitoredObjectManager.h
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

#ifndef _MONITORED_OBJECT_MANAGER_H_
#define _MONITORED_OBJECT_MANAGER_H_

// ============================================================================
// DEPENDENCIES
// ============================================================================
#include <yat/threading/Task.h>
#include <yat4tango/ExceptionHelper.h>

//=============================================================================
// #DEFINEs
//=============================================================================
#define MON_OBJ_MANAGER (MonitoredObjectManager::instance())

//=============================================================================
// FORWARD DECLARATION
//=============================================================================
class MonitoredDevice;

//=============================================================================
// TYPEDEFs
//=============================================================================
typedef std::map<std::string, MonitoredDevice*> MonitoredDeviceMap;
typedef MonitoredDeviceMap::iterator MonitoredDeviceMapIt;
//-----------------------------------------------------------------------------
typedef unsigned long PpMs;
typedef std::multiset<PpMs, std::less<PpMs> > PollingPeriodMSet;
typedef PollingPeriodMSet::iterator PollingPeriodMSetIt;
//-----------------------------------------------------------------------------

//=============================================================================
// CLASS : MonitoredObjectManager (SINGLETON)
//=============================================================================
class MonitoredObjectManager : public yat::Task
{
  //**************************************************************
  //* CURRENT IMPL. GARANTEE: NO CALLBACK TO IGOR FROM TASK-THREAD
  //**************************************************************
  
  friend class TangoBinding;

public:
  static MonitoredObjectManager* instance ();
  //- Returns the unique instance of <MonitoredObjectManager>.

  int add_attribute (const std::string& dev_name,
                     const std::string& attr_name,
                     const std::string& full_path_to_igor_obj,
                     unsigned long polling_period,
                     long& client_id);
  //- Add the specified attribute to the monitored objects list

  int remove_attribute (const std::string& dev_name,
                        const std::string& attr_name,
                        long client_id = -1);
  //- Removes the specified attribute from the monitored objects list

  int suspend_monitor (const std::string& dev_name,
                       const std::string& obj_name,
                       long client_id = -1);
  //- Suspends the specified monitor

  int resume_monitor (const std::string& dev_name,
                      const std::string& obj_name,
                      long client_id = -1);
  //- Resume the specified monitor

  int change_monitor_period (const std::string& dev_name,
                             const std::string& attr_name,
                             unsigned long polling_period,
                             long client_id = -1);
  //- Change a monitor period

  void export_values ();
  //- Exports all values to igor
  
  inline bool has_objects_to_monitor() const {
  //- Something to monitor?
    return ! mdev_map_.empty();
  }
  

protected:
	//- handle_message -----------------------
	virtual void handle_message (yat::Message& msg)
		throw (yat::Exception);

private:
  static int init ();
  //- Instanciates the singleton.
  
  static void cleanup ();
  //- Releases the singleton.

  static MonitoredObjectManager* instance_;
  //- The unique instance of <MonitoredObjectManager>.

  void import_values ();
  //- Imports all values from devices

  MonitoredDeviceMap mdev_map_;
  //- The monitored devices map

  PollingPeriodMSet pp_mset_;
  //- The pollings periods

  // = Disallow these operations (except for TangoClientXop).
  //---------------------------------------------------------
  MonitoredObjectManager();
  MonitoredObjectManager (const MonitoredObjectManager&);
  virtual ~MonitoredObjectManager();
  void operator= (const MonitoredObjectManager&); 
};

#if defined (__XDK_INLINE__)
  #include "MonitoredObjectManager.i"
#endif 

#endif // _MONITORED_OBJECT_MANAGER_H_

   
   