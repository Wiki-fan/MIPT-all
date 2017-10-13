#pragma once

#include <algorithm>
#include <stack>
#include <vector>

#include "../Debug.h"
#include "ThreadPool.h"

class ParallelQuickSort {
 public:
    ParallelQuickSort(const size_t num_workers) : threadPool_(num_workers) {}

    ~ParallelQuickSort() {
        threadPool_.Shutdown();
    }

    template<typename RandomAccessIterator, typename Compare>
    inline void Sort(const RandomAccessIterator first, const RandomAccessIterator last, const Compare comp) {
        subSort(first, last, comp);
    }

 private:
    // Размер, начиная с которого будет сортировка за квадрат, но с маленькой константой
    const static long kLengthOfLinearSort = 1024;
    ThreadPool<void> threadPool_;

 private:
    template<typename RandomAccessIterator, typename Compare>
    void subSort(RandomAccessIterator first,
                 RandomAccessIterator last,
                 const Compare compare) {

        while (last - first > kLengthOfLinearSort) {
            const auto pivot = *std::next(first, std::distance(first, last) / 2);

            const RandomAccessIterator middle1 =
                std::partition(first, last, [pivot, compare](const auto& em) { return compare(em, pivot); });
            const RandomAccessIterator middle2 =
                std::partition(middle1, last, [pivot, compare](const auto& em) { return !compare(pivot, em); });

            debug(std::cout << *first << ' ' << *middle1 << ' ' << *middle2 << ' ' << *(last - 1) << std::endl;)
            const auto sortLeftPart =
                std::bind(&ParallelQuickSort::subSort<RandomAccessIterator, Compare>, this, first, middle1, compare);
            const auto leftPartSorted = threadPool_.Submit(sortLeftPart);
            first = middle2;
            threadPool_.Wait(leftPartSorted);
        }

        std::sort(first, last, compare);
    }

};
