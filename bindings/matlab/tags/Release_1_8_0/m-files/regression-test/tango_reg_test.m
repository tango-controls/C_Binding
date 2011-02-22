function result = tango_reg_test (inst_name, num_it)
%TANGO_REG_TEST Executes a TANGO binding regression test using the TangoTest device.
%
% Syntax:
% -------
%   success = tango_reg_test (inst_name, it)
%
% Argin: inst_name
% ----------------
% %   |- type: 1-by-n char array (string)
%   |- desc: the instance name of TangoTest device
%
% Argin: it
% ---------
%   |- type: 1-by-1 double array 
%   |- desc: the # of times you want to execute the test
%
% Argout: result
% --------------
%   |- type: 1-by-1 double array 
%   |- desc: -1 on error, 0 otherwise
%
print_title('* TANGO BINDING FOR MATLAB *');
for counter = 1:num_it 
 if (do_it(inst_name) == -1)
    print_title('* REG-TEST FAILED *  REG-TEST FAILED *');
    return;
 end
end
% if (test_groups(num_it) == -1)
%  print_title('* REG-TEST PASSED *  REG-TEST PASSED *'); 
%  return;
% end;
print_title('* REG-TEST PASSED *  REG-TEST PASSED *'); 
return;

%==================================================================================
% FUNCTION do_it: THE ACTUAL REGRESSION TEST 
%==================================================================================
function result = do_it (inst_name)
%==================================================================================
% DEVICE NAME (OUR OWN TANGOTEST INSTANCE NAME GOES HERE) 
%==================================================================================
dev_name = strcat('tango/tangotest/',inst_name);
%==================================================================================
disp(sprintf('- regression test on %s', dev_name));
%**********************************************************************************
%==================================================================================
%                                 MISC. FUNCTIONS
%==================================================================================
%**********************************************************************************
print_sub_title('TESTING MISC. FUNCTIONS');
%----------------------------------------------------------------------------------
disp('- testing tango_ping');
elapsed_usec = tango_ping(dev_name);
if (tango_error == -1) 
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp(sprintf('%s is up and ready [RTT: %f usec]\r', dev_name, elapsed_usec));
%----------------------------------------------------------------------------------
disp('- testing tango_admin_name');
adm_name = tango_admin_name(dev_name);
if (tango_error == -1) 
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp(sprintf('  `--> %s\r', adm_name));
%----------------------------------------------------------------------------------
disp('- testing tango_description');
desc = tango_description(dev_name);
if (tango_error == -1) 
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp(sprintf('  `--> %s\r', desc));
%----------------------------------------------------------------------------------
disp('- testing tango_status');
status = tango_status(dev_name);
if (tango_error == -1) 
  disp('  `--> ERROR');
  tango_print_error_stack;
  return;
end
disp(sprintf('  `--> %s\r', status));
%----------------------------------------------------------------------------------
disp('- testing tango_state');
state = tango_state(dev_name);
if (tango_error == -1) 
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp(sprintf('  `--> %s [%.0f]\r', state.name, state.value));
%----------------------------------------------------------------------------------
disp('- testing tango_info');
info = tango_info(dev_name);
if (tango_error == -1) 
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp(info);
%----------------------------------------------------------------------------------
disp('- testing tango_idl_version');
idl_ver = tango_idl_version(dev_name);
if (tango_error == -1) 
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp(sprintf('  `--> passed [%s supports TANGO IDL version %d]\r', dev_name, idl_ver));
%----------------------------------------------------------------------------------
disp('- testing tango_get_source');
src = tango_get_source(dev_name);
if (tango_error == -1) 
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp(sprintf('  `--> %s [%.0f]\r', src.name, src.value));
%----------------------------------------------------------------------------------
disp('- testing tango_set_source (as numeric value)');
tango_set_source(dev_name, mod(src.value + 1, 2));
if (tango_error == -1) 
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
%----------------------------------------------------------------------------------
disp('- testing tango_set_source (as string value)');
tango_set_source(dev_name, 'DevICe');
if (tango_error == -1) 
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
%----------------------------------------------------------------------------------
disp('- testing tango_set_source (as string value - error expected)');
tango_set_source(dev_name, 'DUMMY-SOURCE');
if (tango_error == 0) 
  disp('  `--> ERROR EXPECTED (tango_error should be -1)');
  result = -1; return;
end
%----------------------------------------------------------------------------------
disp('- testing tango_get_timeout');
tmo = tango_get_timeout(dev_name);
if (tango_error == -1) 
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp(sprintf('  `--> %.0f ms\r', tmo));
%----------------------------------------------------------------------------------
disp('- testing tango_set_timeout');
tango_set_timeout(dev_name, tmo + 1);
if (tango_error == -1) 
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
%----------------------------------------------------------------------------------
disp('-testing tango_black_box');
bb = tango_black_box(dev_name, 5);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
[m, n] = size(bb);
for i = 1:n
  disp(sprintf('\t%s\r', char(bb(i))));
end

%**********************************************************************************
%==================================================================================
%                                     COMMANDS
%==================================================================================
%**********************************************************************************
print_sub_title('TESTING COMMANDS AND COMMAND ORIENTED FUNCTIONS');
%==================================================================================
% DEVICE CMD QUERY
%==================================================================================
disp('-testing tango_command_query');
cmd_info = tango_command_query(dev_name, 'DevVarDoubleStringArray');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp(cmd_info);
%==================================================================================
% DEVICE CMD LIST QUERY
%==================================================================================
disp('-testing tango_command_list_query');
cmd_list = tango_command_list_query(dev_name);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
[m, n] = size(cmd_list);
for i = 1:n
  disp(cmd_list(i));
end
%==================================================================================
% TEST CMD: DevVoid
%==================================================================================
disp('-testing DevVoid command');
tango_command_inout(dev_name, 'DevVoid');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% TEST CMD: DevVarLongStringArray
%==================================================================================
disp('-testing DevVarLongStringArray command');
argin.lvalue = int32(0:1023);
argin.svalue = cell(1,256);
argin.svalue(1,1:256) = {'dev-var-long-string-array-test'};
argout = tango_command_inout(dev_name, 'DevVarLongStringArray', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
[argin_m, argin_n] = size(argin.lvalue);
[argout_m, argout_n] = size(argin.lvalue);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin.lvalue) ~= size(argout.lvalue)');
  result = -1; return;
end
if (argin.lvalue ~= argout.lvalue)
  disp('  `--> ERROR::argin.lvalue ~= argout.lvalue');
  result = -1; return;
end
[argin_m, argin_n] = size(argin.svalue);
[argout_m, argout_n] = size(argin.svalue);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin.svalue) ~= size(argout.svalue)');
  result = -1; return;
end
diff = 0;
for i = 1:argin_n
  argin_str = argin.svalue{1,i};
  argout_str = argout.svalue{1,i};
  if (argin_str ~= argout_str)
    diff = diff + 1;
  end
end
if (diff ~= 0)
  disp('  `--> ERROR::argin.svalue ~= argout.svalue');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% TEST CMD: DevVarDoubleStringArray
%==================================================================================
disp('-testing DevVarDoubleStringArray command');
argin.dvalue = 1024:2047;
argin.svalue = cell(1,256);
argin.svalue(1,1:256) = {'dev-var-double-string-array-test'};
argout = tango_command_inout(dev_name, 'DevVarDoubleStringArray', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
[argin_m, argin_n] = size(argin.dvalue);
[argout_m, argout_n] = size(argin.dvalue);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin.dvalue) ~= size(argout.dvalue)');
  result = -1; return;
end
if (argin.dvalue ~= argout.dvalue)
  disp('  `--> ERROR::argin.dvalue ~= argout.dvalue');
  result = -1; return;
end
[argin_m, argin_n] = size(argin.svalue);
[argout_m, argout_n] = size(argin.svalue);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin.svalue) ~= size(argout.svalue)');
  result = -1; return;
end
diff = 0;
for i = 1:argin_n
  argin_str = argin.svalue{1,i};
  argout_str = argout.svalue{1,i};
  if (argin_str ~= argout_str)
    diff = diff + 1;
  end
end
if (diff ~= 0)
  disp('  `--> ERROR::argin.svalue ~= argout.svalue');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% TEST CMD: DevVarStringArray
%==================================================================================
disp('-testing DevVarStringArray command');
argin = cell(1,256);
argin(1,1:256) = {'dev-var-string-array-test'};
argout = tango_command_inout(dev_name, 'DevVarStringArray', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
diff = 0;
for i = 1:argin_n
  if (argin{i} ~= argout{i})
    diff = diff + 1;
  end
end
if (diff ~= 0)
  disp('  `--> ERROR::argin ~= argout');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% TEST CMD: DevVarDoubleArray
%==================================================================================
disp('-testing DevVarDoubleArray command');
argin = 0:0.1:128;
argout = tango_command_inout(dev_name, 'DevVarDoubleArray', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% TEST CMD: DevVarFloatArray
%==================================================================================
disp('-testing DevVarFloatArray command');
argin = single(-128:0.1:0);
argout = tango_command_inout(dev_name, 'DevVarFloatArray', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% TEST CMD: DevVarULongArray
%==================================================================================
disp('-testing DevVarULongArray command');
argin = uint32(0:256);
argout = tango_command_inout(dev_name, 'DevVarULongArray', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% TEST CMD: DevVarLongArray
%==================================================================================
disp('-testing DevVarLongArray command');
argin = int32(-1024:0);
argout = tango_command_inout(dev_name, 'DevVarLongArray', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% TEST CMD: DevVarUShortArray
%==================================================================================
disp('-testing DevVarUShortArray command');
argin = uint16(0:1024);
argout = tango_command_inout(dev_name, 'DevVarUShortArray', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% TEST CMD: DevVarShortArray
%==================================================================================
disp('-testing DevVarShortArray command');
argin = int16(-1024:0);
argout = tango_command_inout(dev_name, 'DevVarShortArray', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% TEST CMD: DevVarCharArray
%==================================================================================
disp('-testing DevVarCharArray command');
argin = uint8(0:256);
argout = tango_command_inout(dev_name, 'DevVarCharArray', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% TEST CMD: DevString
%==================================================================================
disp('-testing DevString command');
argin = 'dev-string';
argout = tango_command_inout(dev_name, 'DevString', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% TEST CMD: DevBoolean
%==================================================================================
disp('-testing DevBoolean command');
argin = uint8(1);
argout = tango_command_inout(dev_name, 'DevBoolean', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% TEST CMD: DevUShort
%==================================================================================
disp('-testing DevUShort command');
argin = uint16(2^16 - 1);
argout = tango_command_inout(dev_name, 'DevUShort', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% TEST CMD: DevShort
%==================================================================================
disp('-testing DevShort command');
argin = int16(-2^16);
argout = tango_command_inout(dev_name, 'DevShort', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% TEST CMD: DevULong
%==================================================================================
disp('-testing DevULong command');
argin = uint32(2^32 -1);
argout = tango_command_inout(dev_name, 'DevULong', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% TEST CMD: DevLong
%==================================================================================
disp('-testing DevLong command');
argin = int32(-2^32);
argout = tango_command_inout(dev_name, 'DevLong', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% TEST CMD: DevFloat
%==================================================================================
disp('-testing DevFloat command');
argin = single(-pi);
argout = tango_command_inout(dev_name, 'DevFloat', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% TEST CMD: DevDouble
%==================================================================================
disp('-testing DevDouble command');
argin = pi;
argout = tango_command_inout(dev_name, 'DevDouble', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  result = -1; return;
end
disp('  `--> passed');

%**********************************************************************************
%==================================================================================
%                                    ASYNCH. COMMANDS
%==================================================================================
%**********************************************************************************
print_sub_title('TESTING ASYNCHRONOUS COMMANDS');
%==================================================================================
% TEST CMD: DevVarLongStringArray
%==================================================================================
disp('-testing asynch DevVarLongStringArray command');
dvla_argin.lvalue = int32(0:1023);
dvla_argin.svalue = cell(1,256);
dvla_argin.svalue(1,1:256) = {'dev-var-long-string-array-test'};
req_desc = tango_command_inout_asynch(dev_name, 'DevVarLongStringArray', 0, dvla_argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
pause(0.1);
dvla_argout = tango_command_inout_reply(req_desc, 100);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
[argin_m, argin_n] = size(dvla_argin);
[argout_m, argout_n] = size(dvla_argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
[argin_m, argin_n] = size(dvla_argin.lvalue);
[argout_m, argout_n] = size(dvla_argin.lvalue);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin.lvalue) ~= size(argout.lvalue)');
  result = -1; return;
end
if (dvla_argin.lvalue ~= dvla_argout.lvalue)
  disp('  `--> ERROR::argin.lvalue ~= argout.lvalue');
  result = -1; return;
end
[argin_m, argin_n] = size(dvla_argin.svalue);
[argout_m, argout_n] = size(dvla_argin.svalue);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin.svalue) ~= size(argout.svalue)');
  result = -1; return;
end
diff = 0;
for i = 1:argin_n
  argin_str = dvla_argin.svalue{1,i};
  argout_str = dvla_argout.svalue{1,i};
  if (argin_str ~= argout_str)
    diff = diff + 1;
  end
end
if (diff ~= 0)
  disp('  `--> ERROR::argin.svalue ~= argout.svalue');
  result = -1; return;
end
disp('  `--> passed');
%**********************************************************************************
%==================================================================================
%                                 ATTRIBUTES
%==================================================================================
%**********************************************************************************
print_sub_title('TESTING ATTRIBUTE ORIENTED FUNCTIONS');
%**********************************************************************************
% DEVICE ATTRIBUTES - GET ATTRIBUTE LIST
%**********************************************************************************
disp('-testing tango_get_attribute_list');
attr_list = tango_get_attribute_list(dev_name);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
[m, n] = size(attr_list);
for i = 1:n
  disp(attr_list(i));
end
%**********************************************************************************
% DEVICE ATTRIBUTES - READ ATTRIBUTE CONFIG
%**********************************************************************************
disp('-testing tango_get_attribute_config (get config. for one attribute)');
attr_config = tango_get_attribute_config(dev_name, 'short_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp(attr_config);
%**********************************************************************************
% DEVICE ATTRIBUTES - READ ATTRIBUTES CONFIG
%**********************************************************************************
disp('-testing tango_get_attributes_config (get config. for several attributes)');
attr_list = {'short_scalar', 'long_scalar', 'double_scalar', ...
             'short_spectrum', 'long_spectrum', 'double_spectrum', ...
             'short_image', 'long_image', 'double_image'};
attr_config = tango_get_attributes_config(dev_name, attr_list);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
[m, n] = size(attr_config);
for i = 1:n
  disp(attr_config(i));
end
%**********************************************************************************
% DEVICE ATTRIBUTES - WRITE ATTRIBUTE CONFIG
%**********************************************************************************
disp('-testing tango_set_attribute_config');
attr_config = tango_get_attribute_config(dev_name, 'short_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> original <short_scalar> config');
disp(attr_config);
%change some values
attr_config.description = 'a simple short scalar attribute';
attr_config.label = 'sc';
attr_config.unit = 'A.U.';
attr_config.standard_unit = attr_config(1).unit;
attr_config.display_unit = attr_config(1).unit;
attr_config.format = 'none';
attr_config.min_value = '-16384';
attr_config.max_value = '16384';
tango_set_attributes_config(dev_name, attr_config);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return; 
end
disp('  `--> passed');
disp('  `--> modified <short_scalar> config');
attr_config = tango_get_attribute_config(dev_name, 'short_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp(attr_config);
%**********************************************************************************
% DEVICE ATTRIBUTES - READ/WRITE INDIVIDUALLY EACH ATTRIBUTE
%**********************************************************************************
print_sub_title('READING/WRITTING ATTRIBUTES (INDIVIDUALLY)');
%==================================================================================
% READ/WRITE ATTRIBUTE: string_scalar (read/write)
%==================================================================================
disp('-writing string_scalar');
str_in = 'a1:b2:c3:d4';
tango_write_attribute(dev_name, 'string_scalar', str_in );
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp(sprintf('  `--> passed (wrote %s)', str_in));
disp('-reading string_scalar');
str_out = tango_read_attribute(dev_name, 'string_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp(str_out);
if (strcmp(str_in, str_out.value) == 0) 
  disp('  `--> ERROR');
  result = -1; return;
end
%==================================================================================
% READ/WRITE ATTRIBUTE: boolean_scalar (read/write)
%==================================================================================
disp('-writing boolean_scalar');
val_in = uint8(1);
tango_write_attribute(dev_name, 'boolean_scalar', val_in);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp(sprintf('  `--> passed (wrote %d)', double(val_in)));
disp('-reading boolean_scalar');
val_out = tango_read_attribute(dev_name, 'boolean_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp(val_out);
if (val_in(1) ~= val_out.value(2)) 
  disp('  `--> ERROR');
  result = -1; return;
end
%==================================================================================
% READ/WRITE ATTRIBUTE: uchar_scalar (read/write)
%==================================================================================
disp('-writing uchar_scalar');
val_in = uint8(1);
tango_write_attribute(dev_name, 'uchar_scalar', val_in);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp(sprintf('  `--> passed (wrote %d)', double(val_in)));
disp('-reading uchar_scalar');
val_out = tango_read_attribute(dev_name, 'uchar_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp(val_out);
if (val_in(1) ~= val_out.value(2)) 
  disp('  `--> ERROR');
  result = -1; return;
end
%==================================================================================
% READ/WRITE ATTRIBUTE: short_scalar (read/write)
%==================================================================================
disp('-writing short_scalar');
val_in = int16(-4096);
tango_write_attribute(dev_name, 'short_scalar', val_in);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp(sprintf('  `--> passed (wrote %d)', double(val_in)));
disp('-reading short_scalar');
val_out = tango_read_attribute(dev_name, 'short_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp(val_out);
if (val_in(1) ~= val_out.value(2)) 
  disp('  `--> ERROR');
  result = -1; return;
end
%==================================================================================
% READ/WRITE ATTRIBUTE: ushort_scalar (read/write)
%==================================================================================
disp('-writing ushort_scalar');
val_in = uint16(-4096);
tango_write_attribute(dev_name, 'ushort_scalar', val_in);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp(sprintf('  `--> passed (wrote %d)', double(val_in)));
disp('-reading ushort_scalar');
val_out = tango_read_attribute(dev_name, 'ushort_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp(val_out);
if (val_in(1) ~= val_out.value(2)) 
  disp('  `--> ERROR');
  result = -1; return;
end
%==================================================================================
% READ/WRITE ATTRIBUTE: long_scalar (read/write)
%==================================================================================
disp('-writing long_scalar');
val_in = int32(8192);
tango_write_attribute(dev_name, 'long_scalar', val_in);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp(sprintf('  `--> passed (wrote %d)', double(val_in)));
disp('-reading long_scalar');
val_out = tango_read_attribute(dev_name, 'long_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp(val_out);
if (val_in(1) ~= val_out.value(2)) 
  disp('  `--> ERROR');
  result = -1; return;
end
%==================================================================================
% READ/WRITE ATTRIBUTE: float_scalar (read/write)
%==================================================================================
disp('-writing float_scalar');
val_in = single(pi);
tango_write_attribute(dev_name, 'float_scalar', val_in);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp(sprintf('  `--> passed (wrote %d)', val_in));
disp('-reading float_scalar');
val_out = tango_read_attribute(dev_name, 'float_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp(val_out);
if (val_in(1) ~= val_out.value(2)) 
  disp('  `--> ERROR');
  result = -1; return;
end
%==================================================================================
% READ/WRITE ATTRIBUTE: double_scalar (read/write)
%==================================================================================
disp('-writing double_scalar');
val_in = pi;
tango_write_attribute(dev_name, 'double_scalar', pi);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp(sprintf('  `--> passed (wrote %d)', val_in));
disp('-reading double_scalar');
val_out = tango_read_attribute(dev_name, 'double_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp(val_out);
if (val_in(1) ~= val_out.value(2)) 
  disp('  `--> ERROR');
  result = -1; return;
end
%==================================================================================
% READ ATTRIBUTE: boolean_spectrum (read/write)
%==================================================================================
disp('-reading boolean_spectrum_ro');
val_in = tango_read_attribute(dev_name, 'boolean_spectrum_ro');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-writing boolean_spectrum');
tango_write_attribute(dev_name, 'boolean_spectrum', val_in.value);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-reading boolean_spectrum');
val_out = tango_read_attribute(dev_name, 'boolean_spectrum');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-comparing read/write');
if (val_in.value ~= val_out.value) 
  disp('  `--> ERROR');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% READ ATTRIBUTE: uchar_spectrum (read/write)
%==================================================================================
disp('-reading uchar_spectrum_ro');
val_in = tango_read_attribute(dev_name, 'uchar_spectrum_ro');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-writing uchar_spectrum');
tango_write_attribute(dev_name, 'uchar_spectrum', val_in.value);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; 
  %return;
end
disp('  `--> passed');
disp('-reading uchar_spectrum');
val_out = tango_read_attribute(dev_name, 'uchar_spectrum');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-comparing read/write');
if (val_in.value ~= val_out.value) 
  disp('  `--> ERROR');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% READ ATTRIBUTE: short_spectrum (read/write)
%==================================================================================
disp('-reading short_spectrum_ro');
val_in = tango_read_attribute(dev_name, 'short_spectrum_ro');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-writing short_spectrum');
tango_write_attribute(dev_name, 'short_spectrum', val_in.value);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-reading short_spectrum');
val_out = tango_read_attribute(dev_name, 'short_spectrum');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-comparing read/write');
if (val_in.value ~= val_out.value) 
  disp('  `--> ERROR');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% READ ATTRIBUTE: ushort_spectrum (read/write)
%==================================================================================
disp('-reading ushort_spectrum_ro');
val_in = tango_read_attribute(dev_name, 'ushort_spectrum_ro');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-writing ushort_spectrum');
tango_write_attribute(dev_name, 'ushort_spectrum', val_in.value);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-reading ushort_spectrum');
val_out = tango_read_attribute(dev_name, 'ushort_spectrum');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-comparing read/write');
if (val_in.value ~= val_out.value) 
  disp('  `--> ERROR');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% READ ATTRIBUTE: long_spectrum (read/write)
%==================================================================================
disp('-reading long_spectrum_ro');
val_in = tango_read_attribute(dev_name, 'long_spectrum_ro');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-writing long_spectrum');
tango_write_attribute(dev_name, 'long_spectrum', val_in.value);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-reading long_spectrum');
val_out = tango_read_attribute(dev_name, 'long_spectrum');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-comparing read/write');
if (val_in.value ~= val_out.value) 
  disp('  `--> ERROR');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% READ ATTRIBUTE: float_spectrum (read/write)
%==================================================================================
disp('-reading float_spectrum_ro');
val_in = tango_read_attribute(dev_name, 'float_spectrum_ro');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-writing float_spectrum');
tango_write_attribute(dev_name, 'float_spectrum', val_in.value);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-reading float_spectrum');
val_out = tango_read_attribute(dev_name, 'float_spectrum');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-comparing read/write');
if (val_in.value ~= val_out.value) 
  disp('  `--> ERROR');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% READ ATTRIBUTE: double_spectrum (read/write)
%==================================================================================
disp('-reading float_spectrum_ro');
val_in = tango_read_attribute(dev_name, 'double_spectrum_ro');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-writing double_spectrum');
tango_write_attribute(dev_name, 'double_spectrum', val_in.value);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-reading double_spectrum');
val_out = tango_read_attribute(dev_name, 'double_spectrum');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-comparing read/write');
if (val_in.value ~= val_out.value) 
  disp('  `--> ERROR');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% READ ATTRIBUTE: boolean_image (read/write)
%==================================================================================
disp('-reading boolean_image_ro');
val_in = tango_read_attribute(dev_name, 'boolean_image_ro');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-writing boolean_image');
tango_write_attribute(dev_name, 'boolean_image', val_in.value);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-reading boolean_image');
val_out = tango_read_attribute(dev_name, 'boolean_image');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-comparing read/write');
if (val_in.value ~= val_out.value) 
  disp('  `--> ERROR');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% READ ATTRIBUTE: uchar_image (read/write)
%==================================================================================
disp('-reading uchar_image_ro');
val_in = tango_read_attribute(dev_name, 'uchar_image_ro');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-writing uchar_image');
tango_write_attribute(dev_name, 'uchar_image', val_in.value);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-reading uchar_image');
val_out = tango_read_attribute(dev_name, 'uchar_image');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-comparing read/write');
if (val_in.value ~= val_out.value) 
  disp('  `--> ERROR');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% READ ATTRIBUTE: short_image (read/write)
%==================================================================================
% disp('-reading short_image_ro');
% val_in = tango_read_attribute(dev_name, 'short_image_ro');
% if (tango_error == -1)
%   disp('  `--> ERROR');
%   tango_print_error_stack;
%   result = -1; return;
% end
% disp('  `--> passed');
% disp('-writing short_image');
% tango_write_attribute(dev_name, 'short_image', val_in.value);
% if (tango_error == -1)
%   disp('  `--> ERROR');
%   tango_print_error_stack;
%   result = -1; return;
% end
% disp('  `--> passed');
% disp('-reading short_image');
% val_out = tango_read_attribute(dev_name, 'short_image');
% if (tango_error == -1)
%   disp('  `--> ERROR');
%   tango_print_error_stack;
%   result = -1; return;
% end
% disp('  `--> passed');
% disp('-comparing read/write');
% if (val_in.value ~= val_out.value) 
%   disp('  `--> ERROR');
%   result = -1; return;
% end
% disp('  `--> passed');
%==================================================================================
% READ ATTRIBUTE: ushort_image (read/write)
%==================================================================================
disp('-reading ushort_image_ro');
val_in = tango_read_attribute(dev_name, 'ushort_image_ro');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-writing ushort_image');
tango_write_attribute(dev_name, 'ushort_image', val_in.value);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-reading ushort_image');
val_out = tango_read_attribute(dev_name, 'ushort_image');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-comparing read/write');
if (val_in.value ~= val_out.value) 
  disp('  `--> ERROR');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% READ ATTRIBUTE: long_image (read/write)
%==================================================================================
disp('-reading long_image_ro');
val_in = tango_read_attribute(dev_name, 'long_image_ro');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-writing long_image');
tango_write_attribute(dev_name, 'long_image', val_in.value);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-reading long_image');
val_out = tango_read_attribute(dev_name, 'long_image');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-comparing read/write');
if (val_in.value ~= val_out.value) 
  disp('  `--> ERROR');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% READ ATTRIBUTE: float_image (read/write)
%==================================================================================
disp('-reading float_image_ro');
val_in = tango_read_attribute(dev_name, 'float_image_ro');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-writing float_image');
tango_write_attribute(dev_name, 'float_image', val_in.value);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-reading float_image');
val_out = tango_read_attribute(dev_name, 'float_image');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-comparing read/write');
if (val_in.value ~= val_out.value) 
  disp('  `--> ERROR');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% READ ATTRIBUTE: double_image (read/write)
%==================================================================================
disp('-reading double_image_ro');
val_in = tango_read_attribute(dev_name, 'double_image_ro');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-writing double_image');
tango_write_attribute(dev_name, 'double_image', val_in.value);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-reading double_image');
val_out = tango_read_attribute(dev_name, 'double_image');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp('-comparing read/write');
if (val_in.value ~= val_out.value) 
  disp('  `--> ERROR');
  result = -1; return;
end
disp('  `--> passed');
%**********************************************************************************
% DEVICE ATTRIBUTES - READ/WRITE ALL ATTRIBUTES IN ONE CALL
%**********************************************************************************
print_sub_title('READING/WRITTING ATTRIBUTES (GLOBALLY)');
%==================================================================================
% WRITE ALL ATTRIBUTES
%==================================================================================
disp('-testing tango_write_attributes (writting several attributes in one call)');
attr_name_val_list(1).name  = 'short_scalar';
attr_name_val_list(1).value = int16(4096);
attr_name_val_list(2).name  = 'long_scalar';
attr_name_val_list(2).value = int32(8192);
attr_name_val_list(3).name  = 'double_scalar';
attr_name_val_list(3).value = -pi;
tango_write_attributes(dev_name, attr_name_val_list);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
else
  disp('  `--> passed');
end
%==================================================================================
% READ ALL ATTRIBUTES
%==================================================================================
disp('-testing tango_read_attributes (reading all attributes in one call)');
attr_list = {'short_scalar', 'long_scalar', 'double_scalar', ...
             'short_spectrum', 'long_spectrum', 'double_spectrum', ...
             'short_image', 'long_image', 'double_image'};
val = tango_read_attributes(dev_name, attr_list);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
[m, n] = size(val);
for i = 1:n
  disp(val(i));
end
%==================================================================================
% ASYNCH WRITE/READ ALL ATTRIBUTES
%==================================================================================
disp('-testing tango_write_attributes_asynch (asynch. writting all attributes in one call)');
attr_name_val_list(1).name  = 'short_scalar';
attr_name_val_list(1).value = int16(1024);
attr_name_val_list(2).name  = 'long_scalar';
attr_name_val_list(2).value = int32(4096);
attr_name_val_list(3).name  = 'double_scalar';
attr_name_val_list(3).value = 2 * pi;
req_desc = tango_write_attributes_asynch(dev_name, attr_name_val_list);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
else
  disp('  `--> tango_write_attributes_asynch passed');
end
tango_write_attributes_reply(req_desc, 0);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> tango_write_attributes_reply passed');
disp('-testing tango_read_attributes_asynch (asynch. reading all attributes in one call)');
req_desc = tango_read_attributes_asynch(dev_name, attr_list);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> tango_read_attributes_asynch passed');
pause(0.1);
val = tango_read_attributes_reply(req_desc, 0);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> tango_read_attributes_reply passed');
[m, n] = size(val);
for i = 1:n
  disp(val(i));
end
%==================================================================================
%                           ASYNCH ATTRIBUTE READ/WRITE
%==================================================================================
%==================================================================================
% ASYNCH WRITE/READ ATTRIBUTE: short_scalar
%==================================================================================
disp('-asynch. writing short_scalar');
val_in = int16(-1024);
req_desc = tango_write_attribute_asynch(dev_name, 'short_scalar', val_in);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp(sprintf('  `--> tango_write_attribute_asynch passed (wrote %d)', double(val_in)));
tango_write_attribute_reply(req_desc, 0);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> tango_write_attribute_reply passed');
disp('-asynch. reading short_scalar');
req_desc = tango_read_attribute_asynch(dev_name, 'short_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> tango_read_attribute_asynch passed');
val = tango_read_attribute_reply(req_desc, 0);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> tango_read_attribute_reply passed');
disp(val);
%==================================================================================
% ASYNCH READ ATTRIBUTE: short_spectrum
%==================================================================================
disp('-asynch. reading short_spectrum');
req_desc = tango_read_attribute_asynch(dev_name, 'short_spectrum');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> tango_read_attribute_asynch passed');
val = tango_read_attribute_reply(req_desc, 0);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> tango_read_attribute_reply passed');
disp(val);
%==================================================================================
% ASYNCH READ ATTRIBUTE: short_image
%==================================================================================
disp('-asynch. reading short_image');
req_desc = tango_read_attribute_asynch(dev_name, 'short_image');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> tango_read_attribute_asynch passed');
val = tango_read_attribute_reply(req_desc, 0);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> tango_read_attribute_reply passed');
disp(val);
%**********************************************************************************
%==================================================================================
%                                   POLLING
%==================================================================================
%**********************************************************************************
print_sub_title('TESTING POLLING ORIENTED FUNCTIONS');
%==================================================================================
% CMD POLLING
%==================================================================================
disp('-testing tango_poll_command');
tango_poll_command(dev_name, 'DevVoid', 500);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% CHECK CMD POLLING
%==================================================================================
disp('-testing tango_is_command_polled');
polled = tango_is_command_polled(dev_name, 'DevVoid');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
if (polled ~= 1)
  disp('  `--> ERROR::cmd DevVoid should be polled');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% CMD POLLING PERIOD
%==================================================================================
disp('-testing tango_get_command_poll_period');
pp = tango_get_command_poll_period(dev_name, 'DevVoid');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
if (pp ~= 500)
  disp(sprintf('  `--> ERROR::polling period should be 500 ms - got %.2fms', pp));
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% ATTR POLLING
%==================================================================================
disp('-testing tango_poll_attribute');
tango_poll_attribute(dev_name, 'short_scalar', 250);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% CHECK ATTR POLLING
%==================================================================================
disp('-testing tango_is_attribute_polled');
polled = tango_is_attribute_polled(dev_name, 'short_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
if (polled ~= 1)
  disp('  `--> ERROR::attr short_scalar should be polled');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% ATTR POLLING PERIOD
%==================================================================================
disp('-testing tango_get_attribute_poll_period');
pp = tango_get_attribute_poll_period(dev_name, 'short_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
if (pp ~= 250)
  disp(sprintf('  `--> ERROR::polling period should be 250 ms - got %.2fms', pp));
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% WAIT A SECOND FOR POLLING BUFFERS TO BE FILLED
%==================================================================================
disp('-waiting for polling buffers to be filled');
pause(1);
%==================================================================================
% POLLING STATUS
%==================================================================================
disp('-testing tango_polling_status');
ps = tango_polling_status(dev_name);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
[m, n] = size(ps);
for i = 1:n
  disp(ps(i));
end
%==================================================================================
% DEVICE CMD HISTORY
%==================================================================================
disp('-testing tango_command_history');
hist = tango_command_history(dev_name, 'DevVoid', 5);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  %result = -1; return;
else
  disp('  `--> passed');
  [m, n] = size(hist);
  for i = 1:n
    disp(hist(i));
  end
end
%==================================================================================
% DEVICE ATTRIBUTE HISTORY
%==================================================================================
disp('-testing tango_attribute_history');
hist = tango_attribute_history(dev_name, 'short_scalar', 5);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  %result = -1; return;
else
  disp('  `--> passed');
  [m, n] = size(hist);
  for i = 1:n
    disp(sprintf('\t-Attr.History(%d) ----------------\r', i));
    disp(hist(i));
    disp(sprintf('\t-Attr.History(%d).value ----------\r', i));
    disp(hist(i).value);  
  end
end
%==================================================================================
% STOP POLLING CMD 
%==================================================================================
disp('-testing tango_stop_poll_command');
tango_stop_poll_command(dev_name, 'DevVoid');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
polled = tango_is_command_polled(dev_name, 'DevVoid');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
if (polled ~= 0)
  disp('  `--> ERROR:: cmd DevVoid should NOT be polled');
  result = -1; return;
end
disp('  `--> passed');
%==================================================================================
% STOP POLLING ATTR 
%==================================================================================
disp('-testing tango_stop_poll_attribute');
tango_stop_poll_attribute(dev_name, 'short_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
polled = tango_is_attribute_polled(dev_name, 'short_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
if (polled ~= 0)
  disp('  `--> ERROR::attr short_scalar should NOT be polled');
  result = -1; return;
end
disp('  `--> passed');
%**********************************************************************************
%==================================================================================
%                                   PROPERTIES
%==================================================================================
%**********************************************************************************
print_sub_title('TESTING PROPERTY ORIENTED FUNCTIONS');
%==================================================================================
% GET SINGLE PROPERTY
%==================================================================================
disp('-testing tango_get_property');
prop = tango_get_property(dev_name, 'mthreaded_impl');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
disp(prop);
%==================================================================================
% GET SEVERAL PROPERTIES
%==================================================================================
disp('-testing tango_get_properties');
prop_list = {'mthreaded_impl', 'sleep_period', 'polled_attr'};
prop = tango_get_properties(dev_name, prop_list);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
[m, n] = size(prop);
for i = 1:n
  disp(prop(i));
end
%==================================================================================
% PUT SINGLE PROPERTY
%==================================================================================
disp('-testing tango_put_property');
tango_put_property(dev_name, 'dummy_0', {'dummy_0_value_1', 'dummy_0_value_2'});
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
prop = tango_get_property(dev_name, 'mthreaded_impl');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp(prop);
%==================================================================================
% PUT SEVERAL PROPERTIES
%==================================================================================
disp('-testing tango_put_properties');
clear prop
prop.name = 'mthreaded_impl';
prop.value = {'1'};
prop(2).name = 'sleep_period';
prop(2).value = {'100'};
prop(2).name = 'dummy_1';
prop(2).value = {'dummy_1_value_1', 'dummy_1_value_2', 'dummy_1_value_3'};
prop(3).name = 'dummy_2';
prop(3).value = {'dummy_2_value_1', 'dummy_2_value_2'};
prop(4).name = 'dummy_3';
prop(4).value = {'dummy_3_value_1'};
tango_put_properties(dev_name, prop);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
clear prop
prop = {'mthreaded_impl', 'sleep_period'};
prop = tango_get_properties(dev_name, prop);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
[m, n] = size(prop);
for i = 1:n
  disp(prop(i));
end
%==================================================================================
% DELETE SINGLE PROPERTY
%==================================================================================
disp('-testing tango_del_property');
tango_del_property(dev_name, 'dummy_0');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
clear prop
prop = {'dummy_0', 'dummy_1', 'dummy_2', 'dummy_3'};
prop = tango_get_properties(dev_name, prop);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
[m, n] = size(prop);
for i = 1:n
  disp(prop(i));
end
%==================================================================================
% DELETE SEVERAL PROPERTIES
%==================================================================================
disp('-testing tango_del_properties');
tango_del_properties(dev_name, {'dummy_1', 'dummy_2', 'dummy_3'});
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
disp('  `--> passed');
clear prop
prop = {'dummy_1', 'dummy_2', 'dummy_3'};
prop = tango_get_properties(dev_name, prop);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  result = -1; return;
end
[m, n] = size(prop);
for i = 1:n
  disp(prop(i));
end
%==================================================================================
% END OF REGRESSION TEST
%==================================================================================
result = 0;
return;

%**********************************************************************************
% TANGO GROUP TEST
%**********************************************************************************
function err = test_groups (num_it)
print_sub_title('TESTING TANGO GROUPS');
disp('-testing tango_group_create: test1');
test1_id = tango_group_create('test1');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  return;
end
disp('-testing tango_group_add: tango/tangotest/* -> test1');
tango_group_add(test1_id, 'tango/tangotest/*');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err = -1;
  return;
end
disp('-testing tango_group_remove: tango/tangotest/256 -> test1');
tango_group_remove(test1_id, 'tango/tangotest/256', 0);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err = -1;
  return;
end
disp('-testing tango_group_create: test2');
test2_id = tango_group_create('test2');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err = -1;
  return;
end
disp('-testing tango_group_add: tests/tangotest/* -> test2');
tango_group_add(test2_id, 'tests/tangotest/*');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err = -1;
  return;
end
disp('-testing tango_group_remove: tests/tangotest/djeremaille -> test2');
tango_group_remove(test2_id, 'tests/tangotest/djeremaille', 0);
tango_group_remove(test2_id, 'tests/tangotest/core*', 0);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err = -1;
  return;
end
disp('-testing tango_group_create: test');
test_id = tango_group_create('test');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err = -1;
  return;
end
disp('-testing tango_group_add: test1 -> test');
tango_group_add(test_id, test1_id);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err = -1;
  return;
end
disp('-testing tango_group_add: test2 -> test');
tango_group_add(test_id, test2_id);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err = -1;
  return;
end

disp('-testing tango_group_create: root');
root_id = tango_group_create('root');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err = -1;
  return;
end

disp('-testing tango_group_add: test -> root');
tango_group_add(root_id, test_id);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err = -1;
  return;
end

disp('-testing tango_group_dump: root');
tango_group_dump(root_id);

% disp('-testing tango_group_command_inout(cmd: Status) on group <root>');
% r = tango_group_command_inout(root_id, 'Status', 0, 1);
% if (tango_error == -1)
%   disp('  `--> ERROR');
%   tango_print_error_stack;
%   err = -1;
%   return;
% end
% if (r.has_failed)
%   [r_m, r_n] = size(r.replies);
%   for i = 1:r_n
%       if (r.replies(i).has_failed)
%         disp(r.replies(i));
%         [err_m, err_n] = size(r.replies(i).error);
%         for j = 1:err_n
%             disp(r.replies(i).error(j));
%         end
%       end
%   end
% end

% disp('-testing tango_group_dump: root');
% tango_group_dump(root_id);

for i = 1:num_it
  %--------------------------------------------------------
  err = tangotest_group_command_inout (test1_id, 0, 16);
  if (err == -1)
    return;
  end
  %--------------------------------------------------------
  err = tangotest_group_command_inout (test_id, 1, 28);
  if (err == -1)
    return;
  end
  %--------------------------------------------------------
  err = tangotest_group_write_attribute (test1_id, 0, 16);
  if (err == -1)
    return;
  end
  %--------------------------------------------------------
  err = tangotest_group_write_attribute (test_id, 1, 28);
  if (err == -1)
    return;
  end
  %--------------------------------------------------------
  err = tangotest_group_read_attribute (test1_id, 16);
  if (err == -1)
    return;
  end
end

% disp('-testing tango_group_command_inout(cmd: DevDouble)');
% r = tango_group_command_inout(test_id, 'DevDouble', 0, 1, 3.14);
% if (tango_error == -1)
%   disp('  `--> GOT EXPECTED ERROR');
% end

% disp('-testing tango_group_create: bpm');
% bpm_id = tango_group_create('bpm');
% if (tango_error == -1)
%   disp('  `--> ERROR');
%   tango_print_error_stack;
%   err = -1;
%   return;
% end
% 
% disp('-testing tango_group_add: ans*/*/bpm.* -> bpm');
% tango_group_add(bpm_id, 'ans*/*/bpm.*');
% if (tango_error == -1)
%   disp('  `--> ERROR');
%   tango_print_error_stack;
%   err = -1;
%   return;
% end
% 
% disp('-testing tango_group_add: bpm -> root');
% tango_group_add(root_id, bpm_id);
% if (tango_error == -1)
%   disp('  `--> ERROR');
%   tango_print_error_stack;
%   err = -1;
%   return;
% end
% 
% disp('-testing tango_group_command_inout (cmd: Status)');
% r = tango_group_command_inout(bpm_id, 'Status', 0, 1);
% if (tango_error == -1)
%   disp('  `--> ERROR');
%   tango_print_error_stack;
%   err = -1;
%   return;
% end
% if (r.has_failed)
%   [r_m, r_n] = size(r.replies);
%   for i = 1:r_n
%       if (r.replies(i).has_failed)
%         disp(r.replies(i));
%         [err_m, err_n] = size(r.replies(i).error);
%         for j = 1:err_n
%             disp(r.replies(i).error(j));
%         end
%       end
%   end
% end 
% 
% disp('-testing tango_group_command_inout (cmd: GetOffsetH - loop test)');
% count = num_it;
% while (count) 
%   r = tango_group_command_inout(bpm_id, 'GetOffsetH', 0, 1);
%   if (r.has_failed)
%       [r_m, r_n] = size(r.replies);
%       for i = 1:r_n
%           if (r.replies(i).has_failed)
%             disp(r.replies(i));
%             [err_m, err_n] = size(r.replies(i).error);
%             for j = 1:err_n
%                 disp(r.replies(i).error(j));
%             end
%           end
%       end  
%     break
%   end
%   count = count - 1;
% end

% disp('-testing tango_group_read_attribute (attr: X - loop test)');
% count = num_it;
% while (count) 
%   r = tango_group_read_attribute(bpm_id, 'X', 0);
%   if (r.has_failed)
%       [r_m, r_n] = size(r.replies);
%       for i = 1:r_n
%           if (r.replies(i).has_failed)
%             disp(r.replies(i));
%             [err_m, err_n] = size(r.replies(i).error);
%             for j = 1:err_n
%                 disp(r.replies(i).error(j));
%             end
%           end
%       end  
%     break
%   end
%   count = count - 1;
% end

% disp('-testing tango_group_read_attribute (attr: Xv - loop test)');
% count = num_it;
% while (count) 
%   r = tango_group_read_attribute(bpm_id, 'Xv', 0);
%   if (r.has_failed)
%     disp(r.replies(1));
%     [r_m, r_n] = size(r.replies);
%     for i = 1:r_n
%         if (r.replies(i).has_failed)
%           disp(r.replies(i));
%           [err_m, err_n] = size(r.replies(i).error);
%           for j = 1:err_n
%             disp(r.replies(i).error(j));
%           end
%         end
%     end  
%     break
%   end
%   count = count - 1;
% end

% disp('-testing tango_group_remove (string)');
% tango_group_remove(root_id, 'tango/tangotest/1', 1);
% if (tango_error == -1)
%   disp('  `--> ERROR');
%   tango_print_error_stack;
%   err = -1;
%   return;
% end
% disp('-testing tango_group_remove (cell array)');
% tango_group_remove(root_id, {'tango/tangotest/2', 'tango/tangotest/3'}, 1);
% if (tango_error == -1)
%   disp('  `--> ERROR');
%   tango_print_error_stack;
%   err = -1;
%   return;
% end
% disp('-testing tango_group_remove (group_id)');
% tango_group_remove(root_id, root_id, 1);
% if (tango_error == -1)
%   disp('  `--> GOT EXPECTED ERROR');
% end
% disp('-testing tango_group_kill');
% tango_group_kill(test_id);
% if (tango_error == -1)
%   disp('  `--> ERROR');
%   tango_print_error_stack;
%   err = -1;
%   return;
% end
% disp('-testing tango_group_kill (group_id)');
% tango_group_kill(test4_id);
% if (tango_error == -1)
%   disp('  `--> ERROR');
%   tango_print_error_stack;
%   err = -1;
%   return;
% end
% disp('-testing tango_group_remove (group_id)');
% tango_group_remove(root_id, test3_id, 1);
% if (tango_error == -1)
%   disp('  `--> GOT EXPECTED ERROR');
% end
% disp('-testing tango_group_remove (group_id)');
% tango_group_remove(root_id, test2_id, 1);
% if (tango_error == -1)
%   disp('  `--> GOT EXPECTED ERROR');
% end
disp('-testing tango_group_kill');
tango_group_kill(root_id);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err = -1;
  return;
end
% disp('-testing tango_group_kill');
% tango_group_kill(root_id);
% if (tango_error == -1)
%   disp('  `--> GOT EXPECTED ERROR');
% end
% disp('-testing tango_group_id');
% root_id = tango_group_id('root');
% if (tango_error == -1)
%   disp('  `--> GOT EXPECTED ERROR');
% end
err = 0;
return;

%**********************************************************************************
function err = tangotest_group_command_inout (grp_id, fwd, n)
%--------------------------------------------------------------------------------
fgt = 0;
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: Status)');
cmd = 'Status';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd) == -1) 
  err = -1;
  return;   
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevString)');
cmd = 'DevString';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, '1:a:2:b:3:c') == -1)
  err = -1;
  return;      
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevDouble)');
cmd = 'DevDouble';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, 3.14) == -1)
  err = -1;
  return;     
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevFloat)');
cmd = 'DevFloat';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, single(3.14)) == -1)
  err = -1;
  return;    
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevLong)');
cmd = 'DevLong';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, int32(1)) == -1)
  err = -1;
  return;      
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevULong)');
cmd = 'DevULong';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, uint32(1)) == -1)
  err = -1;
  return;     
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevShort)');
cmd = 'DevShort';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, int16(1)) == -1)
  err = -1;
  return;      
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevUShort)');
cmd = 'DevUShort';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, uint16(1)) == -1)
  err = -1;
  return;   
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevBoolean)');
cmd = 'DevBoolean';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, uint8(1)) == -1)
  err = -1;
  return;     
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarStringArray)');
cmd = 'DevVarStringArray';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, {'0', '1', '2', '3'}) == -1)
  err = -1;
  return;      
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarDoubleArray)');
cmd = 'DevVarDoubleArray';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, [0, 1, 2, 3]) == -1)
  err = -1;
  return;     
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarFloatArray)');
cmd = 'DevVarFloatArray';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, single([0, 1, 2, 3])) == -1)
  err = -1;
  return;      
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarLongArray)');
cmd = 'DevVarLongArray';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, int32([0, 1, 2, 3])) == -1)
  err = -1;
  return;      
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarULongArray)');
cmd = 'DevVarULongArray';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, uint32([0, 1, 2, 3])) == -1)
  err = -1;
  return;      
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarShortArray)');
cmd = 'DevVarShortArray';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, int16([0, 1, 2, 3])) == -1)
  err = -1;
  return;      
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarUShortArray)');
cmd = 'DevVarUShortArray';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, uint16([0, 1, 2, 3])) == -1)
  err = -1;
  return;     
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarCharArray)');
cmd = 'DevVarCharArray';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, int8([0, 1, 2, 3])) == -1)
  err = -1;
  return;      
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVar{unsigned}CharArray)');
cmd = 'DevVarCharArray';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, uint8([0, 1, 2, 3])) == -1)
  err = -1;
  return;     
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarLongStringArray)');
cmd = 'DevVarLongStringArray';
dvlsa.lvalue = int32([1,2,3,4]);
dvlsa.svalue = {'1','2','3','4'};
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dvlsa) == -1)
  err = -1;
  return;     
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarDoubleStringArray)');
cmd = 'DevVarDoubleStringArray';
dvdsa.dvalue = [1,2,3,4];
dvdsa.svalue = {'1','2','3','4'};
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dvdsa) == -1)
  err = -1;
  return;     
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevString) - specific arg per device');
cmd = 'DevString';
argin = {};
for i = 1:n
  argin = cat(2, argin, {'astring'});
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, argin) == -1)
  err = -1;
  return;      
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevDouble) - specific arg per device');
cmd = 'DevDouble';
argin = [];
for i = 1:n
  argin = cat(2, argin, i);
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, argin) == -1)
  err = -1;
  return;    
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevFloat) - specific arg per device');
cmd = 'DevFloat';
argin = single([]);
for i = 1:n
  argin = cat(2, argin, single(i));
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, argin) == -1)
  err = -1;
  return;      
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevLong) - specific arg per device');
cmd = 'DevLong';
argin = int32([]);
for i = 1:n
  argin = cat(2, argin, int32(i));
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, argin) == -1)
  err = -1;
  return;      
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevULong) - specific arg per device');
cmd = 'DevULong';
argin = uint32([]);
for i = 1:n
  argin = cat(2, argin, uint32(i));
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, argin) == -1)
  err = -1;
  return;   
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevShort) - specific arg per device');
cmd = 'DevShort';
argin = int16([]);
for i = 1:n
  argin = cat(2, argin, int16(i));
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, argin) == -1)
  err = -1;
  return;       
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevUShort) - specific arg per device');
cmd = 'DevUShort';
argin = uint16([]);
for i = 1:n
  argin = cat(2, argin, uint16(i));
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, argin) == -1)
  err = -1;
  return;   
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevBoolean) - specific arg per device');
cmd = 'DevBoolean';
argin = uint8([]);
for i = 1:n
  argin = cat(2, argin, uint8(i));
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, argin) == -1)
  err = -1;
  return;      
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarStringArray) - specific arg per device');
cmd = 'DevVarStringArray';
argin = {};
for i = 1:n
  argin = cat(2, argin, {'a:0', 'b:1', 'b:2'});
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, argin) == -1)
  err = -1;
  return;     
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarDoubleArray) - specific arg per device');
cmd = 'DevVarDoubleArray';
argin = {};
for i = 1:n
  argin = cat(2, argin, {[1:n]});
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, argin) == -1)
  err = -1;
  return;   
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarFloatArray) - specific arg per device');
cmd = 'DevVarFloatArray';
argin = {};
for i = 1:n
  argin = cat(2, argin, {single([1:n])});
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, argin) == -1)
  err = -1;
  return;     
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarLongArray) - specific arg per device');
cmd = 'DevVarLongArray';
argin = {};
for i = 1:n
  argin = cat(2, argin, {int32([1:n])});
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, argin) == -1)
  err = -1;
  return;      
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarULongArray) - specific arg per device');
cmd = 'DevVarULongArray';
argin = {};
for i = 1:n
  argin = cat(2, argin, {uint32([1:n])});
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, argin) == -1)
  err = -1;
  return;     
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarShortArray) - specific arg per device');
cmd = 'DevVarShortArray';
argin = {};
for i = 1:n
  argin = cat(2, argin, {int16([1:n])});
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, argin) == -1)
  err = -1;
  return;     
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarUShortArray) - specific arg per device');
cmd = 'DevVarUShortArray';
argin = {};
for i = 1:n
  argin = cat(2, argin, {uint16([1:n])});
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, argin) == -1)
  err = -1;
  return;   
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarCharArray) - specific arg per device');
cmd = 'DevVarCharArray';
argin = {};
for i = 1:n
  argin = cat(2, argin, {int8([1:n])});
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, argin) == -1)
  err = -1;
  return;
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVar{unsigned}CharArray) - specific arg per device');
cmd = 'DevVarCharArray';
argin = {};
for i = 1:n
  argin = cat(2, argin, {uint8([1:n])});
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, argin) == -1)
  err = -1;
  return;   
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarLongStringArray) - specific arg per device');
cmd = 'DevVarLongStringArray';
dvlsa.lvalue = int32([1,2,3,4]);
dvlsa.svalue = {'1','2','3','4'};
argin = {};
for i = 1:n
  argin = cat(2, argin, {dvlsa});
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, argin) == -1)
  err = -1;
  return;     
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarDoubleStringArray) - specific arg per device');
cmd = 'DevVarDoubleStringArray';
dvdsa.dvalue = [1,2,3,4];
dvdsa.svalue = {'1','2','3','4'};
argin = {};
for i = 1:n
  argin = cat(2, argin, {dvdsa});
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, argin) == -1)
  err = -1;
  return;      
end;
err = 0;
return;

%**********************************************************************************
% test_tango_group_command_inout
%**********************************************************************************
function err = test_tango_group_command_inout (grp_id, cmd, fgt, fwd, varargin)
if (nargin == 4)
    r = tango_group_command_inout(grp_id, cmd, fgt, fwd);
else
    r = tango_group_command_inout(grp_id, cmd, fgt, fwd, varargin{1});    
end
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err = -1;
  return;
end
if (r.has_failed)
  [r_m, r_n] = size(r.replies);
  for i = 1:r_n
      if (r.replies(i).has_failed)
        disp(r.replies(i));
        [err_m, err_n] = size(r.replies(i).error);
        for j = 1:err_n
            disp(r.replies(i).error(j));
        end
      end
  end
end 
err = 0;
return;

%**********************************************************************************
function err = tangotest_group_write_attribute (grp_id, fwd, n)
%--------------------------------------------------------------------------------
disp('-testing tango_group_write_attribute(attr: string_scalar)');
attr = 'string_scalar';
if (test_tango_group_write_attribute (grp_id, attr, fwd, '1:a:2:b:3:c') == -1)
 err = -1;
 return;
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_write_attribute(attr: double_scalar)');
attr = 'double_scalar';
if (test_tango_group_write_attribute (grp_id, attr, fwd, 3.14) == -1)
  err = -1;
  return;
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_write_attribute(attr: double_scalar_w)');
attr = 'double_scalar_w';
if (test_tango_group_write_attribute (grp_id, attr, fwd, 3.14) == -1)
  err = -1;
  return;
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_write_attribute(attr: long_scalar)');
attr = 'long_scalar';
if (test_tango_group_write_attribute (grp_id, attr, fwd, int32(256)) == -1)
  err = -1;
  return;
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_write_attribute(attr: long_scalar_w)');
attr = 'long_scalar_w';
if (test_tango_group_write_attribute (grp_id, attr, fwd, int32(256)) == -1)
  err = -1;
  return;
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_write_attribute(attr: short_scalar)');
attr = 'short_scalar';
if (test_tango_group_write_attribute (grp_id, attr, fwd, int16(128)) == -1)
  err = -1;
  return;
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_write_attribute(attr: short_scalar_w)');
attr = 'short_scalar_w';
if (test_tango_group_write_attribute (grp_id, attr, fwd, int16(128)) == -1)
  err = -1;
  return;
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_write_attribute(attr: string_scalar) - specific arg per device');
attr = 'string_scalar';
value = {};
for i = 1:n
  value = cat(2, value, {'astring'});
end
if (test_tango_group_write_attribute (grp_id, attr, fwd, value) == -1)
 err = -1;
 return;
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_write_attribute(attr: double_scalar) - specific arg per device');
attr = 'double_scalar';
value = [];
for i = 1:n
  value = cat(2, value,  i / 10);
end
if (test_tango_group_write_attribute (grp_id, attr, fwd, value) == -1)
  err = -1;
  return;
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_write_attribute(attr: double_scalar_w) - specific arg per device');
attr = 'double_scalar_w';
value = [];
for i = 1:n
  value = cat(2, value,  i / 10);
end
if (test_tango_group_write_attribute (grp_id, attr, fwd, value) == -1)
  err = -1;
  return;
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_write_attribute(attr: long_scalar) - specific arg per device');
attr = 'long_scalar';
value = int32([]);
for i = 1:n
  value = cat(2, value,  int32(i));
end
if (test_tango_group_write_attribute (grp_id, attr, fwd, value) == -1)
  err = -1;
  return;
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_write_attribute(attr: long_scalar_w) - specific arg per device');
attr = 'long_scalar_w';
value = int32([]);
for i = 1:n
  value = cat(2, value,  int32(i));
end
if (test_tango_group_write_attribute (grp_id, attr, fwd, value) == -1)
  err = -1;
  return;
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_write_attribute(attr: short_scalar) - specific arg per device');
attr = 'short_scalar';
value = int16([]);
for i = 1:n
  value = cat(2, value,  int16(i));
end
if (test_tango_group_write_attribute (grp_id, attr, fwd, value) == -1)
  err = -1;
  return;
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_write_attribute(attr: short_scalar_w) - specific arg per device');
attr = 'short_scalar_w';
value = int16([]);
for i = 1:n
  value = cat(2, value,  int16(i));
end
if (test_tango_group_write_attribute (grp_id, attr, fwd, value) == -1)
  err = -1;
  return;
end
err = 0;
return;

%**********************************************************************************
% test_tango_group_write_attribute
%**********************************************************************************
function err = test_tango_group_write_attribute (grp_id, attr, fwd, value)
r = tango_group_write_attribute(grp_id, attr, fwd, value);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err = -1;
  return;
end
if (r.has_failed)
  [r_m, r_n] = size(r.replies);
  for i = 1:r_n
      if (r.replies(i).has_failed)
        disp(r.replies(i));
        [err_m, err_n] = size(r.replies(i).error);
        for j = 1:err_n
            disp(r.replies(i).error(j));
        end
      end
  end
end
err = 0;
return;

%**********************************************************************************
function err = tangotest_group_read_attribute (grp_id, n)
%--------------------------------------------------------------------------------
fwd = 1;
%--------------------------------------------------------------------------------
disp('-testing tango_group_read_attribute(attr: string_scalar)');
attr = 'string_scalar';
if (test_tango_group_read_attribute (grp_id, attr, fwd) == -1)
 err = -1;
 return;
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_read_attribute(attr: double_scalar)');
attr = 'double_scalar';
if (test_tango_group_read_attribute (grp_id, attr, fwd) == -1)
  err = -1;
  return;
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_read_attribute(attr: double_scalar_rww)');
attr = 'double_scalar_rww';
if (test_tango_group_read_attribute (grp_id, attr, fwd) == -1)
  err = -1;
  return;
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_read_attribute(attr: long_scalar)');
attr = 'long_scalar';
if (test_tango_group_read_attribute (grp_id, attr, fwd) == -1)
  err = -1;
  return;
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_read_attribute(attr: long_scalar_rww)');
attr = 'long_scalar_rww';
if (test_tango_group_read_attribute (grp_id, attr, fwd) == -1)
  err = -1;
  return;
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_read_attribute(attr: short_scalar)');
attr = 'short_scalar';
if (test_tango_group_read_attribute (grp_id, attr, fwd) == -1)
  err = -1;
  return;
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_read_attribute(attr: short_scalar_ro)');
attr = 'short_scalar_ro';
if (test_tango_group_read_attribute (grp_id, attr, fwd) == -1)
  err = -1;
  return;
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_read_attribute(attr: short_scalar_rww)');
attr = 'short_scalar_rww';
if (test_tango_group_read_attribute (grp_id, attr, fwd) == -1)
  err = -1;
  return;
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_read_attribute(attr: double_spectrum)');
attr = 'double_spectrum';
if (test_tango_group_read_attribute (grp_id, attr, fwd) == -1)
  err = -1;
  return;
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_read_attribute(attr: long_spectrum)');
attr = 'long_spectrum';
if (test_tango_group_read_attribute (grp_id, attr, fwd) == -1)
  err = -1;
  return;
end
%--------------------------------------------------------------------------------
disp('-testing tango_group_read_attribute(attr: short_spectrum)');
attr = 'short_spectrum';
if (test_tango_group_read_attribute (grp_id, attr, fwd) == -1)
  err = -1;
  return;
end
% %--------------------------------------------------------------------------------
% disp('-testing tango_group_read_attribute(attr: double_image)');
% attr = 'double_image';
% if (test_tango_group_read_attribute (grp_id, attr, fwd) == -1)
%   err = -1;
%   return;
% end
% %--------------------------------------------------------------------------------
% disp('-testing tango_group_read_attribute(attr: long_image)');
% attr = 'long_image';
% if (test_tango_group_read_attribute (grp_id, attr, fwd) == -1)
%   err = -1;
%   return;
% end
%--------------------------------------------------------------------------------
disp('-testing tango_group_read_attribute(attr: short_image)');
attr = 'short_image';
if (test_tango_group_read_attribute (grp_id, attr, fwd) == -1)
  err = -1;
  return;
end
err = 0;
return;

%**********************************************************************************
% test_tango_group_read_attribute
%**********************************************************************************
function err = test_tango_group_read_attribute (grp_id, attr, fwd)
r = tango_group_read_attribute(grp_id, attr, fwd);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err = -1;
  return;
end
if (r.has_failed)
  [r_m, r_n] = size(r.replies);
  for i = 1:r_n
      if (r.replies(i).has_failed)
        disp(r.replies(i));
        [err_m, err_n] = size(r.replies(i).error);
        for j = 1:err_n
            disp(r.replies(i).error(j));
        end
      end
  end
end
err = 0;
return;

%**********************************************************************************
% print_sub_title
%**********************************************************************************
function print_sub_title (sub_title) 
disp(cat(2, '-| ', sub_title));  
return;


%**********************************************************************************
% print_title
%**********************************************************************************
function print_title (title)
[m, n] = size(title);
l(1:n) = char('*');
disp(l);
disp(title);
disp(l);
return;


