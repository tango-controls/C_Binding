// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for LabVIEW
//
// = FILENAME
//   LvTypes.h
//
// = AUTHOR
//   Nicolas Leclercq - SOLEIL
//
// ============================================================================

#ifndef _LV_TYPES_H_
#define _LV_TYPES_H_

#if defined(WIN32)
# pragma pack(push, before_lv_structs)
# pragma pack(1)
#endif

//-----------------------------------------------------------------------------
// LABVIEW ARRAYS
//-----------------------------------------------------------------------------
template <typename _T> struct Labview1DArray 
{ 
  //- length of the array (i.e. # of element)
	long length;
  //- data base address
	_T data[1];
};
//-----------------------------------------------------------------------------
template <typename _T> struct Labview2DArray 
{ 
  //- dims of the array (i.e. # of element in each dimension)
	long length[2];
  //- data base address
	_T data[1];
};

//-----------------------------------------------------------------------------
// DECLARE MACRO
//-----------------------------------------------------------------------------
#define DECLARE_LABVIEW_1D_ARRAY(_TYPE, _LV_TYPE_NAME) \
  typedef Labview1DArray<_TYPE> _LV_TYPE_NAME; \
  typedef Labview1DArray<_TYPE> **_LV_TYPE_NAME##Hdl;
//-----------------------------------------------------------------------------
#define DECLARE_LABVIEW_2D_ARRAY(_TYPE, _LV_TYPE_NAME) \
  typedef Labview2DArray<_TYPE> _LV_TYPE_NAME; \
  typedef Labview2DArray<_TYPE> **_LV_TYPE_NAME##Hdl;

//-----------------------------------------------------------------------------
// INTEGER NUM ARRAYS
//-----------------------------------------------------------------------------
DECLARE_LABVIEW_1D_ARRAY(char, LvChar1DArray);
DECLARE_LABVIEW_1D_ARRAY(unsigned char, LvUChar1DArray);
DECLARE_LABVIEW_1D_ARRAY(short, LvShort1DArray);
DECLARE_LABVIEW_1D_ARRAY(unsigned short, LvUShort1DArray);
DECLARE_LABVIEW_1D_ARRAY(int, LvInt1DArray);
DECLARE_LABVIEW_1D_ARRAY(unsigned int, LvUInt1DArray);
DECLARE_LABVIEW_1D_ARRAY(long, LvLong1DArray);
DECLARE_LABVIEW_1D_ARRAY(unsigned long, LvULong1DArray);
//-----------------------------------------------------------------------------
DECLARE_LABVIEW_2D_ARRAY(char, LvChar2DArray);
DECLARE_LABVIEW_2D_ARRAY(unsigned char, LvUChar2DArray);
DECLARE_LABVIEW_2D_ARRAY(short, LvShort2DArray);
DECLARE_LABVIEW_2D_ARRAY(unsigned short, LvUShort2DArray);
DECLARE_LABVIEW_2D_ARRAY(int, LvInt2DArray);
DECLARE_LABVIEW_2D_ARRAY(unsigned int, LvUInt2DArray);
DECLARE_LABVIEW_2D_ARRAY(long, LvLong2DArray);
DECLARE_LABVIEW_2D_ARRAY(unsigned long, LvULong2DArray);

//-----------------------------------------------------------------------------
// FLOATING-POINT NUM ARRAYS
//-----------------------------------------------------------------------------
DECLARE_LABVIEW_1D_ARRAY(float, LvFloat1DArray);
DECLARE_LABVIEW_1D_ARRAY(double, LvDouble1DArray);
//-----------------------------------------------------------------------------
DECLARE_LABVIEW_2D_ARRAY(float, LvFloat2DArray);
DECLARE_LABVIEW_2D_ARRAY(double, LvDouble2DArray);

//-----------------------------------------------------------------------------
// STRING ARRAYS
//-----------------------------------------------------------------------------
DECLARE_LABVIEW_1D_ARRAY(unsigned char, LvString);
DECLARE_LABVIEW_1D_ARRAY(LvStringHdl, LvStringArray);

//-----------------------------------------------------------------------------
// TANGO DATA TYPE ENUM - MUST MATCH <av.vi> ENUM (same order required) 
//-----------------------------------------------------------------------------
enum {
  SCA_STATE =  0,
  SCA_STRING,
  SPE_STRING,
  IMG_STRING,
  SCA_BOOLEAN,
  SPE_BOOLEAN,
  IMG_BOOLEAN,
  SCA_UCHAR,
  SPE_UCHAR,
  IMG_UCHAR,
  SCA_SHORT,
  SPE_SHORT,
  IMG_SHORT,
  SCA_USHORT,
  SPE_USHORT,
  IMG_USHORT,
  SCA_LONG,
  SPE_LONG,
  IMG_LONG,
  SCA_ULONG,
  SPE_ULONG,
  IMG_ULONG,
  SCA_FLOAT,
  SPE_FLOAT,
  IMG_FLOAT,
  SCA_DOUBLE,
  SPE_DOUBLE,
  IMG_DOUBLE,
  SCA_LONG64,
  SPE_LONG64,
  IMG_LONG64,
  SCA_ULONG64,
  SPE_ULONG64,
  IMG_ULONG64,
  UNKNOWN_DATA_TYPE
};
//-----------------------------------------------------------------------------
// TANGO ATTRIBUTE VALUE CLUSTER
//-----------------------------------------------------------------------------
typedef struct _AttributeValue 
{
  //- device name
	LvStringHdl dev_name;
  //- attribute name
	LvStringHdl attr_name;
  //- attribute quality value
	short int quality_val;
  //- attribute quality string
	LvStringHdl quality_str;
  //- timestamp
	double timestamp;
  //- x dim.size
	long dim_x;
  //- y dim.size
	long dim_y;
  //- actual attribute value (generic/polymorphic representation)
  void * data; 
  //- error code (-1 if exception thrown, 0 otherwise)
	long error_code;
  //- error stack 
	LvStringArrayHdl error_stack;
  //- data type
	unsigned short int data_type;
} LvAttributeValue;
//-----------------------------------------------------------------------------
typedef LvAttributeValue  *LvAttributeValuePtr;
typedef LvAttributeValuePtr *LvAttributeValueHdl;
//-----------------------------------------------------------------------------
typedef struct _AttributeValueArray
{ 
  //- length of the array (i.e. # of element)
	long length;
  //- handles to attr_values 
	LvAttributeValue attr_values[1];
} LvAttributeValueArray;
//-----------------------------------------------------------------------------
typedef LvAttributeValueArray *LvAttributeValueArrayPtr;
typedef LvAttributeValueArray **LvAttributeValueArrayHdl;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// TANGO CMD ARGIN/ARGOUT CLUSTER
//-----------------------------------------------------------------------------
typedef struct _ArgIO 
{
  //- numeric data (generic/polymorphic representation)
  void * num_data;
  //- string data
  LvStringArrayHdl str_data;
} LvArgIO;
//-----------------------------------------------------------------------------
typedef LvArgIO *LvArgIOPtr;
typedef LvArgIOPtr *LvArgIOHdl;
//-----------------------------------------------------------------------------
typedef Labview1DArray<unsigned char> LvGenericNumArray;
typedef LvGenericNumArray *LvGenericNumArrayPtr;
typedef LvGenericNumArrayPtr *LvGenericNumArrayHdl;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// REQUIRED SPACE FOR MEMORY ADDRESS STORAGE [32 vs 64 bits platforms]
//-----------------------------------------------------------------------------
#if (_TBFL_HAS_64_BITS_PLATFORM_ == 1)
//- uQ: lv type code for 64-bits unsigned integer
# define LvMemoryAddress uQ 
#else
//- uL: lv type code for 32-bits unsigned integer
# define LvMemoryAddress uL 
#endif

#if defined(WIN32)
# pragma pack(pop, before_lv_structs)
#endif

#endif //- _LV_TYPES_H_

