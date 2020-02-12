#include "active_object.h"

template <typename T>
active_object<T>::active_object()
{

};

template <typename T>
void active_object<T>::enqueue(T item)
{
	l.push_front(item);
}

template <typename T>
void active_object<T>::run_service()
{
	cout << l.pop_back() << endl;
}