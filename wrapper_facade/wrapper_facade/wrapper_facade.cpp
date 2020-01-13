// wrapper_facade.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
using namespace std;

void printLineMsg()
{
	cout << "Hello World" << endl;
}

int main(int argc, char** argv)
{
	// 3
	// printLineMsg();

	// 4
	/*thread t(printLineMsg);
	t.join();*/

	// 5
	thread t1(printLineMsg);
	thread t2(printLineMsg);
	t1.join();
	t2.join();

	return 0;
}

