#pragma once
class parallel_sort {
public:
    parallel_sort(size_t num_workers) : tp(num_workers) {}

    ~parallel_sort() {
        tp.shutdown();
    }

    template<typename RandomAccessIterator, typename Compare>
    inline void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
        using vect_of_elems = std::vector<typename RandomAccessIterator::value_type>;
        vect_of_elems buf(last - first);
        vect_of_elems v(last - first);
        std::copy(first, last, v.begin());
        std::copy(first, last, buf.begin());
        subSort<RandomAccessIterator, Compare>(0, v.size(), comp, buf, v);
        std::copy(v.begin(), v.end(), first);
    }
private:

    const static long LengthOfLinearSort = 6;

    template<typename RandomAccessIterator, typename Compare>
    void subSort(size_t first,
                 size_t last,
                 Compare compare,
                 std::vector<typename RandomAccessIterator::value_type>& buf,
                 std::vector<typename RandomAccessIterator::value_type>& v) {

        if (last - first < LengthOfLinearSort) {
            std::sort(v.begin()+first, v.begin()+last, compare);
            return;
        }
        size_t mid = first + (last - first) / 2;

        auto sortLeftPart(std::bind(&parallel_sort::subSort<RandomAccessIterator, Compare>, this, first, mid, compare, std::ref(buf), std::ref(v)));
        auto sortRightPart(std::bind(&parallel_sort::subSort<RandomAccessIterator, Compare>, this, mid, last, compare, std::ref(buf), std::ref(v)));

        sortRightPart();
        auto leftPartSorted = tp.submit(sortLeftPart);

//sortLeftPart();
        tp.wait(leftPartSorted);

        std::merge(v.begin()+first, v.begin()+mid, v.begin()+mid, v.begin()+last, buf.begin() + first);
//std::swap_ranges(first, last, buf.begin());
        std::swap(v, buf);

    }

    thread_pool<void> tp;
};

