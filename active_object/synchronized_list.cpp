#include "synchronized_list.h"

template <class T>
synchronized_list<T>::synchronized_list()
{
	low = NULL;
	high = NULL;
};

synchronized_list<int>::synchronized_list()
{
	low = 0;
	high = 0;
	unlimit = true;
};

template <class T>
synchronized_list<T>::synchronized_list(size_t ilow, size_t ihigh) : low(ilow), high(ihigh) {}

template <class T>
void synchronized_list<T>::push_back(T item)
{

	unique_lock<mutex> lk(m);
	cv.wait(lk, [this] { return (l.size() < high) || unlimit; });		// wait until list size <= high
	// wake up
	l.push_back(item);
	lk.unlock();
	// use nofity_all as default
	cv.notify_all(); // notify all threads, all threads check above while loop condition, and then one of them get lock
//}
};

template <class T>
T synchronized_list<T>::pop_back()
{
	unique_lock<mutex> lk(m);
	cv.wait(lk, [this] { return l.size() > low || (unlimit && !l.empty()); });		// wait until list size >= low
	// wake up
	T item = l.front();
	l.pop_back();
	lk.unlock();
	cv.notify_all();
	return item;
};

template <class T>
void synchronized_list<T>::push_front(T item)
{
	unique_lock<mutex> lk(m);
	cv.wait(lk, [this] { return (l.size() < high) || unlimit; });		// wait until list size <= high
	// wake up
	l.push_front(item);
	lk.unlock();
	// use nofity_all as default
	cv.notify_all();
};

template <class T>
T synchronized_list<T>::pop_front()
{
	unique_lock<mutex> lk(m);
	cv.wait(lk, [this] { return l.size() > low || (unlimit && !l.empty()); });		// wait until list size >= low
	// wake up
	T item = l.front();
	l.pop_front();
	lk.unlock();
	cv.notify_all();
	return item;
	
};