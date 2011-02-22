function replies = tango_group_read_attribute (group_id, attr_name, forward)
%TANGO_GROUP_READ_ATTRBIUTE Read a single attribute on a TANGO group.
%
% Syntax:
% -------
%   replies = tango_group_read_attribute (group_id, attr_name, forward)
%
% Argin: group_id
% ---------------
%   |- type: 1-by-1 double array
%   |- desc: the group identifier
%
% Argin: attr_name
% ----------------
%   |- type: 1-by-n char array (string)
%   |- desc: the name of the attribute to write (1)
%
% Argin: forward
% --------------
%   |- type: 1-by-1 numeric array (i.e. any numeric type)
%   |- desc: if set to non-null value (true) the command is forwarded to sub-groups.
%
% Argout: replies
% ---------------
%   |- type: 1-by-n struct array
%   |- desc: the replies (i.e. acknowledges)
%
%  (1) - Attribute name
%
%     Must be the name of a 'writable' attribute.
%
% See also TANGO_GROUP_READ_ATTRIBUTE_ASYNCH

% Forward to the asynch version of <tango_group_read_attribute>
req_desc = tango_group_read_attribute_asynch(group_id, attr_name, forward);
if (tango_error == -1)
   replies = -1;
   return;
end
replies = tango_group_read_attribute_reply(req_desc, 0);
return;

