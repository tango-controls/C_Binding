// ============================================================================
//
// = CONTEXT
//   XDK
//
// = FILENAME
//   XDK_Xop.cpp
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

///============================================================================
// DEPENDENCIEs
//=============================================================================
#include "XDK_StandardHeaders.h"
#include "XDK_Xop.h"
#if defined(_XOP_ADDS_OPERATIONS_)
# include "XDK_Operations.h"
#endif
#if defined(_XOP_ADDS_FUNCTIONS_)
# include "XDK_Functions.h"
#endif

//=============================================================================
// GLOBAL CONSTs
//=============================================================================
const int kNoError =  0;
const int kError   = -1;

//=============================================================================
// STATIC MEMBERS
//=============================================================================
XDK_Xop* XDK_Xop::instance_ = 0;

//-----------------------------------------------------------------------------
// XDK_Xop member : startup
//-----------------------------------------------------------------------------
int XDK_Xop::startup (void) 
{
  if (::igorVersion < _MIN_IGOR_VERSION_)
    return IGOR_OBSOLETE;
  
  if (XDK_Utils::init(kXOP_NAME) == kError)
    return XOP_CANT_INIT;

  int result = this->init();

  #if defined (_XDK_DEBUG_)
    if (result == kNoError) {
      XDK_UTILS->notify("XOP MSG : XOP initialized.");  
    }
  #endif

  return (result == kError) ? XOP_CANT_INIT : 0;
}

//-----------------------------------------------------------------------------
// XDK_Xop member : XDK_Xop (ctor)                                           
//-----------------------------------------------------------------------------
XDK_Xop::XDK_Xop (IORecHandle _ioRecHandle) 
{
  ::XOPInit(_ioRecHandle); 
  
  ::SetXOPEntry(XDK_Xop::process_igor_message);
  
  #if defined(_XOP_NEEDS_IDLE_)                
    ::SetXOPType(XOP_RESIDENT | XOP_IDLES);
  #elif defined (_XOP_AUTO_QUIT_)
    ::SetXOPType(XOP_TRANSIENT);
  #endif
  
  XDK_Xop::instance_ = this;
}

//-----------------------------------------------------------------------------
// XDK_Xop member : quit
//-----------------------------------------------------------------------------
void XDK_Xop::quit (void) 
{
  this->cleanup();

  #if defined (_XDK_DEBUG_)
    XDK_UTILS->notify("XOP MSG: XOP closed and discarded.");  
  #endif

  XDK_Utils::cleanup();
  
  XDK_Xop::instance_ = 0;

  delete this;
}

//-----------------------------------------------------------------------------
// XDK_Xop member : ~XDK_Xop (dtor)
//-----------------------------------------------------------------------------
XDK_Xop::~XDK_Xop (void) 
{
  
}

//-----------------------------------------------------------------------------
// XDK_Xop member : process_igor_message                        
//-----------------------------------------------------------------------------   
void XDK_Xop::process_igor_message (void)
{	  
  try 
  {
    int result = kNoError;

    XDK_Xop* xop = XDK_Xop::instance_;
    if (xop == 0) {
      ::SetXOPResult(kXOP_INTERNAL_ERROR);
      return;
    }

  	switch (::GetXOPMessage()) 
  	{
      //-- EXTERNAL OPERARTIONs --------------------------------------------
      #if defined(_XOP_ADDS_OPERATIONS_)
        case CMD: {
          long op_id = ::GetXOPItem(1);                     
          result = XOP::exec(op_id);
        }
  		  break;
      #endif 
      //--  EXTERNAL FUNCTIONs ---------------------------------------------
      #if defined(_XOP_ADDS_FUNCTIONS_)
        case FUNCADDRS: { 
  		      long func_id = ::GetXOPItem(0);
            result = XFUNC::call_mode(func_id);
  	      }
  			  break; 
  		  case FUNCTION: {
  		      long func_id = ::GetXOPItem(0);
  		      void *func_params = (void *)::GetXOPItem(1);
  		      result = XFUNC::exec(func_id, func_params);
  		    }
  			  break;
      #endif 
      //-- MENUITEM & MENUENABLE -------------------------------------------
      #if defined(_XOP_ADDS_MENUS_) || defined(_XOP_ADDS_ITEMS_TO_IGOR_MENUS_)              
  		  case MENUITEM:
          //--to do : exec menu command
  			  break;
  		  case MENUENABLE:	
          //--to do : update menu
  			  break;
      #endif 
      //-- OBJECT IN USE ---------------------------------------------------
      case OBJINUSE:
        break;     
      //-- IDLE ------------------------------------------------------------
      #if defined(_XOP_NEEDS_IDLE_)
        case IDLE:   
          xop->idle();
          break;
      #endif 
      //-- NEW EXPERIMENT -------------------------------------------------- 
      case NEW:
        xop->new_experiment();
        break;  
      //-- LOAD EXPERIMENT -------------------------------------------------        
      case LOAD: 
        xop->load_experiment();
        break;  
      //-- CLEAR MODIFIED --------------------------------------------------        
      case CLEAR_MODIFIED: 
        xop->clear_modified();
        break;  
      //-- CLEANUP ---------------------------------------------------------
      case CLEANUP:  
        xop->quit(); 
        break; 
      //-- IGNORED MESSAGES ------------------------------------------------       
   	  default:  
        //XOP_MSG("Unhandled message received (ignored)");
        break; 	
  	}  							
  	::SetXOPResult(result);
	}
	catch (...) {
    XOP_ERROR(("caught unknown exception in XDK_Xop::process_igor_message"));
	}
}

//-----------------------------------------------------------------------------
// XDK_Xop member : new_experiment                        
//-----------------------------------------------------------------------------   
int XDK_Xop::new_experiment (void)
{	  
  return kNoError;
}

//-----------------------------------------------------------------------------
// XDK_Xop member : load_experiment                       
//-----------------------------------------------------------------------------   
int XDK_Xop::load_experiment (void)
{	  
  return kNoError;
}

//-----------------------------------------------------------------------------
// XDK_Xop member : clear_modified                     
//-----------------------------------------------------------------------------   
int XDK_Xop::clear_modified (void)
{	  
  return kNoError;
}

