#include <iostream>
#include <boost/format.hpp>

#include "TypeDef.h"

using namespace std;
using namespace boost;

#pragma region numeric_cast

#include <boost/numeric/conversion/cast.hpp>
#include <boost/detail/lightweight_test.hpp>

void TestNumericCast()
{
	short s = std::numeric_limits<short>::max();
	int i = boost::numeric_cast<int>(10);
	BOOST_ASSERT(i == s);

	BOOST_TEST_EQ(i, s);

	std::cout<<BOOST_CURRENT_FUNCTION<<std::endl;
	
	try
	{
		char c = boost::numeric_cast<char>(s);
	}
	catch (std::bad_cast& e)
	{
		std::cout<<e.what()<<std::endl;
	}
}

#pragma endregion numeric_cast

uint32 main()
{
	std::string st;
	if (st.size())
	{
	}

	if (st.empty())
	{
	}

	TestNumericCast();
	return 0;
}