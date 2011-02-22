//- ============================================================================
//
//- = CONTEXT
//-   Tango Generic Client for Matlab
//
//- = FILENAME
//-   DataAdapter.cpp
//
//- = AUTHOR
//-   Nicolas Leclercq - SOLEIL
//
//- ============================================================================

//=============================================================================
//- DEPENDENCIES
//=============================================================================
#ifndef WIN32
#  include <sys/time.h> 
#endif 
#include "StandardHeader.h"
#include "DevRepository.h"
#include "DataAdapter.h"

#if !defined (_MEX_INLINE_)
#include "DataAdapter.i"
#endif 

//=============================================================================
//- WORKAROUND FOR OLDER MATLAB VERSION
//=============================================================================
#if defined (_HAS_NO_MWSIZE_TYPE_)
typedef int mwSize; 
#endif 

//=============================================================================
//- STATIC MEMBERS
//=============================================================================
DataAdapter* DataAdapter::instance_ = 0;

//=============================================================================
//- DataAdapter::init
//=============================================================================
int DataAdapter::init (void)
{
  if (DataAdapter::instance_ != 0)
    return 0;
  
  DataAdapter::instance_ = new (std::nothrow) DataAdapter;
  
  return (DataAdapter::instance_) ? 0 : -1; 
}

//=============================================================================
//- DataAdapter::cleanup
//=============================================================================
void DataAdapter::cleanup (void)
{
  if (DataAdapter::instance_) 
  {
    delete DataAdapter::instance_;
    DataAdapter::instance_ = 0;
  }
}

//=============================================================================
//- DataAdapter::DataAdapter 
//=============================================================================
DataAdapter::DataAdapter (void)
{
  //- no-op ctor
}

//=============================================================================
//- DataAdapter::~DataAdapter 
//=============================================================================
DataAdapter::~DataAdapter (void)
{
  //- no-op dtor
}

//=============================================================================
//- DataAdapter::encode_argin
//=============================================================================
int DataAdapter::encode_argin (DeviceDesc * _ddesc, 
                               int _cmd_id, 
                               mxArray * _argin,
                               Tango::DeviceData & dd_in_)
{
  if (_ddesc == 0) 
  {
    MEX_UTILS->set_error("internal error",
                         "unexpected NULL argument",
                         "DataAdapter::encode_argin");
    return kError;
  }
  
  try 
  {
    int argin_type = (_ddesc->cmd_list())[_cmd_id].in_type;
    switch (argin_type) 
    {
        //-- DEV_VOID ------------------------------------
      case Tango::DEV_VOID: 
      {
        return kNoError; 
      } break;
        
      //-- DEVVAR_LONGSTRINGARRAY ----------------------
      case Tango::DEVVAR_LONGSTRINGARRAY: 
      {
        //- check argin type - must be a 1-by-1 struct array
        if (::mxIsStruct(_argin) == false || ::mxGetM(_argin) != 1 || ::mxGetN(_argin) != 1) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "1-by-1 struct array expected",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- the struct should contains a <lvalue> field containing a 1-by-n int32 array
        mxArray * lvalue = ::mxGetField(_argin, 0, "lvalue");
        if (lvalue == 0) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "struct should contain a struct field named 'lvalue'",
                               "DataAdapter::encode_argin");
          return kError;
        }
        if (::mxIsInt32(lvalue) == false || ::mxGetM(lvalue) != 1) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "1-by-n int32 array expected for struct field 'lvalue'",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- the struct should contains a <svalue> field containing a 1-by-n cell array
        mxArray * svalue = ::mxGetField(_argin, 0, "svalue");
        if (svalue == 0) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "struct should contain a struct field named 'svalue'",
                               "DataAdapter::encode_argin");
          return kError;
        }
        if (::mxIsCell(svalue) == false || ::mxGetM(svalue) != 1) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "1-by-n cell array expected for struct field 'svalue'",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- create a DevVarLongStringArray
        Tango::DevVarLongStringArray * dest = new (std::nothrow) Tango::DevVarLongStringArray;
        if (dest == 0) 
        {
          MEX_UTILS->set_error("out of memory",
                               "Tango::DevVarLongStringArray allocation failed",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- store lvalue into the numeric part of the DevVarLongStringArray  
        size_t len = ::mxGetN(lvalue); 
        dest->lvalue.length(len);
        ::memcpy(dest->lvalue.get_buffer(), ::mxGetData(lvalue), len * sizeof(Tango::DevLong));
        //- store svalue into the string part of the DevVarLongStringArray
        len = ::mxGetN(svalue); 
        dest->svalue.length(len);
        for (size_t i = 0; i < len; i++) 
        {
          //- get <i>th cell of the array
          mxArray * cell = ::mxGetCell(svalue, i);
          if (cell == 0) 
          {
            MEX_UTILS->set_error("internal error",
                                 "unexpected NULL cell",
                                 "DataAdapter::encode_argin");
            delete dest;
            return kError;
          }
          //- cell should contain a string
          if (::mxIsChar(cell) == false) 
          {
            MEX_UTILS->set_error("internal error",
                                 "unexpected cell content (string expected)",
                                 "DataAdapter::encode_argin");
            delete dest;
            return kError;
          }
          //- get c-string from matlab char array.
          char * cstr = ::mxArrayToString(cell);
          if (cstr == 0) 
          {
            MEX_UTILS->set_error("internal error", 
                                 "could not extract string from cell", 
                                 "DataAdapter::encode_argin");
            delete dest;
            return kError;
          }
          //- actual storage - is there should be a way ctsr without duplicate it?
          dest->svalue[i] = CORBA::string_dup(cstr);
          //- release memory
          ::mxFree(cstr);
        } //- for i ...
          //- now insert the Tango::DevVarLongStringArray into the Tango::DeviceData
        dd_in_ << dest;
      } break;
        
      //-- DEVVAR_DOUBLESTRINGARRAY --------------------
      case Tango::DEVVAR_DOUBLESTRINGARRAY: 
      {
        //- check argin type - must be a 1x1 struct array
        if (::mxIsStruct(_argin) == false || ::mxGetM(_argin) != 1 || ::mxGetN(_argin) != 1) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "1x1 struct array expected",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- the struct should contains a <dvalue> field containing a 1-by-n double array
        mxArray * dvalue = ::mxGetField(_argin, 0, "dvalue");
        if (dvalue == 0) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "struct should contain a field named 'dvalue'",
                               "DataAdapter::encode_argin");
          return kError;
        }
        if (::mxIsDouble(dvalue) == false || ::mxGetM(dvalue) != 1) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "1-by-n double array expected for field 'dvalue'",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- the struct should contains a <svalue> field containing a 1-by-n cell array
        mxArray * svalue = ::mxGetField(_argin, 0, "svalue");
        if (svalue == 0) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "struct should contain a field named 'svalue'",
                               "DataAdapter::encode_argin");
          return kError;
        }
        if (::mxIsCell(svalue) == false || ::mxGetM(svalue) != 1) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "1-by-n cell array expected for field 'svalue'",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- create a DevVarDoubleStringArray
        Tango::DevVarDoubleStringArray * dest = new (std::nothrow) Tango::DevVarDoubleStringArray;
        if (dest == 0) 
        {
          MEX_UTILS->set_error("out of memory",
                               "Tango::DevVarDoubleStringArray allocation failed",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- store dvalue into the numeric part of the DevVarDoubleStringArray  
        size_t len = ::mxGetN(dvalue); 
        dest->dvalue.length(len);
        ::memcpy(dest->dvalue.get_buffer(), ::mxGetData(dvalue), len * sizeof(Tango::DevDouble));
        //- store svalue into the string part of the DevVarDoubleStringArray
        len = ::mxGetN(svalue); 
        dest->svalue.length(len);
        for (size_t i = 0; i < len; i++) 
        {
          //- get <i>th cell of the array
          mxArray * cell = ::mxGetCell(svalue, i);
          if (cell == 0) 
          {
            MEX_UTILS->set_error("internal error",
                                 "unexpected NULL cell",
                                 "DataAdapter::encode_argin");
            delete dest;
            return kError;
          }
          //- cell should contain a string
          if (::mxIsChar(cell) == false) 
          {
            MEX_UTILS->set_error("internal error",
                                 "unexpected cell content (string expected)",
                                 "DataAdapter::encode_argin");
            delete dest;
            return kError;
          }
          //- get c-string from matlab char array.
          char * cstr = ::mxArrayToString(cell);
          if (cstr == 0) 
          {
            MEX_UTILS->set_error("internal error", 
                                 "could not extract string from cell", 
                                 "DataAdapter::encode_argin");
            delete dest;
            return kError;
          }
          //- actual storage - is there a way to use ctsr without duplicate it?
          dest->svalue[i] = CORBA::string_dup(cstr);
          //- release allocate d memory
          ::mxFree(cstr);
        } //- for i ...
          //- now insert the Tango::DevVarLongStringArray into the Tango::DeviceData
        dd_in_ << dest;
      } break;
        
      //-- DEVVAR_STRINGARRAY --------------------------
      case Tango::DEVVAR_STRINGARRAY: 
      {
        //- check argin type - must be a 1-by-n cell array
        if (::mxIsCell(_argin) == false || ::mxGetM(_argin) != 1) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "1-by-n cell array expected",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- create a DevVarStringArray
        Tango::DevVarStringArray* dest = new (std::nothrow) Tango::DevVarStringArray;
        if (dest == 0) 
        {
          MEX_UTILS->set_error("out of memory",
                               "Tango::DevVarStringArray allocation failed",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- populate the DevVarLongStringArray
        size_t len = ::mxGetN(_argin); 
        dest->length(len);
        for (size_t i = 0; i < len; i++) 
        {
          //- get <i>th cell of the array   
          mxArray * cell = ::mxGetCell(_argin, i);
          if (cell == 0) 
          {
            MEX_UTILS->set_error("internal error",
                                 "unexpected NULL cell",
                                 "DataAdapter::encode_argin");
            delete dest;
            return kError;
          }
          //- cell should contain a string
          if (::mxIsChar(cell) == false) 
          {
            MEX_UTILS->set_error("internal error",
                                 "unexpected cell content (string expected)",
                                 "DataAdapter::encode_argin");
            delete dest;
            return kError;
          }
          //- get c-string from matlab char array.
          char* cstr = ::mxArrayToString(cell);
          if (cstr == 0) 
          {
            MEX_UTILS->set_error("internal error", 
                                 "could not extract string from cell", 
                                 "DataAdapter::encode_argin");
            delete dest;
            return kError;
          }
          //- actual storage - is there a way to use ctsr without duplicate it?
          (*dest)[i] = CORBA::string_dup(cstr);
          //- release allocated memory
          ::mxFree(cstr);
        } //- for i ...
          //- now insert the Tango::DevVarStringArray into the Tango::DeviceData
        dd_in_ << dest;
      } break;
        
      //-- DEVVAR_DOUBLEARRAY --------------------------
      case Tango::DEVVAR_DOUBLEARRAY: 
      {
        //- check argin type - must be a 1-by-n double array
        if (::mxIsDouble(_argin) == false || ::mxGetM(_argin) != 1) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "1-by-n double array expected",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- create a DevVarDoubleArray
        Tango::DevVarDoubleArray* dest = new (std::nothrow) Tango::DevVarDoubleArray;
        if (dest == 0) 
        {
          MEX_UTILS->set_error("out of memory",
                               "Tango::DevVarDoubleArray allocation failed",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- populate the DevVarDoubleArray
        size_t len = ::mxGetN(_argin); 
        dest->length(len);
        ::memcpy(dest->get_buffer(), ::mxGetData(_argin), len * sizeof(Tango::DevDouble));
        //- now insert the Tango::DevVarDoubleArray into the Tango::DeviceData
        dd_in_ << dest;
      } break;
        
      //-- DEVVAR_FLOATARRAY ---------------------------
      case Tango::DEVVAR_FLOATARRAY: 
      {
        //- check argin type - must be a 1-by-n float array
        if (::mxIsSingle(_argin) == false || ::mxGetM(_argin) != 1) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "1-by-n single array expected",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- create a DevVarFloatArray
        Tango::DevVarFloatArray* dest = new (std::nothrow) Tango::DevVarFloatArray;
        if (dest == 0) 
        {
          MEX_UTILS->set_error("out of memory",
                               "Tango::DevVarFloatArray allocation failed",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- populate the DevVarFloatArray
        size_t len = ::mxGetN(_argin); 
        dest->length(len);
        ::memcpy(dest->get_buffer(), ::mxGetData(_argin), len * sizeof(Tango::DevFloat));
        //- now insert the Tango::DevVarFloatArray into the Tango::DeviceData
        dd_in_ << dest;
      } 
        break;
        
        //-- DEVVAR_ULONG64ARRAY -------------------------
      case Tango::DEVVAR_ULONG64ARRAY: 
      {
        //- check argin type - must be a 1-by-n uint64 array
        if (::mxIsUint64(_argin) == false || ::mxGetM(_argin) != 1) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "1-by-n uint64 array expected",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- create a DevVarULong64Array
        Tango::DevVarULong64Array* dest = new (std::nothrow) Tango::DevVarULong64Array;
        if (dest == 0) 
        {
          MEX_UTILS->set_error("out of memory",
                               "Tango::DevVarULong64Array allocation failed",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- populate the DevVarULong64Array
        size_t len = ::mxGetN(_argin); 
        dest->length(len);
        ::memcpy(dest->get_buffer(), ::mxGetData(_argin), len * sizeof(Tango::DevULong64));
        //- now insert the Tango::DevVarULong64Array into the Tango::DeviceData
        dd_in_ << dest;
      } break;
        
      //-- DEVVAR_LONG64ARRAY -------------------------
      case Tango::DEVVAR_LONG64ARRAY: 
      {
        //- check argin type - must be a 1-by-n int64 array
        if (::mxIsInt64(_argin) == false || ::mxGetM(_argin) != 1) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "1-by-n int64 array expected",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- create a DevVarLong64Array
        Tango::DevVarLong64Array* dest = new (std::nothrow) Tango::DevVarLong64Array;
        if (dest == 0) 
        {
          MEX_UTILS->set_error("out of memory",
                               "Tango::DevVarLong64Array allocation failed",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- populate the DevVarLong64Array
        size_t len = ::mxGetN(_argin); 
        dest->length(len);
        ::memcpy(dest->get_buffer(), ::mxGetData(_argin), len * sizeof(Tango::DevLong64));
        //- now insert the Tango::DevVarLong64Array into the Tango::DeviceData
        dd_in_ << dest;
      } break;
        
      //-- DEVVAR_ULONGARRAY ---------------------------
      case Tango::DEVVAR_ULONGARRAY: 
      {
        //- check argin type - must be a 1-by-n uint32 array
        if (::mxIsUint32(_argin) == false || ::mxGetM(_argin) != 1) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "1-by-n uint32 array expected",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- create a DevVarULongArray
        Tango::DevVarULongArray* dest = new (std::nothrow) Tango::DevVarULongArray;
        if (dest == 0) 
        {
          MEX_UTILS->set_error("out of memory",
                               "Tango::DevVarULongArray allocation failed",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- populate the DevVarULongArray
        size_t len = ::mxGetN(_argin); 
        dest->length(len);
        ::memcpy(dest->get_buffer(), ::mxGetData(_argin), len * sizeof(Tango::DevULong));
        //- now insert the Tango::DevVarULongArray into the Tango::DeviceData
        dd_in_ << dest;
      } break;
        
      //-- DEVVAR_LONGARRAY ---------------------------
      case Tango::DEVVAR_LONGARRAY: 
      {
        //- check argin type - must be a 1-by-n int32 array
        if (::mxIsInt32(_argin) == false || ::mxGetM(_argin) != 1) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "1-by-n int32 array expected",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- create a DevVarLongArray
        Tango::DevVarLongArray* dest = new (std::nothrow) Tango::DevVarLongArray;
        if (dest == 0) 
        {
          MEX_UTILS->set_error("out of memory",
                               "Tango::DevVarLongArray allocation failed",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- populate the DevVarLongArray
        size_t len = ::mxGetN(_argin); 
        dest->length(len);
        ::memcpy(dest->get_buffer(), ::mxGetData(_argin), len * sizeof(Tango::DevLong));
        //- now insert the Tango::DevVarLongArray into the Tango::DeviceData
        dd_in_ << dest;
      } break;
        
      //-- DEVVAR_USHORTARRAY --------------------------
      case Tango::DEVVAR_USHORTARRAY: 
      {
        //- check argin type - must be a 1-by-n uint16 array
        if (::mxIsUint16(_argin) == false || ::mxGetM(_argin) != 1) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "1-by-n uint16 array expected",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- create a DevVarUShortArray
        Tango::DevVarUShortArray* dest = new (std::nothrow) Tango::DevVarUShortArray;
        if (dest == 0) 
        {
          MEX_UTILS->set_error("out of memory",
                               "Tango::DevVarUShortArray allocation failed",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- populate the DevVarUShortArray
        size_t len = ::mxGetN(_argin); 
        dest->length(len);
        ::memcpy(dest->get_buffer(), ::mxGetData(_argin), len * sizeof(Tango::DevUShort));
        //- now insert the Tango::DevVarUShortArray into the Tango::DeviceData
        dd_in_ << dest;
      } break;
        
      //-- DEVVAR_SHORTARRAY ---------------------------
      case Tango::DEVVAR_SHORTARRAY: 
      {
        //- check argin type - must be a 1-by-n int16 array
        if (::mxIsInt16(_argin) == false || ::mxGetM(_argin) != 1) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "1-by-n int16 array expected",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- create a DevVarShortArray
        Tango::DevVarShortArray* dest = new (std::nothrow) Tango::DevVarShortArray;
        if (dest == 0) 
        {
          MEX_UTILS->set_error("out of memory",
                               "Tango::DevVarShortArray allocation failed",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- populate the DevVarShortArray
        size_t len = ::mxGetN(_argin); 
        dest->length(len);
        ::memcpy(dest->get_buffer(), ::mxGetData(_argin), len * sizeof(Tango::DevShort));
        //- now insert the Tango::DevVarShortArray into the Tango::DeviceData
        dd_in_ << dest;
      } break;
        
      //-- DEVVAR_CHARARRAY
      case Tango::DEVVAR_CHARARRAY: 
      {
        //- check argin type - must be a 1-by-n char array
        if ((::mxIsUint8(_argin) == false && ::mxIsInt8(_argin) == false) || ::mxGetM(_argin) != 1) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "1-by-n char array expected",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- create a DevVarCharArray
        Tango::DevVarCharArray* dest = new (std::nothrow) Tango::DevVarCharArray;
        if (dest == 0) 
        {
          MEX_UTILS->set_error("out of memory",
                               "Tango::DevVarCharArray allocation failed",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- populate the DevVarCharArray
        size_t len = ::mxGetN(_argin); 
        dest->length(len);
        ::memcpy(dest->get_buffer(), ::mxGetData(_argin), len * sizeof(Tango::DevUChar));
        //- now insert the Tango::DevVarChartArray into the Tango::DeviceData
        dd_in_ << dest;
      } break;
        
      //-- DEV_STATE -----------------------------------
      case Tango::DEV_STATE: 
      {
        MEX_UTILS->set_error("internal error",
                             "unsupported argin type (DEV_STATE)",
                             "DataAdapter::encode_argin");
        return kError;  
      } break;
        
      //-- CONST_DEV_STRING ----------------------------
      case Tango::CONST_DEV_STRING: 
      {
        MEX_UTILS->set_error("internal error",
                             "unsupported argin type (CONST_DEV_STRING)",
                             "DataAdapter::encode_argin");
        return kError;  
      } break;
        
      //-- DEV_STRING ----------------------------------
      case Tango::DEV_STRING: 
      {
        //- check argin type - must be a 1-by-n char array
        if (::mxIsChar(_argin) == false || ::mxGetM(_argin) != 1) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "1-by-n char array expected",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- get c-string from Matlab array 
        char* cstr = ::mxArrayToString(_argin);
        //- now insert the c-string into the Tango::DeviceData
        dd_in_ << cstr;
        //- release allocated memory
        ::mxFree(cstr);
      } break;
        
      //-- DEV_BOOLEAN ---------------------------------
      case Tango::DEV_BOOLEAN: 
      {
        //- check argin type - must be a 1-by-1 uint8 array
        if (::mxIsUint8(_argin) == false || ::mxGetM(_argin) != 1 || ::mxGetN(_argin) != 1) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "1-by-1 uint8 array expected (boolean mapping)",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- now insert the bool into the Tango::DeviceData
        dd_in_ << (*((Tango::DevUChar*)::mxGetData(_argin)) ? true : false);
      } break;
        
      //-- DEV_SHORT -----------------------------------
      case Tango::DEV_USHORT: 
      {
        //- check argin type - must be a 1-by-1 uint16 array
        if (::mxIsUint16(_argin) == false || ::mxGetM(_argin) != 1 || ::mxGetN(_argin) != 1) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "1-by-1 uint16 array expected",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- now insert the unsigned short into the Tango::DeviceData
        dd_in_ << *((Tango::DevUShort*)::mxGetData(_argin));
      } break;
        
      //-- DEV_SHORT -----------------------------------
      case Tango::DEV_SHORT: 
      {
        //- check argin type - must be a 1-by-1 int16 array
        if (::mxIsInt16(_argin) == false || ::mxGetM(_argin) != 1 || ::mxGetN(_argin) != 1) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "1-by-1 int16 array expected",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- now insert the short into the Tango::DeviceData
        dd_in_ << *((Tango::DevShort*)::mxGetData(_argin));
      } break;
        
      //-- DEV_ULONG64 ---------------------------------
      case Tango::DEV_ULONG64: 
      {
        //- check argin type - must be a 1-by-1 uint64 array
        if (::mxIsUint64(_argin) == false || ::mxGetM(_argin) != 1 || ::mxGetN(_argin) != 1) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "1-by-1 uint64 array expected",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- now insert the unsigned long into the Tango::DeviceData
        dd_in_ << *((Tango::DevULong64*)::mxGetData(_argin));
      } break;
        
      //-- DEV_LONG64 ----------------------------------
      case Tango::DEV_LONG64: 
      {
        //- check argin type - must be a 1-by-1 int64 array
        if (::mxIsInt64(_argin) == false || ::mxGetM(_argin) != 1 || ::mxGetN(_argin) != 1) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "1-by-1 int64 array expected",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- now insert the unsigned long into the Tango::DeviceData
        dd_in_ << *((Tango::DevLong64*)::mxGetData(_argin));
      } break;
        
      //-- DEV_ULONG -----------------------------------
      case Tango::DEV_ULONG: 
      {
        //- check argin type - must be a 1-by-1 uint32 array
        if (::mxIsUint32(_argin) == false || ::mxGetM(_argin) != 1 || ::mxGetN(_argin) != 1) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "1-by-1 uint32 array expected",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- now insert the unsigned long into the Tango::DeviceData
        dd_in_ << *((Tango::DevULong*)::mxGetData(_argin));
      } break;
        
      //-- DEV_LONG ------------------------------------
      case Tango::DEV_LONG: 
      {
        //- check argin type - must be a 1-by-1 int32 array
        if (::mxIsInt32(_argin) == false || ::mxGetM(_argin) != 1 || ::mxGetN(_argin) != 1) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "1-by-1 int32 array expected",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- now insert the long into the Tango::DeviceData
        dd_in_ << *((Tango::DevLong*)::mxGetData(_argin));
      } break;
        
      //-- DEV_FLOAT -----------------------------------
      case Tango::DEV_FLOAT: 
      {
        //- check argin type - must be a 1-by-1 single array
        if (::mxIsSingle(_argin) == false || ::mxGetM(_argin) != 1 || ::mxGetN(_argin) != 1) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "1-by-1 single array expected",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- now insert the float into the Tango::DeviceData
        dd_in_ << *((Tango::DevFloat*)::mxGetData(_argin));
      } break;
        
      //-- DEV_DOUBLE ----------------------------------
      case Tango::DEV_DOUBLE: 
      {
        //- check argin type - must be a 1-by-1 double array
        if (::mxIsDouble(_argin) == false || ::mxGetM(_argin) != 1 || ::mxGetN(_argin) != 1) 
        {
          MEX_UTILS->set_error("invalid argin specified",
                               "1-by-1 double array expected",
                               "DataAdapter::encode_argin");
          return kError;
        }
        //- now insert the double into the Tango::DeviceData
        dd_in_ << *((Tango::DevDouble*)::mxGetPr(_argin));
      } 
      break;
                    
      //-- DEFAULT BEHAVIOUR ---------------------------
      default:
      {
        MEX_UTILS->set_error("unknown TANGO data type",
                             "unexpected TANGO data type for argin",
                             "DataAdapter::encode_argin");
        return kError; 
      } break;
    } //- switch 
  } //try
  catch (const Tango::DevFailed &e) 
  {
    MEX_UTILS->set_error(e);
    return kError;
  }
  catch (...) 
  {
    MEX_UTILS->set_error("unknown error",
                         "unknown exception caught",
                         "DataAdapter::encode_argin");
    return kError;
  }
  return kNoError;
}
                    
//=============================================================================
//- DataAdapter::decode_argout
//=============================================================================
int DataAdapter::decode_argout (DeviceDesc* _ddesc, 
                                int _cmd_id,
                                Tango::DeviceData& _dd_out, 
                                mxArray *& argout_)
{
  argout_ = 0;
          
  if (_ddesc == 0) 
  {
    MEX_UTILS->set_error("internal error",
                         "unexpected NULL argument",
                         "DataAdapter::decode_argout");
    return kError;
  }
  
  _dd_out.set_exceptions(Tango::DeviceData::isempty_flag);
  _dd_out.set_exceptions(Tango::DeviceData::wrongtype_flag);
  
  try 
  {
    int argout_type = (_ddesc->cmd_list())[_cmd_id].out_type;
    switch (argout_type) 
    {
      //-- DEV_VOID ------------------------------------
      case Tango::DEV_VOID: 
      {
        return kNoError; 
      } break;
        
      //-- DEVVAR_LONGSTRINGARRAY ----------------------
      case Tango::DEVVAR_LONGSTRINGARRAY:
      {
        const Tango::DevVarLongStringArray* src = 0;
        _dd_out >> src;
        if (src == 0) {
          MEX_UTILS->set_error("internal error",
                               "could not extract DevVarLongStringArray from DeviceData",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- create a 1-by-1 struct array containing 2 fields
        mwSize dims[2] = {1, 1};
        const char *field_names[] = {"lvalue", "svalue"};
        argout_ = ::mxCreateStructArray(2, dims, 2, field_names);
        if (argout_ == 0) {
          MEX_UTILS->set_error("out of memory",
                               "mxCreateStructArray failed",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- create a 1-by-n int32 array to populate field lvalue
        size_t len = src->lvalue.length();
        dims[1] = len;
        mxArray * lvalue = ::mxCreateNumericArray(2, dims, ::mxINT32_CLASS, ::mxREAL);
        if (lvalue == 0) {
          MEX_UTILS->set_error("out of memory",
                               "mxCreateNumericArray failed",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- copy from src to dest (field lvalue)
        ::memcpy(::mxGetData(lvalue), src->lvalue.get_buffer(), len * sizeof(Tango::DevLong)); 
        //- attach lvalue to argout_.lvalue 
        ::mxSetFieldByNumber(argout_, 0, 0, lvalue);
        //- create a 1-by-n cell array to populate field svalue
        len = src->svalue.length();
        dims[1] = len;
        mxArray * svalue = ::mxCreateCellArray(2, dims);
        if (svalue == 0) {
          MEX_UTILS->set_error("out of memory",
                               "mxCreateCellArray failed",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- copy from src to dest (field svalue)
        for (size_t i = 0; i < len; i++) {
          ::mxSetCell(svalue, i, ::mxCreateString(src->svalue[i]));     
        }
        //- attach svalue to argout_.svalue 
        ::mxSetFieldByNumber(argout_, 0, 1, svalue);
      } break;
        
      //-- DEVVAR_DOUBLESTRINGARRAY --------------------
      case Tango::DEVVAR_DOUBLESTRINGARRAY: 
      {
        const Tango::DevVarDoubleStringArray* src = 0;
        _dd_out >> src;
        if (src == 0) {
          MEX_UTILS->set_error("internal error",
                               "could not extract DevVarDoubleStringArray from DeviceData",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- create a 1-by-1 struct array containing 2 fields
        mwSize dims[2] = {1, 1};
        const char *field_names[] = {"dvalue", "svalue"};
        argout_ = ::mxCreateStructArray(2, dims, 2, field_names);
        if (argout_ == 0) {
          MEX_UTILS->set_error("out of memory",
                               "mxCreateStructArray failed",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- create a 1-by-n double array to populate field dvalue
        size_t len = src->dvalue.length();
        dims[1] = len;
        mxArray * dvalue = ::mxCreateNumericArray(2, dims, ::mxDOUBLE_CLASS, ::mxREAL);
        if (dvalue == 0) {
          MEX_UTILS->set_error("out of memory",
                               "mxCreateNumericArray failed",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- copy from src to dest (field dvalue)
        ::memcpy(::mxGetData(dvalue), src->dvalue.get_buffer(), len * sizeof(Tango::DevDouble)); 
        //- attach dvalue to argout_.dvalue 
        ::mxSetFieldByNumber(argout_, 0, 0, dvalue);
        //- create a 1-by-n cell array to populate field svalue
        len = src->svalue.length();
        dims[1] = len;
        mxArray * svalue = ::mxCreateCellArray(2, dims);
        if (svalue == 0) {
          MEX_UTILS->set_error("out of memory",
                               "mxCreateCellArray failed",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- copy from src to dest (field svalue)
        for (size_t i = 0; i < len; i++) {
          ::mxSetCell(svalue, i, ::mxCreateString(src->svalue[i]));     
        }
        //- attach svalue to argout_.svalue 
        ::mxSetFieldByNumber(argout_, 0, 1, svalue);
      } break;
        
      //-- DEVVAR_STRINGARRAY -------------------------
      case Tango::DEVVAR_STRINGARRAY: 
      {
        const Tango::DevVarStringArray* src = 0;
        _dd_out >> src;
        if (src == 0) {
          MEX_UTILS->set_error("internal error",
                               "could not extract DevVarStringArray from DeviceData",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- create a 1-by-n cell array
        size_t len = src->length();
        const mwSize dims[2] = {1, len};
        argout_  = ::mxCreateCellArray(2, dims);
        if (argout_ == 0) {
          MEX_UTILS->set_error("out of memory",
                               "mxCreateCellArray failed",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- copy from src to dest
        for (size_t i = 0; i < len; i++) {
          ::mxSetCell(argout_, i, ::mxCreateString((*src)[i]));     
        }
      } break;
        
      //-- DEVVAR_DOUBLEARRAY -------------------------
      case Tango::DEVVAR_DOUBLEARRAY: 
      {
        const Tango::DevVarDoubleArray* src = 0;
        _dd_out >> src;
        if (src == 0) {
          MEX_UTILS->set_error("internal error",
                               "could not extract DevVarDoubleArray from DeviceData",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- create a 1-by-n float array
        size_t len = src->length();
        const mwSize dims[2] = {1, len};
        argout_ = ::mxCreateNumericArray(2, dims, ::mxDOUBLE_CLASS, ::mxREAL);
        if (argout_ == 0) {
          MEX_UTILS->set_error("out of memory",
                               "mxCreateNumericArray failed",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- copy from src to dest
        ::memcpy(::mxGetData(argout_), src->get_buffer(), len * sizeof(Tango::DevDouble)); 
      } break;
        
      //-- DEVVAR_FLOATARRAY -------------------------
      case Tango::DEVVAR_FLOATARRAY: 
      {
        const Tango::DevVarFloatArray* src = 0;
        _dd_out >> src;
        if (src == 0) {
          MEX_UTILS->set_error("internal error",
                               "could not extract DevVarFloatArray from DeviceData",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- create a 1-by-n float array
        size_t len = src->length();
        const mwSize dims[2] = {1, len};
        argout_ = ::mxCreateNumericArray(2, dims, ::mxSINGLE_CLASS, ::mxREAL);
        if (argout_ == 0) {
          MEX_UTILS->set_error("out of memory",
                               "mxCreateNumericArray failed",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- copy from src to dest
        ::memcpy(::mxGetData(argout_), src->get_buffer(), len * sizeof(Tango::DevFloat)); 
      } break;
     
      //-- DEVVAR_ULONG64ARRAY -------------------------
      case Tango::DEVVAR_ULONG64ARRAY: 
      {
        const Tango::DevVarULong64Array* src = 0;
        _dd_out >> src;
        if (src == 0) {
          MEX_UTILS->set_error("internal error",
                               "could not extract DevVarULong64Array from DeviceData",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- create a 1-by-n uint64 array
        size_t len = src->length();
        const mwSize dims[2] = {1, len};
        argout_ = ::mxCreateNumericArray(2, dims, ::mxUINT64_CLASS, ::mxREAL);
        if (argout_ == 0) {
          MEX_UTILS->set_error("out of memory",
                               "mxCreateNumericArray failed",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- copy from src to dest
        ::memcpy(::mxGetData(argout_), src->get_buffer(), len * sizeof(Tango::DevULong64)); 
      } break;
      
      //-- DEVVAR_LONG64ARRAY -------------------------
      case Tango::DEVVAR_LONG64ARRAY: 
      {
        const Tango::DevVarLong64Array* src = 0;
        _dd_out >> src;
        if (src == 0) {
          MEX_UTILS->set_error("internal error",
                               "could not extract DevVarLong64Array from DeviceData",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- create a 1-by-n int64 array
        size_t len = src->length();
        const mwSize dims[2] = {1, len};
        argout_ = ::mxCreateNumericArray(2, dims, ::mxINT64_CLASS, ::mxREAL);
        if (argout_ == 0) {
          MEX_UTILS->set_error("out of memory",
                               "mxCreateNumericArray failed",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- copy from src to dest
        ::memcpy(::mxGetData(argout_), src->get_buffer(), len * sizeof(Tango::DevLong64)); 
      } break;
      
      //-- DEVVAR_ULONGARRAY -------------------------
      case Tango::DEVVAR_ULONGARRAY: 
      {
        const Tango::DevVarULongArray* src = 0;
        _dd_out >> src;
        if (src == 0) {
          MEX_UTILS->set_error("internal error",
                               "could not extract DevVarULongArray from DeviceData",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- create a 1-by-n uint32 array
        size_t len = src->length();
        const mwSize dims[2] = {1, len};
        argout_ = ::mxCreateNumericArray(2, dims, ::mxUINT32_CLASS, ::mxREAL);
        if (argout_ == 0) {
          MEX_UTILS->set_error("out of memory",
                               "mxCreateNumericArray failed",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- copy from src to dest
        ::memcpy(::mxGetData(argout_), src->get_buffer(), len * sizeof(Tango::DevULong)); 
      } break;
        
      //-- DEVVAR_LONGARRAY -------------------------
      case Tango::DEVVAR_LONGARRAY: 
      {
        const Tango::DevVarLongArray* src = 0;
        _dd_out >> src;
        if (src == 0) {
          MEX_UTILS->set_error("internal error",
                               "could not extract DevVarLongArray from DeviceData",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- create a 1-by-n int32 array
        size_t len = src->length();
        const mwSize dims[2] = {1, len};
        argout_ = ::mxCreateNumericArray(2, dims, ::mxINT32_CLASS, ::mxREAL);
        if (argout_ == 0) {
          MEX_UTILS->set_error("out of memory",
                               "mxCreateNumericArray failed",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- copy from src to dest
        ::memcpy(::mxGetData(argout_), src->get_buffer(), len * sizeof(Tango::DevLong)); 
      } break;
        
      //-- DEVVAR_USHORTARRAY -------------------------
      case Tango::DEVVAR_USHORTARRAY: 
      {
        const Tango::DevVarUShortArray* src = 0;
        _dd_out >> src;
        if (src == 0) {
          MEX_UTILS->set_error("internal error",
                               "could not extract DevVarUShortArray from DeviceData",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- create a 1-by-n uint16 array
        size_t len = src->length();
        const mwSize dims[2] = {1, len};
        argout_ = ::mxCreateNumericArray(2, dims, ::mxUINT16_CLASS, ::mxREAL);
        if (argout_ == 0) {
          MEX_UTILS->set_error("out of memory",
                               "mxCreateNumericArray failed",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- copy from src to dest
        ::memcpy(::mxGetData(argout_), src->get_buffer(), len * sizeof(Tango::DevUShort)); 
      } break;
        
      //-- DEVVAR_SHORTARRAY -------------------------
      case Tango::DEVVAR_SHORTARRAY: 
      {
        const Tango::DevVarShortArray* src = 0;
        _dd_out >> src;
        if (src == 0) {
          MEX_UTILS->set_error("internal error",
                               "could not extract DevVarShortArray from DeviceData",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- create a 1-by-n int16 array
        size_t len = src->length();
        const mwSize dims[2] = {1, len};
        argout_ = ::mxCreateNumericArray(2, dims, ::mxINT16_CLASS, ::mxREAL);
        if (argout_ == 0) {
          MEX_UTILS->set_error("out of memory",
                               "mxCreateNumericArray failed",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- copy from src to dest
        ::memcpy(::mxGetData(argout_), src->get_buffer(), len * sizeof(Tango::DevShort)); 
      } break;
        
      //-- DEVVAR_CHARARRAY -------------------------
      case Tango::DEVVAR_CHARARRAY: 
      {
        const Tango::DevVarCharArray* src = 0;
        _dd_out >> src;
        if (src == 0) {
          MEX_UTILS->set_error("internal error",
                               "could not extract DevVarCharArray from DeviceData",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- create a 1-by-n uint8 array
        size_t len = src->length();
        const mwSize dims[2] = {1, len};
        argout_ = ::mxCreateNumericArray(2, dims, ::mxUINT8_CLASS, ::mxREAL);
        if (argout_ == 0) {
          MEX_UTILS->set_error("out of memory",
                               "mxCreateNumericArray failed",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- copy from src to dest
        ::memcpy(::mxGetData(argout_), src->get_buffer(), len * sizeof(Tango::DevUChar)); 
      } break;
        
      //-- DEV_STRING --------------------------------
      case Tango::DEV_STRING: 
      {
        std::string str;
        _dd_out >> str;
        //- create a 1-by-n char array
        argout_ = ::mxCreateString(str.c_str());
        if (argout_ == 0) {
          MEX_UTILS->set_error("out of memory",
                               "mxCreateString failed",
                               "DataAdapter::decode_argout");
          return kError;
        }
      } 
        break;
        
        //-- DEV_BOOLEAN -------------------------------
      case Tango::DEV_BOOLEAN: 
      {
        Tango::DevBoolean b;
        _dd_out >> b;
        //- create a 1-by-1 uint8 array
        const mwSize dims[2] = {1, 1};
        argout_ = ::mxCreateNumericArray(2, dims, ::mxUINT8_CLASS, ::mxREAL);
        if (argout_ == 0) {
          MEX_UTILS->set_error("out of memory",
                               "mxCreateNumericArray failed",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- copy from src to dest
        *((Tango::DevUChar*)::mxGetData(argout_)) = (b == true) ? 1 : 0;
      } break;
        
      //-- DEV_USHORT --------------------------------
      case Tango::DEV_USHORT: 
      {
        Tango::DevUShort us;
        _dd_out >> us;
        //- create a 1-by-1 uint16 array
        const mwSize dims[2] = {1, 1};
        argout_ = ::mxCreateNumericArray(2, dims, ::mxUINT16_CLASS, ::mxREAL);
        if (argout_ == 0) {
          MEX_UTILS->set_error("out of memory",
                               "mxCreateNumericArray failed",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- copy from src to dest
        *((Tango::DevUShort*)::mxGetData(argout_)) = us;
      } break;
        
      //-- DEV_SHORT ---------------------------------
      case Tango::DEV_SHORT: 
      {
        Tango::DevShort s;
        _dd_out >> s;
        //- create a 1-by-1 int16 array
        const mwSize dims[2] = {1, 1};
        argout_ = ::mxCreateNumericArray(2, dims, ::mxINT16_CLASS, ::mxREAL);
        if (argout_ == 0) {
          MEX_UTILS->set_error("out of memory",
                               "mxCreateNumericArray failed",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- copy from src to dest
        *((Tango::DevShort*)::mxGetData(argout_)) = s;
      } break;
        
      //-- DEV_ULONG64 -------------------------------
      case Tango::DEV_ULONG64: 
      {
        Tango::DevULong64 ull;
        _dd_out >> ull;
        //- create a 1-by-1 uint64 array
        const mwSize dims[2] = {1, 1};
        argout_ = ::mxCreateNumericArray(2, dims, ::mxUINT64_CLASS, ::mxREAL);
        if (argout_ == 0) {
          MEX_UTILS->set_error("out of memory",
                               "mxCreateNumericArray failed",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- copy from src to dest
        *((Tango::DevULong64*)::mxGetData(argout_)) = ull;
      } break;
      
      //-- DEV_LONG64 --------------------------------
      case Tango::DEV_LONG64: 
      {
        Tango::DevULong64 ll;
        _dd_out >> ll;
        //- create a 1-by-1 uint64 array
        const mwSize dims[2] = {1, 1};
        argout_ = ::mxCreateNumericArray(2, dims, ::mxINT64_CLASS, ::mxREAL);
        if (argout_ == 0) {
          MEX_UTILS->set_error("out of memory",
                               "mxCreateNumericArray failed",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- copy from src to dest
        *((Tango::DevLong64*)::mxGetData(argout_)) = ll;
      } break;
      
      //-- DEV_ULONG ---------------------------------
      case Tango::DEV_ULONG: 
      {
        Tango::DevULong ul;
        _dd_out >> ul;
        //- create a 1-by-1 uint32 array
        const mwSize dims[2] = {1, 1};
        argout_ = ::mxCreateNumericArray(2, dims, ::mxUINT32_CLASS, ::mxREAL);
        if (argout_ == 0) {
          MEX_UTILS->set_error("out of memory",
                               "mxCreateNumericArray failed",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- copy from src to dest
        *((Tango::DevULong*)::mxGetData(argout_)) = ul;
      } break;
        
      //-- DEV_LONG ---------------------------------
      case Tango::DEV_LONG: 
      {
        Tango::DevLong l;
        _dd_out >> l;
        //- create a 1-by-1 int32 array
        const mwSize dims[2] = {1, 1};
        argout_ = ::mxCreateNumericArray(2, dims, ::mxINT32_CLASS, ::mxREAL);
        if (argout_ == 0) {
          MEX_UTILS->set_error("out of memory",
                               "mxCreateNumericArray failed",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- copy from src to dest
        *((Tango::DevLong*)::mxGetData(argout_)) = l;
      } break;
      
      //-- DEV_FLOAT --------------------------------
      case Tango::DEV_FLOAT: 
      {
        Tango::DevFloat f;
        _dd_out >> f;
        //- create a 1-by-1 single array
        const mwSize dims[2] = {1, 1};
        argout_ = ::mxCreateNumericArray(2, dims, ::mxSINGLE_CLASS, ::mxREAL);
        if (argout_ == 0) {
          MEX_UTILS->set_error("out of memory",
                               "mxCreateNumericArray failed",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- copy from src to dest
        *((Tango::DevFloat*)::mxGetData(argout_)) = f;
      } break;
        
      //-- DEV_DOUBLE -------------------------------
      case Tango::DEV_DOUBLE: 
      {
        Tango::DevDouble d;
        _dd_out >> d;
        //- create a 1-by-1 double array
        const mwSize dims[2] = {1, 1};
        argout_ = ::mxCreateNumericArray(2, dims, ::mxDOUBLE_CLASS, ::mxREAL);
        if (argout_ == 0) {
          MEX_UTILS->set_error("out of memory",
                               "mxCreateNumericArray failed",
                               "DataAdapter::decode_argout");
          return kError;
        }
        //- copy from src to dest
        *((Tango::DevDouble*)::mxGetPr(argout_)) = d;
      } break;
        
      //-- DEV_STATE --------------------------------
      case Tango::DEV_STATE: 
      {
        Tango::DevState ds;
        _dd_out >> ds;
        //- limit ds to Tango::DevStateName capacity
        if (ds > Tango::UNKNOWN) {
          ds = Tango::UNKNOWN;
        }
        //- create a 1-by-n char array
        argout_ = ::mxCreateString(Tango::DevStateName[ds]);
        if (argout_ == 0) {
          MEX_UTILS->set_error("out of memory",
                               "mxCreateString failed",
                               "DataAdapter::decode_argout");
          return kError;
        }
      } break;
        
      //-- DEFAULT ----------------------------------
      default:
      {
        MEX_UTILS->set_error("unknown TANGO data type",
                             "unexpected TANGO data type for argout",
                             "DataAdapter::decode_argout");
        return kError;         
      } break;
    } //- switch
  } //- try        
  catch (const Tango::DevFailed &e) 
  {
    MEX_UTILS->set_error(e);
    return kError;
  }
  catch (...) 
  {
    MEX_UTILS->set_error("unknown error",
                         "unknown exception caught",
                         "DataAdapter::decode_argout");
    return kError;
  }
  
  return kNoError;
}
                    
//=============================================================================
//- DataAdapter::encode_attr
//=============================================================================
int DataAdapter::encode_attr (DeviceDesc * _ddesc,
                              int _attr_id,
                              mxArray * _argin,
                              Tango::DeviceAttribute & value_)
{
  //- check input
  if (_ddesc == 0) 
  {
    MEX_UTILS->set_error("internal error",
                         "unexpected NULL argument",
                         "DataAdapter::encode_attr");
    return kError;
  }
  //- get attr list
  const DeviceDesc::MinAttrInfoList &attr_list = _ddesc->attr_list();
  //- get attr format
  int attr_format = attr_list[_attr_id].data_format;
  //- get attr data type
  int attr_type = attr_list[_attr_id].data_type;
  //- set common values
  value_.quality = Tango::ATTR_VALID;
  value_.name = attr_list[_attr_id].name;
#ifdef WIN32
  struct _timeb now;
  ::_ftime(&now);
  value_.time.tv_sec  = (CORBA::Long)now.time;
  value_.time.tv_usec = now.millitm * 1000;
  value_.time.tv_nsec = 0;
#else
  struct timeval now;
  ::gettimeofday(&now, NULL);
  value_.time.tv_sec  = now.tv_sec;
  value_.time.tv_usec = now.tv_usec;
  value_.time.tv_nsec = 0;
#endif
  //- convert mxArray to TANGO type
  try 
  {
    switch (attr_format) 
    {
        //-- SCALAR ATTRIBUTE -------------------------------------------
      case Tango::SCALAR: 
      {
        switch (attr_type) 
        {
            //-- SCALAR::DEV_BOOLEAN -------------------------------------
          case Tango::DEV_BOOLEAN: 
          {
            //- check argin type - must be a 1-by-1 uint8 array
            if (::mxIsUint8(_argin) == false || ::mxGetM(_argin) != 1 || ::mxGetN(_argin) != 1) {
              MEX_UTILS->set_error("invalid argin specified",
                                   "1-by-1 boolean (i.e. uint8) array expected",
                                   "DataAdapter::encode_attr");
              return kError;
            }
            //- insert the unsigned char (i.e. bool) into the Tango::DeviceAttribute
            value_  << (*((unsigned char*)::mxGetData(_argin)) ? true : false);
            //- set dims
            value_.dim_x = 1;
            value_.dim_y = 0;
          } break;
            //-- SCALAR::DEV_UCHAR ---------------------------------------
          case Tango::DEV_UCHAR: 
          {
            //- check argin type - must be a 1-by-1 uint8 array
            if (::mxIsUint8(_argin) == false || ::mxGetM(_argin) != 1 || ::mxGetN(_argin) != 1) {
              MEX_UTILS->set_error("invalid argin specified",
                                   "1-by-1 uint8 array expected",
                                   "DataAdapter::encode_attr");
              return kError;
            }
            //- now insert the unsigned char into the Tango::DeviceAttribute
            value_  << *((Tango::DevUChar*)::mxGetData(_argin));
            //- set dims
            value_.dim_x = 1;
            value_.dim_y = 0;
          } break;
            //-- SCALAR::DEV_USHORT --------------------------------------
          case Tango::DEV_USHORT: 
          {
            //- check argin type - must be a 1-by-1 uint16 array
            if (::mxIsUint16(_argin) == false || ::mxGetM(_argin) != 1 || ::mxGetN(_argin) != 1) {
              MEX_UTILS->set_error("invalid argin specified",
                                   "1-by-1 uint16 array expected",
                                   "DataAdapter::encode_attr");
              return kError;
            }
            //- now insert the unsigned short into the Tango::DeviceAttribute
            value_  << *((Tango::DevUShort*)::mxGetData(_argin));
            //- set dims
            value_.dim_x = 1;
            value_.dim_y = 0;
          } break;
            //-- SCALAR::DEV_SHORT ---------------------------------------
          case Tango::DEV_SHORT: 
          {
            //- check argin type - must be a 1-by-1 int16 array
            if (::mxIsInt16(_argin) == false || ::mxGetM(_argin) != 1 || ::mxGetN(_argin) != 1) {
              MEX_UTILS->set_error("invalid argin specified",
                                   "1-by-1 int16 array expected",
                                   "DataAdapter::encode_attr");
              return kError;
            }
            //- now insert the short into the Tango::DeviceAttribute
            value_  << *((Tango::DevShort*)::mxGetData(_argin));
            //- set dims
            value_.dim_x = 1;
            value_.dim_y = 0;
          } break;
          //-- SCALAR::DEV_LONG64 --------------------------------------
          case Tango::DEV_LONG64: 
          {
            //- check argin type - must be a 1-by-1 int64 array
            if (::mxIsInt64(_argin) == false || ::mxGetM(_argin) != 1 || ::mxGetN(_argin) != 1) {
              MEX_UTILS->set_error("invalid argin specified",
                                   "1-by-1 int64 array expected",
                                   "DataAdapter::encode_attr");
              return kError;
            }
            //- now insert the long into the Tango::DeviceAttribute
            value_  << *((Tango::DevLong64*)::mxGetData(_argin));
            //- set dims
            value_.dim_x = 1;
            value_.dim_y = 0;
          } break;
          //-- SCALAR::DEV_ULONG64 --------------------------------------
          case Tango::DEV_ULONG64: 
          {
            //- check argin type - must be a 1-by-1 uint64 array
            if (::mxIsUint64(_argin) == false || ::mxGetM(_argin) != 1 || ::mxGetN(_argin) != 1) {
              MEX_UTILS->set_error("invalid argin specified",
                                   "1-by-1 uint64 array expected",
                                   "DataAdapter::encode_attr");
              return kError;
            }
            //- now insert the unsigned long long into the Tango::DeviceAttribute
            value_  << *((Tango::DevULong64*)::mxGetData(_argin));
            //- set dims
            value_.dim_x = 1;
            value_.dim_y = 0;
          } break;
          //-- SCALAR::DEV_LONG ----------------------------------------
          case Tango::DEV_LONG: 
          {
            //- check argin type - must be a 1-by-1 int32 array
            if (::mxIsInt32(_argin) == false || ::mxGetM(_argin) != 1 || ::mxGetN(_argin) != 1) {
              MEX_UTILS->set_error("invalid argin specified",
                                   "1-by-1 int32 array expected",
                                   "DataAdapter::encode_attr");
              return kError;
            }
            //- now insert the long into the Tango::DeviceAttribute
            value_  << *((Tango::DevLong*)::mxGetData(_argin));
            //- set dims
            value_.dim_x = 1;
            value_.dim_y = 0;
          } break;
          //-- SCALAR::DEV_ULONG ----------------------------------------
          case Tango::DEV_ULONG: 
          {
            //- check argin type - must be a 1-by-1 uint32 array
            if (::mxIsUint32(_argin) == false || ::mxGetM(_argin) != 1 || ::mxGetN(_argin) != 1) {
              MEX_UTILS->set_error("invalid argin specified",
                                   "1-by-1 uint32 array expected",
                                   "DataAdapter::encode_attr");
              return kError;
            }
            //- now insert the unsigned long into the Tango::DeviceAttribute
            value_  << *((Tango::DevULong*)::mxGetData(_argin));
            //- set dims
            value_.dim_x = 1;
            value_.dim_y = 0;
          } break;
          //-- SCALAR::DEV_FLOAT ---------------------------------------
          case Tango::DEV_FLOAT: 
          {
            //- check argin type - must be a 1-by-1 single array
            if (::mxIsSingle(_argin) == false || ::mxGetM(_argin) != 1 || ::mxGetN(_argin) != 1) {
              MEX_UTILS->set_error("invalid argin specified",
                                   "1-by-1 single array expected",
                                   "DataAdapter::encode_attr");
              return kError;
            }
            //- now insert the float into the Tango::DeviceAttribute
            value_  << *((Tango::DevFloat*)::mxGetData(_argin));
            //- set dims
            value_.dim_x = 1;
            value_.dim_y = 0;
          } break;
          //-- SCALAR::DEV_DOUBLE --------------------------------------
          case Tango::DEV_DOUBLE: 
          {
            //- check argin type - must be a 1-by-1 double array
            if (::mxIsDouble(_argin) == false || ::mxGetM(_argin) != 1 || ::mxGetN(_argin) != 1) {
              MEX_UTILS->set_error("invalid argin specified",
                                   "1-by-1 double array expected",
                                   "DataAdapter::encode_attr");
              return kError;
            }
            //- now insert the double into the Tango::DeviceAttribute
            value_  << *((Tango::DevDouble*)::mxGetPr(_argin));
            //- set dims
            value_.dim_x = 1;
            value_.dim_y = 0;
          } break;
          //-- SCALAR::DEV_STRING --------------------------------------
          case Tango::DEV_STRING: 
          {
            //- check argin type - must be a 1-by-n char array
            if (::mxIsChar(_argin) == false || ::mxGetM(_argin) != 1) {
              MEX_UTILS->set_error("invalid argin specified",
                                   "1-by-n char array expected",
                                   "DataAdapter::encode_attr");
              return kError;
            }
            //- get c-string from Matlab array 
            char* cstr = ::mxArrayToString(_argin);
            //- now insert the c-string into the Tango::DeviceData
            //- make gcc happy : doesn't like : value_ << std::string(cstr);
            std::string str(cstr);
            value_ << str;
            //- release allocated memory
            ::mxFree(cstr);
          } break;
          //------------------------------------------------------------
          default:
          {
            MEX_UTILS->set_error("unexpected TANGO data type",
                                 "unsupported TANGO data type for scalar attribute",
                                 "DataAdapter::encode_attr");
            return kError;
          } break;
        } //- switch::attr_type
      } break; //- SCALAR
      //-- SPECTRUM ATTRIBUTE -----------------------------------------
      case Tango::SPECTRUM: 
      {
        switch (attr_type) 
        {
          //-- SPECTRUM::DEV_BOOLEAN -------------------------------------
          case Tango::DEV_BOOLEAN: 
          {
            //- check argin type - must be a 1-by-n uint8 array
            if (::mxIsUint8(_argin) == false || ::mxGetM(_argin) != 1) {
              MEX_UTILS->set_error("invalid argin specified",
                                   "1-by-n boolean (i.e. uint8) array expected",
                                   "DataAdapter::encode_attr");
              return kError;
            }
            //- create a DevVarBooleanArray
            Tango::DevVarBooleanArray* dest = new (std::nothrow) Tango::DevVarBooleanArray;
            if (dest == 0) {
              MEX_UTILS->set_error("out of memory",
                                   "Tango::DevVarBooleanArray allocation failed",
                                   "DataAdapter::encode_attr");
              return kError;
            }
            //- populate the Tango::DevVarBooleanArray
            size_t len = ::mxGetN(_argin); 
            dest->length(len);
            if (sizeof(bool) == sizeof(unsigned char)) {
              ::memcpy(dest->get_buffer(), ::mxGetData(_argin), len * sizeof(Tango::DevUChar));
            }
            else {
              unsigned char* p = (unsigned char*)::mxGetData(_argin);
              for (size_t i = 0; i < len; i++) {
                (*dest)[i] = p[i] ? true : false;
              }
            }
            //- now insert the Tango::DevVarBooleanArray into the Tango::DeviceAttribute
            value_ << dest;
            //- set dims
            value_.dim_x = len;
            value_.dim_y = 0;
          } break;
          //-- SPECTRUM::DEV_UCHAR ---------------------------------------
          case Tango::DEV_UCHAR: 
          {
            //- check argin type - must be a 1-by-n uint8 array
            if (::mxIsUint8(_argin) == false || ::mxGetM(_argin) != 1) {
             MEX_UTILS->set_error("invalid argin specified",
                                  "1-by-n boolean uint8 array expected",
                                  "DataAdapter::encode_attr");
             return kError;
            }
            //- create a DevVarUCharArray
            Tango::DevVarUCharArray* dest = new (std::nothrow) Tango::DevVarUCharArray;
            if (dest == 0) {
             MEX_UTILS->set_error("out of memory",
                                  "Tango::DevVarUCharArray allocation failed",
                                  "DataAdapter::encode_attr");
             return kError;
            }
            //- populate the Tango::DevVarUCharArray
            size_t len = ::mxGetN(_argin); 
            dest->length(len);
            ::memcpy(dest->get_buffer(), ::mxGetData(_argin), len * sizeof(Tango::DevUChar));
            //- now insert the Tango::DevVarUCharArray into the Tango::DeviceAttribute
            value_ << dest;
            //- set dims
            value_.dim_x = len;
            value_.dim_y = 0;
          } break;
          //-- SPECTRUM::DEV_USHORT --------------------------------------
          case Tango::DEV_USHORT: 
          {
            //- check argin type - must be a 1-by-n uint16 array
            if (::mxIsUint16(_argin) == false || ::mxGetM(_argin) != 1) {
              MEX_UTILS->set_error("invalid argin specified",
                                   "1-by-n uint16 array expected",
                                   "DataAdapter::encode_attr");
              return kError;
            }
            //- create a Tango::DevVarUShortArray
            Tango::DevVarUShortArray* dest = new (std::nothrow) Tango::DevVarUShortArray;
            if (dest == 0) {
              MEX_UTILS->set_error("out of memory",
                                   "Tango::DevVarUShortArray allocation failed",
                                   "DataAdapter::encode_attr");
              return kError;
            }
            //- populate the Tango::DevVarUShortArray
            size_t len = ::mxGetN(_argin); 
            dest->length(len);
            ::memcpy(dest->get_buffer(), ::mxGetData(_argin), len * sizeof(Tango::DevUShort));
            //- now insert the Tango::DevVarUShortArray into the Tango::DeviceAttribute
            value_ << dest;
            //- set dims
            value_.dim_x = len;
            value_.dim_y = 0;
          } break;
          //-- SPECTRUM::DEV_SHORT --------------------------------------
          case Tango::DEV_SHORT: 
          {
            //- check argin type - must be a 1-by-n int16 array
            if (::mxIsInt16(_argin) == false || ::mxGetM(_argin) != 1) {
              MEX_UTILS->set_error("invalid argin specified",
                                   "1-by-n int16 array expected",
                                   "DataAdapter::encode_attr");
              return kError;
            }
            //- create a Tango::DevVarShortArray
            Tango::DevVarShortArray* dest = new (std::nothrow) Tango::DevVarShortArray;
            if (dest == 0) {
              MEX_UTILS->set_error("out of memory",
                                   "Tango::DevVarShortArray allocation failed",
                                   "DataAdapter::encode_attr");
              return kError;
            }
            //- populate the Tango::DevVarShortArray
            size_t len = ::mxGetN(_argin); 
            dest->length(len);
            ::memcpy(dest->get_buffer(), ::mxGetData(_argin), len * sizeof(Tango::DevShort));
            //- now insert the Tango::DevVarShortArray into the Tango::DeviceAttribute
            value_ << dest;
            //- set dims
            value_.dim_x = len;
            value_.dim_y = 0;
          } break;
          //-- SPECTRUM::DEV_LONG64 -------------------------------------
          case Tango::DEV_LONG64: 
          {
            //- check argin type - must be a 1-by-n int64 array
            if (::mxIsInt64(_argin) == false || ::mxGetM(_argin) != 1) {
              MEX_UTILS->set_error("invalid argin specified",
                                   "1-by-n int64 array expected",
                                   "DataAdapter::encode_attr");
              return kError;
            }
            //- create a Tango::DevVarLongArray
            Tango::DevVarLong64Array* dest = new (std::nothrow) Tango::DevVarLong64Array;
            if (dest == 0) {
              MEX_UTILS->set_error("out of memory",
                                   "Tango::DevVarLong64Array allocation failed",
                                   "DataAdapter::encode_attr");
              return kError;
            } 
            //- populate the Tango::DevVarLong64Array
            size_t len = ::mxGetN(_argin); 
            dest->length(len);
            ::memcpy(dest->get_buffer(), ::mxGetData(_argin), len * sizeof(Tango::DevLong64));
            //- now insert the Tango::DevVarLong64Array into the Tango::DeviceAttribute
            value_ << dest;
            //- set dims
            value_.dim_x = len;
            value_.dim_y = 0;
          } break;
          //-- SPECTRUM::DEV_ULONG64 -------------------------------------
          case Tango::DEV_ULONG64: 
          {
            //- check argin type - must be a 1-by-n uint64 array
            if (::mxIsUint64(_argin) == false || ::mxGetM(_argin) != 1) {
              MEX_UTILS->set_error("invalid argin specified",
                                   "1-by-n uint64 array expected",
                                   "DataAdapter::encode_attr");
              return kError;
            }
            //- create a Tango::DevVarLongArray
            Tango::DevVarULong64Array* dest = new (std::nothrow) Tango::DevVarULong64Array;
            if (dest == 0) {
              MEX_UTILS->set_error("out of memory",
                                   "Tango::DevVarULong64Array allocation failed",
                                   "DataAdapter::encode_attr");
              return kError;
            } 
            //- populate the Tango::DevVarULong64Array
            size_t len = ::mxGetN(_argin); 
            dest->length(len);
            ::memcpy(dest->get_buffer(), ::mxGetData(_argin), len * sizeof(Tango::DevULong64));
            //- now insert the Tango::DevVarULong64Array into the Tango::DeviceAttribute
            value_ << dest;
            //- set dims
            value_.dim_x = len;
            value_.dim_y = 0;
          } break;
          //-- SPECTRUM::DEV_LONG ---------------------------------------
          case Tango::DEV_LONG: 
          {
            //- check argin type - must be a 1-by-n int32 array
            if (::mxIsInt32(_argin) == false || ::mxGetM(_argin) != 1) {
              MEX_UTILS->set_error("invalid argin specified",
                                   "1-by-n int32 array expected",
                                   "DataAdapter::encode_attr");
              return kError;
            }
            //- create a Tango::DevVarLongArray
            Tango::DevVarLongArray* dest = new (std::nothrow) Tango::DevVarLongArray;
            if (dest == 0) {
              MEX_UTILS->set_error("out of memory",
                                   "Tango::DevVarLongArray allocation failed",
                                   "DataAdapter::encode_attr");
              return kError;
            } 
            //- populate the Tango::DevVarLongArray
            size_t len = ::mxGetN(_argin); 
            dest->length(len);
            ::memcpy(dest->get_buffer(), ::mxGetData(_argin), len * sizeof(Tango::DevLong));
            //- now insert the Tango::DevVarLongArray into the Tango::DeviceAttribute
            value_ << dest;
            //- set dims
            value_.dim_x = len;
            value_.dim_y = 0;
          } break;
          //-- SPECTRUM::DEV_ULONG ---------------------------------------
          case Tango::DEV_ULONG: 
          {
            //- check argin type - must be a 1-by-n uint32 array
            if (::mxIsUint32(_argin) == false || ::mxGetM(_argin) != 1) {
              MEX_UTILS->set_error("invalid argin specified",
                                   "1-by-n uint32 array expected",
                                   "DataAdapter::encode_attr");
              return kError;
            }
            //- create a Tango::DevVarLongArray
            Tango::DevVarULongArray* dest = new (std::nothrow) Tango::DevVarULongArray;
            if (dest == 0) {
              MEX_UTILS->set_error("out of memory",
                                   "Tango::DevVarULongArray allocation failed",
                                   "DataAdapter::encode_attr");
              return kError;
            } 
            //- populate the Tango::DevVarULongArray
            size_t len = ::mxGetN(_argin); 
            dest->length(len);
            ::memcpy(dest->get_buffer(), ::mxGetData(_argin), len * sizeof(Tango::DevULong));
            //- now insert the Tango::DevVarULongArray into the Tango::DeviceAttribute
            value_ << dest;
            //- set dims
            value_.dim_x = len;
            value_.dim_y = 0;
          } break;
          //-- SPECTRUM::DEV_FLOAT -------------------------------------
          case Tango::DEV_FLOAT: 
          {
            //- check argin type - must be a 1-by-n single array
            if (::mxIsSingle(_argin) == false || ::mxGetM(_argin) != 1) {
              MEX_UTILS->set_error("invalid argin specified",
                                   "1-by-n single array expected",
                                   "DataAdapter::encode_attr");
              return kError;
            }
            //- create a Tango::DevVarFloatArray
            Tango::DevVarFloatArray* dest = new (std::nothrow) Tango::DevVarFloatArray;
            if (dest == 0) {
              MEX_UTILS->set_error("out of memory",
                                   "Tango::DevVarFloatArray allocation failed",
                                   "DataAdapter::encode_attr");
              return kError;
            }
            //- populate the Tango::DevVarFloatArray
            size_t len = ::mxGetN(_argin); 
            dest->length(len);
            ::memcpy(dest->get_buffer(), ::mxGetData(_argin), len * sizeof(Tango::DevFloat));
            //- now insert the Tango::DevVarFloatArray into the Tango::DeviceAttribute
            value_ << dest;
            //- set dims
            value_.dim_x = len;
            value_.dim_y = 0;
          } break;
          //-- SPECTRUM::DEV_DOUBLE -------------------------------------
          case Tango::DEV_DOUBLE: 
          {
            //- check argin type - must be a 1-by-n double array
            if (::mxIsDouble(_argin) == false || ::mxGetM(_argin) != 1) {
              MEX_UTILS->set_error("invalid argin specified",
                                   "1-by-n double array expected",
                                   "DataAdapter::encode_attr");
              return kError;
          }
          //- create a Tango::DevVarDoubleArray
          Tango::DevVarDoubleArray* dest = new (std::nothrow) Tango::DevVarDoubleArray;
          if (dest == 0) {
            MEX_UTILS->set_error("out of memory",
                                 "Tango::DevVarDoubleArray allocation failed",
                                 "DataAdapter::encode_attr");
            return kError;
          }
          //- populate the Tango::DevVarDoubleArray
          size_t len = ::mxGetN(_argin); 
          dest->length(len);
          ::memcpy(dest->get_buffer(), ::mxGetData(_argin), len * sizeof(Tango::DevDouble));
          //- now insert the Tango::DevVarDoubleArray into the Tango::DeviceAttribute
          value_ << dest;
          //- set dims
          value_.dim_x = len;
          value_.dim_y = 0;
        } break;
        //-- SPECTRUM::DEV_STRING -------------------------------------
        case Tango::DEV_STRING: 
        {
          //- check argin type - must be a 1-by-n cell array
          if (::mxIsCell(_argin) == false || ::mxGetM(_argin) != 1) {
            MEX_UTILS->set_error("invalid argin specified",
                                 "1-by-n cell array expected",
                                 "DataAdapter::encode_attr");
            return kError;
          }
          //- create a DevVarStringArray
          Tango::DevVarStringArray* dest = new (std::nothrow) Tango::DevVarStringArray;
          if (dest == 0) {
            MEX_UTILS->set_error("out of memory",
                                 "Tango::DevVarStringArray allocation failed",
                                 "DataAdapter::encode_attr");
            return kError;
          }
          //- populate the DevVarLongStringArray
          char* cstr = 0;
          mxArray * cell = 0;
          size_t len = ::mxGetN(_argin); 
          dest->length(len);
          for (size_t i = 0; i < len; i++) {
            //- get <i>th cell of the array
            cell = ::mxGetCell(_argin, i);
            if (cell == 0) {
              MEX_UTILS->set_error("internal error",
                                   "unexpected NULL cell",
                                   "DataAdapter::encode_attr");
              delete dest;
              return kError;
            }
            //- cell should contain a string
            if (::mxIsChar(cell) == false) {
              MEX_UTILS->set_error("internal error",
                                   "unexpected cell content (string expected)",
                                   "DataAdapter::encode_attr");
              delete dest;
              return kError;
            }
            //- get c-string from matlab char array.
            cstr = ::mxArrayToString(cell);
            if (cstr == 0) {
              MEX_UTILS->set_error("internal error", 
                                   "could not extract string from cell", 
                                   "DataAdapter::encode_attr");
              delete dest;
              return kError;
            }
            //- actual storage
            //- there should be a way to use ctsr without duplicate it?
            (*dest)[i] = CORBA::string_dup(cstr);
            //release allocated memory
            ::mxFree(cstr);
          } //- for i ...
          //- now insert the Tango::DevVarStringArray into the Tango::DeviceData
          value_ << dest;
        } break;
      } //switch attr_type
    } break; //- SPECTRUM
    //-- IMAGE ATTRIBUTE --------------------------------------------
    case Tango::IMAGE: 
    { 
      mxArray * transposed_argin = 0; 
      //- transpose matrix (Fortran to C++ storage) 
      {
        mxArray *plhs = 0;
        mxArray *prhs = const_cast<mxArray *>(_argin);
        if (::mexCallMATLAB(1, &plhs, 1, &prhs, "transpose") != 0) {
          MEX_UTILS->set_error("internal error",
                               "could not transpose matrix (image attribute)",
                               "DataAdapter::encode_attr");
        }
        transposed_argin = plhs;
      }
      if (transposed_argin == 0) {
        MEX_UTILS->set_error("internal error",
                             "unexpected null pointer for transposed matrix",
                             "DataAdapter::encode_attr");
        return kError;
      }
      switch (attr_type) 
      {
        //-- IMAGE::DEV_BOOLEAN -----------------------------------------
        case Tango::DEV_BOOLEAN: 
        {
          //- check argin type - must be a m-by-n uint8 array
          if (::mxIsUint8(transposed_argin) == false) {
            MEX_UTILS->set_error("invalid argin specified",
                                 "m-by-n boolean (i.e. uint8) array expected",
                                 "DataAdapter::encode_attr");
            return kError;
          }
          //- create a DevVarBooleanArray
          Tango::DevVarBooleanArray* dest = new (std::nothrow) Tango::DevVarBooleanArray;
          if (dest == 0) {
            MEX_UTILS->set_error("out of memory",
                                 "Tango::DevVarCharArray allocation failed",
                                 "DataAdapter::encode_attr");
            return kError;
          }
          //- populate the Tango::DevVarCharArray
          size_t len = ::mxGetM(transposed_argin) * ::mxGetN(transposed_argin); 
          dest->length(len);
          if (sizeof(bool) == sizeof(unsigned char)) {
            ::memcpy(dest->get_buffer(), ::mxGetData(transposed_argin), len * sizeof(Tango::DevUChar));
          }
          else {
            unsigned char* p = (unsigned char*)::mxGetData(transposed_argin);
            for (size_t i = 0; i < len; i++) {
              (*dest)[i] = p[i] ? true : false;
            }
          }
          //- now insert the Tango::DevVarCharArray into the Tango::DeviceAttribute
          value_ << dest;
          //- set dims
          value_.dim_x = ::mxGetM(transposed_argin);
          value_.dim_y = ::mxGetN(transposed_argin);
        } break;
        //-- IMAGE::DEV_UCHAR -----------------------------------------
        case Tango::DEV_UCHAR: 
        {
          //- check argin type - must be a m-by-n uint8 array
          if (::mxIsUint8(transposed_argin) == false) {
           MEX_UTILS->set_error("invalid argin specified",
                                "m-by-n uint8 array expected",
                                "DataAdapter::encode_attr");
           return kError;
          }
          //- create a DevVarUCharArray
          Tango::DevVarUCharArray* dest = new (std::nothrow) Tango::DevVarUCharArray;
          if (dest == 0) {
           MEX_UTILS->set_error("out of memory",
                                "Tango::DevVarCharArray allocation failed",
                                "DataAdapter::encode_attr");
           return kError;
          }
          //- populate the Tango::DevVarCharArray
          size_t len = ::mxGetM(transposed_argin) * ::mxGetN(transposed_argin); 
          dest->length(len);
          ::memcpy(dest->get_buffer(), ::mxGetData(transposed_argin), len * sizeof(Tango::DevUChar));
          //- now insert the Tango::DevVarCharArray into the Tango::DeviceAttribute
          value_ << dest;
          //- set dims
          value_.dim_x = ::mxGetM(transposed_argin);
          value_.dim_y = ::mxGetN(transposed_argin);
        } break;
        //-- IMAGE::DEV_USHORT -----------------------------------------
        case Tango::DEV_USHORT: 
        {
          //- check argin type - must be a m-by-n uint16 array
          if (::mxIsUint16(transposed_argin) == false) {
            MEX_UTILS->set_error("invalid argin specified",
                                 "m-by-n uint16 array expected",
                                 "DataAdapter::encode_attr");
            return kError;
          }
          //- create a DevVarUShortArray
          Tango::DevVarUShortArray* dest = new (std::nothrow) Tango::DevVarUShortArray;
          if (dest == 0) {
            MEX_UTILS->set_error("out of memory",
                                 "Tango::DevVarUShortArray allocation failed",
                                 "DataAdapter::encode_attr");
            return kError;
          }
          //- populate the DevVarUShortArray
          size_t len = ::mxGetM(transposed_argin) * ::mxGetN(transposed_argin); 
          dest->length(len);
          ::memcpy(dest->get_buffer(), ::mxGetData(transposed_argin), len * sizeof(Tango::DevUShort));
          //- now insert the Tango::DevVarUShortArray into the Tango::DeviceAttribute
          value_ << dest;
          //- set dims
          value_.dim_x = ::mxGetM(transposed_argin);
          value_.dim_y = ::mxGetN(transposed_argin);
        } break;
        //-- IMAGE::DEV_SHORT -----------------------------------------
        case Tango::DEV_SHORT: 
        {
          //- check argin type - must be a m-by-n int16 array
          if (::mxIsInt16(transposed_argin) == false) {
           MEX_UTILS->set_error("invalid argin specified",
                                "m-by-n int16 array expected",
                                "DataAdapter::encode_attr");
           return kError;
          }
          //- create a DevVarShortArray
          Tango::DevVarShortArray* dest = new (std::nothrow) Tango::DevVarShortArray;
          if (dest == 0) {
           MEX_UTILS->set_error("out of memory",
                                "Tango::DevVarShortArray allocation failed",
                                "DataAdapter::encode_attr");
           return kError;
          }
          //- populate the DevVarShortArray
          size_t len = ::mxGetM(transposed_argin) * ::mxGetN(transposed_argin); 
          dest->length(len);
          ::memcpy(dest->get_buffer(), ::mxGetData(transposed_argin), len * sizeof(Tango::DevShort));
          //- now insert the Tango::DevVarShortArray into the Tango::DeviceAttribute
          value_ << dest;
          //- set dims
          value_.dim_x = ::mxGetM(transposed_argin);
          value_.dim_y = ::mxGetN(transposed_argin);
        } break;
        //-- IMAGE::DEV_LONG64 ----------------------------------------
        case Tango::DEV_LONG64: 
        {
          //- check argin type - must be a m-by-n int64 array
          if (::mxIsInt64(transposed_argin) == false) {
            MEX_UTILS->set_error("invalid argin specified",
                                 "m-by-n int64 array expected",
                                 "DataAdapter::encode_attr");
            return kError;
          }
          //- create a DevVarLong64Array
          Tango::DevVarLong64Array* dest = new (std::nothrow) Tango::DevVarLong64Array;
          if (dest == 0) {
            MEX_UTILS->set_error("out of memory",
                                 "Tango::DevVarLong64Array allocation failed",
                                 "DataAdapter::encode_attr");
            return kError;
          }
          //- populate the DevVarLong64Array
          size_t len = ::mxGetM(transposed_argin) * ::mxGetN(transposed_argin); 
          dest->length(len);
          ::memcpy(dest->get_buffer(), ::mxGetData(transposed_argin), len * sizeof(Tango::DevLong64));
          //- now insert the Tango::DevVarLongArray into the Tango::DeviceAttribute
          value_ << dest;
          //- set dims
          value_.dim_x = ::mxGetM(transposed_argin);
          value_.dim_y = ::mxGetN(transposed_argin);
        } break;
        //-- IMAGE::DEV_ULONG64 ----------------------------------------
        case Tango::DEV_ULONG64: 
        {
          //- check argin type - must be a m-by-n uint64 array
          if (::mxIsUint64(transposed_argin) == false) {
            MEX_UTILS->set_error("invalid argin specified",
                                 "m-by-n uint64 array expected",
                                 "DataAdapter::encode_attr");
            return kError;
          }
          //- create a DevVarULong64Array
          Tango::DevVarULong64Array* dest = new (std::nothrow) Tango::DevVarULong64Array;
          if (dest == 0) {
            MEX_UTILS->set_error("out of memory",
                                 "Tango::DevVarULong64Array allocation failed",
                                 "DataAdapter::encode_attr");
            return kError;
          }
          //- populate the DevVarULong64Array
          size_t len = ::mxGetM(transposed_argin) * ::mxGetN(transposed_argin); 
          dest->length(len);
          ::memcpy(dest->get_buffer(), ::mxGetData(transposed_argin), len * sizeof(Tango::DevULong64));
          //- now insert the Tango::DevVarULong64Array into the Tango::DeviceAttribute
          value_ << dest;
          //- set dims
          value_.dim_x = ::mxGetM(transposed_argin);
          value_.dim_y = ::mxGetN(transposed_argin);
        } break;
        //-- IMAGE::DEV_LONG ------------------------------------------
        case Tango::DEV_LONG: 
        {
          //- check argin type - must be a m-by-n int32 array
          if (::mxIsInt32(transposed_argin) == false) {
           MEX_UTILS->set_error("invalid argin specified",
                                "m-by-n int32 array expected",
                                "DataAdapter::encode_attr");
           return kError;
          }
          //- create a DevVarLongArray
          Tango::DevVarLongArray* dest = new (std::nothrow) Tango::DevVarLongArray;
          if (dest == 0) {
           MEX_UTILS->set_error("out of memory",
                                "Tango::DevVarLongArray allocation failed",
                                "DataAdapter::encode_attr");
           return kError;
          }
          //- populate the DevVarLongArray
          size_t len = ::mxGetM(transposed_argin) * ::mxGetN(transposed_argin); 
          dest->length(len);
          ::memcpy(dest->get_buffer(), ::mxGetData(transposed_argin), len * sizeof(Tango::DevLong));
          //- now insert the Tango::DevVarLongArray into the Tango::DeviceAttribute
          value_ << dest;
          //- set dims
          value_.dim_x = ::mxGetM(transposed_argin);
          value_.dim_y = ::mxGetN(transposed_argin);
        } break;
        //-- IMAGE::DEV_ULONG ------------------------------------------
        case Tango::DEV_ULONG: 
        {
          //- check argin type - must be a m-by-n uint32 array
          if (::mxIsUint32(transposed_argin) == false) {
            MEX_UTILS->set_error("invalid argin specified",
                                 "m-by-n uint32 array expected",
                                 "DataAdapter::encode_attr");
            return kError;
          }
          //- create a DevVarULongArray
          Tango::DevVarULongArray* dest = new (std::nothrow) Tango::DevVarULongArray;
          if (dest == 0) {
           MEX_UTILS->set_error("out of memory",
                                "Tango::DevVarULongArray allocation failed",
                                "DataAdapter::encode_attr");
           return kError;
          }
          //- populate the DevVarULongArray
          size_t len = ::mxGetM(transposed_argin) * ::mxGetN(transposed_argin); 
          dest->length(len);
          ::memcpy(dest->get_buffer(), ::mxGetData(transposed_argin), len * sizeof(Tango::DevULong));
          //- now insert the Tango::DevVarULongArray into the Tango::DeviceAttribute
          value_ << dest;
          //- set dims
          value_.dim_x = ::mxGetM(transposed_argin);
          value_.dim_y = ::mxGetN(transposed_argin);
        } break;
        //-- IMAGE::DEV_FLOAT -------------------------------------
        case Tango::DEV_FLOAT: 
        {
          //- check argin type - must be a 1-by-n single array
          if (::mxIsSingle(transposed_argin) == false) {
           MEX_UTILS->set_error("invalid argin specified",
                                "m-by-n single array expected",
                                "DataAdapter::encode_attr");
           return kError;
          }
          //- create a DevVarFloatArray
          Tango::DevVarFloatArray* dest = new (std::nothrow) Tango::DevVarFloatArray;
          if (dest == 0) {
           MEX_UTILS->set_error("out of memory",
                                "Tango::DevVarFloatArray allocation failed",
                                "DataAdapter::encode_attr");
           return kError;
          }
          //- populate the DevVarFloatArray
          size_t len = ::mxGetM(transposed_argin) * ::mxGetN(transposed_argin); 
          dest->length(len);
          ::memcpy(dest->get_buffer(), ::mxGetData(transposed_argin), len * sizeof(Tango::DevFloat));
          //- now insert the Tango::DevVarFloatArray into the Tango::DeviceAttribute
          value_ << dest;
          //- set dims
          value_.dim_x = ::mxGetM(transposed_argin);
          value_.dim_y = ::mxGetN(transposed_argin);
        } break;
        //-- IMAGE::DEV_DOUBLE -------------------------------------
        case Tango::DEV_DOUBLE: 
        {
          //- check argin type - must be a 1-by-n double array
          if (::mxIsDouble(transposed_argin) == false) {
            MEX_UTILS->set_error("invalid argin specified",
                                 "m-by-n double array expected",
                                 "DataAdapter::encode_attr");
            return kError;
          }
          //- create a DevVarDoubleArray
          Tango::DevVarDoubleArray* dest = new (std::nothrow) Tango::DevVarDoubleArray;
          if (dest == 0) {
            MEX_UTILS->set_error("out of memory",
                                 "Tango::DevVarDoubleArray allocation failed",
                                 "DataAdapter::encode_attr");
            return kError;
          }
          //- populate the DevVarDoubleArray
          size_t len = ::mxGetM(transposed_argin) * ::mxGetN(transposed_argin); 
          dest->length(len);
          ::memcpy(dest->get_buffer(), ::mxGetData(transposed_argin), len * sizeof(Tango::DevDouble));
          //- now insert the Tango::DevVarDoubleArray into the Tango::DeviceAttribute
          value_ << dest;
          //- set dims
          value_.dim_x = ::mxGetM(transposed_argin);
          value_.dim_y = ::mxGetN(transposed_argin);
        } break;
      } //- switch attr_type 
    } break; //- IMAGE
    //-- UNKNOWN ATTRIBUTE ------------------------------------------
    default:
    {
      std::string d = "unsupported TANGO data type for attribute <" + attr_list[_attr_id].name + ">";
      MEX_UTILS->set_error("unexpected TANGO data type",
                           d.c_str(),
                           "DataAdapter::encode_argin");                   
      } break;
    } //- switch::attr_format
  } //try
  catch (const Tango::DevFailed &e) {
    MEX_UTILS->set_error(e);
    return kError;
  }
  catch (...) {
    MEX_UTILS->set_error("unknown error",
                         "unknown exception caught",
                         "DataAdapter::encode_attr");
    return kError;
  }
  return kNoError;
}
                         
//=============================================================================
//- DataAdapter::decode_attr
//=============================================================================
int DataAdapter::decode_attr (DeviceDesc* _ddesc, 
                             int _attr_id,
                             Tango::DeviceAttribute& _value,
                             mxArray *& value_)
{
 //- check input
 if (_ddesc == 0) 
 {
   MEX_UTILS->set_error("internal error",
                        "unexpected NULL argument",
                        "DataAdapter::decode_attr");
   return kError;
 }  
 
 //- check input
 if (_attr_id < 0) 
 {
   MEX_UTILS->set_error("internal error",
                        "unexpected attribute identifier",
                        "DataAdapter::decode_attr");
   return kError;
 }
 
 //- get attr list
 const DeviceDesc::MinAttrInfoList &attr_list = _ddesc->attr_list();
 //- get attr access
 Tango::AttrWriteType attr_write_type = attr_list[_attr_id].writable;
 //- get attr format
 int attr_format = attr_list[_attr_id].data_format;
 //- get attr data type
 int attr_type = attr_list[_attr_id].data_type;
 
 //- extract data
 try 
 {
   //- be sure mxArray is set to NULL in case of error
   value_ = 0;
   //- output mxArray dims
   mwSize dims[2] = {1, 1};
   switch (attr_format) 
   {      
     case Tango::SCALAR: 
       dims[1] = attr_write_type != Tango::READ ? 2 : 1;
       break;
     case Tango::SPECTRUM:
       dims[1] = (mwSize)_value.get_nb_read();
       break;
     case Tango::IMAGE:
     {
       Tango::AttributeDimension r_dim = _value.get_r_dimension();
       dims[0] = r_dim.dim_x;
       dims[1] = r_dim.dim_y;
     } break;
   }
   switch (attr_type) 
   {
     //-- SCALAR::DEV_STRING ----------------------------
     case Tango::DEV_STRING: 
     {
       switch (attr_format) 
       { 
         case Tango::SCALAR:
         {
           std::string str;
           _value >> str;
           value_ = ::mxCreateString(str.c_str());
           if (value_ == 0) 
           {
             MEX_UTILS->set_error("out of memory",
                                  "mxCreateString failed",
                                  "DataAdapter::decode_attr");
             return kError;
           }
         } break;
         case Tango::SPECTRUM:
         {
           Tango::DevVarStringArray* string_array = 0;
           _value >> string_array;
           if (string_array == 0) {
             MEX_UTILS->set_error("internal error",
                                  "could not extract DevVarStringArray from attribute value",
                                  "DataAdapter::decode_attr");
             return kError;
           }
           //- create a 1-by-n cell array
           size_t len = string_array->length();
           const mwSize dims[2] = {1, len};
           value_  = ::mxCreateCellArray(2, dims);
           if (value_ == 0) {
             MEX_UTILS->set_error("out of memory",
                                  "mxCreateCellArray failed",
                                  "DataAdapter::decode_attr");
             return kError;
           }
           //- copy from src to dest
           for (size_t i = 0; i < len; i++) {
             ::mxSetCell(value_, i, ::mxCreateString((*string_array)[i]));     
           }
           //- avoid memory leak
           delete string_array;
         } break;
         case Tango::IMAGE:
         {
           MEX_UTILS->set_error("unsupported feature",
                                "the cuurent MATLAB binding implementation does not support <image of strings> attributes",
                                "DataAdapter::decode_argout");
           return kError;
         } break;
       }
     } break;
     //-- SCALAR::DEV_STATE
     case Tango::DEV_STATE:  
     {
       Tango::DevState state;
       if ((_value >> state) == false) {
         MEX_UTILS->set_error("internal error",
                              "data extraction failed",
                              "DataAdapter::decode_attr");
       }
       dims[1] = 1;
       value_ = ::mxCreateNumericArray(2, dims, ::mxUINT8_CLASS, ::mxREAL);
       if (value_ == 0) {
         MEX_UTILS->set_error("out of memory",
                              "mxCreateNumericArray failed",
                              "DataAdapter::decode_attr");
         return kError;
       }
       unsigned char * p = (unsigned char*)::mxGetData(value_);
       p[0] = static_cast<unsigned char>(state);
     } break;
     //-- SCALAR/SPECTRUM/IMAGE::DEV_BOOLEAN --------------
     case Tango::DEV_BOOLEAN: 
     {
       Tango::DevVarBooleanArray *bool_array = 0;
       if ((_value >> bool_array) == false) {
         MEX_UTILS->set_error("internal error",
                              "data extraction failed",
                              "DataAdapter::decode_attr");
       }
       value_ = ::mxCreateNumericArray(2, dims, ::mxUINT8_CLASS, ::mxREAL);
       if (value_ == 0) {
         delete bool_array;
         MEX_UTILS->set_error("out of memory",
                              "mxCreateNumericArray failed",
                              "DataAdapter::decode_attr");
         return kError;
       }
       if (sizeof(Tango::DevBoolean) == sizeof(unsigned char)) {
         ::memcpy(::mxGetData(value_), bool_array->get_buffer(), dims[0] * dims[1] * sizeof(Tango::DevBoolean)); 
       }
       else {
         size_t max = dims[0] * dims[1];
         unsigned char * p = (unsigned char*)::mxGetData(value_);
         for (size_t i = 0; i < max; i++) {
           p[i] = (*bool_array)[i] ? 1 : 0;
         }
       }
       delete bool_array;
     } break;
     //-- SCALAR/SPECTRUM/IMAGE::DEV_UCHAR --------------
     case Tango::DEV_UCHAR: 
     {
       Tango::DevVarCharArray *uchar_array = 0;
       if ((_value >> uchar_array) == false) {
         MEX_UTILS->set_error("internal error",
                              "data extraction failed",
                              "DataAdapter::decode_attr");
       }
       value_ = ::mxCreateNumericArray(2, dims, ::mxUINT8_CLASS, ::mxREAL);
       if (value_ == 0) {
         delete uchar_array;
         MEX_UTILS->set_error("out of memory",
                              "mxCreateNumericArray failed",
                              "DataAdapter::decode_attr");
         return kError;
       }
       ::memcpy(::mxGetData(value_), uchar_array->get_buffer(), dims[0] * dims[1] * sizeof(Tango::DevUChar)); 
       delete uchar_array;
     } break;
     //-- SCALAR/SPECTRUM/IMAGE::DEV_USHORT --------------
     case Tango::DEV_USHORT: 
     {
       Tango::DevVarUShortArray *ushort_array = 0;
       if ((_value >> ushort_array) == false) {
         MEX_UTILS->set_error("internal error",
                              "data extraction failed",
                              "DataAdapter::decode_attr");
       }
       value_ = ::mxCreateNumericArray(2, dims, ::mxUINT16_CLASS, ::mxREAL);
       if (value_ == 0) {
         delete ushort_array;
         MEX_UTILS->set_error("out of memory",
                              "mxCreateNumericArray failed",
                              "DataAdapter::decode_attr");
         return kError;
       }
       ::memcpy(::mxGetData(value_), ushort_array->get_buffer(), dims[0] * dims[1] * sizeof(Tango::DevUShort)); 
       delete ushort_array;
     } break;
     //-- SCALAR/SPECTRUM/IMAGE::DEV_SHORT --------------
     case Tango::DEV_SHORT: 
     {
       Tango::DevVarShortArray *short_array = 0;
       if ((_value >> short_array) == false) {
         MEX_UTILS->set_error("internal error",
                              "data extraction failed",
                              "DataAdapter::decode_attr");
       }
       value_ = ::mxCreateNumericArray(2, dims, ::mxINT16_CLASS, ::mxREAL);
       if (value_ == 0) {
         delete short_array;
         MEX_UTILS->set_error("out of memory",
                              "mxCreateNumericArray failed",
                              "DataAdapter::decode_attr");
         return kError;
       }
       ::memcpy(::mxGetData(value_), short_array->get_buffer(), dims[0] * dims[1] * sizeof(Tango::DevShort)); 
       delete short_array;
     } break;
     //-- SCALAR/SPECTRUM/IMAGE::DEV_LONG64  ------------
     case Tango::DEV_LONG64: 
     {
       Tango::DevVarLong64Array *long_long_array = 0;
       if ((_value >> long_long_array) == false) {
         MEX_UTILS->set_error("internal error",
                              "data extraction failed",
                              "DataAdapter::decode_attr");
       }
       value_ = ::mxCreateNumericArray(2, dims, ::mxINT64_CLASS, ::mxREAL);
       if (value_ == 0) {
         delete long_long_array;
         MEX_UTILS->set_error("out of memory",
                              "mxCreateNumericArray failed",
                              "DataAdapter::decode_attr");
         return kError;
       }
       ::memcpy(::mxGetData(value_), long_long_array->get_buffer(), dims[0] * dims[1] * sizeof(Tango::DevLong64));
       delete long_long_array;
     } break;
     //-- SCALAR/SPECTRUM/IMAGE::DEV_LONG64  ------------
     case Tango::DEV_ULONG64: 
     {
       Tango::DevVarULong64Array *ulong_long_array = 0;
       if ((_value >> ulong_long_array) == false) {
         MEX_UTILS->set_error("internal error",
                              "data extraction failed",
                              "DataAdapter::decode_attr");
       }
       value_ = ::mxCreateNumericArray(2, dims, ::mxUINT64_CLASS, ::mxREAL);
       if (value_ == 0) {
         delete ulong_long_array;
         MEX_UTILS->set_error("out of memory",
                              "mxCreateNumericArray failed",
                              "DataAdapter::decode_attr");
         return kError;
       }
       ::memcpy(::mxGetData(value_), ulong_long_array->get_buffer(), dims[0] * dims[1] * sizeof(Tango::DevULong64));
       delete ulong_long_array;
     } break;
     //-- SCALAR/SPECTRUM/IMAGE::DEV_LONG  --------------
     case Tango::DEV_LONG: 
     {
       Tango::DevVarLongArray *long_array = 0;
       if ((_value >> long_array) == false) {
         MEX_UTILS->set_error("internal error",
                              "data extraction failed",
                              "DataAdapter::decode_attr");
       }
       value_ = ::mxCreateNumericArray(2, dims, ::mxINT32_CLASS, ::mxREAL);
       if (value_ == 0) {
         delete long_array;
         MEX_UTILS->set_error("out of memory",
                              "mxCreateNumericArray failed",
                              "DataAdapter::decode_attr");
         return kError;
       }
       ::memcpy(::mxGetData(value_), long_array->get_buffer(), dims[0] * dims[1] * sizeof(Tango::DevLong));
       delete long_array;
     } break;
     //-- SCALAR/SPECTRUM/IMAGE::DEV_ULONG  --------------
     case Tango::DEV_ULONG: 
     {
       Tango::DevVarULongArray *ulong_array = 0;
       if ((_value >> ulong_array) == false) {
         MEX_UTILS->set_error("internal error",
                              "data extraction failed",
                              "DataAdapter::decode_attr");
       }
       value_ = ::mxCreateNumericArray(2, dims, ::mxUINT32_CLASS, ::mxREAL);
       if (value_ == 0) {
         delete ulong_array;
         MEX_UTILS->set_error("out of memory",
                              "mxCreateNumericArray failed",
                              "DataAdapter::decode_attr");
         return kError;
       }
       ::memcpy(::mxGetData(value_), ulong_array->get_buffer(), dims[0] * dims[1] * sizeof(Tango::DevULong));
       delete ulong_array;
     } break;
     //-- SCALAR/SPECTRUM/IMAGE::DEV_FLOAT ---------------
     case Tango::DEV_FLOAT: 
     {
       Tango::DevVarFloatArray *float_array = 0;
       if ((_value >> float_array) == 0) {
         MEX_UTILS->set_error("internal error",
                              "data extraction failed",
                              "DataAdapter::decode_attr");
       }
       value_ = ::mxCreateNumericArray(2, dims, ::mxSINGLE_CLASS, ::mxREAL);
       if (value_== 0) {
         delete float_array;
         MEX_UTILS->set_error("out of memory",
                              "mxCreateNumericArray failed",
                              "DataAdapter::decode_attr");
         return kError;
       }
       ::memcpy(::mxGetData(value_), float_array->get_buffer(), dims[0] * dims[1] * sizeof(Tango::DevFloat));
       delete float_array;
     } break;
     //-- SCALAR/SPECTRUM/IMAGE::DEV_DOUBLE --------------
     case Tango::DEV_DOUBLE: 
     {
       Tango::DevVarDoubleArray *double_array = 0;
       if ((_value >> double_array) == 0) {
         MEX_UTILS->set_error("internal error",
                              "data extraction failed",
                              "DataAdapter::decode_attr");
       }
       value_ = ::mxCreateNumericArray(2, dims, ::mxDOUBLE_CLASS, ::mxREAL);
       if (value_== 0) {
         delete double_array;
         MEX_UTILS->set_error("out of memory",
                              "mxCreateNumericArray failed",
                              "DataAdapter::decode_attr");
         return kError;
       }
       ::memcpy(::mxGetData(value_), double_array->get_buffer(), dims[0] * dims[1] * sizeof(Tango::DevDouble));
       delete double_array;
     } break;
     //-- UNKNOWN TYPE -----------------------------------
     default: 
     {
       MEX_UTILS->set_error("unexpected TANGO data type",
                            "unsupported attribute data type",
                            "DataAdapter::decode_attr");
       return kError;
     } break;
   } //- switch attr_type
   //- transpose matrix (C++ to Fortran storage) 
   if (attr_format == Tango::IMAGE) {
     mxArray *plhs = 0;
     mxArray *prhs = value_;
     if (::mexCallMATLAB(1, &plhs, 1, &prhs, "transpose") != 0) {
       MEX_UTILS->set_error("internal error",
                            "could not transpose matrix (image attribute)",
                            "DataAdapter::decode_attr");
     }
     ::mxDestroyArray(prhs);
     value_ = plhs;
   }
 } //- try
 catch (const Tango::DevFailed &e) {
   MEX_UTILS->set_error(e);
   return kError;
 }
 catch (...) {
   MEX_UTILS->set_error("unknown error",
                        "unknown exception caught",
                        "DataAdapter::decode_attr");
   return kError;
 }
 return kNoError;
}
                         
//=============================================================================
//- DataAdapter::mxarray_to_vector_of_string
//============================================================================= 
std::vector<std::string> * DataAdapter::mxarray_to_vector_of_string (mxArray * mx_array) 
{
 //- be sure mx_array is a cell array of string
 if (MEX_UTILS->is_array_of_string(mx_array) == false) 
 {
   MEX_UTILS->set_error("invalid mxArray specified",
                        "cell array of string expected",
                        "DataAdapter::mxarray_to_vector_of_string");
   SET_DEFAULT_PRHS_THEN_RETURN(0); 
 }
 //- # elements in mx_array
 Tango::DevLong n = ::mxGetN(mx_array);
 //- allocate the returned vector
 std::vector<std::string>* v = new (std::nothrow) std::vector<std::string>(n);
 if (v == 0 || (*v).size() != n) {
   MEX_UTILS->set_error("out of memory",
                        "std::vector allocation failed",
                        "DataAdapter::mxarray_to_vector");
   SET_DEFAULT_PRHS_THEN_RETURN(0); 
 }
 //- populate v
 char* s = 0;
 mxArray * tmp = 0; 
 for (Tango::DevLong c = 0; c < n; c++) {
   tmp = ::mxGetCell(mx_array, c);
   if (tmp == 0) {
     delete v;
     MEX_UTILS->set_error("invalid mxArray specified",
                          "unexpected empty cell",
                          "DataAdapter::mxarray_to_vector");
     SET_DEFAULT_PRHS_THEN_RETURN(0); 
   }
   s = ::mxArrayToString(tmp);
   ((*v)[c]).assign(s);
   ::mxFree(s);
 }
 return v;
}
                         
//=============================================================================
//- DataAdapter::mxarray_to_vector_of_vector_of_string
//============================================================================= 
std::vector<std::vector<std::string> >* 
DataAdapter::mxarray_to_vector_of_vector_of_string (mxArray * mx_array)
{
 if (MEX_UTILS->is_array_of_array_of_string(mx_array) == false) {
   MEX_UTILS->set_error("invalid mxArray specified",
                        "expected cell array of cell array of string",
                        "DataAdapter::mxarray_to_vector_of_vector_of_string");
   SET_DEFAULT_PRHS_THEN_RETURN(0); 
 }
 //- # elements in mx_array
 Tango::DevLong n = ::mxGetN(mx_array);
 //- allocate the returned vector
 std::vector<vector<std::string> >* v = new (std::nothrow) std::vector<vector<std::string> >(n);
 if (v == 0 || (*v).size() != n) {
   MEX_UTILS->set_error("out of memory",
                        "std::vector allocation failed",
                        "DataAdapter::mxarray_to_vector_of_vector_of_string");
   SET_DEFAULT_PRHS_THEN_RETURN(0); 
 }
 //- mx_array is an array of array
 char* s = 0;
 Tango::DevLong tmp_n = 0;
 mxArray * tmp = 0;
 for (Tango::DevLong i = 0; i < n; i++) {
   //- for each array in the array
   tmp = ::mxGetCell(mx_array, i);
   if (tmp == 0) {
     delete v;
     MEX_UTILS->set_error("invalid mxArray specified",
                          "unexpected empty cell",
                          "DataAdapter::mxarray_to_vector_of_vector_of_string");
     SET_DEFAULT_PRHS_THEN_RETURN(0); 
   }
   //- get # elements of tmp
   tmp_n = ::mxGetN(tmp);
   //- populate the i-th vector of v 
   for (Tango::DevLong j = 0; j < tmp_n; j++) {
     s = ::mxArrayToString(::mxGetCell(tmp, j));
     if (s == 0) {
       delete v;
       MEX_UTILS->set_error("invalid mxArray specified",
                            "expected cell array of cell array of string",
                            "DataAdapter::mxarray_to_vector_of_vector_of_string");
       SET_DEFAULT_PRHS_THEN_RETURN(0); 
     }
     (*v)[i].push_back(std::string(s));
     ::mxFree(s);
   }
 }
 return v;
}
                         
//=============================================================================
//- DataAdapter::mxarray_to_vector_dvlsa
//============================================================================= 
std::vector<Tango::DevVarLongStringArray*>* 
DataAdapter::mxarray_to_vector_of_dvlsa (mxArray * mx_array)
{
 if (MEX_UTILS->is_array_of_struct(mx_array) == false) {
   MEX_UTILS->set_error("invalid mxArray specified",
                        "expected cell array of struct",
                        "DataAdapter::mxarray_to_vector_of_dvlsa");
   SET_DEFAULT_PRHS_THEN_RETURN(0); 
 }
 //- # elements in mx_array
 size_t n = ::mxGetN(mx_array);
 //- allocate the returned vector
 std::vector<Tango::DevVarLongStringArray*>* v 
 = new (std::nothrow) std::vector<Tango::DevVarLongStringArray*>(n, 0);
 if (v == 0 || (*v).size() != n) {
   MEX_UTILS->set_error("out of memory",
                        "std::vector allocation failed",
                        "DataAdapter::mxarray_to_vector_of_dvlsa");
   SET_DEFAULT_PRHS_THEN_RETURN(0); 
 }
 //- for each cell in the array
 size_t i, j;
 char *cstr = 0;
 Tango::DevVarLongStringArray *dest = 0;
 mxArray *tmp = 0, *lvalue = 0, *svalue = 0, *cell = 0;
 for (i = 0; i < n; i++) {
   tmp = ::mxGetCell(mx_array, i);
   if (tmp == 0) {
     MEX_UTILS->set_error("invalid mxArray specified",
                          "unexpected empty cell",
                          "DataAdapter::mxarray_to_vector_of_dvlsa");
     goto _error;
   }
   //- the struct should contains a <lvalue> field containing a 1-by-n int32 array
   lvalue = ::mxGetField(tmp, 0, "lvalue");
   if (lvalue == 0) {
     MEX_UTILS->set_error("invalid argin specified",
                          "struct should contain a field named 'lvalue'",
                          "DataAdapter::mxarray_to_vector_of_dvlsa");
     goto _error;
   }
   if (::mxIsInt32(lvalue) == false || ::mxGetM(lvalue) != 1) {
     MEX_UTILS->set_error("invalid argin specified",
                          "1-by-n int32 array expected for field 'lvalue'",
                          "DataAdapter::mxarray_to_vector_of_dvlsa");
     goto _error;
   }
   //- the struct should contains a <svalue> field containing a 1-by-n cell array
   svalue = ::mxGetField(tmp, 0, "svalue");
   if (svalue == 0) {
     MEX_UTILS->set_error("invalid argin specified",
                          "struct should contain a field named 'svalue'",
                          "DataAdapter::mxarray_to_vector_of_dvlsa");
     goto _error;
   }
   if (::mxIsCell(svalue) == false || ::mxGetM(svalue) != 1) {
     MEX_UTILS->set_error("invalid argin specified",
                          "1-by-n cell array expected for field 'svalue'",
                          "DataAdapter::mxarray_to_vector_of_dvlsa");
     goto _error;
   }
   //- create a DevVarLongStringArray
   dest = new (std::nothrow) Tango::DevVarLongStringArray;
   if (dest == 0) {
     MEX_UTILS->set_error("out of memory",
                          "Tango::DevVarLongStringArray allocation failed",
                          "DataAdapter::mxarray_to_vector_of_dvlsa");
     goto _error;
   }
   //- store lvalue into the numeric part of the DevVarLongStringArray  
   size_t len = ::mxGetN(lvalue); 
   dest->lvalue.length(len);
   ::memcpy(dest->lvalue.get_buffer(), ::mxGetData(lvalue), len * sizeof(Tango::DevLong));
   //- store svalue into the string part of the DevVarLongStringArray
   len = ::mxGetN(svalue); 
   dest->svalue.length(len);
   for (j = 0; j < len; j++) {
     //- get <i>th cell of the array
     cell = ::mxGetCell(svalue, j);
     if (cell == 0) {
       MEX_UTILS->set_error("internal error",
                            "unexpected empty cell",
                            "DataAdapter::mxarray_to_vector_of_dvlsa");
       goto _error;
     }
     //- cell should contain a string
     if (::mxIsChar(cell) == false) {
       MEX_UTILS->set_error("internal error",
                            "unexpected cell content (string expected)",
                            "DataAdapter::mxarray_to_vector_of_dvlsa");
       goto _error;
     }
     //- get c-string from matlab char array.
     cstr = ::mxArrayToString(cell);
     if (cstr == 0) {
       MEX_UTILS->set_error("internal error", 
                            "could not extract string from cell", 
                            "DataAdapter::mxarray_to_vector_of_dvlsa");
       goto _error;
     }
     //- actual storage
     dest->svalue[j] = CORBA::string_dup(cstr);
     //release allocated memory
     ::mxFree(cstr);
   } //- for j ...
     //- now insert the Tango::DevVarLongStringArray into the vector
   (*v)[i] = dest;
 }
 return v;
_error:
 for (i = 0; i < n; i++) {
   if ((*v)[i] != 0) delete (*v)[i];
 }
 delete v;
 return 0;
}
                         
//=============================================================================
//- DataAdapter::mxarray_to_vector_dvdsa
//============================================================================= 
std::vector<Tango::DevVarDoubleStringArray*>* 
DataAdapter::mxarray_to_vector_of_dvdsa (mxArray * mx_array)
{
 if (MEX_UTILS->is_array_of_struct(mx_array) == false) {
   MEX_UTILS->set_error("invalid mxArray specified",
                        "expected cell array of struct",
                        "DataAdapter::mxarray_to_vector_of_dvdsa");
   SET_DEFAULT_PRHS_THEN_RETURN(0); 
 }
 //- # elements in mx_array
 size_t n = ::mxGetN(mx_array);
 //- allocate the returned vector
 std::vector<Tango::DevVarDoubleStringArray*>* v 
 = new (std::nothrow) std::vector<Tango::DevVarDoubleStringArray*>(n, 0);
 if (v == 0 || (*v).size() != n) {
   MEX_UTILS->set_error("out of memory",
                        "std::vector allocation failed",
                        "DataAdapter::mxarray_to_vector_of_dvdsa");
   SET_DEFAULT_PRHS_THEN_RETURN(0); 
 }
 //- for each cell in the array
 size_t i, j;
 char* cstr = 0;
 Tango::DevVarDoubleStringArray* dest = 0;
 mxArray *tmp = 0, *dvalue = 0, *svalue = 0, *cell = 0;
 for (i = 0; i < n; i++) {
   tmp = ::mxGetCell(mx_array, i);
   if (tmp == 0) {
     MEX_UTILS->set_error("invalid mxArray specified",
                          "unexpected empty cell",
                          "DataAdapter::mxarray_to_vector_of_dvdsa");
     goto _error;
   }
   //- the struct should contains a <dvalue> field containing a 1-by-n double array
   dvalue = ::mxGetField(tmp, 0, "dvalue");
   if (dvalue == 0) {
     MEX_UTILS->set_error("invalid argin specified",
                          "struct should contain a field named 'dvalue'",
                          "DataAdapter::mxarray_to_vector_of_dvdsa");
     goto _error;
   }
   if (::mxIsDouble(dvalue) == false || ::mxGetM(dvalue) != 1) {
     MEX_UTILS->set_error("invalid argin specified",
                          "1-by-n double array expected for field 'dvalue'",
                          "DataAdapter::mxarray_to_vector_of_dvdsa");
     goto _error;
   }
   //- the struct should contains a <svalue> field containing a 1-by-n cell array
   svalue = ::mxGetField(tmp, 0, "svalue");
   if (svalue == 0) {
     MEX_UTILS->set_error("invalid argin specified",
                          "struct should contain a field named 'svalue'",
                          "DataAdapter::mxarray_to_vector_of_dvdsa");
     goto _error;
   }
   if (::mxIsCell(svalue) == false || ::mxGetM(svalue) != 1) {
     MEX_UTILS->set_error("invalid argin specified",
                          "1-by-n cell array expected for field 'svalue'",
                          "DataAdapter::mxarray_to_vector_of_dvdsa");
     goto _error;
   }
   //- create a DevVarLongStringArray
   dest = new (std::nothrow) Tango::DevVarDoubleStringArray;
   if (dest == 0) {
     MEX_UTILS->set_error("out of memory",
                          "Tango::DevVarLongStringArray allocation failed",
                          "DataAdapter::mxarray_to_vector_of_dvdsa");
     goto _error;
   }
   //- store lvalue into the numeric part of the DevVarLongStringArray  
   size_t len = ::mxGetN(dvalue); 
   dest->dvalue.length(len);
   ::memcpy(dest->dvalue.get_buffer(), ::mxGetData(dvalue), len * sizeof(Tango::DevDouble));
   //- store svalue into the string part of the DevVarLongStringArray
   len = ::mxGetN(svalue); 
   dest->svalue.length(len);
   for (j = 0; j < len; j++) {
     //- get <i>th cell of the array
     cell = ::mxGetCell(svalue, j);
     if (cell == 0) {
       MEX_UTILS->set_error("internal error",
                            "unexpected empty cell",
                            "DataAdapter::mxarray_to_vector_of_dvdsa");
       goto _error;
     }
     //- cell should contain a string
     if (::mxIsChar(cell) == false) {
       MEX_UTILS->set_error("internal error",
                            "unexpected cell content (string expected)",
                            "DataAdapter::mxarray_to_vector_of_dvdsa");
       goto _error;
     }
     //- get c-string from matlab char array.
     cstr = ::mxArrayToString(cell);
     if (cstr == 0) {
       MEX_UTILS->set_error("internal error", 
                            "could not extract string from cell", 
                            "DataAdapter::mxarray_to_vector_of_dvdsa");
       goto _error;
     }
     //- actual storage
     dest->svalue[j] = CORBA::string_dup(cstr);
     //release allocated memory
     ::mxFree(cstr);
   } //- for j ...
     //- now insert the Tango::DevVarDoubleStringArray into the vector
   (*v)[i] = dest;
 }
 return v;
_error:
 for (i = 0; i < n; i++) {
   if ((*v)[i] != 0) delete (*v)[i];
 }
 delete v;
 return 0;
}
