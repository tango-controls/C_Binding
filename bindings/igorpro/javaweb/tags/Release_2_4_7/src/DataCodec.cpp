// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for Igor Pro
//
// = FILENAME
//   DataCodec.cpp
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

//=============================================================================
// IMPL_OPTION: reinject data into the Tango::DeviceAttribute after extraction
//=============================================================================
#define __REUSABLE_ATTR_VALUE__

//=============================================================================
// DEPENDENCIES
//=============================================================================
#include <math.h>
#include "XDK_StandardHeaders.h"
#include "DevRepository.h"
#include "DataCodec.h"

#if !defined (__XDK_INLINE__)
  #include "DataCodec.i"
#endif 

#if defined(WIN32)
# pragma warning (disable:4800)
#endif

//=============================================================================
// CONSTs
//=============================================================================
// # of secs elapsed between 1/1/1904 (Igor ref) and 1/1/1970 (TANGO ref)
static const double k_time_offset = 2082844800.0;

//=============================================================================
// DEFINEs
//=============================================================================
#define __NAN__ ::sqrt(-1)

//=============================================================================
// STATIC MEMBERS
//=============================================================================
DataCodec* DataCodec::instance_ = 0;

//=============================================================================
// DataCodec::init
//=============================================================================
int DataCodec::init ()
{
  if (DataCodec::instance_ != 0)
    return 0;

  DataCodec::instance_ = new DataCodec;

  return (DataCodec::instance_) ? 0 : -1;
}

//=============================================================================
// DataCodec::cleanup
//=============================================================================
void DataCodec::cleanup ()
{
  if (DataCodec::instance_) 
  {
    delete DataCodec::instance_;
    DataCodec::instance_ = 0;
  }
}

//=============================================================================
// DataCodec::DataCodec 
//=============================================================================
DataCodec::DataCodec ()
{
 // no-op ctor
}

//=============================================================================
// DataCodec::~DataCodec 
//=============================================================================
DataCodec::~DataCodec ()
{
 // no-op dtor
}

//=============================================================================
// DataCodec::encode_argin
//=============================================================================
int DataCodec::encode_argin (DevDescriptor* _ddesc, 
                             const std::string& _argin, 
                             int _cmd_id,
                             Tango::DeviceData& dd_in_)
{
  if (_ddesc == 0) 
  {
    XDK_UTILS->set_error("XOP internal error",
                         "unexpected NULL argument",
                         "DataCodec::encode_argin");
    return kError;
  }

  DFHndl dfh;
  std::string obj_name;
  if (XDK_UTILS->get_obj_df(_argin, dfh, obj_name) != kNoError) 
  {
    XDK_UTILS->set_error("XOP internal error",
                         "XDK_Utils::get_obj_df failed",
                         "DataCodec::encode_argin");
    return kError;
  }
  
  //- does the user provide the actual argin or a string containing argin?
  //- first try to get a global string var named <_argin> in <dfh>
  //- if there is no string var named <_argin> in <dfh> <_argin> is unchanged
  //- otherwise <_argin> becomes equal to its content  
  XDK_UTILS->get_df_obj(dfh, obj_name, obj_name);

  try 
  {
    int argin_type = (_ddesc->cmd_list())[_cmd_id].in_type;
    switch (argin_type) 
    {
      //-------------------------------------------------
      case Tango::DEV_VOID: 
      {
        return kNoError; 
      } break;
      //-------------------------------------------------
      case Tango::DEVVAR_LONGSTRINGARRAY: 
      {
        std::string::size_type pos = _argin.find(kNameSep);
        if (pos == std::string::npos) 
        {
          std::string d = "argin syntax error (" + _argin + ")";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "DataCodec::encode_argin");
          return kError; 
        }
        std::string tmp;
        tmp.assign(_argin, 0, pos);
        if (XDK_UTILS->get_obj_df(tmp, dfh, obj_name) != kNoError) 
        {
          XDK_UTILS->set_error("XOP internal error",
                               "XDK_Utils::get_obj_df failed",
                               "DataCodec::encode_argin");
          return kError;
        }
        waveHndl lw;
        if (XDK_UTILS->fetch_wave(&lw, dfh, obj_name, NT_I32, 1)) 
        {
          std::string d = "wave <" + obj_name;
          d += "> either does not exists";
          d += " or has incompatible type/dimensions";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "DataCodec::encode_argin");
          return kError; 
        }
        tmp.assign(_argin, pos + 1, _argin.size());
        if (XDK_UTILS->get_obj_df(tmp, dfh, obj_name) != kNoError) 
        {
          XDK_UTILS->set_error("XOP internal error",
                               "XDK_Utils::get_obj_df failed",
                               "DataCodec::encode_argin");
          return kError;
        }
        waveHndl sw;
        if (XDK_UTILS->fetch_wave(&sw, dfh, obj_name, TEXT_WAVE_TYPE, 1)) 
        {
          std::string d = "wave <" + obj_name;
          d += "> either does not exists";
          d += " or has incompatible type/dimensions";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "DataCodec::encode_argin");
          return kError; 
        }
        this->insert(dd_in_, lw, sw);
      } break;
      //-------------------------------------------------
      case Tango::DEVVAR_DOUBLESTRINGARRAY: 
      {
        std::string::size_type pos = _argin.find(kNameSep);
        if (pos == std::string::npos) 
        {
          std::string d = "argin syntax error (" + _argin + ")";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "DataCodec::encode_argin");
          return kError; 
        }
        std::string tmp;
        tmp.assign(_argin, 0, pos);
        if (XDK_UTILS->get_obj_df(tmp, dfh, obj_name) != kNoError) 
        {
          XDK_UTILS->set_error("XOP internal error",
                               "XDK_Utils::get_obj_df failed",
                               "DataCodec::encode_argin");
          return kError;
        }
        waveHndl dw;
        if (XDK_UTILS->fetch_wave(&dw, dfh, obj_name, NT_FP64, 1)) 
        {
          std::string d = "wave <" + obj_name + ">";
          d += " either does not exists";
          d += " or has incompatible type/dimensions";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "DataCodec::encode_argin");
          return kError; 
        }
        tmp.assign(_argin, pos + 1, _argin.size());
        if (XDK_UTILS->get_obj_df(tmp, dfh, obj_name) != kNoError) 
        {
          XDK_UTILS->set_error("XOP internal error",
                               "XDK_Utils::get_obj_df failed",
                               "DataCodec::encode_argin");
          return kError;
        }
        waveHndl sw;
        if (XDK_UTILS->fetch_wave(&sw, dfh, obj_name, TEXT_WAVE_TYPE, 1)) 
        {
          std::string d = "wave <" + obj_name + ">";
          d += " either does not exists";
          d += " or has incompatible type/dimensions";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "DataCodec::encode_argin");
          return kError; 
        }
        this->insert(dd_in_, dw, sw);
      } break;
      //-------------------------------------------------
      case Tango::DEVVAR_STRINGARRAY: 
      {
        waveHndl wave_in;
        if (XDK_UTILS->fetch_wave(&wave_in, dfh, obj_name, TEXT_WAVE_TYPE, 1)) 
        {
          std::string d = "wave <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type/dimensions";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "DataCodec::encode_argin");
          return kError;  
        }
        this->insert(dd_in_, wave_in);
      } break;
      //-- DEVVAR_DOUBLEARRAY --------------------------
      case Tango::DEVVAR_DOUBLEARRAY: 
      {
        waveHndl wave_in;
        if (XDK_UTILS->fetch_wave(&wave_in, dfh, obj_name, NT_FP64, 1)) 
        {
          std::string d = "wave <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type/dimensions";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "DataCodec::encode_argin");
          return kError;  
        }
        this->insert(dd_in_, wave_in);
      } break;
      //-------------------------------------------------
      case Tango::DEVVAR_FLOATARRAY: 
      {
        waveHndl wave_in;
        if (XDK_UTILS->fetch_wave(&wave_in, dfh, obj_name, NT_FP32, 1)) 
        {
          std::string d = "wave <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type/dimensions";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "DataCodec::encode_argin");
          return kError;  
        }
        this->insert(dd_in_, wave_in);
      } break;
      //-------------------------------------------------
      case Tango::DEVVAR_ULONGARRAY: 
      {
        waveHndl wave_in;
        if (XDK_UTILS->fetch_wave(&wave_in, dfh, obj_name, NT_I32|NT_UNSIGNED, 1)) 
        {
          std::string d = "wave <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type/dimensions";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "DataCodec::encode_argin");
          return kError;  
        }
        this->insert(dd_in_, wave_in);
      } break;
      //-------------------------------------------------
      case Tango::DEVVAR_LONGARRAY: 
      {
        waveHndl wave_in;
        if (XDK_UTILS->fetch_wave(&wave_in, dfh, obj_name, NT_I32, 1)) 
        {
          std::string d = "wave <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type/dimensions";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "DataCodec::encode_argin");
          return kError;  
        }
        this->insert(dd_in_, wave_in);
      } break;
      //-------------------------------------------------
      case Tango::DEVVAR_USHORTARRAY: 
      {
        waveHndl wave_in;
        if (XDK_UTILS->fetch_wave(&wave_in, dfh, obj_name, NT_I16|NT_UNSIGNED, 1)) 
        {
          std::string d = "wave <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type/dimensions";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "DataCodec::encode_argin");
          return kError;   
        }
        this->insert(dd_in_, wave_in);
      } break;
      //-------------------------------------------------
      case Tango::DEVVAR_SHORTARRAY: 
      {
        waveHndl wave_in;
        if (XDK_UTILS->fetch_wave(&wave_in, dfh, obj_name, NT_I16, 1)) 
        {
          std::string d = "wave <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type/dimensions";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "DataCodec::encode_argin");
          return kError;  
        }
        this->insert(dd_in_, wave_in);
      } break;
      //-------------------------------------------------
      case Tango::DEVVAR_CHARARRAY: 
      {
        waveHndl wave_in;
        if (XDK_UTILS->fetch_wave(&wave_in, dfh, obj_name, NT_I8|NT_UNSIGNED, 1)) 
        {
          std::string d = "wave <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type/dimensions";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "DataCodec::encode_argin");
          return kError;   
        }
        this->insert(dd_in_, wave_in);
      } break;
      //-------------------------------------------------
      case Tango::DEV_STATE: 
      {
        XDK_UTILS->set_error("XOP internal error",
                             "unsupported argin type (DEV_STATE)",
                             "DataCodec::encode_argin");
        return kError;  
      } break;
      //-------------------------------------------------
      case Tango::CONST_DEV_STRING: 
      {
        XDK_UTILS->set_error("XOP internal error",
                             "unsupported argin type (CONST_DEV_STRING)",
                             "DataCodec::encode_argin");
        return kError;  
      } break;
      //-------------------------------------------------
      case Tango::DEV_STRING: 
      {
        dd_in_ << const_cast<std::string&>(obj_name);
      } break;
      //-------------------------------------------------
      case Tango::DEV_BOOLEAN: 
      {
        double d;
        if (XDK_UTILS->get_df_obj (dfh, obj_name, &d)) 
        {
          std::string d = "global variable <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "DataCodec::encode_argin");
          return kError; 
        }
        bool b = d ? true : false;
        dd_in_ << b;
      } break;
      //-------------------------------------------------
      case Tango::DEV_USHORT: 
      {
        double d;
        if (XDK_UTILS->get_df_obj (dfh, obj_name, &d)) 
        {
          std::string d = "global variable <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "DataCodec::encode_argin");
          return kError; 
        }
        unsigned short us;
        if (::ConvertData2(&d, &us, 1, NT_FP64, NT_I16|NT_UNSIGNED)) 
        {
          XDK_UTILS->set_error("XOP internal error",
                               "ConvertData2 failed",
                               "DataCodec::encode_argin");
          return kError;
        }
        dd_in_ << us;
      } break;
      //-------------------------------------------------
      case Tango::DEV_SHORT: 
      {
        double d;
        if (XDK_UTILS->get_df_obj (dfh, obj_name, &d)) 
        {
          std::string d = "global variable <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "DataCodec::encode_argin");
          return kError; 
        }
        short s;
        if (::ConvertData2(&d, &s, 1, NT_FP64, NT_I16)) 
        {
          XDK_UTILS->set_error("XOP internal error",
                               "ConvertData2 failed",
                               "DataCodec::encode_argin");
          return kError;
        }
        dd_in_ << s;
      } break;
      //-------------------------------------------------
      case Tango::DEV_ULONG: 
      {
        double d;
        if (XDK_UTILS->get_df_obj (dfh, obj_name, &d)) 
        {
          std::string d = "global variable <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "DataCodec::encode_argin");
          return kError; 
        }
        unsigned long ul;
        if (::ConvertData2(&d, &ul, 1, NT_FP64, NT_I32|NT_UNSIGNED)) 
        {
          XDK_UTILS->set_error("XOP internal error",
                               "ConvertData2 failed",
                               "DataCodec::encode_argin");
          return kError;
        }
        dd_in_ << ul;
      } break;
      //-------------------------------------------------
      case Tango::DEV_LONG: 
      {
        double d;
        if (XDK_UTILS->get_df_obj (dfh, obj_name, &d)) 
        {
          std::string d = "global variable <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "DataCodec::encode_argin");
          return kError; 
        }
        long l;
        if (::ConvertData2(&d, &l, 1, NT_FP64, NT_I32)) 
        {
          XDK_UTILS->set_error("XOP internal error",
                               "ConvertData2 failed",
                               "DataCodec::encode_argin");
          return kError;
        }
        dd_in_ << l;
      } break;
      //-------------------------------------------------
      case Tango::DEV_FLOAT: 
      {
        double d;
        if (XDK_UTILS->get_df_obj (dfh, obj_name, &d)) 
        {
          std::string d = "global variable <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "DataCodec::encode_argin");
          return kError; 
        }
        float f;
        if (::ConvertData2(&d, &f, 1, NT_FP64, NT_FP32)) 
        {
          XDK_UTILS->set_error("XOP internal error",
                               "ConvertData2 failed",
                               "DataCodec::encode_argin");
          return kError;
        }
        dd_in_ << f;
      } break;
      //-------------------------------------------------
      case Tango::DEV_DOUBLE: 
      {
        double d;
        if (XDK_UTILS->get_df_obj (dfh, obj_name, &d)) 
        {
          std::string d = "global variable <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "DataCodec::encode_argin");
          return kError;
        }
        dd_in_ << d;
      } break;
      //-------------------------------------------------
      default:
        std::string d = "unsupported TANGO data type for argin <" + _argin + ">";
        XDK_UTILS->set_error("unsupported TANGO data type",
                              d.c_str(),
                             "DataCodec::encode_argin");
        return kError; 
        break;
    } // switch 
  } //try
  catch (const Tango::DevFailed &e) 
  {
    XDK_UTILS->set_error(e);
		return kError;
	}
  catch (...) 
  {
    XDK_UTILS->set_error("unknown error",
                         "unknown exception caught",
                         "DataCodec::encode_argin");
    return kError;
  }
  return kNoError;
}

//=============================================================================
// DataCodec::decode_argout
//=============================================================================
int DataCodec::decode_argout (DevDescriptor* _ddesc, 
                              const std::string& _argout, 
                              int _cmd_id,
                              Tango::DeviceData& dd_out_)
{
  if (_ddesc == 0) 
  {
    XDK_UTILS->set_error("XOP internal error",
                         "unexpected NULL argument",
                         "DataCodec::decode_argout");
    return kError;
  }

  /* 
  do we really need this?
  ================================================
  bitset<Tango::DeviceData::numFlags> ex_mode = 0;
  dd_out_.exceptions(ex_mode);
  if (dd_out_.is_empty()) 
  {
    XDK_UTILS->set_error("invalid data",
                         "failed to extract data from device answer [empty]",
                         "DataCodec::decode_argout");
    return kError;
  }
  */

  DFHndl dfh;
  std::string obj_name;
  if (XDK_UTILS->get_obj_df(_argout, dfh, obj_name, true) != kNoError) 
  {
    XDK_UTILS->set_error("XOP internal error",
                         "XDK_Utils::get_obj_df failed",
                         "DataCodec::decode_argout");
    return kError;
  }

  try 
  {
    waveHndl num_wave = 0;
    waveHndl str_wave = 0;
    int argout_type = (_ddesc->cmd_list())[_cmd_id].out_type;
    switch (argout_type) 
    {
      //-------------------------------------------------
      case Tango::DEV_VOID: 
      {
        return kNoError; 
      } break;
      //-------------------------------------------------
      case Tango::DEVVAR_LONGSTRINGARRAY: 
      {
        // search 
        std::string::size_type sep_pos = _argout.find(kNameSep);
        if (sep_pos == std::string::npos) 
        {
          std::string d = "argout syntax error (" + _argout + ")";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "DataCodec::decode_argout");
          return kError; 
        }
        // get num_wave name
        std::string num_wname;
        std::string num_wave_full_path;
        num_wave_full_path.assign(_argout, 0, sep_pos);
        std::string::size_type pos = num_wave_full_path.rfind(':');
        if (pos == std::string::npos) 
          num_wname = num_wave_full_path; 
        else 
          num_wname.assign(num_wave_full_path.begin() + pos + 1, num_wave_full_path.end()); 
        // fetch wave num_wave
        if (XDK_UTILS->fetch_or_make_wave(&num_wave, dfh, num_wname, NT_I32, XDK_Utils::TYPE)) 
        {
          std::string d = "failed to make or change wave <" + num_wname + ">";
          d += " (object name or type conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "DataCodec::decode_argout");
          return kError;
        }
        // get str_wave name
        std::string str_wname;
        std::string str_wave_full_path;
        str_wave_full_path.assign(_argout, sep_pos + 1, std::string::npos);
        pos = str_wave_full_path.rfind(':');
        if (pos == std::string::npos) 
          str_wname = str_wave_full_path; 
        else 
          str_wname.assign(str_wave_full_path.begin() + pos + 1, str_wave_full_path.end()); 
        // fetch_wave str_wave
        if (XDK_UTILS->fetch_or_make_wave(&str_wave, dfh, str_wname, TEXT_WAVE_TYPE, XDK_Utils::TYPE)) 
        {
          std::string d = "failed to make or change wave <" + str_wname + ">";
          d += " (object name or type conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "DataCodec::decode_argout");
          return kError;
        }
        DataCodec::extract(dd_out_, num_wave, str_wave);
      } break;
      //-------------------------------------------------
      case Tango::DEVVAR_DOUBLESTRINGARRAY: 
      {
        std::string::size_type sep_pos = _argout.find(kNameSep);
        if (sep_pos == std::string::npos) 
        {
          std::string d = "argout syntax error (" + _argout + ")";
          XDK_UTILS->set_error("argout syntax error",
                               d.c_str(),
                               "DataCodec::decode_argout");
          return kError; 
        }
        // get num_wave name
        std::string num_wname;
        std::string num_wave_full_path;
        num_wave_full_path.assign(_argout, 0, sep_pos);
        std::string::size_type pos = num_wave_full_path.rfind(':');
        if (pos == std::string::npos) 
          num_wname = num_wave_full_path; 
        else 
          num_wname.assign(num_wave_full_path.begin() + pos + 1, num_wave_full_path.end()); 
        if (XDK_UTILS->fetch_or_make_wave(&num_wave, dfh, num_wname, NT_FP64, XDK_Utils::TYPE)) 
        {
          std::string d = "failed to make or change wave <" + num_wname + ">";
          d += " (object name or type conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "DataCodec::decode_argout");
          return kError; 
        }
        // get str_wave name
        std::string str_wname;
        std::string str_wave_full_path;
        str_wave_full_path.assign(_argout, sep_pos + 1, std::string::npos);
        pos = str_wave_full_path.rfind(':');
        if (pos == std::string::npos) 
          str_wname = str_wave_full_path; 
        else 
          str_wname.assign(str_wave_full_path.begin() + pos + 1, str_wave_full_path.end()); 
        if (XDK_UTILS->fetch_or_make_wave(&str_wave, dfh, str_wname, TEXT_WAVE_TYPE, XDK_Utils::TYPE)) 
        {
          std::string d = "failed to make or change wave <" + str_wname + ">";
          d += " (object name or type conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "DataCodec::decode_argout");
          return kError;
        }
        this->extract(dd_out_, num_wave, str_wave);
      } break;
      //-------------------------------------------------
      case Tango::DEVVAR_STRINGARRAY: 
      { 
        if (XDK_UTILS->fetch_or_make_wave(&str_wave, dfh, obj_name, TEXT_WAVE_TYPE, XDK_Utils::TYPE)) 
        {
          std::string d = "failed to make or change wave <" + _argout + ">";
          d += " (object name or type conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "DataCodec::decode_argout");
          return kError;
        }
        this->extract(dd_out_, str_wave);
      } break;
      //-------------------------------------------------
      case Tango::DEVVAR_DOUBLEARRAY: 
      { 
        if (XDK_UTILS->fetch_or_make_wave(&num_wave, dfh, obj_name, NT_FP64, XDK_Utils::TYPE)) 
        {
          std::string d = "failed to make or change wave <" + _argout + ">";
          d += " (object name or type conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "DataCodec::decode_argout");
          return kError;
        }
        this->extract(dd_out_, num_wave);
      } break;
      //-------------------------------------------------
      case Tango::DEVVAR_FLOATARRAY: 
      {
        if (XDK_UTILS->fetch_or_make_wave(&num_wave, dfh, obj_name, NT_FP32, XDK_Utils::TYPE)) 
        {
          std::string d = "failed to make or change wave <" + _argout + ">";
          d += " (object name or type conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "DataCodec::decode_argout");
          return kError; 
        }
        this->extract(dd_out_, num_wave);
      } break;
      //-------------------------------------------------
      case Tango::DEVVAR_ULONGARRAY: 
      {
        if (XDK_UTILS->fetch_or_make_wave(&num_wave, dfh, obj_name, NT_I32|NT_UNSIGNED, XDK_Utils::TYPE)) 
        {
          std::string d = "failed to make or change wave <" + _argout + ">";
          d += " (object name or type conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "DataCodec::decode_argout");
          return kError; 
        }
        this->extract(dd_out_, num_wave);
      } break;
      //-------------------------------------------------
      case Tango::DEVVAR_LONGARRAY: 
      { 
        if (XDK_UTILS->fetch_or_make_wave(&num_wave, dfh, obj_name, NT_I32, XDK_Utils::TYPE)) 
        {
          std::string d = "failed to make or change wave <" + _argout + ">";
          d += " (object name or type conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "DataCodec::decode_argout");
          return kError;
        }
        this->extract(dd_out_, num_wave);
      } break;
      //-------------------------------------------------
      case Tango::DEVVAR_USHORTARRAY: 
      { 
        if (XDK_UTILS->fetch_or_make_wave(&num_wave, dfh, obj_name, NT_I16|NT_UNSIGNED, XDK_Utils::TYPE)) 
        {
          std::string d = "failed to make or change wave <" + _argout + ">";
          d += " (object name or type conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "DataCodec::decode_argout");
          return kError;
        }
        this->extract(dd_out_, num_wave);
      } break;
      //-------------------------------------------------
      case Tango::DEVVAR_SHORTARRAY: 
      {
        if (XDK_UTILS->fetch_or_make_wave(&num_wave, dfh, obj_name, NT_I16, XDK_Utils::TYPE)) 
        {
          std::string d = "failed to make or change wave <" + _argout + ">";
          d += " (object name or type conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "DataCodec::decode_argout");
          return kError;
        }
        this->extract(dd_out_, num_wave);
      } break;
      //-------------------------------------------------
      case Tango::DEVVAR_CHARARRAY: 
      {
        if (XDK_UTILS->fetch_or_make_wave(&num_wave, dfh, obj_name, NT_I8|NT_UNSIGNED, XDK_Utils::TYPE)) 
        {
          std::string d = "failed to make or change wave <" + _argout + ">";
          d += " (object name or type conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "DataCodec::decode_argout");
          return kError;
        }
        this->extract(dd_out_, num_wave);
      } break;
      //-------------------------------------------------
      case Tango::DEV_STRING: 
      {
        std::string s;
        dd_out_ >> s;
        if (XDK_UTILS->set_df_obj(dfh, obj_name, s) != kNoError) 
        {
          std::string d = "failed to set global string <" + _argout + ">";
          d += " (object name or type conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "DataCodec::decode_argout");
          return kError;
        }
      } break;
      //-------------------------------------------------
      case Tango::DEV_BOOLEAN: 
      {
        bool b;
        dd_out_ >> b;
        long l = static_cast<long>(b);
        if (XDK_UTILS->set_df_obj(dfh, obj_name, l) != kNoError) 
        {
          std::string d = "failed to set global variable <" + _argout + ">";
          d += " (object name or type conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "DataCodec::decode_argout");
          return kError;
        }
      } break;
      //-------------------------------------------------
      case Tango::DEV_USHORT: 
      {
        unsigned short us;
        dd_out_ >> us;
        long l = static_cast<long>(us);
        if (XDK_UTILS->set_df_obj(dfh, obj_name, l) != kNoError) 
        {
          std::string d = "failed to set global variable <" + _argout + ">";
          d += " (object name or type conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "DataCodec::decode_argout");
          return kError;
        }
      } break;
      //-------------------------------------------------
      case Tango::DEV_SHORT: 
      {
        short s;
        dd_out_ >> s;
        long l = static_cast<long>(s);
        if (XDK_UTILS->set_df_obj(dfh, obj_name, l) != kNoError) 
        {
          std::string d = "failed to set global variable <" + _argout + ">";
          d += " (object name or type conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "DataCodec::decode_argout");
          return kError;
        }
      } break;
      //-------------------------------------------------
      case Tango::DEV_ULONG: 
      {
        unsigned long ul;
        dd_out_ >> ul;
        long l = static_cast<long>(ul);
        if (XDK_UTILS->set_df_obj(dfh, obj_name, l) != kNoError) 
        {
          std::string d = "failed to set global variable <" + _argout + ">";
          d += " (object name or type conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "DataCodec::decode_argout");
          return kError;
        }
      } break;
      //-------------------------------------------------
      case Tango::DEV_LONG: 
      {
        long l;
        dd_out_ >> l;
        if (XDK_UTILS->set_df_obj(dfh, obj_name, l) != kNoError) 
        {
          std::string d = "failed to set global variable <" + _argout + ">";
          d += " (object name or type conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "DataCodec::decode_argout");
          return kError;
        }
      } break;
      //-------------------------------------------------
      case Tango::DEV_FLOAT: 
      {
        float f;
        dd_out_ >> f;
        double d = f;
        if (XDK_UTILS->set_df_obj(dfh, obj_name, d) != kNoError) 
        {
          std::string d = "failed to set global variable <" + _argout + ">";
          d += " (object name or type conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "DataCodec::decode_argout");
          return kError;
        }
      } break;
      //-------------------------------------------------
      case Tango::DEV_DOUBLE: 
      {
        double d;
        dd_out_ >> d;
        if (XDK_UTILS->set_df_obj(dfh, obj_name, d) != kNoError) 
        {
          std::string d = "failed to set global variable <" + _argout + ">";
          d += " (object name or type conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "DataCodec::decode_argout");
          return kError;
        }
      } break;
      //-------------------------------------------------
      case Tango::DEV_STATE: 
      {
        Tango::DevState ds;
        dd_out_ >> ds;
        if (XDK_UTILS->set_df_obj(dfh, obj_name, static_cast<long>(ds))) 
        {
          std::string d = "failed to set global variable <" + _argout + ">";
          d += " (object name or type conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "DataCodec::decode_argout");
          return kError;
        }
      } break;
      //-------------------------------------------------
      case Tango::CONST_DEV_STRING: 
      {
        XDK_UTILS->set_error("XOP internal error",
                             "unsupported argout type (CONST_DEV_STRING)",
                             "DataCodec::decode_argout");
        return kError; 
      }
      break;
      //-------------------------------------------------
      default:
        std::string d = "unsupported TANGO data type for argout <" + _argout + ">";
        XDK_UTILS->set_error("unsupported TANGO data type",
                              d.c_str(),
                             "DataCodec::decode_argout");
        return kError; 
        break;
    } // switch
    if (num_wave) ::WaveHandleModified(num_wave);
    if (str_wave) ::WaveHandleModified(str_wave);
  } // try        
  catch (const Tango::DevFailed &e) 
  {
    XDK_UTILS->set_error(e);
		return kError;
	}
  catch (...) 
  {
    XDK_UTILS->set_error("unknown error",
                         "unknown exception caught",
                         "DataCodec::decode_argout");
    return kError;
  }

  return kNoError;
}


//=============================================================================
// DataCodec::encode_attr
//=============================================================================
int DataCodec::encode_attr (DevDescriptor* _ddesc, 
                            const std::string& _argin, 
                            int _attr_id,
                            Tango::DeviceAttribute& value_)
{
  if (_ddesc == 0) 
  {
    XDK_UTILS->set_error("XOP internal error",
                         "unexpected NULL argument",
                         "DataCodec::encode_attr");
    return kError;
  }

  DFHndl dfh;
  std::string obj_name;
  if (XDK_UTILS->get_obj_df(_argin, dfh, obj_name) != kNoError) 
  {
    XDK_UTILS->set_error("XOP internal error",
                         "XDK_Utils::get_obj_df failed",
                         "DataCodec::encode_attr");
    return kError;
  }

  const Tango::AttributeInfoList &attr_list = _ddesc->attr_list();
  //- attr format (scalar, spectrum or image)
  int attr_format = attr_list[_attr_id].data_format;
  //- attr data type (DEV_SHORT, DEV_LONG, ...)
  int attr_type = attr_list[_attr_id].data_type;

  //- set common values
  value_.quality = Tango::ATTR_VALID;
  value_.name = attr_list[_attr_id].name;
#ifdef WIN32
  struct _timeb now;
  ::_ftime(&now);
  value_.time.tv_sec  = static_cast<CORBA::Long>(now.time);
  value_.time.tv_usec = static_cast<CORBA::Long>(now.millitm * 1000);
  value_.time.tv_nsec = static_cast<CORBA::Long>(0);
#else
  struct timeval now;
  ::gettimeofday(&now, NULL);
  value_.time.tv_sec  = now.tv_sec;
  value_.time.tv_usec = now.tv_usec;
  value_.time.tv_nsec = 0;
#endif

  waveHndl wave = 0;

  try 
  {
    switch (attr_format) 
    {
      case Tango::SCALAR: 
      {
        switch (attr_type) 
        {
          //-------------------------------------------------
          case Tango::DEV_STRING: 
          {
            if (XDK_UTILS->fetch_wave(&wave, dfh, obj_name, TEXT_WAVE_TYPE)) 
            {
              //- does the user provide the actual argin or a string containing argin?
              //- first try to get a global string var named <_argin> in <dfh>
              //- if there is no string var named <_argin> in <dfh> <_argin> is unchanged
              //- <_argin> becomes equal to its content otherwise 
              XDK_UTILS->get_df_obj(dfh, obj_name, obj_name);
              value_ << const_cast<std::string&>(obj_name);
            }
            else
            {
              Handle txt_hndl = ::NewHandle(0);
              if (txt_hndl == 0) 
              {
                XDK_UTILS->set_error("out of memory",
                                     "NewHandle failed",
                                     "DataCodec::encode_attr");
                return kError; 
              }
              MDWaveDims dim_indx;
              ::MemClear(dim_indx, sizeof(MDWaveDims));
              dim_indx[0] = 0;
              if (::MDGetTextWavePointValue(wave, dim_indx, txt_hndl)) 
              {
                ::DisposeHandle(txt_hndl);
                XDK_UTILS->set_error("XOP internal error",
                                     "MDGetTextWavePointValue failed",
                                     "DataCodec::encode_attr");
                return kError; 
              }
              char * tmp = CORBA::string_alloc(::GetHandleSize(txt_hndl) + 1);
              if (tmp == 0) 
              {
                XDK_UTILS->set_error("out of memory",
                                     "NewHandle failed",
                                     "DataCodec::encode_attr");
                return kError; 
              }
              ::MemClear(tmp, ::GetHandleSize(txt_hndl) + 1);
              ::memcpy(tmp, *txt_hndl, ::GetHandleSize(txt_hndl));
              DisposeHandle(txt_hndl);
              std::string std_str(tmp);
              value_ << std_str;
              CORBA::string_free(tmp);
            }
            value_.dim_x = 1;
            value_.dim_y = 0;
          } break;
          //-------------------------------------------------
          case Tango::DEV_BOOLEAN: 
          {
            bool b;
            double d;
            if (XDK_UTILS->get_df_obj (dfh, obj_name, &d)) 
            {
              if (XDK_UTILS->fetch_wave(&wave, dfh, obj_name, NT_I8|NT_UNSIGNED)) 
              {
                std::string d = "wave <" + _argin + ">";
                d += " either does not exists";
                d += " or has incompatible type/dimensions";
                XDK_UTILS->set_error("invalid argin specified",
                                     d.c_str(),
                                     "DataCodec::encode_attr");
                return kError; 
              }
              BCInt offset;
              if (::MDAccessNumericWaveData(wave, kMDWaveAccessMode0, &offset))
              {
                XDK_UTILS->set_error("XOP internal error",
                                     "MDAccessNumericWaveData failed",
                                     "DataCodec::encode_attr");
                return kError; 
              }
              b = *(reinterpret_cast<unsigned char*>(static_cast<char*>(*wave) + offset)) ? true : false;
            }
            else
            {
              b = d ? true : false;
            }
            value_ << b;
            value_.dim_x = 1;
            value_.dim_y = 0;
          } break;
          //-------------------------------------------------
          case Tango::DEV_UCHAR: 
          {
            unsigned char us;
            double d;
            if (XDK_UTILS->get_df_obj (dfh, obj_name, &d)) 
            {
              if (XDK_UTILS->fetch_wave(&wave, dfh, obj_name, NT_I8|NT_UNSIGNED)) 
              {
                std::string d = "wave <" + _argin + ">";
                d += " either does not exists";
                d += " or has incompatible type/dimensions";
                XDK_UTILS->set_error("invalid argin specified",
                                     d.c_str(),
                                     "DataCodec::encode_attr");
                return kError; 
              }
              BCInt offset;
              if (::MDAccessNumericWaveData(wave, kMDWaveAccessMode0, &offset))
              {
                XDK_UTILS->set_error("XOP internal error",
                                     "MDAccessNumericWaveData failed",
                                     "DataCodec::encode_attr");
                return kError; 
              }
              us = *(reinterpret_cast<unsigned char*>(static_cast<char*>(*wave) + offset));
            }
            else 
            {
              if (::ConvertData2(&d, &us, 1, NT_FP64, NT_I8|NT_UNSIGNED)) 
              {
                XDK_UTILS->set_error("XOP internal error",
                                     "ConvertData2 failed",
                                     "DataCodec::encode_attr");
                return kError;
              }
            }
            value_ << us;
            value_.dim_x = 1;
            value_.dim_y = 0;
          } break;
          //-------------------------------------------------
          case Tango::DEV_USHORT: 
          {
            unsigned short us;
            double d;
            if (XDK_UTILS->get_df_obj (dfh, obj_name, &d)) 
            {
              if (XDK_UTILS->fetch_wave(&wave, dfh, obj_name, NT_I16|NT_UNSIGNED)) 
              {
                std::string d = "wave <" + _argin + ">";
                d += " either does not exists";
                d += " or has incompatible type/dimensions";
                XDK_UTILS->set_error("invalid argin specified",
                                     d.c_str(),
                                     "DataCodec::encode_attr");
                return kError; 
              }
              BCInt offset;
              if (::MDAccessNumericWaveData(wave, kMDWaveAccessMode0, &offset))
              {
                XDK_UTILS->set_error("XOP internal error",
                                     "MDAccessNumericWaveData failed",
                                     "DataCodec::encode_attr");
                return kError; 
              }
              us = *(reinterpret_cast<unsigned short*>(static_cast<char*>(*wave) + offset));
            } 
            else 
            {
              if (::ConvertData2(&d, &us, 1, NT_FP64, NT_I16|NT_UNSIGNED)) 
              {
                XDK_UTILS->set_error("XOP internal error",
                                     "ConvertData2 failed",
                                     "DataCodec::encode_attr");
                return kError;
              }
            }
            value_ << us;
            value_.dim_x = 1;
            value_.dim_y = 0;
          } break;
          //-------------------------------------------------
          case Tango::DEV_SHORT: 
          {
            short s;
            double d;
            if (XDK_UTILS->get_df_obj(dfh, obj_name, &d)) 
            {
              if (XDK_UTILS->fetch_wave(&wave, dfh, obj_name, NT_I16)) 
              {
                std::string d = "wave <" + _argin + ">";
                d += " either does not exists";
                d += " or has incompatible type/dimensions";
                XDK_UTILS->set_error("invalid argin specified",
                                     d.c_str(),
                                     "DataCodec::encode_attr");
                return kError; 
              }
              BCInt offset;
              if (::MDAccessNumericWaveData(wave, kMDWaveAccessMode0, &offset))
              {
                XDK_UTILS->set_error("XOP internal error",
                                     "MDAccessNumericWaveData failed",
                                     "DataCodec::encode_attr");
                return kError; 
              }
              s = *(reinterpret_cast<short*>(static_cast<char*>(*wave) + offset));
            }
            else 
            {
              if (::ConvertData2(&d, &s, 1, NT_FP64, NT_I16)) 
              {
                XDK_UTILS->set_error("XOP internal error",
                                     "ConvertData2 failed",
                                     "DataCodec::encode_attr");
                return kError;
              }
            }
            value_ << s;
            value_.dim_x = 1;
            value_.dim_y = 0;
          } break;
          //-------------------------------------------------
          case Tango::DEV_ULONG: 
          {
            unsigned long l;
            double d;
            if (XDK_UTILS->get_df_obj (dfh, obj_name, &d)) 
            {
              if (XDK_UTILS->fetch_wave(&wave, dfh, obj_name, NT_I32|NT_UNSIGNED)) 
              {
                std::string d = "wave <" + _argin + ">";
                d += " either does not exists";
                d += " or has incompatible type/dimensions";
                XDK_UTILS->set_error("invalid argin specified",
                                     d.c_str(),
                                     "DataCodec::encode_attr");
                return kError; 
              }
              BCInt offset;
              if (::MDAccessNumericWaveData(wave, kMDWaveAccessMode0, &offset))
              {
                XDK_UTILS->set_error("XOP internal error",
                                     "MDAccessNumericWaveData failed",
                                     "DataCodec::encode_attr");
                return kError; 
              }
              l = *(reinterpret_cast<long*>(static_cast<char*>(*wave) + offset));
            }
            else 
            {
              if (::ConvertData2(&d, &l, 1, NT_FP64, NT_I32|NT_UNSIGNED)) 
              {
                XDK_UTILS->set_error("XOP internal error",
                                     "ConvertData2 failed",
                                     "DataCodec::encode_attr");
                return kError;
              }
            }
            value_ << l;
            value_.dim_x = 1;
            value_.dim_y = 0;
          } break;
          //-------------------------------------------------
          case Tango::DEV_LONG: 
          {
            long l;
            double d;
            if (XDK_UTILS->get_df_obj (dfh, obj_name, &d)) 
            {
              if (XDK_UTILS->fetch_wave(&wave, dfh, obj_name, NT_I32)) 
              {
                std::string d = "wave <" + _argin + ">";
                d += " either does not exists";
                d += " or has incompatible type/dimensions";
                XDK_UTILS->set_error("invalid argin specified",
                                     d.c_str(),
                                     "DataCodec::encode_attr");
                return kError; 
              }
              BCInt offset;
              if (::MDAccessNumericWaveData(wave, kMDWaveAccessMode0, &offset))
              {
                XDK_UTILS->set_error("XOP internal error",
                                     "MDAccessNumericWaveData failed",
                                     "DataCodec::encode_attr");
                return kError; 
              }
              l = *(reinterpret_cast<long*>(static_cast<char*>(*wave) + offset));
            }
            else 
            {
              if (::ConvertData2(&d, &l, 1, NT_FP64, NT_I32)) 
              {
                XDK_UTILS->set_error("XOP internal error",
                                     "ConvertData2 failed",
                                     "DataCodec::encode_attr");
                return kError;
              }
            }
            value_ << l;
            value_.dim_x = 1;
            value_.dim_y = 0;
          } break;
          //-------------------------------------------------
          case Tango::DEV_FLOAT: 
          {
            float f;
            double d;
            if (XDK_UTILS->get_df_obj (dfh, obj_name, &d)) 
            {
              if (XDK_UTILS->fetch_wave(&wave, dfh, obj_name, NT_FP32)) 
              {
                std::string d = "wave <" + _argin + ">";
                d += " either does not exists";
                d += " or has incompatible type/dimensions";
                XDK_UTILS->set_error("invalid argin specified",
                                     d.c_str(),
                                     "DataCodec::encode_attr");
                return kError; 
              }
              BCInt offset;
              if (::MDAccessNumericWaveData(wave, kMDWaveAccessMode0, &offset))
              {
                XDK_UTILS->set_error("XOP internal error",
                                     "MDAccessNumericWaveData failed",
                                     "DataCodec::encode_attr");
                return kError; 
              }
              f = *(reinterpret_cast<float*>(static_cast<char*>(*wave) + offset));
            }
            else 
            {
              if (::ConvertData2(&d, &f, 1, NT_FP64, NT_FP32)) 
              {
                XDK_UTILS->set_error("XOP internal error",
                                     "ConvertData2 failed",
                                     "DataCodec::encode_attr");
                return kError;
              }
            }
            value_ << f;
            value_.dim_x = 1;
            value_.dim_y = 0;
          } break;
          //-------------------------------------------------
          case Tango::DEV_DOUBLE: 
          {
            double d;
            if (XDK_UTILS->get_df_obj (dfh, obj_name, &d)) 
            {
              if (XDK_UTILS->fetch_wave(&wave, dfh, obj_name, NT_FP64)) 
              {
                std::string d = "wave <" + _argin + ">";
                d += " either does not exists";
                d += " or has incompatible type/dimensions";
                XDK_UTILS->set_error("invalid argin specified",
                                     d.c_str(),
                                     "DataCodec::encode_attr");
                return kError; 
              }
              BCInt offset;
              if (::MDAccessNumericWaveData(wave, kMDWaveAccessMode0, &offset))
              {
                XDK_UTILS->set_error("XOP internal error",
                                     "MDAccessNumericWaveData failed",
                                     "DataCodec::encode_attr");
                return kError; 
              }
              d = *(reinterpret_cast<double*>(static_cast<char*>(*wave) + offset));
            }
            value_ << d;
            value_.dim_x = 1;
            value_.dim_y = 0;
          } break;
          default:
            XDK_UTILS->set_error("unsupported TANGO data type",
                                 "unsupported TANGO data type for scalar attribute",
                                 "DataCodec::encode_attr");
            return kError;
            break;
        } // switch::attr_type
      } break;
      //-------------------------------------------------
      case Tango::SPECTRUM: 
      case Tango::IMAGE: 
      {
        int itype;
        switch (attr_type) 
        {
          case Tango::DEV_BOOLEAN:
            itype = NT_I8;
            break;
          case Tango::DEV_UCHAR:
            itype = NT_I8 | NT_UNSIGNED;
            break;
          case Tango::DEV_USHORT:
            itype = NT_I16 | NT_UNSIGNED;
            break;
          case Tango::DEV_SHORT:
            itype = NT_I16;
            break;
          case Tango::DEV_ULONG:
            itype = NT_I32| NT_UNSIGNED;
            break;
          case Tango::DEV_LONG:
            itype = NT_I32;
            break;
          case Tango::DEV_FLOAT:
            itype = NT_FP32;
            break;
          case Tango::DEV_DOUBLE:
            itype = NT_FP64;
            break;
          case Tango::DEV_STRING:
            itype = TEXT_WAVE_TYPE;
            break;
          default: 
          {
            std::string format = (attr_format == Tango::IMAGE) ? "image" : "spectrum";
            std::string d = "unsupported TANGO data type for " + format + " attribute";
            XDK_UTILS->set_error("unsupported TANGO data type",
                                 d.c_str(),
                                 "DataCodec::encode_attr");
            return kError;
          } break;
        }
        if (XDK_UTILS->fetch_wave(&wave, dfh, obj_name, itype, (attr_format == Tango::IMAGE) ? 2 : 1)) 
        {
          std::string d = "wave <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type/dimensions";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "DataCodec::encode_attr");
          return kError; 
        }
        int w_ndims;
        MDWaveDims w_dims;
        ::MemClear(&w_dims, sizeof(MDWaveDims));
        if (::MDGetWaveDimensions(wave, &w_ndims, w_dims)) 
        {
          XDK_UTILS->set_error("XOP internal error",
                               "MDGetWaveDimensions failed",
                               "DataCodec::encode_attr");
          return kError;  
        }
        if (attr_format == Tango::SPECTRUM && w_ndims != 1) 
        {
          std::string d = "wave <" + _argin + ">";
          d += " has incompatible dimensions (1D wave expected)";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "DataCodec::encode_argin");
          return kError; 
        }
        else if (attr_format == Tango::IMAGE && w_ndims != 2) 
        {
          std::string d = "wave <" + _argin + ">";
          d += " has incompatible dimensions (2D wave expected)";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "DataCodec::encode_argin");
          return kError;
        }
        switch (attr_format)
        {
          case Tango::SPECTRUM:
            this->insert(value_, wave, w_dims[0]); 
            value_.dim_x = w_dims[0];
            value_.dim_y = 0;
            break;
          case Tango::IMAGE:
            this->insert(value_, wave, w_dims[0], w_dims[1]); 
            value_.dim_x = w_dims[0];
            value_.dim_y = w_dims[1];
            break;
        }
      } break; 
      //-------------------------------------------------
      default:
        std::string d = "unsupported TANGO data type for attribute <" + attr_list[_attr_id].name + ">";
        XDK_UTILS->set_error("unexpected TANGO data type",
                              d.c_str(),
                             "DataCodec::encode_argin");
        break;
    } // switch::attr_format
  } //try
  catch (const Tango::DevFailed &e) 
  {
    XDK_UTILS->set_error(e);
		return kError;
	}
  catch (...) 
  {
    XDK_UTILS->set_error("unknown error",
                         "unknown exception caught",
                         "DataCodec::encode_attr");
    return kError;
  }
  return kNoError;
}

//=============================================================================
// DataCodec::decode_attr
//=============================================================================
int DataCodec::decode_attr (DevDescriptor* _ddesc, 
                            const std::string& _argout, 
                            int _attr_id,
                            Tango::DeviceAttribute& value_,
                            bool _export_ts,
                            bool _export_nan_if_invalid)
{
  if (_ddesc == 0) 
  {
    XDK_UTILS->set_error("XOP internal error",
                         "unexpected NULL argument",
                         "DataCodec::decode_attr");
    return kError;
  }
  
  bitset<Tango::DeviceAttribute::numFlags> ex_mode = 0;
  value_.exceptions(ex_mode);
 
  if (! _export_nan_if_invalid)
  {
    if (value_.has_failed())
    {
      Tango::DevFailed e(value_.get_err_stack());
      XDK_UTILS->set_error(e);
      return kError;
    }
    else if (value_.quality == Tango::ATTR_INVALID || value_.is_empty())
    {
      if (value_.quality == Tango::ATTR_INVALID)
        XDK_UTILS->set_error("invalid data",
                             "failed to extract attribute value from device answer [invalid data]",
                             "DataCodec::decode_attr");
      else
        XDK_UTILS->set_error("invalid data",
                             "received a valid but empty attribute value from device [no data]",
                             "DataCodec::decode_attr");
      return kError;
    }
  }

  DFHndl dfh;
  std::string obj_name;
  if (XDK_UTILS->get_obj_df(_argout, dfh, obj_name, true) != kNoError) 
  {
    XDK_UTILS->set_error("XOP internal error",
                         "XDK_Utils::get_obj_df failed",
                         "DataCodec::decode_attr");
    return kError;
  }

  //- the timestamp is returned in a global variable named 
  //- <obj_name + "_ts">. Other scalar types are mapped to 
  //- complex variables which real part contains the actual
  //- attr value and imaginary part the timestamp

  //- attr value timestamp (igor time ref)
  double tstamp = k_time_offset
                + value_.time.tv_sec 
                + 1.E-6 * value_.time.tv_usec 
                + 1.E-9 * value_.time.tv_nsec;
  try 
  {
    int err = kNoError;
    const Tango::AttributeInfoList &attr_list = _ddesc->attr_list();
    //- attr format (scalar, spectrum or image)
    int attr_format = attr_list[_attr_id].data_format;
    //- attr data type (DEV_SHORT, DEV_LONG, ...)
    int attr_type = attr_list[_attr_id].data_type;
    //- do this before reinjecting the value into the Tango::DeviceAttribute
    if (_export_ts)
    {
      this->export_ts(obj_name, dfh, tstamp); 
      this->export_qlt(obj_name, dfh, value_.quality); 
    }
    //- extract data from attr value 
    switch (attr_format) 
    {
      //-------------------------------------------------
      case Tango::SCALAR: 
      {
        if (value_.quality == Tango::ATTR_INVALID || value_.is_empty())
        {
          if (attr_type != Tango::DEV_STRING)
            XDK_UTILS->set_df_obj(dfh, obj_name, DOUBLE_NAN);  
          else
            XDK_UTILS->set_df_obj(dfh, obj_name, "Error Reading Attribute");
          if (_export_ts)
          {
            this->export_ts(obj_name, dfh, tstamp); 
            this->export_qlt(obj_name, dfh, value_.quality); 
          }
          return kNoError;
        }
        switch (attr_type) 
        {
          //-------------------------------------------------
          case Tango::DEV_STATE: 
          {
            Tango::DevState s;
            value_ >> s;
            err = XDK_UTILS->set_df_obj(dfh, obj_name, static_cast<long>(s));  
            if (err) 
            {
              std::string d = "failed to set global variable <" + _argout + ">";
              d += " (object name or type conflict)";
              XDK_UTILS->set_error("invalid argout specified",
                                   d.c_str(),
                                   "DataCodec::decode_attr");
              return kError;
            }
#if defined (__REUSABLE_ATTR_VALUE__)
            Tango::AttrQuality qlt = value_.quality;
            value_ << (short)s;
            value_.quality = qlt;
#endif
          } break;
          //-------------------------------------------------
          case Tango::DEV_STRING: 
          {
            //- create_global_tstamp = 1;
            std::vector<std::string> vstr;
            value_ >> vstr;
            if (XDK_UTILS->set_df_obj(dfh, obj_name, vstr[0].c_str()) != kNoError) 
            {
              std::string d = "failed to set global string <" + _argout + ">";
              d += " (object name or type conflict)";
              XDK_UTILS->set_error("invalid argout specified",
                                   d.c_str(),
                                   "DataCodec::decode_attr");
              return kError;
            }
#if defined (__REUSABLE_ATTR_VALUE__)
            Tango::AttrQuality qlt = value_.quality;
            value_ << vstr;
            value_.quality = qlt;
#endif
          } break;
          //-------------------------------------------------
          case Tango::DEV_BOOLEAN: 
          {
            bool b;
            value_ >> b;
            err = XDK_UTILS->set_df_obj(dfh, obj_name, static_cast<long>(b ? 1 : 0));  
            if (err) 
            {
              std::string d = "failed to set global variable <" + _argout + ">";
              d += " (object name or type conflict)";
              XDK_UTILS->set_error("invalid argout specified",
                                   d.c_str(),
                                   "DataCodec::decode_attr");
              return kError; 
            }
#if defined (__REUSABLE_ATTR_VALUE__)
            Tango::AttrQuality qlt = value_.quality;
            value_ << b;
            value_.quality = qlt;
#endif
          } break;
          //-------------------------------------------------
          case Tango::DEV_UCHAR: 
          {
            unsigned char uc;
            value_ >> uc;
            err = XDK_UTILS->set_df_obj(dfh, obj_name, static_cast<long>(uc));  
            if (err) 
            {
              std::string d = "failed to set global variable <" + _argout + ">";
              d += " (object name or type conflict)";
              XDK_UTILS->set_error("invalid argout specified",
                                   d.c_str(),
                                   "DataCodec::decode_attr");
              return kError;
            }
#if defined (__REUSABLE_ATTR_VALUE__)
            Tango::AttrQuality qlt = value_.quality;
            value_ << uc;
            value_.quality = qlt;
#endif
          } break;
          //-------------------------------------------------
          case Tango::DEV_USHORT: 
          {
            unsigned short us;
            value_ >> us;
            err = XDK_UTILS->set_df_obj(dfh, obj_name, static_cast<long>(us));  
            if (err) 
            {
              std::string d = "failed to set global variable <" + _argout + ">";
              d += " (object name or type conflict)";
              XDK_UTILS->set_error("invalid argout specified",
                                   d.c_str(),
                                   "DataCodec::decode_attr");
              return kError;
            }
#if defined (__REUSABLE_ATTR_VALUE__)
            Tango::AttrQuality qlt = value_.quality;
            value_ << us;
            value_.quality = qlt;
#endif
          } break;
          //-------------------------------------------------
          case Tango::DEV_SHORT: 
          {
            short s;
            value_ >> s;
            err = XDK_UTILS->set_df_obj(dfh, obj_name, static_cast<long>(s));  
            if (err) 
            {
              std::string d = "failed to set global variable <" + _argout + ">";
              d += " (object name or type conflict)";
              XDK_UTILS->set_error("invalid argout specified",
                                   d.c_str(),
                                   "DataCodec::decode_attr");
              return kError;
            }
#if defined (__REUSABLE_ATTR_VALUE__)
            Tango::AttrQuality qlt = value_.quality;
            value_ << s;
            value_.quality = qlt;
#endif
          } break;
          //-------------------------------------------------
          case Tango::DEV_ULONG: 
          {
            unsigned long ul;
            value_ >> ul;
            err = XDK_UTILS->set_df_obj(dfh, obj_name, static_cast<long>(ul));  
            if (err) 
            {
              std::string d = "failed to set global variable <" + _argout + ">";
              d += " (object name or type conflict)";
              XDK_UTILS->set_error("invalid argout specified",
                                   d.c_str(),
                                   "DataCodec::decode_attr");
              return kError;
            }
#if defined (__REUSABLE_ATTR_VALUE__)
            Tango::AttrQuality qlt = value_.quality;
            value_ << ul;
            value_.quality = qlt;
#endif
          } break;
          //-------------------------------------------------
          case Tango::DEV_LONG: 
          {
            long l;
            value_ >> l;
            err = XDK_UTILS->set_df_obj(dfh, obj_name, l);  
            if (err) 
            {
              std::string d = "failed to set global variable <" + _argout + ">";
              d += " (object name or type conflict)";
              XDK_UTILS->set_error("invalid argout specified",
                                   d.c_str(),
                                   "DataCodec::decode_attr");
              return kError;
            }
#if defined (__REUSABLE_ATTR_VALUE__)
            Tango::AttrQuality qlt = value_.quality;
            value_ << l;
            value_.quality = qlt;
#endif
          } break;
          //-------------------------------------------------
          case Tango::DEV_FLOAT: 
          {
            float f;
            value_ >> f;
            err = XDK_UTILS->set_df_obj(dfh, obj_name, static_cast<double>(f)); 
            if (err) 
            {
              std::string d = "failed to set global variable <" + _argout + ">";
              d += " (object name or type conflict)";
              XDK_UTILS->set_error("invalid argout specified",
                                   d.c_str(),
                                   "DataCodec::decode_attr");
              return kError;
            }
#if defined (__REUSABLE_ATTR_VALUE__)
            Tango::AttrQuality qlt = value_.quality;
            value_ << f;
            value_.quality = qlt;
#endif
          } break;
          //-------------------------------------------------
          case Tango::DEV_DOUBLE: 
          {
            double d;
            value_ >> d;
            err = XDK_UTILS->set_df_obj(dfh, obj_name, d); 
            if (err) 
            {
              std::string d = "failed to set global variable <" + _argout + ">";
              d += " (object name or type conflict)";
              XDK_UTILS->set_error("invalid argout specified",
                                   d.c_str(),
                                   "DataCodec::decode_attr");
              return kError;
            }
#if defined (__REUSABLE_ATTR_VALUE__)
            Tango::AttrQuality qlt = value_.quality;
            value_ << d;
            value_.quality = qlt;
#endif
          } break;
          //-------------------------------------------------
          default:
            XDK_UTILS->set_error("unsupported TANGO data type",
                                 "unsupported TANGO data type for scalar attribute",
                                 "DataCodec::decode_attr");
            return kError;
            break;
        }
      } break;
      //-------------------------------------------------
      case Tango::SPECTRUM: 
      case Tango::IMAGE: 
      {
        MDWaveDims w_dims;
        ::MemClear(&w_dims, sizeof(MDWaveDims));
        if (value_.quality == Tango::ATTR_INVALID || value_.is_empty())
        {
          if (attr_format == Tango::SPECTRUM)
          {
            w_dims[0] = 0;
            w_dims[1] = 0;
          }
          else
          {
            w_dims[0] = 1;
            w_dims[1] = 1;
          }
          waveHndl dwave = 0;
          XDK_UTILS->fetch_or_make_wave(&dwave, dfh, obj_name, NT_FP32, XDK_Utils::DIMS, w_dims);
          ::WaveHandleModified(dwave);
          if (_export_ts)
          {
            this->export_ts(obj_name, dfh, tstamp); 
            this->export_qlt(obj_name, dfh, Tango::ATTR_INVALID); 
          }
          return kNoError;
        }
        if (attr_format == Tango::SPECTRUM)
        {
          w_dims[0] = value_.get_nb_read();
          w_dims[1] = 0;
        }
        else
        {
          Tango::AttributeDimension r_dim = value_.get_r_dimension();
          w_dims[0] = r_dim.dim_x;
          w_dims[1] = r_dim.dim_y;
        }
        int itype;
        switch (attr_type) 
        {
          case Tango::DEV_BOOLEAN:
            itype = NT_I8;
            break;
          case Tango::DEV_UCHAR:
            itype = NT_I8 | NT_UNSIGNED;
            break;
          case Tango::DEV_USHORT:
            itype = NT_I16 | NT_UNSIGNED;
            break;
          case Tango::DEV_SHORT:
            itype = NT_I16;
            break;
          case Tango::DEV_ULONG:
            itype = NT_I32;
            break;
          case Tango::DEV_LONG:
            itype = NT_I32 | NT_UNSIGNED;
            break;
          case Tango::DEV_FLOAT:
            itype = NT_FP32;
            break;
          case Tango::DEV_DOUBLE:
            itype = NT_FP64;
            break;
          case Tango::DEV_STRING:
            itype = TEXT_WAVE_TYPE;
            break;
          default: 
          {
            std::string format = (attr_format == Tango::IMAGE) ? "image" : "spectrum";
            std::string d = "unsupported TANGO data type for " + format + " attribute";
            XDK_UTILS->set_error("unsupported TANGO data type",
                                 d.c_str(),
                                 "DataCodec::decode_attr");
            return kError;
          } break;
        }
        waveHndl wave = 0;
        if (XDK_UTILS->fetch_or_make_wave(&wave, dfh, obj_name, itype, XDK_Utils::ANY, w_dims)) 
        {
          std::string d = "failed to make or change wave <" + _argout + ">";
          d += " (object name or type conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "DataCodec::decode_attr");
          return kError; 
        }
        this->extract(value_, wave, w_dims[0], w_dims[1]); 
        ::WaveHandleModified(wave);
      } break; 
      //-------------------------------------------------
      default:
        std::string d = "unsupported TANGO data type for attribute <" + attr_list[_attr_id].name + ">";
        XDK_UTILS->set_error("unexpected TANGO data type",
                              d.c_str(),
                             "DataCodec::decode_attr");
        return kError;
        break;
    }
  } // try
  catch (const Tango::DevFailed &e) 
  {
    XDK_UTILS->set_error(e);
		return kError;
	}
  catch (...) 
  {
    XDK_UTILS->set_error("unknown error",
                         "unknown exception caught",
                         "DataCodec::decode_attr");
    return kError;
  }

  return kNoError;
}

//=============================================================================
// DataCodec::export_ts
//=============================================================================
int DataCodec::export_ts (const std::string& name, DFHndl dfh, double tstamp)
{
  std::string ts_obj_name = name + "_ts";
  int err = XDK_UTILS->set_df_obj(dfh, ts_obj_name, tstamp);  
  if (err) 
  {
    std::string d = "failed to set global variable <" + name + ">";
    d += " (object name or type conflict)";
    XDK_UTILS->set_error("invalid argout specified",
                       d.c_str(),
                       "DataCodec::decode_attr");
    return kError;
  }
  return kNoError;
}

//=============================================================================
// DataCodec::export_qlt
//=============================================================================
int DataCodec::export_qlt (const std::string& name, DFHndl dfh, Tango::AttrQuality qlt)
{
  std::string qlt_obj_name = name + "_qlt";
  int err = XDK_UTILS->set_df_obj(dfh, qlt_obj_name, static_cast<double>(qlt));  
  if (err) 
  {
    std::string d = "failed to set global variable <" + name + ">";
    d += " (object name or type conflict)";
    XDK_UTILS->set_error("invalid argout specified",
                       d.c_str(),
                       "DataCodec::decode_attr");
    return kError;
  }
  return kNoError;
}

//=============================================================================
// TEMPLATE CLASS Insert - member num_wave works for waves with up to 4 dims
//=============================================================================
template <class A, class T> class Insert
{
public:
  static int num_wave(waveHndl _wh, Tango::DeviceData& dd_)
  {
    int ndims = 0;
    MDWaveDims dim_size;
    ::MemClear(dim_size, sizeof(MDWaveDims));
    if (::MDGetWaveDimensions(_wh, &ndims, dim_size))
      return kError;
    long np = dim_size[0];
    switch (ndims) 
    {
      case 2:
        np *= dim_size[1];
        break;
      case 3:
        np *= dim_size[1] * dim_size[2];
        break;
      case 4:
        np *= dim_size[1] * dim_size[2] * dim_size[3];
        break;
    }
    A *array = new A;
    if (array == 0)
      return kError;
    array->length(np);
    BCInt offset;
    if (::MDAccessNumericWaveData(_wh, kMDWaveAccessMode0, &offset)) 
      return kError;
    char *base_addr = static_cast<char*>(*_wh) + offset;
    ::memcpy(array->get_buffer(), base_addr, np * sizeof(T));
    dd_ << array;
    return kNoError;
  }

  static int num_wave(waveHndl _wh, Tango::DeviceAttribute& da_)
  {
    int ndims = 0;
    MDWaveDims dim_size;
    ::MemClear(dim_size, sizeof(MDWaveDims));
    if (::MDGetWaveDimensions(_wh, &ndims, dim_size))
      return kError;
    long np = dim_size[0];
	  da_.dim_x = dim_size[0];
	  da_.dim_y = 0;
    switch (ndims) 
    {
      case 1:
        //- already done
        break;
      case 2:
        np *= dim_size[1];
        da_.dim_y = dim_size[1];
        break;
      default:
        return kError;
        break;
    }
    A *array = new A;
    if (array == 0)
      return kError;
    array->length(np);
    BCInt offset;
    if (::MDAccessNumericWaveData(_wh, kMDWaveAccessMode0, &offset))
      return kError;
    char *base_addr = static_cast<char*>(*_wh) + offset;
    ::memcpy(array->get_buffer(), base_addr, np * sizeof(T));
    da_ << array;
    return kNoError;
  }
};

// ============================================================================
// DataCodec::insert 
// ============================================================================
void DataCodec::insert (Tango::DeviceData& _dd, const waveHndl _wh)
{
  switch (::WaveType(_wh)) 
  {
    //-------------------------------------------------
    case NT_I8: 
    {
      if (sizeof(Tango::DevBoolean) != sizeof(char)) 
      {
        Tango::Except::throw_exception((const char*)"internal error",
                                       (const char*)"can't insert boolean data", 
                                       (const char*)"DataCodec::insert");
      }
      if (Insert<Tango::DevVarBooleanArray, char>::num_wave(_wh, _dd)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not insert data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::insert");
      }
    } break;
    //-------------------------------------------------
    case NT_I8|NT_UNSIGNED: 
    {
      if (Insert<Tango::DevVarUCharArray, unsigned char>::num_wave(_wh, _dd)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not insert data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::insert");
      }
    } break;
    //-------------------------------------------------
    case NT_I16: 
    {
      if (Insert<Tango::DevVarShortArray, short>::num_wave(_wh, _dd)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not insert data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::insert");
      } 
    } break;
    //-------------------------------------------------
    case NT_I16|NT_UNSIGNED: 
    {
      if (Insert<Tango::DevVarUShortArray, unsigned short>::num_wave(_wh, _dd))  
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not insert data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::insert");
      } 
    } break;
    //-------------------------------------------------
    case NT_I32: 
    {
      if (Insert<Tango::DevVarLongArray, long>::num_wave(_wh, _dd)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not insert data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::insert");
      }  
    } break;
    //-------------------------------------------------
    case NT_I32|NT_UNSIGNED: 
    {
      if (Insert<Tango::DevVarULongArray, unsigned long>::num_wave(_wh, _dd)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not insert data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::insert");
      }   
    } break;
    //-------------------------------------------------
    case NT_FP32: 
    {
      if (Insert<Tango::DevVarFloatArray, float>::num_wave(_wh, _dd)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not insert data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::insert");
      }  
    } break;
    //-------------------------------------------------
    case NT_FP64: 
    {
      if (Insert<Tango::DevVarDoubleArray, double>::num_wave(_wh, _dd)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not insert data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::insert");
      }   
    } break;
    //-------------------------------------------------
    case TEXT_WAVE_TYPE: 
    {
      int ndims = 0;
      MDWaveDims dim_size;
      ::MemClear(dim_size, sizeof(MDWaveDims));
      if (::MDGetWaveDimensions(_wh, &ndims, dim_size)) 
      {  
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not insert data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::insert");
      }
	    Tango::DevVarStringArray *array = new Tango::DevVarStringArray();
      if (array == 0) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "out of memory error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not insert data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::insert");
      }
	    array->length(dim_size[0]);
      MDWaveDims dim_indx;
      ::MemClear(dim_indx, sizeof(MDWaveDims));
      Handle txt_hndl = ::NewHandle(0);
      if (txt_hndl == 0) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "out of memory error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not insert data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::insert");
      }
      char *tmp = 0;
      for (int i = 0; i < dim_size[0]; i++) 
      {
        dim_indx[0] = i;
        if (::MDGetTextWavePointValue(_wh, dim_indx, txt_hndl)) 
        {
          ::DisposeHandle(txt_hndl);
          char wname[MAX_OBJ_NAME + 1];
          ::WaveName(_wh, wname);
          TangoSys_OMemStream reason;
          reason << "XOP internal error" << ends;
          TangoSys_OMemStream desc;
          desc << "could not insert data from wave " << wname << ends;
          Tango::Except::throw_exception(reason.str(), 
                                         desc.str(), 
                                         (const char*)"DataCodec::insert");
        }
        tmp = CORBA::string_alloc(::GetHandleSize(txt_hndl) + 1);
        if (tmp == 0) 
        {
          ::DisposeHandle(txt_hndl);
          char wname[MAX_OBJ_NAME + 1];
          ::WaveName(_wh, wname);
          TangoSys_OMemStream reason;
          reason << "out of memory error" << ends;
          TangoSys_OMemStream desc;
          desc << "could not insert data from wave " << wname << ends;
          Tango::Except::throw_exception(reason.str(), 
                                         desc.str(), 
                                         (const char*)"DataCodec::insert");
        }
        ::MemClear(tmp, ::GetHandleSize(txt_hndl) + 1);
        ::memcpy(tmp, *txt_hndl, ::GetHandleSize(txt_hndl));
        (*array)[i] = tmp;
      }
      DisposeHandle(txt_hndl);
	    _dd << array;
    } break;
    //------------------------------------------------- 
    default:
      char wname[MAX_OBJ_NAME + 1];
      ::WaveName(_wh, wname);
      TangoSys_OMemStream reason;
      reason << "invalid argument specified" << ends;
      TangoSys_OMemStream desc;
      desc << "could not insert data from wave " << wname << " (unsupported Igor data type)" << ends;
      Tango::Except::throw_exception(reason.str(), 
                                     desc.str(), 
                                     (const char*)"DataCodec::insert");
      break;
  }
} 

// ============================================================================
// DataCodec::insert
// ============================================================================
void DataCodec::insert (Tango::DeviceData& _dd, const waveHndl _wn, const waveHndl _ws) 
{
  //- Check input
  int wn_type = ::WaveType(_wn);
  int ws_type = ::WaveType(_ws);
  if ((wn_type != NT_I32 && wn_type != NT_FP64) || ws_type != TEXT_WAVE_TYPE) 
  {
    char wname[MAX_OBJ_NAME + 1];
    ::WaveName(_wn, wname);
    TangoSys_OMemStream reason;
    reason << "invalid argument specified" << ends;
    TangoSys_OMemStream desc;
    desc << "could not insert data from wave " << wname << " (incompatible wave type)" << ends;
    Tango::Except::throw_exception(reason.str(), 
                                   desc.str(), 
                                   (const char*)"DataCodec::insert");
  }

  long wn_np = ::WavePoints(_wn);
  long ws_np = ::WavePoints(_ws);
  //- Insert wave<long | double>
  BCInt offset;
  if (::MDAccessNumericWaveData(_wn, kMDWaveAccessMode0, &offset)) 
  {
    char wname[MAX_OBJ_NAME + 1];
    ::WaveName(_wn, wname);
    TangoSys_OMemStream reason;
    reason << "XOP internal error" << ends;
    TangoSys_OMemStream desc;
    desc << "could not insert data from wave " << wname << ends;
    Tango::Except::throw_exception(reason.str(), 
                                   desc.str(), 
                                   (const char*)"DataCodec::insert");
  }
  Tango::DevVarStringArray *s_value = 0;
  Tango::DevVarLongStringArray *ls_array = 0;
  Tango::DevVarDoubleStringArray *ds_array = 0;
  switch (wn_type)
  {
    //-------------------------------------------------
    case NT_I32:
    {
      ls_array = new Tango::DevVarLongStringArray();
      if (ls_array == 0) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wn, wname);
        TangoSys_OMemStream reason;
        reason << "out of memory error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not insert data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::insert");
      }
      ls_array->lvalue.length(wn_np);
      char *base_addr = static_cast<char*>(*_wn) + offset;
      ::memcpy(ls_array->lvalue.get_buffer(), base_addr, wn_np * sizeof(long));
      s_value = &ls_array->svalue;  
    } break;
    //-------------------------------------------------
    case NT_FP64:
    {
      ds_array = new Tango::DevVarDoubleStringArray();
      if (ds_array == 0) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wn, wname);
        TangoSys_OMemStream reason;
        reason << "out of memory error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not insert data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::insert");
      }
      ds_array->dvalue.length(wn_np);
      char *base_addr = static_cast<char*>(*_wn) + offset; 
      ::memcpy(ds_array->dvalue.get_buffer(), base_addr, wn_np * sizeof(double));
      s_value = &ds_array->svalue;  
    } break;
  }
  //- Insert wave<string>
	s_value->length(ws_np);
  MDWaveDims dim_indx;
  ::MemClear(dim_indx, sizeof(MDWaveDims));
  Handle txt_hndl = ::NewHandle(0);
  if (txt_hndl == 0) 
  {
    char wname[MAX_OBJ_NAME + 1];
    ::WaveName(_ws, wname);
    TangoSys_OMemStream reason;
    reason << "out of memory error" << ends;
    TangoSys_OMemStream desc;
    desc << "NewHandle failed";
    Tango::Except::throw_exception(reason.str(), 
                                   desc.str(), 
                                   (const char*)"DataCodec::insert");
  }
  char *tmp = 0;
  for (int i = 0; i < ws_np; i++) 
  {
    dim_indx[0] = i;
    if (::MDGetTextWavePointValue(_ws, dim_indx, txt_hndl)) 
    {
      ::DisposeHandle(txt_hndl);
      char wname[MAX_OBJ_NAME + 1];
      ::WaveName(_ws, wname);
      TangoSys_OMemStream reason;
      reason << "XOP internal error" << ends;
      TangoSys_OMemStream desc;
      desc << "MDGetTextWavePointValue failed" << ends;
      Tango::Except::throw_exception(reason.str(), 
                                     desc.str(), 
                                     (const char*)"DataCodec::insert");
    }
    tmp = CORBA::string_alloc(::GetHandleSize(txt_hndl) + 1);
    if (tmp == 0) 
    {
      ::DisposeHandle(txt_hndl);
      char wname[MAX_OBJ_NAME + 1];
      ::WaveName(_ws, wname);
      TangoSys_OMemStream reason;
      reason << "out of memory error" << ends;
      TangoSys_OMemStream desc;
      desc << "CORBA::string_alloc failed" << ends;
      Tango::Except::throw_exception(reason.str(), 
                                     desc.str(), 
                                     (const char*)"DataCodec::insert"); 
    }
    ::MemClear(tmp, ::GetHandleSize(txt_hndl) + 1);
    ::memcpy(tmp, *txt_hndl, ::GetHandleSize(txt_hndl));
    (*s_value)[i] = tmp;
  }
  DisposeHandle(txt_hndl);

  switch (wn_type) 
  {
    case NT_I32:
	    _dd << ls_array;
      break;
    case NT_FP64:
	    _dd << ds_array;
      break;
  }
}

// ============================================================================
// DataCodec::insert 
// ============================================================================
void DataCodec::insert (Tango::DeviceAttribute& _da, waveHndl _wh, long _dim_x, long _dim_y)
{
  switch (::WaveType(_wh)) 
  {
    //-------------------------------------------------
    case NT_I8: 
    {
      if (sizeof(Tango::DevBoolean) != sizeof(char)) 
      {
        Tango::Except::throw_exception((const char*)"internal error",
                                       (const char*)"can't insert boolean data", 
                                       (const char*)"DataCodec::insert");
      }
      if (Insert<Tango::DevVarBooleanArray, char>::num_wave(_wh, _da)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not insert data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::insert");
      }
    } break;
    //-------------------------------------------------
    case NT_I8|NT_UNSIGNED: 
    {
      if (Insert<Tango::DevVarCharArray, unsigned char>::num_wave(_wh, _da)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not insert data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::insert");
      }
    } break;
    //-------------------------------------------------
    case NT_I16: 
    {
      if (Insert<Tango::DevVarShortArray, short>::num_wave(_wh, _da)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not insert data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::insert");
      } 
    } break;
    //-------------------------------------------------
    case NT_I16|NT_UNSIGNED: 
    {
      if (Insert<Tango::DevVarUShortArray, unsigned short>::num_wave(_wh, _da)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not insert data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::insert");
      } 
    } break;
    //-------------------------------------------------
    case NT_I32: 
    {
      if (Insert<Tango::DevVarLongArray, long>::num_wave(_wh, _da)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not insert data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::insert");
      }  
    } break;
    //-------------------------------------------------
    case NT_FP32: 
    {
      if (Insert<Tango::DevVarFloatArray, float>::num_wave(_wh, _da)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not insert data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::insert");
      }  
    } break;
    //-------------------------------------------------
    case NT_FP64: 
    {
      if (Insert<Tango::DevVarDoubleArray, double>::num_wave(_wh, _da)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not insert data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::insert");
      }   
    } break;
    //-------------------------------------------------
    case TEXT_WAVE_TYPE: 
    {
      int ndims = 0;
      MDWaveDims dim_size;
      ::MemClear(dim_size, sizeof(MDWaveDims));
      if (::MDGetWaveDimensions(_wh, &ndims, dim_size)) 
      {  
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not insert data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::insert");
      }
	    Tango::DevVarStringArray *array = new Tango::DevVarStringArray();
      if (array == 0) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "out of memory error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not insert data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::insert");
      }
      array->length((ndims <= 1) ? dim_size[0] : dim_size[0] * dim_size[1]);
      MDWaveDims dim_indx;
      ::MemClear(dim_indx, sizeof(MDWaveDims));
      Handle txt_hndl = ::NewHandle(0);
      if (txt_hndl == 0) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "out of memory error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not insert data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::insert");
      }
      char *tmp = 0;
      int  max_j = (ndims > 1) ? dim_size[1] : 1;
      for (int j = 0; j < max_j; j++)
      {
        for (int i = 0; i < dim_size[0]; i++) 
        {
          dim_indx[0] = i;
          dim_indx[1] = j;
          if (::MDGetTextWavePointValue(_wh, dim_indx, txt_hndl)) 
          {
            ::DisposeHandle(txt_hndl);
            char wname[MAX_OBJ_NAME + 1];
            ::WaveName(_wh, wname);
            TangoSys_OMemStream reason;
            reason << "XOP internal error" << ends;
            TangoSys_OMemStream desc;
            desc << "could not insert data from wave " << wname << ends;
            Tango::Except::throw_exception(reason.str(), 
                                           desc.str(), 
                                           (const char*)"DataCodec::insert");
          }
          tmp = CORBA::string_alloc(::GetHandleSize(txt_hndl) + 1);
          if (tmp == 0) 
          {
            ::DisposeHandle(txt_hndl);
            char wname[MAX_OBJ_NAME + 1];
            ::WaveName(_wh, wname);
            TangoSys_OMemStream reason;
            reason << "out of memory error" << ends;
            TangoSys_OMemStream desc;
            desc << "could not insert data from wave " << wname << ends;
            Tango::Except::throw_exception(reason.str(), 
                                           desc.str(), 
                                           (const char*)"DataCodec::insert");
          }
          ::MemClear(tmp, ::GetHandleSize(txt_hndl) + 1);
          ::memcpy(tmp, *txt_hndl, ::GetHandleSize(txt_hndl));
          (*array)[i + j * max_j] = tmp;
        }
      }
      DisposeHandle(txt_hndl);
	    _da << array;
    } break;
    //-------------------------------------------------
    default:
      char wname[MAX_OBJ_NAME + 1];
      ::WaveName(_wh, wname);
      TangoSys_OMemStream reason;
      reason << "invalid argument specified" << ends;
      TangoSys_OMemStream desc;
      desc << "could not insert data from wave " << wname << " (unsupported Igor data type)" << ends;
      Tango::Except::throw_exception(reason.str(), 
                                     desc.str(), 
                                     (const char*)"DataCodec::insert");
      break;
  }
}

//=============================================================================
// TEMPLATE CLASS Extract
//=============================================================================
template <class A, class T = unsigned char> class Extract 
{
public: 
  //-- extract numeric wave from CORBA::Any
  static int num_wave (waveHndl _wh, Tango::DeviceData& _dd)
  {
    const A * array = 0;

    if((_dd >> array) == false) 
      return kError;
    
    return Extract<A,T>::num_wave(array, _wh, array->length(), 0);
  }

  //-- extract numeric wave from CORBA::sequence
  static int num_wave (const A *_seq, waveHndl _wh, long dim_x, long dim_y = 0)
  {
    int w_ndims = 0;
    MDWaveDims w_dims;
    ::MemClear(w_dims, sizeof(MDWaveDims));
    if (_seq == 0 || _seq->length() == 0)
    {
      if (::MDChangeWave(_wh, ::WaveType(_wh), w_dims))
          return kError;
      return kNoError;
    }
    if (::MDGetWaveDimensions(_wh, &w_ndims, w_dims))
      return kError;
    int actual_ndims = dim_y ? 2 : 1;
    switch (actual_ndims)
    {
      case 1:
        if (w_dims[0] != dim_x || w_ndims != actual_ndims) 
        {
          ::MemClear(w_dims, sizeof(MDWaveDims));
          w_dims[0] = dim_x;
          w_dims[1] = 0;
          if (::MDChangeWave(_wh, ::WaveType(_wh), w_dims))
            return kError;
        }
        break;
      case 2:
        if (w_dims[0] != dim_x || w_ndims != actual_ndims || w_dims[1] != dim_y) 
        {
          ::MemClear(w_dims, sizeof(MDWaveDims));
          w_dims[0] = dim_x;
          w_dims[1] = dim_y;
          if (::MDChangeWave(_wh, ::WaveType(_wh), w_dims))
            return kError;
        }
        break;
    }
    BCInt offset;
    if (::MDAccessNumericWaveData(_wh, kMDWaveAccessMode0, &offset))
      return kError;
    char *base_addr = (char*)(*_wh) + offset;
    size_t size_to_copy = dim_x * (dim_y ? dim_y : 1);
    ::memcpy(base_addr, _seq->get_buffer(), size_to_copy * sizeof(T));
    return kNoError;
  }

  //-- extract text wave from CORBA::Any
  static int txt_wave (waveHndl _wh, Tango::DeviceData& _dd)
  {
    const A * array = 0;

    if((_dd >> array) == false) 
      return kError;

    return Extract<A,T>::txt_wave(array, _wh, array->length());
  }

  //-- extract text wave from CORBA::sequence
  static int txt_wave (const A* _seq, waveHndl _wh, long dim_x, long dim_y = 0)
  {
    int w_ndims = 0;
    MDWaveDims w_dims;
    ::MemClear(w_dims, sizeof(MDWaveDims));
    if (_seq == 0 || _seq->length() == 0)
    {
      if (::MDChangeWave(_wh, TEXT_WAVE_TYPE, w_dims))
          return kError;
      return kNoError;
    }
    if (::MDGetWaveDimensions(_wh, &w_ndims, w_dims))
      return kError;
    int actual_ndims = dim_y ? 2 : 1;
    switch (actual_ndims)
    {
      case 1:
        if (w_dims[0] != dim_x || w_ndims != actual_ndims) 
        {
          ::MemClear(w_dims, sizeof(MDWaveDims));
          w_dims[0] = dim_x;
          w_dims[1] = 0;
          if (::MDChangeWave(_wh, TEXT_WAVE_TYPE, w_dims))
            return kError;
        }
        break;
      case 2:
        if (w_dims[0] != dim_x || w_ndims != actual_ndims || w_dims[1] != dim_y) 
        {
          ::MemClear(w_dims, sizeof(MDWaveDims));
          w_dims[0] = dim_x;
          w_dims[1] = dim_y;
          if (::MDChangeWave(_wh, TEXT_WAVE_TYPE, w_dims))
            return kError;
        }
        break;
    }
    Handle txt_hndl = ::NewHandle(0L);
    if (txt_hndl == 0)
      return kError;
    if (! dim_y)
      dim_y++;
    for (int j = 0; j < dim_y; j++) 
    {
      for (int i = 0; i < dim_x; i++) 
      {
        w_dims[0] = i;
        w_dims[1] = j;
        ::SetHandleSize(txt_hndl, ::strlen((*_seq)[i + j * dim_y]));
        if (::MemError())
        {
          ::DisposeHandle(txt_hndl);
          return kError;
        }
        ::memcpy(*txt_hndl, (*_seq)[i + j * dim_y], ::strlen((*_seq)[i + j * dim_y]) * sizeof(T));
        ::MDSetTextWavePointValue(_wh, w_dims, txt_hndl);
      }
    }
    ::DisposeHandle(txt_hndl);
    return kNoError;
  }
};

// ============================================================================
// DataCodec::extract - extracts an Igor Pro wave from a CORBA::sequence
// ============================================================================
void DataCodec::extract (const Tango::DeviceData& _const_dd, waveHndl _wh)
{
  Tango::DeviceData& _dd = const_cast<Tango::DeviceData&>(_const_dd);

  switch (::WaveType(_wh)) 
  {
    //-------------------------------------------------
    case NT_I8|NT_UNSIGNED: 
    {
      if (Extract<Tango::DevVarCharArray, unsigned char>::num_wave(_wh, _dd)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends; 
        TangoSys_OMemStream desc;
        desc << "could not extract data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::extract");
      }   
    } break;
    //-------------------------------------------------
    case NT_I16:
    {
      if (Extract<Tango::DevVarShortArray, short>::num_wave(_wh, _dd)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not extract data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::extract");
      }   
    } break;
    //-------------------------------------------------
    case NT_I16|NT_UNSIGNED:
    {
      if (Extract<Tango::DevVarUShortArray, unsigned short>::num_wave(_wh, _dd)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not extract data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::extract");
      }   
    } break;
    //-------------------------------------------------
    case NT_I32:
    {
      if (Extract<Tango::DevVarLongArray, long>::num_wave(_wh, _dd)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not extract data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::extract");
      }   
    } break;
    //-------------------------------------------------
    case NT_I32|NT_UNSIGNED:
    {
      if (Extract<Tango::DevVarULongArray, unsigned long>::num_wave(_wh, _dd)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not extract data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::extract");
      }   
    } break;
    //-------------------------------------------------
    case NT_FP32:
    {
      if (Extract<Tango::DevVarFloatArray, float>::num_wave(_wh, _dd)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not extract data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::extract");
      }   
    } break;
    //-------------------------------------------------
    case NT_FP64:
    {
      if (Extract<Tango::DevVarDoubleArray, double>::num_wave(_wh, _dd)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not extract data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::extract");
      }   
    } break;
    //-------------------------------------------------
    case TEXT_WAVE_TYPE:
    {
      if (Extract<Tango::DevVarStringArray, unsigned char>::txt_wave(_wh, _dd)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not extract data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::extract");
      }   
    } break;
    //-------------------------------------------------
    default:
      char wname[MAX_OBJ_NAME + 1];
      ::WaveName(_wh, wname);
      TangoSys_OMemStream reason;
      reason << "XOP internal error" << ends;
      TangoSys_OMemStream desc;
      desc << "unsupported TANGO data type" << ends;
      Tango::Except::throw_exception(reason.str(), 
                                     desc.str(), 
                                     (const char*)"DataCodec::extract");
      break;
  }
} 

// ============================================================================
// DataCodec::extract
// ============================================================================
void DataCodec::extract (const Tango::DeviceData& _const_dd, waveHndl _wn, waveHndl _ws) 
{
  Tango::DeviceData& _dd = const_cast<Tango::DeviceData&>(_const_dd);

  Tango::DevVarStringArray * s_value = 0;

  switch (::WaveType(_wn))
  {
    //-------------------------------------------------
    case NT_I32:
    {
      const Tango::DevVarLongStringArray * ls_array;
      _dd >> ls_array;
      const Tango::DevVarLongArray * l_value = &ls_array->lvalue;
      if (Extract<Tango::DevVarLongArray, long>::num_wave(l_value, _wn, l_value->length())) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wn, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not extract data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::extract");
      }
      s_value = &(const_cast<Tango::DevVarLongStringArray*>(ls_array)->svalue);
    } break;
    //-------------------------------------------------
    case NT_FP64:
    {
      const Tango::DevVarDoubleStringArray * ds_array;
      _dd >> ds_array;
      const Tango::DevVarDoubleArray * d_value = &ds_array->dvalue;
      if (Extract<Tango::DevVarDoubleArray, double>::num_wave(d_value, _wn, d_value->length())) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wn, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not extract data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::extract");
      }
      s_value = &(const_cast<Tango::DevVarDoubleStringArray*>(ds_array)->svalue);
    } break;
  }
  if (Extract<Tango::DevVarStringArray>::txt_wave(s_value, _ws, s_value->length())) 
  {
    char wname[MAX_OBJ_NAME + 1];
    ::WaveName(_ws, wname);
    TangoSys_OMemStream reason;
    reason << "XOP internal error" << ends;
    TangoSys_OMemStream desc;
    desc << "could not extract data into wave " << wname << ends;
    Tango::Except::throw_exception(reason.str(), 
                                   desc.str(), 
                                   (const char*)"DataCodec::extract");
    return;
  }
}

// ============================================================================
// DataCodec::extract
// ============================================================================
void DataCodec::extract (const Tango::DeviceAttribute& _const_da, waveHndl _wh, long _dim_x, long _dim_y)
{
  Tango::DeviceAttribute& _da = const_cast<Tango::DeviceAttribute&>(_const_da);

  switch (::WaveType(_wh)) 
  {
    //-------------------------------------------------
    case NT_I8:
    {
      if (sizeof(Tango::DevBoolean) != sizeof(char)) 
      {
        Tango::Except::throw_exception((const char*)"internal error",
                                       (const char*)"can't extract boolean data", 
                                       (const char*)"DataCodec::extract");
      }
      Tango::DevVarBooleanArray *tango_array = 0;
      _da >> tango_array;
      if (Extract<Tango::DevVarBooleanArray, bool>::num_wave(tango_array, _wh, _dim_x, _dim_y)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not extract data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::extract");
      }  
#if defined (__REUSABLE_ATTR_VALUE__)
      Tango::AttrQuality qlt = _da.quality;
      _da.insert(tango_array, _dim_x, _dim_y);
      _da.quality = qlt;
#else
      delete tango_array;
#endif
    } break;
    //-------------------------------------------------
    case NT_I8|NT_UNSIGNED:
    {
      Tango::DevVarCharArray *tango_array = 0;
      _da >> tango_array;
      if (Extract<Tango::DevVarCharArray, unsigned char>::num_wave(tango_array, _wh, _dim_x, _dim_y)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not extract data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::extract");
      }   
#if defined (__REUSABLE_ATTR_VALUE__)
      Tango::AttrQuality qlt = _da.quality;
      _da.insert(tango_array, _dim_x, _dim_y);
      _da.quality = qlt;
#else
      delete tango_array;
#endif
    } break;
    //-------------------------------------------------
    case NT_I16|NT_UNSIGNED:
    {
      Tango::DevVarUShortArray *tango_array = 0;
      _da >> tango_array;
      if (Extract<Tango::DevVarUShortArray, unsigned short>::num_wave(tango_array, _wh, _dim_x, _dim_y)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not extract data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::extract");
      }   
#if defined (__REUSABLE_ATTR_VALUE__)
      Tango::AttrQuality qlt = _da.quality;
      _da.insert(tango_array, _dim_x, _dim_y);
      _da.quality = qlt;
#else
      delete tango_array;
#endif
    } break;
    //-------------------------------------------------
    case NT_I16:
    {
      Tango::DevVarShortArray *tango_array = 0;
      _da >> tango_array;
      if (Extract<Tango::DevVarShortArray, short>::num_wave(tango_array, _wh, _dim_x, _dim_y)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not extract data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::extract");
      }  
#if defined (__REUSABLE_ATTR_VALUE__)
      Tango::AttrQuality qlt = _da.quality;
      _da.insert(tango_array, _dim_x, _dim_y);
      _da.quality = qlt;
#else
      delete tango_array;
#endif
    } break;
    //-------------------------------------------------
    case NT_I32:
    {
      Tango::DevVarLongArray *tango_array = 0;
      _da >> tango_array;
      if (Extract<Tango::DevVarLongArray, long>::num_wave(tango_array, _wh, _dim_x, _dim_y)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not extract data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::extract");
      }  
#if defined (__REUSABLE_ATTR_VALUE__)
      Tango::AttrQuality qlt = _da.quality;
      _da.insert(tango_array, _dim_x, _dim_y);
      _da.quality = qlt;
#else
      delete tango_array;
#endif
    } break;
    //-------------------------------------------------
    case NT_FP32:
    {
      Tango::DevVarFloatArray *tango_array = 0;
      _da >> tango_array;
      if (Extract<Tango::DevVarFloatArray, float>::num_wave(tango_array, _wh, _dim_x, _dim_y)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not extract data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::extract");
      }  
#if defined (__REUSABLE_ATTR_VALUE__)
      Tango::AttrQuality qlt = _da.quality;
      _da.insert(tango_array, _dim_x, _dim_y);
      _da.quality = qlt;
#else
      delete tango_array;
#endif
    } break;
    //-------------------------------------------------
    case NT_FP64:
    {
      Tango::DevVarDoubleArray * tango_array = 0;
      _da >> tango_array;
      if (Extract<Tango::DevVarDoubleArray, double>::num_wave(tango_array, _wh, _dim_x, _dim_y)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not extract data from wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::extract");
      } 
#if defined (__REUSABLE_ATTR_VALUE__)
      Tango::AttrQuality qlt = _da.quality;
      _da.insert(tango_array, _dim_x, _dim_y);
      _da.quality = qlt;
#else
      delete tango_array;
#endif
    } break;
    //-------------------------------------------------
    case TEXT_WAVE_TYPE:
    {
      Tango::DevVarStringArray * tango_array = 0;
      _da >> tango_array;
      if (Extract<Tango::DevVarStringArray>::txt_wave(tango_array, _wh, _dim_x, _dim_y)) 
      {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not extract data into wave " << wname << ends;
        Tango::Except::throw_exception(reason.str(), 
                                       desc.str(), 
                                       (const char*)"DataCodec::extract");
        return;
      }
#if defined (__REUSABLE_ATTR_VALUE__)
      Tango::AttrQuality qlt = _da.quality;
      _da.insert(tango_array, _dim_x, _dim_y);
      _da.quality = qlt;
#else
      delete tango_array;
#endif
    } break;
    //-------------------------------------------------
    default:
    {
      char wname[MAX_OBJ_NAME + 1];
      ::WaveName(_wh, wname);
      TangoSys_OMemStream reason;
      reason << "XOP internal error" << ends;
      TangoSys_OMemStream desc;
      desc << "could not extract data from wave " << wname << " (unsupported TANGO data type)" << ends;
      Tango::Except::throw_exception(reason.str(), 
                                     desc.str(), 
                                     (const char*)"DataCodec::extract");
    } break;
  }
}
