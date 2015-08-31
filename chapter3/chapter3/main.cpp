#include <iostream>
#include <list>
#include <algorithm>
#include <mutex>
#include <string>

std::list<int> some_list;
std::mutex some_mutex;

void add_to_list(int val)
{
	std::lock_guard<std::mutex> guard(some_mutex);
	some_list.push_back(val);
}

bool list_contains(int val)
{
	std::lock_guard<std::mutex> guard(some_mutex);

	return std::find(some_list.begin(), some_list.end(), val) != some_list.end();
}


class some_data
{
	int a;
	std::string b;

public:
	void do_something()
	{
		a = 100;
		b = "blah";
		std::cout << "a: " << a << ", b: " << b << std::endl;
	}
};

class data_wrapper
{
	some_data data;
	std::mutex m;

public:
	template<typename Function>
	void process_data(Function func)
	{
		std::lock_guard<std::mutex> l(m);
		func(data);
	}
};

some_data* unprotected;
data_wrapper x;

void malicious_function(some_data& protected_data)
{
	unprotected = &protected_data;
}

void foo()
{
	x.process_data(malicious_function);
	unprotected->do_something();
}

int main()
{
	some_list.push_back(2);
	some_list.push_back(21);
	some_list.push_back(342);
	some_list.push_back(44);

	std::cout << "Hello" << std::endl;

	foo();
}