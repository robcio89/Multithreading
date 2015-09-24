#include "hierarchical_mutex.h"
#include <stdexcept>
#include <iostream>

__declspec(thread) unsigned long hierarchical_mutex::this_thread_hierarchy_value(ULONG_MAX);

void hierarchical_mutex::lock()
{
	check_for_hierarchy_violation();
	internal_mutex.lock();
	update_hierarchy_value();
}

void hierarchical_mutex::unlock()
{
	this_thread_hierarchy_value = prev_hierachy_value;
	internal_mutex.unlock();
}

bool hierarchical_mutex::try_lock()
{
	check_for_hierarchy_violation();

	if (!internal_mutex.try_lock())
	{
		return false;
	}

	update_hierarchy_value();
	return true;
}

void hierarchical_mutex::check_for_hierarchy_violation()
{
	std::cout << "check_for_hierarchy_violation()" << std::endl;
	if (this_thread_hierarchy_value <= hierarchy_value)
	{
		throw std::logic_error("Naruszono hierarchiê mutexow");
	}
}

void hierarchical_mutex::update_hierarchy_value()
{
	prev_hierachy_value = this_thread_hierarchy_value;
	this_thread_hierarchy_value = hierarchy_value;
}
