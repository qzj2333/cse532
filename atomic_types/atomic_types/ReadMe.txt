1. Lin Li, Nicole Wang

2.	All atomic types are supported from table 5.1 and 5.2
atomic_bool;
atomic_char;
atomic_schar;
atomic_uchar;
atomic_int;
atomic_uint;
atomic_short;
atomic_ushort;
atomic_long;
atomic_ulong;
atomic_llong;
atomic_ullong;
atomic_char16_t;
atomic_char32_t;
atomic_wchar_t;
atomic_int_least8_t;
atomic_uint_least8_t;
atomic_int_least16_t;
atomic_uint_least16_t;
atomic_int_least32_t;
atomic_uint_least32_t;
atomic_int_least64_t;
atomic_uint_least64_t;
atomic_int_fast8_t;
atomic_uint_fast8_t;
atomic_int_fast16_t;
atomic_uint_fast16_t;
atomic_int_fast32_t;
atomic_uint_fast32_t;
atomic_int_fast64_t;
atomic_uint_fast64_t;
atomic_intptr_t;
atomic_uintptr_t;
atomic_size_t ast;
atomic_ptrdiff_t;
atomic_intmax_t;
atomic_uintmax_t;

3.	All instances listed on table 5.1 and tbale 5.2 are lock free with is_lock_free() method. 
atomic_bool;
atomic_char;
atomic_schar;
atomic_uchar;
atomic_int;
atomic_uint;
atomic_short;
atomic_ushort;
atomic_long;
atomic_ulong;
atomic_llong;
atomic_ullong;
atomic_char16_t;
atomic_char32_t;
atomic_wchar_t;
atomic_int_least8_t;
atomic_uint_least8_t;
atomic_int_least16_t;
atomic_uint_least16_t;
atomic_int_least32_t;
atomic_uint_least32_t;
atomic_int_least64_t;
atomic_uint_least64_t;
atomic_int_fast8_t;
atomic_uint_fast8_t;
atomic_int_fast16_t;
atomic_uint_fast16_t;
atomic_int_fast32_t;
atomic_uint_fast32_t;
atomic_int_fast64_t;
atomic_uint_fast64_t;
atomic_intptr_t;
atomic_uintptr_t;
atomic_size_t ast;
atomic_ptrdiff_t;
atomic_intmax_t;
atomic_uintmax_t;

4. 
(1) Command: atomic_types.exe
(2) Output: 1

(1) Command: atomic_types.exe 4
(2) Output: 
1
2
3
4

(1) Command: atomic_types.exe 8
(2) Output:
1
2
3
4
5
6
7
8

5. 
(1) The behavior does not change from the output given, but the process does change.
(2) Both memory_order_relaxed and memory_order_seq_cst give the same output, but memory_order_relaxed is not controlled by the ordering of thread,
	whereas memory_order_seq_cst will strictly follow the ordering of the thread.

6.
(1) Memory_order_relaxed is more similar to acquire release ordering. 
(2) Acquire-release ordering is similar to memory_order_relaxed because it also achieves additional synchronization without the overhead of full-blown
sequential consistency. Although it introduced some synchronization but no total order of operations. 

