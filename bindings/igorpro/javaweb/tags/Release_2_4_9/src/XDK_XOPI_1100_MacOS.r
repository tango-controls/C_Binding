// ============================================================================
// 				              General XOP properties : XOPI 1100 	
// ----------------------------------------------------------------------------
//                                 [DO NOT EDIT]
// ============================================================================
resource 'vers' (1) {						// XOP version info
	0x01, 0x00, final, 0x00, 0,		// version bytes and country integer
	"x.y",
	"x.y, © 2003-2009 - NL - Synchrotron SOLEIL"
};

resource 'vers' (2) {						// Igor version info */
	0x00, 0x00, release, 0x00, 0,	// version bytes and country integer
	"x.y",
	"(for Igor 6.00 or later)"
};

resource 'XOPI' (1100) {
  XOP_VERSION,          // Version number of host XOP system.
  DEV_SYS_CODE,         // Code for development system used to make XOP.
	0,										// Obsolete - set to zero.
	0,										// Obsolete - set to zero.
	XOP_TOOLKIT_VERSION,  // XOP Toolkit version.
};
