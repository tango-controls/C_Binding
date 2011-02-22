// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for LabVIEW
//
// = FILENAME
//   GroupRepository.h
//
// = AUTHOR
//   Nicolas Leclercq - SOLEIL
//
// ============================================================================

#ifndef _GROUP_REPOSITORY_H_
#define _GROUP_REPOSITORY_H_

//=============================================================================
// DEPENDENCIEs
//=============================================================================
#include <map>
#include "StandardHeader.h"
#include "GroupDescriptor.h"

//=============================================================================
// DEFINEs
//=============================================================================
#define GRP_REP (GroupRepository::instance())

//=============================================================================
// CLASS : GroupRepository (SINGLETON)
//=============================================================================
class GroupRepository
{
  friend class ObjectManager;

  typedef std::map<long, GroupDescriptor*> GroupMap;
  // GroupMap: a STL map of <GroupMap*> indexed by group identifier

  typedef GroupMap::iterator GroupMapIterator;
  // GroupMapIterator: a GroupMap iterator

public:

  static GroupRepository * instance ();
  // Returns the unique instance of <GroupRepository>.

  long add (const std::string& name, 
            const DeviceList & devices_to_add,
            const DeviceList & devices_to_remove)
    throw (Tango::DevFailed);
  // Given a group name, create the group then insert it into the repository.

  void remove (long group_id)
    throw (Tango::DevFailed);
  // Given a group identify, remove it from the repository.

  GroupDescriptor * get (long group_id);
  // Given a group id, returns a ref to this group or NULL if no such group

private:
  static long group_cnt;
  //- group identifier 
  
  static void init ()
    throw (Tango::DevFailed);
  //- Instanciates the singleton.
  
  static void cleanup ()
    throw (Tango::DevFailed);
  //- Releases the singleton.

  static GroupRepository * instance_;
  //- The unique instance of <GroupRepository>.

  void remove_group_i (long group_id);
  //- Providing a group id, removes it from the repository.

  GroupMap group_map_;
  //- The actual repository (see typedef above).

  omni_mutex lock_;
  //- Use a lock in order to make the repository thread-safe.

  // = Disallow these operations (except for TangoClientXop).
  //---------------------------------------------------------
  GroupRepository();
  GroupRepository (const GroupRepository&);
  virtual ~GroupRepository();
  void operator= (const GroupRepository&); 
};

//=============================================================================
// INLINED CODE
//=============================================================================
#if defined (_LV_INLINE_)
  #include "GroupRepository.i"
#endif 

#endif // _GROUP_REPOSITORY_H_

   
   
