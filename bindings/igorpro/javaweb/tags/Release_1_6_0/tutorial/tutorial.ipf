//==============================================================================
// 					    Tutorial.ipf - N.Leclercq - SOLEIL - 01/2006
//==============================================================================
// This file contains a "using TANGO API for Igor Pro" tutorial. It also act as 
// a regression test for the binding itself. You will find very detailed examples
// of the TANGO API (application programing interface). 
// 
// What is the prerequisite for programming with the TANGO API for Igor Pro? 
// The minimum is to be confortable with the TANGO concepts of <attribute> and
// <command>.  
//
// This tutorial use the <TangoTest> device. An instance of this device must be
// running into your control system in order to run this test/tutorial. Once you
// have a living TangoTest, just pass its device name to the test functions [e.g.
// test_tango_cmd_io("tango/tangotest/1")]. You can also test everything by 
// typping "tango_test_all(n_it)" - however, the name od the running TangoTest 
// device should be "tango/tangotest/1".
// 
//  Table of Content:
//
//    Chapter I   : Executing a TANGO command
//    Chapter II  : Reading one (or more) attribute(s) on a TANGO device
//    Chapter III : Writing one (or more) attribute(s) on a TANGO device
//
// This doc may contain some errors. Please let me know.
//
// Happy reading!
// NL - SOLEIL   
//==============================================================================

//==============================================================================
// GLOBALs ACCESS METHOD
//==============================================================================
#pragma rtGlobals=2

//==============================================================================
// DEPENDENCIES
//==============================================================================
//- include the Tango API (required in each procedure using the Tango API)
#include "tango"

//==============================================================================
// TUTORIAL CHAPTER I : Executing a TANGO command
//==============================================================================
// The TANGO API defines a structure for both the input (argin) and the output 
// (argout) argument of a TANGO command. The <CmdArgIn> structure is related to 
// argin while <CmdArgOut> represents argout. These two structue are defined in
// the <Tango.ipf> file. 
//
// <CmdArgIn> has the following structure:
// ---------------------------------------
//  Structure CmdArgIn
//    String dev           //- device name
//    String cmd           //- command name
//    Variable var         //- command argin for any TANGO numeric scalar type 
//    String str           //- command argin for DevString
//    String num_wave_path //- full path to the argin wave for any TANGO numeric array type
//    String str_wave_path //- full path to the argin wave for TANGO string arrays 
//  EndStructure
// 
// On the other end, <CmdArgOut> his defined as follows:
// -----------------------------------------------------
//  Structure CmdArgIn
//    String dev      //- device name
//    String cmd      //- command name
//    Variable var    //- command argout for any TANGO numeric scalar type 
//    String str      //- command argout for DevString
//    Wave num_wave   //- argout wave for any TANGO numeric array type
//    Wave/T str_wave //- argout wave for TANGO string arrays 
//  EndStructure
//
// TANGO command signature vs TANGO for Igor Pro API function:
// -----------------------------------------------------------
// Note: <DEV_VOID> means no argument.
//
// - for a command which argin=<DEV_VOID> and argout=<DEV_VOID> use <tango_cmd_in> (1)
// - for a command which argin=<ANY TANGO TYPE> and argout=<DEV_VOID> use <tango_cmd_in> 
// - for a command which argin=<DEV_VOID> and argout=<ANY TANGO TYPE> use <tango_cmd_out> 
// - for a command which argin=<ANY TANGO TYPE> and argout=<ANY TANGO TYPE> use <tango_cmd_inout> 
//
// (1) may seems strange but we need to specify at least both the device name and the 
// command name. That's why we need to pass a CmdArgIn to a "in=DEV_VOID, out=DEV_VOID" 
// TANGO command (see DEV_VOID/DEV_VOID example).
//
// The following <test_tango_cmd_io> function gives an example for each TANGO 
// argin and argout type. The usage of both <CmdArgIn> and <CmdArgOut> is 
// detailed.
//
// In the proposed examples, argin and argout always have the same type (this due to the 
// way the TangoTest device is implemented). Anyway, if you need to execute a command
// for which argin and argout are different (99% of the cases), just mix the "argin part"
// of the example corresponding to "your" argin type with the "argout part" of the example 
// corresponding to "your" argout type.
//
// TangoTest has a command for each TANGO argument type. Each command is named 
// with the name of the associated type and returns in <argout> a copy of <argin> 
// (i.e. echo like behaviour). It means that we can easily test and valide the 
// TANGO API for Igor by comparing <argin> with <argout> (they should equal each 
// other). The proposed examples just setup argin, execute the command then check
// the result by comparing <argin> with <argout>.    
//==============================================================================  
function test_tango_cmd_io (dev_name)

  //- function arg: the name of the device on which the commands will be executed 
  String dev_name
  
  //- verbose
  print "Starting <Tango-API::tango_cmd_io> test...\r"
  
  //- let's declare our <argin> and <argout> structures. 
  //- be aware that <argout> will be overwritten (and reset) each time we execute a 
  //- command it means that you must use another <CmdArgOut> if case you want to 
  //- store more than one command result at a time. here we reuse both argin and 
  //- argout for each command. 
  
  //- argin
  Struct CmdArgIn argin
  //- argout 
  Struct CmdArgOut argout
  
  //- populate argin: <dev> struct member
  //- the name of the device on which the command will be executed
  //- NB: since the commands are executed on the same device (i.e. dev_name), we set 
  //- the <CmdArgIn.dev> struct member only once (i.e. no need to set it not each time 
  //- we execute a command)  
  argin.dev = dev_name
  
  //-------------------------------------------------------------------------------------------
  // DEV_VOID/DEV_VOID COMMAND  
  //-------------------------------------------------------------------------------------------
  //- an example of a command that does not have any <in> nor <out> argument. In such a case, 
  //- <tango_cmd_in>.
  
  //- populate argin: <CmdArgIn.cmd> struct member
  //- name of the command to be executed on <argin.dev> 
  argin.cmd = "DevVoid"
  
  //- verbose
  print "executing <" + argin.cmd + ">\r"
  
  //- actual cmd execution 
  //- ALWAYS CHECK THE CMD RESULT: 0 means NO_ERROR, -1 means ERROR
  if (tango_cmd_in(argin) == -1)
    //- the cmd failed, display error...
    tango_display_error()
    //- ... then return error
    return -1
  endif
  
  print "<" + argin.cmd + "> cmd passed\r"
  
  //- congratulations you only just executed your first command on a TANGO device!
  //- note that there is no previous connection to the device (it is implicitely done for you)  
  
  //-------------------------------------------------------------------------------------------
  // DEV_BOOL/DEV_BOOL COMMAND  
  //-------------------------------------------------------------------------------------------
  //- an example of a command where argin and argout are a single boolean (i.e. not an array 
  //- of boolean). In such a case, <tango_cmd_inout>. 
  
  //- populate argin: <CmdArgIn.cmd> struct member
  //- name of the command to be executed on <argin.dev> 
  argin.cmd = "DevBoolean"
  
  //- verbose
  print "executing <" + argin.cmd + ">\r"
  
  //- since the command input argument is a numeric scalar (i.e. single numeric value of type
  //- boolean), we stored its value into the <var> member of the <CmdArgIn> structure. This is 
  //- true for any numeric scalar type (see following examples).
  //- for a boolean, 0 means FALSE, 1 means TRUE
  argin.var = 1 
  
  //- actual cmd execution
  //- if an error occurs during command execution, argout is undefined (null or empty members)
  //- ALWAYS CHECK THE CMD RESULT BEFORE TRYING TO ACCESS ARGOUT: 0 means NO_ERROR, -1 means ERROR
  if (tango_cmd_inout(argin, argout) == -1)
    //- the cmd failed, display error...
    tango_display_error()
    //- ... then return error
    return -1
  endif
  
  //- <argout> is populated (i.e. filled) by <tango_cmd_inout> uppon return of the command.
  //- since the command ouput argument is a numeric scalar (i.e. single numeric value), it 
  //- is stored in the <var> member of the <CmdArgOut> structure. This is true for any numeric 
  //- scalar type (see following examples).
  //- as previously explained, we are testing our TANGO binding on a TangoTest device. 
  //- consequently, we check that <argin.var = argout.var> in order to be sure that everything is ok
  if (argin.var != argout.var)
    //- the cmd failed, display error...
    tango_display_error_str("ERROR:DevBoolean:unexpected cmd result - aborting test")
    //- ... then return error
    return -1
  endif
  
  //- verbose
  print argout
  print "<" + argin.cmd + "> cmd passed\r"
  
  //- ok you know how to a execute a command with a scalar numeric arg for argin and/or argout
  //- let's test the remaining "scalar numeric" commands using a dedicated generic function (less 
  //- boring than copy/paste the code)
  
  //- test DevShort
  if (test_num_scalar_cmd (dev_name, "DevShort", -128) == -1)
    return -1
  endif
  //- test DevUShort
  if (test_num_scalar_cmd (dev_name, "DevUShort", 128) == -1)
    return -1
  endif
  //- test DevLong
  if (test_num_scalar_cmd (dev_name, "DevLong", -2048) == -1)
    return -1
  endif
  //- test DevULong
  if (test_num_scalar_cmd (dev_name, "DevULong", 2048) == -1)
    return -1
  endif 
  //- test DevFloat
  if (test_num_scalar_cmd (dev_name, "DevFloat", -3.14159) == -1)
    //- ignore numeric error - numeric rounding (double to float) generates 
    //- a side effect when comparing argin.var to argout.var 
    if (tango_error() != 0)
      //- error comes from TANGO: abort test, continue otherwise
      return -1
    endif
  endif 
  //- test DevDouble
  if (test_num_scalar_cmd (dev_name, "DevDouble", 3.14159) == -1)
    return -1
  endif 
  
  //- ok "numeric scalar" commands are tested and seems to work properly
  //- now, let's try with a string scalar arg...
  
  //-------------------------------------------------------------------------------------------
  // DEV_STRING/DEV_STRING COMMAND  
  //-------------------------------------------------------------------------------------------
  //- an example of a command where argin and argout are a single string (i.e. not an array 
  //- of strings). In such a case, <tango_cmd_inout>. 
  
  //- populate argin: <CmdArgIn.cmd> struct member
  //- name of the command to be executed on <argin.dev> 
  argin.cmd = "DevString"

  //- verbose
  print "executing <" + argin.cmd + ">\r"
  
  //- since the command argin is a string scalar (i.e. single string), we stored its its value 
  //- into the <str> member of the <CmdArgIn> structure. 
  argin.str = "hello world"
  
  //- actual cmd execution
  //- if an error occurs during command execution, argout is undefined (null or empty members)
  //- ALWAYS CHECK THE CMD RESULT BEFORE TRYING TO ACCESS ARGOUT: 0 means NO_ERROR, -1 means ERROR
  if (tango_cmd_inout(argin, argout) == -1)
    //- the cmd failed, display error...
    tango_display_error()
    //- ... then return error
    return -1
  endif
  
  //- <argout> is populated (i.e. filled) by <tango_cmd_inout> uppon return of the command.
  //- since the command ouput argument is a string scalar (i.e. single string), it is stored 
  //- in the <str> member of the <CmdArgOut> structure.

  //- as previously explained, we are testing our TANGO binding on a TangoTest device. 
  //- consequently, we check that <argin.str = argout.str> in order to be sure that everything is ok
  if (cmpstr(argin.str, argout.str) != 0)
    //- the cmd failed, display error...
    tango_display_error_str("ERROR:DevString:unexpected cmd result - aborting test")
    //- ... then return error
    return -1
  endif
  
  //- verbose
  print argout
  print "<" + argin.cmd + "> cmd passed\r"
  
  //- ok all "scalar" commands are tested and seems to work properly
  //- now, let's try with arrays...

  //-------------------------------------------------------------------------------------------
  // DEV_STRING/DEV_STRING COMMAND  
  //-------------------------------------------------------------------------------------------
  //- an example of a command where argin and argout are both an array of longs. Here again, we
  //- use <tango_cmd_inout> in order to execute the comand.
  
  //- populate argin: <cmd> struct member
  //- name of the command to be executed on <argin.dev> 
  argin.cmd = "DevVarLongArray"
  
  //- verbose
  print "executing <" + argin.cmd + ">\r"
  
  //- build the argin wave...
  //- be aware that its data type must match the exact TANGO argin type. the simplest way to 
  //- do that is to use the provided helper function <tango_argin_type_to_wave_type>. this
  //- function of the TANGO API returns the correct wave data type for the specified device
  //- and command. the /Y option of Make will do the rest. 
  //- so get the wave data type...
  Variable wave_type = tango_argin_type_to_wave_type(argin.dev, argin.cmd)
  //- ...then the argin wave   
  Make /O /N=128 /Y=(wave_type) argin_wave = abs(enoise(1024))
  
  //- we do not actually pass the wave to <tango_cmd_inout> - what is required is the full path 
  //- to the argin wave. in fact we need to provide the TANGO API with the location of the argin 
  //- wave (this due to an Igor Pro limitation in the management of the TANGO genericity). 
  //- since argin is a numeric wave, its path is stored into the <num_wave_path> of the <CmdArgIn>
  //- structure. this is true for any numeric argin wave. an utility function, <tools_wave_path> 
  //- is provided in order to ease this process  
  argin.num_wave_path = tools_wave_path(argin_wave)
  
  //- actual cmd execution
  //- if an error occurs during command execution, argout is undefined (null or empty members)
  //- ALWAYS CHECK THE CMD RESULT BEFORE TRYING TO ACCESS ARGOUT: 0 means NO_ERROR, -1 means ERROR
  if (tango_cmd_inout(argin, argout) == -1)
    //- the cmd failed, display error...
    tango_display_error()
    //- ... then return error
    return -1
  endif
  
  //- <argout> is populated (i.e. filled) by <tango_cmd_inout> uppon return of the command.
  //- since the command ouput argument is a numeric array of long, it is stored in the 
  //- <num_wave> member of the <CmdArgOut> structure. this is true for any numeric argout array.   
  //- in order to access the command result, we first need to obtain a reference to argout.num_wave ...
  WAVE argout_wave = argout.num_wave
  
  //- as previously explained, we are testing our TANGO binding on a TangoTest device. 
  //- consequently, we check that <argin = argout> in order to be sure that everything is ok
  //- we use the Igor's <x> operator in order for the comparaison. just magic!
  if (argin_wave[x] != argout_wave[x])
    //- the cmd failed, display error...
    tango_display_error_str("ERROR:DevVarLongStringArray:unexpected cmd result - aborting test")
    //- ... then return error
    return -1
  endif
  
  //- verbose
  print argout
  print "<" + argin.cmd + "> cmd passed\r"
  
  //- cleanup (suppose that waves are no longer needed)
  KillWaves/Z argin_wave, argout_wave  
  
  //- ok, you know how to a execute a command with a numeric array arg for argin and/or argout
  //- let's test the remaining commands using a dedicated generic function (less boring than 
  //- copy/paste the code)
  
  //- test DevVarCharArray
  if (test_num_array_cmd (dev_name, "DevVarCharArray") == -1)
    return -1
  endif
  //- test DevVarShortArray
  if (test_num_array_cmd (dev_name, "DevVarShortArray") == -1)
    return -1
  endif
  //- test DevVarUShortArray
  if (test_num_array_cmd (dev_name, "DevVarUShortArray") == -1)
    return -1
  endif
  //- test DevVarULongArray
  if (test_num_array_cmd (dev_name, "DevVarULongArray") == -1)
    return -1
  endif
  //- test DevVarFloatArray
  if (test_num_array_cmd (dev_name, "DevVarFloatArray") == -1)
    //- ignore numeric error - numeric rounding (double to float) generates 
    //- a side effect when comparing argin.num_wave to argout.num_wave 
    if (tango_error() != 0)
      //- error does comes from TANGO: abort test, continue otherwise
      return -1
    endif
  endif 
  //- test DevVarDoubleArray
  if (test_num_array_cmd (dev_name, "DevVarDoubleArray") == -1)
    return -1
  endif 

  //- ok numeric all numeric array commands are tested and seems to work properly
  //- now, let's try with an array of strings argin...

  //-------------------------------------------------------------------------------------------
  // DEVVAR_STRINGARRAY/DEVVAR_STRINGARRAY COMMAND  
  //-------------------------------------------------------------------------------------------
  //- an example of a command where argin and argout are both an array of strings. We still 
  //- use <tango_cmd_inout> 
  
  //- populate argin: <cmd> struct member
  //- name of the command to be executed on <argin.dev> 
  argin.cmd = "DevVarStringArray"
  
  //- verbose
  print "executing <" + argin.cmd + ">\r"
  
  //- build the argin wave - be aware that it must be of the expected type
  //- note the /T option - it tells Make taht we want to create a text wave (i.e. array of strings) 
  Make /O /N=128 /T argin_str_wave = "hello world"
  
  //- here again, we do not actually pass the wave to <tango_cmd_inout> - what is required is the 
  //- full path to the argin wave. in fact we need to provide the TANGO API with the location of 
  //- the argin wave (i.e. the datafolder in which the wave is located). since argin is a text 
  //- wave, its path is stored into the <str_wave_path> of the <CmdArgIn>. 
  argin.str_wave_path = tools_wave_path(argin_str_wave)
  
  //- actual cmd execution
  //- if an error occurs during command execution, argout is undefined (null or empty members)
  //- ALWAYS CHECK THE CMD RESULT BEFORE TRYING TO ACCESS ARGOUT: 0 means NO_ERROR, -1 means ERROR
  if (tango_cmd_inout(argin, argout) == -1)
    //- the cmd failed, display error...
    tango_display_error()
    //- ... then return error
    return -1
  endif
  
  //- <argout> is populated (i.e. filled) by <tango_cmd_inout> uppon return of the command.
  //- since the command ouput arg is an array of strings, it is stored in the <str_wave> member 
  //- of the <CmdArgOut> structure. 
  //- as previously explained, we are testing our TANGO binding on a TangoTest device. 
  //- consequently, we check that <argin = argout> in order to be sure that everything is ok
  //- we first need to obtain a reference to argout.str_wave ...
  WAVE/T argout_str_wave = argout.str_wave
  //- ... then we use the Igor's <x> operator in order for the comparaison. just magic!
  if (cmpstr(argin_str_wave[x], argout_str_wave[x]) != 0)
    //- the cmd failed, display error...
    tango_display_error_str("ERROR:DevVarStringArray:unexpected cmd result - aborting test")
    //- ... then return error
    return -1
  endif
  
  //- verbose
  print argout
  print "<" + argin.cmd + "> cmd passed\r"
  
  //- cleanup (suppose that waves are no longer needed)
  KillWaves/Z argin_str_wave, argout_str_wave  
  
  //- now, let's try with the TANGO composite arrays: DevVar[Long and Double]StringArray
  
  //-------------------------------------------------------------------------------------------
  // DEVVAR_DOUBLESTRINGARRAY/DEVVAR_DOUBLESTRINGARRAY
  //-------------------------------------------------------------------------------------------
  //- an example of a command where argin and argout are both an DEVVAR_DOUBLESTRINGARRAY (i.e.
  //- a TANGO composite type containing an array of double and an array of strings).The TANGO API
  //- function <tango_cmd_inout still do the job. this example is just a mix of DevVarDoubleArray 
  //- and the DevVarStringArray examples. here we use both the numeric and the text part of both 
  //- <CmdArgIn> and <CmdArgOut>. 
  
  //- populate argin: <cmd> struct member
  //- name of the command to be executed on <argin.dev> 
  argin.cmd = "DevVarDoubleStringArray"
  
  //- verbose
  print "executing <" + argin.cmd + ">\r"
  
  //- get wave type for the numeric part of argin
  wave_type = tango_argin_type_to_wave_type(argin.dev, argin.cmd)
  
  //- build the numeric part of argin - the /Y option is used to specify the right data type
  Make /O /N=128 /Y=(wave_type) argin_num_wave = enoise(1024)
  
  //- build the text part of argin - note the /T for text wave 
  Make /O /N=256 /T argin_str_wave = "hello world"
  
  //- did you note that the num and str waves may not have the same number of points ?
   
  //- set paths to both part of argin
  argin.num_wave_path = tools_wave_path(argin_num_wave)
  argin.str_wave_path = tools_wave_path(argin_str_wave)
  
  //- actual cmd execution
  //- if an error occurs during command execution, argout is undefined (null or empty members)
  //- ALWAYS CHECK THE CMD RESULT BEFORE TRYING TO ACCESS ARGOUT: 0 means NO_ERROR, -1 means ERROR
  if (tango_cmd_inout(argin, argout) == -1)
    //- the cmd failed, display error...
    tango_display_error()
    //- ... then return error
    return -1
  endif
  
  //- Tangotest specific ckecking...
  
  //- first check result for the txt part...
  WAVE/T argout_str_wave = argout.str_wave
  //- ... then we use the Igor's <x> operator in order for the comparaison. just magic!
  if (cmpstr(argin_str_wave[x], argout_str_wave[x]) != 0)
    //- the cmd failed, display error...
    tango_display_error_str("ERROR:DevVarDoubleStringArray:unexpected cmd result on text part - aborting test")
    //- ... then return error
    return -1
  endif
  
  //- then check result for the numeric part...
  WAVE argout_num_wave = argout.num_wave
  //- ... then we use the Igor's <x> operator in order for the comparaison. just magic!
  if (argin_num_wave[x] != argout_num_wave[x])
    //- the cmd failed, display error...
    tango_display_error_str("ERROR:DevVarDoubleStringArray:unexpected cmd result on numeric part - aborting test")
    //- ... then return error
    return -1
  endif
  
  //- verbose
  print argout
  print "<" + argin.cmd + "> cmd passed\r"
  
  //- cleanup (suppose that waves no are longer needed)
  KillWaves/Z argin_num_wave, argout_num_wave
  KillWaves/Z argin_str_wave, argout_str_wave
  
  //-------------------------------------------------------------------------------------------
  // DEVVAR_LONGSTRINGARRAY/DEVVAR_LONGSTRINGARRAY
  //-------------------------------------------------------------------------------------------
  //- an example of a command where argin and argout are both an DEVVAR_DOUBLESTRINGARRAY (i.e.
  //- a TANGO composite type containing an array of double and an array of strings).The TANGO API
  //- function <tango_cmd_inout still do the job. this example is just a mix of DevVarDoubleArray 
  //- and the DevVarStringArray examples. here we use both the numeric and the text part of both 
  //- <CmdArgIn> and <CmdArgOut>. 
  
  //- populate argin: <cmd> struct member
  //- name of the command to be executed on <argin.dev> 
  argin.cmd = "DevVarLongStringArray"
  
  //- verbose
  print "executing <" + argin.cmd + ">\r"
  
  //- get wave type for the numeric part of argin
  wave_type = tango_argin_type_to_wave_type(argin.dev, argin.cmd)
  
  //- build the numeric part of argin - the /Y option is used to specify the right data type
  Make /O /N=128 /Y=(wave_type) argin_num_wave = enoise(1024)
  
  //- build the text part of argin - note the /T for text wave 
  Make /O /N=256 /T argin_str_wave = "hello world"
  
  //- did you note that the num and str waves may not have the same number of points ?
   
  //- set paths to both part of argin
  argin.num_wave_path = tools_wave_path(argin_num_wave)
  argin.str_wave_path = tools_wave_path(argin_str_wave)
  
  //- actual cmd execution
  //- if an error occurs during command execution, argout is undefined (null or empty members)
  //- ALWAYS CHECK THE CMD RESULT BEFORE TRYING TO ACCESS ARGOUT: 0 means NO_ERROR, -1 means ERROR
  if (tango_cmd_inout(argin, argout) == -1)
    //- the cmd failed, display error...
    tango_display_error()
    //- ... then return error
    return -1
  endif
  
  //- Tangotest specific ckecking...
  
  //- first check result for the txt part...
  WAVE/T argout_str_wave = argout.str_wave
  //- ... then we use the Igor's <x> operator in order for the comparaison. just magic!
  if (cmpstr(argin_str_wave[x], argout_str_wave[x]) != 0)
    //- the cmd failed, display error...
    tango_display_error_str("ERROR:DevVarLongStringArray:unexpected cmd result on text part - aborting test")
    //- ... then return error
    return -1
  endif
  
  //- then check result for the numeric part...
  WAVE argout_num_wave = argout.num_wave
  //- ... then we use the Igor's <x> operator in order for the comparaison. just magic!
  if (argin_num_wave[x] != argout_num_wave[x])
    //- the cmd failed, display error...
    tango_display_error_str("ERROR:DevVarLongStringArray:unexpected cmd result on numeric part - aborting test")
    //- ... then return error
    return -1
  endif
  
  //- verbose
  print argout
  print "<" + argin.cmd + "> cmd passed\r"
  
  //- cleanup (suppose that waves no are longer needed)
  KillWaves/Z argin_num_wave, argout_num_wave
  KillWaves/Z argin_str_wave, argout_str_wave
    
  //- no error - great!
  print "<Tango-API::tango_cmd_io> : TEST PASSED\r"
  return 0
end
//==============================================================================
// TUTORIAL CHAPTER I : Executing a TANGO command
//------------------------------------------------------------------------------
// utility function - a generic function for testing TangoTest "num scalar" cmds
//==============================================================================
//- this function is called from test_tango_cmd_io
function test_num_scalar_cmd (dev, cmd, num_scalar_val)
  //- function args
  String dev
  String cmd
  Variable num_scalar_val
  //- local variables
  Struct CmdArgIn argin
  Struct CmdArgOut argout
  //- populate argin
  argin.dev = dev
  argin.cmd = cmd
  argin.var = num_scalar_val
  //- verbose
  print "executing " + cmd + "...\r"
  //- actual cmd execution
  //- if an error occurs during command execution, argout is undefined (null or empty members)
  //- ALWAYS CHECK THE CMD RESULT BEFORE TRYING TO ACCESS ARGOUT: 0 means NO_ERROR, -1 means ERROR
  if (tango_cmd_inout(argin, argout) == -1)
    //- the cmd failed, display error...
    tango_display_error()
    //- ... then return error
    return -1
  endif
  //- check that <argin = argout> in order to be sure that everything is ok
  if (argin.var - argout.var != 0)
    print "ERROR:" + cmd + ":unexpected cmd result or numeric rounding side effect [expected error for DevFloat]"
    print cmd + " failed!"
    //- ... then return error
    return -1
  endif
  print argout
  print "<" + argin.cmd + "> cmd passed\r"
  //- no error - great!
  return 0
end 
//==============================================================================
// TUTORIAL CHAPTER I : Executing a TANGO command
//------------------------------------------------------------------------------
// utility function - a generic function for testing TangoTest "num array" cmds
//==============================================================================
//- this function is called from test_tango_cmd_io
function test_num_array_cmd (dev, cmd)
  //- function args
  String dev
  String cmd
  //- local variables
  Struct CmdArgIn argin
  Struct CmdArgOut argout
  //- populate argin
  argin.dev = dev
  argin.cmd = cmd
  //- verbose
  print "executing " + cmd + "...\r"
  //- get wave type
  Variable wave_type = tango_argin_type_to_wave_type(dev, cmd)
  //- we can now build the argin wave   
  Make /O /N=128 /Y=(wave_type) argin_wave = enoise(1024)
  //- set path to argin wave
  argin.num_wave_path = tools_wave_path(argin_wave)
  //- actual cmd execution
  //- if an error occurs during command execution, argout is undefined (null or empty members)
  //- ALWAYS CHECK THE CMD RESULT BEFORE TRYING TO ACCESS ARGOUT: 0 means NO_ERROR, -1 means ERROR
  if (tango_cmd_inout(argin, argout) == -1)
    //- the cmd failed, display error...
    tango_display_error()
    //- ... then return error
    return -1
  endif
  //- check result
  WAVE argout_wave = argout.num_wave
  if (argin_wave[x] != argout_wave[x])
    print "ERROR:" + cmd + ":unexpected cmd result or numeric rounding side effect [expected error for DevVarFloatArray]"
    print cmd + " failed!"
    //- ... then return error
    return -1
  endif
  print argout
  print "<" + argin.cmd + "> cmd passed\r"
  //- cleanup - wave no longer needed
  KillWaves/Z argin_wave, argout_wave 
  //- no error - great!
  return 0
end

//==============================================================================
// TUTORIAL CHAPTER II : Reading one (or more) attribute(s) on a TANGO device
//==============================================================================
// The TANGO API defines and use the same structure for both reading and writing 
// an attribue: <AttributeValue>. This structue is defined in the <Tango.ipf> file. 
//
// <AttributeValue> has the following structure:
// ---------------------------------------------
//  Structure AttributeValue
//    String dev           //- device name
//    String attr          //- attribute name
//    int16 format         //- attribute format: kSCALAR, kSPECTRUM or kIMAGE
//    int16 type           //- attribute data type : kSTRING, kLONG, kDOUBLE, ... 
//    double ts            //- timestamp in seconds since "Igor's time reference"
//    String str_val       //- attribute value for string Scalar attributes
//    Variable var_val     //- attribute value for numric Scalar attributes
//    Wave wave_val        //- attribute value for any Spectrum or Image attribute 
//    String wave_val_path //- full path to <wave_val> (datafolder)
//  EndStructure
//
//  When reading an attribute you must specify the device and the attribute
//  name. Others <AttributeValue> structure members are not required and will
//  be filled by the reading function <tango_read_attr>.
//
//  Note about <AttributeValue.format>: 
//  -----------------------------------
//  The attribute format: kSCALAR, kSPECTRUM or kIMAGE.
//  Placed in <AttributeValue> in order to ease "generic" data processing. 
//  Not really usefull in 99% of the use cases. This struct member is filled 
//  by the TANGO API reading function <tango_read_attr>. 
//  
//  Note about <AttributeValue.type>: 
//  ---------------------------------
//  The attribute data type : kSTRING, kLONG, kDOUBLE, ...
//  Placed in <AttributeValue> in order to ease "generic" data processing. 
//  Not really usefull in 99% of the use cases. This struct member is filled 
//  by the TANGO API reading function <tango_read_attr>. 
//
//  Note about <AttributeValue.ts>: 
//  ----------------
//  The timestamp of the attribute. We use the Igor's <DateTime> format. See
//  Igor's DateTime documentation for more info. 
//
//  Note about <AttributeValue.str_val>: 
//  ------------------------------------
//  Attribute value for string <Scalar> attributes (i.e. for single string).
//  Valid if <format> = kSCALAR and <type> = kSTRING, undefined otherwise.
//  Also undefined in case of error during attribute reading - it means that 
//  you should **ALWAYS** check the error code returned by <tango_read_attr> 
//  before trying to access this string.
//
//  Note about <AttributeValue.var_val>: 
//  ------------------------------------
//  Attribute value for numeric <Scalar> attributes (i.e. for single numeric 
//  value). Valid if <format> = kSCALAR and <type> != kSTRING, undefined otherwise.
//  Also undefined in case of error during attribute reading - it means that 
//  you should always check the error code returned by <tango_read_attr> before 
//  trying to access this variable.
//
//  Note about <AttributeValue.wave_val>: 
//  -------------------------------------
//  Attribute value any <Spectrum> or <Image> attributes (i.e. any array, even arrays
//  of stings). The nature of <wave_val> depends on the attribute format:
//  if <format> = kSCALAR, undefined - <str_val> or <val_var> is used instead
//  If <format> = SPECTRUM, <wave_val> is a 1D wave of <type> 
//  If <format> = IMAGE, <wave_val> is 2D wave of <type> 
//  As you can see, <wave_val> is only valid for any data type if the attribute a either 
//  spectrum or an image. Also undefined in case of error during attribute reading - it 
//  means that you should **ALWAYS** check the error code returned by <tango_read_attr> 
//  before trying to access this wave. Be aware that this is a global object overwritten 
//  each time the attribute is read. This member is used when reading an attribute (set 
//  by the reading function). It can't be used for writing an attribute.    
//
//  Note about <AttributeValue.wave_val_path>: 
//  ------------------------------------------
//  Full path to <wave_val> - fully qualified path (from root:) to the datafolder into 
//  which <wave_val> is stored. This struct member is filled by the TANGO API reading 
//  function <tango_read_attr>. It will also be used for writing an attribute. 
//
// Reading several attributes in one call 
// --------------------------------------
// The TANGO API for Igor Pro provides a way to read several attributes on the SAME
// device in a single call. In such a cas, the function <tango_read_attrs> is used 
// (note the 's' in the function name). In order to achieve such a magical feature,
// we need an data structure capable of storing more than one attribute value. The
// <AttributeValues> structure has been introduced for this purpose (again, note 
// the 's' in the structure name). <AttributeValues> has the following members:
//
//  Structure AttributeValues
//    String dev                                //- the name of device
//    int16 nattrs                              //- actual the num of attributes to read
//    Strut AttributeValue vals[kMAX_NUM_ATTR]  //- an array of <AttributeValues>
//  EndStructure
//
//  Note about <AttributeValues.nattrs>: 
//  ------------------------------------
//  Actual the num of attributes to read - must be <= kMAX_NUM_ATTR. Should 
//  obviously equal the number of valid <AttributeValue>s you pass in the 
//  <vals> member. Its value must be <= kMAX_NUM_ATTR. This constant is defined
//  in the Tango.ipf file and set to 16. If you need to read more than 16 
//  attributes on the SAME device, edit the Tango.ipf file and change kMAX_NUM_ATTR
//  to the appropriate value. 
//
//  Note about <AttributeValues.vals>: 
//  ----------------------------------
//  An array of kMAX_NUM_ATTR <AttributeValue> is used to stored the attribute values.
//  Uppon return of the reading function <tango_read_attrs>, <AttributeValues.nattrs>
//  attribute values are actually valid in the array. In case of error during attributes 
//  reading, the content of <AttributeValues.vals> is undefined. That's why you should 
//  **ALWAYS** check the error code returned by <tango_read_attr> before trying to use 
//  the content of <AttributeValues.vals>.
//
// The following <test_tango_read_attr> function gives an example for each TANGO 
// attribute type. The usage of <AttributeValue> is detailed.
// 
// TangoTest has an attribute for each TANGO type. Each attribute is named with 
// the name of the associated type.    
//==============================================================================  
function test_tango_read_attr (dev_name)

  //- function arg: the name of the device on which the attributes will be read
  String dev_name
  
  //- verbose
  print "Starting <Tango-API::tango_read_attr> test...\r"
  
  //- let's declare our <AttributeValue> structure. 
  //- be aware that <av> will be overwritten (and reset) each time we read
  //- an attribute. it means that you must use another <AttributeValue> if case 
  //- you want to store more than one attribue value at a time. here we reuse 
  //- <av> for each attribute reading 
  Struct AttributeValue av
  
  //- populate attr_val: <dev> struct member
  //- the name of the device on which the attribute will be read
  //- NB: since the attributes will be read on the same device (i.e. dev_name), 
  //- we set the <AttributeValue.dev> struct member only once (i.e. no need to 
  //- set it not each time we execute a command)  
  av.dev = dev_name

  //- populate attr_val: <attr> struct member
  //- it's simply the name of the attribute to read
  av.attr = "short_scalar"
  
  //- no need to fill any other <AttributeValue> members in order to read an 
  //- attribute. so let's read it...
  if (tango_read_attr (av) == -1)
   //- could not read the attribute, display error..
   tango_display_error()
    //- ... then return error
    return -1
  endif
  
  //- let's play with the attribute value...
  //---------------------------------------- 
  print "\r"
  //- dump full attribute name
  print "\t-attr.........." + av.dev + "/" + av.attr    
  //- dump attribute format...
  switch (av.format)
    case kSCALAR:
      print "\t-attr format...SCALAR"
      break
    case kSPECTRUM:
      print "\t-attr format...SPECTRUM"
      break
    case kIMAGE:
      print "\t-attr format...IMAGE"
      break
  endswitch
  //- dump attribute type...
  switch (av.type)
    case kSTRING:
      print "\t-attr type.....STRING"
      break
    case kBOOL:
      print "\t-attr type.....BOOLEAN (i.e. NT_I8 integer)"
      break
    case kCHAR:
      print "\t-attr type.....CHAR (i.e. NT_I8 integer)"
      break
    case kUCHAR:
      print "\t-attr type.....UNSIGNED CHAR (i.e. NT_U8 integer)"
      break
    case kSHORT:
      print "\t-attr type.....SHORT (i.e. NT_I16 integer)"
      break
    case kUSHORT:
      print "\t-attr type.....UNSIGNED SHORT (i.e. NT_U16 integer)"
      break 
    case kLONG:
      print "\t-attr type.....LONG (i.e. NT_I32 integer)"
      break 
    case kLONG:
      print "\t-attr type.....UNSIGNED LONG (i.e. NT_UI32 integer)"
      break 
     case kFLOAT:
      print "\t-attr type.....FLOAT (i.e. NT_FP32 integer)"
      break   
     case kDOUBLE:
      print "\t-attr type.....DOUBLE (i.e. NT_FP64 integer)"
      break   
  endswitch
  //- dump attribute value timestamp...
  print "\t-timestamp....." + Secs2Date(av.ts,1) + " - " + Secs2Time(av.ts, 3, 2)
  //- dump actual value (this a numeric scalar attribute, its value is stored in av.var_val)
  print "\t-attr val......" + num2str(av.var_val)
  
  //- read long_scalar
  av.attr = "long_scalar"
  if (tango_read_attr (av) == -1)
    tango_display_error()
    return -1
  endif
  //- we printout <av>. observe that <av.var_val> is the only valid "value member" 
  //- in the "returned" struct. here the struct member <av.var_val> contains the 
  //- actual value of the <long_scalar> attribute. all others are undefined.
  dump_attribute_value (av)
  
  av.attr = "boolean_scalar"
  if (tango_read_attr (av) == -1)
    tango_display_error()
    return -1
  endif
  dump_attribute_value (av)
  
  av.attr = "uchar_scalar"
  if (tango_read_attr (av) == -1)
    tango_display_error()
    return -1
  endif
  dump_attribute_value (av)
  
  av.attr = "ushort_scalar"
  if (tango_read_attr (av) == -1)
    tango_display_error()
    return -1
  endif
  dump_attribute_value (av)
  
  av.attr = "double_scalar"
  if (tango_read_attr (av) == -1)
    tango_display_error()
    return -1
  endif
  dump_attribute_value (av)
  
  av.attr = "State"
  if (tango_read_attr (av) == -1)
    tango_display_error()
    return -1
  endif
  //- we printout <av>. observe that <av.var_val> is the only valid "value member" 
  //- in the "returned" struct. here the struct member <av.var_val> contains the 
  //- actual value of the <State> attribute. all others are undefined. The following
  //- state constants are defined in the <Tango.ipf> file:
  //  constant kDeviceStateON      = 0
  //  constant kDeviceStateOFF     = 1
  //  constant kDeviceStateCLOSE   = 2
  //  constant kDeviceStateOPEN    = 3
  //  constant kDeviceStateINSERT  = 4 
  //  constant kDeviceStateEXTRACT = 5
  //  constant kDeviceStateMOVING  = 6
  //  constant kDeviceStateSTANDBY = 7
  //  constant kDeviceStateFAULT   = 8
  //  constant kDeviceStateINIT    = 9
  //  constant kDeviceStateRUNNING = 10
  //  constant kDeviceStateALARM   = 11
  //  constant kDeviceStateDISABLE = 12
  //  constant kDeviceStateUNKNOWN = 13
  dump_attribute_value (av)
  
  //- now try with a string scalar attribute. 
  //- no change except that the value is returned in the <str_val> member of <AttributeValue>
  av.attr = "string_scalar"
  if (tango_read_attr (av) == -1)
    tango_display_error()
    return -1
  endif
  dump_attribute_value (av)
  
  av.attr = "Status"
  if (tango_read_attr (av) == -1)
    tango_display_error()
    return -1
  endif
  dump_attribute_value (av)
  
  //- do the same test with some SPECTRUM attributes. 
  //- here the value is returned in the <wave_val> member of the <AttributeValue> structure.
  av.attr = "boolean_spectrum"
  if (tango_read_attr (av) == -1)
    tango_display_error()
    return -1
  endif
  dump_attribute_value (av)

  av.attr = "uchar_spectrum"
  if (tango_read_attr (av) == -1)
    tango_display_error()
    return -1
  endif
  dump_attribute_value (av)
  
  av.attr = "ushort_spectrum"
  if (tango_read_attr (av) == -1)
    tango_display_error()
    return -1
  endif
  dump_attribute_value (av)
  
  av.attr = "short_spectrum"
  if (tango_read_attr (av) == -1)
    tango_display_error()
    return -1
  endif
  dump_attribute_value (av)
  
  av.attr = "long_spectrum"
  if (tango_read_attr (av) == -1)
    tango_display_error()
    return -1
  endif
  dump_attribute_value (av)
  
  av.attr = "float_spectrum"
  if (tango_read_attr (av) == -1)
    tango_display_error()
    return -1
  endif
  dump_attribute_value (av)
  
  av.attr = "double_spectrum"
  if (tango_read_attr (av) == -1)
    tango_display_error()
    return -1
  endif
  dump_attribute_value (av)  
  
  //- and finally with the IMAGE attributes
  //- here again the value is returned in the <wave_val> member of the <AttributeValue> structure
  av.attr = "boolean_image"
  if (tango_read_attr (av) == -1)
  
    tango_display_error()
    return -1
  endif
  dump_attribute_value (av)
  
  av.attr = "uchar_image"
  if (tango_read_attr (av) == -1)
  
    tango_display_error()
    return -1
  endif
  dump_attribute_value (av)

  av.attr = "ushort_image"
  if (tango_read_attr (av) == -1)
  
    tango_display_error()
    return -1
  endif
  dump_attribute_value (av)
  
  av.attr = "short_image"
  if (tango_read_attr (av) == -1)
  
    tango_display_error()
    return -1
  endif
  dump_attribute_value (av)
  
  av.attr = "long_image"
  if (tango_read_attr (av) == -1)
  
    tango_display_error()
    return -1
  endif
  dump_attribute_value (av)
  
  av.attr = "float_image"
  if (tango_read_attr (av) == -1)
  
    tango_display_error()
    return -1
  endif
  dump_attribute_value (av)
  
  av.attr = "double_image"
  if (tango_read_attr (av) == -1)
    tango_display_error()
    return -1
  endif
  dump_attribute_value (av)
  
  //- no error - great!
  print "<Tango-API::tango_read_attr> : TEST PASSED\r"
  return 0
end
//==============================================================================
// TUTORIAL CHAPTER II : Reading attribute(s) on a TANGO device
//------------------------------------------------------------------------------
// utility function - a generic function to printout an <AttributeValue>
//==============================================================================
//- this function is called from test_tango_read_attr
function dump_attribute_value (av)
  Struct AttributeValue &av
  
  //- uncomment the following line if you want to dump the struct in Igor's history
  //- print av 
  
//- let's play with the attribute value...
  //---------------------------------------- 
  print "\r"
  //- dump full attribute name
  print "\t-attr.........." + av.dev + "/" + av.attr    
  //- dump attribute format...
  switch (av.format)
    case kSCALAR:
      print "\t-attr format...SCALAR (mapped to an Igor's variable or string)"
      break
    case kSPECTRUM:
      print "\t-attr format...SPECTRUM (mapped to an Igor's 1D<wave)"
      break
    case kIMAGE:
      print "\t-attr format...IMAGE (mapped to an Igor's 2D wave)"
      break
  endswitch
  //- dump attribute type...
  switch (av.type)
    case kSTRING:
      print "\t-attr type.....STRING"
      break
    case kBOOL:
      print "\t-attr type.....BOOLEAN (i.e. NT_I8 integer)"
      break
    case kCHAR:
      print "\t-attr type.....CHAR (i.e. NT_I8 integer)"
      break
    case kUCHAR:
      print "\t-attr type.....UNSIGNED CHAR (i.e. NT_U8 integer)"
      break
    case kSHORT:
      print "\t-attr type.....SHORT (i.e. NT_I16 integer)"
      break
    case kUSHORT:
      print "\t-attr type.....UNSIGNED SHORT (i.e. NT_U16 integer)"
      break 
    case kLONG:
      print "\t-attr type.....LONG (i.e. NT_I32 integer)"
      break 
    case kLONG:
      print "\t-attr type.....UNSIGNED LONG (i.e. NT_UI32 integer)"
      break 
     case kFLOAT:
      print "\t-attr type.....FLOAT (i.e. NT_FP32 integer)"
      break   
     case kDOUBLE:
      print "\t-attr type.....DOUBLE (i.e. NT_FP64 integer)"
      break   
  endswitch
  //- dump attribute value timestamp...
  print "\t-timestamp....." + Secs2Date(av.ts,1) + " - " + Secs2Time(av.ts, 3, 2)
  //- dump actual value (this a numeric scalar attribute, its value is stored in av.var_val)
  String verbose_str = ""
  if (av.format == kSCALAR)
    if (av.type != kSTRING)
      verbose_str += num2str(av.var_val)
	 else
      verbose_str += "Its value was: " + av.str_val + "\r\t" 
    endif
  else
    Variable np_x  = Dimsize(av.wave_val, 0)
    Variable np_y  = Dimsize(av.wave_val, 1)
    print "\t-dims.........." + "[" + num2str(np_x) + "x" + num2str(np_y) + "]" 
    if (av.format == kSPECTRUM)
      if (av.type != kSTRING)
        WAVE num_wave = av.wave_val
        verbose_str += num2str(num_wave[0]) + "," +  num2str(num_wave[1]) + "," +  num2str(num_wave[2]) + ",...]" 
      else
        WAVE/T txt_wave = av.wave_val
        verbose_str += "[" + txt_wave[0] + "," + txt_wave[1] + "," +  txt_wave[2] + ",...]" 
      endif
    else
      if (av.type != kSTRING)
        WAVE num_wave = av.wave_val
        verbose_str += "[0][" + num2str(num_wave[0][0]) + "," +  num2str(num_wave[0][1]) + "," +  num2str(num_wave[0][2]) + ",...]" 
      else
        WAVE/T txt_wave = av.wave_val
        verbose_str += "[0][" + txt_wave[0][0] + "," + txt_wave[0][1] + "," +  txt_wave[0][2] + ",...]" 
      endif
    endif
  endif
  print "\t-attr val......" + verbose_str + "\r"
end
//==============================================================================
// test_tango_read_attrs
//==============================================================================
function test_tango_read_attrs (dev_name)
 //- function arg: the name of the device on which the attributes will be read
 String dev_name

 //- verbose
 print "Starting <Tango-API::tango_read_attrs> test...\r"
  
 Struct AttributeValues avs
 
 //- filling the <AttributeValues>: the "manual" way
 //-------------------------------------------------
 avs.nattrs = 4
 avs.dev = dev_name
 avs.vals[0].attr = "long_scalar"
 avs.vals[1].attr = "long_spectrum"
 avs.vals[2].attr = "State"
 avs.vals[3].attr = "string_scalar"
 if (tango_read_attrs (avs) == -1)
    tango_display_error()
    return -1
 endif
 Variable n
 for (n = 0; n < avs.nattrs; n += 1)
   dump_attribute_value(avs.vals[n])
 endfor
 
 //- filling the <AttributeValues>: the "list" approach
 //- this is usefull when writting a our own scan loop using the Igor's background task 
 //- in such a situation, it becomes necessary to maintain the list of attributes to 
 //- read in a dedicated string in which attributes names are separated by the ; character.
 //- since we can only read several attributes on the SAME device, one should maintain
 //- an attribute list per device. why maintaining such a list? couldn't we maintain
 //- AttributeValues struct per device instead. Unfortunatly, Igor structs can "live"
 //- outside the context of a function. It means that we can create a global struct (i.e.
 //- Sruct/G does not work). Consequently, we have to rebuild the reading structure each 
 //- time we want to read the attributes from the background task. Maintaining the device
 //- list will simply ease the process. Here is an example... 
 //------------------------------------------------------------------------------------
 
 //- first, create a global string... (this is normally done somewhere else)
 String/G root:attr_list_gstr = ""
 //- then get a local ref to it...
 SVAR/Z attr_list = root:attr_list_gstr
 //- fill our attribute list whith the dedicated function...
 attr_list = tools_str_list_add("short_scalar", attr_list)
 attr_list = tools_str_list_add("short_spectrum", attr_list)
 attr_list = tools_str_list_add("dummy", attr_list)
 attr_list = tools_str_list_add("short_spectrum", attr_list)
 //- oups, we intoduced a "dummy" attribute. let's remove it from the list   
 attr_list = tools_str_list_remove ("dummy", attr_list)
 
 //- now let's suppose that we only just enter the background task...
 
 //- declare the AttributeValues...
 Struct AttributeValues some_av
 //- ... then fill (i.e prepare) the struct using the maintained list and a 
 //- dedicated function ...
 tango_attr_list_to_attr_vals(dev_name, attr_list, some_av)
 //- actual reading 
 if (tango_read_attrs (some_av) == -1)
    tango_display_error()
    return -1
 endif
 
 //- dump reading results
 for (n = 0; n < some_av.nattrs; n += 1)
   dump_attribute_value(some_av.vals[n])
 endfor
 
 //- cleanup tmp objects (this is normally done somewhere else)
 KillStrings root:attr_list_gstr 
 
 //- verbose
 print "<Tango-API::tango_read_attrs> : TEST PASSED\r"
 return 0
end

//==============================================================================
// TUTORIAL CHAPTER III : Writting one (or more) attribute(s) on a TANGO device
//==============================================================================
// The TANGO API defines and use the same structure for both reading and writing 
// an attribue: <AttributeValue>. This structue is defined in the <Tango.ipf> file. 
//
// <AttributeValue> has the following structure:
// ---------------------------------------------
//  Structure AttributeValue
//    String dev           //- device name
//    String attr          //- attribute name
//    int16 format         //- attribute format: kSCALAR, kSPECTRUM or kIMAGE
//    int16 type           //- attribute data type : kSTRING, kLONG, kDOUBLE, ... 
//    double ts            //- timestamp in seconds since "Igor's time reference"
//    String str_val       //- attribute value for string Scalar attributes
//    Variable var_val     //- attribute value for numric Scalar attributes
//    Wave wave_val        //- attribute value for any Spectrum or Image attribute 
//    String wave_val_path //- full path to <wave_val> (datafolder)
//  EndStructure
//
//  When writting an attribute you must specify the device, the attribute
//  name and the value to write. Others <AttributeValue> structure members are 
//  not required.
//
//  Note about <AttributeValue.format>: 
//  -----------------------------------
//  Not used here. See previous read examples.
//  
//  Note about <AttributeValue.type>: 
//  -----------------------------------
//  Not used here. See previous read examples.
//
//  Note about <AttributeValue.ts>: 
//  -----------------------------------
//  Not used here. See previous read examples.
//
//  Note about <AttributeValue.str_val>: 
//  ------------------------------------
//  Attribute value for string <Scalar> attributes (i.e. for single string).
//
//  Note about <AttributeValue.var_val>: 
//  ------------------------------------
//  Attribute value for numeric <Scalar> attributes (i.e. for single numeric value). 
//
//  Note about <AttributeValue.wave_val>: 
//  -------------------------------------
//  Not used (and CAN'T be) used here. See previous read examples.  
//
//  Note about <AttributeValue.wave_val_path>: 
//  ------------------------------------------
//  Full path to <wave_val> - fully qualified path (from root:) to the datafolder into 
//  which <wave_val> is stored. This the path to the "wave value" for any <Spectrum> 
//  or <Image> attributes (i.e. any array, even arrays of stings). The nature of <wave_val> 
//  depends on the attribute format:
//  if <format> = kSCALAR, undefined - <str_val> or <val_var> is used instead
//  If <format> = SPECTRUM, <wave_val> is a 1D wave of <attribute type> 
//  If <format> = IMAGE, <wave_val> is 2D wave of <attribute type> 
//
// Writting several attributes in one call 
// ---------------------------------------
// The TANGO API for Igor Pro provides a way to writ several attributes on the SAME
// device in a single call. In such a cas, the function <tango_write_attrs> is used 
// (note the 's' in the function name). In order to achieve such a magical feature,
// we need an data structure capable of storing more than one attribute value. The
// <AttributeValues> structure has been introduced for this purpose (again, note 
// the 's' in the structure name). <AttributeValues> has the following members:
//
//  Structure AttributeValues
//    String dev                                //- the name of device
//    int16 nattrs                              //- actual the num of attributes to write
//    Strut AttributeValue vals[kMAX_NUM_ATTR]  //- an array of <AttributeValues>
//  EndStructure
//
//  Note about <AttributeValues.nattrs>: 
//  ------------------------------------
//  Actual the num of attributes to read - must be <= kMAX_NUM_ATTR. Should 
//  obviously equal the number of valid <AttributeValue>s you pass in the 
//  <vals> member. Its value must be <= kMAX_NUM_ATTR. This constant is defined
//  in the Tango.ipf file and set to 16. If you need to read more than 16 
//  attributes on the SAME device, edit the Tango.ipf file and change kMAX_NUM_ATTR
//  to the appropriate value. 
//
//  Note about <AttributeValues.vals>: 
//  ----------------------------------
//  An array of kMAX_NUM_ATTR <AttributeValue> is used to stored the attribute values.
//  <AttributeValues.nattrs> "values"  must be valid in the array when the <AttributeValues>
//  is passed to the <tango_write_attrs> function. 
//
// The following <test_tango_write_attr> function gives an example for some TANGO 
// attribute type. The usage of <AttributeValue> is detailed.
// 
// TangoTest has an attribute for each TANGO type. Each attribute is named with 
// the name of the associated type.    
//============================================================================== 
//==============================================================================
// test_tango_write_attr
//==============================================================================
function test_tango_write_attr (dev_name)
 //- function arg: the name of the device on which the attributes will be read
 String dev_name
 
 //- verbose
 print "Starting <Tango-API::test_tango_write_attr> test...\r"
 
 //- declare an AttributeValue struct 
 Struct AttributeValue av
 
 //- specify the target device name
 av.dev = dev_name
 
 //- let's write the numeric scalar attribute <short_scalar>...
 av.attr = "short_scalar"
 //- since its a numeric scalar, we store the value to write in the <var_val> 
 //- member of the AttributeValue struct. the TangoTest device is written so 
 //- that the <short_scalar> will take its "read value" in the range [0, last_write_value].
 //- here is the actual range will be [0, 100].
 av.var_val = 100
 //- apply the specified value...
 if (tango_write_attr(av) == -1)
    tango_display_error()
    return -1
 endif  
 
 //- now, let's write the string scalar attribute <string_scalar>...
 av.attr = "string_scalar"
 //- <string_scalar> is a single string attribute. in such a case, we store 
 //- the value to write in the <str_val> member of the AttributeValue struct
 av.str_val = "testing new write attr impl"
 //- apply the specified value... 
 if (tango_write_attr(av) == -1)
    tango_display_error()
    return -1
 endif 
 //- read back <string_scalar> 
 if (tango_read_attr (av) == -1)
    tango_display_error()
    return -1
 endif
 //- dump read result
 dump_attribute_value (av)

 //- test some writable spectrum and image attributes
 Variable err = 0
 err = test_tango_write_spec_or_img (dev_name, "boolean_spectrum_ro", "boolean_spectrum")
 if (err == -1)
   return -1
 endif
 err = test_tango_write_spec_or_img (dev_name, "uchar_spectrum_ro", "uchar_spectrum")
 if (err == -1)
   return -1
 endif
 err = test_tango_write_spec_or_img (dev_name, "short_spectrum_ro", "short_spectrum")
 if (err == -1)
   return -1
 endif
 err = test_tango_write_spec_or_img (dev_name, "ushort_spectrum_ro", "ushort_spectrum")
 if (err == -1)
   return -1
 endif
 err = test_tango_write_spec_or_img (dev_name, "long_spectrum_ro", "long_spectrum")
 if (err == -1)
   return -1
 endif
 err = test_tango_write_spec_or_img (dev_name, "float_spectrum_ro", "float_spectrum")
 if (err == -1)
   return -1
 endif
 err = test_tango_write_spec_or_img (dev_name, "double_spectrum_ro", "double_spectrum")
 if (err == -1)
   return -1
 endif
 err = test_tango_write_spec_or_img (dev_name, "boolean_image_ro", "boolean_image")
 if (err == -1)
   return -1
 endif
 err = test_tango_write_spec_or_img (dev_name, "uchar_image_ro", "uchar_image")
 if (err == -1)
   return -1
 endif
 err = test_tango_write_spec_or_img (dev_name, "short_image_ro", "short_image")
 if (err == -1)
   return -1
 endif
 err = test_tango_write_spec_or_img (dev_name, "ushort_image_ro", "ushort_image")
 if (err == -1)
   return -1
 endif
 err = test_tango_write_spec_or_img (dev_name, "long_image_ro", "long_image")
 if (err == -1)
   return -1
 endif
 err = test_tango_write_spec_or_img (dev_name, "float_image_ro", "float_image")
 if (err == -1)
   return -1
 endif
 err = test_tango_write_spec_or_img (dev_name, "double_image_ro", "double_image")
 if (err == -1)
   return -1
 endif
 
 //- no error - great!
 print "<Tango-API::tango_write_attr> : TEST PASSED\r"
 return 0 
end
//==============================================================================
// test_tango_write_spectrum_or_image
//==============================================================================
function test_tango_write_spec_or_img (dev_name, ro_attr, rw_attr)
  //- function arg: the name of the device on which the attributes will be read
  String dev_name
  String ro_attr
  String rw_attr
 
  Struct AttributeValue rd_av
  rd_av.dev = dev_name
  rd_av.attr = ro_attr
  if (tango_read_attr (rd_av) == -1)
    tango_display_error()
    return -1
  endif
	
  WAVE wt_wave = rd_av.wave_val
  
  Struct AttributeValue wt_av
  wt_av.dev = dev_name
  wt_av.attr = rw_attr
  wt_av.wave_val_path = rd_av.wave_val_path
  
  if (tango_write_attr (wt_av) == -1)
    tango_display_error()
    return -1
  endif

  rd_av.dev = dev_name
  rd_av.attr = ro_attr
  
  if (tango_read_attr (rd_av) == -1)
    tango_display_error()
    return -1
  endif
  
  WAVE rd_wave = rd_av.wave_val
  
  if (rd_wave[x] != wt_wave[x])
    tango_display_error_str("ERROR:unexpected result - waves should equal")
	 return -1
  endif
end
//==============================================================================
// test_tango_write_attrs
//==============================================================================
function test_tango_write_attrs (dev_name)
 //- function arg: the name of the device on which the attributes will be written
 String dev_name
 
 //- verbose
 print "Starting <Tango-API::tango_write_attrs> test...\r"
 
 Struct AttributeValues avs
 avs.nattrs = 2
 avs.dev = dev_name
 avs.vals[0].attr = "short_scalar"
 avs.vals[0].var_val = 1024
 avs.vals[1].attr = "string_scalar"
 avs.vals[1].str_val = "testing write attrs"
 if (tango_write_attrs (avs) == -1)
   tango_display_error()
   return -1
 endif
 if (tango_read_attrs (avs) == -1)
    tango_display_error()
    return -1
 endif
 Variable n
 for (n = 0; n < avs.nattrs; n += 1)
   dump_attribute_value(avs.vals[n])
 endfor
 
 //- no error - great!
 print "<Tango-API::tango_write_attrs> : TEST PASSED\r"
 return 0 
end

//==============================================================================
// tango_test_all
//==============================================================================
function tango_test_all (n_it)
  Variable n_it
  if (n_it < 0) 
    n_it = 1 
  endif
  String dev_name = "tango/tangotest/1"
  Variable i
  for (i = 0; i < n_it; i += 1)
	 if (test_tango_cmd_io(dev_name) == -1)
	    print "\r\t**** TANGO BINDING FOR IGOR PRO - TEST FAILED ****"
	    return -1
	 endif
	 if (test_tango_read_attr(dev_name) == -1)
	    print "\r\t**** TANGO BINDING FOR IGOR PRO - TEST FAILED ****"
	    return -1
	 endif
	 if (test_tango_read_attrs(dev_name) == -1)
	    print "\r\t**** TANGO BINDING FOR IGOR PRO - TEST FAILED ****"
	    return -1
	 endif
	 if (test_tango_write_attr(dev_name) == -1)
	    print "\r\t**** TANGO BINDING FOR IGOR PRO - TEST FAILED ****"
	    return -1
	 endif
	 if (test_tango_write_attrs(dev_name) == -1)
	    print "\r\t**** TANGO BINDING FOR IGOR PRO - TEST FAILED ****"
	    return -1
	 endif
  endfor
  print "\r\t**** TANGO BINDING FOR IGOR PRO - TEST PASSED ****"
end