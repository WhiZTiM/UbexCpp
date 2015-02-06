#ifndef CONVERSIONS_HPP
#define CONVERSIONS_HPP

#include <cstdint>
#include <cstring>
#include <endian.h>
#include <limits>
#include "types.hpp"

namespace timl{

    using cbyte = char;
    static_assert(sizeof(cbyte) == 1, "");
    static_assert(sizeof(std::uint16_t) == 2, "");
    static_assert(sizeof(std::uint32_t) == 4, "");
    static_assert(sizeof(std::uint64_t) == 8, "");
    static_assert(sizeof(float) == 4, "");
    static_assert(sizeof(double) == 8, "");

    inline byte* to_byte(cbyte* b) { return reinterpret_cast<byte*>(b); }
    inline cbyte* to_cbyte(byte* b) { return reinterpret_cast<cbyte*>(b); }

    struct KeyMarker
    {
        byte len;
        byte marker;
        byte value[256];
    };

    inline bool in_range(double value, double min, double max)
    { return (min <= value and value <= max); }


    uint16_t toBigEndian16(uint16_t val)
    {  return htobe16(val); }

    uint32_t toBigEndian32(uint32_t val)
    {  return htobe32(val); }

    uint64_t toBigEndian64(uint64_t val)
    {  return htobe64(val); }

    uint32_t toBigEndianFloat32(float val)
    {
        uint32_t rtn;
        std::memcpy(&rtn, &val, sizeof(rtn));
        return toBigEndian32( rtn );
    }

    uint64_t toBigEndianFloat64(double val)
    {
        uint64_t rtn;
        std::memcpy(&rtn, &val, sizeof(rtn));
        return toBigEndian64( rtn );
    }



    uint16_t fromBigEndian16(uint16_t val)
    {  return be16toh(val); }

    uint32_t fromBigEndian32(uint32_t val)
    {  return be32toh(val); }

    uint64_t fromBigEndian64(uint64_t val)
    {  return be64toh(val); }

    float fromBigEndianFloat32(uint32_t val)
    {
        float rtn;
        val = fromBigEndian32(val);
        std::memcpy(&rtn, &val, sizeof(rtn));
        return rtn;
    }

    double fromBigEndianFloat64(uint64_t val)
    {
        double rtn;
        val = fromBigEndian64(val);
        std::memcpy(&rtn, &val, sizeof(rtn));
        return rtn;
    }



    ////////////////////////////////////
    ///
    ///
    ///
    ///////////////////////////////////////


    uint8_t fromBigEndian8(byte* b)
    {
        return *b;
    }

    uint16_t fromBigEndian16(byte* b)
    {
        uint16_t rtn;
        std::memcpy(&rtn, b, 2);
        return fromBigEndian16(rtn);
    }

    uint32_t fromBigEndian32(byte* b)
    {
        uint32_t rtn;
        std::memcpy(&rtn, b, 4);
        return fromBigEndian32(rtn);
    }

    uint64_t fromBigEndian64(byte* b)
    {
        uint64_t rtn;
        std::memcpy(&rtn, b, 8);
        return fromBigEndian64(rtn);
    }

    float fromBigEndianFloat32(byte* b)
    { return static_cast<float>(fromBigEndian32(b)); }

    double fromBigEndianFloat64(byte* b)
    { return static_cast<double>(fromBigEndian64(b)); }

}

#endif // CONVERSIONS_HPP

