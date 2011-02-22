// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for LabVIEW
//
// = FILENAME
//   StandardHeader.h
//
// = AUTHOR
//   Nicolas Leclercq - SOLEIL
//
// ============================================================================

#ifndef _STD_HEADER_H_
#define _STD_HEADER_H_

//-----------------------------------------------------------------------------
// DEPENDENCIES
//-----------------------------------------------------------------------------
#include "TargetPlatform.h"

//-----------------------------------------------------------------------------
// DEBUG 
//-----------------------------------------------------------------------------
#if !defined (_DEBUG)
# define _LV_INLINE_
#endif

//-----------------------------------------------------------------------------
// INLINE
//-----------------------------------------------------------------------------
#if defined(_LV_INLINE_)
# define LV_INLINE inline
#else
# define LV_INLINE
#endif

//-----------------------------------------------------------------------------
// LABVIEW SUPPORT
//-----------------------------------------------------------------------------
#include <extcode.h>

//-----------------------------------------------------------------------------
// TANGO SUPPORT
//-----------------------------------------------------------------------------
#include <tango.h>

//-----------------------------------------------------------------------------
// OTHERS
//-----------------------------------------------------------------------------
#include "LvTypes.h"

//-----------------------------------------------------------------------------
// CONSTS
//-----------------------------------------------------------------------------
extern const int kNoError;
extern const int kError;

//-----------------------------------------------------------------------------
// PSEUDO CONSTS
//-----------------------------------------------------------------------------
#define kTIME_OFFSET 2082844800.

//-----------------------------------------------------------------------------
// MACROS
//-----------------------------------------------------------------------------
#define _CPTC(X) static_cast<const char*>(X)

#endif // __STD_HEADER_H_
