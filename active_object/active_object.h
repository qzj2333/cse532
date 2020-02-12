#ifndef ACTIVE_OBJECT_H
#define ACTIVE_OBJECT_H

#pragma once
#include "synchronized_list.h"

template<typename T>
class active_object
{
private:
	synchronized_list<T> l;
public:
	active_object();
	void enqueue(T item);
	void run_service();
};

#ifdef TEMPLATE_HEADERS_INCLUDE_SOURCE
#include "active_object.cpp"
#endif
#endif