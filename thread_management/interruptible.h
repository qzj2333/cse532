#pragma once
#include <thread>
#include <future>
class interrupt_flag
{
private:
	bool flag = false;
public:
	void set()
	{
		flag = true;
	};
	bool is_set() const
	{
		return flag;
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
			f(); });
		flag = p.get_future().get();
	}
	void interrupt()
	{
		if (flag) {
			flag->set();
		}
	}
};