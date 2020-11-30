#include <iostream>



#pragma region boost::factory

#include <boost/functional/factory.hpp>
#include <boost/checked_delete.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/bind.hpp>

void TestFactory()
{
	//指针
	int* pInt1 = boost::factory<int*>()();
	int* pInt2 = new int;

	std::string* ps1 = boost::factory<std::string*>()();
	std::string* ps2 = new std::string;

	std::pair<int, double>* ppair1 = boost::factory<std::pair<int, double>*>()();
	std::pair<int, double>* ppair2 = new std::pair<int, double>;

	boost::checked_delete(pInt1);
	boost::checked_delete(ps1);
	boost::checked_delete(ppair1);

	//对象
	std::auto_ptr<int> ap = boost::factory<std::auto_ptr<int>>()();
	boost::shared_ptr<std::string> sp = boost::factory<boost::shared_ptr<std::string>>()();

	//scoped_ptr<int> p = factory<scoped_ptr<int>>()();									编译错误

	//带参数
	int a = 10, b = 20;
	int* pInt3 = boost::factory<int*>()(a);
	//int* pInt3 = boost::factory<int*>()(10);														Err

	std::string* ps3 = boost::factory<std::string*>()("char* lvalue");

	std::pair<int, int>* ppair3 = boost::factory<std::pair<int, int>*>()(a, b);
	//std::pair<int, int>* ppair3 = boost::factory<std::pair<int, int>*>()(1, 2);			Err	

	int* pInt4 = boost::bind(boost::factory<int*>(), 10)();
	int* pInt5 = boost::bind(boost::factory<int*>(), _1)(10);
}

#pragma endregion boost::factory

#pragma region boost::value_factory

#include <boost/functional/value_factory.hpp>			//只能类似factory创建对象，不能创建指针
#include <boost/typeof/typeof.hpp>

void TestValueFactory()
{
	int i = boost::value_factory<int>()();
	std::string str = boost::value_factory<std::string>()("Hello");
	BOOST_AUTO(p, (boost::value_factory<std::pair<int, std::string>>(),(i, str)));

	int b = boost::bind(boost::value_factory<int>(), 10)();
	int c = boost::bind(boost::value_factory<int>(), _1)(10);

	//std::complex<int> c = boost::bind(boost::value_factory<std::complex<int>>(), _1, _2, )(10, 20);
}

#pragma endregion boost::value_factory

void main()
{
	TestFactory();
	TestValueFactory();
}