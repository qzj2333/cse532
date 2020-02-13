#include <iostream>
#include <atomic>
#include <thread>
using namespace std;


enum arg
{
	singleArgument = 1,
	success = 0
};

std::atomic<bool> x, y;
std::atomic<int> z;


void write_x_then_y()
{
	/*4
	x.store(true, std::memory_order_relaxed);
	y.store(true, std::memory_order_relaxed);
	*/

	/*5.
	x.store(true, std::memory_order_seq_cst);
	y.store(true, std::memory_order_seq_cst);
	*/


	x.store(true, std::memory_order_release);
	y.store(true, std::memory_order_release);
}
void read_y_then_x()
{
	/*4
	while (!y.load(std::memory_order_relaxed));
	if (x.load(std::memory_order_relaxed))
		++z;
	*/

	/*5
	while (!y.load(std::memory_order_seq_cst));
	if (x.load(std::memory_order_seq_cst))
		++z;
	*/


	while (!y.load(std::memory_order_acquire));
	if (x.load(std::memory_order_acquire))
		++z;
}


int main(int argc, char** argv) 
{
	std::atomic<bool> sab = true;
	std::atomic<char> sac = 'a';
	std::atomic<signed char> sasc = 'a';
	std::atomic<unsigned char> sauc = 'b';
	std::atomic<int> sai = 2;
	std::atomic<unsigned> saui = 3;
	std::atomic<short> sas = 2;
	std::atomic<unsigned short> saus = 2;
	std::atomic<long> sal = 2;
	std::atomic<unsigned long> saul = 4;
	std::atomic<long long> sall = 3;
	std::atomic<unsigned long long> saull = 2;
	std::atomic<char16_t> sac16t = 'a';
	std::atomic<char32_t> sac32t = 'v';
	std::atomic<wchar_t> sawct = 'v';
	std::atomic<int_least8_t> sail8t = 1;
	std::atomic<uint_least8_t> sauil8t = 1;
	std::atomic<int_least16_t> sail16t = 1;
	std::atomic<uint_least16_t> sauil16t = 1;
	std::atomic<int_least32_t> sail32t = 1;
	std::atomic<uint_least32_t> sauil32t = 1;
	std::atomic<int_least64_t> sail64t = 1;
	std::atomic<uint_least64_t> sauil64t = 1;
	std::atomic<int_fast8_t> saif8t = 1;
	std::atomic<uint_fast8_t> sauif8t = 1;
	std::atomic<int_fast16_t> saif16t = 1;
	std::atomic<uint_fast16_t> sauif16t = 1;
	std::atomic<int_fast32_t> saif32t = 1;
	std::atomic<uint_fast32_t> sauif32t = 1;
	std::atomic<int_fast64_t> saif64t = 1;
	std::atomic<uint_fast64_t> sauif64t = 1;
	std::atomic<intptr_t> saipt = NULL;
	std::atomic<uintptr_t> sauipt = NULL;
	std::atomic<size_t> sast = 1;
	std::atomic<ptrdiff_t> sapdt = NULL;
	std::atomic<intmax_t> saimt = 1;
	std::atomic<uintmax_t> sauimt = 1;


	cout << "atomic_bool is lock free: " << sab.is_lock_free() << endl;
	cout << "atomic_char is lock free: " << sac.is_lock_free() << endl;
	cout << "atomic_schar is lock free: " << sasc.is_lock_free() << endl;
	cout << "atomic_uchar is lock free: " << sauc.is_lock_free() << endl;
	cout << "atomic_int is lock free: " << sai.is_lock_free() << endl;
	cout << "atomic_uint is lock free: " << saui.is_lock_free() << endl;
	cout << "atomic_short is lock free: " << sas.is_lock_free() << endl;
	cout << "atomic_ushort is lock free: " << saus.is_lock_free() << endl;
	cout << "atomic_long is lock free: " << sal.is_lock_free() << endl;
	cout << "atomic_ulong is lock free: " << saul.is_lock_free() << endl;
	cout << "atomic_llong is lock free: " << sall.is_lock_free() << endl;
	cout << "atomic_ullong is lock free: " << saull.is_lock_free() << endl;
	cout << "atomic_char16_t is lock free: " << sac16t.is_lock_free() << endl;
	cout << "atomic_char32_t is lock free: " << sac32t.is_lock_free() << endl;
	cout << "atomic_wchar_t is lock free: " << sawct.is_lock_free() << endl;
	cout << "atomic_int_least8_t is lock free: " << sail8t.is_lock_free() << endl;
	cout << "atomic_uint_least8_t is lock free: " << sauil8t.is_lock_free() << endl;
	cout << "atomic_int_least16_t is lock free: " << sail16t.is_lock_free() << endl;
	cout << "atomic_uint_least16_t is lock free: " << sauil16t.is_lock_free() << endl;
	cout << "atomic_int_least32_t is lock free: " << sail32t.is_lock_free() << endl;
	cout << "atomic_uint_least32_t is lock free: " << sauil32t.is_lock_free() << endl;
	cout << "atomic_int_least64_t is lock free: " << sail64t.is_lock_free() << endl;
	cout << "atomic_uint_least64_t is lock free: " << sauil64t.is_lock_free() << endl;
	cout << "atomic_int_fast8_t is lock free: " << saif8t.is_lock_free() << endl;
	cout << "atomic_uint_fast8_t is lock free: " << sauif8t.is_lock_free() << endl;
	cout << "atomic_int_fast16_t is lock free: " << saif16t.is_lock_free() << endl;
	cout << "atomic_uint_fast16_t is lock free: " << sauif16t.is_lock_free() << endl;
	cout << "atomic_int_fast32_t is lock free: " << saif32t.is_lock_free() << endl;
	cout << "atomic_uint_fast32_t is lock free: " << sauif32t.is_lock_free() << endl;
	cout << "atomic_int_fast64_t is lock free: " << saif64t.is_lock_free() << endl;
	cout << "atomic_uint_fast64_t is lock free: " << sauif64t.is_lock_free() << endl;
	cout << "atomic_intptr_t is lock free: " << saipt.is_lock_free() << endl;
	cout << "atomic_uintptr_t is lock free: " << sauipt.is_lock_free() << endl;
	cout << "atomic_size_t is lock free: " << sast.is_lock_free() << endl;
	cout << "atomic_ptrdiff_t is lock free: " << sapdt.is_lock_free() << endl;
	cout << "atomic_intmax_t is lock free: " << saimt.is_lock_free() << endl;
	cout << "atomic_uintmax_t is lock free: " << sauimt.is_lock_free() << endl;

	z = 0;
	if (argc == singleArgument)
	{
		x = false;
		y = false;
		std::thread a(write_x_then_y);
		std::thread b(read_y_then_x);
		a.join();
		b.join();
		cout << z << endl;
		
		return 0;
	}
	else if (argc > singleArgument)
	{
		for (int i = 0; i < atoi(argv[1]); ++i) {
			x = false;
			y = false;
			std::thread a(write_x_then_y);
			std::thread b(read_y_then_x);
			a.join();
			b.join();
			cout << z << endl;
			
		}
		return success;
	}
	
}