function all_devices_alive = tango_group_ping (group_id, forward)
%TANGO_GROUP_ID Returns the id of the group named <group_name>.
%
% Syntax:
% -------
%   all_devices_alive = tango_group_id (group_id, forward)
%
% Argin: group_id
% ---------------
%   |- type: 1-by-1 double array
%   |- desc: the group identifier
%
% Argin: forward
% --------------
%   |- type: 1-by-1 numeric array (i.e. any numeric type)
%   |- desc: if set to non-null value (true), the function pings 
%            all devices in the hierarchy (specified group and
%            its sub-groups). if set to 0, the function only pings the 
%            devices in the specified group.  
%
% Argout: all_devices_alive
% ------------------
%   |- type: 1-by-1 double array
%   |- desc: 1 (i.e true) means "all devices alive"
%            0 (i.e false) means "at least one device does not reply".
%
% Example:
% --------
%
% See also TANGO_GROUP_*

% TANGO_GROUP_PING cmd-id: 69
all_devices_alive = tango_binding(int16(69), group_id, forward);
return;
