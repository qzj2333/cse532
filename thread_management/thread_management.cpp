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