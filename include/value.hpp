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

/**
  * @file value.hpp
  * A generalized container object for all value types exchageable by the protocol
  *
  * @brief A generalized container object for all value types exchageable by the protocol
  * @author WhiZTiM
  * @date January, 2015
  * @version 0.0.1
  *
  * The @class Value provides a simple and initutive inteface for storage and manipulation of types
  * The class stores ints, unsigned ints, floating point numbers, strings, character, ke-value maps and arrays
  *
  * @code
  * Value v{345};
    Value ha;
    Value array = { Value("name", "Joy"), Value("id", 34) };

    cout << "NAME: " << array[0]["name"].asString() << "\nid: " << array[1]["id"].asString() << endl;

    ha["sani"] = "Mark";
    ha["musa"] = "Yusuf";
    ha["kabir"] = 4546.34;
    cout << "value: " << ha["kabir"].asString() << endl << endl;
    for(const auto& a : ha)
        cout << '\t' << a.asFloat() << endl;
    @endcode
  *
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

    /*!
     * \brief The Value class
     *
     */
    class Value
    {
    public:
        //! An alias for std::vector<std::strings> which is the Type returned by \ref keys()
        using Keys = std::vector<std::string>;

        //! A pattern type alias, i.e (std::shared_ptr<Value>)
        using Sptr = std::shared_ptr<Value>;

        //! A pattern type alias, i.e (std::unique_ptr<Value>)
        using Uptr = std::unique_ptr<Value>;

        //! An alias used to internally represent \ref Type "Array" types
        using ArrayType = std::vector<Uptr>;

        //! An alias used to internally represent \ref Type "Binary" types
        //! \note \a byte is an alias for \e unsigned \e char
        using BinaryType = std::vector<byte>;

        //! An alias used to internally represent \ref Type "Map" types
        using MapType = std::unordered_map<std::string, Uptr>;

        //! Iterator alias for accessing values of an iterable value object
        using iterator = value_iterator<Value, ArrayType::iterator, MapType::iterator>;

        //! Iterator alias for accessing values of an iterable value object
        using const_iterator = value_iterator<const Value, ArrayType::const_iterator, MapType::const_iterator>;

        friend iterator;
        friend const_iterator;


        /*!
         * \brief This is the union type that actually stores the data for Value class
         * Every Value object has exactly one instance of a ValueHolder.
         * \remarks ValueHolder is 64bytes on 64bit systems
         */
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
        Value(const std::string&, Value);
        Value(std::string);
        Value(BinaryType);
        Value(std::initializer_list<Value>);

        Value(Value&&);
        Value(const Value&);

        ~Value();

        Type type() const noexcept;

        /*!
         * \brief returns the number of items contained in this Value Object
         * \return std::size_t(1) for none Null, Array and Map types
         */
        size_t size() const noexcept;

        //! Returns whether the contained type is a (Key-Value) Map {like std::unordered_map}
        bool isMap() const noexcept;

        //! Returns whether the contained type is Null (empty)
        bool isNull() const noexcept;

        //! Returns whether the contained type is a single Character (char)
        bool isChar() const noexcept;

        //! Returns whether the contained type is a boolean (bool)
        bool isBool() const noexcept;

        //! Returns whether the contained type is a floating point type (double)
        bool isFloat() const noexcept;

        //! Returns whether the contained type is a floating point type. (double)
        bool isArray() const noexcept;

        //! The same thing as \ref isMap()
        bool isObject() const noexcept;

        //! Returns whether the contained type is a string type. (std::string)
        bool isString() const noexcept;

        //! Returns whether the contained type is \ref BinaryType "Binary"
        bool isBinary() const noexcept;

        //! Returns whether the contained type is a numeric type. ( double or {(unsigned)int/long long} )
        bool isNumeric() const noexcept;

        //! Returns whether the contained type is an integer type. ( {(unsigned)int/long long} )
        bool isInteger() const noexcept;

        //! Returns whether the contained type is an signed integer type. ( int/long long )
        bool isSignedInteger() const noexcept;

        //! Returns whether the contained type is an unsigned integer type. ( unsigned int/unsigned long long )
        bool isUnsignedInteger() const noexcept;

        /*!
         * \fn isComparableWith
         * \brief determines whether the type of \a rhs can be compared with the type of the current \ref Value object
         * \param[in] rhs
         * \return \a true if
         * \code this->type() == rhs.type() or timl::isNumeric(rhs.type()) and timl::isNumeric(this->type()) \endcode
         */
        bool isComparableWith(const Value& rhs) const noexcept;

        bool                asBool()   const noexcept;
        int                 asInt()    const noexcept;
        unsigned int        asUint()   const noexcept;
        long long           asInt64()  const noexcept;
        unsigned long long  asUint64() const noexcept;
        double              asFloat()  const noexcept;
        std::string         asString() const noexcept;
        BinaryType          asBinary() const noexcept;

        Value& operator = (const Value& lhs);
        Value& operator = (Value&& lhs);

        Value& operator [] (int i);
        Value const& operator [] (int i) const;

        Value& operator [] (const char*);
        Value const& operator [] (const char*) const;

        Value& operator [] (const std::string&);
        Value const& operator [] (const std::string&) const;

        void push_back(const Value&);
        void push_back(Value&&);

        bool contains(const Value&) const;
        void remove(const Value&);

        iterator find(const Value&);
        const_iterator find(const Value&) const;

        /*!
         * \brief retrieve all the keys for accessing this Object through operator []
         * \pre \code this->isMap() \endcode
         * \return \ref Keys containing map keys. If precondition isn't met, an empty \ref Keys "key"
         */
        Keys keys() const;

        iterator begin()
        { return iterator(this, iterator::pos::begin); }

        iterator end()
        { return iterator(this, iterator::pos::end); }

        const_iterator begin() const
        { return cbegin(); }

        const_iterator end() const
        { return cend(); }

        const_iterator cbegin() const
        { return const_iterator(this, const_iterator::pos::begin); }

        const_iterator cend() const
        { return const_iterator(this, const_iterator::pos::end); }

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

        operator std::string () &&;
        operator std::string& () &;
        operator std::string const& () const&;

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


    };

    void swap(Value&, Value&);
    bool operator == (const Value&, const Value&);
    bool operator != (const Value&, const Value&s);

}
#endif // VALUE_H
