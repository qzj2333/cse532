1. Nicole Wang, Lin Li

2. 
CODE:
class thread_interrupted : public std::exception {
public:
 virtual char const* what() const noexcept { return "thread_interrupted."; }
};

class interrupt_flag
{
private:
 bool f = false;
public:
 void set()
 {
  f = true;
 };
 bool is_set() const
 {
  return f;
 }
 string print()
 {
  if (f)
   return "1";
  return "0";
 };
};

thread_local interrupt_flag this_thread_interrupt_flag;
class interruptible_thread
{
 std::thread internal_thread;
 interrupt_flag* flag;
public:
 template<typename FunctionType>
 interruptible_thread(FunctionType&& f)
 {
  std::promise<interrupt_flag*> p;
  internal_thread = std::thread([&f, &p] {
   p.set_value(&this_thread_interrupt_flag);
   });
  flag = p.get_future().get();
 }
 void interrupt()
 {
  if (flag) {
   flag->set();
  }
 }

 void interruption_point()
 {

  if (flag->is_set())
  {
   throw "thread_interrupted";
  }
 }

 void join()
 {
  flag = NULL;
  internal_thread.join();
 }

 void detach()
 {
  flag = NULL;
  internal_thread.detach();
 }

 bool joinable() const
 {
  return internal_thread.joinable();
 }
};

int fib(unsigned int n)
{
 if (n <= 1)
 {
  return n;
 }
 else
 {
  int result = fib(n - 1) + fib(n - 2);
  cout << "result: " << result << endl;
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
  cout << "start " << endl;
  t2.interrupt();
  t2.interruption_point();
  t1.interruption_point();
  t2.detach();
  t1.detach();

  cout << "stop interrupt" << endl;

 }
 catch (...)
 {
  cout << "catch: " << endl;
  t2.detach();
  t1.detach();
 }
 cout << "done" << endl;
 return 0;
}

EXPLAINATION:
We write a function fib() to be the functionType that passed into the interrupitable_thread. We add interrupt() and interruption_point() as described in textbook into the interrupitible_thread class.
The constructor of the class will connect its data member to the global variable this_thread_interrupt_flag, and it'll set the flag when interrupt() is called.
If any thread is been interrupted, the interrpution_point method will throw exception when it been called.

3. Code: 
#pragma once
#include <thread>
#include <future>
#include <atomic>
using namespace std;


class thread_interrupted : public std::exception {
public:
	virtual char const* what() const noexcept { return "thread_interrupted."; }
};

thread_local atomic_bool this_thread_interrupt_flag;
class interruptible_thread
{
	std::thread internal_thread;
	atomic_bool* flag;
public:
	template<typename FunctionType>
	interruptible_thread(FunctionType && f, int n)
	{
		std::promise<atomic_bool*> p;
		internal_thread = std::thread([&f, n, &p] {
			p.set_value(&this_thread_interrupt_flag);
			f(n);
			});
		flag = p.get_future().get();
	}
	void interrupt()
	{
		*flag = true;
	}

	void interruption_point()
	{
		
		if (*flag)
		{
			throw thread_interrupted();
		}
	}

	void join()
	{
		flag = NULL;
		internal_thread.join();
	}

	void detach()
	{
		flag = NULL;
		internal_thread.detach();
	}

	bool joinable() const
	{
		return internal_thread.joinable();
	}
};

#include <iostream>
#include <thread>
#include <exception>
#include <future>
#include <functional>
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
		return result;
	}
}

int main(int argc, char** argv)
{
	int n = 2;
	vector< packaged_task<int(int)>> tasks;
	vector<future<int>> results;
	vector<future_status> status;
	for (int i = 0; i < n; ++i)
	{
		tasks.push_back(packaged_task<int(int)> (fib));
	}

	for (int i = 0; i < n; ++i)
	{
		results.push_back(tasks[i].get_future());
		future_status state;
		status.push_back(state);
	}

	// construct thread 1 by 1
	vector<interruptible_thread> ts;
	ts.emplace_back(interruptible_thread (move(tasks[0]), 10));
	ts.emplace_back(interruptible_thread (move(tasks[1]), 8));

	bool firstFinish = false;
	int first = -1;
	do {
		for (int i = 0; i < n; i++)
		{
			status[i] = results[i].wait_for(chrono::seconds(1));
			if (status[i] == std::future_status::ready)
			{
				firstFinish = true;
				first = i;
			}
			if (status[i] == std::future_status::deferred) {
				std::cout << "deferred\n";
			}
			else if (status[i] == std::future_status::timeout) {
				std::cout << "timeout\n";
			}
			else if (status[i] == std::future_status::ready) {
				std::cout << "ready!\n";
			}
		}
	} while (! firstFinish);
	
	ts[first].interrupt();
	for (int i = 0; i < n; i++) {
		if (i != first) {
			try
			{
				ts[i].interruption_point();
				if (ts[i].joinable()) {
					ts[i].join();
				}
			}
			catch (thread_interrupted & e) 
			{
				cout <<e.what() << endl;
				if (ts[i].joinable()) {
					ts[i].join();
				}
				
			}
		}
		else
		{
			ts[i].join();
		}
	}
	
	int res = results[first].get();
	cout << "result: " << res << endl;
	return 0;
}


Output: 
ready!
ready!
thread_interrupted.
result: 21

5. Code:
#pragma once
#include <thread>
#include <future>
#include <atomic>
using namespace std;


class thread_interrupted : public std::exception {
public:
	virtual char const* what() const noexcept { return "thread_interrupted."; }
};


class interrupt_flag
{
	std::atomic<bool> flag;
	std::condition_variable* thread_cond;
	std::mutex set_clear_mutex;
public:
	interrupt_flag() :
		thread_cond(0)
	{}
	void set()
	{
		flag.store(true, std::memory_order_relaxed);
		std::lock_guard<std::mutex> lk(set_clear_mutex);
		if (thread_cond)
		{
			thread_cond->notify_all();
		}
	}
	bool is_set() const
	{
		return flag.load(std::memory_order_relaxed);
	}
	void set_condition_variable(std::condition_variable& cv)
	{
		std::lock_guard<std::mutex> lk(set_clear_mutex);
		thread_cond = &cv;
	}
	void clear_condition_variable()
	{
		std::lock_guard<std::mutex> lk(set_clear_mutex);
		thread_cond = 0;
	}
};

thread_local interrupt_flag this_thread_interrupt_flag;

struct clear_cv_on_destruct
{
	~clear_cv_on_destruct()
	{
		this_thread_interrupt_flag.clear_condition_variable();
	}
};

void interruption_point()
{
	if (this_thread_interrupt_flag.is_set())
	{
		throw thread_interrupted();
	}
};
void interruptible_wait(std::condition_variable& cv,
	std::unique_lock<std::mutex>& lk)
{
	interruption_point();
	this_thread_interrupt_flag.set_condition_variable(cv);
	clear_cv_on_destruct guard;
	interruption_point();
	cv.wait_for(lk, std::chrono::milliseconds(1));
	interruption_point();
}

/*
template<typename Predicate, typename T>
void interruptible_wait(std::condition_variable& cv,
	std::unique_lock<std::mutex>& lk,
	Predicate pred, std::future<T>& uf)
{
	//interruption_point();
	this_thread_interrupt_flag.set_condition_variable(cv);
	interrupt_flag::clear_cv_on_destruct guard;
	while (!this_thread_interrupt_flag.is_set() && !pred() && uf.wait_for(lk, std::chrono::milliseconds(1)) !=
		std::future_status::ready)
	{
		cv.wait_for(lk, std::chrono::milliseconds(1));
	}
	//interruption_point();
};
*/


/*template<typename T>
void interruptible_wait(std::future<T>& uf)
{
	while (!this_thread_interrupt_flag.is_set())
	{
		if (uf.wait_for(lk, std::chrono::milliseconds(1)) == std::future_status::ready)
		{
			break;
		}
	}
	//interruption_point();
};*/

/*
class interrupt_flag
{
private:
	bool f = false;
public:
	void set()
	{
		f = true;
	};
	bool is_set() const
	{
		return f;
	}
	string print()
	{
		if(f)
			return "1";
		return "0";
	};
};
*/

class interruptible_thread
{
	std::thread internal_thread;

public:
	interrupt_flag* flag;
	template<typename FunctionType>
	interruptible_thread(FunctionType f, int n)
	{
		std::promise<interrupt_flag*> p;
		internal_thread = std::thread([f, n, &p] {
			p.set_value(&this_thread_interrupt_flag);
			f(n);
			});
		flag = p.get_future().get();
	}
	void interrupt()
	{
		if (flag) {
			flag->set();
		}
	}


	void join()
	{
		flag = NULL;
		internal_thread.join();
	}

	void detach()
	{
		flag = NULL;
		internal_thread.detach();
	}

	bool joinable() const
	{
		return internal_thread.joinable();
	}
};

#include <iostream>
#include <thread>
#include <exception>
#include <future>
#include <functional>
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
		return result;
	}
}

int main(int argc, char** argv)
{
	vector< packaged_task<int(int)>> tasks;
	vector<future<int>> results;
	vector<future_status> status;
	vector<interruptible_thread> ts;
	std::mutex m;
	condition_variable cv;
	bool isInterrupt = false;
	atomic_bool launchNew = false;
	int size = 0;
	string response = "";
	string end;
	atomic_uint input = 0;

	while (response.compare("stop")!= 0)
	{
		while (response.compare("stop") != 0 && input <= 0)
		{
			cout << "Please enter <stop> or a positive integer: ";
			cin >> response;
			input = atoi(response.c_str());
		}
		if (input > 0)
		{
			launchNew = true;
		}
		else
		{
			if (size > 0)
			{
				ts[0].interrupt();
				isInterrupt = true;
			}
			else
			{
				break;
			}
		}
		std::unique_lock<std::mutex> lk(m);
		interruptible_wait(cv,lk);
		if (launchNew)
		{
			tasks.push_back(packaged_task<int(int)>(fib));
			results.push_back(tasks.back().get_future());
			future_status state;
			status.push_back(state);
			ts.emplace_back(interruptible_thread(ref(tasks.back()), input));
			launchNew = false;
			size++;
			cout << "size: " << size << endl;
		}
		try
		{
			interruption_point();
		}
		catch (thread_interrupted& e)
		{
			isInterrupt = true;
		}

		if (isInterrupt)
		{
			cout << "thread_interrupted" << endl;
			for (int i = 0; i < ts.size(); i++)
			{
				if (ts[i].joinable()) {
					ts[i].join();
				}
			}
			break;
		}

		for (int i = 0; i < size; i++)
		{
			status[i] = results[i].wait_for(chrono::seconds(1));
			if (status[i] == std::future_status::ready)
			{
				cout << results[i].get() << endl;
				tasks.erase(tasks.begin() + i);
				results.erase(results.begin() + i);
				status.erase(status.begin() + i);
				ts[i].join();
				ts.erase(ts.begin() + i);
				size--;

			}
		}
		response = "";
		input = 0;
	}



	
	return 0;
}

Output:
H:\cse532-master\thread_management\x64\Debug>thread_management.exe
Please enter <stop> or a positive integer: sklf
Please enter <stop> or a positive integer: 2
size: 1
1
Please enter <stop> or a positive integer: 10
size: 1
55
Please enter <stop> or a positive integer: 38
size: 1
Please enter <stop> or a positive integer: 40
size: 2
Please enter <stop> or a positive integer: stop
thread_interrupted
