#pragma once
#include <thread>
#include <future>
using namespace std;
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
thread_local interrupt_flag this_thread_interrupt_flag;
class interruptible_thread
{
	std::thread internal_thread;
	interrupt_flag* flag;
public:
	template<typename FunctionType>
	interruptible_thread(FunctionType f)
	{
		std::promise<interrupt_flag*> p;
		internal_thread = std::thread([f, &p] {
			p.set_value(&this_thread_interrupt_flag);
			//cout << "f result: "<< f << endl;
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
		cout << "enter interruption_point:" << this_thread_interrupt_flag.print() << endl;
		if (this_thread_interrupt_flag.is_set())
		{
			cout << "!!!!" << endl;
			//throw "thread_interrupted";
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