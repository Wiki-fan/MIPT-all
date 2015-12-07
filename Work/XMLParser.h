#pragma once
#include "stdafx.h"
#include "SingleLinkedList.h"


class XMLParser
{
public:
	XMLParser() {}
	~XMLParser() {}

	void parse(std::istream& is);
	void Test()
	{
		std::ifstream ifs("XML.xml", std::ios_base::in);
		parse(ifs);
		ifs.close();
	}
private:
	struct CNode
	{
		std::string tag;
		std::string text;
		CSingleLinkedList<CNode> childs;
	};
	CNode* head;
};

void XMLParser::parse( std::istream& is )
{
	std::string s;
	is >> s;
	std::cout <<s;

	while (!is.eof()) {
		is >> s;
		if (s[0] == '<') {
			std::cout << s.substr(1, s.length()-2);
		}

	}
}


