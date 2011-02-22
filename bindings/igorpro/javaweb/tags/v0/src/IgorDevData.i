// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for Igor Pro
//
// = FILENAME
//   IgorDevData.i
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

//=============================================================================
// IgorDevData::instance
//=============================================================================
XDK_INLINE IgorDevData* IgorDevData::instance (void)
{
  return IgorDevData::instance_;
}
