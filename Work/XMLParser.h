#pragma once
#include "stdafx.h"
#include "SingleLinkedList.h"

// Простой парсер XML-файлов.
// Считается, что теги не имеют атрибутов, уровень вложенности не превосходит 4,
// символы <>/ встречаются только в качестве управляющих.
class XMLParser {
public:
	XMLParser() { }
	~XMLParser() { }

	void Parse( std::string& s );

private:
	struct CNode {
		CNode() { }
		CNode( std::string& _tag ) : tag( _tag ) { }
		std::string tag;
		std::string text;
		std::vector<CNode*> childs;
	};

	void parse( std::string& s, CNode* parent );
	CNode* head;
};

void XMLParser::parse( std::string& s, CNode* parent )
{

	std::stringstream is;
	is << s;
	std::cout << s;
	std::string str;
	int count = 0;
	// запилить cur

	parent->childs.push_back( new CNode );
	CNode* cur = parent->childs.back();
	// Проматываем не теги.
	while( str[0] != '<' ) {
		cur->text.append( str );
	}
	// Добавляем главный тег.
	parent->tag = str.substr( 1, str.length() - 2 );

	while( is.good()) {
		is >> str;
		std::cout << str << std::endl;
		if( str[0] != '<' ) { // Текст.
			cur->text.append( str );
		} else { // Тег.
			std::string tmp = str.substr( 1, str.length() - 2 );
			if( tmp[0] != '/' ) { // Любой открывающийся.
				std::cout << "Opentag " << tmp << std::endl;
				if( tmp == cur->tag ) { // Такой же, открывающийся.
					++count;
				} else { // Другой, открывающийся.
				}
				cur->text.append( str );
			} else { // Любой закрывающийся.
				std::cout << "Closetag " << tmp << std::endl;
				if( tmp == '/' + cur->tag ) { // Такой же, закрывающийся.
					--count;
				} else { // Другой, закрывающийся.
				}
				if( count != 0 ) {
					cur->text.append( str );
				} // Закрывающийся тег главного тега не нужно дописывать.
			}
			if( count == 0 ) {
				parse( cur->text, cur );
			}
		}
	}
}

void XMLParser::Parse( std::string& s )
{
	parse( s, head );
}

