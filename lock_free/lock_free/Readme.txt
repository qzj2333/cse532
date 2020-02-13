1. Lin Li, Nicole Wang

2. We don't need to reload the head before while loop, because the compiler does that for us, since by using compare_exchange_weak method,
the head pointer has the same value and we set it in new_node. Also, since we only loop directly on failure, so we can use compare_exchange_weak.

3.Two files involved:
(1) listing_7_3.h
#pragma once
#include <iostream>
#include <atomic>
using namespace std;

template<typename T>
class lock_free_stack
{
private:
	struct node
	{
		std::shared_ptr<T> data;
		node* next;
		node(T const& data_) :
			data(std::make_shared<T>(data_))
		{}
	};
	std::atomic<node*> head;
public:
	void push(T const& data)
	{
		node* const new_node = new node(data);
		new_node->next = head.load();
		while (!head.compare_exchange_weak(new_node->next, new_node));
	}
	std::shared_ptr<T> pop()
	{
		node* old_head = head.load();
		while (old_head &&
			!head.compare_exchange_weak(old_head, old_head->next));
		return old_head ? old_head->data : std::shared_ptr<T>();
	}
};
(2) lock_free.cpp
#include <iostream>
#include "listing_7_3.h"
using namespace std;

enum arg
{
	success = 0
};

int main(int argc, char** argv)
{
	lock_free_stack<int> stack;

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

	return success;
}

Output:
push: 1
push: 2
pop: 2
push: 3
push: 4
pop: 4
pop: 3
push: 5

4. 