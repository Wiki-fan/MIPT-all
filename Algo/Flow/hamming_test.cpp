#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>
#include <boost/format.hpp>
#include <iostream>
#include <fstream>
#include "Network.h"
#include "hamming.h"

BOOST_AUTO_TEST_CASE(HammingTest) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    for (int i = 0; i <= 2; ++i) {
        std::ifstream ifs((boost::format("tests/inHamming%d.txt") % i).str(), std::ios_base::in);
        std::streambuf* cinbuf = std::cin.rdbuf();
        std::cin.rdbuf(ifs.rdbuf());

        HammingOptimizer<uint, int, int> hamming;
        hamming.solve();

        std::cin.rdbuf(cinbuf);
    }

}