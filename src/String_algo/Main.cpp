#include <iostream>
#include <vector>
#include "TypeDef.h"

#include <boost/algorithm/string.hpp>
#include <boost/typeof/typeof.hpp>

using namespace std;

using namespace boost;


uint32 main()
{
	string test = "test.txt";
	if (ends_with(test, "txt"))
	{
		std::cout<<to_upper_copy(test)+" UPPER"<<std::endl;
	}

	std::cout<<"replace_first_copy of text = "<<replace_first_copy(test, "t", "g")<<std::endl;

	vector<char> v(test.begin(), test.end());
	for (BOOST_AUTO(pos, v.begin()); pos != v.end(); pos++)
		std::cout<<*pos<<std::endl;
	
	replace_all(test, "te", "TE");
	if (starts_with(test, "TE"))
	{
		std::cout<<to_lower_copy(test)+" UPPER"<<std::endl;
	}

	if (iends_with(test, "TXT"))
	{
		std::cout<<to_upper_copy(test)+" iends_with"<<std::endl;
	}

	if (icontains(test, "TXT"))
	{
		std::cout<<to_upper_copy(test)+" icontains"<<std::endl;
	}

// 	if (//all())
// 	{
// 	}
	return 0;
}