#include "synchronized_list.h"

template <typename T>
synchronized_list<T>::synchronized_list()
{
	low = 0;
	high = 0;
	unlimit = true;
};

template <typename T>
synchronized_list<T>::synchronized_list(size_t ilow, size_t ihigh) : low(ilow), high(ihigh) { }

template <typename T>
void synchronized_list<T>::push_back(T item)
{

	unique_lock<mutex> lk(m);
	cv.wait(lk, [this] { return (l.size() < high) || unlimit || isShutDown; });		// wait until list size <= high
	if (!isShutDown)
	{
		// wake up
		l.push_back(item);
		lk.unlock();
		// use nofity_all as default
		cv.notify_all(); // notify all threads, all threads check above while loop condition, and then one of them get lock
	}	

};

template <typename T>
T synchronized_list<T>::pop_back()
{
	unique_lock<mutex> lk(m);
	cv.wait(lk, [this] { return l.size() > low || (unlimit && !l.empty()) || isShutDown; });		// wait until list size >= low
	if (!isShutDown)
	{																							// wake up
		T item = l.back();
		l.pop_back();
		lk.unlock();
		cv.notify_all();
		return item;
	}
	return NULL;
};

template <typename T>
void synchronized_list<T>::push_front(T item)
{
	unique_lock<mutex> lk(m);
	cv.wait(lk, [this] { return (l.size() < high) || unlimit || isShutDown; });		// wait until list size <= high
	if (!isShutDown)
	{
		// wake up
		l.push_front(item);
		lk.unlock();
		// use nofity_all as default
		cv.notify_all();
	}

};

template <typename T>
T synchronized_list<T>::pop_front()
{
	unique_lock<mutex> lk(m);
	cv.wait(lk, [this] { return l.size() > low || (unlimit && !l.empty()) || isShutDown; });		// wait until list size >= low
	if (!isShutDown)
	{
		// wake up
		T item = l.front();
		l.pop_front();
		lk.unlock();
		cv.notify_all();
		return item;
	}
	return NULL;
};

template <typename T>
void synchronized_list<T>::shut_down()
{
	unique_lock<mutex> lk(m);
	isShutDown = true;
	lk.unlock();
	cv.notify_all();
};