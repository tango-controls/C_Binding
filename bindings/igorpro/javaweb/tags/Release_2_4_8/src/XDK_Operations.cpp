// ============================================================================
//
// = CONTEXT
//   XDK
//
// = FILENAME
//   XDK_Operations.h
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

//=============================================================================
// DEPENDENCIEs
//=============================================================================
#include "XDK_StandardHeaders.h"

#if defined(_XOP_ADDS_OPERATIONS_) 

#include "XDK_Operations.h"
     
//=============================================================================
// XDK_Operations member : exec                                                
//=============================================================================
int XOP::exec (long _xop_id)
{ 
  int err = kNoError;
  
  switch (_xop_id) {  
    case 0:
      //dummy case to make MSVC happy
      break;
    default:
      err = UNKNOWN_DEST_OR_BAD_CMD;
      break;
	}
	
  return err;
}

#endif // _XOP_ADDS_OPERATIONS_