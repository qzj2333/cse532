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