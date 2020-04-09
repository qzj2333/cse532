#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <queue>
#include <sstream>
#include <fstream>
using namespace std;

bool isPrime(int n)
{
	for (int i = 2; i < n; i++)
	{
		if (n % i == 0)
		{
			return false;
		}
	}
	return true;
}

class Wrapper
{
	ostream& o;
	mutex m;
public:
	Wrapper(ostream& os) :o(os) {};
	void print(const string& s)
	{
		lock_guard<mutex> lk(m);
		o << s << endl;
	}
};

class ThreadPool
{
	Wrapper* wrapper;
	vector<thread> pool;
	queue<int> working_queue;
	mutex m;
	condition_variable cv;
public:
	bool isEnd;
	ThreadPool(int n)	// n: number of threads in the pool
	{
		wrapper = new Wrapper(cout);
		isEnd = false;
		for (int i = 0; i < n; i++)
		{
			thread t ([this]
			{
				prepare();
			});
			pool.emplace_back(move(t));
		}
	}

	~ThreadPool()
	{
		cv.notify_all();
		for (thread& t : pool)
		{
			if (t.joinable())
			{
				t.join();
			}
		}
	}

	void prepare()
	{
		while (!working_queue.empty())
		{
			unique_lock<mutex> lk(m);
			cv.wait(lk, [this]
			{
				return !this->working_queue.empty() || isEnd;
			});
			if (this->working_queue.empty() && isEnd)
			{
				lk.unlock();
				cv.notify_all();
				return;
			}
			stringstream ss;
			ss << this_thread::get_id();
			wrapper->print(ss.str() + " becomes leader");
			int n = working_queue.front();
			working_queue.pop();

			if (n % 2 == 0)
			{
				wrapper->print(to_string(n)+" is even number! not prime.");
			}
			else if (n == 1)
			{
				wrapper->print("1 is prime.");
			}
			else
			{
				lk.unlock();
				cv.notify_all();
				if (isPrime(n))
				{
					wrapper->print( to_string(n)+" is prime.");
				}
				else // !isPrime(n)
				{
					wrapper->print(to_string(n) + " is not prime.");
				}
			}
		}
	}

	void inputNum(int n)
	{
		unique_lock<mutex> lk(m);
		working_queue.push(n);
		//cout << working_queue.size() << endl;
		lk.unlock();
		cv.notify_all();
	}
};

int main(int argc, char* argv [] )
{
	// 2
	/*cout << "7 is prime: " << isPrime(7) << endl;
	cout << "20 is not prime: " << isPrime(20) << endl;
	cout << "71 is prime: " << isPrime(71) << endl;
	cout << "100 is not prime: " << isPrime(100) << endl;*/

	// 3 4
	/*shared_ptr<ThreadPool> tp(new ThreadPool(10));
	tp->inputNum(0);
	tp->inputNum(1);
	tp->inputNum(7);
	tp->inputNum(20);
	tp->inputNum(71);
	tp->inputNum(91);
	tp->inputNum(100);

	tp->isEnd = true;*/

	// 5
	shared_ptr<ThreadPool> tp(new ThreadPool(10));

	// for part 2
	ifstream in("test.txt");
	std::streambuf* cinbuf = std::cin.rdbuf(); //save old buf
	std::cin.rdbuf(in.rdbuf()); //redirect std::cin to txt file
	
	int n;
	while (cin >> n)
	{
		tp->inputNum(n);
	}
	
	tp->isEnd = true;
}