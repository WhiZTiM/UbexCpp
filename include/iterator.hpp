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

#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>
#include "types.hpp"

namespace timl
{

    template<typename Value_Type,
             typename Array_Type,
             typename Map_Type>
    class value_iterator :
            public std::iterator<std::forward_iterator_tag, Value_Type>
    {
    public:

        enum class pos { begin, end };
        value_iterator(Value_Type* v, pos p)
            : parent(v), current(v)
        {
            switch (parent->vtype) {
            case Type::Array:
                arr_iter = (p == pos::begin) ? parent->value.Array.begin() : parent->value.Array.end();
                break;
            case Type::Map:
                map_iter = (p == pos::begin) ? parent->value.Map.begin() : parent->value.Map.end();
                break;
            default:
                break;
            }
        }

        Value_Type& operator * ()
        {   return *operator->(); }


        Value_Type* operator -> ()
        {
            switch (parent->type()) {
            case Type::Array:
                current = arr_iter->get();
                return current;
            case Type::Map:
                current = map_iter->second.get();
                return current;
            default:
                break;
            }
            return current;
        }

        value_iterator& operator ++ () //prefix
        {
            switch (parent->type()) {
            case Type::Array:
                ++arr_iter;
                break;
            case Type::Map:
                ++map_iter;
                break;
            default:
                break;
            }
            return *this;
        }

        value_iterator operator ++ (int)
        {
            auto rtn = value_iterator(*this);
            operator ++();
            return rtn;
        }

        friend bool operator == (const value_iterator& lhs, const value_iterator& rhs)
        {
            // if the iterators do not have the same parent
            if( lhs.parent != rhs.parent)
                return false;

            switch (lhs.parent->type()) {
            case Type::Array:
                return lhs.arr_iter == rhs.arr_iter;
            case Type::Map:
            {
                bool mm = lhs.map_iter == rhs.map_iter;
                return lhs.map_iter == rhs.map_iter;
            }
            default:
                break;
            }

            return true;
        }

        friend bool operator != (const value_iterator& lhs, const value_iterator& rhs)
        {   return  !(lhs == rhs); }

    private:
        Value_Type* parent;
        Value_Type* current;
        typename Map_Type::iterator map_iter;
        typename Array_Type::iterator arr_iter;
    };

}

#endif // ITERATOR_HPP

