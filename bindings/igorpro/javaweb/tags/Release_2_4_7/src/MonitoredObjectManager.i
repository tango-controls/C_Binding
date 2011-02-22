// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for Igor Pro
//
// = FILENAME
//   MonitoredObjectManager.i
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

//=============================================================================
// MonitoredObjectManager::instance
//=============================================================================
XDK_INLINE MonitoredObjectManager* MonitoredObjectManager::instance ()
{
  return MonitoredObjectManager::instance_;
}
