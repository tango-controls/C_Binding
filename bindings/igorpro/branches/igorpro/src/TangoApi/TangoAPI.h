// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for Igor Pro
//
// = FILENAME
//   DevClientApi.h 
//   A QUICK AND DIRTY HACK OF THE OFFICIAL TANGO API
//   WILL BE CHANGED IN A NEAR FUTURE (TANGO 2.0)
//
// = AUTHOR
//   ANONYMOUS
//
// ============================================================================

#ifndef _TANGO_DEV_API_H_
#define _TANGO_DEV_API_H_

// ============================================================================
// CONFIGURATION
// ============================================================================

// ============================================================================
// DEPENDENCIES
// ============================================================================
#include <tango.h> 
#include <vector>

namespace TangoApi {

// ============================================================================
// FORWARD DECL
// ============================================================================
class DbDevice;
class DbDatum;

// ============================================================================
// CLASS : DevData 
// ============================================================================
class DevData
{
  friend class DevProxy;

public:
  //- Ctors & Dtor
  DevData ();
  DevData (const DevData&);
  virtual ~DevData ();

  //- operator=
  DevData& operator= (const DevData&); 

  //- Exception flags.
  enum ExFlag {
    off = 0x0,
    on  = 0x1
  };
  void exceptions (DevData::ExFlag flag = on);

  //- insert methods fo basic C++ types
	void operator << (const bool&);
	void operator << (const short&);
	void operator << (const unsigned short&);
	void operator << (const long&);
	void operator << (const unsigned long&);
	void operator << (const float&);
	void operator << (const double&);
	void operator << (char*&);
	void operator << (const char*&);
  void operator << (const std::string&);
  
  //- extract methods fo basic C++ types
	void operator >> (bool&);
	void operator >> (short&);
	void operator >> (unsigned short&);
	void operator >> (long&);
	void operator >> (unsigned long&);
	void operator >> (float&);
	void operator >> (double&);
	void operator >> (const char*&);
  void operator >> (std::string&);

  //- insert methods for STL types
	void operator << (const vector<unsigned char>&);
	void operator << (const vector<string>&);
	void operator << (const vector<short>&);
	void operator << (const vector<unsigned short>&);
	void operator << (const vector<long>&);
	void operator << (const vector<unsigned long>&);
	void operator << (const vector<float>&);
	void operator << (const vector<double>&);
  void operator << (const Tango::DevState&);
	void insert (const vector<long>&, const vector<std::string>&);
	void insert (const vector<double>&, const vector<std::string>&);

  //- extract methods for STL types
	void operator >> (vector<unsigned char>&);
  void operator >> (vector<std::string>&);
	void operator >> (vector<short>&);
	void operator >> (vector<unsigned short>&);
	void operator >> (vector<long>&);
	void operator >> (vector<unsigned long>&);
	void operator >> (vector<float>&);
	void operator >> (vector<double>&);
  void operator >> (Tango::DevState&);
	void extract (vector<long>&, vector<std::string>&);
	void extract (vector<double>&, vector<std::string>&);

#if defined(_XDK_XOP_)
  //- insert methods for Igor Pro waves
  void operator << (waveHndl wh);
	void insert (waveHndl l_value, waveHndl s_value);
  void insert_spectrum (waveHndl _wh, long _dim_x);
  void insert_image (waveHndl _wh, long _dim_x, long _dim_y);

  //- extract methods for Igor Pro waves
  void operator >> (waveHndl wh);
	void extract (waveHndl d_value, waveHndl s_value);
  void extract_spectrum (waveHndl wh, long dim_x);
  void extract_image (waveHndl wh, long dim_x, long dim_y);
#endif // _XDK_XOP_

#if defined(_MATLAB_MEX_)
  //- insert methods for MatLab types
  void operator << (...);
	void insert (/* to do */);
	void insert (/* to do */);

  //- extract methods for MatLab types
	void operator >> (...);
	void extract (/* to do */);
	void extract (/* to do */;
#endif // _MATLAB_MEX_

  CORBA::Any_var any_;
  //- The actual data (should be private).

private:
	bool is_valid() const;
  //- Returns <true> if the underlying <CORBA::any> is NULL, <false> otherwise.

  DevData::ExFlag ex_flag_;
  //- Type of exception that can be throw
};

// ============================================================================
// TYPEDEFs
// ============================================================================
typedef vector<DbDatum> DbData;

typedef struct _DevCmdInfo {
  std::string cmd_name;
	long cmd_tag;
	long in_type;
	long out_type;
  std::string in_type_desc;
  std::string out_type_desc;
} DevCmdInfo;

typedef vector<DevCmdInfo> DevCmdInfoList;

typedef struct _DevInfo {
  std::string dev_class;
  std::string server_id;
  std::string server_host;
  long server_version;
  std::string doc_url;
} DevInfo;
  
typedef struct _DevAttrInfo {
  std::string name;
  Tango::AttrWriteType writable;
  std::string writable_str;
  Tango::AttrDataFormat data_format;
  std::string data_format_str;
  long data_type;
  std::string data_type_str;
  long max_dim_x;
  long max_dim_y;
  std::string description;
  std::string label;
  std::string unit;
  std::string standard_unit;
  std::string display_unit;
  std::string format;
  std::string min_value;
  std::string max_value;
  std::string min_alarm;
  std::string max_alarm;
  std::string writable_attr_name;
} DevAttrInfo;

typedef vector<DevAttrInfo> DevAttrInfoList;

typedef struct _DevAttrValue {
  DevData data;
  Tango::AttrQuality quality;
  Tango::TimeVal time;
  std::string name;
  long dim_x;
  long dim_y;
} DevAttrValue;

typedef vector<DevAttrValue> DevAttrValueList;

// ============================================================================
// CLASS : DevProxy
// ============================================================================
class DevProxy
{
public :
  static int init (void);
  //
  static void cleanup (void);
  //
  DevProxy(const std::string& dev_name);
  // Ctor
	virtual ~DevProxy();
  // Dtor 
  TangoApi::DevInfo info (void);
  //
	const std::string status (void);
  //
	int ping (void);
  //
  void set_timeout(int timeout);
  //
	int get_timeout (void);
  //
  TangoApi::DevData command_inout (const std::string&);
  //
	TangoApi::DevData command_inout (const std::string&, const TangoApi::DevData&);
  //
  TangoApi::DevAttrValue read_attribute (const std::string& attr_name);
  //
  Tango::AttributeValueList* read_attributes (const Tango::DevVarStringArray& attr_names);
  //
  void write_attribute (const TangoApi::DevAttrValue& value);
  //
  void write_attributes (const TangoApi::DevAttrValueList& values);
  //
	void command_list_query (TangoApi::DevCmdInfoList&);
  //
	void attribute_list_query (TangoApi::DevAttrInfoList&);
  //
  void black_box  (std::vector<std::string>& blackbox_);
  //

private:
  enum ConState {
    CONNECTED,
    DISCONNECTED
  };
  void build_connection (void);
  //
	void rebuild_connection(void);
  //
  CORBA::Any_var command_inout (const std::string&, const CORBA::Any&, int retrying = 0);
  //
  Tango::Device_var dev_;
  //
  std::string dev_name_;
  //
	int tmo_;
  //
  int connection_state_;
  //
	static CORBA::ORB_ptr orb;
  //

  // = Disallow these operations
  //----------------------------------------
  DevProxy (const DevProxy&);
  DevProxy& operator= (const DevProxy&); 
};

} // namespace TangoApi.


#endif // _TANGO_DEV_API_H_
