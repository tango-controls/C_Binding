// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for LabVIEW
//
// = FILENAME
//   Endianness.h - CODE STOLEN FROM THE FABULOUS ACE FRAMEWORK
//
// = ORIGINAL AUTHORS
//   Aniruddha Gokhale <gokhale@cs.wustl.edu>
//   Carlos O'Ryan<coryan@cs.wustl.edu>
//   Jeff Parsons <parsons@cs.wustl.edu>
//   Istvan Buki	<istvan.buki@euronet.be>
//
// = THIEF
//   Nicolas Leclercq - SOLEIL
//
// ============================================================================

//=============================================================================
/*
 * The current implementation assumes that the host has	1-byte,
 * 2-byte and 4-byte integral types, and that it has single
 * precision and double	precision IEEE floats.
 * Those assumptions are pretty	good these days, with Crays being
 * the only known exception.
 */
//=============================================================================


#ifndef	_ENDIANNESS_H_
#define	_ENDIANNESS_H_

// ============================================================================
class Endianness
{
public:
  // = Constants defined by the	CDR protocol.
  // By	defining as many of these constants as possible	as enums we
  // ensure they get inlined and avoid pointless static	memory
  // allocations.

  enum
  {
    // Note that some of these get reused as part of the standard
    // binary format: unsigned is the same size	as its signed cousin,
    // float is	LONG_SIZE, and double is LONGLONG_SIZE.
    OCTET_SIZE = 1,
    SHORT_SIZE = 2,
    LONG_SIZE =	4,
    LONGLONG_SIZE = 8,
    LONGDOUBLE_SIZE = 16,
    OCTET_ALIGN	= 1,
    SHORT_ALIGN	= 2,
    LONG_ALIGN = 4,
    LONGLONG_ALIGN = 8,
    LONGDOUBLE_ALIGN = 8,
    MAX_ALIGNMENT = 8,
  };

  /**
   * Do	byte swapping for each basic IDL type size.  There exist only
   * routines to put byte, halfword (2 bytes), word (4 bytes),
   * doubleword	(8 bytes) and quadword (16 byte); because those	are
   * the IDL basic type	sizes.
   */
  static void swap_2  (const char *orig, char *target);
  static void swap_4  (const char *orig, char *target);
  static void swap_8  (const char *orig, char *target);
  static void swap_16 (const char *orig, char *target);

  static void swap_2_array  (const char *orig, char *target, size_t length);
  static void swap_4_array  (const char *orig, char *target, size_t length);
  static void swap_8_array  (const char *orig, char *target, size_t length);
  static void swap_16_array (const char	*orig, char *target, size_t length);
};

//=============================================================================
// INLINED CODE
//=============================================================================
#if defined (_LV_INLINE_)
  #include "Endianness.i"
#endif 

#endif //- _ENDIANNESS_H_
