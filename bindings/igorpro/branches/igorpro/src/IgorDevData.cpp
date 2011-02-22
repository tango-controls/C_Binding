// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for Igor Pro
//
// = FILENAME
//   IgorDevData.cpp
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

//=============================================================================
// DEPENDENCIES
//=============================================================================
#include "XDK_StandardHeaders.h"
#include "TangoApi/TangoAPI.h"
#include "DevRepository.h"
#include "IgorDevData.h"

#if !defined (__XDK_INLINE__)
  #include "IgorDevData.i"
#endif 


//=============================================================================
// CONSTs
//=============================================================================
// # of secs elapsed between 1/1/1904 (Igor ref) and 1/1/1970 (TANGO ref)
static const DOUBLE k_time_offset = 2082844800.0;

//=============================================================================
// STATIC MEMBERS
//=============================================================================
IgorDevData* IgorDevData::instance_ = 0;

//=============================================================================
// IgorDevData::init
//=============================================================================
int IgorDevData::init (void)
{
  if (IgorDevData::instance_ != 0)
    return 0;

  IgorDevData::instance_ = new IgorDevData;

  return (IgorDevData::instance_) ? 0 : -1;
}

//=============================================================================
// IgorDevData::cleanup
//=============================================================================
void IgorDevData::cleanup (void)
{
  if (IgorDevData::instance_) {
    delete IgorDevData::instance_;
    IgorDevData::instance_ = 0;
  }
}

//=============================================================================
// IgorDevData::IgorDevData 
//=============================================================================
IgorDevData::IgorDevData (void)
{
 // no-op ctor
}

//=============================================================================
// IgorDevData::~IgorDevData 
//=============================================================================
IgorDevData::~IgorDevData (void)
{
 // no-op dtor
}

//=============================================================================
// IgorDevData::encode_argin
//=============================================================================
int IgorDevData::encode_argin (DevDescriptor* _ddesc, 
                               const std::string& _argin, 
                               int _cmd_id,
                               TangoApi::DevData& dd_in_)
{
  if (_ddesc == 0) {
    XOP_ERROR(("IgorDevData::encode_argin::invalid argument (NULL)"));
    XDK_UTILS->set_error("XOP internal error",
                         "unexpected NULL argument",
                         "IgorDevData::encode_argin");
    return kError;
  }

  DFHndl dfh;
  std::string obj_name;
  if (XDK_UTILS->get_obj_df(_argin, dfh, obj_name) != kNoError) {
    XOP_ERROR(("IgorDevData::encode_argin::get_obj_df failed"));
    XDK_UTILS->set_error("XOP internal error",
                         "XDK_Utils::get_obj_df failed",
                         "IgorDevData::encode_argin");
    return kError;
  }
  
  //- does the user provide the actual argin or a string containing argin?
  //- first try to get a global string var named <_argin> in <dfh>
  //- if there is no string var named <_argin> in <dfh> <_argin> is unchanged
  //- otherwise <_argin> becomes equal to its content  
  XDK_UTILS->get_df_obj(dfh, obj_name, obj_name);

  try {
    int argin_type = (_ddesc->cmd_list())[_cmd_id].in_type;
    switch (argin_type) {
      //-- DEV_VOID ------------------------------------
      case Tango::DEV_VOID: 
      {
        return kNoError; 
      } break;
      //-- DEVVAR_LONGSTRINGARRAY ----------------------
      case Tango::DEVVAR_LONGSTRINGARRAY: 
      {
        std::string::size_type pos = obj_name.find(kNameSep);
        if (pos == std::string::npos) {
          std::string d = "argin syntax error (" + obj_name + ")";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "IgorDevData::encode_argin");
          return kError; 
        }
        //-- LONG WAVE TO VECTOR<LONG>
        waveHndl lw;
        std::string lw_name;
        lw_name.assign(obj_name, 0, pos);
        if (XDK_UTILS->fetch_wave(&lw, dfh, lw_name, NT_I32, 1)) {
          std::string d = "wave <" + lw_name;
          d += "> either does not exists";
          d += " or has incompatible type/dimensions";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "IgorDevData::encode_argin");
          return kError; 
        }
        //-- TEXT WAVE TO VECTOR<STRING>
        waveHndl sw;
        std::string sw_name;
        sw_name.assign(obj_name, pos + 1, obj_name.size());
        if (XDK_UTILS->fetch_wave(&sw, dfh, sw_name, TEXT_WAVE_TYPE, 1)) {
          std::string d = "wave <" + sw_name;
          d += "> either does not exists";
          d += " or has incompatible type/dimensions";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "IgorDevData::encode_argin");
          return kError; 
        }
        dd_in_.insert(lw, sw);
      } break;
      //-- DEVVAR_DOUBLESTRINGARRAY --------------------
      case Tango::DEVVAR_DOUBLESTRINGARRAY: 
      {
        std::string::size_type pos = obj_name.find(kNameSep);
        if (pos == std::string::npos) {
          std::string d = "argin syntax error (" + _argin + ")";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "IgorDevData::encode_argin");
          return kError; 
        }
        //-- LONG WAVE TO VECTOR<DOUBLE>
        waveHndl dw;
        std::string dw_name;
        dw_name.assign(obj_name, 0, pos);
        if (XDK_UTILS->fetch_wave(&dw, dfh, dw_name, NT_FP64, 1)) {
          std::string d = "wave <" + dw_name + ">";
          d += " either does not exists";
          d += " or has incompatible type/dimensions";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "IgorDevData::encode_argin");
          return kError; 
        }
        //-- TEXT WAVE TO VECTOR<STRING>
        waveHndl sw;
        std::string sw_name;
        sw_name.assign(obj_name, pos + 1, obj_name.size());
        if (XDK_UTILS->fetch_wave(&sw, dfh, sw_name, TEXT_WAVE_TYPE, 1)) {
          std::string d = "wave <" + sw_name + ">";
          d += " either does not exists";
          d += " or has incompatible type/dimensions";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "IgorDevData::encode_argin");
          return kError; 
        }
        dd_in_.insert(dw, sw);
      } break;
      //-- DEVVAR_STRINGARRAY --------------------------
      case Tango::DEVVAR_STRINGARRAY: 
      {
        waveHndl wave_in;
        if (XDK_UTILS->fetch_wave(&wave_in, dfh, obj_name, TEXT_WAVE_TYPE, 1)) {
          std::string d = "wave <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type/dimensions";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "IgorDevData::encode_argin");
          return kError;  
        }
        dd_in_ << wave_in;
      } break;
      //-- DEVVAR_DOUBLEARRAY --------------------------
      case Tango::DEVVAR_DOUBLEARRAY: 
      {
        waveHndl wave_in;
        if (XDK_UTILS->fetch_wave(&wave_in, dfh, obj_name, NT_FP64, 1)) {
          std::string d = "wave <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type/dimensions";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "IgorDevData::encode_argin");
          return kError;  
        }
        dd_in_ << wave_in;
      } break;
      //-- DEVVAR_FLOATARRAY ---------------------------
      case Tango::DEVVAR_FLOATARRAY: 
      {
        waveHndl wave_in;
        if (XDK_UTILS->fetch_wave(&wave_in, dfh, obj_name, NT_FP32, 1)) {
          std::string d = "wave <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type/dimensions";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "IgorDevData::encode_argin");
          return kError;  
        }
        dd_in_ << wave_in;
      } break;
      //-- DEVVAR_ULONGARRAY ---------------------------
      case Tango::DEVVAR_ULONGARRAY: 
      {
        waveHndl wave_in;
        if (XDK_UTILS->fetch_wave(&wave_in, dfh, obj_name, NT_I32|NT_UNSIGNED, 1)) {
          std::string d = "wave <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type/dimensions";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "IgorDevData::encode_argin");
          return kError;  
        }
        dd_in_ << wave_in;
      } break;
      //-- DEVVAR_LONGARRAY ---------------------------
      case Tango::DEVVAR_LONGARRAY: 
      {
        waveHndl wave_in;
        if (XDK_UTILS->fetch_wave(&wave_in, dfh, obj_name, NT_I32, 1)) {
          std::string d = "wave <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type/dimensions";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "IgorDevData::encode_argin");
          return kError;  
        }
        dd_in_ << wave_in;
      } break;
      //-- DEVVAR_USHORTARRAY --------------------------
      case Tango::DEVVAR_USHORTARRAY: 
      {
        waveHndl wave_in;
        if (XDK_UTILS->fetch_wave(&wave_in, dfh, obj_name, NT_I16|NT_UNSIGNED, 1)) {
          std::string d = "wave <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type/dimensions";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "IgorDevData::encode_argin");
          return kError;   
        }
        dd_in_ << wave_in;
      } break;
      //-- DEVVAR_SHORTARRAY ---------------------------
      case Tango::DEVVAR_SHORTARRAY: 
      {
        waveHndl wave_in;
        if (XDK_UTILS->fetch_wave(&wave_in, dfh, obj_name, NT_I16, 1)) {
          std::string d = "wave <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type/dimensions";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "IgorDevData::encode_argin");
          return kError;  
        }
        dd_in_ << wave_in;
      } break;
      //-- DEVVAR_CHARARRAY -- TO BE MODIFIED : PB WITH TANGO 1.6 --
      case Tango::DEVVAR_CHARARRAY: 
      {
        waveHndl wave_in;
        if (XDK_UTILS->fetch_wave(&wave_in, dfh, obj_name, NT_I8|NT_UNSIGNED, 1)) {
          std::string d = "wave <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type/dimensions";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "IgorDevData::encode_argin");
          return kError;   
        }
        dd_in_ << wave_in;
      } break;
      //-- DEV_STATE -----------------------------------
      case Tango::DEV_STATE: 
      {
        XDK_UTILS->set_error("XOP internal error",
                             "unsupported argin type (DEV_STATE)",
                             "IgorDevData::encode_argin");
        return kError;  
      } break;
      //-- CONST_DEV_STRING ----------------------------
      case Tango::CONST_DEV_STRING: 
      {
        XDK_UTILS->set_error("XOP internal error",
                             "unsupported argin type (CONST_DEV_STRING)",
                             "IgorDevData::encode_argin");
        return kError;  
      } break;
      //-- DEV_STRING ----------------------------------
      case Tango::DEV_STRING: 
      {
        dd_in_ << const_cast<std::string&>(obj_name);
      } break;
      //-- DEV_USHORT ----------------------------------
      case Tango::DEV_BOOLEAN: 
      {
        DOUBLE d;
        if (XDK_UTILS->get_df_obj (dfh, obj_name, &d)) {
          std::string d = "global variable <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "IgorDevData::encode_argin");
          return kError; 
        }
        bool b = d ? true : false;
        dd_in_ << b;
      } break;
      //-- DEV_USHORT ----------------------------------
      case Tango::DEV_USHORT: 
      {
        DOUBLE d;
        if (XDK_UTILS->get_df_obj (dfh, obj_name, &d)) {
          std::string d = "global variable <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "IgorDevData::encode_argin");
          return kError; 
        }
        unsigned short us;
        if (::ConvertData2(&d, &us, 1, NT_FP64, NT_I16|NT_UNSIGNED)) {
          XDK_UTILS->set_error("XOP internal error",
                               "ConvertData2 failed",
                               "IgorDevData::encode_argin");
          return kError;
        }
        dd_in_ << us;
      } break;
      //-- DEV_SHORT -----------------------------------
      case Tango::DEV_SHORT: 
      {
        DOUBLE d;
        if (XDK_UTILS->get_df_obj (dfh, obj_name, &d)) {
          std::string d = "global variable <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "IgorDevData::encode_argin");
          return kError; 
        }
        short s;
        if (::ConvertData2(&d, &s, 1, NT_FP64, NT_I16)) {
          XDK_UTILS->set_error("XOP internal error",
                               "ConvertData2 failed",
                               "IgorDevData::encode_argin");
          return kError;
        }
        dd_in_ << s;
      } break;
      //-- DEV_ULONG -----------------------------------
      case Tango::DEV_ULONG: 
      {
        DOUBLE d;
        if (XDK_UTILS->get_df_obj (dfh, obj_name, &d)) {
          std::string d = "global variable <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "IgorDevData::encode_argin");
          return kError; 
        }
        unsigned long ul;
        if (::ConvertData2(&d, &ul, 1, NT_FP64, NT_I32|NT_UNSIGNED)) {
          XDK_UTILS->set_error("XOP internal error",
                               "ConvertData2 failed",
                               "IgorDevData::encode_argin");
          return kError;
        }
        dd_in_ << ul;
      } break;
      //-- DEV_LONG ------------------------------------
      case Tango::DEV_LONG: 
      {
        DOUBLE d;
        if (XDK_UTILS->get_df_obj (dfh, obj_name, &d)) {
          std::string d = "global variable <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "IgorDevData::encode_argin");
          return kError; 
        }
        long l;
        if (::ConvertData2(&d, &l, 1, NT_FP64, NT_I32)) {
          XDK_UTILS->set_error("XOP internal error",
                               "ConvertData2 failed",
                               "IgorDevData::encode_argin");
          return kError;
        }
        dd_in_ << l;
      } break;
      //-- DEV_FLOAT -----------------------------------
      case Tango::DEV_FLOAT: 
      {
        DOUBLE d;
        if (XDK_UTILS->get_df_obj (dfh, obj_name, &d)) {
          std::string d = "global variable <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "IgorDevData::encode_argin");
          return kError; 
        }
        float f;
        if (::ConvertData2(&d, &f, 1, NT_FP64, NT_FP32)) {
          XDK_UTILS->set_error("XOP internal error",
                               "ConvertData2 failed",
                               "IgorDevData::encode_argin");
          return kError;
        }
        dd_in_ << f;
      } break;
      //-- DEV_DOUBLE ----------------------------------
      case Tango::DEV_DOUBLE: 
      {
        DOUBLE d;
        if (XDK_UTILS->get_df_obj (dfh, obj_name, &d)) {
          std::string d = "global variable <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "IgorDevData::encode_argin");
          return kError;
        }
        dd_in_ << d;
      } break;
      //-- DEFAULT BEHAVIOUR ---------------------------
      default:
        std::string d = "unexpected TANGO data type for argin <" + _argin + ">";
        XDK_UTILS->set_error("unknown TANGO data type",
                              d.c_str(),
                             "IgorDevData::encode_argin");
        return kError; 
        break;
    } // switch 
  } //try
  catch (const Tango::DevFailed &e) {
    XDK_UTILS->set_error(e);
		return kError;
	}
  catch (...) {
    XDK_UTILS->set_error("unknown error",
                         "unknown exception caught",
                         "IgorDevData::encode_argin");
    return kError;
  }
  return kNoError;
}


//=============================================================================
// IgorDevData::decode_argout
//=============================================================================
int IgorDevData::decode_argout (DevDescriptor* _ddesc, 
                                  const std::string& _argout, 
                                  int _cmd_id,
                                  TangoApi::DevData& dd_out_)
{
  if (_ddesc == 0) {
    XDK_UTILS->set_error("XOP internal error",
                         "unexpected NULL argument",
                         "IgorDevData::decode_argout");
    return kError;
  }

  DFHndl dfh;
  std::string obj_name;
  if (XDK_UTILS->get_obj_df(_argout, dfh, obj_name, 1) != kNoError) {
    XOP_ERROR(("IgorDevData::encode_argin::get_obj_df failed"));
    XDK_UTILS->set_error("XOP internal error",
                         "XDK_Utils::get_obj_df failed",
                         "IgorDevData::decode_argout");
    return kError;
  }

  try 
  {
    waveHndl num_wave = 0;
    waveHndl str_wave = 0;
    int argout_type = (_ddesc->cmd_list())[_cmd_id].out_type;
    switch (argout_type) 
    {
      case Tango::DEV_VOID: 
      {
        return kNoError; 
      } break;
      case Tango::DEVVAR_LONGSTRINGARRAY:
      {
        // 
        std::string::size_type pos = obj_name.find(kNameSep);
        if (pos == std::string::npos) {
          std::string d = "argout syntax error (" + _argout + ")";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "IgorDevData::decode_argout");
          return kError; 
        }
        // get num_wave name
        std::string num_wname;
        num_wname.assign(obj_name, 0, pos);
        // fetch wave num_wave
        if (XDK_UTILS->fetch_or_make_wave(&num_wave, dfh, num_wname, NT_I32, XDK_Utils::TYPE)) {
          std::string d = "failed to make or change wave <" + num_wname + ">";
          d += " (object name conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "IgorDevData::decode_argout");
          return kError;
        }
        // get str_wave name
        std::string str_wname;
        str_wname.assign(obj_name, pos + 1, obj_name.size());
        // fetch_wave str_wave
        if (XDK_UTILS->fetch_or_make_wave(&str_wave, dfh, str_wname, TEXT_WAVE_TYPE, XDK_Utils::TYPE)) {
          std::string d = "failed to make or change wave <" + str_wname + ">";
          d += " (object name conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "IgorDevData::decode_argout");
          return kError;
        }
        dd_out_.extract(num_wave, str_wave);
      } break;
      case Tango::DEVVAR_DOUBLESTRINGARRAY: 
      {
        std::string::size_type pos = obj_name.find(kNameSep);
        if (pos == std::string::npos) {
          std::string d = "argout syntax error (" + _argout + ")";
          XDK_UTILS->set_error("argout syntax error",
                               d.c_str(),
                               "IgorDevData::decode_argout");
          return kError; 
        }
        // get num_wave name
        std::string num_wname;
        num_wname.assign(obj_name, 0, pos);
        // fetch wave num_wave
        if (XDK_UTILS->fetch_or_make_wave(&num_wave, dfh, num_wname, NT_FP64, XDK_Utils::TYPE)) {
          std::string d = "failed to make or change wave <" + num_wname + ">";
          d += " (object name conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "IgorDevData::decode_argout");
          return kError; 
        }
        // get str_wave name
        std::string str_wname;
        str_wname.assign(obj_name, pos + 1, obj_name.size());
        // fetch_wave str_wave
        if (XDK_UTILS->fetch_or_make_wave(&str_wave, dfh, str_wname, TEXT_WAVE_TYPE, XDK_Utils::TYPE)) {
          std::string d = "failed to make or change wave <" + str_wname + ">";
          d += " (object name conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "IgorDevData::decode_argout");
          return kError;
        }
        dd_out_.extract(num_wave, str_wave);
      } break;
      case Tango::DEVVAR_STRINGARRAY: 
      { 
        if (XDK_UTILS->fetch_or_make_wave(&str_wave, dfh, obj_name, TEXT_WAVE_TYPE, XDK_Utils::TYPE)) {
          std::string d = "failed to make or change wave <" + _argout + ">";
          d += " (object name conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "IgorDevData::decode_argout");
          return kError;
        }
        dd_out_ >> str_wave;
      } break;
      case Tango::DEVVAR_DOUBLEARRAY: 
      { 
        if (XDK_UTILS->fetch_or_make_wave(&num_wave, dfh, obj_name, NT_FP64, XDK_Utils::TYPE)) {
          std::string d = "failed to make or change wave <" + _argout + ">";
          d += " (object name conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "IgorDevData::decode_argout");
          return kError;
        }
        dd_out_ >> num_wave;
      } break;
      case Tango::DEVVAR_FLOATARRAY: 
      {
        if (XDK_UTILS->fetch_or_make_wave(&num_wave, dfh, obj_name, NT_FP32, XDK_Utils::TYPE)) {
          std::string d = "failed to make or change wave <" + _argout + ">";
          d += " (object name conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "IgorDevData::decode_argout");
          return kError; 
        }
        dd_out_ >> num_wave;
      } break;
      case Tango::DEVVAR_ULONGARRAY: 
      {
        if (XDK_UTILS->fetch_or_make_wave(&num_wave, dfh, obj_name, NT_I32|NT_UNSIGNED, XDK_Utils::TYPE)) {
          std::string d = "failed to make or change wave <" + _argout + ">";
          d += " (object name conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "IgorDevData::decode_argout");
          return kError; 
        }
        dd_out_ >> num_wave;
      } break;
      case Tango::DEVVAR_LONGARRAY: 
      { 
        if (XDK_UTILS->fetch_or_make_wave(&num_wave, dfh, obj_name, NT_I32, XDK_Utils::TYPE)) {
          std::string d = "failed to make or change wave <" + _argout + ">";
          d += " (object name conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "IgorDevData::decode_argout");
          return kError;
        }
        dd_out_ >> num_wave;
      } break;
      case Tango::DEVVAR_USHORTARRAY: 
      { 
        if (XDK_UTILS->fetch_or_make_wave(&num_wave, dfh, obj_name, NT_I16|NT_UNSIGNED, XDK_Utils::TYPE)) {
          std::string d = "failed to make or change wave <" + _argout + ">";
          d += " (object name conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "IgorDevData::decode_argout");
          return kError;
        }
        dd_out_ >> num_wave;
      } break;
      case Tango::DEVVAR_SHORTARRAY: 
      {
        if (XDK_UTILS->fetch_or_make_wave(&num_wave, dfh, obj_name, NT_I16, XDK_Utils::TYPE)) {
          std::string d = "failed to make or change wave <" + _argout + ">";
          d += " (object name conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "IgorDevData::decode_argout");
          return kError;
        }
        dd_out_ >> num_wave;
      } break;
      case Tango::DEVVAR_CHARARRAY: 
      {
        if (XDK_UTILS->fetch_or_make_wave(&num_wave, dfh, obj_name, NT_I8|NT_UNSIGNED, XDK_Utils::TYPE)) {
          std::string d = "failed to make or change wave <" + _argout + ">";
          d += " (object name conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "IgorDevData::decode_argout");
          return kError;
        }
        dd_out_ >> num_wave;
      } break;
      case Tango::DEV_STRING: 
      {
        std::string s;
        dd_out_ >> s;
        if (XDK_UTILS->create_df_obj(dfh, obj_name, s) != kNoError) {
          std::string d = "failed to set global string <" + _argout + ">";
          d += " (object name conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "IgorDevData::decode_argout");
          return kError;
        }
      } break;
      case Tango::DEV_BOOLEAN: 
      {
        bool b;
        dd_out_ >> b;
        long l = static_cast<long>(b);
        if (XDK_UTILS->create_df_obj(dfh, obj_name, l) != kNoError) {
          std::string d = "failed to set global variable <" + _argout + ">";
          d += " (object name conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "IgorDevData::decode_argout");
          return kError;
        }
      } break;
      case Tango::DEV_USHORT: 
      {
        unsigned short us;
        dd_out_ >> us;
        long l = static_cast<long>(us);
        if (XDK_UTILS->create_df_obj(dfh, obj_name, l) != kNoError) {
          std::string d = "failed to set global variable <" + _argout + ">";
          d += " (object name conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "IgorDevData::decode_argout");
          return kError;
        }
      } break;
      case Tango::DEV_SHORT: 
      {
        short s;
        dd_out_ >> s;
        long l = static_cast<long>(s);
        if (XDK_UTILS->create_df_obj(dfh, obj_name, l) != kNoError) {
          std::string d = "failed to set global variable <" + _argout + ">";
          d += " (object name conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "IgorDevData::decode_argout");
          return kError;
        }
      } break;
      case Tango::DEV_ULONG: 
      {
        unsigned long ul;
        dd_out_ >> ul;
        long l = static_cast<long>(ul);
        if (XDK_UTILS->create_df_obj(dfh, obj_name, l) != kNoError) {
          std::string d = "failed to set global variable <" + _argout + ">";
          d += " (object name conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "IgorDevData::decode_argout");
          return kError;
        }
      } break;
      case Tango::DEV_LONG: 
      {
        long l;
        dd_out_ >> l;
        if (XDK_UTILS->create_df_obj(dfh, obj_name, l) != kNoError) {
          std::string d = "failed to set global variable <" + _argout + ">";
          d += " (object name conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "IgorDevData::decode_argout");
          return kError;
        }
      } break;
      case Tango::DEV_FLOAT: 
      {
        float f;
        dd_out_ >> f;
        DOUBLE d = f;
        if (XDK_UTILS->create_df_obj(dfh, obj_name, d) != kNoError) {
          std::string d = "failed to set global variable <" + _argout + ">";
          d += " (object name conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "IgorDevData::decode_argout");
          return kError;
        }
      } break;
      case Tango::DEV_DOUBLE: 
      {
        double d;
        dd_out_ >> d;
        if (XDK_UTILS->create_df_obj(dfh, obj_name, d) != kNoError) {
          std::string d = "failed to set global variable <" + _argout + ">";
          d += " (object name conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "IgorDevData::decode_argout");
          return kError;
        }
      } break;
      case Tango::DEV_STATE: 
      {
        Tango::DevState ds;
        dd_out_ >> ds;
        if (XDK_UTILS->create_df_obj(dfh, obj_name, Tango::DevStateName[ds]) != kNoError) {
          std::string d = "failed to set global string <" + _argout + ">";
          d += " (object name conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "IgorDevData::decode_argout");
          return kError;
        }
      } break;
      case Tango::CONST_DEV_STRING: 
      {
        XDK_UTILS->set_error("XOP internal error",
                             "unsupported argout type (CONST_DEV_STRING)",
                             "IgorDevData::decode_argout");
        return kError; 
      } break;
      default:
        std::string d = "unexpected TANGO data type for argout <" + _argout + ">";
        XDK_UTILS->set_error("unknown TANGO data type",
                              d.c_str(),
                             "IgorDevData::decode_argout");
        return kError; 
        break;
    } // switch
    if (num_wave) ::WaveHandleModified(num_wave);
    if (str_wave) ::WaveHandleModified(str_wave);
  } // try        
  catch (const Tango::DevFailed &e) {
    XDK_UTILS->set_error(e);
		return kError;
	}
  catch (...) {
    XDK_UTILS->set_error("unknown error",
                         "unknown exception caught",
                         "IgorDevData::decode_argout");
    return kError;
  }

  return kNoError;
}


//=============================================================================
// IgorDevData::encode_attr
//=============================================================================
int IgorDevData::encode_attr (DevDescriptor* _ddesc, 
                                const std::string& _argin, 
                                int _attr_id,
                                TangoApi::DevAttrValue& value_)
{
  if (_ddesc == 0) {
    XOP_ERROR(("IgorDevData::encode_argin::invalid argument (NULL)"));
    XDK_UTILS->set_error("XOP internal error",
                         "unexpected NULL argument",
                         "IgorDevData::encode_attr");
    return kError;
  }

  DFHndl dfh;
  std::string obj_name;
  if (XDK_UTILS->get_obj_df(_argin, dfh, obj_name) != kNoError) {
    XOP_ERROR(("IgorDevData::encode_argin::get_obj_df failed"));
    XDK_UTILS->set_error("XOP internal error",
                         "XDK_Utils::get_obj_df failed",
                         "IgorDevData::encode_attr");
    return kError;
  }

  const TangoApi::DevAttrInfoList &attr_list = _ddesc->attr_list();
  //- attr format (scalar, spectrum or image)
  int attr_format = attr_list[_attr_id].data_format;
  //- attr data type (DEV_SHORT, DEV_LONG, ...)
  int attr_type = attr_list[_attr_id].data_type;

  //- set common values
  value_.quality = Tango::ATTR_VALID;
  value_.name = attr_list[_attr_id].name;
  value_.time.tv_sec  = 0;
  value_.time.tv_usec = 0;
  value_.time.tv_nsec = 0;

  //- does the user provide the actual argin or a string containing argin?
  //- first try to get a global string var named <_argin> in <dfh>
  //- if there is no string var named <_argin> in <dfh> <_argin> is unchanged
  //- <_argin> becomes equal to its content otherwise 
  XDK_UTILS->get_df_obj(dfh, obj_name, obj_name);

  try {
    switch (attr_format) 
    {
      case Tango::SCALAR: 
      {
        switch (attr_type) 
        {
          case Tango::DEV_STRING: 
          {
            vector<std::string> vstr(1);
            vstr[0] = _argin;
            value_.data << vstr;
            value_.dim_x = 1;
            value_.dim_y = 0;
          } break;
          case Tango::DEV_SHORT: 
          {
            DOUBLE d;
            if (XDK_UTILS->get_df_obj (dfh, obj_name, &d)) {
              std::string d = "global variable <" + _argin + ">";
              d += " either does not exists";
              d += " or has incompatible type";
              XDK_UTILS->set_error("invalid argin specified",
                                   d.c_str(),
                                   "IgorDevData::encode_attr");
              return kError; 
            }
            short s;
            if (::ConvertData2(&d, &s, 1, NT_FP64, NT_I16)) {
              XDK_UTILS->set_error("XOP internal error",
                                   "ConvertData2 failed",
                                   "IgorDevData::encode_attr");
              return kError;
            }
            vector<short> vs(1);
            vs[0] = s;
            value_.data << vs;
            value_.dim_x = 1;
            value_.dim_y = 0;
          } break;
          case Tango::DEV_LONG: 
          {
            DOUBLE d;
            if (XDK_UTILS->get_df_obj (dfh, obj_name, &d)) {
              std::string d = "global variable <" + _argin + ">";
              d += " either does not exists";
              d += " or has incompatible type";
              XDK_UTILS->set_error("invalid argin specified",
                                   d.c_str(),
                                   "IgorDevData::encode_attr");
              return kError; 
            }
            long l;
            if (::ConvertData2(&d, &l, 1, NT_FP64, NT_I32)) {
              XDK_UTILS->set_error("XOP internal error",
                                   "ConvertData2 failed",
                                   "IgorDevData::encode_attr");
              return kError;
            }
            vector<long> vl(1);
            vl[0] = l;
            value_.data << vl;
            value_.dim_x = 1;
            value_.dim_y = 0;
          } break;
          case Tango::DEV_DOUBLE: 
          {
            DOUBLE d;
            if (XDK_UTILS->get_df_obj (dfh, obj_name, &d)) {
              std::string d = "global variable <" + _argin + ">";
              d += " either does not exists";
              d += " or has incompatible type";
              XDK_UTILS->set_error("invalid argin specified",
                                   d.c_str(),
                                   "IgorDevData::encode_attr");
              return kError; 
            }
            vector<double> vd(1);
            vd[0] = d;
            value_.data << vd;
            value_.dim_x = 1;
            value_.dim_y = 0;
          } break;
          default:
            XDK_UTILS->set_error("unexpected TANGO data type",
                                 "unsupported TANGO data type for scalar attribute",
                                 "IgorDevData::encode_attr");
            return kError;
            break;
        } // switch::attr_type
      } break;
      case Tango::SPECTRUM: 
      case Tango::IMAGE: 
      {
        int itype;
        waveHndl num_wave = 0;
        switch (attr_type) 
        {
          case Tango::DEV_SHORT:
            itype = NT_I16;
            break;
          case Tango::DEV_LONG:
            itype = NT_I32;
            break;
          case Tango::DEV_DOUBLE:
            itype = NT_FP64;
            break;
          default: 
          {
            std::string format = (attr_format == Tango::IMAGE) ? "image" : "spectrum";
            std::string d = "unsupported TANGO data type for " + format + " attribute";
            XDK_UTILS->set_error("unexpected TANGO data type",
                                 d.c_str(),
                                 "IgorDevData::encode_attr");
            return kError;
          } break;
        }
        if (XDK_UTILS->fetch_or_make_wave(&num_wave, dfh, obj_name, itype)) {
          std::string d = "wave <" + _argin + ">";
          d += " either does not exists";
          d += " or has incompatible type/dimensions";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "IgorDevData::encode_attr");
          return kError; 
        }
        long w_ndims;
        MDWaveDims w_dims;
        ::MemClear(&w_dims, sizeof(MDWaveDims));
        if (::MDGetWaveDimensions(num_wave, &w_ndims, w_dims)) {
          XDK_UTILS->set_error("XOP internal error",
                               "MDGetWaveDimensions failed",
                               "IgorDevData::encode_attr");
          return kError;  
        }
        if (attr_format == Tango::SPECTRUM && w_ndims != 1) {
          std::string d = "wave <" + _argin + ">";
          d += " has incompatible dimensions (1D wave expected)";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "IgorDevData::encode_argin");
          return kError; 
        }
        else if (attr_format == Tango::IMAGE && w_ndims != 2) {
          std::string d = "wave <" + _argin + ">";
          d += " has incompatible dimensions (2D wave expected)";
          XDK_UTILS->set_error("invalid argin specified",
                               d.c_str(),
                               "IgorDevData::encode_argin");
          return kError;
        }
        value_.data << num_wave; 
        value_.dim_x = w_dims[0];
        value_.dim_y = (attr_format == Tango::IMAGE) ? w_dims[1] : 0;
      } break; 
      default:
        std::string d = "unsupported TANGO data type for attribute <" + attr_list[_attr_id].name + ">";
        XDK_UTILS->set_error("unexpected TANGO data type",
                              d.c_str(),
                             "IgorDevData::encode_argin");
        break;
    } // switch::attr_format
  } //try
  catch (const Tango::DevFailed &e) {
    XDK_UTILS->set_error(e);
		return kError;
	}
  catch (...) {
    XDK_UTILS->set_error("unknown error",
                         "unknown exception caught",
                         "IgorDevData::decode_attr");
    return kError;
  }
  return kNoError;
}

//=============================================================================
// IgorDevData::decode_attr
//=============================================================================
int IgorDevData::decode_attr (DevDescriptor* _ddesc, 
                              const std::string& _argout, 
                              int _attr_id,
                              TangoApi::DevAttrValue& value_)
{
  if (_ddesc == 0) {
    XDK_UTILS->set_error("XOP internal error",
                         "unexpected NULL argument",
                         "IgorDevData::decode_attr");
    return kError;
  }
  
  DFHndl dfh;
  std::string obj_name;
  if (XDK_UTILS->get_obj_df(_argout, dfh, obj_name, 1) != kNoError) {
    XOP_ERROR(("IgorDevData::encode_argin::get_obj_df failed"));
    XDK_UTILS->set_error("XOP internal error",
                         "XDK_Utils::get_obj_df failed",
                         "IgorDevData::decode_attr");
    return kError;
  }

  // for scalar::string, any::sprectrum and any::image
  // the timestamp is returned in a global variable 
  // named <obj_name + "_ts">. Other scalar types are 
  // mapped to complex variables which real part contains 
  // the actual attr value and imaginary part the timestamp
  //int create_global_tstamp = 0;
  //- attr value timestamp (igor time ref)
  DOUBLE tstamp = k_time_offset
                + value_.time.tv_sec 
                + 1.E-6 * value_.time.tv_usec 
                + 1.E-9 * value_.time.tv_nsec;
  try 
  {
    int err = kNoError;
    const TangoApi::DevAttrInfoList &attr_list = _ddesc->attr_list();
    //- attr access (DEV_SHORT, DEV_LONG, ...)
    Tango::AttrWriteType attr_write_type = attr_list[_attr_id].writable;
    //- attr format (scalar, spectrum or image)
    int attr_format = attr_list[_attr_id].data_format;
    //- attr data type (DEV_SHORT, DEV_LONG, ...)
    int attr_type = attr_list[_attr_id].data_type;
    //- extract data from attr value 
    switch (attr_format) 
    {
      case Tango::SCALAR: 
      {
        switch (attr_type) 
        {
          case Tango::DEV_STRING: 
          {
            //create_global_tstamp = 1;
            std::vector<std::string> vstr;
            value_.data >> vstr;
            if (XDK_UTILS->create_df_obj(dfh, obj_name, vstr[0].c_str()) != kNoError) {
              std::string d = "failed to set global string <" + _argout + ">";
              d += " (object name conflict)";
              XDK_UTILS->set_error("invalid argout specified",
                                   d.c_str(),
                                   "IgorDevData::decode_attr");
              return kError;
            }
          } break;
          case Tango::DEV_SHORT: 
          {
            std::vector<short> vs;
            value_.data >> vs;
            err = XDK_UTILS->create_df_obj(dfh, obj_name, vs[0], tstamp, 1);  
            if (err) {
              std::string d = "failed to set global variable <" + _argout + ">";
              d += " (object name conflict)";
              XDK_UTILS->set_error("invalid argout specified",
                                   d.c_str(),
                                   "IgorDevData::decode_attr");
              return kError;
            }
          } break;
          case Tango::DEV_LONG: 
          {
            std::vector<long> vl;
            value_.data >> vl;
            err = XDK_UTILS->create_df_obj(dfh, obj_name, vl[0], tstamp, 1);  
            if (err) {
              std::string d = "failed to set global variable <" + _argout + ">";
              d += " (object name conflict)";
              XDK_UTILS->set_error("invalid argout specified",
                                   d.c_str(),
                                   "IgorDevData::decode_attr");
              return kError;
            }
          } break;
          case Tango::DEV_DOUBLE: 
          {
            std::vector<double> vd;
            value_.data >> vd;
            err = XDK_UTILS->create_df_obj(dfh, obj_name, vd[0], tstamp, 1);
            if (err) {
              std::string d = "failed to set global variable <" + _argout + ">";
              d += " (object name conflict)";
              XDK_UTILS->set_error("invalid argout specified",
                                   d.c_str(),
                                   "IgorDevData::decode_attr");
              return kError;
            }
          } break;
          default:
            XDK_UTILS->set_error("unexpected TANGO data type",
                                 "unsupported TANGO data type for scalar attribute",
                                 "IgorDevData::decode_attr");
            return kError;
            break;
        }
      } break;
      case Tango::SPECTRUM: 
      case Tango::IMAGE: 
      {
        //create_global_tstamp = 1;
        int itype;
        waveHndl num_wave = 0;
        MDWaveDims w_dims;
        ::MemClear(&w_dims, sizeof(MDWaveDims));
        w_dims[0] = value_.dim_x;
        w_dims[1] = (attr_format == Tango::IMAGE) ? value_.dim_y : 0;
        switch (attr_type) 
        {
          case Tango::DEV_SHORT:
            itype = NT_I16;
            break;
          case Tango::DEV_LONG:
            itype = NT_I32;
            break;
          case Tango::DEV_DOUBLE:
            itype = NT_FP64;
            break;
          default: 
          {
            std::string format = (attr_format == Tango::IMAGE) ? "image" : "spectrum";
            std::string d = "unsupported TANGO data type for " + format + " attribute";
            XDK_UTILS->set_error("unexpected TANGO data type",
                                 d.c_str(),
                                 "IgorDevData::decode_attr");
            return kError;
          } break;
        }
        if (XDK_UTILS->fetch_or_make_wave(&num_wave, dfh, obj_name, itype, XDK_Utils::ANY, w_dims)) {
          std::string d = "failed to make or change wave <" + _argout + ">";
          d += " (object name conflict)";
          XDK_UTILS->set_error("invalid argout specified",
                               d.c_str(),
                               "IgorDevData::decode_attr");
          return kError; 
        }
        if (attr_format == Tango::SPECTRUM) {
          value_.data.extract_spectrum(num_wave, value_.dim_x);
        }
        else {
          value_.data.extract_image(num_wave, value_.dim_x, value_.dim_y); 
        }
        ::WaveHandleModified(num_wave);
      } break; 
      default:
        std::string d = "unsupported TANGO data type for attribute <" + attr_list[_attr_id].name + ">";
        XDK_UTILS->set_error("unexpected TANGO data type",
                              d.c_str(),
                             "IgorDevData::decode_attr");
        break;
    }
  } // try
  catch (const Tango::DevFailed &e) {
    XDK_UTILS->set_error(e);
		return kError;
	}
  catch (...) {
    XDK_UTILS->set_error("unknown error",
                         "unknown exception caught",
                         "IgorDevData::decode_attr");
    return kError;
  }

  /*
  if (create_global_tstamp) {
    std::string tstamp_var_name = obj_name + "_ts";
    if (XDK_UTILS->create_df_obj(dfh, tstamp_var_name.c_str(), tstamp) != kNoError) {
      std::string d = "failed to set global variable <" + tstamp_var_name + ">";
      d += " (object name conflict)";
      XDK_UTILS->set_error("XOP internal error",
                           d.c_str(),
                           "IgorDevData::decode_attr");
      return kError;
    }
  }
  */

  return kNoError;
}
