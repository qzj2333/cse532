#ifndef SYNCHRONIZED_LIST_H
#define SYNCHRONIZED_LIST_H

#pragma once
#include <iostream>
#include<list>
#include <mutex>
#include <condition_variable>
#include <thread>
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
	bool isShutDown = false; // 5 return from wait if isShutDown == true
public:
	bool finishedPush = false;
	synchronized_list();
	synchronized_list(size_t ilow, size_t ihigh);
	void push_back(T item);
	T pop_back();
	void push_front(T item);
	T pop_front();
	void shut_down();
};
#ifdef TEMPLATE_HEADERS_INCLUDE_SOURCE
#include "synchronized_list.cpp"
#endif
#endif