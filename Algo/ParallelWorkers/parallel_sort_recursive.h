#pragma once
#include "thread_pool.h"
#include <algorithm>
#include <iostream>

class parallel_sort {
public:
    parallel_sort(size_t num_workers) : threadPool(num_workers) {}

    ~parallel_sort() {
        threadPool.shutdown();
    }

    template<typename RandomAccessIterator, typename Compare>
    inline void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        std::vector<typename RandomAccessIterator::value_type> buf(last - first);
        std::copy(first, last, buf.begin());
        subSort(first, last, comp, buf);
    }

private:
    std::mutex mutex;
    const static long LengthOfLinearSort = 2048;

    template<typename RandomAccessIterator, typename Compare>
    void subSort(RandomAccessIterator first,
                 RandomAccessIterator last,
                 Compare compare,
                 std::vector<typename RandomAccessIterator::value_type>& buf) {

        if (last - first < LengthOfLinearSort) {
            std::sort(first, last, compare);
            return;
        }
        RandomAccessIterator mid = first + (last - first) / 2;
        //std::cout << *first <<' ' <<*mid <<' ' << *(last-1) <<std::endl;
        //std::cout.flush();
        auto sortLeftPart(std::bind(&parallel_sort::subSort<RandomAccessIterator, Compare>,
        this, first, mid, compare, std::ref(buf)));
        auto sortRightPart(std::bind(&parallel_sort::subSort<RandomAccessIterator, Compare>,
        this, mid, last, compare, std::ref(buf)));

        auto leftPartSorted = threadPool.submit(sortLeftPart);
        sortRightPart();
        threadPool.wait(leftPartSorted);

        mutex.lock();
        std::merge(first, mid, mid, last, buf.begin());
        std::swap_ranges(first, last, buf.begin());
        mutex.unlock();

    }

    thread_pool<void> threadPool;
};
