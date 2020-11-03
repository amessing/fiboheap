/**
 * Fibonacci Heap
 * Copyright (c) 2014, Emmanuel Benazera beniz@droidnik.fr, All rights reserved.
 * Copyright (c) 2020, Andrew Messing, All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 */
#pragma once

// global
#include <memory>
#include <utility>

namespace fiboheap
{
    /**!
     * \brief Node for a FiboHeap
     *
     * \tparam KeyType The type representing the priority for this node
     * \tparam ValueType A pointer to the payload of this node
     */
    template <typename KeyType, typename ValueType=void>
    class FiboNode
    {
       public:
        FiboNode(KeyType k, std::shared_ptr<ValueType> payload)
            : key(std::move(k))
            , mark(false)
            , p(nullptr)
            , left(nullptr)
            , right(nullptr)
            , child(nullptr)
            , degree(-1)
            , payload(payload)
        {}

        KeyType key;
        bool mark;
        FiboNode *p;
        FiboNode *left;
        FiboNode *right;
        FiboNode *child;
        int degree;
        std::shared_ptr<ValueType> payload;
    };
}  // namespace fiboheap
