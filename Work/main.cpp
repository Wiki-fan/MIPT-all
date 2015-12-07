#include "stdafx.h"
#include "MeldableHeapTests.h"
#include "XMLParser.h"
int main()
{
	//TestMyHeaps();
	//ManualTest();
	std::ifstream ifs("/home/sharik/code/didactic-woof/Work/XML.xml", std::ios_base::in);
	FILE* infile = fopen("/home/sharik/code/didactic-woof/Work/XML.xml", "r");

	massert(ifs.good());
	while (ifs.good()) {
		std::string s;
		ifs >>s;
		std::cout <<s;
	}
	XMLParser parser;
	parser.Test();
	std::cin.get();
	return 0;
}

/*#include <iostream>

using namespace std;
int main( int argc,      // Number of strings in array argv
          char *argv[],   // Array of command-line argument strings
          char *envp[] )  // Array of environment variable strings
{
	int count;

	// Display each command-line argument.
	cout << "\nCommand-line arguments:\n";
	for( count = 0; count < argc; count++ )
		cout << "  argv[" << count << "]   "
				<< argv[count] << "\n";
}
*/
