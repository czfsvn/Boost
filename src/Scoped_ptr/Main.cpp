#include <iostream>
#include <list>
#include <Windows.h>


#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/ptr_container/ptr_map.hpp>

#include "TypeDef.h"

class Test;

typedef std::list<boost::shared_ptr<Test>> TestPtrList;

class Test
{
public:
	Test() : m_id(0)
	{
		std::cout<<"Test:"<<m_id<<std::endl;
	}

	Test(uint32 i) : m_id(i)
	{
		std::cout<<"Test:"<<m_id<<std::endl;
	}

	~Test()
	{
		std::cout<<"~Test:"<<m_id<<std::endl;
	}

	void Print()
	{
		std::cout<<"Print:"<<m_id<<std::endl;
	}

	uint32 GetId()
	{
		return m_id;
	}
protected:
private:
	uint32 m_id;
};

void TestScopePtr()
{
	boost::scoped_ptr<int> i(new int);
	*i = 1;
	*i.get() = 2;
	i.reset(new int);

	boost::scoped_ptr<Test> scTest(new Test(1));
	scTest.reset(new Test(2));
}

void TestScopeArray()
{
	boost::scoped_array<int> i(new int[2]); 
	*i.get() = 1; 
	i[1] = 2; 
	i.reset(new int[3]); 

	boost::scoped_array<Test> scArr(new Test[3]);
	scArr[0].Print();
	scArr.reset(new Test[2]);
}

void TestSharedPtr()
{
	{	
		TestPtrList ptrList;
		{
			for (uint32 idx = 0; idx < 5; idx++)
			{
				boost::shared_ptr<Test> ptr(new Test(idx));			
				ptrList.push_back(ptr);
			}
		}
		BOOST_FOREACH(boost::shared_ptr<Test> ptr, ptrList)
		{
			if (ptr.get())
			{
				if (ptr->GetId() == 4)
				{
					ptr.reset();
					//ptrList.remove(ptr);
					break;
				}
			}
		}
	}
}

void TestSharedArray()
{
	{
		boost::shared_array<uint32> arr_1(new uint32[2]);
		{
			boost::shared_array<uint32> arr_2(arr_1);
			arr_1[0] = 1;
			std::cout << arr_2[0] << std::endl; 
		}		
	}	
}

DWORD WINAPI reset(LPVOID p) 
{ 
	boost::shared_ptr<int> *sh = static_cast<boost::shared_ptr<int>*>(p); 
	sh->reset(); 
	return 0; 
} 

DWORD WINAPI print(LPVOID p) 
{ 
	boost::weak_ptr<int> *w = static_cast<boost::weak_ptr<int>*>(p); 
	boost::shared_ptr<int> sh = w->lock(); 
	if (sh) 
		std::cout << *sh << std::endl; 
	return 0; 
} 

void TestWeakPtr()
{
	boost::shared_ptr<int> sh(new int(99)); 
	boost::weak_ptr<int> w(sh); 
	HANDLE threads[2]; 
	threads[0] = CreateThread(0, 0, reset, &sh, 0, 0); 
	threads[1] = CreateThread(0, 0, print, &w, 0, 0); 
	WaitForMultipleObjects(2, threads, TRUE, INFINITE);
}



int main()
{
	TestWeakPtr();

	TestScopePtr();

	TestScopeArray();

	TestSharedPtr();

	TestSharedArray();
	
	return 0;
}