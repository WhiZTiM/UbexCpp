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
        byte value[256];
    };

    enum class MarkerType { Object, HetroArray, HomoArray };
    template<typename StreamType>
    class StreamReader
    {
    public:
        StreamReader(StreamType& Stream);// : stream(Stream) {};

        Value getNextValue();
    private:
        Value extract_nextValue(size_t value_count, MarkerType type = MarkerType::Object, byte type_mark = 'n');

        KeyMarker extract_nextKeyMarker();

        std::pair<std::size_t, bool> extract_itemCount();
        std::pair<int8_t, bool> extract_Uint8();
        std::pair<int16_t, bool> extract_Int16();
        std::pair<int32_t, bool> extract_Int32();
        std::pair<int64_t, bool> extract_Int64();
        std::pair<uint16_t, bool> extract_Uint16();
        std::pair<uint32_t, bool> extract_Uint32();
        std::pair<uint64_t, bool> extract_Uint64();
        std::pair<float, bool> extract_Float32();
        std::pair<double, bool> extract_Float64();
        std::pair<std::string, bool> extract_String();
        std::pair<Value::BinaryType, bool> extract_Binary();

        Value extract_count_and_Value();
        Value extract_count_and_HomoArray();
        Value extract_count_and_HetroArray();
        void extract_singleValueTo(byte marker, Value& value);
        void extract_containerValueTo(byte marker, Value& value);
        void validate_container_end(MarkerType type);

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

        return extract_count_and_Value();
    }

    template<typename StreamType>
    Value StreamReader<StreamType>::extract_nextValue(size_t value_count, MarkerType type, byte type_mark)
    {
        Value rtn;
        decltype(KeyMarker::marker) marker;
        std::string key;
        KeyMarker km;
        while (value_count > 0) {

            Value value;
            switch (type) {
            case MarkerType::Object:
                km = extract_nextKeyMarker();
                marker = km.marker;
                key = std::string(to_cbyte(km.value), to_cbyte(km.value) + size_t(km.len));
                break;
            case MarkerType::HetroArray:
                marker = extract_Uint8().first;
            case MarkerType::HomoArray:
                marker = type_mark;
            default:
                break;
            }

            extract_singleValueTo(marker, value);
            extract_containerValueTo(marker, value);

            switch (type) {
            case MarkerType::Object:
                rtn[key] = std::move(value);
                break;
            case MarkerType::HetroArray:
            case MarkerType::HomoArray:
                rtn[key].push_back( std::move(value) );
                break;
            }
            --value_count;
        }
        validate_container_end(type);
        return rtn;
    }


    template<typename StreamType>
    void StreamReader<StreamType>::extract_singleValueTo(byte marker, Value& value)
    {
        if(isNull(marker))
        {
            value = Value();
        }
        else if(isTrue(marker))
        {
            value = true;
        }
        else if(isFalse(marker))
        {
            value = false;
        }
        else if(isChar(marker))
        {
            uint8_t extracted(extract_Uint8().first);
            char val(extracted);
            value = val;
        }
        else if(isUint8(marker))
        {
            uint8_t extracted(extract_Uint8().first);
            unsigned long long val(extracted);
            value = val;
        }
        else if(isInt16(marker))
        {
            int16_t extracted(extract_Int16().first);
            int val(extracted);
            value = val;
        }
        else if(isInt32(marker))
        {
            int32_t extracted(extract_Int32().first);
            int val(extracted);
            value = val;
        }
        else if(isInt64(marker))
        {
            int64_t extracted(extract_Int64().first);
            long long val(extracted);
            value = val;
        }
        else if(isUint16(marker))
        {
            uint16_t extracted(extract_Uint16().first);
            unsigned long long val(extracted);
            value = val;
        }
        else if(isUint32(marker))
        {
            uint32_t extracted(extract_Uint32().first);
            unsigned long long val(extracted);
            value = val;
        }
        else if(isUint64(marker))
        {
            uint64_t extracted(extract_Uint64().first);
            unsigned long long val(extracted);
            value = val;
        }
        else if(isFloat32(marker))
        {
            float extracted(extract_Float32().first);
            double val(extracted);
            value = val;
        }
        else if(isFloat64(marker))
        {
            double extracted(extract_Float64().first);
            double val(extracted);
            value = val;
        }
        else if(isString(marker))
        {
            value = extract_String().first;
        }
        else if(isBinary(marker))
        {
            value = extract_Binary().first;
        }
    }

    template<typename StreamType>
    void StreamReader<StreamType>::extract_containerValueTo(byte marker, Value &value)
    {
        if(isObjectStart(marker))
        {
            value.push_back(extract_count_and_Value());
        }

        else if(isHomoArrayStart(marker))
        {
            value.push_back(extract_count_and_HomoArray());
        }

        else if(isHetroArrayStart(marker))
        {
            value.push_back(extract_count_and_HetroArray());
        }
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
    Value StreamReader<StreamType>::extract_count_and_Value()
    {
        auto icount = extract_itemCount();
        if(not icount.second)
            throw parsing_exception("Stream does not contain a valid Object count - Count");

        return extract_nextValue(icount.first);
    }

    template<typename StreamType>
    Value StreamReader<StreamType>::extract_count_and_HomoArray()
    {
        byte type_mark = static_cast<byte>(extract_Uint8().first);
        auto icount = extract_itemCount();
        if(not icount.second)
            throw parsing_exception("Stream does not contain a valid Object count - Count");

        return extract_nextValue(icount.first, MarkerType::HomoArray, type_mark);
    }

    template<typename StreamType>
    Value StreamReader<StreamType>::extract_count_and_HetroArray()
    {
        auto icount = extract_itemCount();
        if(not icount.second)
            throw parsing_exception("Stream does not contain a valid Object count - Count");

        return extract_nextValue(icount.first, MarkerType::HetroArray);
    }

    template<typename StreamType>
    void StreamReader<StreamType>::validate_container_end(MarkerType type)
    {
        byte b;
        stream.read(to_cbyte(&b), 1);
        std::cout << "END-MARKER: " << b << std::endl;
        switch (type) {
        case MarkerType::Object:
            if(not isObjectEnd(b))
                throw parsing_exception("Object is incomplete otherwise corrupt");
            break;
        case MarkerType::HetroArray:
            if(not isHetroArrayEnd(b))
                throw parsing_exception("Object is incomplete otherwise corrupt");
            break;
        case MarkerType::HomoArray:
            if(not isHomoArrayEnd(b))
                throw parsing_exception("Object is incomplete otherwise corrupt");
        default:
            break;
        }
    }



    template<typename StreamType>
    std::pair<int16_t, bool> StreamReader<StreamType>::extract_Int16()
    {
        byte b[2];
        stream.read(to_cbyte(b), 2);
        return std::make_pair(fromBigEndian16(b), true);
    }

    template<typename StreamType>
    std::pair<int32_t, bool> StreamReader<StreamType>::extract_Int32()
    {
        byte b[4];
        stream.read(to_cbyte(b), 4);
        return std::make_pair(fromBigEndian32(b), true);
    }

    template<typename StreamType>
    std::pair<int64_t, bool> StreamReader<StreamType>::extract_Int64()
    {
        byte b[8];
        stream.read(to_cbyte(b), 8);
        return std::make_pair(fromBigEndian64(b), true);
    }

    template<typename StreamType>
    std::pair<uint16_t, bool> StreamReader<StreamType>::extract_Uint16()
    {
        byte b[2];
        stream.read(to_cbyte(b), 2);
        return std::make_pair(fromBigEndian16(b), true);
    }

    template<typename StreamType>
    std::pair<uint32_t, bool> StreamReader<StreamType>::extract_Uint32()
    {
        byte b[4];
        stream.read(to_cbyte(b), 4);
        return std::make_pair(fromBigEndian32(b), true);
    }

    template<typename StreamType>
    std::pair<uint64_t, bool> StreamReader<StreamType>::extract_Uint64()
    {
        byte b[8];
        stream.read(to_cbyte(b), 8);
        return std::make_pair(fromBigEndian64(b), true);
    }

    template<typename StreamType>
    std::pair<int8_t, bool> StreamReader<StreamType>::extract_Uint8()
    {
        byte b;
        stream.read(to_cbyte(&b), 1);
        return std::make_pair(fromBigEndian8(&b), true);
    }

    template<typename StreamType>
    std::pair<float, bool> StreamReader<StreamType>::extract_Float32()
    {
        byte b[4];
        stream.read(to_cbyte(b), 4);
        return std::make_pair(fromBigEndianFloat32(b), true);
    }

    template<typename StreamType>
    std::pair<double, bool> StreamReader<StreamType>::extract_Float64()
    {
        byte b[8];
        stream.read(to_cbyte(b), 8);
        return std::make_pair(fromBigEndianFloat64(b), true);
    }

    template<typename StreamType>
    std::pair<std::string, bool> StreamReader<StreamType>::extract_String()
    {
        auto icount = extract_itemCount();
        if(not icount.second)
            throw parsing_exception("Invalid count token encounted!");

        //byte* b = new byte[icount.first];
        std::unique_ptr<byte[]> b(new byte[icount.first]);
        stream.read(to_cbyte(b.get()), icount.first);

        std::string rtn(to_cbyte(b.get()), to_cbyte(b.get()) + icount.first);
        return std::make_pair(rtn, true);
    }

    template<typename StreamType>
    std::pair<Value::BinaryType, bool> StreamReader<StreamType>::extract_Binary()
    {
        auto icount = extract_itemCount();
        if(not icount.second)
            throw parsing_exception("Invalid count token encounted!");

        std::unique_ptr<byte[]> b(new byte[icount.first]);
        stream.read(to_cbyte(b.get()), icount.first);

        Value::BinaryType rtn(to_cbyte(b.get()), to_cbyte(b.get()) + icount.first);
        return std::make_pair(rtn, true);
    }

    using OstreamReader = StreamReader<std::ifstream>;
}

#endif // STREAM_READER_HPP

