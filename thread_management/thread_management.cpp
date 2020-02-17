#include <iostream>
#include <thread>
#include <exception>
#include "interruptible.h"

using namespace std;

int fib(unsigned int n)
{
	if (n <= 1)
	{
		return n;
	}
	else
	{
		int result = fib(n - 1) + fib(n - 2);
		//cout << "result: " << result << endl;
		return result;
	}
}

int main(int argc, char** argv)
{
	interruptible_thread t1(fib(10));

	//interruption_point();
	interruptible_thread t2(fib(7));
	cout << "finish initialization" << endl;
	try
	{
		t1.detach();
		t2.detach();
		t2.interrupt();
		t2.interruption_point();
		//t2.detach();
		t1.interruption_point();
		//t1.detach();

		cout << "interrupt t1" << endl;

	}
	catch (...)
	{
		cout << "t1: " << endl;
	}
	/*try
	{
		t1.interrupt();
		cout << "interrupt t2" << endl;
		t2.detach();
	}
	catch (...)
	{
		cout << "t2: " << endl;

	}*/
	cout << "done" << endl;
	return 0;
}