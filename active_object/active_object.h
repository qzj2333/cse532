#pragma once
#include "synchronized_list.h"

template<class T>
class active_object
{
private:
	synchronized_list<T> l;
public:
	active_object();
	void enqueue(T item);
	void run_service();
};

