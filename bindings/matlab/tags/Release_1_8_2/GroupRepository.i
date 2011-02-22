// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for Matlab
//
// = FILENAME
//   GroupRepository.i
//
// = AUTHOR
//   Nicolas Leclercq - SOLEIL
//
// ============================================================================

//=============================================================================
// GroupRepository::instance
//=============================================================================
MEX_INLINE GroupRepository* GroupRepository::instance (void)
{
  return GroupRepository::instance_;
}

//=============================================================================
// GroupRepository::get
//=============================================================================
MEX_INLINE Tango::Group* GroupRepository::get (const std::string& name, bool set_error)
{
  GroupSetIterator it = begin();
  GroupSetIterator _end = end();
  Tango::Group* g = 0;
  for (; it != _end && g == 0; it++) {
    g = (*it)->get_group(name);
  }
  if (g == 0) {
    if (set_error) {
      MEX_UTILS->set_error("invalid group name",
			                     "no group by that name",
			                     "GroupRepository::get"); 
    }
    return 0;
  }
  return g;
}

//=============================================================================
// GroupRepository::get
//=============================================================================
MEX_INLINE Tango::Group* GroupRepository::get (long group_id, bool set_error)
{
  Tango::Group* g = (Tango::Group*)group_id;
  if (find(g) == end()) {
    if (set_error) {
      MEX_UTILS->set_error("invalid group id",
			                     "no such group",
			                     "GroupRepository::get"); 
    }
    return 0;
  }
  return g;
}

