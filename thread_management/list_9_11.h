#pragma once
#include <thread>
#include <future>
#include <atomic>
using namespace std;

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
	struct clear_cv_on_destruct
	{
		~clear_cv_on_destruct()
		{
			this_thread_interrupt_flag.clear_condition_variable();
		}
	};
};

template<typename Predicate, typename T>
void interruptible_wait(std::condition_variable& cv,
	std::unique_lock<std::mutex>& lk,
	Predicate pred, std::future<T>& uf) 
{
	interruption_point();
	this_thread_interrupt_flag.set_condition_variable(cv);
	interrupt_flag::clear_cv_on_destruct guard;
	while (!this_thread_interrupt_flag.is_set() && !pred() && uf.wait_for(lk, std::chrono::milliseconds(1) !=
		std::future_status::ready)
	{
		cv.wait_for(lk, std::chrono::milliseconds(1));
	}
	interruption_point();
};

template<typename T>
void interruptible_wait(std::future<T>& uf)
{
	while (!this_thread_interrupt_flag.is_set())
	{
		if (uf.wait_for(lk, std::chrono::milliseconds(1) ==
			std::future_status::ready)
			break;
	}
	interruption_point();
}