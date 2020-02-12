1. Nicole Wang, Lin Li
2. 
CODE:
template <typename T> 
class synchronized_list
{
private:
	list<T> l;
public:
	synchronized_list() {};
	void push_back(T item)
	{
		l.push_back(item);
	};
	T pop_back()
	{
		if (!l.empty())
		{
			T item = l.back();
			l.pop_back();
			return item;
		}
		return NULL;
	};
	void push_front(T item)
	{
		l.push_front(item);
	};
	T pop_front()
	{
		if (!l.empty())
		{
			T item = l.front();
			l.pop_front();
			return item;
		}
		return NULL;
	};
};


int main(int argc, char** argv)
{
	synchronized_list<int> l;
	for (int i = 1; i < 5; i++)	// did not push 0 becuase 0 consider as NULL
	{
		l.push_back(i);
	}
	int item = l.pop_front();
	while (item != NULL)
	{
		cout << item << endl;
		item = l.pop_front();
	}
	return 0;
};

RESULT:
1
2
3
4

3.1
int main(int argc, char** argv)
{
	synchronized_list<int> l;
	thread t1([&l]
		{
			for (int i = 1; i < 5; i++)
			{
				l.push_front(i);
			}
		});
	thread t2([&l]
		{
			for (int i = 5; i < 10; i++)
			{
				l.push_front(i);
			}
		});

	t1.join();
	t2.join();

	int item = l.pop_back();
	while (item != NULL)
	{
		cout << item << endl;
		item = l.pop_back();
	}

	return 0;
3.2
5
6
7
8
9
1
2
3
4

t1: 4

3.3
The number added in one thread is in correct order, but no order between threads.

4.1
int main(int argc, char** argv)
{
	synchronized_list<int> l;

	// 3
	thread t1([&l]
		{
			for (int i = 1; i < 5; i++)
			{
				l.push_front(i);
			}
			// 4
			int item = l.pop_back();
			while (item != NULL)
			{
				cout << "t1: " << item << endl;
				item = l.pop_back();
			}
		});
	thread t2([&l]
		{
			for (int i = 5; i < 10; i++)
			{
				l.push_front(i);
			}

			// 4
			int item = l.pop_back();
			while (item != NULL)
			{
				cout << "t2: " << item << endl;
				item = l.pop_back();
			}
		});

	t1.join();
	t2.join();

	return 0;
};

4.2
t1: t2: 21

t1: 3
t2: 4
t1: 5
t2: 6
t2: 7
t1: 8
t2: 9

4.3
Same as 3.3

5.1
template <typename T> 
class synchronized_list
{
private:
	list<T> l;
	// 4
	mutex m;
	condition_variable cv;
public:
	bool finishedPush = false;
	synchronized_list() {};
	void push_back(T item)
	{
		lock_guard<mutex> lk(m);
		l.push_back(item);
		cv.notify_one();
	};
	T pop_back()
	{
		if (!l.empty())
		{
			T item = l.back();
			l.pop_back();
			return item;
		}
		return NULL;
	};
	void push_front(T item)
	{
		lock_guard<mutex> lk(m);
		l.push_front(item);
		cv.notify_one();
	};
	T pop_front()
	{
		if (!l.empty())
		{
			T item = l.front();
			l.pop_front();
			return item;
		}
		return NULL;
	};

	void popAll()	// pop from back
	{
		// 5
		while (!l.empty() || !finishedPush)
		{
			unique_lock<mutex> lk(m);
			cv.wait(lk, [this] { return !l.empty(); });		// wait until list is empty
			// wake up
			T item = pop_back();
			cout << item << endl;
			lk.unlock();
		}
	}
};

int main(int argc, char** argv)
{
	synchronized_list<int> l;
	// 3
	thread t1([&l]
		{
			// 4
			for (int i = 1; i < 5; i++)
			{
				l.push_front(i);
			}
			l.finishedPush = true;
			l.popAll();
		});
	thread t2([&l]
		{
			for (int i = 5; i < 10; i++)
			{
				l.push_front(i);
			}
			l.finishedPush = true;
			l.popAll();
		});

	t1.join();
	t2.join();

	return 0;
};

5.2
1
2
3
4
5
6
7
8
9

5.3
To make sure cout outputs are in the same order it processed, I put cout inside the lk lock.
In this time, all ints pushed by one thread will output as a whole and in the right order.

6. 
SOME NOTES:
- We add a boolean variable "unlimit" to indicate whether there is any water mark or not (when low=high=0)
- Since we only test this part on push_back and pop_front, we only modify these 2 functions. The other 2 functions (push_front & pop_front) can be modified in similarv way.
- Before doing p6, we put all methods in cpp file. Starting from p6 of this studio, we put declaration code into header file and execution code in source file for better coding style.

template <typename T> 
class synchronized_list
{
private:
	list<T> l;
	// 4
	mutex m;
	condition_variable cv;
	typename list<T>::size_type low; 
	typename list<T>::size_type high;
	bool unlimit = false; // true if low = high = 0

public:
	bool finishedPush = false;
	synchronized_list();
	synchronized_list(size_t ilow, size_t ihigh);
	void push_back(T item);
	T pop_back();
	void push_front(T item);
	T pop_front();
	void popAll();
};

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
	// 6
	unique_lock<mutex> lk(m);
	cv.wait(lk, [this] { return (l.size() < high) || unlimit; });		// wait until list size <= high
	// wake up
	l.push_back(item);
	lk.unlock();
	// use nofity_all as default
	cv.notify_all(); // notify all threads, all threads check above while loop condition, and then one of them get lock
};

template <class T>
T synchronized_list<T>::pop_front()
{
	unique_lock<mutex> lk(m);
	cv.wait(lk, [this] { return l.size() > low || (unlimit && !l.empty()); });		// wait until list size >= low
	// wake up
	T item = l.front();
	l.pop_front();
	cout << "pop:" << item << endl;
	lk.unlock();
	cv.notify_all();
	return item;
};


int main(int argc, char** argv)
{
	synchronized_list<int> l(0,1);
	thread t1([&l]
		{
			l.push_back(5);
		});
	thread t2([&l]
		{
			*/
			l.push_back(8);
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

	return 0;
};