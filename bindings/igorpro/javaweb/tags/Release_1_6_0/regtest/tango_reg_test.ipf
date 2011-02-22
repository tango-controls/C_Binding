//==============================================================================
// DO NOT USE - SEE TUTORIAL.IPF  - DO NOT USE - SEE TUTORIAL.IPF - DO NOT USE 
//==============================================================================
// DO NOT USE - SEE TUTORIAL.IPF  - DO NOT USE - SEE TUTORIAL.IPF - DO NOT USE 
//==============================================================================
// DO NOT USE - SEE TUTORIAL.IPF  - DO NOT USE - SEE TUTORIAL.IPF - DO NOT USE 
//==============================================================================
// DO NOT USE - SEE TUTORIAL.IPF  - DO NOT USE - SEE TUTORIAL.IPF - DO NOT USE 
//==============================================================================
// DO NOT USE - SEE TUTORIAL.IPF  - DO NOT USE - SEE TUTORIAL.IPF - DO NOT USE 
//==============================================================================
// DO NOT USE - SEE TUTORIAL.IPF  - DO NOT USE - SEE TUTORIAL.IPF - DO NOT USE 
//==============================================================================
// DO NOT USE - SEE TUTORIAL.IPF  - DO NOT USE - SEE TUTORIAL.IPF - DO NOT USE 
//==============================================================================
// DO NOT USE - SEE TUTORIAL.IPF  - DO NOT USE - SEE TUTORIAL.IPF - DO NOT USE 
//==============================================================================
// DO NOT USE - SEE TUTORIAL.IPF  - DO NOT USE - SEE TUTORIAL.IPF - DO NOT USE 
//==============================================================================
// DO NOT USE - SEE TUTORIAL.IPF  - DO NOT USE - SEE TUTORIAL.IPF - DO NOT USE 
//==============================================================================
// DO NOT USE - SEE TUTORIAL.IPF  - DO NOT USE - SEE TUTORIAL.IPF - DO NOT USE 
//==============================================================================
// DO NOT USE - SEE TUTORIAL.IPF  - DO NOT USE - SEE TUTORIAL.IPF - DO NOT USE 
//==============================================================================
// USE MODERN GLOBAL ACCESS METHOD
//==============================================================================
#pragma rtGlobals=1 

//==============================================================================
// DEPENDENCIES
//==============================================================================
#include "tango"

//==============================================================================
// fonction :   reg_test
//==============================================================================
function reg_test (dev_inst_name)
  String dev_inst_name
  Variable it
  tango_test_I(dev_inst_name,1, 1)
  tango_test_II(dev_inst_name, 1, 2048)
  return 0
end

//==============================================================================
// fonction :   stress
//==============================================================================
function stress (dev_inst_name, it)
  String dev_inst_name
  Variable it
  Variable i = 0, err = 0
  Variable timer = StartMSTimer
  if (tango_test_I(dev_inst_name, 1, 1) == -1) 
    err = -1
  else
    for (i = 0; i < it;  i += 1) 
      if (tango_test_II(dev_inst_name, 1, 2048) == -1) 
        err = -1
        break
      endif
    endfor
  endif
  Variable elapsed_ms = StopMSTimer(timer) * 1.E-3
  Variable msec_per_it = elapsed_ms / it
  if (err == 0)
    printf "*** TANGO test passed ****\r"
    printf "Test took : %.2f s : %.2f ms/iteration\r", elapsed_ms * 1.E-3, msec_per_it
  else
    return -1
  endif
  return 0
end

//==============================================================================
// fonction :   tango_test_I
//------------------------------------------------------------------------------
// Execute tango_test <it> times. If <it> is large enough, we can evaluate both
// XOP stability and memory leaks using Windows XP proc. manager (Ctrl-Alt-Del) 
//==============================================================================
function tango_test_I (device, it, check_result)
  String device
  Variable it
  Variable check_result
  Variable i = 0, err = 0
  Variable timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_test(device, check_result) == -1)
      err = -1
      break
    endif
  endfor
  Variable elapsed_ms = StopMSTimer(timer) * 1.E-3
  Variable msec_per_it = elapsed_ms / it
  if (err == 0)
    printf "*** TANGO test passed ****\r"
  else
    printf "*** TANGO test failed ****\r"
    return -1
  endif
  printf "Test took : %.2f s : %.2f ms/iteration\r", elapsed_ms * 1.E-3, msec_per_it
  return 0
end

//==============================================================================
// fonction :   tango_test 
//------------------------------------------------------------------------------
// Executes each command of the <tester> device and reads each attribute
// If <check_result> is true (i.e. not null) the result is evaluated. For a 
// command argout is compared to argin (since each <tester> command returns 
// a copy of its input arg, argin and argout must be equal). For an attribute,
// we just verify that the returned value exists.
//==============================================================================
function tango_test (device, check_result)
  String device
  Variable check_result
  Variable i = 0
  String ldf = GetDataFolder(1)
  NewDataFolder/O/S root:tango_test_I
  //------------------------------------------------------
  //- PING THE DEVICE (IS THERE SOMEONE OUT THERE!)
  //------------------------------------------------------
  if (tango_ping_device(device) == -1)
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif 
  //------------------------------------------------------
  //- PRINT DEVIVE STATUS
  //------------------------------------------------------
  print device + " status: " + tango_get_dev_status(device)
  //------------------------------------------------------
  //- EXEC DEV_VOID
  //------------------------------------------------------
  if (tango_command_inout(device, "DevVoid", "", "") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- EXEC DEV_BOOLEAN
  //------------------------------------------------------
  Variable/G argin = 1
  if (tango_command_inout(device, "DevBoolean", "argin", "argout") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  NVAR/C argout
  if (check_result && (argin != argout))
    tango_display_error_str("DevBoolean error : argout != argin")
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- EXEC DEV_SHORT
  //------------------------------------------------------
  argin = trunc(enoise(256))
  if (tango_command_inout(device, "DevShort", "argin", "argout") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  if (check_result && (argin != argout))
    tango_display_error_str("DevShort error : argout != argin")
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- EXEC DEV_USHORT
  //------------------------------------------------------
  argin = trunc(abs(enoise(256)))
  if (tango_command_inout(device, "DevUShort", "argin", "argout") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  if (check_result && (argin != argout))
    tango_display_error_str("DevUShort error : argout != argin")
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- EXEC DEV_LONG
  //------------------------------------------------------
  argin = trunc(enoise(256))
  if (tango_command_inout(device, "DevLong", "argin", "argout") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  if (check_result && (argin != argout))
    tango_display_error_str("DevLong error : argout != argin")
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- EXEC DEV_ULONG
  //------------------------------------------------------
  argin = trunc(abs(enoise(256)))
  if (tango_command_inout(device, "DevULong", "argin", "argout") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  if (check_result && (argin != argout))
    tango_display_error_str("DevULong error : argout != argin")
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- EXEC DEV_FLOAT
  //------------------------------------------------------
  argin = enoise(256)
  if (tango_command_inout(device, "DevFloat", "argin", "argout") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  if (check_result && (argin != argout))
    //Due to the conversion from double to float argin 
    //can't be strictly equal to argout. 
  endif
  //------------------------------------------------------
  //- EXEC DEV_DOUBLE
  //------------------------------------------------------
  argin = enoise(256)
  if (tango_command_inout(device, "DevDouble", "argin", "argout") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  if (check_result && (argin != argout))
    tango_display_error_str("DevDouble error : argout != argin")
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- EXEC DEV_STRING
  //------------------------------------------------------
  //- METHOD I
  String/G argin_str_ = "abcdefghijklmnopqrstuvwxyz123456789"
  if (tango_command_inout(device, "DevString", argin_str_, "argout_str_") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  SVAR/Z argout_str_
  if (SVAR_Exists(argout_str_) == 0)
    tango_display_error_str("DevString error : no answer found")
    SetDataFolder ldf
    return -1
  endif
  if (check_result && cmpstr(argout_str_, argin_str_) != 0)
    tango_display_error_str("DevString error : argout != argin")
    print "argin: " + argin_str_
    print "argout: " + argout_str_
    SetDataFolder ldf
    return -1
  endif
  //- METHOD II
  argout_str_ = "" // reset argout_str_
  if (tango_command_inout(device, "DevString", "argin_str_", "argout_str_") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  SVAR/Z argout_str_
  if (SVAR_Exists(argout_str_) == 0)
    tango_display_error_str("DevString error : no answer found")
    SetDataFolder ldf
    return -1
  endif
  if (check_result && (cmpstr(argout_str_, argin_str_) != 0))
    tango_display_error_str("DevString error : argout != argin")
    print "argin: " + argin_str_
    print "argout: " + argout_str_
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- EXEC DEVVAR_CHARARRAY  (SHOULD BE DEVVAR_UCHARARRAY!)
  //------------------------------------------------------
  Make /O /N=1024 /Y=(0x08|0x40) argin_wav  // a wave of <unsigned char> 
  argin_wav = trunc(enoise(1024))
  if (tango_command_inout(device, "DevVarCharArray", "argin_wav", "argout_wav") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  WAVE/Z argout_wav
  if (WAVEExists(argout_wav) == 0) 
    tango_display_error_str("DevVarCharArray error : no answer found")
    SetDataFolder ldf
    return -1
  endif
  if (check_result && (argin_wav[x] != argout_wav[x]))
    tango_display_error_str("DevVarCharArray error : argout != argin")
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- EXEC DEVVAR_USHORTARRAY 
  //------------------------------------------------------
  Make /O /N=1024 /Y=(0x10|0x40) argin_wav  // a wave of <unsigned short> 
  argin_wav = trunc(abs(enoise(1024)))
  if (tango_command_inout(device, "DevVarUShortArray", "argin_wav", "argout_wav") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  if (check_result && (argin_wav[x] != argout_wav[x]))
    tango_display_error_str("DevVarUShortArray error : argout != argin")
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- EXEC DEVVAR_SHORTARRAY 
  //------------------------------------------------------
  Make /O /N=1024 /Y=(0x10) argin_wav  // a wave of <short> 
  argin_wav = trunc(enoise(1024))
  if (tango_command_inout(device, "DevVarShortArray", "argin_wav", "argout_wav") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  if (check_result && (argin_wav[x] != argout_wav[x]))
    tango_display_error_str("DevVarShortArray error : argout != argin")
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- EXEC DEVVAR_ULONGARRAY 
  //------------------------------------------------------
  Make /O /N=2048 /Y=(0x20|0x40) argin_wav  // a wave of <unsigned long> 
  argin_wav = trunc(abs(enoise(1024)))
  if (tango_command_inout(device, "DevVarULongArray", "argin_wav", "argout_wav") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  if (check_result && (argin_wav[x] != argout_wav[x]))
    tango_display_error_str("DevVarULongArray error : argout != argin")
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- EXEC DEVVAR_LONGARRAY 
  //------------------------------------------------------
  Make /O /N=1024 /Y=(0x20) argin_wav  // a wave of <long> 
  argin_wav = trunc(enoise(1024))
  if (tango_command_inout(device, "DevVarLongArray", "argin_wav", "argout_wav") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  if (check_result && (argin_wav[x] != argout_wav[x]))
    tango_display_error_str("DevVarLongArray error : argout != argin")
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- EXEC DEVVAR_FLOATARRAY 
  //------------------------------------------------------
  Make /O /N=2048 /Y=(0x2) argin_wav  // a wave of <float> 
  argin_wav = enoise(1024)
  if (tango_command_inout(device, "DevVarFloatArray", "argin_wav", "argout_wav") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  if (check_result && (argin_wav[x] != argout_wav[x]))
    tango_display_error_str("DevVarFloatArray error : argout != argin")
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- EXEC DEVVAR_DOUBLEARRAY 
  //------------------------------------------------------
  Make /O /N=1024 /Y=(0x4) argin_wav  // a wave of <double> 
  argin_wav = enoise(1024)
  if (tango_command_inout(device, "DevVarDoubleArray", "argin_wav", "argout_wav") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  if (check_result && (argin_wav[x] != argout_wav[x]))
    tango_display_error_str("DevVarDoubleArray error : argout != argin")
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- EXEC DEVVAR_STRINGARRAY 
  //------------------------------------------------------
  Make /O /N=1024 /T in_swav // a wave of <string>
  in_swav = "VarStringArray-Test"  
  if (tango_command_inout(device, "DevVarStringArray", "in_swav", "out_swav") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  WAVE/T out_swav
  if (check_result && (cmpstr(in_swav[x],out_swav[x]) != 0))
    tango_display_error_str("DevVarStringArray error : argout != argin")
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- EXEC DEVVAR_LONGSTRINGARRAY 
  //------------------------------------------------------
  Make /O /N=1024 /Y=(0x20) in_nwav   // a wave of <long> 
  in_nwav = trunc(enoise(1024))  
  Make /O /N=1024 /T in_swav // a wave of <string>
  in_swav = "VarLongStringArray-Test"  
  KillWaves/Z out_nwav, out_swav
  String cmd = "DevVarLongStringArray"
  String argin_str  = "in_nwav" + kNAME_SEP + "in_swav"
  String argout_str = "out_nwav" + kNAME_SEP + "out_swav"
  if (tango_command_inout(device, cmd, argin_str, argout_str) == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  WAVE/Z out_nwav
  if (check_result && (WaveExists(out_nwav) == 0 || (in_nwav[x] != out_nwav[x])))
    tango_display_error_str("DevVarLongStringArray error : argout != argin (long data)")
    SetDataFolder ldf
    return -1
  endif
  WAVE/T/Z out_swav
  if (check_result && (WaveExists(out_swav) == 0 || (cmpstr(in_swav[x], out_swav[x]) != 0)))
    tango_display_error_str("DevVarLongStringArray error : argout != argin (string data)")
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- EXEC DEVVAR_DOUBLESTRINGARRAY 
  //------------------------------------------------------
  Make /O /N=1024 /Y=(0x4) in_nwav    // a wave of <double> 
  in_nwav = enoise(1024)
  Make /O /N=1024 /T in_swav // a wave of <string>
  in_swav = "VarDoubleStringArray-Test"  
  cmd = "DevVarDoubleStringArray"
  if (tango_command_inout(device, cmd, "in_nwav" + kNAME_SEP + "in_swav", "out_nwav" + kNAME_SEP + "out_swav") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  if (check_result && (in_nwav[x] != out_nwav[x]))
    tango_display_error_str("DevVarDoubleStringArray error : argout != argin (double data)")
    SetDataFolder ldf
    return -1
  endif
  if (check_result && (cmpstr(in_swav[x], out_swav[x]) != 0))
    tango_display_error_str("DevVarDoubleStringArray error : argout != argin (string data)")
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- READ ATTRIBUTE SCALAR STRING
  //------------------------------------------------------
  if (tango_read_attribute(device, "string_scalar", "attr_str") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  SVAR/Z attr_str
  if (check_result && SVAR_Exists(attr_str) == 0)
    tango_display_error_str("tango_read_attribute : no answer found")
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- READ ATTRIBUTE SCALAR SHORT
  //------------------------------------------------------
  if (tango_read_attribute(device, "short_scalar", "attr_val") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  NVAR/C/Z attr_val
  if (check_result && NVAR_Exists(attr_val) == 0)
    tango_display_error_str("tango_read_attribute : no answer found")
    SetDataFolder ldf
    return -1
  endif
  KillVariables attr_val
  //------------------------------------------------------
  //- READ ATTRIBUTE SCALAR LONG
  //------------------------------------------------------
  if (tango_read_attribute(device, "long_scalar", "attr_val") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  NVAR/C/Z attr_val
  if (check_result && NVAR_Exists(attr_val) == 0)
    tango_display_error_str("tango_read_attribute : no answer found")
    SetDataFolder ldf
    return -1
  endif
  KillVariables attr_val
  //------------------------------------------------------
  //- READ ATTRIBUTE SCALAR DOUBLE
  //------------------------------------------------------
  if (tango_read_attribute(device, "double_scalar", "attr_val") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  NVAR/C/Z attr_val
  if (check_result && NVAR_Exists(attr_val) == 0)
    tango_display_error_str("tango_read_attribute : no answer found")
    SetDataFolder ldf
    return -1
  endif
  KillVariables attr_val
  //------------------------------------------------------
  //- READ ATTRIBUTE SPECTRUM SHORT
  //------------------------------------------------------
  if (tango_read_attribute(device, "short_spectrum", "attr_spec") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  WAVE/Z attr_spec
  if (check_result && WaveExists(attr_spec) == 0)
    tango_display_error_str("tango_read_attribute : no answer found")
    SetDataFolder ldf
    return -1
  endif
  KillWaves attr_spec
  //------------------------------------------------------
  //- READ ATTRIBUTE SPECTRUM LONG
  //------------------------------------------------------
  if (tango_read_attribute(device, "long_spectrum", "attr_spec") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  WAVE/Z attr_spec
  if (check_result && WaveExists(attr_spec) == 0)
    tango_display_error_str("tango_read_attribute : no answer found")
    SetDataFolder ldf
    return -1
  endif
  KillWaves attr_spec
  //------------------------------------------------------
  //- READ ATTRIBUTE SPECTRUM DOUBLE
  //------------------------------------------------------
  if (tango_read_attribute(device, "double_spectrum", "attr_spec") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  WAVE/Z attr_spec
  if (check_result && WaveExists(attr_spec) == 0)
    tango_display_error_str("tango_read_attribute : no answer found")
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- READ ATTRIBUTE IMAGE SHORT
  //------------------------------------------------------
  if (tango_read_attribute(device, "short_image", "attr_img") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  WAVE/Z attr_img
  if (check_result && WaveExists(attr_img) == 0)
    tango_display_error_str("tango_read_attribute : no answer found")
    return -1
  endif
  KillWaves attr_img
  //------------------------------------------------------
  //- READ ATTRIBUTE IMAGE LONG
  //------------------------------------------------------
  if (tango_read_attribute(device, "long_image", "attr_img") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  WAVE/Z attr_img
  if (check_result && WaveExists(attr_img) == 0)
    tango_display_error_str("tango_read_attribute : no answer found")
    SetDataFolder ldf
    return -1
  endif
  KillWaves attr_img
  //------------------------------------------------------
  //- READ ATTRIBUTE IMAGE DOUBLE
  //------------------------------------------------------
  if (tango_read_attribute(device, "double_image", "attr_img") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  WAVE/Z attr_img
  if (check_result && WaveExists(attr_img) == 0)
    tango_display_error_str("tango_read_attribute : no answer found")
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- WRITE ATTRIBUTE SCALAR LONG (ASSOC READ WITH WRITE)
  //------------------------------------------------------
  Variable/G ss_w = 2
  if (tango_write_attribute(device, "short_scalar_w", "ss_w") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- READ ATTRIBUTE SCALAR LONG (READ WITH WRITE)
  //------------------------------------------------------
  if (tango_read_attribute(device, "short_scalar_rww", "ss_rww_val") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  NVAR/C ss_rww_val
  if (check_result && NVAR_Exists(ss_rww_val) == 0)
    tango_display_error_str("tango_read_attribute (rww) : no answer found")
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- WTITE ATTRIBUTE SCALAR LONG (ASSOC READ WITH WRITE)
  //------------------------------------------------------
  Variable/G ls_w = 20
  if (tango_write_attribute(device, "long_scalar_w", "ls_w") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- READ ATTRIBUTE SCALAR LONG (READ WITH WRITE)
  //------------------------------------------------------
  if (tango_read_attribute(device, "long_scalar_rww", "ls_rww_val") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  NVAR/C ls_rww_val
  if (check_result && NVAR_Exists(ls_rww_val) == 0)
    tango_display_error_str("tango_read_attribute (rww) : no answer found")
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- WRITE ATTRIBUTE SCALAR LONG (ASSOC READ WITH WRITE)
  //------------------------------------------------------
  Variable/G ds_w = 200
  if (tango_write_attribute(device, "double_scalar_w", "ds_w") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- READ ATTRIBUTE SCALAR LONG (READ WITH WRITE)
  //------------------------------------------------------
  if (tango_read_attribute(device, "double_scalar_rww", "ds_rww_val") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  NVAR/C ds_rww_val
  if (check_result && NVAR_Exists(ds_rww_val) == 0)
    tango_display_error_str("tango_read_attribute (rww) : no answer found")
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- READ ALL ATTRIBUTES - WITH ARG = LIST <ATTR:VAL>
  //------------------------------------------------------
  String list = ""
  list = tango_attr_val_list_add(list, "string_scalar", "attr_str")
  list = tango_attr_val_list_add(list, "short_scalar", "attr_val1") 
  list = tango_attr_val_list_add(list, "long_scalar", "attr_val2") 
  list = tango_attr_val_list_add(list, "double_scalar", "attr_val3") 
  list = tango_attr_val_list_add(list, "short_spectrum", "attr_spec1") 
  list = tango_attr_val_list_add(list, "long_spectrum", "attr_spec2")
  list = tango_attr_val_list_add(list, "double_spectrum", "attr_spec3")
  list = tango_attr_val_list_add(list, "short_image", "attr_img1")
  list = tango_attr_val_list_add(list, "long_image", "attr_img2")
  list = tango_attr_val_list_add(list, "double_image", "attr_img3")
  if (tango_read_attributes(device, list) == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  if (check_result) 
    Variable err = 0
    SVAR/Z attr_str
    if (SVAR_Exists(attr_str) == 0)
      err += 1
    endif
    NVAR/C/Z attr_val1
    if (NVAR_Exists(attr_val1) == 0)
      err += 1
    endif
    NVAR/C/Z attr_val2
    if (NVAR_Exists(attr_val2) == 0)
      err += 1
    endif
    NVAR/C/Z attr_val3
    if (NVAR_Exists(attr_val3) == 0)
      err += 1
    endif
    WAVE/Z attr_spec1
    if (WAVEExists(attr_spec1) == 0)
      err += 1
    endif
    WAVE/Z attr_spec2
    if (WAVEExists(attr_spec2) == 0)
      err += 1
    endif
    WAVE/Z attr_spec2
    if (WAVEExists(attr_spec3) == 0)
      err += 1
    endif
    WAVE/Z attr_img1
    if (WAVEExists(attr_img1) == 0)
      err += 1
    endif
    WAVE/Z attr_img2
    if (WAVEExists(attr_img2) == 0)
      err += 1
    endif
    WAVE/Z attr_img2
    if (WAVEExists(attr_img3) == 0)
      err += 1
    endif
    if (err > 0) 
      tango_display_error_str("tango_read_attributes : at least one answer found")
      SetDataFolder ldf
      return -1
    endif
  endif
  //------------------------------------------------------
  KillStrings/A/Z 
  KillVariables/A/Z
  KillWaves/A/Z
  //------------------------------------------------------
  //- READ ALL ATTRIBUTES - WITH ARG = WAVE/T 2D
  //------------------------------------------------------
  Make/O/T/N=(10,2) attr_val_wav
  attr_val_wav[0][0] = "string_scalar"
  attr_val_wav[0][1] = "attr_str"
  attr_val_wav[1][0] = "short_scalar"
  attr_val_wav[1][1] = "attr_val1"
  attr_val_wav[2][0] = "long_scalar"
  attr_val_wav[2][1] = "attr_val2"
  attr_val_wav[3][0] = "double_scalar"
  attr_val_wav[3][1] = "attr_val3"
  attr_val_wav[4][0] = "short_spectrum"
  attr_val_wav[4][1] = "attr_spec1"
  attr_val_wav[5][0] = "long_spectrum"
  attr_val_wav[5][1] = "attr_spec2"
  attr_val_wav[6][0] = "double_spectrum"
  attr_val_wav[6][1] = "attr_spec3"
  attr_val_wav[7][0] = "short_image"
  attr_val_wav[7][1] = "attr_img1"
  attr_val_wav[8][0] = "long_image"
  attr_val_wav[8][1] = "attr_img2"
  attr_val_wav[9][0] = "double_image"
  attr_val_wav[9][1] = "attr_img3"
  if (tango_read_attributes(device, "attr_val_wav") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  if (check_result) 
    err = 0
    SVAR/Z attr_str = attr_str
    if (SVAR_Exists(attr_str) == 0)
      err += 1
    endif
    NVAR/C/Z attr_val1
    if (NVAR_Exists(attr_val1) == 0)
      err += 1
    endif
    NVAR/C/Z attr_val2
    if (NVAR_Exists(attr_val2) == 0)
      err += 1
    endif
    NVAR/C/Z attr_val3
    if (NVAR_Exists(attr_val3) == 0)
      err += 1
    endif
    WAVE/Z attr_spec1
    if (WAVEExists(attr_spec1) == 0)
      err += 1
    endif
    WAVE/Z attr_spec2
    if (WAVEExists(attr_spec2) == 0)
      err += 1
    endif
    WAVE/Z attr_spec2
    if (WAVEExists(attr_spec3) == 0)
      err += 1
    endif
    WAVE/Z attr_img1
    if (WAVEExists(attr_img1) == 0)
      err += 1
    endif
    WAVE/Z attr_img2
    if (WAVEExists(attr_img2) == 0)
      err += 1
    endif
    WAVE/Z attr_img2
    if (WAVEExists(attr_img3) == 0)
      err += 1
    endif
    if (err > 0) 
      tango_display_error_str("tango_read_attributes : at least one answer found")
      SetDataFolder ldf
      return -1
    endif
  endif
  //------------------------------------------------------
  //- WRITE ATTRIBUTE SCALAR STRING
  //------------------------------------------------------
  if (tango_write_attribute(device, "string_scalar", "attr_str") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- WRITE ATTRIBUTE SCALAR SHORT
  //------------------------------------------------------
  if (tango_write_attribute(device, "short_scalar", "attr_val1") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- WRITE ATTRIBUTE SCALAR LONG
  //------------------------------------------------------
  if (tango_write_attribute(device, "long_scalar", "attr_val2") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- WRITE ATTRIBUTE SCALAR DOUBLE
  //------------------------------------------------------
  if (tango_write_attribute(device, "double_scalar", "attr_val3") == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- WRITE ALL WRITABLE ATTRIBUTES - WITH ARG = LIST <ATTR:VAL>
  //------------------------------------------------------
  list = ""
  list = tango_attr_val_list_add(list, "string_scalar", "attr_str")
  list = tango_attr_val_list_add(list, "short_scalar", "attr_val1") 
  list = tango_attr_val_list_add(list, "long_scalar", "attr_val2") 
  list = tango_attr_val_list_add(list, "double_scalar", "attr_val3") 
  if (tango_write_attributes(device, list) == -1)
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- WRITE ALL WRITABLE ATTRIBUTES - WITH ARG = WAVE/T 2D
  //------------------------------------------------------
  Make/O/T/N=(4,2) attr_val_wav
  attr_val_wav[0][0] = "string_scalar"
  attr_val_wav[0][1] = "attr_str"
  attr_val_wav[1][0] = "short_scalar"
  attr_val_wav[1][1] = "attr_val1"
  attr_val_wav[2][0] = "long_scalar"
  attr_val_wav[2][1] = "attr_val2"
  attr_val_wav[3][0] = "double_scalar"
  attr_val_wav[3][1] = "attr_val3"
  if (tango_write_attributes(device, "attr_val_wav") == -1)
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- EXEC DEVDOUBLE USING FULL PATH NAME FOR IN & OUT
  //------------------------------------------------------
  NewDataFolder /O root:sub1
  NewDataFolder /O /S root:sub1:sub2
  Variable/G in = pi
  SetDataFolder root:tango_test_I
  String in_path = "root:sub1:sub2:in"
  String out_path = "root:sub1:sub2:sub3:out"
  if (tango_command_inout(device, "DevDouble", in_path, out_path) == -1) 
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif
  NVAR/C/Z out = $out_path
  if (NVAR_exists(out) == 0) 
    tango_display_error_str("tango_read_attribute : no answer found")
    SetDataFolder ldf
    return -1
  endif
  if (out != pi) 
    tango_display_error_str ("tango_read_attribute : argin != argout")
    SetDataFolder ldf
    return -1
  endif
  //------------------------------------------------------
  //- READ ATTR USING FULL PATH NAME
  //------------------------------------------------------
  out_path = "root:sub1:attr_val"
  if (tango_read_attribute(device, "short_scalar", out_path) == -1) 
    tango_display_error()
  endif
  NVAR/C/Z attr_val = $out_path
  if (NVAR_Exists(attr_val) == 0)
    tango_display_error_str("tango_read_attribute : no answer found")
    SetDataFolder ldf
    return -1
  endif
  KillDataFolder root:sub1
  //------------------------------------------------------
  KillStrings/A/Z 
  KillVariables/A/Z
  KillWaves/A/Z
  KillDataFolder root:tango_test_I
  //------------------------------------------------------
  if (DataFolderExists(ldf))
    SetDataFolder ldf
  endif
  return 0
end

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

//==============================================================================
// fonction :   tango_test_II 
//------------------------------------------------------------------------------
// Executes <it> times each command of the <tester> device.  
//==============================================================================
function tango_test_II (device, it, wsize)
  String device
  Variable it
  Variable wsize
  Variable i = 0
  String ldf = GetDataFolder(1)
  NewDataFolder/O/S root:tango_test_II
  Variable timer
  Variable elapsed_ms
  Variable msec_per_it
  //------------------------------------------------------
  //- PING THE DEVICE (IS THERE SOMEONE OUT THERE!)
  //------------------------------------------------------
  if (tango_ping_device(device) == -1)
    tango_display_error()
    SetDataFolder ldf
    return -1
  endif 
  //------------------------------------------------------
  //- EXEC DEV_VOID
  //------------------------------------------------------
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_command_inout(device, "DevVoid", "", "") == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "DevVoid test took : %.6f ms/iteration\r", msec_per_it 
  //------------------------------------------------------
  //- EXEC DEV_BOOLEAN
  //------------------------------------------------------
  Variable/G argin = 1
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_command_inout(device, "DevBoolean", "argin", "argout") == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "DevBoolean test took : %.6f ms/iteration\r", msec_per_it 
  //------------------------------------------------------
  //- EXEC DEV_SHORT
  //------------------------------------------------------
  argin = trunc(enoise(256))
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_command_inout(device, "DevShort", "argin", "argout") == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "DevShort test took : %.6f ms/iteration\r", msec_per_it 
  //------------------------------------------------------
  //- EXEC DEV_USHORT
  //------------------------------------------------------
  argin = trunc(abs(enoise(256)))
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_command_inout(device, "DevUShort", "argin", "argout") == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "DevUShort test took : %.6f ms/iteration\r", msec_per_it 
  //------------------------------------------------------
  //- EXEC DEV_LONG
  //------------------------------------------------------
  argin = trunc(enoise(256))
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_command_inout(device, "DevLong", "argin", "argout") == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "DevUShort test took : %.6f ms/iteration\r", msec_per_it 
  //------------------------------------------------------
  //- EXEC DEV_ULONG
  //------------------------------------------------------
  argin = trunc(abs(enoise(256)))
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_command_inout(device, "DevULong", "argin", "argout") == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "DevULong test took : %.6f ms/iteration\r", msec_per_it 
  //------------------------------------------------------
  //- EXEC DEV_FLOAT
  //------------------------------------------------------
  argin = enoise(256)
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_command_inout(device, "DevFloat", "argin", "argout") == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "DevFloat test took : %.6f ms/iteration\r", msec_per_it 
  //------------------------------------------------------
  //- EXEC DEV_DOUBLE
  //------------------------------------------------------
  argin = enoise(256)
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_command_inout(device, "DevDouble", "argin", "argout") == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "DevDouble test took : %.6f ms/iteration\r", msec_per_it 
  //------------------------------------------------------
  //- EXEC DEV_STRING
  //------------------------------------------------------
  //- METHOD I
  String/G argin_str_ = "abcdefghijklmnopqrstuvwxyz123456789"
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_command_inout(device, "DevString",  "argin_str_", "argout_str_") == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "DevString test took : %.6f ms/iteration\r", msec_per_it 
  //------------------------------------------------------
  //- EXEC DEVVAR_CHARARRAY  (SHOULD BE DEVVAR_UCHARARRAY!)
  //------------------------------------------------------
  Make /O /N=(wsize) /Y=(0x08|0x40) argin_wav  // a wave of <unsigned char> 
  argin_wav = trunc(enoise(wsize))
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_command_inout(device, "DevVarCharArray", "argin_wav", "argout_wav") == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3 
  msec_per_it = elapsed_ms / it
  printf "DevVarCharArray test took : %.6f ms/iteration\r", msec_per_it
  //------------------------------------------------------
  //- EXEC DEVVAR_USHORTARRAY 
  //------------------------------------------------------
  Make /O /N=(wsize) /Y=(0x10|0x40) argin_wav  // a wave of <unsigned short> 
  argin_wav = trunc(abs(enoise(wsize)))
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_command_inout(device, "DevVarUShortArray", "argin_wav", "argout_wav") == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "DevVarUShortArray test took : %.6f ms/iteration\r", msec_per_it 
  //------------------------------------------------------
  //- EXEC DEVVAR_SHORTARRAY 
  //------------------------------------------------------
  Make /O /N=(wsize) /Y=(0x10) argin_wav  // a wave of <short> 
  argin_wav = trunc(enoise(wsize))
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_command_inout(device, "DevVarShortArray", "argin_wav", "argout_wav") == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "DevVarShortArray test took : %.6f ms/iteration\r", msec_per_it 
  //------------------------------------------------------
  //- EXEC DEVVAR_ULONGARRAY 
  //------------------------------------------------------
  Make /O /N=(wsize) /Y=(0x20|0x40) argin_wav  // a wave of <unsigned long> 
  argin_wav = trunc(abs(enoise(wsize)))
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_command_inout(device, "DevVarULongArray", "argin_wav", "argout_wav") == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "DevVarULongArray test took : %.6f ms/iteration\r", msec_per_it 
  //------------------------------------------------------
  //- EXEC DEVVAR_LONGARRAY 
  //------------------------------------------------------
  Make /O /N=(wsize) /Y=(0x20) argin_wav  // a wave of <long> 
  argin_wav = trunc(abs(enoise(wsize)))
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_command_inout(device, "DevVarLongArray", "argin_wav", "argout_wav") == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "DevVarLongArray test took : %.6f ms/iteration\r", msec_per_it 
  //------------------------------------------------------
  //- EXEC DEVVAR_FLOATARRAY 
  //------------------------------------------------------
  Make /O /N=(wsize) /Y=(0x2) argin_wav  // a wave of <float> 
  argin_wav = enoise(wsize)
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_command_inout(device, "DevVarFloatArray", "argin_wav", "argout_wav") == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "DevVarFloatArray test took : %.6f ms/iteration\r", msec_per_it 
  //------------------------------------------------------
  //- EXEC DEVVAR_DOUBLEARRAY 
  //------------------------------------------------------
  Make /O /N=(wsize) /Y=(0x4) argin_wav  // a wave of <double> 
  argin_wav = enoise(wsize)
  timer = StartMSTimer
    for (i = 0; i < it;  i += 1) 
    if (tango_command_inout(device, "DevVarDoubleArray", "argin_wav", "argout_wav") == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "DevVarDoubleArray test took : %.6f ms/iteration\r", msec_per_it
  //------------------------------------------------------
  //- EXEC DEVVAR_STRINGARRAY 
  //------------------------------------------------------
  Make /O /N=(wsize) /T in_swav // a wave of <string>
  in_swav = "VarStringArray-Test"  
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_command_inout(device, "DevVarStringArray", "in_swav", "out_swav") == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "DevVarStringArray test took : %.6f ms/iteration\r", msec_per_it
  //------------------------------------------------------
  //- EXEC DEVVAR_LONGSTRINGARRAY 
  //------------------------------------------------------
  Make /O /N=(wsize) /Y=(0x20) in_nwav    // a wave of <long> 
  in_nwav = trunc(enoise(wsize))  
  Make /O /N=(wsize) /T in_swav // a wave of <string>
  in_swav = "DevVarLongStringArray-Test"  
  String cmd = "DevVarLongStringArray"
  timer = StartMSTimer
  String argin_str = "in_nwav" + kNAME_SEP + "in_swav"
  String argout_str = "out_nwav" + kNAME_SEP + "out_swav"
  for (i = 0; i < it;  i += 1) 
    if (tango_command_inout(device, cmd, argin_str, argout_str) == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "DevVarLongStringArray test took : %.6f ms/iteration\r", msec_per_it
  //------------------------------------------------------
  //- EXEC DEVVAR_DOUBLESTRINGARRAY 
  //------------------------------------------------------
  Make /O /N=(wsize) /Y=(0x4) in_nwav   // a wave of <double> 
  in_nwav = enoise(wsize)
  Make /O /N=(wsize) /T in_swav // a wave of <string>
  in_swav = "DevVarDoubleStringArray-Test"  
  cmd = "DevVarDoubleStringArray"
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_command_inout(device, cmd, argin_str, argout_str) == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "DevVarDoubleStringArray test took : %.6f ms/iteration\r", msec_per_it
  //------------------------------------------------------
  //- READ ATTRIBUTE SCALAR STRING
  //------------------------------------------------------
  Variable cumul = 0
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_read_attribute(device, "string_scalar", "attr_str") == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "tango_read_attribute<string_scalar> test took : %.6f ms/iteration\r", msec_per_it
  cumul += msec_per_it
  //------------------------------------------------------
  //- READ ATTRIBUTE SCALAR SHORT
  //------------------------------------------------------
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_read_attribute(device, "short_scalar", "attr_val") == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "tango_read_attribute<short_scalar> test took : %.6f ms/iteration\r", msec_per_it
  cumul += msec_per_it
  //------------------------------------------------------
  //- READ ATTRIBUTE SCALAR LONG
  //------------------------------------------------------
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_read_attribute(device, "long_scalar", "attr_val") == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "tango_read_attribute<long_scalar> test took : %.6f ms/iteration\r", msec_per_it
  cumul += msec_per_it
  //------------------------------------------------------
  //- READ ATTRIBUTE SCALAR DOUBLE
  //------------------------------------------------------
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_read_attribute(device, "double_scalar", "attr_val") == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "tango_read_attribute<double_scalar> test took : %.6f ms/iteration\r", msec_per_it
  cumul += msec_per_it
  //------------------------------------------------------
  //- READ ATTRIBUTE SPECTRUM SHORT
  //------------------------------------------------------
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_read_attribute(device, "short_spectrum", "attr_spec") == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "tango_read_attribute<short_spectrum> test took : %.6f ms/iteration\r", msec_per_it
  cumul += msec_per_it
  //------------------------------------------------------
  //- READ ATTRIBUTE SPECTRUM LONG
  //------------------------------------------------------
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_read_attribute(device, "long_spectrum", "attr_spec") == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "tango_read_attribute<long_spectrum> test took : %.6f ms/iteration\r", msec_per_it
  cumul += msec_per_it
  //------------------------------------------------------
  //- READ ATTRIBUTE SPECTRUM DOUBLE
  //------------------------------------------------------
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_read_attribute(device, "double_spectrum", "attr_spec") == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "tango_read_attribute<double_spectrum> test took : %.6f ms/iteration\r", msec_per_it
  cumul += msec_per_it
  //------------------------------------------------------
  //- READ ATTRIBUTE IMAGE SHORT
  //------------------------------------------------------
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_read_attribute(device, "short_image", "attr_img") == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "tango_read_attribute<short_image> test took : %.6f ms/iteration\r", msec_per_it
  cumul += msec_per_it
  //------------------------------------------------------
  //- READ ATTRIBUTE IMAGE LONG
  //------------------------------------------------------
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_read_attribute(device, "long_image", "attr_img") == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "tango_read_attribute<long_image> test took : %.6f ms/iteration\r", msec_per_it
  cumul += msec_per_it
  //------------------------------------------------------
  //- READ ATTRIBUTE IMAGE DOUBLE
  //------------------------------------------------------
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_read_attribute(device, "double_image", "attr_img") == -1) 
      tango_display_error()
      SetDataFolder ldf
      return -1
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "tango_read_attribute<double_image> test took : %.6f ms/iteration\r", msec_per_it
  cumul += msec_per_it
  printf "Sum of previous tango_read_attribute<x> : %.6f ms/iteration\r", cumul
  //------------------------------------------------------
  //- READ ATTRIBUTES
  //------------------------------------------------------
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    tango_read_attribute(device, "string_scalar", "attr_str")
    tango_read_attribute(device, "short_scalar", "attr_val1") 
    tango_read_attribute(device, "long_scalar", "attr_val2") 
    tango_read_attribute(device, "double_scalar", "attr_val3") 
    tango_read_attribute(device, "short_spectrum", "attr_spec1") 
    tango_read_attribute(device, "long_spectrum", "attr_spec2")
    tango_read_attribute(device, "double_spectrum", "attr_spec3")
    tango_read_attribute(device, "short_image", "attr_img1")
    tango_read_attribute(device, "long_image", "attr_img2")
    tango_read_attribute(device, "double_image", "attr_img3")
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "Reading each attr once took : %.6f ms/iteration\r", msec_per_it
  printf "for loop overhead : %.6f ms\r", cumul - msec_per_it
  KillStrings/Z attr_str
  KillVariables/Z attr_val1, attr_val2, attr_val3
  KillWaves/Z attr_spec1, attr_spec2, attr_spec3
  KillWaves/Z attr_img1, attr_img2, attr_img3
  String list = ""
  list = tango_attr_val_list_add(list, "string_scalar", "attr_str")
  list = tango_attr_val_list_add(list, "short_scalar", "attr_val1") 
  list = tango_attr_val_list_add(list, "long_scalar", "attr_val2") 
  list = tango_attr_val_list_add(list, "double_scalar", "attr_val3") 
  list = tango_attr_val_list_add(list, "short_spectrum", "attr_spec1") 
  list = tango_attr_val_list_add(list, "long_spectrum", "attr_spec2")
  list = tango_attr_val_list_add(list, "double_spectrum", "attr_spec3")
  list = tango_attr_val_list_add(list, "short_image", "attr_img1")
  list = tango_attr_val_list_add(list, "long_image", "attr_img2")
  list = tango_attr_val_list_add(list, "double_image", "attr_img3")
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    tango_read_attributes(device, list);
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "Reading all attrs in one call took (out objects do not exist): %.6f ms/iteration\r", msec_per_it
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    tango_read_attributes(device, list);
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "Reading all attrs in one call took (out objects exist) : %.6f ms/iteration\r", msec_per_it
  //------------------------------------------------------
  //- WRITE ATTRIBUTES
  //------------------------------------------------------
  timer = StartMSTimer
  String/G wattr_str = "test"
  Variable/G wattr_val = 100
  for (i = 0; i < it;  i += 1) 
    tango_write_attribute(device, "string_scalar", "wattr_str")
    tango_write_attribute(device, "short_scalar", "wattr_val") 
    tango_write_attribute(device, "long_scalar", "wattr_val") 
    tango_write_attribute(device, "double_scalar", "wattr_val") 
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "Writting each attr once took : %.6f ms/iteration\r", msec_per_it
  printf "for loop overhead : %.6f ms\r", cumul - msec_per_it
  list = ""
  list = tango_attr_val_list_add(list, "string_scalar", "wattr_str")
  list = tango_attr_val_list_add(list, "short_scalar", "wattr_val") 
  list = tango_attr_val_list_add(list, "long_scalar", "wattr_val") 
  list = tango_attr_val_list_add(list, "double_scalar", "wattr_val")
  timer = StartMSTimer
  for (i = 0; i < it;  i += 1) 
    if (tango_write_attributes(device, list) == -1) 
      tango_display_error()
      break
    endif
  endfor  
  elapsed_ms = StopMSTimer(timer) * 1.E-3
  msec_per_it = elapsed_ms / it
  printf "Writting all attrs in one call took: %.6f ms/iteration\r", msec_per_it
  //-----------------------------------------------------------------------------
  KillStrings/A/Z 
  KillVariables/A/Z
  KillWaves/A/Z
  KillDataFolder root:tango_test_II
  //-----------------------------------------------------------------------------
  if (DataFolderExists(ldf))
    SetDataFolder ldf
  endif
  return 0
end

//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

//==============================================================================
// fonction :   tango_errors
//==============================================================================
function tango_errors (device, print_errors)
  String device
  Variable print_errors
  Variable i = 0
  String ldf = GetDataFolder(1)
  NewDataFolder/O/S root:tango_errors
  Variable errors = 0
  Variable expected_errors = 0
  //------------------------------------------------------
  //- PASS DUMMY ARGS TO DEV_VOID (NO ERROR EXPECTED)
  //------------------------------------------------------
  if (tango_command_inout(device, "DevVoid", "dummy_in", "dummy_out") == -1)
    errors += 1
    if (print_errors) 
      tango_print_error() 
    endif
  endif
  //------------------------------------------------------
  //- PASS STRING TO DEV_BOOLEAN (ERROR EXPECTED)
  //------------------------------------------------------
  String/G sargin = ""
  expected_errors += 1
  if (tango_command_inout(device, "DevBoolean", "sargin", "argout") == -1) 
    errors += 1
    if (print_errors) 
      tango_print_error() 
    endif
  endif
  //------------------------------------------------------
  //- CREATE A WAVE NAMED ARGOUT THEN CALL DEV_SHORT (ERROR EXPECTED)
  //------------------------------------------------------
  Variable/G vargin = 0
  Make /O /N=0 argout
  expected_errors += 1
  if (tango_command_inout(device, "DevShort", "vargin", "argout") == -1) 
    errors += 1
    if (print_errors) 
      tango_print_error() 
    endif
  endif
  KillWaves argout
  //------------------------------------------------------
  //- PASS A COMPLEX VAR TO DEV_USHORT (ERROR EXPECTED)
  //------------------------------------------------------
  Variable/C cargin
  expected_errors += 1
  if (tango_command_inout(device, "DevUShort", "cargin", "argout") == -1) 
    errors += 1
    if (print_errors) 
      tango_print_error() 
    endif
  endif
  //------------------------------------------------------
  //- EXEC DEV_LONG ON A DUMMY DEVICE (ERROR EXPECTED)
  //------------------------------------------------------
  vargin = trunc(enoise(256))
  expected_errors += 1
  if (tango_command_inout("_dummy_", "DevLong", "vargin", "vargout") == -1) 
    errors += 1
    if (print_errors) 
      tango_print_error() 
    endif
  endif
  //------------------------------------------------------
  //- EXEC DEV_ULONG WITH EMPTY ARGS (ERROR EXPECTED)
  //------------------------------------------------------
  expected_errors += 1
  if (tango_command_inout(device, "DevULong", "", "") == -1) 
    errors += 1
    if (print_errors) 
      tango_print_error() 
    endif
  endif
  //------------------------------------------------------
  //- EXEC A DUMMY CMD ON DEVICE (ERROR EXPECTED)
  //------------------------------------------------------
  vargin = enoise(256)
  expected_errors += 1
  if (tango_command_inout(device, "_dummy_", "vargin", "vargout") == -1) 
    errors += 1
    if (print_errors) 
      tango_print_error() 
    endif
  endif
  //------------------------------------------------------
  //- PASS WAVE<DOUBLE> TO DEVVAR_CHARARRAY (ERROR EXPECTED)
  //------------------------------------------------------
  Make /O /N=1024 /D argin_wav
  argin_wav = trunc(enoise(1024))
  expected_errors += 1
  if (tango_command_inout(device, "DevVarCharArray", "argin_wav", "argout_wav") == -1) 
    errors += 1
    if (print_errors) 
      tango_print_error() 
    endif
  endif
  //------------------------------------------------------
  //- CREATE 3D TEXT WAVE NAMED <argout_w> THEN EXEC DEVVAR_USHORTARRAY (ERROR EXPECTED)
  //------------------------------------------------------
  Make /O /N=2 /U /W argin_w  
  Make /O /N=(1,1,1) /T argout_w
  expected_errors += 1 
  if (tango_command_inout(device, "DevVarUShortArray", "argin_w", "argout_w") == -1) 
    errors += 1
    if (print_errors) 
      tango_print_error() 
    endif
  endif
  //------------------------------------------------------
  //- CREATE 2D FLOAT WAVE  NAMED <argout_wa> THEN EXEC DEVVAR_SHORTARRAY (NO ERROR EXPECTED)
  //------------------------------------------------------
  Make /O /N=1024 /W argin_wa 
  Make /O /N=(2,2) argout_wa 
  argin_wav = trunc(enoise(1024))
  if (tango_command_inout(device, "DevVarShortArray", "argin_wa", "argout_wa") == -1) 
    errors += 1
    if (print_errors) 
      tango_print_error() 
    endif
  endif
  //------------------------------------------------------
  //- SYNTAX ERROR IN DEVVAR_LONGSTRINGARRAY ARGIN (ERROR EXPECTED)
  //------------------------------------------------------
  Make /O /N=1024 /Y=(0x20) in_nwav   // a wave of <long> 
  in_nwav = trunc(enoise(1024))  
  Make /O /N=1024 /T in_swav // a wave of <string>
  in_swav = "VarLongStringArray-Test"  
  String cmd = "DevVarLongStringArray"
  expected_errors += 1
  if (tango_command_inout(device, cmd, "in_nwav;in_swav", "out_nwav.out_swav") == -1) 
    errors += 1
    if (print_errors) 
      tango_print_error() 
    endif
  endif
  expected_errors += 1
  if (tango_command_inout(device, cmd, "in_nwav.in_swav", "out_nwav;out_swav") == -1) 
    errors += 1
    if (print_errors) 
      tango_print_error() 
    endif
  endif
  //------------------------------------------------------
  //- SYNTAX ERROR IN DEVVAR_DOUBLESTRINGARRAY ARGIN (ERROR EXPECTED)
  //------------------------------------------------------
  Make /O /N=1024 /Y=(0x4) in_nwav    // a wave of <double> 
  in_nwav = enoise(1024)
  Make /O /N=1024 /T in_swav // a wave of <string>
  in_swav = "VarDoubleStringArray-Test"  
  cmd = "DevVarDoubleStringArray"
  expected_errors += 1
  if (tango_command_inout(device, cmd, "in_nwav;in_swav", "out_nwav.out_swav") == -1) 
    errors += 1
    if (print_errors) 
      tango_print_error() 
    endif
  endif
  expected_errors += 1
  if (tango_command_inout(device, cmd, "in_nwav.in_swav", "out_nwav;out_swav") == -1) 
    errors += 1
    if (print_errors) 
      tango_print_error() 
    endif
  endif
  //------------------------------------------------------
  //- READ DUMMY ATTRIBUTE (ERROR EXPECTED)
  //------------------------------------------------------
  expected_errors += 1
  if (tango_read_attribute(device, "_dummy_", "attr_str") == -1) 
    errors += 1
    if (print_errors) 
      tango_print_error() 
    endif
  endif
  //------------------------------------------------------
  //- WRITE DUMMY ATTRIBUTE (ERROR EXPECTED)
  //------------------------------------------------------
  expected_errors += 1
  Variable value = 0
  if (tango_write_attribute(device, "_dummy_", "value") == -1) 
    errors += 1
    if (print_errors) 
      tango_print_error() 
    endif
  endif
  //------------------------------------------------------
  //- WRITE SHORT_SCALAR ATTRIBUTE WITH A LOCAL VAR (ERROR EXPECTED)
  //------------------------------------------------------
  expected_errors += 1
  if (tango_write_attribute(device, "short_scalar", "value") == -1) 
    errors += 1
    if (print_errors) 
      tango_print_error() 
    endif
  endif
  //------------------------------------------------------
  KillStrings/A/Z 
  KillVariables/A/Z
  KillWaves/A/Z
  KillDataFolder root:tango_errors
  //------------------------------------------------------
  if (DataFolderExists(ldf))
    SetDataFolder ldf
  endif
  if (expected_errors != errors)
    print "@@@@@@ expected errors != actual errors @@@@@@"
  endif
  return 0
end

