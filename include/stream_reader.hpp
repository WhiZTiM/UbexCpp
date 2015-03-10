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

#ifndef STREAM_READER_HPP
#define STREAM_READER_HPP

#include "stream_helpers.hpp"
#include "value.hpp"
#include <fstream>
#include <cstring>
#include <tuple>
#include <iostream>

namespace timl {

    enum class MarkerType { Object, HetroArray, HomoArray };

    constexpr ValueSizePolicy defaultStreamReaderPolicy()
    { return {32, 1024*1024*64, 1024*1024*8, 1024*1024*65, 1024, 1024}; }

    template<typename StreamType>
    class StreamReader
    {
    public:

        struct policy_violation : parsing_exception
        {
            policy_violation(const std::string& str) : parsing_exception(""), ss(str) {}
            const char* what() const noexcept { return ss.c_str(); }
            std::string ss;
        };


        StreamReader(StreamType& Stream, ValueSizePolicy policy = defaultStreamReaderPolicy());

        Value getNextValue();

        bool getNextValue(Value& v);

        StreamType& getStream() { return stream; }

        std::size_t getBytesRead() const { return bytes_so_far; }

        std::string getLastError() const { return last_error; }

    private:
        void extract_nextValue(Value &vref, size_t value_count, MarkerType type = MarkerType::Object, byte type_mark = 'n');

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

        void extract_count_and_Value(Value& v);
        void extract_count_and_HomoArray(Value& v);
        void extract_count_and_HetroArray(Value& v);
        void extract_singleValueTo(byte marker, Value& value);
        void extract_containerValueTo(byte marker, Value& value);
        void validate_container_end(MarkerType type);

        bool read(byte&);
        bool read(byte*, std::size_t);

        StreamType& stream;
        std::string last_error;
        std::size_t bytes_so_far = 0;    //! bytes so far
        size_t recursive_depth = 0;
        const ValueSizePolicy vsz;
    };

    template<typename StreamType>
    StreamReader<StreamType>::StreamReader(StreamType& Stream, ValueSizePolicy policy)
        : stream(Stream), vsz(policy)
    {}


    template<typename StreamType>
    Value StreamReader<StreamType>::getNextValue()
    {
        Value v;        getNextValue(v);        return v;
    }

    template<typename StreamType>
    bool StreamReader<StreamType>::getNextValue(Value& v)
    {
        bool good = false;

        try
        {
            bytes_so_far = 0;
            recursive_depth = 0;
            byte b;
            read(b);
            if(not isObjectStart(b))
                throw parsing_exception("Stream does not contain a valid Object - ObjectStartMarker");
            extract_count_and_Value(v);
            good = true;
        }
        catch(parsing_exception& pexecpt)
        {
            last_error = pexecpt.what();
        }
        return good;
    }

    template<typename StreamType>
    bool StreamReader<StreamType>::read(byte& b)
    {
        read(&b, 1);
        return true;
    }

    template<typename StreamType>
    bool StreamReader<StreamType>::read(byte* b, std::size_t sz)
    {
        using std::to_string;
        using std::string;

        if(bytes_so_far + sz > vsz.max_object_size)
            throw policy_violation("Maximum Object size read at: " + to_string(bytes_so_far));
        stream.read(to_cbyte(b), sz);
        bytes_so_far += sz;
        return true;
    }

    template<typename StreamType>
    void StreamReader<StreamType>::extract_nextValue(Value& vref, size_t value_count, MarkerType type, byte type_mark)
    {
        if(++recursive_depth > vsz.max_value_depth)
            throw parsing_exception("Maximum Parsing depth Exceeded!");

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
                break;
            case MarkerType::HomoArray:
                marker = type_mark;
            default:
                break;
            }

            extract_singleValueTo(marker, value);
            extract_containerValueTo(marker, value);

            switch (type) {
            case MarkerType::Object:
                vref[key] = std::move(value);
                break;
            case MarkerType::HetroArray:
            case MarkerType::HomoArray:
                vref.push_back( std::move(value) );
                break;
            }
            --value_count;
        }
        validate_container_end(type);
        --recursive_depth;
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
        else if(isInt8(marker))
        {
            int8_t extracted(extract_Uint8().first);
            long long val(extracted);
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
            extract_count_and_Value(value);
        }

        else if(isHomoArrayStart(marker))
        {
            extract_count_and_HomoArray(value);
        }

        else if(isHetroArrayStart(marker))
        {
            extract_count_and_HetroArray(value);
        }
    }


    template<typename StreamType>
    KeyMarker StreamReader<StreamType>::extract_nextKeyMarker()
    {
        KeyMarker km;
        read(km.len);
        read(km.value, std::size_t(km.len));
        read(km.marker);
        return km;
    }

    template<typename StreamType>
    std::pair<std::size_t, bool> StreamReader<StreamType>::extract_itemCount()
    {
        byte b[4];
        read(b[0]);
        if(isUint8(b[0]))
        {
            read(b, 1);
            return std::make_pair(fromBigEndian8(b), true);
        }
        if(isUint16(b[0]))
        {
            read(b, 2);
            return std::make_pair(fromBigEndian16(b), true);
        }
        if(isUint32(b[0]))
        {
            read(b, 4);
            return std::make_pair(fromBigEndian32(b), true);
        }
        return std::make_pair(std::size_t(b[0]), false);
    }

    template<typename StreamType>
    void StreamReader<StreamType>::extract_count_and_Value(Value& v)
    {
        auto icount = extract_itemCount();
        if(not icount.second)
        {
            if(isWidthMarker( static_cast<byte>(icount.first)))
            {
                auto wsize = extract_itemCount();
                if(not wsize.second)
                    throw parsing_exception("Ill formed object!");

                //! \todo TODO: Add this up
                //readHint(wsize.first);

                icount = extract_itemCount();
                if(not icount.second)
                    icount.first = 0;
            }
            else
            {
                byte marker = static_cast<byte>(icount.first);
                if(not isObjectEnd(marker))
                    throw parsing_exception("empty Object is ill-formed");
                return;
            }
        }

        extract_nextValue(v, icount.first, MarkerType::Object);
    }

    template<typename StreamType>
    void StreamReader<StreamType>::extract_count_and_HomoArray(Value& v)
    {
        byte type_mark = static_cast<byte>(extract_Uint8().first);
        auto icount = extract_itemCount();
        if(icount.second)
            extract_nextValue(v, icount.first, MarkerType::HomoArray, type_mark);

        byte marker = static_cast<byte>(icount.first);
        if(not isHomoArrayEnd(marker))
            throw parsing_exception("empty HomogenousArray is ill-formed");
    }

    template<typename StreamType>
    void StreamReader<StreamType>::extract_count_and_HetroArray(Value &v)
    {
        auto icount = extract_itemCount();
        if(icount.second )
            return extract_nextValue(v, icount.first, MarkerType::HetroArray);

        byte marker = static_cast<byte>(icount.first);
        if(not isHetroArrayEnd(marker))
            throw parsing_exception("empty HetrogenousArray is ill-formed");
    }

    template<typename StreamType>
    void StreamReader<StreamType>::validate_container_end(MarkerType type)
    {
        byte b;
        read(b);
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
        read(b, 2);
        return std::make_pair(fromBigEndian16(b), true);
    }

    template<typename StreamType>
    std::pair<int32_t, bool> StreamReader<StreamType>::extract_Int32()
    {
        byte b[4];
        read(b, 4);
        return std::make_pair(fromBigEndian32(b), true);
    }

    template<typename StreamType>
    std::pair<int64_t, bool> StreamReader<StreamType>::extract_Int64()
    {
        byte b[8];
        read(b, 8);
        return std::make_pair(fromBigEndian64(b), true);
    }

    template<typename StreamType>
    std::pair<uint16_t, bool> StreamReader<StreamType>::extract_Uint16()
    {
        byte b[2];
        read(b, 2);
        return std::make_pair(fromBigEndian16(b), true);
    }

    template<typename StreamType>
    std::pair<uint32_t, bool> StreamReader<StreamType>::extract_Uint32()
    {
        byte b[4];
        read(b, 4);
        return std::make_pair(fromBigEndian32(b), true);
    }

    template<typename StreamType>
    std::pair<uint64_t, bool> StreamReader<StreamType>::extract_Uint64()
    {
        byte b[8];
        read(b, 8);
        return std::make_pair(fromBigEndian64(b), true);
    }

    template<typename StreamType>
    std::pair<int8_t, bool> StreamReader<StreamType>::extract_Uint8()
    {
        byte b;
        read(b);
        return std::make_pair(fromBigEndian8(&b), true);
    }

    template<typename StreamType>
    std::pair<float, bool> StreamReader<StreamType>::extract_Float32()
    {
        byte b[4];
        read(b, 4);
        return std::make_pair(fromBigEndianFloat32(b), true);
    }

    template<typename StreamType>
    std::pair<double, bool> StreamReader<StreamType>::extract_Float64()
    {
        byte b[8];
        read(b, 8);
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
        read(b.get(), icount.first);

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
        read(b.get(), icount.first);

        Value::BinaryType rtn(to_cbyte(b.get()), to_cbyte(b.get()) + icount.first);
        return std::make_pair(rtn, true);
    }

    using OstreamReader = StreamReader<std::ifstream>;
}

#endif // STREAM_READER_HPP

