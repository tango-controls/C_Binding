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

#if defined(WIN32)
# pragma pack(pop, before_lv_structs)
#endif

#endif //- _LV_TYPES_H_

