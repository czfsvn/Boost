#include <iostream>
#include "TypeDef.h"

//#define BOOST_DISABLE_ASSERTS			//disable BOOST_ASSERT


#include <boost/assert.hpp>
#include <boost/static_assert.hpp>

class Test
{
public:
protected:
private:
};


void main()
{
	uint32 tes = 123;
	std::string str_1 = IntBStr(tes);
	uint64 num = StrBUInt64(str_1);

	Test* m_testPtr = new Test;

	//BOOST_ASSERT(false);
	//BOOST_STATIC_ASSERT(m_testPtr);
	for (uint16 idx = 0; idx < 100; idx++)
	{
		std::cout<<rand()<<std::endl;
	}
	
}