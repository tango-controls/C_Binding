// ============================================================================
//
// = CONTEXT
//   Tango Generic Client for Igor Pro
//
// = FILENAME
//   DevClientApi.h
//
// = AUTHOR
//   NL - SOLEIL - March 2002
//
// ============================================================================

// ============================================================================
// DEPENDENCIES
// ============================================================================
#include "XDK_StandardHeaders.h"
#include "TangoAPI.h"
        
namespace TangoApi {

// ============================================================================
// DevData::DevData - ctor
// ============================================================================
DevData::DevData ()
: ex_flag_(DevData::ExFlag::on),
  any_ (new CORBA::Any)
{

}

// ============================================================================
// DevData::DevData - copy ctor
// ============================================================================
DevData::DevData (const DevData& _src) 
 : ex_flag_(_src.ex_flag_), 
   any_(_src.any_)
{

}

// ============================================================================
// DevData::~DevData - dtor
// ============================================================================
DevData::~DevData ()
{
  // no-op dtor (this->any_ is a CORBA::Any_var)
}

// ============================================================================
// DevData::operator=
// ============================================================================
DevData& DevData::operator= (const DevData& _src)
{
  if (&_src == this)
    return *this;
  this->ex_flag_ = _src.ex_flag_;
  this->any_ = _src.any_;
  return *this;
}

// ============================================================================
// DevData::exceptions 
// ============================================================================
void DevData::exceptions (DevData::ExFlag _flag)
{
	this->ex_flag_ = _flag;
}

// ============================================================================
// DevData::is_valid
// ============================================================================
bool DevData::is_valid (void) const
{
	CORBA::TypeCode_var tc = this->any_->type();
	if (tc->equal(CORBA::_tc_null)) {
    if (this->ex_flag_) {
      TangoSys_OMemStream reason;
      reason << "invalid data" << ends;
      TangoSys_OMemStream desc;
      desc << "internal error (tried to operate on an empty CORBA::Any)" << ends;
      Tango::Except::throw_exception(reason.str(), 
                                     desc.str(), 
                                     (const char*)"DevData::is_valid");
		}
		return false;
	}
	return true;
}

// ============================================================================
// DevData::operator<< - insert a bool into DevData
// ============================================================================
void DevData::operator << (const bool& datum)
{
	this->any_ <<= CORBA::Any::from_boolean(datum);
}

// ============================================================================
// DevData::operator >> - extract a short from DevData
// ============================================================================
void DevData::operator >> (bool& datum)
{
	if (! this->is_valid()) {
		datum = 0;
	}
	else {
		this->any_ >>= CORBA::Any::to_boolean(datum);
	}
}

// ============================================================================
// DevData::operator<< - insert an unsigned char into DevData
// ============================================================================
void DevData::operator << (const unsigned char& datum)
{
  CORBA::Any::from_char fc(datum);
	this->any_ <<= fc;
}

// ============================================================================
// DevData::operator >> - extract a short from DevData
// ============================================================================
void DevData::operator >> (unsigned char& datum)
{
	if (! this->is_valid()) {
		datum = 0;
	}
	else {
    CORBA::Any::to_char tc(datum);
		this->any_ >>= tc;
	}
}

// ============================================================================
// DevData::operator<< - insert a short into DevData
// ============================================================================
void DevData::operator << (const short& datum)
{
	this->any_ <<= datum;
}

// ============================================================================
// DevData::operator >> - extract a short from DevData
// ============================================================================
void DevData::operator >> (short& datum)
{
	if (! this->is_valid()) {
		datum = 0;
	}
	else {
		this->any_ >>= datum;
	}
}

// ============================================================================
// DevData::operator<< - insert a unsigned short into DevData
// ============================================================================

void DevData::operator << (const unsigned short& datum)
{
	this->any_ <<= datum;
}

// ============================================================================
// DevData::operator >> - extract a unsigned short from DevData
// ============================================================================
void DevData::operator >> (unsigned short& datum)
{
	if (! this->is_valid()) {
		datum = 0;
	}
	else {
		this->any_ >>= datum;
	}
}

// ============================================================================
// DevData::operator<< - insert a long into DevData
// ============================================================================
void DevData::operator << (const long& datum)
{
	this->any_ <<= datum;
}

// ============================================================================
// DevData::operator >> - extract a long from DevData
// ============================================================================
void DevData::operator >> (long& datum)
{
	if (! this->is_valid()) {
		datum = 0;
	}
	else {
		this->any_ >>= datum;
	}
}

// ============================================================================
// DevData::operator << - insert a unsigned short into DevData
// ============================================================================
void DevData::operator << (const unsigned long& datum)
{
	this->any_ <<= datum;
}

// ============================================================================
// DevData::operator >> - extract a unsigned short from DevData
// ============================================================================
void DevData::operator >> (unsigned long& datum)
{
	if (! this->is_valid()){
		datum = 0;
	}
	else {
		this->any_ >>= datum;
	}
}

// ============================================================================
// DevData::operator << - insert a float into DevData
// ============================================================================
void DevData::operator << (const float& datum)
{
	this->any_ <<= datum;
}

// ============================================================================
// DevData::operator >> - extract a float from DevData
// ============================================================================
void DevData::operator >> (float& datum)
{
	if (! this->is_valid()) {
		datum = 0.0;
	}
	else {
		this->any_ >>= datum;
	}
}

// ============================================================================
// DevData::operator << - insert a double into DevData
// ============================================================================
void DevData::operator << (const double& datum)
{
	this->any_ <<= datum;
}

// ============================================================================
// DevData::operator >> - extract a double from DevData
// ============================================================================
void DevData::operator >> (double& datum)
{
	if (! this->is_valid()) {
		datum = 0.0;
	}
	else {
		this->any_ >>= datum;
	}
}

// ============================================================================
// DevData::operator << - insert a string into DevData
// ============================================================================
void DevData::operator << (const std::string& datum)
{
	this->any_ <<= datum.c_str();
}

// ============================================================================
// DevData::operator >> - extract a string from DevData
// ============================================================================
void DevData::operator >> (std::string& datum)
{
	if (! this->is_valid()) {
		datum = "device returned no data";
	}
	else {
		const char *c_string;
		this->any_ >>= c_string;
		datum = c_string;
	}
}

// ============================================================================
// DevData::operator << - insert a char* into DevData
// ============================================================================
void DevData::operator << (char*& datum)
{
	this->any_ <<= datum;
}

// ============================================================================
// DevData::operator << - insert a const char* into DevData
// ============================================================================
void DevData::operator << (const char*& datum)
{
	this->any_ <<= datum;
}

// ============================================================================
// DevData::operator >> - extract a const char* from DevData
// ============================================================================
void DevData::operator >> (const char*& datum)
{
	if (! this->is_valid()) {
		datum = " ";
	}
	else {
		this->any_ >>= datum;
	}
}

// ============================================================================
// DevData::operator <<(DevState &) - insert a DevState into DevData
// ============================================================================
void DevData::operator << (const Tango::DevState& datum)
{
	this->any_.inout() <<= datum;
}

// ============================================================================
// DevData::operator >> - extract a DevState from DevData
// ============================================================================
void DevData::operator >> (Tango::DevState& datum)
{
	if (! this->is_valid()) {
    datum = Tango::UNKNOWN;
	}
	else {
		this->any_ >>= datum;
	}
}

// ============================================================================
// DevData::operator << - insert a vector<bool> into DevData
// ============================================================================
void DevData::operator << (const vector<bool>& datum)
{
  Tango::DevVarBooleanArray *bool_array = new Tango::DevVarBooleanArray();
	bool_array->length(datum.size());
  ::memcpy(bool_array->get_buffer(), 
             datum.begin(), 
             datum.size() * sizeof(bool)); 
	this->any_.inout() <<= bool_array;
}

// ============================================================================
// DevData::operator >> - extract a vector<bool> from DevData
// ============================================================================
void DevData::operator >> (vector<bool>& datum)
{
  if (! this->is_valid()) {
		datum.resize(0);
	}
	else {
		const Tango::DevVarBooleanArray *bool_array;
		this->any_ >>= bool_array;
		datum.resize(bool_array->length());
    const bool* base_addr = bool_array->get_buffer();
    datum.assign(base_addr, base_addr + bool_array->length());
	}
}

// ============================================================================
// DevData::operator << - insert a vector<unsigned char> into DevData
// ============================================================================
void DevData::operator << (const vector<unsigned char>& datum)
{
  Tango::DevVarCharArray *char_array = new Tango::DevVarCharArray();
	char_array->length(datum.size());
  ::memcpy(char_array->get_buffer(), 
           datum.begin(), 
           datum.size() * sizeof(unsigned char)); 
	this->any_.inout() <<= char_array;
}

// ============================================================================
// DevData::operator >> - extract a vector<unsigned char> from DevData
// ============================================================================
void DevData::operator >> (vector<unsigned char>& datum)
{
  if (! this->is_valid()) {
		datum.resize(0);
	}
	else {
		const Tango::DevVarCharArray *char_array;
		this->any_ >>= char_array;
		datum.resize(char_array->length());
    const unsigned char* base_addr = char_array->get_buffer();
    datum.assign(base_addr, base_addr + char_array->length());
	}
}

// ============================================================================
// DevData::operator << - insert a vector<short> into DevData
// ============================================================================
void DevData::operator << (const vector<short>& datum)
{
	Tango::DevVarShortArray *short_array = new Tango::DevVarShortArray();
	short_array->length(datum.size());
  ::memcpy(short_array->get_buffer(), 
           datum.begin(), 
           datum.size() * sizeof(short)); 
	this->any_.inout() <<= short_array;
}

// ============================================================================
// DevData::operator >> - extract a vector<short> from DevData
// ============================================================================
void DevData::operator >> (vector<short>& datum)
{
	if (! this->is_valid()) {
		datum.resize(0);
	}
	else {
		const Tango::DevVarShortArray *short_array;
		this->any_ >>= short_array;
		datum.resize(short_array->length());
    const short* base_addr = short_array->get_buffer();
    datum.assign(base_addr, base_addr + short_array->length());
	}
}

// ============================================================================
// DevData::operator << - insert a vector<unsigned short> into DevData
// ============================================================================
void DevData::operator << (const vector<unsigned short>& datum)
{
	Tango::DevVarUShortArray *ushort_array = new Tango::DevVarUShortArray();
	ushort_array->length(datum.size());
  ::memcpy(ushort_array->get_buffer(), 
           datum.begin(), 
           datum.size() * sizeof(unsigned short));
	this->any_.inout() <<= ushort_array;
}

// ============================================================================
//
// DevData::operator >> - extract a vector<unsigned short> from DevData
//
// ============================================================================
void DevData::operator >> (vector<unsigned short>& datum)
{
  if (! this->is_valid()) {
		datum.resize(0);
	}
	else {
		const Tango::DevVarUShortArray *ushort_array;
		this->any_ >>= ushort_array;
		datum.resize(ushort_array->length());
    const unsigned short* base_addr = ushort_array->get_buffer();
    datum.assign(base_addr, base_addr + ushort_array->length());
	}
}

// ============================================================================
// DevData::operator << - insert a vector<long> into DevData
// ============================================================================
void DevData::operator << (const vector<long>& datum)
{
	Tango::DevVarLongArray *long_array = new Tango::DevVarLongArray();
	long_array->length(datum.size());
  ::memcpy(long_array->get_buffer(), 
           datum.begin(), 
           datum.size() * sizeof(long));
	this->any_.inout() <<= long_array;
}

// ============================================================================
// DevData::operator >> - extract a vector<long> from DevData
// ============================================================================
void DevData::operator >> (vector<long>& datum)
{
	if (! this->is_valid()) {
		datum.resize(0);
	}
	else {
		const Tango::DevVarLongArray *long_array;
		this->any_ >>= long_array;
		datum.resize(long_array->length());
    const long* base_addr = long_array->get_buffer();
    datum.assign(base_addr, base_addr + long_array->length());
	}
}

// ============================================================================
// DevData::operator << - insert a vector<unsigned long> into DevData
// ============================================================================
void DevData::operator << (const vector<unsigned long>& datum)
{
	Tango::DevVarULongArray *ulong_array = new Tango::DevVarULongArray();
	ulong_array->length(datum.size());
  ::memcpy(ulong_array->get_buffer(), 
           datum.begin(), 
           datum.size() * sizeof(unsigned long));
	this->any_.inout() <<= ulong_array;
}

// ============================================================================
// DevData::operator >> - extract a vector<unsigned long> from DevData
// ============================================================================
void DevData::operator >> (vector<unsigned long>& datum)
{
	if (! this->is_valid()) {
		datum.resize(0);
	}
	else {
		const Tango::DevVarULongArray *ulong_array;
		this->any_ >>= ulong_array;
		datum.resize(ulong_array->length());
    const unsigned long* base_addr = ulong_array->get_buffer();
    datum.assign(base_addr, base_addr + ulong_array->length());
	}
}

// ============================================================================
// DevData::operator << - insert a vector<float> into DevData
// ============================================================================
void DevData::operator << (const vector<float>& datum)
{
	Tango::DevVarFloatArray *float_array = new Tango::DevVarFloatArray();
	float_array->length(datum.size());
  ::memcpy(float_array->get_buffer(), 
           datum.begin(), 
           datum.size() * sizeof(float));
	this->any_.inout() <<= float_array;
}

// ============================================================================
// DevData::operator >> - extract a vector<float> from DevData
// ============================================================================
void DevData::operator >> (vector<float>& datum)
{
	if (! this->is_valid()) {
		datum.resize(0);
	}
	else {
		const Tango::DevVarFloatArray *float_array;
		this->any_ >>= float_array;
		datum.resize(float_array->length());
    const float* base_addr = float_array->get_buffer();
    datum.assign(base_addr, base_addr + float_array->length());
	}
}

// ============================================================================
// DevData::operator << - insert a vector<double> into DevData
// ============================================================================
void DevData::operator << (const vector<double>& datum)
{
	Tango::DevVarDoubleArray *double_array = new Tango::DevVarDoubleArray();
	double_array->length(datum.size());
  ::memcpy(double_array->get_buffer(), 
           datum.begin(), 
           datum.size() * sizeof(double));
	this->any_.inout() <<= double_array;
}

// ============================================================================
// DevData::operator >> - extract a vector<double> from DevData
// ============================================================================
void DevData::operator >> (vector<double>& datum)
{
	if (! this->is_valid()) {
		datum.resize(0);
	}
	else {
		const Tango::DevVarDoubleArray *double_array;
		this->any_ >>= double_array;
		datum.resize(double_array->length());
    const double* base_addr = double_array->get_buffer();
    datum.assign(base_addr, base_addr + double_array->length());
	}
}

// ============================================================================
// DevData::operator << - insert a vector<string> into DevData
// ============================================================================
void DevData::operator << (const vector<std::string>& datum)
{
	Tango::DevVarStringArray *string_array = new Tango::DevVarStringArray();
	string_array->length(datum.size());
	for (int i = 0; i < datum.size(); i++) {
    (*string_array)[i] = CORBA::string_dup(datum[i].c_str());
	}
	this->any_.inout() <<= string_array;
}

// ============================================================================
// DevData::operator >> - extract a vector<string> from DevData
// ============================================================================
void DevData::operator >> (vector<std::string>& datum)
{
	if (! this->is_valid()) {
		datum.resize(0);
	}
	else {
		const Tango::DevVarStringArray *string_array;
		this->any_ >>= string_array;
		datum.resize(string_array->length());
		for (int i = 0; i < string_array->length(); i++) {
			datum[i] = (*string_array)[i];
		}
	}
}

// ============================================================================
// DevData::insert - insert a pair of vector<long>,vector<string> 
// ============================================================================
void DevData::insert (const vector<long> &long_datum, 
                      const vector<std::string>& string_datum)
{
	Tango::DevVarLongStringArray *array = new Tango::DevVarLongStringArray();
	array->lvalue.length(long_datum.size());
  ::memcpy(array->lvalue.get_buffer(), 
           long_datum.begin(), 
           long_datum.size() * sizeof(long));
	array->svalue.length(string_datum.size());
	for (int i = 0; i < string_datum.size(); i++) {
    array->svalue[i] = CORBA::string_dup(string_datum[i].c_str());
	}
	this->any_.inout() <<= array;
}

// ============================================================================
// DevData::extract - extract a pair of vector<long>,vector<string>
// ============================================================================
void DevData::extract (vector<long> &long_datum, 
                       vector<std::string>& string_datum)
{
	if (! this->is_valid()){
		long_datum.resize(0);
		string_datum.resize(0);
	}
	else {
		const Tango::DevVarLongStringArray *array; 
		this->any_ >>= array;
		long_datum.resize(array->lvalue.length());
    const long* base_addr = array->lvalue.get_buffer();
    long_datum.assign(base_addr, base_addr + array->lvalue.length());
		string_datum.resize(array->svalue.length());
		for (int i = 0; i < string_datum.size(); i++) {
			string_datum[i] = (array->svalue)[i];
		}
	}
}

// ============================================================================
// DevData::insert - insert a pair of vector<double>,vector<string>
// ============================================================================
void DevData::insert (const vector<double> &double_datum, 
                      const vector<std::string>& string_datum)
{
	Tango::DevVarDoubleStringArray *array = new Tango::DevVarDoubleStringArray();
	array->dvalue.length(double_datum.size());
  ::memcpy(array->dvalue.get_buffer(), 
           double_datum.begin(), 
           double_datum.size() * sizeof(double));
	array->svalue.length(string_datum.size());
	for (int i = 0; i < string_datum.size(); i++) {
    array->svalue[i] = CORBA::string_dup(string_datum[i].c_str());
	}
	this->any_.inout() <<= array;
}

// ============================================================================
// DevData::extract - extract a pair of vector<double>,vector<string>
// ============================================================================
void DevData::extract (vector<double> &double_datum, 
                       vector<std::string>& string_datum)
{
	if (! this->is_valid()) {
		double_datum.resize(0);
		string_datum.resize(0);
	}
	else {
		const Tango::DevVarDoubleStringArray *array; 
		this->any_ >>= array;
		double_datum.resize(array->dvalue.length());
    const double* base_addr = array->dvalue.get_buffer();
    double_datum.assign(base_addr, base_addr + array->dvalue.length());
		string_datum.resize(array->svalue.length());
		for (int i = 0; i < string_datum.size(); i++) {
			string_datum[i] = array->svalue[i];
		}
	}
}

#if defined (_XDK_XOP_)
//=============================================================================
// TEMPLATE CLASS Insert - member num_wave works for waves with up to 4 dims
//=============================================================================
template <class A, class T> class Insert
{
public:
  static int num_wave(waveHndl _wh, CORBA::Any_var& any_)
  {
    long ndims = 0;
    MDWaveDims dim_size;
    if (::MDGetWaveDimensions(_wh, &ndims, dim_size)) {
      return kError;
    }
    long np = dim_size[0];
    switch (ndims) {
      case 2:
        np *= dim_size[1];
        break;
      case 3:
        np *= dim_size[1] * dim_size[2];
        break;
      case 4:
        np *= dim_size[1] * dim_size[2] * dim_size[3];
        break;
    }
    A *array = new A;
    if (array == 0) {
      return kError;
    }
    array->length(np);
    long offset;
    if (::MDAccessNumericWaveData(_wh, kMDWaveAccessMode0, &offset)) {
      return kError;
    }
    /*::HLock((Handle)_wave);*/
    char *base_addr = static_cast<char*>(*_wh) + offset;
    ::memcpy(array->get_buffer(), base_addr, np * sizeof(T));
    /*::HUnlock((Handle)_wave);*/ 
    any_.inout() <<= array;
    return kNoError;
  }
};

// ============================================================================
// DevData::operator <<
// ============================================================================
void DevData::operator << (const waveHndl _wh)
{
  switch (::WaveType(_wh)) 
  {
    case NT_I8: 
    case NT_I8|NT_UNSIGNED: 
    {
      if (Insert<Tango::DevVarCharArray, unsigned char>::num_wave(_wh, this->any_)) {
        if (this->ex_flag_) {
          char wname[MAX_OBJ_NAME + 1];
          ::WaveName(_wh, wname);
          TangoSys_OMemStream reason;
          reason << "XOP internal error" << ends;
          TangoSys_OMemStream desc;
          desc << "could not extract data from wave " << wname << ends;
          Tango::Except::throw_exception(reason.str(), desc.str(), 
                                   (const char*)"DevData::operator<<");
		    }
        return;
      }
    } break;
    case NT_I16: 
    {
      if (Insert<Tango::DevVarShortArray, short>::num_wave(_wh, this->any_)) {
        if (this->ex_flag_) {
          char wname[MAX_OBJ_NAME + 1];
          ::WaveName(_wh, wname);
          TangoSys_OMemStream reason;
          reason << "XOP internal error" << ends;
          TangoSys_OMemStream desc;
          desc << "could not extract data from wave " << wname << ends;
          Tango::Except::throw_exception(reason.str(), desc.str(), 
                                   (const char*)"DevData::operator<<");
		    }
        return;
      } 
    } break;
    case NT_I16|NT_UNSIGNED: 
    {
      if (Insert<Tango::DevVarUShortArray, unsigned short>::num_wave(_wh, this->any_)) {
        if (this->ex_flag_) {
          char wname[MAX_OBJ_NAME + 1];
          ::WaveName(_wh, wname);
          TangoSys_OMemStream reason;
          reason << "XOP internal error" << ends;
          TangoSys_OMemStream desc;
          desc << "could not extract data from wave " << wname << ends;
          Tango::Except::throw_exception(reason.str(), desc.str(), 
                                   (const char*)"DevData::operator<<");
		    }
        return;
      } 
    } break;
    case NT_I32: 
    {
      if (Insert<Tango::DevVarLongArray, long>::num_wave(_wh, this->any_)) {
        if (this->ex_flag_) {
          char wname[MAX_OBJ_NAME + 1];
          ::WaveName(_wh, wname);
          TangoSys_OMemStream reason;
          reason << "XOP internal error" << ends;
          TangoSys_OMemStream desc;
          desc << "could not extract data from wave " << wname << ends;
          Tango::Except::throw_exception(reason.str(), desc.str(), 
                                   (const char*)"DevData::operator<<");
		    }
        return;
      }  
    } break;
    case NT_I32|NT_UNSIGNED: 
    {
      if (Insert<Tango::DevVarULongArray, unsigned long>::num_wave(_wh, this->any_)) {
        if (this->ex_flag_) {
          char wname[MAX_OBJ_NAME + 1];
          ::WaveName(_wh, wname);
          TangoSys_OMemStream reason;
          reason << "XOP internal error" << ends;
          TangoSys_OMemStream desc;
          desc << "could not extract data from wave " << wname << ends;
          Tango::Except::throw_exception(reason.str(), desc.str(), 
                                   (const char*)"DevData::operator<<");
		    }
        return;
      }   
    } break;
    case NT_FP32: 
    {
      if (Insert<Tango::DevVarFloatArray, float>::num_wave(_wh, this->any_)) {
        if (this->ex_flag_) {
          char wname[MAX_OBJ_NAME + 1];
          ::WaveName(_wh, wname);
          TangoSys_OMemStream reason;
          reason << "XOP internal error" << ends;
          TangoSys_OMemStream desc;
          desc << "could not extract data from wave " << wname << ends;
          Tango::Except::throw_exception(reason.str(), desc.str(), 
                                   (const char*)"DevData::operator<<");
		    }
        return;
      }  
    } break;
    case NT_FP64: 
    {
      if (Insert<Tango::DevVarDoubleArray, double>::num_wave(_wh, this->any_)) {
        if (this->ex_flag_) {
          char wname[MAX_OBJ_NAME + 1];
          ::WaveName(_wh, wname);
          TangoSys_OMemStream reason;
          reason << "XOP internal error" << ends;
          TangoSys_OMemStream desc;
          desc << "could not extract data from wave " << wname << ends;
          Tango::Except::throw_exception(reason.str(), desc.str(), 
                                   (const char*)"DevData::operator<<");
		    }
        return;
      }   
    } break;
    case TEXT_WAVE_TYPE: 
    {
      long ndims = 0;
      long dim_size[MAX_DIMENSIONS + 1];

      ::MemClear(dim_size, (MAX_DIMENSIONS + 1) * sizeof(long));

      if (::MDGetWaveDimensions(_wh, &ndims, dim_size)) {  
        if (this->ex_flag_) {
          char wname[MAX_OBJ_NAME + 1];
          ::WaveName(_wh, wname);
          TangoSys_OMemStream reason;
          reason << "XOP internal error" << ends;
          TangoSys_OMemStream desc;
          desc << "could not extract data from wave " << wname << ends;
          Tango::Except::throw_exception(reason.str(), desc.str(), 
                                   (const char*)"DevData::operator<<");
		    }
        return; 
      }

	    Tango::DevVarStringArray *array = new Tango::DevVarStringArray();
      if (array == 0) {
        if (this->ex_flag_) {
          char wname[MAX_OBJ_NAME + 1];
          ::WaveName(_wh, wname);
          TangoSys_OMemStream reason;
          reason << "out of memory error" << ends;
          TangoSys_OMemStream desc;
          desc << "could not extract data from wave " << wname << ends;
          Tango::Except::throw_exception(reason.str(), desc.str(), 
                                   (const char*)"DevData::operator<<");
		    }
        return;
      }
	    array->length(dim_size[0]);
      MDWaveDims dim_indx;
      ::MemClear(dim_indx, sizeof(MDWaveDims));
      Handle txt_hndl = ::NewHandle(0);
      if (txt_hndl == 0) {
        if (this->ex_flag_) {
          char wname[MAX_OBJ_NAME + 1];
          ::WaveName(_wh, wname);
          TangoSys_OMemStream reason;
          reason << "out of memory error" << ends;
          TangoSys_OMemStream desc;
          desc << "could not extract data from wave " << wname << ends;
          Tango::Except::throw_exception(reason.str(), desc.str(), 
                                   (const char*)"DevData::operator<<");
		    }
        return; 
      }
      char *tmp = 0;
      for (int i = 0; i < dim_size[0]; i++) {
        dim_indx[0] = i;
        if (::MDGetTextWavePointValue(_wh, dim_indx, txt_hndl)) {
          ::DisposeHandle(txt_hndl);
          if (this->ex_flag_) {
            char wname[MAX_OBJ_NAME + 1];
            ::WaveName(_wh, wname);
            TangoSys_OMemStream reason;
            reason << "XOP internal error" << ends;
            TangoSys_OMemStream desc;
            desc << "could not extract data from wave " << wname << ends;
            Tango::Except::throw_exception(reason.str(), desc.str(), 
                                     (const char*)"DevData::operator<<");
		      }
          return; 
        }
        tmp = CORBA::string_alloc(::GetHandleSize(txt_hndl) + 1);
        if (tmp == 0) {
          ::DisposeHandle(txt_hndl);
          if (this->ex_flag_) {
            char wname[MAX_OBJ_NAME + 1];
            ::WaveName(_wh, wname);
            TangoSys_OMemStream reason;
            reason << "out of memory error" << ends;
            TangoSys_OMemStream desc;
            desc << "could not extract data from wave " << wname << ends;
            Tango::Except::throw_exception(reason.str(), desc.str(), 
                                     (const char*)"DevData::operator<<");
		      }
          return; 
        }
        //::HLock(txt_hndl);
        ::MemClear(tmp, ::GetHandleSize(txt_hndl) + 1);
        ::memcpy(tmp, *txt_hndl, ::GetHandleSize(txt_hndl));
        (*array)[i] = tmp;
        //::HUnlock(txt_hndl);
      }
      DisposeHandle(txt_hndl);
	    this->any_.inout() <<= array;
    } break;
    //-- UNKNOWN TYPE -----------------------------------------------
    default:
      if (this->ex_flag_) {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "invalid argument specified" << ends;
        TangoSys_OMemStream desc;
        desc << "could not extract data from wave " << wname << " (unsupported Igor data type)" << ends;
        Tango::Except::throw_exception(reason.str(), desc.str(), 
                                 (const char*)"DevData::operator<<");
      }
      return;
      break;
  }
} 

// ============================================================================
// DevData::insert
// ============================================================================
void DevData::insert (const waveHndl _wn, const waveHndl _ws) 
{
  //- Check input
  int wn_type = ::WaveType(_wn);
  int ws_type = ::WaveType(_ws);
  if ((wn_type != NT_I32 && wn_type != NT_FP64) || ws_type != TEXT_WAVE_TYPE) {
    if (this->ex_flag_) {
      char wname[MAX_OBJ_NAME + 1];
      ::WaveName(_wn, wname);
      TangoSys_OMemStream reason;
      reason << "invalid argument specified" << ends;
      TangoSys_OMemStream desc;
      desc << "could not extract data from wave " << wname << " (incompatible wave type)" << ends;
      Tango::Except::throw_exception(reason.str(), desc.str(), 
                               (const char*)"DevData::insert");
    }
    return;
  }

  long wn_np = ::WavePoints(_wn);
  long ws_np = ::WavePoints(_ws);
  //- Insert wave<long | double>
  long offset;
  if (::MDAccessNumericWaveData(_wn, kMDWaveAccessMode0, &offset)) {
    if (this->ex_flag_) {
      char wname[MAX_OBJ_NAME + 1];
      ::WaveName(_wn, wname);
      TangoSys_OMemStream reason;
      reason << "XOP internal error" << ends;
      TangoSys_OMemStream desc;
      desc << "could not extract data from wave " << wname << ends;
      Tango::Except::throw_exception(reason.str(), desc.str(), 
                               (const char*)"DevData::insert");
		}
    return;
  }
  Tango::DevVarStringArray *s_value = 0;
  Tango::DevVarLongStringArray *ls_array = 0;
  Tango::DevVarDoubleStringArray *ds_array = 0;
  switch (wn_type)
  {
    case NT_I32:
    {
      ls_array = new Tango::DevVarLongStringArray();
      if (ls_array == 0) {
        if (this->ex_flag_) {
          char wname[MAX_OBJ_NAME + 1];
          ::WaveName(_wn, wname);
          TangoSys_OMemStream reason;
          reason << "out of memory error" << ends;
          TangoSys_OMemStream desc;
          desc << "could not extract data from wave " << wname << ends;
          Tango::Except::throw_exception(reason.str(), desc.str(), 
                                   (const char*)"DevData::insert");
        }
        return; 
      }
      ls_array->lvalue.length(wn_np);
      //::HLock((Handle)_wave);
      char *base_addr = static_cast<char*>(*_wn) + offset;
      ::memcpy(ls_array->lvalue.get_buffer(), base_addr, wn_np * sizeof(long));
      //::HUnlock((Handle)_wave);
      s_value = &ls_array->svalue;  
    } break;
    case NT_FP64:
    {
      ds_array = new Tango::DevVarDoubleStringArray();
      if (ds_array == 0) {
        if (this->ex_flag_) {
          char wname[MAX_OBJ_NAME + 1];
          ::WaveName(_wn, wname);
          TangoSys_OMemStream reason;
          reason << "out of memory error" << ends;
          TangoSys_OMemStream desc;
          desc << "could not extract data from wave " << wname << ends;
          Tango::Except::throw_exception(reason.str(), desc.str(), 
                                   (const char*)"DevData::insert");
        }
        return; 
      }
      ds_array->dvalue.length(wn_np);
      //::HLock((Handle)_wave);
      char *base_addr = static_cast<char*>(*_wn) + offset; 
      ::memcpy(ds_array->dvalue.get_buffer(), base_addr, wn_np * sizeof(double));
      //::HUnlock((Handle)_wave);
      s_value = &ds_array->svalue;  
    } break;
  }
  //- Insert wave<string>
	s_value->length(ws_np);
  MDWaveDims dim_indx;
  ::MemClear(dim_indx, sizeof(MDWaveDims));
  Handle txt_hndl = ::NewHandle(0);
  if (txt_hndl == 0) {
    if (this->ex_flag_) {
      char wname[MAX_OBJ_NAME + 1];
      ::WaveName(_ws, wname);
      TangoSys_OMemStream reason;
      reason << "out of memory error" << ends;
      TangoSys_OMemStream desc;
      desc << "NewHandle failed";
      Tango::Except::throw_exception(reason.str(), desc.str(), 
                               (const char*)"DevData::insert");
		}
    return; 
  }
  char *tmp = 0;
  for (int i = 0; i < ws_np; i++) {
    dim_indx[0] = i;
    if (::MDGetTextWavePointValue(_ws, dim_indx, txt_hndl)) {
      ::DisposeHandle(txt_hndl);
      if (this->ex_flag_) {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_ws, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "MDGetTextWavePointValue failed" << ends;
        Tango::Except::throw_exception(reason.str(), desc.str(), 
                                 (const char*)"DevData::insert");
		  }
      return; 
    }
    tmp = CORBA::string_alloc(::GetHandleSize(txt_hndl) + 1);
    if (tmp == 0) {
      ::DisposeHandle(txt_hndl);
      if (this->ex_flag_) {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_ws, wname);
        TangoSys_OMemStream reason;
        reason << "out of memory error" << ends;
        TangoSys_OMemStream desc;
        desc << "CORBA::string_alloc failed" << ends;
        Tango::Except::throw_exception(reason.str(), desc.str(), 
                                 (const char*)"DevData::insert");
		  }
      return; 
    }
    //::HLock(txt_hndl);
    ::MemClear(tmp, ::GetHandleSize(txt_hndl) + 1);
    ::memcpy(tmp, *txt_hndl, ::GetHandleSize(txt_hndl));
    (*s_value)[i] = tmp;
    //::HUnlock(txt_hndl);
  }
  DisposeHandle(txt_hndl);

  switch (wn_type) {
    case NT_I32:
	    this->any_.inout() <<= ls_array;
      break;
    case NT_FP64:
	    this->any_.inout() <<= ds_array;
      break;
  }
}

// ============================================================================
// DevData::insert_spectrum 
// ============================================================================
void DevData::insert_spectrum (waveHndl _wh, long _dim_x)
{
  this->operator<<(_wh);
}

// ============================================================================
// DevData::insert_image
// ============================================================================
void DevData::insert_image (waveHndl _wh, long _dim_x, long _dim_y)
{
  this->operator<<(_wh);
}

//=============================================================================
// TEMPLATE CLASS Extract - extracts 1 dim waves only
//=============================================================================
template <class A, class T = unsigned char> class Extract 
{
public: 
  //-- extract numeric wave from CORBA::Any
  static int num_wave (waveHndl _wh, CORBA::Any_var& any_)
  {
    A *array;
    if((any_ >>= array) == false) {
      return kError;
    }
    return Extract<A,T>::num_wave(array, _wh);
  }

  //-- extract numeric wave from CORBA::sequence
  static int num_wave (A* _seq, waveHndl _wh)
  {
    if (_seq == 0) {
      return kError;
    }
    long w_ndims = 0;
    MDWaveDims w_dims;
    if (::MDGetWaveDimensions(_wh, &w_ndims, w_dims)) {
      return kError;
    }
    if (w_dims[0] != _seq->length() || w_ndims != 1) {
      ::MemClear(w_dims, sizeof(MDWaveDims));
      w_dims[0] = _seq->length();
      if (::MDChangeWave(_wh, ::WaveType(_wh), w_dims)) {
        return kError;
      }
    }
    long offset;
    if (::MDAccessNumericWaveData(_wh, kMDWaveAccessMode0, &offset)) {
      return kError;
    }
    /*::HLock((Handle)_wh);*/
    char *base_addr = (char*)(*_wh) + offset;
    ::memcpy(base_addr, _seq->get_buffer(), _seq->length() * sizeof(T));
    /*::HUnlock((Handle)_wh);*/
    return kNoError;
  }

  //-- extract text wave from CORBA::Any
  static int txt_wave (waveHndl _wh, CORBA::Any_var& any_)
  {
    A *array;
    if((any_ >>= array) == false) {
      return kError;
    }
    return Extract<A,T>::txt_wave(array, _wh);
  }

  //-- extract text wave from CORBA::sequence
  static int txt_wave (A* _seq, waveHndl _wh)
  {
    if (_seq == 0) {
      return kError;
    }
    long w_ndims = 0;
    MDWaveDims w_dims;
    if (::MDGetWaveDimensions(_wh, &w_ndims, w_dims)) {
      return kError;
    }
    if (w_dims[0] != _seq->length() || w_ndims != 1 || ::WaveType(_wh) != TEXT_WAVE_TYPE) {
      ::MemClear(w_dims, sizeof(MDWaveDims));
      w_dims[0] = _seq->length();
      if (::MDChangeWave(_wh, TEXT_WAVE_TYPE, w_dims)) {
        return kError;
      }
    }
    Handle txt_hndl = ::NewHandle(0L);
    if (txt_hndl == 0) {
      return kError;
    }
    for (int i = 0; i < _seq->length(); i++) {
      w_dims[0] = i;
      ::SetHandleSize(txt_hndl, ::strlen((*_seq)[i]));
      if (::MemError()) {
        ::DisposeHandle(txt_hndl);
        return kError;
      }
      /*::HLock((Handle)_wh);*/
      ::memcpy(*txt_hndl, (*_seq)[i], ::strlen((*_seq)[i]) * sizeof(T));
      ::MDSetTextWavePointValue(_wh, w_dims, txt_hndl);
      /*::HUnLock((Handle)_wh);*/
    }
    ::DisposeHandle(txt_hndl);
    return kNoError;
  }
};

// ============================================================================
// DevData::operator>> - extracts an Igor Pro wave from a CORBA::sequence
// ============================================================================
void DevData::operator >> (waveHndl _wh)
{
  if (! this->is_valid()) {
    return;
  }

  switch (::WaveType(_wh)) 
  {
    case NT_I8:  
    {
      if (sizeof(bool) != sizeof(unsigned char)) 
      {
        Tango::Except::throw_exception((const char*)"internal error",
                                       (const char*)"can't extract data from boolean array", 
                                       (const char*)"DevData::extract_image");
      }
      if (Extract<Tango::DevVarBooleanArray, unsigned char>::num_wave(_wh, this->any_)) {
        if (this->ex_flag_) {
           char wname[MAX_OBJ_NAME + 1];
           ::WaveName(_wh, wname);
           TangoSys_OMemStream reason;
           reason << "XOP internal error" << ends; 
           TangoSys_OMemStream desc;
           desc << "could not insert data into wave " << wname << ends;
           Tango::Except::throw_exception(reason.str(), desc.str(), 
                                     (const char*)"DevData::operator>>");
		    }
        return;
      }   
    } break;
    case NT_I8|NT_UNSIGNED: 
    {
      if (Extract<Tango::DevVarCharArray, unsigned char>::num_wave(_wh, this->any_)) {
        if (this->ex_flag_) {
           char wname[MAX_OBJ_NAME + 1];
           ::WaveName(_wh, wname);
           TangoSys_OMemStream reason;
           reason << "XOP internal error" << ends; 
           TangoSys_OMemStream desc;
           desc << "could not insert data into wave " << wname << ends;
           Tango::Except::throw_exception(reason.str(), desc.str(), 
                                     (const char*)"DevData::operator>>");
		    }
        return;
      }   
    } break;
    case NT_I16:
    {
      if (Extract<Tango::DevVarShortArray, short>::num_wave(_wh, this->any_)) {
        if (this->ex_flag_) {
           char wname[MAX_OBJ_NAME + 1];
           ::WaveName(_wh, wname);
           TangoSys_OMemStream reason;
           reason << "XOP internal error" << ends;
           TangoSys_OMemStream desc;
           desc << "could not insert data into wave " << wname << ends;
           Tango::Except::throw_exception(reason.str(), desc.str(), 
                                     (const char*)"DevData::operator>>");
		    }
        return;
      }   
    } break;
    case NT_I16|NT_UNSIGNED:
    {
      if (Extract<Tango::DevVarUShortArray, unsigned short>::num_wave(_wh, this->any_)) {
        if (this->ex_flag_) {
           char wname[MAX_OBJ_NAME + 1];
           ::WaveName(_wh, wname);
           TangoSys_OMemStream reason;
           reason << "XOP internal error" << ends;
           TangoSys_OMemStream desc;
           desc << "could not insert data into wave " << wname << ends;
            Tango::Except::throw_exception(reason.str(), desc.str(), 
                                     (const char*)"DevData::operator>>");
		    }
        return;
      }   
    } break;
    case NT_I32:
    {
      if (Extract<Tango::DevVarLongArray, long>::num_wave(_wh, this->any_)) {
        if (this->ex_flag_) {
           char wname[MAX_OBJ_NAME + 1];
           ::WaveName(_wh, wname);
           TangoSys_OMemStream reason;
           reason << "XOP internal error" << ends;
           TangoSys_OMemStream desc;
           desc << "could not insert data into wave " << wname << ends;
           Tango::Except::throw_exception(reason.str(), desc.str(), 
                                     (const char*)"DevData::operator>>");
		    }
        return;
      }   
    } break;
    case NT_I32|NT_UNSIGNED:
    {
      if (Extract<Tango::DevVarULongArray, unsigned long>::num_wave(_wh, this->any_)) {
        if (this->ex_flag_) {
           char wname[MAX_OBJ_NAME + 1];
           ::WaveName(_wh, wname);
           TangoSys_OMemStream reason;
           reason << "XOP internal error" << ends;
           TangoSys_OMemStream desc;
           desc << "could not insert data into wave " << wname << ends;
            Tango::Except::throw_exception(reason.str(), desc.str(), 
                                     (const char*)"DevData::operator>>");
		    }
        return;
      }   
    } break;
    case NT_FP32:
    {
      if (Extract<Tango::DevVarFloatArray, float>::num_wave(_wh, this->any_)) {
        if (this->ex_flag_) {
           char wname[MAX_OBJ_NAME + 1];
           ::WaveName(_wh, wname);
           TangoSys_OMemStream reason;
           reason << "XOP internal error" << ends;
           TangoSys_OMemStream desc;
           desc << "could not insert data into wave " << wname << ends;
           Tango::Except::throw_exception(reason.str(), desc.str(), 
                                     (const char*)"DevData::operator>>");
		    }
        return;
      }   
    } break;
    case NT_FP64:
    {
      if (Extract<Tango::DevVarDoubleArray, double>::num_wave(_wh, this->any_)) {
        if (this->ex_flag_) {
           char wname[MAX_OBJ_NAME + 1];
           ::WaveName(_wh, wname);
           TangoSys_OMemStream reason;
           reason << "XOP internal error" << ends;
           TangoSys_OMemStream desc;
           desc << "could not insert data into wave " << wname << ends;
           Tango::Except::throw_exception(reason.str(), desc.str(), 
                                     (const char*)"DevData::operator>>");
		    }
        return;
      }   
    } break;
    case TEXT_WAVE_TYPE:
    {
      if (Extract<Tango::DevVarStringArray>::txt_wave(_wh, this->any_)) {
        if (this->ex_flag_) {
           char wname[MAX_OBJ_NAME + 1];
           ::WaveName(_wh, wname);
           TangoSys_OMemStream reason;
           reason << "XOP internal error" << ends;
           TangoSys_OMemStream desc;
           desc << "could not insert data into wave " << wname << ends;
           Tango::Except::throw_exception(reason.str(), desc.str(), 
                                     (const char*)"DevData::operator>>");
		    }
        return;
      }   
    } break;
    //-- UNKNOWN TYPE -----------------------------------------------
    default:
      if (this->ex_flag_) {
           char wname[MAX_OBJ_NAME + 1];
           ::WaveName(_wh, wname);
           TangoSys_OMemStream reason;
           reason << "XOP internal error" << ends;
           TangoSys_OMemStream desc;
           desc << "unsupported TANGO data type" << ends;
           Tango::Except::throw_exception(reason.str(), desc.str(), 
                                     (const char*)"DevData::operator>>");
      }
      return;
      break;
  }
} 

// ============================================================================
// DevData::extract
// ============================================================================
void DevData::extract (waveHndl _wn, waveHndl _ws) 
{
  Tango::DevVarStringArray *s_value = 0;

  switch (::WaveType(_wn))
  {
    case NT_I32:
    {
      Tango::DevVarLongStringArray *ls_array;
      this->any_ >>= ls_array;
      Tango::DevVarLongArray *l_value = &ls_array->lvalue;
      if (Extract<Tango::DevVarLongArray, long>::num_wave(l_value, _wn)) {
        if (this->ex_flag_) {
           char wname[MAX_OBJ_NAME + 1];
           ::WaveName(_wn, wname);
           TangoSys_OMemStream reason;
           reason << "XOP internal error" << ends;
           TangoSys_OMemStream desc;
           desc << "could not insert data into wave " << wname << ends;
           Tango::Except::throw_exception(reason.str(), desc.str(), 
                                     (const char*)"DevData::extract");
		    }
        return;
      }
      s_value = &ls_array->svalue;
    } break;
    case NT_FP64:
    {
      Tango::DevVarDoubleStringArray *ds_array;
      this->any_ >>= ds_array;
      Tango::DevVarDoubleArray *d_value = &ds_array->dvalue;
      if (Extract<Tango::DevVarDoubleArray, double>::num_wave(d_value, _wn)) {
        if (this->ex_flag_) {
           char wname[MAX_OBJ_NAME + 1];
           ::WaveName(_wn, wname);
           TangoSys_OMemStream reason;
           reason << "XOP internal error" << ends;
           TangoSys_OMemStream desc;
           desc << "could not insert data into wave " << wname << ends;
           Tango::Except::throw_exception(reason.str(), desc.str(), 
                                     (const char*)"DevData::extract");
		    }
        return;
      }
      s_value = &ds_array->svalue;
    } break;
  }
  if (Extract<Tango::DevVarStringArray>::txt_wave(s_value, _ws)) {
    if (this->ex_flag_) {
      char wname[MAX_OBJ_NAME + 1];
      ::WaveName(_ws, wname);
      TangoSys_OMemStream reason;
      reason << "XOP internal error" << ends;
      TangoSys_OMemStream desc;
      desc << "could not insert data into wave " << wname << ends;
      Tango::Except::throw_exception(reason.str(), desc.str(), 
                               (const char*)"DevData::extract");
	  }
    return;
  }
}

// ============================================================================
// DevData::extract_spectrum 
// ============================================================================
void DevData::extract_spectrum (waveHndl _wh, long _dim_x)
{
  this->operator>>(_wh);
}

// ============================================================================
// DevData::extract_image 
// ============================================================================
void DevData::extract_image (waveHndl _wh, long _dim_x, long _dim_y)
{
  char *array = 0;
  long elem_size = 0;

  switch (::WaveType(_wh)) 
  {
    case NT_I8:
    {
      if (sizeof(bool) != sizeof(unsigned char)) 
      {
        Tango::Except::throw_exception((const char*)"internal error",
                                       (const char*)"can't extract data from boolean array", 
                                       (const char*)"DevData::extract_image");
      }
      Tango::DevVarBooleanArray *b_array = 0;
      this->any_ >>= b_array;
      array = (char*)b_array->get_buffer();
      elem_size = sizeof(bool);
    } break;
    case NT_I8|NT_UNSIGNED:
    {
      Tango::DevVarCharArray *c_array = 0;
      this->any_ >>= c_array;
      array = (char*)c_array->get_buffer();
      elem_size = sizeof(unsigned char);
    } break;
    case NT_I16|NT_UNSIGNED:
    {
      Tango::DevVarUShortArray *us_array;
      this->any_ >>= us_array;
      array = (char*)us_array->get_buffer();
      elem_size = sizeof(unsigned short);
    } break;
    case NT_I16:
    {
      Tango::DevVarShortArray *s_array;
      this->any_ >>= s_array;
      array = (char*)s_array->get_buffer();
      elem_size = sizeof(short);
    } break;
    case NT_I32:
    {
      Tango::DevVarLongArray *l_array;
      this->any_ >>= l_array;
      array = (char*)l_array->get_buffer();
      elem_size = sizeof(long);
    } break;
    case NT_FP32:
    {
      Tango::DevVarFloatArray *f_array;
      this->any_ >>= f_array;
      array = (char*)f_array->get_buffer();
      elem_size = sizeof(float);
    } break;
    case NT_FP64:
    {
      Tango::DevVarDoubleArray *d_array;
      this->any_ >>= d_array;
      array = (char*)d_array->get_buffer();
      elem_size = sizeof(double);
    } break;
    default:
      if (this->ex_flag_) {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "could not insert data into wave " << wname << " (unsupported TANGO data type)" << ends;
        Tango::Except::throw_exception(reason.str(), desc.str(), 
                                 (const char*)"DevData::extract_image");
      }
      return;
      break;
  }
  long w_ndims = 0;
  MDWaveDims w_dims;
  ::MemClear(w_dims, sizeof(MDWaveDims));
  if (::MDGetWaveDimensions(_wh, &w_ndims, w_dims)) {
    if (this->ex_flag_) {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "MDGetWaveDimensions failed" << ends;
        Tango::Except::throw_exception(reason.str(), desc.str(), 
                                 (const char*)"DevData::extract_image");
		}
    return;
  }
  if (w_dims[0] != _dim_x || w_dims[1] != _dim_y || w_ndims != 2) {
    ::MemClear(w_dims, sizeof(MDWaveDims));
    w_dims[0] = _dim_x;
    w_dims[1] = _dim_y;
    if (::MDChangeWave(_wh, ::WaveType(_wh), w_dims)) {
      if (this->ex_flag_) {
        char wname[MAX_OBJ_NAME + 1];
        ::WaveName(_wh, wname);
        TangoSys_OMemStream reason;
        reason << "XOP internal error" << ends;
        TangoSys_OMemStream desc;
        desc << "MDChangeWave failed" << ends;
        Tango::Except::throw_exception(reason.str(), desc.str(), 
                                 (const char*)"DevData::extract_image");
      }
      return;
    }
  }
  long offset;
  if (::MDAccessNumericWaveData(_wh, kMDWaveAccessMode0, &offset)) {
    if (this->ex_flag_) {
      char wname[MAX_OBJ_NAME + 1];
      ::WaveName(_wh, wname);
      TangoSys_OMemStream reason;
      reason << "XOP internal error" << ends;
      TangoSys_OMemStream desc;
      desc << "MDAccessNumericWaveData failed" << ends;
      Tango::Except::throw_exception(reason.str(), desc.str(), 
                               (const char*)"DevData::extract_image");
		}
    return;
  }
  /*::HLock((Handle)_wh);*/ 
  char *base_addr = (char*)(*_wh) + offset;
  ::memcpy(base_addr, array, _dim_x * _dim_y * elem_size);
  /*::HUnlock((Handle)_wh);*/ \
}

#endif // _XDK_XOP_

} // namespace TangoApi