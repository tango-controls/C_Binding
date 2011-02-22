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
XDK_INLINE MonitoredObjectManager* MonitoredObjectManager::instance (void)
{
  return MonitoredObjectManager::instance_;
}
