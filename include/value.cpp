#include "value.hpp"
#include <cstring>

using namespace timl;

/////////////////  FREE FUNCTIONS
template<typename T>
T unique_ptr_copy(const T& src);

template<>
Value::ArrayType unique_ptr_copy(const Value::ArrayType& src)
{
    Value::ArrayType rtn;
    for(auto& v : src)
        rtn.push_back( std::make_unique<Value>(*v) );
    rtn.shrink_to_fit();
    return rtn;
}

template<>
Value::MapType unique_ptr_copy(const Value::MapType& src)
{
    Value::MapType rtn;
    for(auto& v : src)
        rtn.insert( std::make_pair(v.first, std::make_unique<Value>(*(v.second)) ));
    return rtn;
}


//////////////// VALUE IMpl


Value::Value()
    : vtype(Type::Null)
{ }

Value::Value(char c)
    : vtype(Type::Char)
{  value.Char = c; }

Value::Value(bool b)
    : vtype(Type::Bool)
{  value.Bool = b; }

Value::Value(long long ll)
    : vtype(Type::SignedInt)
{  value.SignedInt = ll; }

Value::Value(unsigned long long ull)
    : vtype(Type::UnsignedInt)
{  value.UnsignedInt = ull; }

Value::Value(double d)
    : vtype(Type::Float)
{  value.Float = d; }

Value::Value(BinaryType b)
    : vtype(Type::Binary)
{   construct_fromBinary(std::move(b)); }

Value::Value(std::string s)
    : vtype(Type::String)
{   construct_fromString(std::move(s)); }


Value::Value(Value&& v)
    : Value()
{   move_from(std::move(v)); }

Value::Value(const Value& v)
    : Value()
{   copy_from(v); }


Value& Value::operator = (const Value& v)
{
    copy_from(v);
    return *this;
}

//Work here!
Value& Value::operator = (Value&& v)
{
    move_from(std::move(v));
    return *this;
}


Value::~Value()
{
    destruct();
}

size_t Value::size() const
{
    switch (vtype) {
    case Type::Null:
        return 0;
    case Type::String:
        return value.String.size();
    case Type::Binary:
        return value.Binary.size();
    case Type::Array:
        return value.Array.size();
    case Type::Map:
        return value.Map.size();
    default:
        return 1;
    }
}

Value& Value::operator [] (size_t i)
{
    if(vtype == Type::Array)
        return *(value.Array[i]);
    throw value_exception("Attempt to index 'Value'; 'Value' is not an Array!");
}

Value const& Value::operator [] (size_t i) const
{
    if(vtype == Type::Array)
        return *(value.Array[i]);
    throw value_exception("Attempt to index 'Value const&'; 'Value const&' is not an Array!");
}

Value& Value::operator [] (const std::string& s)
{
    if(vtype == Type::Map)
        return *(value.Map[s]);
    if(vtype == Type::Null)
    {
        // convert to Map
        construct_fromMap(MapType());
        vtype = Type::Map;
        return *(value.Map[s]);
    }
    throw value_exception("Attempt to index 'Value'; 'Value' is not a Key-Value pair (aka Object) !");
}

Value const& Value::operator [] (const std::string& s) const
{
    if(vtype == Type::Map)
        return *(const_cast<const MapType&>(value.Map).at(s));
    throw value_exception("Attempt to index 'Value const&'; 'Value const&' is not a Key-Value pair (aka Object) !");
}


//////////////// PRIVATE ////////////////
/////////////////////////////////////////
///
///
///
/////////////////////////////////////////


void Value::construct_fromString(std::string&& s)
{
    destruct();
    new( &(value.String)) std::string(std::move(s));
}

void Value::construct_fromBinary(BinaryType&& b)
{
    destruct();
    new( &(value.Binary)) BinaryType(std::move(b));
}

void Value::construct_fromArray(ArrayType&& a)
{
    destruct();
    new( &(value.Array)) ArrayType(std::move(a));
}

void Value::construct_fromMap(MapType&& m)
{
    destruct();
    new( &(value.Map)) MapType(std::move(m));
}

void Value::move_from(Value&& v)
{
    destruct();

    switch (v.vtype) {
    case Type::Char:
        value.Char = v.value.Char;
        break;
    case Type::Bool:
        value.Bool = v.value.Bool;
        break;
    case Type::SignedInt:
        value.SignedInt = v.value.SignedInt;
        break;
    case Type::UnsignedInt:
        value.UnsignedInt = v.value.UnsignedInt;
        break;
    case Type::Float:
        value.Float = v.value.Float;
        break;
    case Type::String:
        construct_fromString( std::move(v.value.String) );
        break;
    case Type::Binary:
        construct_fromBinary( std::move(v.value.Binary) );
        break;
    case Type::Array:
        construct_fromArray(  std::move(v.value.Array)  );
        break;
    case Type::Map:
        construct_fromMap(    std::move(v.value.Map)    );
        break;
    default:
        break;
    }

    v.destruct();
}

void Value::copy_from(const Value& v)
{
    destruct();

    switch (v.vtype) {
    case Type::Char:
        value.Char = v.value.Char;
        break;
    case Type::Bool:
        value.Bool = v.value.Bool;
        break;
    case Type::SignedInt:
        value.SignedInt = v.value.SignedInt;
        break;
    case Type::UnsignedInt:
        value.UnsignedInt = v.value.UnsignedInt;
        break;
    case Type::Float:
        value.Float = v.value.Float;
        break;
    case Type::String:
        construct_fromString( std::string( v.value.String ));
        break;
    case Type::Binary:
        construct_fromBinary( BinaryType(  v.value.Binary ));
        break;
    case Type::Array:
        construct_fromArray( ArrayType(unique_ptr_copy(v.value.Array)));
        break;
    case Type::Map:
        construct_fromMap( MapType(unique_ptr_copy(v.value.Map)));
        break;
    default:
        break;
    }
}

inline void Value::destruct() noexcept
{
    using std::string;
    using std::vector;


    switch (vtype) {
    case Type::Null:
        return;
    case Type::String:
        value.String.~string();
        break;
    case Type::Array:
        value.Array.~vector();
        break;
    case Type::Binary:
        value.Binary.~vector();
        break;
    case Type::Map:
        value.Map.~unordered_map();
        break;
    default:
        break;
    }

    vtype = Type::Null;
}

//////////////////////////  CONVERSION OPERATOR  ///////////////////////////
///// I currently don't know a better way to avoid code duplication here
///// C++ forbids a const function to call a none const function


///// long long
Value::operator long long& () &
{
    if(vtype == Type::SignedInt)
        return value.SignedInt;
    throw bad_value_cast("'Value&' cannot be casted to 'long long'");
}

Value::operator long long const& () const&
{
    if(vtype == Type::SignedInt)
        return value.SignedInt;
    throw bad_value_cast("'Value const&' cannot casted to 'long long'");
}


///// unsigned long long
Value::operator unsigned long long& () &
{
    if(vtype == Type::UnsignedInt)
        return value.UnsignedInt;
    throw bad_value_cast("'Value&' cannot be casted to 'unsigned long long'");
}

Value::operator unsigned long long const& () const&
{
    if(vtype == Type::UnsignedInt)
        return value.UnsignedInt;
    throw bad_value_cast("'Value const&' cannot be casted to 'unsigned long long'");
}


///// bool
Value::operator bool & () &
{
    if(vtype == Type::Bool)
        return value.Bool;
    throw bad_value_cast("'Value&' cannot be casted to 'bool'");
}

Value::operator bool const& () const&
{
    if(vtype == Type::Bool)
        return value.Bool;
    throw bad_value_cast("'Value const&' cannot be casted to 'bool'");
}


///// char
Value::operator char & () &
{
    if(vtype == Type::Char)
        return value.Char;
    throw bad_value_cast("'Value&' cannot be casted to 'char'");
}

Value::operator char const& () const&
{
    if(vtype == Type::Char)
        return value.Char;
    throw bad_value_cast("'Value const&' cannot be casted to 'char'");
}


///// double
Value::operator double & () &
{
    if(vtype == Type::Float)
        return value.Float;
    throw bad_value_cast("'Value&' cannot be casted to 'double'");
}

Value::operator double const& () const&
{
    if(vtype == Type::Float)
        return value.Float;
    throw bad_value_cast("'Value&' cannot be casted to 'double'");
}


///// std::string
Value::operator std::string () &&
{
    if(vtype == Type::String)
        return std::move(value.String);
    throw bad_value_cast("'Value&&' cannot be casted to 'std::string&&'");
}

Value::operator std::string& () &
{
    if(vtype == Type::String)
        return value.String;
    throw bad_value_cast("'Value&' cannot be casted to 'std::string&'");
}

Value::operator std::string const& () const&
{
    if(vtype == Type::String)
        return value.String;
    throw bad_value_cast("'Value const&' cannot be casted to 'std::string const&'");
}


///// BinaryType
Value::operator BinaryType () &&
{
    if(vtype == Type::Binary)
        return std::move(value.Binary);
    throw bad_value_cast("'Value&&' cannot be casted to 'BinaryType&&'");
}

Value::operator BinaryType& () &
{
    if(vtype == Type::Binary)
        return value.Binary;
    throw bad_value_cast("'Value&' cannot be casted to 'BinaryType&'");
}

Value::operator BinaryType const& () const&
{
    if(vtype == Type::Binary)
        return value.Binary;
    throw bad_value_cast("'Value const&' cannot be casted to 'BinaryType const&'");
}


//////////////////////// FRIEND FUNCTION ///////////

void timl::swap(Value& v1, Value& v2)
{
    Value vt;
    vt.move_from( std::move( v2 ));
    v2.move_from( std::move( v1 ));
    v1.move_from( std::move( vt ));
}

