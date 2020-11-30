#include <iostream>
#include <set>
#include "TypeDef.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/lambda/lambda.hpp>


#pragma region multi_index_container
//from: http://blog.csdn.net/gongxinheng/article/details/5421914

struct Course
{
	uint32		course_num;		//课程编号
	uint32		course_hour;
	std::string	course_name;

	enum		CourseNum_Enum
	{
		CourseNum_CPP	=	0,
		CourseNum_English,
		CourseNum_Maths,
		CourseNum_Computer,
		CourseNum_DataStructure
	};


	static const Course courses[5];

	Course(uint32 num, uint32 hour, std::string name) : course_num(num),
		course_hour(hour), course_name(name)
	{

	}
};


struct Student
{
	uint32		stu_num;
	std::string	stu_name;
	uint32		stu_age;
	std::set<Course>	stu_courseSet;

	static Student CreateStudent(const std::string& name, uint32 age)
	{
		static uint32 curNum = 1;
		return Student(curNum++, name, age);
	}

private:
	Student(uint32 num, const std::string& name, uint32 age) : stu_num(num),
		stu_name(name), stu_age(age)
	{

	}
};

std::ostream& operator << (std::ostream& os, const Course& cou)
{
	os << "课程编号："<< cou.course_num << "\t课程名：" << cou.course_name <<"\t课时："
		<<cou.course_hour<<"\n";
	return os;
}

std::ostream& operator << (std::ostream& os, const Student& stu)
{
	os << "学号："<< stu.stu_num << "\t姓名：" << stu.stu_name <<"\t年龄："
		<<stu.stu_age<<"\n";
	os << "主修课程:\n";
	std::for_each(stu.stu_courseSet.begin(), stu.stu_courseSet.end(), std::cout<< boost::lambda::_1);

	return os;
}

bool operator < (const Course& lhs, const Course& rhs)
{
	return lhs.course_num < rhs.course_num;
}

const Course Course::courses[5] =
{
	Course(CourseNum_CPP, 80, "C++"),
	Course(CourseNum_English, 60, "English"),
	Course(CourseNum_Maths, 45, "Math"),
	Course(CourseNum_Computer, 50, "Computer"),
	Course(CourseNum_DataStructure, 60, "DataStruct"),
};

struct stu_num{};
struct stu_name{};
struct stu_age{};

using boost::multi_index_container; 
using namespace boost::multi_index;

typedef boost::multi_index_container<Student,
	indexed_by<ordered_unique<tag<stu_num>, 
											BOOST_MULTI_INDEX_MEMBER(Student, uint32, stu_num)>,
					ordered_non_unique<tag<stu_name>, 
											BOOST_MULTI_INDEX_MEMBER(Student, std::string, stu_name)>,
					ordered_non_unique<tag<stu_age>,
											BOOST_MULTI_INDEX_MEMBER(Student, uint32, stu_age)>>>  StudentContainer;


template<typename Tag, typename MultiIndexContainer>
void Print_out_by(const MultiIndexContainer& s)
{
	const typename boost::multi_index::index<MultiIndexContainer, Tag>::type& i = get<Tag>(s);
	typedef typename MultiIndexContainer::value_type value_type;

	std::copy(i.begin(), i.end(), std::ostream_iterator<value_type>(std::cout));
}

class CModifyStudentAge
{
public:
	CModifyStudentAge(uint32 age) : m_age(age)
	{

	}

	void operator() (Student& s)
	{
		s.stu_age = m_age;
	}
protected:
private:
	uint32 m_age;
};


void Test_multi_index_container()
{
	StudentContainer studentSets;

	Student& stu1 = Student::CreateStudent("zhangsan", 19);
	stu1.stu_courseSet.insert(Course::courses[Course::CourseNum_CPP]);
	stu1.stu_courseSet.insert(Course::courses[Course::CourseNum_English]);
	studentSets.insert(stu1);

	Student& stu2 = Student::CreateStudent("lisi", 18);
	stu2.stu_courseSet.insert(Course::courses[Course::CourseNum_CPP]);
	stu2.stu_courseSet.insert(Course::courses[Course::CourseNum_DataStructure]);
	stu2.stu_courseSet.insert(Course::courses[Course::CourseNum_Computer]);
	studentSets.insert(stu2);

	Student& stu3 = Student::CreateStudent("wangwu", 21);
	stu3.stu_courseSet.insert(Course::courses[Course::CourseNum_English]);
	stu3.stu_courseSet.insert(Course::courses[Course::CourseNum_Maths]);
	studentSets.insert(stu3);

	Student& stu4 = Student::CreateStudent("zhangsan", 18);
	stu4.stu_courseSet.insert(Course::courses[Course::CourseNum_Computer]);
	stu4.stu_courseSet.insert(Course::courses[Course::CourseNum_Maths]);
	studentSets.insert(stu4);

	Print_out_by<stu_num>(studentSets);

	//名字做索引
	StudentContainer::index<stu_name>::type& indexOfName = studentSets.get<stu_name>();
	StudentContainer::index<stu_name>::type::iterator it = indexOfName.find("lisi");
	if (it != indexOfName.end())
	{
		std::cout<<"founded lisi"<<std::endl;
		Student stu_tmp = *it;
		stu_tmp.stu_age = 20;
		stu_tmp.stu_num = 1;
		
		// bool ret = indexOfName.replace(it, stu_tmp);
		bool ret = indexOfName.modify(it, CModifyStudentAge(21));
	}
	else
	{
		std::cout<<"Not founded lisi"<<std::endl;
	}

	StudentContainer::index<stu_name>::type::iterator itLow = indexOfName.lower_bound("zhangsan");
	StudentContainer::index<stu_name>::type::iterator itUp = indexOfName.upper_bound("zhangsan");
	while (itLow != itUp)
	{
		std::cout<<*itLow++;
	}
}
#pragma endregion multi_index_container


int main()
{
	Test_multi_index_container();
	return 0;
}
