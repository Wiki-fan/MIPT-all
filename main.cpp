#include "stdafx.h"
//#define MY_CIN_REDIR
int main()
{
	srand( static_cast<unsigned int>(time( 0 )));
#ifdef MY_CIN_REDIR
	std::ifstream ifs( "in.txt", std::ios_base::in );
	std::streambuf* cinbuf = std::cin.rdbuf();
	std::cin.rdbuf( ifs.rdbuf() );
#endif

#ifdef MY_CIN_REDIR
	std::cin.rdbuf( cinbuf );
#endif
	std::cin.get();
	return 0;
}

