//=============================================================================
// XOP SPECIFIC RESOURCE : XOPF 1100 - XOP EXTERNAL FUNCTIONS DEFINITION
//=============================================================================
resource 'XOPF' (1100) {
  {
		//-- kEDS_START 
    "eds_start",
		F_EXTERNAL,
		NT_FP64,
		  {},
		//-- kEDS_RESET 
    "eds_reset",
		F_EXTERNAL,
		NT_FP64,
		  {},
		//-- kEDS_STOP 	
    "eds_stop",
		F_EXTERNAL,
		NT_FP64,
		  {},
		//-- kEDS_ABORT 	
    "eds_abort",
		F_EXTERNAL,
		NT_FP64,
		  {},	
    //-- kEDS_CLOSE 
    "eds_close",
		F_EXTERNAL,
		NT_FP64,
		  {},
		//-- kEDS_STATUS 
    "eds_status",
		F_EXTERNAL,
		NT_FP64,
		  {},
		//-- kEDS_DAQ_RTP
    "eds_daq_rtp",
		F_EXTERNAL,
		NT_FP64,
		  {},
    //-- kEDS_DAQ_ERRS
    "eds_daq_errors",
		F_EXTERNAL,
		NT_FP64,
		  {},
    //-- kEDS_DAQ_DATA_SIZE
    "eds_daq_data_size",
		F_EXTERNAL,
		NT_FP64,
		  {},
    //-- kEDS_DAQ_NAME
    "eds_daq_name",
		F_EXTERNAL,
		NT_FP64,
      {HSTRING_TYPE, NT_FP64,},	
    //-- kEDS_STATS
    "eds_stats\",
		F_EXTERNAL,
		NT_FP64,
		  {},
    //-- kEDS_CHECK_DEADLOCK
    "eds_check_deadlock",
		F_EXTERNAL,
		NT_FP64,
		  {},
    //-- kEDS_DUMP
    "eds_dump",
		F_EXTERNAL,
		NT_FP64,
		  {},
		//-- kEDS_TOF_HISTO 
    "eds_x1_tof_histogram",
		F_EXTERNAL,
		NT_FP64,
		  {WAVE_TYPE,},
		//-- kEDS_E_PSD_IMG 
    "eds_epsd_image",
		F_EXTERNAL,
		NT_FP64,
		  {WAVE_TYPE,},
		//-- kEDS_I_PSD_IMG 
    "eds_ipsd_image",
		F_EXTERNAL,
		NT_FP64,
		  {WAVE_TYPE,},
    //-- kEDS_CONF
    "eds_open_config_dialog",
		F_EXTERNAL,
		NT_FP64,
		  {},
    //-- kEDS_UPDATE_CONF_VARS
    "eds_update_config_vars",
		F_EXTERNAL,
		NT_FP64,
		  {}, 
    //-- kEDS_MAKE_TOF_WAVE 
  	"eds_make_x1_tof_wave",
    F_EXTERNAL,
		HSTRING_TYPE,
		  {HSTRING_TYPE,},		
    //-- kEDS_MAKE_EIMG_WAVE 
  	"eds_make_eimg_wave",
    F_EXTERNAL,
		HSTRING_TYPE,
		  {HSTRING_TYPE,},		
    //-- kEDS_MAKE_IIMG_WAVE 
  	"eds_make_iimg_wave",
    F_EXTERNAL,
		HSTRING_TYPE,
		  {HSTRING_TYPE,},
    //-- kEDS_NEW_DF
    "eds_new_data_filter",
    F_EXTERNAL,
		NT_FP64,
		  {HSTRING_TYPE, NT_FP64, NT_FP64, NT_FP64,},	
    //-- kEDS_REM_DF
    "eds_remove_data_filter",
    F_EXTERNAL,
		NT_FP64,
		  {HSTRING_TYPE,},		
    //-- kEDS_REM_ALL_DF
    "eds_remove_all_data_filters",
    F_EXTERNAL,
		NT_FP64,
		  {},
    //-- kEDS_RESET_DF
    "eds_reset_data_filter",
    F_EXTERNAL,
		NT_FP64,
		  {HSTRING_TYPE,},	
    //-- kEDS_MODIFY_DF
    "eds_modify_data_filter",
    F_EXTERNAL,
		NT_FP64,
      {HSTRING_TYPE, NT_FP64, NT_FP64,},
    //-- kEDS_IMG_DF
    "eds_filtered_img",
    F_EXTERNAL,
		NT_FP64,
		  {HSTRING_TYPE,},	
    //-- kEDS_TOF_DF
    "eds_filtered_tof",
    F_EXTERNAL,
		NT_FP64,
		  {HSTRING_TYPE,},	
    //-- kEDS_INFO_DF
    "eds_data_filter_info",
    F_EXTERNAL,
		NT_FP64,
		  {},
    //-- kEDS_NUM_DF
    "eds_num_data_filter",
    F_EXTERNAL,
		NT_FP64,
		  {},
		  {NT_FP64,},	
    //-- kO974_CLEAR_ALL
  	"o974_clear_all",
    F_EXTERNAL,
		NT_FP64,
		  {},
    //-- kO974_CLEAR_COUNTER
  	"o974_clear_counter",
    F_EXTERNAL,
		NT_FP64,
		  {NT_FP64,},	
    //-- kO974_CLEAR_COUNTERS
  	"o974_clear_counters",
    F_EXTERNAL,
		NT_FP64,
		  {},
    //-- kO974_COUNT_PRESET
  	"o974_clear_count_preset",
    F_EXTERNAL,
		NT_FP64,
		  {},
    //-- kO974_EVENT_PRESET
  	"o974_clear_event_preset",
    F_EXTERNAL,
		NT_FP64,
		  {},	
    //-- kO974_DISABLE_ALARM
  	"o974_disable_alarm",
    F_EXTERNAL,
		NT_FP64,
		  {},	
    //-- kO974_DISABLE_EVENT
  	"o974_disable_event",
    F_EXTERNAL,
		NT_FP64,
		  {},			
    //-- kO974_DISABLE_EVENT_PRESET
  	"o974_disable_event_preset",
    F_EXTERNAL,
		NT_FP64,
		  {},	
    //-- kO974_DISABLE_TRIG_START
  	"o974_disable_trigger_start",
    F_EXTERNAL,
		NT_FP64,
		  {},	
    //-- kO974_DISABLE_TRIG_STOP
  	"o974_disable_trigger_stop",
    F_EXTERNAL,
		NT_FP64,
		  {},			
    //-- kO974_ENABLE_ALARM
  	"o974_enable_alarm",
    F_EXTERNAL,
		NT_FP64,
		  {},		
    //-- kO974_ENABLE_EVT_AUTO
  	"o974_enable_event_auto",
    F_EXTERNAL,
		NT_FP64,
		  {},					
    //-- kO974_ENABLE_EVT_EXT
  	"o974_enable_event_external",
    F_EXTERNAL,
		NT_FP64,
		  {},
    //-- kO974_ENABLE_EVT_PRESET
  	"o974_enable_event_preset",
    F_EXTERNAL,
		NT_FP64,
		  {},	
    //-- kO974_ENABLE_LOCAL
  	"o974_enable_local",
    F_EXTERNAL,
		NT_FP64,
      {},	
    //-- kO974_ENABLE_REMOTE
  	"o974_enable_remote",
    F_EXTERNAL,
		NT_FP64,
		  {},	
    //-- kO974_ENABLE_TRIG_START
  	"o974_enable_trigger_start",
    F_EXTERNAL,
		NT_FP64,
		  {},			
    //-- kO974_ENABLE_TRIG_STOP
  	"o974_enable_trigger_stop",
    F_EXTERNAL,
		NT_FP64,
		  {},	
    //-- kO974_INIT
  	"o974_init",
    F_EXTERNAL,
		NT_FP64,
		  {},	
    //-- kO974_READ_COUNTER
  	"o974_read_counter",
    F_EXTERNAL,
		NT_FP64,
		  {NT_FP64,},
    //-- kO974_READ_TIMER
  	"o974_read_timer",
    F_EXTERNAL,
		NT_FP64,
		  {},	
    //-- kO974_SET_COUNT_PRESET
  	"o974_set_count_preset",
    F_EXTERNAL,
		NT_FP64,
		  {NT_FP64,},			
    //-- kO974_COUNT_PRESET_COMPLETED
  	"o974_count_preset_completed",
    F_EXTERNAL,
		NT_FP64,
      {},					
    //-- kO974_SET_EVT_PRESET
  	"o974_set_event_preset",
    F_EXTERNAL,
		NT_FP64,
		  {NT_FP64,},		
    //-- kO974_SET_MODE_EXT
  	"o974_set_mode_external",
    F_EXTERNAL,
		NT_FP64,
		  {},	
    //-- kO974_SET_MODE_MIN
  	"o974_set_mode_minutes",
    F_EXTERNAL,
		NT_FP64,
		  {},	
    //-- kO974_SET_MODE_SEC
  	"o974_set_mode_seconds",
    F_EXTERNAL,
		NT_FP64,
		  {},	
    //-- kO974_SET_DISPLAY
  	"o974_set_display",
    F_EXTERNAL,
		NT_FP64,
		  {NT_FP64,},			
    //-- kO974_SET_RADIX_BIN
  	"o974_set_radix_binary",
    F_EXTERNAL,
		NT_FP64,
		  {},	
    //-- kO974_SET_RADIX_DEC
  	"o974_set_radix_decimal",
    F_EXTERNAL,
		NT_FP64,
		  {},	
    //-- kO974_START
  	"o974_start",
    F_EXTERNAL,
		NT_FP64,
		  {},			
    //-- kO974_STOP
  	"o974_stop",
    F_EXTERNAL,
		NT_FP64,
		  {},
    //-- kO974_SELF_TEST
  	"o974_test",
    F_EXTERNAL,
		NT_FP64,
		  {},
    //-- kMONO_GO_TO
  	"mono_go_to",
    F_EXTERNAL,
		NT_FP64,
		  {NT_FP64, NT_FP64,},	
    //-- kMONO_DO_STEPS
  	"mono_do_steps",
    F_EXTERNAL,
		NT_FP64,
		  {NT_FP64, NT_FP64,},	
    //-- kMONO_MOTION_COMP
  	"mono_motion_completed",
    F_EXTERNAL,
		NT_FP64,
		  {},		
    //-- kMONO_WAIT_MOTION_COMP
  	"mono_wait_motion_completion",
    F_EXTERNAL,
		NT_FP64,
		  {NT_FP64,},
    //-- kMONO_GO_FORWARD
  	"mono_go_forward",
    F_EXTERNAL,
		NT_FP64,
		  {},					
    //-- kMONO_GO_BACKWARD
  	"mono_go_backward",
    F_EXTERNAL,
		NT_FP64,
		  {},	
    //-- kMONO_GO_STOP
  	"mono_stop",
    F_EXTERNAL,
		NT_FP64,
		  {},		
    //-- kMONO_SET_SPEED
  	"mono_set_speed",
    F_EXTERNAL,
		NT_FP64,
		  {NT_FP64,},	
    //-- kMONO_GET_SPEED
  	"mono_get_speed",
    F_EXTERNAL,
		NT_FP64,
		  {},	
    //-- kMONO_GET_CURRENT_POS
  	"mono_get_current_position",
    F_EXTERNAL,
		NT_FP64,
		  {},		
    //-- kMONO_RESET
  	"mono_reset",
    F_EXTERNAL,
		NT_FP64,
		  {},		
    //-- kNI6704_RESET
    "ni6704_reset",
    F_EXTERNAL,
		NT_FP64,
		  {},	
    //-- kNI6704_SET_UPDT_MODE
    "ni6704_set_update_mode",
    F_EXTERNAL,
		NT_FP64,
      {NT_FP64, NT_FP64,},
    //-- kNI6704_UPDT_OUTPUTS	
    "ni6704_update_outputs",
    F_EXTERNAL,
		NT_FP64,
		  {},	
    //-- kNI6704_SET_OUTPUT_V	
    "ni6704_set_output_voltage",
    F_EXTERNAL,
		NT_FP64,
      {NT_FP64, NT_FP64,},
    //-- kNI6704_SET_OUTPUT_I	
    "ni6704_set_output_current",
    F_EXTERNAL,
		NT_FP64,
      {NT_FP64, NT_FP64,},
    //-- kNI6704_APPLY_VETO
    "ni6704_apply_veto",
    F_EXTERNAL,
		NT_FP64,
      {NT_FP64,},
    //-- kNI6704_RELEASE_VETO
    "ni6704_release_veto",
    F_EXTERNAL,
		NT_FP64,
      {NT_FP64,},
    //--kOFFSET_PSD_IMAGE
    "offset_psd_image",
		F_EXTERNAL,
		NT_FP64,
      {WAVE_TYPE, WAVE_TYPE, NT_FP64, NT_FP64,},	
    //--kPROJ_PSD_IMAGE
    "proj_psd_image",
		F_EXTERNAL,
		NT_FP64,
      {WAVE_TYPE, WAVE_TYPE, NT_FP64, NT_FP64, NT_FP64, NT_FP64,},				
  }
};
