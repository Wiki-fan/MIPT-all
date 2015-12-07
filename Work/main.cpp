#include "stdafx.h"
#include "XMLParser.h"

int main()
{
	std::string s =
			"<body>\n"
					"    Text1.\n"
					"    <tag1>\n"
					"        TextInTag1.\n"
					"    </tag1>\n"
					"</body>\n";
	XMLParser parser;
	//parser.Test();
	parser.Parse( s );
	std::cin.get();
	return 0;
}

