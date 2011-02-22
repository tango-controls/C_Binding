/******************************************************************************
 * 
 * File       :	c_tango.h
 *
 * Project    :	C client interface to Tango
 * 
 * Description:	Definitions necessay to write Tango clients in C
 *
 * Original   :	November 2007	
 *	
 * $Author$
 *
 * $Log$
 * Revision 1.6  2007/12/04 09:29:28  meyer
 * Added typedefs for c compiler
 *
 * Revision 1.5  2007/12/03 14:46:51  meyer
 * Added timeout and data source functions.
 *
 * Revision 1.4  2007/12/03 14:28:30  meyer
 * Added command and attribute query functions.
 *
 * Revision 1.3  2007/12/03 07:26:19  meyer
 * Data types implemented.
 *
 * Revision 1.2  2007/11/30 12:20:43  meyer
 * Added all data types
 *
 * Revision 1.1  2007/11/29 14:01:41  meyer
 * Initial revision
 *
 *
 ******************************************************************************/ 

#ifndef	C_TANGO_H
#define	C_TANGO_H


#include <sys/time.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif


/* Remapping of Tango enumerations.
   This is necessary because in C we don`t have namespaces */
	
enum TangoDataType {
	DEV_VOID = 0,
	DEV_BOOLEAN,
	DEV_SHORT,
	DEV_LONG,
	DEV_FLOAT,
	DEV_DOUBLE,
	DEV_USHORT,
	DEV_ULONG,
	DEV_STRING,
	DEVVAR_CHARARRAY,
	DEVVAR_SHORTARRAY,
	DEVVAR_LONGARRAY,
	DEVVAR_FLOATARRAY,
	DEVVAR_DOUBLEARRAY,
	DEVVAR_USHORTARRAY,
	DEVVAR_ULONGARRAY,
	DEVVAR_STRINGARRAY,
	DEVVAR_LONGSTRINGARRAY,
	DEVVAR_DOUBLESTRINGARRAY,
	DEV_STATE,
	CONST_DEV_STRING,
	DEVVAR_BOOLEANARRAY,
	DEV_UCHAR,
	DEV_LONG64,
	DEV_ULONG64,
	DEVVAR_LONG64ARRAY,
	DEVVAR_ULONG64ARRAY,
	DEV_INT
};

const char * const CmdArgTypeName[] = {
	"DevVoid",
	"DevBoolean",
	"DevShort",
	"DevLong",
	"DevFloat",
	"DevDouble",
	"DevUShort",
	"DevULong",
	"DevString",
	"DevVarCharArray",
	"DevVarShortArray",
	"DevVarLongArray",
	"DevVarFloatArray",
	"DevVarDoubleArray",
	"DevVarUShortArray",
	"DevVarULongArray",
	"DevVarStringArray",
	"DevVarLongStringArray",
	"DevVarDoubleStringArray",
	"DevState",
	"ConstDevString",
	"DevVarBooleanArray",
	"DevUChar",
	"DevLong64",
	"DevULong64",
	"DevVarLong64Array",
	"DevVarULong64Array",
	"DevInt"
};



/* Tango defice state definitions */
enum DevState { 
	ON, 
	OFF, 
	CLOSE, 
	OPEN, 
	INSERT, 
	EXTRACT, 
	MOVING, 
	STANDBY, 
	FAULT, 
	INIT, 
	RUNNING, 
	ALARM, 
	DISABLE, 
	UNKNOWN 
};

//
// The state name strings
//

const char * const DevStateName[] = {
	"ON",
	"OFF",
	"CLOSE",
	"OPEN",
	"INSERT",
	"EXTRACT",
	"MOVING",
	"STANDBY",
	"FAULT",
	"INIT",
	"RUNNING",
	"ALARM",
	"DISABLE",
	"UNKNOWN"
};

/* Attribute releated definitions */

enum AttrQuality { 
	ATTR_VALID, 
	ATTR_INVALID, 
	ATTR_ALARM, 
	ATTR_CHANGING, 
	ATTR_WARNING
};

enum AttrWriteType { 
	READ, 
	READ_WITH_WRITE, 
	WRITE, 
	READ_WRITE
};

enum AttrDataFormat { 
	SCALAR, 
	SPECTRUM, 
	IMAGE
};

enum DispLevel { 
	OPERATOR, 
	EXPERT
};


/* Error related definitions */

enum ErrSeverity { 
	WARN, 
	ERR, 
	PANIC
};

/* Proxy releated definitions */

enum DevSource { 
	DEV, 
	CACHE, 
	CACHE_DEV
};


#ifndef __cplusplus
typedef enum DevState 	  DevState;
typedef enum TangoDataType TangoDataType;
typedef enum AttrQuality AttrQuality;
typedef enum AttrWriteType AttrWriteType;
typedef enum AttrDataFormat AttrDataFormat;
typedef enum DispLevel DispLevel;
typedef enum ErrSeverity ErrSeverity;
typedef enum DevSource DevSource;
#endif

/* Handle the long proplem for 64 bit architectures */

typedef int 				DevLong;
typedef unsigned int 		DevULong;

#if __WORDSIZE == 64
typedef long             DevLong64;
typedef unsigned long      DevULong64;
#else
typedef long long         DevLong64;
typedef unsigned long long  DevULong64;
#endif


/* Array data structures */
struct VarBoolArray	{
							unsigned int length;
							bool   *sequence;
							};
typedef struct VarBoolArray 	VarBoolArray;

struct VarCharArray	{
							unsigned int length;
							unsigned char *sequence;
							};
typedef struct VarCharArray 	VarCharArray;


struct VarShortArray	{
							unsigned int length;
							short  	  *sequence;
							};
typedef struct VarShortArray 	VarShortArray;


struct VarUShortArray	{
							unsigned int 	length;
							unsigned short  *sequence;
							};
typedef struct VarUShortArray 	VarUShortArray;

/* Structures for 32 bit long values */
struct VarLongArray	{
							unsigned int length;
							int  	  	 *sequence;
							};
typedef struct VarLongArray 	VarLongArray;


struct VarULongArray	{
							unsigned int 	length;
							unsigned int   *sequence;
							};
typedef struct VarULongArray 	VarULongArray;


/* Structures for 64 bit long values */
struct VarLong64Array	{
							unsigned int length;
							DevLong64 *sequence;
							};
typedef struct VarLong64Array 	VarLong64Array;


struct VarULong64Array	{
							unsigned int  length;
							DevULong64 *sequence;
							};
typedef struct VarULong64Array 	VarULong64Array;



struct VarFloatArray {
							unsigned int length;
							float  	  *sequence;
							};								
typedef struct VarFloatArray VarFloatArray;


struct VarDoubleArray {
							unsigned int length;
							double  		*sequence;
							};								
typedef struct VarDoubleArray VarDoubleArray;


struct VarStringArray {
							unsigned int length;
							char  	  **sequence;
							};								
typedef struct VarStringArray VarStringArray;


struct VarStateArray	{
							unsigned int length;
							DevState  *sequence;
							};								
typedef struct VarStateArray VarStateArray;


/* union type to replace the CORBA ANY type */
typedef union {
				VarBoolArray   bool_arr;
				VarCharArray	char_arr;
				VarShortArray	short_arr;
				VarUShortArray	ushort_arr;
				VarLongArray	long_arr;
				VarULongArray	ulong_arr;
				VarLong64Array	long64_arr;
				VarULong64Array ulong64_arr;				
				VarFloatArray	float_arr;
				VarDoubleArray double_arr;
				VarStringArray string_arr;
				VarStateArray 	state_arr;
      	  } TangoAttributeData;


typedef union {
				bool			  bool_val;
				short					short_val;
				unsigned short			ushort_val;
				int				   long_val;
				unsigned int 		  ulong_val;
				float					float_val;
				double				double_val;
				char             *string_val;
				DevState			 state_val;
#if __WORDSIZE == 64
				long				  long64_val;
				unsigned long       ulong64_val;
#else
				long long 			long64_val;
				unsigned long long   ulong64_val;
#endif				
				
				VarBoolArray   bool_arr;
				VarCharArray	char_arr;
				VarShortArray	short_arr;
				VarUShortArray	ushort_arr;
				VarLongArray	long_arr;
				VarULongArray	ulong_arr;
				VarLong64Array	long64_arr;
				VarULong64Array ulong64_arr;				
				VarFloatArray	float_arr;
				VarDoubleArray double_arr;
				VarStringArray string_arr;
				VarStateArray 	state_arr;
      	  } TangoCommandData;


/* Command data structure */
typedef struct {
				TangoDataType		arg_type;
				TangoCommandData  cmd_data;
			  } CommandData;

/* Attribute data structure */
typedef struct  {
				TangoDataType 	  data_type;
				TangoAttributeData attr_data;
				
				AttrQuality 		quality;
				char 				    *name;
				int 					dim_x;
				int 					dim_y;
				struct timeval 	 time_stamp;
		  		} AttributeData;

struct AttributeDataList	{
				unsigned int 		length;
				AttributeData  *sequence;
				};
typedef struct AttributeDataList AttributeDataList;					


/* Error data structure */	
typedef struct {
				char 				*desc;
				char 				*reason;
				char 				*origin;
				ErrSeverity 	severity;
				} DevFailed;

struct ErrorStack	{
				unsigned int 	length;
				DevFailed  *sequence;
				};								
typedef struct ErrorStack ErrorStack;				
		
		
/* command query data structures */
struct CommandInfo {
				char 			*cmd_name;
				int 			cmd_tag;
				int 			in_type;
				int 			out_type;
				char 			*in_type_desc;
				char 			*out_type_desc;
				DispLevel	disp_level;
				};
typedef struct CommandInfo CommandInfo;


struct CommandInfoList	{
				unsigned int 		length;
				CommandInfo    *sequence;
				};
typedef struct CommandInfoList CommandInfoList;



/* attribute query data structures */
struct AttributeInfo {
				char 				*name;
				AttrWriteType 	writable;
				AttrDataFormat data_format;
				TangoDataType	data_type;
				int 				 max_dim_x;
				int 				 max_dim_y;
				char 			   *description;
				char 				*label;
				char 				*unit;
				char 				*standard_unit;
				char 				*display_unit;
				char 				*format;
				char 				*min_value;
				char 				*max_value;
				char 				*min_alarm;
				char 				*max_alarm;
				char 				*writable_attr_name;
				DispLevel 		disp_level;
				};
typedef struct AttributeInfo AttributeInfo;				

struct AttributeInfoList {
				unsigned int 		length;
				AttributeInfo *sequence;
				};
typedef struct AttributeInfoList AttributeInfoList;		
		
		
		
/* external function definitions */	

#ifdef __cplusplus
extern "C" {
#endif
/* Proxy related functions */	
bool tango_create_device_proxy (char *dev_name, void **proxy, ErrorStack *error);
bool tango_delete_device_proxy (void **proxy, ErrorStack *error);
bool tango_set_timeout_millis  (void *proxy, int millis, ErrorStack *error);
bool tango_get_timeout_millis  (void *proxy, int *millis, ErrorStack *error);
bool tango_set_source          (void *proxy, DevSource source, ErrorStack *error);
bool tango_get_source          (void *proxy, DevSource *source, ErrorStack *error);

/* Command related functions */
bool tango_command_query      (void *proxy, char *cmd_name, CommandInfo *cmd_info, ErrorStack *error);
bool tango_command_list_query (void *proxy, CommandInfoList *cmd_info_list, ErrorStack *error);
bool tango_command_inout      (void *proxy, char *cmd_name, CommandData *argin, CommandData *argout, ErrorStack *error);

void tango_free_CommandData   	(CommandData 		*command_data);
void tango_free_CommandInfo   	(CommandInfo 		*command_info);
void tango_free_CommandInfoList	(CommandInfoList 	*command_info_list);

/* Attribute related functions */
bool tango_get_attribute_list   (void *proxy, VarStringArray *attr_names, ErrorStack *error);
bool tango_get_attribute_config     (void *proxy, VarStringArray *attr_names, AttributeInfoList *attr_info_list, ErrorStack *error);
bool tango_attribute_list_query (void *proxy, AttributeInfoList *attr_info_list, ErrorStack *error);

bool tango_read_attribute   (void *proxy, char *attr_name, AttributeData *argout, ErrorStack *error);
bool tango_write_attribute  (void *proxy, AttributeData *argin, ErrorStack *error);
bool tango_read_attributes  (void *proxy, VarStringArray *attr_names, AttributeDataList *argout, ErrorStack *error);
bool tango_write_attributes (void *proxy, AttributeDataList *argin, ErrorStack *error);

void tango_free_AttributeData     (AttributeData 		*attribute_data);
void tango_free_AttributeDataList (AttributeDataList 	*attribute_data_list);
void tango_free_VarStringArray    (VarStringArray     *string_arr);
void tango_free_AttributeInfoList (AttributeInfoList 	*attribute_info_list);

/* Error releated functions */
void tango_free_ErrorStack  (ErrorStack *error_stack);
void tango_print_ErrorStack (ErrorStack *error_stack);
#ifdef __cplusplus
}
#endif

/* Missing features ???????? */

/* Query devices in database , query with filters */
/* read polling history, for attributes, for commands */
/* read and write device property values */

#endif
