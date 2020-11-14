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
#include <sstream>

// External
#include <robin_hood.h>

// Local
#include "fiboheap/fibo_heap.hpp"

namespace fiboheap
{
    /**!
     * // \brief A Fibonacci heap with an added fast store for retrieving nodes
     * and decreasing the key's value
     *
     * \tparam KeyType The type being stored
     * \tparam PayloadType The type of the payload to associate with the key
     * \tparam Comp A comparison of the value of keys
     */
    template <typename PriorityType, typename KeyType, typename PayloadType = void, typename Comp = std::less<KeyType>>
    class FiboQueue : public FiboHeap<PriorityType, KeyType, PayloadType, Comp>
    {
        using Heap = FiboHeap<PriorityType, KeyType, PayloadType, Comp>;
       public:
        using Node = typename Heap::Node;
        using Map = robin_hood::unordered_map<KeyType, Node*>;
        using KeyNodeIter = typename Map::iterator;

        //! \brief Default Constructor
        FiboQueue() = default;

        //! \returns The iterator associated with the \p key
        KeyNodeIter find(KeyType key)
        {
            return m_fstore.find(key);
        }

        //! \returns The FiboNode associated with the \p key
        Node* findNode(KeyType key)
        {
            return find(key)->second;
        }

        //! \returns Whether the \p is in the queue
        bool contains(KeyType key)
        {
            return m_fstore.find(key) != m_fstore.end();
        }

        //! \brief Removes the top element from the queue
        void pop()
        {
            if(Heap::empty())
            {
                return;
            }
            Node* x = Heap ::extractMin();
            auto iter = m_fstore.find(x->key);
            if(iter != m_fstore.end())
            {
                m_fstore.erase(iter);
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

        //! \brief Pushes \p key onto the queue
        Node* push(PriorityType priority, KeyType key, std::shared_ptr<PayloadType> payload = nullptr)
        {
            Node* x = Heap::push(std::move(priority), std::move(key), payload);
            m_fstore.insert({key, x});
            return x;
        }

        //! \brief Clears all the elements from the queue
        void clear()
        {
            Heap::clear();
            m_fstore.clear();
        }

       private:
        Map m_fstore;
    };
}