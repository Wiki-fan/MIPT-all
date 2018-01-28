#pragma once
#include "../stdafx.h"

// Простой парсер XML-файлов.
// Считается, что теги не имеют атрибутов, уровень вложенности не превосходит 4,
// символы <>/ встречаются только в качестве управляющих.
// После парсинга в head лежит фиктивная CNode с пустым тегом, 
// содержащая весь текст (сделана для того, чтобы сохранить текст, написанный вне <body>.
// Её детьми будет body.
class XMLParser {
public:
	XMLParser() {}

	~XMLParser();

	// Парсинг строки s.
	void Parse( const std::string& s );

private:
	struct CNode {
		CNode() {}
		~CNode()
		{
			for( int i = 0; i < childs.size(); ++i ) {
				delete childs[i];
			}
		}

		CNode( std::string& _tag ) : tag( _tag ) {}
		std::string tag;
		std::string text;
		std::vector<CNode*> childs;
	};

	// Парсинг подстроки s с родителем parent.
	void parse( const std::string& s, CNode* parent );

	// Корневая вершина.
	CNode* head;
};

XMLParser::~XMLParser()
{
	delete head;
}

void XMLParser::parse( const std::string& s, CNode* parent )
{

	std::stringstream is;
	is << s << '\n';
	//std::cout << s;
	std::string str;
	int mainCount = 0;

	CNode* cur = 0;
	
	while( getline( is, str, '\n' )/*is >> str*/ ) {
		//std::cout << str << std::endl;
		if( str[0] != '<' && mainCount != 0 ) { // Текст, находящийся под областью действия тега.
			cur->text.append( str + '\n' );
		} else if( str[0] == '<' ) { // Тег.
			std::string tmp = str.substr( 1, str.length() - 2 );
			if( tmp[0] != '/' ) { // Любой открывающийся.
				std::cout << "Opentag " << tmp << std::endl;
				if( mainCount == 0 ) { // Главный тег.
					parent->childs.push_back( new CNode );
					cur = parent->childs.back();
					cur->tag = tmp;
					++mainCount;
				} else if( tmp == cur->tag ) { // Такой же, открывающийся.
					++mainCount;
					cur->text.append( str + '\n' );
				} else { // Другой, открывающийся.
					cur->text.append( str + '\n' );
				}
			} else { // Любой закрывающийся.
				std::cout << "Closetag " << tmp << std::endl;
				if( tmp == '/' + cur->tag ) { // Такой же, закрывающийся.
					--mainCount;
				} 
				if( mainCount != 0 ) {
					cur->text.append( str + '\n' );
				} // Закрывающий тег главного тега не нужно дописывать.
			}
			if( mainCount == 0 ) { // Главный тег закрылся и внутри был хотя бы один другой тег.
				std::cout << std::endl;
				parse( cur->text, cur);
			}
		}
	}
}

void XMLParser::Parse( const std::string& s )
{
	head = new CNode;
	head->text = s;
	parse( s, head );
}

void TestParser()
{
	std::string s, tmp;
	while( getline( std::cin, tmp, '\n' ) ) {
		// Убираем идущие в начале пробелы
		size_t first1 = tmp.find_first_not_of( ' ' );
		tmp = tmp.substr( first1, tmp.size() );
		// И знаки табуляции.
		size_t first2 = tmp.find_first_not_of( '\t' );
		s += tmp.substr( first2, tmp.size() );
		s += '\n';
	}

	XMLParser parser;
	parser.Parse( s );
}
