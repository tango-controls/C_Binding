// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for Igor Pro
//
// = FILENAME
//   DevRepository.cpp
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

//=============================================================================
// DEPENDENCIES
//=============================================================================
#include "XDK_StandardHeaders.h"
#include "DevRepository.h"

#if !defined (__XDK_INLINE__)
  #include "DevRepository.i"
#endif 

//=============================================================================
// LOCAL GLOBALS
//=============================================================================
static const char * const lg_CmdArgIgorTypeName[] = 
{ 
	"-",              //- DEV_VOID
	"Variable",       //- DEV_BOOLEAN
	"Variable",       //- DEV_SHORT
	"Variable",       //- DEV_LONG
	"Variable",       //- DEV_FLOAT
	"Variable",       //- DEV_DOUBLE
	"Variable",       //- DEV_USHORT
	"Variable",       //- DEV_ULONG
	"String",         //- DEV_STRING
	"Wave/U/B",       //- DEVVAR_CHARARRAY
	"Wave/W",         //- DEVVAR_SHORTARRAY
	"Wave/I",         //- DEVVAR_LONGARRAY
	"Wave",           //- DEVVAR_FLOATARRAY
	"Wave/D",         //- DEVVAR_DOUBLEARRAY
	"Wave/U/W",       //- DEVVAR_USHORTARRAY
	"Wave/U/I",       //- DEVVAR_ULONGARRAY
	"Wave/T",         //- DEVVAR_STRINGARRAY
	"Wave/I:Wave/T",  //- DEVVAR_LONGSTRINGARRAY
	"Wave/D:Wave/T",  //- DEVVAR_DOUBLESTRINGARRAY
	"Variable",       //- DEV_STATE
	"Unsupported",    //- CONST_DEV_STRING
	"Wave/B",         //- DEVVAR_BOOLEANARRAY
	"Variable",       //- DEV_UCHAR
	"Unsupported",    //- DEV_LONG64
	"Unsupported",    //- DEV_ULONG64
	"Unsupported",    //- DEVVAR_LONG64ARRAY
	"Unsupported",    //- DEVVAR_ULONG64ARRAY
	"Unsupported",    //- DEV_INT
	"Unsupported",    //- DEV_ENCODED
};
//-----------------------------------------------------------------------------
static enum 
{
  kString          = 0x00,
  kBool            = 0x08,
  kChar            = 0x08,
  kUChar           = 0x48,
  kShort           = 0x10,
  kUShort          = 0x50,
  kLong            = 0x20,
  kULong           = 0x60,
  kFloat           = 0x02,
  kDouble          = 0x04,
  kUnsupportedType = 0xFF
} kDataTypes;
//-----------------------------------------------------------------------------
#define kDEFAULT_UNIT       "No unit"
#define kDEFAULT_STD_UNIT   "No standard unit"
#define kDEFAULT_DISP_UNIT  "No display unit"
//-----------------------------------------------------------------------------

//=============================================================================
// STATIC MEMBERS
//=============================================================================
DevRepository* DevRepository::instance_ = 0;

//=============================================================================
// DevDescriptor::DevDescriptor
//=============================================================================
DevDescriptor::DevDescriptor (std::string _dev_name) 
  : dev_name_(_dev_name), 
    dev_alias_(_dev_name), 
    dev_proxy_(0), 
    dev_cmd_list_(0),
    dev_attr_list_(0),
    dev_df_(0)
{
  //- noop
}

//=============================================================================
// DevDescriptor::~DevDescriptor 
//=============================================================================
DevDescriptor::~DevDescriptor ()
{
  if (this->dev_proxy_) 
  {
    delete this->dev_proxy_;
    this->dev_proxy_ = 0;
  }

  if (this->dev_cmd_list_) 
  {
    delete this->dev_cmd_list_;
    this->dev_cmd_list_ = 0;
  }  

  if (this->dev_attr_list_) 
  {
    delete this->dev_attr_list_;
    this->dev_attr_list_ = 0;
  }
}

//=============================================================================
// DevDescriptor::cmd_exists 
//=============================================================================
int DevDescriptor::cmd_exists (const std::string& _cmd_name)
{
  int indx = 0;
  Tango::CommandInfoList::iterator cur = this->dev_cmd_list_->begin();
  Tango::CommandInfoList::iterator end = this->dev_cmd_list_->end();
  while (cur != end) 
  {
    if ((*cur).cmd_name == _cmd_name)
      return indx;
    cur++; indx++;
  }
  return -1;
}

//=============================================================================
// DevDescriptor::update_attr_list_i (THIS MIGHT CAUSE PBS IN THREADED CONTEXT)
//=============================================================================
void DevDescriptor::update_attr_list_i () 
  throw (Tango::DevFailed)
{
  try 
  {
    if (this->dev_attr_list_) 
    {
      delete this->dev_attr_list_;
      this->dev_attr_list_ = 0;
    }

    this->dev_attr_list_ = this->dev_proxy_->unsafe_proxy().attribute_list_query();  
  }
  catch (Tango::DevFailed& e) 
  {
		Tango::Except::re_throw_exception(e,
                                      "attribute_list_query failed",
                                      "failed to get device attributes list (Tango::DevFailed exception caught)",
                                      "DevDescriptor::update_attr_list_i");
  }
  catch (...) 
  {
    Tango::Except::throw_exception("unknown error",
                                   "failed to get device attributes list (unknown exception caught)",
                                   "DevDescriptor::update_attr_list_i");
  }
}

//=============================================================================
// DevDescriptor::update_commands_datafolder
//=============================================================================
int DevDescriptor::update_commands_datafolder (const std::string& _argout)
{
  std::string cmds_df_name = XDK_UTILS->device_to_df_name(this->dev_name_) 
                           + ":commands";

  DFHndl cmds_df = XDK_UTILS->create_df(cmds_df_name);
  if (cmds_df == 0) 
  {
    std::string d = "could not create commands datafolder for device " + this->dev_name_;
    XDK_UTILS->set_error("XOP internal error",
                         d.c_str(),
                         "DevDescriptor::update_commands_datafolder");
    return kError;
  }

  Tango::CommandInfoList & cil = *this->dev_cmd_list_;

  waveHndl w;
  int result = (int)cil.size();

  MDWaveDims dims;
  ::MemClear(dims, sizeof(MDWaveDims));
	dims[0] = (long)cil.size();
	dims[1] = 5;

  if (XDK_UTILS->fetch_or_make_wave(&w, cmds_df, _argout.c_str(), TEXT_WAVE_TYPE, XDK_Utils::ANY, dims)) 
  {
    std::string d = "failed to make or change wave " + _argout;
    d += " (object name conflict)";
    XDK_UTILS->set_error("invalid argout specified",
                         d.c_str(),
                         "DevDescriptor::update_commands_datafolder");
    return kError; 
  }

  const std::string undefined_str("--");
  Handle h_tmp = 0;
  DFHndl cmd_df;
  std::string cmd_df_name;
  size_t name_too_long_counter = 0;

	for (size_t i = 0; i < cil.size(); i++)
	{
    if (cil[i].cmd_name.size() <= MAX_OBJ_NAME)
    {
      cmd_df_name = cmds_df_name + ":" + cil[i].cmd_name;
    }
    else
    {
      TangoSys_OMemStream actual_cmd_df_name;
      actual_cmd_df_name << "error_cmd_name_too_long_"
                         << ++name_too_long_counter;
      cmd_df_name = cmds_df_name + ":" + actual_cmd_df_name.str();
    }
    cmd_df = XDK_UTILS->create_df(cmd_df_name);
    if (cmd_df == 0) 
    {
      std::string d = "could not create command datafolder for device " + this->dev_name_;
      XDK_UTILS->set_error("XOP internal error",
                           d.c_str(),
                           "DevDescriptor::update_commands_datafolder");
      return kError;
    }
    do 
    {
      if(XDK_UTILS->create_df_obj(cmd_df, "name", cil[i].cmd_name)) 
      {
        result = kError; 
        break;
      }
      if(XDK_UTILS->create_df_obj(cmd_df, "argin_type", cil[i].in_type)) 
      {
        result = kError; 
        break;
      }
      if(XDK_UTILS->create_df_obj(cmd_df, "argout_type", cil[i].out_type)) 
      {
        result = kError;
        break;
      }
    } while(0);

    if (result == kError) 
    {
      XDK_UTILS->set_error("XOP internal error",
                           "set_df_obj failed",
                           "DevDescriptor::update_commands_datafolder");
      break; 
    }

		dims[0] = i;

    do 
    {
		  dims[1] = 0;
      if (XDK_UTILS->str_to_handle(cil[i].cmd_name, h_tmp)) 
      {
        result = kError; break;
      }
      ::MDSetTextWavePointValue(w, dims, h_tmp);
      ::DisposeHandle(h_tmp);

      dims[1] = 1;
      if (XDK_UTILS->str_to_handle(lg_CmdArgIgorTypeName[cil[i].in_type], h_tmp)) 
      {
        result = kError; break;
      }
      ::MDSetTextWavePointValue(w, dims, h_tmp);
      ::DisposeHandle(h_tmp);

      dims[1] = 2;
      std::string in_type_desc = cil[i].in_type_desc;
      if (! in_type_desc.size()) 
        in_type_desc = undefined_str;
      if (XDK_UTILS->str_to_handle(in_type_desc, h_tmp)) 
      {
        result = kError; break;
      }
      ::MDSetTextWavePointValue(w, dims, h_tmp);
      ::DisposeHandle(h_tmp);

      dims[1] = 3;
      if (XDK_UTILS->str_to_handle(lg_CmdArgIgorTypeName[cil[i].out_type], h_tmp)) 
      {
        result = kError; break;
      }
      ::MDSetTextWavePointValue(w, dims, h_tmp);
      ::DisposeHandle(h_tmp);

      dims[1] = 4;
      std::string out_type_desc = cil[i].out_type_desc;
      if (! out_type_desc.size()) 
        out_type_desc = undefined_str;
      if (XDK_UTILS->str_to_handle(out_type_desc, h_tmp)) 
      {
        result = kError; break;
      }
      ::MDSetTextWavePointValue(w, dims, h_tmp);
      ::DisposeHandle(h_tmp);
    } while (0);

    if (result == kError) 
    {
      XDK_UTILS->set_error("API_MemoryAllocation",
                           "str_to_handle failed",
                           "DevDescriptor::update_commands_datafolder");
      break; 
    }
	}

  return result;
}

//=============================================================================
// DevDescriptor::update_attributes_datafolder
//=============================================================================
int DevDescriptor::update_attributes_datafolder (const std::string& _argout)
{
  std::string attrs_df_name = XDK_UTILS->device_to_df_name(this->dev_name_) 
                            + ":attributes";

  DFHndl attrs_df = XDK_UTILS->create_df(attrs_df_name);
  if (attrs_df == 0) 
  {
    std::string d = "could not create attributes datafolder for device " + this->dev_name_;
    XDK_UTILS->set_error("XOP internal error",
                         d.c_str(),
                         "DevDescriptor::update_attributes_datafolder");
    return kError;
  }

  Tango::AttributeInfoList & ail = *this->dev_attr_list_;
  if (ail.size() == 0) 
  {
    return kNoError;
  }
  int result = (int)ail.size();

  waveHndl w;
  MDWaveDims dims;
  ::MemClear(dims, sizeof(MDWaveDims));
	dims[0] = (long)ail.size();
	dims[1] = 17;

  if (XDK_UTILS->fetch_or_make_wave(&w, attrs_df, _argout.c_str(), TEXT_WAVE_TYPE, XDK_Utils::ANY, dims)) 
  {
    std::string d = "failed to make or change wave " + _argout;
    d += " (object name conflict)";
    XDK_UTILS->set_error("invalid argout specified",
                         d.c_str(),
                         "DevDescriptor::update_attributes_datafolder");
    return kError; 
  }

  Handle h_tmp = 0;
  TangoSys_OMemStream o;
  DFHndl attr_df;
  std::string attr_df_name;
  size_t name_too_long_counter = 0;
  std::string attr_alias("--");
	for (size_t i = 0; i < ail.size(); i++)
	{
    //- export attribute info to Igor 
    if (ail[i].name.size() <= MAX_OBJ_NAME)
      attr_df_name = attrs_df_name + ":" + ail[i].name;
    else
    {
      TangoSys_OMemStream actual_attr_df_name;
      actual_attr_df_name << "error_attr_name_too_long_"
                          << ++name_too_long_counter;
      attr_df_name = attrs_df_name + ":" + actual_attr_df_name.str();
    }

    attr_df = XDK_UTILS->create_df(attr_df_name);
    if (attr_df == 0) 
    {
      std::string d = "could not create attribute datafolder for device " + this->dev_name_;
      XDK_UTILS->set_error("XOP internal error",
                            d.c_str(),
                            "DevDescriptor::update_attributes_datafolder");
      return kError;
    }
    do 
    {
      if(XDK_UTILS->create_df_obj(attr_df, "name", ail[i].name)) 
      {
        result = kError; 
        break;
      }
      if(XDK_UTILS->create_df_obj(attr_df, "access", (long)ail[i].writable)) 
      {
        result = kError; 
        break;
      }
      if(XDK_UTILS->create_df_obj(attr_df, "format", (long)ail[i].data_format)) 
      {
        result = kError; 
        break;
      }
      if(XDK_UTILS->create_df_obj(attr_df, "max_dim_x", (long)ail[i].max_dim_x))
      {
        result = kError; 
        break;
      }
      if(XDK_UTILS->create_df_obj(attr_df, "max_dim_y", (long)ail[i].max_dim_y)) 
      {
        result = kError; 
        break;
      }
      if(XDK_UTILS->create_df_obj(attr_df, "display_label", ail[i].label.c_str())) 
      {
        result = kError; 
        break;
      }
      if (ail[i].unit == kDEFAULT_UNIT)
      {
        ail[i].unit = "";
      }
      if(XDK_UTILS->create_df_obj(attr_df, "unit", ail[i].unit.c_str())) 
      {
        result = kError; 
        break;
      }
      if (ail[i].standard_unit == kDEFAULT_STD_UNIT)
      {
        ail[i].standard_unit = "";
      }
      if(XDK_UTILS->create_df_obj(attr_df, "standard_unit", ail[i].standard_unit.c_str())) 
      {
        result = kError; 
        break;
      }
      if (ail[i].display_unit == kDEFAULT_DISP_UNIT)
      {
        ail[i].display_unit = "";
      }
      if(XDK_UTILS->create_df_obj(attr_df, "display_unit", ail[i].display_unit.c_str())) 
      {
        result = kError; 
        break;
      }
      if(XDK_UTILS->create_df_obj(attr_df, "display_format", ail[i].format.c_str())) 
      {
        result = kError; 
        break;
      }
      if(XDK_UTILS->create_df_obj(attr_df, "min_value", ail[i].min_value.c_str())) 
      {
        result = kError; 
        break;
      }
      if(XDK_UTILS->create_df_obj(attr_df, "max_value", ail[i].max_value.c_str())) 
      {
        result = kError; 
        break;
      }
      if(XDK_UTILS->create_df_obj(attr_df, "min_alarm", ail[i].min_alarm.c_str())) 
      {
        result = kError; 
        break;
      }
      if(XDK_UTILS->create_df_obj(attr_df, "max_alarm", ail[i].max_alarm.c_str())) 
      {
        result = kError; 
        break;
      }
      if(XDK_UTILS->create_df_obj(attr_df, "alias", attr_alias.c_str())) 
      {
        result = kError; 
        break;
      }
    } while(0);
    if (result == kError) 
    {
      XDK_UTILS->set_error("XOP internal error",
                           "set_df_obj failed",
                           "DevDescriptor::update_attributes_datafolder");
      break; 
    }
		dims[0] = i;
    long itype;
    std::string itype_str = "-";
    do 
    {
		  dims[1] = 0;
      if (XDK_UTILS->str_to_handle(ail[i].name, h_tmp)) 
      {
        result = kError; 
        break;
      }
      ::MDSetTextWavePointValue(w, dims, h_tmp);
      ::DisposeHandle(h_tmp);

      dims[1] = 1;
      std::string str;
      switch (ail[i].writable) 
      {
        case Tango::READ:
          str = "R";
          break;
        case Tango::READ_WITH_WRITE:
          str = "RWW";
          break;
        case Tango::WRITE:
          str = "W";
          break;
        case Tango::READ_WRITE:
          str = "RW";
          break;
      }
      if (XDK_UTILS->str_to_handle(str, h_tmp)) 
      {
        result = kError; 
        break;
      }
      ::MDSetTextWavePointValue(w, dims, h_tmp);
      ::DisposeHandle(h_tmp);
      dims[1] = 2;
      switch (ail[i].data_format) 
      {
        case Tango::SCALAR:
          str = "Scalar";
          break;
        case Tango::SPECTRUM:
          str = "Spectrum";
          break;
        case Tango::IMAGE:
          str = "Image";
          break;
        default:
          str = "??????";
          break; 
      }
      if (XDK_UTILS->str_to_handle(str, h_tmp)) 
      {
        result = kError; 
        break;
      }
      ::MDSetTextWavePointValue(w, dims, h_tmp);
      ::DisposeHandle(h_tmp);
      dims[1] = 3;
      std::string itype_str = "-";
      switch(ail[i].data_type) 
      {
        //-------------------------------------------------
        case Tango::DEV_STATE:
          itype = kDouble;
					itype_str = "Variable";
					break;	
        //-------------------------------------------------
        case Tango::DEV_STRING:
          itype = kString;
					itype_str = "String";
					break;	
        //-------------------------------------------------
        case Tango::DEV_BOOLEAN:
          itype = kBool;
          switch(ail[i].data_format) 
          { 
            case Tango::SCALAR:
							itype_str = "Variable";
							break;
            case Tango::SPECTRUM:
							itype_str = "Wave/N=(n)/B";
							break;
						case Tango::IMAGE:
							itype_str = "Wave/N=(n,m)/B";
							break;
            default:
							itype_str = "??????";
							break; 
          }
					break;
        //-------------------------------------------------
        case Tango::DEV_UCHAR:
          itype = kUChar;
          switch(ail[i].data_format) 
          { 
            case Tango::SCALAR:
              itype_str = "Variable";
							break;
            case Tango::SPECTRUM:
							itype_str = "Wave/N=(n)/U/B";
							break;
						case Tango::IMAGE:
							itype_str = "Wave/N=(n,m)/U/B";
							break;
            default:
							itype_str = "??????";
							break; 
          }
					break;
        //-------------------------------------------------
        case Tango::DEV_USHORT:
          itype = kUShort;
          switch(ail[i].data_format) 
          { 
            case Tango::SCALAR:
							itype_str = "Variable";
							break;
            case Tango::SPECTRUM:
							itype_str = "Wave/N=(n)/U/W";
							break;
						case Tango::IMAGE:
							itype_str = "Wave/N=(n,m)/U/W";
							break;
            default:
							itype_str = "??????";
							break; 
          }
					break;
        //-------------------------------------------------
        case Tango::DEV_SHORT:
          itype = kShort; 
          switch(ail[i].data_format) 
          { 
            case Tango::SCALAR:
							itype_str = "Variable";
							break;
            case Tango::SPECTRUM:
							itype_str = "Wave/N=(n)/W";
							break;
						case Tango::IMAGE:
							itype_str = "Wave/N=(n,m)/W";
							break;
            default:
							itype_str = "??????";
							break; 
          }
					break;
        //-------------------------------------------------
        case Tango::DEV_ULONG:
          itype = kLong;
          switch(ail[i].data_format) 
          {
						case Tango::SCALAR:
							itype_str = "Variable";
							break;
            case Tango::SPECTRUM:
							itype_str = "Wave/N=(n)/U/I";
							break;
						case Tango::IMAGE:
							itype_str = "Wave/N=(n,m)/U/I";
							break;
            default:
							itype_str = "??????";
							break; 
          }
					break;
        //-------------------------------------------------
        case Tango::DEV_LONG:
          itype = kLong;
          switch(ail[i].data_format) 
          {
						case Tango::SCALAR:
							itype_str = "Variable";
							break;
            case Tango::SPECTRUM:
							itype_str = "Wave/N=(n)/I";
							break;
						case Tango::IMAGE:
							itype_str = "Wave/N=(n,m)/I";
							break;
            default:
							itype_str = "??????";
							break; 
          }
					break;
        //-------------------------------------------------
        case Tango::DEV_FLOAT:
          itype = kFloat;
          switch(ail[i].data_format) 
          {
						case Tango::SCALAR:
							itype_str = "Variable";
							break;
            case Tango::SPECTRUM:
							itype_str = "Wave/N=(n)";
							break;
						case Tango::IMAGE:
							itype_str = "Wave/N=(n,m)";
							break;
            default:
							itype_str = "??????";
							break; 
          }
					break;
        //-------------------------------------------------
        case Tango::DEV_DOUBLE:
          itype = kDouble;
          switch(ail[i].data_format) 
          {
						case Tango::SCALAR:
							itype_str = "Variable";
							break;
            case Tango::SPECTRUM:
							itype_str = "Wave/N=(n)/D";
							break;
						case Tango::IMAGE:
							itype_str = "Wave/N=(n,m)/D";
							break;
            default:
							itype_str = "??????";
							break; 
          }
					break;
        //-------------------------------------------------
        default:
          itype = kUnsupportedType;
          itype_str = "Unsupported";
					break;
      }
      if (XDK_UTILS->str_to_handle(itype_str, h_tmp)) 
      {
        result = kError; 
        break;
      }
      if(XDK_UTILS->create_df_obj(attr_df, "type", itype)) 
      {
        result = kError; 
        break;
      }
      ::MDSetTextWavePointValue(w, dims, h_tmp);
      ::DisposeHandle(h_tmp);
      dims[1] = 4;
      o << ail[i].max_dim_x << ends;
      if (XDK_UTILS->str_to_handle(o.str(), h_tmp)) 
      {
        result = kError; 
        break;
      }
      ::MDSetTextWavePointValue(w, dims, h_tmp);
      ::DisposeHandle(h_tmp);
      dims[1] = 5;
      o << ail[i].max_dim_y << ends;
      if (XDK_UTILS->str_to_handle(o.str(), h_tmp)) 
      {
        result = kError; 
        break;
      }
      ::MDSetTextWavePointValue(w, dims, h_tmp);
      ::DisposeHandle(h_tmp);
      dims[1] = 6;
      if (XDK_UTILS->str_to_handle(ail[i].label, h_tmp)) 
      {
        result = kError; 
        break;
      }
      ::MDSetTextWavePointValue(w, dims, h_tmp);
      ::DisposeHandle(h_tmp);
      dims[1] = 7;
      if (XDK_UTILS->str_to_handle(ail[i].unit, h_tmp)) 
      {
        result = kError; 
        break;
      }
      ::MDSetTextWavePointValue(w, dims, h_tmp);
      ::DisposeHandle(h_tmp);
      dims[1] = 8;
      if (XDK_UTILS->str_to_handle(ail[i].standard_unit, h_tmp)) 
      {
        result = kError; 
        break;
      }
      ::MDSetTextWavePointValue(w, dims, h_tmp);
      ::DisposeHandle(h_tmp);
      dims[1] = 9;
      if (XDK_UTILS->str_to_handle(ail[i].display_unit, h_tmp)) 
      {
        result = kError; 
        break;
      }
      ::MDSetTextWavePointValue(w, dims, h_tmp);
      ::DisposeHandle(h_tmp);
      dims[1] = 10;
      if (XDK_UTILS->str_to_handle(ail[i].format, h_tmp)) 
      {
        result = kError; 
        break;
      }
      ::MDSetTextWavePointValue(w, dims, h_tmp);
      ::DisposeHandle(h_tmp);
      dims[1] = 11;
      if (XDK_UTILS->str_to_handle(ail[i].min_value, h_tmp)) 
      {
        result = kError; 
        break;
      }
      ::MDSetTextWavePointValue(w, dims, h_tmp);
      ::DisposeHandle(h_tmp);
      dims[1] = 12;
      if (XDK_UTILS->str_to_handle(ail[i].max_value, h_tmp)) 
      {
        result = kError; 
        break;
      }
      ::MDSetTextWavePointValue(w, dims, h_tmp);
      ::DisposeHandle(h_tmp);
      dims[1] = 13;
      if (XDK_UTILS->str_to_handle(ail[i].min_alarm, h_tmp)) 
      {
        result = kError; 
        break;
      }
      ::MDSetTextWavePointValue(w, dims, h_tmp);
      ::DisposeHandle(h_tmp);
      dims[1] = 14;
      if (XDK_UTILS->str_to_handle(ail[i].max_alarm, h_tmp)) 
      {
        result = kError; 
        break;
      }
      ::MDSetTextWavePointValue(w, dims, h_tmp);
      ::DisposeHandle(h_tmp);

      dims[1] = 16;
      if (XDK_UTILS->str_to_handle(ail[i].writable_attr_name, h_tmp)) 
      {
        result = kError; 
        break;
      }
      ::MDSetTextWavePointValue(w, dims, h_tmp);
      ::DisposeHandle(h_tmp);

      dims[1] = 15;
      if (XDK_UTILS->str_to_handle(ail[i].description, h_tmp)) 
      {
        result = kError; 
        break;
      }
      ::MDSetTextWavePointValue(w, dims, h_tmp);
      ::DisposeHandle(h_tmp);
    } while (0);

    if (result == kError) 
    {
      XDK_UTILS->set_error("API_MemoryAllocation",
                           "str_to_handle failed",
                           "DevDescriptor::update_attributes_datafolder");
      break; 
    }
	}

  return result;
}

//=============================================================================
// DevDescriptor::attr_exists 
//=============================================================================
int DevDescriptor::attr_exists (const std::string& _attr_name)
{
  bool refresh_attrs_list = false;

#if defined (HAS_DYN_ATTRS_SUPPORT)
  yat::AutoMutex<> guard(this->lock);
  refresh_attrs_list = true;
#endif

  return DevDescriptor::attr_exists_i(_attr_name, refresh_attrs_list);
}

//=============================================================================
// DevDescriptor::attr_exists_i
//=============================================================================
int DevDescriptor::attr_exists_i (const std::string& _attr_name, bool _refresh_local_list)
{
  //- search
  int indx = 0;
  if (! this->dev_attr_list_->empty())
  {
    //- there is at least one attr in the list
    Tango::AttributeInfoList::iterator cur = this->dev_attr_list_->begin();
    Tango::AttributeInfoList::iterator end = this->dev_attr_list_->end();
    while (cur != end) 
    {
      if ((*cur).name == _attr_name)
      {
        //- found...
        return indx;
      }
      cur++; 
      indx++;
    }
  }
  else
  {
#if defined (HAS_DYN_ATTRS_SUPPORT)
    //- empty attr list (try to update)
    _refresh_local_list = true;
#endif
  }

  //- not found
  if (! _refresh_local_list)
    return -1;

  //- refresh attrs list...
  try
  {
    this->update_attr_list_i();
  }
  catch (Tango::DevFailed&)
  {
    //- not found (due to list update error)
    return -1;
  }

  //- ... then retry (but no attrs list update this time)
  return this->attr_exists_i(_attr_name, false);
}

//=============================================================================
// DevDescriptor::is_attr_writable
//=============================================================================
bool DevDescriptor::is_attr_writable (const std::string& _attr_name)
{ 
  return this->is_attr_writable(this->attr_exists(_attr_name));
}

//=============================================================================
// DevDescriptor::is_attr_writable
//=============================================================================
bool DevDescriptor::is_attr_writable(int _attr_id)
{
#if defined (HAS_DYN_ATTRS_SUPPORT)
  yat::AutoMutex<> guard(this->lock);
#endif

  if (_attr_id < 0 || (size_t)_attr_id >= this->dev_attr_list_->size())
    return false;

  switch ((*this->dev_attr_list_)[_attr_id].writable) 
  {
    case Tango::READ:
    case Tango::READ_WITH_WRITE:
      return false;
      break;
    default:
      break;
  }
  return true;
}

//=============================================================================
// DevDescriptor::is_attr_readable
//=============================================================================
bool DevDescriptor::is_attr_readable (const std::string& _attr_name)
{ 
  return this->is_attr_writable(this->attr_exists(_attr_name));
}

//=============================================================================
// DevDescriptor::is_attr_readable
//=============================================================================
bool DevDescriptor::is_attr_readable (int _attr_id)
{
#if defined (HAS_DYN_ATTRS_SUPPORT)
  yat::AutoMutex<> guard(this->lock);
#endif

  if (_attr_id < 0 || (size_t)_attr_id >= this->dev_attr_list_->size())  
    return false;

  switch ((*this->dev_attr_list_)[_attr_id].writable) 
  {
    case Tango::WRITE:
      return false;
      break;
    default:
      break;
  }
  return true; 
}

//=============================================================================
// DevRepository::init
//=============================================================================
int DevRepository::init ()
{
  if (DevRepository::instance_ != 0)
    return 0;

  DevRepository::instance_ = new DevRepository;

  return (DevRepository::instance_) ? 0 : -1;
}

//=============================================================================
// DevRepository::cleanup
//=============================================================================
void DevRepository::cleanup ()
{
  if (DevRepository::instance_) 
  {
    delete DevRepository::instance_;
    DevRepository::instance_ = 0;
  }
}

//=============================================================================
// DevRepository::DevRepository 
//=============================================================================
DevRepository::DevRepository ()
{

}

//=============================================================================
// DevRepository::~DevRepository 
//=============================================================================
DevRepository::~DevRepository ()
{
  this->remove_device("*");

  DevRepository::instance_ = 0;
}

//=============================================================================
// DevRepository::device_desc
//=============================================================================
DevDescriptor* DevRepository::device_desc (const std::string& _device_name, bool _connect)
{
  //- enter critical section
  yat::AutoMutex<yat::Mutex> guard(this->lock_);

  //- search for <device_name> in the repository
  DeviceDescMapIterator it = this->dev_map_.find(_device_name);

  //- found ?
  if (it != this->dev_map_.end() && it->second) 
    return it->second;

  //- make connection?
  if (! _connect)
    return 0;

  //- search failed, create the descriptor
  DevDescriptor* ddesc = new DevDescriptor(_device_name);
  if (ddesc == 0) 
  {
    XDK_UTILS->set_error("API_MemoryAllocation",
                         "DevDescriptor allocation failed",
                         "DevRepository::device_desc");
    return 0;
  }
  try 
  {
    //-- instanciate the device proxy
    ddesc->dev_proxy_ = 
        new yat4tango::ThreadSafeDeviceProxy(const_cast<std::string&>(_device_name));

    if (ddesc->dev_proxy_ == 0)
    {
      delete ddesc;
      XDK_UTILS->set_error("API_MemoryAllocation",
                           "Tango::DeviceProxy allocation failed",
                           "DevRepository::device_desc");
      return 0;
    }
    //- enable transparent reconnection
    ddesc->dev_proxy_->unsafe_proxy().set_transparency_reconnection(true);
	}
	catch (const Tango::DevFailed &e) 
  {
		delete ddesc;
    XDK_UTILS->set_error(e);
    std::string d = "failed to create proxy for device " + _device_name;
    XDK_UTILS->push_error("TangoDevProxy instanciation failed",
                          d.c_str(),
                          "DevRepository::device_desc");
		return 0;
	}
  catch (...) 
  {
    delete ddesc;
    XDK_UTILS->set_error("unknown error",
                         "unknown exception caught",
                         "DevRepository::device_desc");
    return 0;
  }
  //- get device commands list & dev info
  try 
  {
    ddesc->dev_cmd_list_ = ddesc->dev_proxy_->unsafe_proxy().command_list_query();
    ddesc->dev_info_ = ddesc->dev_proxy_->unsafe_proxy().info();
  }
  catch (const Tango::DevFailed &e) 
  {
		delete ddesc;
    XDK_UTILS->set_error(e);
    XDK_UTILS->push_error("command_list_query failed",
                          "failed to get device commands list (Tango::DevFailed exception caught)",
                          "DevRepository::device_desc");
		return 0;
  }
  catch (...) 
  {
    delete ddesc;
    XDK_UTILS->set_error("unknown error",
                         "unknown exception caught",
                         "DevRepository::device_desc");
    return 0;
  }
  //- get device attributes list 
  try 
  {
    ddesc->update_attr_list_i();  
  }
  catch (const Tango::DevFailed &e) 
  {
    delete ddesc;
    XDK_UTILS->set_error(e);
    XDK_UTILS->push_error("attribute_list_query failed",
                          "failed to get device attributes list (Tango::DevFailed exception caught)",
                          "DevRepository::device_desc");
		return 0;
  }
  catch (...) 
  {
    delete ddesc;
    XDK_UTILS->set_error("unknown error",
                         "failed to get device attributes list (unknown exception caught)",
                         "DevRepository::device_desc");
    return 0;
  }
  //- get device alias
  try
  {
    Tango::Database * db = ddesc->dev_proxy_->unsafe_proxy().get_device_db();
    db->get_alias(_device_name, ddesc->dev_alias_);
  }
  catch (const Tango::DevFailed&) 
  {
    ddesc->dev_alias_ = "--";
  }
  catch (...)
  {
    ddesc->dev_alias_ = "--";
  }
  //- create device datafolder
  std::string dev_df_name = XDK_UTILS->device_to_df_name(_device_name);
  ddesc->dev_df_ = XDK_UTILS->create_df(dev_df_name);
  if (ddesc->dev_df_ == 0) 
  {
    delete ddesc;
    std::string d = "could not create data folder for device " + _device_name;
    XDK_UTILS->set_error("XOP internal error",
                         d.c_str(),
                         "DevRepository::device_desc");
    return 0;
  }
  //- create dev info df & vars
  DFHndl idf = XDK_UTILS->create_df(dev_df_name + ":info");
  if (idf == 0) 
  {
    delete ddesc;
    std::string d = "could not create <info> datafolder for device " + _device_name;
    XDK_UTILS->set_error("XOP internal error",
                         d.c_str(),
                         "DevRepository::device_desc");
    return 0;
  }
  int err = kNoError;
  do {
    if (XDK_UTILS->create_df_obj(idf, "alias", ddesc->dev_alias_))
      {err = kError; break;}
    if (XDK_UTILS->create_df_obj(idf, "class", ddesc->dev_info_.dev_class))
      {err = kError; break;}
    if (XDK_UTILS->create_df_obj(idf, "server", ddesc->dev_info_.server_id))
      {err = kError; break;}
    if (XDK_UTILS->create_df_obj(idf, "host", ddesc->dev_info_.server_host))
      {err = kError; break;}
    if (XDK_UTILS->create_df_obj(idf, "version", ddesc->dev_info_.server_version))
      {err = kError; break;}
    if (XDK_UTILS->create_df_obj(idf, "doc_url", ddesc->dev_info_.doc_url))
      {err = kError; break;}
  } while (0);

  if (err != kNoError) 
  {
    delete ddesc;
    std::string d = "could not create <info> variables for device " + _device_name;
    XDK_UTILS->set_error("XOP internal error",
                         d.c_str(),
                         "DevRepository::device_desc");
    return 0;
  }

  //- create/populate <commands> datafolder
  if (ddesc->update_commands_datafolder("clist") == kError)
  {
    delete ddesc;
    std::string d = "could not create <commands> datafolder for device " + _device_name;
    XDK_UTILS->set_error("XOP internal error",
                         d.c_str(),
                         "DevRepository::device_desc");
    return 0;
  } 

  //- create/populate <attributes> datafolder
  if (ddesc->update_attributes_datafolder("alist") == kError)
  {
    delete ddesc;
    std::string d = "could not create <attributes> datafolder for device " + _device_name;
    XDK_UTILS->set_error("XOP internal error",
                         d.c_str(),
                         "DevRepository::device_desc");
    return 0;
  } 

  //- store the device descriptor into the repository
  std::pair<DeviceDescMapIterator, bool> result;
  result = this->dev_map_.insert(DeviceDescMap::value_type(_device_name, ddesc));
  if (result.second == 0) 
  {
    delete ddesc;
    XDK_UTILS->set_error("XOP internal error",
                         "DeviceMap::insert failed",
                         "DevRepository::device_desc");
    return 0;
  }
#if defined (_XDK_DEBUG_)
  XDK_UTILS->notify("Tango-Binding::device %s added to repository", _device_name.c_str());  
#endif
  //- return the device descriptor
  return ddesc;
}

//=============================================================================
// DevRepository::remove_device
//=============================================================================
int DevRepository::remove_device (const std::string& _device_name)
{
  //- enter critical section
  yat::AutoMutex<yat::Mutex> guard(this->lock_);

  if (_device_name == "*") 
  {
    DeviceDescMapIterator cur = this->dev_map_.begin();
    DeviceDescMapIterator end = this->dev_map_.end();
    for (; cur != end; cur++) 
    {
      if (cur->second) 
        delete cur->second;
    } 
    this->dev_map_.clear();
#if defined (_XDK_DEBUG_)
    XDK_UTILS->notify("Tango-Binding::devices repository cleared");  
#endif
  }
  else 
  {
    DeviceDescMapIterator it = this->dev_map_.find(_device_name);
    if (it != this->dev_map_.end()) 
    {
      if (it->second) delete it->second;
      this->dev_map_.erase(_device_name);
#if defined (_XDK_DEBUG_)
    XDK_UTILS->notify("Tango-Binding::device %s removed from repository", _device_name.c_str());  
#endif
    }
  }
  return kNoError;
}
