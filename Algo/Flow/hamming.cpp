#include "hamming.h"
#include <stdlib.h>
#include <fstream>
#define MY_CIN_REDIR

int main() {
    //srand(static_cast<unsigned int>(time(0)));
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
#ifdef MY_CIN_REDIR
    std::ifstream ifs("inHamming0.txt", std::ios_base::in);
    std::streambuf* cinbuf = std::cin.rdbuf();
    std::cin.rdbuf(ifs.rdbuf());
#endif

    HammingOptimizer<uint, int, int> hamming;
    hamming.solve();

#ifdef MY_CIN_REDIR
    std::cin.rdbuf(cinbuf);
#endif
    //std::cin.get();
    return 0;
}

