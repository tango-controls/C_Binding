function varargout = tango_panel(varargin)
% TANGO_PANEL Application M-file for tango_panel.fig
%    FIG = TANGO_PANEL launch tango_panel GUI.
%    TANGO_PANEL('callback_name', ...) invoke the named callback.

% Last Modified by GUIDE v2.0 11-Sep-2002 14:40:29

global kTangoDB;
kTangoDB = 'sys/database/dbds';

if nargin == 0  % LAUNCH GUI

	fig = openfig(mfilename,'reuse');

	% Use system color scheme for figure:
	set(fig,'Color',get(0,'defaultUicontrolBackgroundColor'));

	% Generate a structure of handles to pass to callbacks, and store it. 
	handles = guihandles(fig);
	guidata(fig, handles);

	if nargout > 0
		varargout{1} = fig;
	end

elseif ischar(varargin{1}) % INVOKE NAMED SUBFUNCTION OR CALLBACK

	try
		if (nargout)
			[varargout{1:nargout}] = feval(varargin{:}); % FEVAL switchyard
		else
			feval(varargin{:}); % FEVAL switchyard
		end
	catch
		disp(lasterr);
	end

end


%| ABOUT CALLBACKS:
%| GUIDE automatically appends subfunction prototypes to this file, and 
%| sets objects' callback properties to call them through the FEVAL 
%| switchyard above. This comment describes that mechanism.
%|
%| Each callback subfunction declaration has the following form:
%| <SUBFUNCTION_NAME>(H, EVENTDATA, HANDLES, VARARGIN)
%|
%| The subfunction name is composed using the object's Tag and the 
%| callback type separated by '_', e.g. 'slider2_Callback',
%| 'figure1_CloseRequestFcn', 'axis1_ButtondownFcn'.
%|
%| H is the callback object's handle (obtained using GCBO).
%|
%| EVENTDATA is empty, but reserved for future use.
%|
%| HANDLES is a structure containing handles of components in GUI using
%| tags as fieldnames, e.g. handles.figure1, handles.slider2. This
%| structure is created at GUI startup using GUIHANDLES and stored in
%| the figure's application data using GUIDATA. A copy of the structure
%| is passed to each callback.  You can store additional information in
%| this structure at GUI startup, and you can change the structure
%| during callbacks.  Call guidata(h, handles) after changing your
%| copy to replace the stored original so that subsequent callbacks see
%| the updates. Type "help guihandles" and "help guidata" for more
%| information.
%|
%| VARARGIN contains any extra arguments you have passed to the
%| callback. Specify the extra arguments by editing the callback
%| property in the inspector. By default, GUIDE sets the property to:
%| <MFILENAME>('<SUBFUNCTION_NAME>', gcbo, [], guidata(gcbo))
%| Add any extra arguments after the last argument, before the final
%| closing parenthesis.



% --------------------------------------------------------------------
function varargout = lbox_device_Callback(h, eventdata, handles, varargin)
  prop = get(handles.lbox_device);
  selected_device = prop.String{prop.Value};
  command_list = tango_command_list_query(selected_device);
  if (tango_error == -1)
    tango_print_error_stack;
    return;
  end
  n = size(command_list, 2);
  for i = 1:n
    cmd_list(i) =  {[...
                     command_list(i).cmd_name, ' ', ... 
                     command_list(i).in_type, ' ', ... 
                     command_list(i).in_type_desc, ' ', ... 
                     command_list(i).out_type, ' ', ... 
                     command_list(i).out_type_desc, ... 
                   ]};
  end
  set(handles.lbox_cmd, 'String', cmd_list);


% --------------------------------------------------------------------
function varargout = etxt_filter_Callback(h, eventdata, handles, varargin)




% --------------------------------------------------------------------
function varargout = but_update_Callback(h, eventdata, handles, varargin)
  global kTangoDB;
  etxt_filter_prop = get(handles.etxt_filter);
  dev_list = tango_command_inout(kTangoDB, 'DbGetDeviceExportedList', etxt_filter_prop.String);
  set(handles.lbox_device, 'String', dev_list);


% --------------------------------------------------------------------
function varargout = rbut_attributes_Callback(h, eventdata, handles, varargin)




% --------------------------------------------------------------------
function varargout = rbut_blackbox_Callback(h, eventdata, handles, varargin)




% --------------------------------------------------------------------
function varargout = radiobutton1_Callback(h, eventdata, handles, varargin)




% --------------------------------------------------------------------
function varargout = lbox_cmd_Callback(h, eventdata, handles, varargin)

