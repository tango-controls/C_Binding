//========================================================================================
// EXTERNAL OPERATIONS IDs
// ---------------------------------------------------------------------------------------
// Operation id is the #, STARTING FROM ZERO, of the function in XOP's XOPC 1100 rsrc.
//========================================================================================
enum {
  kXDK_XOP0 = 0,
  kXDK_XOP1,
  kXDK_XOP2
};

//========================================================================================
// EXTERNAL FUNCTIONS IDs
// ---------------------------------------------------------------------------------------
// Function id is the #, STARTING FROM ZERO, of the function in XOP's XOPF 1100 rsrc.
//========================================================================================
enum {
  //-- TANGO API XFUNCs --
  kTANGO_OPEN_DEV = 0,
  kTANGO_CLOSE_DEV,
  kTANGO_CMD_INOUT,
  kTANGO_READ_ATTR,
  kTANGO_READ_ATTRS,
  kTANGO_WRITE_ATTR,
  kTANGO_WRITE_ATTRS,
  kTANGO_GET_DEV_STATUS,
  kTANGO_PING_DEV,
  kTANGO_SET_DEV_TMO,
  kTANGO_GET_DEV_TMO,
  kTANGO_CMD_LIST,
  kTANGO_ATTR_LIST,
  kTANGO_BLACK_BOX,
  kTANGO_ERROR_STACK
};
        
//========================================================================================
// CUSTOM ERRORS (STR# 1100 RSRC)
//========================================================================================
enum {
  kIGOR_INTERNAL_ERROR = 1 + FIRST_XOP_ERR,
  kRSRC_MISSING_ERROR,
  kMEMORY_ERROR,
  kXOP_INTERNAL_ERROR,
  kNO_XOP_IMPLEMENTATION,
  kNO_XFUNC_IMPLEMENTATION,
  kWAVES_NOT_SAME_LENGTH,
  kWAVE_DOES_NOT_EXIST,
  kWAVES_NOT_SAME_TYPE
};
