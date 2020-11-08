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
#include <cmath>
#include <functional>
#include <vector>

// Local
#include "fiboheap/fibo_node.hpp"

namespace fiboheap
{
    template <typename PriorityType, typename KeyType, typename PayloadType = void, typename Comparator = std::less<PriorityType>>
    class FiboHeap
    {
       protected:
        using Node = FiboNode<PriorityType, KeyType, PayloadType>;

       public:
        //! \brief Default Constructor
        FiboHeap()
            : m_n(0)
            , m_min(nullptr)
        {}

        //! \brief Destructor
        ~FiboHeap() noexcept
        {
            deleteNodes(m_min);
        }

        //! \returns If the heap has no elements
        bool empty() const noexcept
        {
            return m_n == 0;
        }

        //! \returns The number of elements in the heap
        size_t size() const noexcept
        {
            return m_n;
        }

        //! \returns The minimum node of the heap
        Node* minimum() const
        {
            return m_min;
        }

        //! \returns The node at the top of the tree
        Node* topNode() const
        {
            return minimum();
        }

        //! Removes the minimum element
        void pop()
        {
            if(empty())
            {
                return;
            }
            if(Node* x = extractMin(); x != nullptr)
            {
                delete x;
            }
        }

        /**!
         * \brief Inserts a node into the Heap
         *
         * insert(x)
         * 1. x.degree = 0
         * 2. x.p = NIL
         * 3. x.child = NIL
         * 4. x.mark = FALSE
         * 5. if H.min == NIL
         * 6. 	create a root list for H containing just x
         * 7. 	H.min = x
         * 8. else insert x into H's root list
         * 9. 	if x.key < H.min.key
         *10. 		H.min = x
         *11. H.n = H.n + 1
         */
        void insert(Node* x)
        {
            // 1
            x->degree = 0;
            // 2, 3
            x->child = x->p = nullptr;
            // 4
            x->mark = false;
            if(m_min == nullptr)
            {
                // 6,7
                m_min = x->left = x->right = x;
            }
            else
            {
                // 8
                m_min->left->right = x;
                x->left            = m_min->left;
                m_min->left        = x;
                x->right           = m_min;
                // 9
                if(m_comp(x->priority, m_min->priority))
                {
                    // 10
                    m_min = x;
                }
            }
            // 11
            ++ m_n;
        }

        /**!
         * \brief Add a node to the heap
         *
         * \param priority Value representing the significance of the stored payload
         * \param key Value identifying the stored payload
         * \param payload Data to put into the Heap
         *
         * \returns A pointer to the HeapNode that was recently added to the Heap
         */
        Node* push(PriorityType priority, KeyType key, std::shared_ptr<PayloadType> payload = nullptr)
        {
            Node* x = new Node(std::move(priority), std::move(key), payload);
            insert(x);
            return x;
        }

        /**!
         * \brief
         *
         * union_fibheap(H1,H2)
         * 1. H = MAKE-FIB-HEAP()
         * 2. H.min = H1.min
         * 3. concatenate the root list of H2 with the root list of H
         * 4. if (H1.min == NIL) or (H2.min != NIL and H2.min.key < H1.min.key)
         * 5. 	H.min = H2.min
         * 6. H.n = H1.n + H2.n
         * 7. return H
         */
        static FiboHeap* unionFiboHeap(FiboHeap* h1, FiboHeap* h2)
        {
            // 1
            FiboHeap* h = new FiboHeap();
            // 2
            h->m_min = h1->m_min;
            // 3
            if(h->min != nullptr && h2->min != nullptr)
            {
                h->min->right->left  = h2->min->left;
                h2->min->left->right = h->min->right;
                h->min->right        = h2->min;
                h2->min->left        = h->min;
            }
            // 4
            if(h1->min == nullptr || (h2->min != nullptr && h1->m_comp(h2->min->priority, h1->min->priority)))
            {
                h->m_min = h2->m_min;
            }
            h->m_n = h1->m_n + h2->m_n;
            return h;
        }

        //! \brief Set to infinity so that it hits the top of the heap, then easily remove
        void removeNode(Node* x)
        {
            decreasePriority(x, std::numeric_limits<PriorityType>::min());
            delete extractMin();
        }

        /**!
         * \brief Decrease the priority of a node in the heap
         *
         * decrease_key(x,k)
         * 1. if k > x.key
         * 2. 	error "new key is greater than current key"
         * 3. x.key = k
         * 4. y = x.p
         * 5. if y != NIL and x.key < y.key
         * 6. 	CUT(H,x,y)
         * 7. 	CASCADING-CUT(H,y)
         * 8. if x.key < H.min.key
         * 9. 	H.min = x
         */
        void decreasePriority(Node* x, PriorityType new_priority)
        {
            try
            {
                // 1
                if(m_comp(x->priority, new_priority))
                {
                    // 2
                    throw std::out_of_range("new priority is greater than current priority");
                }
            }
            catch(std::out_of_range& e)
            {
                e.what();
                return;
            }
            // 3
            x->priority = std::move(new_priority);
            // 4, 5
            if(Node* y = x->p; y != nullptr && m_comp(x->priority, y->priority))
            {
                // 6
                cut(x, y);
                // 7
                cascadingCut(y);
            }
            // 8
            if(m_comp(x->priority, m_min->priority))
            {
                // 9
                m_min = x;
            }
        }

       protected:
        /*
         * extract_min
         * 1. z = H.min
         * 2. if z != NIL
         * 3. 	for each child x of z
         * 4. 		add x to the root list of H
         * 5. 		x.p = NIL
         * 6. 	remove z from the root list of H
         * 7.	if z == z.right
         * 8. 		H.min = NIL
         * 9. 	else H.min = z.right
         *10. 		CONSOLIDATE(H)ValueType
         *11. 	H.n = H.n - 1
         *12. return z
         */
        Node* extractMin()
        {
            Node *x, *next;
            Node** childList;

            // 1
            Node* z = m_min;
            // 2
            if(z != nullptr)
            {
                // 3
                x = z->child;
                if(x != nullptr)
                {
                    childList = new Node*[z->degree];
                    next      = x;
                    for(int i = 0; i < (int)z->degree; ++i)
                    {
                        childList[i] = next;
                        next         = next->right;
                    }
                    for(int i = 0; i < (int)z->degree; ++i)
                    {
                        x = childList[i];
                        // 4
                        m_min->left->right = x;
                        x->left            = m_min->left;
                        m_min->left        = x;
                        x->right           = m_min;
                        // 5
                        x->p = nullptr;
                    }
                    delete[] childList;
                }
                // 6
                z->left->right = z->right;
                z->right->left = z->left;
                // 7
                if(z == z->right)
                {
                    // 8
                    m_min = nullptr;
                }
                else
                {
                    // 9
                    m_min = z->right;
                    // 10
                    consolidate();
                }
                // 11
                --m_n;
            }
            // 12
            return z;
        }

        /*
         * fib_heap_link(y,x)
         * 1. remove y from the root list of heap
         * 2. make y a child of x, incrementing x.degree
         * 3. y.mark = FALSE
         */
        void fibHeapLink(Node* y, Node* x)
        {
            // 1
            y->left->right = y->right;
            y->right->left = y->left;
            // 2
            if(x->child != nullptr)
            {
                x->child->left->right = y;
                y->left               = x->child->left;
                x->child->left        = y;
                y->right              = x->child;
            }
            else
            {
                y->left = y->right = x->child = y;
            }
            y->p = x;
            ++x->degree;
            // 3
            y->mark = false;
        }

        void deleteNodes(Node* x)
        {
            if(x == nullptr)
            {
                return;
            }
            Node* cur = x;
            while(true)
            {
                if(cur->left != nullptr && cur->left != x)
                {
                    Node* tmp = cur;
                    cur       = cur->left;
                    if(tmp->child)
                    {
                        deleteNodes(tmp->child);
                    }
                    delete tmp;
                }
                else
                {
                    if(cur->child)
                    {
                        deleteNodes(cur->child);
                    }
                    delete cur;
                    break;
                }
            }
        }

        /*
         * consolidate
         * 1. let A[0 . . D(H.n)] be a new array
         * 2. for i = 0 to D(H.n)
         * 3. 	A[i] = NIL
         * 4. for each node w in the root list of H
         * 5. 	x = w
         * 6. 	d = x.degree
         * 7. 	while A[d] != NIL
         * 8. 		y = A[d]
         * 9. 		if x.key > y.key
         *10.			exchange x with y
         *11. 		FIB-HEAP-LINK(H,y,x)
         *12. 		A[d] = NIL
         *13. 		d = d + 1
         *14. 	A[d] = x
         *15. H.min = NIL
         *16. for i = 0 to D(H.n)
         *17. 	if A[i] != NIL
         *18. 		if H.min == NIL
         *19. 			create a root list for H containing just A[i]
         *20. 			H.min = A[i]
         *21. 		else insert A[i] into H's root list
         *22. 			if A[i].key < H.min.key
         *23. 				H.min = A[i]
         */
        void consolidate()
        {
            // Max degree <= log base golden ratio of n
            int max_degree = static_cast<int>(
                floor(log(static_cast<double>(m_n)) / log(static_cast<double>(1 + sqrt(static_cast<double>(5))) / 2)));

            // 1, 2, 3
            std::vector<Node*> A(max_degree + 2, nullptr);  // plus two both for indexing to max degree and so A[max_degree+1] == NIL
            // 4
            Node* w        = m_min;
            int root_size  = 0;
            Node* next     = w;
            do
            {
                ++root_size;
                next = next->right;
            } while(next != w);
            std::vector<Node*> root_list(root_size);
            for(int i = 0; i < root_size; ++i)
            {
                root_list[i] = next;
                next        = next->right;
            }
            for(int i = 0; i < root_size; ++i)
            {
                // 5
                Node* x = w = root_list[i];
                // 6
                int degree = x->degree;
                // 7
                while(A[degree] != nullptr)
                {
                    // 8
                    Node* y = A[degree];
                    // 9
                    if(m_comp(y->priority, x->priority))
                    {
                        // 10
                        std::swap(x, y);
                    }
                    // 11
                    fibHeapLink(y, x);
                    // 12
                    A[degree] = nullptr;
                    // 13
                    ++ degree;
                }
                // 14
                A[degree] = x;
            }
            // 15
            m_min = nullptr;
            // 16
            for(int i = 0; i < max_degree + 2; ++ i)
            {
                // 17
                if(A[i] != nullptr)
                {
                    // 18
                    if(m_min == nullptr)
                    {
                        // 19, 20
                        m_min = A[i]->left = A[i]->right = A[i];
                    }
                    else
                    {
                        // 21
                        m_min->left->right = A[i];
                        A[i]->left         = m_min->left;
                        m_min->left        = A[i];
                        A[i]->right        = m_min;
                        // 22
                        if(m_comp(A[i]->priority, m_min->priority))
                        {
                            // 23
                            m_min = A[i];
                        }
                    }
                }
            }
        }

        /*
         * cut(x,y)
         * 1. remove x from the child list of y, decrementing y.degree
         * 2. add x to the root list of H
         * 3. x.p = NIL
         * 4. x.mark = FALSE
         */
        void cut(Node* x, Node* y)
        {
            // 1
            if(x->right == x)
            {
                y->child = nullptr;
            }
            else
            {
                x->right->left = x->left;
                x->left->right = x->right;
                if(y->child == x)
                {
                    y->child = x->right;
                }
            }
            -- y->degree;
            // 2
            m_min->right->left = x;
            x->right           = m_min->right;
            m_min->right       = x;
            x->left            = m_min;
            // 3
            x->p = nullptr;
            // 4
            x->mark = false;
        }

        /*
         * cascading_cut(y)
         * 1. z = y.p
         * 2. if z != NIL
         * 3. 	if y.mark == FALSE
         * 4. 		y.mark = TRUE
         * 5. 	else CUT(H,y,z)
         * 6. 		CASCADING-CUT(H,z)
         */
        void cascadingCut(Node* y)
        {
            // 1, 2
            if(Node* z = y->p; z != nullptr)
            {
                // 3
                if(!y->mark)
                {
                    // 4
                    y->mark = true;
                }
                else
                {
                    // 5
                    cut(y, z);
                    // 6
                    cascadingCut(z);
                }
            }
        }

        size_t m_n;
        Node* m_min;
        Comparator m_comp;
    };
}  // namespace fiboheap