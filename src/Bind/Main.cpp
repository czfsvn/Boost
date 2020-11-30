#include <iostream>
#include <vector>
#include <algorithm>

#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/rational.hpp>

#include "TypeDef.h"

using namespace std;
using namespace boost;

typedef int32 (*f_type)(int32, int32);
typedef int32 (*g_type)(int32, int32, int32);
typedef std::pair<int32, std::string> IntStrPair;

#pragma region 绑定普通函数

int32 f(int32 a, int32 b)
{
	return a + b;
}

int32 g(int32 a, int32 b, int32 c)
{
	return a + b * c;
}

void Bind_1()
{
	std::cout<<boost::bind(f, 1, 2)()<<std::endl;
	std::cout<<boost::bind(g, 1, 2, 3)()<<std::endl;

	int32 x = 10, y = 20, z = 30;
	std::cout<<boost::bind(f, _1, 9)(x)<<std::endl;
	std::cout<<boost::bind(f, _1, _2)(x, y)<<std::endl;
	std::cout<<boost::bind(f, _2, _1)(x, y)<<std::endl;
	std::cout<<boost::bind(f, _1, _1)(x, y)<<std::endl;
	std::cout<<boost::bind(g, _1, 8, _2)(x, y)<<std::endl;
	std::cout<<boost::bind(g, _3, _2, _2)(x, y, z)<<std::endl;

	std::cout<<boost::bind(g, boost::cref(z), y, boost::ref(y))()<<std::endl;
}

#pragma endregion 绑定普通函数

#pragma region 绑定函数指针
void Bind_funPtr()
{
	f_type pf = f;
	g_type pg = g;

	int32 x = 10, y = 20, z = 30;
	std::cout<<boost::bind(pf, _1, 9)(x)<<std::endl;
	std::cout<<(*pf)(x, 9)<<std::endl;

	std::cout<<boost::bind(pg, _3, _2, _2)(x, y, z)<<std::endl;
	std::cout<<(*pg)(z, y, y)<<std::endl;
}
#pragma endregion 绑定函数指针

#pragma region 绑定成员函数
class Demo
{
public:
	int32 fun(int32 a, int32 b)
	{
		return a * b;
	}
protected:
private:
};


void Bind_2()
{
	Demo a, &ra = a;
	Demo* pa = &a;
	boost::shared_ptr<Demo> ptr(new Demo);

	int32 x = 10, y = 20;
	std::cout<<boost::bind(&Demo::fun, a, _1, 20)(x)<<std::endl;
	std::cout<<boost::bind(&Demo::fun, ra, _2, _1)(x, y)<<std::endl;
	std::cout<<boost::bind(&Demo::fun, pa, _1, _2)(x, 7)<<std::endl;
	std::cout<<boost::bind(&Demo::fun, ptr, _1, _2)(x, 7)<<std::endl;

	std::cout<<boost::bind(&Demo::fun, boost::ref(a), _1, 20)(x)<<std::endl;
}
#pragma endregion 绑定成员函数

#pragma region TestBind
class PointTest
{
public:
	PointTest(int32 a = 0, int32 b = 0) : m_x(a)
		,m_y(b)
	{

	}

	void Print()
	{
		std::cout<<"("<<m_x<<", "<<m_y<<")\n";
	}
protected:
private:

public:
	int32 m_x, m_y;
};

void TestBind()
{
	std::vector<PointTest> v(10);
	for_each(v.begin(), v.end(), boost::bind(&PointTest::Print, _1));
}

#pragma endregion TestBind

#pragma region 绑定成员变量
void Bind_3()
{
	std::vector<PointTest> v(10);
	std::vector<int32> v2(10);

	transform(v.begin(), v.end(), v2.begin(), boost::bind(&PointTest::m_x, _1));
	for_each(v2.begin(), v2.end(), std::cout<<boost::lambda::_1<<"\n");

	std::pair<int32, std::string> strPair(123, "string");
	std::cout<<boost::bind(&std::pair<int32, std::string>::first, strPair)()<<std::endl;
	std::cout<<boost::bind(&std::pair<int32, std::string>::second, strPair)()<<std::endl;
}
#pragma endregion 绑定成员变量

#pragma region 绑定函数对象

struct FStruct
{
	uint32 operator()(int32 a, int32 b)
	{
		return a + b;
	}
};

void Bind_4()
{
	rational<uint32> rUint;
	std::cout<<boost::bind<uint32>(FStruct(), _1, _2)(10, 20)<<std::endl;
}
#pragma endregion 绑定函数对象

#pragma region Nest bind
void Bind_5()
{
	std::cout<<boost::bind(f, _1, boost::bind(g, _2, _3, _3))(1, 2, 5)<<std::endl;
}
#pragma endregion Nest bind



void main()
{
	Bind_1();
	Bind_funPtr();
	Bind_2();
	TestBind();
	Bind_3();
	Bind_4();
	Bind_5();
}
