#include <boost/thread.hpp>    
#include <iostream>    
#include <vector>    
#include <cstdlib>    
#include <ctime> 

#define MAX_SIZE			100


//------------------------------------------normal

boost::shared_mutex mutex;   
std::vector<int> random_numbers; 

void wait(int seconds)   
{   
	boost::this_thread::sleep(boost::posix_time::seconds(seconds));   
}   

void fill()   
{   
	std::srand(static_cast<unsigned int>(std::time(0)));   
	for (int i = 0; i < MAX_SIZE; ++i)   
	{   
		boost::unique_lock<boost::shared_mutex> lock(mutex);  
		int val = std::rand();
		random_numbers.push_back(val);  
		std::cout <<"fill:"<<val<<", i = "<<i<< std::endl;   
		lock.unlock();   
		wait(1);   
	}   
}   

void print()   
{   
	for (int i = 0; i < MAX_SIZE; ++i)   
	{
		wait(1);   
		boost::shared_lock<boost::shared_mutex> lock(mutex);   
		std::cout <<"print:"<<random_numbers.back()<<", i = "<<i<< std::endl;   
	}   
}   

int sum = 0;   

void count()   
{   
	for (int i = 0; i < MAX_SIZE; ++i)   
	{   
		wait(1);   
		boost::shared_lock<boost::shared_mutex> lock(mutex);   
		sum += random_numbers.back();   
		std::cout <<"count: sum = "<<sum<<", i = "<<i<< std::endl;   
	}   
}   

//------------------------------------------condition

boost::mutex cond_mutex;   
boost::condition_variable_any cond;   
std::list<int> cond_random_list;

void Condition_Fill()
{
	std::srand(static_cast<unsigned int>(std::time(0)));   
	for (int i = 0; i < MAX_SIZE; ++i)   
	{   
		boost::unique_lock<boost::mutex> lock(cond_mutex);  
		int val = std::rand();
		cond_random_list.push_back(val);   
		std::cout<<"Condition_Fill:: i = "<<i<<", val = "<<val<<std::endl;
		cond.notify_all();   
		//cond.wait(cond_mutex); 
		wait(1);
	}
}

void Condition_Print()   
{   
	std::size_t next_size = 1;   
	for (int i = 0; ; ++i)   
	{   
		boost::unique_lock<boost::mutex> lock(cond_mutex);   
		while (cond_random_list.size() == 0)   
			cond.wait(cond_mutex);   
		std::cout<<"Condition_Print:: i = "<<i<<", "<< cond_random_list.back() << std::endl; 
		cond_random_list.pop_back();
		//++next_size;   
		//cond.notify_all(); 
		wait(1);
	}   
}

//------------------------------------------TLS: compare with tls show the  virtue of tls

boost::mutex tls_mutex_weak;
void init_number_generator()   
{   
	static bool done = false;   
	if (!done)   
	{   
		done = true;   
		std::srand(static_cast<unsigned int>(std::time(0)));   
	}   
}   

void random_number_generator(int idx)   
{   
	init_number_generator();   
	int i = std::rand();   
	boost::lock_guard<boost::mutex> lock(tls_mutex_weak);   
	std::cout <<"random_number_generator: idx = "<<idx<<", i = "<<i << std::endl;   
}  

//------------------------------------------TLS: show the  virtue of tls
boost::mutex tls_mutex;   

void tls_init_number_generator()   
{   
	static boost::thread_specific_ptr<bool> tls;   
	if (!tls.get())   
		tls.reset(new bool(false));   
	if (!*tls)   
	{   
		*tls = true;   
		std::srand(static_cast<unsigned int>(std::time(0)));   
	}   
}   



void tls_random_number_generator(int idx)   
{   
	tls_init_number_generator();   
	int i = std::rand();   
	boost::lock_guard<boost::mutex> lock(tls_mutex);   
	std::cout <<"tls_random_number_generator: idx = "<<idx<<", i = "<<i << std::endl; 
}   
int main()   
{   
	/*boost::thread t1(fill);   
	boost::thread t2(print);   
	boost::thread t3(count);   
	t1.join();   
	t2.join();   
	t3.join();   
	std::cout << "Sum: " << sum << std::endl;   */

	
	//boost::thread t4(Condition_Fill);
	//boost::thread t5(Condition_Print);
	//t4.join();
	//t5.join();


	boost::thread t[MAX_SIZE], tls[MAX_SIZE];   
	for (int i = 0; i < MAX_SIZE; ++i)   
	{
		//t[i] = boost::thread(random_number_generator, i); 
		tls[i] = boost::thread(tls_random_number_generator, i); 
	}

	for (int i = 0; i < MAX_SIZE; ++i)   
	{
		//t[i].join(); 
		tls[i].join();
	}

	
	return 0;
}   