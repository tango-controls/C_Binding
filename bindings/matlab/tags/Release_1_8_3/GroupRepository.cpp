// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for Matlab
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
#include "GroupRepository.h"

#if !defined (_MEX_INLINE_)
  #include "GroupRepository.i"
#endif 

//=============================================================================
// STATIC MEMBERS
//=============================================================================
GroupRepository* GroupRepository::instance_ = 0;
//-----------------------------------------------------------------------------

//=============================================================================
// GroupRepository::init
//=============================================================================
int GroupRepository::init (void)
{
  if (GroupRepository::instance_ != 0)
    return 0;
 
  GroupRepository::instance_ = new GroupRepository;

  return (GroupRepository::instance_) ? 0 : -1;
}

//=============================================================================
// GroupRepository::cleanup
//=============================================================================
void GroupRepository::cleanup (void)
{
  if (GroupRepository::instance_) {
    delete GroupRepository::instance_;
    GroupRepository::instance_ = 0;
  }
}

//=============================================================================
// GroupRepository::GroupRepository 
//=============================================================================
GroupRepository::GroupRepository (void)
{

}

//=============================================================================
// GroupRepository::~GroupRepository 
//=============================================================================
GroupRepository::~GroupRepository (void)
{
 GroupSetIterator _cur = begin();
 GroupSetIterator _end = end();
 std::vector<Tango::Group*> gl(0);
 for (; _cur != _end; _cur++) {
   //- delete only <root> groups 
   if (*_cur && (*_cur)->get_parent() == 0) {
      gl.push_back(*_cur);
   }
 }
 clear();
 //- actual delete
 for (size_t i = 0; i < gl.size(); i++) {
    if (gl[i]) delete gl[i];
 }
}

//=============================================================================
// GroupRepository::add
//=============================================================================
long GroupRepository::add (const std::string& name)
{ 
  Tango::Group* g = new Tango::Group(name);
  if (g == 0) {
    MEX_UTILS->push_error("internal error",
			  "out of memory error (could not instanciate Tango::Group)",
			  "GroupRepository::create_group");
    SET_DEFAULT_PRHS_THEN_RETURN(kError); 
  }
  pair<GroupSetIterator, bool> result;
  result = insert(g);
  if (result.second == 0) {
    delete g;
    MEX_UTILS->set_error("group already exists",
			                   "GroupRepository::insert failed",
			                   "GroupRepository::create_group");
    SET_DEFAULT_PRHS_THEN_RETURN(kError); 
  }
  return (long)g;
}

//=============================================================================
// GroupRepository::remove
//=============================================================================
int GroupRepository::remove (long group_id, bool release)
{ 
  //- Convert from long to pointer to group
  Tango::Group* g = (Tango::Group*)group_id;

  //- Is <g> in the repository?
  GroupSetIterator it = find(g);
  if (it == end()) {
    MEX_UTILS->set_error("invalid group identifier",
			                   "no such group",
			                   "GroupRepository::remove");
    SET_DEFAULT_PRHS_THEN_RETURN(kError); 
  }

  //- Remove all groups belonging to <g> from the repository (no delete)
  vector<Tango::Group*> groups_to_remove(0);
  it = begin();
  GroupSetIterator _end = end();
  for (; it != _end; it++) {
    if (*it != g && g->contains((*it)->get_fully_qualified_name())) {
      groups_to_remove.push_back(*it);
    }
  }
  for (size_t i = 0; i < groups_to_remove.size(); i++) 
  {
    it = find(groups_to_remove[i]);
    if (it != end())
      erase(it);
  }

  //- Remove <g> from the repository
  it = find(g);
  if (it != end()) {
    if (release && *it) {
      delete *it;
    }
    erase(it);
  }
  else {
    MEX_UTILS->set_error("internal error - invalid group identifier",
			                   "could not retrieve group from repository",
			                   "GroupRepository::remove");
    SET_DEFAULT_PRHS_THEN_RETURN(kError); 
  }

  return kNoError; 
}

//=============================================================================
// GroupRepository::dump
//=============================================================================
#if defined(_MEX_DEBUG_)
void GroupRepository::dump (long group_id)
{
  if (group_id == -1) 
  {
    MEX_MSG(("Dumping Group Repository Content"));
    if (empty()) 
    {
      MEX_DEBUG(("TANGO group repository is empty\n")); 
      return;
    }
    for (GroupSetIterator it = begin(); it != end(); it++) 
    {
      MEX_DEBUG("Group: %s\n", (*it)->get_fully_qualified_name().c_str()); 
      {
        std::vector<std::string> dl = (*it)->get_device_list(false);
        for (size_t i = 0; i < dl.size(); i++)
          MEX_DEBUG("\t%s\n", dl[i].c_str());
      }
    }
  } 
  else 
  {
    Tango::Group* g = get(group_id, false);
    if (g) {
      MEX_DEBUG("* TANGO Group: %s\n", g->get_fully_qualified_name().c_str()); 
      std::vector<std::string> dl = g->get_device_list(false);
      for (size_t i = 0; i < dl.size(); i++) {
        MEX_DEBUG("\t- %s\n", dl[i].c_str()); 
      }
    }
  }
}
#endif




