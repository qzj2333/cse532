#pragma once
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

template<typename T, typename U> class foo
{
private:
	vector<T> vi;
	//mutex m; // 4
	U m; // 6
public:
	foo* operator<<(const int& i);
	int operator()();

	// 4 synchronized print method (synchronize = add mutex?)
	void printMethod(int sum)
	{
		//lock_guard<mutex> guard(m);
		lock_guard<U> guard(m); // 6
		cout << sum << endl;
	}

	// 5
	long factorial_of_sum()
	{
		//lock_guard<mutex> guard(m);
		lock_guard<U> guard(m); // 6
		int factorial = (*this)();

		for (int i = factorial - 1; i > 0; i--)
		{
			factorial *= i;
		}
		return factorial;
	}
};
