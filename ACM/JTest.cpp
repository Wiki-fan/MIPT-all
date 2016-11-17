#define BOOST_TEST_MODULE JTest
#include <boost/test/unit_test.hpp>
#include "J.h"

BOOST_AUTO_TEST_CASE(JTest) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::stringstream iss(
            "5 2\n"
                    "1 5 4\n"
                    "1 4 5\n"
                    "1 3 2\n"
                    "4 1 2\n"
                    "5 6 1"
    );
    std::streambuf* cinbuf = std::cin.rdbuf();
    std::cin.rdbuf(iss.rdbuf());

    std::stringstream oss;
    std::streambuf* coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());
    J j;
    j.solve();

    BOOST_TEST_MESSAGE(oss.str());
    BOOST_TEST(oss.str() == "5");

    std::cin.rdbuf(cinbuf);


}