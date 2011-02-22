// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for LabVIEW
//
// = FILENAME
//   TargetPlatform.h
//
// = AUTHOR
//   Nicolas Leclercq - SOLEIL
//
// ============================================================================

#ifndef _TARGET_PLATFORM_H_
#define _TARGET_PLATFORM_H_

//-----------------------------------------------------------------------------
// PLATFORM CONFIGURATION
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// WINDOWS on INTEL PENTIUM OR AMD ATHLON-XP [*** TESTED ***]
//-----------------------------------------------------------------------------
// supported compiler: Microsoft Vsual C++ [>= 6.0]
// supported processors: 32 bits Intel Pentium and AMD Athlon-XP 
// endianness : little 
//-----------------------------------------------------------------------------
#if defined(WIN32)
# define WIN32_LEAN_AND_MEAN
# if (_M_IX86 > 400)
#  define _TBFL_HAS_PENTIUM_ 1
#  define _TBFL_LITTLE_ENDIAN_PLATFORM_ 1
# else
#  error "no support for this processor"
# endif
# if !defined(_MSC_VER)
#  error "no support for this WIN32 compiler - MSVC++ compiler required"
# elif (_MSC_VER < 1200)
#  error "Microsoft Visual C++ >= 6.0 required"
# else
#  define _TBFL_HAS_STATIC_OBJ_MANAGER_ 0
# endif
//-----------------------------------------------------------------------------
// LINUX on INTEL PENTIUM OR AMD ATHLON-XP [*** TESTED ***]
//-----------------------------------------------------------------------------
// supported compiler: gcc [>= 3.2.0]
// supported processors: 32 bits Intel Pentium and AMD Athlon-XP 
// endianness : little 
//-----------------------------------------------------------------------------
#elif defined(_linux_) || defined (__linux__)
# if defined(i386) || defined(__i386__)
#  define _TBFL_HAS_PENTIUM_ 1
#  define _TBFL_LITTLE_ENDIAN_PLATFORM_ 1
# elif defined(i686) || defined(__i686__)
#  define _TBFL_HAS_PENTIUM_ 1
#  define _TBFL_LITTLE_ENDIAN_PLATFORM_ 1
# else
#  error "no support for this processor"
# endif
# if !defined(__GNUG__)
#  error "no support for this compiler - GCC compiler required"
# else
#  define _TBFL_HAS_STATIC_OBJ_MANAGER_ 0
# endif 
//-----------------------------------------------------------------------------
// MACOSX on INTEL PENTIUM [*** TESTED ***]
//-----------------------------------------------------------------------------
// supported compiler: gcc [>= 4.2.1]
// supported processors: 32 bits & 64 bits Intel Pentium
// endianness : little 
//-----------------------------------------------------------------------------
#elif defined(__APPLE__) && defined (__MACH__)
# if defined(i386) || defined(__i386__)
#  define _TBFL_HAS_PENTIUM_ 1
#  define _TBFL_LITTLE_ENDIAN_PLATFORM_ 1
# elif defined(i686) || defined(__i686__)
#  define _TBFL_HAS_PENTIUM_ 1
#  define _TBFL_LITTLE_ENDIAN_PLATFORM_ 1
# elif defined(x86_64) || defined(__x86_64__)
#  define _TBFL_HAS_PENTIUM_ 1
#  define _TBFL_LITTLE_ENDIAN_PLATFORM_ 1
# else
#  error "no support for this processor"
# endif
# if !defined(__GNUC__)
#  error "no support for this compiler - GCC compiler required"
# else
#  define _TBFL_HAS_STATIC_OBJ_MANAGER_ 0
# endif 
//-----------------------------------------------------------------------------
// SunOS on SPARC or PENTIUM [*** NOT TESTED ***]
//-----------------------------------------------------------------------------
// supported compiler: sun pro cc [>= ?]
// supported processors: 32 bits SPARC or PENTIUM
// endianness : big on SPARC, little on Pentium 
//-----------------------------------------------------------------------------
// NOTE FOR SOLARIS USERS: add -Dsun to your compiler flags
//-----------------------------------------------------------------------------
#elif defined(__sun) || defined (sun) 
# if defined(__sparcv9)
#  error "no support for 64 bits mode"
# elif (__sparc) || defined (__sparc__)
#  define _TBFL_HAS_PENTIUM_ 0
#  define _TBFL_LITTLE_ENDIAN_PLATFORM_ 0
# elif defined(__i386) || defined(i386) || defined(__i386__)
#  define _TBFL_HAS_PENTIUM_ 1
#  define _TBFL_LITTLE_ENDIAN_PLATFORM_ 1
# else
#  error "no support for this processor"
# endif
# if !defined(__SUNPRO_CC) && !defined(__GNUG__)
#  error "no support for this compiler - Sun-CC or GNU-GCC compiler required"
# else
#  define _TBFL_HAS_STATIC_OBJ_MANAGER_ 1
# endif
//-----------------------------------------------------------------------------
// UNKNOWN/UNSUPPORTED PLATEFORM
//-----------------------------------------------------------------------------
#else
#  error "no support for this platform"
#endif


#endif // _TARGET_PLATFORM_H_
