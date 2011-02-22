// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for Igor Pro
//
// = FILENAME
//   DataCodec.i
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

//=============================================================================
// DataCodec::instance
//=============================================================================
XDK_INLINE DataCodec* DataCodec::instance (void)
{
  return DataCodec::instance_;
}
