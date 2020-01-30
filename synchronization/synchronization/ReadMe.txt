1. Nicole Wang, Lin Li
2.
CODE:
#include <vector>
#include <iostream>
using namespace std;

class foo
{
private:
	vector<int> vi;
public:
	foo operator<<(const int& i)
	{
		vi.push_back(i);
		return *this;
	}

	int operator()()
	{
		int sum = 0;
		for (int i : vi)
		{
			sum += i;
		}
		return sum;
	}
};


int main(int argc, char** argv)
{
	foo f;
	for (int i = 0; i < 5; ++i)
	{
		f << i;
	}
	cout << f() << endl;
	return 0;
}

RESULT: 10

3.1
thread t1([&f]
		{
			for (int i = 0; i < 5; i++)
			{
				f << i;
				cout << "thread 1 insert: " << i << endl;
			}
			cout << "thread1 sum:" << f() << endl;
		});
	thread t2([&f]
		{
			for (int i = 6; i < 10; i++)
			{
				f << i;
				cout << "thread 2 insert: " << i << endl;
			}
			cout << "thread2 sum:" << f() << endl;
		});

	t1.join();
	t2.join();

3.2
thread 1 insert: 0thread 2 insert: 6

thread 2 insert: 7
thread 1 insert: 1
thread 2 insert: 8
thread 1 insert: 2
thread 2 insert: 9
thread2 sum:36
thread 1 insert: 3
thread 1 insert: 4
thread1 sum:40

3.3 
If class object is created in a sepearate thread, then other thread that do insertion may execute before the creation thread and cause error.
Otherwise, since all threads inserts into same object's vector, depend on the order of thread execution, the summation from each thread's result may be different.

4.1
class foo
{
private:
	vector<int> vi;
	mutex m; // 4
public:
	foo* operator<<(const int& i)
	{
		lock_guard<mutex> guard(m); // 4
		vi.push_back(i);
		return this;
	};

	int operator()()
	{
		lock_guard<mutex> guard(m); // 4
		int sum = 0;
		for (int i : vi)
		{
			sum += i;
		}
		return sum;
	}

	// 4 synchronized print method (synchronize = add mutex?)
	void printMethod(int sum)
	{
		lock_guard<mutex> guard(m);
		cout << sum << endl;
	}
};


int main(int argc, char** argv)
{
	// 3
	thread t1([&f]
		{
			for (int i = 0; i < 5; i++)
			{
				f << i;
			}
			f.printMethod(f()); // 4
		});
	thread t2([&f]
		{
			for (int i = 6; i < 10; i++)
			{
				f << i;
			}
			f.printMethod(f()); // 4
		});

	t1.join();
	t2.join();

4.2
30
40

4.3 It acts similar as previous since mutex only locks part of insertion, and locks insertion and calculation separately. 

5. program crashed because of self-deadlock ()

6.1
template<typename T, typename U> class foo
{
private:
	vector<T> vi;
	//mutex m; // 4
	U m; // 6
public:
	foo* operator<<(const int& i)
	{
		//lock_guard<mutex> guard(m); // 4
		lock_guard<U> guard(m); // 6
		vi.push_back(i);
		return this;
	};

	int operator()()
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

		for (int i = factorial -1; i > 0; i--)
		{
			factorial *= i;
		}
		return factorial;
	}
};


int main(int argc, char** argv)
{
	// 3
	thread t1([&f]
		{
			for (int i = 0; i < 5; i++)
			{
				f << i;
			}
			//f.printMethod(f()); // 4
			cout << "factorial1: " << f.factorial_of_sum() << endl; // 5
		});
	thread t2([&f]
		{
			for (int i = 6; i < 7; i++)
			{
				f << i;
			}
			//f.printMethod(f()); // 4
			cout << "factorial2: " << f.factorial_of_sum() << endl; // 5
		});

	t1.join();
	t2.join();
	return 0;
}

6.2 Program compiles and runs with recursive_lock. Change class to template and paramatize the mutex type allow user use different type of locks based on their needs.