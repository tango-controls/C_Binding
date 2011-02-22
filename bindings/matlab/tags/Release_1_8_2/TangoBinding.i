// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for Matlab
//
// = FILENAME
//   TangoBinding.i
//
// = AUTHOR
//   Nicolas Leclercq - SOLEIL
//
// ============================================================================

//=============================================================================
// TangoBinding::instance
//=============================================================================
MEX_INLINE TangoBinding* 
TangoBinding::instance (void)
{
  return TangoBinding::instance_;
}
