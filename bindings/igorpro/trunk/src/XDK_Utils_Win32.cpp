// ============================================================================
//
// = CONTEXT
//   XDK
//
// = FILENAME
//   XDK_Utils_Win32.cpp
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

//==============================================================================
// HEADERS
//==============================================================================
#include "XDK_StandardHeaders.h"
#include "XDK_Utils.h"
#include "XDK_Utils_Win32.h"

//==============================================================================
// #DEFINEs
//==============================================================================
#define kMIN_TIMER 0
#define kMAX_TIMER 9
static LARGE_INTEGER lg_timer_start_time[kMAX_TIMER + 1]; 

//------------------------------------------------------------------------------
// start_timer
//------------------------------------------------------------------------------
void start_timer (int _timer_id)
{
  if (_timer_id < kMIN_TIMER  || _timer_id  > kMAX_TIMER) 
    return;

  ::QueryPerformanceCounter(&lg_timer_start_time[_timer_id]);
}

//------------------------------------------------------------------------------
// elapsed_usec
//------------------------------------------------------------------------------
double elapsed_usec (int _timer_id)
{
  static const double kTimerConstant = 4.294967296E9;

  LARGE_INTEGER stop; 
  ::QueryPerformanceCounter(&stop);

  LARGE_INTEGER freq; 
  ::QueryPerformanceFrequency(&freq);

  if (_timer_id < kMIN_TIMER  || _timer_id  > kMAX_TIMER) 
    return 0.;

	LARGE_INTEGER start = lg_timer_start_time[_timer_id]; 

	double dfreq,dstart,dstop;
	dfreq  = (freq.u.HighPart * kTimerConstant + freq.u.LowPart);
	dstart = (start.u.HighPart * kTimerConstant + start.u.LowPart);
	dstop  = (stop.u.HighPart * kTimerConstant + stop.u.LowPart); 
	
  return static_cast<double>((dstop - dstart) / dfreq);
}

//------------------------------------------------------------------------------
// elapsed_sec
//------------------------------------------------------------------------------
double elapsed_sec (int _timer_id)
{
  return elapsed_usec(_timer_id) * 1.E-6;
}

