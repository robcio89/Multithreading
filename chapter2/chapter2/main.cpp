#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <chrono>

#include "accumulate.h"

using namespace std;

void do_something(unsigned int& x)
{
	for (unsigned i = 0; i < 999999998; ++i)
		x += i;

	std::cout << "Thread id: " << std::this_thread::get_id() <<  " ==> x = " << x << std::endl;
};

void do_something_in_current_thread()
{
	int x = 0;

	for (unsigned i = 0; i < 999999989; ++i)
		x *= i;
};

struct func
{
	int& i;

	func(int& i_) : i(i_)
	{
	}

	void operator()()
	{
		for (unsigned j = 0; j < 100000; ++j)
		{
			do_something(j);
		}
	}
};


class scoped_thread
{
	std::thread t;

public:
	explicit scoped_thread(std::thread t_) :
		t(std::move(t_))
	{
		if (!t.joinable())
			throw std::logic_error("Brak w¹tku");
	}

	~scoped_thread()
	{
		t.join();
	}

	scoped_thread(const scoped_thread&) = delete;
	scoped_thread& operator=(const scoped_thread&) = delete;
};

void f()
{
	int some_local_stat = 0;
	scoped_thread t(std::thread(func(some_local_state)));

	do_something_in_current_thread();
}

void g()
{
	std::vector<std::thread> threads;

	for (unsigned i = 0; i < 20; ++i)
	{
		threads.push_back(std::thread(do_something, i));
	}

	std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
}

int main()
{
	cout << "hello, hardware_concurrency is: " << std::thread::hardware_concurrency() << endl;

	auto start_time = std::chrono::high_resolution_clock::now();

//	f();
	g();

	auto end_time = std::chrono::high_resolution_clock::now();
	auto time = end_time - start_time;

	std::cout << "Time in ms: " <<
		std::chrono::duration_cast<std::chrono::microseconds>(time).count() << std::endl;;

	return 0;
}