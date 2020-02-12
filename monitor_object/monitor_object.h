#ifndef MONITOR_OBJECT_H
#define MONITOR_OBJECT_H

#pragma once
#include<list>
#include <mutex>
using namespace std;

template <typename T> 
class synchronized_list
{
private:
	list<T> l;
	// 4
	mutex m;
	condition_variable cv;
	typename list<T>::size_type low; 
	typename list<T>::size_type high;
	bool unlimit = false; // true if low = high = 0

public:
	bool finishedPush = false;
	synchronized_list();
	synchronized_list(size_t ilow, size_t ihigh);
	void push_back(T item);
	T pop_back();
	void push_front(T item);
	T pop_front();
	void popAll();
};


#endif
