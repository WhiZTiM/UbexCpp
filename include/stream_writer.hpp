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

#ifndef STREAM_WRITER_HPP
#define STREAM_WRITER_HPP

#include "stream_helpers.hpp"
#include <fstream>
#include "value.hpp"

namespace timl {

    template<typename StreamType>
    class StreamWriter
    {
    public:
        StreamWriter(StreamType& Stream);

        std::pair<std::size_t, bool> writeValue(const Value&);
        StreamType& getStream() { return stream; }

    private:

        std::pair<size_t, bool> append_key(const std::string&);

        std::pair<size_t, bool> append_object(const Value&);
        std::pair<size_t, bool> append_null();
        std::pair<size_t, bool> append_char(char);
        std::pair<size_t, bool> append_bool(bool);
        std::pair<size_t, bool> append_float(double);
        std::pair<size_t, bool> append_signedInt(long long);
        std::pair<size_t, bool> append_unsignedInt(unsigned long long);
        std::pair<size_t, bool> append_size(std::size_t);

        std::pair<size_t, bool> append_string(const std::string&);
        std::pair<size_t, bool> append_binary(const Value::BinaryType&);
        std::pair<size_t, bool> append_array(const Value&);

        void update(const std::pair<size_t, bool>&, std::pair<size_t, bool>&);

        bool write(byte);
        bool write(byte*, std::size_t);

        StreamType& stream;
    };


    template<typename StreamType>
    StreamWriter<StreamType>::StreamWriter(StreamType& Stream)
        : stream(Stream) {}

    template<typename StreamType>
    std::pair<size_t, bool> StreamWriter<StreamType>::writeValue(const Value& value)
    {
        if(not value.isMap())
            return make_pair(0, false);
        return append_object(value);
    }

    template<typename StreamType>
    std::pair<size_t, bool> StreamWriter<StreamType>::append_object(const Value& value)
    {
        auto keys = value.keys();
        std::pair<size_t, bool> rtn(1, false);
        write('{');

        for(const auto& key : keys)
        {
            decltype(rtn) k(0, false);
            const auto& v = value[key];

            k = append_key(key);
            update(k, rtn);

            if(v.isNull())
                k = append_null();
            else if(v.isBool())
                k = append_bool(v);
            else if(v.isChar())
                k = append_char(v);
            else if(v.isSignedInteger())
                k = append_signedInt(v);
            else if(v.isUnsignedInteger())
                k = append_unsignedInt(v);
            else if(v.isFloat())
                k = append_float(v);
            else if(v.isString())
                k = append_string(v);
            else if(v.isArray())
                k = append_array(v);
            else if(v.isObject())
                k = append_object(v);

            update(k, rtn);
        }
        write('}');
        rtn.first += 1;
        return rtn;
    }

    template<typename StreamType>
    bool StreamWriter<StreamType>::write(byte b)
    {
        return write(&b, 1);
    }

    template<typename StreamType>
    bool StreamWriter<StreamType>::write(byte* b, std::size_t sz)
    {
        stream.write(to_cbyte(b), sz);
        return true;
    }

    template<typename StreamType>
    inline void StreamWriter<StreamType>::update(const std::pair<size_t, bool>& src, std::pair<size_t, bool>& dest)
    {
        dest.first += src.first;
        dest.second = dest.second && src.second;
    }

    template<typename StreamType>
    std::pair<size_t, bool> StreamWriter<StreamType>::append_null()
    {
        write(static_cast<byte>(Marker::Null));
        return std::make_pair(1, true);
    }

    template<typename StreamType>
    std::pair<size_t, bool> StreamWriter<StreamType>::append_Bool(bool b)
    {
        if(b)
            write(static_cast<byte>(Marker::True));
        else
            write(static_cast<byte>(Marker::False));
        return std::make_pair(1, true);
    }

    template<typename StreamType>
    std::pair<size_t, bool> StreamWriter<StreamType>::append_char(char c)
    {
        write(static_cast<byte>(Marker::Char));
        write(static_cast<byte>(c));
        return std::make_pair(1, true);
    }

    template<typename StreamType>
    std::pair<size_t, bool> StreamWriter<StreamType>::append_key(const std::string& key)
    {
        //! \todo Assert key.size() is less than 256 characters
        if(key.size() > 255)
            throw std::logic_error("Don't you obey invariants?");

        std::size_t key_size = key.size();
        write(static_cast<byte>(key_size));
        write(to_byte(key.c_str()), key_size);

        return std::make_pair(1 + key_size, true);
    }

}

#endif // STREAM_WRITER_HPP

