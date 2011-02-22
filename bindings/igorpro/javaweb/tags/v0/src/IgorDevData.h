// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for Igor Pro
//
// = FILENAME
//   IgorDevData.h
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

#ifndef _IGOR_DEV_DATA_H_
#define _IGOR_DEV_DATA_H_

//=============================================================================
// FORWARD DECL
//=============================================================================
class DevDescriptor;

//=============================================================================
// #DEFINEs
//=============================================================================
#define IDEV_DATA (IgorDevData::instance())

//=============================================================================
// CLASS : IgorDevData 
//=============================================================================
class IgorDevData 
{
  friend class TangoBinding;

public:
  static IgorDevData* instance (void);
  //- Returns the unique instance of <IgorDevData>.
  
  int encode_argin (DevDescriptor* ddesc, 
                    const std::string& argin, 
                    int cmd_id,
                    TangoApi::DevData& dd_in);
  //- Converts <argin> from Igor Pro types to TANGO-API types

  int decode_argout (DevDescriptor* ddesc, 
                     const std::string& argout, 
                     int cmd_id,
                     TangoApi::DevData& dd_out);
  //- Converts <argout> from TANGO-API types  to Igor Pro types

  int encode_attr (DevDescriptor* ddesc, 
                   const std::string& argin, 
                   int attr_id,
                   TangoApi::DevAttrValue& value);
  //- Converts <argin> from Igor Pro types to TANGO-API types

  int decode_attr (DevDescriptor* ddesc, 
                   const std::string& argout, 
                   int _attr_id,
                   TangoApi::DevAttrValue& value);
  //- Converts <argout> from TANGO-API types  to Igor Pro types

private:
  static int init (void);
  //- Instanciates the singleton.
  
  static void cleanup (void);
  //- Releases the singleton.

  static IgorDevData* instance_;
  //- The unique instance of <IgorDevData>.

  // = Disallow these operations (except for TangoClientXop).
  //---------------------------------------------------------
  IgorDevData();
  IgorDevData (const IgorDevData&);
  virtual ~IgorDevData();
  void operator= (const IgorDevData&); 
};

#if defined (__XDK_INLINE__)
  #include "IgorDevData.i"
#endif 

#endif // _IGOR_DEV_DATA_H_

   
   