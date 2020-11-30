#include <iostream>
#include "TypeDef.h"

#include <boost/filesystem.hpp>

void TestPath()
{
	boost::filesystem::path p1("D:\\");
	boost::filesystem::path p2("D:\\Test");
	boost::filesystem::path p3("D:\\Test\\Test");
}

uint32 main()
{
	TestPath();
	return 0;
}