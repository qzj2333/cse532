#include <iostream>
#include <thread>
#include "monitor_object.h"
using namespace std;

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
};

template <class T>
synchronized_list<T>::synchronized_list(list<T>::size_type low, list<T>::size_type high)
{
	this.low = low;
	this.high = high;
};

template <class T>
void synchronized_list<T>::push_back(T item)
{
	/*lock_guard<mutex> lk(m);
	l.push_back(item);
	cv.notify_one();*/
	// 6
	while (high > 0 && !finishedPush)
	{
		unique_lock<mutex> lk(m);
		cv.wait(lk, [this] { return l.size() <= high; });		// wait until list size <= high
		// wake up
		l.push_back(item);
		lk.unlock();
		// use nofity_all as default
		cv.notify_all(); // notify all threads, all threads check above while loop condition, and then one of them get lock
	}
};

template <class T>
T synchronized_list<T>::pop_back()
{
	if (!l.empty())
	{
		T item = l.back();
		l.pop_back();
		return item;
	}
	return NULL;
};

template <class T>
void synchronized_list<T>::push_front(T item)
{
	lock_guard<mutex> lk(m);
	l.push_front(item);
	cv.notify_all();
};

template <class T>
T synchronized_list<T>::pop_front()
{
	/*if (!l.empty())
	{
		T item = l.front();
		l.pop_front();
		return item;
	}
	return NULL;*/
	while (!l.empty())
	{
		unique_lock<mutex> lk(m);
		cv.wait(lk, [this] { return l.size() >= low; });		// wait until list size >= low
		// wake up
		T item = l.front();
		l.pop_front();
		lk.unlock();
		cv.notify_all();
		return item;
	}
	return NULL;
};

template <class T>
void synchronized_list<T>::popAll()	// pop from back
{
	// 5
	while (!l.empty() || !finishedPush)
	{
		unique_lock<mutex> lk(m);
		cv.wait(lk, [this] { return !l.empty(); });		// wait until list is not empty
		// wake up
		T item = pop_front();
		cout << item << endl;
		lk.unlock();
	}
};

int main(int argc, char** argv)
{
	synchronized_list<int> l;

	// 2
	/*for (int i = 1; i < 5; i++)	// did not push 0 becuase 0 consider as NULL
	{
		l.push_back(i);
	}
	int item = l.pop_front();
	while (item != NULL)
	{
		cout << item << endl;
		item = l.pop_front();
	}*/
	// 6
	//synchronized_list<int> l();
	// 3
	thread t1([&l]
		{
			// 4
			for (int i = 1; i < 5; i++)
			{
				l.push_front(i);
			}
			l.finishedPush = true;
			// 4
			/*int item = l.pop_back();
			while (item != NULL)
			{
				cout << "t1: " << item << endl;
				item = l.pop_back();
			}*/
			//l.popAll();
		});
	thread t2([&l]
		{
			for (int i = 5; i < 10; i++)
			{
				l.push_front(i);
			}
			l.finishedPush = true;
			// 4
			/*int item = l.pop_back();
			while (item != NULL)
			{
				cout << "t2: " << item << endl;
				item = l.pop_back();
			}*/
			l.popAll();
		});

	t1.join();
	t2.join();

	// 3
	/*int item = l.pop_back();
	while (item != NULL)
	{
		cout << item << endl;
		item = l.pop_back();
	}*/

	return 0;
};