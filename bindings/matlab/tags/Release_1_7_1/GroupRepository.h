// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for Matlab
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
#include <set>
#include <map>

//=============================================================================
// FORWARD DECLARATION
//=============================================================================
class Tango::Group;

//=============================================================================
// TYPEDEFs
//=============================================================================
typedef std::set<Tango::Group*> GroupSet;
typedef GroupSet::iterator GroupSetIterator;

//=============================================================================
// DEFINEs
//=============================================================================
#define GRP_REP (GroupRepository::instance())

//=============================================================================
// CLASS : GroupRepository (SINGLETON)
//=============================================================================
class GroupRepository : public GroupSet
{
  friend class TangoBinding;

public:
  static GroupRepository* instance (void);
  // Returns the unique instance of <GroupRepository>.

  long add (const std::string& name);
  // Given a group name, create the group then insert it into the repository.

  int remove (long group_id, bool release = true);
  // Given a group name, remove the group from the repository.

  Tango::Group* get (long group_id, bool set_error = true);
  // Given a group id, returns a ref to this group or NULL if no such group.

  Tango::Group* get (const std::string& name, bool set_error = true);
  // Given a group name, returns its id.

private:
  static int init (void);
  //- Instanciates the singleton.
  
  static void cleanup (void);
  //- Releases the singleton.

  static GroupRepository* instance_;
  //- The unique instance of <GroupRepository>.

#if defined(_MEX_DEBUG_)
  void dump (long group_id = -1);
  //- Dump repository content
#endif

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
#if defined (_MEX_INLINE_)
  #include "GroupRepository.i"
#endif 

#endif // _GROUP_REPOSITORY_H_

   
   
