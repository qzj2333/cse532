#ifndef ACTIVE_OBJECT_H
#define ACTIVE_OBJECT_H

#pragma once
#include "synchronized_list.h"

template<typename T>
class active_object
{
private:
	synchronized_list<T> l;
	bool isActive;	// 3
	active_object* obj; // 5
	void run_service(); // 3
public:
	active_object();
	active_object(active_object* ao);
	void enqueue(T item);
	//void run_service();  // 2
	void active(); // 3
	void deactive(); // 4
};

#ifdef TEMPLATE_HEADERS_INCLUDE_SOURCE
#include "active_object.cpp"
#endif
#endif