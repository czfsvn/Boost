#include <iostream>
#include <algorithm>

#include "TypeDef.h"

#include <boost\assign\std\vector.hpp>
#include <boost\assign\std\list.hpp>
#include <boost\assign\list_inserter.hpp>
#include <boost\bind.hpp>

using namespace std;
using namespace boost::assign;


void Print(uint32 i)
{
	std::cout<<"i = "<<i<<std::endl;
}

class AddClass : public std::binary_function<uint32, uint32, void>
{
public:
	void operator()(uint32 i, uint32 j) const
	{
		std::cout<<"Add class"<<i + j<<std::endl;
	}
protected:
private:
};

void AddFunc(uint32 i, uint32 j)
{
	std::cout<<"Add function"<<i + j<<std::endl;
}

bool Compare(uint32 i, uint32 j)
{
	return i > j;
}

void Test()
{
	vector<uint32> vec;
	vec += 1,2,3,5,6,7,8;

	list<uint32> uList;
	uList += 1,2,3,5,6,7,8;

	std::for_each(vec.begin(), vec.end(), Print);

	std::for_each(vec.begin(), vec.end(), std::bind1st(AddClass(), 10));

	std::for_each(vec.begin(), vec.end(), boost::bind(AddFunc, 10, _1));

	std::sort(vec.begin(), vec.end(), boost::bind(Compare, _1, _2));
	std::sort(vec.begin(), vec.end(), Compare);
	std::sort(vec.begin(), vec.end(), boost::bind(Compare, _2, _1));
	std::sort(uList.begin(), uList.end(), Compare);
	uList.sort();
	uList.sort(Compare);
	insert(vec)(9,10,11,12,13);
	insert(vec)(14)(15)(16)(17)(18)(19)(20)(21);
}


uint32 main()
{
	Test();
	return 0;
};