#include <iostream>
#include <thread>
#include <exception>
#include "interruptible.h"

using namespace std;

void interruption_point()
{
	//cout << "!!" << endl;
	if (this_thread_interrupt_flag.is_set())
	{
		cout << "Q" << endl;
		//throw "thread_interrupted";
	}
}


int main(int argc, char** argv)
{
	interruptible_thread t1(interruption_point);
	interruptible_thread t2(interruption_point);
	cout << "finish initialization" << endl;
	/*try
	{
		t1.interrupt();
		cout << "interrupt t1" << endl;
	}
	catch (...)
	{
		cout << "t1: " << endl;
	}
	try
	{
		t2.interrupt();
		cout << "interrupt t2" << endl;
	}
	catch (...)
	{
		cout << "t2: " << endl;

	}*/
	cout << "done" << endl;
	return 0;
}