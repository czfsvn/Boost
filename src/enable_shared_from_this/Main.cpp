#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#pragma region enable_shared_from_this_1

struct Ansible;

typedef boost::shared_ptr<Ansible> Shared_Ansible;

struct Ansible : public boost::enable_shared_from_this<Ansible>
{
	Shared_Ansible Get_Shared()
	{
		Shared_Ansible r(this);
		return r;
	}

	~Ansible()
	{
		std::cout<<"destructor"<<std::endl;
	}
};

void Test_enable_shared_from_this_1()
{
	Shared_Ansible a(new Ansible);
	Ansible& r = *a;
	Shared_Ansible b = r.shared_from_this();

	std::cout<<"count = "<<a.use_count()<<", "<<b.use_count()<<std::endl;
}

#pragma endregion enable_shared_from_this_1

int32 main()
{
	Test_enable_shared_from_this_1();
	return 0;
}