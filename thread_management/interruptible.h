#pragma once
#include <thread>
#include <future>
using namespace std;


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
		if(f)
			return "1";
		return "0";
	};
};

thread_local interrupt_flag this_thread_interrupt_flag;
class interruptible_thread
{
	std::thread internal_thread;
	interrupt_flag * flag;
public:
	template<typename FunctionType>
	interruptible_thread(FunctionType && f)
	{
		std::promise<interrupt_flag*> p;
		internal_thread = std::thread([&f, &p] {
			p.set_value(&this_thread_interrupt_flag);
			//cout << "f result: "<< f << endl;
			/*
			try {
				std::forward<FunctionType>(f);
			}
			catch (thread_interrupted const&) {
			}
			*/
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
			cout << "!!!!" << endl;
			throw "thread_interrupted";
		}
		/*
		if (flag != 0 && flag ->is_set()) {
			//throw thread_interrupted();
			cout << "aaaaa" << endl;
		}
		else if (flag == 0) {
			cout << "eeeee" << endl;
		}
		*/
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