#include "parallel_sort.h"
#include <functional>
#include <iostream>

int main()
{
    std::vector<int> v(10000);
    for (size_t i = 0; i<v.size(); ++i) {
        v[i] = i;
    }
    std::random_shuffle(v.begin(), v.end());
    parallel_sort s(4);
    s.sort(v.begin(), v.end(), std::less_equal<int>());

    for (size_t i = 0; i<v.size(); ++i) {
        if (v[i] != static_cast<int> (i)) {
            std::cout <<"Wrong" <<i <<std::endl;
        }
    }

    return 0;
}