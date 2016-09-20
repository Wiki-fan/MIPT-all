#include "BadParquet.h"
#include <stdlib.h>
#include <fstream>
//#define MY_CIN_REDIR

int main() {
    srand(static_cast<unsigned int>(time(0)));
#ifdef MY_CIN_REDIR
    std::ifstream ifs( "inParquet.txt", std::ios_base::in );
    std::streambuf* cinbuf = std::cin.rdbuf();
    std::cin.rdbuf( ifs.rdbuf() );
#endif

    BadParquet bp;
    bp.solve();

#ifdef MY_CIN_REDIR
    std::cin.rdbuf( cinbuf );
#endif
    //std::cin.get();
    return 0;
}