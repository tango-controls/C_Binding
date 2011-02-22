// ============================================================================
//
// = CONTEXT
//   XDK
//
// = FILENAME
//   XDK_StandardHeaders.h
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

#ifndef _XDK_STD_H_
#define _XDK_STD_H_

//-----------------------------------------------------------------------------
// TANGO Support (should be includes before any others)
//-----------------------------------------------------------------------------
#include "tango.h"

//-----------------------------------------------------------------------------
// WaveMetrics XOP Support
//-----------------------------------------------------------------------------
#include "XOPStandardHeaders.h"

//-----------------------------------------------------------------------------
// XDK configuration
//-----------------------------------------------------------------------------
#include "XDK_Configuration.h"

//-----------------------------------------------------------------------------
// Check XDK configuration
//-----------------------------------------------------------------------------
#ifndef XDK_TARGET_PLATFORM
  #error XDK configuration error. No XDK_TARGET_PLATFORM specified.
#endif

#ifdef _XOP_ADDS_ITEMS_TO_IGOR_MENUS_
  #ifndef _XOP_ADDS_MENUS_   
    #error XDK option _XOP_ADDS_MENUS_ should be defined.   
    #define _XOP_ADDS_MENUS_
  #endif
#endif
#ifdef _XOP_MENU_HAS_SUBMENUS_
  #ifndef _XOP_ADDS_MENUS_
    #error XDK option _XOP_ADDS_MENUS_ should be defined.
    #define _XOP_ADDS_MENUS_
  #endif
#endif
#ifdef _XOP_ADDS_FUNCTIONS_
  #ifdef _XOP_AUTO_QUIT_
    #error XDK options _XOP_ADDS_FUNCTIONS_ and _XOP_AUTO_QUIT_ are incompatible.
    #undef _XOP_AUTO_QUIT_
  #endif
#endif
#ifdef USE_REZ
  #if defined(_WINDOWS_) || (XDK_TARGET_PLATFORM == _XDK_WIN32_)
    #error No Rez support on Win32 platform.
    #undef USE_REZ
  #endif
#endif  

//-----------------------------------------------------------------------------
// XDK Inline
//-----------------------------------------------------------------------------
#if !defined(_XDK_DEBUG_)
# if !defined (__XDK_INLINE__)
#  define __XDK_INLINE__ 1
# endif
#endif

#if defined (__XDK_INLINE__)
# define XDK_INLINE inline
#else
# define XDK_INLINE
#endif

//-----------------------------------------------------------------------------
// XDK Debug
//-----------------------------------------------------------------------------
#if defined(_DEBUG) || defined (DEBUG)
# define _XDK_DEBUG_
#endif

//-----------------------------------------------------------------------------
// XDK constants
//-----------------------------------------------------------------------------
#include "XDK_Constants.h"

//-----------------------------------------------------------------------------
// XDK resources
//-----------------------------------------------------------------------------
#include "XDK_ResourceDefs.h"

//-----------------------------------------------------------------------------
// XDK utilities
//-----------------------------------------------------------------------------
#include "XDK_Utils.h"


#endif // _XDK_STD_H_