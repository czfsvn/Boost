#include <iostream>
#include <assert.h>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/assign.hpp>

#include "TypeDef.h"

using namespace boost;

#pragma region function
int32 f(int32 a, int32 b)
{
	return a + b;
}

void function_1()
{
	boost::function<int32(int32, int32)> func;
	assert(!func);

	func = f;
	if (func)
	{
		std::cout<<func(10, 20)<<std::endl;
	}
	func = 0;
	assert(func.empty());
}
#pragma endregion function

void fun(std::string& s)
{
	std::cout<<"fun: s = "<<s<<std::endl;
}

void function_str()
{
	boost::function<void(std::string& s)> func;
	assert(!func);

	func = fun;
	if (func)
	{
		std::string msg = "test";
		func(msg);
	}
	func = 0;
	assert(func.empty());
}

//-----------------------------------------------------------Demo_class
#pragma region Demo_class
struct Demo_class 
{
	int32 Add(int32 a, int32 b)
	{
		return a + b;
	}

	int32 operator()(int x) const
	{
		return x * x;
	}
};

void function_2()
{
	function<int32(Demo_class&, int32, int32)> func1 = bind(&Demo_class::Add, _1, _2, _3);
	Demo_class sc;
	std::cout<<func1(sc, 10, 20)<<std::endl;

	function<int32(int32, int32)> func2 = bind(&Demo_class::Add, &sc, _1, _2);
	std::cout<<func2(10, 20)<<std::endl;
}
#pragma endregion Demo_class

#pragma region ref function

template<typename T>
struct Summary 
{
	typedef void result_type;
	T sum;
	Summary(T v = T()) : sum(v)
	{

	}

	void operator()(T const& x)
	{
		sum += x;
	}

};

void function_cref()
{
	Demo_class sc;
	function<int32(int32)> func;
	func = cref(sc);
	std::cout<<func(10)<<std::endl;
}

void function_ref()
{
	using namespace boost::assign;
	std::vector<int32> v = (list_of(1), 3, 5, 7, 9);
	Summary<int32> sum;
	function<void(int32 const&)> func(ref(sum));
	std::for_each(v.begin(), v.end(), func);
	std::cout<<sum.sum<<std::endl;
}
#pragma endregion ref function

#pragma region function of callback
class Call_Demo_class
{
public:
	Call_Demo_class(int32 i) : m_n(i)
	{

	}

	template<typename CallBack>
	void Accept(CallBack f)
	{
		func = f;
	}

	void run()
	{
		func(m_n);
	}
	
protected:
private:
	typedef function<void(int32)> func_t;
	func_t func;
	int32 m_n;
};

void call_back_fun(int32 i)
{
	std::cout<<"call_back_fun:"<<i * 2<<std::endl;
}

void function_CallBack()
{
	Call_Demo_class dc(10);
	dc.Accept(call_back_fun);
	dc.run();
}
#pragma endregion function of callback

#pragma region Call_back_obj
class Call_back_obj
{
public:
	Call_back_obj(int32 i) : x(i)
	{

	}

	void operator()(int32 i)
	{
		std::cout<<"Call_back_obj : "<<i * x++<<std::endl;
	}

protected:
private:
	int32 x;
};

void function_CallBackObj()
{
	Call_Demo_class dc(10);
	Call_back_obj cbo(2);
	dc.Accept(ref(cbo));
	dc.run();
	dc.run();
}
#pragma endregion Call_back_obj

#pragma region Call_back_factory
class Call_back_factory
{
public:
	void call_back_fun1(int32 i)
	{
		std::cout<<"Call_back_factory1 : "<<i * 2<<std::endl;
	}

	void call_back_fun2(int32 i, int32 j)
	{
		std::cout<<"Call_back_factory2 : "<<i * j * 2<<std::endl;
	}
protected:
private:
};

void function_CallBackFactory()
{
	Call_Demo_class dc(10);
	Call_back_factory cbf;
	dc.Accept(bind(&Call_back_factory::call_back_fun1, cbf, _1));
	dc.run();
	dc.Accept(bind(&Call_back_factory::call_back_fun2, cbf, _1, 5));
	dc.run();
}
#pragma endregion Call_back_factory

#pragma region String test
class StrTest
{
public:
	StrTest()
	{
	}

	~StrTest()
	{
	}

	void StrPrint(std::string& s)
	{
		std::cout<<"StrPrint: s = "<<s<<std::endl;
	}

protected:
private:
};

void func_strTest()
{
	function<void (StrTest&, std::string&)> Func;
	//Func fun = bind();
}
#pragma endregion String test

void main()
{
	function_1();
	function_str();
	function_2();
	function_ref();
	function_cref();
	function_CallBack();
	function_CallBackObj();
	function_CallBackFactory();
	std::cout<<"ddd"<<std::endl;
}