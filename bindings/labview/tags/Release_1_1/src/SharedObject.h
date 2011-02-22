// ============================================================================
//
// = CONTEXT
//    Tango Generic Client for Labview
//
// = FILENAME
//    SharedObject.h
//
// = AUTHOR
//    Nicolas Leclercq
//
// ============================================================================

#ifndef _SHARED_OBJECT_H_
#define _SHARED_OBJECT_H_

// ============================================================================
// DEPENDENCIES
// ============================================================================
#include "StandardHeader.h"

// ============================================================================
// Class: SharedObject (a reference counted object abstraction) 
// ============================================================================
class SharedObject
{
public:

  SharedObject (void);
  // Constructor.
  
  virtual ~SharedObject (void);
  // Destructor.

  SharedObject * duplicate (void);
  // Return a "shallow" copy. Increment the reference count by 1
  // to avoid deep copies.
  
  void release (void);
  // Decrease the shared reference count by 1.  If the reference count
  // equals 0, then delete <this> and return 0. Behavior is undefined 
  // if reference count < 0. 
 
  int reference_count (void) const;
  // Returns the current reference count.
  
  void lock(void);
  // Gets exclusive access to the data.

  void unlock(void);
  // Release exclusive access to the data.
  
protected:
  // a mutex to protect the data against race conditions
  omnimutex lock_; 

private:
  // internal release implementation
  SharedObject * release_i (void);

  //- reference count for (used to avoid deep copies)
  int reference_count_;

  // = Disallow these operations.
  //--------------------------------------------
  SharedObject& operator= (const SharedObject&);
  SharedObject (const SharedObject&);
};

//=============================================================================
// INLINED CODE
//=============================================================================
#if defined (_LV_INLINE_)
# include "SharedObject.i"
#endif

#endif // _SHARED_OBJECT_H_
