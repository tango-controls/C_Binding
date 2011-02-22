// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for LabVIEW
//
// = FILENAME
//   GroupRepository.cpp
//
// = AUTHOR
//   Nicolas Leclercq - SOLEIL
//
// ============================================================================

//=============================================================================
// DEPENDENCIES
//=============================================================================
#include "StandardHeader.h"
#include "DeviceRepository.h"
#include "GroupRepository.h"

#if !defined (_LV_INLINE_)
# include "GroupRepository.i"
#endif 

//=============================================================================
// STATIC MEMBERS
//=============================================================================
GroupRepository* GroupRepository::instance_ = 0; 

//=============================================================================
// CONSTs
//=============================================================================
#define kALL_GROUPS -2

//=============================================================================
// GroupRepository::init
//=============================================================================
void GroupRepository::init (void)
    throw (Tango::DevFailed)
{
  if (GroupRepository::instance_ != 0)
    return;

  try
  {
    GroupRepository::instance_ = new GroupRepository;
    if (GroupRepository::instance_ == 0) 
      throw std::bad_alloc();
  }
  catch (const std::bad_alloc&)
  {
    Tango::Except::throw_exception(_CPTC("out of memory"),
			                             _CPTC("binding error: Tango::GroupRepository instanciation failed"),
			                             _CPTC("GroupRepository::init"));
  }
  catch (Tango::DevFailed& df)
  {
    Tango::Except::re_throw_exception(df,
                                      _CPTC("software error"),
			                                _CPTC("binding error: Tango::GroupRepository instanciation failed [Tango exception caught]"),
			                                _CPTC("GroupRepository::init"));
  }
  catch (...)
  {
    Tango::Except::throw_exception(_CPTC("software error"),
			                             _CPTC("binding error: Tango::GroupRepository instanciation failed [unknown exception caught]"),
			                             _CPTC("GroupRepository::init"));
  }
}

//=============================================================================
// GroupRepository::cleanup
//=============================================================================
void GroupRepository::cleanup (void)
    throw (Tango::DevFailed)
{
  if (GroupRepository::instance_) 
  {
    delete GroupRepository::instance_;
    GroupRepository::instance_ = 0;
  }
}

//=============================================================================
// GroupRepository::GroupRepository 
//=============================================================================
GroupRepository::GroupRepository (void)
{
  //- noop
}

//=============================================================================
// GroupRepository::~GroupRepository 
//=============================================================================
GroupRepository::~GroupRepository (void)
{
  //- critical section
  omni_mutex_lock guard(this->lock_);

  this->remove_group_i(kALL_GROUPS);
}

//=============================================================================
// GroupRepository::add
//=============================================================================
long GroupRepository::add (const std::string& _name, 
                           const DeviceList & _devices_to_add,
                           const DeviceList & _devices_to_remove)
    throw (Tango::DevFailed)
{ 
  Tango::Group * g = 0;

  if (! _devices_to_add.size())
  {
    Tango::Except::throw_exception(_CPTC("invalid argument"),
			                             _CPTC("can't create empty Tango::Group [empty managed device list]"),
                                   _CPTC("GroupRepository::add"));
  }

  try
  {
    g = new Tango::Group(_name);
    if (g == 0) 
      throw std::bad_alloc();

    g->add(_devices_to_add);

    if (_devices_to_remove.size())
      g->remove(_devices_to_remove);
  }
  catch (const std::bad_alloc&)
  {
    Tango::Except::throw_exception(_CPTC("out of memory"),
			                             _CPTC("binding error: Tango::Group instanciation failed"),
			                             _CPTC("GroupRepository::add"));
  }
  catch (Tango::DevFailed& df)
  {
    Tango::Except::re_throw_exception(df,
                                      _CPTC("software error"),
			                                _CPTC("binding error: Tango::Group instanciation failed [Tango exception caught]"),
			                                _CPTC("GroupRepository::add"));
  }
  catch (...)
  {
    Tango::Except::throw_exception(_CPTC("software error"),
			                             _CPTC("binding error: Tango::Group instanciation failed [unknown exception caught]"),
			                             _CPTC("GroupRepository::add"));
  }

  GroupDescriptor * gdesc = 0;

  try
  {
    gdesc = new GroupDescriptor(g);
    if (gdesc == 0) 
      throw std::bad_alloc();
  }
  catch (const std::bad_alloc &)
  {
    Tango::Except::throw_exception(_CPTC("out of memory"),
			                             _CPTC("GroupDescriptor instanciation failed"),
			                             _CPTC("GroupRepository::add"));
  }
  catch (...)
  {
    Tango::Except::throw_exception(_CPTC("unknown error"),
			                             _CPTC("GroupDescriptor instanciation failed [unknown error]"),
			                             _CPTC("GroupRepository::add"));
  }

  //- critical section on repository
  omni_mutex_lock guard(this->lock_);

  //- store the group into the repository
  std::pair<GroupMapIterator, bool> result;
  result = this->group_map_.insert(GroupMap::value_type((long)g, gdesc));
  if (result.second == 0) 
  {
    delete gdesc;
    Tango::Except::throw_exception(_CPTC("software error"),
			                             _CPTC("could insert Tango group into the repository [std::map::insert failed]"),
			                             _CPTC("GroupRepository::add"));
  }

  //- push group devices into the dev. repository
  long num_device = g->get_size();
  for (long d = 1; d <= num_device; d++)
  {
    DEV_REP->device_desc(g->get_device(d), false);
  }

  return (long)g;
}

//=============================================================================
// GroupRepository::remove
//=============================================================================
void GroupRepository::remove (long _group_id)
    throw (Tango::DevFailed)
{ 
  //- critical section
  omni_mutex_lock guard(this->lock_);

  this->remove_group_i(_group_id);
}

//=============================================================================
// GroupRepository::remove_group_i
//=============================================================================
void GroupRepository::remove_group_i (long _group_id)
{
  if (_group_id == kALL_GROUPS) 
  {
    GroupMapIterator cur = this->group_map_.begin();
    GroupMapIterator end = this->group_map_.end();
    for (; cur != end; cur++) 
    {
      GroupDescriptor * gd = cur->second; 
      if (gd)
      {
        //- remove devices from repository
        const DeviceList& dl = gd->device_list();
	      for (size_t d = 0; d < dl.size(); d++)
          DEV_REP->remove_device(dl[d]);
        //- delete group descriptor
        delete gd;
      }
    } 
    this->group_map_.clear();
  }
  else 
  {
    GroupMapIterator it = this->group_map_.find(_group_id);
    if (it != this->group_map_.end()) 
    {
      GroupDescriptor * gd = it->second; 
      if (gd)
      {
        //- remove device from repository
        const DeviceList& dl = gd->device_list();
	      for (size_t d = 0; d < dl.size(); d++)
          DEV_REP->remove_device(dl[d]);
        this->group_map_.erase(_group_id);
        //- delete group descriptor
        delete gd;
      }
    }
  }
}




