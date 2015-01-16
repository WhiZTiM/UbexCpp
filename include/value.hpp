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
#ifndef VALUE_H
#define VALUE_H

#include <memory>
#include <string>
#include <vector>
#include <numeric>
#include <unordered_map>
#include <initializer_list>
#include "exception.hpp"
#include "iterator.hpp"
#include "types.hpp"

namespace timl {

    class Value
    {
    public:
        using Sptr = std::shared_ptr<Value>;
        using Uptr = std::unique_ptr<Value>;
        using ArrayType = std::vector<Uptr>;
        using BinaryType = std::vector<byte>;
        using MapType = std::unordered_map<std::string, Uptr>;
        using iterator = value_iterator<Value, ArrayType, MapType>;
        //TODO
        //using const_iterator = const value_iterator<Value, ArrayType, MapType>;


        union ValueHolder
        {
            char Char;
            bool Bool;
            long long SignedInt;
            unsigned long long UnsignedInt;
            double Float;
            std::string String;
            ArrayType Array;
            BinaryType Binary;
            MapType Map;

            ValueHolder() {}
            ~ValueHolder() {}
        };


        Value();
        Value(int);
        Value(bool);
        Value(char);
        Value(double);
        Value(long long);
        Value(const char*);
        Value(unsigned long long);
        Value(std::string);
        Value(BinaryType);
        Value(std::initializer_list<Value>);

        Value(Value&&);
        Value(const Value&);

        ~Value();

        Type type() const noexcept;
        size_t size() const noexcept;

        bool isMap() const noexcept;
        bool isNull() const noexcept;
        bool isChar() const noexcept;
        bool isBool() const noexcept;
        bool isFloat() const noexcept;
        bool isArray() const noexcept;
        bool isObject() const noexcept;
        bool isString() const noexcept;
        bool isBinary() const noexcept;
        bool isNumeric() const noexcept;
        bool isInteger() const noexcept;
        bool isSignedInteger() const noexcept;
        bool isUnsignedInteger() const noexcept;

        bool isComparableWith(const Value& rhs) const noexcept;

        bool                asBool()   const noexcept;  //done
        int                 asInt()    const noexcept;  //done
        unsigned int        asUint()   const noexcept;  //done
        long long           asInt64()  const noexcept;  //done
        unsigned long long  asUint64() const noexcept;  //done
        double              asFloat()  const noexcept;  //done
        std::string         asString() const noexcept;  //done
        BinaryType          asBinary() const noexcept;  //done

        Value& operator = (const Value& lhs); //{ ss = lhs.ss;  return *this; }
        Value& operator = (Value&& lhs); //{ ss = std::move(lhs.ss); return *this; }

        Value& operator [] (int i);
        Value const& operator [] (int i) const;

        Value& operator [] (const char*);
        Value const& operator [] (const char*) const;

        Value& operator [] (const std::string&);
        Value const& operator [] (const std::string&) const;

        void push_back(const Value&);
        void push_back(Value&&);

        iterator begin() { return iterator(this, iterator::pos::begin); }
        iterator end() { return iterator(this, iterator::pos::end); }

        /* TODO
        const_iterator begin() const { return cbegin(); }
        const_iterator end() const { return cend(); }

        const_iterator cbegin() const { return const_iterator(const_cast<const Value*>(this), iterator::pos::begin); }
        const_iterator cend() const { return const_iterator(this, iterator::pos::end); }
        */

        operator int ();
        operator int () const;

        operator bool& () &;
        operator bool const& () const&;

        operator char& () &;
        operator char const& () const&;

        operator double& () &;
        operator double const& () const&;

        operator long long& () &;
        operator long long const& () const&;

        operator unsigned long long& () &;
        operator unsigned long long const& () const&;

        operator std::string () &&; // { std::cout << "Move called\n"; return std::move(ss); }
        operator std::string& () &; // { std::cout << "Ref called\n"; return ss; }
        operator std::string const& () const&; // { std::cout << "Const Ref called\n"; return ss; }

        operator BinaryType () &&;
        operator BinaryType& () &;
        operator BinaryType const& () const&;

        friend void swap(Value&, Value&);
        friend bool operator == (const Value&, const Value&);

    private:

        void construct_fromString(std::string&&);
        void construct_fromArray(ArrayType&&);
        void construct_fromBinary(BinaryType&&);
        void construct_fromMap(MapType&&);
        inline void destruct() noexcept;

        void move_from(Value&&);
        void copy_from(const Value&);

        ValueHolder value;
        Type vtype = Type::Null;
        friend iterator;


    };

    void swap(Value&, Value&);
    bool operator == (const Value&, const Value&);
    bool operator != (const Value&, const Value&s);

}
#endif // VALUE_H
