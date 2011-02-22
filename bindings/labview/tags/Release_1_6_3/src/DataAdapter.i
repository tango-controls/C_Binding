// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for LabVIEW
//
// = FILENAME
//   DataAdapter.i
//
// = AUTHOR
//   Nicolas Leclercq - SOLEIL
//
// ============================================================================

//=============================================================================
// DataAdapter::instance
//=============================================================================
LV_INLINE DataAdapter* DataAdapter::instance (void)
{
  return DataAdapter::instance_;
}
