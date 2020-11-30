#include <iostream>
#include <functional>
#include <vector>

#include <boost/bind.hpp>
#include <boost/assign.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/shared_ptr.hpp>

#include "TypeDef.h"

#pragma region differ mem_fun & fun_ptr

class Some_class
{
public:
	void Print_String(const std::string& s)const
	{
		std::cout<<s<<"\n";
	}

	void Print_ClassName()
	{
		std::cout<<"Some_class"<<std::endl;
	}
protected:
private:
};

void Print_String(const std::string s)
{
	std::cout<<s<<std::endl;
}

void Print_FunctionName()
{
	std::cout << "Print_functionname" <<std::endl;
}

void Test_1()
{
	std::ptr_fun(&Print_String)("hello1");
	//std::ptr_fun<void>(&Print_FunctionName)();
	Some_class sc0;
	std::mem_fun_ref(&Some_class::Print_ClassName)(sc0);
	std::mem_fun_ref<void,Some_class>(&Some_class::Print_ClassName)(sc0);
	//std::mem_fun1_ref<void,some_class,const std::stirng>(&some_class::print_string)(sc0,"hello2");

	(boost::bind(&Print_String,_1))("Hello func!");  
	boost::bind(Print_String,_1)("Hello func!");  

	boost::bind(&Print_FunctionName);
	boost::bind(Print_FunctionName);

	Some_class sc;  
	(boost::bind(&Some_class::Print_ClassName,_1)(sc));
	(boost::bind(&Some_class::Print_String,_1,_2))(sc,"Hello member!");
}

#pragma endregion differ mem_fun & fun_ptr

#pragma region Bind1st && Bind2nd

void Test_2()
{
	std::vector<uint32> ints;
	boost::assign::push_back(ints)(7)(4)(12)(11);

	uint32 count = std::count_if(ints.begin(), ints.end(),
		boost::bind(std::logical_and<bool>(), boost::bind(std::greater<uint32>(), _1, 5), boost::bind(std::less_equal<uint32>(), _1, 10)));
	
	std::vector<uint32>::iterator iter = std::find_if(ints.begin(), ints.end(),
		boost::bind(std::logical_and<bool>(), 
					boost::bind(std::greater<uint32>(), _1, 5), 
					boost::bind(std::less_equal<uint32>(), _1, 10)));

	if (iter != ints.end())
	{
		std::cout<<*iter<<std::endl;
	}
}

#pragma endregion Bind1st && Bind2nd

#pragma region ref && instance
class Tracer
{
public:
	Tracer()
	{
		std::cout<<"Tracer::Tracer()"<<std::endl;
	}

	Tracer(const Tracer& other)
	{
		std::cout<<"Tracer::Tracer(const Tracer& other)"<<std::endl;
	}

	Tracer& operator=(const Tracer& other)
	{
		std::cout<<"Tracer& Tracer::oprator=(const Tracer& other)"<<std::endl;
		return *this;
	}

	~Tracer()
	{
		std::cout << "tracer::~tracer()\n";
	}

	void Print(const std::string& s) const
	{
		std::cout<<s<<std::endl;
	}
protected:
private:
};

void Test_3()
{
	Tracer t, t1;
	boost::bind(&Tracer::Print, t, _1)(std::string("I'm called on a copy of t\n"));

	boost::bind(&Tracer::Print, boost::ref(t1), _1)(std::string("I'm called directly on t\n"));
}
#pragma endregion ref && instance

#pragma region bind virtual class function
class Base
{
public:
	virtual void Print() const
	{
		std::cout<<"Base"<<std::endl;
	}

	virtual ~Base()
	{

	}
protected:
private:
};

class Derived : public Base
{
public:
	virtual void Print() const
	{
		std::cout<<"Derived"<<std::endl;
	}

	~Derived()
	{

	}
protected:
private:
};

void Test_4()
{
	Derived d;
	Base b;
	boost::bind(&Base::Print, _1)(b);
	boost::bind(&Base::Print, _1)(d);
}
#pragma endregion bind virtual class function

#pragma region Bind class member(shared_ptr)
class personal_info 
{ 
	std::string name_;  
	std::string surname_;
	unsigned int age_;
public: 
	personal_info(const std::string& n,const std::string& s,unsigned int age):name_(n)
		,surname_(s)
		,age_(age)
	{

	} 

	~personal_info()
	{
		std::cout<<"age_ = "<<age_<<" destroy"<<std::endl;
	}
	std::string name() const 
	{
		return name_;
	} 
	std::string surname() const 
	{
		return surname_;
	} 
	unsigned int age() const 
	{
		return age_;
	}

	void Print()
	{
		std::cout<<"name_ = "<<name_<<", surname_ = "<<surname_<<", age_ = "<<age_<<std::endl;
	}
};

void Print(std::vector<boost::shared_ptr<personal_info>>& vec)
{
	for_each(vec.begin(), vec.end(), bind(&personal_info::Print, _1));
}

void Test_5()
{
	using namespace boost::assign;
	std::vector<boost::shared_ptr<personal_info>> vec;
	boost::shared_ptr<personal_info> ptr_1(new personal_info("Little","John",30));
	boost::shared_ptr<personal_info> ptr_2(new personal_info("Friar", "Tuck",50));
	boost::shared_ptr<personal_info> ptr_3(new personal_info("Robin", "Hood",40));
	//(1)boost::assign::push_back(vec)(ptr_1)(ptr_2)(ptr_3);
	vec += ptr_1, ptr_2, ptr_3;
	//(3)std::vector<boost::shared_ptr<personal_info>> vec = (list_of(ptr_1), ptr_2, ptr_3);

	Print(vec);

	std::sort(vec.begin(),
		vec.end(),
		boost::bind(std::less<unsigned int>(),boost::bind(&personal_info::age,_1),boost::bind(&personal_info::age,_2))
		);

	Print(vec);

	std::sort(vec.begin(),
		vec.end(),
		boost::bind(std::less<std::string>(),boost::bind(&personal_info::surname,_1),boost::bind(&personal_info::surname,_2))
		);

	Print(vec);
}
#pragma endregion Bind class member


uint32 main()
{
	Test_1();
	Test_2();
	Test_3();
	Test_4();
	Test_5();
	return 0;
}