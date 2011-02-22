// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for Igor Pro
//
// = FILENAME
//   DevClientAPI.cpp
//
// = AUTHOR
//   NL - SOLEIL - March 2002
//
// ============================================================================

// ============================================================================
// DEPENDENCIES
// ============================================================================
#include "XDK_StandardHeaders.h"
#include <time.h>
#include <signal.h>
#ifdef WIN32
# include <sys/timeb.h>
#endif
#include "TangoAPI.h"
       

namespace TangoApi {

// ============================================================================
// STATIC MEMBERS
// ============================================================================
CORBA::ORB_ptr DevProxy::orb = CORBA::ORB::_nil();
//-----------------------------------------------------------------------------

// ============================================================================
// DevProxy::init
// ============================================================================
int DevProxy::init (void)
{
	if (CORBA::is_nil(DevProxy::orb)) {
	  int argc = 1;
	  char **argv = (char**)malloc(sizeof(char*));
    argv[0] = const_cast<char*>("dummy");
    DevProxy::orb = CORBA::ORB_init(argc, argv);
	}
  return CORBA::is_nil(DevProxy::orb) ? -1 : 0;
}

// ============================================================================
// DevProxy::cleanup
// ============================================================================
void DevProxy::cleanup (void)
{
	if (CORBA::is_nil(DevProxy::orb) == false) {
    DevProxy::orb->destroy();
    DevProxy::orb = CORBA::ORB::_nil();
	}
}

// ============================================================================
// DevProxy::DevProxy
// ============================================================================
DevProxy::DevProxy (const std::string& _dname)
  : dev_name_(_dname), tmo_ (3000)
{
	if (CORBA::is_nil(DevProxy::orb)) {
    DevProxy::init();
	}
  this->build_connection();
}

// ============================================================================
// DevProxy::~DevProxy
// ============================================================================
DevProxy::~DevProxy ()
{
  // no-op dtor
}

// ============================================================================
// DevProxy::build_connection
// ============================================================================
void DevProxy::build_connection (void)
{
  if (CORBA::is_nil(DevProxy::orb)) {
    this->connection_state_ = DISCONNECTED;
    TangoSys_OMemStream reason;
    reason << "failed to connect to device " << this->dev_name_ << ends;
    TangoSys_OMemStream desc;
    desc << "internal error (ORB not initialized)" << ends;
    Tango::Except::throw_exception(reason.str(), 
                                   desc.str(), 
                                   (const char*)"DevProxy::build_connection");
  }
  try {
    Tango::DbDevice db_dev(this->dev_name_);
    Tango::DbDevImportInfo import_info = db_dev.import_device();
    const char *ior = import_info.ior.c_str(); 
    CORBA::Object_var obj = DevProxy::orb->string_to_object(ior);
    this->dev_ = Tango::Device::_narrow(obj);
    if (CORBA::is_nil(this->dev_)) {
      TangoSys_OMemStream reason;
      reason << "failed to connect to device " << this->dev_name_ << ends;
      TangoSys_OMemStream desc;
      desc << "internal error (invalid CORBA::Object reference)" << ends;
      Tango::Except::throw_exception(reason.str(), 
                                     desc.str(), 
                                     (const char*)"DevProxy::build_connection");
    }
    this->set_timeout(this->tmo_);
    this->connection_state_ = CONNECTED;
  }
  catch (Tango::DevFailed &dfe) {
    TangoSys_OMemStream reason;
    reason << "failed to connect to device " << this->dev_name_ << ends;
    TangoSys_OMemStream desc;
    desc << "Tango::DevFailed exception caught" << ends;
    Tango::Except::re_throw_exception(dfe, reason.str(), desc.str(), 
                                      (const char*)"DevProxy::build_connection");
  }
  catch (CORBA::SystemException &e) {
    this->connection_state_ = DevProxy::DISCONNECTED;
		try {
			Tango::Except::throw_exception(e, (const char*)"DevProxy::build_connection");
		}
    catch (Tango::DevFailed &dfe) { 
      TangoSys_OMemStream reason;
      reason << "failed to connect to device " << this->dev_name_ << ends;
      TangoSys_OMemStream desc;
      desc << "CORBA system exception caught" << ends;
      Tango::Except::re_throw_exception(dfe, reason.str(), desc.str(), 
                                        (const char*)"DevProxy::build_connection");
    }
	}

}

// ============================================================================
// DevProxy::rebuild_connection
// ============================================================================
void DevProxy::rebuild_connection (void)
{
	if (this->connection_state_ != DevProxy::CONNECTED) {
		this->build_connection();
	}
}

// ============================================================================
// DevProxy::get_timeout
// ============================================================================
int DevProxy::get_timeout (void)
{
	return this->tmo_;
}

// ============================================================================
// DevProxy::set_timeout
// ============================================================================
void DevProxy::set_timeout (int _msecs)
{
  if (CORBA::is_nil(DevProxy::orb)) {
    TangoSys_OMemStream reason;
    this->connection_state_ = DISCONNECTED;
    reason << "internal error (ORB not initialized)" << ends;
    Tango::Except::throw_exception((const char*)"DevProxy failed",
                                   reason.str(), 
                                   (const char*)"DevProxy::set_timeout");
  }

/*
	CORBA::Any any;
	CORBA::ULong ul_timeout;
	ul_timeout = _msecs;
  any <<= ul_timeout;

  CORBA::PolicyList policy;
  policy.length(1);
  policy[0] = DevProxy::orb->create_policy(OB::TIMEOUT_POLICY_ID, any);

  CORBA::Object_var policy_obj;
  policy_obj = this->dev_->_set_policy_overrides(policy,CORBA::ADD_OVERRIDE);
  this->dev_ = Tango::Device::_narrow(policy_obj);
  if (CORBA::is_nil(this->dev_)) {
      TangoSys_OMemStream reason;
      reason << "failed to change to device timeout" << this->dev_name_ << ends;
      TangoSys_OMemStream desc;
      desc << "internal error (invalid CORBA::Object reference)" << ends;
      Tango::Except::throw_exception(reason.str(), 
                                     desc.str(), 
                                     (const char*)"DevProxy::set_timeout");
  }

	this->tmo_ = _msecs;
*/
}

// ============================================================================
// DevProxy::command_inout 
// ============================================================================
TangoApi::DevData DevProxy::command_inout (const string& _cmd)
{
  TangoApi::DevData data_in;
  return command_inout(_cmd.c_str(), data_in);
}

// ============================================================================
// DevProxy::command_inout
// ============================================================================
TangoApi::DevData DevProxy::command_inout (const string& _cmd, 
                                           const TangoApi::DevData& _data_in)
{
  TangoApi::DevData data_out;
  data_out.any_ = this->command_inout(_cmd.c_str(), _data_in.any_);
  return data_out;
}

// ============================================================================
// DevProxy::command_inout
// ============================================================================
CORBA::Any_var DevProxy::command_inout (const string& _cmd, 
                                        const CORBA::Any& _any, 
                                        int _retrying)
{
  CORBA::Any_var any;
	try {
    if (this->connection_state_ != DevProxy::CONNECTED) {
      this->rebuild_connection();
    }
		any = this->dev_->command_inout(_cmd.c_str(), _any);
	}
	catch (Tango::DevFailed &e) {
      TangoSys_OMemStream reason;
      reason << "command_inout failed to execute " << _cmd << " on device " << this->dev_name_ << ends;
      TangoSys_OMemStream desc;
      desc << "Tango::DevFailed exception caught" << ends;
      Tango::Except::re_throw_exception(e, reason.str(), desc.str(), 
                                        (const char*)"DevProxy::command_inout");
	}
  catch (CORBA::SystemException &e) {
    this->connection_state_ = DevProxy::DISCONNECTED;
    if (_retrying == 0) {
      any = this->command_inout(_cmd, _any, 1);
    }
    else {
		  try {
			  Tango::Except::throw_exception(e,(const char*)"DevProxy::command_inout");
		  }
		  catch (Tango::DevFailed &dfe) {
        TangoSys_OMemStream reason;
        reason << "command_inout failed to execute " << _cmd << " on device " << this->dev_name_ << ends;
        TangoSys_OMemStream desc;
        desc << "CORBA system exception caught" << ends;
        Tango::Except::re_throw_exception(dfe, reason.str(), desc.str(), 
                                          (const char*)"DevProxy::command_inout");
		  }
    }
	}
  return any._retn();
}

// ============================================================================
// DevProxy::read_attribute
// ============================================================================
TangoApi::DevAttrValue DevProxy::read_attribute(const std::string& _attr)
{
  Tango::DevVarStringArray names;
  Tango::AttributeValueList_var values;

  names.length(1);
  names[0] = CORBA::string_dup(_attr.c_str());
  
  try {
    if (this->connection_state_ != DevProxy::CONNECTED) {
      this->rebuild_connection();
    }
		values = this->dev_->read_attributes(names);  
  }
  catch (Tango::DevFailed &e) {
    TangoSys_OMemStream reason;
    reason << "failed to read attribute " << _attr << " on device " << this->dev_name_ << ends;
    TangoSys_OMemStream desc;
    desc << "Tango::DevFailed exception caught" << ends;
    Tango::Except::re_throw_exception(e, reason.str(), desc.str(), 
                                      (const char*)"DevProxy::read_attribute");
	}
  catch (CORBA::SystemException &e) {
    this->connection_state_ = DevProxy::DISCONNECTED;
		try {
			Tango::Except::throw_exception(e, (const char*)"DevProxy::read_attribute");
		}
		catch (Tango::DevFailed &dfe) {
      TangoSys_OMemStream reason;
      reason << "failed to read attribute " << _attr << " on device " << this->dev_name_ << ends;
      TangoSys_OMemStream desc;
      desc << "CORBA system exception caught" << ends;
      Tango::Except::re_throw_exception(dfe, reason.str(), desc.str(), 
                                        (const char*)"DevProxy::read_attribute");
		}
	}

  if (values->length() != 1) {
    TangoSys_OMemStream reason;
    reason << "failed to read attribute " << _attr << " on device " << this->dev_name_ << ends;
    TangoSys_OMemStream desc;
    desc << "unexpected data received from device" << ends;
    Tango::Except::throw_exception(reason.str(), 
                                   desc.str(), 
                                   (const char*)"DevProxy::read_attribute");

  }

  TangoApi::DevAttrValue value;
  CORBA::Any* an_any = new CORBA::Any(values[0].value);
  if (an_any == 0) {
    TangoSys_OMemStream reason;
    reason << "failed to read attribute " << _attr << " on device " << this->dev_name_ << ends;
    TangoSys_OMemStream desc;
    desc << "out of memory error" << ends;
    Tango::Except::throw_exception(reason.str(), 
                                   desc.str(), 
                                   (const char*)"DevProxy::read_attribute");
  }
  value.data.any_ = an_any;
  value.quality = values[0].quality;
  value.time = values[0].time;
  value.name = values[0].name;
  value.dim_x = values[0].dim_x;
  value.dim_y = values[0].dim_y;

  return value;
}


// ============================================================================
// DevProxy::read_attributes
// ============================================================================
Tango::AttributeValueList* DevProxy::read_attributes (const Tango::DevVarStringArray& _names)
{
  Tango::AttributeValueList_var values;
  try {
    if (this->connection_state_ != DevProxy::CONNECTED) {
      this->rebuild_connection();
    }
		values = this->dev_->read_attributes(_names);  
  }
  catch (Tango::DevFailed &e) {
    TangoSys_OMemStream reason;
    reason << "failed to read attributes on device " << this->dev_name_ << ends;
    TangoSys_OMemStream desc;
    desc << "Tango::DevFailed exception caught" << ends;
    Tango::Except::re_throw_exception(e, reason.str(), desc.str(), 
                                      (const char*)"DevProxy::read_attributes");
	}
  catch (CORBA::SystemException &e) {
    this->connection_state_ = DevProxy::DISCONNECTED;
		try {
			Tango::Except::throw_exception(e, (const char*)"DevProxy::read_attributes");
		}
		catch (Tango::DevFailed &dfe) {
      TangoSys_OMemStream reason;
      reason << "failed to read attributes on device " << this->dev_name_ << ends;
      TangoSys_OMemStream desc;
      desc << "CORBA system exception caught" << ends;
      Tango::Except::re_throw_exception(dfe, reason.str(), desc.str(), 
                                        (const char*)"DevProxy::read_attributes");
		}
	}

  return values._retn();
}

// ============================================================================
// DevProxy::write_attribute
// ============================================================================
void DevProxy::write_attribute(const TangoApi::DevAttrValue& _attr_val)
{
  try {
    if (this->connection_state_ != DevProxy::CONNECTED) {
      this->rebuild_connection();
    }

    Tango::AttributeValueList values;
    values.length(1);
    values[0].value = _attr_val.data.any_.in();
    values[0].quality = _attr_val.quality;
    values[0].time = _attr_val.time;
    values[0].name = CORBA::string_dup(_attr_val.name.c_str());
    values[0].dim_x = _attr_val.dim_x;
    values[0].dim_y = _attr_val.dim_y;

    this->dev_->write_attributes(values);  
  }
  catch (Tango::DevFailed &e) {
    TangoSys_OMemStream reason;
    reason << "failed to write attribute " << _attr_val.name;
    reason << " on device " << this->dev_name_ << ends;
    TangoSys_OMemStream desc;
    desc << "Tango::DevFailed exception caught" << ends;
    Tango::Except::re_throw_exception(e, reason.str(), desc.str(), 
                                      (const char*)"DevProxy::write_attribute");
	}
  catch (CORBA::SystemException &e) {
    this->connection_state_ = DevProxy::DISCONNECTED;
		try {
			Tango::Except::throw_exception(e, (const char*)"DevProxy::write_attribute");
		}
		catch (Tango::DevFailed &dfe) {
      TangoSys_OMemStream reason;
      reason << "failed to write attribute " << _attr_val.name;
      reason << " on device " << this->dev_name_ << ends;
      TangoSys_OMemStream desc;
      desc << "CORBA system exception caught" << ends;
      Tango::Except::re_throw_exception(dfe, reason.str(), desc.str(), 
                                        (const char*)"DevProxy::write_attribute");
		}
	}
}

// ============================================================================
// DevProxy::write_attributes
// ============================================================================
void DevProxy::write_attributes(const TangoApi::DevAttrValueList& _attr_vals)
{
  try {
    
    if (_attr_vals.size() == 0) {
      return;
    }

    if (this->connection_state_ != DevProxy::CONNECTED) {
      this->rebuild_connection();
    }

    Tango::AttributeValueList values;
    values.length(_attr_vals.size());
    for (int i = 0; i < _attr_vals.size(); i++) {
      values[i].value = _attr_vals[i].data.any_.in();
      values[i].quality = _attr_vals[i].quality;
      values[i].time = _attr_vals[i].time;
      values[i].name = CORBA::string_dup(_attr_vals[i].name.c_str());
      values[i].dim_x = _attr_vals[i].dim_x;
      values[i].dim_y = _attr_vals[i].dim_y;
    }

    this->dev_->write_attributes(values);  
  }
  catch (Tango::DevFailed &e) {
    TangoSys_OMemStream reason;
    reason << "failed to write attributes on device " << this->dev_name_ << ends;
    TangoSys_OMemStream desc;
    desc << "Tango::DevFailed exception caught" << ends;
    Tango::Except::re_throw_exception(e, reason.str(), desc.str(), 
                                      (const char*)"DevProxy::write_attributes");
	}
  catch (CORBA::SystemException &e) {
    this->connection_state_ = DevProxy::DISCONNECTED;
		try {
			Tango::Except::throw_exception(e, (const char*)"DevProxy::write_attributes");
		}
		catch (Tango::DevFailed &dfe) {
      TangoSys_OMemStream reason;
      reason << "failed to write attributes on device " << this->dev_name_ << ends;
      TangoSys_OMemStream desc;
      desc << "CORBA system exception caught" << ends;
      Tango::Except::re_throw_exception(dfe, reason.str(), desc.str(), 
                                        (const char*)"DevProxy::write_attribute");
		}
	}
}

// ============================================================================
// DevProxy::ping 
// ============================================================================
int DevProxy::ping (void)
{
	long elapsed;

#ifndef WIN32
  struct timeval before, after;
  gettimeofday(&before, NULL);
#else
	struct _timeb before, after;
	_ftime(&before);
#endif /* WIN32 */

	try {
    if (this->connection_state_ != DevProxy::CONNECTED) {
      this->rebuild_connection();
    }
		this->dev_->ping();
	}
  catch (Tango::DevFailed &e) {
    TangoSys_OMemStream reason;
    reason << "failed to ping device " << this->dev_name_ << ends;
    TangoSys_OMemStream desc;
    desc << "Tango::DevFailed exception caught" << ends;
    Tango::Except::re_throw_exception(e, reason.str(), desc.str(), 
                                      (const char*)"DevProxy::ping");
	}
  catch (CORBA::SystemException &e) {
    this->connection_state_ = DevProxy::DISCONNECTED;
    try {
			Tango::Except::throw_exception(e,(const char*)"DevProxy::ping");
		}
		catch (Tango::DevFailed &dfe) {
      TangoSys_OMemStream reason;
      reason << "failed to ping device " << this->dev_name_ << ends;
      TangoSys_OMemStream desc;
      desc << "CORBA system exception caught" << ends;
      Tango::Except::re_throw_exception(dfe, reason.str(), desc.str(), 
                                        (const char*)"DevProxy::ping");
		}
	}

#ifndef WIN32
	gettimeofday(&after, NULL);
	elapsed = (after.tv_sec - before.tv_sec) * 1000000;
	elapsed = (after.tv_usec - before.tv_usec) + elapsed;
#else
	_ftime(&after);
	elapsed = (after.time - before.time) * 1000000;
	elapsed = (after.millitm - before.millitm) * 1000 + elapsed;
#endif /* WIN32 */

	return elapsed;
}

// ============================================================================
// DevProxy::command_list_query 
// ============================================================================
void DevProxy::command_list_query (TangoApi::DevCmdInfoList& command_info_list_) 
{
	Tango::DevCmdInfoList_var cmd_info_list;
	try {
    if (this->connection_state_ != DevProxy::CONNECTED) {
      this->rebuild_connection();
    }
		cmd_info_list = this->dev_->command_list_query();
	}
  catch (Tango::DevFailed &e) {
    TangoSys_OMemStream reason;
    reason << "failed to get device " << this->dev_name_ << " command list" << ends;
    TangoSys_OMemStream desc;
    desc << "Tango::DevFailed exception caught" << ends;
    Tango::Except::re_throw_exception(e, reason.str(), desc.str(), 
                                      (const char*)"DevProxy::command_list_query");
	}
  catch (CORBA::SystemException &e) {
    this->connection_state_ = DevProxy::DISCONNECTED;
		try {
			Tango::Except::throw_exception(e, (const char*)"DevProxy::command_list_query");
		}
		catch (Tango::DevFailed &dfe) {
      TangoSys_OMemStream reason;
      reason << "failed to get device " << this->dev_name_ << " command list" << ends;
      TangoSys_OMemStream desc;
      desc << "CORBA system exception caught" << ends;
      Tango::Except::re_throw_exception(dfe, reason.str(), desc.str(), 
                                        (const char*)"DevProxy::command_list_query");
		}
	}

	command_info_list_.resize(cmd_info_list->length());

	for (int i = 0; i < cmd_info_list->length(); i++) {
		command_info_list_[i].cmd_name = cmd_info_list[i].cmd_name;
		command_info_list_[i].cmd_tag = cmd_info_list[i].cmd_tag;
		command_info_list_[i].in_type = cmd_info_list[i].in_type;
		command_info_list_[i].out_type = cmd_info_list[i].out_type;
		command_info_list_[i].in_type_desc = cmd_info_list[i].in_type_desc;
		command_info_list_[i].out_type_desc = cmd_info_list[i].out_type_desc;
	}
}

// ============================================================================
// DevProxy::attribute_list_query - return the list of commands 
// ============================================================================
void DevProxy::attribute_list_query (TangoApi::DevAttrInfoList& attr_info_list_)
{
  Tango::AttributeConfigList_var attr_conf_list;
	try {
    if (this->connection_state_ != DevProxy::CONNECTED) {
      this->rebuild_connection();
    }
    Tango::DevVarStringArray names;
    names.length(1);
    names[0] = CORBA::string_dup(AllAttr);
		attr_conf_list = this->dev_->get_attribute_config(names);
	}
  catch (Tango::DevFailed &e) {
    std::string r(e.errors[0].reason);
    if (r == "API_AttrNotFound") {
      attr_info_list_.resize(0);
      return;
    }
    TangoSys_OMemStream reason;
    reason << "failed to get device " << this->dev_name_ << " attribute list" << ends;
    TangoSys_OMemStream desc;
    desc << "Tango::DevFailed exception caught" << ends;
    Tango::Except::re_throw_exception(e, reason.str(), desc.str(), 
                                      (const char*)"DevProxy::attribute_list_query");
  }
  catch (CORBA::SystemException &e) {
    this->connection_state_ = DevProxy::DISCONNECTED;
		try {
      attr_info_list_.resize(0);
			Tango::Except::throw_exception(e, (const char*)"DevProxy::attribute_list_query");
		}
		catch (Tango::DevFailed &dfe) {
      TangoSys_OMemStream reason;
      reason << "failed to get device " << this->dev_name_ << " attribute list" << ends;
      TangoSys_OMemStream desc;
      desc << "CORBA system exception caught" << ends;
      Tango::Except::re_throw_exception(dfe, reason.str(), desc.str(), 
                                        (const char*)"DevProxy::attribute_list_query");
		}
	}
	attr_info_list_.resize(attr_conf_list->length());
	for (int i = 0; i < attr_conf_list->length(); i++) 
  {
    attr_info_list_[i].name = attr_conf_list[i].name;
    attr_info_list_[i].writable = attr_conf_list[i].writable;
    switch (attr_conf_list[i].writable) 
    {
      case Tango::READ:
        attr_info_list_[i].writable_str = "R";
        break;
      case Tango::READ_WITH_WRITE:
        attr_info_list_[i].writable_str = "RWW";
        break;
      case Tango::WRITE:
        attr_info_list_[i].writable_str = "W";
        break;
      case Tango::READ_WRITE:
        attr_info_list_[i].writable_str = "RW";
        break;
    }
    attr_info_list_[i].data_format = attr_conf_list[i].data_format;
    switch (attr_conf_list[i].data_format) 
    {
      case Tango::SCALAR:
        attr_info_list_[i].data_format_str = "Scalar";
        break;
      case Tango::SPECTRUM:
        attr_info_list_[i].data_format_str = "Spectrum";
        break;
      case Tango::IMAGE:
        attr_info_list_[i].data_format_str = "Image";
        break;
    }
    attr_info_list_[i].data_type = attr_conf_list[i].data_type;
    attr_info_list_[i].data_type_str = Tango::CmdArgTypeName[attr_conf_list[i].data_type];
    attr_info_list_[i].max_dim_x = attr_conf_list[i].max_dim_x;
    attr_info_list_[i].max_dim_y = attr_conf_list[i].max_dim_y;
    attr_info_list_[i].description = attr_conf_list[i].description;
    attr_info_list_[i].label = attr_conf_list[i].label; 
    attr_info_list_[i].unit = attr_conf_list[i].unit;
    attr_info_list_[i].standard_unit = attr_conf_list[i].standard_unit;
    attr_info_list_[i].display_unit = attr_conf_list[i].display_unit; 
    attr_info_list_[i].format = attr_conf_list[i].format;
    attr_info_list_[i].min_value = attr_conf_list[i].min_value; 
    attr_info_list_[i].max_value = attr_conf_list[i].max_value; 
    attr_info_list_[i].min_alarm = attr_conf_list[i].min_alarm;
    attr_info_list_[i].max_alarm = attr_conf_list[i].max_alarm; 
    attr_info_list_[i].writable_attr_name = attr_conf_list[i].writable_attr_name; 
	}
}

// ============================================================================
// DevProxy::black_box 
// ============================================================================
void DevProxy::black_box  (std::vector<std::string>& blackbox_)
{
  Tango::DevVarStringArray_var bb;
	try {
    if (this->connection_state_ != DevProxy::CONNECTED) {
      this->rebuild_connection();
    }
    long n = (blackbox_.size() > 0) ? blackbox_.size() : 30;
		bb = this->dev_->black_box(n);
	}
  catch (Tango::DevFailed &e) {
    std::string r(e.errors[0].reason);
    if (r == "API_BlackBoxEmpty") {
      blackbox_.resize(0);
      return;
    }
    TangoSys_OMemStream reason;
    reason << "failed to get device " << this->dev_name_ << " blackbox" << ends;
    TangoSys_OMemStream desc;
    desc << "Tango::DevFailed exception caught" << ends;
    Tango::Except::re_throw_exception(e, reason.str(), desc.str(), 
                                      (const char*)"DevProxy::black_box");
  }
  catch (CORBA::SystemException &e) {
    this->connection_state_ = DevProxy::DISCONNECTED;
		try {
      blackbox_.resize(0);
			Tango::Except::throw_exception(e, (const char*)"DevProxy::black_box");
		}
		catch (Tango::DevFailed &dfe) {
      TangoSys_OMemStream reason;
      reason << "failed to get device " << this->dev_name_ << " blackbox" << ends;
      TangoSys_OMemStream desc;
      desc << "CORBA system exception caught" << ends;
      Tango::Except::re_throw_exception(dfe, reason.str(), desc.str(), 
                                        (const char*)"DevProxy::black_box");
		}
	}
	blackbox_.resize(bb->length());
	for (int i = 0; i < bb->length(); i++) {
    blackbox_[i] = bb[i];
	}
}

// ============================================================================
// DevProxy::info 
// ============================================================================
TangoApi::DevInfo DevProxy::info (void) 
{
  Tango::DevInfo_var tdi;

  try {
    if (this->connection_state_ != DevProxy::CONNECTED) {
      this->rebuild_connection();
    }
		tdi = this->dev_->info();
	}
  catch (Tango::DevFailed &e) {
    TangoSys_OMemStream reason;
    reason << "failed to get device " << this->dev_name_ << " info" << ends;
    TangoSys_OMemStream desc;
    desc << "Tango::DevFailed exception caught" << ends;
    Tango::Except::re_throw_exception(e, reason.str(), desc.str(), 
                                      (const char*)"DevProxy::info");
  }
  catch (CORBA::SystemException &e) {
    this->connection_state_ = DevProxy::DISCONNECTED;
		try {
			Tango::Except::throw_exception(e, (const char*)"DevProxy::info");
		}
		catch (Tango::DevFailed &dfe) {
      TangoSys_OMemStream reason;
      reason << "failed to get device " << this->dev_name_ << " info" << ends;
      TangoSys_OMemStream desc;
      desc << "CORBA system exception caught" << ends;
      Tango::Except::re_throw_exception(dfe, reason.str(), desc.str(), 
                                        (const char*)"DevProxy::info");
		}
	}

  TangoApi::DevInfo di;
  di.dev_class = tdi->dev_class;
  di.server_id = tdi->server_id;
  di.server_host = tdi->server_host;
  di.server_version = tdi->server_version;
  di.doc_url = tdi->doc_url;

  return di;
}

// ============================================================================
// DevProxy::status 
// ============================================================================
const std::string DevProxy::status (void)
{
  CORBA::String_var status;

  try {
    if (this->connection_state_ != DevProxy::CONNECTED) {
      this->rebuild_connection();
    }
		status = this->dev_->status();
	}
  catch (Tango::DevFailed &e) {
    TangoSys_OMemStream reason;
    reason << "failed to get device " << this->dev_name_ << " status" << ends;
    TangoSys_OMemStream desc;
    desc << "Tango::DevFailed exception caught" << ends;
    Tango::Except::re_throw_exception(e, reason.str(), desc.str(), 
                                      (const char*)"DevProxy::status");
  }
  catch (CORBA::SystemException &e) {
    this->connection_state_ = DevProxy::DISCONNECTED;
		try {
			Tango::Except::throw_exception(e, (const char*)"DevProxy::status");
		}
		catch (Tango::DevFailed &dfe) {
      TangoSys_OMemStream reason;
      reason << "failed to get device " << this->dev_name_ << " status" << ends;
      TangoSys_OMemStream desc;
      desc << "CORBA system exception caught" << ends;
      Tango::Except::re_throw_exception(dfe, reason.str(), desc.str(), 
                                        (const char*)"DevProxy::info");
		}
	}
  return std::string(status);
}

} // namespace TangoApi
