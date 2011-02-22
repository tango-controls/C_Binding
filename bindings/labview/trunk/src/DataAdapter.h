// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for LabVIEW
//
// = FILENAME
//   DataAdapter.h
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

#ifndef _DATA_ADAPTER_H_
#define _DATA_ADAPTER_H_

//=============================================================================
// FORWARD DECL
//=============================================================================
class DeviceDescriptor;

//=============================================================================
// #DEFINEs
//=============================================================================
#define DATA_ADAPTER (DataAdapter::instance())

//=============================================================================
// CLASS : DataAdapter 
//=============================================================================
class DataAdapter 
{
  friend class ObjectManager;

public:

  static DataAdapter* instance (void);
  //- Returns the unique instance of <DataAdapter>.
  
  int encode_argin (DeviceDescriptor* ddesc, 
                    int cmd_id,
                    LvArgIOPtr argin,
                    Tango::DeviceData& dd_in) 
    throw (Tango::DevFailed);
  //- Converts <argin> from Labview type to TANGO type

  int decode_argout (DeviceDescriptor* ddesc, 
                     int cmd_id,
                     Tango::DeviceData& dd_out,
                     LvArgIOPtr argout) 
    throw (Tango::DevFailed);
  //- Converts <dd_out> from TANGO type to Labview type
  
  int encode_attr (DeviceDescriptor* ddesc, 
                   int attr_id,
                   LvAttributeValuePtr attr_value,
                   Tango::DeviceAttribute& attr_in) 
    throw (Tango::DevFailed);
  //- Converts <argin> from Labview type to TANGO type

  int decode_attr (DeviceDescriptor* ddesc, 
                   int attr_id,
                   Tango::DeviceAttribute& attr_out, 
                   void*& argout) 
    throw (Tango::DevFailed);
  //- Low level converter for <attr_value>

  void tango_to_lv_attribute_value (DeviceDescriptor* _ddesc, 
                                    const std::string& attr_name,
                                    Tango::DeviceAttribute& _tav, 
                                    LvAttributeValuePtr lvav_)
    throw (Tango::DevFailed);
  //- High level converter for <attr_value>

private:

  static int init (void);
  //- Instanciates the singleton.
  
  static void cleanup (void);
  //- Releases the singleton.

  static DataAdapter* instance_;
  //- The unique instance of <DataAdapter>.

  // = Disallow these operations (except for ObjectManager).
  //-------------------------------------------------------
  DataAdapter();
  DataAdapter (const DataAdapter&);
  virtual ~DataAdapter();
  void operator= (const DataAdapter&); 
};

//=============================================================================
// INLINED CODE
//=============================================================================
#if defined (_LV_INLINE_)
  #include "DataAdapter.i"
#endif 

#endif // _DATA_ADAPTER_H_

   
   
