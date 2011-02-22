//==============================================================================
// tools.ipf
//------------------------------------------------------------------------------
// N.Leclercq - SOLEIL
//==============================================================================

//==============================================================================
// METHODE D'ACCES AUX GLOBALES
//==============================================================================
#pragma rtGlobals=2

//==============================================================================
// CONSTANTES
//==============================================================================
static strconstant kTERM = "\r\n"
//------------------------------------------------------------------------------
static strconstant kTYPE = "TYPE"
static strconstant kVAL  = "VAL"		
static strconstant kLEN  = "LEN"
static strconstant kPATH = "PATH"
//------------------------------------------------------------------------------
static strconstant kSWAV = "SWAV"
static strconstant kNWAV = "NWAV" 
static strconstant kSVAR = "SVAR" 
static strconstant kNVAR = "NVAR" 
//------------------------------------------------------------------------------

//==============================================================================
// fonction : tools_r2e
//------------------------------------------------------------------------------
// PSD e- : conversion position -> energie. L'onde <c> contient les coeffs d'un 
// polynome de degre 5 (calibration), <r> est la valeur a convertir.
//==============================================================================
function tools_r2e (c, r)
	Wave c
	Variable r
	return c[0] + r*c[1] + r*r*c[2] + r*r*r*c[3] + r*r*r*r*c[4] + r*r*r*r*r*c[5]
end

//==============================================================================
// fonction : tools_e2r
//------------------------------------------------------------------------------
// PSD e- : conversion energie -> position. L'onde <c> contient les coeffs d'un 
// polynome de degre 5 (calibration), <e> est la valeur a convertir.
//==============================================================================
function tools_e2r (c, e)
	Wave c
	Variable e
	return c[0] + e*c[1] + e*e*c[2] + e*e*e*c[3] + e*e*e*e*c[4] + e*e*e*e*e*c[5]
end

//==============================================================================
// fonction : tools_xy2rt 
//------------------------------------------------------------------------------
// Conversion coords. rectangulaires  -> coords. polaires. <xy> est un complexe
// dont la partie reelle contient la coord x et la partie imaginaire, la coord y
// Retourne un "complex" dont la partie relle contient la coord r (module) et 
// la partie imaginaire la coord teta (argument) en DEGRES. 
// Exemple d'utilisation: print tools_xy2rt(cmplx(1,1)) -> affiche (1.41421,45)
//==============================================================================
function/C tools_xy2rt (xy)
	Variable/C xy
	Variable/C rt = r2polar(xy)
	return cmplx(real(rt), 180. * imag(rt) / pi)
end

//==============================================================================
// fonction : tools_rt2xy
//------------------------------------------------------------------------------
// Conversion coords. polaires -> coords. rectangulaires. <rt> est un complexe
// dont la partie reelle contient la coord polaire r (module) et la partie 
// imaginaire la coord polaire teta (argument) en DEGRES. Retourne un complexe 
// dont la partie relle contient la coord x et la partie imaginaire la coord y.
// Exemple d'utilisation : print tools_rt2xy(cmplx(sqrt(2),45)) -> affiche (1,1)
//==============================================================================
function/C tools_rt2xy (rt)
	Variable/C rt
	return p2rect(cmplx(real(rt), imag(rt) * pi / 180.))
end

//==============================================================================
// fonction : tools_nearest_multiple
//------------------------------------------------------------------------------
// Retourne le multiple de <resol> le plus proche de <val>
//==============================================================================
function tools_nearest_multiple (val, resol)
	Variable val
	Variable resol
	Variable base_val = val / resol
 	Variable int_part = trunc(base_val)
	if (abs(base_val - int_part) > 0.5)
		int_part += (val < 0) ? -1 : 1
	endif
	return int_part * resol
end

//==============================================================================
// fonction : tools_swap
//------------------------------------------------------------------------------
// Echange les valeurs de <a> et <b> ssi (a > b) == VRAI. Les parametres sont
// passes par reference (car modifies par la fonction).
//==============================================================================
function tools_swap_lohi (a, b)
	Variable &a
	Variable &b
	if (a > b)
		Variable temp = a
		a = b
		b = temp
	endif
end

//==============================================================================
// fonction : tools_delete_df
//------------------------------------------------------------------------------
// Detruit "proprement" le DF <df_to_delete>.
//==============================================================================
function tools_delete_df (df_to_delete)
	String df_to_delete
	tools_kill_df_graphs(df_to_delete)
	tools_kill_df_dep(df_to_delete)
	tools_kill_df(df_to_delete)
end

//==============================================================================
// fonction : tools_kill_df_graphs
//------------------------------------------------------------------------------
// Referme tout graph, layout, table ou fenetre d'XOP contenant une onde de <df>
// ou de l'un de ses sous-DF (fonction recursive).
//==============================================================================
function tools_kill_df_graphs (df)
	String df
	if (strlen(df) == 0 || DataFolderExists(df) == 0)
		return 0
	endif
	String previous_df = GetDataFolder(1)
	SetDataFolder df
	String sub_df
	Variable index = 0
	do
		sub_df = GetIndexedObjName(df, 4, index)
		if (strlen(sub_df) == 0)
			break
		endif
		tools_kill_df_graphs(df + ":" + sub_df)
		index += 1
	while(1)
	String current_df_wav_list = WaveList("*",";","")
	if (strlen(current_df_wav_list) == 0) 
		SetDataFolder previous_df
		return 0
	endif 
	String windows_type = "WIN:" + num2str(1+2+4+4096)
	String win_list = WinList("*",";",windows_type)
	if (strlen(win_list) == 0) 
		if (DataFolderExists(previous_df))
			SetDataFolder previous_df
		endif
		return 0
	endif 
	String win_name
	String win_wav_list
	String target_win
	Variable win_type
	Variable i = 0
	do
		win_name = StringFromList(i, win_list)
		if (strlen(win_name) == 0)
			break
		endif
		win_type = WinType(win_name)
		if (win_type != 0 && win_type != 13)
			target_win = "WIN:" + win_name
			if (strlen(WaveList("*",";",target_win)) != 0) 
				DoWindow/K $win_name
			endif
		elseif (win_type == 13)
				DoWindow/K $win_name
		endif
		i += 1
	while (1)
	SetDataFolder previous_df
	return 0
end

//==============================================================================
// fonction : tools_kill_df_dep
//------------------------------------------------------------------------------
// Detruit TOUTES les dependences sur les variables, les chaines de caracteres
// et les ondes de <df> et de ses sous-DF (fonction recursive).
//==============================================================================
function tools_kill_df_dep (df)
	String df
	if (strlen(df) == 0 || DataFolderExists(df) == 0)
		return 0
	endif
	String previous_df = GetDataFolder(1)
	SetDataFolder df
	String sub_df
	//--appels recursifs
	Variable index = 0
	do
		sub_df = GetIndexedObjName(df, 4, index)
		if (strlen(sub_df) == 0)
			break
		endif
		tools_kill_df_dep(df + ":" + sub_df)
		index += 1
	while(1)
	String obj
	//--destruction des dependences sur les ondes (obj.type 1)
	index = 0
	do 
		obj = GetIndexedObjName(df, 1, index)
		if (strlen(obj) == 0)
			break
		endif
		SetFormula $obj,""
		index += 1
	while (1)
	//--destruction des dependences sur les variables (obj.type 2)
	index = 0
	do 
		obj = GetIndexedObjName(df, 2, index)
		if (strlen(obj) == 0)
			break
		endif
		SetFormula $obj,""
		index += 1
	while (1)
	//--destruction des dependences sur les chaines (obj.type 3)
	index = 0
	do 
		obj = GetIndexedObjName(df, 3, index)
		if (strlen(obj) == 0)
			break
		endif
		SetFormula $obj,""
		index += 1
	while (1)
	//--restore DF precedent
	SetDataFolder previous_df
	return 0
end

//==============================================================================
// fonction : tools_kill_df
//------------------------------------------------------------------------------
// Detruit le data folder <df> (function recursive).
//==============================================================================
function tools_kill_df (df)
	String df
	if (strlen(df) == 0 || DataFolderExists(df) == 0)
		return 0
	endif
	String previous_df = GetDataFolder(1)
	SetDataFolder df
	String sub_df
	do
		sub_df = GetIndexedObjName(df, 4, 0)
		if (strlen(sub_df) == 0)
			break
		endif
		tools_kill_df(df + ":" + sub_df)
	while(1)
	KillDataFolder df
	if (DataFolderExists(previous_df))
		SetDataFolder previous_df
	endif
	return 0
end

//==============================================================================
// fonction : tools_get_parent_df
//==============================================================================
function/S tools_get_parent_df (df)
	String df
	if (strlen(df) == 0)
		return ""
	endif
	Variable len = strlen(df)
	Variable end_pos = len
	if (char2num(df[end_pos]) != char2num(":"))
		end_pos -= 1
	endif
	do
		end_pos -= 1
	while (char2num(df[end_pos]) != char2num(":"))
	return df[0, end_pos]
end

//==============================================================================
// fonction : tools_build_path
//==============================================================================
function/S tools_build_path (prefix, sufix)
	String prefix
	String sufix
	Variable len = strlen(prefix)
	if (len == 0)
		return sufix
	endif
	String path
	if (char2num(prefix[len - 1]) == char2num(":"))
		path = prefix + sufix
	else
		path = prefix + ":" +  sufix
	endif
	return path
end

//==============================================================================
// fonction : tools_target_df
//==============================================================================
function/S tools_target_df ()
	String previous_df = GetDataFolder(1)
	String pstr = "prompt=\"Use the red arrow to select the target folder.\""
	String boptions = ",showWaves=1,showVars=0,showStrs=0"
	Execute "CreateBrowser " + pstr + boptions
	String target_df = GetDataFolder(1)
	NVAR V_Flag = $tools_build_path(previous_df, "V_Flag")
	if (V_Flag == 0)
	 target_df = ""
	endif
	SetDataFolder previous_df
	KillVariables/Z V_Flag, S_BrowserList
	return target_df
end

//==============================================================================
// static fonction : object_type
//==============================================================================
static function/S object_type (str)
	String str
	return StringByKey(kTYPE, str)
end

//==============================================================================
// static fonction : object_path
//==============================================================================
static function/S object_path (str)
	String str
	return StringByKey(kPATH, str)
end

//==============================================================================
// static fonction : str_object_val
//==============================================================================
static function/S str_object_val (str)
	String str
	return StringByKey(kVAL, str)
end

//==============================================================================
// static fonction : num_object_val
//==============================================================================
static function num_object_val (str)
	String str
	return NumberByKey(kVAL, str)
end

//==============================================================================
// static fonction : wave_object_len
//==============================================================================
static function wave_object_len (str)
	String str
	return NumberByKey(kLEN, str)
end

//==============================================================================
// fonction : save_df_objets
//==============================================================================
function save_df_objets (df, full_path)
 String df
 String full_path
 Variable refNum
 Open refNum as full_path
 if (refNum == 0)
 		return -1
 endif
	write_df_objets(df, refNum)
 Close refNum
 return 0
end

//==============================================================================
// static fonction : write_df_objets
//==============================================================================
static function write_df_objets (df, refNum)
	String df
	Variable refNum
	String str
	if (strlen(df) == 0 || DataFolderExists(df) == 0)
		return 0
	endif
	String previous_df = GetDataFolder(1)
	SetDataFolder df
	String sub_df
	//--appels recursifs
	Variable index = 0
	do
		sub_df = GetIndexedObjName(df, 4, index)
		if (strlen(sub_df) == 0)
			break
		endif
		write_df_objets(df + ":" + sub_df, refNum)
		index += 1
	while(1)
	String obj
	//--sauvegarde des ondes (obj.type 1)
	index = 0
	do 
		obj = GetIndexedObjName(df, 1, index)
		if (strlen(obj) == 0)
			break
		endif
		WAVE wn = $tools_build_path (df, obj)
		Variable wave_type = WaveType(wn)
		String wave_type_str = kNWAV
		if (wave_type == 0)
			WAVE/T wt = $tools_build_path (df, obj)
			wave_type_str = kSWAV
		endif
		str = "TYPE:" +  wave_type_str  + ";PATH:" + df  + ":" 
		str += obj  + ";LEN:" + num2str(Numpnts(wn)) + ";" + kTERM
		fprintf refNum, "%s" str
		Variable i
		for (i = 0; i < Numpnts(wn); i += 1)
			if (wave_type == 0)
				fprintf refNum, "%s:%s[%d] = \"%s\"\r\n" df, obj, i, wt[i]  
			else
				fprintf refNum, "%s:%s[%d] = %g\r\n" df, obj, i, wn[i]  
			endif
		endfor
		index += 1
	while (1)
	//--sauvegarde des variables (obj.type 2)
	index = 0
	do 
		obj = GetIndexedObjName(df, 2, index)
		if (strlen(obj) == 0)
			break
		endif
		NVAR val = $tools_build_path (df, obj)
		str = "TYPE:" + kNVAR + ";PATH:" + df  + ":"
		str += obj  + ";VAL:" + num2str(val) + ";" + kTERM
		fprintf refNum, "%s" str
		index += 1
	while (1)
	//--sauvegarde des chaines (obj.type 3)
	index = 0
	do 
		obj = GetIndexedObjName(df, 3, index)
		if (strlen(obj) == 0)
			break
		endif
		SVAR str_var = $tools_build_path (df, obj)
		str = "TYPE:" + kSVAR + ";PATH:" + df  + ":" 
		str += obj  + ";VAL:" + str_var + ";" + kTERM 
		fprintf refNum, "%s" str
		index += 1
	while (1)
	//--restore DF precedent
	SetDataFolder previous_df
	return 0
end

//==============================================================================
// fonction : restore_df_objects
//==============================================================================
function restore_df_objects (full_path)
 String full_path
 Variable refNum
 Open/R/Z=1 refNum as full_path
	if (refNum == 0)
  return -1
	endif
	read_df_objets(refNum)
 Close refNum
 return 0
end

//==============================================================================
// fonction : read_df_objets
//==============================================================================
function read_df_objets (refNum)
 Variable refNum
 String str
 String obj_path
 Variable nobj_val
	String sobj_val
 do
		freadline refNum, str
		if (strlen(str) == 0)
			break
		endif
		obj_path = object_path(str)
		strswitch (object_type(str))
			case kNVAR:
				nobj_val = num_object_val(str)
			 Variable/G $obj_path = nobj_val
			 break
			case kSVAR:
				sobj_val = str_object_val(str)
			 String/G $obj_path = sobj_val
			 break
			case kNWAV:
			case kSWAV:
				nobj_val = wave_object_len(str)
				Variable i
				for (i = 0; i < nobj_val; i += 1)
				 freadline refNum, str
					execute(str)
				endfor
			 break
		endswitch
 while (1)
 return 0
end

//==============================================================================
// tools_get_listbox_colwidths
//==============================================================================
function tools_get_listbox_colwidths (lwname, wwname, min_wd)
	String lwname
	String wwname
	Variable min_wd 
	WAVE/T lw = $(lwname)
	WAVE ww = $(wwname)
	Variable nrow = dimsize(lw, 0)
	Variable ncol = numpnts(ww)
	Variable i, j, len, max_len, wd
	min_wd = min_wd + ceil(0.25 * min_wd)
	for (j = 0; j < ncol; j += 1)
		len = 0
		max_len = 0
		for (i = 0; i < nrow; i += 1)
			len = FontSizeStringWidth("Courier New", kLB_FONTSIZE, 0, lw[i][j])
			if (len > max_len)
				max_len = len 
			endif
		endfor
		wd =  max_len + ceil(0.25 * max_len)
		ww[j] = (wd > min_wd) ? wd : min_wd
	endfor
end 

//==============================================================================
// fonction : tools_wave_list
//==============================================================================
function tools_wave_list (df, list, type, dims)
	String df
	String &list
	Variable type
	Variable dims
	if (strlen(df) == 0 || DataFolderExists(df) == 0)
		return 0
	endif
	String previous_df = GetDataFolder(1)
	SetDataFolder df
	String sub_df
	Variable index = 0
	do
		sub_df = GetIndexedObjName(":", 4, index)
		if (strlen(sub_df) == 0)
			break
		endif
		tools_wave_list (sub_df, list, type, dims)
		index += 1
	while(1)
	String dims_option = ""
	if (dims > 0)
		dims_option = "DIMS:" + num2str(dims)
	endif
	String current_df_wav_list = WaveList("*",";",dims_option)
	if (strlen(current_df_wav_list) == 0) 
		SetDataFolder previous_df
		return 0
	endif 
	String wname
	Variable i = 0
	do
		wname = StringFromList(i, current_df_wav_list)
		if (strlen(wname) == 0)
			break
		endif
		if (type >= 0 && WaveType(wname) == type)
			list += GetDataFolder(1) + wname + ";"
		endif
		i += 1
	while (1)
	SetDataFolder previous_df
	return 0
end
//==============================================================================
// fonction : tools_wave_path - wave must be in the current datafolder
//==============================================================================
function/S tools_wave_path (w)
   WAVE &w
	return GetDataFolder(1) + NameOfWave(w)
end
//==============================================================================
// fonction : tools_str_list_add
//==============================================================================
function/S tools_str_list_add (item, list)
 String item
 String list
 if (strlen(list) != 0)
   list += ";" 
 endif
 list += item
 return list
end
//==============================================================================
// fonction : tools_str_list_remove
//==============================================================================
function/S tools_str_list_remove (item_tbr, list)
 String item_tbr
 String list
 String item
 Variable i = 0
 String new_list = ""
 do
  item = StringFromList(i, list)
  if (strlen(item) == 0)
    break
  endif
  if (cmpstr(item, item_tbr) != 0)
    new_list += item + ";" 
  endif
  i += 1
 while (1)
 return new_list
end