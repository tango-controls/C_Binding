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
constant kSCALAR			= 0 	
constant kSPECTRUM	= 1
constant kIMAGE    	= 2	
//------------------------------------------------------------------------------
constant kSTRING = 0x00	
constant kSHORT  = 0x10	
constant kLONG   = 0x20
constant kDOUBLE = 0x04
//------------------------------------------------------------------------------
constant kSTR_VAR				= 0x00	
constant kCPL_VAR				= 0x01	
constant kDBL_VAR				= 0x02
constant k1DT_WAV				= 0x03
constant k1DB_WAV				= 0x04
constant k1DW_WAV				= 0x05
constant k1DI_WAV				= 0x06
constant k1DI_1DT_WAV 	= 0x07
constant k1DF_WAV				= 0x08
constant k1DD_WAV				= 0x09
constant k1DD_1DT_WAV 	= 0x10
constant k2DW_WAV				= 0x11
constant k2DI_WAV				= 0x12
constant k2DD_WAV				= 0x13
//------------------------------------------------------------------------------
constant kREAD   						= 0x0
constant kREAD_WITH_WRITE = 0x1
constant kWRITE							= 0x2
constant kREAD_WRITE   		= 0x3
//------------------------------------------------------------------------------
constant kSVAR = 0x00	
constant kNVAR = 0x01
constant kTWAV = 0x02
constant kNWAV = 0x03
//------------------------------------------------------------------------------
strconstant kNAME_SEP = "."

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
// fonction : tango_show_error
//==============================================================================
function tango_show_error ( )
	tep_open(0)
end
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
	DoWindow/T tep_modal, "*** TANGO - Client XOP Error ***" 
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
// fonction : tango_show_error_str
//==============================================================================
function tango_show_error_str (err_str)
	String err_str
	DoAlert 0, err_str
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
function/S tango_enter_device_df (device_name)
	String device_name
	String device_df = tango_device_to_df_name(device_name)
	String cur_df = GetDataFolder(1)
	if (cmpstr(device_df, cur_df) == 0)
		return cur_df
	endif 
	if (DataFolderExists(device_df) == 0)
		if (tango_open_device(device_name) == -1)
			tango_show_error()
			abort
		endif
	endif
	SetDataFolder $device_df
	return cur_df
end
//==============================================================================
// fonction : tango_enter_attrs_df 
//==============================================================================
function/S tango_enter_attrs_df  (dev)
	String dev
	String attrs_df = tango_device_to_df_name(dev) + ":attributes"
	String cur_df = GetDataFolder(1)
	if (cmpstr(attrs_df, cur_df) == 0)
		return cur_df
	endif 
	if (DataFolderExists(attrs_df) == 0)
		NewDataFolder/O/S $attrs_df
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
	String attr_df = tango_device_to_df_name(dev) + ":attributes:" + attr
	String cur_df = GetDataFolder(1)
	if (cmpstr(attr_df, cur_df) == 0)
		return cur_df
	endif 
	if (DataFolderExists(attr_df) == 0)
		NewDataFolder/O/S $attr_df
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
	String cmds_df = tango_device_to_df_name(dev) + ":commands"
	String cur_df = GetDataFolder(1)
	if (cmpstr(cmds_df, cur_df) == 0)
		return cur_df
	endif 
	if (DataFolderExists(cmds_df) == 0)
		NewDataFolder/O/S $cmds_df
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
	String cmd_df = tango_device_to_df_name(dev) + ":commands:" + cmd
	String cur_df = GetDataFolder(1)
	if (cmpstr(cmd_df, cur_df) == 0)
		return cur_df
	endif 
	if (DataFolderExists(cmd_df) == 0)
		NewDataFolder/O/S $cmd_df
	else
		SetDataFolder $cmd_df
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
	String str_to_add = attr + kNAME_SEP
	Variable found = strsearch(list, str_to_add, 0)
	if (found != -1) 
		return list
	endif
	str_to_add += val	
	Variable last_char = char2num(list[strlen(list) - 1])
	if (strlen(list) && last_char != char2num(";"))
		list += ";"
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
	Variable start = strsearch(list, attr + kNAME_SEP, 0)
	if (start == -1) 
		return list
	endif
	Variable stop = strsearch(list, ";", start)
	if (stop == -1)
		stop = strlen(list)
	endif
	String new_list = list[0, start - 1] + list[stop + 1, strlen(list) - 1]
	return new_list
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