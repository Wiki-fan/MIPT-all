#include "maximal_flow.h"
#include <stdlib.h>
#include <fstream>
#define MY_CIN_REDIR

int main() {
    srand(static_cast<unsigned int>(time(0)));
#ifdef MY_CIN_REDIR
    std::ifstream ifs( "inFlow.txt", std::ios_base::in );
    std::streambuf* cinbuf = std::cin.rdbuf();
    std::cin.rdbuf( ifs.rdbuf() );
#endif

    MaximalFlow flow;
    flow.find();

#ifdef MY_CIN_REDIR
    std::cin.rdbuf( cinbuf );
#endif
    //std::cin.get();
    return 0;
}