//==============================================================================
// tango.ipf
//------------------------------------------------------------------------------
// N.Leclercq - SOLEIL
//==============================================================================

//==============================================================================
// USE MODERN GLOBAL ACCESS METHOD
//==============================================================================
#pragma rtGlobals=2

//==============================================================================
// DEPENDENCIES
//==============================================================================
#include "tools"

//==============================================================================
//  CONSTS
//==============================================================================
strconstant kTangoDB = "sys/database/dbds"
//------------------------------------------------------------------------------
constant kLB_FONTSIZE = 12
//------------------------------------------------------------------------------
constant kSCALAR		= 0 	
constant kSPECTRUM	= 1
constant kIMAGE    	= 2	
//------------------------------------------------------------------------------
constant kSTRING = 0x00
constant kCHAR   = 0x08
constant kBOOL   = 0x08
constant kUCHAR  = 0x48		
constant kSHORT  = 0x10	
constant kUSHORT = 0x50	
constant kLONG   = 0x20
constant kULONG  = 0x60
constant kFLOAT  = 0x02
constant kDOUBLE = 0x04
//------------------------------------------------------------------------------
constant kDEVVOID                 = 0
constant kDEVBOOLEAN              = 1
constant kDEVSHORT                = 2
constant kDEVLONG                 = 3
constant kDEVFLOAT                = 4
constant kDEVDOUBLE               = 5
constant kDEVUSHORT               = 6
constant kDEVULONG                = 7
constant kDEVSTRING               = 8
constant kDEVVARCHARARRAY         = 9
constant kDEVVARSHORTARRAY        = 10
constant kDEVVARLONGARRAY         = 11
constant kDEVVARFLOATARRAY        = 12
constant kDEVVARDOUBLEARRAY       = 13
constant kDEVVARUSHORTARRAY       = 14
constant kDEVVARULONGARRAY        = 15
constant kDEVVARSTRINGARRAY       = 16
constant kDEVVARLONGSTRINGARRAY   = 17
constant kDEVVARDOUBLESTRINGARRAY = 18
constant kDEVSTATE                = 19
constant kCONSTDESTRING           = 20
constant kDEVVARBOOLEANARRAY      = 21
constant kDEVUCHAR                = 22
//------------------------------------------------------------------------------
constant kSTR_VAR		= 0x00	
constant kCPL_VAR		= 0x01	
constant kDBL_VAR		= 0x02
constant k1DT_WAV		= 0x03
constant k1DB_WAV		= 0x04
constant k1DW_WAV		= 0x05
constant k1DI_WAV		= 0x06
constant k1DI_1DT_WAV 	= 0x07
constant k1DF_WAV		= 0x08
constant k1DD_WAV		= 0x09
constant k1DD_1DT_WAV 	= 0x10
constant k2DW_WAV		= 0x11
constant k2DI_WAV		= 0x12
constant k2DD_WAV		= 0x13
//------------------------------------------------------------------------------
constant kREAD 				= 0x0
constant kREAD_WITH_WRITE = 0x1
constant kWRITE				= 0x2
constant kREAD_WRITE 		= 0x3
//------------------------------------------------------------------------------
constant kSVAR = 0x00	
constant kNVAR = 0x01
constant kTWAV = 0x02
constant kNWAV = 0x03
//------------------------------------------------------------------------------
strconstant kNAME_SEP = "."
//------------------------------------------------------------------------------
constant kDeviceStateON      = 0
constant kDeviceStateOFF     = 1
constant kDeviceStateCLOSE   = 2
constant kDeviceStateOPEN    = 3
constant kDeviceStateINSERT  = 4 
constant kDeviceStateEXTRACT = 5
constant kDeviceStateMOVING  = 6
constant kDeviceStateSTANDBY = 7
constant kDeviceStateFAULT   = 8
constant kDeviceStateINIT    = 9
constant kDeviceStateRUNNING = 10
constant kDeviceStateALARM   = 11
constant kDeviceStateDISABLE = 12
constant kDeviceStateUNKNOWN = 13
//------------------------------------------------------------------------------
strconstant kWORKING_DF = "root:tango:tmp"
//------------------------------------------------------------------------------
// structure CmdArgIn
//------------------------------------------------------------------------------
Structure CmdArgIn
 //- device name
 String dev 
 //- command name
 String cmd
 //- command argin for any TANGO basic type except DevString  
 Variable var
 //- command argin for DevString 
 String str
 //- path to the argin wave for any TANGO numeric array type or numeric part of DevVar[Long,Double]StringArray
 String num_wave_path 
 //- path to the argin wave for for TANGO DevVarStringArray or DevVar[Long,Double]StringArray
 String str_wave_path  
EndStructure
//------------------------------------------------------------------------------
// structure CmdArgOut 
//------------------------------------------------------------------------------
Structure CmdArgOut
 //- device name
 String dev 
 //- command name
 String cmd
 //- command argout for any TANGO basic type except DevString  
 Variable var
 //- command argout for DevString 
 String str
 //- command argout for any TANGO numeric array type or DevVar[Long,Double]StringArray
 Wave num_wave 
 //- command argout for TANGO DevVarStringArray or numeric part of DevVar[Long,Double]StringArray
 Wave/T str_wave 
EndStructure
//------------------------------------------------------------------------------
// structure AttributeValue : used to to read or write an attribute  
//------------------------------------------------------------------------------
Structure AttributeValue
 //------------------------------------------------------------------------
 //- struct member: dev
 //- desc: device name
 //------------------------------------------------------------------------
 String dev 
 //------------------------------------------------------------------------
 //- struct member: attr
 //- desc: attribute name
 //------------------------------------------------------------------------
 String attr
 //------------------------------------------------------------------------
 //- struct member: format 
 //- desc: attribute data format - 0:kSCALAR, 1:kSPECTRUM or 2:kIMAGE
 //- set when attribute is read (in order to ease data processing)
 //- no need to set this member when writting the attribute
 //------------------------------------------------------------------------
 int16 format 
 //------------------------------------------------------------------------
 //- struct member: type 
 //- desc: attribute data type - 0:kSTRING, ..., 0x04:kDOUBLE (see constants)
 //- set when attribute is read (in order to ease data processing)
 //- no need to set this member when writting the attribute
 //------------------------------------------------------------------------
 int16 type  
 //------------------------------------------------------------------------ 
 //- struct member: ts
 //- desc: timestamp in seconds since Igor's time reference (01/01/1970)
 //------------------------------------------------------------------------
 double ts
 //------------------------------------------------------------------------ 
 //- struct member: str_val
 //- desc: attribute value for string Scalar attributes
 //------------------------------------------------------------------------
 //- valid if format = kSCALAR and type = kSTRING - undefined otherwise 
 //- undefined in case of error during attribute reading - it means that you 
 //- should always check the error code returned by tango_read_attr before 
 //- trying to access this string
 //------------------------------------------------------------------------
 String str_val
 //-------------------------------------------------------------------------
 //- struct member: var_val 
 //- desc: attribute value for Scalar attributes
 //-------------------------------------------------------------------------
 //- valid if format = kSCALAR and type != kSTRING - undefined otherwise 
 //- undefined in case of error during attribute reading - it means that you 
 //- should always check the error code returned by tango_read_attr 
 //- before trying to access this variable
 //------------------------------------------------------------------------
 Variable var_val 
 //-------------------------------------------------------------------------
 //- struct member: wave_val
 //- desc: attribute value for Spectrum or Image attributes
 //------------------------------------------------------------------------
 //- valid for any data type if format is kSPECTRUM or kIMAGE - undefined otherwise 
 //- when used to read an attr: a global wave overwritten each time the attribute is read 
 //- when used to write an attr: not used use wave_val_path to specify where the wave is located
 //- undefined in case of error during attribute reading - it means that you 
 //- should always check the error code returned by tango_read_attr 
 //- before trying to access this variable
 //-------------------------------------------------------------------------
 //- read SCALAR attr: undefined - do not try to use or access - see var_val and str_val 
 //- read SPECTRUM attr: 1D wave of "attr-type" containing only n elements
 //- read IMAGE attr: 2D wave of "attr-type" containing only n * m elements
 //-------------------------------------------------------------------------
 Wave wave_val
 //-------------------------------------------------------------------------
 //- struct member: wave_val_path
 //- desc: full path to the data - fully qualified path (from root:) to the 
 //- datafolder into which the <wave_val> wave is stored (e.g. root:mydf:mywave)
 //-------------------------------------------------------------------------
 String wave_val_path
EndStructure
//------------------------------------------------------------------------------
//- maximum num of attributes than can be read in one call using tango_read_attrs
constant kMAX_NUM_ATTR = 16
//------------------------------------------------------------------------------
Structure AttributeValues
 //- the name of device on which the nattrs attributes should be read
 String dev
 //- actual the num of attributes to read - must be <= kMAX_NUM_ATTR
 //- should obviously equal the num of valid AttributeValue in the values member
 int16 nattrs
 //- an array of kMAX_NUM_ATTR AttributeValue - if you want to be able to read
 //- more than kMAX_NUM_ATTR attributes in one call just increase the value of
 //- the kMAX_NUM_ATTR constant
 STRUCT AttributeValue vals[kMAX_NUM_ATTR]
EndStructure
//------------------------------------------------------------------------------

//==============================================================================
// tango_open_dev
//==============================================================================
function tango_open_dev (dev_name)
  String dev_name
  tango_close_dev(dev_name)
  if (tango_open_device(dev_name) == -1)
		return -1
  endif
  String attrs_df = tango_device_to_df_name(dev_name) + ":attributes"
  tools_kill_df(attrs_df)
  if (tango_get_dev_attr_list(dev_name, "alist") == -1)
		return -1
  endif
  String cmds_df = tango_device_to_df_name(dev_name) + ":commands"
  tools_kill_df(cmds_df)
  if (tango_get_dev_cmd_list(dev_name, "clist") == -1)
		return -1
  endif
  return 0
end
//==============================================================================
// tango_close_dev
//==============================================================================
function tango_close_dev (dev_name)
  String dev_name
  if (tango_close_device(dev_name) == -1)
    tango_display_error()
	 return -1
  endif
  tools_kill_df(tango_device_to_df_name(dev_name))
  return 0
end
//==============================================================================
// tango_cmd
//==============================================================================
function tango_cmd (argin)
  Struct CmdArgIn &argin
  Struct CmdArgOut argout
  return tango_cmd_inout (argin, argout)
end
//==============================================================================
// tango_cmd_in
//==============================================================================
function tango_cmd_in (argin)
  Struct CmdArgIn &argin
  Struct CmdArgOut argout
  return tango_cmd_inout (argin, argout)
end
//==============================================================================
// tango_cmd_out
//==============================================================================
function tango_cmd_out (argout)
  Struct CmdArgOut &argout
  Struct CmdArgIn argin
  argin.dev = argout.dev
  argin.cmd = argout.cmd
  return tango_cmd_inout (argin, argout)
end
//==============================================================================
// tango_cmd_inout
//==============================================================================
function tango_cmd_inout (argin, argout)
  Struct CmdArgIn &argin
  Struct CmdArgOut &argout
  String cur_df = tango_enter_tmp_df(argin.dev)
  Variable argin_type = tango_get_cmd_argin_type(argin.dev, argin.cmd)
  Variable argout_type = tango_get_cmd_argout_type(argin.dev, argin.cmd)
  String argin_str, argout_str
  switch (argin_type)
    case kDEVVOID:
      argin_str = ""
      break
    case kDEVBOOLEAN:
    case kDEVSHORT:
    case kDEVLONG:
    case kDEVFLOAT:
    case kDEVDOUBLE:
    case kDEVUSHORT:
    case kDEVULONG:
    case kDEVUCHAR:
      Variable/G var_in_tmp_ = argin.var
      argin_str = "var_in_tmp_";
      break
    case kDEVSTRING:
      String/G str_in_tmp_ = argin.str
      argin_str = "str_in_tmp_";
      break
    case kDEVVARCHARARRAY:
    case kDEVVARSHORTARRAY:
    case kDEVVARLONGARRAY:
    case kDEVVARFLOATARRAY:
    case kDEVVARDOUBLEARRAY:
    case kDEVVARUSHORTARRAY:
    case kDEVVARULONGARRAY:
      argin_str = argin.num_wave_path
      break
    case kDEVVARSTRINGARRAY:
      argin_str = argin.str_wave_path
      break
    case kDEVVARLONGSTRINGARRAY:
    case kDEVVARDOUBLESTRINGARRAY:
      argin_str = argin.num_wave_path + "." + argin.str_wave_path
      break
    default:
      tango_display_error_str("ERROR:in function tango_cmd_io:unexpected/supported data type for argin")
      tango_leave_tmp_df(cur_df)
      return -1
      break
  endswitch    
  String argout_str_wave_name, argout_num_wave_name
  switch (argout_type)
    case kDEVVOID:
      argout_str = ""
      break
    case kDEVBOOLEAN:
    case kDEVSHORT:
    case kDEVLONG:
    case kDEVFLOAT:
    case kDEVDOUBLE:
    case kDEVUSHORT:
    case kDEVULONG:
    case kDEVUCHAR:
      argout_str = LowerStr(argin.cmd) + "var_out";
      break
    case kDEVSTRING:
      String/G str_tmp_ = argin.str
      argout_str = LowerStr(argin.cmd) + "str_out";
      break
    case kDEVVARCHARARRAY:
    case kDEVVARSHORTARRAY:
    case kDEVVARLONGARRAY:
    case kDEVVARFLOATARRAY:
    case kDEVVARDOUBLEARRAY:
    case kDEVVARUSHORTARRAY:
    case kDEVVARULONGARRAY:
      argout_str = LowerStr(argin.cmd) + "_out";
      argout_num_wave_name = argout_str
      break
    case kDEVVARSTRINGARRAY:
      argout_str = LowerStr(argin.cmd) + "_out";
      argout_str_wave_name = argout_str
      break
    case kDEVVARLONGSTRINGARRAY:
    case kDEVVARDOUBLESTRINGARRAY:
      argout_num_wave_name = LowerStr(argin.cmd) + "_out_nw"
      argout_str_wave_name = LowerStr(argin.cmd) + "_out_sw"
      argout_str = argout_num_wave_name + "." + argout_str_wave_name;
      break
    default:
      tango_display_error_str("ERROR:in function tango_cmd_io:unexpected/supported data type for argout")
      tango_leave_tmp_df(cur_df)
      return -1
      break
  endswitch  
  if (tango_command_inout(argin.dev, argin.cmd, argin_str, argout_str) == -1)
    tango_leave_tmp_df(cur_df)
    return -1
  endif
  argout.dev = argin.dev
  argout.cmd = argin.cmd
  switch (argout_type)
    case kDEVVOID:
      argout.str = ""
      argout.var = NAN
      WAVE num_wave = argout.num_wave
      KillWaves/Z num_wave
      WAVE/T str_wave = argout.str_wave
      KillWaves/Z str_wave
      break
    case kDEVBOOLEAN:
    case kDEVSHORT:
    case kDEVLONG:
    case kDEVFLOAT:
    case kDEVDOUBLE:
    case kDEVUSHORT:
    case kDEVULONG:
    case kDEVUCHAR:
      argout.str = ""
      NVAR var_out_tmp = $argout_str
      argout.var = var_out_tmp
      WAVE num_wave = argout.num_wave
      KillWaves/Z num_wave
      WAVE/T str_wave = argout.str_wave
      KillWaves/Z str_wave
      break
    case kDEVSTRING:
      SVAR str_out_tmp = $argout_str
      argout.str = str_out_tmp
      argout.var = NAN
      WAVE num_wave = argout.num_wave
      KillWaves/Z num_wave
      WAVE/T str_wave = argout.str_wave
      KillWaves/Z str_wave
      break
    case kDEVVARCHARARRAY:
    case kDEVVARSHORTARRAY:
    case kDEVVARLONGARRAY:
    case kDEVVARFLOATARRAY:
    case kDEVVARDOUBLEARRAY:
    case kDEVVARUSHORTARRAY:
    case kDEVVARULONGARRAY:
      argout.str = ""
      argout.var = NAN
      WAVE argout.num_wave = $argout_num_wave_name
      WAVE/T str_wave = argout.str_wave
      KillWaves/Z str_wave
      break
    case kDEVVARSTRINGARRAY:
      argout.str = ""
      argout.var = NAN
      WAVE num_wave = argout.num_wave
      KillWaves/Z num_wave
      WAVE/T argout.str_wave = $argout_str_wave_name
      break
    case kDEVVARLONGSTRINGARRAY:
    case kDEVVARDOUBLESTRINGARRAY:
      argout.str = ""
      argout.var = NAN
      WAVE argout.num_wave = $argout_num_wave_name
      WAVE/T argout.str_wave = $argout_str_wave_name
      break
  endswitch
  tango_leave_tmp_df(cur_df)
  return 0
end
//==============================================================================
// tango_read_attr
//==============================================================================
function tango_read_attr (attr_val_io)
 Struct AttributeValue &attr_val_io
 String cur_df = tango_enter_tmp_df(attr_val_io.dev)
 String result_name = attr_val_io.attr + "_val"
 if (tango_read_attribute(attr_val_io.dev, attr_val_io.attr, result_name) == -1)
   tango_leave_tmp_df(cur_df)
   return -1
 endif
 attr_val_io.type = tango_get_attr_type(attr_val_io.dev, attr_val_io.attr)
 attr_val_io.format = tango_get_attr_format(attr_val_io.dev, attr_val_io.attr)
 switch (attr_val_io.format)
   case kSCALAR:
     if (attr_val_io.type == kSTRING)
       SVAR scalar_txt_result = $result_name
       attr_val_io.str_val = scalar_txt_result
       NVAR ts = timestamp
 	    attr_val_io.ts = ts
 	    attr_val_io.var_val = NAN
     else
       NVAR/C scalar_num_result = $result_name
       attr_val_io.var_val = real(scalar_num_result)
       attr_val_io.ts = imag(scalar_num_result)
       attr_val_io.str_val = ""
     endif
     WAVE wave_val = attr_val_io.wave_val
     KillWaves/Z wave_val
     attr_val_io.wave_val_path = ""
	  break
   case kIMAGE:
   case kSPECTRUM:
     WAVE attr_val_io.wave_val = $result_name 
     NVAR ts = timestamp
 	  attr_val_io.ts = ts
 	  attr_val_io.wave_val_path = GetDataFolder(1) + ":" + result_name
 	  attr_val_io.var_val = NAN
 	  attr_val_io.str_val = ""
     break
 endswitch
 tango_leave_tmp_df(cur_df)
 return 0
end
//==============================================================================
// tango_write_attr
//==============================================================================
function tango_write_attr (attr_val_io)
 Struct AttributeValue &attr_val_io
 String cur_df = tango_enter_tmp_df(attr_val_io.dev)
 Variable type = tango_get_attr_type(attr_val_io.dev, attr_val_io.attr)
 Variable format = tango_get_attr_format(attr_val_io.dev, attr_val_io.attr)
 switch (format)
   case kSCALAR:
 	  switch(type)
 	    case kSTRING:
 		  if (tango_write_attribute(attr_val_io.dev, attr_val_io.attr, attr_val_io.str_val) == -1)
 		    tango_leave_tmp_df(cur_df)
 		    return -1
 		  endif
 		  break
		 default:
		   Variable/G var_tmp_
		   if (tango_write_attribute(attr_val_io.dev, attr_val_io.attr, "var_tmp_") == -1)
		     KillVariables/Z var_tmp_
		     tango_leave_tmp_df(cur_df)
		     return -1
		   endif
		   KillVariables/Z var_tmp_
		   break	
 		endswitch
 		break
  default:
    if (tango_write_attribute(attr_val_io.dev, attr_val_io.attr, attr_val_io.wave_val_path) == -1)
 	   tango_leave_tmp_df(cur_df)
      return -1
 	 endif
 	 break
 endswitch
 tango_leave_tmp_df(cur_df)
 return 0
end
//==============================================================================
// tango_attr_list_to_attr_vals
//==============================================================================
function tango_attr_listw_to_attr_vals (dev_name, attr_list_wave, attr_vals_io)
 String dev_name
 WAVE/T/Z attr_list_wave
 Struct AttributeValues &attr_vals_io
 if (WaveExists(attr_list_wave) == 0 || (WaveType(attr_list_wave) != 0))
   tango_display_error_str("ERROR:in function tango_read_attrs:invalid attribute list specified [valid TEXT wave expected]")
   return -1
 endif 
 Variable n = DimSize(attr_list_wave, 0)
 if (n == 0)
   tango_display_error_str("ERROR:in function tango_read_attrs:empty attribute list specified")
   return -1
 endif 
 attr_vals_io.dev = dev_name
 attr_vals_io.nattrs = n
 Variable i
 for (i = 0; i < n; i += 1)
   attr_vals_io.vals[i].attr = attr_list_wave[i]	
 endfor
 return 0
end
//==============================================================================
// tango_attr_list_to_attr_vals
//==============================================================================
function tango_attr_list_to_attr_vals (dev_name, attr_list_str, attr_vals_io)
 String dev_name
 String attr_list_str
 Struct AttributeValues &attr_vals_io
 Variable i = 0
 String attr_name
 do
  attr_name = StringFromList(i, attr_list_str)
  if (strlen(attr_name) == 0)
    break
  endif
  if (i < kMAX_NUM_ATTR)
    attr_vals_io.vals[i].attr = attr_name 
  endif
  i += 1
 while (1)
 attr_vals_io.dev = dev_name
 attr_vals_io.nattrs = i
 return 0
end
//==============================================================================
// tango_read_attrs
//==============================================================================
function tango_read_attrs (attr_vals_io)
 Struct AttributeValues &attr_vals_io
 String cur_df = tango_enter_tmp_df(attr_vals_io.dev)
 if (attr_vals_io.nattrs <= 0 || attr_vals_io.nattrs >= kMAX_NUM_ATTR)
   tango_display_error_str("ERROR:in function tango_read_attrs:invalid AttributesValues.nattrs specified")
   tango_leave_tmp_df(cur_df)
   return -1
 endif
 String attr_list = "", val_name = ""
 Variable n, attr_type, attr_format;
 for (n = 0; n < attr_vals_io.nattrs; n += 1)
   val_name = attr_vals_io.vals[n].attr + "_val"
   attr_list = tango_attr_val_list_add(attr_list, attr_vals_io.vals[n].attr, val_name)
 endfor
 if (tango_read_attributes(attr_vals_io.dev, attr_list) == -1)
  tango_leave_tmp_df(cur_df)
  return -1
 endif
 for (n = 0; n < attr_vals_io.nattrs; n += 1)
     val_name = attr_vals_io.vals[n].attr + "_val"
 	  attr_vals_io.vals[n].type = tango_get_attr_type(attr_vals_io.dev, attr_vals_io.vals[n].attr)
 	  attr_vals_io.vals[n].format = tango_get_attr_format(attr_vals_io.dev, attr_vals_io.vals[n].attr)
     attr_vals_io.vals[n].dev = attr_vals_io.dev 
     switch (attr_vals_io.vals[n].format)
       case kSCALAR:
         if (attr_vals_io.vals[n].type == kSTRING)
           SVAR scalar_txt_result = $val_name
           attr_vals_io.vals[n].str_val = scalar_txt_result
           NVAR ts = timestamp
 	        attr_vals_io.vals[n].ts = ts
 	        attr_vals_io.vals[n].var_val = NAN
         else
           NVAR/C scalar_num_result = $val_name
           attr_vals_io.vals[n].var_val = real(scalar_num_result)
           attr_vals_io.vals[n].ts = imag(scalar_num_result)
           attr_vals_io.vals[n].str_val = ""
         endif
         WAVE wave_val = attr_vals_io.vals[n].wave_val
         KillWaves/Z wave_val
         attr_vals_io.vals[n].wave_val_path = ""
	      break
       case kIMAGE:
       case kSPECTRUM:
         WAVE attr_vals_io.vals[n].wave_val = $val_name
         attr_vals_io.vals[n].wave_val_path = GetDataFolder(1) + ":" + val_name
         NVAR ts = timestamp
         attr_vals_io.vals[n].ts = ts
         attr_vals_io.vals[n].var_val = NAN
 	      attr_vals_io.vals[n].str_val = ""
         break
     endswitch
 endfor
 tango_leave_tmp_df(cur_df)
 return 0
end
//==============================================================================
// tango_write_attrs
//==============================================================================
function tango_write_attrs (attr_vals_io)
 Struct AttributeValues &attr_vals_io
 String cur_df = tango_enter_tmp_df(attr_vals_io.dev)
 if (attr_vals_io.nattrs <= 0 || attr_vals_io.nattrs >= kMAX_NUM_ATTR)
   tango_display_error_str("ERROR:in function tango_write_attrs:invalid AttributesValues.nattrs specified")
   tango_leave_tmp_df(cur_df)
   return -1
 endif
 String attr_list = "", val_name = ""
 Variable n, attr_type, attr_format;
 for (n = 0; n < attr_vals_io.nattrs; n += 1)
	attr_type = tango_get_attr_type(attr_vals_io.dev, attr_vals_io.vals[n].attr)
	attr_format = tango_get_attr_format(attr_vals_io.dev, attr_vals_io.vals[n].attr)
   switch (attr_format)
     case kSCALAR:
       val_name = attr_vals_io.vals[n].attr + "_val"
 	    switch(attr_type)
 	      case kSTRING:
 	        String/G $val_name 
 	        SVAR str_val = $val_name
 	        str_val = attr_vals_io.vals[n].str_val
 		     break
		   default:
		     Variable/G $val_name 
		     NVAR var_val = $val_name
 	        var_val = attr_vals_io.vals[n].var_val
 		     break
 		 endswitch
 		 attr_list = tango_attr_val_list_add(attr_list, attr_vals_io.vals[n].attr, val_name)
 		 break
     default:
       attr_list = tango_attr_val_list_add(attr_list, attr_vals_io.vals[n].attr, attr_vals_io.vals[n].wave_val_path)
       break
   endswitch
 endfor
 if (tango_write_attributes(attr_vals_io.dev, attr_list) == -1) 
   tango_leave_tmp_df(cur_df)
   return -1
 endif
 tango_leave_tmp_df(cur_df)
 return 0
 end
//==============================================================================
// tango_get_global_obj
//==============================================================================
function/S tango_get_global_obj (obj_name, obj_type)
	String obj_name
	Variable obj_type
	String tango_root = "root:tango:common"
	if (DataFolderExists(tango_root) == 0)
		DoAlert 0, "TANGO root datafolder has been deleted!"
		return ""
	endif
	String obj_full_name = tools_build_path("root:tango:common", obj_name)
	switch (obj_type)
		case kSVAR:
			SVAR/Z str_obj = $obj_full_name
			if (SVAR_Exists(str_obj) == 0)
				String/G $obj_full_name = ""
			endif
			break
		case kNVAR:
			NVAR/Z num_obj = $obj_full_name
			if (NVAR_Exists(num_obj) == 0)
				Variable/G $obj_full_name = 0
			endif
			break
		case kTWAV:
			WAVE/T/Z twav_obj = $obj_full_name
			if (WAVEExists(twav_obj) == 0)
				Make/O/N=0/T $obj_full_name
			endif
			break
		case kNWAV:
			WAVE/Z nwav_obj = $obj_full_name
			if (WAVEExists(nwav_obj) == 0)
				Make/O/N=0 $obj_full_name = 0
			endif
			break
	endswitch
	return obj_full_name
end
//==============================================================================
// fonction : tango_error
//==============================================================================
function tango_error ( )
	NVAR error = root:tango:common:error
	return error
end
//==============================================================================
// fonction : tango_display_error
//==============================================================================
function tango_display_error ( )
	tep_open(0)
end
//==============================================================================
// fonction : tango_show_error (for backward compatibility)
//==============================================================================
function tango_show_error ( )
	tango_display_error() 
end
//==============================================================================
// fonction : tep_open
//==============================================================================
static function tep_open (show_err_stack)
	Variable show_err_stack 
	if (show_err_stack == 0)
		DoWindow tep_modal
		if (V_Flag == 1)
			Button close_but_1,win=tep_modal,title="Next Error"
			Button close_but_2,win=tep_modal,title="Next Error"
			DoWindow/F tep_modal
			PauseForUSer tep_modal
		endif
	else
		Variable error_pending = 0
		ControlInfo/W=tep_modal close_but_1
		if (strsearch(S_Recreation, "Close", 0) == -1)
			error_pending = 1
		endif
	endif
	DoWindow/K tep_modal
	if (show_err_stack == 0)
		tango_get_error_stack()
	endif		
	WAVE/T error_stack = root:tango:common:error_stack
	if (WaveExists(error_stack) == 0 || numpnts(error_stack) == 0)
		return 0
	endif
	SetDimLabel 1, 0, Severity, error_stack
	SetDimLabel 1, 1, Reason, error_stack
	SetDimLabel 1, 2, Description, error_stack
	SetDimLabel 1, 3, Origin, error_stack
	Make /O /N=(dimsize(error_stack,1)) ww
	Variable min_wd = FontSizeStringWidth("Courier New", kLB_FONTSIZE, 0, "Severity")
	tools_get_listbox_colwidths("root:tango:common:error_stack", "ww", min_wd)
	if (show_err_stack)
		NewPanel /K=2 /W=(129,122.75,879.75,530)
	else
		NewPanel /K=2 /W=(129,122.75,879.75,311)
	endif
	DoWindow/C tep_modal
	DoWindow/T tep_modal, "*** TANGO Error Panel ***" 
	ModifyPanel/W=tep_modal fixedSize=1
	GroupBox grp_box,win=tep_modal,pos={17,6},size={716,138}
	GroupBox grp_box,win=tep_modal,title="Error",font="Courier New"
	ListBox err_list,win=tep_modal,pos={18,167},size={715,189},frame=2,disable=1
	ListBox err_list,win=tep_modal,mode=2, font="Courier New",fsize=kLB_FONTSIZE
	ListBox err_list,win=tep_modal,widths={ww[0],ww[1],ww[2],ww[3],ww[4]}
	ListBox err_list,win=tep_modal,listWave=error_stack
	KillWaves/Z ww   
	Variable ctrl_enable = (show_err_stack) ? 0 : 1
	ListBox err_list,win=tep_modal,disable=ctrl_enable
	String ldf = GetDataFolder(1)
	SetDataFolder root:tango:common
	String/G reason = error_stack[0][1]
	SetVariable reason,win=tep_modal,pos={77,30},size={651,18},title="Reason:"
	SetVariable reason,win=tep_modal,font="Courier New",fsize=kLB_FONTSIZE
	SetVariable reason,win=tep_modal,limits={0,0,0},noedit=1,frame=0
	SetVariable reason,win=tep_modal,value=reason
	String/G desc = error_stack[0][2]
	SetVariable desc,win=tep_modal,pos={42,51},size={686,18},title="Description:"
	SetVariable desc,win=tep_modal,font="Courier New",fsize=kLB_FONTSIZE
	SetVariable desc,win=tep_modal,limits={0,0,0},noedit=1,frame=0
	SetVariable desc,win=tep_modal,value=desc
	String/G init_err = error_stack[dimsize(error_stack,0)-1][2]
	SetVariable initial,win=tep_modal,pos={28,72},size={701,18},title="Initial Error:"
	SetVariable initial,win=tep_modal,font="Courier New",fsize=kLB_FONTSIZE
	SetVariable initial,win=tep_modal,limits={0,0,0},noedit=1,frame=0
	SetVariable initial,win=tep_modal,value=init_err
	String/G severity = error_stack[dimsize(error_stack,0)-1][0]
	SetVariable severity,win=tep_modal,pos={63,93},size={666,18},title="Severity:"
	SetVariable severity,win=tep_modal,font="Courier New",fsize=kLB_FONTSIZE
	SetVariable severity,win=tep_modal,limits={0,0,0},noedit=1,frame=0
	SetVariable severity,win=tep_modal,value=severity
	String/G origin = error_stack[dimsize(error_stack,0)-1][3]
	SetVariable origin,win=tep_modal,pos={77,114},size={653,18},title="Origin:"
	SetVariable origin,win=tep_modal,font="Courier New",fsize=kLB_FONTSIZE
	SetVariable origin,win=tep_modal,limits={0,0,0},noedit=1,frame=0
	SetVariable origin,win=tep_modal,value=origin
	String bt = "Close"
	if (error_pending)
		bt = "Next Error"
	endif
	Button close_but_1,win=tep_modal,pos={673,156},size={60,22}
	Button close_but_1,win=tep_modal,proc=tep_close_but_proc,title=bt
	ctrl_enable = (show_err_stack) ? 1 : 0
	Button close_but_1,win=tep_modal,disable=ctrl_enable
	Button close_but_2,win=tep_modal,pos={338,371},size={60,22}, disable=1
	Button close_but_2,win=tep_modal,proc=tep_close_but_proc,title=bt
	ctrl_enable = (show_err_stack) ? 0 : 1
	Button close_but_2,win=tep_modal,disable=ctrl_enable
	Button err_stack_but,win=tep_modal,pos={17,156},size={120,22}
	Button err_stack_but,win=tep_modal,proc=tep_error_stack_but_proc
	Button err_stack_but,win=tep_modal,title="Show Error Stack"
	ctrl_enable = (show_err_stack) ? 1 : 0
	Button err_stack_but,win=tep_modal,disable=ctrl_enable
	SetDataFolder ldf
	PauseForUSer tep_modal
	return 0
end
//==============================================================================
function tep_close_but_proc (ctrlName)
	String ctrlName
	DoWindow/K tep_modal
end
//==============================================================================
function tep_error_stack_but_proc (ctrlName)
	String ctrlName
	tep_open(1)
end
//==============================================================================
// fonction : tango_print_error
//==============================================================================
function tango_print_error ( )
	tango_get_error_stack()
	WAVE/T error_stack = root:tango:common:error_stack
	print "*** TANGO CLIENT ERROR *********************"
	if (WaveExists(error_stack) != 0 && numpnts(error_stack) != 0)
		Variable i
		for (i = 0; i < dimsize(error_stack, 0); i += 1)
			print "\tSEVERITY: " + error_stack[dimsize(error_stack,0)-1][0]
			print "\tREASON: " + error_stack[i][1]
			print "\tDESC: " + error_stack[i][2]
			print "\tINITIAL ERROR: " + error_stack[dimsize(error_stack,0)-1][2]
			print "\tORIGIN: " + error_stack[dimsize(error_stack,0)-1][3]
		endfor
	endif
	print "********************************************"
end
//==============================================================================
// fonction : tango_get_error_str
//==============================================================================
function/S tango_get_error_str ( )
	tango_get_error_stack()
	WAVE/T/Z error_stack = root:tango:common:error_stack
	if (WaveExists(error_stack) != 0 && numpnts(error_stack) != 0)
		WAVE/T error_stack = root:tango:common:error_stack
		SetDimLabel 1, 0, Severity, error_stack
		SetDimLabel 1, 1, Reason, error_stack
		SetDimLabel 1, 2, Description, error_stack
		SetDimLabel 1, 3, Origin, error_stack
		return error_stack[dimsize(error_stack,1)][%Description]
	endif
	return ""
end		
//==============================================================================
// fonction : tango_display_error_str
//==============================================================================
function tango_display_error_str (err_str)
	String err_str
	DoAlert 0, err_str
end
//==============================================================================
// fonction : tango_show_error_str (for backward compatibility)
//==============================================================================
function tango_show_error_str (err_str)
	String err_str
	tango_display_error_str (err_str)
end
//==============================================================================
// fonction : tango_device_to_df_name
//==============================================================================
function/S tango_device_to_df_name (device_name)
	String device_name
	Variable cpos = 0
	Variable lpos = 0
	device_name[0] = "'"
	do 
		cpos = strsearch(device_name, "/", lpos)
		if (cpos == -1) 
			break
		endif
		device_name[cpos] = "'"
		cpos += 1
		device_name[cpos, cpos] = ":"
		cpos += 1
		device_name[cpos] = "'"
		lpos = cpos + 1
	while (1)
	cpos = strlen(device_name)
	device_name[cpos, cpos] = "'"
	return "root:tango:devices:" + device_name
end
//==============================================================================
// fonction : tango_enter_device_df 
//==============================================================================
function tango_enter_device_df (device_name, cur_df)
	String device_name
	String &cur_df
	String device_df = tango_device_to_df_name(device_name)
	cur_df = GetDataFolder(1)
	if (cmpstr(device_df, cur_df) == 0)
		return -1
	endif 
	if (DataFolderExists(device_df) == 0)
		if (tango_open_dev(device_name) == -1)
			tango_display_error()
			return -1
		endif
	endif
	SetDataFolder $device_df
	return 0
end
//==============================================================================
// fonction : tango_enter_working_df 
//==============================================================================
function/S tango_enter_working_df ()
	String df = kWORKING_DF
	String cur_df = GetDataFolder(1)
	if (cmpstr(df, cur_df) == 0)
		return cur_df
	endif 
	if (DataFolderExists(df) == 0)
		NewDataFolder/O/S $df
	else
		SetDataFolder $df
	endif
	return cur_df
end
//==============================================================================
// fonction : tango_leave_tmp_df 
//==============================================================================
function tango_leave_tmp_df  (last_df)
	String last_df
	KillVariables /A /Z
	KillStrings /A /Z 
	if (DataFolderExists(last_df))
		SetDataFolder last_df
	endif
end
//==============================================================================
// fonction : tango_enter_attrs_df 
//==============================================================================
function/S tango_enter_attrs_df  (dev)
	String dev
	String dev_df = tango_device_to_df_name(dev)
	if (DataFolderExists(dev_df) == 0)
	  if (tango_open_dev(dev) == -1)
	    tango_display_error()
	  	 abort
	  endif
	endif
	String attrs_df = dev_df + ":attributes"
	String cur_df = GetDataFolder(1)
	if (cmpstr(attrs_df, cur_df) == 0)
		return cur_df
	endif 
	if (DataFolderExists(attrs_df) == 0)
	  if (tango_open_dev(dev) == -1)
	    tango_display_error()
	  	 abort
	  endif
	else
		SetDataFolder $attrs_df
	endif
	return cur_df
end
//==============================================================================
// fonction : tango_enter_attr_df 
//==============================================================================
function/S tango_enter_attr_df  (dev, attr)
	String dev
	String attr
	String dev_df = tango_device_to_df_name(dev)
	if (DataFolderExists(dev_df) == 0)
	  if (tango_open_dev(dev) == -1)
	    tango_display_error()
	  	 abort
	  endif
	endif
	String attr_df = dev_df + ":attributes:" + attr
	String cur_df = GetDataFolder(1)
	if (cmpstr(attr_df, cur_df) == 0)
		return cur_df
	endif 
	if (DataFolderExists(attr_df) == 0)
	  if (tango_open_dev(dev) == -1)
	    tango_display_error()
	  	 abort
	  endif
	else
		SetDataFolder $attr_df
	endif
	return cur_df
end
//==============================================================================
// fonction : tango_enter_cmds_df 
//==============================================================================
function/S tango_enter_cmds_df (dev)
	String dev
	String dev_df = tango_device_to_df_name(dev)
	if (DataFolderExists(dev_df) == 0)
	  if (tango_open_dev(dev) == -1)
	    tango_display_error()
	  	 abort
	  endif
	endif
	String cmds_df = dev_df + ":commands"
	String cur_df = GetDataFolder(1)
	if (cmpstr(cmds_df, cur_df) == 0)
		return cur_df
	endif 
	if (DataFolderExists(cmds_df) == 0)
	  if (tango_open_dev(dev) == -1)
	    tango_display_error()
	  	 abort
	  endif
	else
		SetDataFolder $cmds_df
	endif
	return cur_df
end
//==============================================================================
// fonction : tango_enter_cmd_df 
//==============================================================================
function/S tango_enter_cmd_df (dev, cmd)
	String dev
	String cmd
	String dev_df = tango_device_to_df_name(dev)
	if (DataFolderExists(dev_df) == 0)
	  if (tango_open_dev(dev) == -1)
	    tango_display_error()
	  	 abort
	  endif
	endif
	String cmd_df = dev_df + ":commands:" + cmd
	String cur_df = GetDataFolder(1)
	if (cmpstr(cmd_df, cur_df) == 0)
		return cur_df
	endif 
	if (DataFolderExists(cmd_df) == 0)
	  if (tango_open_dev(dev) == -1)
	    tango_display_error()
	  	 abort
	  endif
	else
		SetDataFolder $cmd_df
	endif
	return cur_df
end
//==============================================================================
// fonction : tango_enter_tmp_df
//==============================================================================
function/S tango_enter_tmp_df (dev)
	String dev
	String dev_df = tango_device_to_df_name(dev)
	if (DataFolderExists(dev_df) == 0)
	  if (tango_open_dev(dev) == -1)
	    tango_display_error()
	  	 abort
	  endif
	endif
	String tmp_df = dev_df + ":tmp"
	String cur_df = GetDataFolder(1)
	if (cmpstr(tmp_df, cur_df) == 0)
		return cur_df
	endif 
	if (DataFolderExists(tmp_df) == 0)
		NewDataFolder/O/S $tmp_df
	else
		SetDataFolder $tmp_df
	endif
	return cur_df
end
//==============================================================================
// fonction : tango_leave_df 
//==============================================================================
function tango_leave_df (last_df)
	String last_df
	if (DataFolderExists(last_df))
		SetDataFolder last_df
	endif
end
//==============================================================================
// fonction : tango_get_attr_access
//==============================================================================
function tango_get_attr_access (dev, attr)
	String dev
	String attr
	String ldf = tango_enter_attr_df(dev, attr)
	Variable access = NumVarOrDefault(":access", -1)
	tango_leave_df(ldf)
	return access
end
//==============================================================================
// fonction : tango_get_attr_type
//==============================================================================
function tango_get_attr_type (dev, attr)
	String dev
	String attr
	String ldf = tango_enter_attr_df(dev, attr)
	Variable type = NumVarOrDefault(":type", -1)
	tango_leave_df(ldf)
	return type
end
//==============================================================================
// fonction : tango_get_cmd_argin_type
//==============================================================================
function tango_get_cmd_argin_type (dev, cmd)
	String dev
	String cmd
	String ldf = tango_enter_cmd_df(dev, cmd)
	Variable type = NumVarOrDefault(":argin_type", -1)
	tango_leave_df(ldf)
	return type
end
//==============================================================================
// fonction : tango_get_cmd_argout_type
//==============================================================================
function tango_get_cmd_argout_type (dev, cmd)
	String dev
	String cmd
	String ldf = tango_enter_cmd_df(dev, cmd)
	Variable type = NumVarOrDefault(":argout_type", -1)
	tango_leave_df(ldf)
	return type
end
//==============================================================================
// fonction : tango_argin_type_to_wave_type
//==============================================================================
function tango_argin_type_to_wave_type (dev, cmd)
  String dev
  String cmd
  Variable arg_type = tango_get_cmd_argin_type (dev, cmd)
  if (arg_type == -1)
    return -1
  endif
  Variable igor_type
  switch (arg_type)
    case kDEVVOID:
      return -1
      break
    case kDEVUCHAR:
    case kDEVBOOLEAN:
    case kDEVVARCHARARRAY:
      return kUCHAR
      break
    case kDEVSHORT:  
    case kDEVVARSHORTARRAY:
      return kSHORT
      break
    case kDEVFLOAT:
    case kDEVVARFLOATARRAY:
      return kFLOAT
      break
    case kDEVUSHORT:
    case kDEVVARUSHORTARRAY:
      return kUSHORT
      break
    case kDEVULONG: 
    case kDEVVARULONGARRAY:
      return kULONG
      break
    case kDEVSTRING:
    case kDEVVARSTRINGARRAY:
      return kSTRING
      break
    case kDEVLONG:
    case kDEVVARLONGARRAY:
    case kDEVVARLONGSTRINGARRAY:
      return kLONG
      break
    case kDEVDOUBLE:
    case kDEVVARDOUBLEARRAY:
    case kDEVVARDOUBLESTRINGARRAY:
      return kDOUBLE
      break
    default:
      tango_display_error_str("ERROR:in function tango_cmd_io:unexpected/supported data type for argout")
      return -1
      break
  endswitch  
  return -1
end

//==============================================================================
// fonction : tango_get_attr_format
//==============================================================================
function tango_get_attr_format (dev, attr)
	String dev
	String attr
	String ldf = tango_enter_attr_df(dev, attr)
	Variable format = NumVarOrDefault(":format", -1)
	tango_leave_df(ldf)
	return format
end
//==============================================================================
// fonction : tango_attr_val_list_add
//==============================================================================
function/S tango_attr_val_list_add (list, attr, val)
	String list
	String attr
	String val
	String tstmp
	String str_to_add = attr + kNAME_SEP + val + ";"
	Variable found = strsearch(list, str_to_add, 0)
	if (found != -1) 
		return list
	endif
	list += str_to_add
	return list
end
//==============================================================================
// fonction : tango_attr_val_list_remove
//==============================================================================
function/S tango_attr_val_list_remove (list, attr)
	String list
	String attr
	String str_to_remove = attr + kNAME_SEP
	if (strsearch(list, ";", 0) == -1)
	  return list	
	endif
	Variable pos = 0, sep_pos = 0, start = 0, sep_start = 0
	do
	  sep_pos = strsearch(list, ";", sep_start)
	  pos = strsearch(list, attr + kNAME_SEP, start) 
	  if (pos == -1)
	    return list
	  endif	
	  if (pos == 0 || (pos - sep_pos == 1)) 
	   start = pos
	  	break
	  endif
	  sep_start = start
	  start = sep_pos + 1
	while (start < strlen(list))
	Variable stop = strsearch(list, ";", start)
	if (stop == -1)
		stop = strlen(list)
	endif
	String new_list = list[0, start - 1] + list[stop + 1, strlen(list) - 1]
	return new_list
end
//==============================================================================
// fonction : tango_attr_val_list_find
//==============================================================================
function tango_attr_val_list_find (list, attr)
	String list
	String attr
	Variable pos = 0, sep_pos = 0, start = 0
	do
	  sep_pos = strsearch(list, ";", start)
	  pos = strsearch(list, attr + kNAME_SEP, start) 
	  if (pos == -1)
	    return 0
	  endif	
	  if (pos == 0 || (pos - sep_pos == 1)) 
	   return 1
	  endif
	  start = sep_pos + 1
	while (start < strlen(list))
	return 0
end
//==============================================================================
// fonction : tango_get_attr_val_name
//==============================================================================
function/S tango_get_attr_val_name (list, attr)
	String list
	String attr
	String val
	Variable start = strsearch(list, attr + kNAME_SEP, 0)
	if (start == -1) 
		return ""
	endif
	start += strlen(attr + kNAME_SEP)
	Variable stop = strsearch(list, ";", start)
	if (stop == -1)
		stop = strlen(list)
	endif
	return list[start, stop - 1]
end