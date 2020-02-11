#pragma once
#include<list>
#include <mutex>
using namespace std;

template <class T> 
class synchronized_list
{
private:
	list<T> l;
	// 4
	mutex m;
	condition_variable cv;
	list<T>::size_type low; 
	list<T>::size_type high;
public:
	bool finishedPush = false;
	synchronized_list();
	synchronized_list(list<T>::size_type ilow, list<T>::size_type ihigh);
	void push_back(T item);
	T pop_back();
	void push_front(T item);
	T pop_front();
	void popAll();
};
