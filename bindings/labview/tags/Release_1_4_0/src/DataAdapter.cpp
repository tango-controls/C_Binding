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
template <class _A, class _T> class Encode
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
#if TBFL_LITTLE_ENDIAN_PLATFORM == 1
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
      default:
        Tango::Except::throw_exception(_CPTC("unexpected data type"),
                                       _CPTC("no support for > 64-bits data type"),
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
#if (TBFL_LITTLE_ENDIAN_PLATFORM == 1)
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
      default:
        Tango::Except::throw_exception(_CPTC("unexpected data type"),
                                       _CPTC("no support for > 64-bits data type"),
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
    long np = _avp->dim_x * ((_avp->dim_y) ? _avp->dim_y : 1);
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
  static void lv_char_array_to_corba_seq (LvGenericNumArrayPtr _lv_data, _A* corba_seq_)
    throw (Tango::DevFailed)
  {
    if (_lv_data == 0 || corba_seq_ == 0) 
    {
      Tango::Except::throw_exception(_CPTC("invalid input argument"),
                                     _CPTC("binding internal error: unexpected null input argument"),
                                     _CPTC("Encode::lv_array_to_corba_seq"));
    }
#if (TBFL_LITTLE_ENDIAN_PLATFORM == 1)
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
      default:
        Tango::Except::throw_exception(_CPTC("unexpected data type"),
                                       _CPTC("no support for > 64-bits data type"),
                                       _CPTC("Encode::lv_array_to_corba_seq"));
        break;
    }
#else
    ::memcpy(corba_seq_->get_buffer(), _lv_data->data, corba_seq_->length());
#endif
  }
};
//-----------------------------------------------------------------------------
#define ENCODE_CMD_SCALAR(_T,_ARG_IN, _TANGO_DATA) \
  Encode<_T, DUMMY_TMPL_ARG>::cmd_scalar(_ARG_IN, _TANGO_DATA);
//-----------------------------------------------------------------------------
#define ENCODE_ATTR_SCALAR(_T,_ARG_IN, _TANGO_DATA) \
  Encode<_T, DUMMY_TMPL_ARG>::attr_scalar(_ARG_IN, _TANGO_DATA);
//-----------------------------------------------------------------------------
#define ENCODE_CMD_ARRAY(_A, _T, _ARG_IN, _TANGO_DATA) \
  Encode<_A,_T>::cmd_array(_ARG_IN, _TANGO_DATA);
//-----------------------------------------------------------------------------
#define LV_CHAR_ARRAY_TO_CORBA_SEQ(_A, _T, _ARG_IN, _TANGO_DATA) \
  Encode<_A,_T>::lv_char_array_to_corba_seq(_ARG_IN, _TANGO_DATA);
//-----------------------------------------------------------------------------
#define ENCODE_ATTR_ARRAY(_A, _T, _ATTR_VAL, _TANGO_DATA) \
  Encode<_A,_T>::attr_array(_ATTR_VAL, _TANGO_DATA);
//-----------------------------------------------------------------------------

//=============================================================================
// DECODE CLASS & MACROS
//=============================================================================
//-----------------------------------------------------------------------------
// TEMPLATE CLASS: Decode
//-----------------------------------------------------------------------------
template <class _A, class _T> class Decode 
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

    long err = ::NumericArrayResize(uB, 
                                    1L, 
                                    reinterpret_cast<UHandle*>(&lv_data_), 
                                    sizeof(_A));
    if (err != noErr) 
    {
      Tango::Except::throw_exception(_CPTC("out of memory"),
                                     _CPTC("binding internal error: memory allocation failed"),
                                     _CPTC("Decode::cmd_scalar"));
    } 
    LvGenericNumArrayHdl lvh = reinterpret_cast<LvGenericNumArrayHdl>(lv_data_);
    (*lvh)->length = sizeof(_A);
#if (TBFL_LITTLE_ENDIAN_PLATFORM == 1)
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
      default:
        Tango::Except::throw_exception(_CPTC("unexpected data type"),
                                       _CPTC("no support for > 64-bits data type"),
                                       _CPTC("Encode::cmd_scalar"));
        break;
    }
#endif
    *(reinterpret_cast<_A *>((*lvh)->data)) = a; 
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

    size_t read_size = static_cast<size_t>(corba_seq->length());

    Decode::corba_seq_to_lv_array(read_size, corba_seq, lv_data_);
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

    long err = ::NumericArrayResize(iB, 
                                    1L, 
                                    reinterpret_cast<UHandle*>(&lv_data_), 
                                    sizeof(_T));
    if (err != noErr) 
    {
      Tango::Except::throw_exception(_CPTC("out of memory"),
                                     _CPTC("binding internal error: memory allocation failed"),
                                     _CPTC("Decode:::attr_scalar"));
    }  
    LvGenericNumArrayHdl lvh = reinterpret_cast<LvGenericNumArrayHdl>(lv_data_);
    (*lvh)->length = sizeof(_T);
#if (TBFL_LITTLE_ENDIAN_PLATFORM == 1)
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
      default:
        Tango::Except::throw_exception(_CPTC("unexpected data type"),
                                       _CPTC("no support for > 64-bits data type"),
                                       _CPTC("Encode::attr_scalar"));
        break;
    }
#endif
    *(reinterpret_cast<_A *>((*lvh)->data)) = a; 
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

    size_t data_size = 0;
#if defined(HAS_TANGO_5_OR_SUP)
    switch  (attr_format)
    {
      case Tango::SCALAR:
        {
          data_size = attr_writable ? 2 : 1;
        }
        break;
      case Tango::SPECTRUM:
        {
          data_size = _tango_data.get_nb_read();
        }
        break;
      case Tango::IMAGE:
        {
          Tango::AttributeDimension r_dim = _tango_data.get_r_dimension();
          data_size = r_dim.dim_x * r_dim.dim_y;
        }
        break;
    }
    _A * corba_seq;
    _tango_data >> corba_seq;
#else
    _A * corba_seq;
    _tango_data >> corba_seq;
    data_size = static_cast<size_t>(corba_seq->length());
#endif

    try 
    {
      Decode::corba_seq_to_lv_array(data_size, corba_seq, lv_data_);
    }
    catch (...)
    {
      delete corba_seq;
      throw;
    }
    delete corba_seq;
  }

  //-----------------------------------------------------------------------------
  // Decode::corba_seq_to_lv_array
  //-----------------------------------------------------------------------------
  static void corba_seq_to_lv_array (size_t data_size, const _A* _corba_seq, void*& lv_data_)
    throw (Tango::DevFailed)
  {
    long err = ::NumericArrayResize(uB, 
                                    1L, 
                                    reinterpret_cast<UHandle*>(&lv_data_), 
                                    data_size * sizeof(_T));
    if (err != noErr) 
    {
      Tango::Except::throw_exception(_CPTC("out of memory"),
                                     _CPTC("binding internal error: memory allocation failed"),
                                     _CPTC("Decode::corba_seq_to_lv_array"));
    }  
    LvGenericNumArrayHdl lvh = reinterpret_cast<LvGenericNumArrayHdl>(lv_data_);
    (*lvh)->length = data_size * sizeof(_T);
#if (TBFL_LITTLE_ENDIAN_PLATFORM == 1)
    switch(sizeof(_T))
    {
      case 1:
        ::memcpy((*lvh)->data, _corba_seq->get_buffer(), (*lvh)->length);
        break;
      case 2:
        Endianness::swap_2_array(reinterpret_cast<const char*>(_corba_seq->get_buffer()),
                                 reinterpret_cast<char*>((*lvh)->data),
                                 data_size);
        break;
      case 4:
        Endianness::swap_4_array(reinterpret_cast<const char*>(_corba_seq->get_buffer()),
                                 reinterpret_cast<char*>((*lvh)->data),
                                 data_size);
        break;
      case 8:
        Endianness::swap_8_array(reinterpret_cast<const char*>(_corba_seq->get_buffer()),
                                 reinterpret_cast<char*>((*lvh)->data),
                                 data_size);
        break;
      default:
        Tango::Except::throw_exception(_CPTC("unexpected data type"),
                                       _CPTC("no support for > 64-bits data type"),
                                       _CPTC("Decode::corba_seq_to_lv_array"));
        break;
    }
#else
    ::memcpy((*lvh)->data, _corba_seq->get_buffer(), (*lvh)->length);
#endif
  } 
}; 
//-----------------------------------------------------------------------------
#define DECODE_CMD_SCALAR(_T, _ARG_OUT, _TANGO_DATA) \
  Decode<_T, DUMMY_TMPL_ARG>::cmd_scalar(_TANGO_DATA, _ARG_OUT);
//-----------------------------------------------------------------------------
#define DECODE_ATTR_SCALAR(_T, _ARG_OUT, _TANGO_DATA) \
  Decode<_T, DUMMY_TMPL_ARG>::attr_scalar(_TANGO_DATA, _ARG_OUT);
//-----------------------------------------------------------------------------
#define CORBA_SEQ_TO_LV_ARRAY(_A, _T, _TANGO_DATA, _LV_DATA) \
  Decode<_A,_T>::corba_seq_to_lv_array((_TANGO_DATA)->length(), _TANGO_DATA, _LV_DATA);
//-----------------------------------------------------------------------------
#define DECODE_CMD_ARRAY(_A, _T, _ARG_OUT, _TANGO_DATA) \
  Decode<_A,_T>::cmd_array(_TANGO_DATA, _ARG_OUT);
//-----------------------------------------------------------------------------
#define DECODE_ATTR_ARRAY(_F, _W, _A, _T, _ARG_OUT, _TANGO_DATA) \
  Decode<_A,_T>::attr_array(_F, _W, _TANGO_DATA, _ARG_OUT);
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
  if (DataAdapter::instance_) {
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
int DataAdapter::encode_argin (DeviceDesc* _ddesc, 
                               int _cmd_id, 
                               LvArgIOPtr _argin,
                               Tango::DeviceData& dd_in_)
    throw (Tango::DevFailed)
{
  if (_ddesc == 0) {
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
          //- encode long data
          LvGenericNumArrayHdl lvgah = reinterpret_cast<LvGenericNumArrayHdl>(_argin->num_data);
          dvls_array->lvalue.length((*lvgah)->length / sizeof(long));
          LV_CHAR_ARRAY_TO_CORBA_SEQ(Tango::DevVarLongArray, long, *lvgah, &(dvls_array->lvalue));
          //- encode string data
          LvStringArrayHdl sah = reinterpret_cast<LvStringArrayHdl>(_argin->str_data);
          if (sah == 0) {
            Tango::Except::throw_exception(_CPTC("invalid input argument"),
                                           _CPTC("binding internal error: unexpected null input argument"),
                                           _CPTC("DataAdapter::encode_argin"));
          }
          dvls_array->svalue.length((*sah)->length);
          LvStringHdl sh = 0;
	        for (long i = 0; i < (*sah)->length; i++) 
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
	        for (long i = 0; i < (*sah)->length; i++) 
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
	        for (long i = 0; i < (*h)->length; i++) 
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
          ENCODE_CMD_ARRAY(Tango::DevVarDoubleArray, double, _argin->num_data, dd_in_);
        } 
        break;
      //-- DEVVAR_FLOATARRAY ---------------------------
      case Tango::DEVVAR_FLOATARRAY: 
        {
          ENCODE_CMD_ARRAY(Tango::DevVarFloatArray, float, _argin->num_data, dd_in_);
        } 
        break;
      //-- DEVVAR_LONGARRAY ---------------------------
      case Tango::DEVVAR_LONGARRAY: 
        {
          ENCODE_CMD_ARRAY(Tango::DevVarLongArray, long, _argin->num_data, dd_in_);
        } 
        break;
      //-- DEVVAR_ULONGARRAY ---------------------------
      case Tango::DEVVAR_ULONGARRAY: 
        {
          ENCODE_CMD_ARRAY(Tango::DevVarULongArray, unsigned long, _argin->num_data, dd_in_);
        } 
        break;
      //-- DEVVAR_SHORTARRAY ---------------------------
      case Tango::DEVVAR_SHORTARRAY: 
        {
          ENCODE_CMD_ARRAY(Tango::DevVarShortArray, short, _argin->num_data, dd_in_);
        } 
        break;
      //-- DEVVAR_USHORTARRAY --------------------------
      case Tango::DEVVAR_USHORTARRAY: 
        {
          ENCODE_CMD_ARRAY(Tango::DevVarUShortArray, unsigned short, _argin->num_data, dd_in_);
        } 
        break;
      //-- DEVVAR_CHARARRAY
      case Tango::DEVVAR_CHARARRAY: 
        {
          ENCODE_CMD_ARRAY(Tango::DevVarCharArray, unsigned char, _argin->num_data, dd_in_);
        } break;
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
 		      ENCODE_CMD_SCALAR(bool, _argin->num_data, dd_in_); 
        } 
        break;
      //-- DEV_SHORT -----------------------------------
      case Tango::DEV_SHORT: 
        {
 		      ENCODE_CMD_SCALAR(short, _argin->num_data, dd_in_); 
        } 
        break;
      //-- DEV_SHORT -----------------------------------
      case Tango::DEV_USHORT: 
        {
 		      ENCODE_CMD_SCALAR(unsigned short, _argin->num_data, dd_in_); 
        } 
        break;
      //-- DEV_LONG ------------------------------------
      case Tango::DEV_LONG: 
        {
 		      ENCODE_CMD_SCALAR(long, _argin->num_data, dd_in_); 
        } 
        break;
      //-- DEV_ULONG -----------------------------------
      case Tango::DEV_ULONG: 
        {
 		      ENCODE_CMD_SCALAR(unsigned long, _argin->num_data, dd_in_); 
        } 
        break;
      //-- DEV_FLOAT -----------------------------------
      case Tango::DEV_FLOAT: 
        {
 		      ENCODE_CMD_SCALAR(float, _argin->num_data, dd_in_); 
        } 
        break;
      //-- DEV_DOUBLE ----------------------------------
      case Tango::DEV_DOUBLE: 
        {
 		      ENCODE_CMD_SCALAR(double, _argin->num_data, dd_in_); 
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
int DataAdapter::decode_argout (DeviceDesc* _ddesc, 
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
          //- decode long data
          CORBA_SEQ_TO_LV_ARRAY(Tango::DevVarLongArray, long, &(corba_struct->lvalue), argout_->num_data);
          //- decode string data
          LvStringArrayHdl sah = argout_->str_data;
          if (sah == 0) {
            Tango::Except::throw_exception(_CPTC("invalid output argument"),
                                           _CPTC("binding internal error: unexpected null output argument"),
                                           _CPTC("DataAdapter::decode_argout"));
          }
          long err = ::NumericArrayResize(iL, 
                                          1L, 
                                          reinterpret_cast<UHandle*>(&sah), 
                                          corba_struct->svalue.length());
          if (err != noErr) {
            Tango::Except::throw_exception(_CPTC("out of memory"),
                                           _CPTC("binding internal error: memory allocation failed"),
                                           _CPTC("DataAdapter::decode_argout"));
          }
		      (*sah)->length = corba_struct->svalue.length();
          ::memset((*sah)->data, 0, (*sah)->length * sizeof(long));
          size_t l = 0;
          LvStringHdl sh = 0;
		      for (long i = 0; i < (*sah)->length; i++) 
          {
            sh = 0;
			      l = ::strlen(corba_struct->svalue[i]);
            err = ::NumericArrayResize(iB, 1L, reinterpret_cast<UHandle*>(&sh), l);
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
           //- decode long data
          CORBA_SEQ_TO_LV_ARRAY(Tango::DevVarDoubleArray, double, &(corba_struct->dvalue), argout_->num_data);
          //- decode string data
          LvStringArrayHdl sah = argout_->str_data;
          if (sah == 0) {
            Tango::Except::throw_exception(_CPTC("invalid output argument"),
                                           _CPTC("binding internal error: unexpected null output argument"),
                                           _CPTC("DataAdapter::decode_argout"));
          }
          long err = ::NumericArrayResize(iL, 
                                          1L, 
                                          reinterpret_cast<UHandle*>(&sah), 
                                          corba_struct->svalue.length());
          if (err != noErr) {
            Tango::Except::throw_exception(_CPTC("out of memory"),
                                           _CPTC("binding internal error: memory allocation failed"),
                                           _CPTC("DataAdapter::decode_argout"));
          }
		      (*sah)->length = corba_struct->svalue.length();
          ::memset((*sah)->data, 0, (*sah)->length * sizeof(long));
          size_t l = 0;
          LvStringHdl sh = 0;
		      for (long i = 0; i < (*sah)->length; i++) 
          {
            sh = 0;
			      l = ::strlen(corba_struct->svalue[i]);
            err = ::NumericArrayResize(iB, 1L, reinterpret_cast<UHandle*>(&sh), l);
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
          long err = ::NumericArrayResize(iL, 
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
          ::memset((*lvh)->data, 0, (*lvh)->length * sizeof(long));
          size_t l = 0;
          LvStringHdl sh = 0;
		      for (long i = 0; i < (*lvh)->length; i++) 
          {
            sh = 0;
			      l = ::strlen((*corba_seq)[i]);
            err = ::NumericArrayResize(iB, 1L, reinterpret_cast<UHandle*>(&sh), l);
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
      //-- DEVVAR_DOUBLEARRAY -------------------------
      case Tango::DEVVAR_DOUBLEARRAY: 
        {
          DECODE_CMD_ARRAY(Tango::DevVarDoubleArray, double, argout_->num_data, _dd_out);
        } 
        break;
      //-- DEVVAR_FLOATARRAY -------------------------
      case Tango::DEVVAR_FLOATARRAY: 
        {
          DECODE_CMD_ARRAY(Tango::DevVarFloatArray, float, argout_->num_data, _dd_out);
        } 
        break;
      //-- DEVVAR_LONGARRAY -------------------------
      case Tango::DEVVAR_LONGARRAY: 
        { 
          DECODE_CMD_ARRAY(Tango::DevVarLongArray, long, argout_->num_data, _dd_out);
        } 
        break;
      //-- DEVVAR_ULONGARRAY -------------------------
      case Tango::DEVVAR_ULONGARRAY: 
        {   
          DECODE_CMD_ARRAY(Tango::DevVarULongArray, unsigned long, argout_->num_data, _dd_out);
        } 
        break;
      //-- DEVVAR_SHORTARRAY -------------------------
      case Tango::DEVVAR_SHORTARRAY: 
        {
          DECODE_CMD_ARRAY(Tango::DevVarShortArray, short, argout_->num_data, _dd_out);
        } 
        break;
      //-- DEVVAR_USHORTARRAY -------------------------
      case Tango::DEVVAR_USHORTARRAY: 
        {
          DECODE_CMD_ARRAY(Tango::DevVarUShortArray, unsigned short, argout_->num_data, _dd_out);
        } 
        break;
      //-- DEVVAR_CHARARRAY -------------------------
      case Tango::DEVVAR_CHARARRAY: 
        {
          DECODE_CMD_ARRAY(Tango::DevVarCharArray, unsigned char, argout_->num_data, _dd_out);
        } 
        break;
      //-- DEV_STRING --------------------------------
      case Tango::DEV_STRING: 
        {
          std::string s;
          _dd_out >> s;
          long err = ::NumericArrayResize(iB, 
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
          DECODE_CMD_SCALAR(bool, argout_->num_data, _dd_out);
        }  
        break;
      //-- DEV_SHORT ---------------------------------
      case Tango::DEV_SHORT: 
        {
          DECODE_CMD_SCALAR(short, argout_->num_data, _dd_out);
        } 
        break;
      //-- DEV_USHORT --------------------------------
      case Tango::DEV_USHORT: 
        {
          DECODE_CMD_SCALAR(unsigned short, argout_->num_data, _dd_out);
        }  
        break;
      //-- DEV_LONG ---------------------------------
      case Tango::DEV_LONG: 
        {
          DECODE_CMD_SCALAR(long, argout_->num_data, _dd_out);
        }  
        break;      
      //-- DEV_ULONG ---------------------------------
      case Tango::DEV_ULONG: 
        {
          DECODE_CMD_SCALAR(unsigned long, argout_->num_data, _dd_out);
        }  
        break;
      //-- DEV_FLOAT --------------------------------
      case Tango::DEV_FLOAT: 
        {
          DECODE_CMD_SCALAR(float, argout_->num_data, _dd_out);
        } 
        break;
      //-- DEV_DOUBLE -------------------------------
      case Tango::DEV_DOUBLE: 
        {
          DECODE_CMD_SCALAR(double, argout_->num_data, _dd_out);
        } 
        break;
      //-- DEV_STATE --------------------------------
      case Tango::DEV_STATE: 
        {
          DECODE_CMD_SCALAR(Tango::DevState, argout_->num_data, _dd_out);
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
int DataAdapter::encode_attr (DeviceDesc* _ddesc,
                              int _attr_id,
                              LvAttributeValuePtr _avp,
                              Tango::DeviceAttribute& value_)
    throw (Tango::DevFailed)
{
  //- check input
  if (_ddesc == 0) {
    Tango::Except::throw_exception(_CPTC("internal error"),
                                   _CPTC("binding internal error: unexpected null argument"),
                                   _CPTC("DataAdapter::decode_attr"));
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
  value_.time.tv_sec  = now.time;
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
#if defined(HAS_TANGO_5_OR_SUP)
          //-- SCALAR::DEV_BOOLEAN -----------------------------------
          case Tango::DEV_BOOLEAN: 
            {
              ENCODE_ATTR_SCALAR(bool, _avp->data, value_);
            } 
            break;
          //-- SCALAR::DEV_UCHAR -------------------------------------
          case Tango::DEV_UCHAR: 
            {
              ENCODE_ATTR_SCALAR(unsigned char, _avp->data, value_);
            } 
            break;
#endif
          //-- SCALAR::DEV_SHORT -------------------------------------
          case Tango::DEV_SHORT: 
            {
              ENCODE_ATTR_SCALAR(short, _avp->data, value_);
            } 
            break;
#if defined(HAS_TANGO_5_OR_SUP)
          //-- SCALAR::DEV_USHORT ------------------------------------
          case Tango::DEV_USHORT: 
            {
              ENCODE_ATTR_SCALAR(unsigned short, _avp->data, value_);
            } 
            break;
#endif
          //-- SCALAR::DEV_LONG --------------------------------------
          case Tango::DEV_LONG: 
            {
              ENCODE_ATTR_SCALAR(long, _avp->data, value_);
            } 
            break;
#if defined(HAS_TANGO_5_OR_SUP)
          //-- SCALAR::DEV_FLOAT -------------------------------------
          case Tango::DEV_FLOAT: 
            {
              ENCODE_ATTR_SCALAR(float, _avp->data, value_);
            } 
            break;
#endif
          //-- SCALAR::DEV_DOUBLE ------------------------------------
          case Tango::DEV_DOUBLE: 
            {
              ENCODE_ATTR_SCALAR(double, _avp->data, value_);
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
#if defined(HAS_TANGO_5_OR_SUP)
      //-- SPECTRUM/IMAGE ATTRIBUTE -----------------------------------------
      case Tango::SPECTRUM: 
      case Tango::IMAGE: 
      {
        //- handle data type
        switch (attr_type) 
        {
          //-- SPECTRUM::DEV_BOOLEAN ------------------------------------
          case Tango::DEV_BOOLEAN: 
            {
              ENCODE_ATTR_ARRAY(Tango::DevVarBooleanArray, bool, _avp, value_);
            } 
            break;
          //-- SPECTRUM::DEV_UCHAR --------------------------------------
          case Tango::DEV_UCHAR: 
            {
              ENCODE_ATTR_ARRAY(Tango::DevVarUCharArray, unsigned char, _avp, value_);
            } 
            break;
          //-- SPECTRUM::DEV_SHORT --------------------------------------
          case Tango::DEV_SHORT: 
            {
              ENCODE_ATTR_ARRAY(Tango::DevVarShortArray, short, _avp, value_);
            } 
            break;
          //-- SPECTRUM::DEV_USHORT --------------------------------------
          case Tango::DEV_USHORT: 
            {
              ENCODE_ATTR_ARRAY(Tango::DevVarUShortArray, unsigned short, _avp, value_);
            } 
            break;
          //-- SPECTRUM::DEV_LONG ---------------------------------------
          case Tango::DEV_LONG: 
            {
              ENCODE_ATTR_ARRAY(Tango::DevVarLongArray, long, _avp, value_);
            } 
            break;
          //-- SPECTRUM::DEV_FLOAT -------------------------------------
          case Tango::DEV_FLOAT: 
            {
              ENCODE_ATTR_ARRAY(Tango::DevVarFloatArray, float, _avp, value_);
            } 
            break;
          //-- SPECTRUM::DEV_DOUBLE -------------------------------------
          case Tango::DEV_DOUBLE: 
            {
              ENCODE_ATTR_ARRAY(Tango::DevVarDoubleArray, double, _avp, value_);
            } 
            break;
          default:
            Tango::Except::throw_exception(_CPTC("unexpected TANGO data type"),
                                           _CPTC("binding internal error: unsupported TANGO data type for attribute"),
                                           _CPTC("DataAdapter::encode_attr"));
            break;
        } //switch attr_type
      } 
      break; 
#endif //- HAS_TANGO_5_OR_SUP
      //-- UNKNOWN ATTRIBUTE ------------------------------------------
      default:
        std::string d = "unsupported TANGO format for attribute <" + attr_list[_attr_id].name + ">";
        Tango::Except::throw_exception(_CPTC("unexpected TANGO data type"),
                                       _CPTC(d.c_str()),
                                       _CPTC("DataAdapter::decode_attr"));
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
                                   _CPTC("DataAdapter::decode_attr"));
  }
  return kNoError;
}

//=============================================================================
// DataAdapter::decode_attr
//=============================================================================
int DataAdapter::decode_attr (DeviceDesc* _ddesc, 
                              int _attr_id,
                              Tango::DeviceAttribute& _value,
                              void*& value_)
    throw (Tango::DevFailed)
{
  //- check input
  if (_ddesc == 0) {
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
      //-- SCALAR::DEV_STRING ----------------------------
      case Tango::DEV_STRING: 
        {
          _value.exceptions(Tango::DeviceAttribute::isempty_flag 
                             | Tango::DeviceAttribute::wrongtype_flag);
          std::string s;
          _value >> s;
          long err = ::NumericArrayResize(iB, 1L, (UHandle*)&value_, s.size());
          if (err != noErr) {
            Tango::Except::throw_exception(_CPTC("out of memory error"),
                                           _CPTC("binding internal error: memory allocation failed"),
                                           _CPTC("DataAdapter::decode_attr"));
          }
          LvStringHdl lvh = reinterpret_cast<LvStringHdl>(value_);
          (*lvh)->length = s.size();
          ::memcpy((*lvh)->data, s.c_str(), s.size());
        } 
        break;
#if defined(HAS_TANGO_5_OR_SUP)
      //-- SCALAR/SPECTRUM/IMAGE::DEV_BOOLEAN ------------
      case Tango::DEV_BOOLEAN: 
        {
          DECODE_ATTR_ARRAY(frmt, is_rw, Tango::DevVarBooleanArray, bool, value_, _value);
        } 
        break;
      //-- SCALAR/SPECTRUM/IMAGE::DEV_UCHAR --------------
      case Tango::DEV_UCHAR: 
        {
          DECODE_ATTR_ARRAY(frmt, is_rw, Tango::DevVarUCharArray, unsigned char, value_, _value);
        } 
        break;
#endif
      //-- SCALAR/SPECTRUM/IMAGE::DEV_SHORT --------------
      case Tango::DEV_SHORT: 
        {
          DECODE_ATTR_ARRAY(frmt, is_rw, Tango::DevVarShortArray, short, value_, _value);
        } 
        break;
#if defined(HAS_TANGO_5_OR_SUP)
      //-- SCALAR/SPECTRUM/IMAGE::DEV_USHORT -------------
      case Tango::DEV_USHORT: 
        {
          DECODE_ATTR_ARRAY(frmt, is_rw, Tango::DevVarUShortArray, unsigned short, value_, _value);
        } 
        break;
#endif
      //-- SCALAR/SPECTRUM/IMAGE::DEV_LONG  --------------
      case Tango::DEV_LONG: 
        {
          DECODE_ATTR_ARRAY(frmt, is_rw, Tango::DevVarLongArray, long, value_, _value);
        } 
        break;
#if defined(HAS_TANGO_5_OR_SUP)
      //-- SCALAR/SPECTRUM/IMAGE::DEV_FLOAT --------------
      case Tango::DEV_FLOAT: 
        {
          DECODE_ATTR_ARRAY(frmt, is_rw, Tango::DevVarFloatArray, float, value_, _value);
        } 
        break;
#endif
      //-- SCALAR/SPECTRUM/IMAGE::DEV_DOUBLE -------------
      case Tango::DEV_DOUBLE: 
        {
          DECODE_ATTR_ARRAY(frmt, is_rw, Tango::DevVarDoubleArray, double, value_, _value);
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



