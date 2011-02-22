// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for Igor Pro
//
// = FILENAME
//   DataCodec.h
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

#ifndef _DATA_CODEC_H_
#define _DATA_CODEC_H_

//=============================================================================
// FORWARD DECL
//=============================================================================
class DevDescriptor;

//=============================================================================
// #DEFINEs
//=============================================================================
#define DATA_CODEC (DataCodec::instance())

//=============================================================================
// CLASS : DataCodec 
//=============================================================================
class DataCodec 
{
  friend class TangoBinding;

public:

  static DataCodec* instance ();
  //- Returns the unique instance of <DataCodec>.
  
  int encode_argin (DevDescriptor* ddesc, 
                    const std::string& argin, 
                    int cmd_id,
                    Tango::DeviceData& dd_in);
  //- Converts from Igor Pro types to TANGO-API types

  int decode_argout (DevDescriptor* ddesc, 
                     const std::string& argout, 
                     int cmd_id,
                     Tango::DeviceData& dd_out);
  //- Converts from TANGO-API types  to Igor Pro types

  int encode_attr (DevDescriptor* ddesc, 
                   const std::string& argin, 
                   int attr_id,
                   Tango::DeviceAttribute& value);
  //- Converts from Igor Pro types to TANGO-API types

  int decode_attr (DevDescriptor* ddesc, 
                   const std::string& argout, 
                   int _attr_id,
                   Tango::DeviceAttribute& value,
                   bool _create_ts_obj = true,
                   bool _create_nan_obj = false);
  //- Converts from TANGO-API types to Igor Pro types

private:
  static int init ();
  //- Instanciates the singleton.
  
  static void cleanup ();
  //- Releases the singleton.

  static DataCodec* instance_;
  //- The unique instance of <DataCodec>.

  //- insert methods
  void insert (Tango::DeviceData&, waveHndl wh);
	void insert (Tango::DeviceData&, waveHndl l_value, waveHndl s_value);
  void insert (Tango::DeviceAttribute& da, waveHndl _wh, long _dim_x, long _dim_y = 0);

  //- extract methods
  void extract (const Tango::DeviceData&, waveHndl wh);
	void extract (const Tango::DeviceData&, waveHndl d_value, waveHndl s_value);
  void extract (const Tango::DeviceAttribute& da, waveHndl wh, long dim_x, long dim_y = 0);

  //- export attribute timestamp to Igor
  int export_ts (const std::string& name, DFHndl dfh, double tstamp);

  //- export attribute quality to Igor
  int export_qlt (const std::string& name, DFHndl dfh, Tango::AttrQuality qlt);

  // = Disallow these operations (except for TangoClientXop).
  //---------------------------------------------------------
  DataCodec();
  DataCodec (const DataCodec&);
  virtual ~DataCodec();
  void operator= (const DataCodec&); 
};

#if defined (__XDK_INLINE__)
  #include "DataCodec.i"
#endif 

#endif // _DATA_CODEC_H_

   
   