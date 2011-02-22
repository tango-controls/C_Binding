// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for Matlab
//
// = FILENAME
//   DataAdapter.h
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

#ifndef _DATA_ADAPTER_H_
#define _DATA_ADAPTER_H_

//=============================================================================
// FORWARD DECL
//=============================================================================
class DeviceDesc;

//=============================================================================
// #DEFINEs
//=============================================================================
#define DATA_ADAPTER (DataAdapter::instance())

//=============================================================================
// CLASS : DataAdapter 
//=============================================================================
class DataAdapter 
{
  friend class TangoBinding;

public:
  static DataAdapter* instance (void);
  //- Returns the unique instance of <DataAdapter>.
  
  int encode_argin (DeviceDesc* ddesc, 
                    int cmd_id,
                    const mxArray* argin,
                    Tango::DeviceData& dd_in);
  //- Converts <argin> from Matlab type to TANGO type

  int decode_argout (DeviceDesc* ddesc, 
                     int cmd_id,
                     Tango::DeviceData& dd_out,
                     mxArray*& argout);
  //- Converts <dd_out> from TANGO type to Matlab type
  
  int encode_attr (DeviceDesc* ddesc, 
                   int attr_id,
                   const mxArray* argin,
                   Tango::DeviceAttribute& attr_value);
  //- Converts <argin> from Matlab type to TANGO type

  int decode_attr (DeviceDesc* ddesc, 
                   int attr_id,
                   Tango::DeviceAttribute& attr_value, 
                   mxArray*& value);
  //- Converts <attr_value> from TANGO type to Matlab type

  template<typename T> std::vector<T>* 
        mxarray_to_vector (const mxArray* mx_array, const T& template_arg)
  //- 
  {
    //- check input 
    if (mx_array == 0) {
      MEX_UTILS->set_error("internal error",
			                     "unexpected null array",
			                     "DataAdapter::mxarray_to_vector");
     _SET_DEFAULT_PRHS_THEN_RETURN(0); 

    }
    //- # elements in mx_array
    long n = ::mxGetN(mx_array);
    //- allocate the returned vector
    std::vector<T>* v = new std::vector<T>(n);
    if (v == 0 || (*v).size() != n) {
      MEX_UTILS->set_error("out of memory",
			                     "std::vector allocation failed",
			                     "DataAdapter::mxarray_to_vector");
     _SET_DEFAULT_PRHS_THEN_RETURN(0); 
    }
    //- populate v
    for (long i = 0; i < n; i++) {
      (*v)[i] = static_cast<T>(::mxGetPr(mx_array)[i]);
    }
    return v;
  }

  template<typename T> std::vector<std::vector<T> >* 
        mxarray_to_vector_of_vector (const mxArray* mx_array, const T& template_arg)
  //-
  {
    //- check input 
    if (mx_array == 0) {
      MEX_UTILS->set_error("internal error",
			                     "unexpected null array",
			                     "DataAdapter::mxarray_to_vector");
     _SET_DEFAULT_PRHS_THEN_RETURN(0); 

    }
    if (::mxIsCell(mx_array) == false) {
      MEX_UTILS->set_error("invalid mxArray specified",
			                     "expected cell array",
			                     "DataAdapter::cellarray_to_vector_of_vector");
     _SET_DEFAULT_PRHS_THEN_RETURN(0); 
    }
    //- # elements in mx_array
    long n = ::mxGetN(mx_array);
    //- allocate the returned vector
    std::vector<std::vector<T> >* v = new std::vector<std::vector<T> >(n);
    if (v == 0 || (*v).size() != n) {
      MEX_UTILS->set_error("out of memory",
			                     "std::vector allocation failed",
			                     "DataAdapter::cellarray_to_vector_of_vector");
     _SET_DEFAULT_PRHS_THEN_RETURN(0); 
    }
    //- mx_array is an array of array
    long tmp_n = 0;
    mxArray* tmp = 0;
    for (long i = 0; i < n; i++) {
      //- for each array in the array
      tmp = ::mxGetCell(mx_array, i);
      if (tmp == 0) {
        delete v;
        MEX_UTILS->set_error("invalid mxArray specified",
			                       "unexpected empty cell",
			                       "DataAdapter::cellarray_to_vector_of_vector");
        _SET_DEFAULT_PRHS_THEN_RETURN(0); 
      }
      //- get # elements of tmp
      tmp_n = ::mxGetN(tmp);
      //- resize the i-th vector of v 
      (*v)[i].resize(tmp_n);
      //- populate the i-th vector of v 
      for (long j = 0; j < tmp_n; j++) {
        ((*v)[i])[j] = ((T*)::mxGetPr(tmp))[j]; 
      }  
    } 
    return v;
  }

  std::vector<std::string>* 
       mxarray_to_vector_of_string (const mxArray* mx_array);
  //- 

  std::vector<std::vector<std::string> >* 
       mxarray_to_vector_of_vector_of_string (const mxArray* mx_array);
  //- 

  std::vector<Tango::DevVarLongStringArray*>* 
       mxarray_to_vector_of_dvlsa (const mxArray* mx_array);
  //- 

  std::vector<Tango::DevVarDoubleStringArray*>* 
       mxarray_to_vector_of_dvdsa (const mxArray* mx_array);
  //- 

private:
  static int init (void);
  //- Instanciates the singleton.
  
  static void cleanup (void);
  //- Releases the singleton.

  static DataAdapter* instance_;
  //- The unique instance of <DataAdapter>.

  // = Disallow these operations (except for TangoBinding).
  //---------------------------------------------------------
  DataAdapter();
  DataAdapter (const DataAdapter&);
  virtual ~DataAdapter();
  void operator= (const DataAdapter&); 
};

//=============================================================================
// INLINED CODE
//=============================================================================
#if defined (_MEX_INLINE_)
  #include "DataAdapter.i"
#endif 

#endif // _DATA_ADAPTER_H_

   
   
