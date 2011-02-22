// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for Igor Pro
//
// = FILENAME
//   MonitoredObject.i
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

// ============================================================================
// MonitoredAttribute::num_ms_to_polling_period_expiration
// ============================================================================
XDK_INLINE unsigned long MonitoredObject::num_ms_to_polling_period_expiration (void)
{
  //- get current time
  yat::Timestamp now;
  _GET_TIME(now);

  //- compute diff between min_pp and elapsed msec since last import
  long dt = this->min_pp_ - _ELAPSED_MSEC(this->import_ts_, now);

  return (dt <= 0) ? 0 : static_cast<unsigned long>(dt);
}

//=============================================================================
// MonitoredDevice::has_monitored_objects
//=============================================================================
XDK_INLINE bool MonitoredDevice::has_monitored_objects (void) const
{
  return ! this->mattr_map_.empty();
}


