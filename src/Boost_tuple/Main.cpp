#include <iostream>
#include <string>

#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>

using namespace std;
using namespace boost;

typedef boost::tuple<int, string, string> IISTuple;

int main()
{
	IISTuple m_tuple(1, "Name", "Passwod");
	std::cout<<m_tuple<<std::endl;
	std::cout<<tuples::set_open('{');
	std::cout<<tuples::set_close('}');
	std::cout<<tuples::set_delimiter(',');
	std::cout<<m_tuple<<std::endl;
	

	return 0;
}


