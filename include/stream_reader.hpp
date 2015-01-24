#ifndef STREAM_READER_HPP
#define STREAM_READER_HPP

#include "conversions.hpp"
#include "value.hpp"
#include <fstream>
#include <cstring>
#include <tuple>
#include <iostream>

namespace timl {

    using cbyte = char;
    static_assert(sizeof(cbyte) == 1, "");

    inline byte* to_byte(cbyte* b) { return reinterpret_cast<byte*>(b); }
    inline cbyte* to_cbyte(byte* b) { return reinterpret_cast<cbyte*>(b); }

    struct KeyMarker
    {
        byte len;
        byte marker;
        byte value[255];
    };

    template<typename StreamType>
    class StreamReader
    {
    public:
        StreamReader(StreamType& Stream);// : stream(Stream) {};

        Value getNextValue();
    private:
        Value extract_nextValue(size_t value_count);

        KeyMarker extract_nextKeyMarker();

        std::pair<std::size_t, bool> extract_itemCount();
        std::pair<int16_t, bool> extract_Int16();

        StreamType& stream;
    };

    template<typename StreamType>
    StreamReader<StreamType>::StreamReader(StreamType& Stream)
        : stream(Stream)
    {}


    template<typename StreamType>
    Value StreamReader<StreamType>::getNextValue()
    {
        byte b;
        stream.read(to_cbyte(&b), 1);
        if(not isObjectStart(b))
            throw parsing_exception("Stream does not contain a valid Object - ObjectStartMarker");

        auto icount = extract_itemCount();
        if(not icount.second)
            throw parsing_exception("Stream does not contain a valid Object count - Count");

        return extract_nextValue(icount.first);
    }

    template<typename StreamType>
    Value StreamReader<StreamType>::extract_nextValue(size_t value_count)
    {
        Value rtn;
        while (value_count > 0) {
            KeyMarker km = extract_nextKeyMarker();

            std::string key(to_cbyte(km.value), to_cbyte(km.value) + size_t(km.len));
            Value value;
            if(isNull(km.marker))
            {
                //
            }
            else if(isChar(km.marker))
            {
                //
            }
            else if(isTrue(km.marker))
            {
                //
            }
            else if(isFalse(km.marker))
            {
                //
            }
            else if(isInt16(km.marker))
            {
                int16_t extracted(extract_Int16().first);
                int val(extracted);
                //std::cout << "extraction: " <<
                rtn[key] = val;
            }
            else if(isInt32(km.marker))
            {
                //
            }
            else if(isInt64(km.marker))
            {
                //
            }
            else if(isUint8(km.marker))
            {
                //
            }
            else if(isUint16(km.marker))
            {
                //
            }
            else if(isUint32(km.marker))
            {
                //
            }
            else if(isUint64(km.marker))
            {
                //
            }
            else if(isFloat32(km.marker))
            {
                //
            }
            else if(isFloat64(km.marker))
            {
                //
            }
            else if(isString(km.marker))
            {
                //
            }
            else if(isBinary(km.marker))
            {
                //
            }


            else if(isObjectStart(km.marker))
            {
                //
            }
            else if(isObjectEnd(km.marker))
            {
                //
            }
            else if(isHomoArrayStart(km.marker))
            {
                //
            }
            else if(isHomoArrayEnd(km.marker))
            {
                //
            }
            else if(isHetroArrayStart(km.marker))
            {
                //
            }
            else if(isHetroArrayEnd(km.marker))
            {
                //
            }

            --value_count;
        }
        byte b;
        stream.read(to_cbyte(&b), 1);
        std::cout << "END-MARKER: " << b << std::endl;
        if(not isObjectEnd(b))
            throw parsing_exception("Object is incomplete otherwise corrupt");
        return rtn;
    }

    template<typename StreamType>
    KeyMarker StreamReader<StreamType>::extract_nextKeyMarker()
    {
        KeyMarker km;
        stream.read(to_cbyte(&km.len), 1);
        stream.read(to_cbyte(km.value), std::size_t(km.len));
        stream.read(to_cbyte(&km.marker), 1);
        return km;
    }

    template<typename StreamType>
    std::pair<std::size_t, bool> StreamReader<StreamType>::extract_itemCount()
    {
        byte b[4];
        stream.read(to_cbyte(b), 1);
        if(isUint8(b[0]))
        {
            stream.read(to_cbyte(b), 1);
            return std::make_pair(fromBigEndian8(b), true);
        }
        if(isUint16(b[0]))
        {
            stream.read(to_cbyte(b), 2);
            return std::make_pair(fromBigEndian16(b), true);
        }
        if(isUint32(b[0]))
        {
            stream.read(to_cbyte(b), 4);
            return std::make_pair(fromBigEndian32(b), true);
        }
        return std::make_pair(0, false);
    }



    template<typename StreamType>
    std::pair<int16_t, bool> StreamReader<StreamType>::extract_Int16()
    {
        byte b[2];
        stream.read(to_cbyte(b), 2);
        return std::make_pair(fromBigEndian16(b), true);
    }




    using OstreamReader = StreamReader<std::ifstream>;
}

#endif // STREAM_READER_HPP

