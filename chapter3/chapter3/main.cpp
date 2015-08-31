#include <iostream>
#include <list>
#include <algorithm>
#include <mutex>

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

int main()
{
	some_list.push_back(2);
	some_list.push_back(21);
	some_list.push_back(342);

	std::cout << "Hello" << std::endl;
}