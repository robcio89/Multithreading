#include <iostream>
#include "hierarchical_mutex.h"

hierarchical_mutex high_level_mutex(10000);
hierarchical_mutex low_level_mutex(5000);
//zmiana wartoœci na wiêksz¹ ni¿ 10000 nie powoduje b³êdu w programie
hierarchical_mutex other_mutex(100);

int do_low_level_stuff()
{
	for (int i = 0; i < 5; ++i)
	{
		std::cout << "Number " << i << std::endl;
	}
	return 1;
}

void do_high_level_stuff(int param)
{
	for (int i = 0; i < param; ++i)
	{
		std::cout << "Number " << i << std::endl;
	}
}

int low_level_func()
{
	std::lock_guard<hierarchical_mutex> lk(low_level_mutex);
	return do_low_level_stuff();
}

void high_level_func()
{
	std::lock_guard<hierarchical_mutex> lk(high_level_mutex);
	do_high_level_stuff(low_level_func());
}

void do_other_stuff()
{
	std::cout << "Function do_other_stuff()" <<  std::endl;
}

void other_stuff()
{
	high_level_func();
	do_other_stuff();
}

void thread_a()
{
	high_level_func();
}

void thread_b()
{
	std::lock_guard<hierarchical_mutex> lk(other_mutex);
	other_stuff();
}

int main()
{
	std::cout << "Hierarchical mutex example" << std::endl;

	thread_a();
	thread_b();

	return 0;
}