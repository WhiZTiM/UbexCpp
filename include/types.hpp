#ifndef TYPES_HPP
#define TYPES_HPP

namespace timl {

    using byte = unsigned char;

    enum class Type
    {
        Null,
        Char,
        Bool,
        SignedInt,
        UnsignedInt,
        Float,
        Map,
        Array,
        Binary,
        String
    };

    enum class Marker : byte
    {
        Null    = 'n',
        Char    = 'c',
        True    = 't',
        False   = 'f',

        Int16   = 'j',
        Int32   = 'k',
        Int64   = 'l',
        Uint8   = 'I',
        Uint16  = 'J',
        Uint32  = 'K',
        Uint64  = 'L',
        Float32 = 'd',
        Float64 = 'D',

        String  = 's',
        Binary  = 'b',
        Object,
        Array,
    };

    constexpr bool operator == (byte b, Marker m) { return b == static_cast<byte>(m); }

    constexpr bool isNull(byte b) { return b == Marker::Null; }
    constexpr bool isChar(byte b) { return b == Marker::Char; }
    constexpr bool isTrue(byte b) { return b == Marker::True; }
    constexpr bool isFalse(byte b) { return b == Marker::False; }
    constexpr bool isInt16(byte b) { return b == Marker::Int16; }
    constexpr bool isInt32(byte b) { return b == Marker::Int32; }
    constexpr bool isInt64(byte b) { return b == Marker::Int64; }
    constexpr bool isUint8(byte b) { return b == Marker::Uint8; }
    constexpr bool isUint16(byte b) { return b == Marker::Uint16; }
    constexpr bool isUint32(byte b) { return b == Marker::Uint32; }
    constexpr bool isUint64(byte b) { return b == Marker::Uint64; }
    constexpr bool isFloat32(byte b) { return b == Marker::Float32; }
    constexpr bool isFloat64(byte b) { return b == Marker::Float64; }
    constexpr bool isString(byte b) { return b == Marker::String; }
    constexpr bool isBinary(byte b) { return b == Marker::Binary; }


    constexpr bool isSignedNumber(byte b)
    { return isInt16(b) or isInt32(b) or isInt64(b);  }

    constexpr bool isUnsignedNumber(byte b)
    { return isUint16(b) or isUint32(b) or isUint64(b);  }

    constexpr bool isNumber(byte b)
    { return isSignedNumber(b) or isUnsignedNumber(b);  }

    constexpr bool isNone(byte b)
    { return isNull(b); }

    constexpr bool isObjectStart(byte b)
    { return b == '{';  }

    constexpr bool isHetroArrayStart(byte b)
    { return b == '[';  }

    constexpr bool isHomoArrayStart(byte b)
    { return b == '(';  }

    constexpr bool isObjectEnd(byte b)
    { return b == '}';  }

    constexpr bool isHetroArrayEnd(byte b)
    { return b == ']';  }

    constexpr bool isHomoArrayEnd(byte b)
    { return b == ')';  }

    constexpr bool requiresPayload(byte b)
    { return isObjectStart(b) or isString(b) or isBinary(b) or isHomoArrayStart(b) or isHetroArrayStart(b); }


    static_assert(sizeof(byte) == 1, "a byte must be exactly one byte(8 bits)");
}   //end namespace::timl

#endif // TYPES_HPP

