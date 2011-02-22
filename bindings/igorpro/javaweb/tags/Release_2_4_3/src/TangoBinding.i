// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for Igor Pro
//
// = FILENAME
//   TangoBinding.i
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

//=============================================================================
// TangoBinding::instance
//=============================================================================
XDK_INLINE TangoBinding* TangoBinding::instance (void)
{
  return TangoBinding::instance_;
}
