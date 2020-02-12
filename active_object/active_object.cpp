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

		for (int i = factorial - 1; i > 0; i--)
		{
			factorial *= i;
		}
		return factorial;
	}
};


int main(int argc, char** argv)
{
	// 2
	foo<int, recursive_mutex> f;
	/*for (int i = 0; i < 5; ++i)
	{
		f << i;
	}
	cout << f() << endl;*/

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
