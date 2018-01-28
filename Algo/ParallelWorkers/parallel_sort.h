#pragma once

#include "thread_pool.h"
#include <algorithm>
#include <iostream>
#include <stack>

class parallel_sort {
public:
    parallel_sort(size_t num_workers) : threadPool(num_workers) {}

    ~parallel_sort() {
        threadPool.shutdown();
    }

    template<typename RandomAccessIterator, typename Compare>
    inline void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        std::vector<typename RandomAccessIterator::value_type> buf(last - first);
        subSort(first, last, comp, buf);
    }

private:
    const static long LengthOfLinearSort = 1024;
    std::mutex mutex;

    template<typename RandomAccessIterator, typename Compare>
    void subSort(RandomAccessIterator first,
                 RandomAccessIterator last,
                 Compare compare,
                 std::vector<typename RandomAccessIterator::value_type>& buf) {

        std::stack<RandomAccessIterator> firsts;
        std::stack<std::future<void>> leftParts;

        while(last - first > LengthOfLinearSort) {
            RandomAccessIterator mid = first + (last - first) / 2;
            firsts.push(first);
            //std::cout << *first <<' ' <<*mid <<' ' << *(last-1) <<std::endl;
            //std::cout.flush();
            auto sortLeftPart(std::bind(&parallel_sort::subSort<RandomAccessIterator, Compare>,
                                        this, first, mid, compare, std::ref(buf)));
            auto leftPartSorted = threadPool.submit(sortLeftPart);
            first = mid;
            leftParts.push(std::move(leftPartSorted));
        }

        std::sort(first, last, compare);

        while(!firsts.empty()) {
            auto leftPartSorted = std::move(leftParts.top());
            leftParts.pop();
            threadPool.wait(leftPartSorted);

            mutex.lock();
            auto first = firsts.top();
            RandomAccessIterator mid = first + (last - first) / 2;

            std::merge(first, mid, mid, last, buf.begin(), compare);
            std::copy(buf.begin(), buf.begin()+(last-first), first);
            firsts.pop();
            mutex.unlock();
        }
    }

    thread_pool<void> threadPool;
};
