// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for LabVIEW
//
// = FILENAME
//   ErrorStack.cpp
//
// = AUTHOR
//   Nicolas Leclercq - SOLEIL
//
// ============================================================================

//=============================================================================
// DEPENDENCIEs
//=============================================================================
#include <cstdarg>
#include "StandardHeader.h"
#include "ErrorStack.h"

#if !defined (_LV_INLINE_)
  #include "ErrorStack.i"
#endif 

//=============================================================================
// #DEFINEs                                                 
//=============================================================================
#define kNUM_STR_PER_ERROR 4

//=============================================================================
// STATIC MEMBERS
//=============================================================================
ErrorStack* ErrorStack::instance_ = 0;  

//=============================================================================
// ErrorStack::init
//=============================================================================
int ErrorStack::init (void)
{ 
  if (ErrorStack::instance_ != 0)
    return 0;

  ErrorStack::instance_ = new ErrorStack;

  return (ErrorStack::instance_) ? 0 : -1;
}

//=============================================================================
// ErrorStack::cleanup
//=============================================================================
void ErrorStack::cleanup (void)
{

}

//=============================================================================
// ErrorStack::ErrorStack                                                
//=============================================================================
ErrorStack::ErrorStack ()
{

}

//=============================================================================
// ErrorStack::~ErrorStack                                                
//=============================================================================
ErrorStack::~ErrorStack (void)
{

}

//=============================================================================
// ErrorStack::no_error                                   
//=============================================================================
int ErrorStack::no_error (const char* o, LvStringArrayHdl& lv_error_)
{
  unsigned int nerr = kNUM_STR_PER_ERROR;

  long err = ::NumericArrayResize(LvMemoryAddress, 
                                  1L, 
                                  reinterpret_cast<UHandle*>(&lv_error_), 
                                  nerr);
  if (err != noErr) {
    return kError;
  }
  (*lv_error_)->length = nerr;
  ::memset((*lv_error_)->data, 0, (*lv_error_)->length * sizeof(long));
  size_t l;
  LvStringHdl sh;
  std::string str;
  for (unsigned int i = 0; i < nerr; i++)
  {
    switch (i)
    {
      case 3:
        str = o;
        break;
      default:
        str = "no error";
        break;
    }
    sh = 0;
	  l = str.size();
    err = ::NumericArrayResize(iB, 1L, reinterpret_cast<UHandle*>(&sh), l);
    if (err != noErr) {
      return kError;
    }
	  (*sh)->length = l;
    ::memcpy((*sh)->data, str.c_str(), l);
    (*lv_error_)->data[i] = sh;
  }
  return kNoError;
}

//=============================================================================
// ErrorStack::unknown                                   
//=============================================================================
int ErrorStack::unknown (const char* o, LvStringArrayHdl& lv_error_)
{
  unsigned int nerr = kNUM_STR_PER_ERROR;

  long err = ::NumericArrayResize(uL, 
                                  1L, 
                                  reinterpret_cast<UHandle*>(&lv_error_), 
                                  nerr);
  if (err != noErr) {
    return kError;
  }
  (*lv_error_)->length = nerr;
  ::memset((*lv_error_)->data, 0, (*lv_error_)->length * sizeof(long));
  size_t l;
  LvStringHdl sh;
  std::string str;
  for (unsigned int i = 0; i < nerr; i++)
  {
    switch (i)
    {
      case 0:
        str = "ERROR";
        break;
      case 1:
        str = "unknown error (may be due to a bug in the TANGO binding for Labview)";
        break;
      case 2:
        str = "unknown exception caught";
        break;
      case 3:
        str = o;
        break;
      default:
        str = "-";
        break;
    }
    sh = 0;
	  l = str.size();
    err = ::NumericArrayResize(iB, 1L, reinterpret_cast<UHandle*>(&sh), l);
    if (err != noErr) {
      return kError;
    }
	  (*sh)->length = l;
    ::memcpy((*sh)->data, str.c_str(), l);
    (*lv_error_)->data[i] = sh;
  }
  return kNoError;
}

//=============================================================================
// ErrorStack::devfailed                                    
//=============================================================================
int ErrorStack::devfailed (const Tango::DevFailed& _tango_error, LvStringArrayHdl& lv_error_)
{
  unsigned long nerr = _tango_error.errors.length();

  long err = ::NumericArrayResize(LvMemoryAddress, 
                                  1L, 
                                  reinterpret_cast<UHandle*>(&lv_error_), 
                                  nerr * kNUM_STR_PER_ERROR);
  if (err != noErr) {
    return kError;
  }
  (*lv_error_)->length = nerr * kNUM_STR_PER_ERROR;
  ::memset((*lv_error_)->data, 0, (*lv_error_)->length * sizeof(long));
  size_t l = 0;
  LvStringHdl sh = 0;
  std::string severity;
  for (unsigned long i = 0, j = 0; i < nerr; i++) 
  {
    //- severity ---------------------------------------------------------------
    switch(_tango_error.errors[i].severity)
    {
      case Tango::WARN:
        severity = "WARNING";
        break;
      case Tango::PANIC:
        severity = "PANIC";
        break;
      case Tango::ERR:
      default:
        severity = "ERROR";
        break;
    }
    sh = 0;
	  l = severity.size();
    err = ::NumericArrayResize(iB, 1L, reinterpret_cast<UHandle*>(&sh), l);
    if (err != noErr) {
      return kError;
    }
	  (*sh)->length = l;
    ::memcpy((*sh)->data, severity.c_str(), l);
    (*lv_error_)->data[j++] = sh;
    //- reason ------------------------------------------------------------------
    sh = 0;
    l = ::strlen(_tango_error.errors[i].reason);
    err = ::NumericArrayResize(iB, 1L, reinterpret_cast<UHandle*>(&sh), l);
    if (err != noErr) {
      return kError;
    }
	  (*sh)->length = l;
    ::memcpy((*sh)->data, _tango_error.errors[i].reason, l);
    (*lv_error_)->data[j++] = sh;
    //- desc --------------------------------------------------------------------
    sh = 0;
    l = ::strlen(_tango_error.errors[i].desc);
    err = ::NumericArrayResize(iB, 1L, reinterpret_cast<UHandle*>(&sh), l);
    if (err != noErr) {
      return kError;
    }
	  (*sh)->length = l;
    ::memcpy((*sh)->data, _tango_error.errors[i].desc, l);
    (*lv_error_)->data[j++] = sh;
    //- origin --------------------------------------------------------------------
    sh = 0;
    l = ::strlen(_tango_error.errors[i].origin);
    err = ::NumericArrayResize(iB, 1L, reinterpret_cast<UHandle*>(&sh), l);
    if (err != noErr) {
      return kError;
    }
	  (*sh)->length = l;
    ::memcpy((*sh)->data, _tango_error.errors[i].origin, l);
    (*lv_error_)->data[j++] = sh;
  }
  return kNoError;
}
