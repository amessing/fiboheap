/**
 * Copyright (c) 2014, Emmanuel Benazera beniz@droidnik.fr, All rights reserved.
 * Copyright (c) 2020, Andrew Messing, All rights reserved
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
// global
#include <assert.h>
#include <queue>
#include <stdlib.h>

// lib
#include "fiboheap/fibo_heap.hpp"
#include "fiboheap/fibo_queue.hpp"

struct lowerI
{
    bool operator()(const int &d1, const int &d2)
    {
        return d1 > d2;
    }
};

void fillHeaps(fiboheap::FiboHeap<int, int> &fh, std::priority_queue<int, std::vector<int>, lowerI> &pqueue, const int &n)
{
    for(int i = 0; i < n; i++)
    {
        int r = rand();
        fh.push(r, r);
        pqueue.push(r);
    }
    assert(fh.size() == n);
    assert(pqueue.size() == n);
}

void matchHeaps(fiboheap::FiboHeap<int, int> &fh, std::priority_queue<int, std::vector<int>, lowerI> &pqueue)
{
    while(!pqueue.empty())
    {
        int i1 = pqueue.top();
        int i2 = fh.top();
        assert(i1 == i2);
        pqueue.pop();
        fh.pop();
    }
    assert(fh.empty());
}

void fillQueues(fiboheap::FiboQueue<int, int> &fh, std::priority_queue<int, std::vector<int>, lowerI> &pqueue, const int &n)
{
    for(int i = 0; i < n; i++)
    {
        int r = rand();
        fh.push(r, r);
        pqueue.push(r);
    }
    assert(fh.size() == n);
    assert(pqueue.size() == n);
}

void matchQueues(fiboheap::FiboQueue<int, int> &fh, std::priority_queue<int, std::vector<int>, lowerI> &pqueue)
{
    while(!pqueue.empty())
    {
        int i1 = pqueue.top();
        int i2 = fh.top();
        assert(i1 == i2);
        pqueue.pop();
        fh.pop();
    }
    assert(fh.empty());
}

int main(int argc, char *argv[])
{
    fiboheap::FiboHeap<int, int> fh;
    int n = 10;
    std::priority_queue<int, std::vector<int>, lowerI> pqueue;

    // srand(time(0));

    fillHeaps(fh, pqueue, n);
    matchHeaps(fh, pqueue);

    fillHeaps(fh, pqueue, n);
    int r = pqueue.top() - 1;
    pqueue.pop();
    pqueue.push(r, r);
    std::make_heap(const_cast<int *>(&pqueue.top()), const_cast<int *>(&pqueue.top()) + pqueue.size(), lowerI());
    fh.decreasePriority(fh.topNode(), r);
    matchHeaps(fh, pqueue);

    fiboheap::FiboQueue<int, int> fq;
    fillQueues(fq, pqueue, n);
    matchQueues(fq, pqueue);

    fillQueues(fq, pqueue, n);
    r = rand();
    fq.push(r);
    auto *x = fq.findNode(r);
    assert(x != nullptr);
    int nr = r - rand() / 2;
    fq.decreasePriority(x, nr);
    pqueue.push(nr);
    matchQueues(fq, pqueue);
}
