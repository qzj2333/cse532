#define _ENABLE_ATOMIC_ALIGNMENT_FIX
#include <iostream>
// 2.
//#include "listing_7_2.h"

// 3.
//#include "listing_7_3.h"

// 5.
#include "listing_7_13.h"
#include <vector>
#include <thread>
using namespace std;


/*
enum arg
{
	success = 0
};
*/

int main(int argc, char** argv)
{
	lock_free_stack<int> stack;
	vector<int> ov;
	vector<int> ev;

	/* 2
	for (int i = 0; i < 10; i++) {
		cout << "push: " << i << endl;
		stack.push(i);
	}
	*/

	/* 3
	stack.push(1);
	cout << "push: " << 1 << endl;
	stack.push(2);
	cout << "push: " << 2 << endl;
	cout << "pop: " << *stack.pop() << endl;
	stack.push(3);
	cout << "push: " << 3 << endl;
	stack.push(4);
	cout << "push: " << 4 << endl;
	cout << "pop: " << *stack.pop() << endl;
	cout << "pop: " << *stack.pop() << endl;
	stack.push(5);
	cout << "push: " << 5 << endl;
	*/

	
	thread t1([&stack, &ov] {
		for (int i = 0; i < 10; i++) {
			if (i % 2 == 1) 
			{
				cout << "push odd: " << i << endl;
				stack.push(i);
			}
		}

		for (int i = 0; i < 10; i++) {
			if (i % 2 == 1) {
				ov.push_back(*stack.pop());
			}
		}
		});

	
	thread t2([&stack, &ev] {
		for (int i = 0; i < 10; i++) {
			if (i % 2 == 0)
			{
				cout << "push even: " << i << endl;
				stack.push(i);
			}
		}

		for (int i = 0; i < 10; i++) {
			if (i % 2 == 0) {
				ev.push_back(*stack.pop());
			}
		}
		});
	
	
	t1.join();
	t2.join();

	for (int n : ov) {
		cout << "ov: " << n << '\n';
	}
	for (int n : ev) {
		cout << "ev: " << n << '\n';
	}

	return 0;
}