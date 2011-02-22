// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for LabVIEW
//
// = FILENAME
//   ErrorStack.i
//
// = AUTHOR
//   Nicolas Leclercq - SOLEIL
//
// ============================================================================

//=============================================================================
// ErrorStack::instance
//=============================================================================
LV_INLINE ErrorStack* ErrorStack::instance (void)
{
  return ErrorStack::instance_;
}

