#include "synchronized_list.h"

template<typename T, typename U>
foo* foo<T, U>::operator<<(const int& i)
{
	//lock_guard<mutex> guard(m); // 4
	lock_guard<U> guard(m); // 6
	vi.push_back(i);
	return this;
}

int foo::operator()()
{
	//lock_guard<mutex> guard(m); // 4
	lock_guard<U> guard(m); // 6
	int sum = 0;
	for (int i : vi)
	{
		sum += i;
	}
	return sum;
}