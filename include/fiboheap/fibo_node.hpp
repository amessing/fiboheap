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

// Global
#include <memory>
#include <utility>

namespace fiboheap
{
    /**!
     * \brief Node for a FiboHeap
     *
     * \tparam PriorityType The type used to represent the priority of this node
     * \tparam KeyType The type used for the identifier for the payload in this node
     * \tparam PayloadType The data to store with this node
     */
    template <typename PriorityType, typename KeyType, typename PayloadType>
    class FiboNode
    {
       public:
        FiboNode(PriorityType priority, KeyType k, std::shared_ptr<PayloadType> payload= nullptr)
            : priority(std::move(priority))
            , key(std::move(k))
            , payload(payload)
            , mark(false)
            , p(nullptr)
            , left(nullptr)
            , right(nullptr)
            , child(nullptr)
            , degree(-1)
        {}

        PriorityType priority;
        KeyType key;
        std::shared_ptr<PayloadType> payload;
        bool mark;
        FiboNode *p;
        FiboNode *left;
        FiboNode *right;
        FiboNode *child;
        int degree;
    };
}  // namespace fiboheap
