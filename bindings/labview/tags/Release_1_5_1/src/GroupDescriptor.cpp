// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for LabVIEW
//
// = FILENAME
//   GroupDescriptor.cpp
//
// = AUTHOR
//   Nicolas Leclercq - SOLEIL
//
// ============================================================================

//=============================================================================
// DEPENDENCIES
//=============================================================================
#include "StandardHeader.h"
#include "GroupDescriptor.h"

#if !defined (_LV_INLINE_)
# include "GroupDescriptor.i"
#endif 
 
//=============================================================================
// GroupDescriptor::GroupDescriptor
//=============================================================================
GroupDescriptor::GroupDescriptor (Tango::Group* _g)
    : g(_g), dl(0)
{
  if (g) 
    dl = g->get_device_list();
}

//=============================================================================
// GroupDescriptor::~GroupDescriptor
//=============================================================================
GroupDescriptor::~GroupDescriptor ()
{
  dl.clear();
  if (g) delete g;
}


