//==============================================================================
// tango_panel.ipf
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
#include "tango"

//==============================================================================
// menu TANGO
//==============================================================================
Menu "Tango"
	"Open TANGO Panel",Execute/P "tango_panel()"
End

//==============================================================================
//  tango_panel
//==============================================================================
window tango_panel() : Panel
	PauseUpdate; Silent 1		// building window...
	tp_build()
endMacro
//==============================================================================
function tp_build() 
	//--close it if it's already opened
	DoWindow /K tango_panel
	//--create the window (see <NewPanel> documentation for the meaning of </K=1>)
	NewPanel /K=1 /W=(140,130,1030,550)
	//--set window name <daq_panel>
	DoWindow /C tango_panel
	//--set windows title
	DoWindow /T tango_panel "TANGO - Browser" 
	ModifyPanel fixedSize=1
	TabControl tab, pos={278,71},size={596,336}, font="Courier New", fsize=kLB_FONTSIZE
	TabControl tab, tabLabel(0)="Commands"
	TabControl tab, tabLabel(1)="Attributes"
	TabControl tab, tabLabel(2)="Blackbox"
	TabControl tab, value=0, proc=tp_ctrlproc_tab
	ListBox dev_list, pos={15,15}, size={249,327}, frame=2
	ListBox dev_list, mode=2, proc=tp_ctrlproc_device_listbox
	ListBox dev_list, font="Courier New", fsize=kLB_FONTSIZE
	ListBox dev_list, widths={256*FontSizeStringWidth("Courier New", kLB_FONTSIZE, 0, "*")}
	SetVariable wild_card,pos={16,355},size={251,18},title="Filter"
	SetVariable wild_card,font="Courier New", proc=tp_ctrlproc_wc
	ListBox cmd_list, pos={286,102}, size={580,293}, frame=2
	ListBox cmd_list, mode=2, font="Courier New", fsize=kLB_FONTSIZE
	ListBox cmd_list, proc=tp_ctrlproc_cmd_listbox
	ListBox attr_list, disable=1, pos={286,102}, size={580,293}, frame=2
	ListBox attr_list, mode=2, font="Courier New", fsize=kLB_FONTSIZE
	ListBox attr_list, proc=tp_ctrlproc_attr_listbox
	ListBox bb_list,  disable=1, pos={286,102}, size={580,293}, frame=2
	ListBox bb_list, mode=2, font="Courier New", fsize=kLB_FONTSIZE
	Button dev_update_but, pos={15,382}, size={251,25}, title="Update List"
	Button dev_update_but, proc=tp_ctrlproc_update_list_but
	SetVariable version_sv, pos={276,15}, size={80,17}, title="Version"
	SetVariable version_sv, font="Courier New", limits={0,0,0}, noedit=1
	SetVariable class_sv, pos={365,15}, size={150,17}, title="Class"
	SetVariable class_sv, font="Courier New", limits={0,0,0}, noedit=1
	SetVariable url_sv, pos={277,42}, size={540,17}, title="URL"
	SetVariable url_sv, font="Courier New", limits={0,0,0}, noedit=1
	SetVariable host_sv, pos={523,15}, size={352,17}, title="Host"
	SetVariable host_sv, font="Courier New", limits={0,0,0}, noedit= 1
	Button open_url_but, pos={827,42}, size={47,17}
	Button open_url_but, proc=tp_open_url, title="Open"
	tp_disable_items()
	String wc = tango_get_global_obj("wild_card", kSVAR)
	SVAR wc_value = $wc
	if (strlen(wc_value) == 0)
		wc_value = "*"
	endif
	SetVariable wild_card, value=$wc
	DoUpdate
	tp_ctrlproc_update_list_but("dummy")
end 
//==============================================================================
// fonction : tp_enable_items
//==============================================================================
function tp_enable_items ( )
	Button open_url_but, disable=0
	Button tab, disable=0
end
//==============================================================================
// fonction : tp_disable_items
//==============================================================================
function tp_disable_items ( )
	TabControl tab, win=tango_panel, value=0, disable=2
	ListBox cmd_list, win=tango_panel, listWave=none, disable=2
	ListBox attr_list, win=tango_panel, listWave=none, disable=1
	ListBox bb_list, win=tango_panel, listWave=none, disable=1
	String empty_var = tango_get_global_obj("empty_var", kSVAR)
	SetVariable version_sv, win=tango_panel, value=$empty_var
	SetVariable class_sv, win=tango_panel, value=$empty_var
	SetVariable host_sv, win=tango_panel, value=$empty_var
	SetVariable url_sv, win=tango_panel, value=$empty_var
	Button open_url_but, disable=2
end
//==============================================================================
// fonction : tp_ctrlproc_wc
//==============================================================================
function tp_ctrlproc_wc (ctrlName, varNum, varStr, varName) : SetVariableControl
	String ctrlName
	Variable varNum
	String varStr	
	String varName	
	tp_ctrlproc_update_list_but("dummy")
end
//==============================================================================
// fonction : tp_ctrlproc_tab
//==============================================================================
function tp_ctrlproc_tab (name, tab)
	String name
	Variable tab
	Variable disable_ctrl
	//--page 0: commands
	disable_ctrl = (tab!=0)
	ListBox cmd_list, win=tango_panel, disable=disable_ctrl
	//--page 1: attributes
	disable_ctrl = (tab!=1)
	ListBox attr_list, win=tango_panel, disable=disable_ctrl
	//--page 2: attributes
	disable_ctrl = (tab!=2)
	ListBox bb_list, win=tango_panel, disable=disable_ctrl
	ControlInfo /W=tango_panel dev_list
	WAVE/T list = $(S_DataFolder + S_Value)
	if (V_Value == -1)
		return 0
	endif
	String ldf = tango_enter_device_df(list[V_Value])
		switch (tab)
			case 0:
				tp_update_cmd_list(list[V_Value])
				break
			case 1:
				tp_update_attr_list(list[V_Value])
				break
			case 2:
				tp_update_bb_list(list[V_Value])
				break
		endswitch
	tango_leave_df(ldf)
end
//==============================================================================
//  tp_ctrlproc_update_list_but
//==============================================================================
function tp_ctrlproc_update_list_but (ctrlName) : ButtonControl
	String ctrlName
	tp_disable_items()
	SVAR wc = $tango_get_global_obj("wild_card", kSVAR)
	if (strlen(wc) == 0)
			wc = "*"
	endif
	String temp_var = tango_device_to_df_name(kTangoDB) + ":tmp_dlist"
	if (tango_command_inout(kTangoDB, "DbGetDeviceExportedList", wc, temp_var) == -1)
		tango_show_error()
		KillStrings/Z temp_var 
		return 0
	endif
	String ldf = tango_enter_device_df(kTangoDB)
		if (tango_close_device("*") == -1)
			tango_show_error()
			return 0
		endif
		String empty_list_name = tango_get_global_obj("empty_list", kTWAV)
		WAVE/T empty_list = $empty_list_name
		WAVE/Z/T tmp_dlist = $temp_var
		ListBox dev_list, selRow = -1
		Variable n = dimsize(tmp_dlist,0)
		if (WAVEExists(tmp_dlist) == 0 || n == 0)
			ListBox dev_list, win=tango_panel, listWave=empty_list, disable=2
			ListBox cmd_list, win=tango_panel, listWave=empty_list, disable=2
			ListBox attr_list, win=tango_panel, listWave=empty_list, disable=1
			ListBox bb_list, win=tango_panel, listWave=empty_list, disable=1
			KillWaves/Z tmp_dlist
			KillStrings/Z temp_var 
			tango_leave_df(ldf)
			return 0
		else
			ListBox dev_list, win=tango_panel, listWave=tmp_dlist
			ListBox attr_list, win=tango_panel, listWave=empty_list, disable=1
			ListBox bb_list, win=tango_panel, listWave=empty_list, disable=1
			ListBox cmd_list, win=tango_panel, listWave=empty_list, disable=0
		endif 
		Make /O /N=(numpnts(tmp_dlist),1) /T dlist
		dlist[][x] = tmp_dlist[x]
		SetDimLabel 1, 0, Devices, dlist	
		Make /O /B /U /N=(numpnts(tmp_dlist),1,1) dlist_sw = 0
		SetDimLabel 2,0,foreColors,dlist_sw
		Make /O /W /U dlist_colors = {{0,0,0},{65535,0,0}}
		MatrixTranspose dlist_colors
		ListBox dev_list, win=tango_panel, listWave=dlist
		ListBox dev_list, win=tango_panel, selWave=dlist_sw
		ListBox dev_list, win=tango_panel, colorWave=dlist_colors
		KillStrings/Z temp_var 
		KillWaves/Z tmp_dlist
	tango_leave_df(ldf)
end
//==============================================================================
//  tp_ctrlproc_device_listbox
//==============================================================================
function tp_ctrlproc_device_listbox (ctrl_name, row, col, event)
	String ctrl_name    
	Variable row        
	Variable col        
	Variable event
	if (event != 4)
		return 0
	endif
	ControlInfo /W=tango_panel dev_list
	if (V_Value == -1)
		return 0
	endif
	WAVE/T list = $(S_DataFolder + S_Value)
	if (row >= numpnts(list))
		row = numpnts(list) - 1
		ListBox dev_list, win=tango_panel, selRow=row
	endif
	String ldf = tango_enter_device_df(list[row])
		Variable err = 0
		ControlInfo /W=tango_panel tab
		switch (V_Value)
			case 0:
				err = tp_update_cmd_list(list[row])
				break
			case 1:	
				err = tp_update_attr_list(list[row])
				break
			case 2:	
				err = tp_update_bb_list(list[row])
				break 
		endswitch
		if (err == -1)
			tp_device_error(row)
		else
			tp_device_noerror(row)
		endif
	tango_leave_df(ldf)
	return 0
end
//==============================================================================
//  tp_ctrlproc_cmd_listbox
//==============================================================================
function tp_ctrlproc_cmd_listbox (ctrl_name, row, col, event)
	String ctrl_name    
	Variable row        
	Variable col        
	Variable event
	if (event != 3) 
		return 0
	endif
	ControlInfo /W=tango_panel dev_list
	if (V_Value == -1)
		return 0
	endif
	WAVE/T dlist = $(S_DataFolder + S_Value)
	String dev = dlist[V_Value]
	ControlInfo /W=tango_panel cmd_list
	WAVE/T clist = $(S_DataFolder + S_Value)
	String cmd = clist[V_Value][0]
	String argin_type = clist[V_Value][1]
	String argin_note = clist[V_Value][2]
	if (cmpstr(argin_note, "-") == 0 || cmpstr(argin_note, "Uninitialised") == 0)
		argin_note = ""
	endif
	String wlist = ""
	String argout_type = clist[V_Value][3]
	String ldf = tango_enter_device_df(dev)
		String win_title = "Execute " + dev + "::" + cmd
		String user_input = ""
		String argin_name = "argin"
		String prompt_str = ""
		strswitch(argin_type)
			case "-":
				break
			case "String":
				String argin_str = ""
				prompt_str = "Enter "
				if (strlen(argin_note) > 0)
					prompt_str += lowerStr(argin_note)
				else
					prompt_str += "argin string"
				endif
				prompt_str += ":"
				Prompt argin_str, prompt_str
				DoPrompt win_title, argin_str
				if (V_Flag)
					SetDataFolder(ldf)
					return 0
				endif
				user_input = argin_str
				String/G $argin_name = argin_str
				break
			case "Variable":
				Variable argin_val = 0
				prompt_str = "Enter "
				if (strlen(argin_note) > 0)
					prompt_str += lowerStr(argin_note)
				else
					prompt_str += "argin value"
				endif
				prompt_str += ":"
				Prompt argin_val, prompt_str
				DoPrompt win_title, argin_val
				if (V_Flag)
					SetDataFolder(ldf)
					return 0
				endif
				user_input = num2str(argin_val)
				Variable/G $argin_name = argin_val
				break
			case "Wave":
			case "Wave/T":
			case "Wave/B":
			case "Wave/W":
			case "Wave/I":
			case "Wave/D":
			case "Wave/U/B":
			case "Wave/U/W":
			case "Wave/U/I":
				Variable type = 2
				strswitch(argin_type)
					case "Wave/T":
						type = 0x00
						break
					case "Wave/B":
						type = 0x08
						break
					case "Wave/W":
						type = 0x10
						break
					case "Wave/I":
						type = 0x20
						break
					case "Wave/D":
						type = 0x04
						break
					case "Wave/U/B":
						type = 0x08 | 0x40
						break
					case "Wave/U/W":
						type = 0x10 | 0x40
						break
					case "Wave/U/I":
						type = 0x20 | 0x40
						break
				endswitch
				String argin_wave = "argin_w_"
				if (WaveExists($argin_wave))
					KillWaves $argin_wave
				endif
				Make /O /N=1 /Y=(type) $argin_wave
				Edit/K=1 $argin_wave
				String table_name = uniquename("input", 7, 0)
				DoWindow/C $table_name
				DoWindow/T $table_name, dev + "::" + cmd
				Execute("Modifytable alignment=1, font=\"Courier New\"")
				PauseForUser $table_name
				user_input = argin_wave
				String/G $argin_name = argin_wave
				break
			case "Wave/I:Wave/T":
			case "Wave/D:Wave/T":
				strswitch(argin_type)
					case "Wave/I:Wave/T":
						type = 0x20
						break
					case "Wave/D:Wave/T":
						type = 0x04
						break
				endswitch
				String argin_nwave = "argin_nw_"
				if (WaveExists($argin_nwave))
					KillWaves $argin_nwave
				endif
				Make /O /N=1 /Y=(type) $argin_nwave
				String argin_twave = "argin_tw_"
				if (WaveExists($argin_twave))
					KillWaves $argin_twave
				endif
				Make /O /N=1 /Y=(0) $argin_twave
				Edit/K=1 $argin_nwave, $argin_twave
				table_name = uniquename("input", 7, 0)
				DoWindow/C $table_name
				DoWindow/T $table_name, dev + "::" + cmd
				Execute("Modifytable alignment=1, font=\"Courier New\"")
				PauseForUser $table_name
				user_input = argin_nwave + kNAME_SEP + argin_twave
				String/G $argin_name = argin_nwave + kNAME_SEP + argin_twave
				break
			default:
				SetDataFolder(ldf)
				return 0
				break
		endswitch
		String argout_name = "argout"
		strswitch(argout_type)
			case "String":
				argout_name += "_str_"
				break
			case "Variable":
			case "Variable/C":
				argout_name += "_val_"
				break
			case "Wave/T":
				argout_name += "_tw_"
				break
			case "Wave/I:Wave/T":
			case "Wave/D:Wave/T":
				String argout_nwave = argout_name + "_nw_" 
				String argout_twave = argout_name + "_tw_" 
				argout_name = argout_nwave + kNAME_SEP + argout_twave 
				break
			default:
				argout_name += "_nw_"
				break
		endswitch
		String exec_cmd = dev + "::" + cmd + "(" + user_input + ")"
		print exec_cmd
		String prefix = "\t-> "
		if (tango_command_inout(dev, cmd, argin_name, argout_name) == -1)
				tango_show_error()
				KillStrings/Z $argin_name
				KillVariables/Z $argin_name
				SetDataFolder(ldf)
				print prefix + "[error: " + tango_get_error_str() + "]"
				return -1
		endif
		KillStrings/Z $argin_name
		KillVariables/Z $argin_name
		strswitch(argout_type)
			case "-":
				print prefix + "[completed successfully]"
				break
			case "String":
				SVAR arg_out_str = $argout_name
				if (strlen(arg_out_str))
					print prefix + arg_out_str
				else
					print prefix + "[valid but empty string received from device]"
				endif
				KillStrings/Z argout_name
				break
			case "Variable":
				NVAR arg_out_val = $argout_name
				print prefix + num2str(arg_out_val)
				KillVariables/Z argout_name
				break
			case "Variable/C":
				NVAR arg_out_val = $argout_name
				print prefix + "result - read: " + num2str(real(arg_out_val))
				print prefix + "result - last write: " + num2str(imag(arg_out_val))
				KillVariables/Z argout_name
				break
			case "Wave/I:Wave/T":
			case "Wave/D:Wave/T":
				WAVE argout_nw = $argout_nwave
				WAVE/T argout_tw = $argout_twave
				Variable np = max(numpnts(argout_nw), numpnts(argout_tw))
				if (np == 0)
					print prefix + "[valid but empty waves received from device]"
					break
				endif
				if (np < 5 && numpnts(argout_nw) == numpnts(argout_tw)) 
					Variable i
					for (i= 0; i < np; i += 1)
						print prefix + num2str(argout_nw[i]) + " : "  + argout_tw[i]
					endfor
				else 
					Edit/K=1 $argout_nwave, $argout_twave 
					table_name = uniquename("result", 7, 0)
					DoWindow/C $table_name
					DoWindow/T $table_name, exec_cmd
					Execute("Modifytable alignment=1, font=\"Courier New\"")
					print prefix + "[num. part stored in " + GetDataFolder(1) + argout_nwave + "]"
					print prefix + "[txt. part stored in " + GetDataFolder(1) + argout_twave + "]"
				endif
				break
			default:
				np = numpnts($argout_name)
				if (np == 0)
					print prefix + "[valid but empty wave received from device]"
					break
				endif
				if (np < 5)
					if (WaveType($argout_name) != 0)
						WAVE argout_nw = $argout_name
						for (i= 0; i < np; i += 1)
							print prefix + num2str(argout_nw[i])
						endfor
					else 
						WAVE/T argout_tw = $argout_name
						for (i= 0; i < np; i += 1)
							print prefix + argout_tw[i]
						endfor
					endif
				else
					Edit/K=1 $argout_name 
					table_name = uniquename("result", 7, 0)
					DoWindow/C $table_name
					DoWindow/T $table_name, exec_cmd
					Execute("Modifytable alignment=1, font=\"Courier New\"")
					print prefix + "[result stored in " + GetDataFolder(1) + argout_name + "]"
				endif
				break
		endswitch
	SetDataFolder(ldf)
	return 0
end
//==============================================================================
//  tp_ctrlproc_attr_listbox
//==============================================================================
function tp_ctrlproc_attr_listbox (ctrl_name, row, col, event)
	String ctrl_name    
	Variable row        
	Variable col        
	Variable event
	if (event == 3) 
		ControlInfo /W=tango_panel dev_list
		if (V_Value == -1)
			return 0
		endif
		WAVE/T dlist = $(S_DataFolder + S_Value)
		String dev = dlist[V_Value]
		ControlInfo /W=tango_panel attr_list
		WAVE/T alist = $(S_DataFolder + S_Value)
		String attr = alist[V_Value][0] 
		Variable access = tango_get_attr_access(dev, attr)
		Variable format = tango_get_attr_format(dev, attr)
		Variable type = tango_get_attr_type(dev, attr)
		String wattr = lowerstr(alist[V_Value][16]) 
		if (cmpstr(wattr,"none") == 0)
			wattr = ""
		endif
		if (access == kWRITE)
			Variable attr_val_
			String prompt_str = "Enter " + attr + " value:"
			Prompt attr_val_, prompt_str
			DoPrompt "Write attribute", attr_val_
			if (V_Flag)
				return 0
			endif
			Variable/G g_attr_val_ = attr_val_
			if (tango_write_attribute(dev, attr, "g_attr_val_") == -1)
				tango_show_error()
			endif
			KillVariables/Z g_attr_val_
			return 0
		else
			tmon_create(dev, attr, format, type, access, wattr, 1);
		endif
	endif
	return 0
end
//==============================================================================
//  tp_update_cmd_list
//==============================================================================
function tp_update_cmd_list (device_name)
	String device_name
	String ldf = tango_enter_device_df(device_name)
	String/G temp_var = "clist"
	String empty_list_name = tango_get_global_obj("empty_list", kTWAV)
	WAVE/T empty_list = $empty_list_name	
	ListBox cmd_list, win=tango_panel, listWave=empty_list
	if (tango_get_dev_cmd_list(device_name, temp_var) == -1)
 		tango_show_error()
 		KillStrings temp_var 
 		tango_leave_df(ldf)
 		String empty_var = tango_get_global_obj("empty_var", kSVAR)
 		SetVariable version_sv, win=tango_panel, value=$empty_var
		SetVariable class_sv, win=tango_panel, value=$empty_var
		SetVariable host_sv, win=tango_panel, value=$empty_var
		SetVariable url_sv, win=tango_panel, value=$empty_var
		return -1
	endif
	WAVE/T clist = $temp_var
	SetDimLabel 1, 0, Name, clist
	SetDimLabel 1, 1, Argin, clist
	SetDimLabel 1, 2, Argin_Note, clist
	SetDimLabel 1, 3, Argout, clist
	SetDimLabel 1, 4, Argout_Note, clist
	Make /O /N=(dimsize(clist,1)) ww
	Variable min_wd = FontSizeStringWidth("Courier New", kLB_FONTSIZE, 0, "Argin_note")
	tools_get_listbox_colwidths(temp_var, "ww", min_wd)
	ListBox cmd_list, win=tango_panel, widths={ww[0],ww[1],ww[2],ww[3],ww[4]}
	ListBox cmd_list, win=tango_panel, listWave=clist, row=0, selRow=0 
	if (DataFolderExists("info"))
		SetVariable version_sv, win=tango_panel, value=:info:version
		SetVariable class_sv, win=tango_panel, value=:info:class
		SetVariable host_sv, win=tango_panel, value=:info:host
		SetVariable url_sv, win=tango_panel, value=:info:doc_url
	endif 
	KillWaves/Z ww   
	KillStrings temp_var 
	tango_leave_df(ldf)
	return 0
end
//==============================================================================
//  tp_update_attr_list
//==============================================================================
function tp_update_attr_list (device_name)
	String device_name
	String ldf = tango_enter_attrs_df(device_name)
	String/G temp_var = "alist"
	String empty_list_name = tango_get_global_obj("empty_list", kTWAV)
	WAVE/T empty_list = $empty_list_name		
	ListBox attr_list, win=tango_panel, listWave=empty_list
	if (tango_get_dev_attr_list(device_name, temp_var) == -1)
		tango_show_error()
		KillStrings temp_var 
		tango_leave_df(ldf)
		return -1
	endif
	WAVE/Z/T alist = $temp_var
	if (WAVEExists(alist) == 0 || dimsize(alist,0) == 0)
		KillStrings temp_var 
		tango_leave_df(ldf)
		return 0
	endif 
	SetDimLabel 1, 0, Name, alist
	SetDimLabel 1, 1, Access, alist
	SetDimLabel 1, 2, Format, alist
	SetDimLabel 1, 3, Type, alist
	SetDimLabel 1, 4, DimX, alist
	SetDimLabel 1, 5, DimY, alist
	SetDimLabel 1, 6, Label, alist
	SetDimLabel 1, 7, Unit, alist
	SetDimLabel 1, 8, StdUnit, alist
	SetDimLabel 1, 9, DispUnit, alist
	SetDimLabel 1,10, Format, alist
	SetDimLabel 1,11, minValue, alist
	SetDimLabel 1,12, maxValue, alist
	SetDimLabel 1,13, minAlarm, alist
	SetDimLabel 1,14, maxAlarm, alist
	SetDimLabel 1,15, Desc, alist
	SetDimLabel 1,16, WAttrName, alist
	Make /O /N=(dimsize(alist,1)) ww
	Variable min_wd = FontSizeStringWidth("Courier New", kLB_FONTSIZE, 0, "WAttrName ")
	tools_get_listbox_colwidths(temp_var, "ww", min_wd)
	ListBox attr_list, win=tango_panel, widths={ww[0],ww[1],ww[2],ww[3],ww[4],ww[5],ww[6],ww[7],ww[8],ww[9]}
	ListBox attr_list, win=tango_panel, listWave=alist, row=0, selRow=0 
	KillWaves/Z ww
	KillStrings/Z temp_var 
	tango_leave_df(ldf)
	return 0
end
//==============================================================================
//  tp_update_bb_list
//==============================================================================
function tp_update_bb_list (device_name)
	String device_name
	String ldf = tango_enter_device_df(device_name)
	String/G temp_var = "bblist"
	String empty_list_name = tango_get_global_obj("empty_list", kTWAV)
	WAVE/T empty_list = $empty_list_name		
	ListBox bb_list, win=tango_panel, listWave=empty_list
	if (tango_get_dev_black_box(device_name, temp_var, 128) == -1)
		tango_show_error()
		KillStrings temp_var 
		tango_leave_df(ldf)
		return -1
	endif
	WAVE/Z/T bblist = $temp_var
	if (WAVEExists(bblist) == 0 || dimsize(bblist,0) == 0)
		return 0
	endif 
	Make /O /N=2 ww
	tools_get_listbox_colwidths(temp_var, "ww", 256)
	ListBox bb_list, win=tango_panel, widths={ww[0]}
	ListBox bb_list, win=tango_panel, listWave=bblist, row=0, selRow=0 
	KillStrings/Z temp_var 
	KillWaves/Z ww
	tango_leave_df(ldf)
	return 0
end
//==============================================================================
//  tp_open_url
//==============================================================================
function tp_open_url (ctrlname) : ButtonControl
	String ctrlname
	ControlInfo /W=tango_panel dev_list
	WAVE/T list = $(S_DataFolder + S_Value)
	String ldf = tango_enter_device_df(list[V_Value])
		SVAR/Z url = :info:doc_url
		if (SVAR_exists(url))
			BrowseURL/Z url
		endif
	tango_leave_df(ldf)
end
//==============================================================================
// tp_device_error
//==============================================================================
function tp_device_error (idx)
	Variable idx
	tp_disable_items()
	String ldf = tango_enter_device_df(kTangoDB)
		WAVE/Z dlist_sw
		if (WaveExists(dlist_sw))
			dlist_sw[idx][0][0] = 1
		endif
	tango_leave_df(ldf)
end
//==============================================================================
// tp_device_noerror
//==============================================================================
function tp_device_noerror (idx)
	Variable idx
	ControlInfo/W=tango_panel tab
	TabControl tab, win=tango_panel, disable=0
	Variable disabled = (V_Value == 0) ? 0 : 1
	ListBox cmd_list, win=tango_panel, disable=disabled
	disabled = (V_Value == 1) ? 0 : 1
	ListBox attr_list, win=tango_panel, disable=disabled
	disabled = (V_Value == 2) ? 0 : 1
	ListBox bb_list, win=tango_panel, disable=disabled
	Button open_url_but, win=tango_panel, disable=0
	String ldf = tango_enter_device_df(kTangoDB)
		WAVE/Z dlist_sw
		if (WaveExists(dlist_sw))
			dlist_sw[idx][0][0] = 0
		endif
	tango_leave_df(ldf)
end
//==============================================================================
// tp_fit_to_win -- NOT USED -- IN PROGRESS
//==============================================================================
function tp_fit_to_win (win, ctrlName)
	String win, ctrlName
	GetWindow $win wsize
	Variable winH = V_bottom - V_top	 // points
	winH *= ScreenResolution / 72	 // points to pixels
	ControlInfo/W=$win $ctrlName
	if( V_Flag )
		String posInfo = StringByKey("pos", S_recreation,"=",",")
		Variable xpos = str2num(posInfo[1,inf])	// pixels
		String sizeInfo= StringByKey("size", S_recreation,"=",",")	
		Variable width = str2num(sizeInfo[1,inf])	// pixels
		ListBox $ctrlName, pos={xpos,4}, size={width, winH}
	endif
end
//==============================================================================
// tp_min_win_size -- NOT USED -- IN PROGRESS
//==============================================================================
function tp_min_win_size (winName, minw, minh)
	String winName
	Variable minw, minh
	GetWindow $winName wsize
	Variable width= max(V_right-V_left,minw)
	Variable height= max(V_bottom-V_top,minh)
	MoveWindow/W=$winName V_left, V_top, V_left+width, V_top+height
end
//==============================================================================
// tp_hook -- NOT USED -- IN PROGRESS
//==============================================================================
function tp_hook (infoStr)
	String infoStr
	String event= StringByKey("EVENT",infoStr)
	Variable statusCode= 0
	strswitch (event) 
		case "resize":
			String win= StringByKey("WINDOW",infoStr)
			tp_min_win_size(win, 200, 200)
			tp_fit_to_win(win,"theList")
			statusCode = 1
			break;
	endswitch
	return statusCode
end