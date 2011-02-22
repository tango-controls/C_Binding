// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for LabVIEW
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
LV_INLINE GroupRepository* GroupRepository::instance (void)
{
  return GroupRepository::instance_;
}


//=============================================================================
// GroupRepository::get
//=============================================================================
LV_INLINE GroupDescriptor * GroupRepository::get (long group_id)
{
  //- critical section
  omni_mutex_lock guard(this->lock_);

  //- search for <group_id> in the repository
  GroupMapIterator it = this->group_map_.find(group_id);
  if (it != this->group_map_.end() && it->second) 
    return it->second;

  Tango::Except::throw_exception(_CPTC("invalid argument"),
			                           _CPTC("invalid group id specified [no such group]"),
			                           _CPTC("GroupRepository::get"));

	//- make gcc happy...
	return 0;
}

