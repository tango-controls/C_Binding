// ============================================================================
// Custom Error Messages : STR# 1100 
// ============================================================================
resource 'STR#' (1100) {
 {
  //-- kIGOR_INTERNAL_ERROR ------------
  "Igor Pro internal error (external function parameters struct is nil).", 
  //-- kRSRC_MISSING_ERROR -------------
  "required XOP resource could not be found (XOP file seems corrupted).", 
  //-- kMEMORY_ERROR -------------------
  "out of memory.",  
  //-- kXOP_INTERNAL_ERROR -------------
  "XOP internal error.",   
  //-- kNO_XOP_IMPLEMENTATION ----------
  "external operation has no implementation on this platform.",
  //-- kNO_XFUNC_IMPLEMENTATION --------
  "external function has no implementation on this platform.",
  //-- kXOP_CORBA_COM_ERROR ------------
  "CORBA communication failure (a server is down or unreachable).",   
  //-- kNO_SUCH_WINDOW -----------------
  "expected PSD image window identifier.",
  //-- kWAVES_NOT_SAME_LENGTH ----------
  "waves must have the same length.",
  //-- kWAVE_DOES_NOT_EXIST ------------
  "unknown wave.",
  //-- kWAVES_NOT_FP64 -----------------
  "this operation required NT_FP64 waves.",
  //-- End of resource
 }
};


