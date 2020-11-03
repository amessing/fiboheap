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
#include <sstream>

// local
#include "fibo_heap.hpp"

namespace fiboheap
{
    /**!
     *
     * // \brief A Fibonacci heap with an added fast store for retrieving nodes
     * and decreasing their key key
     *
     * \tparam KeyType
     * \tparam ValueType
     * \tparam Comp
     */
    template <typename KeyType, typename ValueType = void*, typename Comp = std::less<KeyType>>
    class FiboQueue : public FiboHeap<KeyType, ValueType, Comp>
    {
        using Heap = FiboHeap<KeyType, ValueType, Comp>;
        using Node = typename Heap::Node;
        using KeyNodeIter = typename std::unordered_map<KeyType, Node*>::iterator;
       public:
        FiboQueue() = default;

        KeyNodeIter find(KeyType key)
        {
            return m_fstore.find(key);
        }

        Node* findNode(KeyType key)
        {
            return find(key)->second;
        }

        bool contains(KeyType key)
        {
            return m_fstore.find(key) != m_fstore.end;
        }

        void decreaseKey(Node* x, KeyType key)
        {
            m_fstore.erase(find(x->key));
            m_fstore.insert({key, x});
            Heap::decreaseKey(x, std::move(key));
        }

        void pop()
        {
            if(Heap::empty())
            {
                return;
            }
            Node* x = Heap ::extractMin();
            auto range = m_fstore.equal_range(x->key);
            auto mit = find_if(range.first, range.second,
                               [x](const auto& element) {
                                 return element.second == x;
                               }
            );
            if(mit != range.second)
            {
                m_fstore.erase(mit);
            }
            else
            {
                std::stringstream ss;
                ss << "[Error]: key " << x->key
                      << " cannot be found in FiboQueue fast store";
                throw std::runtime_error(ss.str());
            }
            delete x;
        }

        Node* push(KeyType key, std::shared_ptr<ValueType> payload = nullptr)
        {
            Node* x = Heap::push(std::move(key), payload);
            m_fstore.insert({key, x});
            return x;
        }

        void clear()
        {
            Heap::clear();
            m_fstore.clear();
        }

       private:
        std::unordered_map<KeyType, Node*> m_fstore;
    };
}