#ifndef VALUE_H
#define VALUE_H

#include <memory>
#include <string>
#include <vector>
#include <numeric>
#include <unordered_map>
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


        explicit Value(bool);
        explicit Value(char);
        explicit Value(unsigned long long);
        Value();
        Value(double);
        Value(long long);
        Value(BinaryType);
        Value(std::string);

        Value(Value&&);
        Value(const Value&);

        ~Value();

        Type type() const;
        size_t size() const;

        bool isMap() const;
        bool isChar() const;
        bool isBool() const;
        bool isArray() const;
        bool isObject() const;
        bool isString() const;
        bool isBinary() const;
        bool isNumber() const;


        bool isUnsignedNumber() const;
        bool isSignedNumber() const;
        bool isInteger() const;
        bool isFloat() const;

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

        Value& operator = (const Value& lhs); //{ ss = lhs.ss;  return *this; }
        Value& operator = (Value&& lhs); //{ ss = std::move(lhs.ss); return *this; }

        Value& operator [] (size_t i);
        Value const& operator [] (size_t i) const;

        Value& operator [] (const std::string&);
        Value const& operator [] (const std::string&) const;

        iterator begin() { return iterator(this, iterator::pos::begin); }

        friend void swap(Value&, Value&);

    private:

        void construct_fromString(std::string&&);
        void construct_fromArray(ArrayType&&);
        void construct_fromBinary(BinaryType&&);
        void construct_fromMap(MapType&&);
        inline void destruct() noexcept;

        void move_from(Value&&);
        void copy_from(const Value&);

        ValueHolder value;
        Type vtype;
        friend iterator;


    };

    void swap(Value&, Value&);

}
#endif // VALUE_H
