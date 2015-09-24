#pragma once
//#define thread_local __declspec( thread )

#include <mutex>
#include <memory>
#include <thread>

class hierarchical_mutex
{
public:
	explicit hierarchical_mutex(unsigned long value) :
		hierarchy_value(value), prev_hierachy_value(value)
	{}
	~hierarchical_mutex()
	{}

	void lock();
	void unlock();
	bool try_lock();

private:
	void check_for_hierarchy_violation();
	void update_hierarchy_value();

	std::mutex internal_mutex;
	const unsigned long hierarchy_value;
	unsigned long prev_hierachy_value;
	static __declspec(thread) unsigned long this_thread_hierarchy_value;
};

