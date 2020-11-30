#include <iostream>
#include <string>

#include <boost/bimap.hpp>
#include <boost/bimap/bimap.hpp>
#include <boost/bimap/list_of.hpp>
#include <boost/bimap/unordered_set_of.hpp>

using namespace boost::bimaps;

struct english {};
struct spanish {};

// typedef bimap
// 	<
// 	unordered_set_of< tagged< std::string, spanish > >,
// 	unordered_set_of< tagged< std::string, english > >,
// 	list_of_relation
// 	> Translator;


typedef boost::bimap< int, std::string > UintStrBiMap;

int main()
{
	//Translator trans;
	//trans.push_back(Translator::value_type("1", "Hello"));
	UintStrBiMap m_biMap;
	m_biMap.insert(UintStrBiMap::value_type(3, "three"));
	m_biMap.insert(UintStrBiMap::value_type(1, "one"));
	m_biMap.insert(UintStrBiMap::value_type(2, "two"));
	std::cout<<"m_biMap.size() = "<<m_biMap.size()<<std::endl;
	UintStrBiMap::iterator iter = m_biMap.begin();
	for (; iter != m_biMap.end(); iter++)
	{		
		std::cout<<iter->left<<"<---->"<<iter->right<<std::endl;
	}

	UintStrBiMap::right_map::iterator lIter = m_biMap.right.begin();
	for (; lIter != m_biMap.right.end(); lIter++)
	{		
		std::cout<<lIter->first<<"<---->"<<lIter->second<<std::endl;
	}
	return 0;
}