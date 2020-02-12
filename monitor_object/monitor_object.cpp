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
	unlimit = true;
};

template <class T>
synchronized_list<T>::synchronized_list(size_t ilow, size_t ihigh) : low(ilow), high(ihigh) {}

template <class T>
void synchronized_list<T>::push_back(T item)
{
	/*lock_guard<mutex> lk(m);
	l.push_back(item);
	cv.notify_one();*/
	// 6
	//while (high > 0 || unlimit)
	//{
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
	//while (l.size() > low || (unlimit && !l.empty()))
	//{
		unique_lock<mutex> lk(m);
		cv.wait(lk, [this] { return l.size() > low || (unlimit && !l.empty()); });		// wait until list size >= low
		// wake up
		T item = l.front();
		l.pop_front();
		cout << "pop:" << item << endl;
		lk.unlock();
		cv.notify_all();
		return item;
	//}
	//return NULL;
};

/*template <class T>
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
};*/

int main(int argc, char** argv)
{
	synchronized_list<int> l(0,1);

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
			/*
			for (int i = 1; i < 5; i++)
			{
				l.push_back(i);
			}
			*/
			l.push_back(5);
			//cout << "push: 5" << endl;
			// 4
			/*int item = l.pop_back();
			while (item != NULL)
			{
				cout << "t1: " << item << endl;
				item = l.pop_back();
			}*/
		});
	thread t2([&l]
		{
			/*
			for (int i = 5; i < 10; i++)
			{
				l.push_back(i);
			}
			*/
			l.push_back(8);
			//cout << "push: 8" << endl;
			// 4
			/*int item = l.pop_back();
			while (item != NULL)
			{
				cout << "t2: " << item << endl;
				item = l.pop_back();
			}*/
		});
	thread t3([&l]
		{
			l.push_back(10);
		});
	thread t4([&l]
		{
			l.push_back(3);
		});

	thread t5([&l]
		{
			l.pop_front();
		});

	thread t6([&l]
		{
			l.pop_front();
		});

	thread t7([&l]
		{
			l.pop_front();
		});

	thread t8([&l]
		{
			l.pop_front();
		});

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();
	t8.join();

	// 3
	/*int item = l.pop_back();
	while (item != NULL)
	{
		cout << item << endl;
		item = l.pop_back();
	}*/
	return 0;
};