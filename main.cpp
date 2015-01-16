/*
 * Copyright(C):    WhiZTiM, 2015
 *
 * This file is part of the TIML::UBEX C++14 library
 *
 * Distributed under the Boost Software License, Version 1.0.
 *      (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 *
 * Author: Ibrahim Timothy Onogu
 * Email:  ionogu@acm.org
 */

#include <iostream>
#include <iomanip>
#include <chrono>
#include <type_traits>
#include "include/value.hpp"
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

using namespace std;
using namespace timl;

//for CPPUNIT_ASSERT_EQUAL ...using ADL
ostream& operator << (ostream& out, const Value::BinaryType& b)
{
    out << hex;
    for(auto a : b)
        out << int(a) << ' ';
    return out;
}

class Value_Construction_Test : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( Value_Construction_Test );
    CPPUNIT_TEST( testConstructors );
    CPPUNIT_TEST( testSizes );
    CPPUNIT_TEST( testEqaulity );
    CPPUNIT_TEST( testIneqaulity );
    CPPUNIT_TEST( testCopyConstruction );
    CPPUNIT_TEST( testMoveConstruction );
    CPPUNIT_TEST( testCopyAssignment );
    CPPUNIT_TEST( testMoveAssignment );
    CPPUNIT_TEST_SUITE_END();
public:
    using T = Value::BinaryType::value_type;

    void setUp() override
    {
        v_empty = std::make_unique<Value>(Value());
        v_char = std::make_unique<Value>(Value('c'));
        v_bool = std::make_unique<Value>(Value(true));
        v_signedint = std::make_unique<Value>(Value(-700));
        v_unsignedint = std::make_unique<Value>(Value(800ull));
        v_float = std::make_unique<Value>(Value(3.1416));
        v_string = std::make_unique<Value>(Value("string"));
        v_array = std::make_unique<Value>(Value({34.657, "Yeepa", 466, -53, 'g'}));
        v_map = std::make_unique<Value>(Value());

        v_binary = std::make_unique<Value>(Value(Value::BinaryType({T(0xab), T(0xbc), T(0xcd), T(0xdf)})));

        (*v_map)["name"] = "WhiZTiM";
        (*v_map)["id"] = 12343;
        (*v_map)["extras"] = { *v_array, "nice one bro!" };
    }
private:
    Value::Uptr v_empty;
    Value::Uptr v_char;
    Value::Uptr v_bool;
    Value::Uptr v_signedint;
    Value::Uptr v_unsignedint;
    Value::Uptr v_float;
    Value::Uptr v_string;
    Value::Uptr v_array;
    Value::Uptr v_map;
    Value::Uptr v_binary;
public:
    void testConstructors()
    {
        CPPUNIT_ASSERT( v_empty->isNull() );
        CPPUNIT_ASSERT( v_char->isChar() );
        CPPUNIT_ASSERT( v_bool->isBool() );
        CPPUNIT_ASSERT( v_signedint->isSignedInteger() );
        CPPUNIT_ASSERT( v_unsignedint->isUnsignedInteger() );
        CPPUNIT_ASSERT( v_float->isFloat() );
        CPPUNIT_ASSERT( v_string->isString() );
        CPPUNIT_ASSERT( v_array->isArray() );
        CPPUNIT_ASSERT( v_map->isObject() );
        CPPUNIT_ASSERT( v_binary->isBinary() );

        CPPUNIT_ASSERT( v_signedint->isInteger() );
        CPPUNIT_ASSERT( v_unsignedint->isInteger() );

        CPPUNIT_ASSERT( v_signedint->isNumeric() );
        CPPUNIT_ASSERT( v_unsignedint->isNumeric() );
        CPPUNIT_ASSERT( v_float->isNumeric() );

        CPPUNIT_ASSERT( v_map->operator[]("extras").isArray() );
    }

    void testSizes()
    {
        CPPUNIT_ASSERT_EQUAL( std::size_t(0), v_empty->size() );
        CPPUNIT_ASSERT_EQUAL( std::size_t(1), v_char->size() );
        CPPUNIT_ASSERT_EQUAL( std::size_t(1), v_bool->size() );
        CPPUNIT_ASSERT_EQUAL( std::size_t(1), v_signedint->size() );
        CPPUNIT_ASSERT_EQUAL( std::size_t(1), v_unsignedint->size() );
        CPPUNIT_ASSERT_EQUAL( std::size_t(1), v_float->size() );
        CPPUNIT_ASSERT_EQUAL( std::size_t(1), v_string->size() );
        CPPUNIT_ASSERT_EQUAL( std::size_t(5), v_array->size() );
        CPPUNIT_ASSERT_EQUAL( std::size_t(3), v_map->size() );
        CPPUNIT_ASSERT_EQUAL( std::size_t(1), v_binary->size() );
    }

    void testEqaulity()
    {
        CPPUNIT_ASSERT( *v_empty == Value()    );
        CPPUNIT_ASSERT( *v_char == Value('c')  );
        CPPUNIT_ASSERT( *v_bool == Value(true) );
        CPPUNIT_ASSERT( *v_signedint == Value(-700) );
        CPPUNIT_ASSERT( *v_unsignedint == Value(800) );
        CPPUNIT_ASSERT( *v_float == Value(3.141600000000000) );
        CPPUNIT_ASSERT( *v_string == "string" );
        CPPUNIT_ASSERT( *v_array == Value({34.657, "Yeepa", 466, -53, 'g'}) );
        CPPUNIT_ASSERT( *v_binary == Value(Value::BinaryType({T(0xab), T(0xbc), T(0xcd), T(0xdf)})));

        Value map;
        map["name"] = "WhiZTiM";
        map["id"] = 12343;
        map["extras"] = { *v_array, "nice one bro!" };
        CPPUNIT_ASSERT( *v_map == map );
    }

    void testIneqaulity()
    {
        CPPUNIT_ASSERT( *v_empty != Value('c')    );
        CPPUNIT_ASSERT( *v_char != Value()  );
        CPPUNIT_ASSERT( *v_bool != Value(false) );
        CPPUNIT_ASSERT( *v_signedint != Value(700) );
        CPPUNIT_ASSERT( *v_unsignedint != Value(-800) );
        CPPUNIT_ASSERT( *v_float != Value(3.141600000000001) );
        CPPUNIT_ASSERT( *v_string != "strings" );
        CPPUNIT_ASSERT( *v_array != Value({34.6574, "Yeepa", 466, -53, 'g'}) );
        CPPUNIT_ASSERT( *v_map != Value({34.657, "Yeepa", 466, -53, 'g'}) );
        CPPUNIT_ASSERT( *v_binary != Value(Value::BinaryType({T(0xab), T(0xbc), T(0xcd), T(0xff)})));
    }

    void testCopyConstruction()
    {
        CPPUNIT_ASSERT( *v_empty == Value(*v_empty)    );
        CPPUNIT_ASSERT( *v_char == Value(*v_char)  );
        CPPUNIT_ASSERT( *v_bool == Value(*v_bool) );
        CPPUNIT_ASSERT( *v_signedint == Value(*v_signedint) );
        CPPUNIT_ASSERT( *v_unsignedint == Value(*v_unsignedint) );
        CPPUNIT_ASSERT( *v_float == Value(*v_float) );
        CPPUNIT_ASSERT( *v_string == Value(*v_string) );
        CPPUNIT_ASSERT( *v_array == Value(*v_array));
        CPPUNIT_ASSERT( *v_map == Value(*v_map));
        CPPUNIT_ASSERT( *v_binary == Value(*v_binary));
    }

    void testMoveConstruction()
    {

        Value Null_1(*v_empty);
        Value Char_1(*v_char);
        Value Bool_1(*v_bool);
        Value SignedInt_1(*v_signedint);
        Value UnsignedInt_1(*v_unsignedint);
        Value Float_1(*v_float);
        Value String_1(*v_string);
        Value Array_1(*v_array);
        Value Map_1(*v_map);
        Value Binary_1(*v_binary);

        //redo copy construction equality test
        CPPUNIT_ASSERT( *v_empty == Null_1    );
        CPPUNIT_ASSERT( *v_char == Char_1  );
        CPPUNIT_ASSERT( *v_bool == Bool_1 );
        CPPUNIT_ASSERT( *v_signedint == SignedInt_1 );
        CPPUNIT_ASSERT( *v_unsignedint == UnsignedInt_1 );
        CPPUNIT_ASSERT( *v_float == Float_1 );
        CPPUNIT_ASSERT( *v_string == String_1 );
        CPPUNIT_ASSERT( *v_array == Array_1);
        CPPUNIT_ASSERT( *v_map == Map_1 );
        CPPUNIT_ASSERT( *v_binary == Binary_1 );

        //Move items
        Value Null_2(std::move(Null_1));
        Value Char_2(std::move(Char_1));
        Value Bool_2(std::move(Bool_1));
        Value SignedInt_2(std::move(SignedInt_1));
        Value UnsignedInt_2(std::move(UnsignedInt_1));
        Value Float_2(std::move(Float_1));
        Value String_2(std::move(String_1));
        Value Array_2(std::move(Array_1));
        Value Map_2(std::move(Map_1));
        Value Binary_2(std::move(Binary_1));

        //Assert moved items are Null
        CPPUNIT_ASSERT( Null_1.isNull()         );
        CPPUNIT_ASSERT( Char_1.isNull()         );
        CPPUNIT_ASSERT( Bool_1.isNull()         );
        CPPUNIT_ASSERT( SignedInt_1.isNull()    );
        CPPUNIT_ASSERT( UnsignedInt_1.isNull()  );
        CPPUNIT_ASSERT( Float_1.isNull()        );
        CPPUNIT_ASSERT( String_1.isNull()       );
        CPPUNIT_ASSERT( Array_1.isNull()        );
        CPPUNIT_ASSERT( Map_1.isNull()          );
        CPPUNIT_ASSERT( Binary_1.isNull()          );

        //assert move-constructed items are same as original
        CPPUNIT_ASSERT( *v_empty == Null_2    );
        CPPUNIT_ASSERT( *v_char == Char_2  );
        CPPUNIT_ASSERT( *v_bool == Bool_2 );
        CPPUNIT_ASSERT( *v_signedint == SignedInt_2 );
        CPPUNIT_ASSERT( *v_unsignedint == UnsignedInt_2 );
        CPPUNIT_ASSERT( *v_float == Float_2 );
        CPPUNIT_ASSERT( *v_string == String_2 );
        CPPUNIT_ASSERT( *v_array == Array_2 );
        CPPUNIT_ASSERT( *v_map == Map_2 );
        CPPUNIT_ASSERT( *v_binary == Binary_2 );

    }

    void testMoveAssignment()
    {

        Value Null_to_Char(*v_empty);
        Value Char_to_Unsignedint(*v_char);
        Value Bool_to_Map(*v_bool);
        Value SignedInt_to_Array(*v_signedint);
        Value UnsignedInt_to_Null(*v_unsignedint);
        Value Float_to_Binary(*v_float);
        Value String_to_Bool(*v_string);
        Value Array_to_Float(*v_array);
        Value Map_to_Signedint(*v_map);
        Value Binary_to_String(*v_map);

        //make second copy, to be moved
        Value Null(*v_empty);
        Value Char(*v_char);
        Value Bool(*v_bool);
        Value SignedInt(*v_signedint);
        Value UnsignedInt(*v_unsignedint);
        Value Float(*v_float);
        Value String(*v_string);
        Value Array(*v_array);
        Value Map(*v_map);
        Value Binary(*v_binary);

        //move elements
        Null_to_Char = std::move(Char);
        Char_to_Unsignedint = std::move(UnsignedInt);
        Bool_to_Map = std::move(Map);
        SignedInt_to_Array = std::move(Array);
        UnsignedInt_to_Null = std::move(Null);
        Float_to_Binary = std::move(Binary);
        String_to_Bool = std::move(Bool);
        Array_to_Float = std::move(Float);
        Map_to_Signedint = std::move(SignedInt);
        Binary_to_String = std::move(String);

        //Assert Moved elements are nulled
        CPPUNIT_ASSERT( Null.isNull()         );
        CPPUNIT_ASSERT( Char.isNull()         );
        CPPUNIT_ASSERT( Bool.isNull()         );
        CPPUNIT_ASSERT( SignedInt.isNull()    );
        CPPUNIT_ASSERT( UnsignedInt.isNull()  );
        CPPUNIT_ASSERT( Float.isNull()        );
        CPPUNIT_ASSERT( String.isNull()       );
        CPPUNIT_ASSERT( Array.isNull()        );
        CPPUNIT_ASSERT( Map.isNull()          );
        CPPUNIT_ASSERT( Binary.isNull()          );

        //Assert move assignments
        CPPUNIT_ASSERT( *v_empty == UnsignedInt_to_Null    );
        CPPUNIT_ASSERT( *v_char == Null_to_Char  );
        CPPUNIT_ASSERT( *v_bool == String_to_Bool );
        CPPUNIT_ASSERT( *v_signedint == Map_to_Signedint );
        CPPUNIT_ASSERT( *v_unsignedint == Char_to_Unsignedint );
        CPPUNIT_ASSERT( *v_float == Array_to_Float );
        CPPUNIT_ASSERT( *v_string == Binary_to_String );
        CPPUNIT_ASSERT( *v_array == SignedInt_to_Array );
        CPPUNIT_ASSERT( *v_map == Bool_to_Map );
        CPPUNIT_ASSERT( *v_binary == Float_to_Binary );

    }

    void testCopyAssignment()
    {
        Value Null_to_Char(*v_empty);
        Value Char_to_Unsignedint(*v_char);
        Value Bool_to_Map(*v_bool);
        Value SignedInt_to_Array(*v_signedint);
        Value UnsignedInt_to_Null(*v_unsignedint);
        Value Float_to_Binary(*v_float);
        Value String_to_Bool(*v_string);
        Value Array_to_Float(*v_array);
        Value Map_to_Signedint(*v_map);
        Value Binary_to_String(*v_map);

        //assign new elements to them
        Null_to_Char = *v_char;
        Char_to_Unsignedint = *v_unsignedint;
        Bool_to_Map = *v_map;
        SignedInt_to_Array = *v_array;
        UnsignedInt_to_Null = *v_empty;
        Float_to_Binary = *v_binary;
        String_to_Bool = *v_bool;
        Array_to_Float = *v_float;
        Map_to_Signedint = *v_signedint;
        Binary_to_String = *v_string;

        //Assert assignments
        CPPUNIT_ASSERT( *v_empty == UnsignedInt_to_Null    );
        CPPUNIT_ASSERT( *v_char == Null_to_Char  );
        CPPUNIT_ASSERT( *v_bool == String_to_Bool );
        CPPUNIT_ASSERT( *v_signedint == Map_to_Signedint );
        CPPUNIT_ASSERT( *v_unsignedint == Char_to_Unsignedint );
        CPPUNIT_ASSERT( *v_float == Array_to_Float );
        CPPUNIT_ASSERT( *v_string == Binary_to_String );
        CPPUNIT_ASSERT( *v_array == SignedInt_to_Array );
        CPPUNIT_ASSERT( *v_map == Bool_to_Map );
        CPPUNIT_ASSERT( *v_binary == Float_to_Binary );

    }

};





class Value_Conversion_Test : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( Value_Conversion_Test );
    CPPUNIT_TEST( test_asBool );
    CPPUNIT_TEST( test_asInt );
    CPPUNIT_TEST( test_asUInt );
    CPPUNIT_TEST( test_asInt64 );
    CPPUNIT_TEST( test_asUInt64 );
    CPPUNIT_TEST( test_asFloat );
    CPPUNIT_TEST( test_asString );
    CPPUNIT_TEST( test_asBinary );
    /////////////////////////////
    CPPUNIT_TEST( test_intOperator );
    CPPUNIT_TEST( test_boolOperator );
    CPPUNIT_TEST( test_charOperator );
    CPPUNIT_TEST( test_doubleOperator );
    CPPUNIT_TEST( test_longlongOperator );
    CPPUNIT_TEST( test_unsignedlonglongOperator );
    CPPUNIT_TEST( test_stringTypeOperator );
    CPPUNIT_TEST( test_stringMoveOperation );
    CPPUNIT_TEST( test_binaryTypeOperator );
    CPPUNIT_TEST( test_binaryMoveOperation );
    CPPUNIT_TEST_SUITE_END();
public:
    using T = Value::BinaryType::value_type;
    void setUp() override
    {
        v_empty = std::make_unique<Value>(Value());
        v_char = std::make_unique<Value>(Value('c'));
        v_bool = std::make_unique<Value>(Value(true));
        v_signedint = std::make_unique<Value>(Value(-700));
        v_unsignedint = std::make_unique<Value>(Value(800ull));
        v_float = std::make_unique<Value>(Value(3.1416));
        v_string = std::make_unique<Value>(Value("string"));
        v_array = std::make_unique<Value>(Value({34.657, "Yeepa", 466, -53, 'g'}));
        v_map = std::make_unique<Value>(Value());

        v_binary = std::make_unique<Value>(Value(Value::BinaryType({T(0xab), T(0xbc), T(0xcd), T(0xdf)})));

        (*v_map)["name"] = "WhiZTiM";
        (*v_map)["id"] = 12343;
        (*v_map)["extras"] = { *v_array, "nice one bro!" };
    }
private:
    Value::Uptr v_empty;
    Value::Uptr v_char;
    Value::Uptr v_bool;
    Value::Uptr v_signedint;
    Value::Uptr v_unsignedint;
    Value::Uptr v_float;
    Value::Uptr v_string;
    Value::Uptr v_array;
    Value::Uptr v_map;
    Value::Uptr v_binary;
public:
    void test_asBool()
    {
        CPPUNIT_ASSERT_EQUAL( v_empty->asBool(),        false );
        CPPUNIT_ASSERT_EQUAL( v_char->asBool(),         true  );
        CPPUNIT_ASSERT_EQUAL( v_bool->asBool(),         true  );
        CPPUNIT_ASSERT_EQUAL( v_signedint->asBool(),    true  );
        CPPUNIT_ASSERT_EQUAL( v_unsignedint->asBool(),  true  );
        CPPUNIT_ASSERT_EQUAL( v_float->asBool(),        true  );
        CPPUNIT_ASSERT_EQUAL( v_string->asBool(),       true  );
        CPPUNIT_ASSERT_EQUAL( v_array->asBool(),        true  );
        CPPUNIT_ASSERT_EQUAL( v_map->asBool(),          true  );
        CPPUNIT_ASSERT_EQUAL( v_binary->asBool(),       true  );
    }

    void test_asInt()
    {
        CPPUNIT_ASSERT_EQUAL(0,             v_empty->asInt() );  //valid
        CPPUNIT_ASSERT_EQUAL(99,             v_char->asInt() );  //valid
        CPPUNIT_ASSERT_EQUAL(1,              v_bool->asInt() );  //valid
        CPPUNIT_ASSERT_EQUAL(-700,      v_signedint->asInt() );  //valid
        CPPUNIT_ASSERT_EQUAL(800,     v_unsignedint->asInt() );  //valid
        CPPUNIT_ASSERT_EQUAL(3,             v_float->asInt() );  //valid, floor division
        CPPUNIT_ASSERT_EQUAL(0,            v_string->asInt() );  //undefined if invalid
        CPPUNIT_ASSERT_EQUAL(5,             v_array->asInt() );  //undefined
        CPPUNIT_ASSERT_EQUAL(3,               v_map->asInt() );  //undefined
        CPPUNIT_ASSERT_EQUAL(1,            v_binary->asInt() );  //undefined
    }

    void test_asUInt()
    {
        CPPUNIT_ASSERT_EQUAL(0u,            v_empty->asUint() );  //valid
        CPPUNIT_ASSERT_EQUAL(99u,            v_char->asUint() );  //valid
        CPPUNIT_ASSERT_EQUAL(1u,             v_bool->asUint() );  //valid
        CPPUNIT_ASSERT_EQUAL(0u,        v_signedint->asUint() );  //valid
        CPPUNIT_ASSERT_EQUAL(800u,    v_unsignedint->asUint() );  //valid
        CPPUNIT_ASSERT_EQUAL(3u,            v_float->asUint() );  //valid, floor division
        CPPUNIT_ASSERT_EQUAL(0u,           v_string->asUint() );  //undefined if invalid
        CPPUNIT_ASSERT_EQUAL(5u,            v_array->asUint() );  //undefined
        CPPUNIT_ASSERT_EQUAL(3u,              v_map->asUint() );  //undefined
        CPPUNIT_ASSERT_EQUAL(1u,           v_binary->asUint() );  //undefined
    }

    void test_asInt64()
    {
        CPPUNIT_ASSERT_EQUAL(0ll,            v_empty->asInt64() );  //valid
        CPPUNIT_ASSERT_EQUAL(99ll,            v_char->asInt64() );  //valid
        CPPUNIT_ASSERT_EQUAL(1ll,             v_bool->asInt64() );  //valid
        CPPUNIT_ASSERT_EQUAL(-700ll,        v_signedint->asInt64() );  //valid
        CPPUNIT_ASSERT_EQUAL(800ll,    v_unsignedint->asInt64() );  //valid
        CPPUNIT_ASSERT_EQUAL(3ll,            v_float->asInt64() );  //valid, floor division
        CPPUNIT_ASSERT_EQUAL(0ll,           v_string->asInt64() );  //undefined if invalid
        CPPUNIT_ASSERT_EQUAL(5ll,            v_array->asInt64() );  //undefined
        CPPUNIT_ASSERT_EQUAL(3ll,              v_map->asInt64() );  //undefined
        CPPUNIT_ASSERT_EQUAL(1ll,           v_binary->asInt64() );  //undefined
    }

    void test_asUInt64()
    {
        CPPUNIT_ASSERT_EQUAL(0ull,            v_empty->asUint64() );  //valid
        CPPUNIT_ASSERT_EQUAL(99ull,            v_char->asUint64() );  //valid
        CPPUNIT_ASSERT_EQUAL(1ull,             v_bool->asUint64() );  //valid
        CPPUNIT_ASSERT_EQUAL(0ull,        v_signedint->asUint64() );  //valid
        CPPUNIT_ASSERT_EQUAL(800ull,    v_unsignedint->asUint64() );  //valid
        CPPUNIT_ASSERT_EQUAL(3ull,            v_float->asUint64() );  //valid, floor division
        CPPUNIT_ASSERT_EQUAL(0ull,           v_string->asUint64() );  //undefined if invalid
        CPPUNIT_ASSERT_EQUAL(5ull,            v_array->asUint64() );  //undefined
        CPPUNIT_ASSERT_EQUAL(3ull,              v_map->asUint64() );  //undefined
        CPPUNIT_ASSERT_EQUAL(1ll,             v_binary->asInt64() );  //undefined
    }

    void test_asFloat()
    {
        CPPUNIT_ASSERT_EQUAL(0.0,            v_empty->asFloat() );  //valid
        CPPUNIT_ASSERT_EQUAL(99.0,            v_char->asFloat() );  //valid
        CPPUNIT_ASSERT_EQUAL(1.0,             v_bool->asFloat() );  //valid
        CPPUNIT_ASSERT_EQUAL(0.0,        v_signedint->asFloat() );  //valid
        CPPUNIT_ASSERT_EQUAL(800.0,    v_unsignedint->asFloat() );  //valid
        CPPUNIT_ASSERT_EQUAL(3.1416,         v_float->asFloat() );  //valid, floor division
        CPPUNIT_ASSERT_EQUAL(0.0,           v_string->asFloat() );  //undefined if invalid
        CPPUNIT_ASSERT_EQUAL(5.0,            v_array->asFloat() );  //undefined
        CPPUNIT_ASSERT_EQUAL(3.0,              v_map->asFloat() );  //undefined
        CPPUNIT_ASSERT_EQUAL(1.0,           v_binary->asFloat() );  //undefined
    }

    void test_asString()
    {
        CPPUNIT_ASSERT_EQUAL(""s,               v_empty->asString() );  //valid
        CPPUNIT_ASSERT_EQUAL("c"s,               v_char->asString() );  //valid
        CPPUNIT_ASSERT_EQUAL("true"s,            v_bool->asString() );  //valid
        CPPUNIT_ASSERT_EQUAL("-700"s,       v_signedint->asString() );  //valid
        CPPUNIT_ASSERT_EQUAL("800"s,      v_unsignedint->asString() );  //valid
        CPPUNIT_ASSERT_EQUAL("3.141600"s,       v_float->asString() );  //valid, zero dependent
        CPPUNIT_ASSERT_EQUAL("string"s,           v_string->asString() );  //undefined if invalid
        CPPUNIT_ASSERT_EQUAL(""s,            v_array->asString() );  //undefined
        CPPUNIT_ASSERT_EQUAL(""s,              v_map->asString() );  //undefined
        CPPUNIT_ASSERT_EQUAL(""s,           v_binary->asString() );  //undefined
    }

    void test_asBinary()
    {
        using Binary = Value::BinaryType;
        Value v1('c'), v2(true), v3(false);
        CPPUNIT_ASSERT_EQUAL(Binary({T(0x63)}),  v1.asBinary() );  //valid
        CPPUNIT_ASSERT_EQUAL(Binary({T(0x01)}),  v2.asBinary() );  //valid
        CPPUNIT_ASSERT_EQUAL(Binary({T(0x00)}),  v3.asBinary() );  //valid

        CPPUNIT_ASSERT_EQUAL(Binary(),          v_empty->asBinary() );  //valid
        CPPUNIT_ASSERT_EQUAL(Binary({T(0x63)}),  v_char->asBinary() );  //valid
        CPPUNIT_ASSERT_EQUAL(Binary({T(0x1)}),   v_bool->asBinary() );  //valid

        CPPUNIT_ASSERT_EQUAL(Value::BinaryType({T(0xab), T(0xbc), T(0xcd), T(0xdf)}), v_binary->asBinary() );

        //The rest are platform independent
    }

    void test_intOperator()
    {
        CPPUNIT_ASSERT_THROW(static_cast<int>(*v_empty), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(static_cast<int>(*v_char), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(static_cast<int>(*v_bool), timl::bad_value_cast);
        CPPUNIT_ASSERT_NO_THROW(static_cast<int>(*v_signedint));
        CPPUNIT_ASSERT_THROW(static_cast<int>(*v_unsignedint), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(static_cast<int>(*v_float), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(static_cast<int>(*v_string), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(static_cast<int>(*v_array), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(static_cast<int>(*v_map), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(static_cast<int>(*v_binary), timl::bad_value_cast);

        CPPUNIT_ASSERT_THROW(int k = *v_empty, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(int k = *v_char, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(int k = *v_bool, timl::bad_value_cast);
        CPPUNIT_ASSERT_NO_THROW(int k = *v_signedint);
        CPPUNIT_ASSERT_THROW(int k = *v_unsignedint, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(int k = *v_float, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(int k = *v_string, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(int k = *v_array, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(int k = *v_map, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(int k = *v_binary, timl::bad_value_cast);

        //Call const and call version
        CPPUNIT_ASSERT_THROW(int k = const_cast<const Value&>(*v_empty), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(int k = const_cast<const Value&>(*v_char), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(int k = const_cast<const Value&>(*v_bool), timl::bad_value_cast);
        CPPUNIT_ASSERT_NO_THROW(int k = const_cast<const Value&>(*v_signedint));
        CPPUNIT_ASSERT_THROW(int k = const_cast<const Value&>(*v_unsignedint), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(int k = const_cast<const Value&>(*v_float), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(int k = const_cast<const Value&>(*v_string), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(int k = const_cast<const Value&>(*v_array), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(int k = const_cast<const Value&>(*v_map), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(int k = const_cast<const Value&>(*v_binary), timl::bad_value_cast);
    }

    void test_boolOperator()
    {
        //Type bool is a first-class type of Value, hence, we can obtain a reference
        CPPUNIT_ASSERT_THROW(bool& k = *v_empty, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(bool& k = *v_char, timl::bad_value_cast);
        CPPUNIT_ASSERT_NO_THROW(bool& k = *v_bool);
        CPPUNIT_ASSERT_THROW(bool& k = *v_signedint, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(bool& k = *v_unsignedint, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(bool& k = *v_float, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(bool& k = *v_string, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(bool& k = *v_array, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(bool& k = *v_map, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(bool& k = *v_binary, timl::bad_value_cast);

        //Call const and call version
        CPPUNIT_ASSERT_THROW(const bool& k = const_cast<const Value&>(*v_empty), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const bool& k = const_cast<const Value&>(*v_char), timl::bad_value_cast);
        CPPUNIT_ASSERT_NO_THROW(const bool& k = const_cast<const Value&>(*v_bool));
        CPPUNIT_ASSERT_THROW(const bool& k = const_cast<const Value&>(*v_signedint), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const bool& k = const_cast<const Value&>(*v_unsignedint), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const bool& k = const_cast<const Value&>(*v_float), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const bool& k = const_cast<const Value&>(*v_string), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const bool& k = const_cast<const Value&>(*v_array), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const bool& k = const_cast<const Value&>(*v_map), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const bool& k = const_cast<const Value&>(*v_binary), timl::bad_value_cast);

        // Reference modification
        Value Bool(true);
        CPPUNIT_ASSERT(Bool == Value(true));
        bool& ref_var = Bool;
        ref_var = false;
        CPPUNIT_ASSERT(Bool == Value(false));
    }

    void test_charOperator()
    {

        //Type char is a first-class type of Value, hence, we can obtain a reference
        CPPUNIT_ASSERT_THROW(char& k = *v_empty, timl::bad_value_cast);
        CPPUNIT_ASSERT_NO_THROW(char& k = *v_char);
        CPPUNIT_ASSERT_THROW(char& k = *v_bool, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(char& k = *v_signedint, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(char& k = *v_unsignedint, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(char& k = *v_float, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(char& k = *v_string, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(char& k = *v_array, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(char& k = *v_map, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(char& k = *v_binary, timl::bad_value_cast);

        //Call const and call version
        CPPUNIT_ASSERT_THROW(const char& k = const_cast<const Value&>(*v_empty), timl::bad_value_cast);
        CPPUNIT_ASSERT_NO_THROW(const char& k = const_cast<const Value&>(*v_char));
        CPPUNIT_ASSERT_THROW(const char& k = const_cast<const Value&>(*v_bool), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const char& k = const_cast<const Value&>(*v_signedint), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const char& k = const_cast<const Value&>(*v_unsignedint), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const char& k = const_cast<const Value&>(*v_float), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const char& k = const_cast<const Value&>(*v_string), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const char& k = const_cast<const Value&>(*v_array), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const char& k = const_cast<const Value&>(*v_map), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const char& k = const_cast<const Value&>(*v_binary), timl::bad_value_cast);

        // Reference modification
        Value Char('Q');
        CPPUNIT_ASSERT(Char == Value('Q'));
        char& ref_var = Char;
        ref_var = '@';
        CPPUNIT_ASSERT(Char == Value('@'));
    }

    void test_doubleOperator()
    {
        //Type double is a first-class type of Value, hence, we can obtain a reference
        CPPUNIT_ASSERT_THROW(double& k = *v_empty, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(double& k = *v_char, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(double& k = *v_bool, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(double& k = *v_signedint, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(double& k = *v_unsignedint, timl::bad_value_cast);
        CPPUNIT_ASSERT_NO_THROW(double& k = *v_float);
        CPPUNIT_ASSERT_THROW(double& k = *v_string, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(double& k = *v_array, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(double& k = *v_map, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(double& k = *v_binary, timl::bad_value_cast);

        //Call const and call version
        CPPUNIT_ASSERT_THROW(const double& k = const_cast<const Value&>(*v_empty), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const double& k = const_cast<const Value&>(*v_char), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const double& k = const_cast<const Value&>(*v_bool), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const double& k = const_cast<const Value&>(*v_signedint), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const double& k = const_cast<const Value&>(*v_unsignedint), timl::bad_value_cast);
        CPPUNIT_ASSERT_NO_THROW(const double& k = const_cast<const Value&>(*v_float));
        CPPUNIT_ASSERT_THROW(const double& k = const_cast<const Value&>(*v_string), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const double& k = const_cast<const Value&>(*v_array), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const double& k = const_cast<const Value&>(*v_map), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const double& k = const_cast<const Value&>(*v_binary), timl::bad_value_cast);

        // Reference modification
        Value Double(3.1416);
        CPPUNIT_ASSERT(Double == Value(3.1416));
        double& ref_var = Double;
        ref_var = 9.80665;
        CPPUNIT_ASSERT(Double == Value(9.80665));
    }

    void test_longlongOperator()
    {
        //Type long long is a first-class type of Value, hence, we can obtain a reference
        CPPUNIT_ASSERT_THROW(long long& k = *v_empty, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(long long& k = *v_char, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(long long& k = *v_bool, timl::bad_value_cast);
        CPPUNIT_ASSERT_NO_THROW(long long& k = *v_signedint);
        CPPUNIT_ASSERT_THROW(long long& k = *v_unsignedint, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(long long& k = *v_float, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(long long& k = *v_string, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(long long& k = *v_array, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(long long& k = *v_map, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(long long& k = *v_binary, timl::bad_value_cast);

        //Call const and call version
        CPPUNIT_ASSERT_THROW(const long long& k = const_cast<const Value&>(*v_empty), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const long long& k = const_cast<const Value&>(*v_char), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const long long& k = const_cast<const Value&>(*v_bool), timl::bad_value_cast);
        CPPUNIT_ASSERT_NO_THROW(const long long& k = const_cast<const Value&>(*v_signedint));
        CPPUNIT_ASSERT_THROW(const long long& k = const_cast<const Value&>(*v_unsignedint), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const long long& k = const_cast<const Value&>(*v_float), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const long long& k = const_cast<const Value&>(*v_string), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const long long& k = const_cast<const Value&>(*v_array), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const long long& k = const_cast<const Value&>(*v_map), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const long long& k = const_cast<const Value&>(*v_binary), timl::bad_value_cast);

        // Reference modification
        Value LongLong(987654321);
        CPPUNIT_ASSERT(LongLong == Value(987654321));
        long long& ref_var = LongLong;
        ref_var = -123456789;
        CPPUNIT_ASSERT(LongLong == Value(-123456789));
    }

    void test_unsignedlonglongOperator()
    {
        //Type unsigned long long is a first-class type of Value, hence, we can obtain a reference
        CPPUNIT_ASSERT_THROW(unsigned long long& k = *v_empty, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(unsigned long long& k = *v_char, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(unsigned long long& k = *v_bool, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(unsigned long long& k = *v_signedint, timl::bad_value_cast);
        CPPUNIT_ASSERT_NO_THROW(unsigned long long& k = *v_unsignedint);
        CPPUNIT_ASSERT_THROW(unsigned long long& k = *v_float, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(unsigned long long& k = *v_string, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(unsigned long long& k = *v_array, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(unsigned long long& k = *v_map, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(unsigned long long& k = *v_binary, timl::bad_value_cast);

        //Call const and call version
        CPPUNIT_ASSERT_THROW(const unsigned long long& k = const_cast<const Value&>(*v_empty), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const unsigned long long& k = const_cast<const Value&>(*v_char), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const unsigned long long& k = const_cast<const Value&>(*v_bool), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const unsigned long long& k = const_cast<const Value&>(*v_signedint), timl::bad_value_cast);
        CPPUNIT_ASSERT_NO_THROW(const unsigned long long& k = const_cast<const Value&>(*v_unsignedint));
        CPPUNIT_ASSERT_THROW(const unsigned long long& k = const_cast<const Value&>(*v_float), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const unsigned long long& k = const_cast<const Value&>(*v_string), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const unsigned long long& k = const_cast<const Value&>(*v_array), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const unsigned long long& k = const_cast<const Value&>(*v_map), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const unsigned long long& k = const_cast<const Value&>(*v_binary), timl::bad_value_cast);

        // Reference modification
        Value ULongLong(987654321ull);
        CPPUNIT_ASSERT(ULongLong == Value(987654321));
        unsigned long long& ref_var = ULongLong;
        ref_var = 123456789;
        CPPUNIT_ASSERT(ULongLong == Value(123456789));
    }

    void test_stringTypeOperator()
    {
        //Type std::string is a first-class type of Value, hence, we can obtain a reference
        CPPUNIT_ASSERT_THROW(std::string& k = *v_empty, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(std::string& k = *v_char, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(std::string& k = *v_bool, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(std::string& k = *v_signedint, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(std::string& k = *v_unsignedint, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(std::string& k = *v_float, timl::bad_value_cast);
        CPPUNIT_ASSERT_NO_THROW(std::string& k = *v_string);
        CPPUNIT_ASSERT_THROW(std::string& k = *v_array, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(std::string& k = *v_map, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(std::string& k = *v_binary, timl::bad_value_cast);

        //Call const and call version
        CPPUNIT_ASSERT_THROW(const std::string& k = const_cast<const Value&>(*v_empty), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const std::string& k = const_cast<const Value&>(*v_char), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const std::string& k = const_cast<const Value&>(*v_bool), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const std::string& k = const_cast<const Value&>(*v_signedint), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const std::string& k = const_cast<const Value&>(*v_unsignedint), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const std::string& k = const_cast<const Value&>(*v_float), timl::bad_value_cast);
        CPPUNIT_ASSERT_NO_THROW(const std::string& k = const_cast<const Value&>(*v_string));
        CPPUNIT_ASSERT_THROW(const std::string& k = const_cast<const Value&>(*v_array), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const std::string& k = const_cast<const Value&>(*v_map), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const std::string& k = const_cast<const Value&>(*v_binary), timl::bad_value_cast);

        // Reference modification
        Value String_A("Nice");
        CPPUNIT_ASSERT(String_A == Value("Nice"));
        std::string& ref_var = String_A;
        ref_var = "Great";
        CPPUNIT_ASSERT(String_A == Value("Great"));
    }

    void test_stringMoveOperation()
    {
        //For Move conversion test
        Value Null(*v_empty);
        Value Char(*v_char);
        Value Bool(*v_bool);
        Value SignedInt(*v_signedint);
        Value UnsignedInt(*v_unsignedint);
        Value Float(*v_float);
        Value String(*v_string);
        Value Array(*v_array);
        Value Map(*v_map);
        Value Binary(*v_binary);

        CPPUNIT_ASSERT_THROW(std::string&& k = std::move(Null), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(std::string&& k = std::move(Char), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(std::string&& k = std::move(Bool), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(std::string&& k = std::move(SignedInt), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(std::string&& k = std::move(UnsignedInt), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(std::string&& k = std::move(Float), timl::bad_value_cast);
        CPPUNIT_ASSERT_NO_THROW(std::string&& k = std::move(String));
        CPPUNIT_ASSERT_THROW(std::string&& k = std::move(Array), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(std::string&& k = std::move(Map), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(std::string&& k = std::move(Binary), timl::bad_value_cast);

        //Assert Invariants are maintained after move failure
        CPPUNIT_ASSERT( *v_empty == Null    );
        CPPUNIT_ASSERT( *v_char == Char  );
        CPPUNIT_ASSERT( *v_bool == Bool );
        CPPUNIT_ASSERT( *v_signedint == SignedInt );
        CPPUNIT_ASSERT( *v_unsignedint == UnsignedInt );
        CPPUNIT_ASSERT( *v_float == Float );
        CPPUNIT_ASSERT( *v_string != String );
        CPPUNIT_ASSERT( *v_array == Array );
        CPPUNIT_ASSERT( *v_map == Map );
        CPPUNIT_ASSERT( *v_binary == Binary);

        // Move Assertion
        Value String_A("Nice");
        CPPUNIT_ASSERT(String_A == Value("Nice"));
        std::string var = std::move(String_A);
        CPPUNIT_ASSERT(String_A == Value(""));
        CPPUNIT_ASSERT(var == Value("Nice"));
    }

    void test_binaryTypeOperator()
    {
        //Type Value::BinaryType is a first-class type of Value, hence, we can obtain a reference
        CPPUNIT_ASSERT_THROW(Value::BinaryType& k = *v_empty, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(Value::BinaryType& k = *v_char, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(Value::BinaryType& k = *v_bool, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(Value::BinaryType& k = *v_signedint, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(Value::BinaryType& k = *v_unsignedint, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(Value::BinaryType& k = *v_float, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(Value::BinaryType& k = *v_string, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(Value::BinaryType& k = *v_array, timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(Value::BinaryType& k = *v_map, timl::bad_value_cast);
        CPPUNIT_ASSERT_NO_THROW(Value::BinaryType& k = *v_binary);

        //Call const and call version
        CPPUNIT_ASSERT_THROW(const Value::BinaryType& k = const_cast<const Value&>(*v_empty), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const Value::BinaryType& k = const_cast<const Value&>(*v_char), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const Value::BinaryType& k = const_cast<const Value&>(*v_bool), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const Value::BinaryType& k = const_cast<const Value&>(*v_signedint), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const Value::BinaryType& k = const_cast<const Value&>(*v_unsignedint), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const Value::BinaryType& k = const_cast<const Value&>(*v_float), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const Value::BinaryType& k = const_cast<const Value&>(*v_string), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const Value::BinaryType& k = const_cast<const Value&>(*v_array), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(const Value::BinaryType& k = const_cast<const Value&>(*v_map), timl::bad_value_cast);
        CPPUNIT_ASSERT_NO_THROW(const Value::BinaryType& k = const_cast<const Value&>(*v_binary));


        // Reference modification
        Value Binary = Value::BinaryType({T(0xab), T(0xbc), T(0xcd), T(0xdf)});
        CPPUNIT_ASSERT(Binary == Value::BinaryType({T(0xab), T(0xbc), T(0xcd), T(0xdf)}));
        Value::BinaryType& ref_var = Binary;
        ref_var[3] = T(0xFF);
        CPPUNIT_ASSERT(Binary == Value::BinaryType({T(0xab), T(0xbc), T(0xcd), T(0xff)}));
    }

    void test_binaryMoveOperation()
    {
        //For Move conversion test
        Value Null(*v_empty);
        Value Char(*v_char);
        Value Bool(*v_bool);
        Value SignedInt(*v_signedint);
        Value UnsignedInt(*v_unsignedint);
        Value Float(*v_float);
        Value String(*v_string);
        Value Array(*v_array);
        Value Map(*v_map);
        Value Binary(*v_binary);

        CPPUNIT_ASSERT_THROW(Value::BinaryType&& k = std::move(Null), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(Value::BinaryType&& k = std::move(Char), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(Value::BinaryType&& k = std::move(Bool), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(Value::BinaryType&& k = std::move(SignedInt), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(Value::BinaryType&& k = std::move(UnsignedInt), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(Value::BinaryType&& k = std::move(Float), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(Value::BinaryType&& k = std::move(String), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(Value::BinaryType&& k = std::move(Array), timl::bad_value_cast);
        CPPUNIT_ASSERT_THROW(Value::BinaryType&& k = std::move(Map), timl::bad_value_cast);
        CPPUNIT_ASSERT_NO_THROW(Value::BinaryType&& k = std::move(Binary));

        //Assert Invariants are maintained after move failure
        CPPUNIT_ASSERT( *v_empty == Null    );
        CPPUNIT_ASSERT( *v_char == Char  );
        CPPUNIT_ASSERT( *v_bool == Bool );
        CPPUNIT_ASSERT( *v_signedint == SignedInt );
        CPPUNIT_ASSERT( *v_unsignedint == UnsignedInt );
        CPPUNIT_ASSERT( *v_float == Float );
        CPPUNIT_ASSERT( *v_string == String );
        CPPUNIT_ASSERT( *v_array == Array );
        CPPUNIT_ASSERT( *v_map == Map );
        CPPUNIT_ASSERT( *v_binary != Binary);

        // Move Assertion
        Value Binary_A = Value::BinaryType({T(0xab), T(0xbc), T(0xcd), T(0xdf)});
        CPPUNIT_ASSERT(Binary_A == Value::BinaryType({T(0xab), T(0xbc), T(0xcd), T(0xdf)}));
        Value::BinaryType var = std::move(Binary_A);
        CPPUNIT_ASSERT(Binary_A == Value(Value::BinaryType()));
        CPPUNIT_ASSERT(var == Value::BinaryType({T(0xab), T(0xbc), T(0xcd), T(0xdf)}));
    }
};




class Value_Map_and_Array_Test : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( Value_Map_and_Array_Test );
    CPPUNIT_TEST( test_pushBack );
    CPPUNIT_TEST( test_IndexingOperator );
    CPPUNIT_TEST_SUITE_END();
public:
    using T = Value::BinaryType::value_type;
    void setUp() override
    {
        v_empty = std::make_unique<Value>(Value());
        v_char = std::make_unique<Value>(Value('c'));
        v_bool = std::make_unique<Value>(Value(true));
        v_signedint = std::make_unique<Value>(Value(-700));
        v_unsignedint = std::make_unique<Value>(Value(800ull));
        v_float = std::make_unique<Value>(Value(3.1416));
        v_string = std::make_unique<Value>(Value("string"));
        v_array = std::make_unique<Value>(Value({34.657, "Yeepa", 466, -53, 'g'}));
        v_map = std::make_unique<Value>(Value());

        v_binary = std::make_unique<Value>(Value(Value::BinaryType({T(0xab), T(0xbc), T(0xcd), T(0xdf)})));

        (*v_map)["name"] = "WhiZTiM";
        (*v_map)["id"] = 12343;
        (*v_map)["extras"] = { *v_array, "nice one bro!" };
    }
private:
    Value::Uptr v_empty;
    Value::Uptr v_char;
    Value::Uptr v_bool;
    Value::Uptr v_signedint;
    Value::Uptr v_unsignedint;
    Value::Uptr v_float;
    Value::Uptr v_string;
    Value::Uptr v_array;
    Value::Uptr v_map;
    Value::Uptr v_binary;
public:

    void test_pushBack()
    {
        Value Null(*v_empty);
        Value Char(*v_char);
        Value Bool(*v_bool);
        Value SignedInt(*v_signedint);
        Value UnsignedInt(*v_unsignedint);
        Value Float(*v_float);
        Value String(*v_string);
        Value Array(*v_array);
        Value Map(*v_map);
        Value Binary(*v_binary);

        Null.push_back( "Sweet" );
        CPPUNIT_ASSERT( Null.isArray() );
        CPPUNIT_ASSERT_EQUAL(size_t(1), Null.size());
        CPPUNIT_ASSERT( Value({"Sweet"}) == Null );

        Char.push_back( "Sweet" );
        CPPUNIT_ASSERT( Char.isArray() );
        CPPUNIT_ASSERT_EQUAL(size_t(2), Char.size());
        CPPUNIT_ASSERT( Value({'c', "Sweet"}) == Char );

        Bool.push_back( Char );
        CPPUNIT_ASSERT( Bool.isArray() );
        CPPUNIT_ASSERT_EQUAL(size_t(2), Bool.size());
        CPPUNIT_ASSERT( Value({true, {'c', "Sweet"}}) == Bool );

        SignedInt.push_back( std::move(Bool) );
        CPPUNIT_ASSERT( SignedInt.isArray() );
        CPPUNIT_ASSERT_EQUAL(size_t(2), SignedInt.size());
        CPPUNIT_ASSERT( Value(
                                { -700,
                                  { true,
                                     {'c', "Sweet"}
                                  }
                                }
                            ) == SignedInt );
        CPPUNIT_ASSERT(Bool.isNull());

        //CPPUNIT_ASSERT_THROW( String )

    }

    void test_IndexingOperator()
    {
        Value Null(*v_empty);
        Value Char(*v_char);
        Value Bool(*v_bool);
        Value SignedInt(*v_signedint);
        Value UnsignedInt(*v_unsignedint);
        Value Float(*v_float);
        Value String(*v_string);
        Value Array(*v_array);
        Value Map(*v_map);
        Value Binary(*v_binary);

        //test Array indexing operator
        CPPUNIT_ASSERT_THROW(Null[0], timl::value_exception);
        CPPUNIT_ASSERT_THROW(Char[2], timl::value_exception);
        CPPUNIT_ASSERT_THROW(Bool[7], timl::value_exception);
        CPPUNIT_ASSERT_THROW(SignedInt[0], timl::value_exception);
        CPPUNIT_ASSERT_THROW(UnsignedInt[0], timl::value_exception);
        CPPUNIT_ASSERT_THROW(Float[0], timl::value_exception);
        CPPUNIT_ASSERT_THROW(String[0], timl::value_exception);
        CPPUNIT_ASSERT_NO_THROW(Array[0] );
        CPPUNIT_ASSERT_THROW(Map[0], timl::value_exception);
        CPPUNIT_ASSERT_THROW(Binary[0], timl::value_exception);


        CPPUNIT_ASSERT_EQUAL( std::size_t(3), Map.size() );
        //Test Map insexing Operator
        CPPUNIT_ASSERT_NO_THROW(Null["haha"]);
        CPPUNIT_ASSERT_THROW(Char["haha"], timl::value_exception);
        CPPUNIT_ASSERT_THROW(Bool["haha"], timl::value_exception);
        CPPUNIT_ASSERT_THROW(SignedInt["haha"], timl::value_exception);
        CPPUNIT_ASSERT_THROW(UnsignedInt["haha"], timl::value_exception);
        CPPUNIT_ASSERT_THROW(Float["haha"], timl::value_exception);
        CPPUNIT_ASSERT_THROW(String["haha"], timl::value_exception);
        CPPUNIT_ASSERT_THROW(Array["haha"], timl::value_exception );
        CPPUNIT_ASSERT_NO_THROW(Map["haha"]);
        CPPUNIT_ASSERT_THROW(Binary["haha"], timl::value_exception);
        CPPUNIT_ASSERT_EQUAL( std::size_t(4), Map.size() );
    }

};



CPPUNIT_TEST_SUITE_REGISTRATION( Value_Construction_Test );
CPPUNIT_TEST_SUITE_REGISTRATION( Value_Conversion_Test );
CPPUNIT_TEST_SUITE_REGISTRATION( Value_Map_and_Array_Test );

int main()
{
    /*
    Value v{345};
    Value ha;
    ha["sani"] = "Mark";
    ha["musa"] = "Yusuf";
    ha["kabir"] = 4546.34;
    cout << "Hello World!" << endl;
    double m = static_cast<int>(v) * static_cast<double>(ha["kabir"]) * 3.0;

    cout << "m: " << m << endl;
    cout << "value: " << ha["kabir"].asString() << endl << endl;

    for(auto a = ha.begin(); a != ha.end() ; a++)
    {
        cout << '\t' << a->asString() << endl;
    }

    for(const auto& a : ha)
        cout << '\t' << a.asFloat() << endl;

    cout << "Sizeof(Value::ValueHolder::Char): " << sizeof(Value::ValueHolder::Char) << " bytes\n";
    cout << "Sizeof(Value::ValueHolder::Bool): " << sizeof(Value::ValueHolder::Bool) << " bytes\n";
    cout << "Sizeof(Value::ValueHolder::SignedInt): " << sizeof(Value::ValueHolder::SignedInt) << " bytes\n";
    cout << "Sizeof(Value::ValueHolder::UnsignedInt): " << sizeof(Value::ValueHolder::UnsignedInt) << " bytes\n";
    cout << "Sizeof(Value::ValueHolder::Float): " << sizeof(Value::ValueHolder::Float) << " bytes\n";
    cout << "Sizeof(Value::ValueHolder::String): " << sizeof(Value::ValueHolder::String) << " bytes\n";
    cout << "Sizeof(Value::ValueHolder::Array): " << sizeof(Value::ValueHolder::Array) << " bytes\n";
    cout << "Sizeof(Value::ValueHolder::Binary): " << sizeof(Value::ValueHolder::Binary) << " bytes\n";
    cout << "Sizeof(Value::ValueHolder::Map): " << sizeof(Value::ValueHolder::Map) << " bytes\n\n";

    cout << "Sizeof(Value::ValueHolder): " << sizeof(Value::ValueHolder) << " bytes\n";
    cout << "Sizeof(Value): " << sizeof(Value) << " bytes\n";
    */
    using namespace std::chrono;
    auto start = high_resolution_clock::now();

    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();
    runner.addTest( registry.makeTest() );
    runner.run();

    cout << "Test time:" << (duration_cast<milliseconds>(high_resolution_clock::now() - start ).count()) << " milliseconds\n";

    return 0;
}

