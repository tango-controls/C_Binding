function result = tango_reg_test (dev_name, num_it, dump)
%TANGO_REG_TEST Executes a TANGO binding regression test using the TangoTest device.
%
% Syntax:
% -------
%   result = tango_reg_test (dev_name, num_it, dump)
%
% Argin: dev_name
% ----------------
%   |- type: 1-by-n char array (string)
%   |- desc: the device name of a TangoTest device
%
% Argin: num_it
% -------------
%   |- type: 1-by-1 double array
%   |- desc: the # of times you want to execute the test
%
% Argin: dump
% -----------
%   |- type: 1-by-1 double array
%   |- desc: dump results if set to true
%
% Argout: result
% --------------
%   |- type: 1-by-1 double array
%   |- desc: -1 on error, 0 otherwise
%
print_sub_title('TANGO BINDING FOR MATLAB - REG.TEST');
err = 0;
for counter = 1:num_it
  % -------------------------------
  err = test_misc(dev_name, dump);
  if (err == -1) break; end
  % -------------------------------
  err = test_command(dev_name, dump);
  if (err == -1) break; end
  % -------------------------------
  err = test_attribute(dev_name, dump);
  if (err == -1) break; end
  % -------------------------------
  err = test_polling(dev_name, dump);
  if (err == -1) break; end
  % -------------------------------
  err = test_property(dev_name, dump);
  if (err == -1) break; end
  %-------------------------------
  %err = test_groups(dev_name, dump);
  %if (err == -1) break; end
end
if (err == -1)
 print_title('* REG-TEST FAILED * REG-TEST FAILED * REG-TEST FAILED *');
else
 print_title('* REG-TEST PASSED * REG-TEST PASSED * REG-TEST PASSED *');
end
result = err;
return;

%==========================================================================
% FUNCTION test_misc
%==========================================================================
function err = test_misc (dev_name, dump)
print_sub_title('TESTING MISC. FUNCTIONS');
%--------------------------------------------------------------------------
disp('- testing tango_ping');
elapsed_usec = tango_ping(dev_name);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
  disp(sprintf('%s is up and ready [RTT: %f usec]\r', dev_name, elapsed_usec));
end
%--------------------------------------------------------------------------
disp('- testing tango_admin_name');
adm_name = tango_admin_name(dev_name);
if (tango_error == -1) 
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
  disp(sprintf('  `--> %s\r', adm_name));
end
%--------------------------------------------------------------------------
disp('- testing tango_description');
desc = tango_description(dev_name);
if (tango_error == -1) 
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
  disp(sprintf('  `--> %s\r', desc));
end
%--------------------------------------------------------------------------
disp('- testing tango_status');
status = tango_status(dev_name);
if (tango_error == -1) 
  disp('  `--> ERROR');
  tango_print_error_stack;
  return;
end
disp('  `--> passed');
if (dump)
  disp(sprintf('  `--> %s\r', status));
end
%--------------------------------------------------------------------------
disp('- testing tango_state');
state = tango_state(dev_name);
if (tango_error == -1) 
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
  disp(sprintf('  `--> %s [%.0f]\r', state.name, state.value));
end
%--------------------------------------------------------------------------
disp('- testing tango_info');
info = tango_info(dev_name);
if (tango_error == -1) 
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
  disp(info);
end
%--------------------------------------------------------------------------
disp('- testing tango_idl_version');
idl_ver = tango_idl_version(dev_name);
if (tango_error == -1) 
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
  disp(sprintf('  `--> %s supports TANGO IDL version %d\r', dev_name, idl_ver));
end
%--------------------------------------------------------------------------
disp('- testing tango_get_source');
src = tango_get_source(dev_name);
if (tango_error == -1) 
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp(sprintf('  `--> %s [%.0f]\r', src.name, src.value));
%--------------------------------------------------------------------------
disp('- testing tango_set_source (as numeric value)');
tango_set_source(dev_name, mod(src.value + 1, 2));
if (tango_error == -1) 
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('- testing tango_set_source (as string value)');
tango_set_source(dev_name, 'DevICe');
if (tango_error == -1) 
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('- testing tango_set_source (as string value)');
tango_set_source(dev_name, 'DUMMY-SOURCE');
if (tango_error == 0) 
  disp('  `--> ERROR EXPECTED (tango_error should be -1)');
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('- testing tango_get_timeout');
tmo = tango_get_timeout(dev_name);
if (tango_error == -1) 
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
  disp(sprintf('  `--> %.0f ms\r', tmo));
end
%--------------------------------------------------------------------------
disp('- testing tango_set_timeout');
tango_set_timeout(dev_name, tmo + 1);
if (tango_error == -1) 
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('-testing tango_black_box');
bb = tango_black_box(dev_name, 5);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
  [m, n] = size(bb);
  for i = 1:n
    disp(sprintf('\t%s\r', char(bb(i))));
  end
end
%--------------------------------------------------------------------------
err =  0;
return;

%==========================================================================
% FUNCTION test_command
%==========================================================================
function err =  test_command (dev_name, dump)
print_sub_title('TESTING COMMANDS AND COMMAND ORIENTED FUNCTIONS');
%--------------------------------------------------------------------------
disp('-testing tango_command_query');
cmd_info = tango_command_query(dev_name, 'DevVarDoubleStringArray');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
  disp(cmd_info);
end
%--------------------------------------------------------------------------
disp('-testing tango_command_list_query');
cmd_list = tango_command_list_query(dev_name);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
  [m, n] = size(cmd_list);
  for i = 1:n
    disp(cmd_list(i));
  end
end
%--------------------------------------------------------------------------
disp('-testing DevVoid command');
tango_command_inout(dev_name, 'DevVoid');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('-testing DevVarLongStringArray command');
argin.lvalue = int32(0:1023);
argin.svalue = cell(1,256);
argin.svalue(1,1:256) = {'dev-var-long-string-array-test'};
argout = tango_command_inout(dev_name, 'DevVarLongStringArray', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
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
  err =  -1; return;
end
if (argin.lvalue ~= argout.lvalue)
  disp('  `--> ERROR::argin.lvalue ~= argout.lvalue');
  err =  -1; return;
end
[argin_m, argin_n] = size(argin.svalue);
[argout_m, argout_n] = size(argin.svalue);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin.svalue) ~= size(argout.svalue)');
  err =  -1; return;
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
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('-testing DevVarDoubleStringArray command');
argin.dvalue = 1024:2047;
argin.svalue = cell(1,256);
argin.svalue(1,1:256) = {'dev-var-double-string-array-test'};
argout = tango_command_inout(dev_name, 'DevVarDoubleStringArray', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
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
  err =  -1; return;
end
if (argin.dvalue ~= argout.dvalue)
  disp('  `--> ERROR::argin.dvalue ~= argout.dvalue');
  err =  -1; return;
end
[argin_m, argin_n] = size(argin.svalue);
[argout_m, argout_n] = size(argin.svalue);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin.svalue) ~= size(argout.svalue)');
  err =  -1; return;
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
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('-testing DevVarStringArray command');
argin = cell(1,256);
argin(1,1:256) = {'dev-var-string-array-test'};
argout = tango_command_inout(dev_name, 'DevVarStringArray', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
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
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('-testing DevVarDoubleArray command');
argin = 0:0.1:128;
argout = tango_command_inout(dev_name, 'DevVarDoubleArray', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('-testing DevVarFloatArray command');
argin = single(-128:0.1:0);
argout = tango_command_inout(dev_name, 'DevVarFloatArray', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('-testing DevVarULongArray command');
argin = uint32(0:256);
argout = tango_command_inout(dev_name, 'DevVarULongArray', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('-testing DevVarLongArray command');
argin = int32(-1024:0);
argout = tango_command_inout(dev_name, 'DevVarLongArray', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('-testing DevVarUShortArray command');
argin = uint16(0:1024);
argout = tango_command_inout(dev_name, 'DevVarUShortArray', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('-testing DevVarShortArray command');
argin = int16(-1024:0);
argout = tango_command_inout(dev_name, 'DevVarShortArray', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('-testing DevVarCharArray command');
argin = uint8(0:256);
argout = tango_command_inout(dev_name, 'DevVarCharArray', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('-testing DevString command');
argin = 'dev-string';
argout = tango_command_inout(dev_name, 'DevString', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('-testing DevBoolean command');
argin = uint8(1);
argout = tango_command_inout(dev_name, 'DevBoolean', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('-testing DevUShort command');
argin = uint16(2^16 - 1);
argout = tango_command_inout(dev_name, 'DevUShort', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('-testing DevShort command');
argin = int16(-2^16);
argout = tango_command_inout(dev_name, 'DevShort', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('-testing DevULong command');
argin = uint32(2^32 -1);
argout = tango_command_inout(dev_name, 'DevULong', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('-testing DevLong command');
argin = int32(-2^32);
argout = tango_command_inout(dev_name, 'DevLong', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('-testing DevFloat command');
% work around for matlab compiler bug in double to single conversion
argin = -pi;
argin = single(argin);
argout = tango_command_inout(dev_name, 'DevFloat', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('-testing DevDouble command');
argin = pi;
argout = tango_command_inout(dev_name, 'DevDouble', argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
[argin_m, argin_n] = size(argin);
[argout_m, argout_n] = size(argout);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin) ~= size(argout)');
end
if (argin ~= argout)
  disp('  `--> ERROR::argin ~= argout');
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('-testing asynch DevVarLongStringArray command');
argin.lvalue = int32(0:1023);
argin.svalue = cell(1,256);
argin.svalue(1,1:256) = {'dev-var-long-string-array-test'};
req_desc = tango_command_inout_asynch(dev_name, 'DevVarLongStringArray', 0, argin);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
pause(0.1);
argout = tango_command_inout_reply(req_desc, 100);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
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
  err =  -1; return;
end
if (argin.lvalue ~= argout.lvalue)
  disp('  `--> ERROR::argin.lvalue ~= argout.lvalue');
  err =  -1; return;
end
[argin_m, argin_n] = size(argin.svalue);
[argout_m, argout_n] = size(argin.svalue);
if (argin_m ~= argout_m | argin_n ~= argout_n)
  disp('  `--> ERROR::size(argin.svalue) ~= size(argout.svalue)');
  err =  -1; return;
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
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
err =  0;
return;


%==========================================================================
% FUNCTION test_attribute
%==========================================================================
function err =  test_attribute(dev_name, dump)
%--------------------------------------------------------------------------
print_sub_title('TESTING ATTRIBUTE ORIENTED FUNCTIONS');
%--------------------------------------------------------------------------
disp('-testing tango_get_attribute_list');
attr_list = tango_get_attribute_list(dev_name);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
if (dump)
  disp('  `--> passed');
  [m, n] = size(attr_list);
  for i = 1:n
   disp(attr_list(i));
  end
end
%--------------------------------------------------------------------------
disp('-testing tango_get_attribute_config (get config. for one attribute)');
attr_config = tango_get_attribute_config(dev_name, 'short_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
  disp(attr_config);
end
%--------------------------------------------------------------------------
disp('-testing tango_get_attributes_config (get config. for several attributes)');
attr_list = {'short_scalar', 'long_scalar', 'double_scalar', ...
             'short_spectrum', 'long_spectrum', 'double_spectrum', ...
             'short_image', 'long_image', 'double_image'};
attr_config = tango_get_attributes_config(dev_name, attr_list);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
if (dump)
  disp('  `--> passed');
  [m, n] = size(attr_config);
  for i = 1:n
    disp(attr_config(i));
  end
end
%--------------------------------------------------------------------------
disp('-testing tango_set_attribute_config');
attr_config_before = tango_get_attribute_config(dev_name, 'short_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end 
if (dump)
 disp('  `--> original <short_scalar> config');
 disp(attr_config_before);
end
%change some values
attr_config_before.description = 'a simple short scalar attribute';
attr_config_before.label = 'sc';
attr_config_before.unit = 'A.U.';
attr_config_before.standard_unit = attr_config(1).unit;
attr_config_before.display_unit = attr_config(1).unit;
attr_config_before.format = 'none';
attr_config_before.min_value = '-16384';
attr_config_before.max_value = '16384';
if (dump)
 disp('  `--> <short_scalar> config after modifications (before write)');
 disp(attr_config_before);
end
tango_set_attributes_config(dev_name, attr_config_before);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return; 
end
disp('  `--> passed');
attr_config_after = tango_get_attribute_config(dev_name, 'short_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
if (dump)
 disp('  `--> <short_scalar> config after modifications (after re-read)');
 disp(attr_config_after);
end
%--------------------------------------------------------------------------
print_sub_title('READING/WRITTING ATTRIBUTES (INDIVIDUALLY)');
%--------------------------------------------------------------------------
disp('-writing string_scalar');
str_in = 'a1:b2:c3:d4';
tango_write_attribute(dev_name, 'string_scalar', str_in );
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
 disp(sprintf('  `--> wrote %s', str_in));
end
disp('-reading string_scalar');
str_out = tango_read_attribute(dev_name, 'string_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
 disp(str_out);
end
if (strcmp(str_in, str_out.value) == 0) 
  disp('  `--> ERROR');
  err =  -1; return;
end
%--------------------------------------------------------------------------
disp('-writing boolean_scalar');
val_in = uint8(0);
tango_write_attribute(dev_name, 'boolean_scalar', val_in);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
if (dump)
  disp(sprintf('  `--> wrote %d', double(val_in)));
end
disp('  `--> passed');
disp('-reading boolean_scalar');
val_out = tango_read_attribute(dev_name, 'boolean_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
 disp(val_out);
end
% if (val_in(1) ~= val_out.value(2)) 
%   disp('  `--> ERROR');
%   err =  -1; return;
% end
%--------------------------------------------------------------------------
disp('-writing uchar_scalar');
val_in = uint8(128);
tango_write_attribute(dev_name, 'uchar_scalar', val_in);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
if (dump)
  disp(sprintf('  `--> wrote %d', double(val_in)));
end
disp('  `--> passed');
disp('-reading uchar_scalar');
val_out = tango_read_attribute(dev_name, 'uchar_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
 disp(val_out);
end
% if (val_in(1) ~= val_out.value(2)) 
%   disp('  `--> ERROR');
%   err =  -1; return;
% end
%--------------------------------------------------------------------------
disp('-writing ushort_scalar');
val_in = uint16(4096);
tango_write_attribute(dev_name, 'ushort_scalar', val_in);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
if (dump)
  disp(sprintf('  `--> wrote %d', double(val_in)));
end
disp('  `--> passed');
disp('-reading ushort_scalar');
val_out = tango_read_attribute(dev_name, 'ushort_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
 disp(val_out);
end
% if (val_in(1) ~= val_out.value(2)) 
%   disp('  `--> ERROR');
%   err =  -1; return;
% end
%--------------------------------------------------------------------------
disp('-writing short_scalar');
val_in = int16(8192);
tango_write_attribute(dev_name, 'short_scalar', val_in);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
if (dump)
  disp(sprintf('  `--> wrote %d', double(val_in)));
end
disp('  `--> passed');
disp('-reading short_scalar');
val_out = tango_read_attribute(dev_name, 'short_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
 disp(val_out);
end
% if (val_in(1) ~= val_out.value(2)) 
%   disp('  `--> ERROR');
%   err =  -1; return;
% end
%--------------------------------------------------------------------------
disp('-writing long_scalar');
val_in = int32(8192);
tango_write_attribute(dev_name, 'long_scalar', val_in);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
if (dump)
  disp(sprintf('  `--> wrote %d', double(val_in)));
end
disp('  `--> passed');
disp('-reading long_scalar');
val_out = tango_read_attribute(dev_name, 'long_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
  disp(val_out);
end
% if (val_in(1) ~= val_out.value(2)) 
%   disp('  `--> ERROR');
%   err =  -1; return;
% end
%--------------------------------------------------------------------------
disp('-writing float_scalar');
val_in = single(pi);
tango_write_attribute(dev_name, 'float_scalar', val_in);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
if (dump)
  disp(sprintf('  `--> wrote %d', val_in));
end
disp('  `--> passed');
disp('-reading float_scalar');
val_out = tango_read_attribute(dev_name, 'float_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
  disp(val_out);
end
% if (val_in(1) ~= val_out.value(2)) 
%   disp('  `--> ERROR');
%   err =  -1; return;
% end
%--------------------------------------------------------------------------
disp('-writing double_scalar');
val_in = pi;
tango_write_attribute(dev_name, 'double_scalar', pi);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
if (dump)
  disp(sprintf('  `--> wrote %d', val_in));
end
disp('  `--> passed');
disp('-reading double_scalar');
val_out = tango_read_attribute(dev_name, 'double_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
  disp(val_out);
end
% if (val_in(1) ~= val_out.value(2)) 
%   disp('  `--> ERROR');
%   err =  -1; return;
% end
%--------------------------------------------------------------------------
disp('-reading boolean_spectrum');
val = tango_read_attribute(dev_name, 'boolean_spectrum');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
  disp(val);
end
%--------------------------------------------------------------------------
disp('-reading uchar_spectrum');
val = tango_read_attribute(dev_name, 'uchar_spectrum');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
  disp(val);
end
%--------------------------------------------------------------------------
disp('-reading ushort_spectrum');
val = tango_read_attribute(dev_name, 'ushort_spectrum');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
  disp(val);
end
%--------------------------------------------------------------------------
disp('-reading short_spectrum');
val = tango_read_attribute(dev_name, 'short_spectrum');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
  disp(val);
end
%--------------------------------------------------------------------------
disp('-reading long_spectrum');
val = tango_read_attribute(dev_name, 'long_spectrum');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
  disp(val);
end
%--------------------------------------------------------------------------
disp('-reading float_spectrum');
val = tango_read_attribute(dev_name, 'float_spectrum');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
  disp(val);
end
%--------------------------------------------------------------------------
disp('-reading double_spectrum');
val = tango_read_attribute(dev_name, 'double_spectrum');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
  disp(val);
end
%--------------------------------------------------------------------------
disp('-reading boolean_image');
val = tango_read_attribute(dev_name, 'boolean_image');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump) disp(val); end
%--------------------------------------------------------------------------
disp('-reading uchar_image');
val = tango_read_attribute(dev_name, 'uchar_image');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump) disp(val); end
%--------------------------------------------------------------------------
disp('-reading ushort_image');
val = tango_read_attribute(dev_name, 'ushort_image');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump) disp(val); end
%--------------------------------------------------------------------------
disp('-reading short_image');
val = tango_read_attribute(dev_name, 'short_image');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump) disp(val); end
%--------------------------------------------------------------------------
disp('-reading long_image');
val = tango_read_attribute(dev_name, 'long_image');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
 disp(val);
end
%--------------------------------------------------------------------------
disp('-reading float_image');
val = tango_read_attribute(dev_name, 'float_image');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
  disp(val);
end
%--------------------------------------------------------------------------
disp('-reading double_image');
val = tango_read_attribute(dev_name, 'double_image');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
  disp(val);
end
%--------------------------------------------------------------------------
print_sub_title('READING/WRITTING ATTRIBUTES (GLOBALLY)');
%--------------------------------------------------------------------------
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
  err =  -1; return;
else
  disp('  `--> passed');
end
%--------------------------------------------------------------------------
disp('-testing tango_read_attributes (reading all attributes in one call)');
attr_list = {'short_scalar', 'long_scalar', 'double_scalar', ...
             'short_spectrum', 'long_spectrum', 'double_spectrum', ...
             'short_image', 'long_image', 'double_image'};
val = tango_read_attributes(dev_name, attr_list);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
  [m, n] = size(val);
  for i = 1:n
    disp(val(i));
  end
end
%--------------------------------------------------------------------------
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
  err =  -1; return;
else
  disp('  `--> tango_write_attributes_asynch passed');
end
tango_write_attributes_reply(req_desc, 0);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> tango_write_attributes_reply passed');
disp('  `--> passed');
disp('-testing tango_read_attributes_asynch (asynch. reading all attributes in one call)');
req_desc = tango_read_attributes_asynch(dev_name, attr_list);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> tango_read_attributes_asynch passed');
pause(0.1);
val = tango_read_attributes_reply(req_desc, 0);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> tango_read_attributes_reply passed');
disp('  `--> passed');
if (dump)
 [m, n] = size(val);
 for i = 1:n
   disp(val(i));
 end
end
%--------------------------------------------------------------------------
disp('-asynch. writing short_scalar');
val_in = int16(-1024);
req_desc = tango_write_attribute_asynch(dev_name, 'short_scalar', val_in);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> tango_write_attribute_asynch passed');
tango_write_attribute_reply(req_desc, 0);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> tango_write_attribute_reply passed');
disp('  `--> passed');
disp('-asynch. reading short_scalar');
req_desc = tango_read_attribute_asynch(dev_name, 'short_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> tango_read_attribute_asynch passed');
val = tango_read_attribute_reply(req_desc, 0);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> tango_read_attribute_reply passed');
disp('  `--> passed');
if (dump) disp(val); end
%--------------------------------------------------------------------------
disp('-asynch. reading short_spectrum');
req_desc = tango_read_attribute_asynch(dev_name, 'short_spectrum');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> tango_read_attribute_asynch passed');
val = tango_read_attribute_reply(req_desc, 0);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> tango_read_attribute_reply passed');
disp('  `--> passed');
if (dump) disp(val); end
%--------------------------------------------------------------------------
disp('-asynch. reading short_image');
req_desc = tango_read_attribute_asynch(dev_name, 'short_image');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> tango_read_attribute_asynch passed');
val = tango_read_attribute_reply(req_desc, 0);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> tango_read_attribute_reply passed');
disp('  `--> passed');
if (dump) disp(val); end
%--------------------------------------------------------------------------
err =  0;
return;

%==========================================================================
% FUNCTION test_polling
%==========================================================================
function err =  test_polling(dev_name, dump)
%--------------------------------------------------------------------------
print_sub_title('TESTING POLLING ORIENTED FUNCTIONS');
%--------------------------------------------------------------------------
disp('-testing tango_poll_command');
tango_poll_command(dev_name, 'DevVoid', 500);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('-testing tango_is_command_polled');
polled = tango_is_command_polled(dev_name, 'DevVoid');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
if (polled ~= 1)
  disp('  `--> ERROR::cmd DevVoid should be polled');
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('-testing tango_get_command_poll_period');
pp = tango_get_command_poll_period(dev_name, 'DevVoid');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
if (pp ~= 500)
  disp(sprintf('  `--> ERROR::polling period should be 500 ms - got %.2fms', pp));
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('-testing tango_poll_attribute');
tango_poll_attribute(dev_name, 'short_scalar', 250);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('-testing tango_is_attribute_polled');
polled = tango_is_attribute_polled(dev_name, 'short_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
if (polled ~= 1)
  disp('  `--> ERROR::attr short_scalar should be polled');
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('-testing tango_get_attribute_poll_period');
pp = tango_get_attribute_poll_period(dev_name, 'short_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
if (pp ~= 250)
  disp(sprintf('  `--> ERROR::polling period should be 250 ms - got %.2fms', pp));
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('-waiting for polling buffers to be filled');
pause(0.25);
disp(sprintf('%s', '  `--> #'));
pause(0.25);
disp(sprintf('%s', '  `--> # #'));
pause(0.25);
disp(sprintf('%s', '  `--> # # #'));
pause(0.25);
disp(sprintf('%s', '  `--> # # # #'));
%--------------------------------------------------------------------------
disp('-testing tango_polling_status');
ps = tango_polling_status(dev_name);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
  [m, n] = size(ps);
  for i = 1:n
    disp(ps(i));
  end
end
%--------------------------------------------------------------------------
disp('-testing tango_command_history');
hist = tango_command_history(dev_name, 'DevVoid', 5);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  %err =  -1; return;
else
  disp('  `--> passed');
  if (dump)
    [m, n] = size(hist);
    for i = 1:n
      disp(hist(i));
    end
  end
end
%--------------------------------------------------------------------------
disp('-testing tango_attribute_history');
hist = tango_attribute_history(dev_name, 'short_scalar', 5);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  %err =  -1; return;
else
  disp('  `--> passed');
  if (dump)
    [m, n] = size(hist);
    for i = 1:n
      disp(sprintf('\t-Attr.History(%d) ----------------\r', i));
      disp(hist(i));
      disp(sprintf('\t-Attr.History(%d).value ----------\r', i));
      disp(hist(i).value);
    end
  end
end
%--------------------------------------------------------------------------
disp('-testing tango_stop_poll_command');
tango_stop_poll_command(dev_name, 'DevVoid');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
polled = tango_is_command_polled(dev_name, 'DevVoid');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
if (polled ~= 0)
  disp('  `--> ERROR:: cmd DevVoid should NOT be polled');
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
disp('-testing tango_stop_poll_attribute');
tango_stop_poll_attribute(dev_name, 'short_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
polled = tango_is_attribute_polled(dev_name, 'short_scalar');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
if (polled ~= 0)
  disp('  `--> ERROR::attr short_scalar should NOT be polled');
  err =  -1; return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------
err =  0;
return;

%==========================================================================
% FUNCTION test_property
%==========================================================================
function err =  test_property (dev_name, dump)
%--------------------------------------------------------------------------
print_sub_title('TESTING PROPERTY ORIENTED FUNCTIONS');
%--------------------------------------------------------------------------
disp('-testing tango_get_property');
prop = tango_get_property(dev_name, 'mthreaded_impl');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
  disp(prop);
end
%--------------------------------------------------------------------------
disp('-testing tango_get_properties');
prop_list = {'mthreaded_impl', 'sleep_period', 'polled_attr'};
prop = tango_get_properties(dev_name, prop_list);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
if (dump)
  [m, n] = size(prop);
  for i = 1:n
    disp(prop(i));
  end
end
%--------------------------------------------------------------------------
disp('-testing tango_put_property');
tango_put_property(dev_name, 'dummy_0', {'dummy_0_value_1', 'dummy_0_value_2'});
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
prop = tango_get_property(dev_name, 'mthreaded_impl');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
if (dump)
  disp(prop);
end
%--------------------------------------------------------------------------
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
  err =  -1; return;
end
disp('  `--> passed');
clear prop
prop = {'mthreaded_impl', 'sleep_period'};
prop = tango_get_properties(dev_name, prop);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
if (dump)
  [m, n] = size(prop);
  for i = 1:n
    disp(prop(i));
  end
end
%--------------------------------------------------------------------------
disp('-testing tango_del_property');
tango_del_property(dev_name, 'dummy_0');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
clear prop
prop = {'dummy_0', 'dummy_1', 'dummy_2', 'dummy_3'};
prop = tango_get_properties(dev_name, prop);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
if (dump)
  [m, n] = size(prop);
  for i = 1:n
    disp(prop(i));
  end
end
%--------------------------------------------------------------------------
disp('-testing tango_del_properties');
tango_del_properties(dev_name, {'dummy_1', 'dummy_2', 'dummy_3'});
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
disp('  `--> passed');
clear prop
prop = {'dummy_1', 'dummy_2', 'dummy_3'};
prop = tango_get_properties(dev_name, prop);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err =  -1; return;
end
if (dump)
  [m, n] = size(prop);
  for i = 1:n
    disp(prop(i));
  end
end
%--------------------------------------------------------------------------
err = 0;
return;

%==========================================================================
% FUNCTION test_groups
%==========================================================================
function err = test_groups (dev_name, dump)
%--------------------------------------------------------------------------
print_sub_title('TESTING TANGO GROUPS');
%--------------------------------------------------------------------------
disp('-testing tango_group_create: test1');
test1_id = tango_group_create('test1');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------
disp('-testing tango_group_add: tango/tangotest/* -> test1');
dev_prefix='tango/tangotest/';
dev_list = {};
for i = 0:9
  dev = strcat(dev_prefix, num2str(i));
  dev_list = cat(2, dev_list, {dev});
end
tango_group_add(test1_id, dev_list);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------
%disp('-testing tango_group_remove: tango/tangotest/256 -> test1');
%tango_group_remove(test1_id, 'tango/tangotest/256', 0);
%if (tango_error == -1)
%  disp('  `--> ERROR');
%  tango_print_error_stack;
%  err = -1;
%  return;
%end
%disp('  `--> passed');
%--------------------------------------------------------
%disp('-testing tango_group_create: test2');
%test2_id = tango_group_create('test2');
%if (tango_error == -1)
%  disp('  `--> ERROR');
%  tango_print_error_stack;
%  err = -1;
%  return;
%end
%disp('  `--> passed');
%--------------------------------------------------------
%disp('-testing tango_group_add: tests/tangotest/* -> test2');
%tango_group_add(test2_id, 'tests/tangotest/*');
%if (tango_error == -1)
%  disp('  `--> ERROR');
%  tango_print_error_stack;
%  err = -1;
%  return;
%end
%disp('  `--> passed');
%--------------------------------------------------------
%disp('-testing tango_group_remove: tests/tangotest/djeremaille -> test2');
%tango_group_remove(test2_id, 'tests/tangotest/djeremaille', 0);
%tango_group_remove(test2_id, 'tests/tangotest/core*', 0);
%if (tango_error == -1)
%  disp('  `--> ERROR');
%  tango_print_error_stack;
%  err = -1;
%  return;
%end
%disp('  `--> passed');
%--------------------------------------------------------
disp('-testing tango_group_create: test');
test_id = tango_group_create('test');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------
disp('-testing tango_group_add: test1 -> test');
tango_group_add(test_id, test1_id);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------
%disp('-testing tango_group_add: test2 -> test');
%tango_group_add(test_id, test2_id);
%if (tango_error == -1)
%  disp('  `--> ERROR');
%  tango_print_error_stack;
%  err = -1;
%  return;
%end
%disp('  `--> passed');
%--------------------------------------------------------
disp('-testing tango_group_create: root');
root_id = tango_group_create('root');
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------
disp('-testing tango_group_add: test -> root');
tango_group_add(root_id, test_id);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err = -1;
  return;
end
disp('  `--> passed');
if (dump)
  disp('-testing tango_group_dump: root');
  tango_group_dump(root_id);
end
%--------------------------------------------------------
% disp('-testing tango_group_command_inout(cmd: Status) on group <root>');
% r = tango_group_command_inout(root_id, 'Status', 0, 1);
% if (tango_error == -1)
%   disp('  `--> ERROR');
%   tango_print_error_stack;
%   err = -1;
%   return;
% end
% if (r.has_failed && dump)
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
%--------------------------------------------------------
err = tangotest_group_command_inout (test1_id, 0, 10, dump);
if (err == -1)
  return;
end
%--------------------------------------------------------
%err = tangotest_group_command_inout (test_id, 1, 10, dump);
%if (err == -1)
%  return;
%end
%--------------------------------------------------------
err = tangotest_group_write_attribute (test1_id, 0, 10, dump);
if (err == -1)
  return;
end
%--------------------------------------------------------
%err = tangotest_group_write_attribute (test_id, 1, 10, dump);
%if (err == -1)
%  return;
%end
%--------------------------------------------------------
err = tangotest_group_read_attribute (test1_id, 10, dump);
if (err == -1)
  return;
end
%--------------------------------------------------------
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
%--------------------------------------------------------
% disp('-testing tango_group_add: ans*/*/bpm.* -> bpm');
% tango_group_add(bpm_id, 'ans*/*/bpm.*');
% if (tango_error == -1)
%   disp('  `--> ERROR');
%   tango_print_error_stack;
%   err = -1;
%   return;
% end
%--------------------------------------------------------
% disp('-testing tango_group_add: bpm -> root');
% tango_group_add(root_id, bpm_id);
% if (tango_error == -1)
%   disp('  `--> ERROR');
%   tango_print_error_stack;
%   err = -1;
%   return;
% end
%--------------------------------------------------------
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
%--------------------------------------------------------
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
%--------------------------------------------------------
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
%--------------------------------------------------------
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
%--------------------------------------------------------
% disp('-testing tango_group_remove (string)');
% tango_group_remove(root_id, 'tango/tangotest/1', 1);
% if (tango_error == -1)
%   disp('  `--> ERROR');
%   tango_print_error_stack;
%   err = -1;
%   return;
% end
%--------------------------------------------------------
% disp('-testing tango_group_remove (cell array)');
% tango_group_remove(root_id, {'tango/tangotest/2', 'tango/tangotest/3'}, 1);
% if (tango_error == -1)
%   disp('  `--> ERROR');
%   tango_print_error_stack;
%   err = -1;
%   return;
% end
%--------------------------------------------------------
% disp('-testing tango_group_remove (group_id)');
% tango_group_remove(root_id, root_id, 1);
% if (tango_error == -1)
%   disp('  `--> GOT EXPECTED ERROR');
% end
%--------------------------------------------------------
% disp('-testing tango_group_kill');
% tango_group_kill(test_id);
% if (tango_error == -1)
%   disp('  `--> ERROR');
%   tango_print_error_stack;
%   err = -1;
%   return;
% end
%--------------------------------------------------------
% disp('-testing tango_group_kill (group_id)');
% tango_group_kill(test4_id);
% if (tango_error == -1)
%   disp('  `--> ERROR');
%   tango_print_error_stack;
%   err = -1;
%   return;
% end
%--------------------------------------------------------
% disp('-testing tango_group_remove (group_id)');
% tango_group_remove(root_id, test3_id, 1);
% if (tango_error == -1)
%   disp('  `--> GOT EXPECTED ERROR');
% end
%--------------------------------------------------------
% disp('-testing tango_group_remove (group_id)');
% tango_group_remove(root_id, test2_id, 1);
% if (tango_error == -1)
%   disp('  `--> GOT EXPECTED ERROR');
% end
%--------------------------------------------------------
disp('-testing tango_group_kill');
tango_group_kill(root_id);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------
disp('-testing tango_group_kill');
tango_group_kill(root_id);
if (tango_error == -1)
  disp('  `--> GOT EXPECTED ERROR');
end
disp('  `--> passed');
%--------------------------------------------------------
disp('-testing tango_group_id');
root_id = tango_group_id('root');
if (tango_error == -1)
   disp('  `--> GOT EXPECTED ERROR');
end
disp('  `--> passed');
%--------------------------------------------------------
err = 0;
return;

%**********************************************************************************
function err = tangotest_group_command_inout (grp_id, fwd, n, dump)
%--------------------------------------------------------------------------------
fgt = 0;
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: Status)');
cmd = 'Status';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump) == -1) 
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevString)');
cmd = 'DevString';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, '1:a:2:b:3:c') == -1)
  err = -1;
  return;      
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevDouble)');
cmd = 'DevDouble';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, 3.14) == -1)
  err = -1;
  return;     
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevFloat)');
cmd = 'DevFloat';
work_around = 3.14;
work_around = single(work_around);
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, work_around) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevLong)');
cmd = 'DevLong';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, int32(1)) == -1)
  err = -1;
  return;      
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevULong)');
cmd = 'DevULong';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, uint32(1)) == -1)
  err = -1;
  return;     
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevShort)');
cmd = 'DevShort';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, int16(1)) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevUShort)');
cmd = 'DevUShort';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, uint16(1)) == -1)
  err = -1;
  return;   
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevBoolean)');
cmd = 'DevBoolean';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, uint8(1)) == -1)
  err = -1;
  return;     
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarStringArray)');
cmd = 'DevVarStringArray';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, {'0', '1', '2', '3'}) == -1)
  err = -1;
  return;      
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarDoubleArray)');
cmd = 'DevVarDoubleArray';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, [0, 1, 2, 3]) == -1)
  err = -1;
  return;     
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarFloatArray)');
cmd = 'DevVarFloatArray';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, single([0, 1, 2, 3])) == -1)
  err = -1;
  return;      
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarLongArray)');
cmd = 'DevVarLongArray';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, int32([0, 1, 2, 3])) == -1)
  err = -1;
  return;      
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarULongArray)');
cmd = 'DevVarULongArray';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, uint32([0, 1, 2, 3])) == -1)
  err = -1;
  return;      
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarShortArray)');
cmd = 'DevVarShortArray';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, int16([0, 1, 2, 3])) == -1)
  err = -1;
  return;      
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarUShortArray)');
cmd = 'DevVarUShortArray';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, uint16([0, 1, 2, 3])) == -1)
  err = -1;
  return;     
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarCharArray)');
cmd = 'DevVarCharArray';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, int8([0, 1, 2, 3])) == -1)
  err = -1;
  return;      
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVar{unsigned}CharArray)');
cmd = 'DevVarCharArray';
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, uint8([0, 1, 2, 3])) == -1)
  err = -1;
  return;     
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarLongStringArray)');
cmd = 'DevVarLongStringArray';
dvlsa.lvalue = int32([1,2,3,4]);
dvlsa.svalue = {'1','2','3','4'};
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, dvlsa) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarDoubleStringArray)');
cmd = 'DevVarDoubleStringArray';
dvdsa.dvalue = [1,2,3,4];
dvdsa.svalue = {'1','2','3','4'};
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, dvdsa) == -1)
  err = -1;
  return;     
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevString) - specific arg per device');
cmd = 'DevString';
argin = {};
for i = 1:n
  argin = cat(2, argin, {'astring'});
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, argin) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevDouble) - specific arg per device');
cmd = 'DevDouble';
argin = [];
for i = 1:n
  argin = cat(2, argin, i);
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, argin) == -1)
  err = -1;
  return;    
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevFloat) - specific arg per device');
cmd = 'DevFloat';
argin = single([]);
for i = 1:n
  argin = cat(2, argin, single(i));
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, argin) == -1)
  err = -1;
  return;      
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevLong) - specific arg per device');
cmd = 'DevLong';
argin = int32([]);
for i = 1:n
  argin = cat(2, argin, int32(i));
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, argin) == -1)
  err = -1;
  return;      
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevULong) - specific arg per device');
cmd = 'DevULong';
argin = uint32([]);
for i = 1:n
  argin = cat(2, argin, uint32(i));
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, argin) == -1)
  err = -1;
  return;   
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevShort) - specific arg per device');
cmd = 'DevShort';
argin = int16([]);
for i = 1:n
  argin = cat(2, argin, int16(i));
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, argin) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevUShort) - specific arg per device');
cmd = 'DevUShort';
argin = uint16([]);
for i = 1:n
  argin = cat(2, argin, uint16(i));
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, argin) == -1)
  err = -1;
  return;   
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevBoolean) - specific arg per device');
cmd = 'DevBoolean';
argin = uint8([]);
for i = 1:n
  argin = cat(2, argin, uint8(i));
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, argin) == -1)
  err = -1;
  return;      
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarStringArray) - specific arg per device');
cmd = 'DevVarStringArray';
argin = {};
for i = 1:n
  argin = cat(2, argin, {'a:0', 'b:1', 'b:2'});
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, argin) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarDoubleArray) - specific arg per device');
cmd = 'DevVarDoubleArray';
argin = {};
for i = 1:n
  argin = cat(2, argin, {[1:n]});
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, argin) == -1)
  err = -1;
  return;   
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarFloatArray) - specific arg per device');
cmd = 'DevVarFloatArray';
argin = {};
for i = 1:n
  argin = cat(2, argin, {single([1:n])});
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, argin) == -1)
  err = -1;
  return;     
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarLongArray) - specific arg per device');
cmd = 'DevVarLongArray';
argin = {};
for i = 1:n
  argin = cat(2, argin, {int32([1:n])});
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, argin) == -1)
  err = -1;
  return;      
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarULongArray) - specific arg per device');
cmd = 'DevVarULongArray';
argin = {};
for i = 1:n
  argin = cat(2, argin, {uint32([1:n])});
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, argin) == -1)
  err = -1;
  return;     
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarShortArray) - specific arg per device');
cmd = 'DevVarShortArray';
argin = {};
for i = 1:n
  argin = cat(2, argin, {int16([1:n])});
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, argin) == -1)
  err = -1;
  return;     
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarUShortArray) - specific arg per device');
cmd = 'DevVarUShortArray';
argin = {};
for i = 1:n
  argin = cat(2, argin, {uint16([1:n])});
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, argin) == -1)
  err = -1;
  return;   
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarCharArray) - specific arg per device');
cmd = 'DevVarCharArray';
argin = {};
for i = 1:n
  argin = cat(2, argin, {int8([1:n])});
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, argin) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVar{unsigned}CharArray) - specific arg per device');
cmd = 'DevVarCharArray';
argin = {};
for i = 1:n
  argin = cat(2, argin, {uint8([1:n])});
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, argin) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarLongStringArray) - specific arg per device');
cmd = 'DevVarLongStringArray';
dvlsa.lvalue = int32([1,2,3,4]);
dvlsa.svalue = {'1','2','3','4'};
argin = {};
for i = 1:n
  argin = cat(2, argin, {dvlsa});
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, argin) == -1)
  err = -1;
  return;     
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_command_inout(cmd: DevVarDoubleStringArray) - specific arg per device');
cmd = 'DevVarDoubleStringArray';
dvdsa.dvalue = [1,2,3,4];
dvdsa.svalue = {'1','2','3','4'};
argin = {};
for i = 1:n
  argin = cat(2, argin, {dvdsa});
end
if (test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, argin) == -1)
  err = -1;
  return;      
end;
disp('  `--> passed');
%--------------------------------------------------------------------------------
err = 0;
return;

%**********************************************************************************
% test_tango_group_command_inout
%**********************************************************************************
function err = test_tango_group_command_inout (grp_id, cmd, fgt, fwd, dump, varargin)
if (nargin == 5)
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
if (r.has_failed && dump)
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
function err = tangotest_group_write_attribute (grp_id, fwd, n, dump)
%--------------------------------------------------------------------------------
disp('-testing tango_group_write_attribute(attr: string_scalar)');
attr = 'string_scalar';
if (test_tango_group_write_attribute (grp_id, attr, fwd, dump, '1:a:2:b:3:c') == -1)
 err = -1;
 return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_write_attribute(attr: double_scalar)');
attr = 'double_scalar';
if (test_tango_group_write_attribute (grp_id, attr, fwd, dump, 3.14) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_write_attribute(attr: double_scalar_w)');
attr = 'double_scalar_w';
if (test_tango_group_write_attribute (grp_id, attr, fwd, dump, 3.14) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_write_attribute(attr: long_scalar)');
attr = 'long_scalar';
if (test_tango_group_write_attribute (grp_id, attr, fwd, dump, int32(256)) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_write_attribute(attr: long_scalar_w)');
attr = 'long_scalar_w';
if (test_tango_group_write_attribute (grp_id, attr, fwd, dump, int32(256)) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_write_attribute(attr: short_scalar)');
attr = 'short_scalar';
if (test_tango_group_write_attribute (grp_id, attr, fwd, dump, int16(128)) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_write_attribute(attr: short_scalar_w)');
attr = 'short_scalar_w';
if (test_tango_group_write_attribute (grp_id, attr, fwd, dump, int16(128)) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_write_attribute(attr: string_scalar) - specific arg per device');
attr = 'string_scalar';
value = {};
for i = 1:n
  value = cat(2, value, {'astring'});
end
if (test_tango_group_write_attribute (grp_id, attr, fwd, dump, value) == -1)
 err = -1;
 return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_write_attribute(attr: double_scalar) - specific arg per device');
attr = 'double_scalar';
value = [];
for i = 1:n
  value = cat(2, value,  i / 10);
end
if (test_tango_group_write_attribute (grp_id, attr, fwd, dump, value) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_write_attribute(attr: double_scalar_w) - specific arg per device');
attr = 'double_scalar_w';
value = [];
for i = 1:n
  value = cat(2, value,  i / 10);
end
if (test_tango_group_write_attribute (grp_id, attr, fwd, dump, value) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_write_attribute(attr: long_scalar) - specific arg per device');
attr = 'long_scalar';
value = int32([]);
for i = 1:n
  value = cat(2, value,  int32(i));
end
if (test_tango_group_write_attribute (grp_id, attr, fwd, dump, value) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_write_attribute(attr: long_scalar_w) - specific arg per device');
attr = 'long_scalar_w';
value = int32([]);
for i = 1:n
  value = cat(2, value,  int32(i));
end
if (test_tango_group_write_attribute (grp_id, attr, fwd, dump, value) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_write_attribute(attr: short_scalar) - specific arg per device');
attr = 'short_scalar';
value = int16([]);
for i = 1:n
  value = cat(2, value,  int16(i));
end
if (test_tango_group_write_attribute (grp_id, attr, fwd, dump, value) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_write_attribute(attr: short_scalar_w) - specific arg per device');
attr = 'short_scalar_w';
value = int16([]);
for i = 1:n
  value = cat(2, value,  int16(i));
end
if (test_tango_group_write_attribute (grp_id, attr, fwd, dump, value) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
err = 0;
return;

%**********************************************************************************
% test_tango_group_write_attribute
%**********************************************************************************
function err = test_tango_group_write_attribute (grp_id, attr, fwd, dump, value)
r = tango_group_write_attribute(grp_id, attr, fwd, value);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err = -1;
  return;
end
if (r.has_failed && dump)
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
function err = tangotest_group_read_attribute (grp_id, n, dump)
%--------------------------------------------------------------------------------
fwd = 1;
%--------------------------------------------------------------------------------
disp('-testing tango_group_read_attribute(attr: string_scalar)');
attr = 'string_scalar';
if (test_tango_group_read_attribute (grp_id, attr, fwd, dump) == -1)
 err = -1;
 return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_read_attribute(attr: double_scalar)');
attr = 'double_scalar';
if (test_tango_group_read_attribute (grp_id, attr, fwd, dump) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_read_attribute(attr: double_scalar_rww)');
attr = 'double_scalar_rww';
if (test_tango_group_read_attribute (grp_id, attr, fwd, dump) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_read_attribute(attr: long_scalar)');
attr = 'long_scalar';
if (test_tango_group_read_attribute (grp_id, attr, fwd, dump) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_read_attribute(attr: long_scalar_rww)');
attr = 'long_scalar_rww';
if (test_tango_group_read_attribute (grp_id, attr, fwd, dump) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_read_attribute(attr: short_scalar)');
attr = 'short_scalar';
if (test_tango_group_read_attribute (grp_id, attr, fwd, dump) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_read_attribute(attr: short_scalar_ro)');
attr = 'short_scalar_ro';
if (test_tango_group_read_attribute (grp_id, attr, fwd, dump) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_read_attribute(attr: short_scalar_rww)');
attr = 'short_scalar_rww';
if (test_tango_group_read_attribute (grp_id, attr, fwd, dump) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_read_attribute(attr: double_spectrum)');
attr = 'double_spectrum';
if (test_tango_group_read_attribute (grp_id, attr, fwd, dump) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_read_attribute(attr: long_spectrum)');
attr = 'long_spectrum';
if (test_tango_group_read_attribute (grp_id, attr, fwd, dump) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_read_attribute(attr: short_spectrum)');
attr = 'short_spectrum';
if (test_tango_group_read_attribute (grp_id, attr, fwd, dump) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
% %--------------------------------------------------------------------------------
% disp('-testing tango_group_read_attribute(attr: double_image)');
% attr = 'double_image';
% if (test_tango_group_read_attribute (grp_id, attr, fwd, dump) == -1)
%   err = -1;
%   return;
% end
% disp('  `--> passed');
% %--------------------------------------------------------------------------------
% disp('-testing tango_group_read_attribute(attr: long_image)');
% attr = 'long_image';
% if (test_tango_group_read_attribute (grp_id, attr, fwd, dump) == -1)
%   err = -1;
%   return;
% end
% disp('  `--> passed');
%--------------------------------------------------------------------------------
disp('-testing tango_group_read_attribute(attr: short_image)');
attr = 'short_image';
if (test_tango_group_read_attribute (grp_id, attr, fwd, dump) == -1)
  err = -1;
  return;
end
disp('  `--> passed');
%--------------------------------------------------------------------------------
err = 0;
return;

%**********************************************************************************
% test_tango_group_read_attribute
%**********************************************************************************
function err = test_tango_group_read_attribute (grp_id, attr, fwd, dump)
r = tango_group_read_attribute(grp_id, attr, fwd);
if (tango_error == -1)
  disp('  `--> ERROR');
  tango_print_error_stack;
  err = -1;
  return;
end
if (r.has_failed && dump)
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
l(1:80) = char('-');
disp(l);
disp(cat(2, '    ', sub_title));  
disp(l);
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


