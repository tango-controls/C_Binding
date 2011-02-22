#pragma rtGlobals = 2
#pragma version = 1.0
#pragma IgorVersion = 6.0

//==============================================================================
// soleil_axis_panel.ipf
//------------------------------------------------------------------------------
// N.Leclercq - SOLEIL
//==============================================================================

//==============================================================================
// DEPENDENCIES
//==============================================================================
#include "tango"
 
//------------------------------------------------------------------------
// SOME PRIVATE CONSTs
//------------------------------------------------------------------------
static strconstant kAXIS_KEYWORD		= "AXIS"
static strconstant kDEVICE_KEYWORD	= "DEV"
static strconstant kACID_KEYWORD		= "ACID"
static strconstant kSCID_KEYWORD		= "SCID"
static strconstant kFLSCID_KEYWORD	= "FCID"
static strconstant kBLSCID_KEYWORD	= "BCID"
//------------------------------------------------------------------------
static constant kAXIS_WIDGET_W		= 330  
static constant kAXIS_WIDGET_H		= 180
static constant kAXIS_WIDGET_OFFSET	= 10 
//------------------------------------------------------------------------
static strconstant kSTATE_ATTR	= "State"
static strconstant kPOS_ATTR		= "position" 
static strconstant kBLS_ATTR		= "backwardLimitSwitch" 
static strconstant kFLS_ATTR		= "forwardLimitSwitch" 
//------------------------------------------------------------------------
static strconstant kCTRL_GBOX		= "axis_gb_" 
static strconstant kCTRL_SETPOS	= "axis_spos_"
static strconstant kCTRL_GETPOS	= "axis_gpos_"
static strconstant kCTRL_DEFPOS	= "axis_dpos_"
static strconstant kCTRL_STATE	= "axis_state_"
static strconstant kCTRL_GOTO		= "axis_goto_"
static strconstant kCTRL_STOP		= "axis_stop_"
static strconstant kCTRL_BCKWD	= "axis_bckwd_"
static strconstant kCTRL_FWD		= "axis_fwd_"
static strconstant kCTRL_TAB		= "axis_tab_"
static strconstant kCTRL_BLS		= "axis_bls_"
static strconstant kCTRL_FLS		= "axis_fls_"
static strconstant kCTRL_CHOFF	= "axis_choff_"
static strconstant kCTRL_CPOFF	= "axis_cpoff_"
static strconstant kCTRL_CHACC	= "axis_chacc_"
static strconstant kCTRL_IRP   	= "axis_irp_"
static strconstant kCTRL_ONOFF	= "axis_onoff_"
static strconstant kCTRL_IDEV		= "axis_idev_"
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// just an example showing how to use the generic axis panel
//------------------------------------------------------------------------
//function axis_test ()
//	Variable n_axis
//	//- build axis list (2 axis in this example - can contain up to 16 axis)
//	//- we use a temp wave of strings containing axis-label/device-name pairs
//	Make/O/T/N=(2, 2) axis_list_tmp
//	//- axis label of the first axis
//	axis_list_tmp[0][0] = "Axis.D"
//	//- device name of the of the first axis
//	axis_list_tmp[0][1] = "galil/test_v2/ga.D"
//	//- axis label of the second axis
//	axis_list_tmp[1][0] = "Axis.E"
//	//- device name of the of the second axis
//	axis_list_tmp[1][1] = "galil/test_v2/ga.E"
//	//- open the panel passing both the the axis list and (optionally) a widow name
//	axis_open_panels(axis_list_tmp, win_title="Axis Panel Example")
//	//- kill the temp wave
//	KillWaves/Z axis_list_tmp
//end
//------------------------------------------------------------------------
// function: axis_open_panel - one axis shortcut to axis_open_panels [PUBLIC]
//------------------------------------------------------------------------
function axis_open_panel (device_name, [alias])
	//- axis device name
	String device_name
	//- axis alias
	String alias
	//- optional window name
	String win_name
	//- optional window title
	String win_title
	Make/O/T/N=(1, 2) axis_list_tmp
	if (ParamIsDefault(alias))
		axis_list_tmp[0][0] = "Axis"
	else
		axis_list_tmp[0][0] = alias
	endif
	axis_list_tmp[0][1] = device_name
	Variable error
	error = axis_open_panels(axis_list_tmp)
	KillWaves/Z axis_list_tmp
	return error
end
//------------------------------------------------------------------------
// function: axis_open_panels [PUBLIC]
//------------------------------------------------------------------------
function axis_open_panels (axis_list, [win_name, win_title])
	//- axis device name/alias
	Wave/T axis_list
	//- optional window name
	String win_name
	//- optional window title
	String win_title
	//- panel window name
	if (ParamIsDefault(win_name))
		win_name = UniqueName("axis_panel_", 9, 0)
	endif
	//- number of axis
	Variable num_axis = DimSize(axis_list, 0)
	//- make sure its a 2D wave
	if (! num_axis || WaveDims(axis_list) != 2)
	  //-todo : error handling
	  return kERROR
	endif
	//- can host up to 16 axis
	if (num_axis > 16)
	  //-todo : error handling
	  return kERROR
	endif
	//- compute panel size
	Variable wsize_w 
	wsize_w  = kAXIS_WIDGET_OFFSET 
	wsize_w += ceil(num_axis / 4) * (kAXIS_WIDGET_W + kAXIS_WIDGET_OFFSET)
	Variable wsize_h
	wsize_h  = kAXIS_WIDGET_OFFSET 
	wsize_h += min(num_axis, 4) * (kAXIS_WIDGET_H + kAXIS_WIDGET_OFFSET) 
	//- close the panel if it already exists
	DoWindow/K $win_name
	//- open the panel with the right size and name
	NewPanel /K=1 /W=(150, 150, 150 + wsize_w, 150 + wsize_h) /N=$win_name
	ModifyPanel /W=$win_name frameStyle=4, fixedSize=1
	SetWindow $win_name hook(main)=axis_panel_hook, hookevents=1
	//- set panel title
	if (ParamIsDefault(win_title))
		win_title = "Axis panel"
	endif
	DoWindow/T $win_name win_title
	//- win note string
	String win_note = ""
	SetWindow $win_name note=win_note
	//- for each axis: add the associated controls
	Variable i = 0
	String keyword
	for (i = 0; i < num_axis; i += 1)
		if (axis_controls(win_name, i, axis_list[i][0], axis_list[i][1]) == kERROR)
			DoWindow/K $win_name
			return kERROR		
		endif
		keyword = kAXIS_KEYWORD + num2str(i)
		tools_win_note_set_str_val(win_name, keyword, axis_list[i][1])
	endfor
	return kNO_ERROR
end
//------------------------------------------------------------------------
// static function: axis_panel_hook [PRIVATE]
//------------------------------------------------------------------------
function axis_panel_hook (hs)
	STRUCT WMWinHookStruct &hs
	switch(hs.eventCode)
		//- event: kill
		case 2:
			axis_panel_kill_all(hs.winName)
			break
		default:
			break
	endswitch		
end 
//------------------------------------------------------------------------
// static function: axis_panel_kill_all [PRIVATE]
//------------------------------------------------------------------------
static function axis_panel_kill_all (win_name)
	String win_name
	Variable i = 0
	String keyword
	String device
	String cur_df
	Variable cid
	do
		keyword = kAXIS_KEYWORD + num2str(i)
		device = tools_win_note_get_str_val(win_name, keyword)
		if (! strlen(device))
			break
		endif 
		keyword = kACID_KEYWORD + num2str(i)
		cid = tools_win_note_get_num_val(win_name, keyword) 
		tango_monitor_stop(device, kPOS_ATTR, cid = cid)
		keyword = kSCID_KEYWORD + num2str(i)
		cid = tools_win_note_get_num_val(win_name, keyword)
		tango_monitor_stop(device, kSTATE_ATTR, cid = cid)
		keyword = kBLSCID_KEYWORD + num2str(i)
		cid = tools_win_note_get_num_val(win_name, keyword)
		tango_monitor_stop(device, kBLS_ATTR, cid = cid)
		keyword = kFLSCID_KEYWORD + num2str(i)
		cid = tools_win_note_get_num_val(win_name, keyword)
		tango_monitor_stop(device, kFLS_ATTR, cid = cid)
		tango_enter_device_df(device, prev_df = cur_df)
		tools_df_delete(GetDataFolder(1) + "controls")
		tango_leave_df(cur_df)
		i += 1
	while (1)			
end 
//------------------------------------------------------------------------
// static function: axis_controls [PRIVATE]
//------------------------------------------------------------------------
static function axis_controls (win_name, axis_id, axis_alias, axis_device)
	String win_name
	Variable axis_id
	String axis_alias
	String axis_device
	String axis_id_str = num2str(axis_id)
	String user_data 
	Variable left_offset = trunc(axis_id/4) * (kAXIS_WIDGET_W + kAXIS_WIDGET_OFFSET)
	Variable top_offset = mod(axis_id,4) * (kAXIS_WIDGET_H + kAXIS_WIDGET_OFFSET)
	Variable pos_left = 10 + left_offset
	Variable pos_top = 10 + top_offset
	String ctrl_name = kCTRL_GBOX	 + axis_id_str
	String gb_tiltle = "Axis: "
	if (strlen(axis_alias))
		gb_tiltle += axis_alias + " [" + axis_device + "]"
	else
		gb_tiltle += axis_device
	endif
	GroupBox $ctrl_name, win=$win_name, pos={pos_left, pos_top}
	GroupBox $ctrl_name, win=$win_name, size={330,180}, fstyle=1
	GroupBox $ctrl_name, win=$win_name, title=gb_tiltle
	pos_left = 20 + left_offset
	pos_top = 30 + top_offset
	ctrl_name = kCTRL_TAB + axis_id_str
	TabControl $ctrl_name, win=$win_name, pos={pos_left, pos_top} 
	TabControl $ctrl_name, win=$win_name, size={310,150}
	TabControl $ctrl_name, win=$win_name, tabLabel(0)="Motion Ctrl."
	TabControl $ctrl_name, win=$win_name, tabLabel(1)="Misc. Actions"
	TabControl $ctrl_name, win=$win_name, labelBack=(56576,56576,56576)
	TabControl $ctrl_name, win=$win_name, value=0
	TabControl $ctrl_name, win=$win_name, proc=axis_tab_cb
	TabControl $ctrl_name, win=$win_name, userdata(axis)=num2str(axis_id)
	TabControl $ctrl_name, win=$win_name, userdata(device)=axis_device
	TabControl $ctrl_name, win=$win_name, userdata(df)=GetDataFolder(1)
	String cur_df
	if (tango_enter_device_df(axis_device, prev_df = cur_df) == kERROR)
	  return kERROR
	endif
	NewDataFolder/O/S controls
 	pos_left = 297 + left_offset
	pos_top = 21 + top_offset
	Variable/G axis_state = 13
	Variable/G axis_last_state = -1
	Variable/G axis_state_dv = 13
	String/G axis_state_str = "UNKNOWN"
	String cstate_path = GetDataFolder(1) + "axis_state"
	String lstate_path = GetDataFolder(1) + "axis_last_state"
	String state_str_path = GetDataFolder(1) + "axis_state_str"
	ctrl_name = kCTRL_STATE + axis_id_str
	TitleBox $ctrl_name, win=$win_name, pos={pos_left, pos_top}, size={0,0} 
	TitleBox $ctrl_name, win=$win_name, fStyle=1
	TitleBox $ctrl_name, win=$win_name, labelBack=(65280,65280,16384)
	TitleBox $ctrl_name, win=$win_name, variable=axis_state_str
	TitleBox $ctrl_name, win=$win_name, anchor=MC, frame=5
	TitleBox $ctrl_name, win=$win_name, userdata(cstate)=cstate_path
	String formula = "axis_state_cb(" + cstate_path + ",\"" + lstate_path 
	formula += "\",\"" + state_str_path + "\",\"" + win_name
	formula += "\",\"" + ctrl_name + "\"," + num2str(axis_id) +")"
	SetFormula  axis_state_dv, formula  
	Variable cid = tango_monitor_start(axis_device, kSTATE_ATTR, cstate_path, 250)
	if (cid == kERROR)
	  tango_display_error()
	  return kERROR
	endif
	String keyword = kSCID_KEYWORD + axis_id_str
	tools_win_note_set_num_val(win_name, keyword, cid)
	Variable/G axis_pos
	Variable/G axis_pos_dv
	pos_left = 32 + left_offset
	pos_top = 70 + top_offset
	ctrl_name = kCTRL_GETPOS + axis_id_str
	SetVariable $ctrl_name, win=$win_name, pos={pos_left, pos_top}
	SetVariable $ctrl_name, win=$win_name, size={230,16}
	SetVariable $ctrl_name, win=$win_name, limits={0,0,0}, noedit=1
	SetVariable $ctrl_name, win=$win_name, title=" "
	SetVariable $ctrl_name, win=$win_name, help={"Current axis position"}
	SetVariable $ctrl_name, win=$win_name, format="%.4f UU", limits={0,0,0}
	SetVariable $ctrl_name, win=$win_name, value=$(GetDataFolder(1) + "axis_pos")
	SetVariable $ctrl_name, win=$win_name, userdata(axis)=num2str(axis_id)
	SetVariable $ctrl_name, win=$win_name, userdata(device)=axis_device
	SetVariable $ctrl_name, win=$win_name, userdata(df)=GetDataFolder(1)
	String pos_path = GetDataFolder(1) + "axis_pos"
	formula = "axis_pos_cb(" + pos_path + ",\"" + cstate_path + "\",\"" + win_name
	formula += "\",\"" +  kCTRL_STATE + axis_id_str + "\")"
	SetFormula axis_pos_dv, formula 
	cid = tango_monitor_start(axis_device, kPOS_ATTR, GetDataFolder(1) + "axis_pos", 250)
	if (cid == kERROR)
	  tango_display_error()
	  return kERROR
	endif
	keyword = kACID_KEYWORD + axis_id_str
	tools_win_note_set_num_val(win_name, keyword, cid)
	pos_left = 268 + left_offset
	pos_top = 68 + top_offset
	ctrl_name = kCTRL_DEFPOS + axis_id_str
	Button $ctrl_name, win=$win_name, pos={pos_left, pos_top}
	Button $ctrl_name, win=$win_name, size={50,20}
	Button $ctrl_name, win=$win_name, title="Def.Pos."
	Button $ctrl_name, win=$win_name, help={"Define axis position [i.e. actual/real position]"}
	Button $ctrl_name, win=$win_name, userdata(axis)=num2str(axis_id)
	Button $ctrl_name, win=$win_name, userdata(device)=axis_device
	Button $ctrl_name, win=$win_name, userdata(df)=GetDataFolder(1)
	Button $ctrl_name, win=$win_name, proc=axis_defpos_cb
	pos_left = 32 + left_offset
	pos_top = 105 + top_offset
	ctrl_name = kCTRL_GOTO + axis_id_str
	Button $ctrl_name, win=$win_name, pos={pos_left, pos_top}
	Button $ctrl_name, win=$win_name, size={50,20}
	Button $ctrl_name, win=$win_name, title="Go to"
	Button $ctrl_name, win=$win_name, help={"Send axis to specified position"}
	Button $ctrl_name, win=$win_name, userdata(axis)=num2str(axis_id)
	Button $ctrl_name, win=$win_name, userdata(device)=axis_device
	Button $ctrl_name, win=$win_name, userdata(df)=GetDataFolder(1)
	Button $ctrl_name, win=$win_name, proc=axis_goto_cb
	Variable/G axis_goto = 0
	pos_left = 89 + left_offset
	pos_top = 107 + top_offset
	ctrl_name = kCTRL_SETPOS + axis_id_str
	SetVariable $ctrl_name, win=$win_name, pos={pos_left, pos_top}
	SetVariable $ctrl_name, win=$win_name, size={228,16} 
	SetVariable $ctrl_name, win=$win_name, title=" "
	SetVariable $ctrl_name, win=$win_name, help={"Specify the axis position here then use 'Go to' button"}
	SetVariable $ctrl_name, win=$win_name, value=$(GetDataFolder(1) + "axis_goto")
	SetVariable $ctrl_name, win=$win_name, format="%.4f UU", limits={-Inf,Inf,0.1}
	SetVariable $ctrl_name, win=$win_name, userdata(axis)=num2str(axis_id)
	SetVariable $ctrl_name, win=$win_name, userdata(device)=axis_device
	SetVariable $ctrl_name, win=$win_name, userdata(df)=GetDataFolder(1)
	pos_left = 86 + left_offset
	pos_top = 145 + top_offset
	ctrl_name = kCTRL_BCKWD + axis_id_str
	Button $ctrl_name, win=$win_name, pos={pos_left, pos_top}
	Button $ctrl_name, win=$win_name, size={50,20}
	Button $ctrl_name, win=$win_name, title="<<"
	Button $ctrl_name, win=$win_name, help={"Start a continous backward motion"}
	Button $ctrl_name, win=$win_name, userdata(axis)=num2str(axis_id)
	Button $ctrl_name, win=$win_name, userdata(device)=axis_device
	Button $ctrl_name, win=$win_name, userdata(df)=GetDataFolder(1)
	Button $ctrl_name, win=$win_name, proc=axis_backward_cb
	pos_left = 150 + left_offset
	ctrl_name = kCTRL_STOP + axis_id_str
	Button $ctrl_name, win=$win_name, pos={pos_left, pos_top}
	Button $ctrl_name, win=$win_name, size={50,20}
	Button $ctrl_name, win=$win_name, title="Stop"
	Button $ctrl_name, win=$win_name, help={"Stop the current motion"}
	Button $ctrl_name, win=$win_name, userdata(axis)=num2str(axis_id)
	Button $ctrl_name, win=$win_name, userdata(device)=axis_device
	Button $ctrl_name, win=$win_name, userdata(df)=GetDataFolder(1)
	Button $ctrl_name, win=$win_name, proc=axis_stop_cb
	pos_left = 213 + left_offset
	ctrl_name = kCTRL_FWD + axis_id_str
	Button $ctrl_name, win=$win_name, pos={pos_left, pos_top}
	Button $ctrl_name, win=$win_name, size={50,20}
	Button $ctrl_name, win=$win_name, title=">>"
	Button $ctrl_name, win=$win_name, help={"Start a continous forward motion"}
	Button $ctrl_name, win=$win_name, userdata(axis)=num2str(axis_id)
	Button $ctrl_name, win=$win_name, userdata(device)=axis_device
	Button $ctrl_name, win=$win_name, userdata(df)=GetDataFolder(1)
	Button $ctrl_name, win=$win_name, proc=axis_forward_cb
	Variable/G axis_bls = 0
	pos_left = 32 + left_offset
	pos_top = 148 + top_offset
	ctrl_name = kCTRL_BLS + axis_id_str
	CheckBox $ctrl_name, win=$win_name, pos={pos_left, pos_top}
	CheckBox $ctrl_name, win=$win_name, size={38,14}
	CheckBox $ctrl_name, win=$win_name, title="BLS", fstyle=1
	CheckBox $ctrl_name, win=$win_name, help={"Backward limit switch status"}
	CheckBox $ctrl_name, win=$win_name, value=axis_bls, disable=2
	cid = tango_monitor_start(axis_device, kBLS_ATTR, GetDataFolder(1) + "axis_bls", 250)
	if (cid == kERROR)
	  tango_display_error()
	  return kERROR
	endif
	keyword = kBLSCID_KEYWORD + axis_id_str
	tools_win_note_set_num_val(win_name, keyword, cid)
	Variable/G axis_fls = 0
	pos_left = 280 + left_offset
	pos_top = 148 + top_offset
	ctrl_name = kCTRL_FLS + axis_id_str
	CheckBox $ctrl_name, win=$win_name, pos={pos_left, pos_top}
	CheckBox $ctrl_name, win=$win_name, size={38,14}
	CheckBox $ctrl_name, win=$win_name, title="FLS", fstyle=1
	CheckBox $ctrl_name, win=$win_name, help={"Forward Limit Switch Status"}
	CheckBox $ctrl_name, win=$win_name, value=axis_fls, disable=2
	cid = tango_monitor_start(axis_device, kFLS_ATTR, GetDataFolder(1) + "axis_fls", 250)
	if (cid == kERROR)
	  tango_display_error()
	  return kERROR
	endif
	keyword = kFLSCID_KEYWORD + axis_id_str
	tools_win_note_set_num_val(win_name, keyword, cid)
	pos_left = 77 + left_offset
	pos_top = 55 + top_offset
	ctrl_name = kCTRL_CHOFF + axis_id_str
	Button $ctrl_name, win=$win_name, pos={pos_left, pos_top}
	Button $ctrl_name, win=$win_name, size={98,20}, disable=1 
	Button $ctrl_name, win=$win_name, title="Change Offset..."
	Button $ctrl_name, win=$win_name, userdata(axis)=num2str(axis_id)
	Button $ctrl_name, win=$win_name, userdata(device)=axis_device
	Button $ctrl_name, win=$win_name, userdata(df)=GetDataFolder(1)
	Button $ctrl_name, win=$win_name, proc=axis_changeoffset_cb
	pos_left = 179 + left_offset
	ctrl_name = kCTRL_CPOFF + axis_id_str
	Button $ctrl_name, win=$win_name, pos={pos_left, pos_top}
	Button $ctrl_name, win=$win_name, size={98,20}, disable=1 
	Button $ctrl_name, win=$win_name, title="Compute Offset..."
	Button $ctrl_name, win=$win_name, userdata(axis)=num2str(axis_id)
	Button $ctrl_name, win=$win_name, userdata(device)=axis_device
	Button $ctrl_name, win=$win_name, userdata(df)=GetDataFolder(1)
	Button $ctrl_name, win=$win_name, proc=axis_computeoffset_cb
	pos_left = 77 + left_offset
	pos_top = 79 + top_offset
	ctrl_name = kCTRL_CHACC + axis_id_str
	Button $ctrl_name, win=$win_name, pos={pos_left, pos_top}
	Button $ctrl_name, win=$win_name, size={200,20}, disable=1 
	Button $ctrl_name, win=$win_name, title="Change Motion Accuracy..."
	Button $ctrl_name, win=$win_name, userdata(axis)=num2str(axis_id)
	Button $ctrl_name, win=$win_name, userdata(device)=axis_device
	Button $ctrl_name, win=$win_name, userdata(df)=GetDataFolder(1)
	Button $ctrl_name, win=$win_name, proc=axis_changeaccuracy_cb
	pos_top = 103 + top_offset
	ctrl_name = kCTRL_IRP + axis_id_str
	Button $ctrl_name, win=$win_name, pos={pos_left, pos_top}
	Button $ctrl_name, win=$win_name, size={200,20}, disable=1 
	Button $ctrl_name, win=$win_name, title="Initialize Reference Position"
	Button $ctrl_name, win=$win_name, userdata(axis)=num2str(axis_id)
	Button $ctrl_name, win=$win_name, userdata(device)=axis_device
	Button $ctrl_name, win=$win_name, userdata(df)=GetDataFolder(1)
	Button $ctrl_name, win=$win_name, proc=axis_initrefpos_cb
	pos_top = 127 + top_offset
	ctrl_name = kCTRL_ONOFF + axis_id_str
	Button $ctrl_name, win=$win_name, pos={pos_left, pos_top}
	Button $ctrl_name, win=$win_name, size={200,20}, disable=1 
	Button $ctrl_name, win=$win_name, title="Switch Axis On/Off"
	Button $ctrl_name, win=$win_name, userdata(axis)=num2str(axis_id)
	Button $ctrl_name, win=$win_name, userdata(device)=axis_device
	Button $ctrl_name, win=$win_name, userdata(df)=GetDataFolder(1)
	Button $ctrl_name, win=$win_name, proc=axis_switchonoff_cb
	pos_top = 151 + top_offset
	ctrl_name = kCTRL_IDEV + axis_id_str
	Button $ctrl_name, win=$win_name, pos={pos_left, pos_top}
	Button $ctrl_name, win=$win_name, size={200,20}, disable=1 
	Button $ctrl_name, win=$win_name, title="Init. device"
	Button $ctrl_name, win=$win_name, userdata(axis)=num2str(axis_id)
	Button $ctrl_name, win=$win_name, userdata(device)=axis_device
	Button $ctrl_name, win=$win_name, userdata(df)=GetDataFolder(1)
	Button $ctrl_name, win=$win_name, proc=axis_initdevice_cb
	tango_leave_df(cur_df)
	return kNO_ERROR
end
//------------------------------------------------------------------------
// function: axis_pos_cb [PRIVATE]
//------------------------------------------------------------------------
function axis_pos_cb (pos_val, state_path, win_name, ctrl_name)
	Variable pos_val
	String state_path
	String win_name
	String ctrl_name
	if (numtype(pos_val) != 0)
		NVAR/Z state = $state_path
		if (NVAR_Exists(state))
			state = kDeviceStateUNKNOWN
		endif
	endif
	return kNO_ERROR
end
//------------------------------------------------------------------------
// function: axis_state_cb [PRIVATE]
//------------------------------------------------------------------------
function axis_state_cb (current_state, last_state_path, state_str_path, win_name, ctrl_name, axis_id)
	Variable current_state
	String last_state_path
	String state_str_path
	String win_name
	String ctrl_name
	Variable axis_id
	if (numtype(current_state) != 0)
		current_state = kDeviceStateUNKNOWN
	endif
	NVAR/Z last_state = $last_state_path
	if (NVAR_Exists(last_state) && last_state == current_state)
		return kNO_ERROR
	endif
	SVAR/Z state_str = $state_str_path
	if (SVAR_Exists(state_str))
		state_str = tango_get_state_str(current_state)
	endif
	Variable r, g, b
	tango_get_state_color(current_state, r, g, b)
	TitleBox/Z $ctrl_name, win=$win_name, labelBack=(r,g,b)
	switch (current_state)
		case kDeviceStateOFF:
			ctrl_name = kCTRL_TAB + num2str(axis_id)
			axis_switch_tab(win_name, ctrl_name, 1)
			ctrl_name = kCTRL_ONOFF + num2str(axis_id)
			Button $ctrl_name, win=$win_name, title="Switch Axis On" 
			axis_disable_widgets(win_name, axis_id)
			break
		case kDeviceStateMOVING:
			ctrl_name = kCTRL_TAB + num2str(axis_id)
			TabControl $ctrl_name, win=$win_name, value=0
			axis_disable_widgets(win_name, axis_id)
			break
		default:
			ctrl_name = kCTRL_ONOFF + num2str(axis_id)
			Button $ctrl_name, win=$win_name, title="Switch Axis Off" 
			axis_enable_widgets(win_name, axis_id)
			break
	endswitch
	if (NVAR_Exists(last_state))
		last_state = current_state
	endif
end
//------------------------------------------------------------------------
// fonction : axis_forward_cb [PRIVATE]
//------------------------------------------------------------------------
function axis_forward_cb (ba)
	Struct WMButtonAction &ba
	if (ba.eventCode != 2)
		return kNO_ERROR
	endif
	String device = GetUserData(ba.win, ba.ctrlName, "device")
	axis_ctrl_forward(device)	
end
//------------------------------------------------------------------------
// fonction : axis_backward_cb [PRIVATE]
//------------------------------------------------------------------------
function axis_backward_cb (ba)
	Struct WMButtonAction &ba
	if (ba.eventCode != 2)
		return kNO_ERROR
	endif
	String device = GetUserData(ba.win, ba.ctrlName, "device")
	axis_ctrl_Backward(device)	
end
//------------------------------------------------------------------------
// fonction : axis_stop_cb [PRIVATE]
//------------------------------------------------------------------------
function axis_stop_cb (ba)
	Struct WMButtonAction &ba
	if (ba.eventCode != 2)
		return kNO_ERROR
	endif
	String device = GetUserData(ba.win, ba.ctrlName, "device")
	axis_ctrl_Stop(device)	
end
//------------------------------------------------------------------------
// fonction : axis_goto_cb [PRIVATE]
//------------------------------------------------------------------------
function axis_goto_cb (ba)
	Struct WMButtonAction &ba
	if (ba.eventCode != 2)
		return kNO_ERROR
	endif
	String axis = GetUserData(ba.win, ba.ctrlName, "axis")
	String device = GetUserData(ba.win, ba.ctrlName, "device")
	ControlInfo /W=$(ba.win) $(kCTRL_SETPOS + axis)
	axis_ctrl_SetPosition(device, V_Value)	
end
//------------------------------------------------------------------------
// fonction : axis_defpos_cb [PRIVATE]
//------------------------------------------------------------------------
function axis_defpos_cb (ba)
	Struct WMButtonAction &ba
	if (ba.eventCode != 2)
		return kNO_ERROR
	endif
	Variable actual_pos
	Prompt actual_pos, "Enter new position [in User Units]"
	DoPrompt "Axis Define Postion", actual_pos
	if (V_flag == 1)
		return kNO_ERROR
	endif
	String device = GetUserData(ba.win, ba.ctrlName, "device")
	if (axis_ctrl_DefinePosition(device, actual_pos) != kERROR)
		String path = GetUserData(ba.win, ba.ctrlName, "df")
		NVAR/Z axis_goto = $(path + "axis_goto")
		if (NVAR_Exists(	axis_goto))
			axis_goto = actual_pos
		endif
	endif
end
//------------------------------------------------------------------------
// fonction : axis_changeoffset_cb [PRIVATE]
//------------------------------------------------------------------------
function axis_changeoffset_cb (ba)
	Struct WMButtonAction &ba
	if (ba.eventCode != 2)
		return kNO_ERROR
	endif
	DoAlert 1, "About to change the axis position offset.\nContinue?"
	if (V_flag == 2)
		return kNO_ERROR
	endif
	String device = GetUserData(ba.win, ba.ctrlName, "device")
	Variable current_offset
	if (axis_ctrl_GetOffset(device, current_offset))
		return kERROR
	endif
	Variable new_offset = current_offset
	Prompt new_offset, "Enter new offset [in User Units]"
	DoPrompt "Change axis offset...", new_offset
	if (V_flag == 1)
		return kNO_ERROR
	endif
	axis_ctrl_SetOffset(device, new_offset)
end
//------------------------------------------------------------------------
// fonction : axis_ctrl_computeoffset_cb [PRIVATE]
//------------------------------------------------------------------------
function axis_ctrl_computeoffset_cb (ba)
	Struct WMButtonAction &ba
	if (ba.eventCode != 2)
		return kNO_ERROR
	endif
	DoAlert 1, "About to change the axis position offset.\nContinue?"
	if (V_flag == 2)
		return kNO_ERROR
	endif
	String device = GetUserData(ba.win, ba.ctrlName, "device")
	Variable current_pos
	if (axis_ctrl_GetPosition(device, current_pos))
		return kERROR
	endif
	Variable new_position = current_pos
	Prompt new_position, "Enter the actual (i.e. real) axis position [in User Units]"
	DoPrompt "Compute axis offset...", new_position
	if (V_flag == 1)
		return kNO_ERROR
	endif
	axis_ctrl_ComputeNewOffset(device, new_position)
end
//------------------------------------------------------------------------
// fonction : axis_changeaccuracy_cb [PRIVATE]
//------------------------------------------------------------------------
function axis_changeaccuracy_cb (ba)
	Struct WMButtonAction &ba
	if (ba.eventCode != 2)
		return kNO_ERROR
	endif
	String device = GetUserData(ba.win, ba.ctrlName, "device")
	Variable current_accuracy
	if (axis_ctrl_GetAccuracy(device, current_accuracy))
		return kERROR
	endif
	Variable new_accuracy = current_accuracy
	Prompt new_accuracy, "Enter new motion accuracy [in User Units]"
	DoPrompt "Change motion accuracy...", new_accuracy
	if (V_flag == 1)
		return kNO_ERROR
	endif
	axis_ctrl_SetAccuracy(device, new_accuracy)
end
//------------------------------------------------------------------------
// fonction : axis_initrefpos_cb [PRIVATE]
//------------------------------------------------------------------------
function axis_initrefpos_cb (ba)
	Struct WMButtonAction &ba
	if (ba.eventCode != 2)
		return kNO_ERROR
	endif
	DoAlert 1, "About to initialize the axis reference position.\nContinue?"
	if (V_flag == 2)
		return kNO_ERROR
	endif
	String device = GetUserData(ba.win, ba.ctrlName, "device")
	axis_ctrl_InitializeRefPosition(device)
end
//------------------------------------------------------------------------
// fonction : axis_switchonoff_cb [PRIVATE]
//------------------------------------------------------------------------
function axis_switchonoff_cb (ba)
	Struct WMButtonAction &ba
	if (ba.eventCode != 2)
		return kNO_ERROR
	endif
	String path = GetUserData(ba.win, ba.ctrlName, "df")
	String device = GetUserData(ba.win, ba.ctrlName, "device")
	NVAR/Z axis_state = $(path + "axis_state")
	if (NVAR_Exists(	axis_state))
		if (axis_state == kDeviceStateOFF)
			axis_ctrl_MotorON(device)
		else
			axis_ctrl_MotorOFF(device)
		endif
	endif
end
//------------------------------------------------------------------------
// fonction : axis_initdevice_cb [PRIVATE]
//------------------------------------------------------------------------
function axis_initdevice_cb (ba)
	Struct WMButtonAction &ba
	if (ba.eventCode != 2)
		return kNO_ERROR
	endif
	String device = GetUserData(ba.win, ba.ctrlName, "device")
	axis_ctrl_Init(device)
end
//------------------------------------------------------------------------
// fonction : axis_tab_cb [PRIVATE]
//------------------------------------------------------------------------
function axis_tab_cb (ta)
	Struct WMTabControlAction &ta
	if (ta.eventCode != 2)
		return kNO_ERROR
	endif
	axis_switch_tab(ta.win, ta.ctrlName, ta.tab)
end
//------------------------------------------------------------------------
// fonction : axis_switch_tab [PRIVATE]
//------------------------------------------------------------------------
function axis_switch_tab (win_name, ctrl_name, tab)
	String win_name
	String ctrl_name
	Variable tab
	TabControl $ctrl_name, win=$win_name, value=tab
	String axis = GetUserData(win_name, ctrl_name, "axis")
	switch(tab)
		//- tab: motion controls
		case 0:
			//- tab 1 ctrls
			Button $(kCTRL_CHOFF + axis) win=$win_name, disable=1
			Button $(kCTRL_CPOFF + axis) win=$win_name, disable=1
			Button $(kCTRL_CHACC + axis) win=$win_name, disable=1
			Button $(kCTRL_IRP + axis) win=$win_name, disable=1
			Button $(kCTRL_ONOFF + axis) win=$win_name, disable=1
			Button $(kCTRL_IDEV + axis) win=$win_name, disable=1
			//- tab 0 ctrls
			SetVariable $(kCTRL_SETPOS + axis) win=$win_name, disable=0
			SetVariable $(kCTRL_GETPOS + axis) win=$win_name, disable=0
			Button $(kCTRL_GOTO + axis) win=$win_name, disable=0
			Button $(kCTRL_STOP + axis) win=$win_name, disable=0 
			Button $(kCTRL_BCKWD + axis) win=$win_name, disable=0 
			Button $(kCTRL_FWD + axis) win=$win_name, disable=0 
			Button $(kCTRL_DEFPOS + axis) win=$win_name, disable=0 
			Checkbox $(kCTRL_BLS + axis) win=$win_name, disable=2 
			Checkbox $(kCTRL_FLS + axis) win=$win_name, disable=2 
			break
		//- tab: misc. actions
		case 1:
			//- tab 0 ctrls
			SetVariable $(kCTRL_SETPOS + axis) win=$win_name, disable=1
			SetVariable $(kCTRL_GETPOS + axis) win=$win_name, disable=1
			Button $(kCTRL_GOTO + axis) win=$win_name, disable=1 
			Button $(kCTRL_STOP + axis) win=$win_name, disable=1 
			Button $(kCTRL_BCKWD + axis) win=$win_name, disable=1 
			Button $(kCTRL_FWD + axis) win=$win_name, disable=1 
			Button $(kCTRL_DEFPOS + axis) win=$win_name, disable=1 
			Checkbox $(kCTRL_BLS + axis) win=$win_name, disable=1 
			Checkbox $(kCTRL_FLS + axis) win=$win_name, disable=1
			//- tab 1 ctrls
			Button $(kCTRL_CHOFF + axis) win=$win_name, disable=0
			Button $(kCTRL_CPOFF + axis) win=$win_name, disable=0
			Button $(kCTRL_CHACC + axis) win=$win_name, disable=0
			Button $(kCTRL_IRP + axis) win=$win_name, disable=0
			Button $(kCTRL_ONOFF + axis) win=$win_name, disable=0
			Button $(kCTRL_IDEV + axis) win=$win_name, disable=0
			break
	endswitch
end
//------------------------------------------------------------------------
// function: axis_enable_widgets [PRIVATE]
//------------------------------------------------------------------------
function axis_enable_widgets (win_name, axis_id)
	String win_name
	Variable axis_id
	String axis_id_str = num2str(axis_id)
	String ctrl_name = kCTRL_TAB + axis_id_str
	ControlInfo /W=$win_name $ctrl_name
	Variable tab = V_Value
	TabControl $ctrl_name, win=$win_name, disable = 0
	ctrl_name = kCTRL_DEFPOS + axis_id_str
	Button $ctrl_name, win=$win_name, disable = (tab == 0) ? 0 : 1
	ctrl_name = kCTRL_GOTO + axis_id_str
	Button $ctrl_name, win=$win_name, disable = (tab == 0) ? 0 : 1
	ctrl_name = kCTRL_SETPOS + axis_id_str
	SetVariable $ctrl_name, win=$win_name, disable = (tab == 0) ? 0 : 1
	ctrl_name = kCTRL_BCKWD + axis_id_str
	Button $ctrl_name, win=$win_name, disable = (tab == 0) ? 0 : 1
	ctrl_name = kCTRL_STOP + axis_id_str
	Button $ctrl_name, win=$win_name, disable = (tab == 0) ? 0 : 1
	ctrl_name = kCTRL_FWD + axis_id_str
	Button $ctrl_name, win=$win_name, disable = (tab == 0) ? 0 : 1
	ctrl_name = kCTRL_CHOFF + axis_id_str
	Button $ctrl_name, win=$win_name, disable = (tab == 1) ? 0 : 1
	ctrl_name = kCTRL_CPOFF + axis_id_str
	Button $ctrl_name, win=$win_name, disable = (tab == 1) ? 0 : 1
	ctrl_name = kCTRL_CHACC + axis_id_str
	Button $ctrl_name, win=$win_name, disable = (tab == 1) ? 0 : 1
	ctrl_name = kCTRL_IRP + axis_id_str
	Button $ctrl_name, win=$win_name, disable = (tab == 1) ? 0 : 1
	ctrl_name = kCTRL_ONOFF + axis_id_str
	Button $ctrl_name, win=$win_name, disable = (tab == 1) ? 0 : 1
	ctrl_name = kCTRL_IDEV + axis_id_str
	Button $ctrl_name, win=$win_name, disable = (tab == 1) ? 0 : 1
end
//------------------------------------------------------------------------
// function: axis_disable_widgets [PRIVATE]
//------------------------------------------------------------------------
function axis_disable_widgets (win_name, axis_id)
	String win_name
	Variable axis_id
	String axis_id_str = num2str(axis_id)
	String ctrl_name = kCTRL_TAB + axis_id_str
	ControlInfo /W=$win_name $ctrl_name
	Variable tab = V_Value
	ctrl_name = kCTRL_TAB + axis_id_str
	TabControl $ctrl_name, win=$win_name, disable = 2
	ctrl_name = kCTRL_DEFPOS + axis_id_str
	Button $ctrl_name, win=$win_name, disable = (tab == 0) ? 2 : 1
	ctrl_name = kCTRL_GOTO + axis_id_str
	Button $ctrl_name, win=$win_name, disable = (tab == 0) ? 2 : 1
	ctrl_name = kCTRL_SETPOS + axis_id_str
	SetVariable $ctrl_name, win=$win_name, disable = (tab == 0) ? 2 : 1
	ctrl_name = kCTRL_BCKWD + axis_id_str
	Button $ctrl_name, win=$win_name, disable = (tab == 0) ? 2 : 1
	ctrl_name = kCTRL_STOP + axis_id_str
	Button $ctrl_name, win=$win_name, disable = (tab == 0) ? 0 : 1
	ctrl_name = kCTRL_FWD + axis_id_str
	Button $ctrl_name, win=$win_name, disable = (tab == 0) ? 2 : 1
	ctrl_name = kCTRL_CHOFF + axis_id_str
	Button $ctrl_name, win=$win_name, disable = (tab == 1) ? 2 : 1
	ctrl_name = kCTRL_CPOFF + axis_id_str
	Button $ctrl_name, win=$win_name, disable = (tab == 1) ? 2 : 1
	ctrl_name = kCTRL_CHACC + axis_id_str
	Button $ctrl_name, win=$win_name, disable = (tab == 1) ? 2 : 1
	ctrl_name = kCTRL_IRP + axis_id_str
	Button $ctrl_name, win=$win_name, disable = (tab == 1) ? 2 : 1
	ctrl_name = kCTRL_ONOFF + axis_id_str
	Button $ctrl_name, win=$win_name, disable = (tab == 1) ? 0 : 1
	ctrl_name = kCTRL_IDEV + axis_id_str
	Button $ctrl_name, win=$win_name, disable = (tab == 1) ? 2 : 1
end
//------------------------------------------------------------------------
// axis_ctrl_GetAccuracy [PRIVATE]
//------------------------------------------------------------------------
static function axis_ctrl_GetAccuracy (dev, value)
	String dev
	Variable& value
	Struct AttributeValue av
	tango_init_attr_val(av, dev=dev, attr="accuracy")
	if (tango_read_attr(av) == kERROR)
		tango_display_error()
		value = NAN
		return kERROR
	endif
	value = av.var_val
	return kNO_ERROR
end
//------------------------------------------------------------------------
// axis_ctrl_SetAccuracy [PRIVATE]
//------------------------------------------------------------------------
static function axis_ctrl_SetAccuracy (dev, value)
	String dev
	Variable value
	Struct AttributeValue av
	tango_init_attr_val(av, dev=dev, attr="accuracy", nval=value)
	if (tango_write_attr(av) == kERROR)
		tango_display_error()
		return kERROR
	endif
	return kNO_ERROR
end
//------------------------------------------------------------------------
// axis_ctrl_GetOffset [PRIVATE]
//------------------------------------------------------------------------
static function axis_ctrl_GetOffset (dev, value)
	String dev
	Variable& value
	Struct AttributeValue av
	tango_init_attr_val(av, dev=dev, attr="offset")
	if (tango_read_attr(av) == kERROR)
		tango_display_error()
		value = NAN
		return kERROR
	endif
	value = av.var_val
	return kNO_ERROR
end
//------------------------------------------------------------------------
// axis_ctrl_SetOffset [PRIVATE]
//------------------------------------------------------------------------
static function axis_ctrl_SetOffset (dev, value)
	String dev
	Variable value
	Struct AttributeValue av
	tango_init_attr_val(av, dev=dev, attr="offset", nval=value)
	if (tango_write_attr(av) == kERROR)
		tango_display_error()
		return kERROR
	endif
	return kNO_ERROR
end
//------------------------------------------------------------------------
// axis_ctrl_GetPosition [PRIVATE]
//------------------------------------------------------------------------
static function axis_ctrl_GetPosition (dev, value)
	String dev
	Variable& value
	Struct AttributeValue av
	tango_init_attr_val(av, dev=dev, attr="position")
	if (tango_read_attr(av) == kERROR)
		tango_display_error()
		value = NAN
		return kERROR
	endif
	value = av.var_val
	return kNO_ERROR
end
//------------------------------------------------------------------------
// axis_ctrl_SetPosition [PRIVATE]
//------------------------------------------------------------------------
static function axis_ctrl_SetPosition (dev, value)
	String dev
	Variable value
	Struct AttributeValue av
	tango_init_attr_val(av, dev=dev, attr="position", nval=value)
	if (tango_write_attr(av) == kERROR)
		tango_display_error()
		return kERROR
	endif
	return kNO_ERROR
end
//------------------------------------------------------------------------
// axis_ctrl_GetState [PRIVATE]
//------------------------------------------------------------------------
static function axis_ctrl_GetState (dev, value)
	String dev
	Variable& value
	Struct AttributeValue av
	tango_init_attr_val(av, dev=dev, attr="State")
	if (tango_read_attr(av) == kERROR)
		tango_display_error()
		value = NAN
		return kERROR
	endif
	value = av.var_val
	return kNO_ERROR
end
//------------------------------------------------------------------------
// axis_ctrl_Stop [PRIVATE]
//------------------------------------------------------------------------
static function axis_ctrl_Stop (dev_name)
	String dev_name
	if (tango_cmd_inout(dev_name, "Stop") == kERROR)
		tango_display_error()
		return kERROR
	endif
	return kNO_ERROR
end
//------------------------------------------------------------------------
// axis_ctrl_Backward [PRIVATE]
//------------------------------------------------------------------------
static function axis_ctrl_Backward (dev_name)
	String dev_name
	if (tango_cmd_inout(dev_name, "Backward") == kERROR)
		tango_display_error()
		return kERROR
	endif
	return kNO_ERROR
end
//------------------------------------------------------------------------
// axis_ctrl_Forward [PRIVATE]
//------------------------------------------------------------------------
static function axis_ctrl_Forward (dev_name)
	String dev_name
	if (tango_cmd_inout(dev_name, "Forward") == kERROR)
		tango_display_error()
		return kERROR
	endif
	return kNO_ERROR
end
//------------------------------------------------------------------------
// axis_ctrl_InitializeRefPosition [PRIVATE]
//------------------------------------------------------------------------
static function axis_ctrl_InitializeRefPosition (dev_name)
	String dev_name
	if (tango_cmd_inout(dev_name, "InitializeReferencePosition") == kERROR)
		tango_display_error()
		return kERROR
	endif
	return kNO_ERROR
end
//------------------------------------------------------------------------
// axis_ctrl_DefinePosition [PRIVATE]
//------------------------------------------------------------------------
static function axis_ctrl_DefinePosition (dev_name, in_val)
	String dev_name
	Variable in_val
	Struct CmdArgIO cai
	tango_init_cmd_argio (cai)
	cai.var_val = in_val
	if (tango_cmd_inout(dev_name, "DefinePosition", arg_in = cai) == kERROR)
		tango_display_error()
		return kERROR
	endif
	return kNO_ERROR
end
//------------------------------------------------------------------------
// axis_ctrl_ComputeNewOffset [PRIVATE]
//------------------------------------------------------------------------
static function axis_ctrl_ComputeNewOffset (dev_name, in_val)
	String dev_name
	Variable in_val
	Struct CmdArgIO cai
	tango_init_cmd_argio (cai)
	cai.var_val = in_val
	if (tango_cmd_inout(dev_name, "ComputeNewOffset", arg_in = cai) == kERROR)
		tango_display_error()
		return kERROR
	endif
	return kNO_ERROR
end
//------------------------------------------------------------------------
// axis_ctrl_MotorON [PRIVATE]
//------------------------------------------------------------------------
static function axis_ctrl_MotorON (dev_name)
	String dev_name
	if (tango_cmd_inout(dev_name, "MotorON") == kERROR)
		tango_display_error()
		return kERROR
	endif
	return kNO_ERROR
end
//------------------------------------------------------------------------
// axis_ctrl_MotorOFF [PRIVATE]
//------------------------------------------------------------------------
static function axis_ctrl_MotorOFF (dev_name)
	String dev_name
	if (tango_cmd_inout(dev_name, "MotorOFF") == kERROR)
		tango_display_error()
		return kERROR
	endif
	return kNO_ERROR
end
//------------------------------------------------------------------------
// axis_ctrl_Init [PRIVATE]
//------------------------------------------------------------------------
static function axis_ctrl_Init (dev_name)
	String dev_name
	if (tango_cmd_inout(dev_name, "Init") == kERROR)
		tango_display_error()
		return kERROR
	endif
	return kNO_ERROR
end