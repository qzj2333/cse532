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