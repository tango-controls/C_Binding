// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for LabVIEW
//
// = FILENAME
//   DataAdapter.cpp
//
// = AUTHOR
//   Nicolas Leclercq - SOLEIL
//
// ============================================================================

//=============================================================================
// DEPENDENCIES
//=============================================================================
#ifndef WIN32
#  include <sys/time.h> 
#endif 
#include "StandardHeader.h"
#include "DeviceRepository.h"
#include "Endianness.h"
#include "DataAdapter.h"
#include "ErrorStack.h"

#if !defined (_LV_INLINE_)
  #include "DataAdapter.i"
#endif 

//=============================================================================
// WORKARIUND FOR MSVC++ BUG
//=============================================================================
#define DUMMY_TMPL_ARG char 

//=============================================================================
// DECODE CLASS & MACROS
//=============================================================================
// ENCODE_BASIC_TYPE MACRO
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// TEMPLATE CLASS : Encode
//-----------------------------------------------------------------------------
template <typename _A, typename _T> class Encode
{
public:
  //-----------------------------------------------------------------------------
  // Encode::cmd_scalar
  //-----------------------------------------------------------------------------
  static void cmd_scalar (void *_lv_data, Tango::DeviceData& tango_data_)
    throw (Tango::DevFailed)
  {  
    if (_lv_data == 0) 
    {
      Tango::Except::throw_exception(_CPTC("invalid input argument"),
                                     _CPTC("binding internal error: unexpected null input argument"),
                                     _CPTC("Encode::cmd_scalar"));
    }
    LvGenericNumArrayHdl lvh = reinterpret_cast<LvGenericNumArrayHdl>(_lv_data);
#if (_TBFL_LITTLE_ENDIAN_PLATFORM_ == 1)
    switch(sizeof(_A))
    {
      case 1:
        //- cool, nothing to do!
        break;
      case 2:
        Endianness::swap_2(reinterpret_cast<const char*>((*lvh)->data),
                           reinterpret_cast<char*>((*lvh)->data));
        break;
      case 4:
        Endianness::swap_4(reinterpret_cast<const char*>((*lvh)->data),
                           reinterpret_cast<char*>((*lvh)->data));
        break;
      case 8:
        Endianness::swap_8(reinterpret_cast<const char*>((*lvh)->data),
                           reinterpret_cast<char*>((*lvh)->data));
        break;
      case 16:
        Endianness::swap_16(reinterpret_cast<const char*>((*lvh)->data),
                            reinterpret_cast<char*>((*lvh)->data));
        break;
      default:
        Tango::Except::throw_exception(_CPTC("unexpected data type"),
                                       _CPTC("no support for > 128-bits data types"),
                                       _CPTC("Encode::cmd_scalar"));
        break;
    }
#endif
    tango_data_ << *(reinterpret_cast<_A*>((*lvh)->data));
  }

  //-----------------------------------------------------------------------------
  // Encode::cmd_array
  //-----------------------------------------------------------------------------
  static void cmd_array (void *_lv_data, Tango::DeviceData& tango_data_)
    throw (Tango::DevFailed)
  {
    _A * corba_seq = new _A;
    if (corba_seq == 0) 
    {
      Tango::Except::throw_exception(_CPTC("out of memory"),
                                     _CPTC("binding internal error: memory allocation failed"),
                                     _CPTC("Encode::cmd_array"));
    }
    LvGenericNumArrayHdl lvh = reinterpret_cast<LvGenericNumArrayHdl>(_lv_data);
    corba_seq->length((*lvh)->length / sizeof(_T));
    Encode::lv_char_array_to_corba_seq(*lvh, corba_seq);
    tango_data_ << corba_seq;
  };

  //-----------------------------------------------------------------------------
  // Encode::attr_scalar
  //-----------------------------------------------------------------------------
  static void attr_scalar (void *_lv_data, Tango::DeviceAttribute& tango_data_)
    throw (Tango::DevFailed)
  {  
    if (_lv_data == 0) 
    {
      Tango::Except::throw_exception(_CPTC("invalid input argument"),
                                     _CPTC("binding internal error: unexpected null input argument"),
                                     _CPTC("Encode::cmd_scalar"));
    }
    LvGenericNumArrayHdl lvh = reinterpret_cast<LvGenericNumArrayHdl>(_lv_data);
#if (_TBFL_LITTLE_ENDIAN_PLATFORM_ == 1)
    switch(sizeof(_A))
    {
      case 1:
        //- cool, nothing to do!
        break;
      case 2:
        Endianness::swap_2(reinterpret_cast<const char*>((*lvh)->data),
                           reinterpret_cast<char*>((*lvh)->data));
        break;
      case 4:
        Endianness::swap_4(reinterpret_cast<const char*>((*lvh)->data),
                           reinterpret_cast<char*>((*lvh)->data));
        break;
      case 8:
        Endianness::swap_8(reinterpret_cast<const char*>((*lvh)->data),
                           reinterpret_cast<char*>((*lvh)->data));
        break;
      case 16:
        Endianness::swap_16(reinterpret_cast<const char*>((*lvh)->data),
                            reinterpret_cast<char*>((*lvh)->data));
        break;
      default:
        Tango::Except::throw_exception(_CPTC("unexpected data type"),
                                       _CPTC("no support for > 128-bits data types"),
                                       _CPTC("Encode::cmd_scalar"));
        break;
    }
#endif
    //- set dimensions              
    tango_data_.dim_x = 1;
    tango_data_.dim_y = 0;
    //- push LabVIEW data into the TANGO container
    _A a = *(reinterpret_cast<_A*>((*lvh)->data));
    tango_data_ << a;
  }

  //-----------------------------------------------------------------------------
  // Encode::attr_array
  //-----------------------------------------------------------------------------
  static void attr_array (LvAttributeValuePtr _avp, Tango::DeviceAttribute& tango_data_)
    throw (Tango::DevFailed)
  {
    if (_avp == 0) 
    {
      Tango::Except::throw_exception(_CPTC("invalid input argument"),
                                     _CPTC("binding internal error: unexpected null input argument"),
                                     _CPTC("Encode::attr_array"));
    }
    Tango::DevLong np = _avp->dim_x * ((_avp->dim_y) ? _avp->dim_y : 1);
    _A *corba_seq = new _A(np);
    if (corba_seq == 0) 
    {
      Tango::Except::throw_exception(_CPTC("out of memory"),
                                     _CPTC("binding internal error: memory allocation failed"),
                                     _CPTC("Encode::attr_array"));
    }  
    corba_seq->length(np);
    LvGenericNumArrayHdl lvh = reinterpret_cast<LvGenericNumArrayHdl>(_avp->data);
    Encode::lv_char_array_to_corba_seq(*lvh, corba_seq);
    tango_data_ << corba_seq;
    tango_data_.dim_x = _avp->dim_x;
    tango_data_.dim_y = _avp->dim_y;
  }

  //-----------------------------------------------------------------------------
  // Encode::lv_char_array_to_corba_seq
  //-----------------------------------------------------------------------------
  static void lv_char_array_to_corba_seq (LvGenericNumArrayPtr _lv_data, _A * corba_seq_)
    throw (Tango::DevFailed)
  {
    if (_lv_data == 0 || corba_seq_ == 0) 
    {
      Tango::Except::throw_exception(_CPTC("invalid input argument"),
                                     _CPTC("binding internal error: unexpected null input argument"),
                                     _CPTC("Encode::lv_array_to_corba_seq"));
    }
#if (_TBFL_LITTLE_ENDIAN_PLATFORM_ == 1)
    switch(sizeof(_T))
    {
      case 1:
        ::memcpy(corba_seq_->get_buffer(), _lv_data->data, corba_seq_->length());
        break;
      case 2:
        Endianness::swap_2_array(reinterpret_cast<const char*>(_lv_data->data),
                                 reinterpret_cast<char*>(corba_seq_->get_buffer()),
                                 (size_t)corba_seq_->length());
        break;
      case 4:
        Endianness::swap_4_array(reinterpret_cast<const char*>(_lv_data->data),
                                 reinterpret_cast<char*>(corba_seq_->get_buffer()),
                                 (size_t)corba_seq_->length());
        break;
      case 8:
        Endianness::swap_8_array(reinterpret_cast<const char*>(_lv_data->data),
                                 reinterpret_cast<char*>(corba_seq_->get_buffer()),
                                 (size_t)corba_seq_->length());
        break;
      case 16:
        Endianness::swap_16_array(reinterpret_cast<const char*>(_lv_data->data),
                                  reinterpret_cast<char*>(corba_seq_->get_buffer()),
                                  (size_t)corba_seq_->length());
        break;
      default:
        Tango::Except::throw_exception(_CPTC("unexpected data type"),
                                       _CPTC("no support for > 128-bits data types"),
                                       _CPTC("Encode::lv_array_to_corba_seq"));
        break;
    }
#else
    ::memcpy(corba_seq_->get_buffer(), _lv_data->data, corba_seq_->length());
#endif
  }
};
//-----------------------------------------------------------------------------
#define ENCODE_CMD_SCALAR(_T,_ARG_IN, _TD) \
  Encode<_T, DUMMY_TMPL_ARG>::cmd_scalar(_ARG_IN, _TD);
//-----------------------------------------------------------------------------
#define ENCODE_ATTR_SCALAR(_T,_ARG_IN, _TD) \
  Encode<_T, DUMMY_TMPL_ARG>::attr_scalar(_ARG_IN, _TD);
//-----------------------------------------------------------------------------
#define ENCODE_CMD_ARRAY(_A, _T, _ARG_IN, _TD) \
  Encode<_A,_T>::cmd_array(_ARG_IN, _TD);
//-----------------------------------------------------------------------------
#define LV_CHAR_ARRAY_TO_CORBA_SEQ(_A, _T, _ARG_IN, _TD) \
  Encode<_A,_T>::lv_char_array_to_corba_seq(_ARG_IN, _TD);
//-----------------------------------------------------------------------------
#define ENCODE_ATTR_ARRAY(_A, _T, _ATTR_VAL, _TD) \
  Encode<_A,_T>::attr_array(_ATTR_VAL, _TD);
//-----------------------------------------------------------------------------

//=============================================================================
// DECODE CLASS & MACROS
//=============================================================================
//-----------------------------------------------------------------------------
// TEMPLATE CLASS: Decode
//-----------------------------------------------------------------------------
template <typename _A, typename _T> class Decode 
{
public:
  //-----------------------------------------------------------------------------
  // Decode::cmd_scalar
  //-----------------------------------------------------------------------------
  static void cmd_scalar (Tango::DeviceData& _tango_data, void*& lv_data_)
    throw (Tango::DevFailed)
  {
    _tango_data.exceptions(Tango::DeviceData::isempty_flag 
                             | Tango::DeviceData::wrongtype_flag);
    _A a;
    _tango_data >> a;

    MgErr err = ::NumericArrayResize(uB,  
                                     1L, 
                                     reinterpret_cast<UHandle*>(&lv_data_), 
                                     sizeof(_T));
    if (err != noErr) 
    {
      Tango::Except::throw_exception(_CPTC("out of memory"),
                                     _CPTC("binding internal error: memory allocation failed"),
                                     _CPTC("Decode::cmd_scalar"));
    } 
    LvGenericNumArrayHdl lvh = reinterpret_cast<LvGenericNumArrayHdl>(lv_data_);
    (*lvh)->length = sizeof(_T);
#if (_TBFL_LITTLE_ENDIAN_PLATFORM_ == 1)
    switch(sizeof(_A))
    {
      case 1:
        //- cool, nothing to do!
        break;
      case 2:
        Endianness::swap_2(reinterpret_cast<const char*>(&a),
                           reinterpret_cast<char*>(&a));
        break;
      case 4:
        Endianness::swap_4(reinterpret_cast<const char*>(&a),
                           reinterpret_cast<char*>(&a));
        break;
      case 8:
        Endianness::swap_8(reinterpret_cast<const char*>(&a),
                           reinterpret_cast<char*>(&a));
        break;
      case 16:
        Endianness::swap_16(reinterpret_cast<const char*>(&a),
                            reinterpret_cast<char*>(&a));
        break;
      default:
        Tango::Except::throw_exception(_CPTC("unexpected data type"),
                                       _CPTC("no support for > 128-bits data types"),
                                       _CPTC("Encode::cmd_scalar"));
        break;
    }
#endif
    *(reinterpret_cast<_T*>((*lvh)->data)) = static_cast<_T>(a);
  }

  //-----------------------------------------------------------------------------
  // Decode::cmd_array
  //-----------------------------------------------------------------------------
  static void cmd_array (Tango::DeviceData& _tango_data, void*& lv_data_)
    throw (Tango::DevFailed)
  {
    _tango_data.exceptions(Tango::DeviceData::isempty_flag 
                             | Tango::DeviceData::wrongtype_flag);
    const _A * corba_seq;
    if ((_tango_data >> corba_seq) == false)
    {
      Tango::Except::throw_exception(_CPTC("data extraction failed"),
                                     _CPTC("could not extract data from Tango::DeviceData"),
                                     _CPTC("Encode::cmd_array"));

    }

    size_t num_elements = static_cast<size_t>(corba_seq->length());

    Decode::corba_seq_to_lv_array(num_elements, num_elements, corba_seq, lv_data_);
  }

  //-----------------------------------------------------------------------------
  // Decode::attr_scalar
  //-----------------------------------------------------------------------------
  static void attr_scalar (Tango::DeviceAttribute& _tango_data, void*& lv_data_)
    throw (Tango::DevFailed)
  {
    _tango_data.exceptions(Tango::DeviceData::isempty_flag 
                             | Tango::DeviceData::wrongtype_flag);
    _A a;
    _tango_data >> a;

    //- actual attr value + dummy setpoint
    size_t data_size =  2 * sizeof(_T);
    
    MgErr err = ::NumericArrayResize(uB,  
                                     1L, 
                                     reinterpret_cast<UHandle*>(&lv_data_), 
                                     data_size);
    if (err != noErr) 
    {
      Tango::Except::throw_exception(_CPTC("out of memory"),
                                     _CPTC("binding internal error: memory allocation failed"),
                                     _CPTC("Decode:::attr_scalar"));
    }  
    LvGenericNumArrayHdl lvh = reinterpret_cast<LvGenericNumArrayHdl>(lv_data_);
    (*lvh)->length = data_size;
#if (_TBFL_LITTLE_ENDIAN_PLATFORM_ == 1)
    switch(sizeof(_A))
    {
      case 1:
        //- cool, nothing to do!
        break;
      case 2:
        Endianness::swap_2(reinterpret_cast<const char*>(&a),
                           reinterpret_cast<char*>(&a));
        break;
      case 4:
        Endianness::swap_4(reinterpret_cast<const char*>(&a),
                           reinterpret_cast<char*>(&a));
        break;
      case 8:
        Endianness::swap_8(reinterpret_cast<const char*>(&a),
                           reinterpret_cast<char*>(&a));
        break;
      case 16:
        Endianness::swap_16(reinterpret_cast<const char*>(&a),
                            reinterpret_cast<char*>(&a));
        break;
      default:
        Tango::Except::throw_exception(_CPTC("unexpected data type"),
                                       _CPTC("no support for > 128-bits data types"),
                                       _CPTC("Encode::attr_scalar"));
        break;
    }
#endif
    //- actual attr value
    *(reinterpret_cast<_T*>((*lvh)->data) + 0) = static_cast<_T>(a); 
    //- dummy setpoint
    *(reinterpret_cast<_T*>((*lvh)->data) + 1) = static_cast<_T>(0); 
  }

  //-----------------------------------------------------------------------------
  // Decode::attr_array
  //-----------------------------------------------------------------------------
  static void attr_array (int attr_format, 
                          bool attr_writable, 
                          Tango::DeviceAttribute& _tango_data, 
                          void*& lv_data_)
    throw (Tango::DevFailed)
  {
    _tango_data.exceptions(Tango::DeviceAttribute::isempty_flag 
                              | Tango::DeviceAttribute::wrongtype_flag);

    size_t src_num_elements = 0;
    size_t dest_num_elements = 0;
    switch  (attr_format)
    {
      case Tango::SCALAR:
        {
          src_num_elements = attr_writable ? 2 : 1;
          //- actual value + setpoint (might be a dummy one - set to 0)
          dest_num_elements = 2;
        }
        break;
      case Tango::SPECTRUM:
        {
          src_num_elements = _tango_data.get_nb_read();
          //- no setpoint for spectrum
          dest_num_elements = src_num_elements;
        }
        break;
      case Tango::IMAGE:
        {
          Tango::AttributeDimension r_dim = _tango_data.get_r_dimension();
          src_num_elements = r_dim.dim_x * r_dim.dim_y;
          //- no setpoint for image
          dest_num_elements = src_num_elements;
        }
        break;
    }
    
    _A * corba_seq = 0;
    try 
    {
      _tango_data >> corba_seq;
      Decode::corba_seq_to_lv_array(src_num_elements, dest_num_elements, corba_seq, lv_data_);
      delete corba_seq;
    }
    catch (...)
    {
      delete corba_seq;
      throw;
    }
  }

  //-----------------------------------------------------------------------------
  // Decode::corba_seq_to_lv_array
  //-----------------------------------------------------------------------------
  static void corba_seq_to_lv_array (size_t src_num_elements, 
                                     size_t dest_num_elements, 
                                     const _A* _corba_seq, 
                                     void*& lv_data_)
    throw (Tango::DevFailed)
  {
    MgErr err = ::NumericArrayResize(uB, 
                                     1L, 
                                     reinterpret_cast<UHandle*>(&lv_data_), 
                                     dest_num_elements * sizeof(_T));
    if (err != noErr) 
    {
      Tango::Except::throw_exception(_CPTC("out of memory"),
                                     _CPTC("binding internal error: memory allocation failed"),
                                     _CPTC("Decode::corba_seq_to_lv_array"));
    }  
    
    LvGenericNumArrayHdl lvh = reinterpret_cast<LvGenericNumArrayHdl>(lv_data_);
    (*lvh)->length = dest_num_elements * sizeof(_T);
    
    int32 dn = dest_num_elements - src_num_elements;
    if (dn > 0)
      ::memset((*lvh)->data + dn, 0, dn * sizeof(_T));
    
#if (_TBFL_LITTLE_ENDIAN_PLATFORM_ == 1)
    switch(sizeof(_T))
    {
      case 1:
        ::memcpy((*lvh)->data, _corba_seq->get_buffer(), src_num_elements * sizeof(_T));
        break;
      case 2:
        Endianness::swap_2_array(reinterpret_cast<const char*>(_corba_seq->get_buffer()),
                                 reinterpret_cast<char*>((*lvh)->data),
                                 src_num_elements);
        break;
      case 4:
        Endianness::swap_4_array(reinterpret_cast<const char*>(_corba_seq->get_buffer()),
                                 reinterpret_cast<char*>((*lvh)->data),
                                 src_num_elements);
        break;
      case 8:
        Endianness::swap_8_array(reinterpret_cast<const char*>(_corba_seq->get_buffer()),
                                 reinterpret_cast<char*>((*lvh)->data),
                                 src_num_elements);
        break;
      case 16:
        Endianness::swap_16_array(reinterpret_cast<const char*>(_corba_seq->get_buffer()),
                                  reinterpret_cast<char*>((*lvh)->data),
                                  src_num_elements);
        break;
      default:
        Tango::Except::throw_exception(_CPTC("unexpected data type"),
                                       _CPTC("no support for > 128-bits data types"),
                                       _CPTC("Decode::corba_seq_to_lv_array"));
        break;
    }
#else
    ::memcpy((*lvh)->data, _corba_seq->get_buffer(), src_num_elements * sizeof(_T));
#endif
  } 
}; 
//-----------------------------------------------------------------------------
#define DECODE_CMD_SCALAR(_SRC_T, _TD, _LVD) \
  Decode<_SRC_T, _SRC_T>::cmd_scalar(_TD, _LVD);
//-----------------------------------------------------------------------------
#define DECODE_ATTR_SCALAR(_SRC_T, _TD, _LVD) \
  Decode<_SRC_T, _SRC_T>::attr_scalar(_TD, _LVD);
//-----------------------------------------------------------------------------
#define CORBA_SEQ_TO_LV_ARRAY(_A, _T, _TD, _LVD) \
  Decode<_A,_T>::corba_seq_to_lv_array((_TD)->length(), (_TD)->length(), _TD, _LVD);
//-----------------------------------------------------------------------------
#define DECODE_CMD_ARRAY(_A, _T, _TD, _LVD) \
  Decode<_A,_T>::cmd_array(_TD, _LVD);
//-----------------------------------------------------------------------------
#define DECODE_ATTR_ARRAY(_F, _W, _A, _T, _TD, _LVD) \
  Decode<_A,_T>::attr_array(_F, _W, _TD, _LVD);
//-----------------------------------------------------------------------------

//=============================================================================
// STATIC MEMBERS
//=============================================================================
DataAdapter* DataAdapter::instance_ = 0;

//=============================================================================
// DataAdapter::init
//=============================================================================
int DataAdapter::init (void)
{
  if (DataAdapter::instance_ != 0)
    return 0;

  DataAdapter::instance_ = new DataAdapter;

  return (DataAdapter::instance_) ? 0 : -1; 
}

//=============================================================================
// DataAdapter::cleanup
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
// DataAdapter::DataAdapter 
//=============================================================================
DataAdapter::DataAdapter (void)
{
 // no-op ctor
}

//=============================================================================
// DataAdapter::~DataAdapter 
//=============================================================================
DataAdapter::~DataAdapter (void)
{
 // no-op dtor
}

//=============================================================================
// DataAdapter::encode_argin
//=============================================================================
int DataAdapter::encode_argin (DeviceDescriptor* _ddesc, 
                               int _cmd_id, 
                               LvArgIOPtr _argin,
                               Tango::DeviceData& dd_in_)
    throw (Tango::DevFailed)
{
  if (_ddesc == 0) 
  {
    Tango::Except::throw_exception(_CPTC("internal error"),
                                   _CPTC("unexpected NULL argument"),
                                   _CPTC("DataAdapter::encode_argin"));
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
          //- create the DevVarLongStringArray
          Tango::DevVarLongStringArray* dvls_array = new Tango::DevVarLongStringArray;
          if (dvls_array == 0) {
            Tango::Except::throw_exception(_CPTC("out of memory"),
                                           _CPTC("binding internal error: memory allocation failed"),
                                           _CPTC("DataAdapter::encode_argin"));
          }
          //- encode int32 data
          LvGenericNumArrayHdl lvgah = reinterpret_cast<LvGenericNumArrayHdl>(_argin->num_data);
          dvls_array->lvalue.length((*lvgah)->length / sizeof(Tango::DevLong));
          LV_CHAR_ARRAY_TO_CORBA_SEQ(Tango::DevVarLongArray, Tango::DevLong, *lvgah, &(dvls_array->lvalue));
          //- encode string data
          LvStringArrayHdl sah = reinterpret_cast<LvStringArrayHdl>(_argin->str_data);
          if (sah == 0) {
            Tango::Except::throw_exception(_CPTC("invalid input argument"),
                                           _CPTC("binding internal error: unexpected null input argument"),
                                           _CPTC("DataAdapter::encode_argin"));
          }
          dvls_array->svalue.length((*sah)->length);
          LvStringHdl sh = 0;
	        for (int32 i = 0; i < (*sah)->length; i++) 
          {
            sh = (*sah)->data[i]; 
            dvls_array->svalue[i] = CORBA::string_alloc((*sh)->length + 1);
            if (dvls_array->svalue[i] == 0) 
            {
              delete dvls_array;
              Tango::Except::throw_exception(_CPTC("out of memory"),
                                             _CPTC("binding internal error: memory allocation failed"),
                                             _CPTC("DataAdapter::encode_argin"));
            }
            ::memcpy(dvls_array->svalue[i], (*sh)->data, (*sh)->length);
            ::memset(dvls_array->svalue[i] + (*sh)->length, 0 , 1);
	        }
          dd_in_ << dvls_array;
        } break;
      //-- DEVVAR_DOUBLESTRINGARRAY --------------------
      case Tango::DEVVAR_DOUBLESTRINGARRAY: 
        {
          //- create the DevVarDoubleStringArray
          Tango::DevVarDoubleStringArray* dvds_array = new Tango::DevVarDoubleStringArray;
          if (dvds_array == 0) {
            Tango::Except::throw_exception(_CPTC("out of memory"),
                                           _CPTC("binding internal error: memory allocation failed"),
                                           _CPTC("DataAdapter::encode_argin"));
          }
          //- encode double data
          LvGenericNumArrayHdl lvgah = reinterpret_cast<LvGenericNumArrayHdl>(_argin->num_data);
          dvds_array->dvalue.length((*lvgah)->length / sizeof(double));
          LV_CHAR_ARRAY_TO_CORBA_SEQ(Tango::DevVarDoubleArray, double, *lvgah, &(dvds_array->dvalue));
          //- encode string data
          LvStringArrayHdl sah = reinterpret_cast<LvStringArrayHdl>(_argin->str_data);
          if (sah == 0) {
            Tango::Except::throw_exception(_CPTC("invalid input argument"),
                                           _CPTC("binding internal error: unexpected null input argument"),
                                           _CPTC("DataAdapter::encode_argin"));
          }
          dvds_array->svalue.length((*sah)->length);
          LvStringHdl sh = 0;
	        for (int32 i = 0; i < (*sah)->length; i++) 
          {
            sh = (*sah)->data[i]; 
            dvds_array->svalue[i] = CORBA::string_alloc((*sh)->length + 1);
            if (dvds_array->svalue[i] == 0) 
            {
              delete dvds_array;
              Tango::Except::throw_exception(_CPTC("out of memory"),
                                             _CPTC("binding internal error: memory allocation failed"),
                                             _CPTC("DataAdapter::encode_argin"));
            }
            ::memcpy(dvds_array->svalue[i], (*sh)->data, (*sh)->length);
            ::memset(dvds_array->svalue[i] + (*sh)->length, 0 , 1);
	        }
          dd_in_ << dvds_array;
        } break;
      //-- DEVVAR_STRINGARRAY --------------------------
      case Tango::DEVVAR_STRINGARRAY: 
        {
          LvStringArrayHdl h = reinterpret_cast<LvStringArrayHdl>(_argin->str_data);
          if (h == 0) {
            Tango::Except::throw_exception(_CPTC("invalid argument"),
                                           _CPTC("binding internal error: unexpected null argument"),
                                           _CPTC("DataAdapter::encode_argin"));
          }
	        Tango::DevVarStringArray *str_array = new Tango::DevVarStringArray((*h)->length);
          if (str_array == 0) {
            Tango::Except::throw_exception(_CPTC("out of memory"),
                                           _CPTC("binding internal error: memory allocation failed"),
                                           _CPTC("DataAdapter::encode_argin"));
          }
          str_array->length((*h)->length);
          LvStringHdl sh = 0;
	        for (int32 i = 0; i < (*h)->length; i++) 
          {
            sh = (*h)->data[i]; 
            (*str_array)[i] = CORBA::string_alloc((*sh)->length + 1);
            if ((*str_array)[i] == 0) 
            {
              delete str_array;
              Tango::Except::throw_exception(_CPTC("out of memory"),
                                             _CPTC("binding internal error: memory allocation failed"),
                                             _CPTC("DataAdapter::encode_argin"));
            }
            ::memcpy((*str_array)[i], (*sh)->data, (*sh)->length);
            ::memset((*str_array)[i] + (*sh)->length, 0 , 1);
	        }
          dd_in_ << str_array;
        } 
        break;
      //-- DEVVAR_DOUBLEARRAY --------------------------
      case Tango::DEVVAR_DOUBLEARRAY: 
        {
          ENCODE_CMD_ARRAY(Tango::DevVarDoubleArray, Tango::DevDouble, _argin->num_data, dd_in_);
        } 
        break;
      //-- DEVVAR_FLOATARRAY ---------------------------
      case Tango::DEVVAR_FLOATARRAY: 
        {
          ENCODE_CMD_ARRAY(Tango::DevVarFloatArray, Tango::DevFloat, _argin->num_data, dd_in_);
        } 
        break;
      //-- DEVVAR_LONG64ARRAY --------------------------
      case Tango::DEVVAR_LONG64ARRAY: 
        {
          ENCODE_CMD_ARRAY(Tango::DevVarLong64Array, Tango::DevLong64, _argin->num_data, dd_in_);
        } 
        break;
      //-- DEVVAR_ULONG64ARRAY -------------------------
      case Tango::DEVVAR_ULONG64ARRAY: 
        {
          ENCODE_CMD_ARRAY(Tango::DevVarULong64Array, Tango::DevULong64, _argin->num_data, dd_in_);
        } 
        break;
      //-- DEVVAR_LONGARRAY ---------------------------
      case Tango::DEVVAR_LONGARRAY: 
        {
          ENCODE_CMD_ARRAY(Tango::DevVarLongArray, Tango::DevLong, _argin->num_data, dd_in_);
        } 
        break;
      //-- DEVVAR_ULONGARRAY ---------------------------
      case Tango::DEVVAR_ULONGARRAY: 
        {
          ENCODE_CMD_ARRAY(Tango::DevVarULongArray, Tango::DevULong, _argin->num_data, dd_in_);
        } 
        break;
      //-- DEVVAR_SHORTARRAY ---------------------------
      case Tango::DEVVAR_SHORTARRAY: 
        {
          ENCODE_CMD_ARRAY(Tango::DevVarShortArray, Tango::DevShort, _argin->num_data, dd_in_);
        } 
        break;
      //-- DEVVAR_USHORTARRAY --------------------------
      case Tango::DEVVAR_USHORTARRAY: 
        {
          ENCODE_CMD_ARRAY(Tango::DevVarUShortArray, Tango::DevUShort, _argin->num_data, dd_in_);
        } 
        break;
      //-- DEVVAR_CHARARRAY
      case Tango::DEVVAR_CHARARRAY: 
        {
          ENCODE_CMD_ARRAY(Tango::DevVarCharArray, Tango::DevUChar, _argin->num_data, dd_in_);
        } 
        break;
      //-- DEV_STRING ----------------------------------
      case Tango::DEV_STRING: 
        {
          LvStringHdl h = reinterpret_cast<LvStringHdl>(_argin->num_data);
          std::string s;
          s.assign(reinterpret_cast<char*>((*h)->data), 
                   reinterpret_cast<char*>((*h)->data) + (*h)->length);
		      dd_in_ << s;
        } 
        break;
      //-- DEV_BOOLEAN ---------------------------------
      case Tango::DEV_BOOLEAN: 
        {
 		      ENCODE_CMD_SCALAR(Tango::DevBoolean, _argin->num_data, dd_in_); 
        } 
        break;
      //-- DEV_SHORT -----------------------------------
      case Tango::DEV_SHORT: 
        {
 		      ENCODE_CMD_SCALAR(Tango::DevShort, _argin->num_data, dd_in_); 
        } 
        break;
      //-- DEV_SHORT -----------------------------------
      case Tango::DEV_USHORT: 
        {
 		      ENCODE_CMD_SCALAR(Tango::DevUShort, _argin->num_data, dd_in_); 
        } 
        break;
      //-- DEV_LONG ------------------------------------
      case Tango::DEV_LONG: 
        {
 		      ENCODE_CMD_SCALAR(Tango::DevLong, _argin->num_data, dd_in_); 
        } 
        break;
      //-- DEV_ULONG -----------------------------------
      case Tango::DEV_ULONG: 
        {
 		      ENCODE_CMD_SCALAR(Tango::DevULong, _argin->num_data, dd_in_); 
        } 
        break;
      //-- DEV_LONG64 ----------------------------------
      case Tango::DEV_LONG64: 
        {
          ENCODE_CMD_SCALAR(Tango::DevLong64, _argin->num_data, dd_in_); 
        } 
        break;
      //-- DEV_ULONG64 ---------------------------------
      case Tango::DEV_ULONG64: 
        {
          ENCODE_CMD_SCALAR(Tango::DevULong64, _argin->num_data, dd_in_); 
        } 
        break;
      //-- DEV_FLOAT -----------------------------------
      case Tango::DEV_FLOAT: 
        {
 		      ENCODE_CMD_SCALAR(Tango::DevFloat, _argin->num_data, dd_in_); 
        } 
        break;
      //-- DEV_DOUBLE ----------------------------------
      case Tango::DEV_DOUBLE: 
        {
 		      ENCODE_CMD_SCALAR(Tango::DevDouble, _argin->num_data, dd_in_); 
        } 
        break;
      //-- DEFAULT -------------------------------------
      case Tango::DEV_STATE: 
      case Tango::CONST_DEV_STRING: 
      default:
        Tango::Except::throw_exception(_CPTC("unknown or unsupported TANGO data type"),
                                       _CPTC("binding internal error: unsupported argin type for command"),
                                       _CPTC("DataAdapter::encode_argin"));
        break;
    } // switch 
  } //try
  catch (const Tango::DevFailed&) 
  {
    throw;
	}
  catch (...) 
  {
    Tango::Except::throw_exception(_CPTC("unknown error"),
                                   _CPTC("binding internal error: unknown exception caught"),
                                   _CPTC("DataAdapter::encode_argin"));
  }
  return kNoError;
}

//=============================================================================
// DataAdapter::decode_argout
//=============================================================================
int DataAdapter::decode_argout (DeviceDescriptor* _ddesc, 
                                int _cmd_id,
                                Tango::DeviceData& _dd_out, 
                                LvArgIOPtr argout_)
    throw (Tango::DevFailed)
{
  if (_ddesc == 0) {
    Tango::Except::throw_exception(_CPTC("internal error"),
                                   _CPTC("binding internal error: unexpected null argument"),
                                   _CPTC("DataAdapter::decode_argout"));
  }

  _dd_out.exceptions(Tango::DeviceData::isempty_flag | Tango::DeviceData::wrongtype_flag);

  try 
  {
    int argout_type = (_ddesc->cmd_list())[_cmd_id].out_type;

    switch (argout_type) 
    {
      //-- DEV_VOID ------------------------------------
      case Tango::DEV_VOID: 
        {
          return kNoError; 
        } 
        break;
      //-- DEVVAR_LONGSTRINGARRAY ----------------------
      case Tango::DEVVAR_LONGSTRINGARRAY:
        {
          const Tango::DevVarLongStringArray *corba_struct;
          _dd_out >> corba_struct;
          //- decode int32 data
          CORBA_SEQ_TO_LV_ARRAY(Tango::DevVarLongArray, Tango::DevLong, &(corba_struct->lvalue), argout_->num_data);
          //- decode string data
          LvStringArrayHdl sah = argout_->str_data;
          if (sah == 0) {
            Tango::Except::throw_exception(_CPTC("invalid output argument"),
                                           _CPTC("binding internal error: unexpected null output argument"),
                                           _CPTC("DataAdapter::decode_argout"));
          }
          MgErr err = ::NumericArrayResize(iL, 
                                           1L, 
                                           reinterpret_cast<UHandle*>(&sah), 
                                           corba_struct->svalue.length());
          if (err != noErr) {
            Tango::Except::throw_exception(_CPTC("out of memory"),
                                           _CPTC("binding internal error: memory allocation failed"),
                                           _CPTC("DataAdapter::decode_argout"));
          }
		      (*sah)->length = corba_struct->svalue.length();
          ::memset((*sah)->data, 0, (*sah)->length * sizeof(Tango::DevLong));
          size_t l = 0;
          LvStringHdl sh = 0;
		      for (int32 i = 0; i < (*sah)->length; i++) 
          {
            sh = 0;
			      l = ::strlen(corba_struct->svalue[i]);
            err = ::NumericArrayResize(uB,  1L, reinterpret_cast<UHandle*>(&sh), l);
            if (err != noErr) {
              Tango::Except::throw_exception(_CPTC("out of memory"),
                                             _CPTC("binding internal error: memory allocation failed"),
                                             _CPTC("DataAdapter::decode_argout"));
            }
			      (*sh)->length = l;
            ::memcpy((*sh)->data, corba_struct->svalue[i], l);
            (*sah)->data[i] = sh;
          }
        } 
        break;
      //-- DEVVAR_DOUBLESTRINGARRAY --------------------
      case Tango::DEVVAR_DOUBLESTRINGARRAY: 
        {
          const Tango::DevVarDoubleStringArray *corba_struct;
          _dd_out >> corba_struct;
           //- decode double data
          CORBA_SEQ_TO_LV_ARRAY(Tango::DevVarDoubleArray, Tango::DevDouble, &(corba_struct->dvalue), argout_->num_data);
          //- decode string data
          LvStringArrayHdl sah = argout_->str_data;
          if (sah == 0) {
            Tango::Except::throw_exception(_CPTC("invalid output argument"),
                                           _CPTC("binding internal error: unexpected null output argument"),
                                           _CPTC("DataAdapter::decode_argout"));
          }
          MgErr err = ::NumericArrayResize(iL, 
                                           1L, 
                                           reinterpret_cast<UHandle*>(&sah), 
                                           corba_struct->svalue.length());
          if (err != noErr) {
            Tango::Except::throw_exception(_CPTC("out of memory"),
                                           _CPTC("binding internal error: memory allocation failed"),
                                           _CPTC("DataAdapter::decode_argout"));
          }
		      (*sah)->length = corba_struct->svalue.length();
          ::memset((*sah)->data, 0, (*sah)->length * sizeof(Tango::DevLong));
          size_t l = 0;
          LvStringHdl sh = 0;
		      for (int32 i = 0; i < (*sah)->length; i++) 
          {
            sh = 0;
			      l = ::strlen(corba_struct->svalue[i]);
            err = ::NumericArrayResize(uB,  1L, reinterpret_cast<UHandle*>(&sh), l);
            if (err != noErr) {
              Tango::Except::throw_exception(_CPTC("out of memory"),
                                             _CPTC("binding internal error: memory allocation failed"),
                                             _CPTC("DataAdapter::decode_argout"));
            }
			      (*sh)->length = l;
            ::memcpy((*sh)->data, corba_struct->svalue[i], l);
            (*sah)->data[i] = sh;
          }
        } 
        break;
      //-- DEVVAR_STRINGARRAY -------------------------
      case Tango::DEVVAR_STRINGARRAY: 
        {
          const Tango::DevVarStringArray *corba_seq;
          _dd_out >> corba_seq;
          MgErr err = ::NumericArrayResize(iL, 
                                           1L, 
                                           reinterpret_cast<UHandle*>(&(argout_->str_data)), 
                                           corba_seq->length());
          if (err != noErr) {
            Tango::Except::throw_exception(_CPTC("out of memory"),
                                           _CPTC("binding internal error: memory allocation failed"),
                                           _CPTC("DataAdapter::decode_argout"));
          }
          LvStringArrayHdl lvh = reinterpret_cast<LvStringArrayHdl>(argout_->str_data);
		      (*lvh)->length = corba_seq->length();
          ::memset((*lvh)->data, 0, (*lvh)->length * sizeof(Tango::DevLong));
          size_t l = 0;
          LvStringHdl sh = 0;
		      for (int32 i = 0; i < (*lvh)->length; i++) 
          {
            sh = 0;
			      l = ::strlen((*corba_seq)[i]);
            err = ::NumericArrayResize(uB,  1L, reinterpret_cast<UHandle*>(&sh), l);
            if (err != noErr) {
              Tango::Except::throw_exception(_CPTC("out of memory"),
                                             _CPTC("binding internal error: memory allocation failed"),
                                             _CPTC("DataAdapter::decode_argout"));
            }
			      (*sh)->length = l;
            ::memcpy((*sh)->data, (*corba_seq)[i], l);
            (*lvh)->data[i] = sh;
          }
        } 
        break;
      //-- DEVVAR_DOUBLEARRAY ------------------------
      case Tango::DEVVAR_DOUBLEARRAY: 
        {
          DECODE_CMD_ARRAY(Tango::DevVarDoubleArray, Tango::DevDouble, _dd_out, argout_->num_data);
        } 
        break;
      //-- DEVVAR_FLOATARRAY -------------------------
      case Tango::DEVVAR_FLOATARRAY: 
        {
          DECODE_CMD_ARRAY(Tango::DevVarFloatArray, Tango::DevFloat, _dd_out, argout_->num_data);
        } 
        break;
      //-- DEVVAR_LONGARRAY --------------------------
      case Tango::DEVVAR_LONGARRAY: 
        { 
          DECODE_CMD_ARRAY(Tango::DevVarLongArray, Tango::DevLong, _dd_out, argout_->num_data);
        } 
        break;
      //-- DEVVAR_ULONGARRAY -------------------------
      case Tango::DEVVAR_ULONGARRAY: 
        {   
          DECODE_CMD_ARRAY(Tango::DevVarULongArray, Tango::DevULong, _dd_out, argout_->num_data);
        } 
        break;
      //-- DEVVAR_LONG64 -----------------------------
      case Tango::DEVVAR_LONG64ARRAY:
        { 
          DECODE_CMD_ARRAY(Tango::DevVarLong64Array, Tango::DevLong64, _dd_out, argout_->num_data);
        } 
        break;
      //-- DEVVAR_ULONG64 ----------------------------
      case Tango::DEVVAR_ULONG64ARRAY:
        { 
          DECODE_CMD_ARRAY(Tango::DevVarULong64Array, Tango::DevULong64, _dd_out, argout_->num_data);
        } 
        break;
      //-- DEVVAR_SHORTARRAY -------------------------
      case Tango::DEVVAR_SHORTARRAY: 
        {
          DECODE_CMD_ARRAY(Tango::DevVarShortArray, Tango::DevShort, _dd_out, argout_->num_data);
        } 
        break;
      //-- DEVVAR_USHORTARRAY ------------------------
      case Tango::DEVVAR_USHORTARRAY: 
        {
          DECODE_CMD_ARRAY(Tango::DevVarUShortArray, Tango::DevUShort, _dd_out, argout_->num_data);
        } 
        break;
      //-- DEVVAR_CHARARRAY --------------------------
      case Tango::DEVVAR_CHARARRAY: 
        {
          DECODE_CMD_ARRAY(Tango::DevVarCharArray, Tango::DevUChar, _dd_out, argout_->num_data);
        } 
        break;
      //-- DEV_STRING --------------------------------
      case Tango::DEV_STRING: 
        {
          std::string s;
          _dd_out >> s;
          MgErr err = ::NumericArrayResize(uB,  
                                           1L, 
                                           reinterpret_cast<UHandle*>(&(argout_->num_data)), 
                                           s.size());
          if (err != noErr) {
            Tango::Except::throw_exception(_CPTC("out of memory error"),
                                           _CPTC("binding internal error: memory allocation failed"),
                                           _CPTC("DataAdapter::decode_argout"));
          }
          LvStringHdl lvh = reinterpret_cast<LvStringHdl>(argout_->num_data);
          (*lvh)->length = s.size();
          ::memcpy((*lvh)->data, s.c_str(), s.size());
        } 
        break;
      //-- DEV_BOOLEAN -------------------------------
      case Tango::DEV_BOOLEAN: 
        {
          DECODE_CMD_SCALAR(Tango::DevBoolean, _dd_out, argout_->num_data);
        }  
        break;
      //-- DEV_SHORT ---------------------------------
      case Tango::DEV_SHORT: 
        {
          DECODE_CMD_SCALAR(Tango::DevShort, _dd_out, argout_->num_data);
        } 
        break;
      //-- DEV_USHORT --------------------------------
      case Tango::DEV_USHORT: 
        {
          DECODE_CMD_SCALAR(Tango::DevUShort, _dd_out, argout_->num_data);
        }  
        break;
      //-- DEV_LONG ---------------------------------
      case Tango::DEV_LONG: 
        {
          DECODE_CMD_SCALAR(Tango::DevLong, _dd_out, argout_->num_data);
        }  
        break;      
      //-- DEV_ULONG --------------------------------
      case Tango::DEV_ULONG: 
        {
          DECODE_CMD_SCALAR(Tango::DevULong, _dd_out, argout_->num_data);
        }  
        break;
      //-- DEV_LONG64 -------------------------------
      case Tango::DEV_LONG64: 
        {
          DECODE_CMD_SCALAR(Tango::DevLong64, _dd_out, argout_->num_data);
        }  
        break;
      //-- DEV_ULONG64 ------------------------------
      case Tango::DEV_ULONG64: 
        {
          DECODE_CMD_SCALAR(Tango::DevULong64, _dd_out, argout_->num_data);
        }  
        break;
      //-- DEV_FLOAT --------------------------------
      case Tango::DEV_FLOAT: 
        {
          DECODE_CMD_SCALAR(Tango::DevFloat, _dd_out, argout_->num_data);
        } 
        break;
      //-- DEV_DOUBLE -------------------------------
      case Tango::DEV_DOUBLE: 
        {
          DECODE_CMD_SCALAR(Tango::DevDouble, _dd_out, argout_->num_data);
        } 
        break;
      //-- DEV_STATE --------------------------------
      case Tango::DEV_STATE: 
        {
          DECODE_CMD_SCALAR(Tango::DevState, _dd_out, argout_->num_data);
        } 
        break;
      //-- DEFAULT ----------------------------------
      default:
        Tango::Except::throw_exception(_CPTC("unknown or unsupported TANGO data type"),
                                       _CPTC("binding internal error: unexpected TANGO data type for command"),
                                       _CPTC("DataAdapter::decode_argout"));
        break;
    } // switch
  } // try        
  catch (const Tango::DevFailed&) 
  {
    throw;
	}
  catch (...) 
  {
    Tango::Except::throw_exception(_CPTC("unknown error"),
                                   _CPTC("binding internal error: unknown exception caught"),
                                   _CPTC("DataAdapter::decode_argout"));
  }
  return kNoError;
}

//=============================================================================
// DataAdapter::encode_attr
//=============================================================================
int DataAdapter::encode_attr (DeviceDescriptor* _ddesc,
                              int _attr_id,
                              LvAttributeValuePtr _avp,
                              Tango::DeviceAttribute& value_)
    throw (Tango::DevFailed)
{
  //- check input
  if (_ddesc == 0) {
    Tango::Except::throw_exception(_CPTC("internal error"),
                                   _CPTC("binding internal error: unexpected null argument"),
                                   _CPTC("DataAdapter::encode_attr"));
  }
  //- get attr list
  const DeviceDescriptor::MinAttrInfoList &attr_list = _ddesc->attr_list();
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
  //- convert to TANGO type
  try 
  {
    switch (attr_format) 
    {
      //-- SCALAR ATTRIBUTE -------------------------------------------
      case Tango::SCALAR: 
      {
        //- handle data type
        switch (attr_type) 
        {
          //-- SCALAR::DEV_BOOLEAN -----------------------------------
          case Tango::DEV_BOOLEAN: 
            {
              ENCODE_ATTR_SCALAR(Tango::DevBoolean, _avp->data, value_);
            } 
            break;
          //-- SCALAR::DEV_UCHAR -------------------------------------
          case Tango::DEV_UCHAR: 
            {
              ENCODE_ATTR_SCALAR(Tango::DevUChar, _avp->data, value_);
            } 
            break;
          //-- SCALAR::DEV_SHORT -------------------------------------
          case Tango::DEV_SHORT: 
            {
              ENCODE_ATTR_SCALAR(Tango::DevShort, _avp->data, value_);
            } 
            break;
          //-- SCALAR::DEV_USHORT ------------------------------------
          case Tango::DEV_USHORT: 
            {
              ENCODE_ATTR_SCALAR(Tango::DevUShort, _avp->data, value_);
            } 
            break;
          //-- SCALAR::DEV_LONG --------------------------------------
          case Tango::DEV_LONG: 
            {
              ENCODE_ATTR_SCALAR(Tango::DevLong, _avp->data, value_);
            } 
            break;
          //-- SCALAR::DEV_ULONG --------------------------------------
          case Tango::DEV_ULONG: 
            {
              ENCODE_ATTR_SCALAR(Tango::DevULong, _avp->data, value_);
            } 
            break;
          //-- SCALAR::DEV_LONG64  -----------------------------------
          case Tango::DEV_LONG64:
            {
              ENCODE_ATTR_SCALAR(Tango::DevLong64, _avp->data, value_);
            } 
            break;
          //-- SCALAR::DEV_ULONG64  ----------------------------------
          case Tango::DEV_ULONG64: 
            {
              ENCODE_ATTR_SCALAR(Tango::DevULong64, _avp->data, value_);
            } 
            break;
          //-- SCALAR::DEV_FLOAT -------------------------------------
          case Tango::DEV_FLOAT: 
            {
              ENCODE_ATTR_SCALAR(Tango::DevFloat, _avp->data, value_);
            } 
            break;
          //-- SCALAR::DEV_DOUBLE ------------------------------------
          case Tango::DEV_DOUBLE: 
            {
              ENCODE_ATTR_SCALAR(Tango::DevDouble, _avp->data, value_);
            } 
            break;
          //-- SCALAR::DEV_STRING ------------------------------------
          case Tango::DEV_STRING: 
            {
              LvStringHdl h = reinterpret_cast<LvStringHdl>(_avp->data);
              std::string s;
              s.assign(reinterpret_cast<char*>((*h)->data), 
                       reinterpret_cast<char*>((*h)->data) + (*h)->length);
		          value_ << s;
            } 
            break;
          default:
            Tango::Except::throw_exception(_CPTC("unexpected TANGO data type"),
                                           _CPTC("binding internal error: unsupported TANGO data type scalar attribute"),
                                           _CPTC("DataAdapter::encode_attr"));
            break;
        } // switch::attr_type
      } 
      break;
      //-- SPECTRUM/IMAGE ATTRIBUTE -------------------------------------------
      case Tango::SPECTRUM: 
      case Tango::IMAGE: 
      {
        //- handle data type
        switch (attr_type) 
        {
          //-- SPECTRUM/IMAGE::DEV_BOOLEAN ------------------------------------
          case Tango::DEV_BOOLEAN: 
            {
              ENCODE_ATTR_ARRAY(Tango::DevVarBooleanArray, Tango::DevBoolean, _avp, value_);
            } 
            break;
          //-- SPECTRUM/IMAGE::DEV_UCHAR --------------------------------------
          case Tango::DEV_UCHAR: 
            {
              ENCODE_ATTR_ARRAY(Tango::DevVarUCharArray, Tango::DevUChar, _avp, value_);
            } 
            break;
          //-- SPECTRUM/IMAGE::DEV_SHORT --------------------------------------
          case Tango::DEV_SHORT: 
            {
              ENCODE_ATTR_ARRAY(Tango::DevVarShortArray, Tango::DevShort, _avp, value_);
            } 
            break;
          //-- SPECTRUM/IMAGE::DEV_USHORT --------------------------------------
          case Tango::DEV_USHORT: 
            {
              ENCODE_ATTR_ARRAY(Tango::DevVarUShortArray, Tango::DevUShort, _avp, value_);
            } 
            break;
          //-- SPECTRUM::DEV_LONG ----------------------------------------------
          case Tango::DEV_LONG: 
            {
              ENCODE_ATTR_ARRAY(Tango::DevVarLongArray, Tango::DevLong, _avp, value_);
            } 
            break;
          //-- SPECTRUM::DEV_ULONG ---------------------------------------------
          case Tango::DEV_ULONG: 
            {
              ENCODE_ATTR_ARRAY(Tango::DevVarULongArray, Tango::DevULong, _avp, value_);
            } 
            break;
          //-- SPECTRUM::DEV_LONG64  -------------------------------------------
          case Tango::DEV_LONG64: 
            {
              ENCODE_ATTR_ARRAY(Tango::DevVarLong64Array, Tango::DevLong64, _avp, value_);
            } 
            break;
          //-- SPECTRUM::DEV_ULONG64  -------------------------------------------
          case Tango::DEV_ULONG64: 
            {
              ENCODE_ATTR_ARRAY(Tango::DevVarULong64Array, Tango::DevULong64, _avp, value_);
            } 
            break;
          //-- SPECTRUM/IMAGE::DEV_FLOAT -------------------------------------
          case Tango::DEV_FLOAT: 
            {
              ENCODE_ATTR_ARRAY(Tango::DevVarFloatArray, Tango::DevFloat, _avp, value_);
            } 
            break;
          //-- SPECTRUM/IMAGE::DEV_DOUBLE -------------------------------------
          case Tango::DEV_DOUBLE: 
            {
              ENCODE_ATTR_ARRAY(Tango::DevVarDoubleArray, Tango::DevDouble, _avp, value_);
            } 
            break;
          //-- SPECTRUM/IMAGE::DEVVAR_STRINGARRAY -----------------------------
          case Tango::DEV_STRING: 
            {
              if (attr_format == Tango::IMAGE)
              {
                Tango::Except::throw_exception(_CPTC("unsupported feature"),
                                               _CPTC("there is currently no support for <image of strings> attribute"),
                                               _CPTC("DataAdapter::encode_attr"));
              }
              LvStringArrayHdl h = reinterpret_cast<LvStringArrayHdl>(_avp->data);
              if (h == 0) 
              {
                Tango::Except::throw_exception(_CPTC("invalid argument"),
                                               _CPTC("binding internal error: unexpected null argument"),
                                               _CPTC("DataAdapter::encode_attr"));
              }
	            Tango::DevVarStringArray *str_array = new Tango::DevVarStringArray((*h)->length);
              if (str_array == 0) 
              {
                Tango::Except::throw_exception(_CPTC("out of memory"),
                                               _CPTC("binding internal error: memory allocation failed"),
                                               _CPTC("DataAdapter::encode_attr"));
              }
              str_array->length((*h)->length);
              LvStringHdl sh = 0;
	            for (int32 i = 0; i < (*h)->length; i++) 
              {
                sh = (*h)->data[i]; 
                (*str_array)[i] = CORBA::string_alloc((*sh)->length + 1);
                if ((*str_array)[i] == 0) 
                {
                  delete str_array;
                  Tango::Except::throw_exception(_CPTC("out of memory"),
                                                 _CPTC("binding internal error: memory allocation failed"),
                                                 _CPTC("DataAdapter::encode_attr"));
                }
                ::memcpy((*str_array)[i], (*sh)->data, (*sh)->length);
                ::memset((*str_array)[i] + (*sh)->length, 0 , 1);
	            }
              value_ << str_array;
            } 
            break;
          default:
            Tango::Except::throw_exception(_CPTC("unexpected or unsupported TANGO data type"),
                                           _CPTC("binding internal error: unsupported TANGO data type for attribute"),
                                           _CPTC("DataAdapter::encode_attr"));
            break;
        } //switch attr_type
      } 
      break;
      //-- UNKNOWN ATTRIBUTE ------------------------------------------
      default:
        std::string d = "unsupported TANGO format for attribute <" + attr_list[_attr_id].name + ">";
        Tango::Except::throw_exception(_CPTC("unexpected TANGO data type"),
                                       _CPTC(d.c_str()),
                                       _CPTC("DataAdapter::encode_attr"));
        break;
    } // switch::attr_format
  } //try
  catch (const Tango::DevFailed&) 
  {
    throw;
	}
  catch (...) 
  {
    Tango::Except::throw_exception(_CPTC("unknown error"),
                                   _CPTC("binding internal error: unknown exception caught"),
                                   _CPTC("DataAdapter::encode_attr"));
  }
  return kNoError;
}

//=============================================================================
// DataAdapter::decode_attr
//=============================================================================
int DataAdapter::decode_attr (DeviceDescriptor* _ddesc, 
                              int _attr_id,
                              Tango::DeviceAttribute& _value,
                              void*& value_)
    throw (Tango::DevFailed)
{
  //- check input
  if (_ddesc == 0) 
  {
    Tango::Except::throw_exception(_CPTC("internal error"),
                                   _CPTC("binding internal error: unexpected null argument"),
                                   _CPTC("DataAdapter::decode_attr"));
  }

  //- extract data
  try 
  {
    //- attr_format
    int frmt = _ddesc->attr_list()[_attr_id].data_format;
    //- is writable?
    int access = _ddesc->attr_list()[_attr_id].writable;
    bool is_rw = (access == Tango::READ_WRITE) || (access == Tango::READ_WITH_WRITE);
    switch (_ddesc->attr_list()[_attr_id].data_type) 
    {
      //-- SCALAR/SPECTRUM/IMAGE::DEV_STATE --------------
      case Tango::DEV_STATE: 
        {
          Decode<Tango::DevState, Tango::DevLong>::attr_scalar(_value, value_);
        }
        break;
      //-- SCALAR/SPECTRUM/IMAGE::DEV_BOOLEAN ------------
      case Tango::DEV_BOOLEAN: 
        {
          DECODE_ATTR_ARRAY(frmt, is_rw, Tango::DevVarBooleanArray, Tango::DevBoolean, _value, value_);
        } 
        break;
      //-- SCALAR/SPECTRUM/IMAGE::DEV_UCHAR --------------
      case Tango::DEV_UCHAR: 
        {
          DECODE_ATTR_ARRAY(frmt, is_rw, Tango::DevVarUCharArray, Tango::DevUChar, _value, value_);
        } 
        break;
      //-- SCALAR/SPECTRUM/IMAGE::DEV_SHORT --------------
      case Tango::DEV_SHORT: 
        {
          DECODE_ATTR_ARRAY(frmt, is_rw, Tango::DevVarShortArray, Tango::DevShort, _value, value_);
        }
        break;
      //-- SCALAR/SPECTRUM/IMAGE::DEV_USHORT -------------
      case Tango::DEV_USHORT: 
        {
          DECODE_ATTR_ARRAY(frmt, is_rw, Tango::DevVarUShortArray, Tango::DevUShort, _value, value_);
        }
        break;
      //-- SCALAR/SPECTRUM/IMAGE::DEV_LONG  --------------
      case Tango::DEV_LONG: 
        {
          DECODE_ATTR_ARRAY(frmt, is_rw, Tango::DevVarLongArray, Tango::DevLong, _value, value_);
        }
        break;
      //-- SCALAR/SPECTRUM/IMAGE::DEV_ULONG  -------------
      case Tango::DEV_ULONG: 
        {
          DECODE_ATTR_ARRAY(frmt, is_rw, Tango::DevVarULongArray, Tango::DevULong, _value, value_);
        }
        break;
       //-- SCALAR/SPECTRUM/IMAGE::DEV_LONG64  -----------
      case Tango::DEV_LONG64: 
        {
          DECODE_ATTR_ARRAY(frmt, is_rw, Tango::DevVarLong64Array, Tango::DevLong64, _value, value_);
        }
        break;
      case Tango::DEV_ULONG64: 
        {
          DECODE_ATTR_ARRAY(frmt, is_rw, Tango::DevVarULong64Array, Tango::DevULong64, _value, value_);
        }
        break;
      //-- SCALAR/SPECTRUM/IMAGE::DEV_FLOAT --------------
      case Tango::DEV_FLOAT: 
        {
          DECODE_ATTR_ARRAY(frmt, is_rw, Tango::DevVarFloatArray, Tango::DevFloat, _value, value_);
        }
        break;
      //-- SCALAR/SPECTRUM/IMAGE::DEV_DOUBLE -------------
      case Tango::DEV_DOUBLE: 
        {
          DECODE_ATTR_ARRAY(frmt, is_rw, Tango::DevVarDoubleArray, Tango::DevDouble, _value, value_);
        }
        break;
      //-- SPECTRUM/IMAGE::DEV_STRING ------------
      case Tango::DEV_STRING: 
        switch (frmt)
        {
          case Tango::SCALAR:
            {
              _value.exceptions(Tango::DeviceAttribute::isempty_flag 
                                  | Tango::DeviceAttribute::wrongtype_flag);
              std::string s;
              _value >> s;
              MgErr err = ::NumericArrayResize(uB,  1L, reinterpret_cast<UHandle*>(&value_), s.size());
              if (err != noErr) 
              {
                Tango::Except::throw_exception(_CPTC("out of memory error"),
                                               _CPTC("binding internal error: memory allocation failed"),
                                               _CPTC("DataAdapter::decode_attr"));
              }
              LvStringHdl lvh = reinterpret_cast<LvStringHdl>(value_);
              (*lvh)->length = s.size();
              ::memcpy((*lvh)->data, s.c_str(), s.size());
            } 
            break;
          case Tango::SPECTRUM:
            {
              //- big trick here!
              //- data is generically passed to the Labview env. through <LvAttributeValue.data> - which
              //- is supposed to be an array of bytes reinterpreted by the extractor VIs. means that we 
              //- can't pass the result as a 1D array of strings. we choose to pass it as an array of bytes
              //- in which the strings (i.e. the ones contained into the Tango::DevVarStringArray) are 
              //- stored contiguously and separated by a '\0' character. the dedicated extractor VI will 
              //- have to rebuild the expected 1D array of srings (transparent op. for end user).     
              Tango::DevVarStringArray *corba_seq;
              _value >> corba_seq;
              //- compute sizeof "generic u8" dest buffer: make room for the strings
              size_t total_size = 0;
              size_t num_strings_in_corba_seq = corba_seq->length();
              for (size_t i = 0; i < num_strings_in_corba_seq; i++) 
                total_size += ::strlen((*corba_seq)[i]);
              //- compute sizeof "generic u8" dest buffer: make room for string separators
              total_size += corba_seq->length(); 
              //- resize dest buffer
              MgErr err = ::NumericArrayResize(uB, 
                                               1L, 
                                               reinterpret_cast<UHandle*>(&value_), 
                                               total_size);
              if (err != noErr) 
              {
                delete corba_seq;
                Tango::Except::throw_exception(_CPTC("out of memory"),
                                               _CPTC("binding internal error: memory allocation failed"),
                                               _CPTC("DataAdapter::decode_attr"));
              }
              //- reinterpret dest as <LvGenericNumArrayHdl>
              LvGenericNumArrayHdl lvh = reinterpret_cast<LvGenericNumArrayHdl>(value_);
              (*lvh)->length = total_size;
              //- reset memory (will put '\0' between strings at the same time)
              char * p = reinterpret_cast<char*>((*lvh)->data);
              ::memset(p, 0, total_size);
              //- copy strings to buffer
              for (size_t i = 0; i < num_strings_in_corba_seq; i++) 
              {
                size_t str_len = ::strlen((*corba_seq)[i]);
                ::memcpy(p, (*corba_seq)[i], str_len);
                p += str_len + 1;
              }
              delete corba_seq;
            }
            break;
          case Tango::IMAGE:
            Tango::Except::throw_exception(_CPTC("unsupported feature"),
                                           _CPTC("there is currently no support for <image of strings> attribute"),
                                           _CPTC("DataAdapter::decode_attr"));
            break;
        }
        break;
      //-- UNKNOWN TYPE -----------------------------------
      default: 
        Tango::Except::throw_exception(_CPTC("unexpected TANGO data type"),
                                       _CPTC("binding internal error: unsupported TANGO data type for attribute"),
                                       _CPTC("DataAdapter::decode_attr"));
        break;
    } // switch attr_type
  } // try
  catch (const Tango::DevFailed &) 
  {
    throw;
	}
  catch (...) 
  {
    Tango::Except::throw_exception(_CPTC("unknown error"),
                                   _CPTC("binding internal error: unknown exception caught"),
                                   _CPTC("DataAdapter::decode_attr"));
  }
  return kNoError;
}

//=============================================================================
// DataAdapter::tango_to_lv_attribute_value
//=============================================================================
void DataAdapter::tango_to_lv_attribute_value (DeviceDescriptor* _ddesc, 
                                               const std::string& _attr_name,
                                               Tango::DeviceAttribute& _tav, 
                                               LvAttributeValuePtr lvav_)
  throw (Tango::DevFailed)
{
  int32 err = 0;

  if (! lvav_) 
  {
    Tango::Except::throw_exception(_CPTC("unexpected null pointer"),
                                   _CPTC("unexpected null LvAttributeValuePtr as input argument"),
                                   _CPTC("DataAdapter::tango_to_lv_attribute_value"));
  }

  try 
  {
    LvStringHdl sh = reinterpret_cast<LvStringHdl>(lvav_->dev_name);
    if (! sh || ! (*sh))
    {
      Tango::Except::throw_exception(_CPTC("unexpected null handle"),
                                     _CPTC("invalid LvAttributeValue [seems to be uninitialized]"),
                                     _CPTC("DataAdapter::tango_to_lv_attribute_value"));
    }

    const std::string& dev_name = _ddesc->name();
    if ((*sh)->length != static_cast<int32>(dev_name.size()))
    {
      err = ::NumericArrayResize(uB,  1L, (UHandle*)&(lvav_->dev_name), dev_name.size());
      if (err != noErr) 
      {
        Tango::Except::throw_exception(_CPTC("out of memory error"),
                                       _CPTC("binding error: memory allocation failed"),
                                       _CPTC("DataAdapter::tango_to_lv_attribute_value"));
      }
      sh = reinterpret_cast<LvStringHdl>(lvav_->dev_name);
      (*sh)->length = dev_name.size();
      ::memcpy((*sh)->data, dev_name.c_str(), dev_name.size());
    }

    sh = reinterpret_cast<LvStringHdl>(lvav_->attr_name);
    if (! sh || ! (*sh))
    {
      Tango::Except::throw_exception(_CPTC("unexpected null handle"),
                                     _CPTC("invalid LvAttributeValue [seems to be uninitialized]"),
                                     _CPTC("DataAdapter::tango_to_lv_attribute_value"));
    }

    if ((*sh)->length != static_cast<int32>(_attr_name.size()))
    {
      err = ::NumericArrayResize(uB,  1L, (UHandle*)&(lvav_->attr_name), _attr_name.size());
      if (err != noErr) 
      {
        Tango::Except::throw_exception(_CPTC("out of memory error"),
                                       _CPTC("binding error: memory allocation failed"),
                                       _CPTC("DataAdapter::tango_to_lv_attribute_value"));
      }
      sh = reinterpret_cast<LvStringHdl>(lvav_->attr_name);
      (*sh)->length = _attr_name.size();
      ::memcpy((*sh)->data, _attr_name.c_str(), _attr_name.size());
    }

    //- attr aid
    int aid = _ddesc->attr_exists(_attr_name);
    //- attr format
    int frmt = _ddesc->attr_list()[aid].data_format;
    //- attr data type
    int type = _ddesc->attr_list()[aid].data_type;

    if (_tav.has_failed() || _tav.is_empty() || _tav.quality == Tango::ATTR_INVALID)
    {
      //- set data type to its default value
      lvav_->data_type = UNKNOWN_DATA_TYPE;
    }
    else
    {
      switch (frmt)
      {
        case Tango::SCALAR:
        {
          switch (type)
          {
            case Tango::DEV_STRING:
              lvav_->data_type = SCA_STRING;
              break;
            case Tango::DEV_BOOLEAN:
              lvav_->data_type = SCA_BOOLEAN;
              break; 
            case Tango::DEV_UCHAR:
              lvav_->data_type = SCA_UCHAR;
              break; 
            case Tango::DEV_SHORT:
              lvav_->data_type = SCA_SHORT;
              break; 
            case Tango::DEV_USHORT:
              lvav_->data_type = SCA_USHORT;
              break; 
            case Tango::DEV_LONG:
              lvav_->data_type = SCA_LONG;
              break; 
            case Tango::DEV_ULONG:
              lvav_->data_type = SCA_ULONG;
              break; 
            case Tango::DEV_LONG64:
              lvav_->data_type = SCA_LONG64;
              break; 
            case Tango::DEV_ULONG64:
              lvav_->data_type = SCA_ULONG64;
              break; 
            case Tango::DEV_FLOAT:
              lvav_->data_type = SCA_FLOAT;
              break;
            case Tango::DEV_DOUBLE:
              lvav_->data_type = SCA_DOUBLE;
              break; 
            case Tango::DEV_STATE:
              lvav_->data_type = SCA_STATE;
              break; 
          }
        }
        break;
        case Tango::SPECTRUM:
        {
          switch (type)
          {
            case Tango::DEV_STRING:
              lvav_->data_type = SPE_STRING;
              break;
            case Tango::DEV_BOOLEAN:
              lvav_->data_type = SPE_BOOLEAN;
              break; 
            case Tango::DEV_UCHAR:
              lvav_->data_type = SPE_UCHAR;
              break; 
            case Tango::DEV_SHORT:
              lvav_->data_type = SPE_SHORT;
              break; 
            case Tango::DEV_USHORT:
              lvav_->data_type = SPE_USHORT;
              break; 
            case Tango::DEV_LONG:
              lvav_->data_type = SPE_LONG;
              break; 
            case Tango::DEV_ULONG:
              lvav_->data_type = SPE_ULONG;
              break; 
            case Tango::DEV_LONG64:
              lvav_->data_type = SPE_LONG64;
              break; 
            case Tango::DEV_ULONG64:
              lvav_->data_type = SPE_ULONG64;
              break;
            case Tango::DEV_FLOAT:
              lvav_->data_type = SPE_FLOAT;
              break;
            case Tango::DEV_DOUBLE:
              lvav_->data_type = SPE_DOUBLE;
              break; 
#if 0
            case Tango::DEV_STATE:
              lvav_->data_type = SPE_STATE;
              break; 
#endif
          }
        }
        break;
        case Tango::IMAGE:
        { 
          switch (type)
          {
            case Tango::DEV_STRING:
              lvav_->data_type = IMG_STRING;
              break;
            case Tango::DEV_BOOLEAN:
              lvav_->data_type = IMG_BOOLEAN;
              break; 
            case Tango::DEV_UCHAR:
              lvav_->data_type = IMG_UCHAR;
              break; 
            case Tango::DEV_SHORT:
              lvav_->data_type = IMG_SHORT;
              break; 
            case Tango::DEV_USHORT:
              lvav_->data_type = IMG_USHORT;
              break; 
            case Tango::DEV_LONG:
              lvav_->data_type = IMG_LONG;
              break; 
            case Tango::DEV_ULONG:
              lvav_->data_type = IMG_ULONG;
              break; 
            case Tango::DEV_LONG64:
              lvav_->data_type = IMG_LONG64;
              break; 
            case Tango::DEV_ULONG64:
              lvav_->data_type = IMG_ULONG64;
              break;
            case Tango::DEV_FLOAT:
              lvav_->data_type = IMG_FLOAT;
              break;
            case Tango::DEV_DOUBLE:
              lvav_->data_type = IMG_DOUBLE;
              break; 
#if 0
            case Tango::DEV_STATE:
              lvav_->data_type = IMG_STATE;
              break; 
#endif
          }
        }
        break;
      } 
    }

    //- set attribute quality (both value and string)
    std::string s;
    if (_tav.has_failed() || _tav.is_empty())
    {
      lvav_->quality_val = 1;
	    s = "INVALID";
    }
    else
    {
      switch (_tav.quality) 
      {
        case Tango::ATTR_VALID:
	        lvav_->quality_val = 0;
	        s = "VALID";
	        break;
        case Tango::ATTR_INVALID:
	        lvav_->quality_val = 1;
	        s = "INVALID";
	        break;
        case Tango::ATTR_ALARM:
	        lvav_->quality_val = 2;
	         s = "ALARM";
	        break;
        default:
	        lvav_->quality_val = -1;
	        s = "UNKNOWN";
	        break;
      }
    }

    sh = reinterpret_cast<LvStringHdl>(lvav_->quality_str);
    if (! sh || ! (*sh))
    {
      Tango::Except::throw_exception(_CPTC("unexpected null handle"),
                                     _CPTC("invalid LvAttributeValue [seems to be uninitialized]"),
                                     _CPTC("DataAdapter::tango_to_lv_attribute_value"));
    }

    err = ::NumericArrayResize(uB,  1L, (UHandle*)&(lvav_->quality_str), s.size());
    if (err != noErr) 
    {
      Tango::Except::throw_exception(_CPTC("out of memory error"),
                                     _CPTC("binding error: memory allocation failed"),
                                     _CPTC("DataAdapter::tango_to_lv_attribute_value"));
    }
    sh = reinterpret_cast<LvStringHdl>(lvav_->quality_str);
    (*sh)->length = s.size();
    ::memcpy((*sh)->data, s.c_str(), s.size());

    //- set timestamp
    if (_tav.has_failed() || _tav.is_empty())
    {
      lvav_->timestamp = 0;
    }
    else
    {
      //- set timestamp
      lvav_->timestamp = kTIME_OFFSET +
                        (double)(_tav.time.tv_sec) + 
				                 1.E-6 * (double)(_tav.time.tv_usec) + 
				                 1.E-9 * (double)(_tav.time.tv_nsec); 
    }

    //- set dims
    if (_tav.has_failed() || _tav.is_empty() || _tav.quality == Tango::ATTR_INVALID)
    {
      lvav_->dim_x = 0;
      lvav_->dim_y = 0;
    }
    else
    {
      Tango::AttributeDimension r_dim = _tav.get_r_dimension();
      lvav_->dim_x = r_dim.dim_x;
      lvav_->dim_y = r_dim.dim_y;
    }

    //- set individual error code & error stack
    if (_tav.has_failed() || _tav.is_empty() || _tav.quality == Tango::ATTR_INVALID)
    {
      //- set individual error code 
      lvav_->error_code = -1;
      //- set individual error stack
      try
      {
        if (_tav.has_failed())
        {
          throw Tango::DevFailed(_tav.get_err_stack());
        }
        else
        {
          Tango::Except::throw_exception(_CPTC("data extraction failed"),
                                         _CPTC("empty or invalid attribute value received from device"),
                                         _CPTC("DataAdapter::tango_to_lv_attribute_value"));
        }
      }
      catch (const Tango::DevFailed &e)
      {
        //- set individual error stack
        ERROR_STACK->devfailed(e, lvav_->error_stack);
      }
    }
    else
    {
      //- set individual error code 
      lvav_->error_code = 0;
      //- set individual error stack
      ERROR_STACK->no_error("read_attributes", lvav_->error_stack); 
    }

    //- no data to decode...
    if (_tav.has_failed() || _tav.is_empty() || _tav.quality == Tango::ATTR_INVALID)
    {
      lvav_->data = 0;
      return;
    }

    //- decode data (may throw an exception)
    try 
    {
      this->decode_attr(_ddesc, aid, _tav, lvav_->data);
    }
    catch (const Tango::DevFailed& e) 
    {
      //- set individual error code 
      lvav_->error_code = -1;
      //- set individual error stack
      ERROR_STACK->devfailed(e, lvav_->error_stack); 
    }
    catch (...) 
    {
      //- set individual error code 
      lvav_->error_code = -1;
      //- set individual error stack 
      ERROR_STACK->unknown("DataAdapter::tango_to_lv_attribute_value", lvav_->error_stack);
    }
  }
  catch (Tango::DevFailed& e)
  {
    Tango::Except::re_throw_exception(e,
                                      _CPTC("data extraction failed"),
                                      _CPTC("Tango exception caught while trying to extract attribute value"),
                                      _CPTC("DataAdapter::tango_to_lv_attribute_value"));
  }
  catch (...) 
  {
    Tango::Except::throw_exception(_CPTC("data extraction failed"),
                                   _CPTC("unknown exception caught while trying to extract attribute value"),
                                   _CPTC("DataAdapter::tango_to_lv_attribute_value"));
  }
}

