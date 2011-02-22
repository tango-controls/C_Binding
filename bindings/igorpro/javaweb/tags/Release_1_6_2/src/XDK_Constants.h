// ============================================================================
//
// = CONTEXT
//   XDK
//
// = FILENAME
//   XDK_Constants.h
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================
#ifndef _XDK_CONSTANTS_H_
#define _XDK_CONSTANTS_H_

//=============================================================================
// DEFINEs 
//=============================================================================
#if XDK_TARGET_PLATFORM == _XDK_MACOS_
# ifndef topLeft
#   define topLeft(r)	(((Point *) &(r))[0])
# endif
# ifndef botRight
#   define botRight(r)	(((Point *) &(r))[1])
# endif
#endif

//=============================================================================
// CONSTS 
//=============================================================================
extern const int kNoError;
extern const int kError;
//-----------------------------------------------------------------------------
extern const char kNameSep;

#endif //_XOP_CONSTANTS_H_