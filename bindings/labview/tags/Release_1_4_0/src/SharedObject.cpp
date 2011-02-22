// ============================================================================
//
// = CONTEXT
//    Tango Generic Client for Labview
//
// = FILENAME
//    SharedObject.cpp
//
// = AUTHOR
//    Nicolas Leclercq
//
// ============================================================================

// ============================================================================
// DEPENDENCIES
// ============================================================================
#include "SharedObject.h"

#if !defined (_LV_INLINE_)
# include "SharedObject.i"
#endif

// ============================================================================
// SharedObject::SharedObject
// ============================================================================
SharedObject::SharedObject(void)
  : reference_count_ (1)
{

}

// ============================================================================
// SharedObject::~SharedObject
// ============================================================================
SharedObject::~SharedObject (void)
{

}

// ============================================================================
// SharedObject::duplicate
// ============================================================================
SharedObject * SharedObject::duplicate (void)
{
  omnimutex_lock guard(this->lock_);
  
  this->reference_count_++;

  return this;
}

// ============================================================================
// SharedObject::release
// ============================================================================
void SharedObject::release (void)
{
  if (this->release_i() == 0)
    delete this;
}

// ============================================================================
// SharedObject::release_i
// ============================================================================
SharedObject * SharedObject::release_i (void)
{
  omnimutex_lock guard(this->lock_);

  this->reference_count_--;

  return (this->reference_count_ == 0) ? 0 : this;
}
