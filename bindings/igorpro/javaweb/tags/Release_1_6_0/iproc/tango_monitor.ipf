//==============================================================================
// tango_monitor.ipf
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
// CONSTS
//==============================================================================
static strconstant kDEV    	= "DEV"	
static strconstant kATTR   	= "ATTR"	
static strconstant kFORMAT	= "FORM"	
static strconstant kTYPE   	= "TYPE"	
static strconstant kACCESS	= "ACCS"
static strconstant kDF     	= "DF"			
static strconstant kDATA   	= "DATA"
static strconstant kTMST   	= "kTMST"			
static strconstant kDELAY		= "DELAY"
static strconstant kTMS		= "TMS"		
static strconstant kVALUE		= "VAL"	
static strconstant kWATTR		= "WATTR"	
static strconstant kWVALUE	= "WVAL"	
//------------------------------------------------------------------------------
static constant kTMON_RUNNING	= 0
static constant kTMON_PAUSED	= 1
static constant kTMON_STOPPED	= 2
//------------------------------------------------------------------------------
static strconstant ksCT_GRA	= "Grays"
static strconstant ksCT_RBW 	= "Rainbow"
static strconstant ksCT_YHT 	= "YellowHot"
static strconstant ksCT_BHT 	= "BlueHot"
static strconstant ksCT_RGB 	= "BlueRedGreen"
static strconstant ksCT_RWB 	= "RedWhiteBlue"
static strconstant ksCT_EAR 	= "PlanetEarth"
static strconstant ksCT_TER 	= "Terrain"
//------------------------------------------------------------------------------
static strconstant ksDATA_EXT	 = "_data"
static strconstant ksTMST_EXT	 = "_ts"
//------------------------------------------------------------------------------
static constant kMIN_DELAY     = 0.05
static constant kMAX_DELAY     = 1800
static constant kSTEP_DELAY    = 0.01
static constant kDEFAULT_DELAY = 1
//------------------------------------------------------------------------------

//==============================================================================
//   -- ATTR MONITOR RELATED FUNCTIONS --  ATTR MONITOR RELATED FUNCTIONS --
//==============================================================================
//==============================================================================
// tmon_create
//==============================================================================
function tmon_create (dev, attr, format, type, access, wattr, delay)
	String dev
	String attr
	Variable format
	Variable type
	Variable access
	String wattr
	Variable delay
	if (type == kSTRING)
		tango_display_error_str("Can't monitor string attribute.")
		return -1
	endif 
	if (access == kWRITE)
		DoAlert 0, "Can't monitor write only attribute."
		return -1
	endif
	String ldf
	if (tango_enter_device_df(dev, ldf) == -1)
	  return -1
	endif
	String df = GetDataFolder(1)
	SVAR/Z tmon_attr_list = tmon_attr_list
	if (SVAR_Exists(tmon_attr_list)== 0)
		String/G tmon_attr_list = ""
	elseif (tango_attr_val_list_find(tmon_attr_list, attr) == 1)
		tmon_bring_to_front(attr, format)
		tango_leave_df(ldf)
		return 0
	endif
	WAVE/T dev_list = $tango_get_global_obj("tmon_dev_list", kTWAV)
	if (tmon_find(dev) == -1)
		Variable cur_np = numpnts(dev_list)
		InsertPoints cur_np, 1, dev_list
		dev_list[cur_np] = dev
	endif
	String wname = attr +  ksDATA_EXT
	String ts_wname = attr +  ksTMST_EXT
	String win_name = tmon_win_name(attr, format)
	String win_title = dev + "::" + attr
	NVAR/Z tmon_delay = tmon_delay
	if (NVAR_Exists(tmon_delay) == 0) 
		Variable/G tmon_delay = delay
	else
		if (tmon_delay > delay)
			tmon_delay = delay
		endif
	endif
	NVAR/Z tmon_ticks = tmon_ticks
	if (NVAR_Exists(tmon_ticks) == 0) 
		Variable/G tmon_ticks = ticks
	endif
	String val = ""
	String wrt_val = ""
	switch (format) 
		case kSCALAR:
			Variable n = 60
			Variable/G $("tmon_" + attr + "_cp") = -1
			val =  "tmon_" + attr + "_val"
			Variable/G/C $val = NAN
			Make /O /N=(n) /D $wname = NAN
			Make /O /N=(n) /D $ts_wname = DateTime + p
			String val_path = tools_build_path(df, val)
			String wav_path = tools_build_path(df, attr)
			String cp_path  = tools_build_path(df, "tmon_" + attr + "_cp")
			string dl_path  = tools_build_path(df, "tmon_delay");
			String dv = "tmon_" + attr + "_dv"
			Variable/G $dv
			String formula_str = "tmon_scalar_func" 
			formula_str += "(\"" + wav_path + "\",\"" + cp_path 
			formula_str += "\"," + val_path + ",\""
			formula_str += dl_path + "\")"
			SetFormula $dv, formula_str
			tmon_attr_list = tango_attr_val_list_add(tmon_attr_list, attr, val)
			tmon_display_scalar(win_name, win_title, attr)
			if (access == kREAD_WRITE || access == kREAD_WITH_WRITE)
				wrt_val = "tmon_" + attr + "_wrt_val"
				Variable/G $wrt_val = 0
				SetVariable write_attr,win=$win_name,pos={264,4},size={125,16}
				SetVariable write_attr,win=$win_name,title="WAttr.value",format="%g"
				SetVariable write_attr,win=$win_name,limits={-Inf,Inf,1},value=$wrt_val
				String btitle = "Write " + wattr
				Variable w = FontSizeStringWidth("Helvetica", 10, 0, btitle) + 20
				Button write_but,win=$win_name,pos={398,2},size={w,20},title=btitle
				Button write_but,win=$win_name,proc=tmon_write_attr_but_proc
			endif
			break
		case kSPECTRUM:
			val = wname
			tmon_attr_list = tango_attr_val_list_add(tmon_attr_list, attr, wname) 
			Make /O /N=0 /Y=(type) $wname = 0
			tmon_display_spectrum(win_name, win_title, wname)
			break
		case kIMAGE:
			val = wname
			tmon_attr_list = tango_attr_val_list_add(tmon_attr_list, attr, wname) 
			Make /O /N=(1,1) /Y=(type) $wname = 0
			tmon_display_image(win_name, win_title, wname)
			break 
	endswitch
	SetVariable delay, win=$win_name, value=$tools_build_path(df, "tmon_delay")
	tmon_win_note(win_name, dev, attr, format, type, access, df, val, wname, ts_wname)
	if (access == kREAD_WRITE || access == kREAD_WITH_WRITE)
		tmon_set_wattr(win_name, wattr)
		tmon_set_wvalue(win_name, wrt_val)
	endif
	NVAR/Z tmon_counter = tmon_counter
	if (NVAR_Exists(tmon_counter) == 0)
		Variable/G tmon_counter = 1
	else
		tmon_counter += 1
	endif
	tango_leave_df(ldf)
	NVAR tmon_bckg_delay = $tango_get_global_obj("tmon_delay", kNVAR)
	if (tmon_bckg_delay == 0)
		tmon_bckg_delay = 0.5
	elseif (tmon_bckg_delay > delay)
		tmon_bckg_delay = 0.5
	endif
	BackgroundInfo
	Variable vflag = V_Flag
	Variable is_tmon_btt = (strsearch(S_Value, "tmon_background_task",0) != -1) ? 1 : 0
	if (vflag != 0 && is_tmon_btt == 0)
			String ask_user = "WARNING! The background task " + S_value + " is running.\r"
			ask_user += "Do you want to stop it and launch the TANGO monitor?"
			DoAlert 1, ask_user 
			if (V_Flag != 1)
				tmon_remove(win_name)
				return 0
			endif
			CtrlBackground stop
			KillBackground
	endif
	if (vflag != 2 || is_tmon_btt == 0) 
		SetBackground tmon_background_task()
		CtrlBackground period = (tmon_bckg_delay * 60), noburst = 1
		CtrlBackground start
	elseif (vflag != 0)
		CtrlBackground period = (tmon_bckg_delay * 60), noburst = 1
	endif
	return 0
end
//==============================================================================
// fonction : tmon_remove
//==============================================================================
static function tmon_remove (win_name)
	String win_name
	String dev = tmon_get_dev(win_name)
	Variable idx = tmon_find(dev)
	if (idx == -1)
		return 0
	endif
	String ldf
	if (tango_enter_device_df(dev, ldf) == -1)
	  return -1
	endif
	SVAR/Z tmon_attr_list
	if (SVAR_Exists(tmon_attr_list) == 0)
		tango_leave_df(ldf)
		return -1
	endif
	SetWindow $win_name hook=no_hook
	String attr = tmon_get_attr(win_name)
	String wname = attr +  ksDATA_EXT
	String ts_wname = attr +  ksTMST_EXT
	tmon_attr_list = tango_attr_val_list_remove(tmon_attr_list, attr)
	if (tmon_get_format(win_name) == kSCALAR)
		String dv_name = "tmon_" + attr + "_dv"
		NVAR/Z dv = $dv_name
		if (NVAR_Exists(dv))
			SetFormula $dv_name, ""
		endif
		String cp  = "tmon_" + attr + "_cp"
		String val = tmon_get_value(win_name)
		String wrt_val = tmon_get_wvalue(win_name)
		KillVariables/Z $dv_name, $cp, $val, $wrt_val
	else
		String ts  = attr + "_data_ts"
		KillVariables/Z $ts
	endif
	NVAR tmon_counter = tmon_counter
	tmon_counter -= 1
	if (strlen(tmon_attr_list) == 0 && tmon_counter == 0)
		WAVE/T dev_list = $tango_get_global_obj("tmon_dev_list", kTWAV)
		Variable i
		Variable cur_np = numpnts(dev_list)
		for (i = idx; i < cur_np - 1; i += 1)
			dev_list[i] = dev_list[i + 1]
		endfor
		DeletePoints (cur_np - 1), 1, dev_list
		KillVariables/Z :tmon_delay, :tmon_ticks, :tmon_counter
		KillStrings/Z :tmon_attr_list
	endif
	DoWindow/K $win_name
	KillWaves/Z $wname, $ts_wname
	tango_leave_df(ldf)
	return 0
end 
//==============================================================================
// fonction : tmon_find
//==============================================================================
static function tmon_find (dev)
	String dev
	WAVE/T dev_list = $tango_get_global_obj("tmon_dev_list", kTWAV)
	Variable cur_np = numpnts(dev_list)
	Variable i
	for (i = 0; i < cur_np; i += 1)
		if (cmpstr(dev_list[i], dev) == 0)
			return i
		endif
	endfor
	return -1
end 
//==============================================================================
// fonction : tmon_background_task
//==============================================================================
function tmon_background_task ( )
	WAVE/T dev_list = $tango_get_global_obj("tmon_dev_list", kTWAV)
	Variable n = numpnts(dev_list)
	if (n == 0) 
		print "No more attributes to monitor. Monitor task exits."
		return 1 // no more attr to monitor: stop task 
	endif
	Variable i, len = 0
	String ldf = GetDataFolder(1), cur_df
	for (i = 0; i < n; i += 1)
		if (tango_enter_device_df(dev_list[i], cur_df) == -1)
		  return -1
		endif
		NVAR tmon_delay
		NVAR tmon_ticks
		if ((ticks - tmon_ticks) >= (tmon_delay * 56))
			tmon_ticks = ticks
			SVAR attr_list = tmon_attr_list
			len = strlen(attr_list)
			NVAR/Z has_error
			if (len && tango_read_attributes(dev_list[i], attr_list) == -1)
			  if (NVar_Exists(has_error) == 0)
		       tmon_reset_data(dev_list[i])
			    Variable/G has_error = 1
		     endif
		   else
		     if (NVar_Exists(has_error))
		       tmon_reset_error(dev_list[i]) 
		       KillVariables has_error  
		     endif
			endif
		endif
	endfor
	SetDataFolder ldf
	return 0				
end 
//==============================================================================
// fonction : tmon_scalar_func 
//==============================================================================
function tmon_scalar_func (wav_path, cp_path, val, delay_path)
	String wav_path
	String cp_path
	Variable/C val
	String delay_path
	WAVE dwav = $(wav_path + ksDATA_EXT) 
	WAVE tswav = $(wav_path + ksTMST_EXT) 
	NVAR cp = $cp_path
	NVAR delay = $delay_path
	if (cp < numpnts(dwav)) 
	  NVAR/Z has_error
	  if (NVAR_Exists(has_error))
	    dwav[cp] = NAN
	  else
	    dwav[cp] = real(val)
	  endif
	  tswav[cp] = (imag(val) != 0) ? imag(val) : (DateTime - 3600) 
	  if (cp == 0) 
	    tswav = tswav[cp] + p
	  endif
	endif
	cp += 1
	if (cp == numpnts(dwav)) 
	   Variable n = trunc(cp / 2)                 
		Rotate -n, dwav                      
		Rotate -n, tswav
		dwav[n, cp - 1] = NAN
		tswav[n, cp - 1] = tswav[n - 1] + ((p - n + 1) * delay)
		cp = n
	endif
	return 0
end
//==============================================================================
// fonction : tmon_display_scalar 
//==============================================================================
static function tmon_display_scalar (win_name, win_title, wav_name)
	String win_name
	String win_title
	String wav_name
	Variable access
	String ywav_name = wav_name +  ksDATA_EXT
	String xwav_name = wav_name +  ksTMST_EXT
	SetScale d 0,0,"dat", $xwav_name
	tmon_display (win_name, win_title, ywav_name, xwav_name)
	Label /W=$win_name bottom "\\u#2Timestamp"
	ModifyGraph /W=$win_name dateInfo(bottom)={0,0,0}
end
//==============================================================================
// fonction : tmon_display_spectrum 
//==============================================================================
static function tmon_display_spectrum (win_name, win_title, wav_name)
	String win_name
	String win_title
	String wav_name
	tmon_display (win_name, win_title, wav_name, "")
end
//==============================================================================
// fonction : tmon_display (for SCALAR & SPECTRUM ATTRS)
//==============================================================================
static function tmon_display (win_name, win_title, ywav_name, xwav_name)
	String win_name
	String win_title
	String ywav_name
	String xwav_name
	DoWindow/K $win_name
	if (strlen(xwav_name) != 0) 
		Display /K=1 /W=(0,0,380,170) $ywav_name vs $xwav_name
	else
		Display /K=1 /W=(0,0,380,170) $ywav_name
	endif
	DoWindow/C $win_name
	DoWindow/T $win_name, win_title
 	ModifyGraph margin(top)=28,gfSize=10
	ModifyGraph wbRGB=(56797,56797,56797), gbRGB=(0,0,0)
	ModifyGraph rgb=(0,65280,0), gridRGB=(21760,21760,21760)
	ModifyGraph grid=1, mirror=1,  minor(bottom)=1
	ModifyGraph font="Arial",fSize=8
	ModifyGraph axOffset(left)=-2.57143
	SetWindow $win_name hook=tmon_win_hook, hookEvents=0
	ControlBar /W=$win_name 25
	Button reset_but,win=$win_name,pos={2,2},size={50,20}
	Button reset_but,win=$win_name,proc=tmon_reset_but_proc,title="Reset"
	Button pause_but,win=$win_name,pos={57,2},size={50,20}
	Button pause_but,win=$win_name,proc=tmon_pause_but_proc,title="Pause"
	Button kill_but,win=$win_name,pos={112,2},size={50,20}
	Button kill_but,win=$win_name,proc=tmon_kill_but_proc,title="Kill"
	SetVariable delay,win=$win_name,pos={166,4},size={90,16}
	SetVariable delay,win=$win_name,title="Delay",format="%g s"
	SetVariable delay,win=$win_name,limits={kMIN_DELAY, kMAX_DELAY, kSTEP_DELAY}
	SetVariable delay,win=$win_name,proc=tmon_set_delay_proc
	SVAR target_win = $tango_get_global_obj("target_win", kSVAR)
	target_win = win_name
end
//==============================================================================
// fonction : tmon_display_image (for IMAGE ATTRS)
//==============================================================================
static function tmon_display_image (win_name, win_title, wav_name)
	String win_name
	String win_title
	String wav_name
	DoWindow/K $win_name
	Display /K=1 /W=(0,0,400,400)	
	DoWindow/C $win_name
	DoWindow/T $win_name, win_title
	AppendImage $wav_name
	ModifyGraph margin(top)=28,margin(right)=57
	ModifyGraph axOffset(left)=-2.33333
	ModifyImage $wav_name ctab= {*,*,PlanetEarth,0}
	ModifyGraph width={Aspect,1}, height={Aspect,1}
	ModifyGraph wbRGB=(56797,56797,56797)
	ModifyGraph mirror=1, minor=1
	ModifyGraph font="Arial", fSize=8
	ModifyGraph tickUnit(bottom)=1, tickUnit(left)=1
	SetAxis/A/R left
	ColorScale /W=$win_name/N=text0/F=0/S=3/B=1/A=MC/X=43.95/Y=0.40/E
	ColorScale /W=$win_name/C/N=text0 image=$wav_name,heightPct=100
	String cmd_str = "ColorScale /W=" + win_name
	cmd_str += "/C/N=text0 width=7,font=\"Small Fonts\",fsize=6,minor=1"
	Execute(cmd_str)
	AppendText "\\F'Arial'\\F'Arial'\\Z08\\Z06\\Z10\\F'Arial'\\Z10\\U"
	//--fonction hook
	SetWindow $win_name hook=tmon_win_hook, hookEvents=0
	//--controle du graph: tabcontrol (2 ou 3 pages)
	ControlBar 35
	Button reset_but,win=$win_name,pos={7,6},size={50,20},title="Reset"
	Button reset_but,win=$win_name,proc=tmon_reset_but_proc
	Button pause_but,win=$win_name,pos={65,6},size={50,20}
	Button pause_but,win=$win_name,proc=tmon_pause_but_proc,title="Pause"
	Button kill_but,win=$win_name,pos={123,6},size={50,20},title="Kill"
	Button kill_but,win=$win_name,proc=tmon_kill_but_proc
	SetVariable delay,win=$win_name,pos={180,8},size={90,16}
	SetVariable delay,win=$win_name,title="Delay",format="%g s"
	SetVariable delay,win=$win_name,limits={kMIN_DELAY, kMAX_DELAY, kSTEP_DELAY}
  SetVariable delay,win=$win_name,proc=tmon_set_delay_proc
	PopupMenu color_table_popup,win=$win_name,pos={271,6},size={168,21},bodyWidth=100
	PopupMenu color_table_popup,win=$win_name,mode=7
	PopupMenu color_table_popup,win=$win_name,proc=tmon_ctable_proc,title="Color Table"
	cmd_str = "PopupMenu color_table_popup,win=" + win_name + ",value=\""
	cmd_str += ksCT_GRA + ";" + ksCT_RBW + ";" + ksCT_YHT + ";" + ksCT_BHT + ";" 
	cmd_str += ksCT_RGB + ";" + ksCT_RWB + ";" + ksCT_EAR + ";" + ksCT_TER + ";" 
	cmd_str += "\""
	Execute(cmd_str)
	CheckBox reverse_ct_cb,win=$win_name,pos={449,9},size={90,14}
	CheckBox reverse_ct_cb,win=$win_name,title="Reverse",value=0 
	CheckBox reverse_ct_cb,win=$win_name,proc=tmon_reverse_ctable_proc
	SVAR target_win = $tango_get_global_obj("target_win", kSVAR)
	target_win = win_name
	return 0
end
//==============================================================================
// fonction : tmon_ctable_proc
//==============================================================================
function tmon_ctable_proc (ctrlName,popNum,popStr)
	String ctrlName
	Variable popNum
	String popStr
	SVAR win_name = $tango_get_global_obj("target_win", kSVAR)
	if (strlen(win_name) == 0)
		return -1
	endif
	String image = tmon_get_data(win_name)
	ControlInfo /W=$win_name reverse_ct_cb
	ModifyImage /W=$win_name $image ctab={*,*,$popStr,V_Value}
end
//==============================================================================
// fonction : tmon_reverse_ctable_proc
//==============================================================================
function tmon_reverse_ctable_proc (ctrlName,checked)
	String ctrlName
	Variable checked
	SVAR win_name = $tango_get_global_obj("target_win", kSVAR)
	if (strlen(win_name) == 0)
		return -1
	endif
	String image = tmon_get_data(win_name)
	ModifyImage /W=$win_name $image ctab= {*,*,,checked}
end
//==============================================================================
// fonction : tmon_reset_but_proc 
//==============================================================================
function tmon_reset_but_proc (ctrl_name)
	String ctrl_name
	SVAR target_win = $tango_get_global_obj("target_win", kSVAR)
	String df = tmon_get_df(target_win)
	String data = tmon_get_data(target_win)
	String tmst = tmon_get_timestamp(target_win)
	WAVE/Z w = $tools_build_path(df, data)
	if (WAVEExists(w) == 0)
		return 0
	endif
	Variable attr_format = tmon_get_format(target_win)
	if (attr_format == kSCALAR)
		String attr = tmon_get_attr(target_win)
		NVAR/Z cp  = $tools_build_path(df, "tmon_" + attr + "_cp")
		WAVE/Z ts = $tools_build_path(df, tmst)
		if (WAVEExists(ts))
			Variable last_ts
			do 
			 last_ts = ts[cp - 1]
			 cp -= 1
			while (numtype(last_ts) == 2)
			ts = last_ts + p
		endif
		cp = 0
  endif
  w = NAN
end
//==============================================================================
// fonction : tmon_pause_but_proc 
//==============================================================================
function tmon_pause_but_proc (ctrl_name)
	String ctrl_name
	SVAR target_win = $tango_get_global_obj("target_win", kSVAR)
	String ldf
	if (tango_enter_device_df(tmon_get_dev(target_win), ldf) == -1)
	  return -1
	endif
	SVAR alist = tmon_attr_list
	String attr = tmon_get_attr(target_win)
	if (tango_attr_val_list_find(alist, attr) == 0)
		switch(tmon_get_format(target_win))
			case kSCALAR:
				alist = tango_attr_val_list_add(alist, attr, tmon_get_value(target_win))
				break
			default:
				alist = tango_attr_val_list_add(alist, attr, tmon_get_value(target_win))
				break
		endswitch
		Button pause_but, win=$target_win, title="Pause"
		tmon_win_note_set_status (target_win, kTMON_RUNNING)
	else
		alist = tango_attr_val_list_remove(alist, attr)
		Button pause_but, win=$target_win, title="Resume"
		tmon_win_note_set_status (target_win, kTMON_PAUSED)
	endif	
	tango_leave_df(ldf)
end
//==============================================================================
// fonction : tmon_kill_but_proc 
//==============================================================================
function tmon_kill_but_proc (ctrl_name)
	String ctrl_name
	SVAR target_win = $tango_get_global_obj("target_win", kSVAR)
	tmon_remove(target_win)
end
//==============================================================================
// fonction :  tmon_set_delay_proc 
//==============================================================================
function tmon_set_delay_proc (ctrlName,varNum,varStr,varName)
	String ctrlName
	Variable varNum
	String varStr
	String varName
	NVAR tmon_delay = $tango_get_global_obj("tmon_delay", kNVAR)
	if (tmon_delay > varNum)
		tmon_delay = varNum
		CtrlBackground period = (tmon_delay * 60), noburst = 1
	endif
end
//==============================================================================
// fonction :  tmon_write_attr_but_proc
//==============================================================================
function tmon_write_attr_but_proc (ctrlName)
	String ctrlName
	SVAR target_win = $tango_get_global_obj("target_win", kSVAR)
	String dev = tmon_get_dev(target_win)
	String ldf
	if (tango_enter_device_df(dev, ldf) == -1)
	  return -1
	endif
	String attr = tmon_get_attr(target_win)
	Variable access = tmon_get_access(target_win)
	if (access == kREAD_WITH_WRITE)
		attr = tmon_get_wattr(target_win)
	endif
	String wrt_val = tmon_get_wvalue(target_win)
	if (tango_write_attribute(dev, attr, wrt_val) == -1)
		tango_display_error()
	endif 
	tango_leave_df(ldf)
end
//==============================================================================
// fonction : tmon_win_hook
//==============================================================================
function tmon_win_hook (info_str)
	String info_str
	Variable status = 0
	String event = StringByKey("EVENT", info_str)
	String win_name = StringByKey("WINDOW", info_str)
	String previous_df = GetDataFolder(1)
	strswitch(event)
		case "kill":
			tmon_remove(win_name)
			break
		case "activate":
			SVAR target_win = $tango_get_global_obj("target_win", kSVAR)
			target_win = win_name
			break		
		default:
			break
	endswitch
	return status				
end 
//==============================================================================
//	tmon_win_name
//==============================================================================
static function/S tmon_win_name (attr, attr_format)
	String attr
	Variable attr_format
	String wn_prefix
	switch (attr_format)
		case kSCALAR:
			wn_prefix = "scl_"
			break
		case kSPECTRUM:
			wn_prefix = "spec_"
			break
		case kIMAGE:
			wn_prefix = "img_"
			break
		default:
			return ""
	endswitch
	return UniqueName(wn_prefix + attr + "_win", 6, 0) 
end
//==============================================================================
//	tmon_bring_to_front
//==============================================================================
static function tmon_bring_to_front(attr, attr_format)
	String attr
	Variable attr_format
	String match_str
	switch (attr_format)
		case kSCALAR:
			match_str = "scl_*"
			break
		case kSPECTRUM:
			match_str = "spec_*"
			break
		case kIMAGE:
			match_str = "img_*"
			break
		default:
			return 0
	endswitch
	String win_list = WinList(match_str,";","WIN:1")
	if (strlen(win_list) == 0) 
		return 0
	endif 
	String win_name
	Variable i = 0
	do
		win_name = StringFromList(i, win_list)
		if (strlen(win_name) == 0)
			break
		endif
		if (cmpstr(tmon_get_attr(win_name), attr) == 0)
			DoWindow/F $win_name
			break
		endif
		i += 1
	while (1)
end
//==============================================================================
//	tmon_kill_dev_monitors
//==============================================================================
static function tmon_kill_dev_monitors(dev)
	String dev
	String win_list = WinList("*",";","WIN:1")
	if (strlen(win_list) == 0) 
		return 0
	endif 
	String win_name
	Variable i = 0
	do
		win_name = StringFromList(i, win_list)
		if (strlen(win_name) == 0)
			break
		endif
		if (cmpstr(tmon_get_dev(win_name), dev) == 0)
			tmon_remove(win_name)
		endif
		i += 1
	while (1)
end
//==============================================================================
//	tmon_reset_data
//==============================================================================
static function tmon_reset_data (dev)
	String dev
	String win_list = WinList("*",";","WIN:1")
	if (strlen(win_list) == 0) 
		return 0
	endif 
	String win_name
	String value_name
	Variable i = 0
	do
		win_name = StringFromList(i, win_list)
		if (strlen(win_name) == 0)
		  break
		endif
		value_name = tmon_get_value(win_name)
		if (cmpstr(tmon_get_dev(win_name), dev) == 0)
		  Wave/Z num_wav = $value_name
	     Wave/T/Z txt_wav = $value_name
		  NVar/Z/C num_var = $value_name 
		  SVar/Z txt_var = $value_name 
	     if (WaveExists(num_wav))
		    num_wav = 0
		  elseif (WaveExists(txt_wav)) 
		    txt_wav = "error reading attribute"
		  elseif (NVAR_Exists(num_var))
		    num_var = 0
		  elseif (SVAR_Exists(txt_var))
		    txt_var = "error reading attribute"
		  endif
		  SetDrawLayer /W=$win_name UserFront
	     SetDrawEnv /W=$win_name fsize=14, fstyle=1, textrgb=(65280,0,0)
	     DrawText /W=$win_name 0.2, 0.5, "ERROR READING ATTRIBUTE"
		endif
		i += 1
	while (1)
end
//==============================================================================
//	tmon_reset_error
//==============================================================================
static function tmon_reset_error (dev)
	String dev
	String win_list = WinList("*",";","WIN:1")
	if (strlen(win_list) == 0) 
		return 0
	endif 
	String win_name
	String value_name
	Variable i = 0
	do
		win_name = StringFromList(i, win_list)
		if (strlen(win_name) == 0)
		  break
		endif
		if (cmpstr(tmon_get_dev(win_name), dev) == 0)
		  SetDrawLayer /W=$win_name /K UserFront
		endif 
		i += 1
	while (1)
end
//==============================================================================
// tmon_win_note
//==============================================================================
static function tmon_win_note (win, dev, attr, fmt, type, accs, df, val, data, tmst)
	String	win
	String	dev
	String	attr
	Variable	fmt
	Variable	type
	Variable	accs
	String	df
	String 	val
	String	data
	String	tmst
	String win_note = ""
	win_note = ReplaceStringByKey(kDEV, win_note, dev)
	win_note = ReplaceStringByKey(kATTR, win_note, attr)
	win_note = ReplaceNumberByKey(kFORMAT, win_note, fmt)
	win_note = ReplaceNumberByKey(kTYPE, win_note, type)
	win_note = ReplaceNumberByKey(kACCESS, win_note, accs)
	win_note = ReplaceStringByKey(kDF, win_note, df)
	win_note = ReplaceStringByKey(kVALUE, win_note, val)
	win_note = ReplaceStringByKey(kDATA, win_note, data)
	win_note = ReplaceStringByKey(kTMST, win_note, tmst)
	win_note = ReplaceNumberByKey(kTMS, win_note, kTMON_RUNNING)
	SetWindow $win note=win_note
end
//==============================================================================
// tmon_win_note_set_num_val
//==============================================================================
static function tmon_win_note_set_num_val (win_name, keyword, new_value)
	String win_name
	String keyword
	Variable new_value
	GetWindow $win_name note
	String win_note = ReplaceNumberByKey(keyword, S_Value, new_value)
	SetWindow $win_name note=win_note
	return 0
end
//==============================================================================
// tmon_win_note_get_num_val
//==============================================================================
static function tmon_get_num_val (win_name, keyword)
	String win_name
	String keyword
	GetWindow $win_name note
	return NumberByKey(keyword, S_Value)
end
//==============================================================================
// tmon_win_note_set_str_val
//==============================================================================
static function tmon_win_note_set_str_val (win_name, keyword, new_str_value)
	String win_name
	String keyword
	String new_str_value
	GetWindow $win_name note
	String win_note = ReplaceStringByKey(keyword, S_Value, new_str_value)
	SetWindow $win_name note=win_note
	return 0
end
//==============================================================================
// tmon_win_note_get_str_val
//==============================================================================
static function/S tmon_get_str_val (win_name, keyword)
	String win_name
	String keyword
	GetWindow $win_name note
	return StringByKey(keyword, S_Value)
end
//==============================================================================
// tmon_get_dev
//==============================================================================
static function/S tmon_get_dev (win_name)
	String win_name
	GetWindow $win_name note
	return StringByKey(kDEV, S_Value)
end
//==============================================================================
// tmon_get_attr
//==============================================================================
static function/S tmon_get_attr (win_name)
	String win_name
	GetWindow $win_name note
	return StringByKey(kATTR, S_Value)
end
//==============================================================================
// tmon_get_format
//==============================================================================
static function tmon_get_format (win_name)
	String win_name
	GetWindow $win_name note
	return NumberByKey(kFORMAT, S_Value)
end
//==============================================================================
// tmon_get_type
//==============================================================================
static function tmon_get_type (win_name)
	String win_name
	GetWindow $win_name note
	return NumberByKey(kTYPE, S_Value)
end
//==============================================================================
// tmon_get_access
//==============================================================================
static function tmon_get_access (win_name)
	String win_name
	GetWindow $win_name note
	return NumberByKey(kACCESS, S_Value)
end
//==============================================================================
// tmon_get_df
//==============================================================================
static function/S tmon_get_df (win_name)
	String win_name
	GetWindow $win_name note
	return StringByKey(kDF, S_Value)
end
//==============================================================================
//  tmon_get_data
//==============================================================================
static function/S tmon_get_data (win_name)
	String win_name
	GetWindow $win_name note
	return StringByKey(kDATA, S_Value)
end
//==============================================================================
//  tmon_get_timestamp
//==============================================================================
static function/S tmon_get_timestamp (win_name)
	String win_name
	GetWindow $win_name note
	return StringByKey(kTMST, S_Value)
end
//==============================================================================
// tmon_win_note_set_status
//==============================================================================
static function tmon_win_note_set_status (win_name, status)
	String win_name
	Variable status
	tmon_win_note_set_num_val(win_name, kTMS, status)
end
//==============================================================================
// tmon_get_status
//==============================================================================
static function tmon_get_status (win_name)
	String win_name
	GetWindow $win_name note
	return NumberByKey(kTMS, S_Value)
end
//==============================================================================
// tmon_get_value
//==============================================================================
static function/S tmon_get_value (win_name)
	String win_name
	GetWindow $win_name note
	return StringByKey(kVALUE, S_Value)
end
//==============================================================================
// tmon_set_wvalue
//==============================================================================
static function/S tmon_set_wvalue (win_name, wrt_value)
	String win_name
	String wrt_value
	tmon_win_note_set_str_val(win_name, kWVALUE, wrt_value)
end
//==============================================================================
// tmon_get_write_value
//==============================================================================
static function/S tmon_get_wvalue(win_name)
	String win_name
	GetWindow $win_name note
	return StringByKey(kWVALUE, S_Value)
end
//==============================================================================
// tmon_set_watrr
//==============================================================================
static function/S tmon_set_wattr (win_name, wattr)
	String win_name
	String wattr
	tmon_win_note_set_str_val(win_name, kWATTR, wattr)
end
//==============================================================================
// tmon_get_watrr
//==============================================================================
static function/S tmon_get_wattr (win_name)
	String win_name
	GetWindow $win_name note
	return StringByKey(kWATTR, S_Value)
end

