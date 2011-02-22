// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for Matlab
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
// DEBUG 
//-----------------------------------------------------------------------------
#if defined (_DEBUG)
# define _MEX_DEBUG_
#endif

//-----------------------------------------------------------------------------
// INLINE
//-----------------------------------------------------------------------------
#if !defined(_MEX_DEBUG_)
# define _MEX_INLINE_
# define MEX_INLINE inline
#else
# define MEX_INLINE
#endif

//-----------------------------------------------------------------------------
// DEPENDENCIES
//-----------------------------------------------------------------------------
#include <math.h>

//-----------------------------------------------------------------------------
// MATLAB MEX-FILE SUPPORT
//-----------------------------------------------------------------------------
#include <mex.h>

//-----------------------------------------------------------------------------
// TANGO SUPPORT
//-----------------------------------------------------------------------------
#include <tango.h>

//-----------------------------------------------------------------------------
// MISC MEX-FILE UTILITIES
//-----------------------------------------------------------------------------
#include "MexUtils.h"
#include "MexArgs.h"
#include "MexFile.h"

//-----------------------------------------------------------------------------
// CONSTS
//-----------------------------------------------------------------------------
extern const int kNoError;
extern const int kError;

//-----------------------------------------------------------------------------
// DEFINEs
//-----------------------------------------------------------------------------
#define MAX_DEV_NAME 255

//-----------------------------------------------------------------------------
// MACROS
//-----------------------------------------------------------------------------
#define SET_DEFAULT_PRHS(X) \
  if (MEX_ARGS->nlhs())\
    MEX_ARGS->set_output_array(k1ST_ARG, MEX_ARGS->default_prhs(X));

#define SET_DEFAULT_PRHS_THEN_RETURN(X) \
  { \
    if (MEX_ARGS->nlhs())\
      MEX_ARGS->set_output_array(k1ST_ARG, MEX_ARGS->default_prhs()); \
    return X; \
  }

#define _NAN_ ::sqrt((double)-1.)

#endif // __STD_HEADER_H_
